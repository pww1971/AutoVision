//#include "stdafx.h"
#include "BaslerImp.h"
#ifdef _BASLER_CAM
CInstantCameraArray cameras(20);

map<string, CInstantCamera*> mapCameras;

//CEvent m_event;
void BaslerImp::OnImageGrabbed( CInstantCamera& camera, const CGrabResultPtr& ptrGrabResult)
{
	try
	{
	
	}
	catch (const GenericException &e)
	{
		if (camera.IsCameraDeviceRemoved())
        {
//            ::AfxMessageBox(_T("Error:"+ camera.GetDeviceInfo().GetUserDefinedName() + "OnImageGrabbed removed"));
        }
		 else
        {
  //          ::AfxMessageBox(_T("Error:, OnImageGrabbed Grab Failed!"));
        }
	}	
	catch (...)
	{
	}
}


void BaslerImp::OnCameraDeviceRemoved( CInstantCamera& camera)
{
//    ::AfxMessageBox(_T("Error:"+ camera.GetDeviceInfo().GetUserDefinedName() + "OnCameraDeviceRemoved"));
}


void BaslerImp::SetOwner( void* pOwner, BaslerGrabbedCallback funcCallback )
{
	m_pOwner = pOwner;
	m_fCallback = funcCallback;
}

void BaslerImp::setCallbackFlag( bool bset )
{
	m_callbackFlag = bset;
}

BaslerImp::BaslerImp(void)
{
	
	nSoftFlag=0;

	m_iFlag = 0;

	m_callbackFlag = false;

	m_pCamera = NULL;
}


BaslerImp::~BaslerImp(void)
{
	
}

long BaslerImp::EnumerateCam(int& cameraNum ,vector<string>& cameraInfo)
{

	try
	{
		PylonInitialize();
	
		// Get the transport layer factory.
		CTlFactory& tlFactory = CTlFactory::GetInstance();

		// Get all attached devices and exit application if no device is found.
		DeviceInfoList_t devices;
		if ( tlFactory.EnumerateDevices(devices) == 0 )
		{
			//MessageBoxW(NULL,L"No Basler camera present!", L"Error Message", MB_OK | MB_ICONEXCLAMATION);
			return 0;
		}
		cameraInfo.clear();
		mapCameras.clear();
		cameraNum = devices.size();
		// Create and attach all Pylon Devices.
		for ( size_t i = 0; i < cameraNum; ++i)
		{
			CInstantCamera* temCamera = &cameras[ i ];	
			temCamera->Attach( tlFactory.CreateDevice( devices[ i ]));
	
	
			

		//	string strDeviceName = temCamera->GetDeviceInfo().GetModelName();
			string strSerialNumber = temCamera->GetDeviceInfo().GetSerialNumber();
			if(temCamera->IsOpen())
			{
				temCamera->Close();
			}

		

		//	string sCamInfo = strDeviceName + "," + strSerialNumber;

			string sCamInfo = temCamera->GetDeviceInfo().GetUserDefinedName();

			//map<string,CInstantCamera*> mapCameras;
			mapCameras[sCamInfo]= temCamera;

			//vector<string>& cameraInfo
			cameraInfo.push_back(sCamInfo);
		}
	}
	catch(const GenericException &e)
	{
		// Error handling
		return 0;
	}

	return 0;
}

void BaslerImp::DestroyCamera()
{
	map<string,CInstantCamera*>::iterator it = mapCameras.begin();
	for(;it != mapCameras.end();it++)
	{
		CInstantCamera* pTemp = it->second;

		if (pTemp != NULL  && pTemp->IsPylonDeviceAttached())
		{
			pTemp->DetachDevice();
			pTemp->DestroyDevice();	
		}	
	}
	PylonTerminate();
}

