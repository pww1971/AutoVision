#include "stdafx.h"
#include "moveCard.h"
#include "automove.h"

#ifdef _LTD
CLTDCard::CLTDCard()
{
	m_mtNum = 16;
	m_ioNum = 24;
	for (int i = 0; i < m_mtNum; i++)
	{
		m_MvScope.push_back(cv::Point2f(0, 200));
		m_fSpeed.push_back(10000);
		m_fDebugSpeed.push_back(1000);
		m_iMvSpeed.push_back(10000);
		m_mvScale.push_back(1000);
		//		m_fUnitMM[i] = 1;
	}
	m_linkAdd = getExePath() + "LTD.ini";
	m_szHelp = "雷赛运动卡 24个电机 24个io输出 24个io触发输入 24至128是软触发io";
}

CLTDCard::~CLTDCard()
{
	release();
}
void CLTDCard::release()
{
	if (!m_bIni)
	{
		return;
	}
	m_Lock.lock();
	for (int i = 0; i < m_CardNum; i++)
	{
		dmc_emg_stop(m_CardList[i]);
	}
	//for (int i=0;i<m_CardNum;i++)
	//{
	//	dmc_cool_reset(m_CardList[i]);
	//}
	//for (int i = 0; i < 15; i++)//总线卡硬件复位耗时15s左右
	//{
	//	Sleep(1000);
	//}
	dmc_board_close();
	m_bIni = false;
	m_Lock.unLock();
}

bool CLTDCard::ini(string linkAdd)
{
	if (m_bIni)
	{
		release();
	}
	m_Lock.lock();
	char* szContent[] = { "轴号","负向限位","正向限位","手动速度", "速度","导承","回零模式","回零方向","回零速度",\
		"回零低速度","回零高速度","回零加速","回零减速",\
		"起始速度","最大速度","加速度","减速度","停止速度" };
	float defaultData[] = { -10,200,1000,10000,1000,0,1,0, \
		100000,50000,0,0,\
		10000.0,50000.0,10000.0 , 10000.0, 10000.0, };

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

	if (dmc_board_init() <= 0)      //控制卡的初始化操作
	{
		//MessageBox("初始化控制卡失败！", "出错");
		m_Lock.unLock();
		return m_bIni;
	}
	if (linkAdd != "")
	{
		m_linkAdd = linkAdd;
	}
	// Search card's ID
	DWORD CardTypeList[8];   //定义各卡类型
	dmc_get_CardInfList(&m_CardNum, CardTypeList, m_CardList);    //获取正在使用的卡号列表		
	v_total_axis = 0;
	//unsigned short TotalIn = 0, TotalOut = 0;
	for (int i = 0; i < m_CardNum; i++)
	{
		nmc_set_axis_enable(m_CardList[i], 255);
		v_first_axis_id[i] = v_total_axis;
		DWORD TotalAxis = 0;
		if (0 == dmc_get_total_axes(m_CardList[i], &TotalAxis))
			v_total_axis += TotalAxis;
		dmc_emg_stop(m_CardList[i]);
		//nmc_get_total_ionum(m_CardList[i], &TotalIn, &TotalOut);
	}

	
	
	//软触发中断器
	int ioTal = m_ioNum * m_CardNum;//运动卡的io口数量
	int ioSoftPos = ioTal;
	int ioSoftNum = g_ioN - ioSoftPos;
	if (ioSoftNum > 0)
	{
		if (m_ioSoft != NULL)
		{
			delete[] m_ioSoft;
		}
		m_ioSoft = new int[ioSoftNum];
		//软触发计数器清零 
		memset(m_ioSoft, 0, sizeof(int) * ioSoftNum);
	}

	m_bIni = true;

	m_Lock.unLock();
	return m_bIni;
}

