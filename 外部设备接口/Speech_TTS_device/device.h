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
	//�²㴦����߳�ָ�봫��pBuzInterface 
	CVADTreat();
	virtual ~CVADTreat(void);

protected:
	// -------------------------------------------------------------------
	/// @����:  svc --- �̴߳�����
	// -------------------------------------------------------------------
	virtual int svc();
	virtual bool treat(string& text_heard);			//����
private:
	
	
public:
	bool m_runing;						//�߳�����״̬
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
	bool treat(string& text_heard);			//����
private:
	void speechPause();
public:
	string m_procAddress;
	string m_cfgAdd;
	void* pSpVoice;
	bool m_bMsg;//�򿪶���Ϣ�����
	string m_szMyName;//��������
	int m_iState;//����״̬ -1׼���������� 0 ��ʼ����ָ�� 
	long m_timeout;//��ʱ����׼������״̬ m_iState=-1
	long m_time_start, m_time_start2;
	CVisionSystem* m_mb;//
};

#endif