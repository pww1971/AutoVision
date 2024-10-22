
// tot_dlg_posDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Design_dlg_posDlg.h"
#include "afxdialogex.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "checkPatch.h"

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���



// Ctot_dlg_posDlg �Ի���




CDesign_dlg_posDlg::CDesign_dlg_posDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDesign_dlg_posDlg::IDD, pParent)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pCheck = NULL;
	m_hasIni = false;
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
	ON_BN_CLICKED(IDC_BT_OPEN, &CDesign_dlg_posDlg::OnBnClickedBtOpen)
	ON_BN_CLICKED(IDC_BT_SAVE, &CDesign_dlg_posDlg::OnBnClickedBtSave)
	ON_BN_CLICKED(IDC_BT_INFO, &CDesign_dlg_posDlg::OnBnClickedBtInfo)
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BT_CAM_ONE_SHOT, &CDesign_dlg_posDlg::OnBnClickedBtCamOneShot)
	ON_BN_CLICKED(IDC_BT_CALI_INNER, &CDesign_dlg_posDlg::OnBnClickedBtCaliInner)
	ON_BN_CLICKED(IDC_BT_CALI_POSE, &CDesign_dlg_posDlg::OnBnClickedBtCaliPose)
	ON_BN_CLICKED(IDC_BT_CALI_CAPTURE, &CDesign_dlg_posDlg::OnBnClickedBtCaliCapture)
END_MESSAGE_MAP()


// Ctot_dlg_posDlg ��Ϣ�������

BOOL CDesign_dlg_posDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�

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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	UCI::pDis->SetPMainStatic(&m_viewStatic);

	if (NULL == UCI::pDis->m_pMHWinMain)
		m_ProjectViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));

	UCI::pDis->InitThreadCommunication();
	UCI::pDis->m_pMHWinMain->ClearWindow();
	FitImageToClient();

	UCI::pDis->OnMainDisp();

	// ������ͼͼ��: 
	m_ProcTree.SetImageList(&m_ProjectViewImages, TVSIL_NORMAL);
	OnChangeVisualStyle();
	FillProjectTreeView();
	CRect rect;
	m_viewStatic.GetWindowRect(&rect);//��ȡ�ؼ�����Ļ����
	ScreenToClient(&rect);
	if (!m_wndProperties.Create("����", this, CRect(rect.right + 2, rect.top, rect.right + 250, rect.bottom), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ��������ԡ�����\n");
		return FALSE; // δ�ܴ���
	}
	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_PROPERTIES_WND_HC), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);
	vector <S_Cali*>& modelList = ((checkPatch*)m_pCheck)->m_CaliList;
	if (modelList.size() == 0)
	{
		int camN = ((checkPatch*)m_pCheck)->CameraDrv::ini();
		for (int i = 0; i < camN; i++)
		{
			S_Cali* cali = new S_Cali();
			UCI::pDis->hv_Cali = cali;
			int curProject = UCI::OPTIONS["��Ŀ���"];
			UCI::OptionsMap* o = UCI::ELE.add_One_Element(curProject, ELEMENT_CAMERA);
			AddOneElementToProjectTree(o);   // 3. ��ӵ���Ŀ����ȥ
			SetPropertyCurOptionsMap(o);
			ProPertyUpdate();

			modelList.push_back(cali);
		}
	}


	GetClientRect(&rect);
	m_nOldCx = rect.Width();
	m_nOldCy = rect.Height();
	m_hasIni = true;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDesign_dlg_posDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDesign_dlg_posDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	if (m_hasIni == false)
	{
		return;
	}
	// TODO: �ڴ˴������Ϣ����������
	float fspx = (float)cx / m_nOldCx;
	float fspy = (float)cy / m_nOldCy;
	CRect rect;
	UINT nID[] = { IDC_TREE1,IDC_STATIC_POSPIC,ID_VIEW_PROPERTIESWND };
	int len = sizeof(nID) / sizeof(UINT);
	for (int i = 0; i < len; i++)
	{
		CWnd* pWnd = GetDlgItem(nID[i]);
		pWnd->GetWindowRect(&rect);
		this->ScreenToClient(&rect);
		//���¼���ؼ�λ�úʹ�С
		int nNewx = (int)(rect.left * fspx);
		int nNewy = (int)(rect.top);
		int nNewWidth = (int)(rect.Width() * fspx);
		int nNewHeight = (int)((rect.Height() + rect.top) * fspy - rect.top);
		//�����ؼ�
		pWnd->MoveWindow(nNewx, nNewy, nNewWidth, nNewHeight);
	}
	////��ȡ�Ӵ��ڣ��ؼ���
	//CWnd * pWnd = this->GetWindow(GW_CHILD);
	//while(pWnd != NULL)
	//{
	//	pWnd->GetWindowRect(&rect);
	//	this->ScreenToClient(&rect);
	//	//���¼���ؼ�λ�úʹ�С
	//	int nNewx = (int)(rect.left * fspx);
	//	int nNewy = (int)(rect.top * fspy);
	//	int nNewWidth = (int)(rect.Width() * fspx);
	//	int nNewHeight = (int)(rect.Height() * fspy);
	//	//�����ؼ�
	//	pWnd->MoveWindow(nNewx, nNewy, nNewWidth, nNewHeight);

	//	//��ȡ��һ���Ӵ��ڣ��ؼ���
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
	double dRatio = UCI::OPTIONS["ͼ�����"];
	CRect rect;
	m_viewStatic.GetClientRect(rect);

	int w = rect.Width() - gap;
	int h = int(w * dRatio);
	int x = gap;
	int y = gap;

	CRect srect(x, y, w, h);

	UCI::pDis->m_pMHWinMain->SetWindowExtents(0, 0, srect.Width(), srect.Height()); // ֻ�ı�ͼ�����ʾ��С�����ı�ͼ�����ʾ����

	int width = UCI::OPTIONS["ͼ����"];
	int height = UCI::OPTIONS["ͼ��߶�"];
	UCI::pDis->m_pMHWinMain->SetPart(0, 0, height - 1, width - 1);
}

