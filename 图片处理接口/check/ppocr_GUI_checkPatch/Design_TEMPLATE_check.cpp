// TOT_PICTREAT_checkBurrs.cpp : 定义 DLL 应用程序的导出函数。
//


#include "checkPatch.h"

#if (defined WIN32 || defined _WIN32 || defined WINCE)
#ifdef PROCLIB_EXPORTS
#define PROCLIB_API __declspec(dllexport)
#else
#define PROCLIB_API __declspec(dllimport)
#endif

#else
#define TOTLIB_API 
#endif

static CResLock _m2oLock;
//动态库中提取的标函数
extern "C" PROCLIB_API bool DesignDLLTreatPicture(CTpltParam& pa, vector<S_TpltData*>& vctData);
bool DesignDLLTreatPicture(CTpltParam& pa, vector<S_TpltData*>& vctData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//有mfc对话框需要添加
	_m2oLock.lock();
	if (pa.m_dllPa.p == NULL)
	{
		pa.m_dllPa.p = new checkPatch();
	}
	bool ret = ((checkPatch*)pa.m_dllPa.p)->treatPicture(pa,vctData);
	_m2oLock.unLock();
	return ret;
}
extern "C" PROCLIB_API bool DesignDLLClone(CTpltParam& pa,void*p);
bool DesignDLLClone(CTpltParam& pa,void*p)
{

	return false;
}

//必须显示的调用释放内存
extern "C" PROCLIB_API void DesignDllRelease(CTpltParam& pa);
void DesignDllRelease(CTpltParam& pa)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//有mfc对话框需要添加

	_m2oLock.lock();
	if (pa.m_dllPa.p != NULL)
	{
		delete ((checkPatch*)pa.m_dllPa.p);
		pa.m_dllPa.p=NULL;
		pa.m_dllPa.ini = 0;
	}
	_m2oLock.unLock();
}

extern "C" PROCLIB_API void DesignDllWriteFile(CTpltParam& pa);
void DesignDllWriteFile(CTpltParam& pa)
{
	/*_m2oLock.lock();
	if (pa.m_dllPa.p != NULL)
		((checkPatch*)pa.m_dllPa.p)->writeFile(pa);
	_m2oLock.unLock();*/
}
extern "C" PROCLIB_API bool DesignDllDraw(cv::Mat & src, cv::RotatedRect & roiRect, CTpltParam & pa);
bool DesignDllDraw(cv::Mat& src, cv::RotatedRect& roiRect, CTpltParam& pa)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//有mfc对话框需要添加

	if (pa.m_dllPa.p != NULL)
		return ((checkPatch*)pa.m_dllPa.p)->drawResult(src, roiRect, pa);
	return false;
}
