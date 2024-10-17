
// tot_dlg_posDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Design_dlg_posDlg.h"
#include "afxdialogex.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "checkPatch.h"

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框



// Ctot_dlg_posDlg 对话框




CDesign_dlg_posDlg::CDesign_dlg_posDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDesign_dlg_posDlg::IDD, pParent)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pCheck=NULL;
	m_hasIni=false;
}

void CDesign_dlg_posDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_ProcTree);
	DDX_Control(pDX, IDC_STATIC_POSPIC, m_viewStatic);
}

BEGIN_MESSAGE_MAP(CDesign_dlg_posDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_RADIO1, &CDesign_dlg_posDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CDesign_dlg_posDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CDesign_dlg_posDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CDesign_dlg_posDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, &CDesign_dlg_posDlg::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_BT_DEL, &CDesign_dlg_posDlg::OnBnClickedBtDel)
	ON_BN_CLICKED(IDC_BT_DELALL, &CDesign_dlg_posDlg::OnBnClickedBtDelall)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDesign_dlg_posDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BT_OPEN, &CDesign_dlg_posDlg::OnBnClickedBtOpen)
	ON_BN_CLICKED(IDC_BT_SAVE, &CDesign_dlg_posDlg::OnBnClickedBtSave)
	ON_BN_CLICKED(IDC_BT_POS, &CDesign_dlg_posDlg::OnBnClickedBtPos)
	ON_BN_CLICKED(IDC_BT_INFO, &CDesign_dlg_posDlg::OnBnClickedBtInfo)
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// Ctot_dlg_posDlg 消息处理程序

BOOL CDesign_dlg_posDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	UCI::pDis->SetPMainStatic(&m_viewStatic);

	if (NULL == UCI::pDis->m_pMHWinMain)
		m_ProjectViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));

	UCI::pDis->InitThreadCommunication();
	UCI::pDis->m_pMHWinMain->ClearWindow();
	FitImageToClient();

	UCI::pDis->OnMainDisp();

	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(0);

	// 加载视图图像: 
	m_ProcTree.SetImageList(&m_ProjectViewImages, TVSIL_NORMAL);
	OnChangeVisualStyle();
	FillProjectTreeView();
	CRect rect; 
	m_viewStatic.GetWindowRect(&rect);//获取控件的屏幕坐标
	ScreenToClient(&rect);
	CRect rect2;
	GetWindowRect(&rect2);
	ScreenToClient(&rect2);
	if (!m_wndProperties.Create(CST_PosWord14, this, CRect(rect.right + 2, rect.top, rect2.right - 10, rect.bottom), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0(CST_PosWord15);
		return FALSE; // 未能创建
	}
	
	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_PROPERTIES_WND_HC ), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);
	vector <S_Model*>& modelList = ((checkPatch*)m_pCheck)->m_modelList;
	if (modelList.size()==0)
	{
		S_Model* model = new S_Model();
		UCI::pDis->hv_Model = model;
		int curProject = UCI::OPTIONS[CST_PosProcID];
		UCI::ELE.add_One_Element(curProject, ELEMENT_MATCH);
		modelList.push_back(model);
	}

	GetClientRect(&rect);
	m_nOldCx = rect.Width();
	m_nOldCy = rect.Height();
	m_hasIni=true;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDesign_dlg_posDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDesign_dlg_posDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDesign_dlg_posDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDesign_dlg_posDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (GetSafeHwnd() == NULL) 
	{
		return;
	}
	if (m_hasIni==false)
	{
		return;
	}
	// TODO: 在此处添加消息处理程序代码
	float fspx = (float)cx / m_nOldCx;
	float fspy = (float)cy / m_nOldCy;
	CRect rect; 
	UINT nID[] = {IDC_TREE1,IDC_STATIC_POSPIC,ID_VIEW_PROPERTIESWND};
	int len = sizeof(nID)/sizeof(UINT);
	for (int i=0;i<len;i++)
	{
		CWnd * pWnd = GetDlgItem(nID[i]);
		pWnd->GetWindowRect(&rect);
		this->ScreenToClient(&rect);
		//重新计算控件位置和大小
		int nNewx = (int)(rect.left * fspx);
		int nNewy = (int)(rect.top);
		int nNewWidth = (int)(rect.Width() * fspx);
		int nNewHeight = (int)((rect.Height()+rect.top) * fspy-rect.top);
		//调整控件
		pWnd->MoveWindow(nNewx, nNewy, nNewWidth, nNewHeight);
	}
	////获取子窗口（控件）
	//CWnd * pWnd = this->GetWindow(GW_CHILD);
	//while(pWnd != NULL)
	//{
	//	pWnd->GetWindowRect(&rect);
	//	this->ScreenToClient(&rect);
	//	//重新计算控件位置和大小
	//	int nNewx = (int)(rect.left * fspx);
	//	int nNewy = (int)(rect.top * fspy);
	//	int nNewWidth = (int)(rect.Width() * fspx);
	//	int nNewHeight = (int)(rect.Height() * fspy);
	//	//调整控件
	//	pWnd->MoveWindow(nNewx, nNewy, nNewWidth, nNewHeight);

	//	//获取下一个子窗口（控件）
	//	pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	//}

	GetClientRect(&rect);
	m_nOldCx = rect.Width();
	m_nOldCy = rect.Height();

	FitImageToClient();
	
}
void CDesign_dlg_posDlg::FitImageToClient()
{
	int gap = 1; 
	double dRatio = UCI::OPTIONS[CST_PosImageWHR];
	CRect rect;
	m_viewStatic.GetClientRect(rect);

	int w = rect.Width() - gap;
	int h = int(w*dRatio);
	int x = gap;
	int y = gap;

	CRect srect(x,y,w,h );

	UCI::pDis->m_pMHWinMain->SetWindowExtents(0, 0, srect.Width(), srect.Height()); // 只改变图像的显示大小，不改变图像的显示区域

	int width = UCI::OPTIONS[CST_PosImageWidth];
	int height = UCI::OPTIONS[CST_PosImageHeigh];
	UCI::pDis->m_pMHWinMain->SetPart(0, 0,  height - 1, width - 1);
}

