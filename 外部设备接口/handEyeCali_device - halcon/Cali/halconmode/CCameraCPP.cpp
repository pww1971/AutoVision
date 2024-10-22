#include "stdafx.h"
#include "MyData.h"
#include "CCameraCPP.h"

using namespace HalconCpp;
using namespace std;

CCameraCPP::CCameraCPP()
{
	m_pFG = NULL;
	bIsOpened = FALSE;

	this->m_bFGIsRunning = FALSE;

	//this->iWidth = -1;
	//this->iHeight = -1;


	// ����궨

	bOnePIcMaxErrorOK = false;              // �궨ͼ������OK

	InitializeCriticalSection(&newImageMutex);
	newImageEven = CreateEventW(NULL, FALSE, FALSE, NULL);

	ResetEvent(newImageEven);

}

CCameraCPP::~CCameraCPP()
{
	if (m_pFG != NULL) delete m_pFG;

	DeleteCriticalSection(&newImageMutex);
	CloseHandle(this->newImageEven);
}



BOOL CCameraCPP::GetIsOpen()
{
	return this->bIsOpened;
}

//void CCameraCPP::SetShutter(int i)
//{
//	this->m_pFG->SetFramegrabberParam("����ʱ��", i);
//}
//
//void CCameraCPP::SetGain(double g)
//{
//	this->m_pFG->SetFramegrabberParam("ͼ������", g);
//}

//int CCameraCPP::GetWidth()
//{
//	return this->iWidth;
//}
//
//void CCameraCPP::SetWidth(int i)
//{
//	this->iWidth = i;
//}
//
//int CCameraCPP::GetHeight()
//{
//	return this->iHeight;
//}
//
//void CCameraCPP::SetHeight(int i)
//{
//	this->iHeight = i;
//}




int CCameraCPP::GetParameterInt(CString sPara)
{
	HalconCpp::HTuple hv_Value;
	//this->m_pFG->GetFramegrabberParam(CT2A(sPara).m_psz, &hv_Value);
	hv_Value = this->m_pFG->GetFramegrabberParam(CT2A(sPara).m_psz);
	return hv_Value.I();
}

CString CCameraCPP::GetParameterString(CString sPara)
{
	CString s;
	try {
		HalconCpp::HTuple hv_Value;
		hv_Value = this->m_pFG->GetFramegrabberParam(CT2A(sPara).m_psz);
		s.Format("%S", hv_Value.SArr()[0]);
	}
	catch (HalconCpp::HException& except) {
	//	UCI::sDB.Log(except, "GetParameterString()��");

	}
	return s;
}

double CCameraCPP::GetParameterDouble(CString sPara)
{
	HalconCpp::HTuple hv_Value;
	try {
		hv_Value = this->m_pFG->GetFramegrabberParam(CT2A(sPara).m_psz);
	}
	catch (HalconCpp::HException& except) {
	//	UCI::sDB.Log(except, "GetParameterDouble()��");

	}
	return hv_Value.D();
}


void CCameraCPP::SetParameterHTuple(std::string str, HalconCpp::HTuple ht)
{
	try {
		this->m_pFG->SetFramegrabberParam(str.c_str(), ht);
	}
	catch (HalconCpp::HException& except) {
	//	UCI::sDB.Log(except, "SetParameterHTuple()��");

	}
}

HalconCpp::HFramegrabber * CCameraCPP::GetPFG()
{
	return this->m_pFG;
}

