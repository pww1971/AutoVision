// Design_GUI_checkPatch.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "Design_GUI_checkPatch.h"
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

//��̬������ȡ�ı꺯��
extern "C" PROCLIB_API bool DesignDLLTreatPicture(CTpltParam& pa, vector<S_TpltData*>& vctData);
bool DesignDLLTreatPicture(CTpltParam& pa, vector<S_TpltData*>& vctData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//���ز�����

	if (pa.m_dllPa.p == NULL)
	{
		pa.m_dllPa.p = new checkPatch();
	}
	bool ret = ((checkPatch*)pa.m_dllPa.p)->treatPicture(pa, vctData);
	return ret;
}
extern "C" PROCLIB_API bool DesignDLLClone(CTpltParam& pa,void*p);
bool DesignDLLClone(CTpltParam& pa,void*p)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//���ز�����

	return false;
}

//������ʾ�ĵ����ͷ��ڴ�
extern "C" PROCLIB_API void DesignDllRelease(CTpltParam& pa);
void DesignDllRelease(CTpltParam& pa)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//���ز�����

	if (pa.m_dllPa.p != NULL)
	{
		delete ((checkPatch*)pa.m_dllPa.p);
		pa.m_dllPa.p=NULL;
		pa.m_dllPa.ini = 0;
	}
}

extern "C" PROCLIB_API void DesignDllWriteFile(CTpltParam& pa);
void DesignDllWriteFile(CTpltParam& pa)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//���ز�����


}

extern "C" PROCLIB_API bool DesignDllDraw(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa);
bool DesignDllDraw(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (pa.m_dllPa.p != NULL)
		return ((checkPatch*)pa.m_dllPa.p)->drawResult(src,roiRect,pa);
	return false;
}


BEGIN_MESSAGE_MAP(CDesign_GUI_checkPatchApp, CWinApp)
END_MESSAGE_MAP()


// CTOT_GUI_checkPatchApp ����

CDesign_GUI_checkPatchApp::CDesign_GUI_checkPatchApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CTOT_GUI_checkPatchApp ����

CDesign_GUI_checkPatchApp theApp;


// CTOT_GUI_checkPatchApp ��ʼ��

BOOL CDesign_GUI_checkPatchApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