void CDesign_dlg_posDlg::OnChangeVisualStyle()
{
	m_ProjectViewImages.DeleteImageList();

	UINT uiBmpId = IDB_FILE_VIEW_24 ;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T(CST_PosWord16), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= ILC_COLOR24;

	m_ProjectViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_ProjectViewImages.Add(&bmp, RGB(255, 0, 255));

	m_ProcTree.SetImageList(&m_ProjectViewImages, TVSIL_NORMAL);
}

int CDesign_dlg_posDlg::GetCurrentSelElementID()
{
	HTREEITEM hCurrentSel = m_ProcTree.GetSelectedItem();

	if (hCurrentSel == NULL) {
		return -1;         // 没有选择
	}

	HTREEITEM parent = m_ProcTree.GetParentItem(hCurrentSel);
	if (parent == NULL) {   // 这个是根项目
		return -1;
	}	
	CString stParentName = m_ProcTree.GetItemText(parent);
	if (stParentName == CST_PosWord17) {
		return -1;     // 这个项目
	}

	return (int)m_ProcTree.GetItemData(hCurrentSel);

}

void CDesign_dlg_posDlg::FillProjectTreeView()
{
	// 取得当前项目的测量元素
	m_ProcTree.DeleteAllItems();       // 先清空一下。

	//	int prid = UCI::OPTIONS[CST_PosProcID];

	HTREEITEM hRoot = m_ProcTree.InsertItem(_T(CST_PosWord17), 0, 0);
	m_ProcTree.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	try
	{
		//HTREEITEM hSrc;
		// ================================================================
		// 插入一些具体的项目

		UINT mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;


		int id2 = 1;                      // 项目编号
		std::string stProName = CST_PosProcName;
		//	std::string stProNote = "项目内容";

		HTREEITEM hRc = m_ProcTree.InsertItem(mask,
			stProName.c_str(), 1, 1, 0, 0, id2, hRoot, NULL);
		if(NULL != UCI::ELE.EleList)
		{
			POSITION pos = UCI::ELE.EleList->GetHeadPosition();
			while (pos != NULL) 
			{
				UCI::OptionsMap *o = &UCI::ELE.EleList->GetNext(pos);

				CString name = (*o)[CST_PosEleName].getcurrentValue();
				int id = atoi((*o)[CST_PosProcID].getcurrentValue().GetBuffer(0));
				m_ProcTree.InsertItem(mask, name, 2, 2, 0, 0, id, hRc, NULL);						
			}					
			m_ProcTree.Expand(hRc, TVE_EXPAND);
		}
		m_ProcTree.Expand(hRoot, TVE_EXPAND);
	}
	catch (std::exception& e) {
		return;
	}

	m_ProcTree.Expand(hRoot, TVE_EXPAND);		

}
int CDesign_dlg_posDlg::UpdataProjectTreeByList()
{
	return m_ProcTree.UpdataProjectTreeByList();
}
UCI::OptionsMap* CDesign_dlg_posDlg::AddOneElementToProjectTree(UCI::OptionsMap * o)
{
	// 1. 先到到当前选择的节点
	HTREEITEM hCurrentSel = m_ProcTree.GetSelectedItem();
	if (hCurrentSel != NULL) {
		HTREEITEM hParent = m_ProcTree.GetParentItem(hCurrentSel);
		if (hParent != NULL) {
			CString name = m_ProcTree.GetItemText(hParent);
			if (name == CST_PosWord17) { // 这是一个项目节点
			}
			else {  // 这是一个元素节点
				//POSITION pos = (POSITION)this->m_wndProjectTree.GetItemData(hCurrentSel);
				return m_ProcTree.AddOneElement(o, hCurrentSel);
			}
		}
	}


	return m_ProcTree.AddOneElement(o,NULL);


}

