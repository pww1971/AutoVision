#include "stdafx.h"
#include "moveCard.h"

#ifdef _MC_BP 
CMCBPMoveCard::CMCBPMoveCard()
{
	m_mtNum = 16;
	m_ioNum = 16;
	for (int i=0;i<m_mtNum;i++)
	{
		m_MvScope.push_back(cv::Point2f(0,200));
		m_fSpeed.push_back(1000);
		m_fDebugSpeed.push_back(1000);
		//		m_fUnitMM[i] = 1;
	}
	
	m_linkAdd = "192.168.0.200";
	int iRes = 0;

	//一个板卡就声明1个对象，多个板卡就声明多个对象，各个对象之间互不干涉
	//5个参数依次为板卡号、PC端IP地址、PC端端口号，板卡端IP地址，板卡端口号。
	//通常建议不同板卡IP地址要不同，端口号也要不同
	//PC端端口号和板卡端口号要保持一致，建议从60000开始，第二个板卡就60001，依次类推......
	//iRes = g_MultiCard.MC_Open(1,"10.129.41.200",60001,"10.129.41.112",60001);
	iRes = m_handle.MC_Open(1, (char*)m_linkAdd.c_str(), 60001, "192.168.0.1", 60001);
	//iRes += g_MultiCard2.MC_Open(2,"192.168.0.200",60002,"192.168.0.2",60002);

	if (iRes)
	{
		//MessageBox("Open Card Fail,Please turn off wifi ,check PC IP address or connection!");
	}
	else
	{
		//MessageBox("Open Card Successful!");

		m_handle.MC_SetCommuTimer(3);
	}
}

CMCBPMoveCard::~CMCBPMoveCard()
{
	release();
}

void CMCBPMoveCard::release()
{
	if (!m_bIni)
	{
		return;
	}
	m_Lock.lock();
	m_bIni = false;
	m_handle.MC_Close();
	m_Lock.unLock();

}
//void CDemoVCDlg::OnCbnSelchangeCombo1(int iAxisNum)
//{
//	int iRes = 0;
//	short nEncOnOff = 0;
//	short nPosLimOnOff = 0;
//	short nNegLimOfOff = 0;
//
//	iRes = g_MultiCard.MC_GetEncOnOff(iAxisNum, &nEncOnOff);
//
//	if (nEncOnOff)
//	{
//		iRes = g_MultiCard.MC_EncOn(iAxisNum);//使用编码器
//	}
//	else
//	{
//		iRes = g_MultiCard.MC_EncOff(iAxisNum);//脉冲计数器模式
//	}
//
//	iRes = g_MultiCard.MC_GetLmtsOnOff(iAxisNum, &nPosLimOnOff, &nNegLimOfOff);
//
//	if (nPosLimOnOff)
//	{
//		((CButton*)GetDlgItem(IDC_CHECK_POS_LIM_ENABLE))->SetCheck(true);
//	}
//	else
//	{
//		((CButton*)GetDlgItem(IDC_CHECK_POS_LIM_ENABLE))->SetCheck(false);
//	}
//
//	if (nNegLimOfOff)
//	{
//		((CButton*)GetDlgItem(IDC_CHECK_NEG_LIM_ENABLE))->SetCheck(true);
//	}
//	else
//	{
//		((CButton*)GetDlgItem(IDC_CHECK_NEG_LIM_ENABLE))->SetCheck(false);
//	}
//}