long BaslerImp::Init(string &sCameraInfo)
{

	int exitCode = 0;
	try
	{
		m_pCamera = mapCameras[sCameraInfo];

		if (m_pCamera == NULL)
		{
			return -1;
		}

		string strDeviceName = m_pCamera->GetDeviceInfo().GetModelName();
		string strSerialNumber = m_pCamera->GetDeviceInfo().GetSerialNumber();
	//	string sCamInfo = strDeviceName + "," + strSerialNumber;
		string sCamInfo = m_pCamera->GetDeviceInfo().GetUserDefinedName();
		
		if(   sCameraInfo.find(sCamInfo) == string::npos    )
		{// no find
			return -1;
		}
		
		///////////////
		//string sCamInfo = sCameraInfoUserid;
		//size_t nPos = sCameraInfo.find(sCamInfo);
		//if( nPos == string::npos    )
		//{// no find
		//	return -1;
		//}
		//else
		//{
		//	m_pCamera = mapCameras[nPos];
		//}


		//////////////////
		if ( m_pCamera->GetSfncVersion() >= Sfnc_2_0_0)
		{//USB camera devices
			m_iType = 0;
		}
		else
		{//IIDC 1394 and GigE camera devices.
			m_iType = 1;
		}
	}
	catch (const GenericException &e)
	{
		// modify
        return -1;
	}
	catch (...)
	{
		return -1;
	}
	return exitCode;
}
long BaslerImp::OpenCamera()
{
	try
	{		
		if (m_pCamera == NULL)
		{
			return -1;
		}

		if(m_pCamera->IsCameraDeviceRemoved())
		{
			ReAttachCameraIfLost();
		}

		if(m_pCamera->IsPylonDeviceAttached())
		{
			m_pCamera->Open();
			setDefaultMode();

			// register image ready callback
			m_pCamera->RegisterImageEventHandler(this, RegistrationMode_Append, Ownership_ExternalOwnership);

			// register configuration event handler that handles device removal.
            m_pCamera->RegisterConfiguration( this, RegistrationMode_Append, Ownership_ExternalOwnership);		
		}	
	}
	catch (const GenericException &e)
	{
		//CString str ;
		//str.Format("%s\r\n",e.GetDescription());
		//OutputDebugString(str);
	}
	catch (...)
	{
	}
	

	return 0;
}
long BaslerImp::SetParam(string sParma,string sParamValue)
{
	try
	{
		if (m_pCamera == NULL)
		{
			return -1;
		}

		INodeMap& nodemap = m_pCamera->GetNodeMap();

		string strParam = (sParma);
		string strParamValue = (sParamValue);
		float fValue = atoi(strParamValue.c_str());


		if ( m_pCamera->GetSfncVersion() >= Sfnc_2_0_0)
		{//USB camera devices
			
			if ( strParam=="Shutter")
			{
				CFloatPtr exposureTime ( nodemap.GetNode("ExposureTime"));

				int iMin = exposureTime->GetMin();
				int iMax = exposureTime->GetMax();
				int iInc= exposureTime->GetInc();			

				if (fValue <= iMax && fValue >= iMin)
				{
					exposureTime->SetValue(fValue);
				}
			}
			else if(strParam=="Gain")
			{
				CFloatPtr gain( nodemap.GetNode( "Gain"));

				int iMin = gain->GetMin();
				int iMax = gain->GetMax();

				if (fValue <= iMax && fValue >= iMin)
				{
					gain->SetValue(fValue);
				}	
				
			}
		}
		else
		{//IIDC 1394 and GigE camera devices.

			if ( strParam=="Shutter")
			{

				CFloatPtr exposureTime ( nodemap.GetNode("ExposureTimeAbs"));

				int iMin = exposureTime->GetMin();
				int iMax = exposureTime->GetMax();

				if (fValue <= iMax && fValue >= iMin)
				{
					exposureTime->SetValue(fValue);
				}
				
			}
			else if(strParam=="Gain")
			{
				CFloatPtr gainRaw (nodemap.GetNode("GainAbs"));

				int iMin = gainRaw->GetMin();
				int iMax = gainRaw->GetMax();

				if (fValue <= iMax && fValue >= iMin)
				{
					gainRaw->SetValue(fValue);
				}
			}
		}
	}
	catch (const GenericException &e)
	{
		//CString str ;
		//str.Format("%s\r\n",e.GetDescription());
		//OutputDebugString(str);
	}
	catch (...)
	{
	}
	return 0;
}

int64_t BaslerImp::Adjust(int64_t val, int64_t minimum, int64_t maximum, int64_t inc)
{
	// Check the input parameters.
	if (inc <= 0)
	{
		// Negative increments are invalid.
		throw LOGICAL_ERROR_EXCEPTION("Unexpected increment %d", inc);
	}
	if (minimum > maximum)
	{
		// Minimum must not be bigger than or equal to the maximum.
		throw LOGICAL_ERROR_EXCEPTION("minimum bigger than maximum.");
	}

	// Check the lower bound.
	if (val < minimum)
	{
		return minimum;
	}

	// Check the upper bound.
	if (val > maximum)
	{
		return maximum;
	}

	// Check the increment.
	if (inc == 1)
	{
		// Special case: all values are valid.
		return val;
	}
	else
	{
		// The value must be min + (n * inc).
		// Due to the integer division, the value will be rounded down.
		return minimum + ( ((val - minimum) / inc) * inc );
	}
}

