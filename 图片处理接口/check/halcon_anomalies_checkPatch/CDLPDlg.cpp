// CDLPDlg.cpp: 实现文件
//

#include "pch.h"
#include "halcon_anomalies_checkPatch.h"
#include "CDLPDlg.h"
#include "afxdialogex.h"
#include "checkPatch.h"
#include "dl_anomaly_detection_workflow.h"

// CDLPDlg 对话框

IMPLEMENT_DYNAMIC(CDLPDlg, CDialogEx)

CDLPDlg::CDLPDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	m_pCheck = NULL;
}

CDLPDlg::~CDLPDlg()
{
}

void CDLPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCPROPERTYGRID1, m_prop);
}


BEGIN_MESSAGE_MAP(CDLPDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDLPDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON5, &CDLPDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON8, &CDLPDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON7, &CDLPDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON6, &CDLPDlg::OnBnClickedButton6)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
	ON_BN_CLICKED(IDC_BUTTON9, &CDLPDlg::OnBnClickedButton9)
END_MESSAGE_MAP()


// CDLPDlg 消息处理程序


void CDLPDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	CDialogEx::OnOK();
}

bool CDLPDlg::iniHalWin()
{
	try
	{
		if (HDevWindowStack::IsOpen())
			HalconCpp::CloseWindow(HDevWindowStack::Pop());
		//HTuple hv_Width, hv_Height;
		//GetImageSize(m_ImageOrg, &hv_Width, &hv_Height);
		//HWND hWnd = GetDlgItem(IDC_STATIC_POSPIC)->GetSafeHwnd();//获取控件句柄
		//dev_open_window_fit_size(0, 0, hv_Width*1.4, hv_Height*1.4, -1, -1, (LONG)hWnd, &hv_WindowHandle);
		
		HWND hWnd = GetDlgItem(IDC_STATIC_POSPIC)->GetSafeHwnd();//获取控件句柄
		CRect rtWindow;// 定义一个矩形对象
		GetDlgItem(IDC_STATIC_POSPIC)->GetClientRect(&rtWindow);// 获取Picture Control控件的实际大小
		OpenWindow(rtWindow.left, rtWindow.top, rtWindow.Width(), rtWindow.Height(), (Hlong)hWnd, "visible", "", &hv_WindowHandle);//建立和Picture Control控件大小相同的窗口
		HDevWindowStack::Push(hv_WindowHandle);//将新建的窗口句柄推入队列中

		DispObj(m_ImageOrg, hv_WindowHandle);
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
	return true;
}


BOOL CDLPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	iniHalWin();

	propIni();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDLPDlg::OnBnClickedButton5()
{
	string file = ((checkPatch*)m_pCheck)->m_cfgDir+"\\";//windows要求"\\"
	replace(file.begin(), file.end(), '/', '\\');
	CFolderPickerDialog fd(file.c_str(), 0, this, 0);
	if (fd.DoModal() == IDOK)
	{
		string dir = fd.GetPathName().GetBuffer(0);
		CDateTime dt;
		dir += "/" + dt.ToString("%Y-%m-%d-%H-%M-%S");
		WriteImage(m_ImageOrg, "bmp", 0, dir.c_str());
	}
}


void CDLPDlg::OnBnClickedButton8()
{
	((checkPatch*)m_pCheck)->writeMode();
}


void CDLPDlg::OnBnClickedButton7()
{
	try {
		HImage ho_Mask = m_ImageOrg;
		((checkPatch*)m_pCheck)->predict(ho_Mask);
		FlushBuffer(hv_WindowHandle);
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}

void CDLPDlg::OnBnClickedButton6()
{
	BeginWaitCursor();
	try {
		((checkPatch*)m_pCheck)->Train();
		updateProp();
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
	EndWaitCursor();
}

void CDLPDlg::propIni()
{
//#ifdef BGCOLOR
//	m_prop.SetCustomColors(BGCOLOR, RGB(255, 255, 255), BGCOLOR, RGB(255, 255, 0), BGCOLOR, RGB(255, 255, 0), BGCOLOR);
//#endif
	m_prop.RemoveAll();
	CString str;
	HDITEM item;
	item.cxy = 220;
	item.mask = HDI_WIDTH;
	CFont pFont;
	pFont.CreatePointFont(180, _T("楷体"));
	m_prop.SetFont(&pFont);
	m_prop.GetHeaderCtrl().SetItem(0, new HDITEM(item));
	CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(_T("参数"));
	pGroup->SetData(1);
	str.Format("%d", ((checkPatch*)m_pCheck)->hv_ExampleSpecificPreprocessing.I());
	CMFCPropertyGridProperty* pItem = new CMFCPropertyGridProperty(_T("数据集预处理"), str);
	pItem->SetData(11);
	pItem->AddOption(_T("1"));
	pItem->AddOption(_T("0"));
	pItem->AllowEdit(FALSE);
	pGroup->AddSubItem(pItem);
	str.Format("%d", ((checkPatch*)m_pCheck)->hv_ImageWidth.I());//
	pItem = new CMFCPropertyGridProperty(_T("图像缩放宽"), str, _T("32的倍数"));
	pItem->SetData(12);
	pItem->AllowEdit(TRUE);
	pGroup->AddSubItem(pItem);
	str.Format("%d", ((checkPatch*)m_pCheck)->hv_ImageHeight.I());//
	pItem = new CMFCPropertyGridProperty(_T("图像缩放高"), str, _T("32的倍数"));
	pItem->SetData(13);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%d", ((checkPatch*)m_pCheck)->hv_Complexity.I());//
	pItem = new CMFCPropertyGridProperty(_T("复杂度"), str, _T("越大准确率越高，训练越耗时"));
	pItem->SetData(14);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%d", ((checkPatch*)m_pCheck)->hv_use_ok_samples.I());//
	pItem = new CMFCPropertyGridProperty(_T("训练样本选择"), str, _T("1仅使用ok样本训练,0使用okng两种样本训练"));
	pItem->SetData(15);
	pItem->AddOption(_T("1"));
	pItem->AddOption(_T("0"));
	pItem->AllowEdit(FALSE);//允许编辑
	pGroup->AddSubItem(pItem);
	m_prop.AddProperty(pGroup);

	pGroup = new CMFCPropertyGridProperty(_T("训练终止条件"));
	pGroup->SetData(2);
	str.Format("%f", ((checkPatch*)m_pCheck)->hv_ErrorThreshold.D());//
	pItem = new CMFCPropertyGridProperty(_T("错误率"), str);
	pItem->SetData(21);
	pItem->AllowEdit(FALSE);
	pGroup->AddSubItem(pItem);
	str.Format("%d", ((checkPatch*)m_pCheck)->hv_MaxNumEpochs.I());//
	pItem = new CMFCPropertyGridProperty(_T("次数"), str );
	pItem->SetData(22);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%f", ((checkPatch*)m_pCheck)->hv_DomainRatio.D());//
	pItem = new CMFCPropertyGridProperty(_T("训练比例"), str, _T("越大准确率越高，训练越耗时"));
	pItem->SetData(23);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%f", ((checkPatch*)m_pCheck)->hv_RegularizationNoise.D());//
	pItem = new CMFCPropertyGridProperty(_T("正则化噪声"), str, _T("为防止训练失败，可以设置大些"));
	pItem->SetData(24);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%f", ((checkPatch*)m_pCheck)->hv_StandardDeviationFactor.D());//
	pItem = new CMFCPropertyGridProperty(_T("标准差因子"), str, _T("如果缺陷很小，推荐较大值"));
	pItem->SetData(25);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	m_prop.AddProperty(pGroup);

	pGroup = new CMFCPropertyGridProperty(_T("训练结果"));
	pGroup->SetData(3);
	str.Format("%f", ((checkPatch*)m_pCheck)->hv_InferenceClassificationThreshold.D());//
	pItem = new CMFCPropertyGridProperty(_T("分类阈值"), str);
	pItem->SetData(31);
	pItem->AllowEdit(TRUE);
	pGroup->AddSubItem(pItem);
	str.Format("%f", ((checkPatch*)m_pCheck)->hv_InferenceSegmentationThreshold.D());//
	pItem = new CMFCPropertyGridProperty(_T("异常值阈值"), str);
	pItem->SetData(32);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	m_prop.AddProperty(pGroup);

	m_prop.ExpandAll(FALSE);
}

void CDLPDlg::updateProp()
{
	if (!m_prop.IsWindowVisible())
	{
		return;
	}
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
			bool bedit = false;
			if (1 == subid)
			{
				switch (ssubid)
				{
				case 1:
				{
					bedit = true;
					str.Format("%d", ((checkPatch*)m_pCheck)->hv_ExampleSpecificPreprocessing.I());
					break;
				}
				case 2:
				{
					bedit = true;
					str.Format("%d", ((checkPatch*)m_pCheck)->hv_ImageWidth.I());//
					break;
				}
				case 3:
				{
					bedit = true;
					str.Format("%d", ((checkPatch*)m_pCheck)->hv_ImageHeight.I());//
					break;
				}
				case 4:
				{
					bedit = true;
					str.Format("%d", ((checkPatch*)m_pCheck)->hv_Complexity.I());//
					break;
				}
				case 5:
				{
					bedit = false;
					str.Format("%d", ((checkPatch*)m_pCheck)->hv_use_ok_samples.I());//
					break;
				}
				}
			}
			else if (2 == subid)
			{
				bedit = true;
				switch (ssubid)
				{
				case 1:
					str.Format("%f", ((checkPatch*)m_pCheck)->hv_ErrorThreshold.D());//
					break;
				case 2:
					str.Format("%d", ((checkPatch*)m_pCheck)->hv_MaxNumEpochs.I());//
					break;
				case 3:
					str.Format("%f", ((checkPatch*)m_pCheck)->hv_DomainRatio.D());//
					break;
				case 4:
					str.Format("%f", ((checkPatch*)m_pCheck)->hv_RegularizationNoise.D());//
					break;
				case 5:
					str.Format("%f", ((checkPatch*)m_pCheck)->hv_StandardDeviationFactor.D());//
					break;
				}
			}
			else if (3 == subid)
			{
				bedit = true;
				switch (ssubid)
				{
				case 1:
					str.Format("%f", ((checkPatch*)m_pCheck)->hv_InferenceClassificationThreshold.D());//
					break;
				case 2:
					str.Format("%f", ((checkPatch*)m_pCheck)->hv_InferenceSegmentationThreshold.D());//
					break;
				
				}
			}
			pSubProperty->SetValue(str);
			pSubProperty->Enable(bedit);
			//	pSubProperty->AllowEdit(bedit);//允许编辑
		}
	}
}

LRESULT CDLPDlg::OnPropertyChanged(WPARAM, LPARAM lParam)
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
	CString str;
	str = t.bstrVal;      //从COleVariant到CString  
	int subid = id / 10;
	int ssubid = id % 10;
	if (1 == subid)
	{
		switch (ssubid)
		{
		case 1:
			((checkPatch*)m_pCheck)->hv_ExampleSpecificPreprocessing = atoi(str.GetBuffer(0));//
			return 0;
		case 2:
			((checkPatch*)m_pCheck)->hv_ImageWidth = atoi(str.GetBuffer(0));//
			break;
		case 3:
			((checkPatch*)m_pCheck)->hv_ImageHeight = atoi(str.GetBuffer(0));//
			break;
		case 4:
			((checkPatch*)m_pCheck)->hv_Complexity = atoi(str.GetBuffer(0));//
			break;
		case 5:
			((checkPatch*)m_pCheck)->hv_use_ok_samples = atoi(str.GetBuffer(0));//
			break;
		}
	}
	else if (2 == subid)
	{
		switch (ssubid)
		{
		case 1:
			((checkPatch*)m_pCheck)->hv_ErrorThreshold = atof(str.GetBuffer(0));//
			break;
		case 2:
			((checkPatch*)m_pCheck)->hv_MaxNumEpochs = atoi(str.GetBuffer(0));//
			break;
		case 3:
			((checkPatch*)m_pCheck)->hv_DomainRatio = atof(str.GetBuffer(0));//
			break;
		case 4:
			((checkPatch*)m_pCheck)->hv_RegularizationNoise = atof(str.GetBuffer(0));// 
			break;
		case 5:
			((checkPatch*)m_pCheck)->hv_StandardDeviationFactor = atof(str.GetBuffer(0));//
			break;
		}
	}
	else if (3 == subid)
	{
		switch (ssubid)
		{
		case 1:
			((checkPatch*)m_pCheck)->hv_InferenceClassificationThreshold = atof(str.GetBuffer(0));//
			break;
		case 2:
			((checkPatch*)m_pCheck)->hv_InferenceSegmentationThreshold = atof(str.GetBuffer(0));//
			break;
		}
	}

	return 0;
}

void CDLPDlg::OnBnClickedButton9()
{
	CFileDialog fdlg1(true, NULL, getExePath().c_str(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "open file (*.hdl)|*.hdl||");

	if (fdlg1.DoModal() != IDOK)
	{
		return;
	}
	else
	{
		CString str = fdlg1.GetPathName();
		CopyFile(str.GetBuffer(0), ((checkPatch*)m_pCheck)->m_pa.modelFile.c_str());
	}
}
