// 3D_Gripping_device.cpp: 定义 DLL 的初始化例程。
//

#include "pch.h"
#include "framework.h"
#include "3D_Gripping_device.h"

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

// CMy3DGrippingdeviceApp

BEGIN_MESSAGE_MAP(CMy3DGrippingdeviceApp, CWinApp)
END_MESSAGE_MAP()

int (*HCamGrapImage)(int id, std::vector<HImage>& ho_Image) = NULL;//先定义函数指针
int (*RobotCom)(string str) = NULL;//机械臂的通讯控制 
// CMy3dPosdeviceApp 构造
HImage Point2img()//读取点云数据 转为tif图像
{
	HTuple  ObjectModel3D, hv_TriangulatedObjectModel3D, hv_Information;
	HTuple X, Y, Z;//读取点云
	GenObjectModel3dFromPoints(X, Y, Z, &ObjectModel3D);//转为点云图
	//三角化   三维重建 表面比较光滑的  密度一样的
	TriangulateObjectModel3d(ObjectModel3D, "greedy", HTuple(), HTuple(),
		&hv_TriangulatedObjectModel3D, &hv_Information);
	//	SelectPointsObjectModel3d(hv_TriangulatedObjectModel3D, "point_coord_x", pa.bgL, pa.bgH, &X);
	//	SelectPointsObjectModel3d(hv_TriangulatedObjectModel3D, "point_coord_y", pa.bgL, pa.bgH, &Y);
	//	SelectPointsObjectModel3d(hv_TriangulatedObjectModel3D, "point_coord_z", pa.bgL, pa.bgH, &Z);
		//创建一个空图像, 注意type选择'real'
	HImage ImageConst;
	HTuple Width, Height;
	GenImageConst(&ImageConst, 'real', Width, Height);
	//然后使用数组填充图像, 这样就完成了
	SetGrayval(ImageConst, X, Y, Z);//xyz是读取的点云坐标
	return ImageConst;
}
// CMy3DGrippingdeviceApp 构造

CMy3DGrippingdeviceApp::CMy3DGrippingdeviceApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CMy3DGrippingdeviceApp 对象

CMy3DGrippingdeviceApp theApp;


// CMy3DGrippingdeviceApp 初始化

BOOL CMy3DGrippingdeviceApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

string MoveDLLHelp()
{
	string szHelp = "\r\ncheck,-1,-1 = 拍照检测,相机id,制程id\r\n";
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
	//if (NULL == RobotCom)//运动卡里的机械手臂调用接口
	//{
	//	if (!mb->m_mtc.m_handle->GetFunction("RobotCom", (DynaLink::Function&)RobotCom)) //从dll中加载函数
	//	{
	//		AfxMessageBox("RobotCom 加载失败");
	//		return false;
	//	}
	//}

	if (NULL == p2)
	{
		p2 = new CDevice();
	}

	if (NULL != mb)
	{
		if (NULL == HCamGrapImage)
		{
			if (!mb->m_pCheck->m_cam.GetFunction("HCamDLLgrapImage", (DynaLink::Function&)HCamGrapImage)) //从dll中加载函数
			{
				AfxMessageBox("HCamDLLgrapImage 加载失败");
				return false;
			}
		}
		if (-1 == HCamGrapImage(mb->m_pCheck->m_curCamId, ((CDevice*)p2)->m_hImage) || ((CDevice*)p2)->m_hImage.size() < 2)//直接调用该函数
		{
			AfxMessageBox("HCam 拍照失败");
			return false;
		}
		//vector<cv::Mat> vm;
		//if (-1 == mb->m_pCheck->camPic(6, vm))//拍照
		//{
		//	AfxMessageBox("Cam 拍照失败");
		//	return false;
		//}
		//((CDevice*)p2)->m_hImage[0] = Mat2HObject(vm[0]);
	}

	//配置文件放在运动制程文件夹
	string szDir = mb->m_mainMB->m_szCfgAdd;
	int pos = szDir.size() - strlen("MoveCfg");
	szDir = szDir.substr(0, pos);
	((CDevice*)p2)->readMode(szDir);
	//编辑模板
	if (((CDevice*)p2)->EditModel())
	{
		((CDevice*)p2)->writeMode(szDir);
		((CDevice*)p2)->iniMode();
	}

	S_CheckParam& pa = ((CDevice*)p2)->m_DLPa;
	if (!pa.m_iniSucc)
	{
		string data = ((CDevice*)p2)->m_cfgDir + " 3d抓取点模板加载失败！";
		AfxMessageBox(data.c_str());
		delete p2;
		p2 = NULL;
		return false;
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
			szDir = szDir.substr(0, pos);
			((CDevice*)p2)->readMode(szDir);
			S_CheckParam& pa = ((CDevice*)p2)->m_DLPa;
			if (!pa.m_iniSucc)
			{
				data = ((CDevice*)p2)->m_cfgDir + " 3d抓取点模板加载失败！";
				AfxMessageBox(data.c_str());
				delete p2;
				p2 = NULL;
				return false;
			}
		}
		std::vector<string> vct;
		CStrProcess::Split(data, ",", vct);
		if (vct.size() != 3)
		{
			data = "error format:" + data;
			return false;
		}
		else if (vct[0] == "check")//拍照检测 
		{
			int camid = atoi(vct[1].c_str());
			if (-1 == camid)
			{
				camid = mb->m_pCheck->m_curCamId;
			}

			if (NULL != mb)
			{
				if (NULL == HCamGrapImage)
					if (!mb->m_pCheck->m_cam.GetFunction("HCamDLLgrapImage", (DynaLink::Function&)HCamGrapImage)) //从dll中加载函数
					{
						data = "camdll load HCamGrapImage failed:" + data;
						return false;
					}

				if (-1 == HCamGrapImage(camid, ((CDevice*)p2)->m_hImage) || ((CDevice*)p2)->m_hImage.size() < 2)//直接调用该函数
				{
					data = "cam open fail：" + data;
					return false;
				}
			}

			int procid = atoi(vct[2].c_str());
			if (procid < 0)
			{
				procid = 0;
			}
			S_Msg msg;
			msg.m_procid = procid;//制程名称 报表入库时会对应制程表
			msg.raw = msg.vm[0].clone();

			bool bret = ((CDevice*)p2)->checkLocateCheck(msg);
			if (bret)
			{
				data = "";
				mb->m_pCheck->viewWin(procid, msg.vm);//使用制程的界面 因此需要保证制程表有空制程或与制程共画面
				if (NULL != mb->m_tbReport && (1 == mb->m_tbReport->m_bWriteOk || !bret))
				{
					mb->m_tbReport->pushMsg(&msg);
				}
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

