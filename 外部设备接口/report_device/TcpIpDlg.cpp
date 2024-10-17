// TcpIpDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TcpIpDlg.h"
#include "new_move_device.h"
// CTcpIpDlg 对话框

IMPLEMENT_DYNAMIC(CTcpIpDlg, CDialog)

CTcpIpDlg::CTcpIpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTcpIpDlg::IDD, pParent)
{
	m_pData=NULL;
}

CTcpIpDlg::~CTcpIpDlg()
{
}

void CTcpIpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTcpIpDlg, CDialog)
	ON_BN_CLICKED(IDYES, &CTcpIpDlg::OnBnClickedYes)
	ON_BN_CLICKED(IDOK, &CTcpIpDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CTcpIpDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTcpIpDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTcpIpDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CTcpIpDlg 消息处理程序
BOOL CTcpIpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_reportAddress="";
	m_fNGRatio=0;
	if (NULL!=m_pData)
	{
		m_reportAddress=((S_DeviceParam*)m_pData)->m_reportAddress;
		m_fNGRatio=((S_DeviceParam*)m_pData)->m_fNGRatio;
	}
	CString str;
	str.Format("%.3f",m_fNGRatio);
	GetDlgItem(IDC_EDIT1)->SetWindowTextA(str);
	//GetDlgItem(IDC_EDIT1)->GetWindowTextA(m_reportAddress.c_str());
	return TRUE;  
}


void CTcpIpDlg::OnBnClickedYes()
{
	
}


void CTcpIpDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnOK();
}


void CTcpIpDlg::OnBnClickedButton1()
{
	CFileDialog fdlg1(true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "open file (*.*)|*.*||");

	if (fdlg1.DoModal() == IDOK)
	{
		m_reportAddress = fdlg1.GetPathName().GetBuffer(0);
	}
}


void CTcpIpDlg::OnBnClickedButton2()
{
	CString str;
	GetDlgItem(IDC_EDIT1)->GetWindowTextA(str);
	m_fNGRatio = atof(str.GetBuffer(0));
}


void CTcpIpDlg::OnBnClickedButton3()
{
	CString str;
	GetDlgItem(IDC_COMBO1)->GetWindowTextA(str);
	str = "UserDefined" + str;
	cv::namedWindow(str.GetBuffer(0), cv::WINDOW_NORMAL);
}
