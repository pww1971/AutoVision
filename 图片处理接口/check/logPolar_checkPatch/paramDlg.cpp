// paramDlg.cpp: 实现文件
//

#include "pch.h"
#include "logPolar_checkPatch.h"
#include "paramDlg.h"
#include "afxdialogex.h"
#include "checkPatch.h"

// paramDlg 对话框

IMPLEMENT_DYNAMIC(paramDlg, CDialogEx)

paramDlg::paramDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

paramDlg::~paramDlg()
{
}

void paramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(paramDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &paramDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// paramDlg 消息处理程序

BOOL paramDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	checkPatch* pCheck = (checkPatch*)m_pCheck;
	char dis[50];
	sprintf(dis, "%.3f", pCheck->m_myParam.m);
	GetDlgItem(IDC_EDIT1)->SetWindowTextA(dis);
	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(((checkPatch*)m_pCheck)->m_myParam.type);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void paramDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItem(IDC_EDIT1)->GetWindowTextA(str);
	((checkPatch*)m_pCheck)->m_myParam.m = atof(str.GetBuffer(0));
	((checkPatch*)m_pCheck)->m_myParam.type = ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();

	CDialogEx::OnOK();
}
