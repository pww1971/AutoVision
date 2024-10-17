
// TestHalconDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestHalconDlg.h"
#include "afxdialogex.h"
#include "HalconCpp.h" 
#include "checkPatch.h"
#include "Design_GUI_checkPatch.h"

CTestHalconDlg::CTestHalconDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestHalconDlg::IDD, pParent)
{
	m_pCheck = NULL;
	m_dlgSucc=false;	
}

void CTestHalconDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestHalconDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_RBUTTONDOWN()

ON_WM_MOUSEWHEEL()
ON_WM_TIMER()
ON_CBN_SELCHANGE(IDC_COMBO1, &CTestHalconDlg::OnCbnSelchangeCombo1)
ON_CBN_SELCHANGE(IDC_COMBO2, &CTestHalconDlg::OnCbnSelchangeCombo2)
ON_BN_CLICKED(IDC_BUTTON1, &CTestHalconDlg::OnBnClickedButton1)
ON_EN_CHANGE(IDC_EDIT1, &CTestHalconDlg::OnEnChangeEdit1)
ON_BN_CLICKED(IDC_CHECK1, &CTestHalconDlg::OnBnClickedCheck1)
ON_CBN_SELCHANGE(IDC_COMBO3, &CTestHalconDlg::OnCbnSelchangeCombo3)
END_MESSAGE_MAP()


// CTestHalconDlg 消息处理程序

BOOL CTestHalconDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	GetImageSize(m_ImageOrg, &m_WidthOrg, &m_HeightOrg);
	SetWindowAttr("background_color","gray");
	HWND hwnd1;
	CRect rect;
	GetDlgItem(IDC_STATIC)->GetWindowRect(&rect);
	hwnd1 = GetDlgItem(IDC_STATIC)->m_hWnd;
	LONG PictureControlID = (LONG)hwnd1;
	OpenWindow(0, 0, rect.Width(),rect.Height(), PictureControlID, "visible", "", &m_WindowHandleOrg);
	HDevWindowStack::Push(m_WindowHandleOrg);
	SetPart(m_WindowHandleOrg,0,0, rect.Width(),rect.Height());

	if (HDevWindowStack::IsOpen())
	{
		SetColor(HDevWindowStack::GetActive(),"red");//设置颜色
		DispObj(m_ImageOrg, HDevWindowStack::GetActive());	
	}
	guiIni();
//	SetTimer(1,100,NULL);//平移
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestHalconDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestHalconDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}
//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTestHalconDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTestHalconDlg::Zoomfit()
{
	HTuple w=m_WidthOrg;
	HTuple h=m_HeightOrg;

	SetPart(m_WindowHandleOrg,0, 0, h,w);
	m_Zoom=1;
	m_ZoomOrg=1;
	/*
	UpdateData(FALSE);
	clear_window(m_WindowHandleOrg);
	disp_obj(m_ImageOrg,m_WindowHandleOrg);*/
}
void CTestHalconDlg::ControlFunction(HTuple WindowHandleOrg,HTuple ZoomTrans,HTuple Row,HTuple Col,HTuple RowShif,HTuple ColShif)
{
	HTuple  hv_Row0, hv_Column0,hv_Row00, hv_Column00,hv_Wt,hv_Ht;
	GetPart(WindowHandleOrg,&hv_Row0, &hv_Column0,&hv_Row00, &hv_Column00);
	hv_Ht=hv_Row00-hv_Row0;
	hv_Wt=hv_Column00-hv_Column0;	
	HTuple r1,c1,r2,c2;
	r1 = hv_Row0+((1-(1/ZoomTrans))*(Row-hv_Row0))- RowShif/ZoomTrans;
	c1 = hv_Column0+((1-(1/ZoomTrans))*(Col-hv_Column0))-ColShif/ZoomTrans;
	r2 = r1+(hv_Ht/ZoomTrans);
	c2= c1+(hv_Wt/ZoomTrans);
	SetPart(WindowHandleOrg,r1,c1,r2,c2);
	/*CString st;
	st.Format("(%d,%d)", Col[0].L(),Row[0].L());
	m_EditPoint=st;
	UpdateData(FALSE);*/
}

void CTestHalconDlg::scaleState(bool s )
{	
	if (s)
	{
		m_Zoom *= 0.8;
		if (m_Zoom < 1)
		{
			m_Zoom = 1;
			Zoomfit();
		}
	}
	else
	{
		m_Zoom /= 0.8;
		if (m_Zoom > 100)
		{
			m_Zoom = 100;
		}
	}
}

