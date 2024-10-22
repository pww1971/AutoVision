// new_move_device.h : new_move_device DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
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
// �йش���ʵ�ֵ���Ϣ������� new_move_device.cpp
//
struct S_DeviceParam
{
	S_DeviceParam()
	{
		m_timer=clock();//���ö����ʱ�� �Ͷ��dll��
		m_count=0;//����UPHʱʹ�� �ϵ���Ŀ
		m_countOK=0;
		m_countNG=0;
		m_fNGRatio=0;//������
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
			cfg.updateItem("\n#�����ʱ�������","");
			char sz[64];
			sprintf(sz,"%f",m_fNGRatio);
			cfg.updateItem("NGRatio",sz);
			cfg.updateItem("\n#�����ַ","");
			cfg.updateItem("reportAddress",m_reportAddress);
		}
	};
	void uphZero()
	{
		m_lock.lock();
		m_countNG = m_countOK = m_count = 0;
		m_lock.unLock();
	}
	clock_t m_timer;//���ö����ʱ�� �Ͷ��dll��
	CResLock m_lock;
	u_int m_count;//����UPHʱʹ�� �ϵ���Ŀ
	u_int m_countOK;
	u_int m_countNG;
	float m_fNGRatio;//������
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
// ��д
public:
	virtual BOOL InitInstance();
//	CNewMoveDlg m_mainDlg;
	DECLARE_MESSAGE_MAP()
};

extern "C" PROCLIB_API string MoveDLLHelp();
extern "C" PROCLIB_API bool MoveDLLIni(void* p,void*& p2);
extern "C" PROCLIB_API void MoveDLLStop();
//��̬������ȡ�ı꺯��
extern "C" PROCLIB_API bool MoveDLLMove(void* p,void*& p2,string& data);

//������ʾ�ĵ����ͷ��ڴ�
extern "C" PROCLIB_API void MoveDllRelease(void*& p);
extern "C" PROCLIB_API void MoveDllWriteFile();
