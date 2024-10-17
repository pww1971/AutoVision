basler  贝斯乐相机
CAM  海康相机
CAM_halcon  基于halcon的相机 包含畸变校正模块 可以获取相机内外参数配合机械手运动


宏定义：
_OPENCV_CAM 海康相机
_OPENCV_CAM_1 海康主播模式
_OPENCV_CAM_2 海康分机模式
_BASLER_CAM 贝斯勒相机
_SIMULATION 仿真相机
CALI	相机标定输出 不需要Cali时，将工程的Cali文件夹和checkpatch h和cpp文件移除 保证不使用halcon
HCAM CAM_halcon使用halcon的图像格式输出 可以在外部设备调用这个接口

没有海康和贝斯勒定义 就是自动采用opencv内部的usb相机打开模式
有仿真相机 则打开exe本地文件夹simulation里面的数字编号文件夹 01234 分别代表相机编号


雷神激光雷达 直接读取点云转深度图
LSLidar  读取点云转栅格地图 输出最新的栅格地图转深度图