bool CMCBPMoveCard::ini(string linkAdd)
{
	m_Lock.lock();
	m_bIni = false;
	char* szContent[] = {"轴号","负向限位","正向限位", "轴脉冲模式",\
		"加速度","减速度","平滑时间","起始速度","速度",\
		"回零方式","回零方向","回零快移速度","回零定位速度","回零寻找INDEX速度","回零加速度","回零偏移",\
		"轴负向限位","轴正向限位","导承","手动速度"};
	int commandLen = sizeof(szContent)/sizeof(char*);
	m_vctCfgName.clear();
	for (int i=0;i<commandLen;i++)
	{
		m_vctCfgName.push_back(szContent[i]);
	}
	float defaultData[] = {-100,200,0,	\
		0.1,0.1,0,0,10000,\
		1,0,5,1,1,0.2,0,\
		1000,1000,1000,5000};
	commandLen = sizeof(defaultData)/sizeof(float);
	m_vctDefaultData.clear();
	for (int i=0;i<commandLen;i++)
	{
		m_vctDefaultData.push_back(defaultData[i]);
	}
	//软触发中断器
	int ioSoftNum = g_ioN - m_ioNum;
	if (ioSoftNum>0)
	{
		if (m_ioSoft != NULL)
		{
			delete[] m_ioSoft;
		}
		m_ioSoft = new int[ioSoftNum];
		memset(m_ioSoft,0,sizeof(int)*ioSoftNum);
	}

	int iRes =0;
	if (isIPAddressValid(linkAdd.c_str()))
		iRes = m_handle.MC_Open(1, (char*)m_linkAdd.c_str(), 60001, "192.168.0.1", 60001);

	if (iRes)
	{
		//	MessageBox(_T("链接失败"));
		m_Lock.unLock();
		return false;
	}
	m_handle.MC_Stop(0XFFFFFFFF, 0XFFFFFFFF);
	iRes+= m_handle.MC_EStopSetIO(0, 0, 0, 10);//设置主卡通用输入 X0 为紧急停止 IO,滤波时间 10ms
	iRes+= m_handle.MC_EStopOnOff(1);//紧急停止功能打开

	// 脉冲模式设置
	for (int i=0;i<m_mtNum;i++)
	{
		iRes+= m_handle.MC_EncOff(i+1);//脉冲计数器模式
		iRes+= m_handle.MC_AxisOn(i+1);

		iRes = m_handle.MC_PrfTrap(i+1);//设置指定轴为点位模式
	}
	m_linkAdd = linkAdd;
	m_bIni = true;

	m_Lock.unLock();
	return m_bIni;
}

void CMCBPMoveCard::Rapidstop()
{
	m_Lock.lock();
	
	m_handle.MC_Stop(0XFFFFFFFF, 0XFFFFFFFF);
	for (int i = 0; i < m_mtNum; i++)
		m_handle.MC_PrfTrap(i + 1);//设置指定轴为点位模式
	m_Lock.unLock();
}

bool CMCBPMoveCard::isIPAddressValid(const char* pszIPAddr)  
{  
	if (!pszIPAddr) return false; //若pszIPAddr为空  
	char IP1[100],cIP[4];  
	int len = strlen(pszIPAddr);  
	int i = 0,j=len-1;  
	int k, m = 0,n=0,num=0;  
	//去除首尾空格(取出从i-1到j+1之间的字符):  
	while (pszIPAddr[i++] == ' ');  
	while (pszIPAddr[j--] == ' ');  

	for (k = i-1; k <= j+1; k++)  
	{  
		IP1[m++] = *(pszIPAddr + k);  
	}       
	IP1[m] = '\0';  

	char *p = IP1;  

	while (*p!= '\0')  
	{  
		if (*p == ' ' || *p<'0' || *p>'9') return false;  
		cIP[n++] = *p; //保存每个子段的第一个字符，用于之后判断该子段是否为0开头  

		int sum = 0;  //sum为每一子段的数值，应在0到255之间  
		while (*p != '.'&&*p != '\0')  
		{  
			if (*p == ' ' || *p<'0' || *p>'9') return false;  
			sum = sum * 10 + *p-48;  //每一子段字符串转化为整数  
			p++;  
		}  
		if (*p == '.') {  
			if ((*(p - 1) >= '0'&&*(p - 1) <= '9') && (*(p + 1) >= '0'&&*(p + 1) <= '9'))//判断"."前后是否有数字，若无，则为无效IP，如“1.1.127.”  
				num++;  //记录“.”出现的次数，不能大于3  
			else  
				return false;  
		};  
		if ((sum > 255) || (sum > 0 && cIP[0] =='0')||num>3) return false;//若子段的值>255或为0开头的非0子段或“.”的数目>3，则为无效IP  

		if (*p != '\0') p++;  
		n = 0;  
	}  
	if (num != 3) return false;  
	return true;  
}

