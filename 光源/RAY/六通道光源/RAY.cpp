// new_move_device.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "RAY.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
S_vctRay g_Ray;
#define OPT_OLD  //�ϵĹ�Դ������
S_RayDevice::S_RayDevice()
{
	string m_stON[4];//����״̬ ��ʱû��
	unsigned char m_level[4];//���ȵȼ� ��ʱû��

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
	const int buflen = 32;

	size_t recvSize;					//���ν������ݳ���
	unsigned int timeout = 100;			//100΢�� ��ʱʱ��
	int ans = m_rxtx.waitfordata(buflen, timeout, &recvSize);
	if (ans == rp::hal::serial_rxtx::ANS_DEV_ERR || ans == rp::hal::serial_rxtx::ANS_TIMEOUT)
	{
		return false;
	}

	/*if (recvSize > buflen)
		recvSize = buflen;*/
	unsigned char* recvBuffer = new unsigned char[recvSize];	//���ڽ��ջ���
	m_rxtx.recvdata(recvBuffer, recvSize);

	delete[] recvBuffer ;	//���ڽ��ջ���
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
#ifdef OPT_OLD
	m_rxtx.senddata((unsigned char*)st.c_str(), st.size());//�ı���ʽ
#else
	//opt�ͺŹ�Դ������ ���ö����Ʒ��� ʹ���ϵĹ�Դ��ʽ �����������Ϊopt�ͺŵĸ�ʽ ����֧����ͨ��
	int sdLen = 4;
	int dateLen = sdLen*3+4;
	unsigned char* sdDat = new unsigned char[dateLen];
	sdDat[0] = 0XFE;//���
	sdDat[1] = 0X01;//���ȿ�������
	int j=2;
	for (int i=0;i<sdLen;i++)
	{
		sdDat[j++] = i+1;//ͨ�����
		sdDat[j++] = 0;
		sdDat[j++] = atoi(st.substr(1+i*4,3).c_str());
	}
	sdDat[j++] = 0XFF;
	sdDat[j] = 0X00;
	m_rxtx.senddata(sdDat, dateLen);
	delete[] sdDat;
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
#ifdef OPT_OLD	
	return sendData(m_stopSt);
#else
	int sdLen = 4;
	int dateLen = sdLen*3+4;
	unsigned char* sdDat = new unsigned char[dateLen];
	sdDat[0] = 0XFE;//���
	sdDat[1] = 0X05;//�رմ�ָ��
	int j=2;
	for (int i=0;i<sdLen;i++)
	{
		sdDat[j++] = i+1;//ͨ�����
		sdDat[j++] = 0;
		sdDat[j++] = 0;
	}
	sdDat[j++] = 0XFF;
	sdDat[j] = 0X00;
	m_rxtx.senddata(sdDat, dateLen);
	delete[] sdDat;
	return true;
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
//	sdDat[0] = 0XFE;//���
//	sdDat[1] = 0X05;//�رմ�ָ��
//	int j=2;
//	for (int i=0;i<sdLen;i++)
//	{
//		sdDat[j++] = i+1;//ͨ�����
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
//���ݸ�ʿ6ͨ��˵����2�ı���
void S_vctRay::codeStr(string& sendstr,int id)
{
	string sdDat[4];
	for (int i=0;i<4;i++)
	{
		sdDat[i] = sendstr.substr(1+i*4,3)+"#";
	}

	if(id%3==2)//
	{
		sendstr = "SC0" + sdDat[1];
	}
	else if(id%3==1)//
	{
		sendstr = "SB0" + sdDat[1] + "SE0" + sdDat[3];
	}
	else
	{
		sendstr = "SA0" + sdDat[1] + "SF0" + sdDat[2] +"SD0" + sdDat[3];
	}
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
	codeStr( st, id);
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
	string sendstr=m_vctRay[id]->m_stopSt;
	codeStr( sendstr, id);
	return m_vctRay[id]->sendData(sendstr);
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
	string sendstr=m_vctRay[id]->m_st;
	codeStr( sendstr, id);
	return m_vctRay[id]->sendData(sendstr);
}

void S_vctRay::release()
{
	int len = m_vctRay.size();
	for (int i=0;i<len;i+=3)
	{
		delete m_vctRay[i];
	}
	m_vctRay.clear();
}
bool S_vctRay::loadRay(string command,int id)
{
	vector<string > cont;
	CStrProcess::Split(command,MVFLAG,cont);
	int len = cont.size();
	if(3!=len)
		return false;
	if(id%3 > 0)
	{
		S_RayDevice* pRay = m_vctRay[id-1];
		pRay->m_com = cont[0];
		pRay->m_baudrate = atoi(cont[1].c_str());
		if (cont[2]!=""/* && cont[2] != "�رչ�Դ"*/)
			pRay->m_st = cont[2];
		m_vctRay.push_back(pRay);
	}
	else
	{
		S_RayDevice* pRay = new S_RayDevice();
		pRay->m_com = cont[0];
		pRay->m_baudrate = atoi(cont[1].c_str());
		if (cont[2]!=""/* && cont[2] != "�رչ�Դ"*/)
			pRay->m_st = cont[2];
		if (RESULT_OK == pRay->connect(pRay->m_com.c_str(), pRay->m_baudrate))
		{
			//pRay->sendData();
		}
		m_vctRay.push_back(pRay);
	}
	
	return true;
}

bool S_vctRay::loadCfg(string cfgfile)
{
	CConfig cfg(cfgfile);
	string szVal = cfg.getItem("rayNum");//��Դ��
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
				loadRay(szVal,i);
			}
		}
	}
	szVal = cfg.getItem("curRay");//��Դ
	if ("" != szVal)
	{
		m_curRay = atoi(szVal.c_str());
	}
	return true;
}

bool S_vctRay::addRay(string command)
{
	vector<string > cont;
	CStrProcess::Split(command,MVFLAG,cont);
	int len = cont.size();
	if(3!=len)
		return false;
	S_RayDevice* pRay = new S_RayDevice();
	pRay->m_com = cont[0];
	pRay->m_baudrate = atoi(cont[1].c_str());
	if (cont[2]!=""/* && cont[2] != "�رչ�Դ"*/)
		pRay->m_st = cont[2];
	if (RESULT_OK == pRay->connect(pRay->m_com.c_str(), pRay->m_baudrate))
	{
		//pRay->sendData();
		m_vctRay.push_back(pRay);
		m_vctRay.push_back(pRay);
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
	cfg.updateItem("\n#��Դ��","");
	cfg.updateItem("rayNum", CStrProcess::itoa_radixN(m_vctRay.size()));
	for (int i=0;i<m_vctRay.size();i++)
	{
		cfg.updateItem("\n#��Դ"+CStrProcess::itoa_radixN(i),"com�� :������: Ĭ�Ϸ���");
		cfg.updateItem("ray"+CStrProcess::itoa_radixN(i),m_vctRay[i]->getStr());
	}
	cfg.updateItem("\n#��ǰ��Դid","");
	cfg.updateItem("curRay", CStrProcess::itoa_radixN(m_curRay));

	return true;
}

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// Cnew_move_deviceApp

BEGIN_MESSAGE_MAP(CRAYApp, CWinApp)
END_MESSAGE_MAP()


// Cnew_move_deviceApp ����

CRAYApp::CRAYApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� Cnew_move_deviceApp ����

CRAYApp theApp;


// Cnew_move_deviceApp ��ʼ��

BOOL CRAYApp::InitInstance()
{
	CWinApp::InitInstance();
	return TRUE;
}


//��̬������ȡ�ı꺯��
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