
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


double Cal_Correlation_Coefficient(histogram_data_st& his1, histogram_data_st& his2)
{
	double Cor_Coeffi = 0;
	double sumx = 0, sumy = 0, sumxx = 0, sumyy = 0, sumxy = 0;
	for (int i = 0; i < 256; i++) {
		double val1 = his1.his_value[i] / (double)his1.his_max;
		double val2 = his2.his_value[i] / (double)his2.his_max;
		sumx += val1;
		sumy += val2;
		sumxy += val1 * val2;
		sumxx += val1 * val1;
		sumyy += val2 * val2;
	}
	Cor_Coeffi = ((256 * sumxy - sumx * sumy) * (256 * sumxy - sumx * sumy)) /
		((256 * sumxx - sumx * sumx) * (256 * sumyy - sumy * sumy));
	return Cor_Coeffi;
}

S_CheckParam::S_CheckParam()
{
	m_pCheck = NULL;
	m_iHisType = 0;
	m_N = 8;
	m_channel = -1;
}
S_CheckParam::~S_CheckParam()
{

}
void S_CheckParam::readParam(string file)
{
	FILE* fp = fopen(file.c_str(), "r+b");
	if (NULL == fp)
	{
		return;
	}
	fseek(fp, 0, SEEK_END);
	long fpSize = ftell(fp);
	if (0 == fpSize)
	{
		fclose(fp);
		return;
	}
	fseek(fp, 0, SEEK_SET);
	fread(&m_iHisType, sizeof(int), 1, fp);
	fread(&m_channel, sizeof(int), 1, fp);
	fread(&m_N, sizeof(int), 1, fp);
	fclose(fp);
}

void S_CheckParam::writeParam(string file)
{
	remove(file.c_str());
	FILE* fp = fopen(file.c_str(), "ab");
	fwrite(&m_iHisType, sizeof(int), 1, fp);
	fwrite(&m_channel, sizeof(int), 1, fp);
	fwrite(&m_N, sizeof(int), 1, fp);
	fclose(fp);
}

bool S_CheckParam::design(bool add, int id)
{
	cv::Mat m = m_curProc.m_rawPic.clone();
	bool b = ((checkPatch*)m_pCheck)->checkDug(m);
	showMat(m_mainWinName, m);
	return b;
}


checkPatch::checkPatch()
{
	string szStr = GDKT::CStrProcess::itoa_radixN(rand());
	m_myParam.m_mainWinName = DLLVIEW"Proc" + szStr;
//	m_myParam.m_subWinName = DLLVIEW"subProc" + szStr;
//	m_myParam.m_texWinName = DLLVIEW"textProc" + szStr;
	for (int i = 0; i < 3; i++)
	{
		m_myParam.m_hist[i].m_hisWinName = DLLVIEW"His" + szStr + CStrProcess::itoa_radixN(i);
		m_myParam.m_hist[i].m_pDesign = &m_myParam;
	}
	m_myParam.m_pCheck = this;

	m_conAdpt = CpwwRegionConnect::contourFilter;
	m_rgbAdpt = CpwwRegionConnect::rgbAdapt;
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
	HWND hMainWnd=FindWindowA("#32770","视觉控制系统");//通过遍历窗口方式获取目标进程主窗口句柄
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
	m_szRet = sz;

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

bool checkPatch::pwwTreat(cv::Mat& src)
{
	m_sParams = &m_myParam.m_tpltPa;

	S_PicMat psrc;
	S_picMsg::picMatPTMat(psrc, src);
	m_PicF.~CPicFeater();
	m_PicF.mallocRegion();
	treat(psrc);		
	if (m_PicF.curLen >= m_myParam.m_tpltPa.iRegNumMin && m_PicF.curLen < m_myParam.m_tpltPa.iRegNumMax)					//有目标
	{
		if (m_myParam.m_tpltPa.m_drawMask)
		{
			if (m_myParam.m_tpltPa.m_drawMask == 2)
			{
				cv::Mat mask = cv::Mat(src.rows,src.cols,CV_8UC1,cv::Scalar(0));
				getContourMask2(m_PicF,m_sParams->bound,mask,255);//提取mask2
				drawMask(src,mask);
			}
			drawRegion(src, m_PicF);
		}
		return true;
	}

	return false;
}

//图片的处理	一保存模板图片，二加载模板图片进行旋转角度后匹配
bool checkPatch::treatPicture(CTpltParam& pa, vector<S_TpltData*>& vctData, int curid)
{
	cv::Mat& src = vctData[curid]->m_pic;
	if (!src.data )
		return false;
	CTpltParam *tpm = &m_myParam.m_tpltPa;
	*tpm = pa;//同步参数需要

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
			m_myParam.updateFrame(src);
			if(1 == m_dlg.DoModal())
			{
				writeMode(pa.m_dllPa.szfile);
				pa = m_myParam.m_tpltPa;
			}
		}
		pa.m_dllPa.ini=1;
		return true;
	}
	else
	{
		//处理图片
		if(checkDug(src))
		{
			vctData[curid]->m_szRet = m_szRet;
			vctData[curid]->m_checkRet = true;
			return true;
		}
		vctData[curid]->m_szRet = m_szRet;
	}
	return false;
}

