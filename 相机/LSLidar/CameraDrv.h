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
	//����-1ʧ�� ���դ���ͼ
	virtual int grapImage(int id, vector<cv::Mat>& vm);//id���� Ŀǰ��֧��һ�������״� �����ṩ��api��֧��
	virtual int ini();
	void release();
	int setCamType(int t);
	int getGridMap(mrpt::math::TPoint3D& sensorPos);//�ɼ�������Ƹ���դ���ͼ
	void clearMap();
	int grapImage(cv::Mat& m);//ֱ�ӴӼ����״��������ͼ ������դ���ͼ
public:
	std::string m_computerip;//�״��ַ
	int m_angleL, m_angleH;//Lλ�� H��Χ �״�ɨ�跶Χ
	int m_LidarLineNum;//�״�����
	mrpt::math::TPoint3D m_corner_min, m_corner_max;//դ���ͼ��С
	mrpt::math::TPoint3D m_sensorCenter;//�����״��ڵ�ͼ�е�λ��
	float m_gridXY,m_gridZ;//դ��ߴ�
	mrpt::maps::COccupancyGridMap3D m_gridmap;//դ���ͼ
	CResLock  m_gmapLock;
	int m_iType;
};


#endif
