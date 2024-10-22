// new_move_device.h : new_move_device DLL ����ͷ�ļ�
//

#pragma once
#include "stdafx.h"
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

class Cnew_move_deviceApp : public CWinAppEx
{
public:
	Cnew_move_deviceApp();

// ��д
public:
	virtual BOOL InitInstance();
//	CNewMoveDlg m_mainDlg;
	DECLARE_MESSAGE_MAP()
	
	virtual int ExitInstance();
};

extern "C" PROCLIB_API string MoveDLLHelp();
extern "C" PROCLIB_API bool MoveDLLIni(void* pmv,void*& p2);
extern "C" PROCLIB_API void MoveDLLStop();
//��̬������ȡ�ı꺯��
extern "C" PROCLIB_API bool MoveDLLMove(void* pmv,void*& p2,string& data);

//������ʾ�ĵ����ͷ��ڴ�
extern "C" PROCLIB_API void MoveDllRelease(void*& p2);
extern "C" PROCLIB_API void MoveDllWriteFile();
