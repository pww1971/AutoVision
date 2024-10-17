// LSLidar.cpp: 定义 DLL 的初始化例程。
//

#include "pch.h"
#include "framework.h"
#include "LSLidar.h"
#include "LiDarParamDlg.h"
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

CameraDrv g_cam;
LiDarParamDlg g_dlg;
//动态库中提取的标函数
extern "C" PROCLIB_API int CamDLLgrapImage(int id, std::vector<cv::Mat>&vm);
int CamDLLgrapImage(int id, std::vector<cv::Mat>& vm)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	vm.clear();
	return g_cam.grapImage(id, vm);
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
	g_cam.ini();
	g_dlg.m_pCheck = &g_cam;
	if (g_dlg.m_dlgSucc)
	{
		g_dlg.ShowWindow(1);
		return true;
	}
	CConfig cfg(getExePath() + NETCFG);
	string szWinName = cfg.getItem("WINNAME");
	if (szWinName == "")
	{
		szWinName = "视觉控制系统";
	}
	HWND hMainWnd = FindWindowA("#32770", szWinName.c_str());//通过遍历窗口方式获取目标进程主窗口句柄
	if (hMainWnd != 0)
	{
		g_dlg.Create(IDD_DIALOG1, CWnd::FromHandle(hMainWnd));
		g_dlg.ShowWindow(1);
		g_dlg.m_dlgSucc = true;
		return true;
	}
	else
	{
		MessageBoxA(NULL, "雷达参数界面调用失败", "", 0);
		return false;
	}
	return true;
}

//
//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
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
//		出现在每个函数中十分重要。  这意味着
//		它必须作为以下项中的第一个语句:
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CLSLidarApp

BEGIN_MESSAGE_MAP(CLSLidarApp, CWinApp)
END_MESSAGE_MAP()


// CLSLidarApp 构造

CLSLidarApp::CLSLidarApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CLSLidarApp 对象

CLSLidarApp theApp;


// CLSLidarApp 初始化

BOOL CLSLidarApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
