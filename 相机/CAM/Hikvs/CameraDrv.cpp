
#include "CameraDrv.h"

CameraDrv::CameraDrv()
{
	m_iType = -1;
	_putenv("OPENCV_VIDEOIO_MSMF_ENABLE_HW_TRANSFORMS=0");
#if (defined _OPENCV_CAM) || (defined _OPENCV_CAM_1) || (defined _OPENCV_CAM_2) 
	m_iType = 0;
#if (defined _OPENCV_CAM_2) 
	m_hkCam.enumHKDev();
#endif
#endif

#ifdef _BASLER_CAM
	m_iType = 1;
	m_blCam.enumBLDev();
#endif
}

CameraDrv::~CameraDrv()
{

}

int CameraDrv::grapImage(int id, vector<cv::Mat>& vm)//针对海康组播模式 仅仅触发主机的相机 不采图
{
#ifdef _SIMULATION

	string dir = getExePath() + "simulation/" + CStrProcess::itoa_radixN(id) + "/";
	vector<string> vct;
	getFileNameVec(dir.c_str(), vct);
	if (vct.size() == 0)
	{
		return -1;
	}
	static u_int sid = 0;
	cv::Mat m = cv::imread(dir + vct[sid++ % vct.size()], IMREAD_UNCHANGED);
	if (m.empty())//一般相机错误 只会出现图像质量问题 m不会空 仿真可以直接用绿色图 
		m = cv::Mat(1500, 1500, CV_8UC3, cv::Scalar(0, 255, 0));//return -1;
	if (m.elemSize1() > 1)//深度图
	{
		vm.push_back(cv::Mat());
		//转rgb图
		vm[0] = cv::Mat::zeros(m.rows, m.cols, CV_8UC1);
		float maxValue = *max_element(m.begin<float>(), m.end<float>());
		for (int i = 0; i < m.rows; i++)
		{
			float* pt = (float*)m.data + i * m.cols;
			uchar* ut = vm[0].data + i * m.cols;
			for (int j = 0; j < m.cols; j++)
			{
				*(ut + j) = *(pt + j) * 255 / maxValue;
			}
}
		cv::cvtColor(vm[0], vm[0], cv::COLOR_GRAY2RGB);
	}
	vm.push_back(m);

	return 0;
#endif
	vm.push_back(cv::Mat());
	if (m_iType == -1)
	{
		if(!m_cap.isOpened())
			m_cap.open(id, cv::CAP_MSMF);
		if(!m_cap.isOpened())
			return -1;
		m_cap>> vm[0];
		if(vm[0].empty())
			return -1;
		return 0;
	}
	else if (m_iType == 0)
	{
#if (defined _OPENCV_CAM) || (defined _OPENCV_CAM_1) || (defined _OPENCV_CAM_2) 
		m_hkCam.enumHKDev();
		CHKCamera* cam = m_hkCam.getHK(id);
		if (NULL != cam && cam->m_id != -1)//ini error or no know cam
		{
			return cam->grapImage(vm[0]);
		}
#endif
		return -1;
	}
	else
	{
#ifdef _BASLER_CAM
		m_blCam.enumBLDev();
		BaslerImp* cam = m_blCam.getCam(id);
		if (m_blCam.takePic(cam, vm[0]) )//ini error or no know cam
		{
			return 1;
		}
#endif
		return -1;
	}
}

int CameraDrv::ini()
{
#ifdef _SIMULATION
	string dir = getExePath() + "simulation\\";
	vector<string> vct;
	getFileNameDir(dir.c_str(), vct);
	return vct.size();
#endif

	if (m_iType == 0)
	{
#if (defined _OPENCV_CAM) || (defined _OPENCV_CAM_1) || (defined _OPENCV_CAM_2) 
		m_hkCam.enumHKDev();
		return m_hkCam.m_stDevList.nDeviceNum;
#endif
	}
	else
	{
#ifdef _BASLER_CAM
		m_blCam.enumBLDev();
		return m_blCam.m_blcam.size();
#endif
	}
	return 1;
}

void  CameraDrv::release()
{
	if (m_iType == -1)
	{
		if (m_cap.isOpened())
			m_cap.release();
	}
	else if (m_iType == 0)
	{
#if (defined _OPENCV_CAM) || (defined _OPENCV_CAM_1) || (defined _OPENCV_CAM_2) 
		m_hkCam.release();
#endif
	}
	else
	{
#ifdef _BASLER_CAM
		m_blCam.release();
#endif
	}
}

int CameraDrv::setCamType(int t)
{
	return m_iType = t;
}
