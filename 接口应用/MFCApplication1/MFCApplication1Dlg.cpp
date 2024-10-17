
// MFCApplication1Dlg.cpp: 实现文件
//

#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include "tinyxml2.h"
using namespace tinyxml2;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CProcDll m_pCheck;//检测dll 
void onMouse(int event, int x, int y, int flag = 0, void* p = NULL)
{
	if (event == cv::EVENT_LBUTTONDOWN || event == cv::EVENT_RBUTTONUP || event == cv::EVENT_MBUTTONDOWN)
	{
		m_pCheck.dll_onMouseN(0, event, x, y, m_pCheck.m_pCheck);
		/*if (event == cv::EVENT_MBUTTONDOWN)
		{
			MessageBox(0, (*(string*)m_pCheck.m_pCheck).c_str(), "检测线程状态", MB_YESNO | MB_ICONQUESTION);
		}*/
	}
}
static int CALLBACK  BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch (uMsg)
	{
	case BFFM_INITIALIZED:    //初始化消息
	{
		CString filePath = "./";
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)(LPTSTR)(LPCTSTR)filePath);   //  m_filePath 为类的静态变量
		break;
	}
	case BFFM_SELCHANGED:    //选择路径变化，
	{
		TCHAR curr[MAX_PATH];
		SHGetPathFromIDList((LPCITEMIDLIST)lParam, curr);
		::SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)curr);
	}
	break;
	default:
		break;
	}
	return 0;
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 对话框



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
#ifdef USE_HALCON
	SetSystem("use_window_thread", "true");
	SetSystem("parallelize_operators", "true");
#endif
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1Dlg::OnBnClickedButton1)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication1Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCApplication1Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCApplication1Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CMFCApplication1Dlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 消息处理程序

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	RECT rect;
	GetClientRect(&rect);     //取客户区大小  
	m_old.x = rect.right - rect.left;
	m_old.y = rect.bottom - rect.top;
	openDlg();

	string szviewname = "win_opencv";
	m_pCheck.setViewName("");
	cv::namedWindow(szviewname, cv::WINDOW_NORMAL);
	HWND hWnd = (HWND)cvGetWindowHandle(szviewname.c_str());;
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_STATIC_PIC2)->GetSafeHwnd());
	::ShowWindow(hParent, SW_HIDE);
	
	GetDlgItem(IDC_STATIC_PIC2)->GetClientRect(&rect);
	cv::resizeWindow(szviewname, rect.right, rect.bottom);
	cv::setMouseCallback(szviewname, onMouse);

	GetDlgItem(IDC_STATIC_PIC2)->ShowWindow(0);
	GetDlgItem(IDC_BUTTON2)->ShowWindow(0);
	GetDlgItem(IDC_BUTTON3)->ShowWindow(0);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCApplication1Dlg::openDlg()
{
	m_pCheck.RunOpencvGuiDlg(GetDlgItem(IDC_STATIC_PIC));
	m_pCheck.VisibleOpencvGuiDlg(1);
	m_pCheck.SetOpencvGuiDir("d://myproc");

}


void CMFCApplication1Dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!GetDlgItem(IDC_STATIC_PIC)->IsWindowVisible())
	{
		GetDlgItem(IDC_STATIC_PIC)->ShowWindow(1);
		GetDlgItem(IDC_BUTTON4)->ShowWindow(1);
		GetDlgItem(IDC_STATIC_PIC2)->ShowWindow(0);
		GetDlgItem(IDC_BUTTON2)->ShowWindow(0);
		GetDlgItem(IDC_BUTTON3)->ShowWindow(0);
	}
	else
	{
		GetDlgItem(IDC_STATIC_PIC)->ShowWindow(0);
		GetDlgItem(IDC_BUTTON4)->ShowWindow(0);
		GetDlgItem(IDC_STATIC_PIC2)->ShowWindow(1);
		GetDlgItem(IDC_BUTTON2)->ShowWindow(1);
		GetDlgItem(IDC_BUTTON3)->ShowWindow(1);
	}
}


BOOL CMFCApplication1Dlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (GetDlgItem(IDC_STATIC_PIC)->IsWindowVisible() && \
		m_pCheck.DllPreTranslateMessage_opencv_gui(pMsg))
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CMFCApplication1Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (m_old.x == 0 || nType == SIZE_MINIMIZED)
	{
		return;
	}
	float fsp[2];
	POINT Newp; //获取现在对话框的大小
	CRect recta;
	GetClientRect(&recta);     //取客户区大小  
	Newp.x = recta.right - recta.left;
	Newp.y = recta.bottom - recta.top;
	fsp[0] = (float)Newp.x / m_old.x;
	fsp[1] = (float)Newp.y / m_old.y;
	m_pCheck.DllOnSize_opencv_gui(fsp);
	// TODO: 在此处添加消息处理程序代码
}


