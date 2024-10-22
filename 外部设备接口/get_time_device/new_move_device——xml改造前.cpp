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
	string szHelp = "���Ķ���:setTime,n,t = ���ö�ʱ��n�Ķ�ʱʱ��,insertTime,n = n���뿪ʼʱ��,insertActTime,n = ���붯��ʱ��,insertAllTime=�����������п�ʼʱ��,	\
					runTime,n = ִ�ж���ʱ��,cam = ��ȡͼƬ,camTrigger=���մ���,checkpic = ���ͼƬ,zeropic=���ͼƬ,setNGIO,ng����,ioid,tmid=����ng����ӳ��io�Ͷ�ʱ����\
					setRelay,t=���ö�λƫ�Ʋ�������ʱ��/����,setShift,t=���ö�λƫ�Ʋ���,setSortType,t=0 ng 1 ok����";
	return szHelp;
}

bool MoveDLLIni(void* pmv,void*& p2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (pmv==NULL)
	{
		return false;
	}
	CVisionSystem* mb = (CVisionSystem*)pmv;
	S_DeviceParam* devParam;
	if (p2==NULL)
	{
		devParam = new S_DeviceParam();
		string szDir = mb->m_szCfgAdd;
		int pos = szDir.size()-strlen("MoveCfg");
		szDir = szDir.substr(0,pos);
		devParam->m_cfgAdd = szDir+"timerdllcfg";
		devParam->loadcfg();
		p2 = devParam;
	}
	else
		devParam = new S_DeviceParam();
	
	return true;
}