void BaslerImp::setFreeRunMode()
{
	try
	{
		INodeMap& nodemap = m_pCamera->GetNodeMap();
		CEnumerationPtr triggerSelector( nodemap.GetNode("TriggerSelector"));
		CEnumerationPtr triggerMode( nodemap.GetNode("TriggerMode"));
		CEnumerationPtr triggerSource( nodemap.GetNode("TriggerSource"));


	//	if(0==IsContain("TriggerSelector","AcquisitionStart"))	
		if ( IsAvailable( triggerSelector->GetEntryByName( "AcquisitionStart")))
		{
			triggerSelector->FromString("AcquisitionStart");
		}
		if (IsWritable(triggerMode))
		{
			triggerMode->FromString("Off");
		}

	//	if(0==IsContain("TriggerSelector","FrameStart"))	
		if ( IsAvailable( triggerSelector->GetEntryByName( "FrameStart")))
		{
			triggerSelector->FromString("FrameStart");
		}
		if (IsWritable(triggerMode))
		{
			triggerMode->FromString("Off");
		}
	}
	catch (const GenericException &e)
	{
		//CString str ;
		//str.Format("%s\r\n",e.GetDescription());
		//OutputDebugString(str);
	}
	catch (...)
	{

	}
}

void BaslerImp::setSoftWareMode()
{
	try
	{
		INodeMap& nodemap = m_pCamera->GetNodeMap();
		CEnumerationPtr triggerSelector( nodemap.GetNode("TriggerSelector"));
		CEnumerationPtr triggerMode( nodemap.GetNode("TriggerMode"));
		CEnumerationPtr triggerSource( nodemap.GetNode("TriggerSource"));

		//if(0==IsContain("TriggerSelector","AcquisitionStart"))	
		if ( IsAvailable( triggerSelector->GetEntryByName( "AcquisitionStart")))
		{
			triggerSelector->FromString("AcquisitionStart");
		}
		if (IsWritable(triggerMode))
		{
			triggerMode->FromString("Off");
		}

		//if(0==IsContain("TriggerSelector","FrameStart"))	
		if ( IsAvailable( triggerSelector->GetEntryByName( "FrameStart")))
		{
			triggerSelector->FromString("FrameStart");
		}

		if (IsWritable(triggerMode))
		{
			triggerMode->FromString("On");
		}
		if (IsWritable(triggerSource))
		{
			triggerSource->FromString("Software");
		}
	}
	catch (const GenericException &e)
	{
		//CString str ;
		//str.Format("%s\r\n",e.GetDescription());
		//OutputDebugString(str);
	}
	catch (...)
	{

	}
}


void BaslerImp::ExecuteSoftTrig()//执行软触发
{
	INodeMap& nodemap = m_pCamera->GetNodeMap();
	CEnumerationPtr triggerSelector( nodemap.GetNode("TriggerSelector"));

	if(0==IsContain("TriggerSelector","FrameStart"))	
	{
		triggerSelector->FromString("FrameStart");
	}

    CCommandPtr SoftExecute(nodemap.GetNode("TriggerSoftware"));
	if(IsWritable(SoftExecute))
	{
		SoftExecute->Execute();
	}	
}


void BaslerImp::setHardWareMode()
{
	try
	{
		INodeMap& nodemap = m_pCamera->GetNodeMap();
		CEnumerationPtr triggerSelector( nodemap.GetNode("TriggerSelector"));
		CEnumerationPtr triggerMode( nodemap.GetNode("TriggerMode"));
		CEnumerationPtr triggerSource( nodemap.GetNode("TriggerSource"));

	//	if(0==IsContain("TriggerSelector","AcquisitionStart"))	
		if ( IsAvailable( triggerSelector->GetEntryByName( "AcquisitionStart")))
		{
			triggerSelector->FromString("AcquisitionStart");
		}
		if (IsWritable(triggerMode))
		{
			triggerMode->FromString("Off");
		}

	//	if(0==IsContain("TriggerSelector","FrameStart"))	
		if ( IsAvailable( triggerSelector->GetEntryByName( "FrameStart")))
		{
			triggerSelector->FromString("FrameStart");
		}

		if (IsWritable(triggerMode))
		{
			triggerMode->FromString("On");
		}
		if (IsWritable(triggerSource))
		{
			triggerSource->FromString("Line1");
		}
	}
	catch (const GenericException &e)
	{
		//CString str ;
		//str.Format("%s\r\n",e.GetDescription());
		//OutputDebugString(str);
	}
	catch (...)
	{

	}
}

