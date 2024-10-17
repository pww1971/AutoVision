// eyeInHandCali_device.cpp: 定义 DLL 的初始化例程。
//

#include "afxwinappex.h"
#include "afxdialogex.h"
#include "checkPatch.h"
#include "CaliDlg.h"
#include "pch.h"
#include "framework.h"
#include "eyeInHandCali_device.h"

#if (defined WIN32 || defined _WIN32 || defined WINCE)
#ifdef PROCLIB_EXPORTS
#define PROCLIB_API __declspec(dllexport)
#else
#define PROCLIB_API __declspec(dllimport)
#endif
#endif

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

// CeyeInHandCalideviceApp

BEGIN_MESSAGE_MAP(CeyeInHandCalideviceApp, CWinApp)
END_MESSAGE_MAP()


// CeyeInHandCalideviceApp 构造

CeyeInHandCalideviceApp::CeyeInHandCalideviceApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CeyeInHandCalideviceApp 对象

CeyeInHandCalideviceApp theApp;


// CeyeInHandCalideviceApp 初始化

BOOL CeyeInHandCalideviceApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

int (*RobotCom)(string str) = NULL;//机械臂的通讯控制 
extern "C" PROCLIB_API string MoveDLLHelp();
extern "C" PROCLIB_API bool MoveDLLIni(void* pmv, void*& p2);
extern "C" PROCLIB_API void MoveDLLStop();
//动态库中提取的标函数
extern "C" PROCLIB_API bool MoveDLLMove(void* pmv, void*& p2, string & data);

//必须显示的调用释放内存
extern "C" PROCLIB_API void MoveDllRelease(void*& p2);
extern "C" PROCLIB_API void MoveDllWriteFile();

string MoveDLLHelp()
{
	string szHelp = "\r\n仅仅是眼在手标定，保存仿射变换结果到对应目录，方便其他设备调用";
	return szHelp;
}

bool MoveDLLIni(void* pmv, void*& p2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (pmv == NULL)
	{
		return false;
	}
	CMoveBase* mb = (CMoveBase*)pmv;
	if (NULL == RobotCom)//运动卡里的机械手臂调用接口
	{
		if (!mb->m_mtc.m_handle->GetFunction("RobotCom", (DynaLink::Function&)RobotCom)) //从dll中加载函数
		{
			AfxMessageBox("RobotCom 加载失败");
			//return false;
		}
	}
	checkPatch* devCheck;
	if (p2 == NULL)
	{
		devCheck = new checkPatch();
		devCheck->m_CaliParam.pCheck = mb->m_pCheck;
		p2 = devCheck;
	}
	else
		devCheck = (checkPatch*)p2;
	string szDir = mb->m_szCfgAdd;
	int pos = szDir.size() - strlen("MoveCfg");
	szDir = szDir.substr(0, pos);
	devCheck->readMode(szDir + "eyeinhandcalicfg");

	CaliDlg mydlg;
	mydlg.m_pData = p2;
	if (IDOK == mydlg.DoModal())
	{
		devCheck->writeMode(szDir + "eyeinhandcalicfg");
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
	
	CMoveBase* mb = (CMoveBase*)pmv;
	if (NULL != mb)
	{
		//获取调用方的流程id和流程
		CMoveBase* mbtemp = NULL;
		int ioid = mb->m_arrMoveId;
		if (ioid == -1)//主流程
		{
			mbtemp = mb->m_mainMB;
		}
		else
		{
			mbtemp = ((CVisionSystem*)mb->m_mainMB)->m_io.m_ioTrigger[ioid];
		}
		//获取调用方阵列
		CArrayMove& mtc = mbtemp->m_mtc;
		//获取运动卡
		CMoveDllType* handle = mbtemp->m_mtc.m_handle;
		//获取图像检测制程
		CProcDll* pCheck = mbtemp->m_pCheck;
		//获取当前制程id和当前相机id和拍照显示
		int procid = pCheck->m_picProcId;
		int camid = pCheck->m_curCamId;
		CoordinatePosXYZ pd;
		pCheck->camPic(camid, pd.vm);//拍照
		pCheck->viewWin(procid, pd.vm);//显示
		//获取报表系统
		CRPTable* tbReport = mbtemp->m_tbReport;
		//检测和入库
		pd.m_procid = procid;
		pCheck->procCheck(&pd);//检测
		tbReport->pushMsg(&pd);//结果入库

	}
	data = "";
	return true;
}

//必须显示的调用释放内存
void MoveDllRelease(void*& p2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (NULL != p2)
	{
		delete (checkPatch*)p2;
		p2 = NULL;
	}
}

void MoveDllWriteFile()
{

}

