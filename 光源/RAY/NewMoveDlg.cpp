// NewMoveDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RAY.h"
#include "NewMoveDlg.h"

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
	DDX_Control(pDX, IDC_LIST1, m_moveList);
}

BEGIN_MESSAGE_MAP(CNewMoveDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CNewMoveDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CNewMoveDlg::OnBnClickedButton1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CNewMoveDlg::OnLvnItemchangedList1)
END_MESSAGE_MAP()

BOOL CNewMoveDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString str;
	if (((S_vctRay*)m_pData)->m_vctRay.size()>0)
	{
		str = CStrProcess::itoa_radixN(((S_vctRay*)m_pData)->m_vctRay[((S_vctRay*)m_pData)->m_curRay]->m_baudrate).c_str();
		GetDlgItem(IDC_EDIT_Ray)->SetWindowTextA(str);
	}
	else
		GetDlgItem(IDC_EDIT_Ray)->SetWindowTextA("19200");
	for (int i=1;i<11;i++)
	{
		str = ("COM"+CStrProcess::itoa_radixN(i)).c_str();
		((CComboBox*)GetDlgItem(IDC_COMBO_Ray))->AddString(str);
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_Ray))->SetCurSel(0);

	m_moveList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_moveList.InsertColumn(0, "光源设置", LVCFMT_CENTER,150);
	getListInfo();
	return TRUE; 
}

// CNewMoveDlg 消息处理程序
void CNewMoveDlg::OnBnClickedButton2()
{
	CString str;
	GetDlgItem(IDC_EDIT_Ray)->GetWindowTextA(str);
	CString str1;
	GetDlgItem(IDC_COMBO_Ray)->GetWindowTextA(str1);
	str = str1 +MVFLAG+str+MVFLAG;
	if (((S_vctRay*)m_pData)->addRay(str.GetBuffer(0)))
	{
		int id = m_moveList.GetItemCount();
		str += ((S_vctRay*)m_pData)->m_vctRay[id]->m_st.c_str();
		m_moveList.InsertItem(id,str,0);
		AfxMessageBox("连接ok");
	}
	//m_tcpdlg.DoModal();//Create(IDD_DIALOG2);
	//m_tcpdlg.ShowWindow(1);
}

void CNewMoveDlg::OnBnClickedButton1()
{
	int id = m_moveList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (id > -1)
	{
		m_moveList.DeleteItem(id);
		delListInfo(id);
	}
	else
	{
		id = m_moveList.GetItemCount();
		if (id>0)
		{
			m_moveList.DeleteItem(id);
			delListInfo(id);
		}
	}		
}

void CNewMoveDlg::getListInfo()
{
	m_moveList.DeleteAllItems();
	int len = ((S_vctRay*)m_pData)->m_vctRay.size();
	for (int i=0;i<len;i++)
	{
		string str = ((S_vctRay*)m_pData)->m_vctRay[i]->getStr();
		m_moveList.InsertItem(i,str.c_str(),0);
	}
}

void CNewMoveDlg::delListInfo(int id)
{
	int len = ((S_vctRay*)m_pData)->m_vctRay.size();
	if (id>=len)
	{
		return;
	}
	std::vector<S_RayDevice*>::iterator it = ((S_vctRay*)m_pData)->m_vctRay.begin();
	for (int i=0;i<id;i++)
	{
		it++;
	}
	delete *it;
	((S_vctRay*)m_pData)->m_vctRay.erase(it);
}


void CNewMoveDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (pNMLV->uNewState&LVIS_SELECTED)
	{
		if(-1 != pNMLV->iItem)
		{
			((S_vctRay*)m_pData)->m_curRay = pNMLV->iItem;
		}
	}
	else
	{

	}
	
	*pResult = 0;
}