BOOL CTestHalconDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	switch(nFlags)
	{
//	case MK_CONTROL:
		Herror  hv_Error;
		HTuple   hv_Button, hv_Row, hv_Column;
		SetCheck("~give_error");
		hv_Error = get_mposition(m_WindowHandleOrg, &hv_Row, &hv_Column, &hv_Button);
		SetCheck("give_error");
		if (hv_Error==2)
		{
			if (zDelta>0)
			{
				scaleState(1);
			}
			else
				scaleState(0);
			ClearWindow(m_WindowHandleOrg);
			//显示
			HTuple ZoomTrans=m_Zoom/m_ZoomOrg;
			m_ZoomOrg=m_Zoom;
			ControlFunction(m_WindowHandleOrg,ZoomTrans,hv_Row,hv_Column,0,0);
			DispObj(m_ImageOrg,m_WindowHandleOrg);
		}
	//	break;
	}
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CTestHalconDlg::moveShift()
{
	Herror  hv_Error;
	HTuple   hv_Button, hv_Row, hv_Column;
	SetCheck("~give_error");
	hv_Error = get_mposition(m_WindowHandleOrg, &hv_Row, &hv_Column, &hv_Button);
	SetCheck("give_error");
	HTuple r0,c0;
	r0=(hv_Row-m_RowShifOrg);
	c0=(hv_Column-m_ColShifOrg);

	if (hv_Error==2)
	{
		//平移
		if (hv_Button==1)//有鼠标按下
		{	
			HTuple ZoomTrans=m_Zoom/m_ZoomOrg;
			ControlFunction(m_WindowHandleOrg,ZoomTrans,m_RowShifOrg,m_ColShifOrg,r0,c0);
			DispObj(m_ImageOrg,m_WindowHandleOrg);
		}
		else
		{
			{
				HTuple  hv_Row0, hv_Column0,hv_Row00, hv_Column00;
				GetPart(m_WindowHandleOrg,&hv_Row0, &hv_Column0,&hv_Row00, &hv_Column00);
				SetPart(m_WindowHandleOrg,hv_Row0, hv_Column0,hv_Row00, hv_Column00);
				ClearWindow(m_WindowHandleOrg);
				DispObj(m_ImageOrg,m_WindowHandleOrg);
			}
			m_RowShifOrg=hv_Row;
			m_ColShifOrg=hv_Column;		
		}
	}
}

void CTestHalconDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
	{
		moveShift();
	}
	CDialog::OnTimer(nIDEvent);
}




void CTestHalconDlg::guiIni()
{
	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(((checkPatch*)m_pCheck)->m_pa.iBarCode);
	if (0 == ((checkPatch*)m_pCheck)->m_pa.iBarCode)
	{
		GetDlgItem(IDC_COMBO1)->EnableWindow(1);
		GetDlgItem(IDC_COMBO2)->EnableWindow(1);
		GetDlgItem(IDC_COMBO3)->EnableWindow(0);
		((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(((checkPatch*)m_pCheck)->m_pa.index);
		((CComboBox*)GetDlgItem(IDC_COMBO2))->SetCurSel(((checkPatch*)m_pCheck)->m_pa.index2);
	}
	else
	{
		GetDlgItem(IDC_COMBO1)->EnableWindow(0);
		GetDlgItem(IDC_COMBO2)->EnableWindow(0);
		GetDlgItem(IDC_COMBO3)->EnableWindow(1);
		((CComboBox*)GetDlgItem(IDC_COMBO3))->SetCurSel(((checkPatch*)m_pCheck)->m_pa.index3);
	}
}

void CTestHalconDlg::OnCbnSelchangeCombo1()
{
	int id = ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();
	if (id > -1)
	{
		((checkPatch*)m_pCheck)->m_pa.index = id;
		((checkPatch*)m_pCheck)->m_pa.changemodel();
	}
}


void CTestHalconDlg::OnCbnSelchangeCombo2()
{
	int id = ((CComboBox*)GetDlgItem(IDC_COMBO2))->GetCurSel();
	if (id > -1)
	{
		((checkPatch*)m_pCheck)->m_pa.index2 = id;
		((checkPatch*)m_pCheck)->m_pa.changemodel();
	}
}


void CTestHalconDlg::OnBnClickedButton1()
{
	string str = ((checkPatch*)m_pCheck)->m_pa.szregex;
	if(((checkPatch*)m_pCheck)->treatPic(m_ImageOrg,str))
		AfxMessageBox(str.c_str());
}


void CTestHalconDlg::OnEnChangeEdit1()
{
	CString str;
	GetDlgItem(IDC_EDIT1)->GetWindowTextA(str);
	((checkPatch*)m_pCheck)->m_pa.szregex = str.GetBuffer(0);
}


void CTestHalconDlg::OnBnClickedCheck1()
{
	if (((checkPatch*)m_pCheck)->m_pa.iBarCode != ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck())
	{
		((checkPatch*)m_pCheck)->m_pa.clearmode();
		((checkPatch*)m_pCheck)->m_pa.iBarCode = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
		((checkPatch*)m_pCheck)->m_pa.changemodel();
	}
	if (0 == ((checkPatch*)m_pCheck)->m_pa.iBarCode)
	{
		GetDlgItem(IDC_COMBO1)->EnableWindow(1);
		GetDlgItem(IDC_COMBO2)->EnableWindow(1);
		GetDlgItem(IDC_COMBO3)->EnableWindow(0);
	}
	else
	{
		GetDlgItem(IDC_COMBO1)->EnableWindow(0);
		GetDlgItem(IDC_COMBO2)->EnableWindow(0);
		GetDlgItem(IDC_COMBO3)->EnableWindow(1);
	}
}



void CTestHalconDlg::OnCbnSelchangeCombo3()
{
	int id = ((CComboBox*)GetDlgItem(IDC_COMBO3))->GetCurSel();
	if (id > -1)
	{
		((checkPatch*)m_pCheck)->m_pa.index3 = id;
		((checkPatch*)m_pCheck)->m_pa.changemodel();
	}
}
