

#include "automove.h"
#include <HalconCpp.h>
using namespace HalconCpp;
#pragma comment( lib, "../../src\\halcon\\Lib\\x64-win64\\HalconCpp.lib")

//运动界面的响应函数  可以弹出制程对话框
#define MVSHARPWIN(funname) bool funname(string* str) { \
	if(g_MB->getRuningState())					\
		return 0;				\
	printf("%s",(*str).c_str());				\
	return 1;}

//运动界面
MVSHARPWIN(onMvSharpWin1)
MVSHARPWIN(onMvSharpWin2)
MVSHARPWIN(onMvSharpWin3)
MVSHARPWIN(onMvSharpWin4)
MVSHARPWIN(onMvSharpWin5)
MVSHARPWIN(onMvSharpWin6)
MVSHARPWIN(onMvSharpWin7)
MVSHARPWIN(onMvSharpWin8)

void ioCallBack4(int procId, int id, string str, void* p)
{
	if (g_MB->m_bStop)
		return;
	g_MB->m_demoCtrl.setMvState(procId, id);
}


void reloadMVCfg(string dir)
{
	g_MB->Mvstop();
	string file = dir + "/MoveCfg";
	g_MB->reload(file);
	g_MB->analyseArrayMV();
	string command = g_MB->m_mtc.m_szArrayStr;
	g_MB->m_mtc.analyseVec(command);
}

void test1()
{
	if (NULL == g_MB)
	{
		g_MB = new CVisionSystem();
	}


	std::vector<sharpFun> m_vctFun;
	sharpFun fun[] = { onMvSharpWin1,onMvSharpWin2,onMvSharpWin3,onMvSharpWin4,onMvSharpWin5,onMvSharpWin6,onMvSharpWin7,onMvSharpWin8 };
	int len = sizeof(fun) / sizeof(sharpFun);
	for (int i = 0; i < len; i++)
	{
		m_vctFun.push_back(fun[i]);
	}

	g_MB->ini();
	{
		//回调初始化
		g_MB->m_errFun = 0;
		g_MB->m_mtc.m_arrErrFun = 0;
		g_MB->m_io.m_ioStateFun = 0;
		g_MB->m_ioMvStateFun = ioCallBack4;
		g_MB->m_CheckRet = 0;
		g_MB->m_writePic.m_fun = 0;
	}
	{
		string szPath = getExePath();
		//图像制程加载
		g_MB->m_pCheck->loadprocCheck(0, szPath + "picProc/2");
		//运动控制
		szPath += "move/2";
		reloadMVCfg(szPath);
		//运动界面设置
		if (!g_MB->m_demoCtrl.ini(szPath, m_vctFun))
			printf("存在重复的监控元素");
		mysleep(2000);
		string szviewname = g_MB->m_demoCtrl.m_mainWinName;
		cv::namedWindow(szviewname, cv::WINDOW_NORMAL);
		cv::setMouseCallback(szviewname, onMvMouse);

	}

	//开始运行
	g_MB->m_pCheck->m_curCamId = 1;//仿真相机编号 simulation文件夹的0文件夹
	g_MB->m_io.ioTrigPause(false);
	string command = g_MB->m_szChainMv;
	if (command != "")
	{
		g_MB->updateProb();
		g_MB->pushMsg(command);
	}

	printf("%s", g_threadMgr.checkThread().c_str());
	/*char c;
	c = getchar();*/
	system("pause");
	if (NULL != g_MB)
	{
		delete g_MB;
		g_MB = NULL;
	}
}

void test()//图像制程
{
	SetSystem("use_window_thread", "true");
	SetSystem("parallelize_operators", "true");

	CProcDll m_pCheck;//检测dll
	string szexepath = getExePath();
	bool ret = m_pCheck.loadprocCheck(0, szexepath + "16");
//	m_pCheck.setViewName("");
	string dir = szexepath + "simulation/" + "16/";
	std::vector<string> vct;
	getFileNameVec(dir.c_str(), vct);
	CoordinatePosXYZ pd;
	pd.m_procid = 0;
	for (int i = 0; i < vct.size(); i++)
	{
		pd.vm[0] = cv::imread(dir + vct[i]);
		if (!pd.vm[0].empty())
		{
			m_pCheck.procCheck(&pd);
			//提取具体的ng或ok结果
			string szRet = pd.str;
			if (szRet.size() > 6)
			{
				std::vector<string>  o;
				getNgType(szRet, o, "NG");
				for (int i = 0; i < o.size(); i++)
				{
					string szNgType = o[i];//ng具体的类型
					cout << szNgType << endl;
				}
			}
		}
	}

}

void pwwMapSevRun()
{
	CPwwMapServer pwwMapSev;
std:cin.get();
	//pwwMapClientRun();
}
void pwwMapClientRun()
{
	treatPwwMap pwwMapClient;
	pwwMapClient.m_lTimeout = 1;//设置超时时间

	const 	int iIndexLen = 20;//The number of bytes
	const   int iNum = 1000000;//The number of data  1 million don't use diskMap unless the use of solid-state hard disk

	int num = 0;
	string szStr = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&\'()*+,-./:;< => ?@[\\]^_`{|}~";
	int count = 0;
	while (count++ < iNum)//
	{
		string szKey = "";
		int iRand = /*1+rand()%*/iIndexLen;
		for (int i = 0; i < iRand; i++)
		{
			szKey += szStr[rand() % szStr.size()];
		}
		//	cout << szKey  << "		" <<count << endl;
		int iret = pwwMapClient.insertData(szKey);
		if (iret)
			num++;
		else if (-1 == iret)
		{
			cout << "解码错误" << endl;
		}
		else//0==iret
		{
			cout << "网络运算超时" << endl;
		}
	}

	cout << "map num = " << num << endl;
std:cin.get();
}


int main()
{
	//	pwwMapSevRun();
	//pwwMapClientRun();
	test();

	cv::destroyAllWindows();
	std::cin.get();
	
	return 0;
}
