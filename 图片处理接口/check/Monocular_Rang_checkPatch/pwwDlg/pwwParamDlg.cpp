// pwwParamDlg.cpp : 实现文件
//

#include "../pch.h"
#include "../Monocular_Rang_checkPatch.h"
#include "pwwParamDlg.h"
#include "afxdialogex.h"
#include "../checkPatch.h"
ONMOUSE(onMouse0)
ONMOUSE(onMouse1)
ONMOUSE(onMouse2)
//如果要修改 记得for的长度要一个个修改 比较麻烦
static 	CString s_szPreName[] = { "不处理", "平滑", "canny", "抽骨干", "补偿灰度", "增强", "sobel", "红", "绿", "蓝", "灰度" };
static CString s_szColorName[] = { "彩色", "蓝", "绿", "红", "灰度" };
static CString s_szMaskType[] = { "原始","轮廓的缩扩","矩形","圆环","开闭","圆" };

// pwwParamDlg 对话框

IMPLEMENT_DYNAMIC(pwwParamDlg, CDialogEx)

pwwParamDlg::pwwParamDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(pwwParamDlg::IDD, pParent)
{
	m_pCheck = NULL;
	m_bsucc = false;
}

pwwParamDlg::~pwwParamDlg()
{
}

void pwwParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCPROPERTYGRID1, m_pwwProp);
}

BEGIN_MESSAGE_MAP(pwwParamDlg, CDialogEx)
	ON_WM_MOUSEWHEEL()
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT8, &pwwParamDlg::OnEnChangeEdit8)
	ON_EN_CHANGE(IDC_EDIT10, &pwwParamDlg::OnEnChangeEdit10)
	ON_EN_CHANGE(IDC_EDIT11, &pwwParamDlg::OnEnChangeEdit11)
	ON_EN_CHANGE(IDC_EDIT12, &pwwParamDlg::OnEnChangeEdit12)
	ON_EN_CHANGE(IDC_EDIT13, &pwwParamDlg::OnEnChangeEdit13)
	ON_EN_CHANGE(IDC_EDIT14, &pwwParamDlg::OnEnChangeEdit14)
	ON_BN_CLICKED(IDC_CHECK3, &pwwParamDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &pwwParamDlg::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK5, &pwwParamDlg::OnBnClickedCheck5)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &pwwParamDlg::OnDeltaposSpin3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, &pwwParamDlg::OnDeltaposSpin4)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, &pwwParamDlg::OnDeltaposSpin5)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN6, &pwwParamDlg::OnDeltaposSpin6)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN7, &pwwParamDlg::OnDeltaposSpin7)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN8, &pwwParamDlg::OnDeltaposSpin8)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &pwwParamDlg::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &pwwParamDlg::OnDeltaposSpin2)
	ON_EN_CHANGE(IDC_EDIT7, &pwwParamDlg::OnEnChangeEdit7)
	ON_EN_CHANGE(IDC_EDIT6, &pwwParamDlg::OnEnChangeEdit6)
END_MESSAGE_MAP()


// pwwParamDlg 消息处理程序
//设计和显示图片


BOOL pwwParamDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_bsucc = false;
	// TODO:  在此添加额外的初始化
#ifdef BGCOLOR
	m_brush.CreateSolidBrush(BGCOLOR); // 设置画刷颜色
