// new_move_device.cpp : 定义 DLL 的初始化例程。
//

#include "mutilobj_move_device.h"
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
	string szHelp = "\r\n阵列运动, camid = 拍照检测多目标定位检测ok位置形成新的阵列\r\nmove,mvid = mvid电机旋转运动	\
		\r\nmaskObjArr,camid = 拍照检测位置形成阵列";
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
		if (vct.size() < 2)
		{
			data = "error format:" + data;
			return false;
		}
		else if (std::string::npos != vct[0].find(ARRAYMOVE))//根据ok数组生成新的阵列运动
		{
			CoordinatePosXYZ msg;
			int camid = atoi(vct[1].c_str());
			if (-1 == camid)
			{
				camid = mb->m_pCheck->m_curCamId;
			}
			if (-1 == mb->m_pCheck->camPic(camid, msg.vm))
			{
				data = "cam  fail：" + data;
				return false;
			}
			msg.m_procid = mb->m_pCheck->m_picProcId;
			S_MultiObjRet mtRet;
			if (-1 == mb->m_pCheck->procCheck(&msg, mtRet))
			{
				data = "cam check fail：" + data;
				return false;
			}
			CArrayMove& mtc = mb->m_mtc;//新阵列数据
			if (vct[0] == TRIGARRAYMOVE)//触发阵列运动
				mb->m_prob = 0;//触发阵列

			mtc.m_table.clear();
			mtc.m_tbid = 0;//表当前位置从新开始
			mtc.m_prob = 1;//点系列
			//提取ok位置   
			cv::Point2f* dis = mtc.m_handle->getMvScope();//根据电机运行范围计算电机位置
			float wr = (dis[mtc.m_ix].y - dis[mtc.m_ix].x) / msg.vm[0].cols;//电机距离和图像的比例
			float hr = (dis[mtc.m_iy].y - dis[mtc.m_iy].x) / msg.vm[0].rows;
			for (int i = 0; i < mtRet.objNum; i++)
			{
				if (mtRet.label[i] == 1)//ok
				{
					//计算电机的位置，考虑负限位的距离
					mtc.m_table.push_back(cv::Point2f(mtRet.x[i] * wr + dis[mtc.m_ix].x, mtRet.y[i] * hr + dis[mtc.m_iy].x));
				}
			}
			S_MultiObjRet& mtRet2 = ((CDevice*)p2)->m_mtRet;
			mtRet2.rememini(mtc.m_table.size());
			int j = 0;
			for (int i = 0; i < mtRet.objNum; i++)
			{
				if (mtRet.label[i] == 1)//ok
				{
					mtRet2.x[j] = mtc.m_table[j].x;
					mtRet2.y[j] = mtc.m_table[j].y;
					mtRet2.z[j] = mtRet.z[i];
					j++;
				}
			}
			mtc.pointView();
		}
		else if ( "move" == vct[0])
		{
			int mtid = atoi(vct[1].c_str());
			S_MultiObjRet& mtRet = ((CDevice*)p2)->m_mtRet;
			CArrayMove& mtc = mb->m_mtc;//新阵列数据
			CMoveDllType* handle = mb->m_mtc.m_handle;
			int tbid = mtc.getCurPosId();
			if (-1 == tbid)
			{
				data = "没有运动位置 " + data;
				return false;
			}
			int step = mtRet.z[tbid];
			handle->moveAbs(mtid, step);
			CoordinatePosXYZ pd;
			pd.x = mtRet.x[tbid];
			pd.y = mtRet.y[tbid];
			mtc.viewPt(true,pd);
		}
		else if ("maskObjArr"== vct[0])//根据ok数组生成新的阵列运动
		{
			CoordinatePosXYZ msg;
			int camid = atoi(vct[1].c_str());
			if (-1 == camid)
			{
				camid = mb->m_pCheck->m_curCamId;
			}
			if (-1 == mb->m_pCheck->camPic(camid, msg.vm))
			{
				data = "cam  fail：" + data;
				return false;
			}
			msg.m_procid = mb->m_pCheck->m_picProcId;
			if (-1 == mb->m_pCheck->procCheck(&msg))
			{
				data = "cam check fail：" + data;
				return false;
			}
			CArrayMove& mtc = mb->m_mtc;//新阵列数据
			if (vct[0] == TRIGARRAYMOVE)//触发阵列运动
				mb->m_prob = 0;//触发阵列

			mtc.m_table.clear();
			mtc.m_tbid = 0;//表当前位置从新开始
			mtc.m_prob = 1;//点系列
			//提取ok位置   
			cv::Point2f* dis = mtc.m_handle->getMvScope();//根据电机运行范围计算电机位置
			float wr = (dis[mtc.m_ix].y - dis[mtc.m_ix].x) / msg.vm[0].cols;//电机距离和图像的比例
			float hr = (dis[mtc.m_iy].y - dis[mtc.m_iy].x) / msg.vm[0].rows;
			std::vector<std::vector<cv::RotatedRect>> vvRRect;
			mb->m_pCheck->m_vctMkc[msg.m_procid]->m_curProc.getMaskPos(vvRRect, true);
			for (int i = 0; i < vvRRect[0].size(); i++)//只用第一定位结果目标
			{
				//计算电机的位置，考虑负限位的距离
				mtc.m_table.push_back(cv::Point2f(vvRRect[0][i].center.x * wr + dis[mtc.m_ix].x, vvRRect[0][i].center.y * hr + dis[mtc.m_iy].x));
			}
			S_MultiObjRet& mtRet2 = ((CDevice*)p2)->m_mtRet;
			mtRet2.rememini(mtc.m_table.size());
			int j = 0;
			for (int i = 0; i < vvRRect[0].size(); i++)
			{
		//		if (mtRet.label[i] == 1)//ok
				{
					mtRet2.x[j] = mtc.m_table[j].x;
					mtRet2.y[j] = mtc.m_table[j].y;
		//			mtRet2.z[j] = mtRet.z[i];
					j++;
				}
			}
			mtc.pointView();
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


