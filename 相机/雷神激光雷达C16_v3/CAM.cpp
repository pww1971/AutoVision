// CAM.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "CAM.h"

#include "CameraDrv.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#if (defined WIN32 || defined _WIN32 || defined WINCE)
#ifdef PROCLIB_EXPORTS
#define PROCLIB_API __declspec(dllexport)
#else
#define PROCLIB_API __declspec(dllimport)
#endif

#else
#define PROCLIB_API 
#endif



#include "CamSetRoi.h"
CamSetRoi m_dlg;
CameraDrv g_cam;

//动态库中提取的标函数
extern "C" PROCLIB_API int CamDLLgrapImage(int id,vector<cv::Mat>& vm);
int CamDLLgrapImage(int id, vector<cv::Mat>& vm)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	vm.clear();
	return g_cam.grapImage(id,vm);
}

extern "C" PROCLIB_API void CamDLLrelease();
void CamDLLrelease()
{	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	g_cam.release();
}

extern "C" PROCLIB_API bool CamDLLini();
bool CamDLLini()
{	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_dlg.m_pCheck = &g_cam;
	if (m_dlg.m_dlgSucc)
	{
		vector<cv::Mat> vm;
		if(g_cam.grapImage(0,vm)>-1)
			g_MKC.updateFrame(vm);
		m_dlg.ShowWindow(1);
		return true;
	}
	CConfig cfg(getExePath() + NETCFG);
	string szWinName = cfg.getItem("WINNAME");
	if (szWinName == "")
	{
		szWinName = "视觉控制系统";
	}
	HWND hMainWnd=FindWindowA("#32770",szWinName.c_str());//通过遍历窗口方式获取目标进程主窗口句柄
	if(hMainWnd!=0)
	{
		m_dlg.Create(IDD_DIALOG1,CWnd::FromHandle(hMainWnd));
	//	m_dlg.ShowWindow(0);
		m_dlg.m_dlgSucc = true;
		return true;
	}
	else
	{
		MessageBoxA(NULL,"ROI界面调用失败","",0);
		return false;
	}

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

// CCAMApp

BEGIN_MESSAGE_MAP(CCAMApp, CWinApp)
	ON_COMMAND(ID_FILE_NEW, &CCAMApp::OnFileNew)
END_MESSAGE_MAP()


// CCAMApp 构造

CCAMApp::CCAMApp()
{
#ifdef CALI
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
	SetSystem("parallelize_operators", "true");

#endif
}


// 唯一的一个 CCAMApp 对象

CCAMApp theApp;


// CCAMApp 初始化

BOOL CCAMApp::InitInstance()
{
	CWinAppEx::InitInstance();

#ifdef CALI

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
//	CShellManager *pShellManager = new CShellManager;
	
	m_pMainWnd = &checkPatch::S_dlg;
	UCI::pDis = new ST_MYDISP(&checkPatch::S_dlg);
	UCI::initFirst();
#endif
	return TRUE;
}


int CCAMApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	//if (pShellManager != NULL)
	//{
	//	delete pShellManager;
	//}

	return CWinAppEx::ExitInstance();
}