#endif
	RECT rect;
	mouseKeyCtr& MKC = ((checkPatch*)m_pCheck)->m_myParam;
	myOnMouse fun[] = { onMouse0,onMouse1,onMouse2 };
	uint hisid[] = { IDC_STATIC_histogram_1,IDC_STATIC_histogram_2,IDC_STATIC_histogram_3 };
	for (int i = 0; i < 3; i++)
	{
		string str = MKC.m_hist[i].m_hisWinName;
		cv::namedWindow(str, cv::WINDOW_NORMAL);
		HWND hWnd = (HWND)cvGetWindowHandle(str.c_str());
		HWND hParent = ::GetParent(hWnd);
		::SetParent(hWnd, GetDlgItem(hisid[i])->GetSafeHwnd());
		::ShowWindow(hParent, SW_HIDE);
		GetDlgItem(hisid[i])->GetClientRect(&rect);
		cv::resizeWindow(str, rect.right, rect.bottom);
		cv::setMouseCallback(str.c_str(), fun[i], &MKC.m_hist[i]);
	}
	
	pwwPropIni();
	
	CString str;
	uint uID[] = { IDC_EDIT8, IDC_EDIT10, IDC_EDIT11, IDC_EDIT12, IDC_EDIT13, IDC_EDIT14, IDC_CHECK3, IDC_CHECK4, IDC_CHECK5 };
	for (int i = 0; i < 3; i++)
	{
		str.Format("%d", MKC.m_tpltPa.sThdMin[i]);
		GetDlgItem(uID[i * 2])->SetWindowTextA(str);
		str.Format("%d", MKC.m_tpltPa.sThdMax[i]);
		GetDlgItem(uID[i * 2 + 1])->SetWindowTextA(str);
		((CButton*)GetDlgItem(uID[i + 6]))->SetCheck(MKC.m_tpltPa.mode[i]);
	}
	str.Format("%d", MKC.m_tpltPa.xMinConnNum);
	GetDlgItem(IDC_EDIT6)->SetWindowTextA(str);
	str.Format("%d", MKC.m_tpltPa.xNoConnNum);
	GetDlgItem(IDC_EDIT7)->SetWindowTextA(str);
	m_bsucc = true;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
BOOL pwwParamDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_LBUTTONUP)
	{
		if (pMsg->hwnd == GetDlgItem(IDC_STATIC_histogram_1)->GetSafeHwnd() ||
			pMsg->hwnd == GetDlgItem(IDC_STATIC_histogram_2)->GetSafeHwnd() ||
			pMsg->hwnd == GetDlgItem(IDC_STATIC_histogram_3)->GetSafeHwnd())
		{
			CPoint point = pMsg->pt;
			checkHisMouse(point);
		}
	}
	else if (pMsg->message == WM_MOUSEMOVE)
	{
		CPoint point = pMsg->pt;
		checkHisMouse(point);
	}
	return CDialog::PreTranslateMessage(pMsg);
}
void pwwParamDlg::checkHisMouse(CPoint& point)
{
	UINT uid[] = { IDC_STATIC_histogram_1 ,IDC_STATIC_histogram_2,IDC_STATIC_histogram_3 };
	UINT uid1[] = { IDC_EDIT8,  IDC_EDIT11, IDC_EDIT13 };
	UINT uid2[] = { IDC_EDIT10, IDC_EDIT12, IDC_EDIT14 };
	//最后把当前鼠标的坐标转化为相对于rect的坐标
	CRect rect;
	int i = 0;
	mouseKeyCtr& MKC = ((checkPatch*)m_pCheck)->m_myParam;
	for (; i < 3; i++)
	{
		CWnd* pWnd = GetDlgItem(uid[i]);
		pWnd->ScreenToClient(&rect);
		pWnd->GetWindowRect(&rect);
		if (rect.PtInRect(point))
		{
			CString str;
			str.Format(_T("%d"), *MKC.m_hist[i].m_hisPa.p_min);
			GetDlgItem(uid1[i])->SetWindowText(str);
			str.Format(_T("%d"), *MKC.m_hist[i].m_hisPa.p_max);
			GetDlgItem(uid2[i])->SetWindowText(str);
			break;
		}
	}
	if (i == 3)//如果鼠标移出直方图控件 还没有释放 清除标记
	{
		for (i = 0; i < 3; i++)
		{
			if (MKC.m_hist[i].m_iSelectState)
			{
				CString str;
				str.Format(_T("%d"), *MKC.m_hist[i].m_hisPa.p_min);
				GetDlgItem(uid1[i])->SetWindowText(str);
				str.Format(_T("%d"), *MKC.m_hist[i].m_hisPa.p_max);
				GetDlgItem(uid2[i])->SetWindowText(str);
				MKC.m_hist[i].m_iSelectState = 0;
			}
		}
	}
}

