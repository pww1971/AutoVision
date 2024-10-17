// halcon_deeplearn_device.cpp: 定义 DLL 的初始化例程。
//

#include "pch.h"
#include "framework.h"
#include "halcon_deeplearn_device.h"
#include "automove.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

string MoveDLLHelp()
{
	string szHelp = "\r\n使用制程的界面显示 因此n个深度学习模型需要n个制程。\r\ncheck, id, id = 拍照检测, 相机id, 制程id\r\n";
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
	vector<cv::Mat> vm;
	if (-1 == mb->m_pCheck->camPic(mb->m_pCheck->m_curCamId, vm))
	{
		return false;
	}
	if (NULL == p2)
	{
		p2 = new CDevice();
	}
	//配置文件放在运动制程文件夹
	string szDir = mb->m_mainMB->m_szCfgAdd;
	int pos = szDir.size() - strlen("MoveCfg");
	szDir = szDir.substr(0, pos) + "deepLearncfg";
	((CDevice*)p2)->readMode(szDir);
		//编辑模板
	if (((CDevice*)p2)->EditModel(vm[0]))
	{
		((CDevice*)p2)->writeMode(szDir);
		((CDevice*)p2)->iniMode();
	}
	
	int dlnum = ((CDevice*)p2)->m_vctDLPa.size();
	for (int i=0;i<dlnum;i++)
	{
		S_CheckParam& pa = ((CDevice*)p2)->m_vctDLPa[i];
		if (!pa.m_iniSucc)
		{
			string data = ((CDevice*)p2)->m_cfgDir + " 深度学习加载失败！";
			AfxMessageBox(data.c_str());
			delete p2;
			p2 = NULL;
			return false;
		}
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
			string szDir = mb->m_mainMB->m_szCfgAdd;
			int pos = szDir.size() - strlen("MoveCfg");
			szDir = szDir.substr(0, pos) + "deepLearncfg";
			((CDevice*)p2)->readMode(szDir);
			int dlnum = ((CDevice*)p2)->m_vctDLPa.size();
			for (int i = 0; i < dlnum; i++)
			{
				S_CheckParam& pa = ((CDevice*)p2)->m_vctDLPa[i];
				if (!pa.m_iniSucc)
				{
					data = ((CDevice*)p2)->m_cfgDir+ " 深度学习加载失败！";
					AfxMessageBox(data.c_str());
					delete p2;
					p2 = NULL;
					return false;
				}
			}
		}
		
		vector<string> vct;
		CStrProcess::Split(data, ",", vct);
		if (vct.size() != 3)
		{
			data = "error format:" + data;
			return false;
		}
		else if (vct[0] == "check")//拍照检测 
		{
			S_Msg msg;
			cv::Point2f pt;
			if (mb->m_mtc.m_table.size() > 0)
			{
				pt = mb->m_mtc.getCurPos();
			}
			else
			{
				pt.x = rand();
				pt.y = rand();
			}
			msg.pan_id = mb->m_prob + 1;//区分是哪个流程的盘
			msg.x = pt.x;
			msg.y = pt.y;
			if (mb->m_mtc.m_prob == 0)
			{
				msg.pan_x = msg.x / mb->m_mtc.m_mtxl;
				msg.pan_y = msg.y / mb->m_mtc.m_mtyl;
			}
			msg.arrMoveId = mb->m_arrMoveId;//阵列id 
			
			
			int camid = atoi(vct[1].c_str());
			if (-1 == camid)
			{
				camid = mb->m_pCheck->m_curCamId;
			}
			mb->m_pCheck->camPic(camid, msg.vm);
			if (msg.vm[0].empty())
			{
				data = "cam open fail：" + data;
				return false;
			}
			int procid = atoi(vct[2].c_str());
			if (procid < 0)
			{
				procid = ((CDevice*)p2)->m_curId;
			}
			msg.m_procid = procid;//制程名称 报表入库时会对应制程表
			msg.raw = msg.vm[0].clone();
			bool bret = ((CDevice*)p2)->checkLocateCheck(msg, procid);
			data = "";
			mb->m_pCheck->viewWin(procid, msg.vm);//使用制程的界面 因此需要保证制程表有空制程或与制程共画面
			if(NULL != mb->m_tbReport && (1==mb->m_tbReport->m_bWriteOk || !bret) )
			{
				mb->m_tbReport->pushMsg(&msg);
			}
			return bret;
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

// ChalcondeeplearndeviceApp

BEGIN_MESSAGE_MAP(ChalcondeeplearndeviceApp, CWinApp)
END_MESSAGE_MAP()


// ChalcondeeplearndeviceApp 构造

ChalcondeeplearndeviceApp::ChalcondeeplearndeviceApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	HalconCpp::SetHcppInterfaceStringEncodingIsUtf8(false);
}


// 唯一的 ChalcondeeplearndeviceApp 对象

ChalcondeeplearndeviceApp theApp;


// ChalcondeeplearndeviceApp 初始化

BOOL ChalcondeeplearndeviceApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
