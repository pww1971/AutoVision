
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


bool myAdapt(unsigned char*&ptr, int iChannel,CRegParam* params)
{
	if (!hsiTable.data)
	{
		ptr+=iChannel;
		viewTxt("hisTable is NULL");
		return false;
	}
	//rgb按照256*（256*256）的矩阵进行查找相应hsi位置
	uchar * pTab = hsiTable.data + ((ptr[0]<<16)+(ptr[1]<<8)+ptr[2])*iChannel;

	int adaptValue=0;
	for( int i=0;i<iChannel;i++){
		if(params->mode[i])
		{
			if (params->sThdMax[i] < pTab[i] || pTab[i] < params->sThdMin[i])
				adaptValue++;
		}
		else
		{
			if (params->sThdMax[i] >= pTab[i] && pTab[i] >= params->sThdMin[i])
				adaptValue++;
		}
	}
	ptr+=iChannel;
	return adaptValue==iChannel;
};

checkPatch::checkPatch()
{
	if (!hsiTable.data)
	{
		hsiTable = cv::imread(getExePath()+"hsi.bmp");
	}

	m_conAdpt = CpwwRegionConnect::contourFilter;
	m_rgbAdpt = myAdapt;
	m_regAdpt = CpwwRegionConnect::regionFilter;
	m_sPicFeat = &m_PicF;

	m_dlg.m_pMyParam = &m_myParam;
}

checkPatch::~checkPatch()
{
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
	//	m_dlg.ShowWindow(0);
		m_dlg.m_dlgSucc = true;
		return true;
	}
	else
	{
		MessageBoxA(NULL,"调用失败","",0);
		return false;
	}
}

void checkPatch::cmpMask(cv::Mat& src,cv::Mat& pwwMask)
{
	int c = src.channels();
	int h = src.rows;
	int w = src.cols;

	unsigned char* ptrMask = pwwMask.data ;
	//根据掩模显示图像
	unsigned char* ptrData = src.data ;
	for (int i=0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if(*ptrMask > 0)//
			{
				;
			}
			else
			{
				ptrData[0]=0;
				ptrData[1]=0;
				ptrData[2]=0;
			}
			ptrData += c;
			ptrMask++;
		}
	}
}

bool checkPatch::verticalPwwTreat(cv::Mat& src,	CTpltParam& pa)
{
	m_sParams = &pa;

	cv::Mat dst;
	cv::transpose(src,dst);//原图垂直
	S_PicMat psrc;
	S_picMsg::picMatPTMat(psrc, dst);
	m_PicF.~CPicFeater();
	m_PicF.mallocRegion();
	treat(psrc);		
	if (m_PicF.curLen == 0)				
	{
		return false;
	}

	cv::Mat mask = cv::Mat(dst.rows,dst.cols,CV_8UC1,cv::Scalar(0));
	getContourMask2(m_PicF,m_sParams->bound,mask,255);//提取mask
	cv::transpose(mask,mask);//垂直回来
		
	/*cv::namedWindow("src", cv::WINDOW_NORMAL);
	cv::imshow("src",mask);
	cv::waitKey(1);*/

	//备份
	RGBADAPT rgbAdptBK = m_rgbAdpt;
	short smin = m_sParams->sThdMin[0];
	short smax = m_sParams->sThdMax[0];
	bool mode = m_sParams->mode[0];

	m_sParams->sThdMin[0] = 1;
	m_sParams->sThdMax[0] = 255;
	m_sParams->mode[0] = true;
	m_rgbAdpt = rgbAdapt;

	S_picMsg::picMatPTMat(psrc, mask);//处理垂直mask图
	m_PicF.~CPicFeater();
	m_PicF.mallocRegion();
	treat(psrc);	

	//还原
	m_sParams->sThdMin[0] = smin;
	m_sParams->sThdMax[0] = smax;
	m_sParams->mode[0] = mode;
	m_rgbAdpt = rgbAdptBK;

	if (m_PicF.curLen >= pa.iRegNumMin && m_PicF.curLen < pa.iRegNumMax)					//有目标
	{
		cv::Mat mask2 = cv::Mat(mask.rows,mask.cols,CV_8UC1,cv::Scalar(0));
		getContourMask2(m_PicF,m_sParams->bound,mask2,255);//提取mask2
		cmpMask(src,mask2);//根据mask提取src

		/*if (pa.m_drawMask)
		{
			if (pa.m_drawMask == 2)
			{
				cv::Mat mask2 = cv::Mat(mask.rows,mask.cols,CV_8UC1,cv::Scalar(0));
				getContourMask2(m_PicF,m_sParams->bound,mask2,255);//提取mask2
				drawMask(src,mask2);
			}
			drawRegion(src, m_PicF);
		}*/
		char sz[128];
		sprintf(sz,"objNum %d",m_PicF.curLen);
		pa.m_dllPa.szRet = sz;
		return true;
	}

	return false;
}

//图片的处理	一保存模板图片，二加载模板图片进行旋转角度后匹配
bool checkPatch::treatPicture(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa)
{
	if (!src.data )
		return false;
	/*cv::Mat dst;
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
	}*/

	if (pa.m_dllPa.ini<1)
	{		
		if (pa.m_dllPa.ini == 0 && "" != pa.m_dllPa.szfile)
		{
			readMode(pa.m_dllPa.szfile);
		}
		else//ini == -1
		{
			/*if(1 == m_dlg.DoModal())
			{
				writeMode(pa.m_dllPa.szfile);
			}*/
		}
		pa.m_dllPa.ini=1;
		return true;
	}
	else
	{
		//处理图片
		if(verticalPwwTreat(src,pa))
		{
			return true;
		}
	}

	return false;
}

void checkPatch::readMode(string file)
{
	mkdir(file.c_str());
	file += "\\checkParam.cfg";
	//readParamFromTxt(file,*m_sParams);
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
	//writeParamToTxt(file,*m_sParams);
	CConfig cfg(file);
	//把各参数写入配置文件
	cfg.updateItem("\n#参数例子","整型和浮点型 ");
	cfg.updateItem("posx",CStrProcess::itoa_radixN(m_myParam.posx));
	char sz[64];
	sprintf(sz, "%f", m_myParam.ret);
	cfg.updateItem("ret",sz);
}

bool checkPatch::drawResult(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa)
{
	return true;
}
