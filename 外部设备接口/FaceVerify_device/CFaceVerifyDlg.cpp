// CFaceVerifyDlg.cpp: 实现文件
//

#include "pch.h"
#include "FaceVerify_device.h"
#include "CFaceVerifyDlg.h"
#include "afxdialogex.h"
#include "FaceLivingDetector.h"
using namespace FaceLivingDetector;
// CFaceVerifyDlg 对话框

IMPLEMENT_DYNAMIC(CFaceVerifyDlg, CDialogEx)

CFaceVerifyDlg::CFaceVerifyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	m_pFace = NULL;
}

CFaceVerifyDlg::~CFaceVerifyDlg()
{
}

void CFaceVerifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CFaceVerifyDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CFaceVerifyDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CFaceVerifyDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CFaceVerifyDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CFaceVerifyDlg 消息处理程序


BOOL CFaceVerifyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	FaceFeatureExtractor *pFace = (FaceFeatureExtractor*)m_pFace;
	string szwinname = pFace->m_szWinName;
	RECT rect;
	cv::namedWindow(szwinname, cv::WINDOW_NORMAL);
	HWND hWnd = (HWND)cvGetWindowHandle(szwinname.c_str());;
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_STATIC_PIC)->GetSafeHwnd());
	::ShowWindow(hParent, SW_HIDE);
	GetDlgItem(IDC_STATIC_PIC)->GetClientRect(&rect);
	cv::resizeWindow(szwinname, rect.right, rect.bottom);
	CProcDll* m_Cam = pFace->m_mb->m_mainMB->m_pCheck;//相机
	std::vector<cv::Mat> vm;
	if (-1 != m_Cam->camPic(m_Cam->m_curCamId, vm))
	{
		showMat(szwinname, vm[0]);
	}
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.InsertColumn(0, "name", LVCFMT_CENTER, 120);
	listUpdata();
	std::vector<std::vector<string>> vctName;//数据库的名称组
	pFace->m_mb->m_logMag->getUser(vctName);
	((CComboBox*)GetDlgItem(IDC_COMBO1))->ResetContent();
	for (auto szName : vctName)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(szName[0].c_str());
	}
	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetWindowTextA(pFace->m_mb->m_logMag->m_user.c_str());

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CFaceVerifyDlg::listUpdata()
{
	std::unordered_map< string, matrix<float, 0, 1>>& facelib = ((FaceFeatureExtractor*)m_pFace)->m_UserFace;
	m_list.DeleteAllItems();
	int len = facelib.size();
	if (0 == len)
		return;
	int i = 0;
	for (auto face_descriptor : facelib) 
	{
		m_list.InsertItem(i++, face_descriptor.first.c_str(), 0);
	}
	
}

void CFaceVerifyDlg::OnBnClickedButton1()
{
	CString str;
	GetDlgItem(IDC_COMBO1)->GetWindowTextA(str);
	string name = str.GetBuffer(0);
	
	FaceFeatureExtractor* pFace = (FaceFeatureExtractor*)m_pFace;
	pFace->faceRegister(name);

	listUpdata();
}


void CFaceVerifyDlg::OnBnClickedButton2()
{
	CString str;
	GetDlgItem(IDC_COMBO1)->GetWindowTextA(str);
	FaceFeatureExtractor* pFace = (FaceFeatureExtractor*)m_pFace;
	pFace->delFaceFeature(str.GetBuffer(0));
	listUpdata();
}


void CFaceVerifyDlg::OnBnClickedButton3()
{
	FaceFeatureExtractor* pFace = (FaceFeatureExtractor*)m_pFace;
	string name;
	pFace->ActiveFaceVerify2(name);
}