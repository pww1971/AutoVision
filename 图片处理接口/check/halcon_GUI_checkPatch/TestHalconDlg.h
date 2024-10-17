
// TestHalconDlg.h : 头文件
//

#pragma once
#include "halconLocal.h"

#include "Design_GUI_checkPatch.h"
#include "afxwin.h"
#include <string>
#include <fstream>
using namespace std;
#include "resource.h"	
// CTestHalconDlg 对话框
class CTestHalconDlg : public CDialog
{
// 构造
public:
	CTestHalconDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TESTHALCON_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	bool m_dlgSucc;//界面初始化成功
	void* m_pCheck;//check对象

	//图像控件界面操作
	void moveShift();//移动窗体
	void Zoomfit();//恢复窗体
	void scaleState(bool s );//缩放窗体
	void ControlFunction(HTuple WindowHandleOrg,HTuple ZoomTrans,HTuple Row,HTuple Col,HTuple RowShif,HTuple ColShif);
	HTuple   m_WindowHandleOrg;		//窗口
	HObject m_ImageOrg;				//原始图像
	HTuple m_WidthOrg,m_HeightOrg;

	long	m_IDCShowImageW;		//控件宽和高
	long	m_IDCShowImageH;

	float m_Zoom;					//缩放记录
	float m_ZoomOrg;

	HTuple m_RowShifOrg;			//平移记录
	HTuple m_ColShifOrg;

//自定义算法操作
public:


	void guiIni();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnCbnSelchangeCombo3();
};