void BaslerImp::setDefaultMode()
{
	try
	{
		INodeMap& nodemap = m_pCamera->GetNodeMap();
		CEnumerationPtr triggerSelector( nodemap.GetNode("TriggerSelector"));
		CEnumerationPtr triggerMode( nodemap.GetNode("TriggerMode"));
		CEnumerationPtr triggerSource( nodemap.GetNode("TriggerSource"));

		if(0==IsContain("TriggerSelector","AcquisitionStart"))	
		{
			triggerSelector->FromString("AcquisitionStart");

			if (IsWritable(triggerMode))
			{
				triggerMode->FromString("Off");
			}
		}


		if(0==IsContain("TriggerSelector","FrameStart"))	
		{
			triggerSelector->FromString("FrameStart");

			if (IsWritable(triggerMode))
			{
				triggerMode->FromString("Off");
			}
		}


		if (IsWritable(triggerSource))
		{
			triggerSource->FromString("Software");
		}
	}
	catch (const GenericException &e)
	{
		//CString str ;
		//str.Format("%s\r\n",e.GetDescription());
		//OutputDebugString(str);
	}
	catch (...)
	{

	}
}

long BaslerImp::IsContain(const string & nodeName,const string & sContain)
{
	INodeMap& nodemap = m_pCamera->GetNodeMap();

	GENICAM_NAMESPACE::gcstring gcNodeName(nodeName.c_str());
	CEnumerationPtr triggerSelector( nodemap.GetNode(gcNodeName));


	if (IsWritable(triggerSelector))
	{//黑白相机存在“FrameStart"  color camera is not exist -->"Acquisition"
		NodeList_t Entries;
		triggerSelector->GetEntries(Entries);

		int iCount = Entries.size();

		for (size_t i = 0; i < iCount; i++)
		{
			GenApi::INode *pNode = Entries[i];
			if (IsAvailable(pNode->GetAccessMode()))
			{
				GenApi::IEnumEntry *pEnum = dynamic_cast<GenApi::IEnumEntry *>(pNode);
				string s = pEnum->GetSymbolic();
				if (s.find(sContain) != string::npos)
				{//找到了
					
					return 0 ;
				}
			}

		}
	}
	return -1;
}

long BaslerImp::IsCurrentContain(const string & nodeName,const string & sContain)
{
	INodeMap& nodemap = m_pCamera->GetNodeMap();

	GENICAM_NAMESPACE::gcstring gcNodeName(nodeName.c_str());
	CEnumerationPtr triggerSelector( nodemap.GetNode(gcNodeName));


	if (IsWritable(triggerSelector))
	{//黑白相机存在“FrameStart"  color camera is not exist -->"Acquisition"

		GenApi::IEnumEntry *pEnum = triggerSelector->GetCurrentEntry();
		string s = pEnum->GetSymbolic();
		if (s.find(sContain) != string::npos)
		{//找到了
			return 0 ;
		}
	}
	return -1;
}
long BaslerImp::IsMonoCamera()
{
	if (0==IsCurrentContain("PixelFormat","Mono8"))
	{//黑白相机

		m_nChannel=1;
		return 0;
	}
	else if (0==IsCurrentContain("PixelFormat","Bayer"))
	{
		m_nChannel=3;
		return 0;
	}
	return -1;
}

