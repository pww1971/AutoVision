
#include "pch.h"
#include "checkPatch.h"
#include "CRangPaDlg.h"
#define PIXELS 480000    //800*600


S_CheckParam::S_CheckParam()
{
	m_pCheck = NULL;
}
S_CheckParam::~S_CheckParam()
{

}
void S_CheckParam::readMode(string file)
{
	CConfig cfg(file + "\\checkParam1.cfg");
	string szVal = cfg.getItem("hasObjReturnOK");
	if ("" != szVal)
	{
		m_tpltPa.m_hasObj = atoi(szVal.c_str());
	}
	szVal = cfg.getItem("fPercent");
	if ("" != szVal)
	{
		m_tpltPa.m_percent = atoi(szVal.c_str());
	}
	szVal = cfg.getItem("fPercentMax");
	if ("" != szVal)
	{
		m_tpltPa.m_percentMax = atoi(szVal.c_str());
	}
	szVal = cfg.getItem("regNumMin");
	if ("" != szVal)
	{
		m_tpltPa.iRegNumMin = atoi(szVal.c_str());
	}
	else
		m_tpltPa.iRegNumMin = 1;
	szVal = cfg.getItem("regNumMax");
	if ("" != szVal)
	{
		m_tpltPa.iRegNumMax = atoi(szVal.c_str());
	}
	else
		m_tpltPa.iRegNumMax = 8000;//为了兼容老配置文件
	readParamFromTxt(file + "\\checkParam2.cfg", m_tpltPa);
}
void S_CheckParam::writeMode(string file)
{
	CConfig cfg(file + "\\checkParam1.cfg");
	char sz[32];
	cfg.updateItem("\n#图片处理参数", "1有目标返回OK,0有目标返回NG");
	cfg.updateItem("hasObjReturnOK", CStrProcess::itoa_radixN(m_tpltPa.m_hasObj));
	cfg.updateItem("fScale", sz);
	sprintf(sz, "%d", m_tpltPa.m_percent);
	cfg.updateItem("\n#区域适配器参数", "有效点");
	cfg.updateItem("fPercent", sz);
	sprintf(sz, "%d", m_tpltPa.m_percentMax);
	cfg.updateItem("fPercentMax", sz);
	cfg.updateItem("\n#区域数范围", "");
	sprintf(sz, "%d", m_tpltPa.iRegNumMin);
	cfg.updateItem("regNumMin", sz);
	sprintf(sz, "%d", m_tpltPa.iRegNumMax);
	cfg.updateItem("regNumMax", sz);

	writeParamToTxt(file + "\\checkParam2.cfg", m_tpltPa);
}

bool S_CheckParam::design(bool add, int id)
{
	cv::Mat m = m_curProc.m_rawPic[0].clone();
	double distance = ((checkPatch*)m_pCheck)->getdis(m);
	if (distance > -1)
	{
		((checkPatch*)m_pCheck)->draw(m);
		return true;
	}
	return false;
}

checkPatch1::checkPatch1()
{
	m_data = &m_LRD;
}

checkPatch1::~checkPatch1()
{
}

void checkPatch1::rebuildContour(vector<vector<cv::Point> >& contours)
{
	vector<cv::Point> contour;
	for (int i = 0; i < m_data->m_contours.size(); i++)
	{
		if (m_data->m_vSign[i] == 2)//2 删除的
			continue;
		contour.assign(m_data->m_contours[i].begin(), m_data->m_contours[i].end());
		contours.push_back(contour);
	}
}
double checkPatch1::match(vector<vector<cv::Point> >& contours)
{
	vector<vector<cv::Point> > cont1;
	rebuildContour(cont1);
	if (contours.size() > cont1.size())
	{
		return -1;
	}
	double val = 0;
	for (int i = 0; i < contours.size(); i++)
	{
		val += matchShapes(contours[i], cont1[i], CONTOURS_MATCH_I2, 0);
	}
	return val;
}




