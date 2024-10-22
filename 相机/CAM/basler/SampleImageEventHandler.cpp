//#include "StdAfx.h"
#include "SampleImageEventHandler.h"
#ifdef _BASLER_CAM
SampleImageEventHandler::SampleImageEventHandler(void)
{
}

SampleImageEventHandler::~SampleImageEventHandler(void)
{
}

//�ص�����
void SampleImageEventHandler::OnImageGrabbed( CInstantCamera& camera, const CGrabResultPtr& grabResult )
{
	CWidth = grabResult->GetWidth();//ͼ�����ݿ�
	CHeight = grabResult->GetHeight();//ͼ�����ݸ�
	byte *pImageBuffer = (byte*)grabResult->GetBuffer();//ͼ������

	//����ͼƬ
	//CImagePersistence::Save( ImageFileFormat_Bmp, "D:\\GrabbedImage.bmp", grabResult);//ImageFileFormat_Png

	Pylon::DisplayImage(1,grabResult);//������ʾͼ��
}
#endif