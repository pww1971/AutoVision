#pragma once
#include<stdio.h>
#include<stdlib.h>
#include <iostream>
#include <map>
#ifdef _BASLER_CAM

#include <pylon/PylonIncludes.h>
#ifdef PYLON_WIN_BUILD
#    include <pylon/PylonGUI.h>
#endif

using namespace Pylon;
using namespace GenApi;
using namespace std;

static const size_t c_maxCamerasToUse = 20;

extern CInstantCameraArray cameras;

extern map<string, CInstantCamera*> mapCameras;

// �Զ���ص������ĸ�ʽ����������unsigned char* pData���Ը�����Ҫ�Լ�����
typedef void (WINAPI *BaslerGrabbedCallback)(void* pOwner,unsigned char* pData );

class BaslerImp:public CImageEventHandler, public CConfigurationEventHandler
{
public:
	BaslerImp(void);
	virtual ~BaslerImp(void);
public:
	void* m_pImageBuffer;
	size_t m_nWidth;
	size_t m_nHeight;
	size_t m_nChannel;
	HANDLE m_event;
	//callback ptr
	void*  m_pOwner;
	BaslerGrabbedCallback	m_fCallback;
	bool m_callbackFlag;
	virtual void OnImageGrabbed( CInstantCamera& camera, const CGrabResultPtr& ptrGrabResult);
	virtual void OnCameraDeviceRemoved( CInstantCamera& camera);

public:
	CInstantCamera* m_pCamera; // �����
	CGrabResultPtr ptrGrabResult;
	int nSoftFlag;//�ɼ�״̬��0 ��ֹͣ�ɼ�״̬ 1���ɼ�״̬
	int m_iFlag;//���ģʽ�� 0��FreeRun   1:SoftWare  2���ⲿ����Line1
	int m_iType;//0:USB��� 1:IIDC 1394 and GigE camera devices.

	CPylonImage targetImage;
	CImageFormatConverter converter;
	HBITMAP bmp;
	CPylonImage image;
	CPylonBitmapImage destimage;

public:
	// ö�ٳ��������
	static long EnumerateCam(int& cameraNum ,vector<string>& cameraInfo);
	// �����Դ
	static void DestroyCamera();

	// ��ʼ�������return 0 if �ɹ�
	long Init(string &sCameraInfo);

	// ���õ����ߺͻص�����
	void SetOwner(void* pOwner, BaslerGrabbedCallback funcCallback);						
	void setCallbackFlag(bool bset);

	// �������return 0 if �ɹ�
	long OpenCamera();

	// �������������ʱ�䣬return 0 if �ɹ�,���ֵ���������debug�ϵ����ͣ����ʱ��
	// �Լ�������߶೤ʱ���Ժ�OnCameraDeviceRemoved �������Լ��ĵ�
	int SetHeartBeatTime(int64_t nValueMs);

	// �������ģʽ�� 0��FreeRun   1:SoftWare  2���ⲿ����Line1
	long SetCameraMode(int iFlag);

	// ��ȡ���ģʽ
	int  GetTriggerMode(){return m_iFlag;};

	// �����������,exposure & gain
	long SetParam(string sParma,string sParamValue);

	// ��ȡ����ͼ���������ģʽ�е�һ��
	long Grab(void* &pImageBuffer,size_t &nWidth,size_t &nHeight,size_t &nChannel);

	long Grab(void* &pImageBuffer,size_t &nWidth,size_t &nHeight,size_t &nChannel, size_t &nImageSize, EPixelType &ePixelType, uint32_t &nPaddingX);

	// �����ɼ�
	long GrabContiune();

	// ֹͣ�ɼ�
	long Grabstop();

	// �ر����
	long CloseCamera();

	// ���������ߣ����²���Atttach�ɹ��������½���open��һЩ�г�ʼ������
	int ReAttachCameraIfLost(void); 

	// ִ��һ����������
	void ExecuteSoftTrig();

	bool SetWidth( int nWidth);

	bool SetHeight(int nHeight);

	int GetWidth();
	int GetHeight();

	int GetWidthMax();
	int GetHeightMax();

	long GetCameraName(string& cameraName);

	long GetCameraSerialNumber(string& cameraSerialNumber);

	int GetCameraInterfaceType(){return m_iType;};

private:
	int64_t Adjust(int64_t val, int64_t minimum, int64_t maximum, int64_t inc);
	void setFreeRunMode();
	void setSoftWareMode();
	void setHardWareMode();
	void setDefaultMode();// trigger mode off
	long IsContain(const string & nodeName,const string & sContain);
	long IsCurrentContain(const string & nodeName,const string & sContain);
	long IsMonoCamera();
	
};
#endif
