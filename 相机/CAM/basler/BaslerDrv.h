#ifndef __BLCAMERA_H__
#define __BLCAMERA_H__
//相机 厂商提供的代码和功能 
//容器 提供拍照接口
#ifdef _BASLER_CAM
#ifdef WIN32
#include <stdio.h>
#include <windows.h>
#include <process.h>
#include <string.h>
#else
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#endif
#include "BaslerImp.h"
#include "camera.h"



class BLvct
{
public:
	BLvct();
	~BLvct();
	BaslerImp* getCam(int id);
	//列举所有设备
	void enumBLDev();
	//检索相机
	BaslerImp* getCam(string ipname); 

	void release();
	
	bool takePic(BaslerImp* cam,cv::Mat& m);
public:
	vector<BaslerImp*> m_blcam;					//海康相机容器
	map<string,int> m_mpCam;					//ip绑定编号
	bool m_bIni;
protected:
	CResLock m_lock;
};

#endif

#endif