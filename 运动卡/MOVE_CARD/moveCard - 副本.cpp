#include "stdafx.h"
#include "moveCard.h"
#include "function.h"
#define LOADXML
void sleep(unsigned long dwMilliseconds)
{
#ifdef WIN32
	::Sleep(dwMilliseconds);
#else
	timeval wait;
	wait.tv_sec = (dwMilliseconds / 1000);
	wait.tv_usec = (dwMilliseconds - ((long)(dwMilliseconds / 1000)) * 1000) * 1000;
	::select (0, 0, 0, 0, &wait);
#endif
}

CMoveType::CMoveType()
{
	m_mtNum = 16;
	m_ioNum = 16;

	m_ioSoft = NULL;
	m_bIni = false;
}
CMoveType::~CMoveType()
{
	if (m_ioSoft != NULL)
	{
		delete[] m_ioSoft;
	}
}
bool CMoveType::ini(string linkAdd)
{
	if (m_iMvScope.size()==0)
	{
		for (int i=0;i<m_mtNum;i++)
		{
			m_iMvScope.push_back(200);
			m_fSpeed.push_back(1000);
			m_fDebugSpeed.push_back(1000);
			//		m_fUnitMM[i] = 1;
		}
	}
	
	char* szContent[] = {"轴号","运动范围","轴脉冲模式", "s曲线",\
		"脉冲当量","加速度","减速度","速度","回零反找速度","起始速度","轴正向限位","轴负向限位",\
		"回零速度","回零爬行速度","回零模式","导承","手动速度"};
	int commandLen = sizeof(szContent)/sizeof(char*);
	m_vctCfgName.clear();
	for (int i=0;i<commandLen;i++)
	{
		m_vctCfgName.push_back(szContent[i]);
	}
	float defaultData[] = {200,7,0,1000,3000,3000,100,10,3,1000,-1000,1000,1000,4,1000,1000};
	commandLen = sizeof(defaultData)/sizeof(float);
	m_vctDefaultData.clear();
	for (int i=0;i<commandLen;i++)
	{
		m_vctDefaultData.push_back(defaultData[i]);
	}
	//软触发中断器
	int ioSoftNum = g_ioNum - m_ioNum;
	if (ioSoftNum>0)
	{
		if (m_ioSoft != NULL)
		{
			delete[] m_ioSoft;
		}
		m_ioSoft = new int[ioSoftNum];
		for (int i=0;i<ioSoftNum;i++)//软触发计数器清零 1不触发
		{
			m_ioSoft[i] = 1;
		}
	}
	m_bIni = true;
	return true;
}
bool CMoveType::writeMvCfg(CConfig& cfg)
{
	
	return true;
}
bool CMoveType::readMvCfg(CConfig& cfg)
{
	return true;
}
void CMoveType::release()
{

}
void CMoveType::Rapidstop()
{
	sleep(100);
}
bool CMoveType::moveAbs(int mtid,float pos)//绝对运动
{
	sleep(100);
	return true;
}
bool CMoveType::move(int mtid,float dis)//相对运动
{
	sleep(100);
	return true;
}
bool CMoveType::moveV(int mtid,int di)//连续运动 di正向1 反向-1
{
	sleep(100);
	return true;
}
bool CMoveType::moveStop(int mtid)//针对连续运动
{
	sleep(100);
	return true;
}
bool CMoveType::mtZero(int mtid)//归零
{
	return true;
}
bool CMoveType::setIO(int ioid,int onoff)//设置io口输出
{
	sleep(100);
	//软触发中断器
	int ioSoftPos = m_ioNum;
	int ioSoftNum = g_ioNum - ioSoftPos;
	if (ioSoftNum>0 && ioid >= ioSoftPos)
	{
		if (onoff<=0)
		{
			m_ioSoft[ioid-ioSoftPos]--;//计数一次
		}
		else// if (onoff > 0 )//计数设置
		{
			m_ioSoft[ioid-ioSoftPos]=onoff;
		}
		m_Lock.unLock();
		return true;
	}
	return true;
}
bool CMoveType::getIO(uint state[])//获取输入io口的状态
{
	sleep(100);
	//软触发计数器
	int ioSoftPos = m_ioNum;
	int ioSoftNum = g_ioNum - ioSoftPos;
	for (int i=0;i<ioSoftNum;i++)
	{
		if (m_ioSoft[i] > 0)//次数小于等于0 就触发
			state[i+ioSoftPos] = 0;
		else
			state[i+ioSoftPos] = 1;
	}
	return true;
}
bool CMoveType::zeroIO()//初始化io口输出低位
{
	sleep(100);
	return true;
}
bool CMoveType::getMtState(int mtid,int& state)	//检测电机情况state=0运动中 -1没有运动
{
	sleep(100);
	return true;
}
bool CMoveType::getMtPos(int mtid,float& pos)		//检测电机位置
{
	sleep(100);
	return true;
}
bool CMoveType::getParam(vector<string>& vctPa)		//获取当前配置
{
	
	return true;
}
bool CMoveType::setParam(string szPa)		//设置配置
{
	return true;
}
void CMoveType::waitForStop(int mtid)
{
	sleep(100);
}
//获取当前位置与目的的位置差
float CMoveType::getDvPos(int iaxis)
{
	sleep(100);
	return true;
}
//判断ip字符的格式
bool CMoveType::isIPAddressValid(const char* pszIPAddr)
{
	return true;
}
bool CMoveType::getIniState()
{
	return m_bIni;
}
void CMoveType::setSpeed(bool debug)
{

}



CMoveCard::CMoveCard()
{
	m_mtNum = 16;
	m_ioNum = 16;
	for (int i=0;i<m_mtNum;i++)
	{
		m_iMvScope.push_back(200);
		m_fSpeed.push_back(1000);
		m_fDebugSpeed.push_back(1000);
		//		m_fUnitMM[i] = 1;
	}
	m_handle = NULL;
	m_linkAdd = "192.168.0.11";
}

CMoveCard::~CMoveCard()
{
	release();
}

