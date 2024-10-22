//#include "StdAfx.h"
#include "BaslerCamer.h"
#ifdef _BASLER_CAM
using namespace GENICAM_NAMESPACE;
BaslerCamer::BaslerCamer(void)
{
}

BaslerCamer::~BaslerCamer(void)
{
}

void BaslerCamer::CamInit()
{
	Pylon::PylonInitialize();
	camera.Attach(CTlFactory::GetInstance().CreateFirstDevice());//ʵ������һ���ҵ������Camera

	//ͨ��SN�Ŵ����
	//di.SetSerialNumber("20928685");
	//ͨ��������ƴ����
	//di.SetModelName("acA1300-30gm");
	//ͨ���Զ���������ƴ����
	//di.SetUserDefinedName("Cam1");
	//camera.Attach(CTlFactory::GetInstance().CreateDevice(di));//ʵ����ָ�����

	nodemap = &camera.GetNodeMap();//��ʼ��node���Խڵ�

	camera.RegisterConfiguration(new CAcquireContinuousConfiguration,RegistrationMode_ReplaceAll,Cleanup_Delete);//ע��ʵʱģʽ

	camera.RegisterImageEventHandler(new SampleImageEventHandler, RegistrationMode_Append, Cleanup_Delete);//ע��ص�
	camera.MaxNumBuffer = 5;
	camera.Open();//�������
	
	//camera.StartGrabbing(GrabStrategy_OneByOne,GrabLoop_ProvidedByInstantCamera);
}

void BaslerCamer::CamStart()
{
	camera.StartGrabbing(GrabStrategy_OneByOne,GrabLoop_ProvidedByInstantCamera);
}

void BaslerCamer::CamStop()
{
	camera.StopGrabbing();
}

void BaslerCamer::CamDestroy()
{
	camera.StopGrabbing();
	camera.DestroyDevice();
	Pylon::PylonTerminate();
}

string BaslerCamer::CamName()
{
	CStringPtr DeviceName(nodemap->GetNode("DeviceModelName"));
	return ((string)DeviceName->ToString());
}

string BaslerCamer::CamSerialNum()
{
	CStringPtr DeviceID(nodemap->GetNode("DeviceID"));
	return ((string)DeviceID->ToString());
}

string BaslerCamer::CamFormat()
{
	CEnumerationPtr pixelFormat(nodemap->GetNode("PixelFormat"));
	
	return ((string)pixelFormat->ToString());
}

double BaslerCamer::CamFrameRate()
{
	CFloatPtr FrameRate(nodemap->GetNode("ResultingFrameRateAbs"));
	return ((double)FrameRate->GetValue());
	
}

int BaslerCamer::CamWidth()
{
	CIntegerPtr CWidth(nodemap->GetNode("SensorWidth"));
	return CWidth->GetValue();
}

int BaslerCamer::CamHeight()
{
	CIntegerPtr CHeight(nodemap->GetNode("SensorHeight"));
	return CHeight->GetValue();
	
}

double BaslerCamer::GetExpMin()
{
	CFloatPtr ExpTimeMin(nodemap->GetNode("ExposureTimeAbs"));
	return ((double)ExpTimeMin->GetMin());
}

double BaslerCamer::GetExpMax()
{
	CFloatPtr ExpTimeMax(nodemap->GetNode("ExposureTimeAbs"));
	return ((double)ExpTimeMax->GetMax());
}

double BaslerCamer::GetExpCur()
{
	CFloatPtr ExpTimeCur(nodemap->GetNode("ExposureTimeAbs"));
	return ((double)ExpTimeCur->GetValue());
}

void BaslerCamer::SetExp( double ExpTime )
{
	CEnumerationPtr ExpAuto(nodemap->GetNode("ExposureAuto"));
	if (IsWritable(ExpAuto))
	{
		ExpAuto->FromString("Off");
	}
	CFloatPtr SetExpTime(nodemap->GetNode("ExposureTimeAbs"));

	if (ExpTime <SetExpTime->GetMin())
	{
		SetExpTime->SetValue(SetExpTime->GetMin());
	}
	else if (ExpTime > SetExpTime->GetMax())
	{
		SetExpTime->SetValue(SetExpTime->GetMax());
	}
	else
	{
		SetExpTime->SetValue(ExpTime);
	}
}

int BaslerCamer::GetGainMin()
{
	CIntegerPtr GainValueMin(nodemap->GetNode("GainRaw"));
	return ((int)GainValueMin->GetMin());
}

int BaslerCamer::GetGainMax()
{
	CIntegerPtr GainValueMax(nodemap->GetNode("GainRaw"));
	return ((int)GainValueMax->GetMax());
}

int BaslerCamer::GetGainCur()
{
	CIntegerPtr GainValueCur(nodemap->GetNode("GainRaw"));
	return ((int)GainValueCur->GetValue());
}

void BaslerCamer::SetGain( int GainValue )
{
	CEnumerationPtr GainAuto(nodemap->GetNode("GainAuto"));
	if (IsWritable(GainAuto))
	{
		GainAuto->FromString("Off");
	}

	CIntegerPtr SetGainValue(nodemap->GetNode("GainRaw"));
	if (GainValue < SetGainValue->GetMin())
	{
		SetGainValue->SetValue(SetGainValue->GetMin());
	}
	else if (GainValue > SetGainValue->GetMax())
	{
		SetGainValue->SetValue(SetGainValue->GetMax());
	}
	else
	{
		SetGainValue->SetValue(GainValue);
	}
}

void BaslerCamer::SoftTrigEnable( string flag )
{
	CEnumerationPtr triggerSelector(nodemap->GetNode("TriggerSelector"));
	CEnumerationPtr triggerMode(nodemap->GetNode("TriggerMode"));
	CEnumerationPtr triggerSource(nodemap->GetNode("TriggerSource"));

	triggerSelector->FromString("AcquisitionStart");//FrameStart
	triggerMode->FromString((gcstring)flag.c_str());
	triggerSource->FromString("Software");
}

void BaslerCamer::ExecuteSoftTrig()
{
	CCommandPtr SoftExecute(nodemap->GetNode("TriggerSoftware"));
	SoftExecute->Execute();
}

void BaslerCamer::ExtTrigEnable( string flag )
{
	CEnumerationPtr triggerSelector(nodemap->GetNode("TriggerSelector"));
	CEnumerationPtr triggerMode(nodemap->GetNode("TriggerMode"));
	CEnumerationPtr triggerSource(nodemap->GetNode("TriggerSource"));

	triggerSelector->FromString("AcquisitionStart");//FrameStart
	triggerMode->FromString((gcstring)flag.c_str());
	triggerSource->FromString("Line1");
}

#endif
