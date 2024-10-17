#pragma once

#include "EditableListCtrl.h"
#include "checkPatch.h"
#include "resource.h"
// CTcpIpDlg 对话框
#define CaliVIEW "cali"//这里修改视图名称 防止其他dll共用一个视图

class CCaliDlg : public CDialog
{
	DECLARE_DYNAMIC(CCaliDlg)

public:
	CCaliDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCaliDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_Cali };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
#ifdef USE_HALCON
	//图像控件界面操作
	void moveShift();//移动窗体
	void Zoomfit();//恢复窗体
	void scaleState(bool s );//缩放窗体
	void ControlFunction(HTuple WindowHandleOrg,HTuple ZoomTrans,HTuple Row,HTuple Col,HTuple RowShif,HTuple ColShif);
	HTuple   m_WindowHandleOrg;		//窗口
	HImage m_ImageOrg;				//原始图像
	HTuple m_WidthOrg,m_HeightOrg;	

	long	m_IDCShowImageW;		//控件宽和高
	long	m_IDCShowImageH;

	float m_Zoom;					//缩放记录
	float m_ZoomOrg;

	HTuple m_RowShifOrg;			//平移记录
	HTuple m_ColShifOrg;
#else
	cv::Mat m_viewPic;
#endif
public:
	BOOL OnInitDialog();

	void viewClientList();//参数显示到list上
	void listToParam();//list传入参数
	CEditableListCtrl m_ppList;

	afx_msg void OnBnClickedBtAdd();
	afx_msg void OnBnClickedBtCali();
	afx_msg void OnBnClickedBtCali2();
	void* m_pData;

private:
	CStatic m_myPic;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtRoi();
	afx_msg void OnBnClickedBtRoi2();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtDel();
};
