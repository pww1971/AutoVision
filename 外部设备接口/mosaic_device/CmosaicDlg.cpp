// CmosaicDlg.cpp: 实现文件
//

#include "pch.h"
#include "mosaic_device.h"
#include "CmosaicDlg.h"
#include "afxdialogex.h"
#include "mosaicDrv.hpp"

// CmosaicDlg 对话框

IMPLEMENT_DYNAMIC(CmosaicDlg, CDialogEx)

CmosaicDlg::CmosaicDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	m_pCheck = NULL;
}

CmosaicDlg::~CmosaicDlg()
{
}

void CmosaicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCPROPERTYGRID1, m_paramProp);
	DDX_Control(pDX, IDC_LIST1, m_pariList);
}


BEGIN_MESSAGE_MAP(CmosaicDlg, CDialogEx)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BT_ProcAdd, &CmosaicDlg::OnBnClickedBtProcadd)
	ON_BN_CLICKED(IDC_BT_ProcMdy, &CmosaicDlg::OnBnClickedBtProcmdy)
	ON_BN_CLICKED(IDC_BT_ProcDel, &CmosaicDlg::OnBnClickedBtProcdel)
	ON_BN_CLICKED(IDOK, &CmosaicDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BT_ProcAdd2, &CmosaicDlg::OnBnClickedBtProcadd2)
	ON_BN_CLICKED(IDC_BT_ProcDel2, &CmosaicDlg::OnBnClickedBtProcdel2)
END_MESSAGE_MAP()


// CmosaicDlg 消息处理程序


BOOL CmosaicDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	RECT rect;
	cv::namedWindow(mosaitcView, cv::WINDOW_NORMAL);
	HWND hWnd = (HWND)cvGetWindowHandle(mosaitcView);;
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_STATIC_PIC)->GetSafeHwnd());
	::ShowWindow(hParent, SW_HIDE);
	GetDlgItem(IDC_STATIC_PIC)->GetClientRect(&rect);
	cv::resizeWindow(mosaitcView, rect.right, rect.bottom);

	m_pariList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_pariList.InsertColumn(0, "From", LVCFMT_CENTER, 70);
	m_pariList.InsertColumn(1, "To", LVCFMT_LEFT, 70);
	m_pariList.SetColumnCtrlType(0, CCT_EDITBOX);
	m_pariList.SetColumnCtrlType(1, CCT_EDITBOX);
	listUpdata();
	GetDlgItem(IDC_EDIT1)->SetWindowTextA("0");
	GetDlgItem(IDC_EDIT2)->SetWindowTextA("-1");
	propIni();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
