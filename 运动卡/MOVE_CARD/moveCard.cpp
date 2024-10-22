#include "stdafx.h"
#include "moveCard.h"
#include "automove.h"
CThreadMgr g_threadMgr;
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
	m_mtNum = 4;
	m_ioNum = 16;
#ifdef _SIMULATION
	m_MtRunning = NULL;
#endif
	m_ioSoft = NULL;
	m_bIni = false;
	m_szHelp = CST_Word401;
}
CMoveType::~CMoveType()
{
	if (m_ioSoft != NULL)
	{
		delete[] m_ioSoft;
	}
#ifdef _SIMULATION
	m_ontime.~CClkOnTimer();
	delete[] m_MtRunning;//�˶���ʱ �����˶�����
	m_MtRunning = NULL;
#endif
}
bool CMoveType::ini(string linkAdd)
{
	CConfig cfg(getExePath() + MOVECFG);
	string szVal = cfg.getItem("CurDir");
	if (_access(szVal.c_str(), 0) != -1)
	{
		CConfig cfg2(szVal + "MoveCfg");
		string szVal2 = cfg2.getItem("ioNum");
		if (szVal2 != "")
		{
			m_ioNum = atoi(szVal2.c_str());
		}
		szVal2 = cfg2.getItem("MoveParamNum");
		if (szVal2 != "")
		{
			m_mtNum = atoi(szVal2.c_str());
		}
	}

	if (m_MvScope.size()==0)
	{
		for (int i=0;i<m_mtNum;i++)
		{
			m_MvScope.push_back(cv::Point2f(-100,200));
			m_fSpeed.push_back(1000);
			m_fDebugSpeed.push_back(1000);
			//		m_fUnitMM[i] = 1;
		}
	}
#ifdef _SIMULATION
	m_MtRunning = new S_Click[m_mtNum];
	for (int i = 0; i < m_mtNum; i++)
		m_ontime.pushMsg(&m_MtRunning[i]);
	m_ontime.open();
#endif
	char* szContent[] = { CST_AxisID,CST_NegLimit,CST_PosLimit,CST_AxisPulseMode, CST_SCurve,\
		CST_PulseEquivalent, CST_Acc, CST_Dec, CST_Speed, CST_ZeroReverSpeed, CST_StartSpeed,\
		CST_ZeroMode, CST_BearGuide, CST_ZeroSpeed, CST_ManualSpeed };
	int commandLen = sizeof(szContent)/sizeof(char*);
	m_vctCfgName.clear();
	for (int i=0;i<commandLen;i++)
	{
		m_vctCfgName.push_back(szContent[i]);
	}
	float defaultData[] = {-100,200,7,0,	\
		1000,3000,3000,100,10,3,			\
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
bool CMoveType::moveAbs(int mtid,float pos)//�����˶�
{
	float curp = 0;
	getMtPos(mtid,curp);
#ifdef _SIMULATION
	m_MtRunning[mtid].setTime(fabs(pos-curp) * 10);
#endif
	return true;
}
bool CMoveType::move(int mtid,float dis)//����˶�
{
#ifdef _SIMULATION
	m_MtRunning[mtid].setTime(fabs(dis) * 10);
#endif
	
	return true;
}
bool CMoveType::moveV(int mtid,int di)//�����˶� di����1 ����-1
{
#ifdef _SIMULATION
	m_MtRunning[mtid].setTime(MAXINT);
#endif
	return true;
}
bool CMoveType::moveStop(int mtid)//��������˶�
{
#ifdef _SIMULATION
	m_MtRunning[mtid].zeroSate();
#endif
	return true;
}
bool CMoveType::mtZero(int mtid)//����
{
#ifdef _SIMULATION
	m_MtRunning[mtid].setTime(2000);//2��
#endif
	return true;
}
bool CMoveType::setIO(int ioid,int onoff)//����io�����
{
	sleep(100);
	m_Lock.lock();
	//�����ж���
	int ioSoftPos = m_ioNum;
	int ioSoftNum = g_ioN - ioSoftPos;
	if (ioSoftNum>0 && ioid >= ioSoftPos)
	{
		if (onoff<=0)
		{
			m_ioSoft[ioid-ioSoftPos]--;//����һ��
		}
		else// if (onoff > 0 )//��������
		{
			m_ioSoft[ioid-ioSoftPos]=onoff;
		}
		
	}
	m_Lock.unLock();
	return true;
}
bool CMoveType::getIO(uint state[])//��ȡ����io�ڵ�״̬
{
	sleep(100);
	m_Lock.lock();
	//����������
	int ioSoftPos = m_ioNum;
	int ioSoftNum = g_ioN - ioSoftPos;
	for (int i=0;i<ioSoftNum;i++)
	{
		if (m_ioSoft[i] > 0)//����С�ڵ���0 ���½��ش���
			state[i+ioSoftPos] = 1;
		else
			state[i+ioSoftPos] = 0;
	}
	m_Lock.unLock();
	return true;
}
bool CMoveType::zeroIO()//��ʼ��io�������λ
{
	int ioSoftPos = m_ioNum;
	int ioSoftNum = g_ioN - ioSoftPos;
	memset(m_ioSoft,0,sizeof(int)*ioSoftNum);
	sleep(100);
	return true;
}
bool CMoveType::getMtState(int mtid,int& state)	//��������state=0�˶��� -1û���˶�
{
#ifdef _SIMULATION
	state = m_MtRunning[mtid].getSate();
#endif
	return true;
}
bool CMoveType::getMtPos(int mtid,float& pos)		//�����λ��
{
#ifdef _SIMULATION
	pos = m_MtRunning[mtid].getCurPos();
#endif
	return true;
}
bool CMoveType::getParam(std::vector<string>& vctPa)		//��ȡ��ǰ����
{
	m_Lock.lock();
	if (!m_bIni)
	{
		m_Lock.unLock();
		return false;
	}	
	float val;
	char sz[32];
	vctPa.clear();
	for (int mtid = 0; mtid < m_mtNum; mtid++)
	{
		string szPa = "";
		sprintf(sz, "%f", m_MvScope[mtid].x);//�˶���Χ
		szPa += sz; szPa += MVFLAG;
		sprintf(sz, "%f", m_MvScope[mtid].y);//�˶���Χ
		szPa += sz; szPa += MVFLAG;
		
		//����
		szPa += "1000"; szPa += MVFLAG;
		//�ٶ�
		sprintf(sz, "%f", m_fSpeed[mtid]);//�����ٶ�
		szPa += sz; szPa += MVFLAG;
		sprintf(sz, "%f", m_fDebugSpeed[mtid]);//�����ٶ�
		szPa += sz; szPa += MVFLAG;
		vctPa.push_back(szPa);
	}

	m_Lock.unLock();
	return true;
}
bool CMoveType::setParam(string szPa)		//��������
{
	m_Lock.lock();
	if (!m_bIni)
	{
		m_Lock.unLock();
		return false;
	}
	try
	{
		if ("" == szPa)//����Ĭ�ϲ���
		{
			int zeroModel = m_vctDefaultData[12];
			for (int mtid = 0; mtid < m_mtNum; mtid++)
			{
				m_MvScope[mtid] = cv::Point2f(m_vctDefaultData[0], m_vctDefaultData[1]);//�˶���Χ
				m_fDebugSpeed[mtid] = m_vctDefaultData[m_vctDefaultData.size() - 1];//�����ٶ�
				m_fSpeed[mtid] = m_vctDefaultData[7];//�����ٶ�
			}
			m_Lock.unLock();
			return true;
		}
		std::vector<string > cont;
		CStrProcess::Split(szPa, CHAINFLAG, cont, true);
		int len = cont.size();
		if (0 == len)
		{
			m_Lock.unLock();
			return false;
		}
		float val;
		m_mtNum = cont.size();
		m_MvScope.resize(m_mtNum);
		m_fDebugSpeed.resize(m_mtNum); 
		m_fSpeed.resize(m_mtNum);
		for (int mtid = 0; mtid < m_mtNum; mtid++)
		{
			std::vector<string> command;
			CStrProcess::Split(cont[mtid], MVFLAG, command, true);
			len = command.size();
			m_MvScope[mtid] = cv::Point2f(atof(command[0].c_str()), atof(command[1].c_str()));//�˶���Χ
			m_fDebugSpeed[mtid] = atof(command[4].c_str());//�����ٶ�
			m_fSpeed[mtid] = atof(command[3].c_str());//�Զ��ٶ�
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
void CMoveType::waitForStop(int mtid)
{
	sleep(100);
}
//��ȡ��ǰλ����Ŀ�ĵ�λ�ò�
float CMoveType::getDvPos(int iaxis)
{
	sleep(100);
	return true;
}
//�ж�ip�ַ��ĸ�ʽ
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
