// Monocular_Rang_checkPatch.cpp: 定义 DLL 的初始化例程。
//

#include "pch.h"
#include "framework.h"
#include "Monocular_Rang_checkPatch.h"
#include "checkPatch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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

// CMonocularRangcheckPatchApp

#if (defined WIN32 || defined _WIN32 || defined WINCE)
#ifdef PROCLIB_EXPORTS
#define PROCLIB_API __declspec(dllexport)
#else
#define PROCLIB_API __declspec(dllimport)
#endif

#else
#define PROCLIB_API 
#endif

//动态库中提取的标函数
extern "C" PROCLIB_API bool DesignDLLTreatPicture(CTpltParam & pa, vector<S_TpltData*>&vctData);
bool DesignDLLTreatPicture(CTpltParam& pa, vector<S_TpltData*>& vctData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//这句必不可少

	if (pa.m_dllPa.p == NULL)
	{
		pa.m_dllPa.p = new checkPatch();
	}
	bool ret = ((checkPatch*)pa.m_dllPa.p)->treatPicture(pa, vctData);
	return ret;
}
extern "C" PROCLIB_API bool DesignDLLClone(CTpltParam & pa, void* p);
bool DesignDLLClone(CTpltParam& pa, void* p)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//这句必不可少

	return false;
}

//必须显示的调用释放内存
extern "C" PROCLIB_API void DesignDllRelease(CTpltParam & pa);
void DesignDllRelease(CTpltParam& pa)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//这句必不可少

	if (pa.m_dllPa.p != NULL)
	{
		delete ((checkPatch*)pa.m_dllPa.p);
		pa.m_dllPa.p = NULL;
		pa.m_dllPa.ini = 0;
	}
}

extern "C" PROCLIB_API void DesignDllWriteFile(CTpltParam & pa);
void DesignDllWriteFile(CTpltParam& pa)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//这句必不可少


}

extern "C" PROCLIB_API bool DesignDllDraw(cv::Mat & src, cv::RotatedRect & roiRect, CTpltParam & pa);
bool DesignDllDraw(cv::Mat& src, cv::RotatedRect& roiRect, CTpltParam& pa)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (pa.m_dllPa.p != NULL)
		return ((checkPatch*)pa.m_dllPa.p)->drawResult(src, roiRect, pa);
	return false;
}

BEGIN_MESSAGE_MAP(CMonocularRangcheckPatchApp, CWinApp)
END_MESSAGE_MAP()


// CMonocularRangcheckPatchApp 构造

CMonocularRangcheckPatchApp::CMonocularRangcheckPatchApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CMonocularRangcheckPatchApp 对象

CMonocularRangcheckPatchApp theApp;


// CMonocularRangcheckPatchApp 初始化

BOOL CMonocularRangcheckPatchApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
