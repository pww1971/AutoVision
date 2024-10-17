// mosaic_device.cpp: 定义 DLL 的初始化例程。
//

#include "pch.h"
#include "framework.h"
#include "mosaic_device.h"
#include "mosaicDrv.hpp"
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

// CmosaicdeviceApp

BEGIN_MESSAGE_MAP(CmosaicdeviceApp, CWinApp)
END_MESSAGE_MAP()


// CmosaicdeviceApp 构造

CmosaicdeviceApp::CmosaicdeviceApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CmosaicdeviceApp 对象

CmosaicdeviceApp theApp;


// CmosaicdeviceApp 初始化

BOOL CmosaicdeviceApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}



string MoveDLLHelp()
{
	string szHelp = "\r\ncammosaic = 系列拍照后拼接";
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
		p2 = new mosaicHarris();
		((mosaicHarris*)p2)->m_Cam = mb->m_mainMB->m_pCheck;
	}

	//配置文件放在运动制程文件夹
	string szDir = mb->m_mainMB->m_szCfgAdd;
	int pos = szDir.size() - strlen("MoveCfg");
	szDir = szDir.substr(0, pos);
	((mosaicHarris*)p2)->readMode(szDir);
	//编辑模板
	if (((mosaicHarris*)p2)->EditModel())
	{
		((mosaicHarris*)p2)->writeMode(szDir);
	}

	
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
		mosaicHarris* devParam;
		if (p2 == NULL)
		{
			devParam = new mosaicHarris();
			p2 = devParam;
			((mosaicHarris*)p2)->m_Cam = mb->m_mainMB->m_pCheck;
			string szDir = mb->m_mainMB->m_szCfgAdd;
			int pos = szDir.size() - strlen("MoveCfg");
			szDir = szDir.substr(0, pos);
			devParam->readMode(szDir);
		}
		else
			devParam = (mosaicHarris*)p2;
		if (data == "cammosaic")
		{
			std::vector<cv::Mat> vm;
			if (-1 == mb->m_pCheck->camPic(mb->m_pCheck->m_curCamId, devParam->m_vctImage))
			{
				data = "";
				return false;
			}
			if (-1 == mb->m_pCheck->camPic(mb->m_pCheck->m_curCamId, vm))
			{
				data = "";
				return false;
			}
			devParam->m_vctImage.push_back(vm[0]);
			cv::Mat m;
			devParam->mosaicDrv::mosaicImage(devParam->m_vctImage[0], devParam->m_vctImage[1], m);
			vm[0] = m;
			mb->m_pCheck->viewWin(0, vm);
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
		delete (mosaicDrv*)p2;
		p2 = NULL;
	}
}

void MoveDllWriteFile()
{

}
