// vcmCheck.cpp : ���� DLL Ӧ�ó���ĵ���������
//


#include "pwwVision.h"
#pragma comment(lib, "dog_windows_x64_3159259.lib")
#if (defined WIN32 || defined _WIN32 || defined WINCE)
#ifdef TOTLIB_EXPORTS
#define TOTLIB_API __declspec(dllexport)
#else
#define TOTLIB_API __declspec(dllimport)
#endif

#else
#define TOTLIB_API 
#endif

struct S_CheckData:string
{
	S_CheckData()
	{
		checkWinName = "win_opencv";//VIEWWINNAME;		//Ĭ�ϵ���ʾ��������
	}
	~S_CheckData()
	{
		if (m_mcheck.size()>0)
		{
			vector<MultiThreadCheck*>::iterator it = m_mcheck.begin();
			for (; it != m_mcheck.end(); ++it)
			{
				delete (*it);
			}
			m_mcheck.clear();
		}

		if (m_vctMkc.size()>0)
		{
			vector<mouseKeyCtr*>::iterator it3 = m_vctMkc.begin();
			for (; it3 != m_vctMkc.end(); ++it3)
			{
				delete (*it3);
			}
			m_vctMkc.clear();
		}
	}
	vector< MultiThreadCheck*> m_mcheck;	//���߳��Ƴ�
	string checkWinName;//VIEWWINNAME;		//Ĭ�ϵ���ʾ��������
	vector< mouseKeyCtr*> m_vctMkc;			//ʵʱ�Ƴ�
};

CThreadMgr g_threadMgr;

//�Ƴ�i���̼߳��
extern "C" TOTLIB_API int procCheckwait(CoordinatePosXYZ *pd,void*& p );
//������ʾ��������
extern "C" TOTLIB_API void setViewName(string winname,void*& p );
//�Ƴ�i���̼߳����������ȡ
extern "C" TOTLIB_API int waitForRet(int i, S_Msg*& ret ,void*& p);
//�Ƴ�i��ʵʱ������ü���
extern "C" TOTLIB_API bool loadprocCheck(int i, string  m_dir,void*& p);
//�Ƴ�i�Ķ��̼߳�����ü���
extern "C" TOTLIB_API bool loadprocTrdCheck(int i, string  dir,void*& p);
//�Ƴ�i�ļ�� ʵʱ���ؽ�� -1δ��λ 0��λ�ɹ� 1���ok
extern "C" TOTLIB_API int procCheckN(int i, vector<cv::Mat>&vm,void*& p);
//���ok�ŷ��ض�λ���������λ�� 
extern "C" TOTLIB_API int procCheck(CoordinatePosXYZ *pd ,void*& p );
//����¼���Ӧ
extern "C" TOTLIB_API void onMouseN(int i,int event, int x, int y,void*& p);
//��ʾͼƬ
extern "C" TOTLIB_API void viewWin(int i, vector<cv::Mat>&vm,void*& p);
//�����Ƴ�n�Ķ�λĿ��
extern "C" TOTLIB_API cv::RotatedRect getPos(int n,void*& p);
//�˳�dll����ʽ����
extern "C" TOTLIB_API void ExitDll(void*& p);
//��ȡʵʱ��������
extern "C" TOTLIB_API mouseKeyCtr* getProc(int i,void*& p);

//��̬������ȡ�ı꺯��
void ExitDll(void*& p)
{
	if (p==NULL)
	{
		
	}
	else
	{
		delete ((S_CheckData*)p);
		p = NULL;
	}

	cv::destroyAllWindows();
}