bool CMCBPMoveCard::moveAbs(int mtid,float pos)
{
	m_Lock.lock();
	if (!m_bIni || mtid == -1)
	{
		m_Lock.unLock();
		return false;
	}
	int iRes = m_handle.MC_SetPos(mtid+1, pos);

	iRes += m_handle.MC_Update(0X0001 << mtid);

	m_Lock.unLock();
	return 0 == iRes;

}

bool CMCBPMoveCard::move(int mtid,float dis)
{
	m_Lock.lock();
	if (!m_bIni || mtid == -1)
	{
		m_Lock.unLock();
		return false;
	}
	long pos = 0;
	int iRes = m_handle.MC_GetPos(mtid + 1, &pos);
	pos += dis;
	iRes += m_handle.MC_SetPos(mtid + 1, pos);
	iRes += m_handle.MC_Update(0X0001 << mtid);

	m_Lock.unLock();
	return 0 == iRes;

}

bool CMCBPMoveCard::moveV(int mtid,int di)
{
	m_Lock.lock();
	if (!m_bIni || mtid == -1)
	{
		m_Lock.unLock();
		return false;
	}
	int iRes = m_handle.MC_PrfJog(mtid+1);
	TJogPrm m_JogPrm;
	m_JogPrm.dAcc = 0.1;
	m_JogPrm.dDec = 0.1;
	m_JogPrm.dSmooth = 0;
	iRes += m_handle.MC_SetJogPrm(mtid+1, &m_JogPrm);
	iRes += m_handle.MC_SetVel(mtid+1, di);
	iRes += m_handle.MC_Update(0X0001 << mtid);

	m_Lock.unLock();
	return 0==iRes;
}

bool CMCBPMoveCard::moveStop(int mtid)
{
	m_Lock.lock();
	if (!m_bIni || mtid == -1)
	{
		m_Lock.unLock();
		return false;
	}
	bool ret = (0 == m_handle.MC_Stop((0X0001 << mtid), 0));
	m_handle.MC_PrfTrap(mtid + 1);//设置指定轴为点位模式
	m_Lock.unLock();
	return ret;
}

bool CMCBPMoveCard::mtZero(int mtid)
{
	m_Lock.lock();
	if (!m_bIni || mtid == -1)
	{
		m_Lock.unLock();
		return false;
	}
	//设定对应轴的原点输入口信号
	int iRes = 0;
//	short nStatus = 0;
	//启动轴 1 回零
	iRes = m_handle.MC_HomeStart(mtid+1);
	//获取轴 1 回零状态
//	iRes = m_handle.MC_HomeGetSts(mtid+1, &nStatus);


	m_handle.MC_PrfTrap(mtid + 1);//设置指定轴为点位模式
	m_Lock.unLock();
	return 0 == iRes;
}

bool CMCBPMoveCard::setIO(int ioid,int onoff)
{
	m_Lock.lock();
	if (!m_bIni)
	{
		m_Lock.unLock();
		return false;
	}
	//软触发中断器
	int ioSoftNum = g_ioN - m_ioNum;
	if (ioSoftNum>0 && ioid >= m_ioNum)
	{
		if (onoff<=0)
		{
			m_ioSoft[ioid-m_ioNum]--;//计数一次
		}
		else// if (onoff > 0 )//计数设置
		{
			m_ioSoft[ioid-m_ioNum]=onoff;
		}
		m_Lock.unLock();
		return true;
	}
	bool ret = (0 == m_handle.MC_SetExtDoBit(0,ioid,onoff));
	m_Lock.unLock();
	return ret;
}

bool CMCBPMoveCard::getIO( uint state[])
{
	m_Lock.lock();
	if (!m_bIni)
	{
		m_Lock.unLock();
		return false;
	}
	//软触发计数器
	int ioSoftNum = g_ioN - m_ioNum;
	for (int i=0;i<ioSoftNum;i++)
	{
		if (m_ioSoft[i] > 0)//次数小于等于0 就下降沿触发
			state[i+m_ioNum] = 1;
		else
			state[i+m_ioNum] = 0;
	}
	TAllSysStatusData m_AllSysStatusDataTemp;
	int iRes = m_handle.MC_GetAllSysStatus(&m_AllSysStatusDataTemp);
	for(int i = 0; i < m_ioNum; i++ )
		state[i] = (m_AllSysStatusDataTemp.lGpiRaw[0] & (0X0001 << i));
	m_Lock.unLock();
	return 0==iRes;
}

