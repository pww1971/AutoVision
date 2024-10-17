// GuiContrlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Design_GUI_checkPatch.h"
#include "GuiContrlDlg.h"
#include "checkPatch.h"
#include "afxdialogex.h"

ONMOUSE(onMouse2)

// CGuiContrlDlg 对话框

IMPLEMENT_DYNAMIC(CGuiContrlDlg, CDialog)

CGuiContrlDlg::CGuiContrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGuiContrlDlg::IDD, pParent)
{
	m_pCheck = NULL;
	m_dlgSucc=false;	
}

CGuiContrlDlg::~CGuiContrlDlg()
{
}

void CGuiContrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PIC, m_myPic);
}

BEGIN_MESSAGE_MAP(CGuiContrlDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CGuiContrlDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON1, &CGuiContrlDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTONADD, &CGuiContrlDlg::OnBnClickedButtonadd)
	ON_BN_CLICKED(IDC_BUTTONDEL, &CGuiContrlDlg::OnBnClickedButtondel)
	ON_BN_CLICKED(IDC_BUTTON_GENLINE, &CGuiContrlDlg::OnBnClickedButtonGenline)
	ON_BN_CLICKED(IDC_BUTTONADD2, &CGuiContrlDlg::OnBnClickedButtonadd2)
	ON_BN_CLICKED(IDC_BUTTON_FILL, &CGuiContrlDlg::OnBnClickedButtonFill)
	ON_BN_CLICKED(IDC_BUTTON_SCALE, &CGuiContrlDlg::OnBnClickedButtonScale)
	ON_BN_CLICKED(1, &CGuiContrlDlg::OnBnClicked1)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO4, &CGuiContrlDlg::OnCbnSelchangeCombo4)
	ON_WM_MOUSEWHEEL()
	ON_EN_CHANGE(IDC_EDIT15, &CGuiContrlDlg::OnEnChangeEdit15)
	ON_EN_CHANGE(IDC_EDIT16, &CGuiContrlDlg::OnEnChangeEdit16)
	ON_BN_CLICKED(IDC_CHECK6, &CGuiContrlDlg::OnBnClickedCheck6)

	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN9, &CGuiContrlDlg::OnDeltaposSpin9)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN10, &CGuiContrlDlg::OnDeltaposSpin10)
	ON_BN_CLICKED(IDC_BUTTON5, &CGuiContrlDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CGuiContrlDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON4, &CGuiContrlDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON7, &CGuiContrlDlg::OnBnClickedButton7)
END_MESSAGE_MAP()

