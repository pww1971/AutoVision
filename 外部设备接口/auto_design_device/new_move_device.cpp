// new_move_device.cpp : 定义 DLL 的初始化例程。
//

#include "new_move_device.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
//#include "checkPatch.h"
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
	//	SetSystem("parallelize_operators", "true");
}


// 唯一的一个 Cnew_move_deviceApp 对象

Cnew_move_deviceApp theApp;


// Cnew_move_deviceApp 初始化

BOOL Cnew_move_deviceApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	/*UCI::pDis = new ST_MYDISP(&checkPatch::S_dlg);
	UCI::initFirst();*/
	return TRUE;
}

string MoveDLLHelp()
{
	string szHelp = "\r\n把当前图像制程修改图片处理的总面积参数后重新保存到本地文件夹里\r\nautoDesign, id = 自动建模, 相机id\r\niniDesign,dir = 初始化, dir建模主目录\r\n \
setPwwOffSet,0.2 = 设置偏移量\r\ncheckObj,id = 二维码验证目标";
	return szHelp;
}

bool MoveDLLIni(void* pmv, void*& p2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (pmv == NULL)
	{
		return false;
	}
	if (NULL == p2)
	{
		p2 = new CDevice();
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
		if (NULL == p2)
		{
			p2 = new CDevice();
		}

		std::vector<string> vct;
		CStrProcess::Split(data, ",", vct);
		if ("autoDesign" == vct[0])//
		{
			if (vct.size() != 2)
			{
				data = "error format:" + data;
				return false;
			}
			int camid = atoi(vct[1].c_str());
			if (-1 == camid)
			{
				camid = mb->m_pCheck->m_curCamId;
			}
			std::vector<cv::Mat> vm;
			mb->m_pCheck->camPic(camid, vm);
			if (((CDevice*)p2)->autoDesign(vm))//(((CDevice*)p2)->wechatDesign(m))//
				;
			else
			{
				data = "error:" + data;
				return false;
			}
		}
		else if (vct[0] == "iniDesign")//
		{
			if (vct.size() != 2)
			{
				data = "error format:" + data;
				return false;
			}
			int procid = mb->m_pCheck->m_picProcId;
			((CDevice*)p2)->ini(mb->m_pCheck->findProcName(procid) + PROCFILE, vct[1]);
		}
		else if (vct[0] == "setPwwOffSet")
		{
			if (vct.size() != 2)
			{
				data = "error format:" + data;
				return false;
			}
			((CDevice*)p2)->m_mkc.m_pwwoffset = atof(vct[1].c_str());
		}
		else if (vct[0] == "checkObj")
		{
			if (vct.size() != 2)
			{
				data = "error format:" + data;
				return false;
			}
			int camid = atoi(vct[1].c_str());
			if (-1 == camid)
			{
				camid = mb->m_pCheck->m_curCamId;
			}
			std::vector<cv::Mat> vm;
			mb->m_pCheck->camPic(camid, vm);
			if (!((CDevice*)p2)->checkObj(vm))
			{
				data = "error :" + data;
				return false;
			}
		}
		else
		{
			data = "error format:" + data;
			return false;
		}
	}
	else
	{
		data = "error noSysPtr:" + data;
		return false;
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
		delete (CDevice*)p2;
		p2 = NULL;
	}
}

void MoveDllWriteFile()
{

}


