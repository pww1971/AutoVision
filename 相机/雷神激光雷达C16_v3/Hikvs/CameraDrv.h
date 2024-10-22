#ifndef __CAMERADRV_H__
#define __CAMERADRV_H__

#include "pwwVision.h"

class CameraDrv
{
public:
	CameraDrv();
	~CameraDrv();
	bool iniCam(string str="");
	//����-1ʧ��
	virtual int grapImage(int id, vector<cv::Mat>& vm);
	virtual int ini();
	void release();
	int setCamType(int t);
public:
	std::string computer_ip;
	int angleL, angleH;//Lλ�� H��Χ
	int gridX,gridY;//դ���С
	float gridSizeX, gridSizeY;//����դ���С�����դ��ߴ�
	double minPoint;//x��С��
	cv::VideoCapture m_cap;

	int m_iType;
};



#endif
