#ifndef __CHECKPATCH_H__
#define __CHECKPATCH_H__


#include "templateDesign.h"
class checkPatch 
{
public:
	checkPatch();
	~checkPatch();

	//ͼƬ�Ĵ���	
	bool treatPicture(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa);
	void cmpMask(cv::Mat& src,CTpltParam& pa);
private:
	cv::Mat m_tpltImg;//ģ��
};

#endif