// ���̷��ؽ��
int procCheckN(int i, vector<cv::Mat>& vm,void*& p)
{
	if (p==NULL)
	{
		return -1;
	}
	if(i>=((S_CheckData*)p)->m_vctMkc.size())
	{
		return -1;
	}
	if (vm[0].empty())
	{
		return -1;
	}
	int ret = -1;
	try{
		ret = ((S_CheckData*)p)->m_vctMkc[i]->m_curProc.parallelMatchTplt(vm);//g_vctMkc[i]->m_curProc.matchTplt2(m1);//
		//��ʾͼƬ
		((S_CheckData*)p)->m_vctMkc[i]->viewMainWin(vm);
	}
	catch(...)
	{
		return ret;
	}
	return ret;
}

// ���̷��ؽ�� ���ok�ŷ��ض�λ���
int procCheck(CoordinatePosXYZ *pd ,void*& p )
{
	if (p==NULL)
	{
		pd->m_nc_ok = 4;
		return -1;
	}
	int i = pd->m_procid;
	if(i>=((S_CheckData*)p)->m_vctMkc.size())
	{
		pd->m_nc_ok = 4;
		return -1;
	}
	if (pd->vm[0].empty())
	{
		pd->m_nc_ok = 4;
		return -1;
	}
	int ret = -1;
	try{
		pd->raw = pd->vm[0].clone();
	ret = ((S_CheckData*)p)->m_vctMkc[i]->m_curProc.parallelMatchTplt(pd->vm);//g_vctMkc[i]->m_curProc.matchTplt2(m1);//
	pd->str = ((S_CheckData*)p)->m_vctMkc[i]->m_curProc.m_szResult;
	//��ʾͼƬ
	((S_CheckData*)p)->m_vctMkc[i]->viewMainWin(pd->vm);

	if( ret>0 )//���ok
	{
		((S_CheckData*)p)->m_vctMkc[i]->m_curProc.getPosMove(*pd);//��ȡ���
		if (((S_CheckData*)p)->m_vctMkc[i]->m_curProc.m_unit > 0)
		{
			pd->x = pd->px*((S_CheckData*)p)->m_vctMkc[i]->m_curProc.m_unit;
			pd->y = pd->py*((S_CheckData*)p)->m_vctMkc[i]->m_curProc.m_unit;
		}
		pd->m_nc_ok = 3;
	}
	else
	{
		if (ret > -1)
		{
			((S_CheckData*)p)->m_vctMkc[i]->m_curProc.getPosMove(*pd);//��ȡ���
			if (((S_CheckData*)p)->m_vctMkc[i]->m_curProc.m_unit > 0)
			{
				pd->x = pd->px*((S_CheckData*)p)->m_vctMkc[i]->m_curProc.m_unit;
				pd->y = pd->py*((S_CheckData*)p)->m_vctMkc[i]->m_curProc.m_unit;
			}
			pd->m_nc_ok = 2;
			//����ng��λ�úͱ���
			//string szret = g_vctMkc[i]->m_curProc.getNGInfo();
		}
		else//=-1 ��λʧ��
			pd->m_nc_ok = 4;
	}
	}
	catch(...)
	{
		return ret;
	}

	return ret;
}

cv::RotatedRect getPos(int n,void*& p)
{
	if (p==NULL)
	{
		return cv::RotatedRect();
	}
	return ((S_CheckData*)p)->m_vctMkc[n]->m_curProc.getMainRect();
}

//�����������̷���
int procCheckwait(CoordinatePosXYZ *pd ,void*& p )
{
	if (p==NULL)
	{
		return -1;
	}
	int i = pd->m_procid;
	if(i>=((S_CheckData*)p)->m_mcheck.size())
	{
		return -1;
	}
	if (pd->vm[0].empty())
	{
		return -1;
	}
	pd->raw = pd->vm[0].clone();
	return ((S_CheckData*)p)->m_mcheck[i]->pushImage(*pd);
	
}

