#pragma once
#include "new_move_device.h"

#include "afxwinappex.h"
#include "afxdialogex.h"
// CConFigDlg 对话框

class CConFigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConFigDlg)

public:
	CConFigDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CConFigDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = "IDD_DIALOG1" };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	S_DeviceParam* m_pDevParam;
	virtual BOOL OnInitDialog();
};
