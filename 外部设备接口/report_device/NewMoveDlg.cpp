// NewMoveDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "new_move_device.h"
#include "NewMoveDlg.h"

// CNewMoveDlg �Ի���

IMPLEMENT_DYNAMIC(CNewMoveDlg, CDialog)

CNewMoveDlg::CNewMoveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewMoveDlg::IDD, pParent)
{
	m_dlgSucc=false;
}

CNewMoveDlg::~CNewMoveDlg()
{
}

void CNewMoveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNewMoveDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CNewMoveDlg::OnBnClickedButton2)
END_MESSAGE_MAP()

BOOL CNewMoveDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE; 
}

// CNewMoveDlg ��Ϣ�������

bool CNewMoveDlg::iniDlg()
{
	if (m_dlgSucc)
	{
		return true;
	}
	//CConfig cfg(getExePath() + NETCFG);
	//string szWinName = cfg.getItem("WINNAME");
	//if (szWinName == "")
	//{
	//	szWinName = "�Ӿ�����ϵͳ";
	//}
	//HWND hMainWnd = FindWindowA("#32770", szWinName.c_str());//ͨ���������ڷ�ʽ��ȡĿ����������ھ��
	//if(hMainWnd!=0)
	//{
	//	Create(IDD_DIALOG1,CWnd::FromHandle(hMainWnd));
	//	//ShowWindow(0);
	//	m_dlgSucc = true;
	//	return true;
	//}
	//else
	//{
	//	AfxMessageBox("����ʧ��");
	//	return false;
	//}

	INT_PTR nResponse = DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}
	return m_dlgSucc = true;
}

void CNewMoveDlg::OnBnClickedButton2()
{
	m_tcpdlg.DoModal();//Create(IDD_DIALOG2);
	//m_tcpdlg.ShowWindow(1);
}