long BaslerImp::SetCameraMode(int iFlag)
{
	try
	{
		if (m_pCamera == NULL)
		{
			return -1;
		}
		if (!m_pCamera->IsOpen() || m_pCamera->IsCameraDeviceRemoved())
		{
			return -1;
		}
		if (iFlag==0)
		{//Free Run
			
			setFreeRunMode();
		}
		else if (iFlag==1)
		{//SoftWare
			setSoftWareMode();
			
		}
		else if(iFlag==2)
		{//HardWare
			setHardWareMode();
		}
		m_iFlag = iFlag;
	}
	catch (const GenericException &e)
	{
		//CString str ;
		//str.Format("%s\r\n",e.GetDescription());
		//OutputDebugString(str);
	}
	catch (...)
	{

	}
	return 0;
}
long BaslerImp::Grab(void*& pImageBuffer,size_t &nWidth,size_t &nHeight,size_t &nChannel)
{
	long rsTime = 0;
	try
	{
		
		if (m_pCamera == NULL)
		{
			return -1;
		}
		const uint8_t *pImageBuff = NULL;
		if (m_iFlag==0)
		{//连续采集

			long start =GetTickCount();
			bool rst = m_pCamera->GrabOne(5000,ptrGrabResult);
			long end = GetTickCount();

			rsTime= end -start;

			if (rst == false)
			{//失败
				return -1;
			}

			if (ptrGrabResult && ptrGrabResult->GrabSucceeded())
			{
				pImageBuff = (uint8_t *) ptrGrabResult->GetBuffer();

				nWidth = ptrGrabResult->GetWidth();
				nHeight = ptrGrabResult->GetHeight();
				nChannel = ptrGrabResult->GetImageSize()/(nWidth*nHeight);
				pImageBuffer=(void*)pImageBuff;

				//亮度
				/*if(nChannel==1)
				{
					
				}
				else if(nChannel==3)
				{
					pImageBuffer=(uint32_t*) pImageBuff;
				}*/
			}
		
		}
		else if (m_iFlag==1)
		{//软触发--->回调

			if (nSoftFlag==0)
			{				
			//	m_pCamera->StartGrabbing( GrabStrategy_OneByOne, GrabLoop_ProvidedByInstantCamera);
			 ///  GrabLoop_ProvidedByInstantCamera, ///< The grab loop thread is provided by the Instant Camera. It calls RetrieveResult() in a loop. Grabbed images are processed by registered image event handlers. The grab loop thread is started when the grab starts.
              /// 此方式，只能进入OnImageGrabbed去获取图像数据

				m_pCamera->StartGrabbing( GrabStrategy_OneByOne, GrabLoop_ProvidedByUser);
			///	GrabLoop_ProvidedByUser           ///< The user code calls RetrieveResult() in a loop to process grabbed images and camera events.
			/// 此方式首先可以进入OnImageGrabbed，如果这里面不去获取数据，那么数据可以在自己的循环中去调用RetrieveResult去获取，例如下面的软件触发方式


				nSoftFlag = 1;
			}
			
			long start =GetTickCount();
			if (m_pCamera->IsGrabbing())
			{
				ExecuteSoftTrig();

			/*	if ( m_pCamera->WaitForFrameTriggerReady( 500, TimeoutHandling_ThrowException) && nSoftFlag==1)
				{
					m_pCamera->ExecuteSoftwareTrigger();
				}*/
			}

			 // Wait for an image and then retrieve it. A timeout of 50000 ms is used.
                m_pCamera->RetrieveResult( 50000, ptrGrabResult, TimeoutHandling_ThrowException);

            // Image grabbed successfully
            if (ptrGrabResult && ptrGrabResult->GrabSucceeded())
            {
				pImageBuff = (uint8_t *) ptrGrabResult->GetBuffer();

				nWidth = ptrGrabResult->GetWidth();
				nHeight = ptrGrabResult->GetHeight();
				nChannel = ptrGrabResult->GetImageSize()/(nWidth*nHeight);

				pImageBuffer=(void*)pImageBuff;
		    }

			long end = GetTickCount();

			rsTime= end -start;

			//WaitForSingleObject(m_SampleImageEventHandler.m_event,1000);
		}
	}
	catch (const GenericException &e)
	{
		//CString str ;
		//str.Format("%s\r\n",e.GetDescription());
		//OutputDebugString(str);
		//AfxMessageBox(str);
	}
	catch(...)
	{
	
	}

	return 0;	
}

