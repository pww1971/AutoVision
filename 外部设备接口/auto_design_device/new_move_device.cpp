// new_move_device.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "new_move_device.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
//#include "checkPatch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif



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

// Cnew_move_deviceApp

BEGIN_MESSAGE_MAP(Cnew_move_deviceApp, CWinAppEx)
END_MESSAGE_MAP()


// Cnew_move_deviceApp ����

Cnew_move_deviceApp::Cnew_move_deviceApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
	//	SetSystem("parallelize_operators", "true");
}


// Ψһ��һ�� Cnew_move_deviceApp ����

Cnew_move_deviceApp theApp;


// Cnew_move_deviceApp ��ʼ��

BOOL Cnew_move_deviceApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	/*UCI::pDis = new ST_MYDISP(&checkPatch::S_dlg);
	UCI::initFirst();*/
	return TRUE;
}

string MoveDLLHelp()
{
	string szHelp = "\r\n�ѵ�ǰͼ���Ƴ��޸�ͼƬ�������������������±��浽�����ļ�����\r\nautoDesign, id = �Զ���ģ, ���id\r\niniDesign,dir = ��ʼ��, dir��ģ��Ŀ¼\r\n \
setPwwOffSet,0.2 = ����ƫ����\r\ncheckObj,id = ��ά����֤Ŀ��";
	return szHelp;
}

bool MoveDLLIni(void* pmv, void*& p2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (pmv == NULL)
	{
		return false;
	}
	if (NULL == p2)
	{
		p2 = new CDevice();
	}
	return true;
}

void MoveDLLStop()
{

}
//��̬������ȡ�ı꺯��
bool MoveDLLMove(void* pmv, void*& p2, string& data)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CVisionSystem* mb = (CVisionSystem*)pmv;
	if (NULL != mb)
	{
		if (NULL == p2)
		{
			p2 = new CDevice();
		}

		std::vector<string> vct;
		CStrProcess::Split(data, ",", vct);
		if ("autoDesign" == vct[0])//
		{
			if (vct.size() != 2)
			{
				data = "error format:" + data;
				return false;
			}
			int camid = atoi(vct[1].c_str());
			if (-1 == camid)
			{
				camid = mb->m_pCheck->m_curCamId;
			}
			std::vector<cv::Mat> vm;
			mb->m_pCheck->camPic(camid, vm);
			if (((CDevice*)p2)->autoDesign(vm))//(((CDevice*)p2)->wechatDesign(m))//
				;
			else
			{
				data = "error:" + data;
				return false;
			}
		}
		else if (vct[0] == "iniDesign")//
		{
			if (vct.size() != 2)
			{
				data = "error format:" + data;
				return false;
			}
			int procid = mb->m_pCheck->m_picProcId;
			((CDevice*)p2)->ini(mb->m_pCheck->findProcName(procid) + PROCFILE, vct[1]);
		}
		else if (vct[0] == "setPwwOffSet")
		{
			if (vct.size() != 2)
			{
				data = "error format:" + data;
				return false;
			}
			((CDevice*)p2)->m_mkc.m_pwwoffset = atof(vct[1].c_str());
		}
		else if (vct[0] == "checkObj")
		{
			if (vct.size() != 2)
			{
				data = "error format:" + data;
				return false;
			}
			int camid = atoi(vct[1].c_str());
			if (-1 == camid)
			{
				camid = mb->m_pCheck->m_curCamId;
			}
			std::vector<cv::Mat> vm;
			mb->m_pCheck->camPic(camid, vm);
			if (!((CDevice*)p2)->checkObj(vm))
			{
				data = "error :" + data;
				return false;
			}
		}
		else
		{
			data = "error format:" + data;
			return false;
		}
	}
	else
	{
		data = "error noSysPtr:" + data;
		return false;
	}
	data = "";
	return true;
}

//������ʾ�ĵ����ͷ��ڴ�
void MoveDllRelease(void*& p2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (NULL != p2)
	{
		delete (CDevice*)p2;
		p2 = NULL;
	}
}

void MoveDllWriteFile()
{

}


