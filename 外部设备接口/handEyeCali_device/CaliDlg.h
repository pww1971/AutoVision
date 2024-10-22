#pragma once

#include "EditableListCtrl.h"
#include "checkPatch.h"
#include "resource.h"
// CTcpIpDlg �Ի���
#define CaliVIEW "cali"//�����޸���ͼ���� ��ֹ����dll����һ����ͼ

class CCaliDlg : public CDialog
{
	DECLARE_DYNAMIC(CCaliDlg)

public:
	CCaliDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCaliDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_Cali };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
#ifdef USE_HALCON
	//ͼ��ؼ��������
	void moveShift();//�ƶ�����
	void Zoomfit();//�ָ�����
	void scaleState(bool s );//���Ŵ���
	void ControlFunction(HTuple WindowHandleOrg,HTuple ZoomTrans,HTuple Row,HTuple Col,HTuple RowShif,HTuple ColShif);
	HTuple   m_WindowHandleOrg;		//����
	HImage m_ImageOrg;				//ԭʼͼ��
	HTuple m_WidthOrg,m_HeightOrg;	

	long	m_IDCShowImageW;		//�ؼ���͸�
	long	m_IDCShowImageH;

	float m_Zoom;					//���ż�¼
	float m_ZoomOrg;

	HTuple m_RowShifOrg;			//ƽ�Ƽ�¼
	HTuple m_ColShifOrg;
#else
	cv::Mat m_viewPic;
#endif
public:
	BOOL OnInitDialog();

	void viewClientList();//������ʾ��list��
	void listToParam();//list�������
	CEditableListCtrl m_ppList;

	afx_msg void OnBnClickedBtAdd();
	afx_msg void OnBnClickedBtCali();
	afx_msg void OnBnClickedBtCali2();
	void* m_pData;

private:
	CStatic m_myPic;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtRoi();
	afx_msg void OnBnClickedBtRoi2();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtDel();
};
