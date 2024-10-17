/* MOVE_CARD.cpp : 定义 DLL 的初始化例程。
//针对AD转换和插补运动的相关接口 可以根据需求自行添加
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

//动态库中提取的标函数
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
extern "C" PROCLIB_API bool MoveDLLmoveAbs(int mtid,float pos);//绝对运动
bool MoveDLLmoveAbs(int mtid,float pos)
{
	return g_dlg.m_handle.moveAbs(mtid,pos);
}
extern "C" PROCLIB_API bool MoveDLLmove(int mtid,float dis);//相对运动
bool MoveDLLmove(int mtid,float dis)
{
	return g_dlg.m_handle.move(mtid,dis);
}
extern "C" PROCLIB_API bool MoveDLLmoveV(int mtid,int di);//连续运动 di正向1 反向-1
bool MoveDLLmoveV(int mtid,int di)
{
	return g_dlg.m_handle.moveV(mtid,di);
}
extern "C" PROCLIB_API bool MoveDLLmoveStop(int mtid);//针对连续运动
bool MoveDLLmoveStop(int mtid)
{
	return g_dlg.m_handle.moveStop(mtid);
}
extern "C" PROCLIB_API bool MoveDLLmtZero(int mtid);//归零
bool MoveDLLmtZero(int mtid)
{
	return g_dlg.m_handle.mtZero(mtid);
}
extern "C" PROCLIB_API bool MoveDLLsetIO(int ioid,int onoff);//设置io口输出
bool MoveDLLsetIO(int ioid,int onoff)
{
	return g_dlg.m_handle.setIO(ioid,onoff);
}
extern "C" PROCLIB_API bool MoveDLLgetIO( uint state[]);//获取输入io口的状态
bool MoveDLLgetIO( uint state[])
{
	return g_dlg.m_handle.getIO(state);
}
//初始化io口输出低位
extern "C" PROCLIB_API bool MoveDLLzeroIO();
bool MoveDLLzeroIO()
{
	return g_dlg.m_handle.zeroIO();
};
extern "C" PROCLIB_API bool MoveDLLgetMtState(int mtid,int& state);	//检测电机情况state=0运动中 -1没有运动
bool MoveDLLgetMtState(int mtid,int& state)
{
	bool bret;
	try
	{
		bret = g_dlg.m_handle.getMtState(mtid,state);
	}
	catch (char* e)
	{
		MessageBox(NULL,"错误",e, MB_YESNO | MB_ICONQUESTION) ;
		return false;
	}
	return bret;
}
extern "C" PROCLIB_API bool MoveDLLgetMtPos(int mtid,float& pos);		//检测电机位置
bool MoveDLLgetMtPos(int mtid,float& pos)
{
	return g_dlg.m_handle.getMtPos(mtid,pos);
}
extern "C" PROCLIB_API bool MoveDLLgetParam(std::vector<string>& vctPa);		//获取当前配置
bool MoveDLLgetParam(std::vector<string>& vctPa)
{
	return g_dlg.m_handle.getParam(vctPa);
}
extern "C" PROCLIB_API bool MoveDLLsetParam(string szPa);		//设置配置
bool MoveDLLsetParam(string szPa)
{
	return g_dlg.m_handle.setParam(szPa);
}
extern "C" PROCLIB_API void MoveDLLwaitForStop(int mtid);
void MoveDLLwaitForStop(int mtid)
{
	g_dlg.m_handle.waitForStop(mtid);
}
//获取当前位置与目的的位置差
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
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CMOVE_CARDApp

BEGIN_MESSAGE_MAP(CMOVE_CARDApp, CWinApp)
END_MESSAGE_MAP()


// CMOVE_CARDApp 构造

CMOVE_CARDApp::CMOVE_CARDApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CMOVE_CARDApp 对象

CMOVE_CARDApp theApp;


// CMOVE_CARDApp 初始化

BOOL CMOVE_CARDApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
