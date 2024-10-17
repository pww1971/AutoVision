// new_move_device.cpp : 定义 DLL 的初始化例程。
//

#include "new_move_device.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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

// Cnew_move_deviceApp

BEGIN_MESSAGE_MAP(Cnew_move_deviceApp, CWinAppEx)
END_MESSAGE_MAP()


// Cnew_move_deviceApp 构造

Cnew_move_deviceApp::Cnew_move_deviceApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}


// 唯一的一个 Cnew_move_deviceApp 对象

Cnew_move_deviceApp theApp;


// Cnew_move_deviceApp 初始化

BOOL Cnew_move_deviceApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();



	return TRUE;
}


string MoveDLLHelp()
{
	string szHelp = "\r\n定制从xml中提取结果:\r\nsetProcName,制程名称 = 添加项目名 \r\ngetProcValue = 获取项目结果数据\r\n定制飞拍NG分类:\r\n\
setTime,n,t = 设置定时器n的定时时间\r\nrunTime,n = 执行动作时间\r\nsetNGIO,ng名称,ioid,tmid=设置ng类型映射io和定时器\r\n\
setSortType,t=0 ng 1 ok分类 2全ok动作\r\nhasData=判断是否有结果数据\r\nsortData=处理一个数据并分类\r\n";
	return szHelp;
}

bool MoveDLLIni(void* pmv, void*& p2)//界面调用的初始化
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
		string szDir = mb->m_szCfgAdd;//配置文件的主目录
		int pos = szDir.size() - strlen("MoveCfg");
		szDir = szDir.substr(0, pos);
		devParam->m_cfgAdd = szDir + "ngsortdllcfg";//建一个子目录保存配置文件
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
//动态库中提取的标函数
bool MoveDLLMove(void* pmv, void*& p2, string& data)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CVisionSystem* mb = (CVisionSystem*)pmv;
	if (NULL != mb)
	{
		S_DeviceParam* devParam;
		if (p2 == NULL)//初始化
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
				data = "相机打开失败";
				return false;
			}
			cor.m_procid = mb->m_pCheck->m_picProcId;
			int ret = mb->m_pCheck->procCheck(&cor);

			if (NULL != mb->m_tbReport)
				mb->m_tbReport->pushMsg(&cor);
			if (ret != -1)
			{
				data = "单目检测结果：";
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
					devParam->m_mpActTime[id]->settime = settime;//延时时间
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

//必须显示的调用释放内存
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


