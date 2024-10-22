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

	//һ���忨������1�����󣬶���忨������������󣬸�������֮�以������
	//5����������Ϊ�忨�š�PC��IP��ַ��PC�˶˿ںţ��忨��IP��ַ���忨�˿ںš�
	//ͨ�����鲻ͬ�忨IP��ַҪ��ͬ���˿ں�ҲҪ��ͬ
	//PC�˶˿ںźͰ忨�˿ں�Ҫ����һ�£������60000��ʼ���ڶ����忨��60001����������......
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
//		iRes = g_MultiCard.MC_EncOn(iAxisNum);//ʹ�ñ�����
//	}
//	else
//	{
//		iRes = g_MultiCard.MC_EncOff(iAxisNum);//���������ģʽ
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
	char* szContent[] = {"���","������λ","������λ", "������ģʽ",\
		"���ٶ�","���ٶ�","ƽ��ʱ��","��ʼ�ٶ�","�ٶ�",\
		"���㷽ʽ","���㷽��","��������ٶ�","���㶨λ�ٶ�","����Ѱ��INDEX�ٶ�","������ٶ�","����ƫ��",\
		"�Ḻ����λ","��������λ","����","�ֶ��ٶ�"};
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
	//�����ж���
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
		//	MessageBox(_T("����ʧ��"));
		m_Lock.unLock();
		return false;
	}
	m_handle.MC_Stop(0XFFFFFFFF, 0XFFFFFFFF);
	iRes+= m_handle.MC_EStopSetIO(0, 0, 0, 10);//��������ͨ������ X0 Ϊ����ֹͣ IO,�˲�ʱ�� 10ms
	iRes+= m_handle.MC_EStopOnOff(1);//����ֹͣ���ܴ�

	// ����ģʽ����
	for (int i=0;i<m_mtNum;i++)
	{
		iRes+= m_handle.MC_EncOff(i+1);//���������ģʽ
		iRes+= m_handle.MC_AxisOn(i+1);

		iRes = m_handle.MC_PrfTrap(i+1);//����ָ����Ϊ��λģʽ
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
		m_handle.MC_PrfTrap(i + 1);//����ָ����Ϊ��λģʽ
	m_Lock.unLock();
}

