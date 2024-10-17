// export2Mes_device.cpp: 定义 DLL 的初始化例程。
//

#include "pch.h"
#include "framework.h"
#include "export2Mes_device.h"



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

// Cexport2MesdeviceApp

BEGIN_MESSAGE_MAP(Cexport2MesdeviceApp, CWinApp)
END_MESSAGE_MAP()


// Cexport2MesdeviceApp 构造

Cexport2MesdeviceApp::Cexport2MesdeviceApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 Cexport2MesdeviceApp 对象

Cexport2MesdeviceApp theApp;


// Cexport2MesdeviceApp 初始化

BOOL Cexport2MesdeviceApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

string MoveDLLHelp()
{
	string szHelp = "\r\ncheckpic,id,id = 相机id，制程id,拍照检测图片 从xml结果中提取二维码数据进行匹配入库和查重,制程中‘绘图工具’改为‘QR码’否则提取错误\r\n\
设置查重,0=否 1=是\r\n上传Mes\r\n入站\r\n出站\r\n设置结果,0=ng 1=ok\r\n设置检测开始时间\r\n设置检测结束时间\r\n";
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
		devParam->m_basePa.m_cfgAdd = szDir + "mesdllcfg";//建一个子目录保存配置文件
		devParam->m_basePa.loadcfg();
		p2 = devParam;
	}
	else
		devParam = new S_DeviceParam();
	CMesDlg mydlg;
	mydlg.m_pData = p2;
	mydlg.m_auth = ((CVisionSystem*)mb->m_mainMB)->m_logMag->m_auth;
	if (IDOK == mydlg.DoModal())
	{
		devParam->m_basePa.writecfg();
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
			devParam->m_basePa.m_cfgAdd = szDir + "mesdllcfg";
			devParam->m_basePa.loadcfg();

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
			msg.m_procid = atoi(vctData[2].c_str());
			if (-1 == mb->m_pCheck->procCheck(&msg))//检测
			{
				data = "cam check fail：" + data;
				return false;
			}

			if (NULL != mb->m_tbReport)//报表
				mb->m_tbReport->pushMsg(&msg);

			//如果是二次制程里面的 就要从xml里面分析
			string xmlstr = getNgCont(msg.str, "QR码", OKSIGNSTR);//提取检测项目的xml
			devParam->m_basePa.strSN = xmlstr;
			if (0 == devParam->m_basePa.nCheckRe)//不查重
				return true;
			int iret = devParam->m_basePa.pwwMapClient.insertData(xmlstr);
			if (1 == iret)//1 ng 2 ok
			{
				data = "二维码重码:" + data;
				return false;
			}
			else if (2 == iret)//插入记录成功
				;
			else if (-1 == iret)
			{
				data = "解码错误:" + data;
				return false;
			}
			else//0==iret
			{
				data = "网络运算超时:" + data;
				return false;
			}
			
			return true;
		}
		else if ("设置查重" == vctData[0])
		{
			devParam->m_basePa.nCheckRe = atoi(vctData[1].c_str());
		}
		else if ("上传Mes" == vctData[0]) 
		{
			if (0 == devParam->m_basePa.nSentAuth)
				return false;
			//Json::Value value;
			//devParam->m_basePa.getValue(value);
			//
			//std::vector<std::string> szret;
			//runhttpClient(value.toStyledString(), \
			//	"http://58.253.84.177:36118/execute?action=HLQC", szret);

			//	data = szret[1];
			//	return true;

			Json::Value* pMsg = new Json::Value();
			devParam->m_basePa.getValue(*pMsg);
			devParam->pushMsg(pMsg);
		}
		else if ("入站" == vctData[0])
		{
			char strMSGInfo[MAX_PATH] = { 0 }; // 服务端接口执行返回的具体信息
			bool result;                  // 服务端接口执行的结果：true 成功，false 失败

			result = ValidateBoardWorkState(devParam->m_basePa.strIP.c_str(), devParam->m_basePa.strSN.c_str(), devParam->m_basePa.strStaionName.c_str(),\
				devParam->m_basePa.strProductName.c_str(), strMSGInfo, sizeof(strMSGInfo));
			return result;
		}
		else if ("出站" == vctData[0])
		{
			bool bTestResult = false;   // true 成功，false 失败
			char strMSGInfo[MAX_PATH] = { 0 }; // 服务端接口执行返回的具体信息
			bool result;                  // 服务端接口执行的结果：true 成功，false 失败

			result = SubmitBoardWorkState(devParam->m_basePa.strIP.c_str(), devParam->m_basePa.strSN.c_str(), devParam->m_basePa.strStaionName.c_str(), \
				devParam->m_basePa.strProductName.c_str(), devParam->m_basePa.nSlotID, bTestResult,\
				strMSGInfo, sizeof(strMSGInfo));
			return bTestResult;
		}
		else if ("设置结果" == vctData[0])
		{
			if ("0" == vctData[1])
				devParam->m_basePa.strRESULT = "FALSE";
			else
				devParam->m_basePa.strRESULT = "TRUE";
		}
		else if ("设置检测开始时间" == vctData[0])
		{
			devParam->m_basePa.strSTARTTIME = sysUsecTime4();
		}
		else if ("设置检测结束时间" == vctData[0])
		{
			devParam->m_basePa.strENDTIME = sysUsecTime4();
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