checkPatch2::checkPatch2()
{
	m_conAdpt = CpwwRegionConnect::contourFilter;
	m_rgbAdpt = CpwwRegionConnect::rgbAdapt;
	m_regAdpt = CpwwRegionConnect::regionFilter;
	m_sPicFeat = &m_PicF;
}

checkPatch2::~checkPatch2()
{
}

void checkPatch2::rebuildContour(vector<vector<cv::Point> >& contours)
{
	vector<cv::Point> contour;
	for (int i = 0; i < m_PicF.curLen; i++)
	{
		CRegion& reg = m_PicF.region[i];
		CRegion::S_Contour* outline = reg.outline;
		if (NULL == outline)
			return;
		int lenR = outline->curLenIR;
		int len;
		S_Coordinate* sPos;
		cv::Scalar color;//轮廓线分内外颜色显示
		for (int j = 0; j < lenR; j++)
		{
			if (outline->sRingArray[j] == NULL)
				continue;
			len = outline->sRingArray[j]->curLen;
			sPos = outline->sRingArray[j]->ring;
			for (int ii = 0; ii < len; ii++)
			{
				contour.push_back(move(cv::Point(sPos[ii].x, sPos[ii].y)));
			}
			contours.push_back(contour);
		}	
	}
}
double checkPatch2::match(vector<vector<cv::Point> >& contours)
{
	vector<vector<cv::Point> > cont1;
	rebuildContour(cont1);
	if (contours.size() > cont1.size())
	{
		return -1;
	}
	double val = 0;
	for (int i = 0; i < contours.size(); i++)
	{
		val += matchShapes(contours[i], cont1[i], CONTOURS_MATCH_I2, 0);
	}
	return val;
}

