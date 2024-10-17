// CDLPDlg.cpp: 实现文件
//

#include "pch.h"
#include "halcon_deeplearn_device.h"
#include "CDLPDlg.h"
#include "afxdialogex.h"
#include "device.h"

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
	DDX_Control(pDX, IDC_LIST2, m_procList);
}


BEGIN_MESSAGE_MAP(CDLPDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDLPDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CDLPDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDLPDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CDLPDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &CDLPDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CDLPDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CDLPDlg::OnBnClickedButton6)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &CDLPDlg::OnLvnItemchangedList2)
	ON_BN_CLICKED(IDC_BUTTON7, &CDLPDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CDLPDlg::OnBnClickedButton8)
END_MESSAGE_MAP()


// CDLPDlg 消息处理程序


void CDLPDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	setProc(((CDevice*)m_pCheck)->m_curId);
	CDialogEx::OnOK();
}

void CDLPDlg::setProc(int id)
{
	if (((CDevice*)m_pCheck)->m_vctDLPa.size() > id)
	{
		S_CheckParam& pa = ((CDevice*)m_pCheck)->m_vctDLPa[id];
		CString str;
		GetDlgItem(IDC_EDIT1)->GetWindowTextA(str);
		pa.paramFile = str.GetBuffer(0);
		GetDlgItem(IDC_EDIT2)->GetWindowTextA(str);
		pa.modelFile = str.GetBuffer(0);
		int num = m_nameList.GetItemCount();
		vector<string>& vct = pa.vctClassNames;
		vct.clear();
		for (int i = 0; i < num; i++)
		{
			string str = m_nameList.GetItemText(i, 0).GetBuffer(0);
			vct.push_back(str);
		}

		GetDlgItem(IDC_EDIT_confidence)->GetWindowTextA(str);
		pa.minConfidence = atof(str.GetBuffer(0));
		GetDlgItem(IDC_EDIT_overlap)->GetWindowTextA(str);
		pa.maxOverlap = atof(str.GetBuffer(0));
		GetDlgItem(IDC_EDIT_classlap)->GetWindowTextA(str);
		pa.maxOverlapClassAgnostic = atof(str.GetBuffer(0));

		pa.m_procName = m_procList.GetItemText(id, 0).GetBuffer(0);
		pa.m_dlType = ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();
	}

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
		GetDlgItem(IDC_EDIT1)->SetWindowTextA(str);
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
		GetDlgItem(IDC_EDIT2)->SetWindowTextA(str);
	}
}


void CDLPDlg::OnBnClickedButton4()
{
	CFolderPickerDialog fd(getExePath().c_str(), 0, this, 0);
	if (fd.DoModal() == IDOK)
	{
		m_nameList.DeleteAllItems();
		string szDir = fd.GetPathName().GetBuffer(0);
		vector<string> vct;
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
	m_procList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_procList.InsertColumn(0, "制程名", LVCFMT_CENTER, 120);
	m_procList.SetColumnCtrlType(0, CCT_EDITBOX);
	m_procList.DeleteAllItems();
	if (((CDevice*)m_pCheck)->m_vctDLPa.size() > 0)
	{
		updateDlg(((CDevice*)m_pCheck)->m_curId);
		for (int i = 0; i < ((CDevice*)m_pCheck)->m_vctDLPa.size(); i++)
		{
			m_procList.InsertItem(i, ((CDevice*)m_pCheck)->m_vctDLPa[i].m_procName.c_str(), 0);
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CDLPDlg::updateDlg(int id)
{
	if (id >= ((CDevice*)m_pCheck)->m_vctDLPa.size())
	{
		id = ((CDevice*)m_pCheck)->m_vctDLPa.size() - 1;
		if (id == -1)
		{
			return;
		}
	}
	
	S_CheckParam& pa = ((CDevice*)m_pCheck)->m_vctDLPa[id];
	vector<string>& vct = pa.vctClassNames;
	m_nameList.DeleteAllItems();
	for (int i = 0; i < vct.size(); i++)
	{
		m_nameList.InsertItem(i, vct[i].c_str(), 0);
	}
	GetDlgItem(IDC_EDIT1)->SetWindowTextA(pa.paramFile.c_str());
	GetDlgItem(IDC_EDIT2)->SetWindowTextA(pa.modelFile.c_str());
	char sz[32];
	sprintf(sz, "%f", pa.minConfidence);
	GetDlgItem(IDC_EDIT_confidence)->SetWindowTextA(sz);
	sprintf(sz, "%f", pa.maxOverlap);
	GetDlgItem(IDC_EDIT_overlap)->SetWindowTextA(sz);
	sprintf(sz, "%f", pa.maxOverlapClassAgnostic);
	GetDlgItem(IDC_EDIT_classlap)->SetWindowTextA(sz);
	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(pa.m_dlType);
}

void CDLPDlg::OnBnClickedButton3()
{
	int id = m_nameList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (id > -1)
	{
		m_nameList.DeleteItem(id);
	}
}

void CDLPDlg::OnBnClickedButton5()
{
	int id = m_nameList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (id > -1)
	{
		m_nameList.InsertItem(id + 1, "", 0);
	}
	else
	{
		int c = m_nameList.GetItemCount();
		m_nameList.InsertItem(c, "", 0);
	}
}


void CDLPDlg::OnBnClickedButton6()
{
	int id = m_procList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (id > -1)
	{
		id += 1;
		m_procList.InsertItem(id, CDateTime::GetNow().ToString().c_str(), 0);
	}
	else
	{
		id = m_procList.GetItemCount();
		m_procList.InsertItem(id, CDateTime::GetNow().ToString().c_str(), 0);
	}
	m_procList.SetItemState(id, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
	m_procList.SetSelectionMark(id);
	((CDevice*)m_pCheck)->m_vctDLPa.push_back(S_CheckParam());
	updateDlg(id);
}


void CDLPDlg::OnLvnItemchangedList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (pNMLV->uChanged == LVIF_STATE)
	{
		if (pNMLV->uNewState)
		{
			int id = pNMLV->iItem;
			if (id > -1 && ((CDevice*)m_pCheck)->m_curId != id)
			{
				((CDevice*)m_pCheck)->m_curId = id;
				updateDlg(id);
			}
		}
	}	
	
	*pResult = 0;
}


void CDLPDlg::OnBnClickedButton7()
{
	int id = m_procList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (id > -1)
	{
		m_procList.DeleteItem(id);
		vector<S_CheckParam>& vctDLPa = ((CDevice*)m_pCheck)->m_vctDLPa;
		vctDLPa.erase(vctDLPa.begin()+id);
		updateDlg(id);
	}
}


void CDLPDlg::OnBnClickedButton8()
{
	setProc(((CDevice*)m_pCheck)->m_curId);
}
