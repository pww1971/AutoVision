// CAM.h : CAM DLL ����ͷ�ļ�
//

#pragma once
#include "stdafx.h"
#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCAMApp
// �йش���ʵ�ֵ���Ϣ������� CAM.cpp
//

class CCAMApp : public CWinAppEx
{
public:
	CCAMApp();
protected:

	// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
extern CCAMApp theApp;