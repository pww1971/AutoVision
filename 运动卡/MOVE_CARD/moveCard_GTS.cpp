#include "stdafx.h"
#include "moveCard.h"
#include "automove.h"

#ifdef _GTS
CGTSCard::CGTSCard()
{
	m_mtNum = 16;
	m_ioNum = 24;
	for (int i = 0; i < m_mtNum; i++)
	{
		m_MvScope.push_back(cv::Point2f(0, 200));
		m_fSpeed.push_back(50);
		m_fDebugSpeed.push_back(10);
		m_iMvSpeed.push_back(50);
		m_mvScale.push_back(1);
		//		m_fUnitMM[i] = 1;
	}
	m_linkAdd = getExePath() + "GTS.cfg";
	m_szHelp = "�̸��˶��� 24����� 24��io��� 24��io�������� 24��128������io";
}

CGTSCard::~CGTSCard()
{
	release();
}
void CGTSCard::release()
{
	if (!m_bIni)
	{
		return;
	}
	m_Lock.lock();
	for (int i = 0; i < m_CardNum; i++)
	{
		for (int j = 0; j < m_mtNum; j++)
		{
			GT_Stop(i, 1 << j, 1);
			GT_AxisOff(i, j);
		}
		GT_Close(i);
	}
	
	m_bIni = false;
	m_Lock.unLock();
}

bool CGTSCard::ini(string linkAdd)
{
	if (m_bIni)
	{
		release();
	}
	m_Lock.lock();
	char* szContent[] = { "���","������λ","������λ","�ֶ��ٶ�", "�ٶ�","����","����ģʽ","���㷽��","�����ٶ�",\
		"������ٶ�","������ٶ�","�������","�������",\
		"��ʼ�ٶ�","����ٶ�","���ٶ�","���ٶ�","ֹͣ�ٶ�" };
	float defaultData[] = { -10,200,10,50,1,4,1,0, \
		10,50,0,0,\
		10.0,50.0,0.25 , 0.125, 0, };

	int commandLen = sizeof(szContent) / sizeof(char*);
	m_vctCfgName.clear();
	for (int i = 0; i < commandLen; i++)
	{
		m_vctCfgName.push_back(szContent[i]);
	}
	commandLen = sizeof(defaultData) / sizeof(float);
	m_vctDefaultData.clear();
	for (int i = 0; i < commandLen; i++)
	{
		m_vctDefaultData.push_back(defaultData[i]);
	}
		
	if (linkAdd != "")
	{
		m_linkAdd = linkAdd;
	}
	// Search card's ID
	m_CardNum=1;    //��ȡ����ʹ�õĿ����б�		
	v_total_axis = 0;
	//unsigned short TotalIn = 0, TotalOut = 0;
	for (int i = 0; i < m_CardNum; i++)
	{
		GT_Open(i);
	//	GT_LoadConfig(i,(char*)("GT"+CStrProcess::itoa_radixN(i)+".cfg").c_str());//���������ļ�
		//GT_ClrSts(i, axis, m_mtNum);
		for (int j = 0; j < m_mtNum; j++)
		{
			GT_AxisOn(i, j);
			GT_Stop(i, 1 << j, 0);
		}
		GT_ClrSts(i, 1, m_mtNum);//������ᱨ������λ
		v_first_axis_id[i] = v_total_axis;
		v_total_axis += m_mtNum;
		
		//nmc_get_total_ionum(m_CardList[i], &TotalIn, &TotalOut);
	}

	
	
	//�����ж���
	int ioTal = m_ioNum * m_CardNum;//�˶�����io������
	int ioSoftPos = ioTal;
	int ioSoftNum = g_ioN - ioSoftPos;
	if (ioSoftNum > 0)
	{
		if (m_ioSoft != NULL)
		{
			delete[] m_ioSoft;
		}
		m_ioSoft = new int[ioSoftNum];
		//�������������� 
		memset(m_ioSoft, 0, sizeof(int) * ioSoftNum);
	}

	m_bIni = true;

	m_Lock.unLock();
	return m_bIni;
}

