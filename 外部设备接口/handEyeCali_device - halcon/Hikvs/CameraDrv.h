#ifndef __CAMERADRV_H__
#define __CAMERADRV_H__
#ifdef _BASLER_CAM
#include "BaslerDrv.h"
#endif
#if (defined _OPENCV_CAM) || (defined _OPENCV_CAM_1) || (defined _OPENCV_CAM_2) 
#include "HKcamera.h"
#pragma comment(lib, "MvCameraControl.lib")
#endif
#include "CCameraCPP.h"
#include "pwwVision.h"
using namespace HalconCpp;
class CameraDrv
{
public:
	CameraDrv();
	~CameraDrv();

	//·µ»Ø-1Ê§°Ü
	virtual int grapImage(int id,cv::Mat& m);
	virtual int grapImage(string name,cv::Mat& m);
	virtual int ini();
	void release();
	int setCamType(int t);
public:
#if (defined _OPENCV_CAM) || (defined _OPENCV_CAM_1) || (defined _OPENCV_CAM_2) 
	HKvct m_hkCam;
#endif
#ifdef _BASLER_CAM
	BLvct m_blCam;
#endif

	cv::VideoCapture m_cap;

	int m_iType;
};

class HCameraDrv
{
public:
	HCameraDrv();
	~HCameraDrv();

	//·µ»Ø-1Ê§°Ü
	virtual int grapImage(int id, HImage& ho_Image);
	virtual int ini();
	void release();
	int setCamType(int t);
public:
	CCameraCPP m_hcam;

	int m_iType;
};


#endif
