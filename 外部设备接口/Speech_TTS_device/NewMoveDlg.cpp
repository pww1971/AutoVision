// NewMoveDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NewMoveDlg.h"
#include "new_move_device.h"
// CNewMoveDlg 对话框

IMPLEMENT_DYNAMIC(CNewMoveDlg, CDialog)

CNewMoveDlg::CNewMoveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewMoveDlg::IDD, pParent)
{
	m_dlgSucc=false;
	m_pData=NULL;
}

CNewMoveDlg::~CNewMoveDlg()
{
}

void CNewMoveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PIC, m_myPic);
}

BEGIN_MESSAGE_MAP(CNewMoveDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CNewMoveDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CNewMoveDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CNewMoveDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CNewMoveDlg::OnBnClickedButton5)
END_MESSAGE_MAP()

BOOL CNewMoveDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	cv::namedWindow(DLLVIEW, cv::WINDOW_NORMAL);
	HWND hWnd = (HWND)cvGetWindowHandle( DLLVIEW);
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, m_myPic.GetSafeHwnd());
	::ShowWindow(hParent, SW_HIDE);
	RECT rect;
	m_myPic.GetClientRect(&rect);
	cv::resizeWindow(DLLVIEW, rect.right, rect.bottom);
	if (!m_rawPic.empty())
		imshow(DLLVIEW, m_rawPic);
	

	m_procAddress="";
	if (NULL!=m_pData)
	{
		m_procAddress=((CDevice*)m_pData)->m_procAddress;
	}
	GetDlgItem(IDC_EDIT1)->SetWindowTextA(m_procAddress.c_str());
	return TRUE; 
}

// CNewMoveDlg 消息处理程序

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
	//	szWinName = "视觉控制系统";
	//}
	//HWND hMainWnd = FindWindowA("#32770", szWinName.c_str());//通过遍历窗口方式获取目标进程主窗口句柄
	//if(hMainWnd!=0)
	//{
	//	Create(IDD_DIALOG1,CWnd::FromHandle(hMainWnd));
	//	//ShowWindow(0);
	//	m_dlgSucc = true;
	//	return true;
	//}
	//else
	//{
	//	AfxMessageBox("调用失败");
	//	return false;
	//}

	INT_PTR nResponse = DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}
	m_dlgSucc = true;
}

void CNewMoveDlg::OnBnClickedButton2()
{
//	m_tcpdlg.DoModal();//Create(IDD_DIALOG2);
	//m_tcpdlg.ShowWindow(1);
}

void CNewMoveDlg::OnBnClickedButton1()
{
	string localDir = getExePath();
	CFolderPickerDialog fd(localDir.c_str(), 0, this, 0);
	if (fd.DoModal() == IDOK)
	{
		m_procAddress = fd.GetPathName()+"/";
	}
	GetDlgItem(IDC_EDIT1)->SetWindowTextA(m_procAddress.c_str());
}


void CNewMoveDlg::OnBnClickedButton3()
{
	if (!m_rawPic.empty())
		imshow(DLLVIEW, m_rawPic);

}


void CNewMoveDlg::OnBnClickedButton5()
{
	//g_cali.EditModel(m_rawPic);
}
