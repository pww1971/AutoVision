#pragma once
/******************** (C) COPYRIGHT 2021 深圳市镭神智能系统有限公司 *****************
* File Name         : LSLidarDemo.h
* Author            : LSJ
* Version           : v2.1.0
* Date              : 2023/09/18
* Description       : 初版
* Modify Record		:
* Instruction		:
*******************************************************************************/

#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <cmath>
#include <chrono>
#include <cstring>
#include <functional>
#include <string>
#include <regex>
#include <memory>

#ifdef LINUX
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
typedef uint64_t u_int64;
#else
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
typedef unsigned __int64 u_int64;

#define timegm _mkgmtime
#endif

#define PI 3.1415926
#define ConversionUnit	100.0			//单位转换 厘米转换成 米 

typedef struct _Point_XYZ
{
	float x1 = 0.0;
	float y1 = 0.0;
	float z1 = 0.0;

	float x2 = 0.0;
	float y2 = 0.0;
	float z2 = 0.0;
}m_PointXYZ;

typedef struct _MuchLidarData
{
	float X = 0.0;
	float Y = 0.0;
	float Z = 0.0;
	int ID = 0;
	float H_angle = 0.0;
	float V_angle = 0.0;
	float Distance = 0.0;
	int Intensity = 0;
	u_int64 Mtimestamp_nsce = 0;
}MuchLidarData;

typedef struct _LidarPointData
{
	float X = 0.0;
	float Y = 0.0;
	float Z = 0.0;
	int ID = 0;
	int Intensity = 0;
}LidarPointData;

typedef struct _UTC_Time
{
	int year = 2000;
	int month = 0;
	int day = 0;
	int hour = 0;
	int minute = 0;
	int second = 0;
}UTC_Time;

typedef std::function<void(std::vector<std::vector<LidarPointData>>, int)> FunDataPrt;

class GetLidarData
{
public:
	GetLidarData();
	~GetLidarData();

	bool isFrameOK = false;
	int isDormant = 0;                                                   //雷达是否处于低功耗状态，0为正常模式， 1为低功耗模式 
	bool isFrames = true;                                               //帧同步判断，正常为true
	bool isHighVoltage = true;                                          //电压是否异常，正常为true


	/*
		功能: 初始化 数据包端口、设备端口以及目的IP
		参数1: mDataPort	数据包端口		默认为 2368
		参数2: mDevPort		设备包端口		默认为 2369
		参数3: mDestIP		目的IP			默认为 "192.168.1.102"
		参数3: mGroupIp		组播IP			默认为 "224.1.1.102"
		返回值: 无
	*/
	void setPortAndIP(uint16_t mDataPort = 2368, uint16_t mDevPort = 2369, std::string mDestIP = "192.168.1.102", std::string mGroupIp = "224.1.1.102");

	/*
		功能: 传输回调函数，获取雷达数据
		参数1: FunDataPrt*	 传入回调函数指针
		返回值: 无
	*/
	void setCallbackFunction(FunDataPrt*);																//设置点云数据回调函数

	/*
		 功能:开始解析雷达数据
		 参数: 无
		 返回值: 无
	 */
	void LidarStart();

	/*
	 功能:停止解析雷达数据
	 参数: 无
	 返回值: 无
	*/
	void LidarStop();

	/*
		 功能:开启离线网络数据包数据解析 
		 参数: 无
		 返回值: 无
	*/
	void LidarOfflineDataStar();



	/*
	 功能:获取一帧的雷达数据
	 参数: 无
	 返回值: std::vector<std::vector<LidarPointData>> 获取一帧数据点云
	*/
	std::vector<std::vector<LidarPointData>> getLidarPerFrameDate();

	virtual void LidarRun() = 0;
	void CollectionDataArrive(void* pData, uint16_t len);										//把UDP的数据传入到此处

public:

#pragma region //设置雷达参数 
	 unsigned char dataDev[1206];															//接收设备包
	 unsigned char Rest_UCWP_buff[1206];												    //修改配置包
	 std::string ip_sa;																		//发送的雷达IP
	 int m_SpeedValue = 0;                                                                 //设置雷达的转速
	 bool is_speedFlag = false;                                                            //是否开启转速检测，true为开启
	 bool is_speedNormal = true;                                                             //转速是否正常，true为正常                                         

	 /*
	 功能: 修改雷达转速
	 参数: SpeedValue				转速值， 支持300， 600，1200
	 返回值: true 操作成功,  false 操作失败
	 */
	virtual bool setLidarRotateSpeed(int SpeedValue);												//设置转速

