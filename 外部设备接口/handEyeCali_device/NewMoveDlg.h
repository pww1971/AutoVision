#pragma once

#include "CaliDlg.h"
#include "checkPatch.h"
// CNewMoveDlg �Ի���
#define ArrVIEW "arraymove"//�����޸���ͼ���� ��ֹ����dll����һ����ͼ

class CNewMoveDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewMoveDlg)

public:
	CNewMoveDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNewMoveDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	virtual BOOL OnInitDialog();
private:
	CCaliDlg m_calidlg;
	CStatic m_myPic;

public:
	afx_msg void OnBnClickedButton1();
	void* m_pData;
	cv::Mat m_viewPic;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();
};
