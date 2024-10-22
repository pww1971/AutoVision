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

#pragma once

#include "MyData.h"

//class UCI::Option;
//class UCI::OptionsMap;

//#define OUT_Element_Name_ID               999
//#define OUT_Element_Note_ID               998
//#define OUT_Element_Name_Identify         997
//#define USE_LINE1_Name_Identify           996
//#define USE_LINE2_Name_Identify           995
//
//#define USE_Distance1_Name_Identify       994
//#define USE_Distance2_Name_Identify       993
//
//#define "����Ԫ��1������"_ID    993
//#define USE_Distance2_Parament_Name_ID    991




class CPropertiesToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};
class myPropertyGridCtrl:public CMFCPropertyGridCtrl
{
public:
	myPropertyGridCtrl(){};
	~myPropertyGridCtrl(){};


	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
class CPropertiesWnd : public CDockablePane
{
// ����
public:
	CPropertiesWnd();

	void AdjustLayout();

// ����
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

protected:
	CFont m_fntPropList;
	CComboBox m_wndObjectCombo;
	CPropertiesToolBar m_wndToolBar;
	myPropertyGridCtrl m_wndPropList;

// ʵ��
public:
	virtual ~CPropertiesWnd();
	virtual BOOL CanBeClosed() const;
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);



	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg LRESULT OnPropertyChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPropCommboxSelChange();
	DECLARE_MESSAGE_MAP()


	//void InitPropListold();
	
	//void InitPropList_Cammer();    // ���������Э
	void InitPropList_Common();    // ȫ�ֱ�������
	void InitPropList_CurProp();   // ��ǰ������ʾ

	void SetPropListFont();

	int m_nComboHeight;

private:

	LRESULT OnPropertyChangedGlobal(WPARAM wParam, LPARAM lParam);
	//LRESULT OnPropertyChangedCamera(WPARAM wParam, LPARAM lParam);
	LRESULT OnPropertyChangedProperty(WPARAM wParam, LPARAM lParam);

public:

	void InitPropList();

	void SetCurList(ePROPLIST e);

	UCI::OptionsMap* CurOption;          // ��ǰ���Ա��Ԫ��

	//afx_msg void OnViewPropertieswnd();
	//afx_msg void OnUpdateViewPropertieswnd(CCmdUI *pCmdUI);
};


#define AFX_PROP_HAS_LIST		0x0001  
#define AFX_PROP_HAS_BUTTON		0x0002  
#define AFX_PROP_HAS_SPIN		0x0004 

// �����Լ�������
class NodeCAMProperty : public  CMFCPropertyGridProperty
{

	DECLARE_DYNAMIC(NodeCAMProperty)

public:

	// ��������ڵ� ePROPERY_TYPE
	// NodeCAMProperty(UCI::OptionsMap& om, CString groupName);

	NodeCAMProperty(UCI::OptionsMap& om, ePROPERY_TYPE pType);

	
private:
	void initGobleProperty(UCI::OptionsMap& om);
	//void initCameraProperty(UCI::OptionsMap& om);
	void initProtertyProperty(UCI::OptionsMap& om);

	void initProtertyCamera(UCI::OptionsMap& om);
	void initProtertyLine(UCI::OptionsMap& om);
	void initProtertyCircle(UCI::OptionsMap& om);
	void initProtertyPeek(UCI::OptionsMap& om);
	void initProtertyBalloonLength(UCI::OptionsMap& om);
	void initProtertyAngle(UCI::OptionsMap& om);
	void initProtertyDistance(UCI::OptionsMap& om);
	void initProtertyFUNCTION(UCI::OptionsMap& om);
	void initProtertyMATCH(UCI::OptionsMap& om);
	void initProtertyResult(UCI::OptionsMap& om);
	void initProtertyMM(UCI::OptionsMap& om);
	void initProtertyMEASURE_RECTANGLE2(UCI::OptionsMap& om);
	void initPropertyBalloonFlaw(UCI::OptionsMap& om);
	void initPropertyROI(UCI::OptionsMap& om);

	void insertCurrentPosCanUseElement(CMFCPropertyGridProperty* pProp, UCI::OptionsMap& om);   // ���뵱ǰԪ��ǰ������õ�Ԫ��


	//�ڵ�ID��,�ڵ�����
	NodeCAMProperty(int ID, double x, double y, double z);
	BOOL HasButton() const;
};
