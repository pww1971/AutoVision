// new_move_device.h : new_move_device DLL ����ͷ�ļ�
//

#pragma once
#include "stdafx.h"
#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "automove.h"
#if (defined WIN32 || defined _WIN32 || defined WINCE)
#ifdef PROCLIB_EXPORTS
#define PROCLIB_API __declspec(dllexport)
#else
#define PROCLIB_API __declspec(dllimport)
#endif

#else
#define PROCLIB_API 
#endif
struct S_TimeParam
{
	S_TimeParam()
	{
		settime=2000;
	}
	~S_TimeParam()
	{

	}
	std::deque<double> deqtime;
	double settime;
	CResLock m_lock;
};
struct S_DeviceParam
{
	S_DeviceParam()
	{
		m_cfgAdd="";
		m_timeRelay = 0;// -0.13;
		m_Shift = 600;
		m_szSortType= NGSIGNSTR;
		m_viewId = -1;
		m_objW = -1;
		m_setStopCheck = 1;
	}
	~S_DeviceParam()
	{
		release();
	}
	void release()
	{
		map<int,S_TimeParam*>::iterator itor = m_mpTime.begin();
		while(itor != m_mpTime.end())
		{
			delete (*itor).second;
			itor++;
		}
		m_mpTime.clear();
		map<int,S_TimeParam*>::iterator itor2 = m_mpActTime.begin();
		while(itor2 != m_mpActTime.end())
		{
			delete (*itor2).second;
			itor2++;
		}
		m_mpActTime.clear();
		zeropic();
	}
	void zeropic()
	{
		m_lock.lock();
		std::deque<CoordinatePosXYZ*>::iterator itor = m_pic.begin();
		while(itor != m_pic.end())
		{
			delete *itor;
			itor++;
		}
		m_pic.clear();
		m_lock.unLock();
	}
	void loadcfg()
	{
		if (""!=m_cfgAdd)
		{
			CConfig cfg(m_cfgAdd+"\timer.cfg");
			string szVal = cfg.getItem("m_timeRelay");
			if ("" != szVal)
			{
				m_timeRelay = atof(szVal.c_str());
			}
			szVal = cfg.getItem("m_Shift");
			if ("" != szVal)
			{
				m_Shift = atoi(szVal.c_str());
			}
			szVal = cfg.getItem("m_viewId");
			if ("" != szVal)
			{
				m_viewId = atoi(szVal.c_str());
			}
			szVal = cfg.getItem("m_objW");
			if ("" != szVal)
			{
				m_objW = atoi(szVal.c_str());
			}
			szVal = cfg.getItem("m_setStopCheck");
			if ("" != szVal)
			{
				m_setStopCheck = atoi(szVal.c_str());
			}
			szVal = cfg.getItem("m_szSortType");
			if ("" != szVal)
			{
				m_szSortType = szVal;
			}

			release();
			szVal = cfg.getItem("szSort");
			if ("" != szVal)
			{
				std::vector<string> vct;
				CStrProcess::Split(szVal, ",", vct, true);
				int len = vct.size() / 3;
				for (int i = 0; i < len; i++)
				{
					int id = i * 3;
					m_mpNgToIo[vct[id]] = cv::Point(atoi(vct[id+1].c_str()), atoi(vct[id+2].c_str()));
				}
			}
			
			szVal = cfg.getItem("szTime");
			if ("" != szVal)
			{
				std::vector<string> vct;
				CStrProcess::Split(szVal, ",", vct, true);
				int len = vct.size() / 2;
				for (int i = 0; i < len; i++)
				{
					int ii = i * 2;
					int id = atoi(vct[ii].c_str());
					m_mpTime.insert(pair<int, S_TimeParam*>(id, new S_TimeParam()));
					m_mpActTime.insert(pair<int, S_TimeParam*>(id, new S_TimeParam())); 
					m_mpActTime[id]->settime = m_mpTime[id]->settime = atoi(vct[ii+1].c_str());
				}
			}
			
		}
		
	};
	void writecfg()
	{
		if (""!=m_cfgAdd)
		{
			remove((m_cfgAdd + "\timer.cfg").c_str());
			_mkdir(m_cfgAdd.c_str());
			CConfig cfg(m_cfgAdd + "\timer.cfg");
			char sz[32];
			sprintf(sz, "%f", m_timeRelay);
			cfg.updateItem("m_timeRelay", sz);
			sprintf(sz, "%d", m_Shift);
			cfg.updateItem("m_Shift", sz);
			sprintf(sz, "%d", m_viewId);
			cfg.updateItem("m_viewId", sz);
			sprintf(sz, "%d", m_objW);
			cfg.updateItem("m_objW", sz);
			sprintf(sz, "%d", m_setStopCheck);
			cfg.updateItem("m_setStopCheck", sz);
			cfg.updateItem("m_szSortType", m_szSortType);

			string szSort="";
			map<string, cv::Point>::iterator sortit = m_mpNgToIo.begin();
			while (sortit != m_mpNgToIo.end())
			{
				sprintf(sz, "%s,%d,%d,", sortit->first,sortit->second.x, sortit->second.y);
				szSort += sz;
			}
			cfg.updateItem("szSort", szSort);

			string szTime = "";
			map<int, S_TimeParam*>::iterator timeit = m_mpTime.begin();
			while (timeit != m_mpTime.end())
			{
				sprintf(sz, "%d,%.0f,", timeit->first, timeit->second->settime);
				szTime += sz;
			}
			cfg.updateItem("szTime", szTime);
		}
	}
	bool treat()//�Լ����� ʵ�ֶ�Ŀ�� �����㷨��ʵ�ָ��ֶ�λ
	{
		return true;
	}

