#ifndef __HKCAMERA_H__
#define __HKCAMERA_H__
//海康相机 厂商提供的代码和功能 
//海康容器 提供拍照接口
#include "pwwVision.h"
#if (defined _OPENCV_CAM) || (defined _OPENCV_CAM_1) || (defined _OPENCV_CAM_2) 
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
#include "MvCameraControl.h"
#include "camera.h"

class CHKCamera:public CCamera
{
public:
	CHKCamera();
	~CHKCamera();

	//相机抓图
	virtual int grapImage(cv::Mat& m);
	virtual int grapImage(CoordinatePosXYZ& cor,S_Msg*& pMsg);
	//释放相机驱动
	void release();
	/*相机驱动初始化，
	参数：nIndex设备标记 
	返回值：-1 错误，0 正常
	*/
	virtual int ini(unsigned int nIndex,MV_CC_DEVICE_INFO_LIST& stDeviceList);
	int iniGrabbing();
	int setRoi();
	int setRoi(cv::Rect& rt);
	bool getRoi(cv::Rect& rct,cv::Size& sz);
	bool getRawSize(cv::Rect& rt);
	bool getCamState();

	void setTriggerMode(_MV_CAM_TRIGGER_MODE_ mode);
	void SetTriggerSource(bool bSoftWareTriggerCheck);
	void setSoftTriggerOnce();
public:
	bool isColor;
	int m_id;				//相机编号
	int m_monitorMode;//相机采集模式  0 独占 1组播控制 2组播接收
private:
	void* handle;
	unsigned int nBufSizeForSaveImage;
	unsigned char * pBufForDriver;
	int nBufSizeForDriver;
	MV_FRAME_OUT_INFO_EX stImageInfo;
	MV_CC_DEVICE_INFO*      m_pDeviceInfo;
};

class HKvct
{
public:
	HKvct();
	~HKvct();
	CHKCamera* getHK(int id);
	//列举所有设备
	void enumHKDev();
	//检索相机
	CHKCamera* getHK(string ipname); 
	//获取相机状态
	bool getCamState(int id);

	void release();

	
public:
	vector<CHKCamera*> m_hkcam;					//海康相机容器
	map<string,int> m_mpCam;					//ip绑定编号

	MV_CC_DEVICE_INFO_LIST m_stDevList;         // 设备信息列表结构体变量，用来存储设备列表
protected:
	bool m_bIni;
	CResLock m_lock;
};
#endif
#endif