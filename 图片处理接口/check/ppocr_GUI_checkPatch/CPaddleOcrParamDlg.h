#pragma once

#include "resource.h"
// CPaddleOcrParamDlg 对话框
#include "stdafx.h"
#include "pwwVision.h"
class CPaddleOcrParamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPaddleOcrParamDlg)

public:
	CPaddleOcrParamDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPaddleOcrParamDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	void* m_pCheck;
	cv::Mat m_pic;
	bool setParam();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedBtDet();
	afx_msg void OnBnClickedBtRec();
	afx_msg void OnBnClickedBtCls();
	afx_msg void OnBnClickedBtDict();
	afx_msg void OnBnClickedButton2();
};
