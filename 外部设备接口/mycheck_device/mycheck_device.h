// mycheck_device.h: mycheck_device DLL 的主标头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"		// 主符号
#include "automove.h"


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

struct S_DeviceParam
{
	S_DeviceParam()
	{
		m_procAddress = "";
		m_cfgAdd = "";
	}
	~S_DeviceParam()
	{

	}

	void loadcfg()
	{
		if ("" != m_cfgAdd)
		{
			CConfig cfg(m_cfgAdd);
			string szVal = cfg.getItem("procAddress");
			if ("" != szVal)
			{
				m_procAddress = szVal;
			}
			if ("" != m_procAddress)//目录中自动找到极坐标定位的配置文件和模板图片 并能加载成功 
			{
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
			cfg.updateItem("reportAddress", m_procAddress);
		}
	}
	string m_procAddress;
	string m_cfgAdd;

};

// CmycheckdeviceApp
// 有关此类实现的信息，请参阅 mycheck_device.cpp
//

class CmycheckdeviceApp : public CWinApp
{
public:
	CmycheckdeviceApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
