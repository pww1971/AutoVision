#pragma once


// LiDarParamDlg 对话框
#include "pwwVision.h"
#include "afxdialogex.h"

class LiDarParamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LiDarParamDlg)

public:
	LiDarParamDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~LiDarParamDlg();
	bool m_dlgSucc;//界面初始化成功
	void* m_pCheck;//check对象
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CStatic m_myPic;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	void updataParam();
	afx_msg void OnBnClickedButton2();
};