long BaslerImp::Grab(void* &pImageBuffer,size_t &nWidth,size_t &nHeight,size_t &nChannel, size_t &nImageSize, EPixelType &ePixelType, uint32_t &nPaddingX)
{
	long rsTime = 0;
	try
	{
		
		if (m_pCamera == NULL)
		{
			return -1;
		}
		const uint8_t *pImageBuff = NULL;
		if (m_iFlag==0)
		{//单张采集

			long start =GetTickCount();
			bool rst = m_pCamera->GrabOne(5000,ptrGrabResult);
			long end = GetTickCount();

			rsTime= end -start;

			if (rst == false)
			{//失败
				return -1;
			}

			if (ptrGrabResult && ptrGrabResult->GrabSucceeded())
			{
				pImageBuff = (uint8_t *) ptrGrabResult->GetBuffer();

				nWidth = ptrGrabResult->GetWidth();
				nHeight = ptrGrabResult->GetHeight();
				nChannel = ptrGrabResult->GetImageSize()/(nWidth*nHeight);
				nImageSize = ptrGrabResult->GetImageSize();
				ePixelType = ptrGrabResult->GetPixelType();
				nPaddingX = ptrGrabResult->GetPaddingX();
				pImageBuffer=(void*)pImageBuff;

				//亮度
				/*if(nChannel==1)
				{
					
				}
				else if(nChannel==3)
				{
					pImageBuffer=(uint32_t*) pImageBuff;
				}*/
			}

		
		}
		else if (m_iFlag==1)
		{//软触发--->回调

			if (nSoftFlag==0)
			{				
			//	m_pCamera->StartGrabbing( GrabStrategy_OneByOne, GrabLoop_ProvidedByInstantCamera); // only enter OnImageGrabbed

				m_pCamera->StartGrabbing( GrabStrategy_OneByOne, GrabLoop_ProvidedByUser); //both  enter OnImageGrabbed and RetrieveResult when needed
				
				nSoftFlag = 1;
			}
			
			long start =GetTickCount();
			if (m_pCamera->IsGrabbing())
			{
			//	ExecuteSoftTrig();

				if ( m_pCamera->WaitForFrameTriggerReady( 5000, TimeoutHandling_ThrowException) && nSoftFlag==1)
				{
					m_pCamera->ExecuteSoftwareTrigger();
				}
			}

			 // Wait for an image and then retrieve it. A timeout of 50000 ms is used.
                m_pCamera->RetrieveResult( 50000, ptrGrabResult, TimeoutHandling_ThrowException);

            // Image grabbed successfully
            if (ptrGrabResult && ptrGrabResult->GrabSucceeded())
            {
				pImageBuff = (uint8_t *) ptrGrabResult->GetBuffer();

				nWidth = ptrGrabResult->GetWidth();
				nHeight = ptrGrabResult->GetHeight();
				nChannel = ptrGrabResult->GetImageSize()/(nWidth*nHeight);
				nImageSize = ptrGrabResult->GetImageSize();
				ePixelType = ptrGrabResult->GetPixelType();
				nPaddingX = ptrGrabResult->GetPaddingX();

				pImageBuffer=(void*)pImageBuff;
		    }

			long end = GetTickCount();

			rsTime= end -start;

			//WaitForSingleObject(m_SampleImageEventHandler.m_event,1000);
		}
	}
	catch (const GenericException &e)
	{
		//CString str ;
		//str.Format("%s\r\n",e.GetDescription());
		//OutputDebugString(str);
		//AfxMessageBox(str);
	}
	catch(...)
	{
	
	}

	return 0;	

}


long BaslerImp::GrabContiune()
{
	try
	{
		if (m_pCamera == NULL)
		{
			return -1;
		}
		if (m_pCamera->IsGrabbing())
		{
			nSoftFlag = 1;
		}
		else
		{
			//使用GrabLoop_ProvidedByInstantCamera 才能自动进入OnImageGrabbed的回调函数
			m_pCamera->StartGrabbing( GrabStrategy_OneByOne, GrabLoop_ProvidedByInstantCamera); 

			//如果希望在自己的线程通过RetrieveResult来获取数据，那么这个参数需要用GrabLoop_ProvidedByUser才可以实现

			nSoftFlag = 1;
		}
		
	}
	catch (const GenericException &e)
	{
		//CString str ;
		//str.Format("%s\r\n",e.GetDescription());
		//OutputDebugString(str);
	}
	catch(...)
	{
	
	}
	return 0;
}

