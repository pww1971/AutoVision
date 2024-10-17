// TcpIpDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CaliDlg.h"
// CTcpIpDlg 对话框

IMPLEMENT_DYNAMIC(CCaliDlg, CDialog)

CCaliDlg::CCaliDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCaliDlg::IDD, pParent)
{
}

CCaliDlg::~CCaliDlg()
{

}

void CCaliDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ppList);
	DDX_Control(pDX, IDC_STATIC_PIC, m_myPic);
}

BEGIN_MESSAGE_MAP(CCaliDlg, CDialog)
	ON_BN_CLICKED(IDC_BT_ADD, &CCaliDlg::OnBnClickedBtAdd)
	ON_BN_CLICKED(IDC_BT_Cali, &CCaliDlg::OnBnClickedBtCali)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BT_ROI, &CCaliDlg::OnBnClickedBtRoi)
	ON_BN_CLICKED(IDOK, &CCaliDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCaliDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CTcpIpDlg 消息处理程序
BOOL CCaliDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	/*cv::namedWindow(CaliVIEW, cv::WINDOW_NORMAL);
	HWND hWnd = (HWND)cvGetWindowHandle(CaliVIEW);
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, m_myPic.GetSafeHwnd());
	::ShowWindow(hParent, SW_HIDE);
	RECT rect;
	m_myPic.GetClientRect(&rect);
	cv::resizeWindow(CaliVIEW, rect.right, rect.bottom);
	m_viewPic = ((checkPatch*)m_pData)->m_rawPic;
	if (!m_viewPic.empty())
		imshow(CaliVIEW, m_viewPic);*/

	m_ImageOrg = Mat2HObject(((checkPatch*)m_pData)->m_rawPic);
	GetImageSize(m_ImageOrg, &m_WidthOrg, &m_HeightOrg);
	SetWindowAttr("background_color","gray");
	HWND hwnd1;
	CRect rect;
	GetDlgItem(IDC_STATIC_PIC)->GetWindowRect(&rect);
	hwnd1 = GetDlgItem(IDC_STATIC_PIC)->m_hWnd;
	LONG PictureControlID = (LONG)hwnd1;
	OpenWindow(0, 0, rect.Width(),rect.Height(), PictureControlID, "visible", "", &m_WindowHandleOrg);
	HDevWindowStack::Push(m_WindowHandleOrg);
	SetPart(m_WindowHandleOrg,0,0, rect.Width(),rect.Height());

	if (HDevWindowStack::IsOpen())
	{
		SetColor(HDevWindowStack::GetActive(),"green");//设置颜色
		DispObj(m_ImageOrg, HDevWindowStack::GetActive());	
	}
	//	SetTimer(1,100,NULL);//平移

	m_ppList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ppList.InsertColumn(0, "像素x", LVCFMT_CENTER,80);
	m_ppList.InsertColumn(1, "像素y", LVCFMT_LEFT,80);
	m_ppList.InsertColumn(2, "距离x", LVCFMT_CENTER,80);
	m_ppList.InsertColumn(3, "距离y", LVCFMT_LEFT,80);
	m_ppList.SetColumnCtrlType(0, CCT_EDITBOX);
	m_ppList.SetColumnCtrlType(1, CCT_EDITBOX);
	m_ppList.SetColumnCtrlType(2, CCT_EDITBOX);
	m_ppList.SetColumnCtrlType(3, CCT_EDITBOX);
	
	viewClientList();
	return TRUE;  
}

void CCaliDlg::Zoomfit()
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
void CCaliDlg::ControlFunction(HTuple WindowHandleOrg,HTuple ZoomTrans,HTuple Row,HTuple Col,HTuple RowShif,HTuple ColShif)
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

void CCaliDlg::scaleState(bool s )
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


