// vcmCheck.cpp : 定义 DLL 应用程序的导出函数。
//



#include "mouseCtr.h"

#if (defined WIN32 || defined _WIN32 || defined WINCE)
#ifdef TOTLIB_EXPORTS
#define TOTLIB_API __declspec(dllexport)
#else
#define TOTLIB_API __declspec(dllimport)
#endif

#else
#define TOTLIB_API 
#endif

#define _CHECKBUG
vector< MultiThreadCheck*> g_mcheck;	//多线程制程
string checkWinName = "win_opencv";//VIEWWINNAME;		//默认的显示窗体名称
vector< mouseKeyCtr*> g_vctMkc;			//实时制程
vector< string > g_wlog;				//实时制程保存错误图片
CResLock g_Lock;//主要针对实时制程
CThreadMgr g_threadMgr;
//相机i拍照
extern "C" TOTLIB_API int camPic(int i,cv::Mat& m);
extern "C" TOTLIB_API int camPic2(string camName,cv::Mat& m);
//制程i多线程检测
extern "C" TOTLIB_API int procCheckwait(int i, cv::Mat m, CoordinatePosXYZ *pd );
//设置显示窗体名称
extern "C" TOTLIB_API void setViewName(string winname );
//制程i多线程检测后的数据提取
extern "C" TOTLIB_API int waitForRet(int i, CoordinatePosXYZ *pd );
//制程i的实时检测配置加载
extern "C" TOTLIB_API bool loadprocCheck(int i, string  m_dir);
//制程i的多线程检测配置加载
extern "C" TOTLIB_API bool loadprocTrdCheck(int i, string  dir);
//制程i的检测 实时返回结果 -1未定位 0定位成功 1检测ok
extern "C" TOTLIB_API int procCheckN(int i,cv::Mat& m);
//检测ok才返回定位结果和坐标位置 
extern "C" TOTLIB_API int procCheck(int i, cv::Mat m ,CoordinatePosXYZ *pd  );
//鼠标事件响应
extern "C" TOTLIB_API void onMouseN(int i,int event, int x, int y);
//显示图片
extern "C" TOTLIB_API void viewWin(int i,cv::Mat& m);
//返回制程n的定位目标
extern "C" TOTLIB_API cv::RotatedRect getPos(int n);
//退出dll的显式调用
extern "C" TOTLIB_API void ExitDll();

int camPic(int i,cv::Mat& m)
{
	return -1;
}

int camPic2(string camName,cv::Mat& m)
{
	return -1;
}
//动态库中提取的标函数
void ExitDll()
{
	g_Lock.lock();
	if (g_mcheck.size()>0)
	{
		vector<MultiThreadCheck*>::iterator it = g_mcheck.begin();
		for (; it != g_mcheck.end(); ++it)
		{
			delete (*it);
		}
		g_mcheck.clear();
	}
	
	if (g_vctMkc.size()>0)
	{
		vector<mouseKeyCtr*>::iterator it3 = g_vctMkc.begin();
		for (; it3 != g_vctMkc.end(); ++it3)
		{
			delete (*it3);
		}
		g_vctMkc.clear();
	}
	

	g_totLog.m_bStop = true;
	for(int t=0;t<500;t++)
	{
		if(g_totLog.m_runing)
			cv::waitKey(10);
		else
			break;
	}
	g_totLog.stop();
	g_Lock.unLock();
	cv::destroyAllWindows();

}
// 立刻返回结果
int procCheckN(int i,cv::Mat& m)
{
	if(i>=g_vctMkc.size())
	{
		return -1;
	}
	if (m.empty())
	{
#ifdef _CHECKBUG
		S_LogInfo* logInfo = new S_LogInfo();
		logInfo->id = 0;
		logInfo->filename = g_vctMkc[i]->m_curProc.m_procDir+LOGDIR+"vcmInterfaceLog.txt";
		GDKT::CDateTime dt;
		logInfo->content = dt.ToString()+"procCheck no picdata ";
		g_totLog.pushMsg(logInfo);
#endif
		return -1;
	}
	cv::Mat m1=m.clone();
	g_Lock.lock();
	int ret = -1;
	try{
		ret = g_vctMkc[i]->m_curProc.parallelMatchTplt(m1);//g_vctMkc[i]->m_curProc.matchTplt2(m1);//
		//显示图片
		g_vctMkc[i]->viewMainWin(m1);

		static unsigned char s_i=0;
		if (ret>0)
			;
		else
		{
			if (g_wlog[i] != "")
			{
				S_LogInfo* logInfo = new S_LogInfo();
				logInfo->id = 1;
				logInfo->filename = g_vctMkc[i]->m_curProc.m_procDir+ERRORPICFILE+"mainView"+CStrProcess::itoa_radixN(s_i++) + ".jpg";
				//logInfo->pic = m.clone();
				cv::resize(m1,logInfo->pic,cv::Size(0.5*m.cols,0.5*m.rows));
				g_totLog.pushMsg(logInfo);
			}
		}
	}
	catch(...)
	{
		g_Lock.unLock();
		return ret;
	}
	g_Lock.unLock();
	return ret;
}

