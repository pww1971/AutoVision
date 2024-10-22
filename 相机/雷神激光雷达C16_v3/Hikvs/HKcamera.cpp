


#include "HKcamera.h"
#if (defined _OPENCV_CAM) || (defined _OPENCV_CAM_1) || (defined _OPENCV_CAM_2) 
CHKCamera::CHKCamera()
{
	m_pDeviceInfo = NULL;
	pBufForDriver = NULL;
	m_id=-1;
#ifdef _OPENCV_CAM
	m_monitorMode = 0;
#else
	#ifdef _OPENCV_CAM_1
		m_monitorMode = 1;
	#else
		m_monitorMode = 2;
	#endif
#endif
}
CHKCamera::~CHKCamera()
{
	release();
}
//���ץͼ
int CHKCamera::grapImage(cv::Mat& m)
{
	m_Lock.lock();
	int nRet;
	if ( m_monitorMode == 0 || m_monitorMode == 1)//����
	{
		nRet = MV_CC_SetCommandValue(handle, "TriggerSoftware");
		if (MV_OK != nRet)
		{
			m_Lock.unLock();
			return -1;
		}
	}
//	if (softTrig)//ֻ���� ����ͼƬ
//	{
////		MV_FRAME_OUT stOutFrame = {0};
////		MV_CC_FreeImageBuffer(handle,&stOutFrame);
//		nRet = MV_CC_GetOneFrameTimeout(handle, pBufForDriver, nBufSizeForDriver, &stImageInfo, 1000);
//		if (nRet == MV_OK)//ֻ�ɼ���ӡ�����������Ļ���
//		{
//			FILE* fp22 = fopen("./logtt1.txt", "a");
//			fprintf(fp22," nFrameNum[%d], nFrameCounter[%d], nTriggerIndex[%d],nAverageBrightness[%d],nLostPacket[%d]\n", 
//			stImageInfo.nFrameNum,stImageInfo.nFrameCounter,stImageInfo.nTriggerIndex,stImageInfo.nAverageBrightness,
//			stImageInfo.nLostPacket);//���ӡ��������¼��ÿһ��ֵ	
//			fclose(fp22);
//		}
//		m_Lock.unLock();
//		return 1;
//	}
	//Sleep(2500);
	nRet = MV_CC_GetOneFrameTimeout(handle, pBufForDriver, nBufSizeForDriver, &stImageInfo, 1000);
	if (nRet == MV_OK)
	{
	//	FILE* fp11 = fopen("./logtt.txt", "a");
	//	fprintf(fp11," nFrameNum[%d], nFrameCounter[%d], nTriggerIndex[%d],nAverageBrightness[%d],nLostPacket[%d]\n", 
	//	stImageInfo.nFrameNum,stImageInfo.nFrameCounter,stImageInfo.nTriggerIndex,stImageInfo.nAverageBrightness,
	//	stImageInfo.nLostPacket);//���ӡ��������¼��ÿһ��ֵ	
	//	fclose(fp11);
	//// ���ö�Ӧ���������
	//	unsigned char* pImage = (unsigned char*)malloc(60*1024*1024);
	//	MV_SAVE_IMAGE_PARAM_EX stParam = { 0 };
	//	//stParam.enImageType = MV_Image_Jpeg; // ��Ҫ�����ͼ������
	//	stParam.enImageType = MV_Image_Bmp;
	//	stParam.enPixelType = stImageInfo.enPixelType;  // �����Ӧ�����ظ�ʽ
	//	stParam.nWidth = stImageInfo.nWidth;         // �����Ӧ�Ŀ�
	//	stParam.nHeight = stImageInfo.nHeight;          // �����Ӧ�ĸ�
	//	stParam.nDataLen = stImageInfo.nFrameLen;
	//	stParam.pData = pBufForDriver;
	//	stParam.pImageBuffer = pImage;
	//	stParam.nBufferSize = nBufSizeForSaveImage;  // �洢�ڵ�Ĵ�С
	//	stParam.nJpgQuality = 80;       // jpg���룬���ڱ���Jpgͼ��ʱ��Ч������BMPʱSDK�ں��Ըò���
	//	nRet = MV_CC_SaveImageEx(&stParam);
	//	if (MV_OK != nRet)
	//	{
	//		//m_bRetStatus = STATUS_ERROR;
	//		;
	//	}
	//	char pImageName[32] = {0};
	//	sprintf_s(pImageName, 20, "%03d.bmp", stImageInfo.nFrameNum);
	//	FILE* fp = fopen(pImageName, "wb");
	//	fwrite(pImage, 1, stParam.nImageLen, fp);
	//	fclose(fp);
	//	free(pImage);
		if (PixelType_Gvsp_YUV422_Packed == stImageInfo.enPixelType || PixelType_Gvsp_YUV422_YUYV_Packed == stImageInfo.enPixelType)
		{
			m = cv::Mat(stImageInfo.nHeight , stImageInfo.nWidth,CV_8UC3);

			MV_CC_PIXEL_CONVERT_PARAM stYUVParam;
			stYUVParam.enDstPixelType = PixelType_Gvsp_BGR8_Packed;
			stYUVParam.enSrcPixelType = stImageInfo.enPixelType;
			stYUVParam.nDstBufferSize = (uint) (stImageInfo.nHeight * stImageInfo.nWidth * 3);
			stYUVParam.nSrcDataLen = stImageInfo.nFrameLen;
			stYUVParam.nHeight = stImageInfo.nHeight;
			stYUVParam.nWidth = stImageInfo.nWidth;
			stYUVParam.pSrcData = pBufForDriver;
			stYUVParam.pDstBuffer = m.data;

			nRet=MV_CC_ConvertPixelType(handle,&stYUVParam);
			if (MV_OK != nRet)
			{
				m_Lock.unLock();
				printf("YUVת��ΪRGBʧ��!\n");
				return -1;
			}
			if (PixelType_Gvsp_YUV422_YUYV_Packed == stImageInfo.enPixelType)
				cv::cvtColor(m, m, CV_RGB2BGR);
			m_Lock.unLock();
			return 1;
		}
		else if (PixelType_Gvsp_Mono8 != stImageInfo.enPixelType && PixelType_Gvsp_RGB8_Packed != stImageInfo.enPixelType)
		{
			m_Lock.unLock();
			//viewTxt("cam pic type error!");
			return -1;
		}
		printf("Width[%d],Height[%d],FrameNum[%d]\r\n", stImageInfo.nWidth, stImageInfo.nHeight, stImageInfo.nFrameNum);
		int channels = 1;
		if (PixelType_Gvsp_Mono8 != stImageInfo.enPixelType)
		{
			channels = 3;
		}
		// ���ڵ�һ�α���ͼ��ʱ���뻺��
		if (-1 == nBufSizeForSaveImage)
		{
			// BMPͼƬ��С��width * height * 3 + 2048(Ԥ��BMPͷ��С)
			nBufSizeForSaveImage = stImageInfo.nWidth * stImageInfo.nHeight *channels + 2048;
		}
		if (3 == channels)
		{
			m = cv::Mat(stImageInfo.nHeight, stImageInfo.nWidth, CV_8UC3, pBufForDriver);//CV_16UC3
			/*if (!isColor)
				cv::cvtColor(m, m, cv::COLOR_RGB2GRAY);
			else*/
				cv::cvtColor(m, m, CV_BGR2RGB); //CV_RGB2YUV  CV_RGB2BGR
		}
		else
		{
			m = cv::Mat(stImageInfo.nHeight, stImageInfo.nWidth, CV_8UC1, pBufForDriver);//CV_16UC1
			/*if (isColor)
				cv::cvtColor(m, m, cv::COLOR_GRAY2RGB);*/
		}
	}
	else
	{
		m_Lock.unLock();
		printf("No Data!\n");
		return -1;
	}
	m_Lock.unLock();
	return 1;
}