long BaslerImp::Grabstop()
{
	try
	{
		if (m_pCamera == NULL)
		{
			return -1;
		}
		if (m_pCamera->IsGrabbing())
		{
			m_pCamera->StopGrabbing();
			nSoftFlag = 0;
		}

	}
	catch (const GenericException &e)
	{
		//CString str ;
		//str.Format("%s\r\n",e.GetDescription());
		//OutputDebugString(str);
	}
	catch(...)
	{
	
	}
	return 0;

}
long BaslerImp::CloseCamera()
{

	try
	{
		if (m_pCamera == NULL)
		{
			return -1;
		}

		if(m_pCamera->IsOpen())
		{
			m_pCamera->DeregisterImageEventHandler(this);// 需要在Close之前Deregister，顺序不能反过来
			m_pCamera->DeregisterConfiguration(this); // 如果不注册那么退出会报错提示

			if(!m_pCamera->IsCameraDeviceRemoved())			
			{
				m_pCamera->Close();		
			}
			
		}
	}
	catch (const GenericException &e)
	{
		//CString str ;
		//str.Format("%s\r\n",e.GetDescription());
		//OutputDebugString(str);
	}
	catch(...)
	{

	}

	return 0;
}
int BaslerImp::ReAttachCameraIfLost(void)
{
	try
	{
	
        // Create a device info object for remembering the camera properties.
        CDeviceInfo info;

        // Remember the camera properties that allow detecting the same camera again.
        info.SetDeviceClass(m_pCamera->GetDeviceInfo().GetDeviceClass());
        info.SetSerialNumber(m_pCamera->GetDeviceInfo().GetSerialNumber());

        // Destroy the Pylon Device representing the detached camera device.
        // It cannot be used anymore.
        m_pCamera->DestroyDevice();

        // Create a filter containing the CDeviceInfo object info which describes the properties of the device we are looking for.
        DeviceInfoList_t filter;
        filter.push_back(info);    

		CTlFactory *pTlFactory = &CTlFactory::GetInstance ();
		DeviceInfoList_t m_devices;

        for (int i = 0; i < 80; i++)
        {
            // Try to find the camera we are looking for.
            if (pTlFactory->EnumerateDevices(m_devices, filter) > 0)
            {

                // The camera has been found. Create and attach it to the Instant Camera object.
                m_pCamera->Attach(pTlFactory->CreateDevice(m_devices[0]));
                    
                //Exit waiting
                break;
            }

            Sleep(250);
        }

		string sCamInfo = m_pCamera->GetDeviceInfo().GetUserDefinedName();

		//map<string,CInstantCamera*> mapCameras;
		mapCameras[sCamInfo]= m_pCamera;

		////////////////////////
#if define 0
				// Get the transport layer factory.
		CTlFactory& tlFactory = CTlFactory::GetInstance();

		// Get all attached devices and exit application if no device is found.
		DeviceInfoList_t devices;
		if ( tlFactory.EnumerateDevices(devices) == 0 )
		{
			MessageBoxW(NULL,L"No camera present!", L"Error Message", MB_OK | MB_ICONEXCLAMATION);
			return 0;
		}
		cameraInfo.clear();
		mapCameras.clear();
		cameraNum = devices.size();
		// Create and attach all Pylon Devices.
		for ( size_t i = 0; i < cameraNum; ++i)
		{
			CInstantCamera* temCamera = &cameras[ i ];	
			temCamera->Attach( tlFactory.CreateDevice( devices[ i ]));
		//	CInstantCamera temCamera( tlFactory.CreateDevice( devices[i]));
	
			

		//	string strDeviceName = temCamera->GetDeviceInfo().GetModelName();
			string strSerialNumber = temCamera->GetDeviceInfo().GetSerialNumber();
			if(temCamera->IsOpen())
			{
				temCamera->Close();
			}


		//	string sCamInfo = strDeviceName + "," + strSerialNumber;

			string sCamInfo = temCamera->GetDeviceInfo().GetUserDefinedName();

			//map<string,CInstantCamera*> mapCameras;
			mapCameras[sCamInfo]= temCamera;

			//vector<string>& cameraInfo
			cameraInfo.push_back(sCamInfo);
		}

# endif
	}
	catch (GenICam::GenericException &e)
    {
        if (m_pCamera->IsCameraDeviceRemoved())
        {
//            ::AfxMessageBox(_T("Error:Recover Camera fail!"));
        }
        else
        {
  //          ::AfxMessageBox(_T("Error:Recover camera error!"));
        }
    }

return 1;

}

long BaslerImp::GetCameraName( string& cameraName )
{
	try
	{
		if (m_pCamera == NULL)
		{
			return -1;
		}

		cameraName = m_pCamera->GetDeviceInfo().GetModelName();

	}
	catch (const GenericException &e)
	{
	}
	catch(...)
	{

	}

	return 0;
}
long BaslerImp::GetCameraSerialNumber(string& cameraSerialNumber)
{
	try
	{
		if (m_pCamera == NULL)
		{
			return -1;
		}

		cameraSerialNumber = m_pCamera->GetDeviceInfo().GetSerialNumber();

	}
	catch (const GenericException &e)
	{
	}
	catch(...)
	{

	}
	return 0;
}

