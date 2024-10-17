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
void dllViewMain(cv::Mat& m, void* arg)
{
	if (!m.empty())
	{
		cv::namedWindow(DLLVIEW, cv::WINDOW_NORMAL);
		cv::imshow(DLLVIEW, m);
		cv::waitKey(1);
	}
}

void onMouse(int event, int x, int y, int flag, void* p)
{
	if (event == cv::EVENT_LBUTTONDOWN || event == cv::EVENT_LBUTTONUP || event == cv::EVENT_MOUSEMOVE)
	{
		g_MKC.onMouse(event, x, y, flag, p);
	}
}
// CamSetRoi 对话框

IMPLEMENT_DYNAMIC(CamSetRoi, CDialogEx)

CamSetRoi::CamSetRoi(CWnd* pParent /*=NULL*/)
	: CDialogEx(CamSetRoi::IDD, pParent)
{
	m_pCheck = NULL;
	m_dlgSucc = false;
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
	ON_BN_CLICKED(IDC_BT_SETROI, &CamSetRoi::OnBnClickedBtSetroi)
	ON_BN_CLICKED(IDC_BUTTON2, &CamSetRoi::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CamSetRoi::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BT_INIALL, &CamSetRoi::OnBnClickedBtIniall)
	ON_BN_CLICKED(IDC_BUTTON3, &CamSetRoi::OnBnClickedButton3)
END_MESSAGE_MAP()


// CamSetRoi 消息处理程序
BOOL CamSetRoi::OnInitDialog()
{
	CDialog::OnInitDialog();
	vector<cv::Mat> vm;
	if (((CameraDrv*)m_pCheck)->grapImage(0, vm) > -1)
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
	m_camList.InsertColumn(0, "相机id", LVCFMT_CENTER, 50);
	updateList();

	((CComboBox*)GetDlgItem(IDC_COMBO_CAMID))->SetCurSel(0);
	GetDlgItem(IDC_EDIT1)->SetWindowText("0");
	GetDlgItem(IDC_EDIT2)->SetWindowText("2000");

	OnBnClickedButton3();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CamSetRoi::OnBnClickedBtSetroi()
{
	int id = ((CComboBox*)GetDlgItem(IDC_COMBO_CAMID))->GetCurSel();
	if (id == -1)
		return;
	CConfig cfg(getExePath() + PROCCFG);
	string szVal = cfg.getItem("CurDir");
	if (szVal != "")
	{
		szVal = szVal + "\\cam\\";
		pww_mkdir(szVal.c_str());
		szVal = szVal + CStrProcess::itoa_radixN(id);
		pww_mkdir(szVal.c_str());
		szVal = szVal + "\\LSLIDAR.txt";
		CConfig cfg2(szVal);
		char str[128];
		cv::Rect rt = g_MKC.getSelObj().boundingRect();
		if (0 != rt.width)
		{
			sprintf(str, "%d,%d,%d,%d", rt.x, rt.y, rt.width, rt.height);
			cfg2.updateItem("ROIRECT", str);
		}
		CString cstr;
		GetDlgItem(IDC_EDIT1)->GetWindowText(cstr);
		((CameraDrv*)m_pCheck)->angleL = atoi(cstr.GetBuffer(0));
		GetDlgItem(IDC_EDIT2)->GetWindowText(cstr);
		((CameraDrv*)m_pCheck)->angleH = atoi(cstr.GetBuffer(0));
		CIPAddressCtrl* pIP = (CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS1);
		BYTE nf1, nf2, nf3, nf4;
		pIP->GetAddress(nf1, nf2, nf3, nf4);
		sprintf(str, "%d,%d,%d,%d,%d,%d", nf1, nf2, nf3, nf4, ((CameraDrv*)m_pCheck)->angleL, ((CameraDrv*)m_pCheck)->angleH);//这里的nf得到的值是IP值了
		cfg2.updateItem("IPADD", str);

		sprintf(str, "%d,%d,%d,%d,%d,%d", nf1, nf2, nf3, nf4);
		((CameraDrv*)m_pCheck)->computer_ip = str;

		GetDlgItem(IDC_EDIT3)->GetWindowText(cstr);
		((CameraDrv*)m_pCheck)->gridX = atoi(cstr.GetBuffer(0));
		cfg2.updateItem("gridX", cstr.GetBuffer(0));
		GetDlgItem(IDC_EDIT4)->GetWindowText(cstr);
		((CameraDrv*)m_pCheck)->gridSizeX = atof(cstr.GetBuffer(0));
		cfg2.updateItem("gridSizeX", cstr.GetBuffer(0));
		sprintf(str, "%f", ((CameraDrv*)m_pCheck)->minPoint);
		cfg2.updateItem("minPoint", str);
		
	}
}

void CamSetRoi::OnBnClickedButton2()
{
	g_MKC.Ctrl_COrV(9);
	g_MKC.m_bSelectState = true;
	g_MKC.m_maskPa.shape = e_RotatedRect;//e_rectangle;
}

void CamSetRoi::OnBnClickedButton4()
{
	CConfig cfg(getExePath() + PROCCFG);
	string szVal = cfg.getItem("CurDir");
	if (szVal != "")
	{
		int id = m_camList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
		if (id > -1)
		{
		}
		else
		{
			id = m_camList.GetItemCount() - 1;
		}
		string szid = m_camList.GetItemText(id, 0).GetBuffer(0);
		szVal = szVal + "\\cam\\" + szid + "\\LSLIDAR.txt";
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
		((CameraDrv*)m_pCheck)->grapImage(id, vm);
		g_MKC.updateFrame(vm);
	}
}

void CamSetRoi::updateList()
{
	m_camList.DeleteAllItems();
	CConfig cfg(getExePath() + PROCCFG);
	string szVal = cfg.getItem("CurDir");
	if (szVal != "")
	{
		szVal = szVal + "\\cam\\";
		vector<string> vctDir, vctFile;
		getFileNameDir(szVal.c_str(), vctDir);

		int k = 0;
		for (int i = 0; i < vctDir.size(); i++)
		{
			vctFile.clear();
			getFileNameVec((szVal + vctDir[i]).c_str(), vctFile);
			if (vctFile.size() > 0)
			{
				m_camList.InsertItem(k++, vctDir[i].c_str(), 0);
			}
		}
	}
}

void CamSetRoi::OnBnClickedBtIniall()
{
	CString str;
	GetDlgItem(IDC_EDIT1)->GetWindowText(str);
	((CameraDrv*)m_pCheck)->angleL = atoi(str.GetBuffer(0));
	GetDlgItem(IDC_EDIT2)->GetWindowText(str);
	((CameraDrv*)m_pCheck)->angleH = atoi(str.GetBuffer(0));
	CIPAddressCtrl* pIP = (CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS1);
	BYTE nf1, nf2, nf3, nf4;
	pIP->GetAddress(nf1, nf2, nf3, nf4);
	str.Format("%d.%d.%d.%d", nf1, nf2, nf3, nf4);//这里的nf得到的值是IP值了
	//	MessageBox(str);
	((CameraDrv*)m_pCheck)->iniCam(str.GetBuffer(0));
	GetDlgItem(IDC_EDIT4)->GetWindowText(str);
	((CameraDrv*)m_pCheck)->gridX = atoi(str.GetBuffer(0));//栅格数
	vector<cv::Mat> vm;
	if (((CameraDrv*)m_pCheck)->grapImage(0, vm) > -1)
	{
		g_MKC.updateFrame(vm);
		//深度图里计算栅格大小
		vector<Mat> channels;
		split(vm[1], channels);//分割通道

		double  maxPoint;
		cv::minMaxLoc(channels[0], &((CameraDrv*)m_pCheck)->minPoint, &maxPoint);
		((CameraDrv*)m_pCheck)->gridSizeX = 
			(maxPoint- ((CameraDrv*)m_pCheck)->minPoint)/ ((CameraDrv*)m_pCheck)->gridX;//栅格大小
		str.Format("%f", ((CameraDrv*)m_pCheck)->gridSizeX);
		GetDlgItem(IDC_EDIT3)->SetWindowText(str);
	}
}


void CamSetRoi::OnBnClickedButton3()
{
	int id = ((CComboBox*)GetDlgItem(IDC_COMBO_CAMID))->GetCurSel();
	if (id == -1)
	{
		CIPAddressCtrl* pIP = (CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS1);
		pIP->SetAddress(192, 168, 1, 102);
		return;
	}
	cv::Rect rct(0, 0, 0, 0);
	CConfig cfg(getExePath() + PROCCFG);
	string szVal = cfg.getItem("CurDir");
	if (szVal != "")
	{
		szVal = szVal + "\\cam\\" + CStrProcess::itoa_radixN(id) + "\\LSLIDAR.txt";
		CConfig cfg2(szVal);
		szVal = cfg2.getItem("ROIRECT");
		if (szVal != "")
		{
			vector<string> vct;
			CStrProcess::Split(szVal, ",", vct, true);
			if (vct.size() == 4)
			{
				rct.x = atoi(vct[0].c_str());
				rct.y = atoi(vct[1].c_str());
				rct.width = atoi(vct[2].c_str());
				rct.height = atoi(vct[3].c_str());
				if (rct.width > 0)
				{
					//显示图片
				}
			}
		}
		szVal = cfg2.getItem("IPADD");
		if (szVal != "")
		{
			vector<string> vct;
			CStrProcess::Split(szVal, ",", vct, true);
			if (vct.size() < 6)
				return;
			CIPAddressCtrl* pIP = (CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS1);
			pIP->SetAddress(atoi(vct[0].c_str()), atoi(vct[1].c_str()),
				atoi(vct[2].c_str()), atoi(vct[3].c_str()));
			((CameraDrv*)m_pCheck)->computer_ip = vct[0] +"."+ vct[1] + "." + vct[2] + "." + vct[3];
			GetDlgItem(IDC_EDIT1)->SetWindowText(vct[4].c_str());
			GetDlgItem(IDC_EDIT2)->SetWindowText(vct[5].c_str());
			((CameraDrv*)m_pCheck)->angleL = atoi(vct[4].c_str());
			((CameraDrv*)m_pCheck)->angleH = atoi(vct[5].c_str());
		}
		szVal = cfg2.getItem("gridX");
		if (szVal != "")
		{
			GetDlgItem(IDC_EDIT3)->SetWindowText(szVal.c_str());
			((CameraDrv*)m_pCheck)->gridX = atoi(szVal.c_str());
		}
		szVal = cfg2.getItem("gridSizeX");
		if (szVal != "")
		{
			GetDlgItem(IDC_EDIT4)->SetWindowText(szVal.c_str());
			((CameraDrv*)m_pCheck)->gridSizeX = atoi(szVal.c_str());
		}
		szVal = cfg2.getItem("minPoint");
		if (szVal != "")
		{
			((CameraDrv*)m_pCheck)->minPoint = atof(szVal.c_str());
		}
	}
	
}
