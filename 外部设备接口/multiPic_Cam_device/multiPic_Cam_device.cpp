// multiPic_Cam_device.cpp: 定义 DLL 的初始化例程。
//

#include "pch.h"
#include "framework.h"
#include "multiPic_Cam_device.h"

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

// CmultiPicCamdeviceApp

BEGIN_MESSAGE_MAP(CmultiPicCamdeviceApp, CWinApp)
END_MESSAGE_MAP()


// CmultiPicCamdeviceApp 构造

CmultiPicCamdeviceApp::CmultiPicCamdeviceApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CmultiPicCamdeviceApp 对象

CmultiPicCamdeviceApp theApp;


// CmultiPicCamdeviceApp 初始化

BOOL CmultiPicCamdeviceApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}



string MoveDLLHelp()
{
	string szHelp = "\r\ncam,-1 = 拍照, 相机id\r\ncheck,-1 = 检测, 制程id\r\nupdateMain = 更新制程视图";
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
	CMoveBase* mb = (CMoveBase*)pmv;
	if (NULL != mb)
	{
		if (NULL == p2)
		{
			p2 = new CDevice();
		}

		std::vector<string> vct;
		CStrProcess::Split(data, ",", vct);
		if (vct[0] == "cam")
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
			((CDevice*)p2)->m_vm.insert(((CDevice*)p2)->m_vm.begin(),vm.begin(),vm.end());
		}
		else if (vct[0] == "check")//拍照检测 
		{
			if (vct.size() != 2)
			{
				data = "error format:" + data;
				return false;
			}
			CoordinatePosXYZ msg;
			if (mb->m_mtc.m_table.size()>0)
			{
				cv::Point2f pt = mb->m_mtc.getCurPos();
				msg.pan_id = mb->m_prob + 1;//区分是哪个流程的盘
				msg.x = pt.x;
				msg.y = pt.y;
				if (mb->m_mtc.m_prob == 0)
				{
					msg.pan_x = msg.x / mb->m_mtc.m_mtxl;
					msg.pan_y = msg.y / mb->m_mtc.m_mtyl;
				}
			}
			msg.arrMoveId = mb->m_arrMoveId;//阵列id 
			msg.m_procid = atoi(vct[1].c_str());
			if (msg.m_procid == -1)
			{
				msg.m_procid = mb->m_pCheck->m_picProcId;
			}
			
			msg.vm.swap(((CDevice*)p2)->m_vm);
			if (-1 == mb->m_pCheck->procCheck(&msg))
			{
				data = " check fail：" + data;
				return false;
			}
		}
		else if (vct[0] == "updateMain")//
		{
			data = "";
			if (((CDevice*)p2)->m_vm.size() > 0)
				mb->m_mainMB->m_pCheck->updateMain(((CDevice*)p2)->m_vm);
			((CDevice*)p2)->m_vm.clear();
			return true;
		}
		else
		{
			data = "error format:" + data;
			return false;
		}
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