BOOL CGuiContrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	((checkPatch*)m_pCheck)->m_myParam.setSelNull();
	string strName = ((checkPatch*)m_pCheck)->m_myParam.m_mainWinName;
	cv::namedWindow(strName, cv::WINDOW_NORMAL);
	HWND hWnd = (HWND)cvGetWindowHandle(strName.c_str());
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, m_myPic.GetSafeHwnd());
	::ShowWindow(hParent, SW_HIDE);
	RECT rect;
	m_myPic.GetClientRect(&rect);
	cv::resizeWindow(strName, rect.right, rect.bottom);
	if (!((checkPatch*)m_pCheck)->m_myParam.m_curProc.m_rawPic[0].empty())
		cv::imshow(strName, ((checkPatch*)m_pCheck)->m_myParam.m_curProc.m_rawPic[0]);
	cv::setMouseCallback(strName, onMouse,&((checkPatch*)m_pCheck)->m_myParam);


	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(((checkPatch*)m_pCheck)->m_myParam.m_iHisType);
	((CComboBox*)GetDlgItem(IDC_COMBO4))->SetCurSel(((checkPatch*)m_pCheck)->m_myParam.m_channel);
	GetDlgItem(IDC_EDIT1)->SetWindowTextA(CStrProcess::itoa_radixN(((checkPatch*)m_pCheck)->m_myParam.m_N).c_str());

	string str = ((checkPatch*)m_pCheck)->m_myParam.m_hist[2].m_hisWinName;
	cv::namedWindow(str, cv::WINDOW_NORMAL);
	hWnd = (HWND)cvGetWindowHandle(str.c_str());
	hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_STATIC_histogram_4)->GetSafeHwnd());
	::ShowWindow(hParent, SW_HIDE);
	GetDlgItem(IDC_STATIC_histogram_4)->GetClientRect(&rect);
	cv::resizeWindow(str, rect.right, rect.bottom);
	cv::setMouseCallback(str.c_str(), onMouse2, &((checkPatch*)m_pCheck)->m_myParam.m_hist[2]);

	CString cstr;
	cstr.Format("%d", ((checkPatch*)m_pCheck)->m_myParam.m_tpltPa.sThdMin[2]);
	GetDlgItem(IDC_EDIT15)->SetWindowTextA(cstr);
	cstr.Format("%d", ((checkPatch*)m_pCheck)->m_myParam.m_tpltPa.sThdMax[2]);
	GetDlgItem(IDC_EDIT16)->SetWindowTextA(cstr);
	((CButton*)GetDlgItem(IDC_CHECK6))->SetCheck(((checkPatch*)m_pCheck)->m_myParam.m_tpltPa.mode[2]);

	((checkPatch*)m_pCheck)->m_myParam.scaleView();

	auto algorithmsName = BGS_Factory::Instance()->GetRegisteredAlgorithmsName();

	//	std::cout << "List of available algorithms (" << algorithmsName.size() << "):" << std::endl;
	std::copy(algorithmsName.begin(), algorithmsName.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
	for (const std::string& algorithmName : algorithmsName)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO5))->AddString(algorithmName.c_str());
	}
	GetDlgItem(IDC_COMBO5)->SetWindowTextA(((checkPatch*)m_pCheck)->m_myParam.m_bgAlg.c_str());
	if (3 == ((checkPatch*)m_pCheck)->m_myParam.m_iHisType)
	{
		GetDlgItem(IDC_BUTTON4)->ShowWindow(1);
		GetDlgItem(IDC_BUTTON5)->ShowWindow(1);
		GetDlgItem(IDC_BUTTON6)->ShowWindow(1);
		GetDlgItem(IDC_COMBO5)->ShowWindow(1);
	}
	else
	{
		GetDlgItem(IDC_BUTTON4)->ShowWindow(0);
		GetDlgItem(IDC_BUTTON5)->ShowWindow(0);
		GetDlgItem(IDC_BUTTON6)->ShowWindow(0);
		GetDlgItem(IDC_COMBO5)->ShowWindow(0);
	}
	return TRUE;
}

BOOL CGuiContrlDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_LBUTTONUP)
	{
		if (pMsg->hwnd == GetDlgItem(IDC_STATIC_histogram_4)->GetSafeHwnd())
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
void CGuiContrlDlg::checkHisMouse(CPoint& point)
{
	//最后把当前鼠标的坐标转化为相对于rect的坐标
	CRect rect;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_histogram_4);
	pWnd->ScreenToClient(&rect);
	pWnd->GetWindowRect(&rect);
	if (rect.PtInRect(point))
	{
		CString str;
		str.Format(_T("%d"), *((checkPatch*)m_pCheck)->m_myParam.m_hist[2].m_hisPa.p_min);
		GetDlgItem(IDC_EDIT15)->SetWindowText(str);
		str.Format(_T("%d"), *((checkPatch*)m_pCheck)->m_myParam.m_hist[2].m_hisPa.p_max);
		GetDlgItem(IDC_EDIT16)->SetWindowText(str);
	}
	else//如果鼠标移出直方图控件 还没有释放 清除标记
	{
		if (((checkPatch*)m_pCheck)->m_myParam.m_hist[2].m_iSelectState)
		{
			CString str;
			str.Format(_T("%d"), *((checkPatch*)m_pCheck)->m_myParam.m_hist[2].m_hisPa.p_min);
			GetDlgItem(IDC_EDIT15)->SetWindowText(str);
			str.Format(_T("%d"), *((checkPatch*)m_pCheck)->m_myParam.m_hist[2].m_hisPa.p_max);
			GetDlgItem(IDC_EDIT16)->SetWindowText(str);
			((checkPatch*)m_pCheck)->m_myParam.m_hist[2].m_iSelectState = 0;
		}
	}
}