BOOL pwwParamDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	{
		UINT uid[] = { IDC_EDIT8,  IDC_EDIT11, IDC_EDIT13 };
		UINT uid2[] = { IDC_EDIT10, IDC_EDIT12, IDC_EDIT14 };
		for (int i = 0; i < 3; i++)
		{
			if (setHisMouseWheelL(uid[i], i, zDelta))
			{
				break;
			}
			if (setHisMouseWheelH(uid2[i], i, zDelta))
			{
				break;
			}
		}
	}
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

#ifdef BGCOLOR
HBRUSH pwwParamDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	SETBG
		// TODO:  如果默认的不是所需画笔，则返回另一个画笔
		return hbr;
}
#endif


void pwwParamDlg::OnEnChangeEdit8()
{
	setHisEditL(IDC_EDIT8, 0);
}


void pwwParamDlg::OnEnChangeEdit10()
{
	setHisEditH(IDC_EDIT10, 0);
}


void pwwParamDlg::OnEnChangeEdit11()
{
	setHisEditL(IDC_EDIT11, 1);
}


void pwwParamDlg::OnEnChangeEdit12()
{
	setHisEditH(IDC_EDIT12, 1);
}


void pwwParamDlg::OnEnChangeEdit13()
{
	setHisEditL(IDC_EDIT13, 2);
}


void pwwParamDlg::OnEnChangeEdit14()
{
	setHisEditH(IDC_EDIT14, 2);
}

void pwwParamDlg::setHisEditL(UINT uid, int id)
{
	CString str;
	(GetDlgItem(uid))->GetWindowText(str);
	int a = atoi(str.GetBuffer());
	mouseKeyCtr& MKC = ((checkPatch*)m_pCheck)->m_myParam;
	histogram_data_st* pdata = &MKC.m_hist[id].m_hisPa;
	if ((a >= 0) && (a < *pdata->p_max))
	{
		*pdata->p_min = a;
		MKC.m_hist[id].draw_map(true);
	}
}

void pwwParamDlg::setHisEditH(UINT uid, int id)
{
	CString str;
	(GetDlgItem(uid))->GetWindowText(str);
	int a = atoi(str.GetBuffer());
	mouseKeyCtr& MKC = ((checkPatch*)m_pCheck)->m_myParam;
	histogram_data_st* pdata = &MKC.m_hist[id].m_hisPa;
	if ((a < 256) && (a > *pdata->p_min))
	{
		*pdata->p_max = a;
		MKC.m_hist[id].draw_map(true);
	}
}

void pwwParamDlg::OnBnClickedCheck3()
{
	setHisCheck(IDC_CHECK3, 0);
}


void pwwParamDlg::OnBnClickedCheck4()
{
	setHisCheck(IDC_CHECK4, 1);
}


void pwwParamDlg::OnBnClickedCheck5()
{
	setHisCheck(IDC_CHECK5, 2);
}

void pwwParamDlg::setHisCheck(UINT uid, int id)
{
	mouseKeyCtr& MKC = ((checkPatch*)m_pCheck)->m_myParam;
	if (((CButton*)GetDlgItem(uid))->GetCheck() == 1)
		*MKC.m_hist[id].m_hisPa.p_sel = 1;
	else
		*MKC.m_hist[id].m_hisPa.p_sel = 0;

	MKC.m_hist[id].draw_map(true);
	((checkPatch*)m_pCheck)->m_myParam.design();
}

void pwwParamDlg::OnDeltaposSpin3(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	setHisSpinL(IDC_EDIT8, 0, pNMUpDown->iDelta);
	*pResult = 0;
}

void pwwParamDlg::OnDeltaposSpin4(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	setHisSpinH(IDC_EDIT10, 0, pNMUpDown->iDelta);
	*pResult = 0;
}

void pwwParamDlg::OnDeltaposSpin5(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	setHisSpinL(IDC_EDIT11, 1, pNMUpDown->iDelta);
	*pResult = 0;
}

void pwwParamDlg::OnDeltaposSpin6(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	setHisSpinH(IDC_EDIT12, 1, pNMUpDown->iDelta);
	*pResult = 0;
}

void pwwParamDlg::OnDeltaposSpin7(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	setHisSpinL(IDC_EDIT13, 2, pNMUpDown->iDelta);
	*pResult = 0;
}

