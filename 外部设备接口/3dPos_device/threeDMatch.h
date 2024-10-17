#pragma once

#include "3dPos_device.h"
#include "afxdialogex.h"
// threeDMatch 对话框
#  include "HalconCpp.h"
#  include "HDevThread.h"
#include "EditableListCtrl.h"

using namespace HalconCpp;

class threeDMatch : public CDialogEx
{
	DECLARE_DYNAMIC(threeDMatch)

public:
	threeDMatch(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~threeDMatch();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	void updateDlg(int id);
	void getDlgParam();
	void setProc(int id);

public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

public:
	void* m_pCheck;//check对象
	CStatic m_viewStatic;
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnClose();
	CEditableListCtrl m_procList;
	afx_msg void OnLvnItemchangedList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedBtBuildmode();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtDel();
	afx_msg void OnBnClickedBtDelall();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedBtViewmode();
};
