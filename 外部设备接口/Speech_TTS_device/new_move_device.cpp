// new_move_device.cpp : 定义 DLL 的初始化例程。
//

#include "new_move_device.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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

BEGIN_MESSAGE_MAP(Cnew_move_deviceApp, CWinAppEx)
END_MESSAGE_MAP()


// Cnew_move_deviceApp 构造

Cnew_move_deviceApp::Cnew_move_deviceApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}


// 唯一的一个 Cnew_move_deviceApp 对象

Cnew_move_deviceApp theApp;

// Cnew_move_deviceApp 初始化

BOOL Cnew_move_deviceApp::InitInstance()
{
	if (::CoInitialize(NULL) == E_INVALIDARG)
	{
		AfxMessageBox("初始化Com失败!");
	}
	
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	return TRUE;
}

int Cnew_move_deviceApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	::CoUninitialize();

	return CWinAppEx::ExitInstance();
}

string MoveDLLHelp()
{
	string szHelp = "\r\n朗读中文和英文的混合字符串 \r\nsetNmae,小小=设置设备名称 \r\nsetFun,1=打开1关闭0语音触发短消息,固定消息‘启动’‘运行’‘停’‘暂停’‘继续’";
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
//动态库中提取的标函数
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
				devParam->Speech("开启语音识别");
			}
		}
		else
			devParam->Speech(data.c_str());
		
	}
	data="";
	return true;
}

//必须显示的调用释放内存
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




