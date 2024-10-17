#include "BaslerDrv.h"

#ifdef _BASLER_CAM


BLvct::BLvct()
{
	m_bIni = false;
}

BLvct::~BLvct()
{
	release();
}

void BLvct::release()
{
#ifdef _SIMULATION
	return;
#endif
	vector<BaslerImp*>::iterator it = m_blcam.begin();
	for (; it != m_blcam.end(); ++it)
	{
		(*it)->Grabstop();
		(*it)->CloseCamera();
		delete (*it);
	}
	m_blcam.clear();
	m_mpCam.clear();

	BaslerImp::DestroyCamera();
	m_bIni = false;
}

BaslerImp* BLvct::getCam(int id)
{
	if (id >= m_blcam.size())
	{
		return NULL;
	}
	BaslerImp* cam = NULL;
	cam = m_blcam[id];
	return cam;
}

// 按下查找设备按钮:枚举
void BLvct::enumBLDev()
{
	m_lock.lock();
	if (m_bIni == true)
	{
		m_lock.unLock();
		return;
	}

	int camNum=0;
	vector<string> lstCamerInfo ;
	long rst = BaslerImp::EnumerateCam(camNum,lstCamerInfo);
	if(lstCamerInfo.size()<1)
	{
		m_lock.unLock();
		return;
	}
	for (int i=0;i<camNum;i++)
	{
		BaslerImp* pCamera = new BaslerImp();
		if (0==pCamera->Init(lstCamerInfo[i]))
		{
			if (0==pCamera->OpenCamera())
			{
				pCamera->SetCameraMode(1);//
				m_mpCam.insert(pair<string,int>(lstCamerInfo[i],m_blcam.size()));
				m_blcam.push_back(pCamera);
			}
		}
	}
	m_bIni = true;
	m_lock.unLock();
}

BaslerImp* BLvct::getCam(string ipname)
{
	map<string,int>::iterator it = m_mpCam.find(ipname);
	if (it==m_mpCam.end())
		return NULL;
	else
		return getCam(it->second);
}

bool BLvct::takePic(BaslerImp* cam,cv::Mat& m)
{
	if (NULL == cam )
		return false;
	m_lock.lock();
	void *pImageBuffer= NULL;
	size_t nWidth = 0;
	size_t nHeight = 0;
	size_t nChannel = 0;
	size_t nImageSize = 0;
	EPixelType ePixelType;
	uint32_t nPaddingX;

	if(cam->Grab(pImageBuffer,nWidth,nHeight,nChannel, nImageSize, ePixelType, nPaddingX) ==0  )
	{
		CPylonImage pylonImage;
		// Create a pylon ImageFormatConverter object.
		CImageFormatConverter formatConverter;
		// Specify the output pixel format.
		formatConverter.OutputPixelFormat= PixelType_BGR8packed;

		//enum EImageOrientation
		// {
		//     ImageOrientation_TopDown, ///<The first row of the image is located at the start of the image buffer. This is the default for images taken by a camera.
		//     ImageOrientation_BottomUp ///<The last row of the image is located at the start of the image buffer.
		// };

		formatConverter.Convert( pylonImage,
			(uint8_t *) pImageBuffer,
			nImageSize,
			ePixelType,
			nWidth,
			nHeight,
			nPaddingX,
			ImageOrientation_BottomUp   //ImageOrientation_TopDown
			);

		//	formatConverter.Convert(pylonImage, ptrGrabResult);
		//	m=cv::Mat(nHeight,nWidth,CV_8UC3,pImageBuffer); 	// m is an OpenCV image.
		// Create an OpenCV image from a pylon image.
		m= cv::Mat(nHeight, nWidth, CV_8UC3, (uint8_t *) pylonImage.GetBuffer()).clone();
		m_lock.unLock();
		return true;
	}
	m_lock.unLock();
	return false;
}



#endif