void setViewName(string winname,void*& p )
{
	if (p==NULL)
	{
		p = new S_CheckData();
	}
	int len = ((S_CheckData*)p)->m_vctMkc.size();
	if (winname=="")
	{
		((S_CheckData*)p)->checkWinName = "";
		for (int k=0;k<len;k++)
		{
			((S_CheckData*)p)->m_vctMkc[k]->m_mainWinName = "";// "win_opencv";
		}
	}
	else
	{	
		((S_CheckData*)p)->checkWinName = winname;
		for (int k=0;k<len;k++)
		{
			((S_CheckData*)p)->m_vctMkc[k]->m_mainWinName = ((S_CheckData*)p)->checkWinName+CStrProcess::itoa_radixN(k+1);
		}
	}
}

//�ȴ����ؽ��
int waitForRet(int i, S_Msg*& ret,void*& p )
{
	if (p==NULL)
	{
		return 0;
	}
	if(i>=((S_CheckData*)p)->m_mcheck.size())
	{
		return 0;
	}

	while (1)
	{
		int r = ((S_CheckData*)p)->m_mcheck[i]->getRet(ret);
		if (1 == r)
		{
			if (ret->vm[0].data != NULL)
			{
				//��ʾͼƬ
				((S_CheckData*)p)->m_vctMkc[i]->viewMainWin(ret->vm);
			}

			return 1;
		}
		else if (-2 == r)//-2��ʾû��������
		{
			return 0;
		}
	}
	
}

//  �����Ƴ�
bool loadprocCheck(int i, string  dir,void*& p)
{
	if (NULL == p)
	{
		p = new S_CheckData();
	}
	bool ret = false;
	try
	{
		int len = ((S_CheckData*)p)->m_vctMkc.size();//��ʵʱ���� ����̼߳��� ������ֹ���û�ͬʱ����halcon��Դ����
		if(i>=len)
		{
			for (int k=i;k>=len;k--)
			{
				mouseKeyCtr *mkc = new mouseKeyCtr();
				if (((S_CheckData*)p)->checkWinName == "")
				{
					mkc->m_mainWinName = "win_opencv";
				}
				else
					mkc->m_mainWinName = ((S_CheckData*)p)->checkWinName+CStrProcess::itoa_radixN(k+1);
				((S_CheckData*)p)->m_vctMkc.push_back(mkc);
			}
		}
		ret = ((S_CheckData*)p)->m_vctMkc[i]->m_curProc.loadTplt(dir+PROCFILE);
		//Ȼ���ٶ��̼߳���
		len = ((S_CheckData*)p)->m_mcheck.size();
		if(i>=len)
		{
			for (int k=i;k>=len;k--)
			{
				MultiThreadCheck* mcheck = new MultiThreadCheck();	
				((S_CheckData*)p)->m_mcheck.push_back(mcheck);
			}
		}
		ret = ((S_CheckData*)p)->m_mcheck[i]->loadCfg(dir+PROCFILE) && ret ;

	}
	catch (...)
	{
		return ret;
	}
	
	return ret;
}

void onMouseN(int i,int event, int x, int y,void*& p)
{
	if (p==NULL)
	{
		return;
	}
	if (event == CV_EVENT_MBUTTONDOWN)
	{
		*(string*)p = g_threadMgr.checkThread();
		return;
	}
	if(i>=((S_CheckData*)p)->m_vctMkc.size())
	{
		return;
	}
	
	((S_CheckData*)p)->m_vctMkc[i]->m_bScaleState = true;
	((S_CheckData*)p)->m_vctMkc[i]->onMouse( event,  x,  y);
}

void viewWin(int i, vector<cv::Mat>& vm,void*& p)
{
	if (p==NULL)
	{
		return;
	}
	if(i>=((S_CheckData*)p)->m_vctMkc.size())
	{
		return;
	}
	
	((S_CheckData*)p)->m_vctMkc[i]->viewMainWin(vm);
}

mouseKeyCtr* getProc(int i,void*& p)
{
	if (p==NULL)
	{
		return NULL;
	}
	if(i>=((S_CheckData*)p)->m_vctMkc.size())
	{
		return NULL;
	}
	else
		return ((S_CheckData*)p)->m_vctMkc[i];
}

