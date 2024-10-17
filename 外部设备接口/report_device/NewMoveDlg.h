#pragma once

#include "TcpIpDlg.h"

// CNewMoveDlg 对话框

class CNewMoveDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewMoveDlg)

public:
	CNewMoveDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNewMoveDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	virtual BOOL OnInitDialog();

	bool iniDlg();
	bool m_dlgSucc;


private:
	CTcpIpDlg m_tcpdlg;
};
