
#ifndef PROC_INTERFACE_H
#define PROC_INTERFACE_H
#include <Winsock2.h>
//制程和检测接口

#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cstdarg>


#include <list>
#include <map>
#include <set>
#include <vector>
#include <queue>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <strstream>
using namespace std;

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui_c.h"
#ifdef _DEBUG
#pragma comment( lib, "opencv_world500d.lib")
#else
#pragma comment( lib, "opencv_world500.lib")
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

#pragma pack(1)
//与运动控制的接口
class CorBase
{
public:
	CorBase()
	{
		memset(this, 0, sizeof(*this));
		m_procid = arrMoveId = -1;
	};
	virtual ~CorBase()
	{

	};
	// 返回err ( x , y )
	float x;
	float y;
	float z;
	float r;
	//新添加3d位姿的旋转角
	float xr;//x轴旋转
	float yr;//y轴旋转
	float zr;//z轴旋转
	// 1 等待
	// 2 nc
	// 3 ok
	unsigned int m_nc_ok;

	// 拍照1 次加1 阵列位置
	unsigned int m_check_id;

	// 换盘+ 1
	unsigned int pan_id;
	// 盘内x,y
	unsigned int pan_x;
	unsigned int pan_y;

	// 文件名原始文件
	// pan_id + pan_x + pan_y
	int m_procid;
	//返回定位中心和旋转角度
	int px, py;
	float f;
	//阵列标记 和触发下标对应 主线程=-1
	int arrMoveId;

};
#pragma pack()

class CoordinatePosXYZ :public CorBase
{
public:
	CoordinatePosXYZ() { vm.push_back(cv::Mat()); };
	virtual ~CoordinatePosXYZ() {};
	void printData() {};
	string getUniqueStr(string dir) {};
	virtual void encodeMsg(u_int eMsgType, u_int packetid, vector<void*>& vctMsg, u_int picFormat = 0) {};
	virtual void decodeMsg(map<u_int, void*>& mpMsg) {};
	//Msg消息解码函数
	void decodeMsg(char* cpMsgStr) {};
	virtual string encodeMsg() { return ""; };
	string getPanMsg() { return ""; };
	virtual bool decodeMsg(string szMsgStr) { return true; };
//	void copyImg(vector<cv::Mat>& src);
public:
	cv::Mat raw;//原始图 用于保存记录
	vector<cv::Mat> vm;// 处理图和深度图 编解码不考虑深度图 因为无需传递ng显示 只需写文件到本地而已
	string str;
};

#include "dyna.h"
class CProcGUIDll: public DynaLink
{
public:
	CProcGUIDll();
	~CProcGUIDll();
	//name 动态库的文件名
	bool Load(string dllname);

	//动态库中提取的标函数
	//制程接口
	//设置制程的当前目录
	void (*Proc_SetOpencvGuiDir)( std::string str);
	//设置初始化的制程窗体和位置
	void (*Proc_RunOpencvGuiDlg)( void  *m_hwnd );

	//显示和关闭制程界面窗体
	void (*Proc_VisibleOpencvGuiDlg)( int b );
	//更新主视图和制程视图
	void (*Proc_updateMain)(vector<cv::Mat>& vm);

	//更新第二制程 m.data为空 就还原为单制程
	void (*Proc_updateCurProc)(vector<cv::Mat>& vm);

	//主窗体图片显示
	void (*Proc_viewMainWin)(cv::Mat& m);
	//直方图三通道控件重写需要 鼠标键盘事件的回传
	int (*Proc_DllPreTranslateMessage_opencv_gui)(MSG *pMsg);
	//显式调用退出接口
	void (*Proc_exitOpencvGui)();
	//
	void (*Proc_DllOnSize_opencv_gui)(float fsp[2]);
	bool isLoadedOK;
	//CriticalSection processLock;
	string m_dllname;
};

class CCheckDll : public DynaLink
{
public:
	CCheckDll() { m_pCheck = NULL; };
	~CCheckDll() { dll_ExitDll(m_pCheck); };
	//name 动态库的文件名
	bool Load(string dllname)
	{

		m_dllname = dllname;
		//		WaitAndSignal m(processLock);
		if (IsLoaded())
			return true;
#if defined(WIN32)
		dllname += ".dll";
#else
		dllname += ".so";
#endif
		if (!DynaLink::Open(dllname.c_str())) {
			return false;
		}
		if (!GetFunction("setViewName", (Function&)dll_setViewName)) {
			return false;
		}
		if (!GetFunction("loadprocCheck", (Function&)dll_loadprocCheck)) {
			return false;
		}
		if (!GetFunction("procCheck", (Function&)dll_procCheck)) {
			return false;
		}
		if (!GetFunction("getPos", (Function&)dll_getPos)) {
			return false;
		}
		if (!GetFunction("ExitDll", (Function&)dll_ExitDll)) {
			return false;
		}
		if (!GetFunction("onMouseN", (Function&)dll_onMouseN)) {
			return false;
		}
		if (!GetFunction("viewWin", (Function&)dll_viewWin)) {
			return false;
		}

		isLoadedOK = true;
		return true;
	};

	//检测接口

	//设置显示窗体名称
	void (*dll_setViewName)(string winname, void*& p);
	//制程i的配置加载
	bool (*dll_loadprocCheck)(int i, string  dir, void*& p);
	//检测ok才返回定位结果和坐标位置 
	int (*dll_procCheck)( CoordinatePosXYZ* pd, void*& p);
	//返回制程n的定位目标
	cv::RotatedRect(*dll_getPos)(int n, void*& p);
	//退出dll的显式调用
	void (*dll_ExitDll)(void*& p);
	//鼠标事件响应
	void (*dll_onMouseN)(int i, int event, int x, int y, void*& p);
	//显示图片
	void (*dll_viewWin)(int i, vector<cv::Mat>& vm, void*& p);
	bool isLoadedOK;
	//CriticalSection processLock;
	string m_dllname;
	void* m_pCheck;
};

class CProcDll : public CCheckDll
{
public:
	CProcDll();
	~CProcDll();
	//设置制程的当前目录
	void SetOpencvGuiDir( std::string str);
	//设置初始化的制程窗体和位置
	void RunOpencvGuiDlg( void  *m_hwnd );
	//显示和关闭制程界面窗体
	void VisibleOpencvGuiDlg( int b );
	//更新主视图和制程视图
	void updateMain(vector<cv::Mat>& vm);
	//更新第二制程 m.data为空 就还原为单制程
	void updateCurProc(vector<cv::Mat>& vm);
	//主窗体图片显示
	void viewMainWin(cv::Mat& m);
	//直方图三通道控件重写需要 鼠标键盘事件的回传
	int DllPreTranslateMessage_opencv_gui(MSG *pMsg);
	//显式调用退出接口
	void exitOpencvGui();
	//ONSIZE
	void DllOnSize_opencv_gui(float fsp[2]);

	CProcGUIDll m_guidll;

public:
	//检测的dll接口
	//设置显示窗体名称
	void setViewName(string winname);
	//制程i的实时检测配置加载
	bool loadprocCheck(int i, string  dir);
	//制程i的检测 返回值 定位失败-1 检测失败0，检测成功1
	int procCheck( CoordinatePosXYZ* pd);
	
public:
	
};



#endif