void CMoveCard::release()
{
	if (!m_bIni)
	{
		return;
	}
	m_Lock.lock();
	m_bIni = false;
	if(NULL != m_handle)
	{
		ZAux_Rapidstop(m_handle,2);
		ZMC_Close(m_handle);
		m_handle = NULL;
	}
	m_Lock.unLock();

}

bool CMoveCard::ini(string linkAdd)
{
	m_Lock.lock();
	m_bIni = false;
	char* szContent[] = {"轴号","运动范围","轴脉冲模式", "s曲线",\
		"脉冲当量","加速度","减速度","速度","回零反找速度","起始速度","轴正向限位","轴负向限位",\
		"回零模式","导承","回零速度","手动速度"};
	int commandLen = sizeof(szContent)/sizeof(char*);
	m_vctCfgName.clear();
	for (int i=0;i<commandLen;i++)
	{
		m_vctCfgName.push_back(szContent[i]);
	}
	float defaultData[] = {200,7,0,1000,3000,3000,100,10,3,1000,-1000,4,1000,1000,1000};
	commandLen = sizeof(defaultData)/sizeof(float);
	m_vctDefaultData.clear();
	for (int i=0;i<commandLen;i++)
	{
		defaultData[11] *= i;//回零方式
		m_vctDefaultData.push_back(defaultData[i]);
	}
	//软触发中断器
	int ioSoftNum = g_ioNum - m_ioNum;
	if (ioSoftNum>0)
	{
		if (m_ioSoft != NULL)
		{
			delete[] m_ioSoft;
		}
		m_ioSoft = new int[ioSoftNum];
		for (int i=0;i<ioSoftNum;i++)//软触发计数器清零 1不触发
		{
			m_ioSoft[i] = 1;
		}
	}
	if(NULL != m_handle)
	{
		ZMC_Close(m_handle);
		m_handle = NULL;
	}

	int32 iresult=1;
	if (isIPAddressValid(linkAdd.c_str()))
		iresult = ZMC_OpenEth((char*)linkAdd.c_str(), &m_handle);

	if(ERR_SUCCESS != iresult)
	{
		m_handle = NULL;
		//	MessageBox(_T("链接失败"));
		m_Lock.unLock();
		return false;
	}
	ZAux_Rapidstop(m_handle,2);

	// 脉冲模式设置
	for (int i=0;i<m_mtNum;i++)
	{
		ZAux_SetInvertStep(m_handle,i,0);
	}
	m_linkAdd = linkAdd;
	m_bIni = true;

	m_Lock.unLock();
	return m_bIni;
}

void CMoveCard::Rapidstop()
{
	m_Lock.lock();
	
	if(NULL != m_handle)
		ZAux_Rapidstop(m_handle,2);

	m_Lock.unLock();
}

bool CMoveCard::isIPAddressValid(const char* pszIPAddr)  
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

bool CMoveCard::moveAbs(int mtid,float pos)
{
	m_Lock.lock();
	if (!m_bIni || mtid == -1)
	{
		m_Lock.unLock();
		return false;
	}
	bool ret = (ERR_SUCCESS == ZAux_Singl_MoveAbs(m_handle,mtid,pos));
	m_Lock.unLock();
	return ret;

}

bool CMoveCard::move(int mtid,float dis)
{
	m_Lock.lock();
	if (!m_bIni || mtid == -1)
	{
		m_Lock.unLock();
		return false;
	}

	bool ret = (ERR_SUCCESS == ZAux_Singl_Move(m_handle,mtid,dis));
	m_Lock.unLock();
	return ret;

}

bool CMoveCard::moveV(int mtid,int di)
{
	m_Lock.lock();
	if (!m_bIni || mtid == -1)
	{
		m_Lock.unLock();
		return false;
	}

	bool ret = (ERR_SUCCESS == ZAux_Singl_Vmove(m_handle,mtid,di*INT_MAX));
	m_Lock.unLock();
	return ret;
}

bool CMoveCard::moveStop(int mtid)
{
	m_Lock.lock();
	if (!m_bIni || mtid == -1)
	{
		m_Lock.unLock();
		return false;
	}
	bool ret = (ERR_SUCCESS == ZAux_Singl_Cancel(m_handle,mtid,2));
	m_Lock.unLock();
	return ret;
}

bool CMoveCard::mtZero(int mtid)
{
	m_Lock.lock();
	if (!m_bIni || mtid == -1)
	{
		m_Lock.unLock();
		return false;
	}
	//设定对应轴的原点输入口信号
	bool ret = (ERR_SUCCESS == ZAux_Singl_Datum(m_handle,mtid,4));
	m_Lock.unLock();
	return ret;
}

bool CMoveCard::setIO(int ioid,int onoff)
{
	m_Lock.lock();
	if (!m_bIni)
	{
		m_Lock.unLock();
		return false;
	}
	//软触发中断器
	int ioSoftNum = g_ioNum - m_ioNum;
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
	bool ret = (ERR_SUCCESS == ZAux_SetOp(m_handle,ioid,onoff));
	m_Lock.unLock();
	return ret;
}

bool CMoveCard::getIO( uint state[])
{
	m_Lock.lock();
	if (!m_bIni)
	{
		m_Lock.unLock();
		return false;
	}
	//软触发计数器
	int ioSoftNum = g_ioNum - m_ioNum;
	for (int i=0;i<ioSoftNum;i++)
	{
		if (m_ioSoft[i] > 0)//次数小于等于0 就触发
			state[i+m_ioNum] = 0;
		else
			state[i+m_ioNum] = 1;
	}
	
	for(int i = 0; i < m_ioNum; i++ )
		ZAux_GetIn(m_handle,i,&state[i]);
	m_Lock.unLock();
	return true;
}

