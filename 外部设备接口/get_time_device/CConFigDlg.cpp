// CConFigDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "CConFigDlg.h"
#include "afxdialogex.h"


// CConFigDlg 对话框

IMPLEMENT_DYNAMIC(CConFigDlg, CDialogEx)

CConFigDlg::CConFigDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx("IDD_DIALOG1", pParent)
{

}

CConFigDlg::~CConFigDlg()
{
}

void CConFigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConFigDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CConFigDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CConFigDlg 消息处理程序


void CConFigDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
}


BOOL CConFigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