void CmosaicDlg::listUpdata()
{
	mosaicHarris* mos = (mosaicHarris*)m_pCheck;
	m_pariList.DeleteAllItems();
	int len = mos->hv_From.TupleLength();
	if (0 == len)
		return;
	char sz[32];
	for (int i = 0; i < len; i++)
	{
		sprintf(sz, "%d", mos->hv_From[i].I());
		m_pariList.InsertItem(i, sz, 0);
		sprintf(sz, "%d", mos->hv_To[i].I());
		m_pariList.SetItemText(i, 1, sz);
	}
	mos->showAllImage();
}
void CmosaicDlg::propIni()
{
	mosaicHarris* mos = (mosaicHarris*)m_pCheck;
	m_paramProp.RemoveAll();
	CString str;
	HDITEM item;
	item.cxy = 120;
	item.mask = HDI_WIDTH;
	CFont pFont;
	pFont.CreatePointFont(180, _T("楷体"));
	m_paramProp.SetFont(&pFont);
	m_paramProp.GetHeaderCtrl().SetItem(0, new HDITEM(item));
	CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(_T("Harris属性"));
	pGroup->SetData(1);
	str.Format("%f", mos->hv_Alpha.D());
	CMFCPropertyGridProperty* pItem = new CMFCPropertyGridProperty(_T("Alpha"), str,_T("平方梯度矩阵的平方迹的权值"));
	pItem->SetData(11);
	pItem->AllowEdit(TRUE);
	pGroup->AddSubItem(pItem);
	str.Format("%d", mos->hv_Threshold.I());//
	pItem = new CMFCPropertyGridProperty(_T("Threshold"), str,_T("点的最小滤波器响应"));
	pItem->SetData(12);
	pItem->AllowEdit(TRUE);
	pGroup->AddSubItem(pItem);
	str.Format("%d", mos->hv_SigmaGrad.I());//
	pItem = new CMFCPropertyGridProperty(_T("Grad"), str, _T("用于计算梯度的平滑量"));
	pItem->SetData(13);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%d", mos->hv_SigmaSmooth.I());// 
	pItem = new CMFCPropertyGridProperty(_T("Smooth"), str, _T("用于梯度积分的平滑量"));
	pItem->SetData(14);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	m_paramProp.AddProperty(pGroup);

	pGroup = new CMFCPropertyGridProperty(_T("拼接算子"));
	pGroup->SetData(2);
	str.Format("%d", mos->hv_MaskSize.I());// 
	pItem = new CMFCPropertyGridProperty(_T("掩膜尺寸"), str, _T("将图片分为很多小格子，在小格子内进行匹配"));
	pItem->SetData(21);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%d", mos->hv_RowTolerance.I());
	pItem = new CMFCPropertyGridProperty(_T("搜索高"), str, _T("匹配搜索窗口的半高"));
	pItem->SetData(22);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%d", mos->hv_ColTolerance.I());
	pItem = new CMFCPropertyGridProperty(_T("搜索宽"), str, _T("匹配搜索窗口的半宽"));
	pItem->SetData(23);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%f", mos->hv_Rotation.D());
	pItem = new CMFCPropertyGridProperty(_T("旋转范围"), str, _T("旋转角度范围"));
	pItem->SetData(24);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%d", mos->hv_MatchThreshold.I());
	pItem = new CMFCPropertyGridProperty(_T("匹配阈值"), str, _T("灰度值匹配阈值，ssd，sad度量值低于该值，或ncc高于该值时，接受找到的匹配。"));
	pItem->SetData(25);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%f", mos->hv_DistanceThreshold.D());
	pItem = new CMFCPropertyGridProperty(_T("检查阈值"), str, _T("转换一致性检查的阈值"));
	pItem->SetData(26);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	pItem = new CMFCPropertyGridProperty(_T("转换类型"), mos->m_szPtf.c_str(), _T("projective=投射转换 affine=仿射变换 similarity=相似性变换 rigid=刚性变换"));
	pItem->SetData(27);
	for (int i=0;i<mos->m_PossibleTransformations.size();i++)
	{
		pItem->AddOption(mos->m_PossibleTransformations[i].c_str());
	}
	pItem->AllowEdit(FALSE);
	pGroup->AddSubItem(pItem);	
	str.Format("%d", mos->hv_StartImage.I());
	pItem = new CMFCPropertyGridProperty(_T("中心图像"), str, _T("中心输入图像索引"));
	pItem->SetData(28);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	int len = mos->hv_StackingOrder.TupleLength();
	char sz[32];
	str = "";
	for (int i = 0; i < len; i++)
	{
		sprintf(sz, "%d,", mos->hv_StackingOrder[i].I());
		str += sz;
	}
	pItem = new CMFCPropertyGridProperty(_T("堆叠顺序"), str, _T("拼接图像的堆叠顺序"));
	pItem->SetData(29);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	m_paramProp.AddProperty(pGroup);

	pGroup = new CMFCPropertyGridProperty(_T("附加"));
	pGroup->SetData(3);
	str.Format("%d", mos->m_Vertically);
	pItem = new CMFCPropertyGridProperty(_T("垂直拼接"), str, _T("垂直拼接位置，大于图像数就全部水平拼接"));
	pItem->SetData(31);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	m_paramProp.AddProperty(pGroup);

	m_paramProp.ExpandAll(FALSE);
}
void CmosaicDlg::updateProp()
{
	mosaicHarris* mos = (mosaicHarris*)m_pCheck;
	int count = m_paramProp.GetPropertyCount();
	for (int i = 0; i < count; i++)
	{
		CMFCPropertyGridProperty* pProperty = m_paramProp.GetProperty(i);
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
				{
					str.Format("%f", mos->hv_Alpha);
					break;
				}
				case 2:
				{
					str.Format("%d", mos->hv_Threshold);
					break;
				}
				case 3:
				{
					str.Format("%d", mos->hv_SigmaGrad);
					break;
				}
				case 4:
				{
					str.Format("%d", mos->hv_SigmaSmooth);//
					break;
				}
				
				}
			}
			else if (2 == subid)
			{
				switch (ssubid)
				{
				case 1:
					str.Format("%d", mos->hv_MaskSize);//
					break;
				case 2:
					str.Format("%d", mos->hv_RowTolerance);//
					break;
				case 3:
					str.Format("%d", mos->hv_ColTolerance);//
					break;
				case 4:
					str.Format("%d", mos->hv_Rotation);
					break;
				case 5:
					str.Format("%d", mos->hv_MatchThreshold);
					break;
				case 6:
					str.Format("%f", mos->hv_DistanceThreshold);
					break;
				case 7:
					str = mos->m_szPtf.c_str();
					break;
				case 8:
					str.Format("%d", mos->hv_StartImage);
					break;
				case 9:
				{
					int len = mos->hv_StackingOrder.TupleLength();
					char sz[32];
					str = "";
					for (int i=0;i<len;i++)
					{
						sprintf(sz, "%d,", mos->hv_StackingOrder[i].I());
						str+=sz;
					}
					break;
				}
				}
			}
			else if (3 == subid)
			{
				switch (ssubid)
				{
				case 1:
					str.Format("%d", mos->m_Vertically);//
					break;
				}
			}
			pSubProperty->SetValue(str);
			//	pSubProperty->AllowEdit(bedit);//允许编辑
		}
	}
}
void CmosaicDlg::enableProp()
{

}

