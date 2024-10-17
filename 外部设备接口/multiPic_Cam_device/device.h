#ifndef __DEVICE_H__
#define __DEVICE_H__
/*
外部运动模块
自定义图像处理算法 
*/
#include "automove.h"

class CDevice 
{
public:
	CDevice();
	~CDevice();

public:
	bool ini();
	std::vector<cv::Mat> m_vm;
};

#endif