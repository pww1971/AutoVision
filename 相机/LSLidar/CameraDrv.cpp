#include "pch.h"
#include <IncludeFile.h>

#include "CameraDrv.h"

CameraDrv::CameraDrv()
{
	m_computerip = "192.168.1.102";
	m_angleL = 0;
	m_angleH = 2100;
	m_LidarLineNum = 16;
	m_gridXY = 0.1;
	m_gridZ = 0.1;
	m_corner_min = mrpt::math::TPoint3D(-1, -1, -1);
	m_corner_max = mrpt::math::TPoint3D(1, 1, 1);
	m_sensorCenter = mrpt::math::TPoint3D(0, 0, 0);
	m_iType = -1;
}

CameraDrv::~CameraDrv()
{
	
}
bool CameraDrv::iniCam(string str)
{
	bool ret = false;
	if ("" == str) 
	{
		CConfig cfg(getExePath() + PROCCFG);
		string szVal = cfg.getItem("CurDir");
		if (szVal != "")
		{
			szVal = szVal + "\\cam\\" + CStrProcess::itoa_radixN(0) + "\\LSLIDAR.txt";
			CConfig cfg2(szVal);
			szVal = cfg2.getItem("IPADD");
			if (szVal != "")
			{
				m_computerip = szVal;
				m_GetLidarData->setPortAndIP(2368, 2369, m_computerip, "224.1.1.102");				//���ý����״�����ڲ��� �����ݰ��˿ں�, �豸���˿ں�, �״��Ŀ��IP��
				m_GetLidarData->LidarStart();	
				ret = true;
			}
			szVal = cfg2.getItem("gridXY");
			if (szVal != "")
			{
				m_gridXY = atof(szVal.c_str());
			}
			szVal = cfg2.getItem("gridZ");
			if (szVal != "")
			{
				m_gridZ = atof(szVal.c_str());
			}
			szVal = cfg2.getItem("corner_min");
			if (szVal != "")
			{
				vector<string> vct;
				CStrProcess::Split(szVal, ",", vct, true);
				if (vct.size() == 3)
				{
					float x = atof(vct[0].c_str());
					float y = atof(vct[1].c_str());
					float z = atof(vct[2].c_str());
					m_corner_min = mrpt::math::TPoint3D(x, y, z);
				}
			}
			szVal = cfg2.getItem("corner_max");
			if (szVal != "")
			{
				vector<string> vct;
				CStrProcess::Split(szVal, ",", vct, true);
				if (vct.size() == 3)
				{
					float x = atof(vct[0].c_str());
					float y = atof(vct[1].c_str());
					float z = atof(vct[2].c_str());
					m_corner_max = mrpt::math::TPoint3D(x, y, z);
				}
			}
			m_gmapLock.lock();
			m_gridmap.setSize(m_corner_min, m_corner_max, m_gridXY);
			m_gmapLock.unLock();
			szVal = cfg2.getItem("sensorCenter");
			if (szVal != "")
			{
				vector<string> vct;
				CStrProcess::Split(szVal, ",", vct, true);
				if (vct.size() == 3)
				{
					float x = atof(vct[0].c_str());
					float y = atof(vct[1].c_str());
					float z = atof(vct[2].c_str());
					m_sensorCenter = mrpt::math::TPoint3D(x, y, z);
				}
			}
			szVal = cfg2.getItem("angleScope");
			if (szVal != "")
			{
				vector<string> vct;
				CStrProcess::Split(szVal, ",", vct, true);
				if (vct.size() == 2)
				{
					m_angleL = atoi(vct[0].c_str());
					m_angleH = atoi(vct[1].c_str());
				}
			}
		}
	}
	else
	{
		m_GetLidarData->setPortAndIP(2368, 2369, m_computerip, "224.1.1.102");				//���ý����״�����ڲ��� �����ݰ��˿ں�, �豸���˿ں�, �״��Ŀ��IP��
		m_GetLidarData->LidarStart();
		ret = true;				//���ý����״�����ڲ��� �����ݰ��˿ں�, �豸���˿ں�, �״��Ŀ��IP��
	}
	

//�޸��״���� �ɵ����޸�ĳ������ 
	//std::this_thread::sleep_for(std::chrono::milliseconds(2000));			//�ȵȴ�һ��ʱ�� ��ȡ�豸��
	//m_GetLidarData->setLidarRotateSpeed(600);
	//m_GetLidarData->setLidarIP("192.168.1.200");
	//m_GetLidarData->setComputerIP("192.168.1.102");
	//m_GetLidarData->setDataPort(2368);
	//m_GetLidarData->setDevPort(2369);

	//m_GetLidarData->setLidarRotateState(0);
	//m_GetLidarData->setLidarSoureSelection(0);
	//m_GetLidarData->setLidarWorkState(0);

	//m_GetLidarData->sendPackUDP();										//����������󣬱���Ҫ���� sendPackUDP ����UDP��

	return ret;//��ʼ�����״�����
}
void CameraDrv::wirteParam()
{
	CConfig cfg(getExePath() + PROCCFG);
	string szVal = cfg.getItem("CurDir");
	if (szVal != "")
	{
		szVal = szVal + "\\cam\\" + CStrProcess::itoa_radixN(0) + "\\LSLIDAR.txt";
		CConfig cfg2(szVal);
		cfg2.updateItem("IPADD", m_computerip);
		char sz[32];
		sprintf(sz, "%f", m_gridXY);
		cfg2.updateItem("gridXY", sz);
		sprintf(sz, "%f", m_gridZ);
		cfg2.updateItem("gridZ", sz);
		sprintf(sz, "%f,%f,%f", m_corner_min.x, m_corner_min.y, m_corner_min.z);
		cfg2.updateItem("corner_min", sz);
		sprintf(sz, "%f,%f,%f", m_corner_max.x, m_corner_max.y, m_corner_max.z);
		cfg2.updateItem("corner_max", sz);
		sprintf(sz, "%f,%f,%f", m_sensorCenter.x, m_sensorCenter.y, m_sensorCenter.z);
		cfg2.updateItem("sensorCenter", sz);
		sprintf(sz, "%d,%d", m_angleL, m_angleH);
		cfg2.updateItem("angleScope", sz);

	}
}

