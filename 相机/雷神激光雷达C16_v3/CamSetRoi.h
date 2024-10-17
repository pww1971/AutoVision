#pragma once

#include "pwwVision.h"
#include "afxcmn.h"
// CamSetRoi dialog
#define DLLVIEW "LSLIDARSETROI"//这里修改视图名称 防止其他dll共用一个视图
void dllViewMain(cv::Mat& m,void* arg=0);
extern mouseKeyCtr g_MKC;
//鼠标事件响应
void onMouse(int event, int x, int y, int flag=0, void*p=NULL);
// CamSetRoi 对话框

class CamSetRoi : public CDialogEx
{
	DECLARE_DYNAMIC(CamSetRoi)

public:
	CamSetRoi(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CamSetRoi();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CStatic m_myPic;
	virtual BOOL OnInitDialog();
	bool m_dlgSucc;//界面初始化成功
	void* m_pCheck;//check对象
	afx_msg void OnBnClickedBtSetroi();
	afx_msg void OnBnClickedButton2();
	CListCtrl m_camList;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnCbnSelchangeComboCamid();
	void updateList();
	afx_msg void OnBnClickedBtIniall();
	afx_msg void OnBnClickedButton3();
};