void CDesign_dlg_posDlg::OnChangeVisualStyle()
{
	m_ProjectViewImages.DeleteImageList();

	UINT uiBmpId = IDB_FILE_VIEW_24;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("�޷�����λͼ: %x\n"), uiBmpId);
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
		return -1;         // û��ѡ��
	}

	HTREEITEM parent = m_ProcTree.GetParentItem(hCurrentSel);
	if (parent == NULL) {   // ����Ǹ���Ŀ
		return -1;
	}
	CString stParentName = m_ProcTree.GetItemText(parent);
	if (stParentName == "������Ŀ") {
		return -1;     // �����Ŀ
	}

	return (int)m_ProcTree.GetItemData(hCurrentSel);

}

void CDesign_dlg_posDlg::FillProjectTreeView()
{
	// ȡ�õ�ǰ��Ŀ�Ĳ���Ԫ��
	m_ProcTree.DeleteAllItems();       // �����һ�¡�

	//	int prid = UCI::OPTIONS["��Ŀ���"];

	HTREEITEM hRoot = m_ProcTree.InsertItem(_T("������Ŀ"), 0, 0);
	m_ProcTree.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	try
	{
		//HTREEITEM hSrc;
		// ================================================================
		// ����һЩ�������Ŀ

		UINT mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

		int id2 = 1;                      // ��Ŀ���
		std::string stProName = "��Ŀ����";
		//	std::string stProNote = "��Ŀ����";
		int id = 1;//������
		HTREEITEM hRc = m_ProcTree.InsertItem(mask,
			stProName.c_str(), 1, 1, 0, 0, id2, hRoot, NULL);
		if (NULL != UCI::ELE.EleList)
		{
			POSITION pos = UCI::ELE.EleList->GetHeadPosition();
			while (pos != NULL)
			{
				UCI::OptionsMap* o = &UCI::ELE.EleList->GetNext(pos);

				CString name = (*o)["Ԫ������"].getcurrentValue();
				
				m_ProcTree.InsertItem(mask, name, 2, 2, 0, 0, id++, hRc, NULL);
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

UCI::OptionsMap* CDesign_dlg_posDlg::AddOneElementToProjectTree(UCI::OptionsMap* o)
{
	// 1. �ȵ�����ǰѡ��Ľڵ�
	HTREEITEM hCurrentSel = m_ProcTree.GetSelectedItem();
	if (hCurrentSel != NULL) {
		HTREEITEM hParent = m_ProcTree.GetParentItem(hCurrentSel);
		if (hParent != NULL) {
			CString name = m_ProcTree.GetItemText(hParent);
			if (name == "������Ŀ") { // ����һ����Ŀ�ڵ�
			}
			else {  // ����һ��Ԫ�ؽڵ�
				//POSITION pos = (POSITION)this->m_wndProjectTree.GetItemData(hCurrentSel);
				return m_ProcTree.AddOneElement(o, hCurrentSel);
			}
		}
	}

	return m_ProcTree.AddOneElement(o, NULL);
}

void CDesign_dlg_posDlg::OnBnClickedBtOpen()
{
	UCI::pDis->DisOnOpenImage();
}

void CDesign_dlg_posDlg::OnBnClickedBtSave()
{
	if (NULL != m_pCheck)
		((checkPatch*)m_pCheck)->writeMode();
}

void CDesign_dlg_posDlg::OnBnClickedBtInfo()
{
	int selectid = GetCurrentSelElementID();
	if (selectid > 0 && selectid < 10)
		UCI::pDis->DisOnButCaliShowInfo(selectid); // ��ʾ��ǰ�ı궨��Ϣ
	else
	{
		FillProjectTreeView();
		AfxMessageBox("��ѡ�����");
	}
}

BOOL CDesign_dlg_posDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rect;
	m_viewStatic.GetWindowRect(&rect);//��ȡ�ؼ�����Ļ����	
	if (rect.PtInRect(pt))
		UCI::pDis->DisOnMouseWheel(nFlags, zDelta, pt);
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

void CDesign_dlg_posDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (1 == nIDEvent)
	{
		if (GetFocus() != GetDlgItem(IDC_STATIC_POSPIC))
			m_viewStatic.SetFocus();
		else
			KillTimer(1);
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CDesign_dlg_posDlg::OnBnClickedBtCamOneShot()
{
	cv::Mat m;
	int id = GetCurrentSelElementID();
	if (id > -1 )
	{
		UCI::pDis->hv_Cali = ((checkPatch*)m_pCheck)->m_CaliList[id - 1];
		if (-1 != ((checkPatch*)m_pCheck)->CameraDrv::grapImage(id - 1, m))
		{
			UCI::OPTIONS["ͼ����"] = m.cols;
			UCI::OPTIONS["ͼ��߶�"] = m.rows;
			UCI::OPTIONS["ͼ�����"] = (double)m.rows / m.cols;

			UCI::pDis->m_sIPResult.result_img = UCI::pDis->GrabImageDisp = Mat2HObject(m);
			UCI::pDis->DisOnButCaliCorrect(id);
		}
	}
	else
	{
		FillProjectTreeView();
		AfxMessageBox("�����ʧ��,��ѡ�����");
		return;
	}
}

void CDesign_dlg_posDlg::OnBnClickedBtCaliInner()
{
	int selectid = GetCurrentSelElementID();
	if (selectid > 0 && selectid < 10)
		UCI::pDis->DisOnButCamCaliInner(selectid);
	else
	{
		FillProjectTreeView();
		AfxMessageBox("��ѡ�����");
	}
}

void CDesign_dlg_posDlg::OnBnClickedBtCaliPose()
{
	int selectid = GetCurrentSelElementID();
	if (selectid > 0 && selectid < 10)
		UCI::pDis->DisOnButCamCaliPose(selectid);
	else
	{
		FillProjectTreeView();
		AfxMessageBox("��ѡ�����");
	}
}

void CDesign_dlg_posDlg::OnBnClickedBtCaliCapture()
{
	UCI::pDis->DisOnSaveOnCaliPicture();
}