bool checkPatch::checkDug(cv::Mat&src)
{
	m_szRet ="";
	if (src.channels() > 1)
	{
		if (m_myParam.m_channel > 0)
		{
			cv::Mat m[3];
			cv::split(src, m);
			int id = m_myParam.m_channel-1;
			if (id > -1 && id < 3)
			{
				src = m[id];
			}
			else
				src = m[0];
		}
		else
			cv::cvtColor(src, src, cv::COLOR_RGB2GRAY);
	}
	cv::Size siz = cv::Size(src.rows, src.cols);
	cv::Mat m2 = src;
	if (m_myParam.m_tpltPa.m_scale < 1)
	{
		resize(m2, m2, cv::Size(m2.rows * m_myParam.m_tpltPa.m_scale, m2.cols * m_myParam.m_tpltPa.m_scale));
	}
	cv::Mat m = cv::Mat(m2.rows, m2.cols, src.type(), Scalar(0));
	if (m_myParam.m_iHisType == 0)
	{
		lbp(m2,m);
	}
	else
	{
		elbp(m2, m);
	}
	
	calTexture(m_myParam.m_N, m_myParam.m_tpltPa.sThdMin[2], m_myParam.m_tpltPa.sThdMax[2],m, m2);
	
	if (m_myParam.m_tpltPa.m_scale < 1)
	{
		resize(m2, src, siz);
	}
	else
		src = m2;
	return true;// pwwTreat(src);
}

void checkPatch::readMode(string file)
{
	mkdir(file.c_str());
	m_myParam.readParam(file + "\\checkParam1.cfg");
//	readParamFromTxt(file + "\\checkParam2.cfg",m_myParam);
	m_myParam.m_multiSel.loadParam(file + "\\sel.cfg");
}

void checkPatch::writeMode(string file)
{
	mkdir(file.c_str());
	m_myParam.writeParam(file + "\\checkParam1.cfg");
//	writeParamToTxt(file + "\\checkParam2.cfg",m_myParam);
	m_myParam.m_multiSel.writeParam(file + "\\sel.cfg");
}

bool checkPatch::drawResult(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa)
{
	return true;
}

