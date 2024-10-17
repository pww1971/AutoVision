
// MFCApplication2Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication2.h"
#include "MFCApplication2Dlg.h"
#include "afxdialogex.h"
#include "pwwApp.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication2Dlg 对话框



CMFCApplication2Dlg::CMFCApplication2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BT_CHAINMove, &CMFCApplication2Dlg::OnBnClickedBtChainmove)
	ON_BN_CLICKED(IDC_BT_MVStop, &CMFCApplication2Dlg::OnBnClickedBtMvstop)
	ON_BN_CLICKED(IDC_BT_MVStop2, &CMFCApplication2Dlg::OnBnClickedBtMvstop2)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMFCApplication2Dlg 消息处理程序

BOOL CMFCApplication2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	string szviewname = "win_opencv1";
	cv::namedWindow(szviewname, cv::WINDOW_NORMAL);
	HWND hWnd = (HWND)cvGetWindowHandle(szviewname.c_str());;
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_STATIC1)->GetSafeHwnd());
	RECT rect;
	GetDlgItem(IDC_STATIC1)->GetClientRect(&rect);
	cv::resizeWindow(szviewname, rect.right, rect.bottom);
//	cv::setMouseCallback(szviewname, onMvMouse);
	::ShowWindow(hParent, SW_HIDE);

	szviewname = "motorview";
	cv::namedWindow(szviewname, cv::WINDOW_NORMAL);
	hWnd = (HWND)cvGetWindowHandle(szviewname.c_str());;
	hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_STATIC3)->GetSafeHwnd());
	GetDlgItem(IDC_STATIC3)->GetClientRect(&rect);
	cv::resizeWindow(szviewname, rect.right, rect.bottom);
	::ShowWindow(hParent, SW_HIDE);

	szviewname = "mvdemoView";
	cv::namedWindow(szviewname, cv::WINDOW_NORMAL);
	hWnd = (HWND)cvGetWindowHandle(szviewname.c_str());;
	hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_STATIC2)->GetSafeHwnd());
	GetDlgItem(IDC_STATIC2)->GetClientRect(&rect);
	cv::resizeWindow(szviewname, rect.right, rect.bottom);
	::ShowWindow(hParent, SW_HIDE);

	szviewname = "mvdemoView2";
	cv::namedWindow(szviewname, cv::WINDOW_NORMAL);
	hWnd = (HWND)cvGetWindowHandle(szviewname.c_str());;
	hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_STATIC)->GetSafeHwnd());
	GetDlgItem(IDC_STATIC)->GetClientRect(&rect);
	cv::resizeWindow(szviewname, rect.right, rect.bottom);
	::ShowWindow(hParent, SW_HIDE);

	test1();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplication2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication2Dlg::OnBnClickedBtChainmove()
{
	g_MB->m_io.ioTrigPause(false);
	string command = g_MB->m_szChainMv;
	if (command != "")
	{
		g_MB->updateProb();
		g_MB->pushMsg(command);
	}
}


void CMFCApplication2Dlg::OnBnClickedBtMvstop()
{
	g_MB->MvPause();
	if (g_MB->m_iPause)
	{
		GetDlgItem(IDC_BT_MVStop)->SetWindowText("继续");
	}
	else
	{
		GetDlgItem(IDC_BT_MVStop)->SetWindowText("暂停");
	}
}


void CMFCApplication2Dlg::OnBnClickedBtMvstop2()
{
	g_MB->Mvstop();
}


void CMFCApplication2Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if (NULL != g_MB)
	{
		delete g_MB;
		g_MB = NULL;
	}
}