bool CMCBPMoveCard::getParam(vector<string>& vctPa)
{
	m_Lock.lock();
	if (!m_bIni)
	{
		m_Lock.unLock();
		return false;
	}
	
	char sz[32];
	vctPa.clear();
	int iRes = 0;
	for (int mtid=0;mtid<m_mtNum;mtid++)
	{
		string szPa="";
		sprintf(sz,"%f",m_MvScope[mtid].x);//运动范围
		szPa += sz;szPa+=MVFLAG;
		sprintf(sz,"%f",m_MvScope[mtid].y);//运动范围
		szPa += sz;szPa+=MVFLAG;
		short nEncOnOff = 0;
		iRes = m_handle.MC_GetEncOnOff(mtid+1, &nEncOnOff);
		sprintf(sz, "%d", nEncOnOff);//脉冲或伺服
		szPa += sz; szPa += MVFLAG;

		TTrapPrm TrapPrm;
		iRes += m_handle.MC_GetTrapPrm(mtid+1, &TrapPrm);
		TAxisHomePrm AxisHomePrm;
		iRes += m_handle.MC_HomeGetPrm(mtid+1, &AxisHomePrm);
		double vel = 0;
		iRes += m_handle.MC_GetVel(mtid + 1, &vel);//目标速度
		if (0 != iRes)
		{
			m_Lock.unLock();
			return false;
		}

		sprintf(sz,"%.3f", TrapPrm.acc);//加速度
		szPa += sz;szPa+=MVFLAG;
		sprintf(sz, "%.3f", TrapPrm.dec);//减速度
		szPa += sz; szPa += MVFLAG;
		sprintf(sz, "%d", TrapPrm.smoothTime);//平滑时间
		szPa += sz; szPa += MVFLAG;
		sprintf(sz, "%.3f", TrapPrm.velStart);//起始速度
		szPa += sz; szPa += MVFLAG;
		sprintf(sz, "%.3f", vel);//速度
		szPa += sz; szPa += MVFLAG;
		//回零
		sprintf(sz, "%d", AxisHomePrm.nHomeMode);//回零方式
		szPa += sz; szPa += MVFLAG;
		sprintf(sz, "%d", AxisHomePrm.nHomeDir);//回零方向
		szPa += sz; szPa += MVFLAG;
		sprintf(sz, "%.3f", AxisHomePrm.dHomeRapidVel);//回零快移速度
		szPa += sz; szPa += MVFLAG;
		sprintf(sz, "%.3f", AxisHomePrm.dHomeLocatVel);//回零定位速度
		szPa += sz; szPa += MVFLAG;
		sprintf(sz, "%.3f", AxisHomePrm.dHomeIndexVel);//回零寻找INDEX速度
		szPa += sz; szPa += MVFLAG;
		sprintf(sz, "%.3f", AxisHomePrm.dHomeAcc);//回零加速度
		szPa += sz; szPa += MVFLAG;
		sprintf(sz, "%d", AxisHomePrm.lOffset);//回零偏移
		szPa += sz; szPa += MVFLAG;

		sprintf(sz,"%f",m_fDebugSpeed[mtid]);//调试速度
		szPa += sz;szPa+=MVFLAG;
		vctPa.push_back(szPa);
	}

	m_Lock.unLock();
	return true;
}

