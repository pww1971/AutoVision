// export2Mes_device.h: export2Mes_device DLL 的主标头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"		// 主符号
#include "automove.h"
#include "CMesDlg.h"


#if (defined WIN32 || defined _WIN32 || defined WINCE)
#ifdef PROCLIB_EXPORTS
#define PROCLIB_API __declspec(dllexport)
#else
#define PROCLIB_API __declspec(dllimport)
#endif

#else
#define PROCLIB_API 
#endif

extern "C" PROCLIB_API string MoveDLLHelp();
extern "C" PROCLIB_API bool MoveDLLIni(void* pmv, void*& p2);
extern "C" PROCLIB_API void MoveDLLStop();
//动态库中提取的标函数
extern "C" PROCLIB_API bool MoveDLLMove(void* pmv, void*& p2, string & data);

//必须显示的调用释放内存
extern "C" PROCLIB_API void MoveDllRelease(void*& p2);
extern "C" PROCLIB_API void MoveDllWriteFile();
struct S_MesParamBase
{
	S_MesParamBase()
	{
		nSentAuth = 1;
		nCheckRe = 0;
		m_procAddress = "";
		m_cfgAdd = "";
		pwwMapClient.m_lTimeout = 1;//设置超时时间
		strIP = "10.1.8.201";
		strPNUMBER = "HL001";//产品编码
		strGXID = "B002";//工序编码
		strGXNAME = "螺母浮高检测";//工序名称
		strRESULT = "TUER";//检测结果
		strSTARTTIME= sysUsecTime4();//设备本次开始检测的时间
		strENDTIME= strSTARTTIME;//设备本次结束检测的时间
		strSUBMITTIME= strSTARTTIME;//提交数据时间
		strProID = "HL005 /HL063";
		strStaionName = "PINT";
		strProductName = "product_name";
		nSlotID = 1;
		strSN = "0235AFXU**H231017AA10G00931";
	}
	~S_MesParamBase()
	{

	}

	void loadcfg()
	{
		if ("" != m_cfgAdd)
		{
			CConfig cfg(m_cfgAdd);
			string szVal = cfg.getItem("m_procAddress");
			if ("" != szVal)
			{
				m_procAddress = szVal;
			}
			if ("" != m_procAddress)//
			{
			}
			szVal = cfg.getItem("strProID");
			if ("" != szVal)
			{
				strProID = szVal;
			}
			szVal = cfg.getItem("strIP");
			if ("" != szVal)
			{
				strIP = szVal;
			}
			
			szVal = cfg.getItem("strStaionName");
			if ("" != szVal)
			{
				strStaionName = szVal;
			}
			szVal = cfg.getItem("strProductName");
			if ("" != szVal)
			{
				strProductName = szVal;
			}
			szVal = cfg.getItem("nSlotID");
			if ("" != szVal)
			{
				nSlotID = atoi(szVal.c_str());
			}
			szVal = cfg.getItem("nSentAuth");
			if ("" != szVal)
			{
				nSentAuth = atoi(szVal.c_str());
			}
			szVal = cfg.getItem("nCheckRe");
			if ("" != szVal)
			{
				nCheckRe = atoi(szVal.c_str());
			}
		}

	};
	void writecfg()
	{
		if ("" != m_cfgAdd)
		{
			remove(m_cfgAdd.c_str());
			CConfig cfg(m_cfgAdd);
			cfg.updateItem("\n#制程地址", "");
			cfg.updateItem("m_procAddress", m_procAddress);
			cfg.updateItem("strProID", strProID);
			cfg.updateItem("strIP", strIP);
			cfg.updateItem("strStaionName", strStaionName);
			cfg.updateItem("strProductName", strProductName);
			cfg.updateItem("nSlotID", CStrProcess::itoa_radixN(nSlotID));
			cfg.updateItem("nSentAuth", CStrProcess::itoa_radixN(nSentAuth));
			cfg.updateItem("nCheckRe", CStrProcess::itoa_radixN(nCheckRe));
		}
	}
	//获取提交数据
	void getValue(Json::Value& value)
	{
		value["DATAFLAG"] = "01";//存储
		value["SN"] = strSN;
		value["PRID"] = strProID;
		value["PNUMBER"] = strPNUMBER;
		value["GXID"] = strGXID;
		value["GXNAME"] = strGXNAME;
		value["RESULT"] = strRESULT;
		value["STARTTIME"] = strSTARTTIME;
		value["ENDTIME"] = strENDTIME;
		value["SUBMITTIME"] = sysUsecTime4();
		char sz[32];
		for (int i = 0; i < vctother.size(); i++)
		{
			sprintf(sz, "QC%03d", i);
			value[sz] = vctother[i];
		}
	}
	string m_procAddress;
	string m_cfgAdd;
	treatPwwMap pwwMapClient;//pwwMap服务器地址在配置2文件中设置 默认127.0.0.1

