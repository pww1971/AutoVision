#ifndef __CAMERADRV_H__
#define __CAMERADRV_H__

#include "pwwVision.h"
#include <mrpt/maps/COccupancyGridMap3D.h>
#include <mrpt/maps/CSimplePointsMap.h>

class CameraDrv
{
public:
	CameraDrv();
	~CameraDrv();
	bool iniCam(string str="");
	void wirteParam();
	//返回-1失败 输出栅格地图
	virtual int grapImage(int id, vector<cv::Mat>& vm);//id无用 目前就支持一个激光雷达 厂商提供的api不支持
	virtual int ini();
	void release();
	int setCamType(int t);
	int getGridMap(mrpt::math::TPoint3D& sensorPos);//采集激光点云更新栅格地图
	void clearMap();
	int grapImage(cv::Mat& m);//直接从激光雷达输出点云图 不更新栅格地图
public:
	std::string m_computerip;//雷达地址
	int m_angleL, m_angleH;//L位置 H范围 雷达扫描范围
	int m_LidarLineNum;//雷达线数
	mrpt::math::TPoint3D m_corner_min, m_corner_max;//栅格地图大小
	mrpt::math::TPoint3D m_sensorCenter;//激光雷达在地图中的位置
	float m_gridXY,m_gridZ;//栅格尺寸
	mrpt::maps::COccupancyGridMap3D m_gridmap;//栅格地图
	CResLock  m_gmapLock;
	int m_iType;
};


#endif
