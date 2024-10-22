// new_move_device.h : new_move_device DLL ����ͷ�ļ�
//

#pragma once
#include "stdafx.h"
#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "automove.h"
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

struct S_TimeParam
{
	S_TimeParam()
	{
		settime = 2000;
	}
	~S_TimeParam()
	{

	}
	std::deque<double> deqtime;
	double settime;
	CResLock m_lock;
};

struct S_DeviceParam :public CSubscribeClient
{
	S_DeviceParam(CVisionSystem* mb)
	{
		m_mb = mb;
		m_cfgAdd="";
		m_szSortType= NGSIGNSTR;
		m_szDescription = "����CMV���Ľ��ͨѶ�߳�";//�߳�����
		subscribe("CMVRet", "CAMCHECK");//ע��CMVģ�����ͺ���Ϣ���� �յ����Ľ��
	}
	~S_DeviceParam()
	{
		release();
	}
	void release()
	{
		map<int, S_TimeParam*>::iterator itor2 = m_mpActTime.begin();
		while (itor2 != m_mpActTime.end())
		{
			delete (*itor2).second;
			itor2++;
		}
		m_mpActTime.clear();
	}

	virtual bool connectionAnalyse(TcpMsg* pTcpMsg)//ֻ���յ�ע�����Ϣ���ͺ�ģ������
	{
		if (CSubscribeClient::connectionAnalyse(pTcpMsg))//������Ϣ
		{
			return true;
		}

		S_SubscribeMsg msg;
		msg.decodeMsg(pTcpMsg->buf);//������ȡ����Ϣ
		if (msg.szModelType == "CMVRet")
		{
			if (msg.szMsgType == "CAMCHECK")//�յ�CMV�������� ��ng����
			{
				m_msgLock.lock();
				m_deqMsg.push_back(msg.szMsgCont);
				m_msgLock.unLock();
			}
		}

		return true;
	};

	void loadcfg()
	{
		if (""!=m_cfgAdd)
		{
			CConfig cfg(m_cfgAdd+"\timer.cfg");
			string szVal = cfg.getItem("m_szSortType");
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
					m_mpNgToIo[vct[id]] = cv::Point(atoi(vct[id + 1].c_str()), atoi(vct[id + 2].c_str()));
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
					int ii = (i <<1);
					int id = atoi(vct[ii].c_str());
					m_mpActTime.insert(pair<int, S_TimeParam*>(id, new S_TimeParam()));
					m_mpActTime[id]->settime = atoi(vct[ii + 1].c_str());
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
			cfg.updateItem("m_szSortType", m_szSortType);
			string szSort = "";
			char sz[32];
			map<string, cv::Point>::iterator sortit = m_mpNgToIo.begin();
			while (sortit != m_mpNgToIo.end())
			{
				sprintf(sz, "%s,%d,%d,", sortit->first, sortit->second.x, sortit->second.y);
				szSort += sz;
			}
			cfg.updateItem("szSort", szSort);

			string szTime = "";
			map<int, S_TimeParam*>::iterator timeit = m_mpActTime.begin();
			while (timeit != m_mpActTime.end())
			{
				sprintf(sz, "%d,%.0f,", timeit->first, timeit->second->settime);
				szTime += sz;
			}
			cfg.updateItem("szTime", szTime);
		}
	}
	bool hasMsg()
	{
		m_msgLock.lock();
		if (m_deqMsg.size() == 0)
		{
			m_msgLock.unLock();
			return false;
		}
		m_msgLock.unLock();
		return true;
	}
	bool treat()//�Լ����� 
	{
		m_msgLock.lock();
		string szMsg = m_deqMsg.front();
		m_deqMsg.pop_front();
		m_msgLock.unLock();

		//����
		string szType;
		vector<string> vct;
		CStrProcess::Split(szMsg, ",", vct, true);
		double TriggerTime = atof(vct[0].c_str());//����ʱ��
		bool bret = false;
		bool iOK = "OK"== vct[1];
		if (iOK)
			bret = true;
		if (!iOK || m_szSortType == OKSIGNSTR || (iOK && m_szSortType == "allokTri"))
		{
			for (int i = 2; i < vct.size(); i++)
			{
				vector<string> vct2;
				CStrProcess::Split(vct[i], "R", vct2, true);
				if ((vct2[1] == "0" && m_szSortType == NGSIGNSTR) || (vct2[1] == "1" && (m_szSortType == "allokTri" || m_szSortType == OKSIGNSTR)))
				{
					szType = vct2[0];
					break;
				}
			}
			m_mplock.lock();
			map<string, cv::Point>::iterator it = m_mpNgToIo.find(szType);
			if (it != m_mpNgToIo.end())
			{
				int id = it->second.y;//��ʱ��id

				m_mpActTime[id]->m_lock.lock();
				m_mpActTime[id]->deqtime.push_back(TriggerTime);//���봥��ʱ��
				m_mpActTime[id]->m_lock.unLock();

				m_mb->pushIoMsg(it->second.x, 1);//����id
				
			}
			else//���һ���������
			{
				int p = m_mpNgToIo.size() - 1;
				if (p > -1)
				{
					map<string, cv::Point>::iterator it = m_mpNgToIo.end();
					it--;
					int id = it->second.y;//��ʱ��id;
					m_mpActTime[id]->m_lock.lock();
					m_mpActTime[id]->deqtime.push_back(move(TriggerTime));//���봥��ʱ��
					m_mpActTime[id]->m_lock.unLock();
					m_mb->pushIoMsg(it->second.x, 1);
					
				}
			}
			m_mplock.unLock();
		}

		return bret;
	}
	string m_cfgAdd;
	map<int,S_TimeParam*> m_mpActTime;//ng����Ķ���ʱ�� runTimeʹ��
	CResLock m_mplock;
	map<string,cv::Point> m_mpNgToIo;//ng���͵�io�����Ͷ�ʱ��
	string m_szSortType;

	deque<string> m_deqMsg;//���
	CResLock m_msgLock;
	CVisionSystem* m_mb;

	set<string> m_stProcName;//��Ҫ��ȡ�������Ŀ����
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
