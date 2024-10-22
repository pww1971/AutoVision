#ifndef __CAMERA_H__
#define __CAMERA_H__
/*
�������
ʵ��opencv���������Ƶ��ȡ����

*/
#include "pwwVision.h"
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>
				

class CCamera
{
public:
	CCamera();
	~CCamera();
	//���ץͼ
	virtual int grapImage(cv::Mat& m);

protected:
	/*���������ʼ����
	������nIndex�豸��� 
	����ֵ��-1 ����0 ����
	*/
	virtual int ini(unsigned int nIndex = 0);
protected:
	CResLock m_Lock;

};

#endif