#ifndef __CAMERA_H__
#define __CAMERA_H__
/*
相机基类
实现opencv的相机和视频提取功能

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
	//相机抓图
	virtual int grapImage(cv::Mat& m);

protected:
	/*相机驱动初始化，
	参数：nIndex设备标记 
	返回值：-1 错误，0 正常
	*/
	virtual int ini(unsigned int nIndex = 0);
protected:
	CResLock m_Lock;

};

#endif