BOOL CCameraCPP::Connect(CString str)
{

	CString interfaceName = UCI::OPTIONS["���Э��"];
	CString deviceName = UCI::OPTIONS["�������"];
	try {
		if (str != "NotFiles") {
			this->m_pFG = new HFramegrabber("File",             /* Name of Frame Grabber Interface */
				1, 1,               /* Image Resolution                */
				0, 0,               /* Image Size                      */
				0, 0,               /* Start Row and Column            */
				"default",          /* Fields                          */
				-1,                 /* Bits per channel                */
				"default",          /* Color space                     */
				-1.0,               /* Gain                            */
				"false",            /* External trigger                */
				CT2A(str).m_psz,    /* Camera type                     */
				"0",                /* Device                          */
				-1,                 /* Port                            */
				-1                  /* Line in                         */
			);
			this->OneShot();
		}
		else if (interfaceName == "DirectShow") {

			this->m_pFG = new HFramegrabber(CT2A(interfaceName).m_psz,  /* Name of Frame Grabber Interface */
				1, 1,							/* Image Resolution                */
				0, 0,                           /* Image Size                      */
				0, 0,                           /* Start Row and Column            */
				"default",                      /* Fields                          */
				8,								/* Bits per channel                */
				"rgb",                          /* Color space                     */
				-1.0,                           /* Gain                            */
				"false",                        /* External trigger                */
				"default",                      /* Camera type                     */
				CT2A(deviceName).m_psz,                     /* Device                          */
				0,                              /* Port                            */
				-1                              /* Line in                         */
			);


		}
		else if (interfaceName == "MindVision12_X64") {

			//if (deviceName == "CM500") {

			this->m_pFG = new HFramegrabber(CT2A(interfaceName).m_psz,   /* Name of Frame Grabber Interface */
				1, 1,							/* Image Resolution                */
				0, 0,                           /* Image Size                      */
				0, 0,                           /* Start Row and Column            */
				"progressive",                  /* Fields                          */
				8,								/* Bits per channel                */
				"Gray",                         /* Color space                     */
				-1.0,                           /* Gain                            */
				"false",                        /* External trigger                */
				"auto",                         /* Camera type                     */
				CT2A(deviceName).m_psz,         /* Device                          */
				0,                              /* Port                            */
				-1                              /* Line in                         */
			);

			//}
			//else if (deviceName == "CM-300") {
			//	this->m_pFG = new HFramegrabber(interfaceName.c_str(),  /* Name of Frame Grabber Interface */
			//		1, 1,							/* Image Resolution                */
			//		0, 0,                           /* Image Size                      */
			//		0, 0,                           /* Start Row and Column            */
			//		"progressive",                  /* Fields                          */
			//		8,								/* Bits per channel                */
			//		"Gray",                         /* Color space                     */
			//		-1.0,                           /* Gain                            */
			//		"false",                        /* External trigger                */
			//		"auto",                         /* Camera type                     */
			//		deviceName.c_str(),         /* Device                          */
			//		0,                              /* Port                            */
			//		-1                              /* Line in                         */
			//	);
			//}
			// ��ʼ�����
			//OpenFramegrabber("MindVision12_X64",
			//	1, 1, 0, 0, 0, 0, "progressive",
			//	8, "BGR24", -1, "false", "auto", CT2A(deviceName).m_psz, 0, -1, &hv_AcqHandle);

			//OpenFramegrabber("MindVision12_X64",
			//	1, 1, 0, 0, 0, 0, "progressive",
			//	8, "Gray", -1, "false", "auto", CT2A(deviceName).m_psz, 0, -1, &hv_AcqHandle);

			// open_framegrabber ('MindVision12_X64', 1, 1, 0, 0, 0, 0, 
			// 'progressive', 8, 'Gray', -1, 'false', 'auto', 'CM500', 0, -1, AcqHandle)
		}
		else {
			return FALSE;
		}

		HalconCpp::HTuple hv_Value;

		hv_Value = this->m_pFG->GetFramegrabberParam("image_width");
		UCI::OPTIONS["ͼ����"] = hv_Value.I();


		hv_Value = this->m_pFG->GetFramegrabberParam("image_height");
		UCI::OPTIONS["ͼ��߶�"] = hv_Value.I();
		
		//UCI::ELE.PrePareHoGrayImg();

		int w = UCI::OPTIONS["ͼ����"];
		int h = UCI::OPTIONS["ͼ��߶�"];

		// ����Ĳ���ҲҪ����һ�¡�
		UCI::OptionsMap *om = UCI::ELE.GetOptionsMapByEletype(*UCI::ELE.EleList,ELEMENT_CAMERA);
		(*om)["�������"] = w;
		(*om)["�����߶�"] = h;

		UCI::OPTIONS["ORGwhByCamera"] = 1;

		
	//	this->m_pFG->GrabImage();

		this->m_pFG->GrabImageStart(-1);	

		int delay = UCI::OPTIONS["������ʱ"];
		Sleep(delay);
		//HImage  GrabImage;
		//GrabImage = this->m_pFG->GrabImageAsync(-1.0f);

		this->bIsOpened = true;

	}
	catch (HalconCpp::HException& except) {
//		UCI::sDB.Log(except, "Connect(CString str): ");

		return FALSE;
	}

	//if (this->bIsOpened) {
	//	Sleep(1500);
	//	HImage  GrabImage;
	//	GrabImage = this->m_pFG->GrabImageAsync(-1.0f);
	//	


	//	//HImage  GrabImage;
	//	Sleep(1500);
	//	GrabImage = this->m_pFG->GrabImageAsync(-1.0f);
	//	Sleep(1500);
	//	//GrabImage = this->m_pFG->GrabImageAsync(-1.0f);
	//}

	return TRUE;
}

