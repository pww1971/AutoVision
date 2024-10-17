// FaceVerify_device.cpp: 定义 DLL 的初始化例程。
//

#include "pch.h"
#include "framework.h"
#include "FaceVerify_device.h"
#include "FaceLivingDetector.h"
#include "CFaceVerifyDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace FaceLivingDetector;

#if (defined WIN32 || defined _WIN32 || defined WINCE)
#ifdef PROCLIB_EXPORTS
#define PROCLIB_API __declspec(dllexport)
#else
#define PROCLIB_API __declspec(dllimport)
#endif

#else
#define PROCLIB_API 
#endif
extern "C" PROCLIB_API string MoveDLLHelp();
extern "C" PROCLIB_API bool MoveDLLIni(void* pmv, void*& p2);
extern "C" PROCLIB_API void MoveDLLStop();
//动态库中提取的标函数
extern "C" PROCLIB_API bool MoveDLLMove(void* pmv, void*& p2, string & data);

//必须显示的调用释放内存
extern "C" PROCLIB_API void MoveDllRelease(void*& p2);
extern "C" PROCLIB_API void MoveDllWriteFile();
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

// CFaceVerifydeviceApp

BEGIN_MESSAGE_MAP(CFaceVerifydeviceApp, CWinApp)
END_MESSAGE_MAP()


// CFaceVerifydeviceApp 构造

CFaceVerifydeviceApp::CFaceVerifydeviceApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CFaceVerifydeviceApp 对象

CFaceVerifydeviceApp theApp;


// CFaceVerifydeviceApp 初始化

BOOL CFaceVerifydeviceApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


string MoveDLLHelp()
{
	string szHelp = "\r\n";
	return szHelp;
}

bool MoveDLLIni(void* pmv, void*& p2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (pmv == NULL)
	{
		return false;
	}
	CVisionSystem* mb = (CVisionSystem*)pmv;
	if (p2 == NULL)
	{
		p2 = new FaceFeatureExtractor();
	}
	//该变量需要保密 否则通过外部设备访问会破解权限
	((FaceFeatureExtractor*)p2)->m_mb = (CVisionSystem*)mb->m_mainMB;
	int auth = ((FaceFeatureExtractor*)p2)->m_mb->m_logMag->m_auth;//权限 -1未登录 0 最高 1 管理 2 普通用户
	if (0 == auth || 1 == auth)
	{
		CFaceVerifyDlg dlg;
		dlg.m_pFace = p2;
		if (1 == dlg.DoModal())
		{
			//配置文件放在运动制程文件夹
			/*string szDir = mb->m_mainMB->m_szCfgAdd;
			int pos = szDir.size() - strlen("FaceCfg");
			szDir = szDir.substr(0, pos);
			*/
		}
	}
	else
		return false;

	return true;
}

void MoveDLLStop()
{

}

//动态库中提取的标函数
bool MoveDLLMove(void* pmv, void*& p2, string& data)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CVisionSystem* mb = (CVisionSystem*)pmv;
	if (NULL != mb)
	{
		FaceFeatureExtractor* devParam;
		if (p2 == NULL)
		{
			devParam = new FaceFeatureExtractor();
			p2 = devParam;
		}
		else
			devParam = (FaceFeatureExtractor*)p2;
		//该变量需要保密 否则通过外部设备访问会破解权限
		devParam->m_mb = (CVisionSystem*)mb->m_mainMB;
		int auth = devParam->m_mb->m_logMag->m_auth;//权限 -1未登录 0 最高 1 管理 2 普通用户
		if (0 == auth || 1 == auth)
		{
			if (1 == devParam->ActiveFaceVerify2(data))
				return true;
			else
				return false;
		}
		else
		{
			data = "error format:" + data;
			return false;
		}
	}
	return true;
}

//必须显示的调用释放内存
void MoveDllRelease(void*& p2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (NULL != p2)
	{
		delete (FaceFeatureExtractor*)p2;
		p2 = NULL;
	}
}

void MoveDllWriteFile()
{

}
