/* MOVE_CARD.cpp : ���� DLL �ĳ�ʼ�����̡�
//���ADת���Ͳ岹�˶�����ؽӿ� ���Ը��������������
extern "C" PROCLIB_API void myDefineMoveType();
void myDefineMoveType();



*/

#include "stdafx.h"
#include "MOVE_CARD.h"
#include "CfgDlg.h"

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

CCfgDlg g_dlg;

//��̬������ȡ�ı꺯��
extern "C" PROCLIB_API bool MoveDLLini(bool loadDlg);
bool MoveDLLini(bool loadDlg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (!loadDlg)
	{
		g_dlg.m_handle.ini(g_dlg.m_handle.m_linkAdd);
		return true;
	}
	else if(1==g_dlg.DoModal())
		return true;
	else
		return false;
}
extern "C" PROCLIB_API bool MoveDLLwriteMvCfg(CConfig& cfg);
bool MoveDLLwriteMvCfg(CConfig& cfg)
{
	return g_dlg.m_handle.writeMvCfg(cfg);
}
extern "C" PROCLIB_API bool MoveDLLreadMvCfg(CConfig& cfg);
bool MoveDLLreadMvCfg(CConfig& cfg)
{
	return g_dlg.m_handle.readMvCfg(cfg);
}
extern "C" PROCLIB_API void MoveDLLrelease();
void MoveDLLrelease()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	g_dlg.m_handle.release();
}
extern "C" PROCLIB_API void MoveDLLRapidstop();
void MoveDLLRapidstop()
{
	g_dlg.m_handle.Rapidstop(); 
}
extern "C" PROCLIB_API bool MoveDLLmoveAbs(int mtid,float pos);//�����˶�
bool MoveDLLmoveAbs(int mtid,float pos)
{
	return g_dlg.m_handle.moveAbs(mtid,pos);
}
extern "C" PROCLIB_API bool MoveDLLmove(int mtid,float dis);//����˶�
bool MoveDLLmove(int mtid,float dis)
{
	return g_dlg.m_handle.move(mtid,dis);
}
extern "C" PROCLIB_API bool MoveDLLmoveV(int mtid,int di);//�����˶� di����1 ����-1
bool MoveDLLmoveV(int mtid,int di)
{
	return g_dlg.m_handle.moveV(mtid,di);
}
extern "C" PROCLIB_API bool MoveDLLmoveStop(int mtid);//��������˶�
bool MoveDLLmoveStop(int mtid)
{
	return g_dlg.m_handle.moveStop(mtid);
}
extern "C" PROCLIB_API bool MoveDLLmtZero(int mtid);//����
bool MoveDLLmtZero(int mtid)
{
	return g_dlg.m_handle.mtZero(mtid);
}
extern "C" PROCLIB_API bool MoveDLLsetIO(int ioid,int onoff);//����io�����
bool MoveDLLsetIO(int ioid,int onoff)
{
	return g_dlg.m_handle.setIO(ioid,onoff);
}
extern "C" PROCLIB_API bool MoveDLLgetIO( uint state[]);//��ȡ����io�ڵ�״̬
bool MoveDLLgetIO( uint state[])
{
	return g_dlg.m_handle.getIO(state);
}
//��ʼ��io�������λ
extern "C" PROCLIB_API bool MoveDLLzeroIO();
bool MoveDLLzeroIO()
{
	return g_dlg.m_handle.zeroIO();
};
extern "C" PROCLIB_API bool MoveDLLgetMtState(int mtid,int& state);	//��������state=0�˶��� -1û���˶�
bool MoveDLLgetMtState(int mtid,int& state)
{
	bool bret;
	try
	{
		bret = g_dlg.m_handle.getMtState(mtid,state);
	}
	catch (char* e)
	{
		MessageBox(NULL,"����",e, MB_YESNO | MB_ICONQUESTION) ;
		return false;
	}
	return bret;
}
extern "C" PROCLIB_API bool MoveDLLgetMtPos(int mtid,float& pos);		//�����λ��
bool MoveDLLgetMtPos(int mtid,float& pos)
{
	return g_dlg.m_handle.getMtPos(mtid,pos);
}
extern "C" PROCLIB_API bool MoveDLLgetParam(std::vector<string>& vctPa);		//��ȡ��ǰ����
bool MoveDLLgetParam(std::vector<string>& vctPa)
{
	return g_dlg.m_handle.getParam(vctPa);
}
extern "C" PROCLIB_API bool MoveDLLsetParam(string szPa);		//��������
bool MoveDLLsetParam(string szPa)
{
	return g_dlg.m_handle.setParam(szPa);
}
extern "C" PROCLIB_API void MoveDLLwaitForStop(int mtid);
void MoveDLLwaitForStop(int mtid)
{
	g_dlg.m_handle.waitForStop(mtid);
}
//��ȡ��ǰλ����Ŀ�ĵ�λ�ò�
extern "C" PROCLIB_API float MoveDLLgetDvPos(int iaxis);
float MoveDLLgetDvPos(int iaxis)
{
	return g_dlg.m_handle.getDvPos(iaxis);
}

extern "C" PROCLIB_API bool MoveDLLmtReturn();
bool MoveDLLmtReturn()
{
	for (int j=0;j<g_dlg.m_handle.m_mtNum;j++)
		g_dlg.m_handle.mtZero(j);
	return true;
}

extern "C" PROCLIB_API bool MoveDLLHasIni();
bool MoveDLLHasIni()
{
	return g_dlg.m_handle.m_bIni;
}

extern "C" PROCLIB_API cv::Point2f* MoveDLLgetMvScope();
cv::Point2f* MoveDLLgetMvScope()
{
	if (g_dlg.m_handle.m_MvScope.size()>1)
		return &g_dlg.m_handle.m_MvScope[0] ;
	else
		return NULL;
}

extern "C" PROCLIB_API int MoveDLLgetmtNum();
int MoveDLLgetmtNum()
{
	return g_dlg.m_handle.m_mtNum ;
}

extern "C" PROCLIB_API void MoveDLLsetDebugSpeed(bool debug);
void MoveDLLsetDebugSpeed(bool debug)
{
	g_dlg.m_handle.setSpeed(debug) ;
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

// CMOVE_CARDApp

BEGIN_MESSAGE_MAP(CMOVE_CARDApp, CWinApp)
END_MESSAGE_MAP()


// CMOVE_CARDApp ����

CMOVE_CARDApp::CMOVE_CARDApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMOVE_CARDApp ����

CMOVE_CARDApp theApp;


// CMOVE_CARDApp ��ʼ��

BOOL CMOVE_CARDApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
