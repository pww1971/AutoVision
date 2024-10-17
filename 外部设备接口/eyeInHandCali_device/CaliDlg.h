#pragma once


// CaliDlg 对话框
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "checkPatch.h"
#include "CaliDlg.h"
#include "pch.h"
#include "framework.h"
#include "eyeInHandCali_device.h"
#include "EditableListCtrl.h"

class CaliDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CaliDlg)

public:
	CaliDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CaliDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Cali };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtCali2();
	afx_msg void OnBnClickedBtCali();
	afx_msg void OnBnClickedBtCali3();
	CEditableListCtrl m_ppList;
	void viewList();
	void BtCali();
public:
	void* m_pData;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
