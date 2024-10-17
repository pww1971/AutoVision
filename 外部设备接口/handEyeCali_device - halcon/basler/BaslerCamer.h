#pragma once
#ifdef _BASLER_CAM
#include "SampleImageEventHandler.h" //回调类头文件

#include <pylon/PylonIncludes.h> //Pylon头文件


#ifdef PYLON_WIN_BUILD
#include <pylon/PylonGUI.h>
#endif


using namespace std;
using namespace Pylon;//pylon名称空间

using namespace GenApi;//pylon SDK通过型写法名称空间

class BaslerCamer
{
public:
	BaslerCamer(void);
	~BaslerCamer(void);

public:
	CInstantCamera camera;//实例化相机对象
	
	INodeMap *nodemap;//相机属性节点

	CDeviceInfo di;//设备信息（用于UserID，SN号打开相机）

public:
	void CamInit();//相机初始化
	void CamStart();//开始取图
	void CamStop();//停止取图
	void CamDestroy();//退出时关闭相机

	string CamName();//获取相机名
	string CamSerialNum();//获取相机SN号
	string CamFormat();//获取相机图像格式
	double CamFrameRate();//获取相机帧率
	int CamWidth();//分辨率宽
	int CamHeight();//分辨率高

	double GetExpMin();//获取曝光最小值
	double GetExpMax();//获取曝光最大值
	double GetExpCur();//获取当前曝光值
	void SetExp(double ExpTime);//设置曝光值

	int GetGainMin();//获取增益最小值
	int GetGainMax();//获取增益最大值
	int GetGainCur();//获取当前增益值
	void SetGain(int GainValue);//设置增益值

	void SoftTrigEnable(string flag);//软触发使能
	void ExecuteSoftTrig();//执行软触发
	void ExtTrigEnable(string flag);//外触发使能
};
#endif