bool CMCBPMoveCard::setParam(string szPa)
{
	m_Lock.lock();
	if (!m_bIni)
	{
		m_Lock.unLock();
		return false;
	}
	try
	{
		int iRes = 0;
		if (""==szPa)//加载默认参数
		{
			/*char* szContent[] = { "轴号","负向限位","正向限位", \
		"加速度","减速度","平滑时间","起始速度","速度",\
		"回零方式","回零方向","回零快移速度","回零定位速度","回零寻找INDEX速度","回零加速度","回零偏移",\
		"轴负向限位","轴正向限位","导承","手动速度" };*/
			for (int mtid=0;mtid<m_mtNum;mtid++)
			{
				m_MvScope[mtid] = cv::Point2f(m_vctDefaultData[0],m_vctDefaultData[1]);//运动范围
				if (m_vctDefaultData[2]>0)
				{
					iRes = m_handle.MC_EncOn(mtid+1);//使用编码器
				}
				else
				{
					iRes = m_handle.MC_EncOff(mtid+1);//脉冲计数器模式
				}
				TTrapPrm TrapPrm;
				TrapPrm.acc = m_vctDefaultData[3];//加速度
				TrapPrm.dec = m_vctDefaultData[4];//减速度
				TrapPrm.smoothTime = m_vctDefaultData[5];//平滑时间
				TrapPrm.velStart = m_vctDefaultData[6];//起始速度
				iRes = m_handle.MC_SetTrapPrm(mtid + 1, &TrapPrm);
				iRes += m_handle.MC_SetVel(mtid + 1, m_vctDefaultData[7]);//设置目标速度

				m_fDebugSpeed[mtid] = m_vctDefaultData[m_vctDefaultData.size()-1];//调试速度
				m_fSpeed[mtid] = m_vctDefaultData[7];//调试速度

				TAxisHomePrm AxisHomePrm;
				AxisHomePrm.nHomeMode = m_vctDefaultData[9];
				AxisHomePrm.nHomeDir = m_vctDefaultData[10];
				AxisHomePrm.dHomeRapidVel = m_vctDefaultData[11];
				AxisHomePrm.dHomeLocatVel = m_vctDefaultData[12];
				AxisHomePrm.dHomeIndexVel = m_vctDefaultData[13];
				AxisHomePrm.dHomeAcc = m_vctDefaultData[14];
				AxisHomePrm.lOffset = m_vctDefaultData[15];
				//设置轴 1 回零参数
				iRes += m_handle.MC_HomeSetPrm(mtid + 1, &AxisHomePrm);
			}
			m_Lock.unLock();
			return true;
		}
		vector<string > cont;
		CStrProcess::Split(szPa,CHAINFLAG,cont,true);
		int len = cont.size();
		if(0==len)
		{
			m_Lock.unLock();
			return false;
		}
		float val;
		m_mtNum = cont.size();
		for (int mtid=0;mtid<m_mtNum;mtid++)
		{
			vector<string> command;
			CStrProcess::Split(cont[mtid],MVFLAG,command,true);
			len = command.size();
			if(15>len)
				continue;

			m_MvScope[mtid] = cv::Point2f(atof(command[0].c_str()),atof(command[1].c_str()));//运动范围
			if (atoi(command[2].c_str()))
			{
				iRes = m_handle.MC_EncOn(mtid);//使用编码器
			}
			else
			{
				iRes = m_handle.MC_EncOff(mtid);//脉冲计数器模式
			}
			TTrapPrm TrapPrm;
			TrapPrm.acc = atof(command[3].c_str());//加速度
			TrapPrm.dec = atof(command[4].c_str());//减速度
			TrapPrm.smoothTime = atof(command[5].c_str());//平滑时间
			TrapPrm.velStart = atof(command[6].c_str());//起始速度
			iRes = m_handle.MC_SetTrapPrm(mtid + 1, &TrapPrm);
			m_fSpeed[mtid] = atof(command[7].c_str());//速度
			iRes += m_handle.MC_SetVel(mtid + 1, m_fSpeed[mtid]);//自动速度
			m_fDebugSpeed[mtid] = atof(command[len - 1].c_str());//调试速度

			TAxisHomePrm AxisHomePrm;
			AxisHomePrm.nHomeMode = atof(command[8].c_str());
			AxisHomePrm.nHomeDir = atof(command[9].c_str());
			AxisHomePrm.dHomeRapidVel = atof(command[10].c_str());
			AxisHomePrm.dHomeLocatVel = atof(command[11].c_str());
			AxisHomePrm.dHomeIndexVel = atof(command[12].c_str());
			AxisHomePrm.dHomeAcc = atof(command[13].c_str());
			AxisHomePrm.lOffset = atof(command[14].c_str());
			//设置轴 1 回零参数
			iRes += m_handle.MC_HomeSetPrm(mtid + 1, &AxisHomePrm);
		}
	}
	catch (...)
	{
		m_Lock.unLock();
		return false;
	}

	m_Lock.unLock();
	return true;
}