bool CMoveCard::getParam(vector<string>& vctPa)
{
	m_Lock.lock();
	if (!m_bIni)
	{
		m_Lock.unLock();
		return false;
	}
	/*static string szContent[] = {"轴脉冲模式，0-虚拟轴 1-脉冲方式步进或伺服 7-脉冲方式步进或伺服+EZ信号输入", "s曲线，0表示梯形加减速",\
	"脉冲当量","加速度units/s/s","减速度","速度","二次回零反找速度","起始速度","轴正向软限位","轴负向软限位",\
	"回零模式","导承","回零速度","手动速度"};*/
	static char* szCommand[] = {"atype(%d)","sramp(%d)","units(%d)","accel(%d)","decel(%d)","speed(%d)","creep(%d)","lspeed(%d)",\
		"fs_limit(%d)","rs_limit(%d)","datum_in(%d)"};
	int commandLen = sizeof(szCommand)/sizeof(char*);
	float val;
	char sz[32];
	vctPa.clear();
	for (int mtid=0;mtid<m_mtNum;mtid++)
	{
		string szPa="";
		sprintf(sz,"%d",m_iMvScope[mtid]);//运动范围
		szPa += sz;szPa+=MVFLAG;
		for (int i=0;i<commandLen;i++)
		{
			sprintf(sz,szCommand[i],mtid);
			int32 iresult = ZAux_Direct_GetVariablef(m_handle, sz,&val);
			if(ERR_OK != iresult)
			{
				m_Lock.unLock();
				return false;
			}
			sprintf(sz,"%.3f",val);
			szPa += sz;szPa+=MVFLAG;
		}
		//导承
		szPa += "1000";szPa+=MVFLAG;
		//回零速度
		szPa += "1000";szPa+=MVFLAG;
		sprintf(sz,"%f",m_fDebugSpeed[mtid]);//调试速度
		szPa += sz;szPa+=MVFLAG;
		vctPa.push_back(szPa);
	}

	m_Lock.unLock();
	return true;
}

