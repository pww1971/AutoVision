// MOVE_CARD.h : MOVE_CARD DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMOVE_CARDApp
// �йش���ʵ�ֵ���Ϣ������� MOVE_CARD.cpp
//

class CMOVE_CARDApp : public CWinApp
{
public:
	CMOVE_CARDApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
