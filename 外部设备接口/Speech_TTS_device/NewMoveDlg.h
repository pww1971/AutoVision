#pragma once

#include "TcpIpDlg.h"
// CNewMoveDlg 对话框
#define DLLVIEW "arraymove"//这里修改视图名称 防止其他dll共用一个视图

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
	CStatic m_myPic;

public:
	afx_msg void OnBnClickedButton1();
	void* m_pData;
	string m_procAddress;
	cv::Mat m_rawPic;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();

};
