#pragma once

#include "TcpIpDlg.h"
// CNewMoveDlg �Ի���
#define DLLVIEW "arraymove"//�����޸���ͼ���� ��ֹ����dll����һ����ͼ

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
	bool iniDlg();
	bool m_dlgSucc;
private:
	CTcpIpDlg m_tcpdlg;
	CStatic m_myPic;

public:
	afx_msg void OnBnClickedButton1();
	void* m_pData;
	string m_procAddress;
	cv::Mat m_rawPic;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();

};
