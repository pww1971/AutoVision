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
	string szHelp = "check,id,id = 拍照检测,相机id,制程id getdata = 等待数据提取结束 阵列运动或触发阵列运动=ng位置形成阵列\r\n";
	return szHelp;
}

bool MoveDLLIni(void* pmv,void*& p2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (pmv == NULL)
	{
		return false;
	}
	CVisionSystem* mb = (CVisionSystem*)pmv;
	if (NULL==p2)
	{
		p2=new CDllPicTreat();
	}
	
	return true;
}

void MoveDLLStop()
{

}
//动态库中提取的标函数
bool MoveDLLMove(void* pmv,void*& p2,string& data)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CMoveBase* mb = (CMoveBase*)pmv;
	if (NULL != mb)
	{
		CServerConnect* myserver = mb->m_pCheck->m_server;
		if (NULL==p2)
		{
			p2=new CDllPicTreat();
		}
		CDllPicTreat* s_picTreat = (CDllPicTreat*)p2;
		if (NULL == s_picTreat->m_mvb)
			s_picTreat->m_mvb = mb;
		vector<string> vct;
		CStrProcess::Split(data,",",vct);
		if (vct[0] == "check" && vct.size()==3)//拍照检测 
		{
			S_Msg* pmsg = new S_Msg;
			cv::Point2f pt = mb->m_mtc.getCurPos();
			pmsg->pan_id = mb->m_prob+1;//区分是哪个流程的盘
			pmsg->x = pt.x;
			pmsg->y = pt.y;
			if (mb->m_mtc.m_prob==0)
			{
				pmsg->pan_x = pmsg->x/mb->m_mtc.m_mtxl;
				pmsg->pan_y = pmsg->y/mb->m_mtc.m_mtyl;
			}
			pmsg->arrMoveId = mb->m_arrMoveId;//阵列id 
			pmsg->m_procid = atoi(vct[2].c_str());
			int camid = atoi(vct[1].c_str());
			if (-1==camid)
			{
				camid = mb->m_pCheck->m_curCamId;
			}
			if(-1 == mb->m_pCheck->camPic(camid,pmsg->m) )
			{
				data="";
				return false;
			}
			
			//网络分机取图片检测
			static int  s_count2=1;
			if (myserver!=NULL && myserver->m_tcpNum > 0  && s_count2++%(myserver->m_tcpNum+1) > 0)
			{
				myserver->sendMsg(e_CAM_CHECK,pmsg);
				delete pmsg;
			}
			else
			{
				//推人后台就返回
				pmsg->pic = pmsg->m.clone();
				s_picTreat->pushMsg(pmsg);
				
				//mb->m_pTotCheck->procCheck(mb->m_pTotCheck->m_picProcId,m,&cor);
			}
		}
		else if (data == "getdata")//等待全部数据提取完成
		{
			//while ( !mb->m_mtc.mb->m_mtc.ArrIsEnd())//阵列结果采集完成
			//{
			//	Sleep(10);
			//}
			//deque<CoordinatePosXYZ*>::iterator itor = mb->m_mtc.m_deqRet.begin();
			//while (itor != mb->m_mtc.m_deqRet.end())
			//{
			//	if(3 != (*itor)->m_nc_ok)
			//		s_picTreat->m_vctNGPoint.push_back(cv::Point2f((*itor)->x, (*itor)->y));
			//	itor++;
			//}
			
			//也可以单步获取结果
			CoordinatePosXYZ* msg = mb->m_mtc.popArrMsg();
			if (NULL != msg)
			{
				if (3 != msg->m_nc_ok)
					s_picTreat->m_vctNGPoint.push_back(cv::Point2f(msg->x, msg->y));
				else
					delete msg;
			}
			else if (mb->m_mtc.ArrIsEnd())//阵列结果采集结束 数据已经提取空了
			{
			}
		}
		else if (std::string::npos != data.find(ARRAYMOVE))//根据ng数组生成新的阵列运动
		{
			if (s_picTreat->m_vctNGPoint.size()>0)
			{
				if (data == TRIGARRAYMOVE)//触发阵列运动
					mb->m_prob = 0;//触发阵列
				CArrayMove& mtc = mb->m_mtc;
				mtc.m_table.clear();
				mtc.m_prob = 1;//点系列
				mtc.m_table.assign(s_picTreat->m_vctNGPoint.begin(),s_picTreat->m_vctNGPoint.end());
				s_picTreat->m_vctNGPoint.clear();
				mtc.pointView();
			}
			else
			{
				data="";
				return false;
			}
		}
		else
		{
			
		}
	}
	data="";
	return true;
}

//必须显示的调用释放内存
void MoveDllRelease(void*& p2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (NULL!=p2)
	{
		delete (CDllPicTreat*)p2;
		p2=NULL;
	}
}

void MoveDllWriteFile()
{
	
}