void CCaliDlg::moveShift()
{
	HTuple   hv_Button, hv_Row, hv_Column;
	SetCheck("~give_error");
	GetMposition(m_WindowHandleOrg, &hv_Row, &hv_Column, &hv_Button);
	SetCheck("give_error");
	HTuple r0,c0;
	r0=(hv_Row-m_RowShifOrg);
	c0=(hv_Column-m_ColShifOrg);

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


void CCaliDlg::viewClientList()
{
	m_ppList.DeleteAllItems();
	S_HaCaliParam& CaliParam = ((checkPatch*)m_pData)->m_CaliParam;
	vector<cv::Point2f>& obj = CaliParam.object;
	int len = obj.size();
	if(0==len)
		return ;
	int len2 = CaliParam.CaliNum;
	if(len!=len2)
		return ;
	vector<cv::Point2f>& scene = CaliParam.scene;
	if (scene.empty())
	{
		for (int i=0;i<len;i++)
			scene.push_back(cv::Point2f(0,0));
	}

	char sz[64];
	for (int i=0;i<len;i++)
	{
		sprintf(sz,"%f",scene[i].x);
		m_ppList.InsertItem(i,sz,0);
		sprintf(sz,"%f",scene[i].y);
		m_ppList.SetItemText(i, 1, sz);

		sprintf(sz,"%f",obj[i].x);
		m_ppList.SetItemText(i, 2, sz);
		sprintf(sz,"%f",obj[i].y);
		m_ppList.SetItemText(i, 3, sz);
	}
	sprintf(sz,"%f",CaliParam.dSh);
	GetDlgItem(IDC_EDIT1)->SetWindowTextA(sz);
	sprintf(sz,"%f",CaliParam.dSw);
	GetDlgItem(IDC_EDIT2)->SetWindowTextA(sz);
}

void CCaliDlg::OnBnClickedBtAdd()
{
	int i = m_ppList.GetItemCount();
	m_ppList.InsertItem(i,"0",0);
	m_ppList.SetItemText(i, 1, "0");
	m_ppList.SetItemText(i, 2, "0");
	m_ppList.SetItemText(i, 3, "0");
}


void CCaliDlg::OnBnClickedBtCali()
{
	int len = m_ppList.GetItemCount();
	if (len==0)
	{
		return;
	}
	cv::Mat m = ((checkPatch*)m_pData)->m_rawPic.clone();
	S_HaCaliParam& CaliParam = ((checkPatch*)m_pData)->m_CaliParam;
	{
		vector<cv::Point2f>& obj = CaliParam.object;
		vector<cv::Point2f>& scene = CaliParam.scene;
		obj.clear();
		scene.clear();
		CString str;
		float x,y;
		for (int i=0;i<len;i++)
		{
			str = m_ppList.GetItemText(i, 0);
			x = atof(str.GetBuffer(0));
			str = m_ppList.GetItemText(i, 1);
			y = atof(str.GetBuffer(0));
			scene.push_back(cv::Point2f(x,y));

			str = m_ppList.GetItemText(i, 2);
			x = atof(str.GetBuffer(0));
			str = m_ppList.GetItemText(i, 3);
			y = atof(str.GetBuffer(0));
			obj.push_back(cv::Point2f(x,y));
		}
		CaliParam.CaliNum = len;
		CaliParam.setHomMat();
		CaliParam.getUs();
		viewClientList();
	}
}


void CCaliDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
	{
		moveShift();
	}
	CDialog::OnTimer(nIDEvent);
}

void CCaliDlg::OnBnClickedBtRoi()
{
	HTuple Row, Column, Phi, Length1, Length2;

	HalconCpp::HRegion Hreg;                     // 区域
	HalconCpp::HImage hImg;

	hImg = m_ImageOrg;
	// 提示信息
	AfxMessageBox("画标定区域,点击鼠标右键确认");
	ClearWindow(m_WindowHandleOrg);
	DispObj(hImg,m_WindowHandleOrg);
	// 画区域
	DrawRectangle2(m_WindowHandleOrg,&Row, &Column, &Phi, &Length1, &Length2);

	Hreg.GenRectangle2(Row, Column, Phi, Length1, Length2);

	HalconCpp::HImage ImageReduced;
	ImageReduced = hImg.ReduceDomain(Hreg);
	//处理roi图像
	S_HaCaliParam& CaliParam = ((checkPatch*)m_pData)->m_CaliParam;
	if(((checkPatch*)m_pData)->checkLocateCheck(ImageReduced))
	{
		viewClientList();
		for (int i=0;i<CaliParam.CaliNum;i++)
		{
			HTuple hv_r,hv_c;
			hv_r = CaliParam.scene[i].x;
			hv_c = CaliParam.scene[i].y;
			DispLine(m_WindowHandleOrg,hv_r-2,hv_c,hv_r+2,hv_c);
			DispLine(m_WindowHandleOrg,hv_r,hv_c-2,hv_r,hv_c+2);
		}
	}
	/*if (CaliParam.getScenePos(((checkPatch*)m_pData)->m_rawPic))
	{
		CaliParam.setHomMat();
		viewClientList();
		for (int i=0;i<CaliParam.CaliNum;i++)
		{
			HTuple hv_r,hv_c;
			hv_r = CaliParam.scene[i].x;
			hv_c = CaliParam.scene[i].y;
			DispLine(m_WindowHandleOrg,hv_r-2,hv_c,hv_r+2,hv_c);
			DispLine(m_WindowHandleOrg,hv_r,hv_c-2,hv_r,hv_c+2);
		}
	}*/
	//显示效果
	HalconCpp::HXLDCont cont;
	cont=Hreg;// region 可转换成 xld
	DispObj(cont,m_WindowHandleOrg);

}

BOOL CCaliDlg::PreTranslateMessage(MSG* pMsg)
{
	CPoint point=pMsg->pt;
	//最后把当前鼠标的坐标转化为相对于rect的坐标
	CWnd *pWnd = GetDlgItem(IDC_STATIC_PIC);
	pWnd->ScreenToClient(&point);
	RECT rect;
	m_myPic.GetClientRect(&rect);
	if( PtInRect(&rect,point) )
	{
		if(pMsg->message == WM_MOUSEWHEEL)
		{
			short zDelta=HIWORD(pMsg->wParam);
			HTuple   hv_Button, hv_Row, hv_Column;
			SetCheck("~give_error");
			GetMposition(m_WindowHandleOrg, &hv_Row, &hv_Column, &hv_Button);
			SetCheck("give_error");
			if (zDelta == 120)
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
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CCaliDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	((checkPatch*)m_pData)->m_CaliParam.writecfg(((checkPatch*)m_pData)->m_cfgDir);
	ShowWindow(0);
//	CDialog::OnOK();
}


void CCaliDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowWindow(0);
//	CDialog::OnCancel();
}
