// mycheck_device.cpp: 定义 DLL 的初始化例程。
//

#include "pch.h"
#include "framework.h"
#include "mycheck_device.h"

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

// CmycheckdeviceApp

BEGIN_MESSAGE_MAP(CmycheckdeviceApp, CWinApp)
END_MESSAGE_MAP()


// CmycheckdeviceApp 构造

CmycheckdeviceApp::CmycheckdeviceApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CmycheckdeviceApp 对象

CmycheckdeviceApp theApp;


// CmycheckdeviceApp 初始化

BOOL CmycheckdeviceApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}



string MoveDLLHelp()
{
	string szHelp = "\r\ncheckpic,id = 相机id，拍照检测图片\r\n";
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
		devParam->m_cfgAdd = szDir + "mycheckdllcfg";//建一个子目录保存配置文件
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
			devParam->m_cfgAdd = szDir + "mycheckdllcfg";
			devParam->loadcfg();

			p2 = devParam;
		}
		else
			devParam = (S_DeviceParam*)p2;
		CMoveDllType* handle = mb->m_mtc.m_handle;
		std::vector<string> vctData;
		CStrProcess::Split(data, ",", vctData);
		if (vctData[0] == "checkpic")//拍照检测 
		{
			int camid = atoi(vctData[1].c_str());
			CoordinatePosXYZ msg;
			if (-1 == mb->m_pCheck->camPic(camid, msg.vm))//拍照
			{
				data = "";
				return false;
			}
			msg.m_procid = mb->m_pCheck->m_picProcId;//这里可以输入制程id
			S_MultiObjRet mtRet;
			if (-1 == mb->m_pCheck->procCheck(&msg, mtRet))//检测
			{
				data = "cam check fail：" + data;
				return false;
			}

			if (NULL != mb->m_tbReport)//报表
				mb->m_tbReport->pushMsg(&msg);
						
			//分类
			int oknum = 0;
			for (int i = 0; i < mtRet.objNum; i++)
			{
				if (mtRet.label[i] == 1)//ok的目标
				{
					oknum++;
					//位置
					//cv::Point2f(mtRet.x[i] ,  mtRet.y[i]);
				}
			}
			data = "结果数目=" + CStrProcess::itoa_radixN(oknum);

			//如果是二次制程里面的 就要从xml里面分析
			string xmlstr = getNgCont(msg.str, "绘图工具", OKSIGNSTR);//提取检测项目的xml
			vector<string> vct;
			getNgType(xmlstr,  vct,  OKSIGNSTR);//分析xml的ok检测项目名 保存到vct
			if (vct.size()>0)
				data = "结果数目=" + CStrProcess::itoa_radixN(vct.size());

			{//另外一种方式
				vector<string> vct2;
				getInnerXml(msg.str, vct2, 0);//提取第一层的二次制程xml
				if (vct2.size() > 0)
				{
					vct.clear();
					getNgType(vct2[0], vct, OKSIGNSTR);//分析xml的ok检测项目名 保存到vct
					if (vct.size() > 0)
						data = "结果数目=" + CStrProcess::itoa_radixN(vct.size());
				}
			}
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
		delete (S_DeviceParam*)p2;
		p2 = NULL;
	}
}

void MoveDllWriteFile()
{

}
