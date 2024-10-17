// new_move_device.h : new_move_device DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "device.h"
#include "NewMoveDlg.h"
#if (defined WIN32 || defined _WIN32 || defined WINCE)
#ifdef PROCLIB_EXPORTS
#define PROCLIB_API __declspec(dllexport)
#else
#define PROCLIB_API __declspec(dllimport)
#endif

#else
#define PROCLIB_API 
#endif


// Cnew_move_deviceApp
// 有关此类实现的信息，请参阅 new_move_device.cpp
//
struct S_DeviceParam
{
	S_DeviceParam()
	{
		m_timer=clock();//想用多个计时器 就多个dll吧
		m_count=0;//计算UPH时使用 料的数目
		m_countOK=0;
		m_countNG=0;
		m_fNGRatio=0;//不良率
		m_reportAddress=getExePath()+"template1.xlsx";
		m_cfgAdd="";
	}
	void loadcfg()
	{
		if (""!=m_cfgAdd)
		{
			CConfig cfg(m_cfgAdd);
			string szVal = cfg.getItem("NGRatio");
			if (""!=szVal)
			{
				m_fNGRatio = atof(szVal.c_str());
			}
			szVal = cfg.getItem("reportAddress");
			if (""!=szVal)
			{
				m_reportAddress = szVal;
			}
		}
	};
	void writecfg()
	{
		if (""!=m_cfgAdd)
		{
			remove(m_cfgAdd.c_str());
			CConfig cfg(m_cfgAdd);
			cfg.updateItem("\n#不良率报警门限","");
			char sz[64];
			sprintf(sz,"%f",m_fNGRatio);
			cfg.updateItem("NGRatio",sz);
			cfg.updateItem("\n#报表地址","");
			cfg.updateItem("reportAddress",m_reportAddress);
		}
	};
	void uphZero()
	{
		m_lock.lock();
		m_countNG = m_countOK = m_count = 0;
		m_lock.unLock();
	}
	clock_t m_timer;//想用多个计时器 就多个dll吧
	CResLock m_lock;
	u_int m_count;//计算UPH时使用 料的数目
	u_int m_countOK;
	u_int m_countNG;
	float m_fNGRatio;//不良率
	string m_reportAddress;
	string m_cfgAdd;
};

class Cnew_move_deviceApp : public CWinApp
{
public:
	Cnew_move_deviceApp();
	~Cnew_move_deviceApp()
	{
		CoUninitialize();
	}
// 重写
public:
	virtual BOOL InitInstance();
//	CNewMoveDlg m_mainDlg;
	DECLARE_MESSAGE_MAP()
};

extern "C" PROCLIB_API string MoveDLLHelp();
extern "C" PROCLIB_API bool MoveDLLIni(void* p,void*& p2);
extern "C" PROCLIB_API void MoveDLLStop();
//动态库中提取的标函数
extern "C" PROCLIB_API bool MoveDLLMove(void* p,void*& p2,string& data);

//必须显示的调用释放内存
extern "C" PROCLIB_API void MoveDllRelease(void*& p);
extern "C" PROCLIB_API void MoveDllWriteFile();
