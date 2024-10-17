// CRangPaDlg.cpp: 实现文件
//

#include "pch.h"
#include "Monocular_Rang_checkPatch.h"
#include "CRangPaDlg.h"
#include "checkPatch.h"

// CRangPaDlg 对话框

IMPLEMENT_DYNAMIC(CRangPaDlg, CDialogEx)

CRangPaDlg::CRangPaDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent), m_pwwdlg(this)
{
	m_pCheck = NULL;
}

CRangPaDlg::~CRangPaDlg()
{
}

void CRangPaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PIC, m_myPic);

}


BEGIN_MESSAGE_MAP(CRangPaDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRangPaDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CRangPaDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK1, &CRangPaDlg::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CRangPaDlg 消息处理程序


BOOL CRangPaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	string szwinname = DLLVIEW;
	cv::namedWindow(szwinname.c_str(), cv::WINDOW_NORMAL);
	HWND hWnd = (HWND)cvGetWindowHandle(szwinname.c_str());
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, m_myPic.GetSafeHwnd());
	::ShowWindow(hParent, SW_HIDE);
	RECT rect;
	m_myPic.GetClientRect(&rect);
	cv::resizeWindow(szwinname.c_str(), rect.right, rect.bottom);
	cv::Mat& m = ((checkPatch*)m_pCheck)->m_myParam.m_curProc.m_rawPic[0];
	if(m.data)
		imshow(szwinname, m);
	CRect crt;
	GetDlgItem(IDC_STATIC_PWWDLG)->GetWindowRect(&crt);
	ScreenToClient(&crt);
	m_pwwdlg.m_pCheck = m_pCheck;
	m_pwwdlg.Create(IDD_DIALOG_PWW, this);
	m_pwwdlg.MoveWindow(crt.left, crt.top, crt.Width(), crt.Height());
	m_pwwdlg.ShowWindow(SW_SHOW);

	CString str;
	str.Format("%f", ((checkPatch*)m_pCheck)->m_f);
	GetDlgItem(IDC_EDIT1)->SetWindowTextA(str);
	str.Format("%f", ((checkPatch*)m_pCheck)->m_r);
	GetDlgItem(IDC_EDIT2)->SetWindowTextA(str);

	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(((checkPatch*)m_pCheck)->m_pixels == 0);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CRangPaDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	updateDat();
	CDialogEx::OnOK();
}

void CRangPaDlg::updateDat()
{
	CString str;
	GetDlgItem(IDC_EDIT1)->GetWindowTextA(str);
	((checkPatch*)m_pCheck)->m_f = atof(str.GetBuffer(0));
	GetDlgItem(IDC_EDIT2)->GetWindowTextA(str);
	((checkPatch*)m_pCheck)->m_r = atof(str.GetBuffer(0));

}
void CRangPaDlg::OnBnClickedButton1()
{
	updateDat();
	checkPatch* pCheck = (checkPatch*)m_pCheck;
	cv::Mat m = pCheck->m_myParam.m_curProc.m_rawPic[0].clone();
	double distance = pCheck->getdis(m);
	if (distance > -1)
	{
		char dis[50];
		sprintf(dis, "result : %.2f", distance);
		GetDlgItem(IDC_STATIC_INFO)->SetWindowTextA(dis);
		//cv::putText(m, dis, cv::Point(10, 30), 0, 1, cv::Scalar(0, 255, 0), 2);
		pCheck->draw(m);
	}
}


void CRangPaDlg::OnBnClickedCheck1()
{
	checkPatch* pCheck = (checkPatch*)m_pCheck;
	if (((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck())
	{
		pCheck->m_pixels = 0;
	}
	else
	{
		pCheck->m_pixels = 1;
	}
}