	/*
	功能: 修改雷达IP
	参数: IPString	输入要修改IP4地址，  如: "192.168.1.200"
	返回值:  true 操作成功,  false 操作失败
	*/
	virtual bool setLidarIP(std::string IPString);													//设置雷达IP

	/*
	功能: 修改雷达目的IP（即电脑IP）
	参数: IPString	输入要修改IP4地址，  如: "192.168.1.102"
	返回值:  true 操作成功,  false 操作失败
	*/
	virtual bool setComputerIP(std::string IPString);												//设置电脑IP
	
	/*
	功能: 修改雷达数据包端口
	参数: PortNum	输入雷达的数据端口， 默认可输入0~~65536（推荐使用2000以后的端口）
	返回值:  true 操作成功,  false 操作失败
	*/
	virtual bool setDataPort(int PortNum);															//设置数据包端口
	
	/*
	功能: 修改雷达设备包端口
	参数: PortNum	输入雷达的数据端口， 默认可输入0~~65536（推荐使用2000以后的端口）
	返回值:  true 操作成功,  false 操作失败
	*/
	virtual bool setDevPort(int PortNum);															//设置设备包端口
	
	/*
	功能: 修改雷达电机静止或旋转
	参数: StateValue	输入雷达电机模式， 默认0为旋转， 1为静止
	返回值:  true 操作成功,  false 操作失败
	*/
	virtual bool setLidarRotateState(int StateValue);												//雷达旋转/静止

	/*
	功能: 修改时钟源选择
	参数: selectValue	输入雷达模式， 默认0为GPS， 1为PTP   2为NTP
	返回值:  true 操作成功,  false 操作失败
	*/
	virtual bool setLidarSoureSelection(int StateValue);											//设置时钟源的选择

	/*
	功能: 修改雷达低功耗模式 （只发设备包 不发数据包）
	参数: selectValue	输入雷达模式， 默认0为正常模式， 1为低功耗模式
	返回值:  true 操作成功,  false 操作失败
	*/
	virtual bool  setLidarWorkState(int StateValue);												//雷达状态  是否是低功耗

	/*
	功能: 发送UDP包 雷达数据
	参数: 无
	返回值:  true 操作成功,  false 操作失败
	*/
	virtual bool sendPackUDP();																		//发送UDP包

	bool setLidarParam();																			//设置组包
	bool sendPacketToLidar(unsigned char* packet, const char* ip_data, u_short portdata);			//通过套接字发包
#pragma endregion 

protected:
	bool isQuit = false;																		//雷达退出的标记
	std::queue<unsigned char *> allDataValue;													//数据缓存队列
	std::mutex m_mutex;																			//锁

	u_int64 timestamp_nsce;																		//包内秒以下时间戳（单位ns）
	u_int64 allTimestamp;																		//包内的时间戳保存
	u_int64 lastAllTimestamp = 0;																//保存上一包的时间戳
	double PointIntervalT_ns;																	//包内每个点之间的时间间隔	单位为纳秒

	std::vector<std::vector<LidarPointData>> LidarPerFrameDatePrt_Get;						// 获取数据
	std::vector<std::vector<LidarPointData>> LidarPerFrameDatePrt_Send;						// 发送数据

	UTC_Time m_UTC_Time;																		//获取设备包的GPS信息
	void clearQueue(std::queue<unsigned char *>& m_queue);										//清空队列
	void sendLidarData();																		//数据打包发送
	FunDataPrt *callback = NULL;																//回调函数指针 -回调数据包
    std::string time_service_mode = "gps";                                                     //授时方式

	void messFunction(std::string strValue, int gValue);
	//检查源IP 是否符合
	bool checkDefaultIP(std::vector<std::string> m_DefaultIP);
	//检查目的IP是否符合
	bool checkDestIP(std::vector<std::string> m_DestIP);

	//启动线程获取数据包
	void getDataSock();		
	//启动线程获取设备包
	void getDevSock();    

	//间隔段时间后开启转速判断
	void sleepTime();
	//开始转速判断
	void startSleepThread();

private:
	std::vector<std::vector<LidarPointData>> LidarPerFrameDatePer;
	bool islidarDevCome = false;													//判断是否获取雷达设备包
	bool isSendUDP = true;

	int dataPort = 2368;															//数据包端口号
	int devPort = 2369;																//设备包端口号
	std::string computerIP = "192.168.1.102";										//雷达的目的IP (电脑IP)
	std::string groupIp = "224.1.1.102";											 //定义组播IP

};
