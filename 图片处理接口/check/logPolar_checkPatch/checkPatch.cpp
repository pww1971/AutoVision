
#include "pch.h"
#include "checkPatch.h"
#include "paramDlg.h"

S_CheckParam::S_CheckParam()
{
	type = 0;
	m = 80.0;
}

S_CheckParam::~S_CheckParam()
{

}

void S_CheckParam::readMode(string file)
{
	CConfig cfg(file + "\\checkParam1.cfg");
	string szVal = cfg.getItem("type");
	if ("" != szVal)
	{
		type = atoi(szVal.c_str());
	}
}

void S_CheckParam::writeMode(string file)
{
	CConfig cfg(file + "\\checkParam1.cfg");
	cfg.updateItem("type", CStrProcess::itoa_radixN(type));
}

checkPatch::checkPatch()
{

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
			paramDlg m_dlg;
			//m_myParam.m_tpltPa = pa;
			m_dlg.m_pCheck = this;
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
		double w = src[0].cols / 2;
		double h = src[0].rows / 2;
		double maxRadius = hypot(w, h);
		if(m_myParam.type)
			linearPolar(src[0],src[0], cv::Point2f(w, h), maxRadius, cv::WARP_FILL_OUTLIERS + cv::INTER_LINEAR);
		else
		{
			double m = src[0].cols / log(maxRadius);
			logPolar(src[0], src[0], cv::Point2f(w, h), /*m_myParam.*/m, cv::WARP_FILL_OUTLIERS + cv::INTER_LINEAR);
		}
		//rotateImage(src[0], 90);
		cv::rotate(src[0], src[0], cv::ROTATE_90_COUNTERCLOCKWISE);
		//	vctData[curid]->m_szRet = dis;
		vctData[curid]->m_checkRet = true;
		return true;
	}
	return false;
}

void checkPatch::readMode(string file)
{
	mkdir(file.c_str());
	m_myParam.readMode(file);

}

void checkPatch::writeMode(string file)
{
	mkdir(file.c_str());
	m_myParam.writeMode(file);
	
}

bool checkPatch::drawResult(cv::Mat& src, cv::RotatedRect& roiRect, CTpltParam& pa)
{
	
	return true;
}