/**
* @brief 对输入图像进行细化,骨骼化
* @param src为输入图像,用cvThreshold函数处理过的8位灰度图像格式，元素中只有0与1,1代表有元素，0代表为空白
* @param maxIterations限制迭代次数，如果不进行限制，默认为-1，代表不限制迭代次数，直到获得最终结果
* @return 为对src细化后的输出图像,格式与src格式相同，元素中只有0与1,1代表有元素，0代表为空白
*/
cv::Mat thinImage(const cv::Mat & src, const int maxIterations = -1)
{
	assert(src.type() == CV_8UC1);
	cv::Mat dst;
	int width = src.cols;
	int height = src.rows;
	src.copyTo(dst);
	int count = 0;  //记录迭代次数  
	while (true)
	{
		count++;
		if (maxIterations != -1 && count > maxIterations) //限制次数并且迭代次数到达  
			break;
		std::vector<uchar *> mFlag; //用于标记需要删除的点  
		//对点标记  
		for (int i = 0; i < height; ++i)
		{
			uchar * p = dst.ptr<uchar>(i);
			for (int j = 0; j < width; ++j)
			{
				//如果满足四个条件，进行标记  
				//  p9 p2 p3  
				//  p8 p1 p4  
				//  p7 p6 p5  
				uchar p1 = p[j];
				if (p1 != 1) continue;
				uchar p4 = (j == width - 1) ? 0 : *(p + j + 1);
				uchar p8 = (j == 0) ? 0 : *(p + j - 1);
				uchar p2 = (i == 0) ? 0 : *(p - dst.step + j);
				uchar p3 = (i == 0 || j == width - 1) ? 0 : *(p - dst.step + j + 1);
				uchar p9 = (i == 0 || j == 0) ? 0 : *(p - dst.step + j - 1);
				uchar p6 = (i == height - 1) ? 0 : *(p + dst.step + j);
				uchar p5 = (i == height - 1 || j == width - 1) ? 0 : *(p + dst.step + j + 1);
				uchar p7 = (i == height - 1 || j == 0) ? 0 : *(p + dst.step + j - 1);
				if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) >= 2 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) <= 6)
				{
					int ap = 0;
					if (p2 == 0 && p3 == 1) ++ap;
					if (p3 == 0 && p4 == 1) ++ap;
					if (p4 == 0 && p5 == 1) ++ap;
					if (p5 == 0 && p6 == 1) ++ap;
					if (p6 == 0 && p7 == 1) ++ap;
					if (p7 == 0 && p8 == 1) ++ap;
					if (p8 == 0 && p9 == 1) ++ap;
					if (p9 == 0 && p2 == 1) ++ap;

					if (ap == 1 && p2 * p4 * p6 == 0 && p4 * p6 * p8 == 0)
					{
						//标记  
						mFlag.push_back(p + j);
					}
				}
			}
		}

		//将标记的点删除  
		for (std::vector<uchar *>::iterator i = mFlag.begin(); i != mFlag.end(); ++i)
		{
			**i = 0;
		}

		//直到没有点满足，算法结束  
		if (mFlag.empty())
		{
			break;
		}
		else
		{
			mFlag.clear();//将mFlag清空  
		}

		//对点标记  
		for (int i = 0; i < height; ++i)
		{
			uchar * p = dst.ptr<uchar>(i);
			for (int j = 0; j < width; ++j)
			{
				//如果满足四个条件，进行标记  
				//  p9 p2 p3  
				//  p8 p1 p4  
				//  p7 p6 p5  
				uchar p1 = p[j];
				if (p1 != 1) continue;
				uchar p4 = (j == width - 1) ? 0 : *(p + j + 1);
				uchar p8 = (j == 0) ? 0 : *(p + j - 1);
				uchar p2 = (i == 0) ? 0 : *(p - dst.step + j);
				uchar p3 = (i == 0 || j == width - 1) ? 0 : *(p - dst.step + j + 1);
				uchar p9 = (i == 0 || j == 0) ? 0 : *(p - dst.step + j - 1);
				uchar p6 = (i == height - 1) ? 0 : *(p + dst.step + j);
				uchar p5 = (i == height - 1 || j == width - 1) ? 0 : *(p + dst.step + j + 1);
				uchar p7 = (i == height - 1 || j == 0) ? 0 : *(p + dst.step + j - 1);

				if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) >= 2 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) <= 6)
				{
					int ap = 0;
					if (p2 == 0 && p3 == 1) ++ap;
					if (p3 == 0 && p4 == 1) ++ap;
					if (p4 == 0 && p5 == 1) ++ap;
					if (p5 == 0 && p6 == 1) ++ap;
					if (p6 == 0 && p7 == 1) ++ap;
					if (p7 == 0 && p8 == 1) ++ap;
					if (p8 == 0 && p9 == 1) ++ap;
					if (p9 == 0 && p2 == 1) ++ap;

					if (ap == 1 && p2 * p4 * p8 == 0 && p2 * p6 * p8 == 0)
					{
						//标记  
						mFlag.push_back(p + j);
					}
				}
			}
		}

		//将标记的点删除  
		for (std::vector<uchar *>::iterator i = mFlag.begin(); i != mFlag.end(); ++i)
		{
			**i = 0;
		}

		//直到没有点满足，算法结束  
		if (mFlag.empty())
		{
			break;
		}
		else
		{
			mFlag.clear();//将mFlag清空  
		}
	}
	return dst;
}