	string strProID;//项目编码
	string strIP;// = 设备软件参数设置中的MES的IP地址值;
	string strStaionName;// = 设备软件参数设置中本设备的工位名称StationName值;
	string strProductName;// = 当前设备测试的产品名称（可选）;
	int nSlotID;// = 当前设备测试的槽位号（可选）;

	string strSN;// = 设备每轮测试的SN;
	string strPNUMBER;//产品编码
	string strGXID;//工序编码
	string strGXNAME;//工序名称
	string strRESULT;//检测结果
	string strSTARTTIME;//设备本次开始检测的时间
	string strENDTIME;//设备本次结束检测的时间
	string strSUBMITTIME;//提交数据时间
	vector<string> vctother;//其他数据

	int nSentAuth;//数据允许上传
	int nCheckRe;//本地查重

};

//后台上传MES系统
class S_DeviceParam:public CThread
{
public:
	S_DeviceParam()
	{
		m_szDescription = "transMsg线程";
		m_runing=m_bStop = false;
		open();
	};
	~S_DeviceParam()
	{
		m_bStop = true;
		for (int t = 0; t < 500; t++)
		{
			if (m_runing)
				mysleep(10);
			else
				break;
		}
		stop();
		while (1)
		{
			queue<Json::Value*> quMsg;
			int quLen = recvMsg(quMsg);
			if (0 == quLen)
				break;
			while (0 != quLen)
			{
				Json::Value* pMsg = (Json::Value*)quMsg.front();
				delete pMsg;
				quMsg.pop();
				--quLen;
			}
		}
	};

	// -------------------------------------------------------------------
	/// @描述:  pushMsg --- 下层调用，把消息传递到本层
	///
	/// @参数: pMsg --- 输入
	///
	/// @返回值:   true 成功传递，false 传递失败
	// -------------------------------------------------------------------
	bool pushMsg(Json::Value* pMsg)
	{
		m_Lock.lock();
		m_quMsg.push(pMsg);
		m_Lock.unLock();

		return true;
	}
public:
	bool m_bStop;

protected:
	// -------------------------------------------------------------------
	/// @描述:  svc --- 线程处理函数
	// -------------------------------------------------------------------
	virtual int svc()
	{
		m_bStop = false;
		m_runing = true;
		int quLen;
		queue<Json::Value*> quMsg;
		Json::Value* pMsg = NULL;
		while (!m_bStop)
		{
			sendHeartBeat();
			quLen = recvMsg(quMsg);
			if (0 == quLen)
			{
				mysleep(5);
				continue;
			}
			while (0 != quLen)
			{
				sendHeartBeat();

				pMsg = (Json::Value*)quMsg.front();
				if (m_bStop)
				{
					delete pMsg;
					quMsg.pop();
					--quLen;
					continue;
				}
				treat(pMsg);
				delete pMsg;
				quMsg.pop();
				--quLen;
			}
		}

		m_runing = false;
		return 0;
	};
	bool treat(Json::Value* value)
	{
		std::vector<std::string> szret;
		runhttpClient(value->toStyledString(), \
			"http://58.253.84.177:36118/execute?action=HLQC", szret);
		return true;
	};
	//从本层消息队列中取出100条消息进行处理
	u_int recvMsg(queue<Json::Value*>& quMsg)
	{
		u_int uiReceived, uiMaxRecv;

		m_Lock.lock();
		uiReceived = m_quMsg.size();
		if (0 == uiReceived)
		{
			m_Lock.unLock();
			return 0;
		}

		//判断是否将队列中的数据全部取出来
#ifdef MAX_MSG_ONE_RECV
		uiMaxRecv = (uiReceived < MAX_MSG_ONE_RECV) ? uiReceived : MAX_MSG_ONE_RECV;
		uiReceived = uiMaxRecv;
#else
		uiMaxRecv = uiReceived;
#endif

		for (; uiMaxRecv > 0; --uiMaxRecv)
		{
			quMsg.push(m_quMsg.front());
			m_quMsg.pop();
		}
		m_Lock.unLock();

		return uiReceived;
	};

protected:
	queue<Json::Value*> m_quMsg;					//下层传过来的消息队列

	CResLock m_Lock;
public:
	vector<Json::Value> vctother;//其他数据
	S_MesParamBase m_basePa;
	bool m_runing;
};

// Cexport2MesdeviceApp
// 有关此类实现的信息，请参阅 export2Mes_device.cpp
//

class Cexport2MesdeviceApp : public CWinApp
{
public:
	Cexport2MesdeviceApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