BOOL CCameraCPP::close_camera()
{
	if (this->m_pFG != NULL) {
		delete this->m_pFG;
		this->bIsOpened = false;
	}
	return TRUE;
}

//bool CCameraCPP::IsCaliPictureOk()
//{
//	return this->bOnePIcMaxErrorOK;
//}




void CCameraCPP::SetbOnePIcMaxErrorOK(bool b)
{
	this->bOnePIcMaxErrorOK = b;
}

void CCameraCPP::SetfError(double f)
{
	this->fError = f;
}



void CCameraCPP::OneShot()
{	

	try 
	{
		//Sleep(1000);
		HImage  GrabImage;
		GrabImage = this->m_pFG->GrabImageAsync(-1);
		// Image handoff
		EnterCriticalSection(&this->newImageMutex);
		if (this->imgList.size() < MAX_BUFFERS) {
			this->imgList.push_back(GrabImage);
		}
		LeaveCriticalSection(&this->newImageMutex);
		SetEvent(this->newImageEven);
	}
	catch (HalconCpp::HException& except) {
	//	UCI::sDB.Log(except, "OneShot()��");

	}
}

HBOOL CCameraCPP::GetIsFGRuning()
{
	HBOOL b;
	EnterCriticalSection(&newImageMutex);
	b = this->m_bFGIsRunning;
	LeaveCriticalSection(&newImageMutex);
	return b;
}

void CCameraCPP::SetFGRuning(HBOOL b)
{
	EnterCriticalSection(&this->newImageMutex);
	this->m_bFGIsRunning = b;
	LeaveCriticalSection(&this->newImageMutex);
}

void CCameraCPP::GetBufferImage(HalconCpp::HImage * img)
{
	//unsigned       idx;
	// get the new image buffer

	// HalconCpp::HImage img;

	try {

		EnterCriticalSection(&newImageMutex);

		if (imgList.size() > 0) {
			*img = this->imgList.front();
			this->imgList.pop_front();
		}
		LeaveCriticalSection(&newImageMutex);

		ResetEvent(this->newImageEven);
	}
	catch (HalconCpp::HException& except) {
	//	UCI::sDB.Log(except, "GetBufferImage()��");

	}

	return;

}

void CCameraCPP::GrabOneImageAsync(HalconCpp::HImage* img)
{
	//HImage      GrabImage;
	try {
		*img = this->m_pFG->GrabImageAsync(-1);

		// Image handoff
		EnterCriticalSection(&this->newImageMutex);
		if (this->imgList.size() < MAX_BUFFERS) {
			this->imgList.push_back(*img);
		}
		LeaveCriticalSection(&this->newImageMutex);

		SetEvent(this->newImageEven);
	}
	catch (HalconCpp::HException& except) {
	//	UCI::sDB.Log(except, "GrabOneImageAsync()��");

	}

	return;
}

void CCameraCPP::ResetIPNewImageEvent()
{
	ResetEvent(this->newImageEven);
}


HANDLE CCameraCPP::GetNewImageEvent()
{
	return this->newImageEven;
}
