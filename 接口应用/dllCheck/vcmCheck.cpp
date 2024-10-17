// vcmCheck.cpp : 定义 DLL 应用程序的导出函数。
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
		checkWinName = "win_opencv";//VIEWWINNAME;		//默认的显示窗体名称
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
	vector< MultiThreadCheck*> m_mcheck;	//多线程制程
	string checkWinName;//VIEWWINNAME;		//默认的显示窗体名称
	vector< mouseKeyCtr*> m_vctMkc;			//实时制程
};

CThreadMgr g_threadMgr;

//制程i多线程检测
extern "C" TOTLIB_API int procCheckwait(CoordinatePosXYZ *pd,void*& p );
//设置显示窗体名称
extern "C" TOTLIB_API void setViewName(string winname,void*& p );
//制程i多线程检测后的数据提取
extern "C" TOTLIB_API int waitForRet(int i, S_Msg*& ret ,void*& p);
//制程i的实时检测配置加载
extern "C" TOTLIB_API bool loadprocCheck(int i, string  m_dir,void*& p);
//制程i的多线程检测配置加载
extern "C" TOTLIB_API bool loadprocTrdCheck(int i, string  dir,void*& p);
//制程i的检测 实时返回结果 -1未定位 0定位成功 1检测ok
extern "C" TOTLIB_API int procCheckN(int i, vector<cv::Mat>&vm,void*& p);
//检测ok才返回定位结果和坐标位置 
extern "C" TOTLIB_API int procCheck(CoordinatePosXYZ *pd ,void*& p );
//鼠标事件响应
extern "C" TOTLIB_API void onMouseN(int i,int event, int x, int y,void*& p);
//显示图片
extern "C" TOTLIB_API void viewWin(int i, vector<cv::Mat>&vm,void*& p);
//返回制程n的定位目标
extern "C" TOTLIB_API cv::RotatedRect getPos(int n,void*& p);
//退出dll的显式调用
extern "C" TOTLIB_API void ExitDll(void*& p);
//提取实时分析的类
extern "C" TOTLIB_API mouseKeyCtr* getProc(int i,void*& p);

//动态库中提取的标函数
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

// 立刻返回结果
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
		//显示图片
		((S_CheckData*)p)->m_vctMkc[i]->viewMainWin(vm);
	}
	catch(...)
	{
		return ret;
	}
	return ret;
}

// 立刻返回结果 检测ok才返回定位结果
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
	//显示图片
	((S_CheckData*)p)->m_vctMkc[i]->viewMainWin(pd->vm);

	if( ret>0 )//检测ok
	{
		((S_CheckData*)p)->m_vctMkc[i]->m_curProc.getPosMove(*pd);//提取结果
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
			((S_CheckData*)p)->m_vctMkc[i]->m_curProc.getPosMove(*pd);//提取结果
			if (((S_CheckData*)p)->m_vctMkc[i]->m_curProc.m_unit > 0)
			{
				pd->x = pd->px*((S_CheckData*)p)->m_vctMkc[i]->m_curProc.m_unit;
				pd->y = pd->py*((S_CheckData*)p)->m_vctMkc[i]->m_curProc.m_unit;
			}
			pd->m_nc_ok = 2;
			//返回ng的位置和别名
			//string szret = g_vctMkc[i]->m_curProc.getNGInfo();
		}
		else//=-1 定位失败
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

//丢入数据立刻返回
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

//等待返回结果
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
				//显示图片
				((S_CheckData*)p)->m_vctMkc[i]->viewMainWin(ret->vm);
			}

			return 1;
		}
		else if (-2 == r)//-2表示没有数据了
		{
			return 0;
		}
	}
	
}

//  加载制程
bool loadprocCheck(int i, string  dir,void*& p)
{
	if (NULL == p)
	{
		p = new S_CheckData();
	}
	bool ret = false;
	try
	{
		int len = ((S_CheckData*)p)->m_vctMkc.size();//先实时加载 后多线程加载 这样防止多用户同时访问halcon资源报错
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
		//然后再多线程加载
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

