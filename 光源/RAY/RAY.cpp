// new_move_device.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "RAY.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
S_vctRay g_Ray;
//#define WJ
#define KSD //康士达
//#define OPT_OLD  //富士光源控制器
S_RayDevice::S_RayDevice()
{
	string m_stON[4];//开关状态 暂时没用
	unsigned char m_level[4];//亮度等级 暂时没用

	for (int i=0;i<4;i++)
	{
		m_stON[i] = "F";
		m_level[i] = 100;
	}
	m_stopSt = "S000F000F000F000FC#";
	m_st = "S100T100F100T100TC#";
	_isConnected = false;
	m_com = "COM2";
	m_baudrate=19200;

}

S_RayDevice::~S_RayDevice()
{
	closeRay();
	Sleep(50);
	disconnect();
}

bool S_RayDevice::getComNum()
{
	string szcom = "COM";
	for (int i=0;i<10;i++)
	{
		m_com = szcom + CStrProcess::itoa_radixN(i);
		if (RESULT_OK == connect(m_com.c_str(), 19200))
		{
			if (revData())
			{
				return true;
			}
		}
	}
	return false;
}
bool S_RayDevice::revData()
{
	const int buflen = 1;
	size_t recvSize;					//本次接收数据长度
	unsigned int timeout = 1000;			//100微秒 超时时间
	int ans = m_rxtx.waitfordata(buflen, timeout, &recvSize);
	if (ans == rp::hal::serial_rxtx::ANS_DEV_ERR || ans == rp::hal::serial_rxtx::ANS_TIMEOUT)
	{
		return false;
	}

	/*if (recvSize > buflen)
		recvSize = buflen;*/
	unsigned char recvBuffer[1024];	//串口接收缓存
	m_rxtx.recvdata(recvBuffer, recvSize);
	string st = (char*)recvBuffer;
	st = st.substr(0, recvSize);

	return true;
}

u_result S_RayDevice::connect(const char * port_path, _u32 baudrate)
{
	if (isConnected()) return RESULT_ALREADY_DONE;

	// establish the serial connection...
	if (!m_rxtx.bind(port_path, baudrate) || !m_rxtx.open()) {
		return RESULT_INVALID_DATA;
	}

	m_rxtx.flush(0);

	_isConnected = true;

	return RESULT_OK;
}

bool S_RayDevice::sendData(string st)
{
	if (!isConnected())
	{
		return false;
	}
#ifdef WJ
	//string str = "$L0=" + st.substr(1, 3) + ",L1=" + st.substr(5, 3) + ",L2=" + st.substr(9, 3) + ",L3=" + st.substr(13, 3) + "#";
	//m_rxtx.senddata((unsigned char*)str.c_str(), str.size());//文本形式
	//发送4个通道亮度等级 hex
	int w_sdLen = 4;
	int w_dateLen = w_sdLen * 3 + 5;
	unsigned char* w_sdDat = new unsigned char[w_dateLen];
	w_sdDat[0] = 0X40;//------标识符.
	w_sdDat[1] = 0X0E;//------长度. w_dateLen - 2
	w_sdDat[2] = 0X01;//------设备型号.
	w_sdDat[3] = 0X00;//------设备编号(ID).

	int j = 4;
	for (int i = 0; i < w_sdLen; i++)
	{
		w_sdDat[j++] = 0X1A;//亮度命令
		w_sdDat[j++] = i;//通道号 0-3
		w_sdDat[j++] = atoi(st.substr(1 + i * 4, 3).c_str());
	}

	UINT i;
	unsigned char _SUM = 0;
	for (i = 0; i < j; i++)
	{
		_SUM += w_sdDat[i];
	}
	_SUM &= 0xff;
	w_sdDat[j] = _SUM;

	m_rxtx.senddata(w_sdDat, w_dateLen);
	delete[] w_sdDat;
#else
#ifdef KSD
	//字符里面4通道的数字提取出来 重新组合
	string str = "SA0" + st.substr(1, 3) + "#SB0" + st.substr(5, 3) + "#SC0" + st.substr(9, 3) + "#SD0" + st.substr(13, 3) + "#";
	m_rxtx.senddata((unsigned char*)str.c_str(), str.size());//文本形式
#else
#ifdef OPT_OLD
	//st += "\r\n";
	m_rxtx.senddata((unsigned char*)st.c_str(), st.size());//文本形式
#else
	//opt型号光源控制器 采用二进制发送 使用老的光源格式 解析重新组合为opt型号的格式 仅仅支持四通道
	int sdLen = 4;
	int dateLen = sdLen*3+4;
	unsigned char* sdDat = new unsigned char[dateLen];
	sdDat[0] = 0XFE;//标记
	sdDat[1] = 0X01;//亮度控制命令
	int j=2;
	for (int i=0;i<sdLen;i++)
	{
		sdDat[j++] = i+1;//通道标记
		sdDat[j++] = 0;
		sdDat[j++] = atoi(st.substr(1+i*4,3).c_str());//字符里面4通道的数字提取出来 重新组合
	}
	sdDat[j++] = 0XFF;
	sdDat[j] = 0X00;
	m_rxtx.senddata(sdDat, dateLen);
	delete[] sdDat;
#endif
#endif
#endif
	return true;
}

