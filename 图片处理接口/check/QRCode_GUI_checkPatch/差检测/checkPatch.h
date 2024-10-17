#ifndef __CHECKPATCH_H__
#define __CHECKPATCH_H__


#include "templateDesign.h"
class checkPatch 
{
public:
	checkPatch();
	~checkPatch();

	//图片的处理	
	bool treatPicture(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa);
	void cmpMask(cv::Mat& src,CTpltParam& pa);
private:
	cv::Mat m_tpltImg;//模板
};

#endif