int CameraDrv::grapImage(int id, vector<cv::Mat>& vm)//��ͼ
{
	m_gmapLock.lock();
	size_t il = m_gridmap.m_grid.getSizeX();
	size_t jl = m_gridmap.m_grid.getSizeY();
	size_t kl = m_gridmap.m_grid.getSizeZ();
	cv::Mat deep = cv::Mat::zeros(jl, il, CV_32FC3);
	cv::Mat rgb = cv::Mat::zeros(jl, il, CV_8UC1);
	float dv = kl * m_gridmap.m_grid.getResolutionZ();
	for (int i = 0; i < il; i++)
	{
		float* pt = (float*)deep.data + i * jl * deep.channels();
		uchar* ut = rgb.data + i * jl;
		for (int j = 0; j < jl; j++)
		{
			for (int k = kl - 1; k > -1; k--)
			{
				//if (gridmap.m_grid.cellByIndex(i, j, k) != nullptr)
				if (m_gridmap.getCellFreeness(i, j, k) > 0.5)
				{
					float* deepPtr = pt + j * deep.channels();
					*deepPtr = i * m_gridmap.m_grid.getResolutionXY();
					*(deepPtr + 1) = j * m_gridmap.m_grid.getResolutionXY();
					*(deepPtr + 2) = k * m_gridmap.m_grid.getResolutionZ();
					*(ut + j) = *(deepPtr + 2) * 255 / dv;
					break;
				}
			}
		}
	}
	//cv::imwrite("gridmap.tiff", deep);
	m_gmapLock.unLock();
	
	cv::cvtColor(rgb, rgb, cv::COLOR_GRAY2RGB);
	int idx = m_gridmap.m_grid.x2idx(m_sensorCenter.x);
	int idy = m_gridmap.m_grid.y2idx(m_sensorCenter.y);
	drawMarker(rgb, Point(idx,idy), Scalar(0, 0, 255), MARKER_STAR, 20, 1, 8);
	vm.push_back(rgb);
	vm.push_back(deep);
	
	return 0;
}