bool S_RayDevice::closeRay()
{
	/*for (int i=0;i<4;i++)
	m_stON[i] = "F";
	m_stopSt = "S";
	for (int i=0;i<4;i++)
	m_st+=CStrProcess::itoa_radixN(m_level[i]) + m_stON[i];
	m_stopSt += "C#";*/
#ifdef WJ
	return sendData(m_stopSt);
#else
#ifdef KSD
	return sendData(m_stopSt);
#else
#ifdef OPT_OLD	
	return sendData(m_stopSt);
#else
	int sdLen = 4;
	int dateLen = sdLen*3+4;
	unsigned char* sdDat = new unsigned char[dateLen];
	sdDat[0] = 0XFE;//标记
	sdDat[1] = 0X05;//关闭打开指令
	int j=2;
	for (int i=0;i<sdLen;i++)
	{
		sdDat[j++] = i+1;//通道标记
		sdDat[j++] = 0;
		sdDat[j++] = 0;
	}
	sdDat[j++] = 0XFF;
	sdDat[j] = 0X00;
	m_rxtx.senddata(sdDat, dateLen);
	delete[] sdDat;
	return true;
#endif
#endif
#endif
}

//bool S_RayDevice::openRay()//
//{
//#ifdef OPT_OLD
//	return sendData(m_st);
//#else
//	int sdLen = 4;
//	int dateLen = sdLen*3+4;
//	unsigned char* sdDat = new unsigned char[dateLen];
//	sdDat[0] = 0XFE;//标记
//	sdDat[1] = 0X05;//关闭打开指令
//	int j=2;
//	for (int i=0;i<sdLen;i++)
//	{
//		sdDat[j++] = i+1;//通道标记
//		sdDat[j++] = 0;
//		sdDat[j++] = 1;
//	}
//	sdDat[j++] = 0XFF;
//	sdDat[j] = 0X00;
//	m_rxtx.senddata(sdDat, dateLen);
//	delete[] sdDat;
//	return true;
//#endif
//}
string S_RayDevice::getDefault()
{
	return m_st;
}

void S_RayDevice::setDefault(string st)
{
	m_st=st;
}

void S_RayDevice::disconnect()
{
	if (!_isConnected) 
		return;
	m_rxtx.close();
	_isConnected = false;
}

bool S_RayDevice::isConnected()
{
	return _isConnected;
}

string S_RayDevice::getStr()
{
	return m_com + MVFLAG + CStrProcess::itoa_radixN(m_baudrate) + MVFLAG + m_st;
}

S_vctRay::S_vctRay()
{
	m_curRay = 0;
}

S_vctRay::~S_vctRay()
{
	release();
}

bool S_vctRay::openRay(string st,int id)
{
	if (id == -1)
	{
		id = m_curRay;
	}
	if (id >= m_vctRay.size())
	{
		return false;
	}

	return m_vctRay[id]->sendData(st);
}

bool S_vctRay::closeRay(int id)
{
	if (id == -1)
	{
		id = m_curRay;
	}
	if (id >= m_vctRay.size())
	{
		return false;
	}
	return m_vctRay[id]->closeRay();
}

string S_vctRay::getDefault(int id)
{
	if (id == -1)
	{
		id = m_curRay;
	}
	if (id >= m_vctRay.size())
	{
		return "";
	}
	return m_vctRay[id]->m_st;
}

