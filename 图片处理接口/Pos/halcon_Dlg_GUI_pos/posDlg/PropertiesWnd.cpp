// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���  
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�  
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ������� 
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace HalconCpp;
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd()
{
	m_nComboHeight = 0;
	CurOption = nullptr;
}

CPropertiesWnd::~CPropertiesWnd()
{
	m_wndPropList.DestroyWindow();
	DestroyWindow();
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_CBN_SELCHANGE(ID_PROP_COMBOX, OnPropCommboxSelChange)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
	//ON_COMMAND(ID_VIEW_PROPERTIESWND, &CPropertiesWnd::OnViewPropertieswnd)
	//ON_UPDATE_COMMAND_UI(ID_VIEW_PROPERTIESWND, &CPropertiesWnd::OnUpdateViewPropertieswnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar ��Ϣ�������

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd () == NULL || (AfxGetMainWnd() != NULL && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), m_nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + m_nComboHeight, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + m_nComboHeight + cyTlb, rectClient.Width(), rectClient.Height() -(m_nComboHeight+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// �������: 
	//const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, ID_PROP_COMBOX))
	{
		TRACE0(CST_MeasureWord41);
		return -1;      // δ�ܴ���
	}
	
	m_wndObjectCombo.AddString(_T(CST_MeasureWord42));
	//m_wndObjectCombo.AddString(_T("�������"));	
	m_wndObjectCombo.AddString(_T(CST_MeasureWord43));

	m_wndObjectCombo.SetCurSel(0);

	CRect rectCombo;
	m_wndObjectCombo.GetClientRect (&rectCombo);

	m_nComboHeight = rectCombo.Height();

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0(CST_MeasureWord44);
		return -1;      // δ�ܴ���
	}

	SetPropListFont();                             // ����һ������

	m_wndPropList.EnableHeaderCtrl(FALSE);		   // ���ر�ͷ
	m_wndPropList.EnableDescriptionArea();		   // ����˵����
	m_wndPropList.SetVSDotNetLook();               // ���
	m_wndPropList.MarkModifiedProperties();        // �Ӵָ��ĵ���

	// InitPropList(); �����MS������

	// ���Ա�����
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* ������*/);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap( IDB_PROPERTIES_HC , 0, 0, TRUE /* ����*/);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// �������ͨ���˿ؼ�·�ɣ�������ͨ�������·��: 
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}



void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

BOOL CPropertiesWnd::CanBeClosed() const
{
	return FALSE;
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}





//void CPropertiesWnd::InitPropListold()
//{
//	SetPropListFont();
//
//	m_wndPropList.EnableHeaderCtrl(FALSE);
//	m_wndPropList.EnableDescriptionArea();
//	m_wndPropList.SetVSDotNetLook();
//	m_wndPropList.MarkModifiedProperties();
//
//	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("���"));
//
//	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("��ά���"), (_variant_t) false, _T("ָ�����ڵ����岻ʹ�ô��壬���ҿؼ���ʹ����ά�߿�")));
//
//	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("�߿�"), _T("�Ի������"), _T("����֮һ: ���ޡ�����ϸ�������ɵ�����С���򡰶Ի������"));
//	pProp->AddOption(_T("��"));
//	pProp->AddOption(_T("ϸ"));
//	pProp->AddOption(_T("�ɵ�����С"));
//	pProp->AddOption(_T("�Ի������"));
//	pProp->AllowEdit(FALSE);
//
//	pGroup1->AddSubItem(pProp);
//	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("����"), (_variant_t) _T("����"), _T("ָ�����ڱ���������ʾ���ı�")));
//
//	m_wndPropList.AddProperty(pGroup1);
//
//	CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(_T("���ڴ�С"), 0, TRUE);
//
//	pProp = new CMFCPropertyGridProperty(_T("�߶�"), (_variant_t) 250l, _T("ָ�����ڵĸ߶�"));
//	pProp->EnableSpinControl(TRUE, 50, 300);
//	pSize->AddSubItem(pProp);
//
//	pProp = new CMFCPropertyGridProperty( _T("���"), (_variant_t) 150l, _T("ָ�����ڵĿ��"));
//	pProp->EnableSpinControl(TRUE, 50, 200);
//	pSize->AddSubItem(pProp);
//
//	m_wndPropList.AddProperty(pSize);
//
//	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("����"));
//
//	LOGFONT lf;
//	CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
//	font->GetLogFont(&lf);
//
//	_tcscpy_s(lf.lfFaceName, _T("����, Arial"));
//
//	pGroup2->AddSubItem(new CMFCPropertyGridFontProperty(_T("����"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("ָ�����ڵ�Ĭ������")));
//	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("ʹ��ϵͳ����"), (_variant_t) true, _T("ָ������ʹ�á�MS Shell Dlg������")));
//
//	m_wndPropList.AddProperty(pGroup2);
//
//	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("����"));
//	pProp = new CMFCPropertyGridProperty(_T("(����)"), _T("Ӧ�ó���"));
//	pProp->Enable(FALSE);
//	pGroup3->AddSubItem(pProp);
//
//	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("������ɫ"), RGB(210, 192, 254), NULL, _T("ָ��Ĭ�ϵĴ�����ɫ"));
//	pColorProp->EnableOtherButton(_T("����..."));
//	pColorProp->EnableAutomaticButton(_T("Ĭ��"), ::GetSysColor(COLOR_3DFACE));
//	pGroup3->AddSubItem(pColorProp);
//
//	static const TCHAR szFilter[] = _T("ͼ���ļ�(*.ico)|*.ico|�����ļ�(*.*)|*.*||");
//	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("ͼ��"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("ָ������ͼ��")));
//
//	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("�ļ���"), _T("c:\\")));
//
//	m_wndPropList.AddProperty(pGroup3);
//
//	CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("��νṹ"));
//
//	CMFCPropertyGridProperty* pGroup41 = new CMFCPropertyGridProperty(_T("��һ���Ӽ�"));
//	pGroup4->AddSubItem(pGroup41);
//
//	CMFCPropertyGridProperty* pGroup411 = new CMFCPropertyGridProperty(_T("�ڶ����Ӽ�"));
//	pGroup41->AddSubItem(pGroup411);
//
//	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("�� 1"), (_variant_t) _T("ֵ 1"), _T("��Ϊ˵��")));
//	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("�� 2"), (_variant_t) _T("ֵ 2"), _T("��Ϊ˵��")));
//	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("�� 3"), (_variant_t) _T("ֵ 3"), _T("��Ϊ˵��")));
//
//	pGroup4->Expand(FALSE);
//	m_wndPropList.AddProperty(pGroup4);
//}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::OnPropCommboxSelChange()
{
	//int num = this->m_wndObjectCombo.GetCurSel();
	this->InitPropList();
}