void pwwParamDlg::OnDeltaposSpin8(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	setHisSpinH(IDC_EDIT14, 2, pNMUpDown->iDelta);
	*pResult = 0;
}

void pwwParamDlg::setHisSpinL(UINT uid, int id, int iDelta)
{
	mouseKeyCtr& MKC = ((checkPatch*)m_pCheck)->m_myParam;
	histogram_data_st* pdata = &MKC.m_hist[id].m_hisPa;
	short a = *pdata->p_min; // a a
	if (iDelta == -1) // 如果此值为-1 , 说明点击了Spin的往下的箭头
	{
		if (a < 255)
		{
			a++;
		}
	}
	else if (iDelta == 1) // 如果此值为1, 说明点击了Spin的往上的箭头
	{
		if (a > 0)
		{
			a--;
		}
	}

	(GetDlgItem(uid))->SetFocus();

	if (a >= *pdata->p_max)
	{
	}
	else
	{
		CString str;
		str.Format(_T("%d"), a);
		(GetDlgItem(uid))->SetWindowText(str);
		((checkPatch*)m_pCheck)->m_myParam.design();
	}
}

void pwwParamDlg::setHisSpinH(UINT uid, int id, int iDelta)
{
	mouseKeyCtr& MKC = ((checkPatch*)m_pCheck)->m_myParam;
	histogram_data_st* pdata = &MKC.m_hist[id].m_hisPa;
	short a = *pdata->p_max; // a a
	if (iDelta == -1) // 如果此值为-1 , 说明点击了Spin的往下的箭头
	{
		if (a < 255)
		{
			a++;
		}
	}
	else if (iDelta == 1) // 如果此值为1, 说明点击了Spin的往上的箭头
	{
		if (a > 0)
		{
			a--;
		}
	}

	(GetDlgItem(uid))->SetFocus();

	if (a <= *pdata->p_min)
	{
	}
	else
	{
		CString str;
		str.Format(_T("%d"), a);
		(GetDlgItem(uid))->SetWindowText(str);
		((checkPatch*)m_pCheck)->m_myParam.design();
	}
}

bool pwwParamDlg::setHisMouseWheelL(UINT uid, int id, short zDelta)
{
	if (GetFocus() == GetDlgItem(uid))
	{
		mouseKeyCtr& MKC = ((checkPatch*)m_pCheck)->m_myParam;
		histogram_data_st* pdata = &MKC.m_hist[id].m_hisPa;
		short a = *pdata->p_min; // a a
		if (zDelta > 0)
		{
			if (a < 255)
			{
				a++;
			}
		}
		else
		{
			if (a > 1)
			{
				a--;
			}
		}

		if (a >= *pdata->p_max)
		{
		}
		else
		{
			CString str;
			str.Format(_T("%d"), a);
			(GetDlgItem(uid))->SetWindowText(str);
			((checkPatch*)m_pCheck)->m_myParam.design();
		}
		return true;
	}
	return false;
}

bool pwwParamDlg::setHisMouseWheelH(UINT uid, int id, short zDelta)
{
	if (GetFocus() == GetDlgItem(uid))
	{
		mouseKeyCtr& MKC = ((checkPatch*)m_pCheck)->m_myParam;
		histogram_data_st* pdata = &MKC.m_hist[id].m_hisPa;
		short a = *pdata->p_max; // a a
		if (zDelta > 0)
		{
			if (a < 255)
			{
				a++;
			}
		}
		else
		{
			if (a > 1)
			{
				a--;
			}
		}

		if (a <= *pdata->p_min)
		{
		}
		else
		{
			CString str;
			str.Format(_T("%d"), a);
			(GetDlgItem(uid))->SetWindowText(str);
			((checkPatch*)m_pCheck)->m_myParam.design();
		}
		return true;
	}
	return false;
}

