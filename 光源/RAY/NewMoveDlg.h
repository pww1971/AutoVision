#pragma once

#include "afxcmn.h"
#include "pwwVision.h"
// CNewMoveDlg 对话框
#define DLLVIEW "rayView"//这里修改视图名称 防止其他dll共用一个视图
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
	
public:
	void* m_pData;
	CListCtrl m_moveList;
	afx_msg void OnBnClickedButton1();
	void getListInfo();
	void delListInfo(int id);
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
};
