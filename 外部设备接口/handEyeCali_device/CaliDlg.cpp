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
	ON_BN_CLICKED(IDC_BT_Cali2, &CCaliDlg::OnBnClickedBtCali2)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BT_ROI, &CCaliDlg::OnBnClickedBtRoi)
	ON_BN_CLICKED(IDC_BT_ROI2, &CCaliDlg::OnBnClickedBtRoi2)
	ON_BN_CLICKED(IDOK, &CCaliDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCaliDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BT_DEL, &CCaliDlg::OnBnClickedBtDel)
END_MESSAGE_MAP()


// CTcpIpDlg 消息处理程序
BOOL CCaliDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	cv::namedWindow(CaliVIEW, cv::WINDOW_NORMAL);
	HWND hWnd = (HWND)cvGetWindowHandle(CaliVIEW);
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, m_myPic.GetSafeHwnd());
	::ShowWindow(hParent, SW_HIDE);
	RECT rect;
	m_myPic.GetClientRect(&rect);
	cv::resizeWindow(CaliVIEW, rect.right, rect.bottom);
	checkPatch::S_caliProc.m_mainWinName = CaliVIEW;
	if (!checkPatch::S_caliProc.m_curProc.m_rawPic[0].empty())
		imshow(CaliVIEW, checkPatch::S_caliProc.m_curProc.m_rawPic[0]);
	cv::setMouseCallback(CaliVIEW, onMouse);
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
void CCaliDlg::viewClientList()
{
	m_ppList.DeleteAllItems();
	S_CaliParam* CaliParam = &((checkPatch*)m_pData)->m_CaliParam;
	vector<cv::Point2f>& obj = CaliParam->object;
	int len = obj.size();
	if(0==len)
		return ;
	int len2 = CaliParam->CaliNum;
	if(len!=len2)
		return ;
	vector<cv::Point2f>& scene = CaliParam->scene;
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
	sprintf(sz,"%f",CaliParam->dSh);
	GetDlgItem(IDC_EDIT1)->SetWindowTextA(sz);
	sprintf(sz,"%f",CaliParam->dSw);
	GetDlgItem(IDC_EDIT2)->SetWindowTextA(sz);
}

void CCaliDlg::listToParam()
{
	S_CaliParam* CaliParam = &((checkPatch*)m_pData)->m_CaliParam;
	vector<cv::Point2f>& obj = CaliParam->object;
	vector<cv::Point2f>& scene = CaliParam->scene;
	obj.clear();
	scene.clear();
	CString str;
	float x,y;
	for (int i=0;i<CaliParam->CaliNum;i++)
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
}

void CCaliDlg::OnBnClickedBtAdd()
{
	int id = m_ppList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (id>-1)
	{
		int i = id+1;
		m_ppList.InsertItem(i,"0",0);
		m_ppList.SetItemText(i, 1, "0");
		m_ppList.SetItemText(i, 2, "0");
		m_ppList.SetItemText(i, 3, "0");
	}
	else
	{
		int i = m_ppList.GetItemCount();
		m_ppList.InsertItem(i,"0",0);
		m_ppList.SetItemText(i, 1, "0");
		m_ppList.SetItemText(i, 2, "0");
		m_ppList.SetItemText(i, 3, "0");
	}
	((checkPatch*)m_pData)->m_CaliParam.CaliNum = m_ppList.GetItemCount();
	listToParam();
}


void CCaliDlg::OnBnClickedBtCali()
{
	int len = m_ppList.GetItemCount();
	if (len == 0)
	{
		return;
	}
	S_CaliParam& CaliParam = ((checkPatch*)m_pData)->m_CaliParam;
	{
		vector<cv::Point2f>& obj = CaliParam.object;
		vector<cv::Point2f>& scene = CaliParam.scene;
		obj.clear();
		scene.clear();
		CString str;
		float x, y;
		for (int i = 0; i < len; i++)
		{
			str = m_ppList.GetItemText(i, 0);
			x = atof(str.GetBuffer(0));
			str = m_ppList.GetItemText(i, 1);
			y = atof(str.GetBuffer(0));
			scene.push_back(cv::Point2f(x, y));

			str = m_ppList.GetItemText(i, 2);
			x = atof(str.GetBuffer(0));
			str = m_ppList.GetItemText(i, 3);
			y = atof(str.GetBuffer(0));
			obj.push_back(cv::Point2f(x, y));
		}
		CaliParam.CaliNum = len;
		CaliParam.setHomMat();
		CaliParam.getUs();
		viewClientList();
	}
}

