// TOT_GUI_SDI_checkPatch.cpp : 定义 DLL 的初始化例程。
//

#include "Design_GUI_pos.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "checkPatch.h"

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

//动态库中提取的标函数
extern "C" PROCLIB_API bool DesignDLLTreatPicture(CTpltParam& pa, vector<S_TpltData*>& vctData);
bool DesignDLLTreatPicture(CTpltParam& pa, vector<S_TpltData*>& vctData)
{ 
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	try
	{
		int curid = pa.m_tempPa.m_curPosID;
		cv::Mat& src = vctData[curid]->m_vctPic[0];
		S_DllParam& halPa = pa.m_dllPa;
		vctData[curid]->m_szRet = "";

		if (halPa.ini<1)//记录模版大小
		{
			if (halPa.ini == 0 && "" != halPa.szfile)
			{
				if (halPa.p==NULL)
					halPa.p = new checkPatch();
				else
				{
					((checkPatch*)pa.m_dllPa.p)->clearMode();
				}

				((checkPatch*)pa.m_dllPa.p)->readMode(halPa.szfile);
				vctData[curid]->m_roiRect.size.width = src.cols;
				vctData[curid]->m_roiRect.size.height = src.rows;

				halPa.ini=1;
				return true;
			}
			else if (halPa.ini == -1)//未初始化的
			{
				if (NULL!=pa.m_dllPa.p)//已经有初始化内存 说明是clone过来的 为了兼容其他dll必须clone内存
				{
					delete (checkPatch*)pa.m_dllPa.p;
					pa.m_dllPa.p = NULL;
				}
			}
			vctData[curid]->m_roiRect.size.width = src.cols;
			vctData[curid]->m_roiRect.size.height =src.rows;

			/*string szRawPic = getRawPath(halPa.szfile);
			cv::Mat m = imread(szRawPic,IMREAD_GRAYSCALE);

			cv::namedWindow(VIEWWINNAME, cv::WINDOW_NORMAL);
			cv::imshow(VIEWWINNAME,m);
			cv::waitKey(1);*/

			halPa.ini=1;
			return true;
		}
		else
		{
			cv::Mat dst=src;
			if (8 == pa.m_preTreat)//通道选择红
			{
				if (dst.channels()>1)
				{
					cv::Mat m[3];
					cv::split(dst,m);
					dst = m[0];
				}
			}
			else if (9 == pa.m_preTreat)//通道选择绿
			{
				if (dst.channels()>1)
				{
					cv::Mat m[3];
					cv::split(dst,m);
					dst = m[1];
				}
			}
			else if (10 == pa.m_preTreat)//通道选择蓝
			{
				if (dst.channels()>1)
				{
					cv::Mat m[3];
					cv::split(dst,m);
					dst = m[2];
				}
			}
			else if (11 == pa.m_preTreat)//灰度
			{
				if (dst.channels()>1)
					cv::cvtColor(dst, dst, cv::COLOR_RGB2GRAY);
			}
			/*if (src.channels()>1)
			{
			if (halPa.szParam!="")
			{
			cv::Mat m[3];
			cv::split(src,m);
			int id = atoi(halPa.szParam.c_str());
			if (id >-1 && id < 3)
			{
			dst = m[id];
			}
			else
			dst = m[0];
			}
			else
			cv::cvtColor(src, dst, cv::COLOR_RGB2GRAY);
			}*/
			if (halPa.p==NULL)
			{
				halPa.p = new checkPatch();
				((checkPatch*)pa.m_dllPa.p)->readMode(halPa.szfile);
			//	cv::imwrite("1.jpg",src);
				//编辑模板
				if(((checkPatch*)pa.m_dllPa.p)->EditModel(dst) )
				{
					//writeMode(pa.m_dllPa.szfile);
				}
			}
			
			//定位检测
			if (((checkPatch*)pa.m_dllPa.p)->checkLocateCheck(dst, vctData, pa))
			{
				
			}
			else
			{
				return false;
			}
		}
	}
	catch (...)
	{
		MessageBox(0,"动态库（3Part.dll）定位失败", "系统提示", MB_YESNO | MB_ICONQUESTION) ;
		return false;
	}
	return true;
}

extern "C" PROCLIB_API bool DesignDLLClone(CTpltParam& pa,void*p);
bool DesignDLLClone(CTpltParam& pa,void*p)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//这句必不可少
	/*S_DllParam& halPa = pa.m_dllPa;
	if (halPa.p == NULL)
		halPa.p = new checkPatch();
	else
	{
		((checkPatch*)pa.m_dllPa.p)->clearMode();
	}

	((checkPatch*)pa.m_dllPa.p)->readMode(((checkPatch*)p)->m_cfgDir);*/
	return false;
}

extern "C" PROCLIB_API void DesignDllRelease(CTpltParam& pa);
void DesignDllRelease(CTpltParam& pa)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	if (pa.m_dllPa.ini == 1)
	{
		try
		{
			if (NULL!=pa.m_dllPa.p)
			{
				delete (checkPatch*)pa.m_dllPa.p;
				pa.m_dllPa.p = NULL;
			}
		}
		catch (...)
		{
			MessageBox(0,"释放动态库（3Part.dll）失败", "系统提示", MB_YESNO | MB_ICONQUESTION) ;
			return;
		}
		pa.m_dllPa.sel = NULL;
		pa.m_dllPa.ini = 0;
	}
}

extern "C" PROCLIB_API void DesignDllWriteFile(CTpltParam& pa);
void DesignDllWriteFile(CTpltParam& pa)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	if (pa.m_dllPa.ini == 1)
	{
		try
		{
			if (NULL!=pa.m_dllPa.p)
			{
				if (pa.m_dllPa.ini == 1 && "" != pa.m_dllPa.szfile)
				{
					((checkPatch*)pa.m_dllPa.p)->writeMode(pa.m_dllPa.szfile);
				}
			}
		}
		catch (...)
		{
			MessageBox(0,"动态库（3Part.dll）写文件失败", "系统提示", MB_YESNO | MB_ICONQUESTION) ;
			return;
		}
	}

}

extern "C" PROCLIB_API bool DesignDllDraw(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa);
bool DesignDllDraw(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (pa.m_dllPa.p != NULL)
		return ((checkPatch*)pa.m_dllPa.p)->drawResult(src,roiRect,pa);
	return false;
}

BEGIN_MESSAGE_MAP(CDesign_GUI_posApp, CWinAppEx)
END_MESSAGE_MAP()

CDesign_GUI_posApp::CDesign_GUI_posApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
	SetSystem("parallelize_operators", "true");
}


// 唯一的一个 CTOT_GUI_SDI_checkPatchApp 对象

CDesign_GUI_posApp theApp;


// CTOT_GUI_SDI_checkPatchApp 初始化

BOOL CDesign_GUI_posApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
//	CShellManager *pShellManager = new CShellManager;
	
	m_pMainWnd = &checkPatch::S_dlg;
	UCI::pDis = new ST_MYDISP(&checkPatch::S_dlg);
	UCI::initFirst();

	return TRUE;
}



int CDesign_GUI_posApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	//if (pShellManager != NULL)
	//{
	//	delete pShellManager;
	//}

	return CWinAppEx::ExitInstance();
}