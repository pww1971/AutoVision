#pragma once

#include "afxdialogex.h"
#include "EditableListCtrl.h"
#include "afxpropertygridctrl.h"
#include <HalconCpp.h>
using namespace HalconCpp;
// CDLPDlg 对话框

class CDLPDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDLPDlg)

public:
	CDLPDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDLPDlg();
	bool iniHalWin();

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
	HImage m_ImageOrg;
	HTuple hv_WindowHandle;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton6();
	afx_msg LRESULT OnPropertyChanged(WPARAM, LPARAM);
	CMFCPropertyGridCtrl m_prop;
	void propIni();
	void updateProp();

	afx_msg void OnBnClickedButton9();
};