void pwwParamDlg::pwwPropIni()
{
#ifdef BGCOLOR
	m_pwwProp.SetCustomColors(BGCOLOR, RGB(255, 255, 255), BGCOLOR, RGB(255, 255, 0), BGCOLOR, RGB(255, 255, 0), BGCOLOR);
#endif
	mouseKeyCtr& MKC = ((checkPatch*)m_pCheck)->m_myParam;
	m_pwwProp.RemoveAll();
	CString str;
	HDITEM item;
	item.cxy = 120;
	item.mask = HDI_WIDTH;
	CFont pFont;
	pFont.CreatePointFont(180, _T("楷体"));
	m_pwwProp.SetFont(&pFont);
	m_pwwProp.GetHeaderCtrl().SetItem(0, new HDITEM(item));
	CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(_T("总目标属性"));
	pGroup->SetData(1);
	str.Format("%d", MKC.m_tpltPa.m_drawMask - 1);//填充掩膜
	CMFCPropertyGridProperty* pItem = new CMFCPropertyGridProperty(_T("掩膜填充"), str);
	pItem->SetData(12);
	pItem->AddOption(_T("1"));
	pItem->AddOption(_T("0"));
	pItem->AllowEdit(FALSE);
	pGroup->AddSubItem(pItem);
	str.Format("%d", MKC.m_tpltPa.m_percent);//最小面积
	pItem = new CMFCPropertyGridProperty(_T("面积"), str, _T("最小面积"));
	pItem->SetData(13);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%d", MKC.m_tpltPa.m_percentMax);//最大面积
	pItem = new CMFCPropertyGridProperty(_T(""), str, _T("最大面积"));
	pItem->SetData(14);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%d", MKC.m_tpltPa.iRegNumMin);//最少目标数
	pItem = new CMFCPropertyGridProperty(_T("目标数"), str, _T("最小目标数"));
	pItem->SetData(15);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%d", MKC.m_tpltPa.iRegNumMax);//最多目标数
	pItem = new CMFCPropertyGridProperty(_T(""), str, _T("最大目标数"));
	pItem->SetData(16);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	m_pwwProp.AddProperty(pGroup);

	pGroup = new CMFCPropertyGridProperty(_T("目标属性"));
	pGroup->SetData(2);
	str.Format("%d", MKC.m_tpltPa.iNoise);//最小面积
	pItem = new CMFCPropertyGridProperty(_T("面积"), str, _T("最小面积"));
	pItem->SetData(21);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%d", MKC.m_tpltPa.iNoiseH);//最大面积
	pItem = new CMFCPropertyGridProperty(_T(""), str, _T("最大面积"));
	pItem->SetData(22);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%d", MKC.m_tpltPa.xNoise);//最小宽度
	pItem = new CMFCPropertyGridProperty(_T("宽度"), str, _T("最小宽度"));
	pItem->SetData(23);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%d", MKC.m_tpltPa.xNoiseH);//最大宽度
	pItem = new CMFCPropertyGridProperty(_T(""), str, _T("最大宽度"));
	pItem->SetData(24);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%d", MKC.m_tpltPa.yNoise);//最小高
	pItem = new CMFCPropertyGridProperty(_T("高度"), str, _T("最小高度"));
	pItem->SetData(25);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%d", MKC.m_tpltPa.yNoiseH);//最大高
	pItem = new CMFCPropertyGridProperty(_T(""), str, _T("最大高度"));
	pItem->SetData(26);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%d", MKC.m_tpltPa.contourParam.numL);//轮廓最小数
	pItem = new CMFCPropertyGridProperty(_T("轮廓数"), str, _T("最小轮廓数"));
	pItem->SetData(31);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%d", MKC.m_tpltPa.contourParam.num);//轮廓最大数
	pItem = new CMFCPropertyGridProperty(_T(""), str, _T("最大轮廓数"));
	pItem->SetData(32);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%d", MKC.m_tpltPa.contourParam.minLen);//轮廓长最小数
	pItem = new CMFCPropertyGridProperty(_T("轮廓长"), str, _T("最小轮廓长度"));
	pItem->SetData(33);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%ld", MKC.m_tpltPa.contourParam.maxLen);//轮廓长最大数
	pItem = new CMFCPropertyGridProperty(_T(""), str, _T("最大轮廓长度"));
	pItem->SetData(34);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	if (-1 == MKC.m_tpltPa.contourParam.cornerMode)
		str = _T("所有");
	else if (0 == MKC.m_tpltPa.contourParam.cornerMode)
		str = _T("内");
	else
		str = _T("外");
	pItem = new CMFCPropertyGridProperty(_T("轮廓选择"), str);
	pItem->SetData(35);
	pItem->AddOption(_T("内"));
	pItem->AddOption(_T("外"));
	pItem->AddOption(_T("全部"));
	pItem->AllowEdit(TRUE);
	pGroup->AddSubItem(pItem);
	m_pwwProp.AddProperty(pGroup);


	m_pwwProp.ExpandAll(FALSE);
}

