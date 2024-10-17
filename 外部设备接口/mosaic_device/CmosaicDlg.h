#pragma once


// CmosaicDlg 对话框
#include "afxpropertygridctrl.h"
#include <afxdialogex.h>
#include "EditableListCtrl/EditableListCtrl.h"
class CmosaicDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CmosaicDlg)

public:
	CmosaicDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CmosaicDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMFCPropertyGridCtrl m_paramProp;
	virtual BOOL OnInitDialog();
	void listUpdata();
	void propIni();
	void updateProp();
	void enableProp();
	void* m_pCheck;
	afx_msg LRESULT OnPropertyChanged(WPARAM, LPARAM);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CEditableListCtrl m_pariList;
	afx_msg void OnBnClickedBtProcadd();
	afx_msg void OnBnClickedBtProcmdy();
	afx_msg void OnBnClickedBtProcdel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtProcadd2();
	afx_msg void OnBnClickedBtProcdel2();
};
