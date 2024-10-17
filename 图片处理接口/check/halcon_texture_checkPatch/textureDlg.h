#pragma once


// textureDlg 对话框
#include "afxdialogex.h"
#include "afxwin.h"
#include "afxcmn.h"
#include <HalconCpp.h>
#include "afxpropertygridctrl.h"
using namespace HalconCpp;
class textureDlg : public CDialogEx
{
	DECLARE_DYNAMIC(textureDlg)

public:
	textureDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~textureDlg();
	HImage m_ImageOrg;
	HTuple hv_WindowHandle;
	HWND m_halhWnd;
	void* m_pCheck;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void dev_open_window_fit_size(HTuple hv_Row, HTuple hv_Column, HTuple hv_Width,
		HTuple hv_Height, HTuple hv_WidthLimit, HTuple hv_HeightLimit, HTuple* hv_WindowHandle);
	bool iniHalWin();
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
};