void checkPatch2::cmpMask(cv::Mat& src, cv::Mat& pwwMask, CTpltParam& pa, bool bDraw)
{
	int c = src.channels();
	int h = src.rows;
	int w = src.cols;

	unsigned char* ptrMask = pwwMask.data;
	//根据掩模显示图像
	unsigned char* ptrData = src.data;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (*ptrMask > 0 || i<pa.xMinConnNum || i>h - pa.xMinConnNum || j<pa.xMinConnNum || j>w - pa.xMinConnNum)//
			{
				if (bDraw)
				{
					ptrData[0] = 0;
					ptrData[1] = 0;
					ptrData[2] = 0;
				}
			}
			else
			{
				ptrData[0] = 255;
				ptrData[1] = 255;
				ptrData[2] = 255;
			}
			ptrData += c;
			ptrMask++;
		}
	}
}
bool checkPatch2::verticalPwwTreat(cv::Mat& src, CTpltParam& pa, bool bDraw)
{
	m_sParams = &pa;

	cv::Mat dst;
	cv::transpose(src, dst);//原图垂直
	S_PicMat psrc;
	S_picMsg::picMatPTMat(psrc, dst);
	m_PicF.~CPicFeater();
	m_PicF.mallocRegion();
	treat(psrc);
	if (m_PicF.curLen == 0)
	{
		return false;
	}

	cv::Mat mask = cv::Mat(dst.rows, dst.cols, CV_8UC1, cv::Scalar(0));
	getContourMask2(m_PicF, m_sParams->bound, mask, 255);//提取mask
	cv::transpose(mask, mask);//垂直回来

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

	//char sz[128];
	//sprintf(sz, "objNum %d", m_PicF.curLen);
	//m_szRet = sz;

	if (m_PicF.curLen >= pa.iRegNumMin && m_PicF.curLen < pa.iRegNumMax)					//有目标
	{
		cv::Mat mask2 = cv::Mat(mask.rows, mask.cols, CV_8UC1, cv::Scalar(0));
		getContourMask2(m_PicF, m_sParams->bound, mask2, 255);//提取mask2
		cmpMask(src, mask2, pa, bDraw);//根据mask提取src

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

bool checkPatch2::pwwTreat(cv::Mat& src)
{
	S_PicMat psrc;
	S_picMsg::picMatPTMat(psrc, src);
	m_PicF.~CPicFeater();
	m_PicF.mallocRegion();
	treat(psrc);
	if (m_PicF.curLen >= ((CTpltParam*)m_sParams)->iRegNumMin && m_PicF.curLen < ((CTpltParam*)m_sParams)->iRegNumMax)					//有目标
	{
		return true;
	}

	return false;
}




checkPatch::checkPatch()
{
	m_pixels = 1;
	string szStr = GDKT::CStrProcess::itoa_radixN(rand());
	m_myParam.m_mainWinName = DLLVIEW;// "Proc" + szStr;
	//	m_myParam.m_subWinName = DLLVIEW"subProc" + szStr;
	//	m_myParam.m_texWinName = DLLVIEW"textProc" + szStr;
	for (int i = 0; i < 3; i++)
	{
		m_myParam.m_hist[i].m_hisWinName = DLLVIEW"His" + szStr + CStrProcess::itoa_radixN(i);
		m_myParam.m_hist[i].m_pDesign = &m_myParam;
	}
	m_myParam.m_pCheck = this;
	
	m_f = 0.35;  // 焦距
	m_r = 8.5;   // 被测物体宽度
	m_ck2.m_sParams = m_ck1.m_sParams = &m_myParam.m_tpltPa;
}

checkPatch::~checkPatch()
{
}


//图片的处理	一保存模板图片，二加载模板图片进行旋转角度后匹配
bool checkPatch::treatPicture(CTpltParam& pa, vector<S_TpltData*>& vctData)
{
	int curid = pa.m_tempPa.m_curPosID;
	vector<cv::Mat>& src = vctData[curid]->m_vctPic;
	if (!src[0].data)
		return false;

	if (pa.m_dllPa.ini < 1)
	{
		if ("" != pa.m_dllPa.szfile)
		{
			readMode(pa.m_dllPa.szfile);
		}
		if (pa.m_dllPa.ini == 0)
		{
			;
		}
		else//ini == -1
		{
			CRangPaDlg m_dlg;
			//m_myParam.m_tpltPa = pa;
			m_dlg.m_pCheck = this;
			m_myParam.updateFrame(src);
			if (1 == m_dlg.DoModal())
			{
				writeMode(pa.m_dllPa.szfile);
			}
		}
		pa.m_dllPa.ini = 1;
		return true;
	}
	else
	{
		//处理图片
		double distance = getdis(src[0]);
		if (distance > -1)
		{
			char dis[50];
			sprintf(dis, "%.2f", distance);
			vctData[curid]->m_szRet = dis;
			vctData[curid]->m_checkRet = true;
			return true;
		}
		char sz[128];
		if (src[0].total()> PIXELS)
			sprintf(sz, "面积= %d,目标 %d,NO", (int)m_ck1.m_LRD.m_availNum, m_ck1.m_LRD.m_vReg.size());
		else
			sprintf(sz, "面积= %d,目标 %d,NO", (int)m_ck2.m_PicF.availNum, m_ck2.m_PicF.curLen);

		vctData[curid]->m_szRet = sz;
	}
	return false;
}
double checkPatch::getdis(cv::Mat& src)
{
	double ret = -1;
	if (src.total() > PIXELS)
	{
		if (m_ck1.treat(src, &m_ck1.m_LRD))
		{
			if (0 == m_pixels)
			{
				ret = m_ck1.match(m_contours);
			}
			else
			{
				// 计算成像宽/高
				double width = m_ck1.m_LRD.m_vReg[0].width;
				double height = m_ck1.m_LRD.m_vReg[0].height;
				// 分别以宽/高为标准计算距离
				ret = m_r * m_f / (width + height) / 2;
			}
		}
		
	}
	else
	{
		if (m_ck2.pwwTreat(src))
		{
			if (0 == m_pixels)
			{
				ret = m_ck2.match(m_contours);
			}
			else
			{// 计算成像宽/高
				double width = m_ck2.m_PicF.region[0].right - m_ck2.m_PicF.region[0].left;
				double height = m_ck2.m_PicF.region[0].bottom - m_ck2.m_PicF.region[0].top;
				// 分别以宽/高为标准计算距离
				ret = m_r * m_f / (width + height) / 2;
			}
		}
	}
	return ret;
}
void checkPatch::draw(cv::Mat& src)
{
	if (src.total() > PIXELS)
	{
		if (m_myParam.m_tpltPa.m_drawMask)
		{
			if (src.channels() == 1)
			{
				cv::cvtColor(src, src, cv::COLOR_GRAY2RGB);
			}
			if (m_myParam.m_tpltPa.m_drawMask == 2)
			{
				cv::Mat mask = cv::Mat(src.rows, src.cols, CV_8UC1, cv::Scalar(0));
				m_ck1.fillCout(mask);
				drawMask(src, mask);
			}
		}
		m_ck1.drawCout(src);
	}
	else
	{
		if (m_myParam.m_tpltPa.m_drawMask)
		{
			if (m_myParam.m_tpltPa.m_drawMask == 2)
			{
				cv::Mat mask = cv::Mat(src.rows, src.cols, CV_8UC1, cv::Scalar(0));
				getContourMask2(m_ck2.m_PicF, m_ck2.m_sParams->bound, mask, 255);//提取mask2
				drawMask(src, mask);
			}
		}
		drawRegion(src, m_ck2.m_PicF);
	}
	showMat(DLLVIEW, src);
}
void checkPatch::readMode(string file)
{
	mkdir(file.c_str());
	m_myParam.readMode(file);
	CConfig cfg(file + "\\checkParam3.cfg");
	string szVal = cfg.getItem("m_f");
	if ("" != szVal)
	{
		m_f = atof(szVal.c_str());
	}
	szVal = cfg.getItem("m_r");
	if ("" != szVal)
	{
		m_r = atof(szVal.c_str());
	}
	szVal = cfg.getItem("m_pixels");
	if ("" != szVal)
	{
		m_pixels = atoi(szVal.c_str());
	}
	m_contours.clear();
	ifstream fl(file + "\\contours.txt");
	string line;
	while (getline(fl, line)) {
		vector<Point> contour;
		stringstream ss(line);
		int x, y;
		while (ss >> x >> y) {
			contour.push_back(Point(x, y));
		}
		m_contours.push_back(contour);
	}
}

void checkPatch::writeMode(string file)
{
	mkdir(file.c_str());
	m_myParam.writeMode(file);
	CConfig cfg(file + "\\checkParam3.cfg");
	char sz[32];
	sprintf(sz, "%f", m_f);
	cfg.updateItem("m_f", sz);
	sprintf(sz, "%f", m_r);
	cfg.updateItem("m_r", sz);
	sprintf(sz, "%d", m_pixels);
	cfg.updateItem("m_pixels", sz);
	if (0 == m_pixels)
	{
		m_contours.clear();
		if (m_ck1.m_LRD.m_contours.size() > 0)
			m_ck1.rebuildContour(m_contours);
		else
			m_ck2.rebuildContour(m_contours);
	}
	// 将轮廓保存到文件中
	ofstream fl(file + "\\contours.txt");
	for (int i = 0; i < m_contours.size(); i++) {
		for (int j = 0; j < m_contours[i].size(); j++) {
			fl << m_contours[i][j].x << " " << m_contours[i][j].y << " ";
		}
		fl << endl;
	}
}

bool checkPatch::drawResult(cv::Mat& src, cv::RotatedRect& roiRect, CTpltParam& pa)
{
	/*if (pa.m_drawMask)
	{
		if (src.channels() == 1)
		{
			cv::cvtColor(src, src, cv::COLOR_GRAY2RGB);
		}
		if (pa.m_drawMask == 2)
		{
			cv::Mat mask = cv::Mat(src.rows, src.cols, CV_8UC1, cv::Scalar(0));
			fillCout(mask);
			drawMask(src, mask);
		}
		drawCout(src);
	}*/
	return true;
}

