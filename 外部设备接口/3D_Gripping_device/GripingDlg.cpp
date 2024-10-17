// GripingDlg.cpp: 实现文件
//

#include "pch.h"
#include "3D_Gripping_device.h"
#include "GripingDlg.h"
#include "afxdialogex.h"
#include "device.h"

extern HWND g_halhWnd;

// GripingDlg 对话框

IMPLEMENT_DYNAMIC(GripingDlg, CDialogEx)

GripingDlg::GripingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
    g_halhWnd = 0;
}

GripingDlg::~GripingDlg()
{
}

void GripingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCPROPERTYGRID1, m_prop);
}


BEGIN_MESSAGE_MAP(GripingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON9, &GripingDlg::OnBnClickedButton9)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()

// GripingDlg 消息处理程序

void GripingDlg::OnBnClickedButton9()
{
	if (!((CDevice*)m_pCheck)->m_runing)
		((CDevice*)m_pCheck)->open();
}

BOOL GripingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	dev_update_off();
	if (HDevWindowStack::IsOpen())
		HalconCpp::CloseWindow(HDevWindowStack::Pop());
	g_halhWnd = GetDlgItem(IDC_STATIC_POSPIC)->GetSafeHwnd();//获取控件句柄
	CRect rtWindow;// 定义一个矩形对象
	GetDlgItem(IDC_STATIC_POSPIC)->GetClientRect(&rtWindow);// 获取Picture Control控件的实际大小
	OpenWindow(rtWindow.left, rtWindow.top, rtWindow.Width(), rtWindow.Height(), (Hlong)g_halhWnd, "visible", "", &((CDevice*)m_pCheck)->hv_WindowHandle);//建立和Picture Control控件大小相同的窗口
	HDevWindowStack::Push(((CDevice*)m_pCheck)->hv_WindowHandle);//将新建的窗口句柄推入队列中

	DispObj(((CDevice*)m_pCheck)->m_hImage[1], ((CDevice*)m_pCheck)->hv_WindowHandle);
	
	propIni();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void GripingDlg::propIni()
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
	str.Format("%d", ((CDevice*)m_pCheck)->m_DLPa.hv_SortResultsTiltedCamera);
	CMFCPropertyGridProperty* pItem = new CMFCPropertyGridProperty(_T("相机侧对着目标"), str);
	pItem->SetData(11);
	pItem->AddOption(_T("1"));
	pItem->AddOption(_T("0"));
	pItem->AllowEdit(FALSE);
	pGroup->AddSubItem(pItem);
	str.Format("%d", ((CDevice*)m_pCheck)->m_DLPa.image_width);//
	pItem = new CMFCPropertyGridProperty(_T("图像宽"), str);
	pItem->SetData(12);
	pItem->AllowEdit(TRUE);
	pGroup->AddSubItem(pItem);
	str.Format("%d", ((CDevice*)m_pCheck)->m_DLPa.image_height);//
	pItem = new CMFCPropertyGridProperty(_T("图像高"), str);
	pItem->SetData(13);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	str.Format("%d", ((CDevice*)m_pCheck)->m_DLPa.min_area_size);//
	pItem = new CMFCPropertyGridProperty(_T("最小区域面积"), str, _T("抓取点最小区域面积"));
	pItem->SetData(14);
	pItem->AllowEdit(TRUE);//允许编辑
	pGroup->AddSubItem(pItem);
	m_prop.AddProperty(pGroup);

	m_prop.ExpandAll(FALSE);
}

void GripingDlg::updateProp()
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
					str.Format("%d", ((CDevice*)m_pCheck)->m_DLPa.hv_SortResultsTiltedCamera);
					break;
				}
				case 2:
				{
					bedit = true;
					str.Format("%d", ((CDevice*)m_pCheck)->m_DLPa.image_width);//
					break;
				}
				case 3:
				{
					bedit = true;
					str.Format("%d", ((CDevice*)m_pCheck)->m_DLPa.image_height);//
					break;
				}
				case 4:
				{
					bedit = true;
					str.Format("%d", ((CDevice*)m_pCheck)->m_DLPa.min_area_size);//
					break;
				}
				
				}
			}
			pSubProperty->SetValue(str);
			pSubProperty->Enable(bedit);
			//	pSubProperty->AllowEdit(bedit);//允许编辑
		}
	}
}

LRESULT GripingDlg::OnPropertyChanged(WPARAM, LPARAM lParam)
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
			((CDevice*)m_pCheck)->m_DLPa.hv_SortResultsTiltedCamera = atoi(str.GetBuffer(0));//
			return 0;
		case 2:
			((CDevice*)m_pCheck)->m_DLPa.image_width = atoi(str.GetBuffer(0));//
			break;
		case 3:
			((CDevice*)m_pCheck)->m_DLPa.image_height = atoi(str.GetBuffer(0));//
			break;
		case 4:
			((CDevice*)m_pCheck)->m_DLPa.min_area_size = atoi(str.GetBuffer(0));//
			break;
		}
	}

	return 0;
}
