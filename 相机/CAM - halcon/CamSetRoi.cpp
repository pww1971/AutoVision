// CamSetRoi.cpp : 实现文件
//

#include "stdafx.h"
#include "CAM.h"
#include "CamSetRoi.h"
#include "afxdialogex.h"
#include "CameraDrv.h"
mouseKeyCtr g_MKC;
bool iniMKC()
{
	g_MKC.setMainWin(dllViewMain);
	return true;
}
bool a = iniMKC();
void dllViewMain(cv::Mat& m,void* arg)
{
	if (!m.empty())
	{
		cv::namedWindow(DLLVIEW, cv::WINDOW_NORMAL);
		cv::imshow(DLLVIEW, m);
		cv::waitKey(1);
	}
}

void onMouse(int event, int x, int y, int flag, void*p)
{
	if (event == cv::EVENT_LBUTTONDOWN || event == cv::EVENT_LBUTTONUP || event == cv::EVENT_MOUSEMOVE)
	{
		g_MKC.onMouse( event,  x,  y,  flag, p);
	}
}
// CamSetRoi 对话框

IMPLEMENT_DYNAMIC(CamSetRoi, CDialogEx)

CamSetRoi::CamSetRoi(CWnd* pParent /*=NULL*/)
	: CDialogEx(CamSetRoi::IDD, pParent)
{
	m_pCheck = NULL;
	m_dlgSucc=false;	
}

CamSetRoi::~CamSetRoi()
{
}

void CamSetRoi::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PIC, m_myPic);
	DDX_Control(pDX, IDC_LIST1, m_camList);
}

BEGIN_MESSAGE_MAP(CamSetRoi, CDialogEx)
	ON_BN_CLICKED(IDC_BT_RESET, &CamSetRoi::OnBnClickedBtReset)
	ON_BN_CLICKED(IDC_BT_SETROI, &CamSetRoi::OnBnClickedBtSetroi)
	ON_BN_CLICKED(IDC_BUTTON2, &CamSetRoi::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CamSetRoi::OnBnClickedButton4)
	ON_CBN_SELCHANGE(IDC_COMBO_CAMID, &CamSetRoi::OnCbnSelchangeComboCamid)
	ON_BN_CLICKED(IDC_BT_INIALL, &CamSetRoi::OnBnClickedBtIniall)
END_MESSAGE_MAP()


// CamSetRoi 消息处理程序
BOOL CamSetRoi::OnInitDialog()
{
	CDialog::OnInitDialog();
	vector<cv::Mat> vm;
	((CameraDrv*)m_pCheck)->grapImage(0,vm);
	g_MKC.updateFrame(vm);
	// TODO:  Add extra initialization here
	cv::namedWindow(DLLVIEW, cv::WINDOW_NORMAL);
	HWND hWnd = (HWND)cvGetWindowHandle(DLLVIEW);
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, m_myPic.GetSafeHwnd());
	::ShowWindow(hParent, SW_HIDE);
	RECT rect;
	m_myPic.GetClientRect(&rect);
	cv::resizeWindow(DLLVIEW, rect.right, rect.bottom);
	if (!g_MKC.m_curProc.m_rawPic[0].empty())
		imshow(DLLVIEW, g_MKC.m_curProc.m_rawPic[0]);
	cv::setMouseCallback(DLLVIEW, onMouse);

	m_camList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_camList.InsertColumn(0, "相机id", LVCFMT_CENTER,50);
	updateList();

	((CComboBox*)GetDlgItem(IDC_COMBO_CAMID))->SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CamSetRoi::OnBnClickedBtReset()
{
#if (defined _OPENCV_CAM) || (defined _OPENCV_CAM_1) || (defined _OPENCV_CAM_2) 
	int id = ((CComboBox*)GetDlgItem(IDC_COMBO_CAMID))->GetCurSel();
	CHKCamera* cam = ((CameraDrv*)m_pCheck)->m_hkCam.getHK(id);
	if (NULL==cam)
		return;
	cv::Rect rt;
	if(cam->getRawSize(rt))
	{
		cam->setRoi(rt);
	}
	if(cam->getRawSize(rt))
	{
		cam->setRoi(rt);
		cam->iniGrabbing();
		OnCbnSelchangeComboCamid();
		OnBnClickedButton4();
	}
#endif
}

