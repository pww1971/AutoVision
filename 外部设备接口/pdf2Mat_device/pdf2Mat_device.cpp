// pdf2Mat_device.cpp: 定义 DLL 的初始化例程。
//

#include "pch.h"
#include "framework.h"
#include "pdf2Mat_device.h"

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

// Cpdf2MatdeviceApp

BEGIN_MESSAGE_MAP(Cpdf2MatdeviceApp, CWinApp)
END_MESSAGE_MAP()


// Cpdf2MatdeviceApp 构造

Cpdf2MatdeviceApp::Cpdf2MatdeviceApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 Cpdf2MatdeviceApp 对象

Cpdf2MatdeviceApp theApp;


// Cpdf2MatdeviceApp 初始化

BOOL Cpdf2MatdeviceApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


string MoveDLLHelp()
{
	string szHelp = "\r\npdf2mat,文件名 = pdf文件转图片\r\ngerber2mat,50 pcb板文件转图像，50是单像素距离微米";
	return szHelp;
}

bool MoveDLLIni(void* pmv, void*& p2)//界面调用的初始化
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (pmv == NULL)
	{
		return false;
	}
	CVisionSystem* mb = (CVisionSystem*)pmv;
	S_DeviceParam* devParam;
	if (p2 == NULL)
	{
		devParam = new S_DeviceParam();
		string szDir = mb->m_szCfgAdd;//配置文件的主目录
		int pos = szDir.size() - strlen("MoveCfg");
		szDir = szDir.substr(0, pos);
		devParam->m_cfgAdd = szDir + "pdf2matdllcfg";//建一个子目录保存配置文件
		devParam->loadcfg();
		p2 = devParam;
	}
	else
		devParam = new S_DeviceParam();

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
		S_DeviceParam* devParam;
		if (p2 == NULL)//初始化
		{
			devParam = new S_DeviceParam();
			string szDir = mb->m_szCfgAdd;
			int pos = szDir.size() - strlen("MoveCfg");
			szDir = szDir.substr(0, pos);
			devParam->m_cfgAdd = szDir + "pdf2matdllcfg";
			devParam->loadcfg();

			p2 = devParam;
		}
		else
			devParam = (S_DeviceParam*)p2;
		CMoveDllType* handle = mb->m_mtc.m_handle;
		std::vector<string> vct;
		CStrProcess::Split(data, ",", vct);
		if (vct[0] == "pdf2mat")//
		{
			string str = getExePath()+"pdfdir/" + vct[1]+".pdf";
			CoordinatePosXYZ msg;
			if (!PDF2PNG(str, msg.vm))
			{
				data = "pdf2mat fail：" + data;
			}
			mb->m_pCheck->viewWin(0, msg.vm);
			msg.m_procid = mb->m_pCheck->m_picProcId;
			if (-1 == mb->m_pCheck->procCheck(&msg))//检测
			{
				data = "check fail：" + data;
				return false;
			}

			if (NULL != mb->m_tbReport)//报表
				mb->m_tbReport->pushMsg(&msg);
		}
		else if (vct[0] == "gerber2mat")
		{
			string szpath = getExePath();
			string strComd = szpath + "gerber2image.exe --gerber_file " + szpath + "b0257507-01a.top ";
			if (vct[1] != "")
			{
				strComd = strComd + "--um_pixel "+vct[1];
			}
			WinExec(strComd.c_str(), SW_HIDE);
			auto image_file = szpath + "b0257507-01a.top_0_0.jpg";//0和0是分解的大图片的标记
			string szSignFile = szpath + "b0257507-01a.toptmp";//原文件名加tmp后缀做完成标记
			vector<cv::Mat> vm;
			cv::Mat m;
			while (_access(image_file.c_str(),0)!=-1)
			{
				m = imread(image_file.c_str());
				if (m.data)
				{
					vm.push_back(m);
					break;
				}
			}
			mb->m_pCheck->viewWin(0,vm);

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
		delete (S_DeviceParam*)p2;
		p2 = NULL;
	}
}

void MoveDllWriteFile()
{

}