BOOL CGuiContrlDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (setHisMouseWheelL(IDC_EDIT15, 2, zDelta))
	{
		;
	}
	else if (setHisMouseWheelH(IDC_EDIT16, 2, zDelta))
	{
		;
	}
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CGuiContrlDlg::OnCbnSelchangeCombo1()
{
	int id = ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();
	if (id > -1)
	{
		((checkPatch*)m_pCheck)->m_myParam.m_iHisType = id;
		if (3 == id) 
		{
			GetDlgItem(IDC_BUTTON4)->ShowWindow(1);
			GetDlgItem(IDC_BUTTON5)->ShowWindow(1);
			GetDlgItem(IDC_BUTTON6)->ShowWindow(1);
			GetDlgItem(IDC_COMBO5)->ShowWindow(1);
		}
		else
		{
			GetDlgItem(IDC_BUTTON4)->ShowWindow(0);
			GetDlgItem(IDC_BUTTON5)->ShowWindow(0);
			GetDlgItem(IDC_BUTTON6)->ShowWindow(0);
			GetDlgItem(IDC_COMBO5)->ShowWindow(0);
		}
	}
}

void CGuiContrlDlg::OnBnClickedButton1()
{
	cv::Mat m = ((checkPatch*)m_pCheck)->m_myParam.m_curProc.m_rawPic[0].clone();
	CString str;
	GetDlgItem(IDC_EDIT1)->GetWindowTextA(str);
	((checkPatch*)m_pCheck)->m_myParam.m_N = atoi(str.GetBuffer(0));
	if (((checkPatch*)m_pCheck)->m_myParam.m_N < 2)
	{
		((checkPatch*)m_pCheck)->m_myParam.m_N = 2;
		MessageBox("扫矩大小必须大于1");
	}
	((checkPatch*)m_pCheck)->checkDug(m);
	showMat(((checkPatch*)m_pCheck)->m_myParam.m_mainWinName, m);
}

void CGuiContrlDlg::OnBnClickedButtonadd()
{
	((checkPatch*)m_pCheck)->m_myParam.m_bSelectState=true;
	((checkPatch*)m_pCheck)->m_myParam.m_maskPa.shape = e_RotatedRect;
	((checkPatch*)m_pCheck)->m_myParam.m_bScaleState = false;
}

void CGuiContrlDlg::OnBnClickedButtondel()
{
	((checkPatch*)m_pCheck)->m_myParam.m_bDelState = true;
	((checkPatch*)m_pCheck)->m_myParam.m_bScaleState = false;
}

void CGuiContrlDlg::OnBnClickedButtonGenline()
{
	((checkPatch*)m_pCheck)->genMode();
}

void CGuiContrlDlg::OnBnClickedButtonadd2()
{
	((checkPatch*)m_pCheck)->m_myParam.m_bSelectState=true;
	((checkPatch*)m_pCheck)->m_myParam.m_maskPa.shape = e_ellipse;//e_circle;//
	((checkPatch*)m_pCheck)->m_myParam.m_bScaleState = false;
}

void CGuiContrlDlg::OnBnClickedButtonFill()
{
	((checkPatch*)m_pCheck)->m_myParam.m_maskPa.mixMode = !((checkPatch*)m_pCheck)->m_myParam.m_maskPa.mixMode;
	((checkPatch*)m_pCheck)->m_myParam.m_bScaleState = false;
}

