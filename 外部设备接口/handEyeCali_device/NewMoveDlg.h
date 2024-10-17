#pragma once

#include "CaliDlg.h"
#include "checkPatch.h"
// CNewMoveDlg 对话框
#define ArrVIEW "arraymove"//这里修改视图名称 防止其他dll共用一个视图

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
private:
	CCaliDlg m_calidlg;
	CStatic m_myPic;

public:
	afx_msg void OnBnClickedButton1();
	void* m_pData;
	cv::Mat m_viewPic;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();
};