/**
* @brief 对骨骼化图数据进行过滤，实现两个点之间至少隔一个空白像素
* @param thinSrc为输入的骨骼化图像,8位灰度图像格式，元素中只有0与1,1代表有元素，0代表为空白
*/
void filterOver(cv::Mat thinSrc,vector<cv::Point>& vct)
{
	assert(thinSrc.type() == CV_8UC1);
	int width = thinSrc.cols;
	int height = thinSrc.rows;
	for (int i = 0; i < height; ++i)
	{
		uchar * p = thinSrc.ptr<uchar>(i);
		for (int j = 0; j < width; ++j)
		{
			// 实现两个点之间至少隔一个像素
			//  p9 p2 p3  
			//  p8 p1 p4  
			//  p7 p6 p5  
			uchar p1 = p[j];
			if (p1 != 1) continue;
			uchar p4 = (j == width - 1) ? 0 : *(p + j + 1);
			uchar p8 = (j == 0) ? 0 : *(p + j - 1);
			uchar p2 = (i == 0) ? 0 : *(p - thinSrc.step + j);
			uchar p3 = (i == 0 || j == width - 1) ? 0 : *(p - thinSrc.step + j + 1);
			uchar p9 = (i == 0 || j == 0) ? 0 : *(p - thinSrc.step + j - 1);
			uchar p6 = (i == height - 1) ? 0 : *(p + thinSrc.step + j);
			uchar p5 = (i == height - 1 || j == width - 1) ? 0 : *(p + thinSrc.step + j + 1);
			uchar p7 = (i == height - 1 || j == 0) ? 0 : *(p + thinSrc.step + j - 1);
			if (p2 + p3 + p8 + p9 >= 1)
			{
				p[j] = 0;
			}
			else
				vct.push_back(cv::Point(j,i));
		}
	}
}

/**
* @brief 从过滤后的骨骼化图像中寻找端点和交叉点
* @param thinSrc为输入的过滤后骨骼化图像,8位灰度图像格式，元素中只有0与1,1代表有元素，0代表为空白
* @param raudis卷积半径，以当前像素点位圆心，在圆范围内判断点是否为端点或交叉点
* @param thresholdMax交叉点阈值，大于这个值为交叉点
* @param thresholdMin端点阈值，小于这个值为端点
* @return 为对src细化后的输出图像,格式与src格式相同，元素中只有0与1,1代表有元素，0代表为空白
*/
std::vector<cv::Point> getPoints(const cv::Mat &thinSrc, unsigned int raudis = 4, unsigned int thresholdMax = 6, unsigned int thresholdMin = 4)
{
	assert(thinSrc.type() == CV_8UC1);
	int width = thinSrc.cols;
	int height = thinSrc.rows;
	cv::Mat tmp;
	thinSrc.copyTo(tmp);
	std::vector<cv::Point> points;
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			if (*(tmp.data + tmp.step * i + j) == 0)
			{
				continue;
			}
			int count=0;
			for (int k = i - raudis; k < i + raudis+1; k++)
			{
				for (int l = j - raudis; l < j + raudis+1; l++)
				{
					if (k < 0 || l < 0||k>height-1||l>width-1)
					{
						continue;

					}
					else if (*(tmp.data + tmp.step * k + l) == 1)
					{
						count++;
					}
				}
			}

			if (count > thresholdMax||count<thresholdMin)
			{
				Point point(j, i);
				points.push_back(point);
			}
		}
	}
	return points;
}

void checkPatch::genMode()
{
	cv::Mat v;
	cv::Mat mask;
	m_myParam.getRoi(v,mask);
	cv::Mat treatMat;
	cv::Mat graym;
	if (v.channels() > 1)
	{
		if (m_myParam.m_channel > 0)
		{
			cv::Mat m[3];
			cv::split(v, m);
			int id = m_myParam.m_channel-1;
			if (id > -1 && id < 3)
			{
				graym = m[id];
			}
			else
				graym = m[0];
		}
		else
			cv::cvtColor(v, graym, cv::COLOR_RGB2GRAY);
	}

	treatMat = cv::Mat(graym.rows, graym.cols, graym.type(),Scalar(0));
	if (m_myParam.m_iHisType == 0)
	{
		lbp(graym,treatMat);
	}
	else
	{
		elbp(graym, treatMat);
	}
	calLbpHis(treatMat, mask, m_myParam.m_hist[2].m_hisPa);
	m_myParam.m_hist[2].draw_map(true);
	/*v = g_MKC.m_curProc.m_rawPic;
	checkDug(v);
	imshow(m_myParam.m_mainWinName, v);*/

}

bool checkPatch::isMaskScope(cv::Point& pt)
{
	vector<CSelectRoi*>& vctMulSel = m_myParam.m_multiSel.m_vctMultiSel;
	int len = vctMulSel.size();
	for (int i=0;i<len;i++)
	{
		if (DoesRectangleContainPoint(vctMulSel[i]->m_rrect,pt))
			return true;
	}
	
	return false;
}

bool checkPatch::filterPoint(cv::Point& pt1,cv::Point& pt2)
{
	return true;
}