bool CMCBPMoveCard::isIPAddressValid(const char* pszIPAddr)  
{  
	if (!pszIPAddr) return false; //��pszIPAddrΪ��  
	char IP1[100],cIP[4];  
	int len = strlen(pszIPAddr);  
	int i = 0,j=len-1;  
	int k, m = 0,n=0,num=0;  
	//ȥ����β�ո�(ȡ����i-1��j+1֮����ַ�):  
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
		cIP[n++] = *p; //����ÿ���Ӷεĵ�һ���ַ�������֮���жϸ��Ӷ��Ƿ�Ϊ0��ͷ  

		int sum = 0;  //sumΪÿһ�Ӷε���ֵ��Ӧ��0��255֮��  
		while (*p != '.'&&*p != '\0')  
		{  
			if (*p == ' ' || *p<'0' || *p>'9') return false;  
			sum = sum * 10 + *p-48;  //ÿһ�Ӷ��ַ���ת��Ϊ����  
			p++;  
		}  
		if (*p == '.') {  
			if ((*(p - 1) >= '0'&&*(p - 1) <= '9') && (*(p + 1) >= '0'&&*(p + 1) <= '9'))//�ж�"."ǰ���Ƿ������֣����ޣ���Ϊ��ЧIP���硰1.1.127.��  
				num++;  //��¼��.�����ֵĴ��������ܴ���3  
			else  
				return false;  
		};  
		if ((sum > 255) || (sum > 0 && cIP[0] =='0')||num>3) return false;//���Ӷε�ֵ>255��Ϊ0��ͷ�ķ�0�Ӷλ�.������Ŀ>3����Ϊ��ЧIP  

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
	m_handle.MC_PrfTrap(mtid + 1);//����ָ����Ϊ��λģʽ
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
	//�趨��Ӧ���ԭ��������ź�
	int iRes = 0;
//	short nStatus = 0;
	//������ 1 ����
	iRes = m_handle.MC_HomeStart(mtid+1);
	//��ȡ�� 1 ����״̬
//	iRes = m_handle.MC_HomeGetSts(mtid+1, &nStatus);


	m_handle.MC_PrfTrap(mtid + 1);//����ָ����Ϊ��λģʽ
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
	//�����ж���
	int ioSoftNum = g_ioN - m_ioNum;
	if (ioSoftNum>0 && ioid >= m_ioNum)
	{
		if (onoff<=0)
		{
			m_ioSoft[ioid-m_ioNum]--;//����һ��
		}
		else// if (onoff > 0 )//��������
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
	//����������
	int ioSoftNum = g_ioN - m_ioNum;
	for (int i=0;i<ioSoftNum;i++)
	{
		if (m_ioSoft[i] > 0)//����С�ڵ���0 ���½��ش���
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
		sprintf(sz,"%f",m_MvScope[mtid].x);//�˶���Χ
		szPa += sz;szPa+=MVFLAG;
		sprintf(sz,"%f",m_MvScope[mtid].y);//�˶���Χ
		szPa += sz;szPa+=MVFLAG;
		short nEncOnOff = 0;
		iRes = m_handle.MC_GetEncOnOff(mtid+1, &nEncOnOff);
		sprintf(sz, "%d", nEncOnOff);//������ŷ�
		szPa += sz; szPa += MVFLAG;

		TTrapPrm TrapPrm;
		iRes += m_handle.MC_GetTrapPrm(mtid+1, &TrapPrm);
		TAxisHomePrm AxisHomePrm;
		iRes += m_handle.MC_HomeGetPrm(mtid+1, &AxisHomePrm);
		double vel = 0;
		iRes += m_handle.MC_GetVel(mtid + 1, &vel);//Ŀ���ٶ�
		if (0 != iRes)
		{
			m_Lock.unLock();
			return false;
		}

		sprintf(sz,"%.3f", TrapPrm.acc);//���ٶ�
		szPa += sz;szPa+=MVFLAG;
		sprintf(sz, "%.3f", TrapPrm.dec);//���ٶ�
		szPa += sz; szPa += MVFLAG;
		sprintf(sz, "%d", TrapPrm.smoothTime);//ƽ��ʱ��
		szPa += sz; szPa += MVFLAG;
		sprintf(sz, "%.3f", TrapPrm.velStart);//��ʼ�ٶ�
		szPa += sz; szPa += MVFLAG;
		sprintf(sz, "%.3f", vel);//�ٶ�
		szPa += sz; szPa += MVFLAG;
		//����
		sprintf(sz, "%d", AxisHomePrm.nHomeMode);//���㷽ʽ
		szPa += sz; szPa += MVFLAG;
		sprintf(sz, "%d", AxisHomePrm.nHomeDir);//���㷽��
		szPa += sz; szPa += MVFLAG;
		sprintf(sz, "%.3f", AxisHomePrm.dHomeRapidVel);//��������ٶ�
		szPa += sz; szPa += MVFLAG;
		sprintf(sz, "%.3f", AxisHomePrm.dHomeLocatVel);//���㶨λ�ٶ�
		szPa += sz; szPa += MVFLAG;
		sprintf(sz, "%.3f", AxisHomePrm.dHomeIndexVel);//����Ѱ��INDEX�ٶ�
		szPa += sz; szPa += MVFLAG;
		sprintf(sz, "%.3f", AxisHomePrm.dHomeAcc);//������ٶ�
		szPa += sz; szPa += MVFLAG;
		sprintf(sz, "%d", AxisHomePrm.lOffset);//����ƫ��
		szPa += sz; szPa += MVFLAG;

		sprintf(sz,"%f",m_fDebugSpeed[mtid]);//�����ٶ�
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
		if (""==szPa)//����Ĭ�ϲ���
		{
			/*char* szContent[] = { "���","������λ","������λ", \
		"���ٶ�","���ٶ�","ƽ��ʱ��","��ʼ�ٶ�","�ٶ�",\
		"���㷽ʽ","���㷽��","��������ٶ�","���㶨λ�ٶ�","����Ѱ��INDEX�ٶ�","������ٶ�","����ƫ��",\
		"�Ḻ����λ","��������λ","����","�ֶ��ٶ�" };*/
			for (int mtid=0;mtid<m_mtNum;mtid++)
			{
				m_MvScope[mtid] = cv::Point2f(m_vctDefaultData[0],m_vctDefaultData[1]);//�˶���Χ
				if (m_vctDefaultData[2]>0)
				{
					iRes = m_handle.MC_EncOn(mtid+1);//ʹ�ñ�����
				}
				else
				{
					iRes = m_handle.MC_EncOff(mtid+1);//���������ģʽ
				}
				TTrapPrm TrapPrm;
				TrapPrm.acc = m_vctDefaultData[3];//���ٶ�
				TrapPrm.dec = m_vctDefaultData[4];//���ٶ�
				TrapPrm.smoothTime = m_vctDefaultData[5];//ƽ��ʱ��
				TrapPrm.velStart = m_vctDefaultData[6];//��ʼ�ٶ�
				iRes = m_handle.MC_SetTrapPrm(mtid + 1, &TrapPrm);
				iRes += m_handle.MC_SetVel(mtid + 1, m_vctDefaultData[7]);//����Ŀ���ٶ�

				m_fDebugSpeed[mtid] = m_vctDefaultData[m_vctDefaultData.size()-1];//�����ٶ�
				m_fSpeed[mtid] = m_vctDefaultData[7];//�����ٶ�

				TAxisHomePrm AxisHomePrm;
				AxisHomePrm.nHomeMode = m_vctDefaultData[9];
				AxisHomePrm.nHomeDir = m_vctDefaultData[10];
				AxisHomePrm.dHomeRapidVel = m_vctDefaultData[11];
				AxisHomePrm.dHomeLocatVel = m_vctDefaultData[12];
				AxisHomePrm.dHomeIndexVel = m_vctDefaultData[13];
				AxisHomePrm.dHomeAcc = m_vctDefaultData[14];
				AxisHomePrm.lOffset = m_vctDefaultData[15];
				//������ 1 �������
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

			m_MvScope[mtid] = cv::Point2f(atof(command[0].c_str()),atof(command[1].c_str()));//�˶���Χ
			if (atoi(command[2].c_str()))
			{
				iRes = m_handle.MC_EncOn(mtid);//ʹ�ñ�����
			}
			else
			{
				iRes = m_handle.MC_EncOff(mtid);//���������ģʽ
			}
			TTrapPrm TrapPrm;
			TrapPrm.acc = atof(command[3].c_str());//���ٶ�
			TrapPrm.dec = atof(command[4].c_str());//���ٶ�
			TrapPrm.smoothTime = atof(command[5].c_str());//ƽ��ʱ��
			TrapPrm.velStart = atof(command[6].c_str());//��ʼ�ٶ�
			iRes = m_handle.MC_SetTrapPrm(mtid + 1, &TrapPrm);
			m_fSpeed[mtid] = atof(command[7].c_str());//�ٶ�
			iRes += m_handle.MC_SetVel(mtid + 1, m_fSpeed[mtid]);//�Զ��ٶ�
			m_fDebugSpeed[mtid] = atof(command[len - 1].c_str());//�����ٶ�

			TAxisHomePrm AxisHomePrm;
			AxisHomePrm.nHomeMode = atof(command[8].c_str());
			AxisHomePrm.nHomeDir = atof(command[9].c_str());
			AxisHomePrm.dHomeRapidVel = atof(command[10].c_str());
			AxisHomePrm.dHomeLocatVel = atof(command[11].c_str());
			AxisHomePrm.dHomeIndexVel = atof(command[12].c_str());
			AxisHomePrm.dHomeAcc = atof(command[13].c_str());
			AxisHomePrm.lOffset = atof(command[14].c_str());
			//������ 1 �������
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
	cfg.updateItem("\n#�˶����ƿ�ip","");
	cfg.updateItem("MVCardIP",m_linkAdd);
	
	vector<string> mvParam;
	getParam(mvParam);//��ȡ��ǰ�˶�����
	cfg.updateItem("\n#�����˶�������","");
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
	string szVal = cfg.getItem("MVCardIP");//����ip��ַ
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

//��ȡ��ǰλ����Ŀ�ĵ�λ�ò�
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
		if (-1==state || !m_bIni || mtid == -1)//ȷ��ֹͣ״̬
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