// new_move_device.h : new_move_device DLL 的主头文件
//

#pragma once
#include "stdafx.h"
#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
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
	bool treat()//自己处理 实现多目标 根据算法来实现各种定位
	{
		return true;
	}

	void delTimeOnce()//所有动作队列删除时间一次
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
	//两时间队列大小要一致
	map<int,S_TimeParam*> m_mpTime;//触发的原始时间 图像检测使用
	map<int,S_TimeParam*> m_mpActTime;//ng分类的动作时间 runTime使用
	//检测变量
	CResLock m_lock;
	std::deque<CoordinatePosXYZ*> m_pic;
	CResLock m_mplock;
	map<string,cv::Point> m_mpNgToIo;//ng类型到io触发和定时器
	float m_timeRelay;//定位偏移补偿参数
	int m_Shift;	//定位偏移参数
	string m_szSortType;//图像检测流程里各检测项目ng或ok项目分类或全ok分类
	int m_viewId;//-1 不显示ng  >-1显示对应的图片到界面id上
	int m_objW;//提取roi时的目标半径 相机触发有定位 可以根据定位结果提取roi后 丢给检测
	int m_setStopCheck;//是否做停机状态检测
};

// Cnew_move_deviceApp
// 有关此类实现的信息，请参阅 new_move_device.cpp
//

class Cnew_move_deviceApp : public CWinAppEx
{
public:
	Cnew_move_deviceApp();

// 重写
public:
	virtual BOOL InitInstance();
//	CNewMoveDlg m_mainDlg;
	DECLARE_MESSAGE_MAP()
};

extern "C" PROCLIB_API string MoveDLLHelp();
extern "C" PROCLIB_API bool MoveDLLIni(void* pmv,void*& p2);
extern "C" PROCLIB_API void MoveDLLStop();
//动态库中提取的标函数
extern "C" PROCLIB_API bool MoveDLLMove(void* pmv,void*& p2,string& data);

//必须显示的调用释放内存
extern "C" PROCLIB_API void MoveDllRelease(void*& p2);
extern "C" PROCLIB_API void MoveDllWriteFile();
