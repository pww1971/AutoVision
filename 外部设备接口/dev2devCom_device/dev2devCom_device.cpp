// dev2devCom_device.cpp: 定义 DLL 的初始化例程。
//

#include "pch.h"
#include "framework.h"
#include "dev2devCom_device.h"

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

// Cdev2devComdeviceApp

BEGIN_MESSAGE_MAP(Cdev2devComdeviceApp, CWinApp)
END_MESSAGE_MAP()


// Cdev2devComdeviceApp 构造

Cdev2devComdeviceApp::Cdev2devComdeviceApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 Cdev2devComdeviceApp 对象

Cdev2devComdeviceApp theApp;


// Cdev2devComdeviceApp 初始化

BOOL Cdev2devComdeviceApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

string MoveDLLHelp()
{
	string szHelp = "\r\nCMV,Measure或CMV,AutoMeasure和CMV,Stop 发送检测指令给CMV视觉检测程序\r\nCMVgetData获取CMV测量结果\r\n\
CMV,CAMCHECK或CMV,DELAYTIME,1000和CMV,Stop 发送飞拍检测指令给CMV视觉检测程序\r\n";
	return szHelp;
}

bool MoveDLLIni(void* p, void*& p2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	S_SocketDevice* devCom;
	if (p2 == NULL)
	{
		if (p == NULL)
		{
			return false;
		}
		CMoveBase* mb = (CMoveBase*)p;
		devCom = new S_SocketDevice();
		/*string szDir = mb->m_szCfgAdd;
		int pos = szDir.size() - strlen("MoveCfg");
		szDir = szDir.substr(0, pos);
		devCom->m_cfgAdd = szDir + "reportdllcfg";
		devCom->loadcfg();*/
		p2 = devCom;
	}
	else
		devCom = (S_SocketDevice*)p2;
	return true;
}

void MoveDLLStop()
{

}
//动态库中提取的标函数
bool MoveDLLMove(void* p, void*& p2, string& data)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CMoveBase* mb = (CMoveBase*)p;
	
	if (NULL != mb)
	{
		S_SocketDevice* devCom;
		if (p2 == NULL)
		{
			devCom = new S_SocketDevice();
			/*string szDir = mb->m_szCfgAdd;
			int pos = szDir.size() - strlen("MoveCfg");
			szDir = szDir.substr(0, pos);
			devParam->m_cfgAdd = szDir + "reportdllcfg";
			devParam->loadcfg();*/
			p2 = devCom;
		}
		else
			devCom = (S_SocketDevice*)p2;
		std::vector<string> vct;
		CStrProcess::Split(data, ",", vct);
		if (vct[0] == "CMV")//给cmv发指令做运算或停止
		{
			data = "";
			S_SubscribeMsg msg;
			msg.szModelType = "CMV";//模块类型
			msg.szMsgType = vct[1].c_str();// "Measure";//消息类型
			if (vct.size()>2)
				msg.szMsgCont = vct[2];//消息内容 阵列或时间数据##分隔
			else if("CAMCHECK"==msg.szMsgType)
			{
				long lln = clock();
				char sz[32];
				sprintf(sz, "%ld", lln);
				msg.szMsgCont = sz;
			}
			devCom->SendMsg(msg);
		}
		else if (vct[0] == "CMVgetData")//提取cmv的结果
		{
			data = "";
			bool bIsOk = false;
			string szRet;
			if (devCom->getRet(szRet) > 0)
			{
				vector<string> vct;
				CStrProcess::Split(szRet, ",", vct);
				if (vct.size() < 5)
				{
					data += ":定位失败"; 
					mb->pushIoMsg(100, 1);//用100来标记okng显示
				}
				else
				{
					if ("OK" == vct[1])
					{
						bIsOk = true;
					}
					else
						mb->pushIoMsg(100, 1);//用100来标记okng显示
					int y = atof(vct[2].c_str());
					int x = atof(vct[3].c_str());
					int r = atof(vct[4].c_str());
					//电机运动


				}
				
				return bIsOk;
			}
			else
				return bIsOk;
		}
		else if (vct[0] == "阻塞测试")
		{
			S_SubscribeMsg msg;
			msg.szModelType = "NGSort";//模块类型
			msg.szMsgType = "action";//消息类型
			msg.szMsgCont = "阻塞测试";//消息内容
			devCom->m_Sync = false;//同步阻塞
			devCom->SendMsg(msg);
			while (!devCom->m_Sync)//同步阻塞 如果ngsort没有启动就很危险 系统会卡死
			{
				mysleep(10);
			}
		}
		else
		{
			data = "error format:" + data;
			return false;
		}
	}

	data = "";//返回非空主界面会显示
	return true;
}

//必须显示的调用释放内存
void MoveDllRelease(void*& p)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (NULL != p)
	{
		delete (S_SocketDevice*)p;
		p = NULL;
	}
}

void MoveDllWriteFile()
{

}

