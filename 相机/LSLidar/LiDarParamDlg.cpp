// LiDarParamDlg.cpp: 实现文件
//

#include "pch.h"
#include "LSLidar.h"
#include "LiDarParamDlg.h"
#include "afxdialogex.h"
#include "CameraDrv.h"

// LiDarParamDlg 对话框

IMPLEMENT_DYNAMIC(LiDarParamDlg, CDialogEx)

LiDarParamDlg::LiDarParamDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	m_pCheck = NULL;
	m_dlgSucc = false;
}

LiDarParamDlg::~LiDarParamDlg()
{
}

void LiDarParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PIC, m_myPic);
}


BEGIN_MESSAGE_MAP(LiDarParamDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &LiDarParamDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &LiDarParamDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &LiDarParamDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// LiDarParamDlg 消息处理程序

#define DLLVIEW "LSLidar"//这里修改视图名称 防止其他dll共用一个视图

BOOL LiDarParamDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CameraDrv* pcheck = (CameraDrv*)m_pCheck;
	vector<cv::Mat> vm;
	pcheck->grapImage(0, vm);

	cv::namedWindow(DLLVIEW, cv::WINDOW_NORMAL);
	HWND hWnd = (HWND)cvGetWindowHandle(DLLVIEW);
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, m_myPic.GetSafeHwnd());
	::ShowWindow(hParent, SW_HIDE);
	RECT rect;
	m_myPic.GetClientRect(&rect);
	cv::resizeWindow(DLLVIEW, rect.right, rect.bottom);
	if (vm.size() > 0 && !vm[0].empty())
		imshow(DLLVIEW, vm[0]);
	char sz[32];
	sprintf(sz, "%.2f,%.2f,%.2f", pcheck->m_corner_min.x, pcheck->m_corner_min.y, pcheck->m_corner_min.z);
	GetDlgItem(IDC_EDIT1)->SetWindowTextA(sz);
	sprintf(sz, "%.2f,%.2f,%.2f", pcheck->m_corner_max.x, pcheck->m_corner_max.y, pcheck->m_corner_max.z);
	GetDlgItem(IDC_EDIT2)->SetWindowTextA(sz);
	sprintf(sz, "%.2f,%.2f,%.2f", pcheck->m_sensorCenter.x, pcheck->m_sensorCenter.y, pcheck->m_sensorCenter.z);
	GetDlgItem(IDC_EDIT4)->SetWindowTextA(sz);
	sprintf(sz, "%.2f", pcheck->m_gridXY);
	GetDlgItem(IDC_EDIT3)->SetWindowTextA(sz);
	sprintf(sz, "%d,%d", pcheck->m_angleL, pcheck->m_angleH);
	GetDlgItem(IDC_EDIT5)->SetWindowTextA(sz);
	GetDlgItem(IDC_EDIT6)->SetWindowTextA(pcheck->m_computerip.c_str());


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void LiDarParamDlg::updataParam()
{
	CameraDrv* pcheck = (CameraDrv*)m_pCheck;
	CString str;
	GetDlgItem(IDC_EDIT1)->GetWindowTextA(str);
	vector<string> vct;
	CStrProcess::Split(str.GetBuffer(0), ",", vct, true);
	if (vct.size() == 3)
	{
		float x = atof(vct[0].c_str());
		float y = atof(vct[1].c_str());
		float z = atof(vct[2].c_str());
		pcheck->m_corner_min = mrpt::math::TPoint3D(x, y, z);
	}
	GetDlgItem(IDC_EDIT2)->GetWindowTextA(str);
	vct.clear();
	CStrProcess::Split(str.GetBuffer(0), ",", vct, true);
	if (vct.size() == 3)
	{
		float x = atof(vct[0].c_str());
		float y = atof(vct[1].c_str());
		float z = atof(vct[2].c_str());
		pcheck->m_corner_max = mrpt::math::TPoint3D(x, y, z);
	}

	GetDlgItem(IDC_EDIT4)->GetWindowTextA(str);
	vct.clear();
	CStrProcess::Split(str.GetBuffer(0), ",", vct, true);
	if (vct.size() == 3)
	{
		float x = atof(vct[0].c_str());
		float y = atof(vct[1].c_str());
		float z = atof(vct[2].c_str());
		pcheck->m_sensorCenter = mrpt::math::TPoint3D(x, y, z);
	}

	GetDlgItem(IDC_EDIT3)->GetWindowTextA(str);
	pcheck->m_gridXY = atof(str.GetBuffer(0));

	GetDlgItem(IDC_EDIT5)->GetWindowTextA(str);
	vct.clear();
	CStrProcess::Split(str.GetBuffer(0), ",", vct, true);
	if (vct.size() == 2)
	{
		pcheck->m_angleL = atoi(vct[0].c_str());
		pcheck->m_angleH = atoi(vct[1].c_str());
	}

	GetDlgItem(IDC_EDIT6)->GetWindowTextA(str);
	pcheck->m_computerip = str.GetBuffer(0);
}

void LiDarParamDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	updataParam();
	((CameraDrv*)m_pCheck)->wirteParam();
	CDialogEx::OnOK();
}


void LiDarParamDlg::OnBnClickedButton1()
{
	CameraDrv* pcheck = (CameraDrv*)m_pCheck;
	updataParam();
	vector<cv::Mat> vm;
	pcheck->getGridMap(pcheck->m_sensorCenter);
	pcheck->grapImage(0, vm);
	if (vm.size() > 0 && !vm[0].empty())
		imshow(DLLVIEW, vm[0]);
}


void LiDarParamDlg::OnBnClickedButton2()
{
	((CameraDrv*)m_pCheck)->clearMap();
}
