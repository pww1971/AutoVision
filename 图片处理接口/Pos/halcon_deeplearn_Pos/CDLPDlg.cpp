// CDLPDlg.cpp: 实现文件
//

#include "pch.h"
#include "halcon_deeplearn_Pos.h"
#include "CDLPDlg.h"
#include "afxdialogex.h"
#include "checkPatch.h"

// CDLPDlg 对话框

IMPLEMENT_DYNAMIC(CDLPDlg, CDialogEx)

CDLPDlg::CDLPDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	m_pCheck = NULL;
}

CDLPDlg::~CDLPDlg()
{
}

void CDLPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_nameList);
}


BEGIN_MESSAGE_MAP(CDLPDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDLPDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CDLPDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDLPDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CDLPDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &CDLPDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_CHECK1, &CDLPDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CDLPDlg::OnBnClickedCheck2)
END_MESSAGE_MAP()


// CDLPDlg 消息处理程序


void CDLPDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	int num = m_nameList.GetItemCount();
	vector<string>& vct = ((checkPatch*)m_pCheck)->m_pa.vctClassNames;
	vct.clear();
	for (int i = 0; i < num; i++)
	{
		string str = m_nameList.GetItemText(i, 0).GetBuffer(0);
		vct.push_back(str);
	}
	OnBnClickedCheck1();
	CDialogEx::OnOK();
}


void CDLPDlg::OnBnClickedButton1()
{
	CFileDialog fdlg1(true, NULL, getExePath().c_str(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "open file (*.hdict)|*.hdict||");

	if (fdlg1.DoModal() != IDOK)
	{
		return;
	}
	else
	{
		CString str = fdlg1.GetPathName();
		CopyFile(str.GetBuffer(0), ((checkPatch*)m_pCheck)->m_pa.paramFile.c_str());
	}
}


void CDLPDlg::OnBnClickedButton2()
{
	CFileDialog fdlg1(true, NULL, getExePath().c_str(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "open file (*.hdl)|*.hdl||");

	if (fdlg1.DoModal() != IDOK)
	{
		return;
	}
	else
	{
		CString str = fdlg1.GetPathName();
		CopyFile(str.GetBuffer(0), ((checkPatch*)m_pCheck)->m_pa.modelFile.c_str());
	}
}


void CDLPDlg::OnBnClickedButton4()
{
	CFolderPickerDialog fd(getExePath().c_str(), 0, this, 0);
	if (fd.DoModal() == IDOK)
	{
		vector<string>& vct = ((checkPatch*)m_pCheck)->m_pa.vctClassNames;
		vct.clear();
		m_nameList.DeleteAllItems();
		string szDir = fd.GetPathName().GetBuffer(0);
		getFileNameDir(szDir.c_str(),vct);
		for (int i=0;i<vct.size();i++)
		{
			m_nameList.InsertItem(i, vct[i].c_str(), 0);
		}
	}
}


BOOL CDLPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_nameList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_nameList.InsertColumn(0, "类名", LVCFMT_CENTER, 120);
	m_nameList.SetColumnCtrlType(0, CCT_EDITBOX);
	vector<string>& vct = ((checkPatch*)m_pCheck)->m_pa.vctClassNames;
	m_nameList.DeleteAllItems();
	for (int i = 0; i < vct.size(); i++)
	{
		m_nameList.InsertItem(i, vct[i].c_str(), 0);
	}

	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(0==((checkPatch*)m_pCheck)->m_pa.mytype);
	((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(((checkPatch*)m_pCheck)->m_pa.getPic);
	GetDlgItem(IDC_EDIT1)->SetWindowTextA(((checkPatch*)m_pCheck)->m_pa.szPicPath.c_str());
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDLPDlg::OnBnClickedButton3()
{
	int id = m_nameList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (id > -1)
	{
		m_nameList.DeleteItem(id);
	}
}


void CDLPDlg::OnBnClickedCheck1()
{
	if (((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck())
	{
		((checkPatch*)m_pCheck)->m_pa.mytype = 0;
	}
	else
	{
		((checkPatch*)m_pCheck)->m_pa.mytype = 1;
		int id = m_nameList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
		if (id > -1)
		{
			((checkPatch*)m_pCheck)->m_pa.mytype = id;
		}
	}
}


void CDLPDlg::OnBnClickedCheck2()
{
	if (((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck())
	{
		((checkPatch*)m_pCheck)->m_pa.getPic = 1;
		CFolderPickerDialog fd(getExePath().c_str(), 0, this, 0);
		if (fd.DoModal() == IDOK)
			((checkPatch*)m_pCheck)->m_pa.szPicPath = fd.GetPathName().GetBuffer(0);
		else
			((checkPatch*)m_pCheck)->m_pa.szPicPath = "";
	}
	else
	{
		((checkPatch*)m_pCheck)->m_pa.getPic = 0;
		((checkPatch*)m_pCheck)->m_pa.szPicPath = "";
	}
	GetDlgItem(IDC_EDIT1)->SetWindowTextA(((checkPatch*)m_pCheck)->m_pa.szPicPath.c_str());
}
