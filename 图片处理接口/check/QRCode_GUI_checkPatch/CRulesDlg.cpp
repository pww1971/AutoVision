// CRulesDlg.cpp: 实现文件
//

#include "pch.h"
#include "QRCode_checkPatch.h"
#include "CRulesDlg.h"
#include "checkPatch.h"

// CRulesDlg 对话框

IMPLEMENT_DYNAMIC(CRulesDlg, CDialogEx)

CRulesDlg::CRulesDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	m_pCheck = NULL;
}

CRulesDlg::~CRulesDlg()
{
}

void CRulesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRulesDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRulesDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CRulesDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CRulesDlg 消息处理程序

BOOL CRulesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	checkPatch* pCheck = (checkPatch*)m_pCheck;
	GetDlgItem(IDC_EDIT2)->SetWindowTextA(pCheck->m_szRule.c_str());
	char sz[32];
	sprintf(sz, "%d", pCheck->m_mapSign);
	GetDlgItem(IDC_EDIT1)->SetWindowTextA(sz);
	sprintf(sz, "%d", pCheck->m_BN);
	GetDlgItem(IDC_EDIT3)->SetWindowTextA(sz);
	sprintf(sz, "%d", pCheck->m_wBufLen);
	GetDlgItem(IDC_EDIT4)->SetWindowTextA(sz);
	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(pCheck->m_iHashPrefix);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CRulesDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	checkPatch* pCheck = (checkPatch*)m_pCheck;
	CString str;
	GetDlgItem(IDC_EDIT2)->GetWindowTextA(str);
	pCheck->m_szRule = str.GetBuffer(0);
	GetDlgItem(IDC_EDIT1)->GetWindowTextA(str);
	pCheck->m_mapSign = atoi(str.GetBuffer(0));
	GetDlgItem(IDC_EDIT3)->GetWindowTextA(str);
	pCheck->m_BN = atoi(str.GetBuffer(0));
	GetDlgItem(IDC_EDIT4)->GetWindowTextA(str);
	pCheck->m_wBufLen = atoi(str.GetBuffer(0));
	if (((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck())
		pCheck->m_iHashPrefix = 1;
	else
		pCheck->m_iHashPrefix = 0;

	CDialogEx::OnOK();
}


void CRulesDlg::OnBnClickedButton1()
{
	checkPatch* pCheck = (checkPatch*)m_pCheck;
	pCheck->delMap();
}
