#pragma once

#include <afxdialogex.h>
//#include "EditableListCtrl/EditableListCtrl.h"
// CFaceVerifyDlg 对话框

class CFaceVerifyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFaceVerifyDlg)

public:
	CFaceVerifyDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CFaceVerifyDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void listUpdata();
	CListCtrl m_list;
	void* m_pFace;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
