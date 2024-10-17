// new_move_device.cpp : 定义 DLL 的初始化例程。
//

#include "new_move_device.h"
#include "CConFigDlg.h"

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
#ifdef nouseEN
	string szHelp = "\r\n飞拍定制:setTime,n,t = 设置定时器n的定时时间\r\ninsertTime,n = n插入开始时间\r\ninsertActTime,n = 插入动作时间\r\ninsertAllTime=触发插入所有开始时间\r\n\
runTime,n = 执行动作时间\r\ncam,id = 相机id提取图片\r\ncamTrigger,id=相机id拍照触发\r\ncheckpic,id = 制程id检测图片\r\nmovecheck=清除一次检测\r\nzeropic=清除全部检测\r\nsetNGIO,ng名称,ioid,tmid=设置ng类型映射io和定时器\r\n\
setRelay,t=设置定位偏移补偿参数时间/像素\r\nsetShift,t=设置定位偏移参数\r\nsetSortType,t=0 ng 1 ok分类 2全ok动作\r\nsetViewId,id=设置显示id\r\nsetObjW,W=设置目标半径\r\nsetStopCheck,1=设置停机状态检测，0不检测";
#else
	string szHelp = "\r\nFlyShots:setTime,n,t = Set the time of timer\r\ninsertTime,n = n Insert the start time\r\ninsertActTime,n = Insert action time\r\ninsertAllTime=Triggers insertion of all start times\r\n\
runTime,n = The time of the execution of the action\r\ncam,id = Camera ID extracts pictures\r\ncamTrigger,id=Camera ID trigger taking a photo\r\ncheckpic,id = Process ID detection image\r\nmovecheck=Clear detection once\r\nzeropic=Clear all detections\r\nsetNGIO,ng Name,ioid,tmid=Set the ng type mapping io and timer\r\n\
setRelay,t=Set the Positioning Offset Compensation param Time/pixels\r\nsetShift,t=Set the Positioning Offset param\r\nsetSortType,t=0 ng 1 ok classify 2 All OK actions\r\nsetViewId,id=Set view id\r\nsetObjW,W=Set the target radius\r\nsetStopCheck,1=Set up downtime detection，0 Not detected";
#endif
	return szHelp;
}

