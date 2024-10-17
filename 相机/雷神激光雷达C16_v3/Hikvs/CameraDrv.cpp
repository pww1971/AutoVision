#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <IncludeFile.h>

#include "CameraDrv.h"


CameraDrv::CameraDrv()
{
	computer_ip = "192.168.1.102";
	angleL = 0;
	angleH = 2100;
	gridX = 2000;
	gridY = 16;
	gridSizeX = -1;
	gridSizeY = -1;
	minPoint = 0;
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
				computer_ip = szVal;
				m_GetLidarData->setPortAndIP(2368, 2369, computer_ip, "224.1.1.102");				//设置接收雷达的网口参数 （数据包端口号, 设备包端口号, 雷达的目的IP）
				m_GetLidarData->LidarStart();	
				ret = true;
			}
			szVal = cfg2.getItem("gridSizeX");
			if (szVal != "")
			{
				gridSizeX = atoi(szVal.c_str());
			}
			szVal = cfg2.getItem("minPoint");
			if (szVal != "")
			{
				minPoint = atof(szVal.c_str());
			}
		}
	}
	else
	{
		m_GetLidarData->setPortAndIP(2368, 2369, computer_ip, "224.1.1.102");				//设置接收雷达的网口参数 （数据包端口号, 设备包端口号, 雷达的目的IP）
		m_GetLidarData->LidarStart();
		ret = true;				//设置接收雷达的网口参数 （数据包端口号, 设备包端口号, 雷达的目的IP）
	}
	

//修改雷达参数 可单独修改某个参数 
	//std::this_thread::sleep_for(std::chrono::milliseconds(2000));			//先等待一段时间 获取设备包
	//m_GetLidarData->setLidarRotateSpeed(600);
	//m_GetLidarData->setLidarIP("192.168.1.200");
	//m_GetLidarData->setComputerIP("192.168.1.102");
	//m_GetLidarData->setDataPort(2368);
	//m_GetLidarData->setDevPort(2369);

	//m_GetLidarData->setLidarRotateState(0);
	//m_GetLidarData->setLidarSoureSelection(0);
	//m_GetLidarData->setLidarWorkState(0);

	//m_GetLidarData->sendPackUDP();										//设置完参数后，必须要调用 sendPackUDP 发送UDP包

	return ret;//开始解析雷达数据
}

int CameraDrv::grapImage(int id, vector<cv::Mat>& vm)//采图
{
	cv::Mat m = cv::Mat::zeros(gridY, angleH, CV_32FC3);
	//获取点云进行算法处理   
	//方法一 获取一帧雷达数据的方法
	int t;
	int tall = 1000;
	for (t = 0; t < tall; t++)
	{
		if (m_GetLidarData->isFrameOK)
		{
			std::vector<std::vector<LidarPointData>> m_getLidarData = m_GetLidarData->getLidarPerFrameDate();
						
			//输出点云点数
			for (size_t row = 0; row < m_getLidarData.size(); row++)//16
			{
				std::vector<LidarPointData>& m_LidarData_temp = m_getLidarData[row];
				int i = 0;
				int len = angleH < m_LidarData_temp.size()?  angleH:m_LidarData_temp.size();
				for (size_t col = 0; col < len; col++)//2000多
				{
					int pos = col+ angleL;
					if (pos >= m_LidarData_temp.size())
						pos = pos - m_LidarData_temp.size();
					if (gridSizeX == -1)
					{
						m.at<Vec3f>(row, i)[0] = m_LidarData_temp[pos].X;
						m.at<Vec3f>(row, i)[1] = m_LidarData_temp[pos].Y;
						m.at<Vec3f>(row, i++)[2] = m_LidarData_temp[pos].Z;
					}
					else 
					{
						int c = (m_LidarData_temp[pos].X- minPoint) / gridSizeX;
						m.at<Vec3f>(row, c)[0] = m_LidarData_temp[pos].X;
						m.at<Vec3f>(row, c)[1] = m_LidarData_temp[pos].Y;
						m.at<Vec3f>(row, c)[2] = m_LidarData_temp[pos].Z;
					}
				}
			}
			break;
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
	if(t == tall)
	{
		return -1;
	}

	if (m.elemSize1() > 1)//深度图
	{
		vm.push_back(cv::Mat());
		if (m.channels() == 1)
		{
			m.convertTo(vm[0], CV_8UC1);
			cv::cvtColor(vm[0], vm[0], cv::COLOR_GRAY2RGB);
		}
		else
			m.convertTo(vm[0], CV_8UC3);
		//if (m.elemSize1() == 2)//short 
		//{
		//	if (m.channels() == 1)
		//	{
		//		m.convertTo(m, CV_32FC1);//转为float
		//	}
		//	else
		//		m.convertTo(m, CV_32FC3);//转为float
		//}
	}
	vm.push_back(m);
	return 0;
}

int CameraDrv::ini()
{
	return iniCam();				//设置接收雷达的网口参数 （数据包端口号, 设备包端口号, 雷达的目的IP）
}

void  CameraDrv::release()
{
	m_GetLidarData->LidarStop();
	
}

int CameraDrv::setCamType(int t)
{
	return m_iType = t;
}
