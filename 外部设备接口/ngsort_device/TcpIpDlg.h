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
	afx_msg void OnBnClickedBtServerip();

	void viewClientList();
	CListCtrl m_socketList;

	BOOL   SetAutoRun(CString   strPath);//�����Զ����� 
	void OnCancelAuto() ;
	afx_msg void OnBnClickedBtAuto();
	afx_msg void OnBnClickedBtDelauto();
};
