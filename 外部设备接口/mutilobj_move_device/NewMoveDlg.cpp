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
	HWND hWnd = (HWND)cvGetWindowHandle(DLLVIEW);
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, m_myPic.GetSafeHwnd());
	::ShowWindow(hParent, SW_HIDE);
	RECT rect;
	m_myPic.GetClientRect(&rect);
	cv::resizeWindow(DLLVIEW, rect.right, rect.bottom);
	m_viewPic = ((checkPatch*)m_pData)->m_rawPic;
	if (!m_viewPic.empty())
		imshow(DLLVIEW, m_viewPic);
	
	string procAddress="";
	if (NULL!=m_pData)
	{
		procAddress=((checkPatch*)m_pData)->m_DevParam.m_procAddress;
	}
	GetDlgItem(IDC_EDIT1)->SetWindowTextA(procAddress.c_str());

	m_calidlg.m_pData = m_pData;
	m_calidlg.Create(IDD_DIALOG_Cali);
	m_calidlg.ShowWindow(0);
	
	return TRUE; 
}

// CNewMoveDlg 消息处理程序


void CNewMoveDlg::OnBnClickedButton2()
{
	m_calidlg.ShowWindow(1);	
	m_calidlg.viewClientList();
	m_calidlg.m_ImageOrg = Mat2HObject(((checkPatch*)m_pData)->m_rawPic);
	m_calidlg.Zoomfit();
	ClearWindow(m_calidlg.m_WindowHandleOrg);
	m_calidlg.ControlFunction(m_calidlg.m_WindowHandleOrg,1,0,0,0,0);
	DispObj(m_calidlg.m_ImageOrg,m_calidlg.m_WindowHandleOrg);
}

void CNewMoveDlg::OnBnClickedButton1()
{
	string localDir = getExePath();
	CFolderPickerDialog fd(localDir.c_str(), 0, this, 0);
	if (fd.DoModal() == IDOK)
	{
		((checkPatch*)m_pData)->m_DevParam.m_procAddress = fd.GetPathName()+"/";
	}
	GetDlgItem(IDC_EDIT1)->SetWindowTextA(((checkPatch*)m_pData)->m_DevParam.m_procAddress.c_str());
}


void CNewMoveDlg::OnBnClickedButton3()
{
	if (m_pData!=NULL)
	{
		HImage ho_Image0 = Mat2HObject(((checkPatch*)m_pData)->m_rawPic);
		((checkPatch*)m_pData)->checkLocateCheck(ho_Image0);
	}
}


void CNewMoveDlg::OnBnClickedButton5()
{
	if (m_pData!=NULL)
		((checkPatch*)m_pData)->EditModel(m_viewPic);
}