void pwwParamDlg::updatePwwProp()
{
	if (!m_pwwProp.IsWindowVisible())
	{
		return;
	}
	mouseKeyCtr& MKC = ((checkPatch*)m_pCheck)->m_myParam;
	int count = m_pwwProp.GetPropertyCount();
	for (int i = 0; i < count; i++)
	{
		CMFCPropertyGridProperty* pProperty = m_pwwProp.GetProperty(i);
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
					str.Format("%d", MKC.m_tpltPa.m_hasObj);
					break;
				case 2:
					str.Format("%d", MKC.m_tpltPa.m_drawMask - 1);//填充掩膜
					break;
				case 3:
					str.Format("%d", MKC.m_tpltPa.m_percent);//最小面积
					break;
				case 4:
					str.Format("%d", MKC.m_tpltPa.m_percentMax);//最大面积
					break;
				case 5:
					str.Format("%d", MKC.m_tpltPa.iRegNumMin);//最少目标数
					break;
				case 6:
					str.Format("%d", MKC.m_tpltPa.iRegNumMax);//最多目标数
					break;
				}
			}
			else if (2 == subid)
			{
				switch (ssubid)
				{
				case 1:
					str.Format("%d", MKC.m_tpltPa.iNoise);//最小面积
					break;
				case 2:
					str.Format("%d", MKC.m_tpltPa.iNoiseH);//最大面积
					break;
				case 3:
					str.Format("%d", MKC.m_tpltPa.xNoise);//最小宽度
					break;
				case 4:
					str.Format("%d", MKC.m_tpltPa.xNoiseH);//最大宽度
					break;
				case 5:
					str.Format("%d", MKC.m_tpltPa.yNoise);//最小高
					break;
				case 6:
					str.Format("%d", MKC.m_tpltPa.yNoiseH);//最大高
					break;
				}
			}
			else if (3 == subid)
			{
				switch (ssubid)
				{
				case 1:
					str.Format("%d", MKC.m_tpltPa.contourParam.numL);//轮廓最小数
					break;
				case 2:
					str.Format("%d", MKC.m_tpltPa.contourParam.num);//轮廓最大数
					break;
				case 3:
					str.Format("%d", MKC.m_tpltPa.contourParam.minLen);//轮廓长最小数
					break;
				case 4:
					str.Format("%ld", MKC.m_tpltPa.contourParam.maxLen);//轮廓长最大数
					break;
				case 5:
					if (-1 == MKC.m_tpltPa.contourParam.cornerMode)
						str = _T("全部");
					else if (0 == MKC.m_tpltPa.contourParam.cornerMode)
						str = _T("内");
					else
						str = _T("外");
					break;
				default:
					break;
				}
			}
			else if (4 == subid)
			{
				switch (ssubid)
				{
				case 1:
					str.Format("%.3f", MKC.m_tpltPa.m_polarPa.matchRatio);//相似度
					break;
				case 2:
					str.Format("%d", MKC.m_tpltPa.contourParam.inRingFeaNum);//特征点数
					break;
				case 3:
					str.Format("%d", MKC.m_tpltPa.contourParam.outRingFeaNum);//栅格大小
					break;
				case 4:
					str.Format("%.3f", MKC.m_tpltPa.m_polarPa.scope);//旋转范围
					break;
				case 5:
					str.Format("%d", MKC.m_tpltPa.m_tempPa.m_method);//算法类型
					break;
				default:
					break;
				}
			}
			else if (5 == subid)
			{
				switch (ssubid)
				{
				case 1:
					str.Format("%f", MKC.m_tpltPa.m_scale);//图片缩放比例
					break;
				case 2:
					if (MKC.m_tpltPa.m_preTreat >= 0 && MKC.m_tpltPa.m_preTreat < 11)
						str = s_szPreName[MKC.m_tpltPa.m_preTreat];
					else
						str = "不处理";
					break;
				case 3:
					str.Format("%lf", MKC.m_curProc.m_unit);//像素单位
					break;
				case 4:
					if (MKC.m_curProc.m_GrayChannel >= -1 && MKC.m_curProc.m_GrayChannel < 4)
						str = s_szColorName[MKC.m_curProc.m_GrayChannel + 1];
					else
						str = "";
					break;
				default:
					break;
				}
			}
			else if (6 == subid)
			{
				switch (ssubid)
				{
				case 1:
					//掩膜类型参数
					if (MKC.m_tpltPa.m_dllPa.szParam != "")
						str = s_szMaskType[1 + atoi(MKC.m_tpltPa.m_dllPa.szParam.c_str())];
					else
						str = "原始";
					break;
				default:
					break;
				}
			}
			pSubProperty->SetValue(str);
		}
	}
}

