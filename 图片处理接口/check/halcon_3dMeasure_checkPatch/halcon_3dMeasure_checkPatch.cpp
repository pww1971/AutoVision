// halcon_3dMeasure_checkPatch.cpp: 定义 DLL 的初始化例程。
//

#include "pch.h"
#include "framework.h"
#include "halcon_3dMeasure_checkPatch.h"
#include "checkPatch.h"

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
//clone返回false会进入DesignDLLTreatPicture,为了方便无法内存读写初始化就从该函数进行初始化。clone返回ture将不进入
//动态库中提取的标函数
extern "C" PROCLIB_API bool DesignDLLTreatPicture(CTpltParam & pa, vector<S_TpltData*>&vctData);
bool DesignDLLTreatPicture(CTpltParam& pa, vector<S_TpltData*>& vctData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
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
	S_DllParam& halPa = pa.m_dllPa;
	if (halPa.p == NULL)
		halPa.p = new checkPatch();
	else
	{
		//((checkPatch*)pa.m_dllPa.p)->clearMode();
	}
	((checkPatch*)pa.m_dllPa.p)->clone(*(checkPatch*)p);
	/*((checkPatch*)pa.m_dllPa.p)->readMode(((checkPatch*)p)->m_configAdd);
	((checkPatch*)pa.m_dllPa.p)->m_pa.hv_CamParam = ((checkPatch*)p)->m_pa.hv_CamParam;*/
	return true;
}

extern "C" PROCLIB_API void DesignDllRelease(CTpltParam & pa);
void DesignDllRelease(CTpltParam& pa)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (pa.m_dllPa.ini == 1)
	{
		try
		{
			if (NULL != pa.m_dllPa.p)
			{
				delete (checkPatch*)pa.m_dllPa.p;
				pa.m_dllPa.p = NULL;
			}
		}
		catch (...)
		{
			MessageBox(0, "释放动态库（3Part.dll）失败", "系统提示", MB_YESNO | MB_ICONQUESTION);
			return;
		}
		pa.m_dllPa.sel = NULL;
		pa.m_dllPa.ini = 0;
	}
}

extern "C" PROCLIB_API void DesignDllWriteFile(CTpltParam & pa);
void DesignDllWriteFile(CTpltParam& pa)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (pa.m_dllPa.ini == 1)
	{
		try
		{
			if (NULL != pa.m_dllPa.p)
			{
				if (pa.m_dllPa.ini == 1 && "" != pa.m_dllPa.szfile)
				{
					((checkPatch*)pa.m_dllPa.p)->writeMode(pa.m_dllPa.szfile);
				}
			}
		}
		catch (...)
		{
			MessageBox(0, "动态库（3Part.dll）写文件失败", "系统提示", MB_YESNO | MB_ICONQUESTION);
			return;
		}
	}

}

extern "C" PROCLIB_API bool DesignDllDraw(cv::Mat & src, cv::RotatedRect & roiRect, CTpltParam & pa);
bool DesignDllDraw(cv::Mat& src, cv::RotatedRect& roiRect, CTpltParam& pa)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (pa.m_dllPa.p != NULL)
		return ((checkPatch*)pa.m_dllPa.p)->drawResult(src, roiRect, pa);
	return false;
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

// Chalcon3dMeasurecheckPatchApp

BEGIN_MESSAGE_MAP(Chalcon3dMeasurecheckPatchApp, CWinApp)
END_MESSAGE_MAP()


// Chalcon3dMeasurecheckPatchApp 构造

Chalcon3dMeasurecheckPatchApp::Chalcon3dMeasurecheckPatchApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 Chalcon3dMeasurecheckPatchApp 对象

Chalcon3dMeasurecheckPatchApp theApp;


// Chalcon3dMeasurecheckPatchApp 初始化

BOOL Chalcon3dMeasurecheckPatchApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
