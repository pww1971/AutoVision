// threeDMatch.cpp: 实现文件
//

#include "pch.h"
#include "3dPos_device.h"
#include "threeDMatch.h"
#include "afxdialogex.h"

extern HWND g_halhWnd;
// threeDMatch 对话框

IMPLEMENT_DYNAMIC(threeDMatch, CDialogEx)

threeDMatch::threeDMatch(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

threeDMatch::~threeDMatch()
{

}

void threeDMatch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_POSPIC, m_viewStatic);
	DDX_Control(pDX, IDC_LIST2, m_procList);
}


BEGIN_MESSAGE_MAP(threeDMatch, CDialogEx)
	ON_BN_CLICKED(IDOK, &threeDMatch::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON6, &threeDMatch::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &threeDMatch::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &threeDMatch::OnBnClickedButton8)
	ON_WM_CLOSE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &threeDMatch::OnLvnItemchangedList2)
	ON_BN_CLICKED(IDC_BT_BuildMode, &threeDMatch::OnBnClickedBtBuildmode)
	ON_BN_CLICKED(IDC_BUTTON3, &threeDMatch::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &threeDMatch::OnBnClickedButton1)
	ON_BN_CLICKED(IDCANCEL, &threeDMatch::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BT_DEL, &threeDMatch::OnBnClickedBtDel)
	ON_BN_CLICKED(IDC_BT_DELALL, &threeDMatch::OnBnClickedBtDelall)
	ON_CBN_SELCHANGE(IDC_COMBO1, &threeDMatch::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_RADIO1, &threeDMatch::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO4, &threeDMatch::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO2, &threeDMatch::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO5, &threeDMatch::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO3, &threeDMatch::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_BT_ViewMode, &threeDMatch::OnBnClickedBtViewmode)
END_MESSAGE_MAP()


// threeDMatch 消息处理程序


void threeDMatch::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


BOOL threeDMatch::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	CRect Rect;
	g_halhWnd = m_viewStatic.GetSafeHwnd();//获取控件句柄
	::GetWindowRect(g_halhWnd, &Rect);//获取控件大小
	OpenWindow(0, 0, Rect.Width(), Rect.Height(), (Hlong)g_halhWnd, "visible", "", &((CDevice*)m_pCheck)->hv_WindowHandle);
	HDevWindowStack::Push(((CDevice*)m_pCheck)->hv_WindowHandle);

	((CDevice*)m_pCheck)->openDevWin(true);
	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(0);
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


void threeDMatch::OnBnClickedButton6()
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


void threeDMatch::OnBnClickedButton7()
{
	int id = m_procList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (id > -1)
	{
		m_procList.DeleteItem(id);
		std::vector<S_CheckParam>& vctDLPa = ((CDevice*)m_pCheck)->m_vctDLPa;
		vctDLPa.erase(vctDLPa.begin() + id);
		updateDlg(id);
	}
}


void threeDMatch::OnBnClickedButton8()
{
	setProc(((CDevice*)m_pCheck)->m_curId);
}


void threeDMatch::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	g_halhWnd = 0;
	CDialogEx::OnClose();
}


void threeDMatch::OnLvnItemchangedList2(NMHDR* pNMHDR, LRESULT* pResult)
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
void threeDMatch::updateDlg(int id)
{
	if (id >= ((CDevice*)m_pCheck)->m_vctDLPa.size())
	{
		id = ((CDevice*)m_pCheck)->m_vctDLPa.size() - 1;
		if (id == -1)
		{
			return;
		}
	}
	((CDevice*)m_pCheck)->m_curId = id;
	S_CheckParam& pa = ((CDevice*)m_pCheck)->m_vctDLPa[id];
	char sz[32];
	sprintf(sz, "%f", pa.RelSamplingDistance1);
	GetDlgItem(IDC_EDIT_RelSamplingDistance)->SetWindowTextA(sz);
	sprintf(sz, "%f", pa.KeyPointFraction);
	GetDlgItem(IDC_EDIT_KeyPointFraction)->SetWindowTextA(sz);
	sprintf(sz, "%f", pa.MinScore);
	GetDlgItem(IDC_EDIT_MinScore)->SetWindowTextA(sz);
	sprintf(sz, "%d", pa.GenParamValue);
	GetDlgItem(IDC_EDIT_GenParamValue)->SetWindowTextA(sz);
	sprintf(sz, "%f", pa.bgL);
	GetDlgItem(IDC_EDIT_bgL)->SetWindowTextA(sz);
	sprintf(sz, "%f", pa.bgH);
	GetDlgItem(IDC_EDIT_bgH)->SetWindowTextA(sz);
	sprintf(sz, "%f", pa.m_grapsize);
	GetDlgItem(IDC_EDIT_GrabSize)->SetWindowTextA(sz);

	((CComboBox*)GetDlgItem(IDC_COMBORota))->SetCurSel(pa.m_pose);
	CString str;
	str.Format("%.3f", pa.hv_Pose[0].D());
	GetDlgItem(IDC_EDITX)->SetWindowTextA(str);
	str.Format("%.3f", pa.hv_Pose[1].D());
	GetDlgItem(IDC_EDITY)->SetWindowTextA(str);
	str.Format("%.3f", pa.hv_Pose[2].D());
	GetDlgItem(IDC_EDITZ)->SetWindowTextA(str);
}

