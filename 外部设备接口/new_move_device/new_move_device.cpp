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
	string szHelp = "\r\ncheck, id, id = 拍照检测, 相机id, 制程id\r\ngetdata, ok, ng 数据提取, ok, ng的定位位置移动动作 - 1不动作\r\n\
checkdata, ioid, up, 判断阵列数据是否检测完成, io线程id，主线程ioid = -1，up = 1上升沿 = 0下降沿边io\r\n\
阵列运动, ioid, up = 提取阵列中ng位置形成新的阵列";
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
		if (vct.size() != 3)
		{
			data = "error format:" + data;
			return false;
		}
		else if (std::string::npos != vct[0].find(ARRAYMOVE))//根据ng数组生成新的阵列运动
		{
			int ioid = atoi(vct[1].c_str());
			int up = atoi(vct[2].c_str());
			CMoveBase* mbtemp = NULL;
			if (ioid == -1)
			{
				mbtemp = mb->m_mainMB;
			}
			else
			{
				if (up > 0)
					mbtemp = ((CVisionSystem*)mb->m_mainMB)->m_io.m_ioTrigger[ioid];
				else
					mbtemp = ((CVisionSystem*)mb->m_mainMB)->m_io.m_ioTrigger[g_ioNum + ioid];
			}
			if (NULL == mbtemp)
			{
				return false;
			}
			CArrayMove& mtcraw = mbtemp->m_mtc;//源阵列数据
			CArrayMove& mtc = mb->m_mtc;//新阵列数据
			if (vct[0] == TRIGARRAYMOVE)//触发阵列运动
				mb->m_prob = 0;//触发阵列

			mtc.m_table.clear();
			mtc.m_prob = 1;//点系列
			//提取ng位置   
			map<myPoint2f, int>::iterator itor = mtcraw.m_mpArrRet.begin();
			while (itor != mtcraw.m_mpArrRet.end())
			{
				if (itor->second == 0)//ng
					mtc.m_table.push_back(itor->first);
				itor++;
			}
			mtc.pointView();
		}
		else if (vct[0] == "check" )//拍照检测 
		{
			S_Msg msg;
			cv::Point2f pt = mb->m_mtc.getCurPos();
			msg.pan_id = mb->m_prob + 1;//区分是哪个流程的盘
			msg.x = pt.x;
			msg.y = pt.y;
			if (mb->m_mtc.m_prob == 0)
			{
				msg.pan_x = msg.x / mb->m_mtc.m_mtxl;
				msg.pan_y = msg.y / mb->m_mtc.m_mtyl;
			}
			msg.arrMoveId = mb->m_arrMoveId;//阵列id 
			msg.m_procid = atoi(vct[2].c_str());
			if (msg.m_procid == -1)
			{
				msg.m_procid = mb->m_pCheck->m_picProcId;
			}
			int camid = atoi(vct[1].c_str());
			if (-1 == camid)
			{
				camid = mb->m_pCheck->m_curCamId;
			}
			mb->m_pCheck->camPic(camid, msg.vm);
			if (-1 == mb->m_pCheck->procCheckwait(&msg))
			{
				data = "cam check fail：" + data;
				return false;
			}
		}
		else if (vct[0] == "checkdata" )//检测阵列数据是否检测完成
		{
			data = "";
			int ioid = atoi(vct[1].c_str());
			int up = atoi(vct[2].c_str());
			CMoveBase* mbtemp = NULL;
			if (ioid == -1)
			{
				mbtemp = mb->m_mainMB;
			}
			else
			{
				if (up > 0)
					mbtemp = ((CVisionSystem*)mb->m_mainMB)->m_io.m_ioTrigger[ioid];
				else
					mbtemp = ((CVisionSystem*)mb->m_mainMB)->m_io.m_ioTrigger[g_ioNum + ioid];
			}
			if (NULL == mbtemp)
			{
				return false;
			}
			return mbtemp->m_mtc.ArrIsEnd();
		}
		else if (vct[0] == "getdata" )//数据提取
		{
			static CgetResultView getret(mb->m_mainMB);
			S_Msg* pmsg = getret.treatMsg();
			int limittime = 5000;//50秒超时限制
			while (NULL == pmsg)//阻塞式判断
			{
				if (((CVisionSystem*)mb->m_mainMB)->ArrIsEnd())
					return false;
				Sleep(10);
				if (limittime-- <= 0)
				{
					data = "check time out:" + data;
					return false;
				}
				pmsg = getret.treatMsg();
			}
			data = "";
			int iok = atoi(vct[1].c_str());
			int ing = atoi(vct[2].c_str());
			//根据定位结果动作
			if (pmsg->m_nc_ok == 3 && iok > 0)//ok动作
			{
				mb->m_handle->moveAbs(mb->m_mtc.m_ix, pmsg->x);
				mb->m_handle->moveAbs(mb->m_mtc.m_iy, pmsg->y);
			}
			else if (pmsg->m_nc_ok == 2 && ing > 0)//ng
			{
				mb->m_handle->moveAbs(mb->m_mtc.m_ix, pmsg->x);
				mb->m_handle->moveAbs(mb->m_mtc.m_iy, pmsg->y);
			}
			delete pmsg;
			pmsg = NULL;
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


