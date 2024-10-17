注意：如果调用halcon算子，需要在程序入口处添加如下代码和头文件
#include <HalconCpp.h>
using namespace HalconCpp;
#pragma comment( lib, "../../src\\halcon\\Lib\\x64-win64\\HalconCpp.lib")

 SetSystem("use_window_thread", "true");
 SetSystem("parallelize_operators", "true");

cmvComm.h 和cmvComm.cpp是进程通讯飞拍的模块 


dllcheck 检测的dll 提供给MFCApplication1调用。
MFCApplication1 界面程序调用制程和检测dll的例子
MFCApplication2 界面程序调用运动和检测的例子 采用静态库的调用方式
MFCApplication3 pwwMap大数据网络服务器的例子
test2  无界面程序 调用运动和检测的例子 采用静态库的调用方式 同时里面包含pwwMap客户端例子
QtWinApp qt调用mfc静态库的例子
QtWinApp2 qt调用mfc动态库dll的例子
CMMV-master 开源CMV的halcon程序改编 嵌入我的进程通讯和运动控制模块和dxf文件导入模块 