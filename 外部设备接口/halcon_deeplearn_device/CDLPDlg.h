#pragma once

#include "afxdialogex.h"
#include "EditableListCtrl.h"

// CDLPDlg 对话框

class CDLPDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDLPDlg)

public:
	CDLPDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDLPDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	void* m_pCheck;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
	CEditableListCtrl m_nameList, m_procList;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnLvnItemchangedList2(NMHDR* pNMHDR, LRESULT* pResult);

	void updateDlg(int id);
	void setProc(int id);
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
};
