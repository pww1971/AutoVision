#pragma once


// C3dMeatureDlg 对话框
#include "afxdialogex.h"
#include "afxwin.h"
#include ".\EditableListCtrl\EditableListCtrl.h"
#include "afxcmn.h"
#include <HalconCpp.h>
#include "afxpropertygridctrl.h"
using namespace HalconCpp;
class C3dMeatureDlg : public CDialogEx
{
	DECLARE_DYNAMIC(C3dMeatureDlg)

public:
	C3dMeatureDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~C3dMeatureDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	void* m_pCheck;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedBtDel();
	afx_msg void OnBnClickedBtDelall();

	CMFCPropertyGridCtrl m_prop;
	CEditableListCtrl m_procList;
	void updateProp();
	void propIni();
	afx_msg LRESULT OnPropertyChanged(WPARAM, LPARAM);
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedBtViewmode();
	afx_msg void OnLvnItemchangedList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