bool  CGTSCard::writeMvCfg(CConfig& cfg)
{
	if (!m_bIni)
		return false;
	cfg.updateItem("\n#�����˶�������", "cfg�ļ�λ��");
	std::size_t found = m_linkAdd.find_last_of("/\\") + 1;
	cfg.updateItem("MVCardIP", m_linkAdd.substr(found, m_linkAdd.size()));

	vector<string> mvParam;
	getParam(mvParam);//��ȡ��ǰ�˶�����
	cfg.updateItem("\n#�����˶�������", "");
	int len = mvParam.size();
	cfg.updateItem("MoveParamNum", GDKT::CStrProcess::itoa_radixN(len));
	//		char sz[32];
	for (int i = 0; i < len; i++)
	{
		cfg.updateItem("MoveParam" + GDKT::CStrProcess::itoa_radixN(i), mvParam[i]);
		//			cfg.updateItem("MoveScope"+GDKT::CStrProcess::itoa_radixN(i),GDKT::CStrProcess::itoa_radixN(m_handle.m_iMvScope[i]));
		//			sprintf(sz,"%.4f",m_handle.m_fUnitMM[i]);
		//			cfg.updateItem("UnitMM"+GDKT::CStrProcess::itoa_radixN(i),sz);
	}
	return true;
}

bool  CGTSCard::readMvCfg(CConfig& cfg)
{
	string szVal = cfg.getItem("MVCardIP");//����ip��ַ
	if (szVal != "")
	{
		m_linkAdd = getExePath() + szVal;
	}
	if (ini(""))
	{
		//writeCard(szCfg);
		return true;
	}
	else
		return false;
}

void CGTSCard::Rapidstop()
{
	m_Lock.lock();
	for (int i = 0; i < m_CardNum; i++)
	{
		for (int j = 0; j < m_mtNum; j++)
			GT_Stop(i, 1 << j, 1);
	}
	m_Lock.unLock();
}

