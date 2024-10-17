// RAY.h : new_move_device DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
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
//光源控制器
#include "automove.h"
#include<math.h>

using namespace std;
using namespace rp;
using namespace arch;
using namespace net;
/*
通讯协议如下：
波特率 19200
数据位 8
停止位 1
校验位 无
返回字符 ！

通讯指令示例如下：
四通道如：S100T150F200T250FC#
其中S为起始字符，T代表光源状态开，F代表光源状态灭，0～255级可调！一通道亮度级为100，二通道亮度级为150，三通道亮度级为200，四通道亮度级为250。
指令例：S100F150F200F250FC#
S100T150T200T250TC#
两通道如：S100T150TC#
其中S为起始字符，T代表光源状态开，F代表光源状态灭，0～255级可调！一通道亮度级为100，二通道亮度级为150。
*/
struct S_RayDevice
{
	S_RayDevice();
	~S_RayDevice();
	bool revData();
	bool getComNum();//自动检索光源的com口
	u_result connect(const char * port_path, _u32 baudrate);
	bool sendData(string st);
	bool closeRay();
	void disconnect();
	string getDefault();
	void setDefault(string st);
	bool isConnected();
	string getStr();
	string m_com;
	_u32 m_baudrate;
	string m_stopSt;//关闭命令
	string m_st;//默认方案 

	rp::arch::net::raw_serial m_rxtx;
	rp::hal::Locker  m_Lock;
	bool _isConnected;
};

struct S_vctRay
{
	S_vctRay();
	~S_vctRay();
	bool openRay(string st,int id);
	bool closeRay(int id);
	string getDefault(int id);
	void setDefault(string st,int id);
	bool openRay(int id);
	void release();
	bool loadCfg(string cfgfile);
	bool writeCfg(string cfgfile);

	vector<S_RayDevice*> m_vctRay;//所有光源
	int m_curRay;//当前光源

	bool addRay(string command);
private:
	bool loadRay(string command,int id);
	void codeStr(string& sendstr,int id);
};

extern S_vctRay g_Ray;
// Cnew_move_deviceApp
// 有关此类实现的信息，请参阅 new_move_device.cpp
//

class CRAYApp : public CWinApp
{
public:
	CRAYApp();

// 重写
public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};

extern "C" PROCLIB_API bool RayDllcloseRay( int id);
extern "C" PROCLIB_API bool RayDllopenRay(string st,int id);
extern "C" PROCLIB_API void RayDllWriteFile(string cfgfile);
extern "C" PROCLIB_API bool RayDLLLoadFile(string cfgfile);
extern "C" PROCLIB_API void RayDllRelease();
extern "C" PROCLIB_API bool RayDLLIni(string cfgfile);
extern "C" PROCLIB_API string RayDLLgetDefault(int id);
extern "C" PROCLIB_API void RayDLLsetDefault(string st,int id);
extern "C" PROCLIB_API int RayDLLgetRayNum();
