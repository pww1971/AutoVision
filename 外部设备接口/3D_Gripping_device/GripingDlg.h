#pragma once
#include "3D_Gripping_device.h"
#include "afxdialogex.h"
// GripingDlg 对话框
#  include "HalconCpp.h"
#  include "HDevThread.h"
#include "afxpropertygridctrl.h"

using namespace HalconCpp;
class GripingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(GripingDlg)

public:
	GripingDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~GripingDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif
	void* m_pCheck;//check对象

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton9();
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnPropertyChanged(WPARAM, LPARAM);
	CMFCPropertyGridCtrl m_prop;

	void propIni();
	void updateProp();
};
