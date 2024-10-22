// new_move_device.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "new_move_device.h"
#include "CConFigDlg.h"

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
#ifdef nouseEN
	string szHelp = "\r\n���Ķ���:setTime,n,t = ���ö�ʱ��n�Ķ�ʱʱ��\r\ninsertTime,n = n���뿪ʼʱ��\r\ninsertActTime,n = ���붯��ʱ��\r\ninsertAllTime=�����������п�ʼʱ��\r\n\
runTime,n = ִ�ж���ʱ��\r\ncam,id = ���id��ȡͼƬ\r\ncamTrigger,id=���id���մ���\r\ncheckpic,id = �Ƴ�id���ͼƬ\r\nmovecheck=���һ�μ��\r\nzeropic=���ȫ�����\r\nsetNGIO,ng����,ioid,tmid=����ng����ӳ��io�Ͷ�ʱ��\r\n\
setRelay,t=���ö�λƫ�Ʋ�������ʱ��/����\r\nsetShift,t=���ö�λƫ�Ʋ���\r\nsetSortType,t=0 ng 1 ok���� 2ȫok����\r\nsetViewId,id=������ʾid\r\nsetObjW,W=����Ŀ��뾶\r\nsetStopCheck,1=����ͣ��״̬��⣬0�����";
#else
	string szHelp = "\r\nFlyShots:setTime,n,t = Set the time of timer\r\ninsertTime,n = n Insert the start time\r\ninsertActTime,n = Insert action time\r\ninsertAllTime=Triggers insertion of all start times\r\n\
runTime,n = The time of the execution of the action\r\ncam,id = Camera ID extracts pictures\r\ncamTrigger,id=Camera ID trigger taking a photo\r\ncheckpic,id = Process ID detection image\r\nmovecheck=Clear detection once\r\nzeropic=Clear all detections\r\nsetNGIO,ng Name,ioid,tmid=Set the ng type mapping io and timer\r\n\
setRelay,t=Set the Positioning Offset Compensation param Time/pixels\r\nsetShift,t=Set the Positioning Offset param\r\nsetSortType,t=0 ng 1 ok classify 2 All OK actions\r\nsetViewId,id=Set view id\r\nsetObjW,W=Set the target radius\r\nsetStopCheck,1=Set up downtime detection��0 Not detected";
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
	//	//�����ļ������˶��Ƴ��ļ���
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
//��̬������ȡ�ı꺯��
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

		if (data == "cam")//���ռ�� 
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
		else if (data == "camTrigger")//���մ���
		{
			static cv::Point lastPos(0, 0);//�ϴζ�λ����
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
			//Ŀ��λ��ƫ�����ľ�����ƫ������Χ��
			else if (abs(pmsg->px - pmsg->vm[0].cols / 2) > devParam->m_Shift)
			{
				data = "";
				delete pmsg;
				pmsg = NULL;
				lastPos = cv::Point(0, 0);
				return false;
			}
			//���������Ƿ�ͼƬ��ͬ ��ͬ����ͣ��
			if (devParam->m_setStopCheck && abs(pmsg->px - lastPos.x) < 20 && abs(pmsg->py - lastPos.y) < 20)
			{
#ifdef nouseEN
				data = "ͣ��״̬"; 
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
				data = "ͼƬ���п��쳣";
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
				data = "ͼƬ���п��쳣";
#else
				data = "The image queue is empty";
#endif
				return false;
			}
			data = "";
			CoordinatePosXYZ* pmsg = devParam->m_pic.front();
			devParam->m_pic.pop_front();
			devParam->m_lock.unLock();
			//���ngҪ��֤ʱ�򲻱�  ������������ɶԼ�ʱ����ͬ������
			devParam->m_mplock.lock();
			float timeDelay = devParam->m_timeRelay * (pmsg->px - pmsg->vm[0].cols / 2);
			//��ǰroi
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
					int id = it->second.y;//��ʱ��id
					map<int, S_TimeParam*>::iterator it2 = devParam->m_mpTime.find(id);
					if (devParam->m_mpTime.end() != it2)
					{
						double dt = 0;
						it2->second->m_lock.lock();
						if (it2->second->deqtime.size() > 0)
							dt = it2->second->deqtime.front() + timeDelay;//���붯��ʱ��
						it2->second->m_lock.unLock();

						devParam->m_mpActTime[id]->m_lock.lock();
						devParam->m_mpActTime[id]->deqtime.push_back(move(dt));//���붯��ʱ��
						devParam->m_mpActTime[id]->m_lock.unLock();
						
						if (devParam->m_viewId > -1)
							mb->m_pCheck->viewWin(devParam->m_viewId, pmsg->vm);//ʹ���Ƴ̵Ľ��� �����Ҫ��֤�Ƴ̱��п��Ƴ̻����Ƴ̹�����

						mb->pushIoMsg(it->second.x, 1);//����id
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
						int id = it->second.y;//��ʱ��id;
						map<int, S_TimeParam*>::iterator it2 = devParam->m_mpTime.find(id);
						if (devParam->m_mpTime.end() != it2)
						{
							double dt = 0;
							it2->second->m_lock.lock();
							if (it2->second->deqtime.size() > 0)
								dt = it2->second->deqtime.front() + timeDelay;//���붯��ʱ��
							it2->second->m_lock.unLock();

							devParam->m_mpActTime[id]->m_lock.lock();
							devParam->m_mpActTime[id]->deqtime.push_back(move(dt));//���붯��ʱ��
							devParam->m_mpActTime[id]->m_lock.unLock();

							if (devParam->m_viewId > -1)
								mb->m_pCheck->viewWin(devParam->m_viewId, pmsg->vm);//ʹ���Ƴ̵Ľ��� �����Ҫ��֤�Ƴ̱��п��Ƴ̻����Ƴ̹�����

							mb->pushIoMsg(it->second.x, 1);
						}
					}

				}
			}
			devParam->m_mplock.unLock();
			delete pmsg;
			pmsg = NULL;

			devParam->delTimeOnce();

			return ret == 1;//���ɹ�������
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
			if (cont[0] == "cam")//���ռ�� 
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
			else if (cont[0] == "camTrigger")//���մ���
			{
				static CPoint lastPos(0, 0);//�ϴζ�λ����
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
				//Ŀ��λ��ƫ�����ľ�����ƫ������Χ��
				else if (abs(pmsg->px - pmsg->vm[0].cols / 2) > devParam->m_Shift)
				{
					data = "";
					delete pmsg;
					pmsg = NULL;
					lastPos = CPoint(0, 0);
					return false;
				}
				//���������Ƿ�ͼƬ��ͬ ��ͬ����ͣ��
				if (devParam->m_setStopCheck && abs(pmsg->px - lastPos.x) < 20 && abs(pmsg->py - lastPos.y) < 20)
				{
#ifdef nouseEN
					data = "ͣ��״̬";
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
					data = "ͼƬ���п��쳣";
#else
					data = "The image queue is empty";
#endif
					return false;
				}
				data = "";
				CoordinatePosXYZ* pmsg = devParam->m_pic.front();
				devParam->m_pic.pop_front();
				devParam->m_lock.unLock();
				//���ngҪ��֤ʱ�򲻱�  ������������ɶԼ�ʱ����ͬ������
				devParam->m_mplock.lock();
				float timeDelay = devParam->m_timeRelay * (pmsg->px - pmsg->vm[0].cols / 2);
				//��ǰroi
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
						int id = it->second.y;//��ʱ��id
						map<int, S_TimeParam*>::iterator it2 = devParam->m_mpTime.find(id);
						if (devParam->m_mpTime.end() != it2)
						{
							double dt = 0;
							it2->second->m_lock.lock();
							if (it2->second->deqtime.size() > 0)
								dt = it2->second->deqtime.front() + timeDelay;//���붯��ʱ��
							it2->second->m_lock.unLock();

							devParam->m_mpActTime[id]->m_lock.lock();
							devParam->m_mpActTime[id]->deqtime.push_back(move(dt));//���붯��ʱ��
							devParam->m_mpActTime[id]->m_lock.unLock();

							if (devParam->m_viewId > -1)
							{
								mb->m_pCheck->viewWin(devParam->m_viewId, pmsg->vm);//ʹ���Ƴ̵Ľ��� �����Ҫ��֤�Ƴ̱��п��Ƴ̻����Ƴ̹�����
							}
							mb->pushIoMsg(it->second.x, 1);//����id
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
							int id = it->second.y;//��ʱ��id;
							map<int, S_TimeParam*>::iterator it2 = devParam->m_mpTime.find(id);
							if (devParam->m_mpTime.end() != it2)
							{
								double dt = 0;
								it2->second->m_lock.lock();
								if (it2->second->deqtime.size() > 0)
									dt = it2->second->deqtime.front() + timeDelay;//���붯��ʱ��
								it2->second->m_lock.unLock();

								devParam->m_mpActTime[id]->m_lock.lock();
								devParam->m_mpActTime[id]->deqtime.push_back(move(dt));//���붯��ʱ��
								devParam->m_mpActTime[id]->m_lock.unLock();

								if (devParam->m_viewId > -1)
								{
									mb->m_pCheck->viewWin(devParam->m_viewId, pmsg->vm);//ʹ���Ƴ̵Ľ��� �����Ҫ��֤�Ƴ̱��п��Ƴ̻����Ƴ̹�����
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

				return ret == 1;//���ɹ�������
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
