#include "stdafx.h"
#include "moveCard.h"

#ifdef _ZMC 
CMoveCard::CMoveCard()
{
	m_mtNum = 16;
	m_ioNum = 16;
	for (int i=0;i<m_mtNum;i++)
	{
		m_MvScope.push_back(cv::Point2f(0,200));
		m_fSpeed.push_back(1000);
		m_fDebugSpeed.push_back(1000);
		m_fZeroMode.push_back(4);
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
	char* szContent[] = {"���","������λ","������λ","������ģʽ", "s����",\
		"���嵱��","���ٶ�","���ٶ�","�ٶ�","���㷴���ٶ�","��ʼ�ٶ�","�Ḻ����λ","��������λ",\
		"����ģʽ","����","�����ٶ�","�ֶ��ٶ�"};
	int commandLen = sizeof(szContent)/sizeof(char*);
	m_vctCfgName.clear();
	for (int i=0;i<commandLen;i++)
	{
		m_vctCfgName.push_back(szContent[i]);
	}
	float defaultData[] = {-100,200,7,0,	\
		1000,3000,3000,100,10,3,1000,1000,			\
		4,1000,1000,1000};
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
		//	MessageBox(_T("����ʧ��"));
		m_Lock.unLock();
		return false;
	}
	ZAux_Rapidstop(m_handle,2);

	// ����ģʽ����
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
	//�趨��Ӧ���ԭ��������ź�
	bool ret = (ERR_SUCCESS == ZAux_Singl_Datum(m_handle,mtid, m_fZeroMode[mtid]));
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
	//����������
	int ioSoftNum = g_ioN - m_ioNum;
	for (int i=0;i<ioSoftNum;i++)
	{
		if (m_ioSoft[i] > 0)//����С�ڵ���0 ���½��ش���
			state[i+m_ioNum] = 1;
		else
			state[i+m_ioNum] = 0;
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
	/*static string szContent[] = {"������ģʽ��0-������ 1-���巽ʽ�������ŷ� 7-���巽ʽ�������ŷ�+EZ�ź�����", "s���ߣ�0��ʾ���μӼ���",\
	"���嵱��","���ٶ�units/s/s","���ٶ�","�ٶ�","���λ��㷴���ٶ�","��ʼ�ٶ�",\
	"����ģʽ","����","�����ٶ�","�ֶ��ٶ�"};*/
	static char* szCommand[] = {"atype(%d)","sramp(%d)","units(%d)","accel(%d)","decel(%d)","speed(%d)","creep(%d)","lspeed(%d)",\
		"fs_limit(%d)","rs_limit(%d)"};
	int commandLen = sizeof(szCommand)/sizeof(char*);
	float val;
	char sz[32];
	vctPa.clear();
	for (int mtid=0;mtid<m_mtNum;mtid++)
	{
		string szPa="";
		sprintf(sz,"%f",m_MvScope[mtid].x);//�˶���Χ
		szPa += sz;szPa+=MVFLAG;
		sprintf(sz,"%f",m_MvScope[mtid].y);//�˶���Χ
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
		//����ģʽ
		sprintf(sz, "%f", m_fZeroMode[mtid]);
		szPa += sz; szPa += MVFLAG;
		//����
		szPa += "1000";szPa+=MVFLAG;
		//�����ٶ�
		szPa += "1000";szPa+=MVFLAG;
		sprintf(sz,"%f",m_fDebugSpeed[mtid]);//�����ٶ�
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
	static ZAux_Set_i movefun1[] = {NULL,NULL,ZAux_SetAtype,ZAux_SetSramp};
	static ZAux_Set_f movefun2[] = {ZAux_SetUnits,ZAux_SetAccel,ZAux_SetDecel,ZAux_SetSpeed,ZAux_SetCreepSpeed,ZAux_SetLSpeed/*,ZAux_Direct_SetFsLimit,ZAux_Direct_SetRsLimit*/};
	int funLen1 = sizeof(movefun1)/sizeof(ZAux_Set_i);
	int funLen2 = sizeof(movefun2)/sizeof(ZAux_Set_f) + funLen1;
	try
	{
		if (""==szPa)//����Ĭ�ϲ���
		{
			int zeroModel = m_vctDefaultData[12];
			for (int mtid=0;mtid<m_mtNum;mtid++)
			{
				m_MvScope[mtid] = cv::Point2f(m_vctDefaultData[0],m_vctDefaultData[1]);//�˶���Χ
				int i=2;
				for (;i<funLen1;i++)
				{
					movefun1[i](m_handle,mtid,m_vctDefaultData[i]);
				}
				for (;i<funLen2;i++)
				{
					movefun2[i-funLen1](m_handle,mtid,m_vctDefaultData[i]);
				}
				m_fZeroMode[mtid]= zeroModel;
				m_fDebugSpeed[mtid] = m_vctDefaultData[m_vctDefaultData.size()-1];//�����ٶ�
				m_fSpeed[mtid] = m_vctDefaultData[7];//�����ٶ�
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

			m_MvScope[mtid] = cv::Point2f(atof(command[0].c_str()),atof(command[1].c_str()));//�˶���Χ
			int i=2;
			for (;i<funLen1;i++)
			{
				val = atof(command[i].c_str());
				movefun1[i](m_handle,mtid,val);
			}
			for (;i<funLen2;i++)
			{
				val = atof(command[i].c_str());
				movefun2[i-funLen1](m_handle,mtid,val);

				//�������嵱��	1��ʾ��һ������Ϊ��λ ������Ϊ1MM������������������λΪMM
				//return ERR_SUCCESS == ZAux_SetUnits(m_handle,mtid,value);
			}

			if (len>14)
				m_fZeroMode[mtid] = atoi(command[12].c_str());
			m_fDebugSpeed[mtid] = atof(command[len-1].c_str());//�����ٶ�
			m_fSpeed[mtid] = atof(command[7].c_str());//�Զ��ٶ�
			//mtZero(mtid);//���ȿ϶��ǹ�λ
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

bool CMoveCard::readMvCfg(CConfig& cfg)
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

//��ȡ��ǰλ����Ŀ�ĵ�λ�ò�
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
		if (-1==state || !m_bIni || mtid == -1)//ȷ��ֹͣ״̬
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

#endif