void S_vctRay::setDefault(string st,int id)
{
	if (id == -1)
	{
		id = m_curRay;
	}
	if (id >= m_vctRay.size())
	{
		return ;
	}
	m_vctRay[id]->m_st = st;
}

bool S_vctRay::openRay(int id)
{
	if (id == -1)
	{
		id = m_curRay;
	}
	if (id >= m_vctRay.size())
	{
		return false;
	}
	return m_vctRay[id]->sendData(m_vctRay[id]->m_st);
}

void S_vctRay::release()
{
	int len = m_vctRay.size();
	for (int i=0;i<len;i++)
	{
		delete m_vctRay[i];
	}
	m_vctRay.clear();
}

bool S_vctRay::loadCfg(string cfgfile)
{
	CConfig cfg(cfgfile);
	string szVal = cfg.getItem("rayNum");//光源数
	int num = 0;
	if ("" != szVal)
	{
		num = atoi(szVal.c_str());
	}
	if (num>0)
	{
		for(int i=0;i<num;i++)
		{
			szVal = cfg.getItem("ray"+CStrProcess::itoa_radixN(i));
			if ("" != szVal)
			{
				addRay(szVal);
			}
		}
	}
	szVal = cfg.getItem("curRay");//光源
	if ("" != szVal)
	{
		m_curRay = atoi(szVal.c_str());
	}
	return true;
}

bool S_vctRay::addRay(string command)
{
	std::vector<string > cont;
	CStrProcess::Split(command,MVFLAG,cont);
	int len = cont.size();
	if(3!=len)
		return false;
	S_RayDevice* pRay = new S_RayDevice();
	pRay->m_com = cont[0];
	pRay->m_baudrate = atoi(cont[1].c_str());
	if (cont[2]!=""/* && cont[2] != "关闭光源"*/)
		pRay->m_st = cont[2];
	if (RESULT_OK == pRay->connect(pRay->m_com.c_str(), pRay->m_baudrate))
	{
		//pRay->sendData();
		m_vctRay.push_back(pRay);
	}
	else {
		delete pRay;
		return false;
	}
	return true;
}

bool S_vctRay::writeCfg(string cfgfile)
{
	remove(cfgfile.c_str());
	CConfig cfg(cfgfile);
	cfg.updateItem("\n#光源数","");
	cfg.updateItem("rayNum", CStrProcess::itoa_radixN(m_vctRay.size()));
	for (int i=0;i<m_vctRay.size();i++)
	{
		cfg.updateItem("\n#光源"+CStrProcess::itoa_radixN(i),"com口 :波特率: 默认方案");
		cfg.updateItem("ray"+CStrProcess::itoa_radixN(i),m_vctRay[i]->getStr());
	}
	cfg.updateItem("\n#当前光源id","");
	cfg.updateItem("curRay", CStrProcess::itoa_radixN(m_curRay));

	return true;
}

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// Cnew_move_deviceApp

BEGIN_MESSAGE_MAP(CRAYApp, CWinApp)
END_MESSAGE_MAP()


// Cnew_move_deviceApp 构造

CRAYApp::CRAYApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 Cnew_move_deviceApp 对象

CRAYApp theApp;


// Cnew_move_deviceApp 初始化

BOOL CRAYApp::InitInstance()
{
	CWinApp::InitInstance();
	return TRUE;
}


//动态库中提取的标函数
bool RayDllcloseRay(int id )
{
	g_Ray.closeRay(id);
	return true;
}

bool RayDllopenRay(string st,int id)
{
	return g_Ray.openRay(st,id);
}

void RayDllWriteFile(string cfgfile)
{	
	g_Ray.writeCfg(cfgfile);
}

void RayDllRelease()
{	
	g_Ray.release();
}

bool RayDLLLoadFile(string cfgfile)
{	
	return g_Ray.loadCfg(cfgfile);
}
bool RayDLLIni(string cfgfile)
{	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CNewMoveDlg dlg;
	dlg.m_pData = &g_Ray;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		return g_Ray.writeCfg(cfgfile);
	}
	else// if (nResponse == IDCANCEL)
	{
		return false;
	}
}
string RayDLLgetDefault(int id)
{
	return g_Ray.getDefault(id);
}
void RayDLLsetDefault(string st,int id)
{
	g_Ray.setDefault(st,id);
}
int RayDLLgetRayNum()
{
	return g_Ray.m_vctRay.size();
}