int CameraDrv::ini()
{
	return iniCam();				//���ý����״�����ڲ��� �����ݰ��˿ں�, �豸���˿ں�, �״��Ŀ��IP��
}

void  CameraDrv::release()
{
	m_GetLidarData->LidarStop();
	
}

int CameraDrv::setCamType(int t)
{
	return m_iType = t;
}

int CameraDrv::getGridMap(mrpt::math::TPoint3D& sensorPos)
{
	m_sensorCenter = sensorPos;//����λ��
	mrpt::maps::CSimplePointsMap data;//����
	data.reserve(m_LidarLineNum * m_angleH);
	//��ȡ���ƹ���դ���ͼ 
	//����һ ��ȡһ֡�״����ݵķ���
	int t;
	int tall = 1000;
	for (t = 0; t < tall; t++)
	{
		if (m_GetLidarData->isFrameOK)
		{
			std::vector<std::vector<LidarPointData>> m_getLidarData = m_GetLidarData->getLidarPerFrameDate();

			//������Ƶ���
			for (size_t row = 0; row < m_getLidarData.size(); row++)//16
			{
				std::vector<LidarPointData>& m_LidarData_temp = m_getLidarData[row];
				int i = 0;
				int len = m_angleH < m_LidarData_temp.size() ? m_angleH : m_LidarData_temp.size();
				for (size_t col = 0; col < len; col++)//2000��
				{
					int pos = col + m_angleL;
					if (pos >= m_LidarData_temp.size())
						pos = pos - m_LidarData_temp.size();
					data.insertPointFast(m_LidarData_temp[pos].X, m_LidarData_temp[pos].Y, m_LidarData_temp[pos].Z);
				}
			}
			m_gmapLock.lock();
			m_gridmap.insertPointCloud(m_sensorCenter, data);
			m_gmapLock.unLock();
			break;
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
	if (t == tall)
	{
		return -1;
	}
	return 0;
}

void CameraDrv::clearMap()
{
	m_gmapLock.lock();
	m_gridmap.clear();
	m_gmapLock.unLock();
}

int CameraDrv::grapImage(cv::Mat& m)//��ͼ
{
	m = cv::Mat::zeros(m_LidarLineNum, m_angleH, CV_32FC3);
	//��ȡ���ƽ����㷨����   
	//����һ ��ȡһ֡�״����ݵķ���
	int t;
	int tall = 1000;
	for (t = 0; t < tall; t++)
	{
		if (m_GetLidarData->isFrameOK)
		{
			std::vector<std::vector<LidarPointData>> m_getLidarData = m_GetLidarData->getLidarPerFrameDate();

			//������Ƶ���
			for (size_t row = 0; row < m_getLidarData.size(); row++)//16
			{
				std::vector<LidarPointData>& m_LidarData_temp = m_getLidarData[row];
				int i = 0;
				int len = m_angleH < m_LidarData_temp.size() ? m_angleH : m_LidarData_temp.size();
				for (size_t col = 0; col < len; col++)//2000��
				{
					int pos = col + m_angleL;
					if (pos >= m_LidarData_temp.size())
						pos = pos - m_LidarData_temp.size();

					m.at<Vec3f>(row, i)[0] = m_LidarData_temp[pos].X;
					m.at<Vec3f>(row, i)[1] = m_LidarData_temp[pos].Y;
					m.at<Vec3f>(row, i++)[2] = m_LidarData_temp[pos].Z;

				}
			}
			break;
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
	if (t == tall)
	{
		return -1;
	}

	return 0;
}
