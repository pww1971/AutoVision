#ifndef __DEVICE_H__
#define __DEVICE_H__

#include "stdafx.h"
#include "automove.h"
//#include "common.h"
//#include "common-sdl.h"
#pragma comment(lib, "SDL2.lib") 
#include <regex>
#include <Model.h>
#include <Audio.h>
#pragma comment(lib, "fastasr.lib") 
#pragma comment(lib, "libopenblas.lib") 
#pragma comment(lib, "libfftw3l-3.lib") 
#pragma comment(lib, "libfftw3f-3.lib") 
#pragma comment(lib, "libfftw3-3.lib") 
#include <Mmsystem.h>
#pragma comment(lib, "winmm.lib") 
#include "sapi.h"    
#pragma comment(lib, "sapi.lib")


class CVADTreat:public CThread
{
public:
	//下层处理的线程指针传递pBuzInterface 
	CVADTreat();
	virtual ~CVADTreat(void);

protected:
	// -------------------------------------------------------------------
	/// @描述:  svc --- 线程处理函数
	// -------------------------------------------------------------------
	virtual int svc();
	virtual bool treat(string& text_heard);			//处理
private:
	
	
public:
	bool m_runing;						//线程运行状态
	bool m_bStop;
};

class CDevice:public CVADTreat
{
public:
	CDevice();
	virtual ~CDevice();
	void loadcfg();
	void writecfg();

	void Speech(CString str, bool clear = false);
protected:
	bool treat(string& text_heard);			//处理
private:
	void speechPause();
public:
	string m_procAddress;
	string m_cfgAdd;
	void* pSpVoice;
	bool m_bMsg;//打开短消息命令集合
	string m_szMyName;//机器名称
	int m_iState;//接收状态 -1准备接收名称 0 开始接收指令 
	long m_timeout;//超时返回准备接收状态 m_iState=-1
	long m_time_start, m_time_start2;
	CVisionSystem* m_mb;//
};

#endif