void MoveDLLStop()
{

}
//��̬������ȡ�ı꺯��
bool MoveDLLMove(void* pmv,void*& p2,string& data)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CVisionSystem* mb = (CVisionSystem*)pmv;
	if (NULL != mb)
	{
		S_DeviceParam* devParam;
		if (p2==NULL)
		{
			devParam = new S_DeviceParam();
			string szDir = mb->m_szCfgAdd;
			int pos = szDir.size()-strlen("MoveCfg");
			szDir = szDir.substr(0,pos);
			devParam->m_cfgAdd = szDir+"timerdllcfg";
			devParam->loadcfg();
			p2 = devParam;
		}
		else
			devParam = (S_DeviceParam*)p2;

		if (data == "cam")//���ռ�� 
		{
			CoordinatePosXYZ* pmsg = new CoordinatePosXYZ();
			if(-1 == mb->m_pCheck->camPic(mb->m_pCheck->m_curCamId,pmsg->m) )
			{
				data="";
				delete pmsg;
				pmsg = NULL;
				return false;
			}
			devParam->m_lock.lock();
			devParam->m_pic.push_back(pmsg);
			devParam->m_lock.unLock();
		}
		else if (data == "camTrigger")//���մ���
		{
			CoordinatePosXYZ* pmsg = new CoordinatePosXYZ();
			if (-1 == mb->m_pCheck->camPic(mb->m_pCheck->m_curCamId, pmsg->m))
			{
				data = "";
				delete pmsg;
				pmsg = NULL;
				return false;
			}
			int ret = mb->m_pCheck->procCheck(0, pmsg->m, pmsg);
			if (ret != 1)
			{
				data = "";
				delete pmsg;
				pmsg = NULL;
				return false;
			}
			//Ŀ��λ��ƫ�����ľ�����ƫ������Χ��
			else if (abs(pmsg->px - pmsg->m.cols / 2) > devParam->m_Shift)
			{
				data = "";
				delete pmsg;
				pmsg = NULL;
				return false;
			}
			devParam->m_lock.lock();
			devParam->m_pic.push_back(pmsg);
			devParam->m_lock.unLock();
		}
		else if (data == "checkpic")
		{
			data="";
			devParam->m_lock.lock();
			if (devParam->m_pic.size() == 0)
			{
				devParam->m_lock.unLock();
				devParam->delTimeOnce();
				return false;
			}

			CoordinatePosXYZ* pmsg = devParam->m_pic.front();
			devParam->m_pic.pop_front();
			devParam->m_lock.unLock();
			//���ngҪ��֤ʱ�򲻱�  ������������ɶԼ�ʱ����ͬ������
			cv::Mat m = pmsg->m.clone();
			devParam->m_mplock.lock();
			float timeDelay = devParam->m_timeRelay*(pmsg->px-m.cols/2);
			//��ǰroi
			if (timeDelay != 0)
			{
				cv::Rect roi = cv::Rect(pmsg->px - pmsg->z / 2 - 200, pmsg->py - pmsg->r / 2 - 150, pmsg->z + 300, pmsg->r + 300);
				roi &= cv::Rect(0, 0, m.cols, m.rows);
				pmsg->m = cv::Mat(pmsg->m, roi);
			}

			int ret = mb->m_pCheck->procCheck(mb->m_pCheck->m_picProcId,pmsg->m,pmsg);
			if (NULL != mb->m_tbReport)
				mb->m_tbReport->pushMsg(pmsg, m);
			string szSortRet = pmsg->str;
			delete pmsg;
			pmsg = NULL;
			if (ret!=1 || devParam->m_szSortType== "@OK@ ")
			{	
				//����
				if (szSortRet.size()>6)
				{
					vector<string>  o;
					string szRet = szSortRet.substr(6,szSortRet.size()-6);
					CStrProcess::Split(szRet,",",o);
					if(o.size()>0)
					{
						int i=0;
						for (;i<o.size();i++)
						{
							vector<string>  o2;
							CStrProcess::Split(o[i],devParam->m_szSortType,o2);
							if (o2.size()>1 )
							{
								string szNgType = o2[0];
								map<string,cv::Point>::iterator it = devParam->m_mpNgToIo.find(szNgType);
								if (it!=devParam->m_mpNgToIo.end())
								{
									int id = it->second.y;//��ʱ��id
									map<int,S_TimeParam*>::iterator it2 = devParam->m_mpTime.find(id);
									if(devParam->m_mpTime.end() != it2)
									{
										double dt=0;
										it2->second->m_lock.lock();
										if(it2->second->deqtime.size()>0)
											dt = it2->second->deqtime.front()+timeDelay;//���붯��ʱ��
										it2->second->m_lock.unLock();

										devParam->m_mpActTime[id]->m_lock.lock();
										devParam->m_mpActTime[id]->deqtime.push_back(dt);//���붯��ʱ��
										devParam->m_mpActTime[id]->m_lock.unLock();

										mb->pushIoMsg(it->second.x,1);//����id
									}
									else
										data = "error";
									break;
								}
							}	
						}
						if (i==o.size())
						{
							data = "error";
						}
					}
					else
					{
						data = "error";
					}
				}
				else
				{
					data = "error";
				}
				if ("error" == data)
				{
					
					int p = devParam->m_mpNgToIo.size()-1;
					
					if (p>-1)
					{
						map<string,cv::Point>::iterator it = devParam->m_mpNgToIo.end();
						it--;
						int id =it->second.y;//��ʱ��id;
						map<int,S_TimeParam*>::iterator it2 = devParam->m_mpTime.find(id);
						if(devParam->m_mpTime.end() != it2)
						{
							double dt=0;
							it2->second->m_lock.lock();
							if(it2->second->deqtime.size()>0)
								dt = it2->second->deqtime.front()+timeDelay;//���붯��ʱ��
							it2->second->m_lock.unLock();

							devParam->m_mpActTime[id]->m_lock.lock();
							devParam->m_mpActTime[id]->deqtime.push_back(dt);//���붯��ʱ��
							devParam->m_mpActTime[id]->m_lock.unLock();
							mb->pushIoMsg(it->second.x,1);
						}
					}
					
				}
			}
			devParam->m_mplock.unLock();

			devParam->delTimeOnce();
			
			return ret==1;//���ɹ�������
		}
		else if (data == "zeropic")
		{
			char sz[128];
			sprintf(sz,"picnum=%d",devParam->m_pic.size());
			data=sz;
			devParam->zeropic();
		}
		else if(data == "insertAllTime")
		{
			int depNum = devParam->m_mpTime.size();
			if(depNum > 0)
			{
				for (int c=1;c<depNum;c++)
				{
					devParam->m_mpTime[c]->m_lock.lock();
					devParam->m_mpTime[c]->deqtime.push_back(clock());
					devParam->m_mpTime[c]->m_lock.unLock();
				}
				devParam->m_mpActTime[0]->m_lock.lock();
				devParam->m_mpActTime[0]->deqtime.push_back(clock());
				devParam->m_mpActTime[0]->m_lock.unLock();
			}
		}
		else
		{
			vector<string > cont;
			CStrProcess::Split(data,",",cont,true);

			if (cont.size() < 2)
			{
				data = "error format:" + data;
				return false;
			}
			int id = atoi(cont[1].c_str());
			if (cont[0] == "setTime")//
			{
				if (cont.size() < 3)
				{
					data = "error format:" + data;
					return false;
				}
				map<int,S_TimeParam*>::iterator itor = devParam->m_mpTime.find(id);
				if (itor == devParam->m_mpTime.end())
				{
					devParam->m_mpTime.insert(pair<int,S_TimeParam*>(id,new S_TimeParam()));
				}
				
				devParam->m_mpTime[id]->m_lock.lock();
				devParam->m_mpTime[id]->settime = atoi(cont[2].c_str());
				char sz[128];
				sprintf(sz,"id,size,settime=%d,%d,%f",id,devParam->m_mpTime[id]->deqtime.size(),devParam->m_mpTime[id]->settime);
				data=sz;
				devParam->m_mpTime[id]->deqtime.clear();
				devParam->m_mpTime[id]->m_lock.unLock();
				
				map<int,S_TimeParam*>::iterator itor2 = devParam->m_mpActTime.find(id);
				if (itor2 == devParam->m_mpActTime.end())
				{
					devParam->m_mpActTime.insert(pair<int,S_TimeParam*>(id,new S_TimeParam()));
				}
				devParam->m_mpActTime[id]->m_lock.lock();
				devParam->m_mpActTime[id]->settime = atoi(cont[2].c_str());
				devParam->m_mpActTime[id]->deqtime.clear();
				devParam->m_mpActTime[id]->m_lock.unLock();
				return true;
			}
			else if(cont[0] == "insertTime")
			{
				map<int,S_TimeParam*>::iterator it = devParam->m_mpTime.find(id);
				if(devParam->m_mpTime.end() != it)
				{
					it->second->m_lock.lock();
					it->second->deqtime.push_back(clock());
					it->second->m_lock.unLock();
				}
			}
			else if(cont[0] == "insertActTime")
			{
				map<int,S_TimeParam*>::iterator it = devParam->m_mpActTime.find(id);
				if(devParam->m_mpActTime.end() != it)
				{
					it->second->m_lock.lock();
					it->second->deqtime.push_back(clock());
					it->second->m_lock.unLock();
				}
			}
			/*else if(cont[0] == "delTime")
			{
				devParam->m_mpTime[id]->m_lock.lock();
				devParam->m_mpTime[id]->deqtime.pop_front();
				devParam->m_mpTime[id]->m_lock.unLock();
			}*/
			else if(cont[0] == "runTime")
			{
				map<int,S_TimeParam*>::iterator it = devParam->m_mpActTime.find(id);
				if(devParam->m_mpActTime.end() != it)
				{
					it->second->m_lock.lock();
					if (it->second->deqtime.size()==0)
					{
						data="";
						it->second->m_lock.unLock();
						return false;
					}
					double bt = it->second->deqtime.front();
					it->second->deqtime.pop_front();
					double dinv = it->second->settime-(clock()-bt);
					it->second->m_lock.unLock();
					if (dinv>0)
					{
						Sleep(dinv);
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
				devParam->m_mpNgToIo[cont[1]]=cv::Point(atoi(cont[2].c_str()),atoi(cont[3].c_str()));
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
				if (id)
					devParam->m_szSortType = "@OK@ ";
				else
					devParam->m_szSortType = "@NG@ ";
				devParam->m_mplock.unLock();
			}
			else
			{
				data = "error format:" + data;
				return false;
			}
		}
	}
	data="";
	return true;
}

//������ʾ�ĵ����ͷ��ڴ�
void MoveDllRelease(void*& p2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (NULL!=p2)
	{
		delete (S_DeviceParam*)p2;
		p2=NULL;
	}
}

void MoveDllWriteFile()
{
	
}