bool CGTSCard::moveAbs(int mtid, float pos)
{
	m_Lock.lock();
	if (!m_bIni || mtid == -1)
	{
		m_Lock.unLock();
		return false;
	}
	WORD cardid;
	getMtId(cardid, mtid);

	pos *= m_mvScale[mtid];

	// 2. Start  move
	short sRtn = GT_SetPos(cardid, mtid, pos);
	sRtn = GT_Update(cardid, (1 << mtid));
	if (sRtn != 0)
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

bool CGTSCard::move(int mtid, float dis)
{
	m_Lock.lock();
	if (!m_bIni || mtid == -1)
	{
		m_Lock.unLock();
		return false;
	}
	WORD cardid;
	getMtId(cardid, mtid);

	dis *= m_mvScale[mtid];

	double prfPos;
	short sRtn = GT_GetPrfPos(cardid, mtid, &prfPos);
	sRtn = GT_SetPos(cardid, mtid, dis+prfPos);
	sRtn = GT_Update(cardid, (1 << mtid));
	if (sRtn != 0)
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

bool CGTSCard::moveV(int mtid, int di)
{
	m_Lock.lock();
	if (!m_bIni || mtid == -1)
	{
		m_Lock.unLock();
		return false;
	}
	WORD cardid;
	int axid = mtid;
	getMtId(cardid, mtid);

	GT_PrfJog(cardid,mtid);
	TJogPrm jpm;
	GT_SetJogPrm(cardid, mtid, &jpm);//����jog�˶�����

	short return_code;
	if (di < 0)
		return_code = GT_SetVel(cardid, mtid, -m_fSpeed[axid]);//����Ŀ���ٶ�
	else
		return_code = GT_SetVel(cardid, mtid, m_fSpeed[axid]);//����Ŀ���ٶ�
	return_code = GT_Update(cardid, (1 << mtid));//�������˶�

	// 2. Start  move
	if (return_code != 0)
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

bool CGTSCard::moveStop(int mtid)
{
	m_Lock.lock();
	if (!m_bIni || mtid == -1)
	{
		m_Lock.unLock();
		return false;
	}
	WORD cardid;
	getMtId(cardid, mtid);
	// 2.
	short return_code = GT_Stop(cardid, (1 << mtid), 0);          //����ֹͣ 1��ͣ
	/*TTrapPrm pa;
	GT_SetTrapPrm(cardid, mtid, &pa);*/
	if (return_code != 0)
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

bool CGTSCard::mtZero(int mtid)
{
	m_Lock.lock();
	if (!m_bIni || mtid == -1)
	{
		m_Lock.unLock();
		return false;
	}
	WORD cardid;
	getMtId(cardid, mtid);

	//���� Smart Home ��ԭ�����
	THomePrm tHomePrm;
	GT_GetHomePrm(cardid, mtid, &tHomePrm);
	short return_code = GT_GoHome(cardid, mtid, &tHomePrm);
	if (return_code != 0)
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

void CGTSCard::getMtId(WORD& cardid, int& mtid)
{
	cardid = m_CardNum - 1;
	for (int i = cardid; i >= 0; i--)
	{
		if (mtid > v_first_axis_id[i])
		{
			mtid -= v_first_axis_id[i];
			cardid = i;
			break;
		}
	}
}

bool CGTSCard::setIO(int ioid, int onoff)
{
	if (ioid >= g_ioN)
	{
		return false;
	}
	m_Lock.lock();
	if (!m_bIni)
	{
		m_Lock.unLock();
		return false;
	}

	int ioTal = m_ioNum * m_CardNum;//�˶�����io������
	if (ioid > ioTal)
	{
		//�����ж���
		int ioSoftPos = ioTal;
		int ioSoftNum = g_ioN - ioSoftPos;
		if (ioSoftNum > 0 && ioid >= ioSoftPos)
		{
			if (onoff <= 0)
			{
				m_ioSoft[ioid - ioSoftPos]--;//����һ��
			}
			else// if (onoff > 0 )//��������
			{
				m_ioSoft[ioid - ioSoftPos] = onoff;
			}
		}
		m_Lock.unLock();
		return true;
	}
	else
	{
		int cardId = ioid / m_ioNum;
		ioid = ioid % m_ioNum;
		short return_code;
		if (onoff)
		{
			return_code = GT_SetDo(cardId, MC_GPO,(1<<ioid));
		}
		else
		{
			return_code = GT_SetDo(cardId, MC_GPO,~(1 << ioid));
		}
		if (return_code != 0)
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
}

bool CGTSCard::getIO(unsigned int state[])
{
	int ioTal = m_ioNum * m_CardNum;//�˶�����io������
	m_Lock.lock();
	if (!m_bIni)
	{
		m_Lock.unLock();
		return false;
	}
	//����������
	int ioSoftPos = ioTal;
	int ioSoftNum = g_ioN - ioSoftPos;
	for (int i = 0; i < ioSoftNum; i++)
	{
		if (m_ioSoft[i] > 0)//����С�ڵ���0 �ʹ���
			state[i + ioSoftPos] = 1;
		else
			state[i + ioSoftPos] = 0;
	}

	long lGpiValue;
	WORD carid, ioid;
	for (int i = 0; i < m_CardNum; i++)
	{
		carid = i;
		GT_GetDi(carid, MC_GPI, &lGpiValue);
		ioid = i * m_ioNum;
		for (int j = 0; j < m_ioNum; j++)
		{
			if (lGpiValue & (1 << j))
				state[j + ioid] = 1;
			else
				state[j + ioid] = 0;
		}
	}

	m_Lock.unLock();
	return true;
}

bool CGTSCard::getParam(vector<string>& vctPa)
{
	m_Lock.lock();
	if (!m_bIni)
	{
		m_Lock.unLock();
		return false;
	}
	vctPa.clear();
	char sz[32];
	
	//"����ģʽ","���㷽��"��"�����ٶ�"
	WORD home_mode=4,home_dir;
	double vel_mode,Low_Vel,High_Vel,Tacc,Tdec,Min_Vel,Max_Vel,Acc,Dec,stop_vel,equiv;
	int homeCom[] =
	{
		HOME_MODE_LIMIT,//��λ��ԭ��
		HOME_MODE_LIMIT_HOME,//��λ + Home��ԭ��
		HOME_MODE_LIMIT_INDEX,//��λ + Index��ԭ��
		HOME_MODE_LIMIT_HOME_INDEX,//��λ + Home + Index��ԭ��
		HOME_MODE_HOME,//Home��ԭ��
		HOME_MODE_HOME_INDEX,//Home + Index��ԭ��
		HOME_MODE_INDEX,//Index��ԭ��
		HOME_MODE_FORCED_HOME,//ǿ��Home��ԭ��
		HOME_MODE_FORCED_HOME_INDEX//ǿ��Home + Index��ԭ��
	};
	for (int i = 0; i < v_total_axis; i++)
	{
		string szPa = "";
		int axis_id = i;
		WORD cardid;
		getMtId(cardid, axis_id);
		//���嵱�� ����
		m_mvScale[i] = 1;
		sprintf(sz, "%.3f:%.3f", m_MvScope[i].x,m_MvScope[i].y); szPa += sz; szPa += MVFLAG;
		sprintf(sz, "%.3f", m_fDebugSpeed[i]); szPa += sz; szPa += MVFLAG;
		sprintf(sz, "%.3f", m_fSpeed[i]); szPa += sz; szPa += MVFLAG;
		sprintf(sz, "%d", m_mvScale[i]); szPa += sz; szPa += MVFLAG;
		//���㷽�� �ٶ�ģʽ ģʽ
		//���� ���� ����ʱ�� ����ʱ��
		THomePrm tHomePrm;
		GT_GetHomePrm(cardid, axis_id, &tHomePrm);
		for (int k=0;k<9;k++)
		{
			if (tHomePrm.mode == homeCom[k])
			{
				home_mode = k;
				break;
			}
		}
		
		home_dir = tHomePrm.moveDir;
		High_Vel = tHomePrm.velHigh;
		Low_Vel = tHomePrm.velLow;
		Tacc = tHomePrm.acc;
		Tdec = tHomePrm.dec;
		sprintf(sz, "%d:%d:%d:%.3f:%.3f:%.3f:%.3f:", home_mode, home_dir, vel_mode,  Low_Vel, High_Vel, Tacc, Tdec);//
		szPa += sz; 
		
		//��ʼ�ٶȣ�����ٶȣ����ٶȣ����ٶȣ�ֹͣ�ٶ�
		TrapPrm prm;
		GT_GetTrapPrm(cardid, axis_id, &prm);
		Acc = prm.acc;
		Dec = prm.dec;
		sprintf(sz, "%.3f:%.3f:%.3f:%.3f:%.3f:", Min_Vel,Max_Vel, Acc, Dec, stop_vel);
		szPa += sz; 
		
		vctPa.push_back(szPa);
	}
	
/*
	for (int i = 0; i < v_total_axis; i++)
	{
		string szPa = "";
		int axis_id = i + v_first_axis_id[0];
		sprintf(sz, "%.3f", m_MvScope[i].x); szPa += sz; szPa += MVFLAG;
		sprintf(sz, "%.3f", m_MvScope[i].y); szPa += sz; szPa += MVFLAG;
		sprintf(sz, "%.3f", m_fDebugSpeed[i]); szPa += sz; szPa += MVFLAG;
		sprintf(sz, "%.3f", m_fSpeed[i]); szPa += sz; szPa += MVFLAG;
		sprintf(sz, "%d", m_mvScale[i]); szPa += sz; szPa += MVFLAG;
		vctPa.push_back(szPa);
	}*/
	m_Lock.unLock();
	return true;
}

bool CGTSCard::setParam(string szPa)
{
	m_Lock.lock();
	if (!m_bIni)
	{
		m_Lock.unLock();
		return false;
	}
	int homeCom[] =
	{
		HOME_MODE_LIMIT,//��λ��ԭ��
		HOME_MODE_LIMIT_HOME,//��λ + Home��ԭ��
		HOME_MODE_LIMIT_INDEX,//��λ + Index��ԭ��
		HOME_MODE_LIMIT_HOME_INDEX,//��λ + Home + Index��ԭ��
		HOME_MODE_HOME,//Home��ԭ��
		HOME_MODE_HOME_INDEX,//Home + Index��ԭ��
		HOME_MODE_INDEX,//Index��ԭ��
		HOME_MODE_FORCED_HOME,//ǿ��Home��ԭ��
		HOME_MODE_FORCED_HOME_INDEX//ǿ��Home + Index��ԭ��
	};
	WORD home_mode, home_dir;
	double vel_mode, Low_Vel, High_Vel, Tacc, Tdec, Min_Vel, Max_Vel, Acc, Dec, stop_vel, equiv;
	if ("" == szPa)//����Ĭ�ϲ���
	{
		for (int i = 0; i < v_total_axis; i++)
		{
			string szPa = "";
			int axis_id = i;
			WORD cardid;
			getMtId(cardid, axis_id);
		
			m_MvScope[i] = cv::Point2f(m_vctDefaultData[0], m_vctDefaultData[1]);//�˶���Χ
			GT_SetSoftLimit(cardid, axis_id, m_MvScope[i].y, m_MvScope[i].x);
			m_fDebugSpeed[i] = m_vctDefaultData[2];//�ֶ��ٶ�
			m_iMvSpeed[i] = m_fSpeed[i] = m_vctDefaultData[3];//�˶��ٶ�
			m_mvScale[i] = m_vctDefaultData[4];//����
			GT_ProfileScale(cardid, axis_id, 1, m_mvScale[i]);//���嵱��
			home_mode = m_vctDefaultData[5]; 
			home_dir = m_vctDefaultData[6];
			vel_mode = m_vctDefaultData[7]; 
			Low_Vel = m_vctDefaultData[8];
			High_Vel = m_vctDefaultData[9];
			Tacc = m_vctDefaultData[10];
			Tdec = m_vctDefaultData[11];
			THomePrm tHomePrm;
			GT_GetHomePrm(cardid, axis_id, &tHomePrm);
			tHomePrm.mode = homeCom[home_mode];
			if (home_dir)
				tHomePrm.moveDir = 1;
			else
				tHomePrm.moveDir = -1;
			tHomePrm.indexDir = 1;
			tHomePrm.edge = 0;
			tHomePrm.velHigh = High_Vel;
			tHomePrm.velLow = Low_Vel;
			tHomePrm.acc = Tacc;
			tHomePrm.dec = Tdec;
			tHomePrm.searchHomeDistance = 200000;
			tHomePrm.searchIndexDistance = 30000;
			tHomePrm.escapeStep = 1000;
			GT_GoHome(cardid, axis_id, &tHomePrm);
			GT_Stop(cardid, axis_id, 1);

			Min_Vel = m_vctDefaultData[12];
			Max_Vel = m_vctDefaultData[13];
			Acc = m_vctDefaultData[14];
			Dec = m_vctDefaultData[15];
			stop_vel = m_vctDefaultData[16];
			//��ʼ�ٶȣ�����ٶȣ����ٶȣ����ٶȣ�ֹͣ�ٶ�
			TrapPrm prm;
			GT_GetTrapPrm(cardid, axis_id, &prm);
			prm.acc = Acc;
			prm.dec = Dec;

			GT_SetTrapPrm(cardid, axis_id, &prm);
		}
	}
	else
	{
		//		dmc_download_configfile(i,m_linkAdd.c_str());
		//GT_ClrSts(i, axis, m_mtNum);
		vector<string > cont;
		CStrProcess::Split(szPa, CHAINFLAG, cont, true);
		int len = cont.size();
		if (0 == len)
		{
			m_Lock.unLock();
			return false;
		}
		float val;
		v_total_axis = cont.size();
		for (int i = 0; i < v_total_axis; i++)
		{
			int axis_id = i;
			WORD cardid;
			getMtId(cardid, axis_id);

			vector<string> command;
			CStrProcess::Split(cont[i], MVFLAG, command, true);
			len = command.size();
			if (m_vctDefaultData.size() > len)
				continue;

			m_MvScope[i] = cv::Point2f(atof(command[0].c_str()), atof(command[1].c_str()));//�˶���Χ
			GT_SetSoftLimit(cardid, axis_id, m_MvScope[i].y, m_MvScope[i].x);
			m_fDebugSpeed[i] = atof(command[2].c_str());//�ֶ��ٶ�
			m_iMvSpeed[i] = m_fSpeed[i] = atof(command[3].c_str());//�˶��ٶ�
			m_mvScale[i] = atoi(command[4].c_str());//����
			GT_ProfileScale(cardid, axis_id, 1,m_mvScale[i]);//���嵱��

			home_mode = atoi(command[5].c_str());
			home_dir = atoi(command[6].c_str());
			vel_mode = atof(command[7].c_str());
			Low_Vel = atof(command[8].c_str());
			High_Vel = atof(command[9].c_str());
			Tacc = atof(command[10].c_str());
			Tdec = atof(command[11].c_str());

			THomePrm tHomePrm;
			GT_GetHomePrm(cardid, axis_id, &tHomePrm);
			tHomePrm.mode = homeCom[home_mode];
			if (home_dir)
				tHomePrm.moveDir = 1;
			else
				tHomePrm.moveDir = -1;
			tHomePrm.indexDir = 1;
			tHomePrm.edge = 0;
			tHomePrm.velHigh = High_Vel;
			tHomePrm.velLow = Low_Vel;
			tHomePrm.acc = Tacc;
			tHomePrm.dec = Tdec;
			tHomePrm.searchHomeDistance = 200000;
			tHomePrm.searchIndexDistance = 30000;
			tHomePrm.escapeStep = 1000;
			GT_GoHome(cardid, axis_id, &tHomePrm);
			GT_Stop(cardid, axis_id, 1);

			Min_Vel = atof(command[12].c_str());
			Max_Vel = atof(command[13].c_str());
			Acc = atof(command[14].c_str());
			Dec = atof(command[15].c_str());
			stop_vel = atof(command[16].c_str());
			//��ʼ�ٶȣ�����ٶȣ����ٶȣ����ٶȣ�ֹͣ�ٶ�
			TrapPrm prm;
			GT_GetTrapPrm(cardid, axis_id,&prm );
			prm.acc = Acc;
			prm.dec = Dec;
			
			GT_SetTrapPrm(cardid,axis_id,&prm);
		}
	}

	/*if ("" == szPa)//����Ĭ�ϲ���
	{
		//		APS_load_parameter_from_default(v_card_id);
		for (int i = 0; i < v_total_axis; i++)
		{
			int axis_id = i + v_first_axis_id[0];
			m_MvScope[i] = cv::Point2f(m_vctDefaultData[0], m_vctDefaultData[1]);//�˶���Χ
			m_fDebugSpeed[i] = m_vctDefaultData[2];//�ֶ��ٶ�
			m_iMvSpeed[i] = m_fSpeed[i] = m_vctDefaultData[3];//�˶��ٶ�
			m_mvScale[i] = m_vctDefaultData[4];//����
		}
	}
	else
	{
		
		//GT_LoadConfig(i,(char*)(getExePath()+"GT"+CStrProcess::itoa_radixN(i)+".cfg").c_str());//���������ļ�
		vector<string > cont;
		CStrProcess::Split(szPa, CHAINFLAG, cont, true);
		int len = cont.size();
		if (0 == len)
		{
			m_Lock.unLock();
			return false;
		}
		float val;
		v_total_axis = cont.size();
		for (int i = 0; i < v_total_axis; i++)
		{
			vector<string> command;
			CStrProcess::Split(cont[i], MVFLAG, command, true);
			len = command.size();
			if (5 > len)
				continue;

			m_MvScope[i] = cv::Point2f(atof(command[0].c_str()), atof(command[1].c_str()));//�˶���Χ
			m_fDebugSpeed[i] = atof(command[2].c_str());//�ֶ��ٶ�
			m_iMvSpeed[i] = m_fSpeed[i] = atof(command[3].c_str());//�˶��ٶ�
			m_mvScale[i] = atoi(command[4].c_str());//����
		}
	}
*/
	m_Lock.unLock();
	return true;
}

bool CGTSCard::getMtState(int mtid, int& state)
{
	m_Lock.lock();
	if (!m_bIni || mtid == -1)
	{
		m_Lock.unLock();
		return false;
	}
	WORD cardid;
	getMtId(cardid, mtid);

	long lAxisStatus; // ��״̬
	short ret = GT_GetSts(cardid, mtid, &lAxisStatus);           //�жϵ�ǰ��״̬
	// �ŷ�������־
	if (lAxisStatus & 0x2)
	{
		m_Lock.unLock();
		throw("�ŷ�����\n");
	}
	// �������Խ�ޱ�־
	if (lAxisStatus & 0x10)
	{
		m_Lock.unLock();
		throw("�˶�����\n");
	}
	// ������λ
	if (lAxisStatus & 0x20)
	{
		m_Lock.unLock();
		throw("����λ����\n");
	}
	// ������λ
	if (lAxisStatus & 0x40)
	{
		m_Lock.unLock();
		throw("����λ����\n");
	}
	// ƽ��ֹͣ
	/*if (lAxisStatus & 0x80)
	{
		printf("ƽ��ֹͣ����\n");
	}*/
	// ��ͣ��־
	if (lAxisStatus & 0x100)
	{
		m_Lock.unLock();
		throw("��ͣ����\n");
	}
	// �ŷ�ʹ�ܱ�־
	/*if (lAxisStatus & 0x200)
	{
		printf("�ŷ�ʹ��\n");
	}*/
	// �滮�������˶���־
	if (lAxisStatus & 0x400)
	{
		state = 0;//�˶���
		m_Lock.unLock();
		return false; //Now are in motion
		//printf("�滮�������˶�\n");
	}
	else
	{
		state = 1;
		m_Lock.unLock();
		return true; 
	}
}

bool CGTSCard::getMtPos(int mtid, float& pos)
{
	m_Lock.lock();
	if (!m_bIni || mtid == -1)
	{
		m_Lock.unLock();
		return false;
	}
	WORD cardid;
	getMtId(cardid, mtid);
	double position = 0;
	short iresult = GT_GetAxisPrfPos(cardid, mtid, &position);          //��ȡ��ǰ��λ��
	m_Lock.unLock();
	if (0 == iresult)
	{
		pos = position / m_mvScale[mtid];
		return true;
	}
	else
		return false;

}

//��ȡ��ǰλ����Ŀ�ĵ�λ�ò�
float CGTSCard::getDvPos(int mtid)
{
	if (!m_bIni)
		return false;

	m_Lock.lock();
	WORD cardid;
	getMtId(cardid, mtid);
	double position = 0;
	short iresult = GT_GetAxisPrfPos(cardid, mtid, &position);          //��ȡ��ǰ��λ��
	long ftemp2 = 0;
	iresult = GT_GetPos(cardid, mtid, &ftemp2);
	m_Lock.unLock();
	if (0 == iresult )
	{
		return fabs(position - ftemp2 / m_mvScale[mtid]);
	}
	else
		return FLT_MAX;
}

void CGTSCard::waitForStop(int mtid)
{
	int state = 0;
	//	float curPos= FLT_MAX;
	try
	{
		while (true)
		{
			if (!m_bIni || mtid == -1 || getMtState(mtid, state))//ȷ��ֹͣ״̬
			{
				/*m_handle.getMtPos( id, curPos);
				if (curPos==pos)*/
				break;
			}
		}
	}
	catch (char* e)
	{
		MessageBox(NULL, "����", e, MB_YESNO | MB_ICONQUESTION);
	}
}

void CGTSCard::setSpeed(bool debug)
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
		for (int i = 0; i < len; i++)
		{
			m_fSpeed[i] = m_fDebugSpeed[i];
			WORD cardid;
			int mtid = i;
			getMtId(cardid, mtid);
			GT_SetVel(cardid, mtid, m_fSpeed[i]);
		}
	}
	else
	{
		int len = m_fSpeed.size();
		for (int i = 0; i < len; i++)
		{
			m_fSpeed[i] = m_iMvSpeed[i];
			WORD cardid;
			int mtid = i;
			getMtId(cardid, mtid);
			GT_SetVel(cardid, mtid, m_fSpeed[i]);
		}
	}
	m_Lock.unLock();
}

#endif
