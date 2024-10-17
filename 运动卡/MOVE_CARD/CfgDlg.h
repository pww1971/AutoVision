#pragma once
#include "afxcmn.h"
#include "EditableListCtrl.h"
#include "moveCard.h"

// CCfgDlg 对话框

class CCfgDlg : public CDialog
{
	DECLARE_DYNAMIC(CCfgDlg)

public:
	CCfgDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCfgDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtMtset();
	afx_msg void OnBnClickedBtAddmvpa();
	afx_msg void OnBnClickedBtDelmvpa();
	CEditableListCtrl m_listCarParam;
	void updateMoveCard(string cfgfile);
	void viewCarParam();//显示运动卡参数

#ifdef _LINHUA
	CLinHuaCardExt   m_handle;		//运动控制卡  
#else
#ifdef _ZMC
	CMoveCard m_handle;
#else
#ifdef _LTD
	CLTDCard m_handle;
#else 
#ifdef _GTS
	CGTSCard m_handle;
#else
#ifdef _MC_BP
	CMCBPMoveCard m_handle;
#else
	CMoveType m_handle;
#endif
#endif
#endif
#endif
#endif
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCbnSelchangeCombo1();
	float m_step;//步进距离
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedCheck2();
	int m_x,m_y;
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnCbnSelchangeCombo3();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
