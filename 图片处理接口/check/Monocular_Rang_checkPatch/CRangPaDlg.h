#pragma once


// CRangPaDlg 对话框
#include "afxdialogex.h"
#include "resource.h"
#include "pwwDlg/pwwParamDlg.h"

class CRangPaDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRangPaDlg)

public:
	CRangPaDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CRangPaDlg();

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
	CStatic m_myPic;
	pwwParamDlg m_pwwdlg;
	afx_msg void OnBnClickedButton1();
	void updateDat();
	afx_msg void OnBnClickedCheck1();
};