bool MoveDLLIni(void* pmv, void*& p2)
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
		devParam = new S_DeviceParam();
		string szDir = mb->m_szCfgAdd;
		int pos = szDir.size() - strlen("MoveCfg");
		szDir = szDir.substr(0, pos);
		devParam->m_cfgAdd = szDir + "timerdllcfg";
		devParam->loadcfg();
		p2 = devParam;
	}
	else
		devParam = (S_DeviceParam*)p2;
	//CConFigDlg dlg;
	//dlg.m_pDevParam = devParam;
	//if (1 == dlg.DoModal())
	//{
	//	//配置文件放在运动制程文件夹
	//	string szDir = mb->m_szCfgAdd;
	//	int pos = szDir.size() - strlen("MoveCfg");
	//	szDir = szDir.substr(0, pos);
	//	devParam->m_cfgAdd = szDir + "timerdllcfg";
	//	devParam->writecfg();
	//}
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
		if (p2 == NULL)
		{
			devParam = new S_DeviceParam();
			string szDir = mb->m_szCfgAdd;
			int pos = szDir.size() - strlen("MoveCfg");
			szDir = szDir.substr(0, pos);
			devParam->m_cfgAdd = szDir + "timerdllcfg";
			devParam->loadcfg();
			p2 = devParam;
		}
		else
			devParam = (S_DeviceParam*)p2;

		if (data == "cam")//拍照检测 
		{
			CoordinatePosXYZ* pmsg = new CoordinatePosXYZ();
			if (-1 == mb->m_pCheck->camPic(mb->m_pCheck->m_curCamId, pmsg->vm))
			{
				data = "";
				delete pmsg;
				pmsg = NULL;
				return false;
			}
			pmsg->pan_id = rand();
			/*pmsg->pan_x = rand();
			pmsg->pan_y = rand();
			pmsg->m_check_id = rand();*/
			devParam->m_lock.lock();
			devParam->m_pic.push_back(pmsg);
			devParam->m_lock.unLock();
		}
		else if (data == "camTrigger")//拍照触发
		{
			static cv::Point lastPos(0, 0);//上次定位数据
			CoordinatePosXYZ* pmsg = new CoordinatePosXYZ();
			if (-1 == mb->m_pCheck->camPic(mb->m_pCheck->m_curCamId, pmsg->vm))
			{
				data = "";
				delete pmsg;
				pmsg = NULL;
				return false;
			}
			pmsg->pan_id = rand();
			/*pmsg->pan_x = rand();
			pmsg->pan_y = rand();
			pmsg->m_check_id = rand();*/
			pmsg->m_procid = 0;
			int ret = mb->m_pCheck->procCheck(pmsg);
			if (ret != 1)
			{
				data = "";
				delete pmsg;
				pmsg = NULL;
				lastPos = cv::Point(0, 0);
				return false;
			}
			//目标位置偏移中心距离在偏移量范围内
			else if (abs(pmsg->px - pmsg->vm[0].cols / 2) > devParam->m_Shift)
			{
				data = "";
				delete pmsg;
				pmsg = NULL;
				lastPos = cv::Point(0, 0);
				return false;
			}
			//二次拍照是否图片相同 相同就是停机
			if (devParam->m_setStopCheck && abs(pmsg->px - lastPos.x) < 20 && abs(pmsg->py - lastPos.y) < 20)
			{
#ifdef nouseEN
				data = "停机状态"; 
#else
				data = "Downtime status";
#endif
				delete pmsg;
				pmsg = NULL;
				return false;
			}
			lastPos = cv::Point(pmsg->px, pmsg->py);
			devParam->m_lock.lock();
			devParam->m_pic.push_back(pmsg);
			devParam->m_lock.unLock();
		}
		else if (data == "movecheck")
		{
			devParam->m_lock.lock();
			if (devParam->m_pic.size() == 0)
			{
				devParam->m_lock.unLock();
				devParam->delTimeOnce();
#ifdef nouseEN
				data = "图片队列空异常";
#else
				data = "The image queue is empty";
#endif
				return false;
			}
			CoordinatePosXYZ* pmsg = devParam->m_pic.front();
			devParam->m_pic.pop_front();
			devParam->m_lock.unLock();
			delete pmsg;
			pmsg = NULL;
			devParam->delTimeOnce();
		}
		else if (data == "checkpic")
		{
			devParam->m_lock.lock();
			if (devParam->m_pic.size() == 0)
			{
				devParam->m_lock.unLock();
				devParam->delTimeOnce();
#ifdef nouseEN
				data = "图片队列空异常";
#else
				data = "The image queue is empty";
#endif
				return false;
			}
			data = "";
			CoordinatePosXYZ* pmsg = devParam->m_pic.front();
			devParam->m_pic.pop_front();
			devParam->m_lock.unLock();
			//检测ng要保证时序不变  必须在这里完成对计时器的同步操作
			devParam->m_mplock.lock();
			float timeDelay = devParam->m_timeRelay * (pmsg->px - pmsg->vm[0].cols / 2);
			//提前roi
			if (timeDelay != 0 && -1 != devParam->m_objW)
			{
				cv::Rect roi = cv::Rect(pmsg->px - devParam->m_objW, 0, (devParam->m_objW << 1), pmsg->vm[0].rows);
				roi &= cv::Rect(0, 0, pmsg->vm[0].cols, pmsg->vm[0].rows);
				pmsg->vm[0] = cv::Mat(pmsg->vm[0], roi);
			}
			pmsg->m_procid = mb->m_pCheck->m_picProcId;
			int ret = mb->m_pCheck->procCheck(pmsg);

			if (NULL != mb->m_tbReport)
				mb->m_tbReport->pushMsg(pmsg);
			string szSortRet = pmsg->str;

			if (ret != 1 || devParam->m_szSortType == OKSIGNSTR || (ret == 1 && devParam->m_szSortType == "allokTri"))
			{
				string szType;
				if (devParam->m_szSortType == "allokTri")
					szType = getNgType(szSortRet, OKSIGNSTR);
				else
					szType = getNgType(szSortRet, devParam->m_szSortType);

				map<string, cv::Point>::iterator it = devParam->m_mpNgToIo.find(szType);
				if (it != devParam->m_mpNgToIo.end())
				{
					int id = it->second.y;//定时器id
					map<int, S_TimeParam*>::iterator it2 = devParam->m_mpTime.find(id);
					if (devParam->m_mpTime.end() != it2)
					{
						double dt = 0;
						it2->second->m_lock.lock();
						if (it2->second->deqtime.size() > 0)
							dt = it2->second->deqtime.front() + timeDelay;//插入动作时间
						it2->second->m_lock.unLock();

						devParam->m_mpActTime[id]->m_lock.lock();
						devParam->m_mpActTime[id]->deqtime.push_back(move(dt));//插入动作时间
						devParam->m_mpActTime[id]->m_lock.unLock();
						
						if (devParam->m_viewId > -1)
							mb->m_pCheck->viewWin(devParam->m_viewId, pmsg->vm);//使用制程的界面 因此需要保证制程表有空制程或与制程共画面

						mb->pushIoMsg(it->second.x, 1);//触发id
					}
					else
						data = "error";
				}
				else
				{
					data = "error";
				}
				if ("error" == data)
				{

					int p = devParam->m_mpNgToIo.size() - 1;

					if (p > -1)
					{
						map<string, cv::Point>::iterator it = devParam->m_mpNgToIo.end();
						it--;
						int id = it->second.y;//定时器id;
						map<int, S_TimeParam*>::iterator it2 = devParam->m_mpTime.find(id);
						if (devParam->m_mpTime.end() != it2)
						{
							double dt = 0;
							it2->second->m_lock.lock();
							if (it2->second->deqtime.size() > 0)
								dt = it2->second->deqtime.front() + timeDelay;//插入动作时间
							it2->second->m_lock.unLock();

							devParam->m_mpActTime[id]->m_lock.lock();
							devParam->m_mpActTime[id]->deqtime.push_back(move(dt));//插入动作时间
							devParam->m_mpActTime[id]->m_lock.unLock();

							if (devParam->m_viewId > -1)
								mb->m_pCheck->viewWin(devParam->m_viewId, pmsg->vm);//使用制程的界面 因此需要保证制程表有空制程或与制程共画面

							mb->pushIoMsg(it->second.x, 1);
						}
					}

				}
			}
			devParam->m_mplock.unLock();
			delete pmsg;
			pmsg = NULL;

			devParam->delTimeOnce();

			return ret == 1;//检测成功返回真
		}
		else if (data == "zeropic")
		{
			char sz[128];
			sprintf(sz, "picnum=%d", devParam->m_pic.size());
			data = sz;
			devParam->zeropic();
		}
		else if (data == "insertAllTime")
		{
			int depNum = devParam->m_mpTime.size();
			if (depNum > 0)
			{
				for (int c = 1; c < depNum; c++)
				{
					devParam->m_mpTime[c]->m_lock.lock();
					devParam->m_mpTime[c]->deqtime.push_back(move(clock()));
					devParam->m_mpTime[c]->m_lock.unLock();
				}
				devParam->m_mpActTime[0]->m_lock.lock();
				devParam->m_mpActTime[0]->deqtime.push_back(move(clock()));
				devParam->m_mpActTime[0]->m_lock.unLock();
			}
		}
		else
		{
			std::vector<string > cont;
			CStrProcess::Split(data, ",", cont, true);

			if (cont.size() < 2)
			{
				data = "error format:" + data;
				return false;
			}
			int id = atoi(cont[1].c_str());
			if (cont[0] == "cam")//拍照检测 
			{
				if (-1==id)
				{
					id = mb->m_pCheck->m_curCamId;
				}
				CoordinatePosXYZ* pmsg = new CoordinatePosXYZ();
				if (-1 == mb->m_pCheck->camPic(id, pmsg->vm))
				{
					data = "";
					delete pmsg;
					pmsg = NULL;
					return false;
				}
				pmsg->pan_id = rand();
				/*pmsg->pan_x = rand();
				pmsg->pan_y = rand();
				pmsg->m_check_id = rand();*/
				devParam->m_lock.lock();
				devParam->m_pic.push_back(pmsg);
				devParam->m_lock.unLock();
			}
			else if (cont[0] == "setStopCheck")
			{
				devParam->m_setStopCheck = atoi(cont[1].c_str());
			}
			else if (cont[0] == "camTrigger")//拍照触发
			{
				static CPoint lastPos(0, 0);//上次定位数据
				if (-1 == id)
				{
					id = mb->m_pCheck->m_curCamId;
				}
				CoordinatePosXYZ* pmsg = new CoordinatePosXYZ();
				if (-1 == mb->m_pCheck->camPic(id, pmsg->vm))
				{
					data = "";
					delete pmsg;
					pmsg = NULL;
					return false;
				}
				pmsg->pan_id = rand();
				/*pmsg->pan_x = rand();
				pmsg->pan_y = rand();
				pmsg->m_check_id = rand();*/
				pmsg->m_procid = 0;
				int ret = mb->m_pCheck->procCheck(pmsg);
				if (ret != 1)
				{
					data = "";
					delete pmsg;
					pmsg = NULL;
					lastPos = CPoint(0, 0);
					return false;
				}
				//目标位置偏移中心距离在偏移量范围内
				else if (abs(pmsg->px - pmsg->vm[0].cols / 2) > devParam->m_Shift)
				{
					data = "";
					delete pmsg;
					pmsg = NULL;
					lastPos = CPoint(0, 0);
					return false;
				}
				//二次拍照是否图片相同 相同就是停机
				if (devParam->m_setStopCheck && abs(pmsg->px - lastPos.x) < 20 && abs(pmsg->py - lastPos.y) < 20)
				{
#ifdef nouseEN
					data = "停机状态";
#else
					data = "Downtime status";
#endif
					delete pmsg;
					pmsg = NULL;
					return false;
				}
				lastPos = CPoint(pmsg->px, pmsg->py);
				devParam->m_lock.lock();
				devParam->m_pic.push_back(pmsg);
				devParam->m_lock.unLock();
			}
			else if (cont[0] == "checkpic")
			{
				devParam->m_lock.lock();
				if (devParam->m_pic.size() == 0)
				{
					devParam->m_lock.unLock();
					devParam->delTimeOnce();
#ifdef nouseEN
					data = "图片队列空异常";
#else
					data = "The image queue is empty";
#endif
					return false;
				}
				data = "";
				CoordinatePosXYZ* pmsg = devParam->m_pic.front();
				devParam->m_pic.pop_front();
				devParam->m_lock.unLock();
				//检测ng要保证时序不变  必须在这里完成对计时器的同步操作
				devParam->m_mplock.lock();
				float timeDelay = devParam->m_timeRelay * (pmsg->px - pmsg->vm[0].cols / 2);
				//提前roi
				if (timeDelay != 0 && -1 != devParam->m_objW)
				{
					cv::Rect roi = cv::Rect(pmsg->px - devParam->m_objW, 0, (devParam->m_objW << 1), pmsg->vm[0].rows);
					roi &= cv::Rect(0, 0, pmsg->vm[0].cols, pmsg->vm[0].rows);
					pmsg->vm[0] = cv::Mat(pmsg->vm[0], roi);
				}
				if (-1 == id)
				{
					id = mb->m_pCheck->m_picProcId;
				}
				pmsg->m_procid = id;
				int ret = mb->m_pCheck->procCheck( pmsg);

				if (NULL != mb->m_tbReport)
					mb->m_tbReport->pushMsg(pmsg);
				string szSortRet = pmsg->str;

				if (ret != 1 || devParam->m_szSortType == OKSIGNSTR || (ret == 1 && devParam->m_szSortType == "allokTri"))
				{
					string szType;
					if (devParam->m_szSortType == "allokTri")
						szType = getNgType(szSortRet, OKSIGNSTR);
					else
						szType = getNgType(szSortRet, devParam->m_szSortType);

					map<string, cv::Point>::iterator it = devParam->m_mpNgToIo.find(szType);
					if (it != devParam->m_mpNgToIo.end())
					{
						int id = it->second.y;//定时器id
						map<int, S_TimeParam*>::iterator it2 = devParam->m_mpTime.find(id);
						if (devParam->m_mpTime.end() != it2)
						{
							double dt = 0;
							it2->second->m_lock.lock();
							if (it2->second->deqtime.size() > 0)
								dt = it2->second->deqtime.front() + timeDelay;//插入动作时间
							it2->second->m_lock.unLock();

							devParam->m_mpActTime[id]->m_lock.lock();
							devParam->m_mpActTime[id]->deqtime.push_back(move(dt));//插入动作时间
							devParam->m_mpActTime[id]->m_lock.unLock();

							if (devParam->m_viewId > -1)
							{
								mb->m_pCheck->viewWin(devParam->m_viewId, pmsg->vm);//使用制程的界面 因此需要保证制程表有空制程或与制程共画面
							}
							mb->pushIoMsg(it->second.x, 1);//触发id
						}
						else
							data = "error";
					}
					else
					{
						data = "error";
					}
					if ("error" == data)
					{

						int p = devParam->m_mpNgToIo.size() - 1;

						if (p > -1)
						{
							map<string, cv::Point>::iterator it = devParam->m_mpNgToIo.end();
							it--;
							int id = it->second.y;//定时器id;
							map<int, S_TimeParam*>::iterator it2 = devParam->m_mpTime.find(id);
							if (devParam->m_mpTime.end() != it2)
							{
								double dt = 0;
								it2->second->m_lock.lock();
								if (it2->second->deqtime.size() > 0)
									dt = it2->second->deqtime.front() + timeDelay;//插入动作时间
								it2->second->m_lock.unLock();

								devParam->m_mpActTime[id]->m_lock.lock();
								devParam->m_mpActTime[id]->deqtime.push_back(move(dt));//插入动作时间
								devParam->m_mpActTime[id]->m_lock.unLock();

								if (devParam->m_viewId > -1)
								{
									mb->m_pCheck->viewWin(devParam->m_viewId, pmsg->vm);//使用制程的界面 因此需要保证制程表有空制程或与制程共画面
									//HWND hWnd = FindWindowA(NULL, CStrProcess::itoa_radixN(devParam->m_viewId).c_str());
									//if(hWnd !=0)
									//	imshow(CStrProcess::itoa_radixN(devParam->m_viewId), pmsg->m), cv::waitKey(1);
								}
								mb->pushIoMsg(it->second.x, 1);
							}
						}

					}
				}
				devParam->m_mplock.unLock();
				delete pmsg;
				pmsg = NULL;

				devParam->delTimeOnce();

				return ret == 1;//检测成功返回真
			}
			else if (cont[0] == "setTime")//
			{
				if (cont.size() < 3)
				{
					data = "error format:" + data;
					return false;
				}
				map<int, S_TimeParam*>::iterator itor = devParam->m_mpTime.find(id);
				if (itor == devParam->m_mpTime.end())
				{
					devParam->m_mpTime.insert(pair<int, S_TimeParam*>(id, new S_TimeParam()));
				}

				devParam->m_mpTime[id]->m_lock.lock();
				devParam->m_mpTime[id]->settime = atoi(cont[2].c_str());
				char sz[128];
				sprintf(sz, "id,size,settime=%d,%d,%f", id, devParam->m_mpTime[id]->deqtime.size(), devParam->m_mpTime[id]->settime);
				data = sz;
				devParam->m_mpTime[id]->deqtime.clear();
				devParam->m_mpTime[id]->m_lock.unLock();

				map<int, S_TimeParam*>::iterator itor2 = devParam->m_mpActTime.find(id);
				if (itor2 == devParam->m_mpActTime.end())
				{
					devParam->m_mpActTime.insert(pair<int, S_TimeParam*>(id, new S_TimeParam()));
				}
				devParam->m_mpActTime[id]->m_lock.lock();
				devParam->m_mpActTime[id]->settime = atoi(cont[2].c_str());
				devParam->m_mpActTime[id]->deqtime.clear();
				devParam->m_mpActTime[id]->m_lock.unLock();
				return true;
			}
			else if (cont[0] == "insertTime")
			{
				map<int, S_TimeParam*>::iterator it = devParam->m_mpTime.find(id);
				if (devParam->m_mpTime.end() != it)
				{
					it->second->m_lock.lock();
					it->second->deqtime.push_back(move(clock()));
					it->second->m_lock.unLock();
				}
			}
			else if (cont[0] == "insertActTime")
			{
				map<int, S_TimeParam*>::iterator it = devParam->m_mpActTime.find(id);
				if (devParam->m_mpActTime.end() != it)
				{
					it->second->m_lock.lock();
					it->second->deqtime.push_back(move(clock()));
					it->second->m_lock.unLock();
				}
			}
			/*else if(cont[0] == "delTime")
			{
				devParam->m_mpTime[id]->m_lock.lock();
				devParam->m_mpTime[id]->deqtime.pop_front();
				devParam->m_mpTime[id]->m_lock.unLock();
			}*/
			else if (cont[0] == "runTime")
			{
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
			else if ("setRelay" == cont[0])
			{
				if (cont.size() < 2)
				{
					data = "error format:" + data;
					return false;
				}
				devParam->m_mplock.lock();
				devParam->m_timeRelay = atof(cont[1].c_str());
				devParam->m_mplock.unLock();
			}
			else if ("setShift" == cont[0])
			{
				if (cont.size() < 2)
				{
					data = "error format:" + data;
					return false;
				}
				devParam->m_mplock.lock();
				devParam->m_Shift = atof(cont[1].c_str());
				devParam->m_mplock.unLock();
			}
			else if (cont[0] == "setSortType")
			{
				devParam->m_mplock.lock();
				if (1 == id)
					devParam->m_szSortType = OKSIGNSTR;
				else if (0 == id)
					devParam->m_szSortType = NGSIGNSTR;
				else
					devParam->m_szSortType = "allokTri";
				devParam->m_mplock.unLock();
			}
			else if ("setViewId" == cont[0])
			{
				devParam->m_viewId = id;
			}
			else if ("setObjW" == cont[0])
			{
				devParam->m_objW = id;
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