void CPropertiesWnd::InitPropList()
{
	int num = this->m_wndObjectCombo.GetCurSel();
	
	this->m_wndPropList.RemoveAll();               // �Ƴ�ԭ������������
	switch (num) {
	case PROPLIST_OTHER:
		InitPropList_CurProp();                   // ��ʾһ�����Ա�
		break;
	case PROPLIST_GLOBAL:
		InitPropList_Common();                    // ��ʾȫ�ֱ���
		break;
	default:
		break;
	}
	this->m_wndPropList.Invalidate();    // ����ˢ��һ��
}

void CPropertiesWnd::SetCurList(ePROPLIST e)
{
	this->m_wndObjectCombo.SetCurSel(e);
}


// ȫ�ֱ����������Ա�
void CPropertiesWnd::InitPropList_Common() {

	
	if (CurOption == NULL) return;
	NodeCAMProperty* pn = new NodeCAMProperty(UCI::OPTIONS, PROPRETY_GLOBAL);
	m_wndPropList.AddProperty(pn);
}

void CPropertiesWnd::InitPropList_CurProp()
{
	if (CurOption == NULL) return;
	NodeCAMProperty* pn = new NodeCAMProperty(*CurOption, PROPRETY_ELEMENT);
	m_wndPropList.AddProperty(pn);
}

//// ��������б�
//void CPropertiesWnd::InitPropList_Cammer()
//{
//	CMFCPropertyGridProperty* pProp;
//
//	////////////////////////////////////////////////////////////////////////////////////////
//
//	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("���������ѡ��"));
//	// ȡ�õ�ǰ��������
//	int camID = UCI::OPTIONS["CAMCurGroupID"];          // ��ǰ�������
//	UCI::OptionsMap om = UCI::CamOptions[camID-1];
//	std::string sCamGroupName = om["CamGroupName"];
//
//	pProp = new CMFCPropertyGridProperty(_T("��ǰ���������"), CA2T(sCamGroupName.c_str()).m_psz,
//		_T("��ѡ��һ�����������"), CMAERA_CAM_GROUP);
//
//	for (int i = 0; i < MAX_CAM_PARA_GROUP; i++) {
//		om = UCI::CamOptions[i];
//		std::string sCamGroupName = om["CamGroupName"];
//		pProp->AddOption(CA2T(sCamGroupName.c_str()).m_psz);
//	}
//
//	pProp->AllowEdit(FALSE);
//	pGroup1->AddSubItem(pProp);
//
//	m_wndPropList.AddProperty(pGroup1);
//
//	////////////////////////////////////////////////////////////////////////////////////////
//	/// ���в��������б�
//
//	om = UCI::CamOptions[camID - 1];  // ��ǰ�������
//	NodeCAMProperty* pn = new NodeCAMProperty(om, PROPRETY_CAMMRA);
//	m_wndPropList.AddProperty(pn);
//
//}

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
	m_wndObjectCombo.SetFont(&m_fntPropList);
}





BOOL myPropertyGridCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_MOUSEMOVE)
		return TRUE;

	return CMFCPropertyGridCtrl::PreTranslateMessage(pMsg);
}
