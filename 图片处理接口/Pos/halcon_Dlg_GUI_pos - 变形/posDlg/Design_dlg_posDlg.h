
// tot_dlg_posDlg.h : ͷ�ļ�
//

#pragma once
#include "Resource.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "MyDispLay.h"
#include "ProjectTree.h"
#include "PropertiesWnd.h"
using namespace GDKT;
// Ctot_dlg_posDlg �Ի���
class CDesign_dlg_posDlg : public CDialogEx
{
// ����
public:
	CDesign_dlg_posDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CDesign_dlg_posDlg()
	{
		delete UCI::pDis;
		UCI::pDis=NULL;
	}
// �Ի�������
	enum { IDD = IDD_Design_DLG_POS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


public:

	CProjectTree m_ProcTree;
	CImageList m_ProjectViewImages;//����ͼ���б�ؼ�
	CPropertiesWnd    m_wndProperties;            // ������ͼ

	CStatic m_viewStatic;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void FitImageToClient();
	void FillProjectTreeView();
	int UpdataProjectTreeByList();
	void OnChangeVisualStyle();
	int GetCurrentSelElementID();
	UCI::OptionsMap* AddOneElementToProjectTree(UCI::OptionsMap * o);
	void ProPertyUpdate() { m_wndProperties.InitPropList(); };                          // ��������ҳ
	void SetPettyCurSel(ePROPLIST e) { m_wndProperties.SetCurList(e); };
	void SetPropertyCurOptionsMap(UCI::OptionsMap* CurOption) {
		this->m_wndProperties.CurOption = CurOption;
	};
	UCI::OptionsMap* GetPropertyCurOptionsMap() {
		return this->m_wndProperties.CurOption;
	};
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedBtDel();
	afx_msg void OnBnClickedBtDelall();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedBtOpen();
	afx_msg void OnBnClickedBtSave();
	afx_msg void OnBnClickedBtPos();
	afx_msg void OnBnClickedBtInfo();

	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	void * m_pCheck;
	bool m_hasIni;
	float m_nOldCx;
	float m_nOldCy;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
