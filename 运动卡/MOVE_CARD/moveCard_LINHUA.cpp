#include "stdafx.h"
#include "moveCard.h"
#include "automove.h"
#define LOADXML

#ifdef _LINHUA
CLinHuaCard::CLinHuaCard()
{
	m_mtNum = 24;
	m_ioNum = 24;
	for (int i=0;i<m_mtNum;i++)
	{
		m_MvScope.push_back(cv::Point2f(0,200));
		m_fSpeed.push_back(10000);
		m_fDebugSpeed.push_back(1000);
		m_iMvSpeed.push_back(10000);
		m_mvScale.push_back(1000);
		//		m_fUnitMM[i] = 1;
	}
	m_linkAdd = getExePath()+"linghua.xml";
	m_szHelp = "凌华运动卡 24个电机 24个io输出 24个io触发输入 24至128是软触发io";
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
	char* szContent[] = {"轴号","负向限位","正向限位","手动速度","速度","导承"/*,"归零模式","归零方向","EZ对准",\
		"T曲线","归位加速度","归位最大速度","归位VO速度","归位回移距离","归位归位位置",\
		"停减速","加速曲线","加速度","减速度"*/};
	float defaultData[] = {-10,200,1000,10000,1000/*,0,1,0, \
		0,1000000,100000,50000,0,0,10000.0,0.5,10000.0,10000.0*/};
#else
	char* szContent[] = {"轴号","负向限位","正向限位","手动速度", "速度","导承","回零模式","回零方向","EZ对准",\
		"T曲线","回零加速度","回零最大速度","回零VO速度","回零回移距离","回零位置",\
		"停减速","加速曲线","加速度","减速度"};
	float defaultData[] = {-10,200,1000,10000,1000,0,1,0, \
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
		state[g_ioN-n-1] = (( motion_io >> MIO_EMG ) & 1);//急停状态
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
		int axis_id = i + v_first_axis_id[0] ;
		/*F64 v;
		APS_get_command_velocity_f(axis_id,&v);
		m_iMvSpeed[i] = v;*/
		sprintf(sz, "%.3f:%.3f", m_MvScope[i].x, m_MvScope[i].y); szPa += sz; szPa += MVFLAG;
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
		sprintf(sz,"%.3f",m_MvScope[i].x);szPa += sz;szPa+=MVFLAG;
		sprintf(sz,"%.3f",m_MvScope[i].y);szPa += sz;szPa+=MVFLAG;
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
			int axis_id = i + v_first_axis_id[0] ;
			m_MvScope[i] = cv::Point2f(m_vctDefaultData[0],m_vctDefaultData[1]);//运动范围
			m_fDebugSpeed[i] = m_vctDefaultData[2];//手动速度
			m_iMvSpeed[i]=m_fSpeed[i] = m_vctDefaultData[3];//运动速度
			m_mvScale[i] = m_vctDefaultData[4];//承载
			for (int j=0;j<iL;j++)
			{
				APS_set_axis_param(axis_id,iParam[j],m_vctDefaultData[j+5]);
			}
			for (int j=0;j<fL;j++)
			{
				APS_set_axis_param_f(axis_id,fParam[j],m_vctDefaultData[j+iL+5]);
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

			m_MvScope[i] = cv::Point2f(atof(command[0].c_str()),atof(command[1].c_str()));//运动范围
			m_fDebugSpeed[i] = atof(command[2].c_str());//手动速度
			m_iMvSpeed[i]=m_fSpeed[i] = atof(command[3].c_str());//运动速度
			m_mvScale[i] = atoi(command[4].c_str());//承载
			int axis_id = i + v_first_axis_id[0] ;
			for (int j=0;j<iL;j++)
			{
				val = atoi(command[j+5].c_str());
				APS_set_axis_param(axis_id,iParam[j],val);
			}
			for (int j=0;j<fL;j++)
			{
				val = atof(command[j+iL+5].c_str());
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
			m_MvScope[i] = cv::Point2f(m_vctDefaultData[0],m_vctDefaultData[1]);//运动范围
			m_fDebugSpeed[i] = m_vctDefaultData[2];//手动速度
			m_iMvSpeed[i]=m_fSpeed[i] = m_vctDefaultData[3];//运动速度
			m_mvScale[i] = m_vctDefaultData[4];//导承
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
			if(5>len)
				continue;

			m_MvScope[i] = cv::Point2f(atof(command[0].c_str()),atof(command[1].c_str()));//运动范围
			m_fDebugSpeed[i] = atof(command[2].c_str());//手动速度
			m_iMvSpeed[i]=m_fSpeed[i] = atof(command[3].c_str());//运动速度
			m_mvScale[i] = atoi(command[4].c_str());//导承
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
	state = 0;//运动中
	I32 msts;
	msts = APS_motion_status( mtid );// Get motion status
	msts = ( msts >> MTS_NSTP ) & 1;    // Get motion done bit
	// msts = ( msts >> MTS_HMV ) & 1;    // Get homming bit
	I32 Stop_Code;
	// Get stop code.
	APS_get_stop_code( mtid, &Stop_Code );
	
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
			state = -1;
			return true; //error
		}
		else
		{ // Motion success.
			m_Lock.unLock();
			state = -1;
			return true; 
		}
#else
		I32 motion_io = APS_motion_io_status( mtid );
		if (0 == (( motion_io >> MIO_INP ) & 1) )
		{
			state = -1;
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
	try
	{
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
	catch (char* e)
	{
		MessageBox(NULL,"错误",e, MB_YESNO | MB_ICONQUESTION) ;
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

#endif
