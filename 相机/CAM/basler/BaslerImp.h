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

// 自定义回调函数的格式，返回数据unsigned char* pData可以根据需要自己更改
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
	CInstantCamera* m_pCamera; // 相机类
	CGrabResultPtr ptrGrabResult;
	int nSoftFlag;//采集状态：0 ：停止采集状态 1：采集状态
	int m_iFlag;//相机模式： 0：FreeRun   1:SoftWare  2：外部触发Line1
	int m_iType;//0:USB相机 1:IIDC 1394 and GigE camera devices.

	CPylonImage targetImage;
	CImageFormatConverter converter;
	HBITMAP bmp;
	CPylonImage image;
	CPylonBitmapImage destimage;

public:
	// 枚举出所有相机
	static long EnumerateCam(int& cameraNum ,vector<string>& cameraInfo);
	// 相机资源
	static void DestroyCamera();

	// 初始化相机，return 0 if 成功
	long Init(string &sCameraInfo);

	// 设置调用者和回调函数
	void SetOwner(void* pOwner, BaslerGrabbedCallback funcCallback);						
	void setCallbackFlag(bool bset);

	// 打开相机，return 0 if 成功
	long OpenCamera();

	// 设置相机的心跳时间，return 0 if 成功,这个值决定了软件debug断点可以停留的时间
	// 以及相机掉线多长时间以后，OnCameraDeviceRemoved 函数可以检测的到
	int SetHeartBeatTime(int64_t nValueMs);

	// 设置相机模式： 0：FreeRun   1:SoftWare  2：外部触发Line1
	long SetCameraMode(int iFlag);

	// 获取相机模式
	int  GetTriggerMode(){return m_iFlag;};

	// 设置相机参数,exposure & gain
	long SetParam(string sParma,string sParamValue);

	// 获取单张图像，三种相机模式中的一种
	long Grab(void* &pImageBuffer,size_t &nWidth,size_t &nHeight,size_t &nChannel);

	long Grab(void* &pImageBuffer,size_t &nWidth,size_t &nHeight,size_t &nChannel, size_t &nImageSize, EPixelType &ePixelType, uint32_t &nPaddingX);

	// 连续采集
	long GrabContiune();

	// 停止采集
	long Grabstop();

	// 关闭相机
	long CloseCamera();

	// 如果相机掉线，重新查找Atttach成功后，再重新进行open等一些列初始化操作
	int ReAttachCameraIfLost(void); 

	// 执行一次软触发命令
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
