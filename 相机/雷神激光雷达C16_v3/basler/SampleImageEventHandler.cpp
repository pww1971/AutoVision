//#include "StdAfx.h"
#include "SampleImageEventHandler.h"
#ifdef _BASLER_CAM
SampleImageEventHandler::SampleImageEventHandler(void)
{
}

SampleImageEventHandler::~SampleImageEventHandler(void)
{
}

//回调函数
void SampleImageEventHandler::OnImageGrabbed( CInstantCamera& camera, const CGrabResultPtr& grabResult )
{
	CWidth = grabResult->GetWidth();//图像数据宽
	CHeight = grabResult->GetHeight();//图像数据高
	byte *pImageBuffer = (byte*)grabResult->GetBuffer();//图像数据

	//保存图片
	//CImagePersistence::Save( ImageFileFormat_Bmp, "D:\\GrabbedImage.bmp", grabResult);//ImageFileFormat_Png

	Pylon::DisplayImage(1,grabResult);//窗口显示图像
}
#endif