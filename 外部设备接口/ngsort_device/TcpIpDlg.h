#pragma once

#include "pwwVision.h"
#include "afxcmn.h"
#include "Resource.h"
// CTcpIpDlg 对话框

class CTcpIpDlg : public CDialog
{
	DECLARE_DYNAMIC(CTcpIpDlg)

public:
	CTcpIpDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTcpIpDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL OnInitDialog();
	afx_msg void OnBnClickedBtServerip();

	void viewClientList();
	CListCtrl m_socketList;

	BOOL   SetAutoRun(CString   strPath);//开机自动运行 
	void OnCancelAuto() ;
	afx_msg void OnBnClickedBtAuto();
	afx_msg void OnBnClickedBtDelauto();
};