	void delTimeOnce()//���ж�������ɾ��ʱ��һ��
	{
		int depNum = m_mpTime.size();
		for (int c=1;c<depNum;c++)
		{
			m_mpTime[c]->m_lock.lock();
			if (m_mpTime[c]->deqtime.size()>0)
				m_mpTime[c]->deqtime.pop_front();
			m_mpTime[c]->m_lock.unLock();
		}
	}
	string m_cfgAdd;
	//��ʱ����д�СҪһ��
	map<int,S_TimeParam*> m_mpTime;//������ԭʼʱ�� ͼ����ʹ��
	map<int,S_TimeParam*> m_mpActTime;//ng����Ķ���ʱ�� runTimeʹ��
	//������
	CResLock m_lock;
	std::deque<CoordinatePosXYZ*> m_pic;
	CResLock m_mplock;
	map<string,cv::Point> m_mpNgToIo;//ng���͵�io�����Ͷ�ʱ��
	float m_timeRelay;//��λƫ�Ʋ�������
	int m_Shift;	//��λƫ�Ʋ���
	string m_szSortType;//ͼ����������������Ŀng��ok��Ŀ�����ȫok����
	int m_viewId;//-1 ����ʾng  >-1��ʾ��Ӧ��ͼƬ������id��
	int m_objW;//��ȡroiʱ��Ŀ��뾶 ��������ж�λ ���Ը��ݶ�λ�����ȡroi�� �������
	int m_setStopCheck;//�Ƿ���ͣ��״̬���
};

// Cnew_move_deviceApp
// �йش���ʵ�ֵ���Ϣ������� new_move_device.cpp
//

class Cnew_move_deviceApp : public CWinAppEx
{
public:
	Cnew_move_deviceApp();

// ��д
public:
	virtual BOOL InitInstance();
//	CNewMoveDlg m_mainDlg;
	DECLARE_MESSAGE_MAP()
};

extern "C" PROCLIB_API string MoveDLLHelp();
extern "C" PROCLIB_API bool MoveDLLIni(void* pmv,void*& p2);
extern "C" PROCLIB_API void MoveDLLStop();
//��̬������ȡ�ı꺯��
extern "C" PROCLIB_API bool MoveDLLMove(void* pmv,void*& p2,string& data);

//������ʾ�ĵ����ͷ��ڴ�
extern "C" PROCLIB_API void MoveDllRelease(void*& p2);
extern "C" PROCLIB_API void MoveDllWriteFile();