LRESULT pwwParamDlg::OnPropertyChanged(WPARAM, LPARAM lParam)
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lParam;
	if (pProp == nullptr)
		return 0;
	int id = (int)pProp->GetData();
	if (id < 10)
	{
		return 0;
	}
	mouseKeyCtr& MKC = ((checkPatch*)m_pCheck)->m_myParam;
	//	CString s = pProp->GetName();  //被改变的参数名  
	COleVariant t = pProp->GetValue(); //改变之后的值  
	//t = pProp->GetOriginalValue();  //改变之前的值  
	CString str;
	str = t.bstrVal;      //从COleVariant到CString  
	int subid = id / 10;
	int ssubid = id % 10;
	if (1 == subid)
	{
		switch (ssubid)
		{
		case 1:
			MKC.m_tpltPa.m_hasObj = atoi(str.GetBuffer(0));//有目标ok
			return 0;
		case 2:
			MKC.m_tpltPa.m_drawMask = atoi(str.GetBuffer(0)) + 1;//填充掩膜
			break;
		case 3:
			MKC.m_tpltPa.m_percent = atoi(str.GetBuffer(0));//最小面积
			break;
		case 4:
			MKC.m_tpltPa.m_percentMax = atoi(str.GetBuffer(0));//最大面积
			break;
		case 5:
			MKC.m_tpltPa.iRegNumMin = atoi(str.GetBuffer(0));//最少目标数
			break;
		case 6:
			MKC.m_tpltPa.iRegNumMax = atoi(str.GetBuffer(0));//最多目标数
			break;
		}
	}
	else if (2 == subid)
	{
		switch (ssubid)
		{
		case 1:
			MKC.m_tpltPa.iNoise = atoi(str.GetBuffer(0));//最小面积
			break;
		case 2:
			MKC.m_tpltPa.iNoiseH = atoi(str.GetBuffer(0));//最大面积
			break;
		case 3:
			MKC.m_tpltPa.xNoise = atoi(str.GetBuffer(0));//最小宽度
			break;
		case 4:
			MKC.m_tpltPa.xNoiseH = atoi(str.GetBuffer(0));//最大宽度
			break;
		case 5:
			MKC.m_tpltPa.yNoise = atoi(str.GetBuffer(0));//最小高
			break;
		case 6:
			MKC.m_tpltPa.yNoiseH = atoi(str.GetBuffer(0));//最大高
			break;
		}
	}
	else if (3 == subid)
	{
		switch (ssubid)
		{
		case 1:
			MKC.m_tpltPa.contourParam.numL = atoi(str.GetBuffer(0));//轮廓最小数
			break;
		case 2:
			MKC.m_tpltPa.contourParam.num = atoi(str.GetBuffer(0));//轮廓最大数
			break;
		case 3:
			MKC.m_tpltPa.contourParam.minLen = atoi(str.GetBuffer(0));//轮廓长最小数
			break;
		case 4:
			MKC.m_tpltPa.contourParam.maxLen = atoi(str.GetBuffer(0));//轮廓长最大数
			break;
		case 5:
			if (str == _T("全部"))
				MKC.m_tpltPa.contourParam.cornerMode = -1;//内外轮廓选择
			else if (str == _T("内"))
				MKC.m_tpltPa.contourParam.cornerMode = 0;//内外轮廓选择
			else
				MKC.m_tpltPa.contourParam.cornerMode = 1;//内外轮廓选择
			break;
		default:
			break;
		}
	}
	else if (4 == subid)
	{
		switch (ssubid)
		{
		case 1:
			MKC.m_tpltPa.m_polarPa.matchRatio = atof(str.GetBuffer(0));//相似度
			return 0;
		case 2:
			MKC.m_tpltPa.contourParam.inRingFeaNum = atoi(str.GetBuffer(0));//特征点数
			return 0;
		case 3:
			MKC.m_tpltPa.contourParam.outRingFeaNum = atoi(str.GetBuffer(0));//栅格大小
			return 0;
		case 4:
			MKC.m_tpltPa.m_polarPa.scope = atof(str.GetBuffer(0));//旋转范围
			return 0;
		case 5:
			MKC.m_tpltPa.m_tempPa.m_method = atoi(str.GetBuffer(0));//算法类型
		default:
			return 0;
		}
	}
	else if (5 == subid)
	{
		switch (ssubid)
		{
		case 1:
			MKC.m_tpltPa.m_scale = atof(str.GetBuffer(0));//图片缩放比例
			return 0;
		case 2:
			MKC.m_tpltPa.m_preTreat = 0;
			for (int k = 0; k < 11; k++)
			{
				if (str == s_szPreName[k])
				{
					MKC.m_tpltPa.m_preTreat = k;
					break;
				}
			}
			break;
		case 3:
			str.Format("%lf", MKC.m_curProc.m_unit);//像素单位
			return 0;
		case 4:
			MKC.m_curProc.m_GrayChannel = -1;
			for (int k = 0; k < 5; k++)
			{
				if (str == s_szColorName[k])
				{
					MKC.m_curProc.m_GrayChannel = k - 1;
					if (MKC.m_bABProc)
					{
						MKC.m_subProc.m_GrayChannel = MKC.m_curProc.m_GrayChannel;
					}
					break;
				}
			}
			return 0;
		default:
			return 0;
		}
	}
	else if (6 == subid)
	{
		switch (ssubid)
		{
		case 1:
			MKC.m_tpltPa.m_dllPa.szParam = "";//掩膜类型参数 默认就是原始
			for (int k = 1; k < 6; k++)//从原始后面开始匹配
			{
				if (str == s_szMaskType[k])
				{
					MKC.m_tpltPa.m_dllPa.szParam = CStrProcess::itoa_radixN(k - 1);
					break;
				}
			}
			break;
		default:
			return 0;
		}
	}
	if (m_bsucc)
		MKC.design();
	return 0;
}

void pwwParamDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	adjust(IDC_EDIT6, pNMUpDown->iDelta);
	*pResult = 0;
}


void pwwParamDlg::OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	adjust(IDC_EDIT7, pNMUpDown->iDelta);
	*pResult = 0;
}
void pwwParamDlg::adjust(UINT cid, int id)
{
	CString str;
	GetDlgItem(cid)->GetWindowTextA(str);
	int num = atoi(str.GetBuffer(0));
	if (id == -1)
		num++;
	else
	{
		if (num > 1)
			num--;
	}
	str.Format("%d", num);
	GetDlgItem(cid)->SetWindowTextA(str);
	GetDlgItem(cid)->SetFocus();
	
}


void pwwParamDlg::OnEnChangeEdit7()
{
	CString str;
	GetDlgItem(IDC_EDIT7)->GetWindowTextA(str);
	mouseKeyCtr& MKC = ((checkPatch*)m_pCheck)->m_myParam;
	MKC.m_tpltPa.xNoConnNum = atoi(str.GetBuffer());
	if (m_bsucc)
		MKC.design();
}


void pwwParamDlg::OnEnChangeEdit6()
{
	CString str;
	GetDlgItem(IDC_EDIT6)->GetWindowTextA(str);
	mouseKeyCtr& MKC = ((checkPatch*)m_pCheck)->m_myParam;
	MKC.m_tpltPa.xMinConnNum = atoi(str.GetBuffer());
	if (m_bsucc)
		MKC.design();
}
