#ifndef __DEVICE_H__
#define __DEVICE_H__
/*
�ⲿ�˶�ģ��
�Զ���ͼ�����㷨 
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