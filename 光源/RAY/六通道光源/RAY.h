// RAY.h : new_move_device DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "NewMoveDlg.h"

#if (defined WIN32 || defined _WIN32 || defined WINCE)
#ifdef PROCLIB_EXPORTS
#define PROCLIB_API __declspec(dllexport)
#else
#define PROCLIB_API __declspec(dllimport)
#endif

#else
#define PROCLIB_API 
#endif
//��Դ������
#include "automove.h"
#include<math.h>

using namespace std;
using namespace rp;
using namespace arch;
using namespace net;
/*
ͨѶЭ�����£�
������ 19200
����λ 8
ֹͣλ 1
У��λ ��
�����ַ� ��

ͨѶָ��ʾ�����£�
��ͨ���磺S100T150F200T250FC#
����SΪ��ʼ�ַ���T�����Դ״̬����F�����Դ״̬��0��255���ɵ���һͨ�����ȼ�Ϊ100����ͨ�����ȼ�Ϊ150����ͨ�����ȼ�Ϊ200����ͨ�����ȼ�Ϊ250��
ָ������S100F150F200F250FC#
S100T150T200T250TC#
��ͨ���磺S100T150TC#
����SΪ��ʼ�ַ���T�����Դ״̬����F�����Դ״̬��0��255���ɵ���һͨ�����ȼ�Ϊ100����ͨ�����ȼ�Ϊ150��
*/
struct S_RayDevice
{
	S_RayDevice();
	~S_RayDevice();
	bool revData();
	bool getComNum();//�Զ�������Դ��com��
	u_result connect(const char * port_path, _u32 baudrate);
	bool sendData(string st);
	bool closeRay();
	void disconnect();
	string getDefault();
	void setDefault(string st);
	bool isConnected();
	string getStr();
	string m_com;
	_u32 m_baudrate;
	string m_stopSt;//�ر�����
	string m_st;//Ĭ�Ϸ��� 

	rp::arch::net::raw_serial m_rxtx;
	rp::hal::Locker  m_Lock;
	bool _isConnected;
};

struct S_vctRay
{
	S_vctRay();
	~S_vctRay();
	bool openRay(string st,int id);
	bool closeRay(int id);
	string getDefault(int id);
	void setDefault(string st,int id);
	bool openRay(int id);
	void release();
	bool loadCfg(string cfgfile);
	bool writeCfg(string cfgfile);

	vector<S_RayDevice*> m_vctRay;//���й�Դ
	int m_curRay;//��ǰ��Դ

	bool addRay(string command);
private:
	bool loadRay(string command,int id);
	void codeStr(string& sendstr,int id);
};

extern S_vctRay g_Ray;
// Cnew_move_deviceApp
// �йش���ʵ�ֵ���Ϣ������� new_move_device.cpp
//

class CRAYApp : public CWinApp
{
public:
	CRAYApp();

// ��д
public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};

extern "C" PROCLIB_API bool RayDllcloseRay( int id);
extern "C" PROCLIB_API bool RayDllopenRay(string st,int id);
extern "C" PROCLIB_API void RayDllWriteFile(string cfgfile);
extern "C" PROCLIB_API bool RayDLLLoadFile(string cfgfile);
extern "C" PROCLIB_API void RayDllRelease();
extern "C" PROCLIB_API bool RayDLLIni(string cfgfile);
extern "C" PROCLIB_API string RayDLLgetDefault(int id);
extern "C" PROCLIB_API void RayDLLsetDefault(string st,int id);
extern "C" PROCLIB_API int RayDLLgetRayNum();
