// CAM.h : CAM DLL 的主头文件
//

#pragma once
#include "stdafx.h"
#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CCAMApp
// 有关此类实现的信息，请参阅 CAM.cpp
//

class CCAMApp : public CWinAppEx
{
public:
	CCAMApp();
protected:

	// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
extern CCAMApp theApp;