// 立刻返回结果 检测ok才返回定位结果
int procCheck(int i, cv::Mat m ,CoordinatePosXYZ *pd  )
{
	if(i>=g_vctMkc.size())
	{
		pd->m_nc_ok = 4;
		return -1;
	}
	if (m.empty())
	{
#ifdef _CHECKBUG
		S_LogInfo* logInfo = new S_LogInfo();
		logInfo->id = 0;
		logInfo->filename = g_vctMkc[i]->m_curProc.m_procDir+LOGDIR+"vcmInterfaceLog.txt";
		GDKT::CDateTime dt;
		logInfo->content = dt.ToString() + " procCheck no picdata ";
		g_totLog.pushMsg(logInfo);
#endif
		pd->m_nc_ok = 4;
		return -1;
	}
#ifdef _CHECKBUG1
	{
		S_LogInfo* logInfo = new S_LogInfo();
		logInfo->id = 0;
		logInfo->filename = g_vctMkc[i]->m_curProc.m_procDir+LOGDIR+"vcmInterfaceLog.txt";
		GDKT::CDateTime dt;
		logInfo->content = dt.ToString()+"procCheck NO is "+ CStrProcess::itoa_radixN(i)+ " enter ";
		g_totLog.pushMsg(logInfo);
	}

#endif
	cv::Mat m1=m.clone();
	g_Lock.lock();
	int ret = -1;
	try{
	ret = g_vctMkc[i]->m_curProc.parallelMatchTplt(m1);//g_vctMkc[i]->m_curProc.matchTplt2(m1);//
	//显示图片
	g_vctMkc[i]->viewMainWin(m1);
	if( ret>0 )//检测ok
	{
		g_vctMkc[i]->m_curProc.getPosMove(pd->px,pd->py,pd->f);//提取结果
		pd->m_nc_ok = 3;
	}
	else
	{
		if (ret > -1)
		{
			g_vctMkc[i]->m_curProc.getPosMove(pd->px,pd->py,pd->f);//提取结果
			pd->m_nc_ok = 2;
			//返回ng的位置和别名
			//string szret = g_vctMkc[i]->m_curProc.getNGInfo();
		}
		else//=-1 定位失败
			pd->m_nc_ok = 4;

		if (g_wlog[i] != "")
		{
			static unsigned char s_i=0;
			if (pd->m_nc_ok == 3)
				;
			else
			{
				S_LogInfo* logInfo = new S_LogInfo();
				logInfo->id = 1;
				logInfo->filename = g_vctMkc[i]->m_curProc.m_procDir+ERRORPICFILE+ CStrProcess::itoa_radixN(s_i++) +".jpg";
				//logInfo->pic = m.clone();
				cv::resize(m1,logInfo->pic,cv::Size(0.5*m.cols,0.5*m.rows));
				g_totLog.pushMsg(logInfo);
			}
		}
	}
	}
	catch(...)
	{
		g_Lock.unLock();
		return ret;
	}
	g_Lock.unLock();

#ifdef _CHECKBUG1
	{
		S_LogInfo* logInfo = new S_LogInfo();
		logInfo->id = 0;
		logInfo->filename = g_vctMkc[i]->m_curProc.m_procDir+LOGDIR+"vcmInterfaceLog.txt";
		GDKT::CDateTime dt;
		logInfo->content = dt.ToString()+"procCheck NO is "+ CStrProcess::itoa_radixN(i)+ " out ";
		g_totLog.pushMsg(logInfo);
	}
	
#endif

	return ret;
}

cv::RotatedRect getPos(int n)
{
	g_Lock.lock();
	cv::RotatedRect ret = g_vctMkc[n]->m_curProc.getMainRect();
	g_Lock.unLock();
	return ret;
}

