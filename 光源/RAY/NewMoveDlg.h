#pragma once

#include "afxcmn.h"
#include "pwwVision.h"
// CNewMoveDlg �Ի���
#define DLLVIEW "rayView"//�����޸���ͼ���� ��ֹ����dll����һ����ͼ
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
	
public:
	void* m_pData;
	CListCtrl m_moveList;
	afx_msg void OnBnClickedButton1();
	void getListInfo();
	void delListInfo(int id);
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
};