void CamSetRoi::OnBnClickedBtSetroi()
{
#if (defined _OPENCV_CAM) || (defined _OPENCV_CAM_1) || (defined _OPENCV_CAM_2) 
	cv::Rect rt = g_MKC.getSelObj().boundingRect();
	if(0 != rt.width)
	{
		int id = ((CComboBox*)GetDlgItem(IDC_COMBO_CAMID))->GetCurSel();
		CHKCamera* cam = ((CameraDrv*)m_pCheck)->m_hkCam.getHK(id);
		if (NULL==cam)
			return;
		cv::Rect rct;
		cv::Size sz;
		if(cam->getRoi(rct,sz))
		{
			if (sz.width > rt.width || sz.height > rt.height)
			{
				MessageBox("ROI太小");
				return;
			}
			rt.x += rct.x;
			rt.y += rct.y;
			if(cam->setRoi(rt)==0)
			{
				CConfig cfg(getExePath()+PROCCFG);
				string szVal = cfg.getItem("CurDir");
				if(szVal != "")
				{
					szVal = szVal+"\\cam\\";
					pww_mkdir(szVal.c_str());
					szVal = szVal+CStrProcess::itoa_radixN(id);
					pww_mkdir(szVal.c_str());
					szVal = szVal+"\\hk.txt";
					CConfig cfg2(szVal);
					char str[128];
					sprintf(str,"%d,%d,%d,%d",rt.x,rt.y,rt.width,rt.height);
					cfg2.updateItem("ROIRECT",str);
					updateList();
				}
			}
			cam->iniGrabbing();
			OnCbnSelchangeComboCamid();
		}
	}
#endif
	
}

void CamSetRoi::OnBnClickedButton2()
{
	g_MKC.Ctrl_COrV(9);
	g_MKC.m_bSelectState=true;
	g_MKC.m_maskPa.shape = e_RotatedRect;//e_rectangle;
}

void CamSetRoi::OnBnClickedButton4()
{
	CConfig cfg(getExePath()+PROCCFG);
	string szVal = cfg.getItem("CurDir");
	if(szVal != "")
	{
		int id = m_camList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
		if (id > -1)
		{
		}
		else
		{
			id = m_camList.GetItemCount()-1;
		}		
		string szid = m_camList.GetItemText(id,0).GetBuffer(0);
		szVal = szVal+"\\cam\\"+szid+"\\hk.txt";
		remove(szVal.c_str());
		updateList();
	}
}

void CamSetRoi::OnCbnSelchangeComboCamid()
{
	int id = ((CComboBox*)GetDlgItem(IDC_COMBO_CAMID))->GetCurSel();
	if (id > -1)
	{
		vector<cv::Mat> vm;
		((CameraDrv*)m_pCheck)->grapImage(id,vm);
		g_MKC.updateFrame(vm);
	}
}

void CamSetRoi::updateList()
{
	m_camList.DeleteAllItems();
	CConfig cfg(getExePath()+PROCCFG);
	string szVal = cfg.getItem("CurDir");
	if(szVal != "")
	{
		szVal = szVal+"\\cam\\";
		vector<string> vctDir, vctFile;
		getFileNameDir(szVal.c_str(),vctDir);

		int k=0;
		for(int i=0;i<vctDir.size();i++)
		{
			vctFile.clear();
			getFileNameVec((szVal+vctDir[i]).c_str(),vctFile);
			if(vctFile.size()>0)
			{
				m_camList.InsertItem(k++, vctDir[i].c_str(),0);
			}
		}
	}
}

void CamSetRoi::OnBnClickedBtIniall()
{
#if (defined _OPENCV_CAM) || (defined _OPENCV_CAM_1) || (defined _OPENCV_CAM_2) 
	for(int i=0;i<10;i++)
	{
		CHKCamera* cam = ((CameraDrv*)m_pCheck)->m_hkCam.getHK(i);
		if (NULL==cam)
			continue;
		cv::Rect rt;
	if(cam->getRawSize(rt))
	{
		cam->setRoi(rt);
	}
	if(cam->getRawSize(rt))
	{
		cam->setRoi(rt);
	}
		cam->setRoi();
		cam->iniGrabbing();
	}
#endif
}
