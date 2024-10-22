// new_move_device.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "new_move_device.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
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



	return TRUE;
}


string MoveDLLHelp()
{
	string szHelp = "\r\n���ƴ�xml����ȡ���:\r\nsetProcName,�Ƴ����� = �����Ŀ�� \r\ngetProcValue = ��ȡ��Ŀ�������\r\n���Ʒ���NG����:\r\n\
setTime,n,t = ���ö�ʱ��n�Ķ�ʱʱ��\r\nrunTime,n = ִ�ж���ʱ��\r\nsetNGIO,ng����,ioid,tmid=����ng����ӳ��io�Ͷ�ʱ��\r\n\
setSortType,t=0 ng 1 ok���� 2ȫok����\r\nhasData=�ж��Ƿ��н������\r\nsortData=����һ�����ݲ�����\r\n";
	return szHelp;
}

bool MoveDLLIni(void* pmv, void*& p2)//������õĳ�ʼ��
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (pmv == NULL)
	{
		return false;
	}
	CVisionSystem* mb = (CVisionSystem*)pmv;
	S_DeviceParam* devParam;
	if (p2 == NULL)
	{
		devParam = new S_DeviceParam(mb);
		string szDir = mb->m_szCfgAdd;//�����ļ�����Ŀ¼
		int pos = szDir.size() - strlen("MoveCfg");
		szDir = szDir.substr(0, pos);
		devParam->m_cfgAdd = szDir + "ngsortdllcfg";//��һ����Ŀ¼���������ļ�
		devParam->loadcfg();
		p2 = devParam;
	}
	else
		devParam = (S_DeviceParam*)p2;

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
		S_DeviceParam* devParam;
		if (p2 == NULL)//��ʼ��
		{
			devParam = new S_DeviceParam(mb);
			string szDir = mb->m_szCfgAdd;
			int pos = szDir.size() - strlen("MoveCfg");
			szDir = szDir.substr(0, pos);
			devParam->m_cfgAdd = szDir + "ngsortdllcfg";
			devParam->loadcfg();

			p2 = devParam;
		}
		else
			devParam = (S_DeviceParam*)p2;
		CMoveDllType* handle = mb->m_mtc.m_handle;
		if (data == "getProcValue")//
		{
			CoordinatePosXYZ cor;
			if (-1 == mb->m_pCheck->camPic(mb->m_pCheck->m_curCamId, cor.vm))
			{
				data = "�����ʧ��";
				return false;
			}
			cor.m_procid = mb->m_pCheck->m_picProcId;
			int ret = mb->m_pCheck->procCheck(&cor);

			if (NULL != mb->m_tbReport)
				mb->m_tbReport->pushMsg(&cor);
			if (ret != -1)
			{
				data = "��Ŀ�������";
				set<string>::iterator itor = devParam->m_stProcName.begin();
				for (; itor != devParam->m_stProcName.end(); itor++)
				{
					string szName = *itor;
					string szDis = getNgCont(cor.str, szName);
					if ("" != szDis)
					{
						double dis = atof(szDis.c_str());
						data += szDis +"\r\n";
						
					}
				}
				return true;
			}
		}
		else if (data == "hasData")
		{
			data = "";
			return devParam->hasMsg();
		}
		else if (data == "sortData")
		{
			data = "";
			return devParam->treat();
		}
		else
		{
			std::vector<string > cont;
			CStrProcess::Split(data, ",", cont, true);
			if (cont.size() > 0)
			{
				if (cont[0] == "setProcName")//
				{
					string szName = cont[1];
					devParam->m_stProcName.insert(szName);
				}
				else if (cont[0] == "setSortType")
				{
					int id = atoi(cont[1].c_str());
					devParam->m_mplock.lock();
					if (1 == id)
						devParam->m_szSortType = OKSIGNSTR;
					else if (0 == id)
						devParam->m_szSortType = NGSIGNSTR;
					else
						devParam->m_szSortType = "allokTri";
					devParam->m_mplock.unLock();
				}
				else if (cont[0] == "setTime")//
				{
					if (cont.size() < 3)
					{
						data = "error format:" + data;
						return false;
					}
					int id = atoi(cont[1].c_str());

					int settime = atoi(cont[2].c_str());
					char sz[128];
					sprintf(sz, "id,settime=%d,%d", id, settime);
					data = sz;

					map<int, S_TimeParam*>::iterator itor2 = devParam->m_mpActTime.find(id);
					if (itor2 == devParam->m_mpActTime.end())
					{
						devParam->m_mpActTime.insert(pair<int, S_TimeParam*>(id, new S_TimeParam()));
					}
					devParam->m_mpActTime[id]->m_lock.lock();
					devParam->m_mpActTime[id]->settime = settime;//��ʱʱ��
					devParam->m_mpActTime[id]->deqtime.clear();
					devParam->m_mpActTime[id]->m_lock.unLock();
					return true;
				}
				else if (cont[0] == "runTime")
				{
					int id = atoi(cont[1].c_str());
					map<int, S_TimeParam*>::iterator it = devParam->m_mpActTime.find(id);
					if (devParam->m_mpActTime.end() != it)
					{
						it->second->m_lock.lock();
						if (it->second->deqtime.size() == 0)
						{
							data = "";
							it->second->m_lock.unLock();
							return false;
						}
						double bt = it->second->deqtime.front();
						it->second->deqtime.pop_front();
						double dinv = it->second->settime - (clock() - bt);
						it->second->m_lock.unLock();
						if (dinv > 0)
						{
							mysleep(dinv);
						}
						/*else if (dinv < -600)
						{
							data = "time dinv < -200";
							return false;
						}*/
					}
				}
				else if ("setNGIO" == cont[0])
				{
					if (cont.size() < 4)
					{
						data = "error format:" + data;
						return false;
					}
					devParam->m_mplock.lock();
					devParam->m_mpNgToIo[cont[1]] = cv::Point(atoi(cont[2].c_str()), atoi(cont[3].c_str()));
					devParam->m_mplock.unLock();
				}
				else
				{
					data = "error format:" + data;
					return false;
				}
			}
			else
			{
				data = "error format:" + data;
				return false;
			}
		}
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
		delete (S_DeviceParam*)p2;
		p2 = NULL;
	}
}

void MoveDllWriteFile()
{

}


