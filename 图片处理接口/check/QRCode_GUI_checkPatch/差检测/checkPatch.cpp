#include "checkPatch.h"


checkPatch::checkPatch()
{
}

checkPatch::~checkPatch()
{
}
void checkPatch::cmpMask(cv::Mat& src,CTpltParam& pa)
{
	int c = src.channels();
	int bound = pa.bound;
	int h = m_tpltImg.rows;
	int w = m_tpltImg.cols;
	unsigned char* ptrData = src.data ;
	unsigned char* ptrMMask = m_tpltImg.data ;
	int wc = w*c;
	for (int i=0; i < h; i++)
	{
		if (i<bound || h-i<=bound )
		{
			memset(ptrData,0,wc);
			ptrData += wc;
			ptrMMask += wc;
			continue;
		}
		for (int j = 0; j < w; j++)
		{
			if (j<bound || w-j<=bound )
			{
				ptrData[0]=0;
				ptrData[1]=0;
				ptrData[2]=0;
			}
			else if(ptrData[0] == 0 && ptrData[1] == 0 && ptrData[2] == 0 )
			{
			}
			else
			{
				ptrData[0]=abs(ptrData[0]-ptrMMask[0]);
				ptrData[1]=abs(ptrData[1]-ptrMMask[1]);
				ptrData[2]=abs(ptrData[2]-ptrMMask[2]);
			}
			ptrData += c;
			ptrMMask += c;
		}
	}
}

//图片的处理	一保存模板图片，二加载模板图片进行旋转角度后匹配
bool checkPatch::treatPicture(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa)
{
	if (!src.data )
		return false;
	if (pa.m_dllPa.ini<1)
	{
		if (pa.m_scale<1)
		{
			resize(src,m_tpltImg,cv::Size(src.cols*pa.m_scale, src.rows*pa.m_scale));
		}
		else
		{
			m_tpltImg = src;
		}
		pa.m_dllPa.ini=1;
		return true;
	}
	else
	{
		if (pa.m_scale<1)
		{
			resize(src,src,cv::Size(src.cols*pa.m_scale, src.rows*pa.m_scale));
		}
		if (src.rows != m_tpltImg.rows || src.cols != m_tpltImg.cols)
		{
			return false;
		}
//#ifdef _DEBUG
//		cv::namedWindow("src", cv::WINDOW_NORMAL);
//		cv::imshow("src",m_tpltImg);
//		cv::waitKey(1);
//		cv::namedWindow("m_tpltImg", cv::WINDOW_NORMAL);
//		cv::imshow("m_tpltImg",m_tpltImg);
//		cv::waitKey(1);
//#endif
		//absdiff(m_tpltImg,src,src);
		cmpMask(src,pa);
		if (src.channels()==1)
			cv::cvtColor(src, src, CV_GRAY2RGB);
		return  true;
	}

	return false;
}