LRESULT CmosaicDlg::OnPropertyChanged(WPARAM, LPARAM lParam)
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lParam;
	if (pProp == nullptr)
		return 0;
	int id = (int)pProp->GetData();
	if (id < 10)
	{
		return 0;
	}
	mosaicHarris* mos = (mosaicHarris*)m_pCheck;
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
			mos->hv_Alpha = atof(str.GetBuffer(0));//
			return 0;
		case 2:
			mos->hv_Threshold = atoi(str.GetBuffer(0));
			break;
		case 3:
			mos->hv_SigmaGrad = atoi(str.GetBuffer(0));
			break;
		case 4:
			mos->hv_SigmaSmooth = atoi(str.GetBuffer(0));
			break;
		}
	}
	else if (2 == subid)
	{
		switch (ssubid)
		{
		case 1:
			mos->hv_MaskSize = atoi(str.GetBuffer(0));//
			break;
		case 2:
			mos->hv_RowTolerance = atoi(str.GetBuffer(0));//
			break;
		case 3:
			mos->hv_ColTolerance = atoi(str.GetBuffer(0));//
			break;
		case 4:
			mos->hv_Rotation = atoi(str.GetBuffer(0));//
			break;
		case 5:
			mos->hv_MatchThreshold = atoi(str.GetBuffer(0));//
			break;
		case 6:
			mos->hv_DistanceThreshold = atof(str.GetBuffer(0));//
			break;
		case 7:
			mos->m_szPtf = str.GetBuffer(0);//
			break;
		case 8:
			mos->hv_StartImage = atoi(str.GetBuffer(0));//
			break;
		case 9:
		{
			std::vector<string> vct;
			mos->hv_StackingOrder.Clear();
			CStrProcess::Split(str.GetBuffer(0), ",", vct, true);
			for (int i = 0; i < vct.size(); i++)
				mos->hv_StackingOrder[i] = atoi(vct[i].c_str());//
			break;
		}
		}
	}
	else if (3 == subid)
	{
		switch (ssubid)
		{
		case 1:
			mos->m_Vertically = atoi(str.GetBuffer(0));//
			break;
		}
	}
	SetTimer(1, 1, NULL);
	return 0;
}

void CmosaicDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
	{
		KillTimer(1);
		OnBnClickedBtProcmdy();
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CmosaicDlg::OnBnClickedBtProcadd()
{
	int id = m_pariList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	int n = m_pariList.GetItemCount();
	if (id > -1)
	{
		m_pariList.InsertItem(id + 1, CStrProcess::itoa_radixN(id+1).c_str(), 0);
		m_pariList.SetItemText(id + 1, 1, CStrProcess::itoa_radixN(id+2).c_str());
	}
	else
	{
		id = n;
		m_pariList.InsertItem(id, CStrProcess::itoa_radixN(id+1).c_str(), 0);
		m_pariList.SetItemText(id, 1, CStrProcess::itoa_radixN(id+2).c_str());
	}
}


void CmosaicDlg::OnBnClickedBtProcmdy()
{
	mosaicHarris* mos = (mosaicHarris*)m_pCheck;
	mos->hv_From.Clear();
	mos->hv_To.Clear();
	int num = m_pariList.GetItemCount();
	CString str;
	for (int i = 0; i < num; i++)
	{
		str = m_pariList.GetItemText(i, 0);
		mos->hv_From[i] = atoi(str.GetBuffer(0));
		str = m_pariList.GetItemText(i, 1);
		mos->hv_To[i] = atoi(str.GetBuffer(0));
	}
	try
	{
		cv::Mat m;
		mos->mosaicImage(m);	
		if (m.data)
			imshow(mosaitcView, m);
	}
	catch (HException& except) {
		AfxMessageBox("mosaicImage" + (CString)except.ErrorText().Text());
	}
}


void CmosaicDlg::OnBnClickedBtProcdel()
{
	int id = m_pariList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	int n = m_pariList.GetItemCount();
	if (id > -1)
	{
		m_pariList.DeleteItem(id);
	}
	else
	{
		id = n;
		if (id > 0)
			m_pariList.DeleteItem(id - 1);
		else
			return;
	}
	

}


void CmosaicDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CmosaicDlg::OnBnClickedBtProcadd2()
{
	CString str;
	GetDlgItem(IDC_EDIT1)->GetWindowTextA(str);
	int id = atoi(str.GetBuffer(0));
	GetDlgItem(IDC_EDIT2)->GetWindowTextA(str);
	int id2 = atoi(str.GetBuffer(0));
	mosaicHarris* mos = (mosaicHarris*)m_pCheck;
	std::vector<cv::Mat> vm;
	if (-1 != mos->m_Cam->camPic(id, vm))
	{
		if (-1 == id2)
		{
			mos->m_vctImage.push_back(vm[0]);
		}
		else
			mos->m_vctImage.insert(mos->m_vctImage.begin()+id2,vm[0]);
		mos->showAllImage();
	}
	else
	{
		AfxMessageBox("相机打开错误!");
	}
}


void CmosaicDlg::OnBnClickedBtProcdel2()
{
	CString str;
	GetDlgItem(IDC_EDIT2)->GetWindowTextA(str);
	int id = atoi(str.GetBuffer(0));
	mosaicHarris* mos = (mosaicHarris*)m_pCheck;
	if (id > -1 && id <mos->m_vctImage.size())
	{
		mos->m_vctImage.erase(mos->m_vctImage.begin() + id);
	}
	else if(mos->m_vctImage.size()>0)
	{
		mos->m_vctImage.pop_back();
	}
	mos->showAllImage();
}
