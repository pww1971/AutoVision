#pragma once

#include "pwwVision.h"
#include "afxcmn.h"
#include "Resource.h"
// CTcpIpDlg �Ի���

class CTcpIpDlg : public CDialog
{
	DECLARE_DYNAMIC(CTcpIpDlg)

public:
	CTcpIpDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTcpIpDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	BOOL OnInitDialog();
	float m_fNGRatio;
	string m_reportAddress;
	afx_msg void OnBnClickedYes();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	void* m_pData;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
