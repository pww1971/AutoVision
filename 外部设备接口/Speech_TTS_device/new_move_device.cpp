// new_move_device.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "new_move_device.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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

BEGIN_MESSAGE_MAP(Cnew_move_deviceApp, CWinAppEx)
END_MESSAGE_MAP()


// Cnew_move_deviceApp ����

Cnew_move_deviceApp::Cnew_move_deviceApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}


// Ψһ��һ�� Cnew_move_deviceApp ����

Cnew_move_deviceApp theApp;

// Cnew_move_deviceApp ��ʼ��

BOOL Cnew_move_deviceApp::InitInstance()
{
	if (::CoInitialize(NULL) == E_INVALIDARG)
	{
		AfxMessageBox("��ʼ��Comʧ��!");
	}
	
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	return TRUE;
}

int Cnew_move_deviceApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	::CoUninitialize();

	return CWinAppEx::ExitInstance();
}

string MoveDLLHelp()
{
	string szHelp = "\r\n�ʶ����ĺ�Ӣ�ĵĻ���ַ��� \r\nsetNmae,СС=�����豸���� \r\nsetFun,1=��1�ر�0������������Ϣ,�̶���Ϣ�������������С���ͣ������ͣ����������";
	return szHelp;
}

bool MoveDLLIni(void* pmv,void*& p2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (pmv==NULL)
	{
		return false;
	}
	CVisionSystem* mb = (CVisionSystem*)pmv;
	CDevice* devParam;
	if (p2==NULL)
	{
		devParam = new CDevice();
		string szDir = mb->m_szCfgAdd;
		int pos = szDir.size()-strlen("MoveCfg");
		szDir = szDir.substr(0,pos);
		devParam->m_cfgAdd = szDir+"SpeechDllcfg";
		devParam->loadcfg();
		p2 = devParam;
	}
	else
		devParam = new CDevice();

	devParam->m_mb = (CVisionSystem*)mb->m_mainMB;
	
	return true;
}

void MoveDLLStop()
{
	
}
//��̬������ȡ�ı꺯��
bool MoveDLLMove(void* pmv,void*& p2,string& data)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CVisionSystem* mb = (CVisionSystem*)pmv;
	if (NULL != mb)
	{
		CDevice* devParam;
		if (p2==NULL)
		{
			devParam = new CDevice();
			string szDir = mb->m_szCfgAdd;
			int pos = szDir.size()-strlen("MoveCfg");
			szDir = szDir.substr(0,pos);
			devParam->m_cfgAdd = szDir+"SpeechDllcfg";
			devParam->loadcfg();
			p2 = devParam;
		}
		else
			devParam = (CDevice*)p2;
		devParam->m_mb = (CVisionSystem*)mb->m_mainMB;
		std::vector<string > cont;
		CStrProcess::Split(data, ",", cont, true);
		if (cont.size() > 1)
		{
			if ("setNmae" == cont[0])
			{
				devParam->m_szMyName = cont[1];
			}
			else if("setFun" == cont[0])
			{
				devParam->m_bMsg = atoi(cont[1].c_str());
			}
			if (!devParam->m_runing)
			{
				devParam->open();
				devParam->Speech("��������ʶ��");
			}
		}
		else
			devParam->Speech(data.c_str());
		
	}
	data="";
	return true;
}

//������ʾ�ĵ����ͷ��ڴ�
void MoveDllRelease(void*& p2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (NULL!=p2)
	{
		delete (CDevice*)p2;
		p2=NULL;
	}
	
}

void MoveDllWriteFile()
{
	
}




