
#ifndef PROC_INTERFACE_H
#define PROC_INTERFACE_H
#include <Winsock2.h>
//�Ƴ̺ͼ��ӿ�

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
//���˶����ƵĽӿ�
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
	// ����err ( x , y )
	float x;
	float y;
	float z;
	float r;
	//�����3dλ�˵���ת��
	float xr;//x����ת
	float yr;//y����ת
	float zr;//z����ת
	// 1 �ȴ�
	// 2 nc
	// 3 ok
	unsigned int m_nc_ok;

	// ����1 �μ�1 ����λ��
	unsigned int m_check_id;

	// ����+ 1
	unsigned int pan_id;
	// ����x,y
	unsigned int pan_x;
	unsigned int pan_y;

	// �ļ���ԭʼ�ļ�
	// pan_id + pan_x + pan_y
	int m_procid;
	//���ض�λ���ĺ���ת�Ƕ�
	int px, py;
	float f;
	//���б�� �ʹ����±��Ӧ ���߳�=-1
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
	//Msg��Ϣ���뺯��
	void decodeMsg(char* cpMsgStr) {};
	virtual string encodeMsg() { return ""; };
	string getPanMsg() { return ""; };
	virtual bool decodeMsg(string szMsgStr) { return true; };
//	void copyImg(vector<cv::Mat>& src);
public:
	cv::Mat raw;//ԭʼͼ ���ڱ����¼
	vector<cv::Mat> vm;// ����ͼ�����ͼ ����벻�������ͼ ��Ϊ���贫��ng��ʾ ֻ��д�ļ������ض���
	string str;
};

#include "dyna.h"
class CProcGUIDll: public DynaLink
{
public:
	CProcGUIDll();
	~CProcGUIDll();
	//name ��̬����ļ���
	bool Load(string dllname);

	//��̬������ȡ�ı꺯��
	//�Ƴ̽ӿ�
	//�����Ƴ̵ĵ�ǰĿ¼
	void (*Proc_SetOpencvGuiDir)( std::string str);
	//���ó�ʼ�����Ƴ̴����λ��
	void (*Proc_RunOpencvGuiDlg)( void  *m_hwnd );

	//��ʾ�͹ر��Ƴ̽��洰��
	void (*Proc_VisibleOpencvGuiDlg)( int b );
	//��������ͼ���Ƴ���ͼ
	void (*Proc_updateMain)(vector<cv::Mat>& vm);

	//���µڶ��Ƴ� m.dataΪ�� �ͻ�ԭΪ���Ƴ�
	void (*Proc_updateCurProc)(vector<cv::Mat>& vm);

	//������ͼƬ��ʾ
	void (*Proc_viewMainWin)(cv::Mat& m);
	//ֱ��ͼ��ͨ���ؼ���д��Ҫ �������¼��Ļش�
	int (*Proc_DllPreTranslateMessage_opencv_gui)(MSG *pMsg);
	//��ʽ�����˳��ӿ�
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
	//name ��̬����ļ���
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

	//���ӿ�

	//������ʾ��������
	void (*dll_setViewName)(string winname, void*& p);
	//�Ƴ�i�����ü���
	bool (*dll_loadprocCheck)(int i, string  dir, void*& p);
	//���ok�ŷ��ض�λ���������λ�� 
	int (*dll_procCheck)( CoordinatePosXYZ* pd, void*& p);
	//�����Ƴ�n�Ķ�λĿ��
	cv::RotatedRect(*dll_getPos)(int n, void*& p);
	//�˳�dll����ʽ����
	void (*dll_ExitDll)(void*& p);
	//����¼���Ӧ
	void (*dll_onMouseN)(int i, int event, int x, int y, void*& p);
	//��ʾͼƬ
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
	//�����Ƴ̵ĵ�ǰĿ¼
	void SetOpencvGuiDir( std::string str);
	//���ó�ʼ�����Ƴ̴����λ��
	void RunOpencvGuiDlg( void  *m_hwnd );
	//��ʾ�͹ر��Ƴ̽��洰��
	void VisibleOpencvGuiDlg( int b );
	//��������ͼ���Ƴ���ͼ
	void updateMain(vector<cv::Mat>& vm);
	//���µڶ��Ƴ� m.dataΪ�� �ͻ�ԭΪ���Ƴ�
	void updateCurProc(vector<cv::Mat>& vm);
	//������ͼƬ��ʾ
	void viewMainWin(cv::Mat& m);
	//ֱ��ͼ��ͨ���ؼ���д��Ҫ �������¼��Ļش�
	int DllPreTranslateMessage_opencv_gui(MSG *pMsg);
	//��ʽ�����˳��ӿ�
	void exitOpencvGui();
	//ONSIZE
	void DllOnSize_opencv_gui(float fsp[2]);

	CProcGUIDll m_guidll;

public:
	//����dll�ӿ�
	//������ʾ��������
	void setViewName(string winname);
	//�Ƴ�i��ʵʱ������ü���
	bool loadprocCheck(int i, string  dir);
	//�Ƴ�i�ļ�� ����ֵ ��λʧ��-1 ���ʧ��0�����ɹ�1
	int procCheck( CoordinatePosXYZ* pd);
	
public:
	
};



#endif