void CGuiContrlDlg::OnBnClickedButtonScale()
{
	((checkPatch*)m_pCheck)->m_myParam.m_bScaleState = !((checkPatch*)m_pCheck)->m_myParam.m_bScaleState;
}

void CGuiContrlDlg::OnBnClicked1()
{
	CString str;
	GetDlgItem(IDC_EDIT1)->GetWindowTextA(str);
	((checkPatch*)m_pCheck)->m_myParam.m_N = atoi(str.GetBuffer(0));
	if (((checkPatch*)m_pCheck)->m_myParam.m_N <2)
	{
		((checkPatch*)m_pCheck)->m_myParam.m_N = 2;
		MessageBox("扫矩大小必须大于1");
	}
	CDialog::OnOK();
}

void CGuiContrlDlg::OnDestroy()
{
	CDialog::OnDestroy();
	((checkPatch*)m_pCheck)->m_myParam.setSelNull();
	// TODO: 在此处添加消息处理程序代码
}

void CGuiContrlDlg::OnCbnSelchangeCombo4()
{
	((checkPatch*)m_pCheck)->m_myParam.m_channel = ((CComboBox*)GetDlgItem(IDC_COMBO4))->GetCurSel();
}




void CGuiContrlDlg::OnEnChangeEdit15()
{
	setHisEditL(IDC_EDIT15, 2);
}

void CGuiContrlDlg::OnEnChangeEdit16()
{
	setHisEditH(IDC_EDIT16, 2);
}

void CGuiContrlDlg::setHisEditL(UINT uid, int id)
{
	CString str;
	(GetDlgItem(uid))->GetWindowText(str);
	int a = atoi(str.GetBuffer());
	histogram_data_st* pdata = &((checkPatch*)m_pCheck)->m_myParam.m_hist[id].m_hisPa;
	if ((a >= 0) && (a < *pdata->p_max))
	{
		*pdata->p_min = a;
		((checkPatch*)m_pCheck)->m_myParam.m_hist[id].draw_map(true);
	}
}

void CGuiContrlDlg::setHisEditH(UINT uid, int id)
{
	CString str;
	(GetDlgItem(uid))->GetWindowText(str);
	int a = atoi(str.GetBuffer());
	histogram_data_st* pdata = &((checkPatch*)m_pCheck)->m_myParam.m_hist[id].m_hisPa;
	if ((a < 256) && (a > *pdata->p_min))
	{
		*pdata->p_max = a;
		((checkPatch*)m_pCheck)->m_myParam.m_hist[id].draw_map(true);
	}
}

void CGuiContrlDlg::OnBnClickedCheck6()
{
	setHisCheck(IDC_CHECK6, 2);
}

void CGuiContrlDlg::setHisCheck(UINT uid, int id)
{
	if (((CButton*)GetDlgItem(uid))->GetCheck() == 1)
		*((checkPatch*)m_pCheck)->m_myParam.m_hist[id].m_hisPa.p_sel = 1;
	else
		*((checkPatch*)m_pCheck)->m_myParam.m_hist[id].m_hisPa.p_sel = 0;

	((checkPatch*)m_pCheck)->m_myParam.m_hist[id].draw_map(true);
	((checkPatch*)m_pCheck)->m_myParam.design();
}

