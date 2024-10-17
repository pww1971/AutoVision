/* new_move_device.cpp : 定义 DLL 的初始化例程。
n点标定，根据标定数据实现点系列阵列。
可以把标定数据，拷贝到图像制程的unit参数单个像素的毫米，这样测量dll就自动实现了毫米的转换,否则默认是像素距离.
*/

#include "new_move_device.h"
#include "automove.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "checkPatch.h"
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
	SetSystem("parallelize_operators", "true");
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

	UCI::pDis = new ST_MYDISP(&checkPatch::S_dlg);
	UCI::initFirst();
	return TRUE;
}

string MoveDLLHelp()
{
	string szHelp = "\r\n阵列运动=多目标定位阵列\r\nanglemove,n = 轴n旋转角运动\r\nanglemm, f = 设置旋转电机角度参数";
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
	checkPatch* devCheck;
	if (p2 == NULL)
	{
		devCheck = new checkPatch();
		devCheck->m_CaliParam.pCheck = mb->m_pCheck;
		string szDir = mb->m_szCfgAdd;
		int pos = szDir.size() - strlen("MoveCfg");
		szDir = szDir.substr(0, pos);
		devCheck->readMode(szDir + "arrmovedllcfg");
		p2 = devCheck;
	}
	else
		devCheck = (checkPatch*)p2;
	CNewMoveDlg mydlg;
	mydlg.m_pData = p2;
	std::vector<cv::Mat> vm;
	if (-1 == mb->m_pCheck->camPic(mb->m_pCheck->m_curCamId, vm))
	{
		return false;
	}
	devCheck->m_rawPic = vm[0];
	if (IDOK == mydlg.DoModal())
	{
		string szDir = mb->m_szCfgAdd;
		int pos = szDir.size() - strlen("MoveCfg");
		szDir = szDir.substr(0, pos);
		devCheck->writeMode(szDir + "arrmovedllcfg");
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
		checkPatch* devCheck;
		if (p2 == NULL)
		{
			devCheck = new checkPatch();
			devCheck->m_CaliParam.pCheck = mb->m_pCheck;
			string szDir = mb->m_szCfgAdd;
			int pos = szDir.size() - strlen("MoveCfg");
			szDir = szDir.substr(0, pos);
			devCheck->readMode(szDir + "arrmovedllcfg");
			p2 = devCheck;
		}
		else
			devCheck = (checkPatch*)p2;
		if (data == ARRAYMOVE)//根据多目标定位生成阵列运动
		{
			CArrayMove& mtc = mb->m_mtc;
			std::vector<cv::Mat> vm;
			if (-1 == mb->m_pCheck->camPic(mb->m_pCheck->m_curCamId, vm))
			{
				data += "open cam error";
				return false;
			}
			//多目标定位
			if (devCheck->m_Proc.parallelMatchTplt(vm) > -1)//ok
			{
				mtc.m_table.clear();
				mtc.m_prob = 1;
				devCheck->m_DevParam.m_angleTab.clear();
				devCheck->m_DevParam.angleTabId = 0;
				double mm = devCheck->m_CaliParam.dSh;//每像素毫米单位
				std::vector<S_TpltData*>& vctData = devCheck->m_Proc.m_vcTplt[0]->m_vctData;
				for (int ik = 0; ik < vctData.size(); ik++)
				{
					float posX = vctData[ik]->m_roiRect.center.x;
					float posY = vctData[ik]->m_roiRect.center.y;
					float posZ = vctData[ik]->m_roiRect.angle;//旋转角
					devCheck->m_DevParam.m_angleTab.push_back(posZ);
					mtc.m_table.push_back(cv::Point2f(posX * mm, posY * mm));
				}
				mtc.pointView();
				mtc.m_prob = 1;//点系列
			}
			else
			{
				data = "";
				return false;
			}
		}
		else
		{
			CMoveDllType* handle = mb->m_mtc.m_handle;
			if (NULL == handle)
			{
				data += " no movecard";
				return false;
			}
			std::vector<string> vct;
			CStrProcess::Split(data, ",", vct);
			if (vct[0] == "anglemove" && vct.size()==2)//根据多目标定位的阵列运动做旋转角的运动
			{
				if (devCheck->m_DevParam.m_angleTab.size() == 0)
				{
					data += " angle table no data";
					return false;
				}
				int mvid = atoi(vct[1].c_str());
				handle->moveAbs(mvid, devCheck->m_DevParam.m_anglePa * \
					devCheck->m_DevParam.m_angleTab[devCheck->m_DevParam.angleTabId]);
				devCheck->m_DevParam.angleTabId++;
				if (devCheck->m_DevParam.angleTabId == devCheck->m_DevParam.m_angleTab.size())
				{
					devCheck->m_DevParam.angleTabId = 0;
				}
			}
			else if (vct[0] == "cali" && vct.size() == 3)//根据标定获得位置运动
			{
				float px = atof(vct[1].c_str());
				float py = atof(vct[2].c_str());
				cv::Point2f pt;
				if (devCheck->m_CaliParam.getDistance(&px, &py, &pt.x, &pt.y))
					;
			}
			else if (vct[0] == "anglemm" && vct.size() == 2)
			{
				devCheck->m_DevParam.m_anglePa = atof(vct[1].c_str());
			}
			else
			{
				data = "error format:" + data;
				return false;
			}
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
		delete (checkPatch*)p2;
		p2 = NULL;
	}
	delete UCI::pDis; UCI::pDis = NULL;

}

void MoveDllWriteFile()
{

}