bool CMoveCard::setParam(string szPa)
{
	m_Lock.lock();
	if (!m_bIni)
	{
		m_Lock.unLock();
		return false;
	}
	static ZAux_Set_i movefun1[] = {NULL,ZAux_SetAtype,ZAux_SetSramp};
	static ZAux_Set_f movefun2[] = {ZAux_SetUnits,ZAux_SetAccel,ZAux_SetDecel,ZAux_SetSpeed,ZAux_SetCreepSpeed,ZAux_SetLSpeed/*,ZAux_Direct_SetFsLimit,ZAux_Direct_SetRsLimit*/};
	int funLen1 = sizeof(movefun1)/sizeof(ZAux_Set_i);
	int funLen2 = sizeof(movefun2)/sizeof(ZAux_Set_f) + funLen1;
	try
	{
		if (""==szPa)//加载默认参数
		{
			int zeroModel = m_vctDefaultData[13];
			for (int mtid=0;mtid<m_mtNum;mtid++)
			{
				m_iMvScope[mtid] = m_vctDefaultData[0];//运动范围
				int i=1;
				for (;i<funLen1;i++)
				{
					movefun1[i](m_handle,mtid,m_vctDefaultData[i]);
				}
				for (;i<funLen2;i++)
				{
					movefun2[i-funLen1](m_handle,mtid,m_vctDefaultData[i]);
				}
				ZAux_SetDatumin(m_handle, mtid, zeroModel);
				m_fDebugSpeed[mtid] = m_vctDefaultData[m_vctDefaultData.size()-1];//调试速度
				m_fSpeed[mtid] = m_vctDefaultData[6];//调试速度
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
			if(funLen2>len)
				continue;

			m_iMvScope[mtid] = atoi(command[0].c_str());//运动范围
			int i=1;
			for (;i<funLen1;i++)
			{
				val = atof(command[i].c_str());
				movefun1[i](m_handle,mtid,val);
			}
			for (;i<funLen2;i++)
			{
				val = atof(command[i].c_str());
				movefun2[i-funLen1](m_handle,mtid,val);

				//设置脉冲当量	1表示以一个脉冲为单位 ，设置为1MM的脉冲个数，这度量单位为MM
				//return ERR_SUCCESS == ZAux_SetUnits(m_handle,mtid,value);
			}
			int zeroModel = 4;
			if (len>13)
				zeroModel = atoi(command[11].c_str());
			ZAux_SetDatumin(m_handle, mtid, zeroModel);
			m_fDebugSpeed[mtid] = atof(command[len-1].c_str());//调试速度
			m_fSpeed[mtid] = atof(command[6].c_str());//自动速度
			//mtZero(mtid);//首先肯定是归位
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

bool CMoveCard::writeMvCfg(CConfig& cfg)
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

bool CMoveCard::readMvCfg(CConfig& cfg)
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

bool CMoveCard::getMtState(int mtid,int& state)
{
	m_Lock.lock();
	if (!m_bIni || mtid == -1)
	{
		m_Lock.unLock();
		return false;
	}
	bool ret = (ERR_SUCCESS == ZAux_Direct_GetIfIdle(m_handle, mtid, &state));
	if (0 == state)
	{
		ret = false;
	}
	else
		ret = true;
	m_Lock.unLock();
	return ret;
	
}

bool CMoveCard::getMtPos(int mtid,float& pos)
{
	m_Lock.lock();
	if (!m_bIni || mtid == -1)
	{
		m_Lock.unLock();
		return false;
	}
	bool ret = (ERR_SUCCESS == ZAux_Direct_GetDpos(m_handle, mtid, &pos));
	m_Lock.unLock();
	return ret;
}

//获取当前位置与目的的位置差
float CMoveCard::getDvPos(int iaxis)
{
	if (!m_bIni)
		return false;
	float ftemp,ftemp2;
	m_Lock.lock();
	int32 iresult = ZAux_Direct_GetDpos(m_handle, iaxis, &ftemp);
	int32 iresult2 = ZAux_Direct_GetEndMove(m_handle, iaxis, &ftemp2);
	m_Lock.unLock();
	if(ERR_OK == iresult && ERR_OK == iresult2 )
	{
		return fabs(ftemp-ftemp2);
	}
	else
		return FLT_MAX;

}

void CMoveCard::waitForStop(int mtid)
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
void CMoveCard::setSpeed(bool debug)
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
			ZAux_SetSpeed(m_handle,i,m_fDebugSpeed[i]);
		}
	}
	else
	{
		int len = m_fSpeed.size();
		for (int i=0;i<len;i++)
		{
			ZAux_SetSpeed(m_handle,i,m_fSpeed[i]);
		}
	}
	m_Lock.unLock();
}

#ifdef _LINHUA
CLinHuaCard::CLinHuaCard()
{
	m_mtNum = 24;
	m_ioNum = 24;
	for (int i=0;i<m_mtNum;i++)
	{
		m_iMvScope.push_back(200);
		m_fSpeed.push_back(10000);
		m_fDebugSpeed.push_back(10000);
		m_iMvSpeed.push_back(10000);
		m_mvScale.push_back(1000);
		//		m_fUnitMM[i] = 1;
	}
	m_linkAdd = getExePath()+"amp208c-param.xml";
}

CLinHuaCard::~CLinHuaCard()
{
	release();
}
void CLinHuaCard::release()
{
	if (!m_bIni)
	{
		return;
	}
	m_Lock.lock();
	for( int i = 0; i < v_total_axis; i++ )
	{
		int axis_id = i + v_first_axis_id[0] ;
		APS_emg_stop( axis_id );
	}
	APS_close();
	m_bIni = false;

	m_Lock.unLock();
}

bool CLinHuaCard::ini(string linkAdd)
{
	m_Lock.lock();
	if( m_bIni)
	{
		APS_close();
	}
	m_bIni = false;
#ifdef LOADXML
	char* szContent[] = {"轴号","运动范围","手动速度","速度","导承"/*,"归零模式","归零方向","EZ对准",\
		"T曲线","归位加速度","归位最大速度","归位VO速度","归位回移距离","归位归位位置",\
		"停减速","加速曲线","加速度","减速度"*/};
	float defaultData[] = {200,1000,10000,1000/*,0,1,0, \
		0,1000000,100000,50000,0,0,10000.0,0.5,10000.0,10000.0*/};
#else
		char* szContent[] = {"轴号","运动范围","手动速度", "速度","导承","归零模式","归零方向","EZ对准",\
		"T曲线","归位加速度","归位最大速度","归位VO速度","归位回移距离","归位归位位置",\
		"停减速","加速曲线","加速度","减速度"};
	float defaultData[] = {200,1000,10000,1000,0,1,0, \
		0,1000000,100000,50000,0,0,10000.0,0.5,10000.0,10000.0};
#endif
	int commandLen = sizeof(szContent)/sizeof(char*);
	m_vctCfgName.clear();
	for (int i=0;i<commandLen;i++)
	{
		m_vctCfgName.push_back(szContent[i]);
	}
	commandLen = sizeof(defaultData)/sizeof(float);
	m_vctDefaultData.clear();
	for (int i=0;i<commandLen;i++)
	{
		m_vctDefaultData.push_back(defaultData[i]);
	}

	I32 boardID_InBits = 0;
	I32 mode           = INIT_AUTO_CARD_ID;
	I32 board_ID       = -1;

	I32 card_name;

	// Card(Board) initial
	I32 ret = APS_initial( &boardID_InBits, mode );

	// Search card's ID
	if( ret == ERR_NoError )
	{
		int cardN=0;
		for(I32  i = 0; i < 16; i++ )
		{  
			if( ( boardID_InBits >> i ) & 1 )
			{
				APS_get_card_name( i, &card_name );

				if( card_name == DEVICE_NAME_PCI_825458 || card_name == DEVICE_NAME_AMP_20408C )
				{
					v_card_name = card_name;
					board_ID = i;
					v_card_id[cardN++] = board_ID;
				}
			}
		}

		if( board_ID == -1 )
		{
			//		MessageBox( "BoardId search error!" );
			m_Lock.unLock();
			return m_bIni;
		}	

		v_card_num = cardN;
		I32 total_axes;
		v_total_axis = 0;
		for(int x=0;x<v_card_num;x++)
		{
			APS_get_first_axisId( v_card_id[x], &v_first_axis_id[x], &total_axes );
			v_total_axis  += total_axes;
		}

		m_bIni = true;
		//MessageBox( "initial ok" );
	}
	else
	{
		//char MsgStr[256]="Error Code = ";
		//char Buff[256];

		//_itoa_s(ret,Buff,10);     //
		//strcat_s(MsgStr,Buff);   //

		//if (ret != 0)
		//{
		//	return MsgStr;
		//}
	}
	/*if (linkAdd != "")
	{
		m_linkAdd = linkAdd;
	}*/
#ifdef LOADXML
	ret = APS_load_param_from_file( m_linkAdd.c_str() );
	if (ret!= ERR_NoError )
		m_bIni = false;
#endif
	m_Lock.unLock();
	return m_bIni;
}

bool  CLinHuaCard::writeMvCfg(CConfig& cfg)
{
	if (!m_bIni)
		return false;
	cfg.updateItem("\n#设置运动卡参数","xml文件位置");
	std::size_t found = m_linkAdd.find_last_of("/\\")+1; 
	cfg.updateItem("MVCardIP",m_linkAdd.substr(found, m_linkAdd.size()));

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

bool  CLinHuaCard::readMvCfg(CConfig& cfg)
{
	string szVal = cfg.getItem("MVCardIP");//连接ip地址
	if (szVal != "")
	{
		m_linkAdd = getExePath()+szVal;
	}
	if(ini(""))
	{
		//writeCard(szCfg);
		return true;
	}
	else 
		return false;
}

void CLinHuaCard::Rapidstop()
{
	m_Lock.lock();
	for( int i = 0; i < v_total_axis; i++ )
	{
		int axis_id = i + v_first_axis_id[0] ;
		APS_emg_stop( axis_id );
	}
	m_Lock.unLock();
}

bool CLinHuaCard::moveAbs(int mtid,float pos)
{
	mtid += v_first_axis_id[0];	
	m_Lock.lock();
	if (!m_bIni || mtid == -1)
	{
		m_Lock.unLock();
		return false;
	}
	pos *= m_mvScale[mtid];

	if( !( ( APS_motion_io_status( mtid ) >> MIO_SVON ) & 1 ) )  
	{
		APS_set_servo_on( mtid, 1 ); 
		Sleep( 500 ); // Wait stable.
	}
	//F64 v;
	//APS_get_command_velocity_f(mtid,&v);
	// 2. Start  move
	I32 return_code = APS_absolute_move( mtid,pos,m_fSpeed[mtid] ); //Start homing APS_vel( mtid, 0, m_iMvSpeed[mtid], 0 );  // Start a velocity move 
	if( return_code != ERR_NoError )
	{ 
		m_Lock.unLock();
		return false;
	}
	else
	{
		m_Lock.unLock();
		return true;
	}
}

bool CLinHuaCard::move(int mtid,float dis)
{
	mtid += v_first_axis_id[0];	
	m_Lock.lock();
	if (!m_bIni || mtid == -1)
	{
		m_Lock.unLock();
		return false;
	}
	dis *= m_mvScale[mtid];

	//Check servo on or not
	if( !( ( APS_motion_io_status( mtid ) >> MIO_SVON ) & 1 ) )  
	{
		APS_set_servo_on( mtid, 1 ); 
		Sleep( 500 ); // Wait stable.
	}
	/*F64 v;
	APS_get_command_velocity_f(mtid,&v);*/
	// 2. Start  move
	I32 return_code = APS_relative_move( mtid,dis,m_fSpeed[mtid]); //Start homing 
	if( return_code != ERR_NoError )
	{ 
		m_Lock.unLock();
		return false;
	}
	else
	{
		m_Lock.unLock();
		return true;
	}
}

bool CLinHuaCard::moveV(int mtid,int di)
{
	mtid += v_first_axis_id[0];
	m_Lock.lock();
	if (!m_bIni || mtid == -1)
	{
		m_Lock.unLock();
		return false;
	}
	if (di<0)
		APS_set_axis_param( mtid, PRA_HOME_DIR,       0 ); // Set home direction
	else
		APS_set_axis_param( mtid, PRA_HOME_DIR,       1 ); // Set home direction
	//Check servo on or not
	if( !( ( APS_motion_io_status( mtid ) >> MIO_SVON ) & 1 ) )  
	{
		APS_set_servo_on( mtid, 1 ); 
		Sleep( 500 ); // Wait stable.
	}
	/*F64 v;
	APS_get_command_velocity_f(mtid,&v);*/
	// 2. Start  move
	I32 return_code = APS_absolute_move( mtid,di*INT_MAX,m_fSpeed[mtid] ); //Start homing 
	if( return_code != ERR_NoError )
	{ 
		m_Lock.unLock();
		return false;
	}
	else
	{
		m_Lock.unLock();
		return true;
	}
	
}

bool CLinHuaCard::moveStop(int mtid)
{
	m_Lock.lock();
	if (!m_bIni || mtid == -1)
	{
		m_Lock.unLock();
		return false;
	}
	APS_set_axis_param_f( mtid, PRA_STP_DEC, 10000.0 );
	
	//Check servo on or not
	if( !( ( APS_motion_io_status( mtid ) >> MIO_SVON ) & 1 ) )  
	{
		APS_set_servo_on( mtid, 1 ); 
		Sleep( 500 ); // Wait stable.
	}

	// 2.
	I32 return_code = APS_stop_move( mtid );
	if( return_code != ERR_NoError )
	{ 
		m_Lock.unLock();
		return false;
	}
	else
	{
		m_Lock.unLock();
		return true;
	}
}

bool CLinHuaCard::mtZero(int mtid)
{
	m_Lock.lock();
	if (!m_bIni || mtid == -1)
	{
		m_Lock.unLock();
		return false;
	}

	//Check servo on or not
	if( !( ( APS_motion_io_status( mtid ) >> MIO_SVON ) & 1 ) )  
	{
		APS_set_servo_on( mtid, 1 ); 
		Sleep( 500 ); // Wait stable.
	}

	// 2. Start home move
	I32 return_code = APS_home_move( mtid ); //Start homing 
	if( return_code != ERR_NoError )
	{ 
		m_Lock.unLock();
		return false;
	}
	else
	{
		m_Lock.unLock();
		return true;
	}
}

bool CLinHuaCard::setIO(int ioid,int onoff)
{
	m_Lock.lock();
	if (!m_bIni)
	{
		m_Lock.unLock();
		return false;
	}
	int cardId = ioid/m_ioNum;
	ioid = ioid%m_ioNum;
	I32 digital_output_value = 0;
	I32 return_code = APS_read_d_output( cardId
		, 0                     // I32 DO_Group
		, &digital_output_value // I32 *DO_Data
		);
	if( return_code != ERR_NoError )
	{ 
		m_Lock.unLock();
		return false;
	}
	if (onoff)
		digital_output_value |= ( 1 << ioid );
	else
		digital_output_value &= ~( 1 << ioid );
	return_code = APS_write_d_output( cardId
		, 0                     // I32 DO_Group
		, digital_output_value  // I32 DO_Data
		);
	if( return_code != ERR_NoError )
	{ 
		m_Lock.unLock();
		return false;
	}
	else
	{
		m_Lock.unLock();
		return true;
	}
}

bool CLinHuaCard::getIO(unsigned int state[])
{
	m_Lock.lock();
	if (!m_bIni)
	{
		m_Lock.unLock();
		return false;
	}
	for (int n=0;n<v_card_num;n++)
	{
		I32 digital_input_value  = 0;
		APS_read_d_input( v_card_id[n]
		, 0                    // I32 DI_Group
			, &digital_input_value // I32 *DI_Data
			);
		int i = 0;
		for(; i < m_ioNum; i++ )
			state[i+n*m_ioNum] = ( ( digital_input_value >> i ) & 1 );

		I32 motion_io = APS_motion_io_status( v_first_axis_id[n] );
		//	I32 motion_status = APS_motion_status( v_first_axis_id );
		state[i+n*m_ioNum] = (( motion_io >> MIO_EMG ) & 1);//急停状态
	}
	
	m_Lock.unLock();
	return true;
}

bool CLinHuaCard::getParam(vector<string>& vctPa)
{
	m_Lock.lock();
	if (!m_bIni)
	{
		m_Lock.unLock();
		return false;
	}
	vctPa.clear();
	char sz[32];
#ifndef LOADXML
	I32 iParam[] = {PRA_HOME_MODE,PRA_HOME_DIR,PRA_HOME_EZA};//"归零模式","归零方向"，"EZ对准"
	int iL = sizeof(iParam)/sizeof(I32);
	I32* iValue = new I32[iL];
	I32 fParam[] = {PRA_HOME_CURVE,PRA_HOME_ACC,PRA_HOME_VM,PRA_HOME_VO,PRA_HOME_SHIFT,PRA_HOME_POS, \
		PRA_STP_DEC,PRA_CURVE,PRA_ACC,PRA_DEC};
	int fL = sizeof(fParam)/sizeof(I32);
	F64* fValue = new F64[fL];
	for(int i = 0; i < v_total_axis; i++ )
	{
		string szPa="";
		int axis_id = i + v_first_axis_id ;
		/*F64 v;
		APS_get_command_velocity_f(axis_id,&v);
		m_iMvSpeed[i] = v;*/
		sprintf(sz,"%d",m_iMvScope[i]);szPa += sz;szPa+=MVFLAG;
		sprintf(sz,"%.3f",m_fDebugSpeed[i]);szPa += sz;szPa+=MVFLAG;
		sprintf(sz,"%.3f",m_fSpeed[i]);szPa += sz;szPa+=MVFLAG;
		sprintf(sz,"%d",m_mvScale[i]);szPa += sz;szPa+=MVFLAG;
		for (int j=0;j<iL;j++)
		{
			APS_get_axis_param(axis_id,iParam[j],&iValue[j]);
			sprintf(sz,"%d",iValue[j]);//
			szPa += sz;szPa+=MVFLAG;
		}
		for (int j=0;j<fL;j++)
		{
			APS_get_axis_param_f(axis_id,fParam[j],&fValue[j]);
			sprintf(sz,"%.3f",fValue[j]);//
			szPa += sz;szPa+=MVFLAG;
		}
		vctPa.push_back(szPa);
	}
	delete[] iValue;
	delete[] fValue;
#else
	for(int i = 0; i < v_total_axis; i++ )
	{
		string szPa="";
		int axis_id = i + v_first_axis_id[0] ;
		/*F64 v;
		APS_get_command_velocity_f(axis_id,&v);
		m_iMvSpeed[i] = v;*/
		sprintf(sz,"%d",m_iMvScope[i]);szPa += sz;szPa+=MVFLAG;
		sprintf(sz,"%.3f",m_fDebugSpeed[i]);szPa += sz;szPa+=MVFLAG;
		sprintf(sz,"%.3f",m_fSpeed[i]);szPa += sz;szPa+=MVFLAG;
		sprintf(sz,"%d",m_mvScale[i]);szPa += sz;szPa+=MVFLAG;
		vctPa.push_back(szPa);
	}
#endif
	m_Lock.unLock();
	return true;
}

bool CLinHuaCard::setParam(string szPa)
{
	m_Lock.lock();
	if (!m_bIni)
	{
		m_Lock.unLock();
		return false;
	}
#ifndef LOADXML
	I32 iParam[] = {PRA_HOME_MODE,PRA_HOME_DIR,PRA_HOME_EZA};//"归零模式","归零方向"，"EZ对准"
	int iL = sizeof(iParam)/sizeof(I32);
	I32* iValue = new I32[iL];
	I32 fParam[] = {PRA_HOME_CURVE,PRA_HOME_ACC,PRA_HOME_VM,PRA_HOME_VO,PRA_HOME_SHIFT,PRA_HOME_POS, \
		PRA_STP_DEC,PRA_CURVE,PRA_ACC,PRA_DEC};
	int fL = sizeof(fParam)/sizeof(I32);

	if (""==szPa)//加载默认参数
	{
//		APS_load_parameter_from_default(v_card_id);
		for(int i = 0; i < v_total_axis; i++ )
		{
			int axis_id = i + v_first_axis_id ;
			m_iMvScope[i] = m_vctDefaultData[0];//运动范围
			m_fDebugSpeed[i] = m_vctDefaultData[1];//手动速度
			m_iMvSpeed[i]=m_fSpeed[i] = m_vctDefaultData[2];//运动速度
			m_mvScale[i] = m_vctDefaultData[3];//承载
			for (int j=0;j<iL;j++)
			{
				APS_set_axis_param(axis_id,iParam[j],m_vctDefaultData[j+3]);
			}
			for (int j=0;j<fL;j++)
			{
				APS_set_axis_param_f(axis_id,fParam[j],m_vctDefaultData[j+iL+3]);
			}
		}
	}
	else
	{
//		APS_load_param_from_file( m_linkAdd.c_str() );
		vector<string > cont;
		CStrProcess::Split(szPa,CHAINFLAG,cont,true);
		int len = cont.size();
		if(0==len)
		{
			m_Lock.unLock();
			return false;
		}
		float val;
		v_total_axis = cont.size();
		for(int i = 0; i < v_total_axis; i++ )
		{
			vector<string> command;
			CStrProcess::Split(cont[i],MVFLAG,command,true);
			len = command.size();
			if(iL+fL>len)
				continue;

			m_iMvScope[i] = atoi(command[0].c_str());//运动范围
			m_fDebugSpeed[i] = atof(command[1].c_str());//手动速度
			m_iMvSpeed[i]=m_fSpeed[i] = atof(command[2].c_str());//运动速度
			m_mvScale[i] = atoi(command[3].c_str());//承载
			int axis_id = i + v_first_axis_id ;
			for (int j=0;j<iL;j++)
			{
				val = atoi(command[j+3].c_str());
				APS_set_axis_param(axis_id,iParam[j],val);
			}
			for (int j=0;j<fL;j++)
			{
				val = atof(command[j+iL+3].c_str());
				APS_set_axis_param_f(axis_id,fParam[j],val);
			}
		}
	}
#else

	if (""==szPa)//加载默认参数
	{
		//		APS_load_parameter_from_default(v_card_id);
		for(int i = 0; i < v_total_axis; i++ )
		{
			int axis_id = i + v_first_axis_id[0] ;
			m_iMvScope[i] = m_vctDefaultData[0];//运动范围
			m_fDebugSpeed[i] = m_vctDefaultData[1];//手动速度
			m_iMvSpeed[i]=m_fSpeed[i] = m_vctDefaultData[2];//运动速度
			m_mvScale[i] = m_vctDefaultData[3];//导承
		}
	}
	else
	{
		//		APS_load_param_from_file( m_linkAdd.c_str() );
		vector<string > cont;
		CStrProcess::Split(szPa,CHAINFLAG,cont,true);
		int len = cont.size();
		if(0==len)
		{
			m_Lock.unLock();
			return false;
		}
		float val;
		v_total_axis = cont.size();
		for(int i = 0; i < v_total_axis; i++ )
		{
			vector<string> command;
			CStrProcess::Split(cont[i],MVFLAG,command,true);
			len = command.size();
			if(3>len)
				continue;

			m_iMvScope[i] = atoi(command[0].c_str());//运动范围
			m_fDebugSpeed[i] = atof(command[1].c_str());//手动速度
			m_iMvSpeed[i]=m_fSpeed[i] = atof(command[2].c_str());//运动速度
			m_mvScale[i] = atoi(command[3].c_str());//导承
		}
	}
#endif
	m_Lock.unLock();
	return true;
}

const char *CLinHuaCard::stop_code_to_string( I32 Stop_Code )
{
	switch( Stop_Code )
	{
	case 0: return "STOP_NORMAL"; 
	case 1: return "STOP_EMG";         
	case 2: return "STOP_ALM";         
	case 3: return "STOP_SVNO";        
	case 4: return "STOP_PEL";         
	case 5: return "STOP_MEL";         
	case 6: return "STOP_SPEL";        
	case 7: return "STOP_SMEL";        
	case 8: return "STOP_USER_EMG";    
	case 9: return "STOP_USER";        
	case 10: return "STOP_GAN_L1";     
	case 11: return "STOP_GAN_L2";     
	case 12: return "STOP_GEAR_SLAVE"; 
	case 13: return "STOP_ERROR_LEVEL";
	case 20: return "STOP_ORG_EL";     
	case 21: return "STOP_NO_ORG";     
	case 22: return "STOP_NO_EZ";      
	case 23: return "STOP_BOTH_EL";
	default: return "Unknown stop code!";
	}
}

bool CLinHuaCard::getMtState(int mtid,int& state)
{
	mtid += v_first_axis_id[0];
	m_Lock.lock();
	if (!m_bIni || mtid == -1)
	{
		m_Lock.unLock();
		return false;
	}

	I32 msts;
	msts = APS_motion_status( mtid );// Get motion status
	msts = ( msts >> MTS_NSTP ) & 1;    // Get motion done bit
	// msts = ( msts >> MTS_HMV ) & 1;    // Get homming bit
	I32 Stop_Code;
	// Get stop code.
	APS_get_stop_code( mtid, &Stop_Code );
	state = Stop_Code;
	if( msts == 1 )
	{
#ifndef IOSIGN
		// Check move success or not
		msts = APS_motion_status( mtid ); // Get motion status
		msts = ( msts >> MTS_ASTP ) & 1;     // Get abnormal stop bit
		if( msts == 1 )
		{ // Error handling ...
			I32 stop_code;
			APS_get_stop_code( mtid, &stop_code );
			// ...
			m_Lock.unLock();
			char str[128];
			sprintf(str, "Error stop! stop code:%d(%s)", stop_code, stop_code_to_string( stop_code ) );
			throw(str);
			return true; //error
		}
		else
		{ // Motion success.
			m_Lock.unLock();
			return true; 
		}
#else
		I32 motion_io = APS_motion_io_status( mtid );
		if (0 == (( motion_io >> MIO_INP ) & 1) )
		{
			state = 1;
			m_Lock.unLock();
			return true; 
		}
#endif
	}

	m_Lock.unLock();
	return false; //Now are in motion
}

bool CLinHuaCard::getMtPos(int mtid,float& pos)
{
	mtid += v_first_axis_id[0];
	m_Lock.lock();
	if (!m_bIni || mtid == -1)
	{
		m_Lock.unLock();
		return false;
	}
	F64 p;
	I32 iresult  = (ERR_SUCCESS == APS_get_target_position_f(mtid, &p));//APS_get_position_f(mtid, &p));
	pos = p/m_mvScale[mtid-v_first_axis_id[0]];
	m_Lock.unLock();
	if(ERR_NoError != iresult)
	{
		return true;
	}
	else
		return false;

}

//获取当前位置与目的的位置差
float CLinHuaCard::getDvPos(int iaxis)
{
	iaxis += v_first_axis_id[0];
	if (!m_bIni)
		return false;

	F64 ftemp,ftemp2;
	m_Lock.lock();
	I32 iresult = APS_get_position_f( iaxis, &ftemp);
	I32 iresult2 = APS_get_target_position_f( iaxis, &ftemp2);
	m_Lock.unLock();
	if(ERR_NoError != iresult && ERR_NoError != iresult2 )
	{
		return fabs(ftemp-ftemp2)/m_mvScale[iaxis-v_first_axis_id[0]];
	}
	else
		return FLT_MAX;

}

void CLinHuaCard::waitForStop(int mtid)
{
	int state=0;
	//	float curPos= FLT_MAX;
	while(true) 
	{
		if (!m_bIni || mtid+v_first_axis_id[0] == -1 || getMtState( mtid, state))//确认停止状态
		{
			/*m_handle.getMtPos( id, curPos);
			if (curPos==pos)*/
			break;
		}
	}
}
void CLinHuaCard::setSpeed(bool debug)
{
	m_Lock.lock();
	if (!m_bIni)
	{
		m_Lock.unLock();
		return ;
	}
	if (debug)
	{
		int len = m_fDebugSpeed.size();
		for (int i=0;i<len;i++)
		{
			m_fSpeed[i]=m_fDebugSpeed[i];
		}
	}
	else
	{
		int len = m_fSpeed.size();
		for (int i=0;i<len;i++)
		{
			m_fSpeed[i]=m_iMvSpeed[i];
		}
	}
	m_Lock.unLock();
}
CLinHuaCardExt::CLinHuaCardExt()
{
#ifdef IOEXT
	m_ioExtNum = 32;
#else
	m_ioExtNum = 0;
#endif
	m_dwInData = 0x00000000;
	m_dwOutData = 0x00000000;
}

CLinHuaCardExt::~CLinHuaCardExt()
{
#ifdef IOEXT
	if (m_hCard >= 0)
		Release_Card(m_hCard);
#endif
}

bool CLinHuaCardExt::ini(string linkAdd)
{
#ifdef IOEXT
	int nCardID = 0;
	if ((m_hCard=Register_Card(PCI_7432, nCardID)) < 0) 
	{
		/*strTemp.Format( TEXT("Register_Card Error = %d"), hCard);
		AfxMessageBox( strTemp );*/
		return false;
	}
	DO_WritePort( m_hCard, 0, m_dwOutData );
#endif	
	if (CLinHuaCard::ini(linkAdd))
	{
		//软触发中断器
		int ioTal = m_ioNum*v_card_num;//运动卡的io口数量
		int ioSoftPos = ioTal+m_ioExtNum;
		int ioSoftNum = g_ioNum - ioSoftPos;
		if (ioSoftNum>0)
		{
			if (m_ioSoft != NULL)
			{
				delete[] m_ioSoft;
			}
			m_ioSoft = new int[ioSoftNum];
			for (int i=0;i<ioSoftNum;i++)//软触发计数器清零 1不触发
			{
				m_ioSoft[i] = 1;
			}
		}
		return true;
	}
	else
		return false;
}

bool CLinHuaCardExt::setIO(int ioid,int onoff)
{	
	if ( ioid >= g_ioNum)
	{
		return false;
	}
	int ioTal = m_ioNum*v_card_num;//运动卡的io口数量
	if( ioid > ioTal )
	{ 
		m_Lock.lock();
		if (!m_bIni)
		{
			m_Lock.unLock();
			return false;
		}
		//软触发中断器
		int ioSoftPos = ioTal+m_ioExtNum;
		int ioSoftNum = g_ioNum - ioSoftPos;
		if (ioSoftNum>0 && ioid >= ioSoftPos)
		{
			if (onoff<=0)
			{
				m_ioSoft[ioid-ioSoftPos]--;//计数一次
			}
			else// if (onoff > 0 )//计数设置
			{
				m_ioSoft[ioid-ioSoftPos]=onoff;
			}
			m_Lock.unLock();
			return true;
		}
#ifdef IOEXT
		U32 sign = (1<<(ioid-ioTal));
		if( onoff == BST_CHECKED )
		{
			m_dwOutData |= sign;
		}
		else if( onoff == BST_UNCHECKED )
		{
			m_dwOutData &= (~sign);
		}

		DO_WritePort( m_hCard, 0, m_dwOutData );
#endif
		m_Lock.unLock();
		return true;
	}
	else
	{
		return CLinHuaCard::setIO(ioid,onoff);
	}
}

bool CLinHuaCardExt::getIO(unsigned int state[])
{
	int ioTal = m_ioNum*v_card_num;//运动卡的io口数量
	m_Lock.lock();
	if (!m_bIni)
	{
		m_Lock.unLock();
		return false;
	}
#ifdef IOEXT
	U32 dwPortData, dwChangeMask;

    DI_ReadPort(m_hCard, PORT_DI_LOW, &dwPortData);

	dwChangeMask = dwPortData ^ m_dwInData;

	if( dwChangeMask != 0x00 ) // Input had been changed 
	{
		 m_dwInData = dwPortData; // update the DI data
		 U32 sign = 1;
		 for (int i=0;i<m_ioExtNum;i++)
		 {
			 if( (dwChangeMask & sign) == sign ) // if the target bit is changed 
			 {
				 if( (dwPortData & sign) == sign )
				 {
					 state[i+ioTal] = 1;
				 }
				 else
				 {
					 state[i+ioTal] = 0;
				 }
			 }
			 sign = (sign << 1);
		 }
	}
#endif
	//软触发计数器
	int ioSoftPos = ioTal+m_ioExtNum;
	int ioSoftNum = g_ioNum - ioSoftPos;
	for (int i=0;i<ioSoftNum;i++)
	{
		if (m_ioSoft[i] > 0)//次数小于等于0 就触发
			state[i+ioSoftPos] = 0;
		else
			state[i+ioSoftPos] = 1;
	}
	m_Lock.unLock();

	return CLinHuaCard::getIO(state);
}

bool CLinHuaCardExt::zeroIO()
{
	int ioTal = m_ioNum*v_card_num;//运动卡的io口数量
	m_Lock.lock();
	if (!m_bIni)
	{
		m_Lock.unLock();
		return false;
	}
	int ioSoftNum = g_ioNum - ioTal - m_ioExtNum;
	for (int i=0;i<ioSoftNum;i++)//软触发计数器清零 1不触发
	{
		m_ioSoft[i] = 1;
	}
#ifdef IOEXT
	//扩展卡清零
	m_dwOutData = 0x00000000;
	DO_WritePort( m_hCard, 0, m_dwOutData );
#endif
	m_Lock.unLock();
	return CLinHuaCard::zeroIO();
}
#endif
