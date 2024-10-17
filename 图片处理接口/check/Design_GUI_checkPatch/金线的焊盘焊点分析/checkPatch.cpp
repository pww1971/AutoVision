
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

void checkPatch::cmpMask(cv::Mat& src,cv::Mat& pwwMask,CTpltParam& pa,bool bDraw)
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
			if(*ptrMask > 0 || i<pa.xMinConnNum || i>h-pa.xMinConnNum || j<pa.xMinConnNum || j>w-pa.xMinConnNum)//
			{
				if (bDraw)
				{
					ptrData[0]=0;
					ptrData[1]=0;
					ptrData[2]=0;
				}
			}
			else
			{
				ptrData[0]=255;
				ptrData[1]=255;
				ptrData[2]=255;
			}
			ptrData += c;
			ptrMask++;
		}
	}
}

bool checkPatch::verticalPwwTreat(cv::Mat& src,	CTpltParam& pa,bool bDraw)
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
		
	/*cv::imshow(DLLVIEW,mask);
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

	char sz[128];
	sprintf(sz,"objNum %d",m_PicF.curLen);
	pa.m_dllPa.szRet = sz;

	if (m_PicF.curLen >= pa.iRegNumMin && m_PicF.curLen < pa.iRegNumMax)					//有目标
	{
		cv::Mat mask2 = cv::Mat(mask.rows,mask.cols,CV_8UC1,cv::Scalar(0));
		getContourMask2(m_PicF,m_sParams->bound,mask2,255);//提取mask2
		cmpMask(src,mask2,pa,bDraw);//根据mask提取src

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
		int id = m_tempPa.m_preTreat;
		if (id > 6 && id < 10)
		{
			cv::Mat m[3];
			cv::split(srcImg, m);
			srcImg = m[id - 7];
		}
		else
			cv::cvtColor(srcImg, srcImg, cv::COLOR_RGB2GRAY);
	}*/

	if (pa.m_dllPa.ini<1)
	{		
		if ("" != pa.m_dllPa.szfile)
		{
			readMode(pa.m_dllPa.szfile);
		}
		if (pa.m_dllPa.ini == 0 )
		{
			;
		}
		else//ini == -1
		{
			m_dlg.m_pCheck = this;
			g_MKC.updateFrame(src);
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
		if(soldered_dot(src,m_myParam))
		{
			return true;
		}
	}
	return false;
}

bool checkPatch::soldered_dot(cv::Mat&src,CTpltParam pa[])
{
	//1、先焊盘分析 
	cv::Mat m1 = src.clone();
	verticalPwwTreat(m1,m_myParam[0],true);
	//2、再焊点分析 
	cv::Mat m2 = src.clone();
	verticalPwwTreat(m2,m_myParam[1],true);
	//3、焊盘求反 相交焊点
	cv::Mat m3;
	cv::bitwise_not(m1,m3);//得到焊盘图像的差 方便求焊点相交
	//cv::imshow("sdf1",m1);
	//cv::imshow("sdf2",m2);
	//cv::imshow("sdf3",m3);
	cv::bitwise_and(m2,m3,src);//求焊点相交
	//4、有相交点 返回false
	
	return true;
}

void checkPatch::readMode(string file)
{
	mkdir(file.c_str());
	readParamFromTxt(file + "\\checkParam1.cfg",m_myParam[0]);
	readParamFromTxt(file + "\\checkParam2.cfg",m_myParam[1]);
}

void checkPatch::writeMode(string file)
{
	mkdir(file.c_str());
	writeParamToTxt(file + "\\checkParam1.cfg",m_myParam[0]);
	writeParamToTxt(file + "\\checkParam2.cfg",m_myParam[1]);
}

bool checkPatch::drawResult(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa)
{
	//cv::Rect rect = roiRect.boundingRect();
	//int angle = 360-roiRect.angle;

	//rect &= cv::Rect(0, 0, src.cols, src.rows);
	//cv::Scalar c=cv::Scalar(0, 0, 255);
	//int size = src.cols*src.rows/(g_lineRate>>3);
	//int thickness = 1+size/8;
	//{
	//	float anglePI;
	//	float sa;
	//	float ca;
	//	if (angle != 0)//重新计算旋转后的位置
	//	{
	//		if (angle<0)
	//		{
	//			angle += 360;
	//		}
	//		if (angle>=360)
	//		{
	//			angle -= 360;
	//		}
	//		anglePI = (float) (-angle * CV_PI / 180);
	//		sa = sin(anglePI);
	//		ca = cos(anglePI);
	//	}
	//	int piccentx = rect.width/2;
	//	int piccenty = rect.height/2;
	//	float rrdx = roiRect.center.x - roiRect.size.width/2;
	//	float rrdy = roiRect.center.y - roiRect.size.height/2;
	//	for (int i = 0; i < m_PicF.curLen; i++)
	//	{
	//		CRegion& reg = m_PicF.region[i];
	//		int oL = reg.outline->curLenIR;
	//		CRegion::S_Ring* ring = reg.outline->sRingArray[reg.outline->outSign];//对应的环
	//		if (NULL == ring)
	//			continue;
	//		int len = ring->curLen;
	//		if (0 == len)
	//			continue;
	//		if (angle != 0)//重新计算旋转后的位置
	//		{
	//			float yDis = reg.y-piccentx;
	//			float xDis = reg.x-piccenty;

	//			reg.y = yDis*ca - xDis*sa ;
	//			reg.x = yDis*sa + xDis*ca ;

	//			reg.y += piccentx;
	//			reg.x += piccenty;
	//		}
	//		int ix = reg.x + rrdx;
	//		int iy = reg.y + rrdy;
	//		//绘制横线
	//		int ib = (size>>2);
	//		cv::line(src, cv::Point(ix - ib, iy), cv::Point(ix + ib, iy), c, thickness, 8, 0);
	//		//绘制竖线
	//		cv::line(src, cv::Point(ix, iy - ib), cv::Point(ix, iy + ib), c, thickness, 8, 0);
	//	}
	//}
	return true;
}
