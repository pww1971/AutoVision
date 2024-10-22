#ifndef __HKCAMERA_H__
#define __HKCAMERA_H__
//������� �����ṩ�Ĵ���͹��� 
//�������� �ṩ���սӿ�
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

	//���ץͼ
	virtual int grapImage(cv::Mat& m);
	virtual int grapImage(CoordinatePosXYZ& cor,S_Msg*& pMsg);
	//�ͷ��������
	void release();
	/*���������ʼ����
	������nIndex�豸��� 
	����ֵ��-1 ����0 ����
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
	int m_id;				//������
	int m_monitorMode;//����ɼ�ģʽ  0 ��ռ 1�鲥���� 2�鲥����
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
	//�о������豸
	void enumHKDev();
	//�������
	CHKCamera* getHK(string ipname); 
	//��ȡ���״̬
	bool getCamState(int id);

	void release();

	
public:
	vector<CHKCamera*> m_hkcam;					//�����������
	map<string,int> m_mpCam;					//ip�󶨱��

	MV_CC_DEVICE_INFO_LIST m_stDevList;         // �豸��Ϣ�б�ṹ������������洢�豸�б�
protected:
	bool m_bIni;
	CResLock m_lock;
};
#endif
#endif