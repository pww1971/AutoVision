#pragma once

#include "resource.h"
#include "stdafx.h"

// CmosaicDlg 对话框

class CmosaicDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CmosaicDlg)

public:
	CmosaicDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CmosaicDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