bool  CLTDCard::writeMvCfg(CConfig& cfg)
{
	if (!m_bIni)
		return false;
	cfg.updateItem("\n#设置运动卡参数", "ini文件位置");
	std::size_t found = m_linkAdd.find_last_of("/\\") + 1;
	cfg.updateItem("MVCardIP", m_linkAdd.substr(found, m_linkAdd.size()));

	vector<string> mvParam;
	getParam(mvParam);//读取当前运动参数
	cfg.updateItem("\n#设置运动卡参数", "");
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

bool  CLTDCard::readMvCfg(CConfig& cfg)
{
	string szVal = cfg.getItem("MVCardIP");//连接ip地址
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

void CLTDCard::Rapidstop()
{
	m_Lock.lock();
	for (int i = 0; i < m_CardNum; i++)
	{
		dmc_emg_stop(m_CardList[i]);
	}
	m_Lock.unLock();
}

bool CLTDCard::moveAbs(int mtid, float pos)
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

	//if (dmc_check_done(cardid, mtid) == 0) //已经在运动中
	//{
	//	m_Lock.unLock();
	//	return false;
	//}
	// 2. Start  move
	short return_code = dmc_pmove(cardid, mtid, pos, 1);
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

bool CLTDCard::move(int mtid, float dis)
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

	//if (dmc_check_done(cardid, mtid) == 0) //已经在运动中
	//{
	//	m_Lock.unLock();
	//	return false;
	//}

	// 2. Start  move
	short return_code = dmc_pmove(cardid, mtid, dis, 0);
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

bool CLTDCard::moveV(int mtid, int di)
{
	m_Lock.lock();
	if (!m_bIni || mtid == -1)
	{
		m_Lock.unLock();
		return false;
	}
	WORD cardid;
	getMtId(cardid, mtid);

	//if (dmc_check_done(cardid, mtid) == 0) //已经在运动中
	//{
	//	m_Lock.unLock();
	//	return false;
	//}
	short return_code;
	if (di < 0)
		return_code = dmc_vmove(cardid, mtid, 0); // Set home direction
	else
		return_code = dmc_vmove(cardid, mtid, 1); // Set home direction

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

bool CLTDCard::moveStop(int mtid)
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
	short return_code = dmc_stop(cardid, mtid, 0);          //减速停止 1急停
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

bool CLTDCard::mtZero(int mtid)
{
	m_Lock.lock();
	if (!m_bIni || mtid == -1)
	{
		m_Lock.unLock();
		return false;
	}
	WORD cardid;
	getMtId(cardid, mtid);

	// 2. Start home move
	short return_code = dmc_home_move(cardid, mtid);//启动回零
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

void CLTDCard::getMtId(WORD& cardid, int& mtid)
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
	cardid = m_CardList[cardid];
}

bool CLTDCard::setIO(int ioid, int onoff)
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

	int ioTal = m_ioNum * m_CardNum;//运动卡的io口数量
	if (ioid > ioTal)
	{
		//软触发中断器
		int ioSoftPos = ioTal;
		int ioSoftNum = g_ioN - ioSoftPos;
		if (ioSoftNum > 0 && ioid >= ioSoftPos)
		{
			if (onoff <= 0)
			{
				m_ioSoft[ioid - ioSoftPos]--;//计数一次
			}
			else// if (onoff > 0 )//计数设置
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
		cardId = m_CardList[cardId];
		ioid = ioid % m_ioNum;
		short return_code;
		if (onoff)
		{
			return_code = dmc_write_outbit(cardId, ioid, 1);
		}
		else
		{
			return_code = dmc_write_outbit(cardId, ioid, 0);
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

bool CLTDCard::getIO(unsigned int state[])
{
	int ioTal = m_ioNum * m_CardNum;//运动卡的io口数量
	m_Lock.lock();
	if (!m_bIni)
	{
		m_Lock.unLock();
		return false;
	}
	//软触发计数器
	int ioSoftPos = ioTal;
	int ioSoftNum = g_ioN - ioSoftPos;
	for (int i = 0; i < ioSoftNum; i++)
	{
		if (m_ioSoft[i] > 0)//次数小于等于0 就触发
			state[i + ioSoftPos] = 1;
		else
			state[i + ioSoftPos] = 0;
	}

	WORD carid, ioid;
	for (int i = 0; i < m_CardNum; i++)
	{
		carid = m_CardList[i];
		ioid = i * m_ioNum;
		for (int j = 0; j < m_ioNum; j++)
		{
			state[j + ioid] = dmc_read_inbit(carid, j);
		}
	}

	m_Lock.unLock();
	return true;
}

bool CLTDCard::getParam(vector<string>& vctPa)
{
	m_Lock.lock();
	if (!m_bIni)
	{
		m_Lock.unLock();
		return false;
	}
	vctPa.clear();
	char sz[32];
	
	//"归零模式","归零方向"，"归零速度"
	WORD home_mode,home_dir;
	double vel_mode,Low_Vel,High_Vel,Tacc,Tdec,Min_Vel,Max_Vel,Acc,Dec,stop_vel,equiv;

	for (int i = 0; i < v_total_axis; i++)
	{
		string szPa = "";
		int axis_id = i;
		WORD cardid;
		getMtId(cardid, axis_id);
		dmc_get_equiv(cardid, axis_id, &equiv);//脉冲当量 导承
		m_mvScale[i] = equiv;
		sprintf(sz, "%.3f:%.3f", m_MvScope[i].x,m_MvScope[i].y); szPa += sz; szPa += MVFLAG;
		sprintf(sz, "%.3f", m_fDebugSpeed[i]); szPa += sz; szPa += MVFLAG;
		sprintf(sz, "%.3f", m_fSpeed[i]); szPa += sz; szPa += MVFLAG;
		sprintf(sz, "%d", m_mvScale[i]); szPa += sz; szPa += MVFLAG;
		
		short return_code = dmc_get_homemode(cardid, axis_id, &home_dir, &vel_mode, &home_mode, 0);//归零方向 速度模式 模式
		
		dmc_get_home_profile_unit(cardid, axis_id, &Low_Vel, &High_Vel, &Tacc, &Tdec);//低速 高速 加速时间 减速时间
		//short return_code = nmc_get_home_profile(cardid, axis_id, &home_mode[i], &Low_Vel[i], &High_Vel[i], &Tacc[i], &Tdec[i], &offsetpos[i]);
		sprintf(sz, "%d:%d:%d:%.3f:%.3f:%.3f:%.3f:", home_mode, home_dir, vel_mode,  Low_Vel, High_Vel, Tacc, Tdec);//
		szPa += sz; 
		
		dmc_get_acc_profile(cardid, axis_id, &Min_Vel, &Max_Vel, &Acc, &Dec, &stop_vel);//起始速度，最大速度，加速度，减速度，停止速度
			
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

bool CLTDCard::setParam(string szPa)
{
	m_Lock.lock();
	if (!m_bIni)
	{
		m_Lock.unLock();
		return false;
	}
	WORD home_mode, home_dir;
	double vel_mode, Low_Vel, High_Vel, Tacc, Tdec, Min_Vel, Max_Vel, Acc, Dec, stop_vel, equiv;
	if ("" == szPa)//加载默认参数
	{
		for (int i = 0; i < v_total_axis; i++)
		{
			string szPa = "";
			int axis_id = i;
			WORD cardid;
			getMtId(cardid, axis_id);
		
			m_MvScope[i] = cv::Point2f(m_vctDefaultData[0], m_vctDefaultData[1]);//运动范围
			m_fDebugSpeed[i] = m_vctDefaultData[2];//手动速度
			m_iMvSpeed[i] = m_fSpeed[i] = m_vctDefaultData[3];//运动速度
			m_mvScale[i] = m_vctDefaultData[4];//承载
			dmc_set_equiv(cardid, axis_id, m_mvScale[i]);//脉冲当量
			home_mode = m_vctDefaultData[5]; 
			home_dir = m_vctDefaultData[6];
			vel_mode = m_vctDefaultData[7]; 
			short return_code = dmc_set_homemode(cardid, axis_id, home_dir, vel_mode, home_mode, 0);//归零方向 速度模式 模式
			Low_Vel = m_vctDefaultData[8];
			High_Vel = m_vctDefaultData[9];
			Tacc = m_vctDefaultData[10];
			Tdec = m_vctDefaultData[11];
			dmc_set_home_profile_unit(cardid, axis_id, Low_Vel, High_Vel, Tacc, Tdec);//低速 高速 加速时间 减速时间
			//short return_code = nmc_set_home_profile(cardid, axis_id, &home_mode[i], &Low_Vel[i], &High_Vel[i], &Tacc[i], &Tdec[i], &offsetpos[i]);
			Min_Vel = m_vctDefaultData[12];
			Max_Vel = m_vctDefaultData[13];
			Acc = m_vctDefaultData[14];
			Dec = m_vctDefaultData[15];
			stop_vel = m_vctDefaultData[16];
			dmc_set_acc_profile(cardid, axis_id, Min_Vel, Max_Vel, Acc, Dec, stop_vel);//起始速度，最大速度，加速度，减速度，停止速度

		}
	}
	else
	{
		//		dmc_download_configfile(m_CardList[i],m_linkAdd.c_str());
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

			m_MvScope[i] = cv::Point2f(atof(command[0].c_str()), atof(command[1].c_str()));//运动范围
			m_fDebugSpeed[i] = atof(command[2].c_str());//手动速度
			m_iMvSpeed[i] = m_fSpeed[i] = atof(command[3].c_str());//运动速度
			m_mvScale[i] = atoi(command[4].c_str());//承载
			dmc_set_equiv(cardid, axis_id, m_mvScale[i]);//脉冲当量

			home_mode = atoi(command[5].c_str());
			home_dir = atoi(command[6].c_str());
			vel_mode = atof(command[7].c_str());
			short return_code = dmc_set_homemode(cardid, axis_id, home_dir, vel_mode, home_mode, 0);//归零方向 速度模式 模式
			Low_Vel = atof(command[8].c_str());
			High_Vel = atof(command[9].c_str());
			Tacc = atof(command[10].c_str());
			Tdec = atof(command[11].c_str());
			dmc_set_home_profile_unit(cardid, axis_id, Low_Vel, High_Vel, Tacc, Tdec);//低速 高速 加速时间 减速时间
			//short return_code = nmc_set_home_profile(cardid, axis_id, &home_mode[i], &Low_Vel[i], &High_Vel[i], &Tacc[i], &Tdec[i], &offsetpos[i]);
			Min_Vel = atof(command[12].c_str());
			Max_Vel = atof(command[13].c_str());
			Acc = atof(command[14].c_str());
			Dec = atof(command[15].c_str());
			stop_vel = atof(command[16].c_str());
			dmc_set_acc_profile(cardid, axis_id, Min_Vel, Max_Vel, Acc, Dec, stop_vel);//起始速度，最大速度，加速度，减速度，停止速度

		}
	}

	/*if ("" == szPa)//加载默认参数
	{
		//		APS_load_parameter_from_default(v_card_id);
		for (int i = 0; i < v_total_axis; i++)
		{
			int axis_id = i + v_first_axis_id[0];
			m_MvScope[i] = cv::Point2f(m_vctDefaultData[0], m_vctDefaultData[1]);//运动范围
			m_fDebugSpeed[i] = m_vctDefaultData[2];//手动速度
			m_iMvSpeed[i] = m_fSpeed[i] = m_vctDefaultData[3];//运动速度
			m_mvScale[i] = m_vctDefaultData[4];//导承
		}
	}
	else
	{
		//		APS_load_param_from_file( m_linkAdd.c_str() );
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

			m_MvScope[i] = cv::Point2f(atof(command[0].c_str()), atof(command[1].c_str()));//运动范围
			m_fDebugSpeed[i] = atof(command[2].c_str());//手动速度
			m_iMvSpeed[i] = m_fSpeed[i] = atof(command[3].c_str());//运动速度
			m_mvScale[i] = atoi(command[4].c_str());//导承
		}
	}
*/
	m_Lock.unLock();
	return true;
}

bool CLTDCard::getMtState(int mtid, int& state)
{
	m_Lock.lock();
	if (!m_bIni || mtid == -1)
	{
		m_Lock.unLock();
		return false;
	}
	WORD cardid;
	getMtId(cardid, mtid);

	/*unsigned short errcode = 1;
	nmc_get_axis_errcode(cardid, mtid, &errcode);
	if (errcode == 0)
	{
	}
	else
	{
		m_Lock.unLock();
		throw("总线状态：EtherCAT总线出错");
		return false;
	}

	unsigned short Axis_State_machine = 0;
	nmc_get_axis_state_machine(cardid, mtid, &Axis_State_machine);
	switch (Axis_State_machine)// 读取指定轴状态机
	{
	case 0:
		//("轴处于未启动状态");
		break;
	case 1:
		m_Lock.unLock();
		throw("轴处于启动禁止状态");
		return false;
	case 2:
		//("轴处于准备启动状态");
		break;
	case 3:
		//("轴处于启动状态");
		break;
	case 4:
		//("轴处于操作使能状态");
		break;
	case 5:
		//("轴处于停止状态");
		break;
	case 6:
		m_Lock.unLock();
		throw("轴处于错误触发状态");
		return false;
	case 7:
		m_Lock.unLock();
		throw("轴处于错误状态");
		return false;
	default:
		break;
	};
	*/
	DWORD status = dmc_check_done(cardid, mtid);           //判断当前轴状态
	if (status == 1)
	{
		state = -1;
		long stopreason;
		dmc_get_stop_reason(cardid, mtid,&stopreason);
		switch (stopreason)// 读取指定轴状态机
		{
		case 0://正常停止
		case 3://LTC 外部触发立即停止，IMD_STOP_AT_LTC
		case 13://命令立即停止，IMD_STOP_AT_CMD
		case 14://命令减速停止，DEC_STOP_AT_CMD
		case 19://外部 IO 减速停止，DEC_STOP_AT_DEC
			break;
		case 1://ALM 立即停止，IMD_STOP_AT_ALM
		case 2://ALM 减速停止，DEC_STOP_AT_ALM
		case 4://EMG 立即停止，IMD_STOP_AT_EMG
		case 5://正硬限位立即停止，IMD_STOP_AT_ELP
		case 6://负硬限位立即停止，IMD_STOP_AT_ELN
		case 7://正硬限位减速停止，DEC_STOP_AT_ELP
		case 8://负硬限位减速停止，DEC_STOP_AT_ELN
		case 9://正软限位立即停止，IMD_STOP_AT_SOFT_ELP
		case 10://负软限位立即停止，IMD_STOP_AT_SOFT_ELN
		case 11://正软限位减速停止，DEC_STOP_AT_SOFT_ELP
		case 12://负软限位减速停止，DEC_STOP_AT_SOFT_ELN
		case 15://其它原因立即停止，IMD_STOP_AT_OTHER
		case 16://其它原因减速停止，DEC_STOP_AT_OTHER
		case 17://未知原因立即停止，IMD_STOP_AT_UNKOWN
		case 18://未知原因减速停止，DEC_STOP_AT_UNKOWN
			m_Lock.unLock();
			throw("急停限位或伺服报警");
			return false;
		}
		m_Lock.unLock();
		return true; 
	}
	else
	{
		state = 0;//运动中
		m_Lock.unLock();
		return false; //Now are in motion
	}
}

bool CLTDCard::getMtPos(int mtid, float& pos)
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
	short iresult = dmc_get_position_unit(cardid, mtid, &position);          //获取当前轴位置
	m_Lock.unLock();
	if (0 == iresult)
	{
		pos = position / m_mvScale[mtid];
		return true;
	}
	else
		return false;

}

//获取当前位置与目的的位置差
float CLTDCard::getDvPos(int mtid)
{
	if (!m_bIni)
		return false;

	m_Lock.lock();
	WORD cardid;
	getMtId(cardid, mtid);
	double position = 0;
	short iresult = dmc_get_position_unit(cardid, mtid, &position);          //获取当前轴位置
	double ftemp2 = dmc_get_target_position(cardid, mtid);
	m_Lock.unLock();
	if (0 == iresult )
	{
		return fabs(position - ftemp2 / m_mvScale[mtid]);
	}
	else
		return FLT_MAX;
}

void CLTDCard::waitForStop(int mtid)
{
	int state = 0;
	//	float curPos= FLT_MAX;
	try
	{
		while (true)
		{
			if (!m_bIni || mtid == -1 || getMtState(mtid, state))//确认停止状态
			{
				/*m_handle.getMtPos( id, curPos);
				if (curPos==pos)*/
				break;
			}
		}
	}
	catch (char* e)
	{
		MessageBox(NULL, "错误", e, MB_YESNO | MB_ICONQUESTION);
	}
}

void CLTDCard::setSpeed(bool debug)
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
			dmc_change_speed_unit(cardid, mtid, m_fSpeed[i], 0);
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
			dmc_change_speed_unit(cardid, mtid, m_fSpeed[i], 0);
		}
	}
	m_Lock.unLock();
}

#endif
