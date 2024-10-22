// NewMoveDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NewMoveDlg.h"
#include "new_move_device.h"
// CNewMoveDlg �Ի���

IMPLEMENT_DYNAMIC(CNewMoveDlg, CDialog)

CNewMoveDlg::CNewMoveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewMoveDlg::IDD, pParent)
{
	m_pData=NULL;
}

CNewMoveDlg::~CNewMoveDlg()
{
}

void CNewMoveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PIC, m_myPic);
}

BEGIN_MESSAGE_MAP(CNewMoveDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CNewMoveDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CNewMoveDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CNewMoveDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CNewMoveDlg::OnBnClickedButton5)
END_MESSAGE_MAP()

BOOL CNewMoveDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	cv::namedWindow(ArrVIEW, cv::WINDOW_NORMAL);
	HWND hWnd = (HWND)cvGetWindowHandle(ArrVIEW);
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, m_myPic.GetSafeHwnd());
	::ShowWindow(hParent, SW_HIDE);
	RECT rect;
	m_myPic.GetClientRect(&rect);
	cv::resizeWindow(ArrVIEW, rect.right, rect.bottom);
	m_viewPic = checkPatch::S_caliProc.m_curProc.m_rawPic[0];
	if (!m_viewPic.empty())
		imshow(ArrVIEW, m_viewPic);
	
	string procAddress="";
	string caliAddress = "";
	if (NULL!=m_pData)
	{
		procAddress=((checkPatch*)m_pData)->m_DevParam.m_procAddress;
		caliAddress = ((checkPatch*)m_pData)->m_caliDir;
	}
	GetDlgItem(IDC_EDIT1)->SetWindowTextA(procAddress.c_str());
	GetDlgItem(IDC_EDIT2)->SetWindowTextA(caliAddress.c_str());
	m_calidlg.m_pData = m_pData;
	m_calidlg.Create(IDD_DIALOG_Cali);
	m_calidlg.ShowWindow(0);
	
	return TRUE; 
}

// CNewMoveDlg ��Ϣ�������


void CNewMoveDlg::OnBnClickedButton2()
{
	m_calidlg.ShowWindow(1);	
	m_calidlg.viewClientList();
	m_calidlg.m_viewPic = checkPatch::S_caliProc.m_curProc.m_rawPic[0];
	if (!m_calidlg.m_viewPic.empty())
		imshow(CaliVIEW, m_calidlg.m_viewPic);
}

void CNewMoveDlg::OnBnClickedButton1()
{
	string localDir = getExePath();
	CFolderPickerDialog fd(localDir.c_str(), 0, this, 0);
	if (fd.DoModal() == IDOK)
	{
		string szdir = fd.GetPathName()+"/";
		((checkPatch*)m_pData)->loadProc(szdir);
	}
	GetDlgItem(IDC_EDIT1)->SetWindowTextA(((checkPatch*)m_pData)->m_DevParam.m_procAddress.c_str());
}


void CNewMoveDlg::OnBnClickedButton3()
{
	/*if (m_pData!=NULL)
	{
		CTotDll* pTotCheck = ((checkPatch*)m_pData)->m_CaliParam.pTotCheck;
	}*/
	vector<cv::Mat> vm;
	m_viewPic = checkPatch::S_caliProc.m_curProc.m_rawPic[0].clone();
	vm.push_back(m_viewPic);
	if(((checkPatch*)m_pData)->m_Proc.parallelMatchTplt(vm)==-1)//��λ���ɹ�
		return;
	vector<S_TpltData*> & vctData = ((checkPatch*)m_pData)->m_Proc.m_vcTplt[0]->m_vctData;
	for (int ik=0;ik< vctData.size();ik++)
	{
		float posX = vctData[ik]->m_roiRect.center.x;
		float posY = vctData[ik]->m_roiRect.center.y;
		float posZ = vctData[ik]->m_roiRect.angle;//��ת��
		cv::circle(m_viewPic,cv::Point2f(posX,posY),3,CV_RGB(255,0,0),2);
	}
	imshow(ArrVIEW, m_viewPic);
}


void CNewMoveDlg::OnBnClickedButton5()
{
	string localDir = getExePath();
	CFolderPickerDialog fd(localDir.c_str(), 0, this, 0);
	if (fd.DoModal() == IDOK)
	{
		CString str = fd.GetPathName() + "/";
		((checkPatch*)m_pData)->m_caliDir=str.GetBuffer(0);
		GetDlgItem(IDC_EDIT2)->SetWindowTextA(((checkPatch*)m_pData)->m_caliDir.c_str());
	}
}