void CCaliDlg::OnBnClickedBtCali2()
{
	vector<cv::Mat> vm;
	for (int i=0;i< checkPatch::S_caliProc.m_curProc.m_rawPic.size();i++)
		vm.push_back(checkPatch::S_caliProc.m_curProc.m_rawPic[i].clone());
	checkPatch::S_caliProc.m_multiSel.mixPic(vm[0]);
	S_CaliParam* CaliParam = &((checkPatch*)m_pData)->m_CaliParam;
	//处理roi图像
	if (checkPatch::S_caliProc.m_curProc.parallelMatchTplt(vm) > 0)
	{
		vector<S_TpltData*>& vctData = checkPatch::S_caliProc.m_curProc.m_vcTplt[0]->m_vctData;
		vector<cv::Point2f>& scene = CaliParam->scene;
		scene.clear();
		CString str;
		float x, y;
		int len = vctData.size();			
		vm[0] = checkPatch::S_caliProc.m_curProc.m_rawPic[0];
		cv::Mat m2 = vm[0].clone();
		for (int i = 0; i < len; i++)
		{
			x = vctData[i]->m_roiRect.center.x;
			y = vctData[i]->m_roiRect.center.y;
			scene.push_back(cv::Point(x, y));
			cv::line(vm[0], cv::Point(x - 4, y), cv::Point(x + 4, y), cv::Scalar(0,255,0),2);
			//绘制竖线
			cv::line(vm[0], cv::Point(x, y - 4), cv::Point(x, y + 4), cv::Scalar(0, 255, 0),2);

		}
		checkPatch::S_caliProc.viewMainWin(vm);
		checkPatch::S_caliProc.m_curProc.m_rawPic[0] = m2;
		viewClientList();
	}
}

void CCaliDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
	{
	}
	CDialog::OnTimer(nIDEvent);
}

void CCaliDlg::OnBnClickedBtRoi()
{
	checkPatch::S_caliProc.m_bSelectState = true;
	checkPatch::S_caliProc.m_maskPa.shape = e_RotatedRect;//e_rectangle;
	checkPatch::S_caliProc.m_bScaleState = false;
}
void CCaliDlg::OnBnClickedBtRoi2()
{
	checkPatch::S_caliProc.m_bDelState = true;
	checkPatch::S_caliProc.m_bScaleState = false;
}

BOOL CCaliDlg::PreTranslateMessage(MSG* pMsg)
{
	//if (pMsg->message == WM_MOUSEWHEEL)
	//{
	//	CPoint point = pMsg->pt;
	//	//最后把当前鼠标的坐标转化为相对于rect的坐标
	//	CWnd* pWnd = GetDlgItem(IDC_STATIC_PIC);
	//	pWnd->ScreenToClient(&point);
	//	RECT rect;
	//	m_myPic.GetClientRect(&rect);
	//	if (PtInRect(&rect, point))
	//		checkPatch::S_caliProc.m_bScaleState = true;
	//	else 
	//		checkPatch::S_caliProc.m_bScaleState = false;
	//	/*short zDelta = HIWORD(pMsg->wParam);
	//	if (zDelta == 120)
	//	{
	//		onMouse(cv::EVENT_MOUSEWHEEL);
	//	}
	//	else
	//		onMouse(cv::EVENT_MOUSEHWHEEL);*/
	//}
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


void CCaliDlg::OnBnClickedBtDel()
{
	int id = m_ppList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (id > -1)
	{
		m_ppList.DeleteItem(id);
	}
	else
	{
		id = m_ppList.GetItemCount();
		if (id>0)
			m_ppList.DeleteItem(id);
	}	
	listToParam();
}
