
#include "stdafx.h"
#include "checkPatch.h"

CGuiContrlDlg checkPatch::m_dlg;

typedef struct _WND_INFO
{
	HWND hWnd;
	DWORD dwProcessId;
}WNDINFO, *PWNDINFO;

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	WNDINFO* pInfo = (WNDINFO*)lParam;
	DWORD dwProcessId = 0;
	GetWindowThreadProcessId(hWnd, &dwProcessId);//得到枚举出的窗口句柄对应的进程ID.

	if(dwProcessId == pInfo->dwProcessId)//看是不是你想要结束的进程的ID
	{
		pInfo->hWnd = hWnd;//是就用这个参数返回
		return FALSE;//停止枚举.
	}
	return TRUE;
}

HWND GetHwndByProcessId(DWORD dwProcessId)
{
	WNDINFO info = {0};
	info.hWnd = NULL;
	info.dwProcessId = dwProcessId;//传入你想要找的进程的ID
	EnumWindows(EnumWindowsProc, (LPARAM)&info);//开始枚举窗口.
	return info.hWnd;
}

bool checkPatch::iniDlg()
{
	if (m_dlg.m_dlgSucc)
	{
		m_dlg.ShowWindow(1);
		return true;
	}
	HWND hMainWnd=FindWindowA("#32770","TOT视觉控制系统");//通过遍历窗口方式获取目标进程主窗口句柄
	//HWND hMainWnd=GetHwndByProcessId(GetCurrentProcessId());//通过进程获取目标主窗口进程.注意:GetCurrentProcessId为目标进程Pid
	if(hMainWnd!=0)
	{
		m_dlg.Create(IDD_DIALOG1,CWnd::FromHandle(hMainWnd));
		//m_dlg.ShowWindow(0);
		m_dlg.m_dlgSucc = true;
		return true;
	}
	else
	{
		MessageBoxA(NULL,"调用失败","",0);
		return false;
	}
}


checkPatch::checkPatch()
{
	//dst=5*img1;//增加曝光
	//dst=img1/5;//降低曝光
	//bitwise_and(img1,img2,dst);//逻辑与，求交集
	//bitwise_or(img1,img2,dst);//逻辑或，求并集
	//bitwise_not(img1,dst);//逻辑非，求补集
	//bitwise_xor(img1,img2,dst);//异或，相同为0，相异为1
}

checkPatch::~checkPatch()
{
}
void checkPatch::readMode(string file)
{
	mkdir(file.c_str());
	file += "\\checkParam.cfg";
	CConfig cfg(file);
	//从配置文件中读取各参数
	string szVal = cfg.getItem("posx");	
	if (szVal != "")
	{
		m_myParam.posx = atoi(szVal.c_str());
	}
	szVal = cfg.getItem("ret");	
	if (szVal != "")
	{
		m_myParam.ret = atof(szVal.c_str());
	}
}

void checkPatch::writeMode(string file)
{
	mkdir(file.c_str());
	file += "\\checkParam.cfg";
	CConfig cfg(file);
	//把各参数写入配置文件
	cfg.updateItem("\n#参数例子","整型和浮点型 ");
	cfg.updateItem("posx",CStrProcess::itoa_radixN(m_myParam.posx));
	char sz[64];
	sprintf(sz, "%f", m_myParam.ret);
	cfg.updateItem("ret",sz);
}


//图片的处理	
bool checkPatch::treatPicture(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa)
{
	if (!src.data )
		return false;
	cv::Mat dst;
	if (src.channels()>1)
	{
		if (pa.m_dllPa.szParam!="")
		{
			cv::Mat m[3];
			cv::split(src,m);
			int id = atoi(pa.m_dllPa.szParam.c_str());
			if (id >-1 && id < 3)
			{
				dst = m[id];
			}
			else
				dst = m[0];
		}
		else
			cv::cvtColor(src, dst, CV_RGB2GRAY);
	}

	Hobject ho_Image0 = Mat2HObject(dst);
	if (ho_Image0.Id() == H_EMPTY_REGION)	
		return  false;
	if (pa.m_dllPa.ini<1)
	{		
		if (pa.m_dllPa.ini == 0 && "" != pa.m_dllPa.szfile)
		{
			readMode(pa.m_dllPa.szfile);
		}
		else//ini == -1
		{
			m_dlg.m_pMyParam = &m_myParam;
			if(1 == m_dlg.DoModal())
			{
				writeMode(pa.m_dllPa.szfile);
			}
		}
		pa.m_dllPa.ini=1;
		return true;
	}
	else
	{
		//处理图片
		return true;
	}

	return false;
}

bool checkPatch::drawResult(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa)
{
	return true;
}
