#pragma once

#include "pwwVision.h"
#include "afxcmn.h"
// CamSetRoi dialog
#define DLLVIEW "LSLIDARSETROI"//�����޸���ͼ���� ��ֹ����dll����һ����ͼ
void dllViewMain(cv::Mat& m,void* arg=0);
extern mouseKeyCtr g_MKC;
//����¼���Ӧ
void onMouse(int event, int x, int y, int flag=0, void*p=NULL);
// CamSetRoi �Ի���

class CamSetRoi : public CDialogEx
{
	DECLARE_DYNAMIC(CamSetRoi)

public:
	CamSetRoi(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CamSetRoi();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CStatic m_myPic;
	virtual BOOL OnInitDialog();
	bool m_dlgSucc;//�����ʼ���ɹ�
	void* m_pCheck;//check����
	afx_msg void OnBnClickedBtSetroi();
	afx_msg void OnBnClickedButton2();
	CListCtrl m_camList;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnCbnSelchangeComboCamid();
	void updateList();
	afx_msg void OnBnClickedBtIniall();
	afx_msg void OnBnClickedButton3();
};