bool BaslerImp::SetWidth( int nWidth)
{
	//INodeMap& nodemap = m_InstantCamera.GetNodeMap();

	bool btn = true;
	if (m_pCamera == NULL)
	{
		return false ;
	}
	if (!m_pCamera->IsOpen() || m_pCamera->IsCameraDeviceRemoved())
	{//is not opened
		return 0;
	}
	CIntegerPtr Ptrwidth( m_pCamera->GetNodeMap().GetNode( "Width"));

	if ( IsWritable( Ptrwidth))
	{
		int64_t newwidth = Adjust(nWidth, Ptrwidth->GetMin(), Ptrwidth->GetMax(), Ptrwidth->GetInc());
		Ptrwidth->SetValue(newwidth);
	}


	return btn;

}

bool BaslerImp::SetHeight(int nHeight)
{
	bool btn = true;
	if (m_pCamera == NULL)
	{
		return false;
	}
	if (!m_pCamera->IsOpen() || m_pCamera->IsCameraDeviceRemoved())
	{//is not opened
		return 0;
	}
	CIntegerPtr Ptrheight( m_pCamera->GetNodeMap().GetNode( "Height"));

	if ( IsWritable( Ptrheight))
	{
		int64_t newHeight = Adjust(nHeight, Ptrheight->GetMin(), Ptrheight->GetMax(), Ptrheight->GetInc());
		Ptrheight->SetValue(newHeight);
	}
	return btn;
}

int BaslerImp::GetWidth()
{
	int nWidth = 0;
	if (m_pCamera == NULL)
	{
		return false;
	}
	if (!m_pCamera->IsOpen() || m_pCamera->IsCameraDeviceRemoved())
	{//is not opened
		return 0;
	}
	CIntegerPtr Ptrwidth( m_pCamera->GetNodeMap().GetNode( "Width"));

	if ( IsReadable( Ptrwidth))
	{
		nWidth = Ptrwidth->GetValue();
	}

	return nWidth;
}
int BaslerImp::GetHeight()
{
	int nHeight = 0;

	if (m_pCamera == NULL)
	{
		return false;
	}

	if (!m_pCamera->IsOpen() || m_pCamera->IsCameraDeviceRemoved() )
	{//is not opened
		return 0;
	}
	CIntegerPtr Ptrheight( m_pCamera->GetNodeMap().GetNode( "Height"));

	if ( IsReadable( Ptrheight))
	{
		nHeight = Ptrheight->GetValue();
	}

	return nHeight;
}

int BaslerImp::GetWidthMax()
{
	int nWidthMax = 0;
	if (m_pCamera == NULL)
	{
		return false;
	}
	if (!m_pCamera->IsOpen() || m_pCamera->IsCameraDeviceRemoved())
	{//is not opened
		return 0;
	}
	CIntegerPtr Ptrwidth( m_pCamera->GetNodeMap().GetNode( "Width"));

	if ( IsReadable( Ptrwidth))
	{
		nWidthMax = Ptrwidth->GetMax();
	}

	return nWidthMax;
}
int BaslerImp::GetHeightMax()
{
	int nHeightMax = 0;
	if (m_pCamera == NULL)
	{
		return false;
	}
	if (!m_pCamera->IsOpen() || m_pCamera->IsCameraDeviceRemoved())
	{//is not opened
		return 0;
	}
	CIntegerPtr Ptrheight( m_pCamera->GetNodeMap().GetNode( "Height"));

	if ( IsReadable( Ptrheight))
	{
		nHeightMax = Ptrheight->GetMax();
	}
	
	return nHeightMax;
}

int BaslerImp::SetHeartBeatTime(int64_t nValueMs)
{
	CIntegerPtr m_pHeartbeatTimeout; // ms timeout
	m_pHeartbeatTimeout = m_pCamera->GetTLNodeMap().GetNode("HeartbeatTimeout");

	if (!m_pCamera->IsOpen() || m_pCamera->IsCameraDeviceRemoved())
	{//is not opened
		return -1;
	}

	// Do nothing if no heartbeat feature is available.
    if (!m_pHeartbeatTimeout.IsValid())
         return -1;

    // Apply the increment and cut off invalid values if neccessary.
       int64_t correctedValue =Adjust(nValueMs, m_pHeartbeatTimeout->GetMin(), m_pHeartbeatTimeout->GetMax(), m_pHeartbeatTimeout->GetInc());

       m_pHeartbeatTimeout->SetValue(correctedValue);

	   return 0;
}
#endif