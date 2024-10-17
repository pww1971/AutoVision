#pragma once

#include "pwwVision.h"
#include "afxdialogex.h"
// CRulesDlg 对话框

class CRulesDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRulesDlg)

public:
	CRulesDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CRulesDlg();

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
	cv::Mat m_pic;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
};
