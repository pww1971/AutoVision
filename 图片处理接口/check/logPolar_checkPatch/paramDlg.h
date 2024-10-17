#pragma once


// paramDlg 对话框
#include "afxdialogex.h"
#include "resource.h"
class paramDlg : public CDialogEx
{
	DECLARE_DYNAMIC(paramDlg)

public:
	paramDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~paramDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	void* m_pCheck;
	afx_msg void OnBnClickedOk();
};
