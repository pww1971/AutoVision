#ifndef __BLCAMERA_H__
#define __BLCAMERA_H__
//��� �����ṩ�Ĵ���͹��� 
//���� �ṩ���սӿ�
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
	//�о������豸
	void enumBLDev();
	//�������
	BaslerImp* getCam(string ipname); 

	void release();
	
	bool takePic(BaslerImp* cam,cv::Mat& m);
public:
	vector<BaslerImp*> m_blcam;					//�����������
	map<string,int> m_mpCam;					//ip�󶨱��
	bool m_bIni;
protected:
	CResLock m_lock;
};

#endif

#endif