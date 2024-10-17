#ifndef __CAMERADRV_H__
#define __CAMERADRV_H__

#include "pwwVision.h"

class CameraDrv
{
public:
	CameraDrv();
	~CameraDrv();
	bool iniCam(string str="");
	//返回-1失败
	virtual int grapImage(int id, vector<cv::Mat>& vm);
	virtual int ini();
	void release();
	int setCamType(int t);
public:
	std::string computer_ip;
	int angleL, angleH;//L位置 H范围
	int gridX,gridY;//栅格大小
	float gridSizeX, gridSizeY;//根据栅格大小计算的栅格尺寸
	double minPoint;//x最小点
	cv::VideoCapture m_cap;

	int m_iType;
};



#endif