void threeDMatch::getDlgParam()
{
	S_CheckParam& pa = ((CDevice*)m_pCheck)->m_vctDLPa[((CDevice*)m_pCheck)->m_curId];
	CString str;
	GetDlgItem(IDC_EDIT_RelSamplingDistance)->GetWindowTextA(str);
	pa.RelSamplingDistance1 = atof(str.GetBuffer(0));
	GetDlgItem(IDC_EDIT_KeyPointFraction)->GetWindowTextA(str);
	pa.KeyPointFraction = atof(str.GetBuffer(0));
	GetDlgItem(IDC_EDIT_MinScore)->GetWindowTextA(str);
	pa.MinScore = atof(str.GetBuffer(0));
	GetDlgItem(IDC_EDIT_GenParamValue)->GetWindowTextA(str);
	pa.GenParamValue = atoi(str.GetBuffer(0));
	GetDlgItem(IDC_EDIT_bgL)->GetWindowTextA(str);
	pa.bgL = atof(str.GetBuffer(0));
	GetDlgItem(IDC_EDIT_bgH)->GetWindowTextA(str);
	pa.bgH = atof(str.GetBuffer(0));
	GetDlgItem(IDC_EDIT_GrabSize)->GetWindowTextA(str);
	pa.m_grapsize = atof(str.GetBuffer(0));
	pa.m_procName = m_procList.GetItemText(((CDevice*)m_pCheck)->m_curId, 0).GetBuffer(0);

	int sel = ((CComboBox*)GetDlgItem(IDC_COMBORota))->GetCurSel();
	((CDevice*)m_pCheck)->setPoseRota(sel);
	GetDlgItem(IDC_EDITX)->GetWindowTextA(str);
	float val = atof(str.GetBuffer(0));
	((CDevice*)m_pCheck)->setPoseX(val);
	GetDlgItem(IDC_EDITY)->GetWindowTextA(str);
	val = atof(str.GetBuffer(0));
	((CDevice*)m_pCheck)->setPoseY(val);
	GetDlgItem(IDC_EDITZ)->GetWindowTextA(str);
	val = atof(str.GetBuffer(0));
	((CDevice*)m_pCheck)->setPoseZ(val);
}

void threeDMatch::setProc(int id)
{
	if (((CDevice*)m_pCheck)->m_vctDLPa.size() > id)
	{
		((CDevice*)m_pCheck)->m_curId = id;
		getDlgParam();
		((CDevice*)m_pCheck)->writeMode();
	}
}

void threeDMatch::OnBnClickedBtBuildmode()
{
	getDlgParam();
	int arg = 2;
	if (!((CDevice*)m_pCheck)->m_runing)
		((CDevice*)m_pCheck)->open(&arg);
	//((CDevice*)m_pCheck)->openDevWin();
}


void threeDMatch::OnBnClickedButton3()
{
	CFileDialog fdlg1(true, NULL, getExePath().c_str(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "open file (*)|*||");

	if (fdlg1.DoModal() != IDOK)
	{
		return;
	}
	else
	{
		CString str = fdlg1.GetPathName();
		ReadImage(&((CDevice*)m_pCheck)->m_hImage[1], str.GetBuffer(0));
		HDevWindowStack::SetActive(((CDevice*)m_pCheck)->hv_WindowHandle);
		if (HDevWindowStack::IsOpen())
			DispObj(((CDevice*)m_pCheck)->m_hImage[1], HDevWindowStack::GetActive());
	}
}


void threeDMatch::OnBnClickedButton1()
{
	//	((CDevice*)m_pCheck)->actiontest();
	int modeId = ((CDevice*)m_pCheck)->m_curId;
	BeginWaitCursor();
	//((CDevice*)m_pCheck)->LocateCheck(modeId);
	
	int arg = 3;
	if (!((CDevice*)m_pCheck)->m_runing)
	{
		((CDevice*)m_pCheck)->open(&arg);
		while (((CDevice*)m_pCheck)->m_runing)
			Sleep(50);
	}
	EndWaitCursor(); 
}


void threeDMatch::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void threeDMatch::OnBnClickedBtDel()
{
	((CDevice*)m_pCheck)->DisOnElementDelOne();
}


void threeDMatch::OnBnClickedBtDelall()
{
	((CDevice*)m_pCheck)->DisOnMatchDelAllRoi();
}


void threeDMatch::OnCbnSelchangeCombo1()
{
	int sel = ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();
	((CDevice*)m_pCheck)->iCOMBO_MATCH_ROI_TYPE = (eCOMBO_MATCH_ROI_TYPE)sel;
}


void threeDMatch::OnBnClickedRadio1()
{
	getDlgParam();
	((CDevice*)m_pCheck)->DisOnRoiDrawCircle();
}


void threeDMatch::OnBnClickedRadio4()
{
	getDlgParam();
	((CDevice*)m_pCheck)->DisOnRoiDrawRectangle2();
}


void threeDMatch::OnBnClickedRadio2()
{
	getDlgParam();
	((CDevice*)m_pCheck)->DisOnRoiDrawEllipse();
}


void threeDMatch::OnBnClickedRadio5()
{
	getDlgParam();
	((CDevice*)m_pCheck)->DisOnRoiDrawAnyLine();
}


void threeDMatch::OnBnClickedRadio3()
{
	getDlgParam();
	((CDevice*)m_pCheck)->DisOnRoiDrawRectangle();
}


void threeDMatch::OnBnClickedBtViewmode()
{
	try {
		getDlgParam();
		int arg = 1;
		if (!((CDevice*)m_pCheck)->m_runing)
			((CDevice*)m_pCheck)->open(&arg);
		//((CDevice*)m_pCheck)->set3dTran(false);
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}


