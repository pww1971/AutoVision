// C3dMeatureDlg.cpp: 实现文件
//

#include "pch.h"
#include "halcon_3dMeasure_checkPatch.h"
#include "C3dMeatureDlg.h"
#include "afxdialogex.h"
#include "checkPatch.h"
// C3dMeatureDlg 对话框
extern HWND g_halhWnd;

IMPLEMENT_DYNAMIC(C3dMeatureDlg, CDialogEx)

C3dMeatureDlg::C3dMeatureDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	m_pCheck = NULL;
	SetSystem("use_window_thread", "true");
}

C3dMeatureDlg::~C3dMeatureDlg()
{
	g_halhWnd = 0;
}

void C3dMeatureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_procList);
	DDX_Control(pDX, IDC_MFCPROPERTYGRID1, m_prop);
}


BEGIN_MESSAGE_MAP(C3dMeatureDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &C3dMeatureDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_RADIO1, &C3dMeatureDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO4, &C3dMeatureDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO2, &C3dMeatureDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO5, &C3dMeatureDlg::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO3, &C3dMeatureDlg::OnBnClickedRadio3)
	ON_CBN_SELCHANGE(IDC_COMBO1, &C3dMeatureDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BT_DEL, &C3dMeatureDlg::OnBnClickedBtDel)
	ON_BN_CLICKED(IDC_BT_DELALL, &C3dMeatureDlg::OnBnClickedBtDelall)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)

	ON_BN_CLICKED(IDC_BUTTON6, &C3dMeatureDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &C3dMeatureDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &C3dMeatureDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BT_ViewMode, &C3dMeatureDlg::OnBnClickedBtViewmode)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &C3dMeatureDlg::OnLvnItemchangedList2)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// C3dMeatureDlg 消息处理程序


void C3dMeatureDlg::OnBnClickedButton1()
{
	//((checkPatch*)m_pCheck)->treatPic();
	//updateProp();
	int arg = 2;
	if (!((checkPatch*)m_pCheck)->m_runing)
	{
		((checkPatch*)m_pCheck)->open(&arg);
		/*while (((checkPatch*)m_pCheck)->m_runing)
			Sleep(50);*/
	}

}


