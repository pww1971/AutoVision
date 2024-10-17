#pragma once

#include "dlg_draw.h"
// pwwParamDlg 对话框
#include "afxpropertygridctrl.h"
#include <afxdialogex.h>
class pwwParamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(pwwParamDlg)

public:
	pwwParamDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~pwwParamDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PWW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	void* m_pCheck;//check对象
	virtual BOOL OnInitDialog();
	bool m_bsucc;
public:

#ifdef BGCOLOR
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CBrush m_brush;
#endif
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void checkHisMouse(CPoint& point);

	afx_msg void OnEnChangeEdit8();
	afx_msg void OnEnChangeEdit10();
	afx_msg void OnEnChangeEdit11();
	afx_msg void OnEnChangeEdit12();
	afx_msg void OnEnChangeEdit13();
	afx_msg void OnEnChangeEdit14();
	void setHisEditL(UINT uid, int id);
	void setHisEditH(UINT uid, int id);
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnBnClickedCheck5();
	void setHisCheck(UINT uid, int id);
	afx_msg void OnDeltaposSpin3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin4(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin5(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin6(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin7(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin8(NMHDR* pNMHDR, LRESULT* pResult);
	void setHisSpinL(UINT uid, int id, int iDelta);
	void setHisSpinH(UINT uid, int id, int iDelta);
	bool setHisMouseWheelL(UINT uid, int id, short zDelta);
	bool setHisMouseWheelH(UINT uid, int id, short zDelta);
	CMFCPropertyGridCtrl m_pwwProp;
	void updatePwwProp();
	void pwwPropIni();
	afx_msg LRESULT OnPropertyChanged(WPARAM, LPARAM);
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult);
private:
	void adjust(UINT cid, int id);

public:
	afx_msg void OnEnChangeEdit7();
	afx_msg void OnEnChangeEdit6();
};
