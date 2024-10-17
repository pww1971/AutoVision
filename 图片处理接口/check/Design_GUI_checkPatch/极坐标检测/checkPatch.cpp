
#include "stdafx.h"
#include "checkPatch.h"

CGuiContrlDlg checkPatch::m_dlg;

checkPatch::checkPatch():PolarCheck()
{
	m_polarCor.m_iScanNum = 360;
}

checkPatch::~checkPatch()
{
}


bool checkPatch::ini(cv::Mat& tpltImg,cv::Mat& mask,CTpltParam& pa)
{
	m_hasIni = false;
	m_pa = pa;
	cv::Mat m = tpltImg.clone();
	S_PicMat psrc;
	S_picMsg::picMatPTMat(psrc, m);
	m_picF.~CPicFeater();
	m_picF.mallocRegion();
	treat(psrc);
	if (m_picF.curLen < pa.iRegNumMin || m_picF.curLen >= pa.iRegNumMax)					//有目标
		return m_hasIni;
	/*drawRegion(m,m_picF);
	imshow("sd",m);
	cv::waitKey(1);*/
	//整个图域的极坐标化
	bool bret;
	addVct<S_PolarPos>* scanArray = new addVct<S_PolarPos>[m_polarCor.m_iScanNum];
	for (int i = 0; i < m_picF.curLen; i++)
	{
		if (m_pa.m_polarPa.centerPos < 2)//图域重心为中心
		{
			bret = m_polarCor.polarCoordinate(m_picF.region[i],&m_picF, NULL);
		}
		else//轮廓重心为中心
			bret = false;

		if (bret)//拷贝各区域极坐标
		{
			m_hasIni = true;
			for (int x=0;x<m_polarCor.m_iScanNum;x++)
				scanArray[x].append(&m_polarCor.m_scanArray[x]);
		}
	}
	if (m_hasIni)
	{
		CPolarCoordinate::sortPolarCoor(scanArray,m_polarCor.m_iScanNum);//不做均值处理，每度有n个数据的可能
		m_polarCor.~CPolarCoordinate();
		m_polarCor.m_scanArray = scanArray;
	}
	else
		delete[] scanArray;
	return m_hasIni;
}

bool checkPatch::PolarMatch(cv::Mat& src)
{
	bool checkRet = false;
	
	S_PicMat psrc;
	S_picMsg::picMatPTMat(psrc, src);
	m_picF.~CPicFeater();
	m_picF.mallocRegion();
	treat(psrc);							//图像处理 结果在m_picF中
	if (m_picF.curLen < m_pa.iRegNumMin || m_picF.curLen >= m_pa.iRegNumMax)					//有目标
		return checkRet;

//	drawRegion(src,m_picF);
	m_vctRect.clear();						//检测前先清零
	
	int scanlineNum = m_polarCor.m_iScanNum;
	CPolarCoordinate pc2(scanlineNum);
	S_Coordinate* cent=NULL;//默认轮廓重心为中心
	if (m_pa.m_polarPa.centerPos < 2)//图域重心为中心
	{
		cent = new S_Coordinate();
		*cent = m_picF;
	}

	addVct<S_PolarPos>* scanArray2 = new addVct<S_PolarPos>[scanlineNum];
	for (int i = 0; i < m_picF.curLen; i++)
	{
		CRegion& reg = m_picF.region[i];
		if (m_pa.m_polarPa.shapeCheck > -1)
		{
			if( pc2.judgeShape(m_pa.m_polarPa,reg,&src) )
			{
				checkRet = true;
				cv::RotatedRect RoRect;
				//RoRect.rec = pc2.m_courReg;
				RoRect.size = cv::Size(reg.right - reg.left, reg.bottom - reg.top);
				RoRect.center = cv::Point(reg.left+RoRect.size.width/2, reg.top+RoRect.size.height/2);
				RoRect.angle = 0;
				
				m_vctRect.push_back(RoRect);
			}
		}
		else if (pc2.polarCoordinate(reg,cent,NULL,true))
		{
			checkRet = true;
			//拷贝各区域极坐标
			for (int x=0;x<scanlineNum;x++)
				scanArray2[x].append(&pc2.m_scanArray[x]);
		}
	}
	if (NULL!=cent)
	{
		delete cent;
	}
	if (checkRet)
	{
		CPolarCoordinate::sortPolarCoor(scanArray2,scanlineNum);//不做均值处理，每度有n个数据的可能
		pc2.~CPolarCoordinate();
		pc2.m_scanArray = scanArray2;
		checkRet = false;
		double iR = pc2.getRotateAngle(m_pa.m_polarPa, m_polarCor);
		if (-1 != iR)
		{
			drawRegion(src,m_picF);
#ifdef _DEBUG
			//显示匹配结果 因此根据角度 重新匹配了一次
			int* sign = new int[pc2.m_iScanNum];
			memset(sign, 0, sizeof(int)*pc2.m_iScanNum);
			pc2.getMatchPair(m_pair,m_polarCor, sign,  scanlineNum*iR/360,(iR-(int)iR)*(m_polarCor.m_iScanNum/pc2.m_iScanNum));
			pc2.drawPolarCoordinate( src, sign);
			delete[] sign;
#endif
			checkRet = true;
		}
	}

	return checkRet;
}

//图片的处理	一保存模板图片，二加载模板图片进行旋转角度后匹配
bool checkPatch::treatPicture(CTpltParam& pa, vector<S_TpltData*>& vctData, int curid)
{
	cv::Mat& src = vctData[curid]->m_pic;
	if (!src.data )
		return false;
	/*if (pa.m_dllPa.szParam!="")
	{
		int id = atoi(pa.m_dllPa.szParam.c_str());
		if (id >=-1 && id < 4)
		{
			pa.m_polarPa.shapeCheck = id;
		}
	}*/
	if (pa.m_dllPa.ini<1)
	{		
		if ("" != pa.m_dllPa.szfile)
		{
			readMode(pa.m_dllPa.szfile);
		}
		if (pa.m_dllPa.szParam=="" || 
			pa.m_dllPa.ini == 0 )
		{
			ini(src,src,pa);
		}
		else if (pa.m_dllPa.ini == -1)
		{
			m_pa = pa;
			m_dlg.m_pCheck = this;
			g_MKC.updateFrame(src);
			if(IDOK == m_dlg.DoModal())
			{
				pa = m_pa;
				writeMode(pa.m_dllPa.szfile);
			}
		}
		else//ini==-2
		{

		}
		pa.m_dllPa.ini=1;
		return true;
	}
	else
	{
		if (m_hasIni)
		//处理图片
		if(PolarMatch(src))
		{
			vctData[curid]->m_checkRet = true;
			return true;
		}
	}
	return false;
}


void checkPatch::readMode(string file)
{
}

void checkPatch::writeMode(string file)
{
}

bool checkPatch::drawResult(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa)
{
	return true;
}
