// Design_GUI_checkPatch.h : Design_GUI_checkPatch DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

//#include "resource.h"		// ������

// CTOT_GUI_checkPatchApp
// �йش���ʵ�ֵ���Ϣ������� Design_GUI_checkPatch.cpp
//

class CDesign_GUI_checkPatchApp : public CWinApp
{
public:
	CDesign_GUI_checkPatchApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