//丢入数据立刻返回
int procCheckwait(int i, cv::Mat m, CoordinatePosXYZ *pd  )
{
//	pd->printData();
	if(i>=g_mcheck.size())
	{
		return -1;
	}
	if (m.empty())
	{
#ifdef _CHECKBUG
		S_LogInfo* logInfo = new S_LogInfo();
		logInfo->id = 0;
		logInfo->filename = g_vctMkc[i]->m_curProc.m_procDir+LOGDIR+"vcmInterfaceLog.txt";
		GDKT::CDateTime dt;
		logInfo->content = dt.ToString() + " procCheckwait no picdata ";
		g_totLog.pushMsg(logInfo);
#endif
		return -1;
	}
	int ret = g_mcheck[i]->pushImage(m,*pd);

	return ret;
}

void setViewName(string winname )
{
	checkWinName = winname;
	int len = g_vctMkc.size();
	for (int k=0;k<len;k++)
	{
		g_vctMkc[k]->m_mainWinName = checkWinName+CStrProcess::itoa_radixN(k+1);
	}
}

//等待返回结果
int waitForRet(int i, CoordinatePosXYZ *pd )
{
	if(i>=g_mcheck.size())
	{
		return 0;
	}

	S_Msg* ret;
	while (1)
	{
		int r = g_mcheck[i]->getRet(ret);
		if (1 == r)
		{
			*pd = ret->ret;
			if (ret->pic.data != NULL)
			{
				//显示图片
				g_vctMkc[i]->viewMainWin(ret->pic);
				pd->m = ret->pic.clone();
			}
			/*if(ret->ret.m_nc_ok==3)
			{
				viewTxt("OK");
			}
			else
				viewTxt("NG");*/
			delete ret;
			return 1;
		}
		else if (-2 == r)//-2表示没有数据了
		{
			return 0;
		}
	}
}

//  加载制程
bool loadprocCheck(int i, string  dir)
{
	g_Lock.lock();
	bool ret = false;
	try
	{
		int len = g_vctMkc.size();//先实时加载 后多线程加载 这样防止多用户同时访问halcon资源报错
		if(i>=len)
		{
			for (int k=i;k>=len;k--)
			{
				mouseKeyCtr *mkc = new mouseKeyCtr();
				mkc->m_mainWinName = checkWinName+CStrProcess::itoa_radixN(k+1);
				g_vctMkc.push_back(mkc);
				g_wlog.push_back("");
			}
		}
		ret = g_vctMkc[i]->m_curProc.loadTplt(dir+PROCFILE);
		//然后再多线程加载
		/*len = g_mcheck.size();
		if(i>=len)
		{
			for (int k=i;k>=len;k--)
			{
				MultiThreadCheck* mcheck = new MultiThreadCheck();	
				g_mcheck.push_back(mcheck);
			}
		}
		ret = ret && g_mcheck[i]->loadCfg(dir+PROCFILE) ;*/

		CConfig cfg(g_vctMkc[i]->m_curProc.m_procDir+PROCFILE);
		string errpic = cfg.getItem("errorfile");
		if (errpic != "")
		{
			errpic = g_vctMkc[i]->m_curProc.m_procDir+errpic;
		}
		g_wlog[i] = errpic;

	}
	catch (...)
	{
		g_Lock.unLock();
		return ret;
	}
	
	g_Lock.unLock();
	return ret;
}

bool loadprocTrdCheck(int i, string  dir)
{
	g_Lock.lock();
	bool ret = false;
	try
	{
		//多线程加载
		int len = g_mcheck.size();
		if(i>=len)
		{
			for (int k=i;k>=len;k--)
			{
				MultiThreadCheck* mcheck = new MultiThreadCheck();	
				g_mcheck.push_back(mcheck);
			}
		}
		ret = g_mcheck[i]->loadCfg(dir+PROCFILE) ;

	}
	catch (...)
	{
		g_Lock.unLock();
		return ret;
	}
	
	g_Lock.unLock();
	return ret;
}

void onMouseN(int i,int event, int x, int y)
{
	if(i>=g_vctMkc.size())
	{
		return;
	}
	g_vctMkc[i]->m_bScaleState = true;
	g_vctMkc[i]->onMouse( event,  x,  y);
}

void viewWin(int i,cv::Mat& m)
{
	if(i>=g_vctMkc.size())
	{
		return;
	}
	g_vctMkc[i]->viewMainWin(m);
}
