#pragma once
#ifdef _BASLER_CAM
#include <pylon/PylonIncludes.h> //添加Pylon头文件


#ifdef PYLON_WIN_BUILD
#include <pylon/PylonGUI.h> //PylonGUI显示
#endif

using namespace Pylon;//Pylon名称空间

class SampleImageEventHandler : public CImageEventHandler
{
public:
	SampleImageEventHandler(void);
	~SampleImageEventHandler(void);
public:
	int CWidth;//图像数据宽
	int CHeight;//图像数据高
public:
	virtual void OnImageGrabbed(CInstantCamera& camera, const CGrabResultPtr& grabResult);//回调函数
};
#endif