void CGuiContrlDlg::setHisSpinL(UINT uid, int id, int iDelta)
{
	histogram_data_st* pdata = &((checkPatch*)m_pCheck)->m_myParam.m_hist[id].m_hisPa;
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

void CGuiContrlDlg::setHisSpinH(UINT uid, int id, int iDelta)
{
	histogram_data_st* pdata = &((checkPatch*)m_pCheck)->m_myParam.m_hist[id].m_hisPa;
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

bool CGuiContrlDlg::setHisMouseWheelL(UINT uid, int id, short zDelta)
{
	if (GetFocus() == GetDlgItem(uid))
	{
		histogram_data_st* pdata = &((checkPatch*)m_pCheck)->m_myParam.m_hist[id].m_hisPa;
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

bool CGuiContrlDlg::setHisMouseWheelH(UINT uid, int id, short zDelta)
{
	if (GetFocus() == GetDlgItem(uid))
	{
		histogram_data_st* pdata = &((checkPatch*)m_pCheck)->m_myParam.m_hist[id].m_hisPa;
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

void CGuiContrlDlg::OnDeltaposSpin9(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	setHisSpinL(IDC_EDIT15, 2, pNMUpDown->iDelta);
	*pResult = 0;
}

void CGuiContrlDlg::OnDeltaposSpin10(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	setHisSpinH(IDC_EDIT16, 2, pNMUpDown->iDelta);
	*pResult = 0;
}



void CGuiContrlDlg::OnBnClickedButton5()
{
	CFolderPickerDialog fd(getExePath().c_str(), 0, this, 0);
	if (fd.DoModal() == IDOK)
	{
		string dir = fd.GetPathName().GetBuffer(0);
		CDateTime dt;
		dir += "/" + dt.ToString("%Y-%m-%d-%H-%M-%S")+ ".bmp";
		cv::imwrite(dir,((checkPatch*)m_pCheck)->m_myParam.m_curProc.m_rawPic[0]);
	}
}


void CGuiContrlDlg::OnBnClickedButton6()
{
	cv::Mat m = ((checkPatch*)m_pCheck)->m_myParam.m_curProc.m_rawPic[0].clone();
	((checkPatch*)m_pCheck)->checkDug(m);
	showMat(((checkPatch*)m_pCheck)->m_myParam.m_mainWinName, m);
}


void CGuiContrlDlg::OnBnClickedButton4()
{
	CFolderPickerDialog fd(getExePath().c_str(), 0, this, 0);
	if (fd.DoModal() == IDOK)
	{
		string dir = fd.GetPathName().GetBuffer(0);
		vector<string> vct;
		getFileNameVec(dir.c_str(), vct);
		if (vct.size() == 0)
			return;
		BeginWaitCursor();
		try {
			CString algorithmName;
			GetDlgItem(IDC_COMBO5)->GetWindowTextA(algorithmName);
			if ("" == algorithmName)
			{
				AfxMessageBox("选择算法");
				return;
			}
			((checkPatch*)m_pCheck)->m_myParam.m_bgAlg = algorithmName;
			auto bgs = BGS_Factory::Instance()->Create(algorithmName.GetBuffer(0));
			bgs->setShowOutput(false);
			for (int i = 0; i < vct.size(); i++)
			{
				auto img_input = cv::imread(dir+"/"+vct[i], CV_LOAD_IMAGE_COLOR);
				if (img_input.empty())
					continue;
				cv::Mat img_mask;
				cv::Mat& img_bkgmodel = ((checkPatch*)m_pCheck)->m_bgImg;
				bgs->process(img_input, img_mask, img_bkgmodel); // by default, it shows automatically the foreground mask image
				
				if(!img_bkgmodel.empty())
					showMat(((checkPatch*)m_pCheck)->m_myParam.m_mainWinName, img_bkgmodel);
			}

		}
		catch (std::exception& e)
		{
			AfxMessageBox(e.what());
		}
		EndWaitCursor();
	}
}


void CGuiContrlDlg::OnBnClickedButton7()
{
	((checkPatch*)m_pCheck)->m_bgImg = ((checkPatch*)m_pCheck)->m_myParam.m_curProc.m_rawPic[0];
	cv::Mat m = ((checkPatch*)m_pCheck)->m_myParam.m_curProc.m_rawPic[0].clone();
	((checkPatch*)m_pCheck)->checkDug(m);
	showMat(((checkPatch*)m_pCheck)->m_myParam.m_mainWinName, m);
}