bool CMCBPMoveCard::writeMvCfg(CConfig& cfg)
{
	if (!m_bIni)
		return false;
	cfg.updateItem("\n#运动控制卡ip","");
	cfg.updateItem("MVCardIP",m_linkAdd);
	
	vector<string> mvParam;
	getParam(mvParam);//读取当前运动参数
	cfg.updateItem("\n#设置运动卡参数","");
	int len = mvParam.size();
	cfg.updateItem("MoveParamNum",GDKT::CStrProcess::itoa_radixN(len));
	//		char sz[32];
	for (int i=0;i<len;i++)
	{
		cfg.updateItem("MoveParam"+GDKT::CStrProcess::itoa_radixN(i),mvParam[i]);
		//			cfg.updateItem("MoveScope"+GDKT::CStrProcess::itoa_radixN(i),GDKT::CStrProcess::itoa_radixN(m_handle.m_iMvScope[i]));
		//			sprintf(sz,"%.4f",m_handle.m_fUnitMM[i]);
		//			cfg.updateItem("UnitMM"+GDKT::CStrProcess::itoa_radixN(i),sz);
	}
	return true;
}

bool CMCBPMoveCard::readMvCfg(CConfig& cfg)
{
	string szVal = cfg.getItem("MVCardIP");//连接ip地址
	if (szVal != "")
	{
		if(ini(szVal))
		{
			//writeCard(szCfg);
			return true;
		}
	}
	return false;
}

bool CMCBPMoveCard::getMtState(int mtid,int& state)
{
	m_Lock.lock();
	if (!m_bIni || mtid == -1)
	{
		m_Lock.unLock();
		return false;
	}
	long lSts = 0;
	bool ret = (0 == m_handle.MC_GetSts(mtid+1, &lSts));
	if (lSts & (AXIS_STATUS_RUNNING | AXIS_STATUS_HOME_RUNNING))
	{
		state = 0;
		ret = false;
	}
	else
	{
		state = -1;
		ret = true;
	}
	m_Lock.unLock();
	return ret;
	
}

bool CMCBPMoveCard::getMtPos(int mtid,float& pos)
{
	m_Lock.lock();
	if (!m_bIni || mtid == -1)
	{
		m_Lock.unLock();
		return false;
	}
	long lpos = 0;
	bool ret = (0 == m_handle.MC_GetPos(mtid + 1, &lpos));
	m_Lock.unLock();
	pos = lpos;
	return ret;
}

//获取当前位置与目的的位置差
float CMCBPMoveCard::getDvPos(int iaxis)
{
	if (!m_bIni)
		return false;
	long lpos;
	double lpos2;
	m_Lock.lock();
	int iRes = m_handle.MC_GetPos(iaxis + 1, &lpos);
	iRes += m_handle.MC_GetPrfPos(iaxis + 1, &lpos2);
	
	m_Lock.unLock();
	if(0 == iRes)
	{
		return fabs(lpos2-lpos);
	}
	else
		return FLT_MAX;

}

void CMCBPMoveCard::waitForStop(int mtid)
{
	int state=0;
	//	float curPos= FLT_MAX;
	while(true) 
	{
		getMtState( mtid, state);
		if (-1==state || !m_bIni || mtid == -1)//确认停止状态
		{
			/*m_handle.getMtPos( id, curPos);
			if (curPos==pos)*/
			break;
		}
	}
}
void CMCBPMoveCard::setSpeed(bool debug)
{
	m_Lock.lock();
	if (!m_bIni)
	{
		m_Lock.unLock();
		return;
	}
	if (debug)
	{
		int len = m_fDebugSpeed.size();
		for (int i=0;i<len;i++)
		{
			m_handle.MC_SetVel(i + 1, m_fDebugSpeed[i]);
		}
	}
	else
	{
		int len = m_fSpeed.size();
		for (int i=0;i<len;i++)
		{
			m_handle.MC_SetVel(i + 1, m_fSpeed[i]);
		}
	}
	m_Lock.unLock();
}

#endif