void CDesign_dlg_posDlg::OnBnClickedRadio1()
{
	UCI::pDis->DisOnRoiDrawCircle();
}


void CDesign_dlg_posDlg::OnBnClickedRadio2()
{
	UCI::pDis->DisOnRoiDrawEllipse();
}


void CDesign_dlg_posDlg::OnBnClickedRadio3()
{
	UCI::pDis->DisOnRoiDrawRectangle();
}


void CDesign_dlg_posDlg::OnBnClickedRadio4()
{
	UCI::pDis->DisOnRoiDrawRectangle2();
}


void CDesign_dlg_posDlg::OnBnClickedRadio5()
{
	UCI::pDis->DisOnRoiDrawAnyLine();
}


void CDesign_dlg_posDlg::OnBnClickedBtDel()
{
	UCI::pDis->DisOnElementDelOne();
}


void CDesign_dlg_posDlg::OnBnClickedBtDelall()
{
	UCI::pDis->DisOnMatchDelAllRoi();
}


void CDesign_dlg_posDlg::OnCbnSelchangeCombo1()
{
	int sel = ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();
	UCI::RIBmenu.iCOMBO_MATCH_ROI_TYPE = (eCOMBO_MATCH_ROI_TYPE)sel;
}


void CDesign_dlg_posDlg::OnBnClickedBtOpen()
{
	UCI::pDis->OnDisButMatchLoadPic();
}


void CDesign_dlg_posDlg::OnBnClickedBtSave()
{
	UCI::pDis->DisOnButMatchSaveModel();
}


void CDesign_dlg_posDlg::OnBnClickedBtPos()
{
	int selectid = 1;// GetCurrentSelElementID();
	if (selectid > 0 && selectid < 10)
	{
		CString str;
		str.Format(CST_PosWord18,selectid);
		UCI::OptionsMap *om = UCI::ELE.GetOptionsMapByEleName(*UCI::ELE.EleList,str);
		UCI::pDis->DisOnRoiFindModel(om);
	}
	else
	{
		FillProjectTreeView();
		AfxMessageBox(CST_PosWord19);
	}
}


void CDesign_dlg_posDlg::OnBnClickedBtInfo()
{
	int selectid = 1;// GetCurrentSelElementID();
	if (selectid > 0 && selectid < 10)
	{
		UCI::pDis->DisOnRoiInfo(selectid);
	}
	else
	{
		FillProjectTreeView();
		AfxMessageBox(CST_PosWord19);
	}
}



BOOL CDesign_dlg_posDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	m_viewStatic.GetWindowRect(&rect);//获取控件的屏幕坐标	
	if (rect.PtInRect(pt))
		UCI::pDis->DisOnMouseWheel(nFlags, zDelta, pt);
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


void CDesign_dlg_posDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(1==nIDEvent)
	{
		if(GetFocus() != GetDlgItem(IDC_STATIC_POSPIC))
			m_viewStatic.SetFocus();
		else
			KillTimer(1);
	}
	CDialogEx::OnTimer(nIDEvent);
}
