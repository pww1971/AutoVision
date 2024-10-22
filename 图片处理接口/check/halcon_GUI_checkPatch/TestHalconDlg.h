
// TestHalconDlg.h : ͷ�ļ�
//

#pragma once
#include "halconLocal.h"

#include "Design_GUI_checkPatch.h"
#include "afxwin.h"
#include <string>
#include <fstream>
using namespace std;
#include "resource.h"	
// CTestHalconDlg �Ի���
class CTestHalconDlg : public CDialog
{
// ����
public:
	CTestHalconDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TESTHALCON_DIALOG };

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
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	bool m_dlgSucc;//�����ʼ���ɹ�
	void* m_pCheck;//check����

	//ͼ��ؼ��������
	void moveShift();//�ƶ�����
	void Zoomfit();//�ָ�����
	void scaleState(bool s );//���Ŵ���
	void ControlFunction(HTuple WindowHandleOrg,HTuple ZoomTrans,HTuple Row,HTuple Col,HTuple RowShif,HTuple ColShif);
	HTuple   m_WindowHandleOrg;		//����
	HObject m_ImageOrg;				//ԭʼͼ��
	HTuple m_WidthOrg,m_HeightOrg;

	long	m_IDCShowImageW;		//�ؼ���͸�
	long	m_IDCShowImageH;

	float m_Zoom;					//���ż�¼
	float m_ZoomOrg;

	HTuple m_RowShifOrg;			//ƽ�Ƽ�¼
	HTuple m_ColShifOrg;

//�Զ����㷨����
public:


	void guiIni();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnCbnSelchangeCombo3();
};
