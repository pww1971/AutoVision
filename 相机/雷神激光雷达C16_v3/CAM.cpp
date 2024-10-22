// CAM.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "CAM.h"

#include "CameraDrv.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#if (defined WIN32 || defined _WIN32 || defined WINCE)
#ifdef PROCLIB_EXPORTS
#define PROCLIB_API __declspec(dllexport)
#else
#define PROCLIB_API __declspec(dllimport)
#endif

#else
#define PROCLIB_API 
#endif



#include "CamSetRoi.h"
CamSetRoi m_dlg;
CameraDrv g_cam;

//��̬������ȡ�ı꺯��
extern "C" PROCLIB_API int CamDLLgrapImage(int id,vector<cv::Mat>& vm);
int CamDLLgrapImage(int id, vector<cv::Mat>& vm)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	vm.clear();
	return g_cam.grapImage(id,vm);
}

extern "C" PROCLIB_API void CamDLLrelease();
void CamDLLrelease()
{	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	g_cam.release();
}

extern "C" PROCLIB_API bool CamDLLini();
bool CamDLLini()
{	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_dlg.m_pCheck = &g_cam;
	if (m_dlg.m_dlgSucc)
	{
		vector<cv::Mat> vm;
		if(g_cam.grapImage(0,vm)>-1)
			g_MKC.updateFrame(vm);
		m_dlg.ShowWindow(1);
		return true;
	}
	CConfig cfg(getExePath() + NETCFG);
	string szWinName = cfg.getItem("WINNAME");
	if (szWinName == "")
	{
		szWinName = "�Ӿ�����ϵͳ";
	}
	HWND hMainWnd=FindWindowA("#32770",szWinName.c_str());//ͨ���������ڷ�ʽ��ȡĿ����������ھ��
	if(hMainWnd!=0)
	{
		m_dlg.Create(IDD_DIALOG1,CWnd::FromHandle(hMainWnd));
	//	m_dlg.ShowWindow(0);
		m_dlg.m_dlgSucc = true;
		return true;
	}
	else
	{
		MessageBoxA(NULL,"ROI�������ʧ��","",0);
		return false;
	}

	return true;
}
//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CCAMApp

BEGIN_MESSAGE_MAP(CCAMApp, CWinApp)
	ON_COMMAND(ID_FILE_NEW, &CCAMApp::OnFileNew)
END_MESSAGE_MAP()


// CCAMApp ����

CCAMApp::CCAMApp()
{
#ifdef CALI
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
	SetSystem("parallelize_operators", "true");

#endif
}


// Ψһ��һ�� CCAMApp ����

CCAMApp theApp;


// CCAMApp ��ʼ��

BOOL CCAMApp::InitInstance()
{
	CWinAppEx::InitInstance();

#ifdef CALI

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
//	CShellManager *pShellManager = new CShellManager;
	
	m_pMainWnd = &checkPatch::S_dlg;
	UCI::pDis = new ST_MYDISP(&checkPatch::S_dlg);
	UCI::initFirst();
#endif
	return TRUE;
}


int CCAMApp::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ
	//if (pShellManager != NULL)
	//{
	//	delete pShellManager;
	//}

	return CWinAppEx::ExitInstance();
}