// Design_GUI_checkPatch.h : Design_GUI_checkPatch DLL 的主头文件
//

#pragma once
#include "stdafx.h"
#ifndef __AFXWIN_H__
#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

// CTOT_GUI_checkPatchApp
// 有关此类实现的信息，请参阅 Design_GUI_checkPatch.cpp
//

class CDesign_GUI_posApp : public CWinAppEx
{
public:
	CDesign_GUI_posApp();
protected:

	// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()

};
extern CDesign_GUI_posApp theApp;