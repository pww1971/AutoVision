// Design_GUI_checkPatch.h : Design_GUI_checkPatch DLL ����ͷ�ļ�
//

#pragma once
#include "stdafx.h"
#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

// CTOT_GUI_checkPatchApp
// �йش���ʵ�ֵ���Ϣ������� Design_GUI_checkPatch.cpp
//

class CDesign_GUI_posApp : public CWinAppEx
{
public:
	CDesign_GUI_posApp();
protected:

	// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()

};
extern CDesign_GUI_posApp theApp;