void CMFCApplication1Dlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CFolderPickerDialog fd(NULL, 0, this, 0);
	if (fd.DoModal() == IDOK)
	{
		string dir = fd.GetPathName().GetBuffer(0);
		m_pCheck.loadprocCheck(0, dir);
	}
}

string getNgType(string str, string szNgOrOk)
{
	if ("" == str)
	{
		return "空制程";
	}
	tinyxml2::XMLDocument doc;
	doc.Parse(str.c_str());
	if (!doc.Error())
	{
		XMLElement* rootEle = doc.FirstChildElement("POS");
		if (!rootEle)
		{
			return "空制程";
		}
		else
		{
			const char* elementName = "mask";
			XMLElement* maskEle = rootEle->FirstChildElement(elementName);
			if (!maskEle)
			{
				return rootEle->Attribute("AnotherName");
			}
			string str;
			while (maskEle) {//遍历mask
				XMLElement* checkEle = maskEle->FirstChildElement();
				while (checkEle) {//遍历检测子项
					if (NULL != checkEle->Attribute(szNgOrOk.c_str()))//提取ok或ng的制程名
					{
						return maskEle->Attribute("AnotherName");//提取mask的制程名
					}
					checkEle = checkEle->NextSiblingElement();
				}
				maskEle = maskEle->NextSiblingElement(elementName);		//下一个掩膜检测项目		
			}
		}
	}
	else
		return doc.ErrorStr();

	return "";//全ok或全ng
}

//递归获取二次制程的所有ng和ok名称
void getNgType(string xmlStr, vector<string>& vct, string szNgOrOk)
{
	if ("" == xmlStr)
	{
		return;
	}
	tinyxml2::XMLDocument doc;
	doc.Parse(xmlStr.c_str());
	if (!doc.Error())
	{
		XMLElement* rootEle = doc.FirstChildElement("POS");
		if (!rootEle)
		{
			vct.push_back("空制程");
			return;
		}
		else
		{
			const char* elementName = "mask";
			XMLElement* maskEle = rootEle->FirstChildElement(elementName);
			if (!maskEle)
			{
				vct.push_back(rootEle->Attribute("AnotherName"));
				return;
			}
			string str;
			while (maskEle) {//遍历mask
				XMLElement* checkEle = maskEle->FirstChildElement();
				while (checkEle) {//遍历检测子项
					const char* szch = checkEle->Attribute(szNgOrOk.c_str());
					if (NULL != szch)//提取ok或ng的制程名
					{
						vct.push_back(maskEle->Attribute("AnotherName"));//提取mask的制程名
						getNgType(szch, vct, szNgOrOk);
						return;
					}
					checkEle = checkEle->NextSiblingElement();
				}
				maskEle = maskEle->NextSiblingElement(elementName);		//下一个掩膜检测项目		
			}
		}
	}
}

void CMFCApplication1Dlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CoordinatePosXYZ pd;
	pd.vm[0] = cv::imread("mvdesign.png");
	
	if (pd.vm[0].empty())
	{
		return;
	}
	//内含并行运算 不需要多线程访问 单线程就可以把cpu吃光
	pd.m_procid = 0;
	m_pCheck.procCheck(&pd);

	//提取具体的ng或ok结果
	string szRet = pd.str;
	if (szRet.size() > 6)
	{
		vector<string>  o;
		getNgType(szRet, o, "NG");
		for (int i = 0; i < o.size(); i++)
		{
				string szNgType = o[i];//ng具体的类型
		}
	}
}


void CMFCApplication1Dlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	vector<cv::Mat> vm;
	vm.push_back(cv::imread("mvdesign.png"));
	m_pCheck.updateMain(vm);
}


void CMFCApplication1Dlg::OnBnClickedButton5()
{
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(bi));

	TCHAR szDefaultDir[MAX_PATH];
	CString strDef = "./";
	memcpy(szDefaultDir, strDef.GetBuffer(strDef.GetLength() * 2), strDef.GetLength() * 2);
	strDef.ReleaseBuffer();
	szDefaultDir[strDef.GetLength()] = 0;

	bi.hwndOwner = this->GetSafeHwnd();
	bi.pidlRoot = 0;
	bi.pszDisplayName = 0;
	bi.lpszTitle = "浏览文件夹";
	bi.ulFlags = BIF_NEWDIALOGSTYLE;//BIF_STATUSTEXT;
	bi.lpfn = BrowseCallbackProc;        //设置CALLBACK函数
	bi.iImage = 0;
	bi.lParam = long(&szDefaultDir);    //设置默认路径

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (pidl != NULL)
	{
		LPMALLOC pMalloc;

		TCHAR path[MAX_PATH];

		SHGetPathFromIDList(pidl, path);
		if (SUCCEEDED(SHGetMalloc(&pMalloc)))//pidl指向的对象用完应该释放
		{
			pMalloc->Free(pidl);
			pMalloc->Release();
		}
		CString filePath = path;
		filePath += +"/";

		string dir = filePath.GetBuffer(0);
		m_pCheck.SetOpencvGuiDir(dir);
	}
	
}