int CHKCamera::grapImage(CoordinatePosXYZ& cor,S_Msg*& pMsg)
{
	cv::Mat m;
	int ret = grapImage(m);
	if(1==ret)
	{
		pMsg = new S_Msg;
		*(CoordinatePosXYZ*)pMsg = cor;
		pMsg->raw = m;
		pMsg->x = -1;
		pMsg->y = -1;
	}
	
	return ret;
}

/*���������ʼ����
������nIndex�豸��� 
����ֵ��-1 ����0 ����
*/
int CHKCamera::ini(unsigned int nIndex,MV_CC_DEVICE_INFO_LIST& stDeviceList )
{
	release();
	int nRet = MV_OK;

	handle = NULL;
	m_pDeviceInfo = stDeviceList.pDeviceInfo[nIndex];
	// 2. ѡ���豸���������
	nRet = MV_CC_CreateHandle(&handle, m_pDeviceInfo);
	if (MV_OK != nRet)
	{
		printf("MV_CC_CreateHandle fail! nRet [%x]\n", nRet);
		return -1;
	}
	
	// 3.���豸
	if (m_monitorMode==1)
	{
		nRet = MV_CC_OpenDevice(handle, MV_ACCESS_Control);
	}
	else if (m_monitorMode == 2)
	{
		nRet = MV_CC_OpenDevice(handle, MV_ACCESS_Monitor);
	}
	else
		nRet = MV_CC_OpenDevice(handle);
	if (MV_OK != nRet)
	{
		printf("MV_CC_OpenDevice fail! nRet [%x]\n", nRet);
		MV_CC_CloseDevice(handle);
		m_pDeviceInfo = NULL;
		return -1;
	}
	// 4. ���ò���
	// open����
	if ( m_monitorMode == 0 || m_monitorMode == 1)
	{
		//HIK190827
		 // ch:̽��������Ѱ���С(ֻ��GigE�����Ч) | en:Detection network optimal package size(It only works for the GigE camera)
        if (stDeviceList.pDeviceInfo[nIndex]->nTLayerType == MV_GIGE_DEVICE)
        {
            int nPacketSize = MV_CC_GetOptimalPacketSize(handle);
            if (nPacketSize > 0)
            {
                nRet = MV_CC_SetIntValue(handle,"GevSCPSPacketSize",nPacketSize);//һ��ӦΪ8164
                if(nRet != MV_OK)
                {
                    printf("Warning: Set Packet Size fail nRet [0x%x]!", nRet);
                }
				if(nPacketSize<8164)
				{
					FILE* fp11 = fopen("./logtt.txt", "a");
					fprintf(fp11,"Set PacketSize: %d!\n", nPacketSize);//�����1500��������
					fclose(fp11);
				}
            }
            else
            {
                printf("Warning: Get Packet Size fail nRet [0x%x]!", nPacketSize);
            }
        }
		nRet = MV_CC_SetEnumValue(handle, "TriggerMode", MV_TRIGGER_MODE_ON);
		if (MV_OK != nRet)
		{
			printf("Set TriggerMode failed[%x]!\n", nRet);
			m_pDeviceInfo = NULL;
			return -1;
		}
		nRet = MV_CC_SetEnumValue(handle, "TriggerSource", MV_TRIGGER_SOURCE_SOFTWARE);//����
		if (MV_OK != nRet)
		{
			printf("Set TriggerSource failed[%x]!\n", nRet);
			m_pDeviceInfo = NULL;
			return -1;
		}
		nRet = MV_CC_SetEnumValue(handle, "PixelFormat", PixelType_Gvsp_YUV422_Packed);//���óɲ�ɫYUV��ʽ
		if (MV_OK != nRet)
		{
			printf("Set PixelFormat failed[%x]!\n", nRet);
//			m_pDeviceInfo = NULL;
//			return -1;
		}
		//ˮӡ��Ϣ��ӡ
		nRet = MV_CC_SetEnumValue(handle, "FrameSpecInfoSelector", 3);//BrightnessInfo
		nRet = MV_CC_SetBoolValue(handle, "FrameSpecInfo", true);
		nRet = MV_CC_SetEnumValue(handle, "FrameSpecInfoSelector", 6);//ExtTriggerCount
		nRet = MV_CC_SetBoolValue(handle, "FrameSpecInfo", true);
		nRet = MV_CC_SetEnumValue(handle, "FrameSpecInfoSelector", 5);//Framecounter
		nRet = MV_CC_SetBoolValue(handle, "FrameSpecInfo", true);
	}
	if (m_monitorMode == 1 || m_monitorMode == 2)
	{
		// ch:ָ���鲥ip | en:multicast IP
		char strIp[] = "239.0.1.23";
		unsigned int nIp1, nIp2, nIp3, nIp4, nIp;
		sscanf_s(strIp, "%d.%d.%d.%d", &nIp1, &nIp2, &nIp3, &nIp4);
		nIp = (nIp1 << 24) | (nIp2 << 16) | (nIp3 << 8) | nIp4;

		// ch:��ָ���˿ں���Ϊ�鲥��˿� | en:multicast port
		MV_TRANSMISSION_TYPE stTransmissionType;
		memset(&stTransmissionType, 0, sizeof(MV_TRANSMISSION_TYPE));

		stTransmissionType.enTransmissionType = MV_GIGE_TRANSTYPE_MULTICAST;
		stTransmissionType.nDestIp = nIp;
		stTransmissionType.nDestPort = 8787;
		nRet = MV_GIGE_SetTransmissionType(handle, &stTransmissionType);
		if (MV_OK != nRet)
		{
			printf("Set Transmission Type fail! nRet [0x%x]\n", nRet);
			return -1;
		}
	}
	m_id = nIndex;

	setRoi();
	nRet = iniGrabbing();

	return nRet;
}
int CHKCamera::iniGrabbing()
{
	if (NULL == handle)
	{
		return -1;
	}
	int nRet = MV_OK;
	// 5.��ʼץͼ
	nRet = MV_CC_StartGrabbing(handle);
	if (MV_OK != nRet)
	{
		printf("MV_CC_StartGrabbing fail! nRet [%x]\n", nRet);
		m_pDeviceInfo = NULL;
		return -1;
	}
	//5.1������ʱ
	//nRet = MV_CC_SetFloatValue(handle, "TriggerDelay", 200);
	//if (MV_OK != nRet)
	//{
	//	printf("error: TriggerDelay fail [%x]\n", nRet);
	//	return -1;
	//}
	//if ( m_monitorMode == 0 )
	//{
	//	////5.2������ͼһ��
	//	nRet = MV_CC_SetCommandValue(handle, "TriggerSoftware");
	//	if (MV_OK != nRet)
	//	{
	//		printf("error: TriggerSoftware fail [%x]\n", nRet);
	//		m_pDeviceInfo = NULL;
	//		return -1;
	//	}
	//}

	// 6.��ȡͼ
	MVCC_INTVALUE stIntvalue = { 0 };
	nRet = MV_CC_GetIntValue(handle, "PayloadSize", &stIntvalue);
	if (nRet != MV_OK)
	{
		printf("Get PayloadSize failed! nRet [%x]\n", nRet);
		m_pDeviceInfo = NULL;
		return -1;
	}

	nBufSizeForDriver = stIntvalue.nCurValue + 2048;  // һ֡���ݴ�С + Ԥ��2048byte����SDK�ڲ�����
	// �ͷ��ڴ�
	if (pBufForDriver)
	{
		free(pBufForDriver);
		pBufForDriver = NULL;
	}
	pBufForDriver = (unsigned char *)malloc(nBufSizeForDriver);

	memset(&stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
	nBufSizeForSaveImage = -1;

	return nRet;
}
int CHKCamera::setRoi()
{
	cv::Rect rct(0,0,0,0);
	CConfig cfg(getExePath()+PROCCFG);
	string szVal = cfg.getItem("CurDir");
	if(szVal != "")
	{
		szVal = szVal+"\\cam\\"+CStrProcess::itoa_radixN(m_id)+"\\hk.txt";
		CConfig cfg2(szVal);
		szVal = cfg2.getItem("ROIRECT");
		if(szVal != "")
		{
			vector<string> vct;
			CStrProcess::Split(szVal,",",vct,true);
			if(vct.size()==4)
			{
				rct.x = atoi(vct[0].c_str());
				rct.y = atoi(vct[1].c_str());
				rct.width = atoi(vct[2].c_str());
				rct.height = atoi(vct[3].c_str());
				cv::Rect rt;
				cv::Size sz;
				if(getRoi(rt,sz))
				{
					if( rt.x == rct.x && rt.y == rct.y && rt.width == rct.width & rt.height == rct.height)
						return MV_OK;
				}
			}
		}
	}
	if(rct.width==0)
	{	
		return -1;
	}
	return setRoi(rct);
}

int CHKCamera::setRoi(cv::Rect& rt)
{
	if (NULL == handle)
	{
		return -1;
	}
	MV_CC_StopGrabbing(handle);
	int nRet = MV_OK;

	MVCC_INTVALUE stIntvalue = { 0 };
	nRet = MV_CC_GetIntValue(handle, "Width", &stIntvalue);
	if (nRet != MV_OK)
	{
		printf("Get PayloadSize failed! nRet [%x]\n", nRet);
		return -1;
	}
	int step = stIntvalue.nInc;
	rt.width = (rt.width/step)*step;
	nRet = MV_CC_SetIntValueEx(handle,"Width",rt.width);
	if (MV_OK != nRet)
	{
		printf("MV_CC_Width fail! nRet [%x]\n", nRet);
		return -1;
	}
	rt.height = (rt.height/step)*step;
	nRet = MV_CC_SetIntValueEx(handle,"Height",rt.height);
	if (MV_OK != nRet)
	{
		printf("MV_CC_Height fail! nRet [%x]\n", nRet);
		return -1;
	}
	rt.x = (rt.x/step)*step;
	nRet = MV_CC_SetIntValueEx(handle,"OffsetX",rt.x);
	if (MV_OK != nRet)
	{
		printf("MV_CC_OffsetX fail! nRet [%x]\n", nRet);
		return -1;
	}
	rt.y = (rt.y/step)*step;
	nRet = MV_CC_SetIntValueEx(handle,"OffsetY",rt.y);
	if (MV_OK != nRet)
	{
		printf("MV_CC_OffsetY fail! nRet [%x]\n", nRet);
		return -1;
	}
	return nRet;
}

bool CHKCamera::getRoi(cv::Rect& rct,cv::Size& sz)
{	
	MVCC_INTVALUE_EX mval;
	int nRet = MV_OK;
	nRet = MV_CC_GetIntValueEx(handle,"Width",&mval);
	if (MV_OK != nRet)
	{
		printf("MV_CC_Width fail! nRet [%x]\n", nRet);
		return false;
	}
	rct.width = mval.nCurValue;
	sz.width = mval.nMin;
	nRet = MV_CC_GetIntValueEx(handle,"Height",&mval);
	if (MV_OK != nRet)
	{
		printf("MV_CC_Height fail! nRet [%x]\n", nRet);
		return false;
	}
	rct.height = mval.nCurValue;
	sz.height = mval.nMin;
	nRet = MV_CC_GetIntValueEx(handle,"OffsetX",&mval);
	if (MV_OK != nRet)
	{
		printf("MV_CC_OffsetX fail! nRet [%x]\n", nRet);
		return false;
	}
	rct.x = mval.nCurValue;
	nRet = MV_CC_GetIntValueEx(handle,"OffsetY",&mval);
	if (MV_OK != nRet)
	{
		printf("MV_CC_OffsetY fail! nRet [%x]\n", nRet);
		return false;
	}
	rct.y = mval.nCurValue;
	return true;
}
bool CHKCamera::getRawSize(cv::Rect& rt)
{
	MVCC_INTVALUE stIntvalue = { 0 };
	int nRet = MV_CC_GetIntValue(handle, "Width", &stIntvalue);
	if (nRet != MV_OK)
	{
		printf("Get PayloadSize failed! nRet [%x]\n", nRet);
		return false;
	}
	rt.x=0;
	rt.width = stIntvalue.nMax;
	nRet = MV_CC_GetIntValue(handle, "Height", &stIntvalue);
	if (nRet != MV_OK)
	{
		printf("Get PayloadSize failed! nRet [%x]\n", nRet);
		return false;
	}
	rt.y=0;
	rt.height = stIntvalue.nMax;
	return true;
}

bool CHKCamera::getCamState()
{
	if (NULL == m_pDeviceInfo || m_id == -1)
	{
		return false;
	}
	return MV_CC_IsDeviceAccessible(m_pDeviceInfo,MV_ACCESS_Exclusive);
}

//�ͷ��������
void CHKCamera::release()
{
#ifdef _SIMULATION
	return;
#endif
	m_pDeviceInfo = NULL;
	m_id = -1;
	// 7.����ץͼ
	int nRet = MV_OK;
	nRet = MV_CC_StopGrabbing(handle);
	if (MV_OK != nRet)
	{
		printf("MV_CC_StopGrabbing fail! nRet [%x]\n", nRet);
		return;
	}

	// 8.�ر��豸
	nRet = MV_CC_CloseDevice(handle);
	if (MV_OK != nRet)
	{
		printf("MV_CC_CloseDevice fail! nRet [%x]\n", nRet);
		return;
	}


	// 9.���پ��
	nRet = MV_CC_DestroyHandle(handle);
	if (MV_OK != nRet)
	{
		printf("MV_CC_DestroyHandle fail! nRet [%x]\n", nRet);
	}

	// �ͷ��ڴ�
	if (pBufForDriver)
	{
		free(pBufForDriver);
		pBufForDriver = NULL;
	}
	
}

void CHKCamera::setTriggerMode(_MV_CAM_TRIGGER_MODE_ mode)
{
		// 4. ���ò���
	// open���� ����ģʽMV_TRIGGER_MODE_OFF ���� MV_TRIGGER_MODE_ON
	int nRet = MV_CC_SetEnumValue(handle, "TriggerMode", mode);
	if (MV_OK != nRet)
	{
		printf("Set TriggerMode failed[%x]!\n", nRet);
		return ;
	}
	// 4.3���ô���Դ
	//0:Line0 1:Line1 2:Line2 3.Line3 4:Counter0 7:Software 8:FrequencyConverter
	/*nRet = MV_CC_SetEnumValue(handle, "TriggerSource", 7);
	if (MV_OK != nRet)
	{
		printf("Set TriggerSource failed[%x]!\n", nRet);
		return -1;
	}*/
	// ʹ������ģʽ
	/*nRet = MV_CC_SetEnumValue(handle, "AcquisitionMode", MV_ACQ_MODE_CONTINUOUS);
	if (MV_OK != nRet)
	{
	printf("Set AcquisitionMode failed[%x]!\n", nRet);
	return ;
	}*/
}

void CHKCamera::SetTriggerSource(bool bSoftWareTriggerCheck)
{
	if (bSoftWareTriggerCheck)
	{
		int nRet = MV_CC_SetEnumValue(handle, "TriggerMode", MV_TRIGGER_SOURCE_SOFTWARE);
		if (MV_OK != nRet)
		{
			printf("Set TriggerMode failed[%x]!\n", nRet);
			return ;
		}
	}
	else
	{
		int nRet = MV_CC_SetEnumValue(handle, "TriggerMode", MV_TRIGGER_SOURCE_LINE0);
		if (MV_OK != nRet)
		{
			printf("Set TriggerMode failed[%x]!\n", nRet);
			return ;
		}
	}
}

void CHKCamera::setSoftTriggerOnce()
{
	MV_CC_SetCommandValue(handle, "TriggerSoftware");
}

HKvct::HKvct()
{
	m_bIni = false;
}

HKvct::~HKvct()
{
	release();
}

void HKvct::release()
{
	vector<CHKCamera*>::iterator it = m_hkcam.begin();
	for (; it != m_hkcam.end(); ++it)
	{
		delete (*it);
	}
	m_hkcam.clear();
	if (m_mpCam.size()>0)
		m_mpCam.clear();
	m_bIni = false;
}

CHKCamera* HKvct::getHK(int id)
{
	if (id>=m_hkcam.size())
	{
		return NULL;
	}
	m_lock.lock();
	vector<CHKCamera*>::iterator it = m_hkcam.begin();
	for (; it != m_hkcam.end(); ++it)
	{
		if ((*it)->m_id == id)
		{
			m_lock.unLock();
			return (*it);
		}
	}
	m_lock.unLock();
	return NULL;
}

// ���²����豸��ť:ö��
void HKvct::enumHKDev()
{
	m_lock.lock();
	if (m_bIni == true)
	{
		m_lock.unLock();
		return;
	}
	// 2. ��ʼ���豸��Ϣ�б�
	memset(&m_stDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
	release();
	
	// 3. ö�������������豸
	int nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &m_stDevList);
	if (MV_OK != nRet)
	{
		m_lock.unLock();
		// ö���豸ʧ�� 
		return;
	}

	/*��ֵ���뵽��Ϣ�б���в���ʾ����*/
	unsigned int i;
	int nIp1, nIp2, nIp3, nIp4;
	char strMsg[64];
	for (i = 0; i < m_stDevList.nDeviceNum; i++)
	{
		MV_CC_DEVICE_INFO* pDeviceInfo = m_stDevList.pDeviceInfo[i];
		if (NULL == pDeviceInfo)
		{
			continue;
		}
		if (pDeviceInfo->nTLayerType == MV_GIGE_DEVICE)
		{
			nIp1 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0xff000000) >> 24);
			nIp2 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x00ff0000) >> 16);
			nIp3 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x0000ff00) >> 8);
			nIp4 = (m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff);

			wchar_t* pUserName = NULL;
			if (strcmp("", (LPCSTR)(pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName)) != 0)
			{
				DWORD dwLenUserName = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName), -1, NULL, 0);
				pUserName = new wchar_t[dwLenUserName];
				MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName), -1, pUserName, dwLenUserName);
			}
			else
			{
				char strUserName[256] = {0};
				sprintf_s(strUserName, "%s %s (%s)", pDeviceInfo->SpecialInfo.stGigEInfo.chManufacturerName,
					pDeviceInfo->SpecialInfo.stGigEInfo.chModelName,
					pDeviceInfo->SpecialInfo.stGigEInfo.chSerialNumber);
				DWORD dwLenUserName = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, NULL, 0);
				pUserName = new wchar_t[dwLenUserName];
				MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, pUserName, dwLenUserName);
			}
			sprintf(strMsg,"[%d]GigE:    %s  (%d.%d.%d.%d)", i, \
				pUserName, nIp1, nIp2, nIp3, nIp4);
			if (NULL != pUserName)
			{
				delete(pUserName);
				pUserName = NULL;
			}

		}
		else if (pDeviceInfo->nTLayerType == MV_USB_DEVICE)
		{
			wchar_t* pUserName = NULL;

			if (strcmp("", (char*)pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName) != 0)
			{
				DWORD dwLenUserName = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName), -1, NULL, 0);
				pUserName = new wchar_t[dwLenUserName];
				MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName), -1, pUserName, dwLenUserName);
			}
			else
			{
				char strUserName[256];
				sprintf_s(strUserName, "%s %s (%s)", pDeviceInfo->SpecialInfo.stUsb3VInfo.chManufacturerName,
					pDeviceInfo->SpecialInfo.stUsb3VInfo.chModelName,
					pDeviceInfo->SpecialInfo.stUsb3VInfo.chSerialNumber);
				DWORD dwLenUserName = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, NULL, 0);
				pUserName = new wchar_t[dwLenUserName];
				MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, pUserName, dwLenUserName);
			}
			sprintf(strMsg,"[%d]UsbV3:  %s", i, pUserName);
			if (NULL != pUserName)
			{
				delete(pUserName);
				pUserName = NULL;
			}

		}
		else
		{
			//MessageBox(TEXT("ö�ٵ�δ֪���"), TEXT("����"), MB_OK | MB_ICONWARNING);;
		}
		CHKCamera* cam = new CHKCamera();
		cam->ini(i,m_stDevList);
		m_hkcam.push_back(cam);
		m_mpCam.insert(pair<string,int>(strMsg,i));
	}
	m_bIni = true;
	m_lock.unLock();

	if (0 == m_stDevList.nDeviceNum)
	{
		//MessageBox(TEXT("���豸"));
		return;
	}

	return;
}

CHKCamera* HKvct::getHK(string ipname)
{
	map<string,int>::iterator it = m_mpCam.find(ipname);
	if (it==m_mpCam.end())
		return NULL;
	else
		return getHK(it->second);
}

bool HKvct::getCamState(int id)
{
	CHKCamera* cam = getHK(id);
	if (NULL != cam && cam->m_id != -1)//ini error or no know cam
	{
		return cam->getCamState();
	}
	return false;
}


#endif