BOOL C3dMeatureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	g_halhWnd = GetDlgItem(IDC_STATIC_POSPIC)->GetSafeHwnd();//获取控件句柄
	((checkPatch*)m_pCheck)->iniHalWin();
	((checkPatch*)m_pCheck)->m_pDlg = this;
	propIni();
	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(0);
	m_procList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_procList.InsertColumn(0, "制程名", LVCFMT_CENTER, 120);
	m_procList.SetColumnCtrlType(0, CCT_EDITBOX);
	m_procList.DeleteAllItems();
	if (((checkPatch*)m_pCheck)->m_pa.m_vctPa.size() > 0)
	{
		((checkPatch*)m_pCheck)->m_curPa = 0;
		updateProp();
		for (int i = 0; i < ((checkPatch*)m_pCheck)->m_pa.m_vctPa.size(); i++)
		{
			m_procList.InsertItem(i, ((checkPatch*)m_pCheck)->m_pa.m_vctPa[i].m_procName.c_str(), 0);
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void C3dMeatureDlg::OnBnClickedRadio1()
{
	((checkPatch*)m_pCheck)->DisOnRoiDrawCircle();
}


void C3dMeatureDlg::OnBnClickedRadio4()
{
	((checkPatch*)m_pCheck)->DisOnRoiDrawRectangle2();
}


void C3dMeatureDlg::OnBnClickedRadio2()
{
	((checkPatch*)m_pCheck)->DisOnRoiDrawEllipse();
}


void C3dMeatureDlg::OnBnClickedRadio5()
{
	((checkPatch*)m_pCheck)->DisOnRoiDrawAnyLine();
}


void C3dMeatureDlg::OnBnClickedRadio3()
{
	((checkPatch*)m_pCheck)->DisOnRoiDrawRectangle();
}


void C3dMeatureDlg::OnCbnSelchangeCombo1()
{
	int sel = ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();
	((checkPatch*)m_pCheck)->iCOMBO_MATCH_ROI_TYPE = (eCOMBO_MATCH_ROI_TYPE)sel;
}


void C3dMeatureDlg::OnBnClickedBtDel()
{
	((checkPatch*)m_pCheck)->DisOnElementDelOne();
}


void C3dMeatureDlg::OnBnClickedBtDelall()
{
	((checkPatch*)m_pCheck)->DisOnMatchDelAllRoi();
}

//更新当前参数的显示
void C3dMeatureDlg::updateProp()
{
	if (!m_prop.IsWindowVisible())
	{
		return;
	}
	if (((checkPatch*)m_pCheck)->m_pa.m_vctPa.size() < 1)
	{
		return;
	}
	S_CheckProb& pa = ((checkPatch*)m_pCheck)->m_pa.m_vctPa[((checkPatch*)m_pCheck)->m_curPa];
	string proName[] = { "高度","顶点","平面度","角度","高度差","切片","点面距离" };
	int count = m_prop.GetPropertyCount();
	for (int i = 0; i < count; i++)
	{
		CMFCPropertyGridProperty* pProperty = m_prop.GetProperty(i);
		if (pProperty == nullptr)
			continue;
		int subItemCount = pProperty->GetSubItemsCount();
		for (int j = 0; j < subItemCount; j++)
		{
			CMFCPropertyGridProperty* pSubProperty = pProperty->GetSubItem(j);
			if (pSubProperty == nullptr)
				continue;
			//CString name = pSubProperty->GetName();
			//COleVariant value = pSubProperty->GetValue();
			int id = (int)pSubProperty->GetData();
			if (id < 10)
			{
				continue;
			}
			int subid = id / 10;
			int ssubid = id % 10;
			CString str;
			if (1 == subid)
			{
				switch (ssubid)
				{
				case 1:
					str = proName[pa.m_type].c_str();
					m_procList.SetItemText(((checkPatch*)m_pCheck)->m_curPa, 0, pa.m_procName.c_str());
					break;
				case 2:
					str.Format("%.3f", pa.m_bl);//
					break;
				case 3:
					str.Format("%.3f", pa.m_bh);// 
					break;
				case 4:
					str.Format("%d", pa.m_objId);// 
					break;
				case 5:
					str.Format("%d", pa.m_objPairId);// 
					break;
				case 6:
					str.Format("%f", pa.m_dret);// 
					break;
				}
			}
			else if (2 == subid)
			{
				if (ssubid < 8)
				{
					str.Format("%.3f", pa.hv_Pose[ssubid - 1].D());// 
				}
			}
			else if (3 == subid)
			{
				switch (ssubid)
				{
				case 1:
					str.Format("%.3f", ((checkPatch*)m_pCheck)->m_pa.m_zMin);// 
					break;
				case 2:
					str.Format("%.3f", ((checkPatch*)m_pCheck)->m_pa.m_zMax);// 
					break;
				case 3:
					str.Format("%d", ((checkPatch*)m_pCheck)->m_pa.m_distance);// 
					break;
				default:
					break;
				}
			}
			pSubProperty->SetValue(str);
		}
	}
}
void C3dMeatureDlg::propIni()
{
	if (((checkPatch*)m_pCheck)->m_pa.m_vctPa.size()<1)
	{
		return;
	}
	S_CheckProb& pa = ((checkPatch*)m_pCheck)->m_pa.m_vctPa[((checkPatch*)m_pCheck)->m_curPa];
	string proName[] = { "高度","顶点","平面度","角度","高度差","切片","点面距离" };
	m_prop.RemoveAll();
	CString str;
	HDITEM item;
	item.cxy = 120;
	item.mask = HDI_WIDTH;
	CFont pFont;
	pFont.CreatePointFont(180, _T("楷体"));
	m_prop.SetFont(&pFont);
	m_prop.GetHeaderCtrl().SetItem(0, new HDITEM(item));
	CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(_T("检测项目"));
	pGroup->SetData(1);
	CMFCPropertyGridProperty* pItem = new CMFCPropertyGridProperty(_T("检测类型"), proName[pa.m_type].c_str());
	pItem->SetData(11);
	for (int i=0;i<7;i++)
		pItem->AddOption(proName[i].c_str());
	pItem->AllowEdit(FALSE);
	pGroup->AddSubItem(pItem);
	str.Format("%.3f", pa.m_bl);//最小
	pItem = new CMFCPropertyGridProperty(_T("结果范围"), str, _T("上限"));
	pItem->SetData(12);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%.3f", pa.m_bh);//最大
	pItem = new CMFCPropertyGridProperty(_T("结果范围"), str, _T("下限"));
	pItem->SetData(13);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%d", pa.m_objId);//目标id
	pItem = new CMFCPropertyGridProperty(_T("目标1"), str, _T("检测项目1的id"));
	pItem->SetData(14);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%d", pa.m_objPairId);//目标2
	pItem = new CMFCPropertyGridProperty(_T("目标2"), str, _T("检测项目2的id"));
	pItem->SetData(15);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%f", pa.m_dret);//
	pItem = new CMFCPropertyGridProperty(_T("结果"), str, _T("检测结果"));
	pItem->SetData(16);
	pItem->AllowEdit(FALSE);//不允许编辑
	pGroup->AddSubItem(pItem);
	m_prop.AddProperty(pGroup);

	pGroup = new CMFCPropertyGridProperty(_T("切面位姿"));
	pGroup->SetData(2);
	//掩膜类型参数
	str.Format("%.3f", pa.hv_Pose[0].D());
	pItem = new CMFCPropertyGridProperty(_T("x平移"), str, _T("x平移"));
	pItem->SetData(21);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%.3f", pa.hv_Pose[1].D());
	pItem = new CMFCPropertyGridProperty(_T("y平移"), str, _T("y平移"));
	pItem->SetData(22);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%.3f", pa.hv_Pose[2].D());
	pItem = new CMFCPropertyGridProperty(_T("z平移"), str, _T("z平移"));
	pItem->SetData(23);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%.3f", pa.hv_Pose[3].D());
	pItem = new CMFCPropertyGridProperty(_T("x旋转"), str, _T("x旋转"));
	pItem->SetData(24);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%.3f", pa.hv_Pose[4].D());
	pItem = new CMFCPropertyGridProperty(_T("y旋转"), str, _T("y旋转"));
	pItem->SetData(25);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%.3f", pa.hv_Pose[5].D());
	pItem = new CMFCPropertyGridProperty(_T("z旋转"), str, _T("z旋转"));
	pItem->SetData(26);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%.3f", pa.hv_Pose[6].D());
	pItem = new CMFCPropertyGridProperty(_T("扩展"), str, _T("扩展"));
	pItem->SetData(27);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	m_prop.AddProperty(pGroup);

	pGroup = new CMFCPropertyGridProperty(_T("全局变量"));
	pGroup->SetData(3);
	str.Format("%.3f", ((checkPatch*)m_pCheck)->m_pa.m_zMin);//zmap范围
	pItem = new CMFCPropertyGridProperty(_T("z轴范围"), str, _T("上限"));
	pItem->SetData(31);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%.3f", ((checkPatch*)m_pCheck)->m_pa.m_zMax);//
	pItem = new CMFCPropertyGridProperty(_T("z轴范围"), str, _T("下限"));
	pItem->SetData(32);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%d", ((checkPatch*)m_pCheck)->m_pa.m_distance);//
	pItem = new CMFCPropertyGridProperty(_T("筛选距离"), str, _T("点云筛选距离"));
	pItem->SetData(33);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	m_prop.AddProperty(pGroup);
	m_prop.ExpandAll();
}

LRESULT C3dMeatureDlg::OnPropertyChanged(WPARAM, LPARAM lParam)
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lParam;
	if (pProp == nullptr)
		return 0;
	int id = (int)pProp->GetData();
	if (id < 10)
	{
		return 0;
	}
	//	CString s = pProp->GetName();  //被改变的参数名  
	COleVariant t = pProp->GetValue(); //改变之后的值  
	//t = pProp->GetOriginalValue();  //改变之前的值  
	S_CheckProb& pa = ((checkPatch*)m_pCheck)->m_pa.m_vctPa[((checkPatch*)m_pCheck)->m_curPa];
	CString str;
	str = t.bstrVal;      //从COleVariant到CString  
	int subid = id / 10;
	int ssubid = id % 10;
	if (1 == subid)
	{
		switch (ssubid)
		{
		case 1:
		{
			CString proName[] = { "高度","顶点","平面度","角度","高度差","切片","点面距离" };
			for (int i = 0; i < 7; i++)
			{
				if (proName[i] == str)
				{
					pa.m_type = i;
					pa.m_procName = str.GetBuffer(0);
					m_procList.SetItemText(((checkPatch*)m_pCheck)->m_curPa, 0, str);
					break;
				}
			}
			break;
		}
		case 2:
			pa.m_bl = atof(str.GetBuffer(0));// 
			break;
		case 3:
			pa.m_bh = atof(str.GetBuffer(0));// 
			break;
		case 4:
			pa.m_objId = atoi(str.GetBuffer(0));// 
			break;
		case 5:
			pa.m_objPairId = atoi(str.GetBuffer(0));// 
			break;
		}
	}
	else if (2 == subid)
	{
		if (ssubid < 8)
		{
			pa.hv_Pose[ssubid-1] = atof(str.GetBuffer(0));
		}
	}
	else if (3 == subid)
	{
		switch (ssubid)
		{
		case 1:
			((checkPatch*)m_pCheck)->m_pa.m_zMin = atof(str.GetBuffer(0));// 
			break;
		case 2:
			((checkPatch*)m_pCheck)->m_pa.m_zMax = atof(str.GetBuffer(0));// 
			break;
		case 3:
			((checkPatch*)m_pCheck)->m_pa.m_distance = atoi(str.GetBuffer(0));// 
			break;
		default:
			break;
		}
	}
	SetTimer(1, 1, NULL);
	return 0;
}

void C3dMeatureDlg::OnBnClickedButton6()
{
	S_CheckProb pa;
	int id = m_procList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (id > -1)
	{
		id += 1;
	}
	else
	{
		id = m_procList.GetItemCount();
	}
	pa.m_procName = "高度";
	((checkPatch*)m_pCheck)->m_pa.m_vctPa.push_back(pa);
	((checkPatch*)m_pCheck)->m_curPa = id;
	m_procList.InsertItem(id, "高度", 0);
	m_procList.SetItemState(id, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
	m_procList.SetSelectionMark(id);
}


void C3dMeatureDlg::OnBnClickedButton7()
{
	int id = m_procList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (id > -1)
	{
		m_procList.DeleteItem(id);
		vector<S_CheckProb>& vctDLPa = ((checkPatch*)m_pCheck)->m_pa.m_vctPa;
		vctDLPa.erase(vctDLPa.begin() + id);
		((checkPatch*)m_pCheck)->m_curPa = id;
//		updateProp();
	}
}


void C3dMeatureDlg::OnBnClickedButton8()
{
	((checkPatch*)m_pCheck)->writeMode();
}


void C3dMeatureDlg::OnBnClickedBtViewmode()
{
	//((checkPatch*)m_pCheck)->treatOnce();
	//updateProp();
	int arg = 1;
	if (!((checkPatch*)m_pCheck)->m_runing)
	{
		((checkPatch*)m_pCheck)->open(&arg);
		/*while (((checkPatch*)m_pCheck)->m_runing)
			Sleep(50);*/
	}
}


void C3dMeatureDlg::OnLvnItemchangedList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (pNMLV->uChanged == LVIF_STATE)
	{
		if (pNMLV->uNewState)
		{
			int id = pNMLV->iItem;
			if (id > -1 && ((checkPatch*)m_pCheck)->m_curPa != id)
			{
				((checkPatch*)m_pCheck)->m_curPa = id;
				updateProp();
				try
				{
					((checkPatch*)m_pCheck)->DisProcessROImodel();
				}
				catch (HalconCpp::HException& except) {
					AfxMessageBox(except.ErrorText().Text());
				}
			}
		}
	}
	*pResult = 0;
}


void C3dMeatureDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
	{
		KillTimer(1);
		updateProp();
	}
	CDialogEx::OnTimer(nIDEvent);
}
