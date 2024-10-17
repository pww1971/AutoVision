#pragma once

#include "dlg_draw.h"
#include "Design_GUI_checkPatch.h"
#include "afxwin.h"

// CGuiContrlDlg 对话框

class CGuiContrlDlg : public CDialog
{
	DECLARE_DYNAMIC(CGuiContrlDlg)

public:
	CGuiContrlDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGuiContrlDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	bool m_dlgSucc;//界面初始化成功
	void* m_pCheck;//check对象
	CStatic m_myPic;

public:
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonadd();
	afx_msg void OnBnClickedButtondel();
	afx_msg void OnBnClickedButtonGenline();
	afx_msg void OnBnClickedButtonadd2();
	afx_msg void OnBnClickedButtonFill();
	afx_msg void OnBnClickedButtonScale();
	afx_msg void OnBnClicked1();
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangeCombo4();

	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void checkHisMouse(CPoint& point);

	afx_msg void OnEnChangeEdit15();
	afx_msg void OnEnChangeEdit16();
	void setHisEditL(UINT uid, int id);
	void setHisEditH(UINT uid, int id);
	afx_msg void OnBnClickedCheck6();
	void setHisCheck(UINT uid, int id);
	void setHisSpinL(UINT uid, int id, int iDelta);
	void setHisSpinH(UINT uid, int id, int iDelta);
	bool setHisMouseWheelL(UINT uid, int id, short zDelta);
	bool setHisMouseWheelH(UINT uid, int id, short zDelta);
	afx_msg void OnDeltaposSpin9(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin10(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton7();
};
#pragma once

