
#include "CameraDrv.h"

HCameraDrv::HCameraDrv()
{
	m_iType = -1;

}

HCameraDrv::~HCameraDrv()
{

}

int HCameraDrv::grapImage(int id, HImage& ho_Image)//针对海康组播模式 仅仅触发主机的相机 不采图
{
#ifdef _SIMULATION
	string dir = getExePath() + "simulation/" + CStrProcess::itoa_radixN(id) + "/";
	replace(dir.begin(), dir.end(), '\\', '/');
	// Local control variables
	HTuple  hv_ImageFiles, hv_Index;

	//* // Image Acquisition 01: Code generated by Image Acquisition 01
	ListFiles(dir.c_str(), (HTuple("files").Append("follow_links")), &hv_ImageFiles);
	TupleRegexpSelect(hv_ImageFiles, (HTuple("\\.(tif|tiff|gif|bmp|jpg|jpeg|jp2|png|pcx|pgm|ppm|pbm|xwd|ima|hobj)$").Append("ignore_case")),
		&hv_ImageFiles);
	{
		//HTuple end_val3 = (hv_ImageFiles.TupleLength()) - 1;
		//HTuple step_val3 = 1;
		//for (hv_Index = 0; hv_Index.Continue(end_val3, step_val3); hv_Index += step_val3)
		//{
		//	ReadImage(&ho_Image, HTuple(hv_ImageFiles[hv_Index]));
		//	//    * // Image Acquisition 01: Do something
		//}
	}
	static u_int sid = 0;
	ReadImage(&ho_Image, HTuple(hv_ImageFiles[sid++ % hv_ImageFiles.TupleLength().I()]));

	return 0;
#endif
	if (m_iType == -1)
	{
		m_hcam.GrabOneImageAsync(&ho_Image);
		return 0;
	}
	else if (m_iType == 0)
	{
		return -1;
	}
	else
	{
		return -1;
	}
}

int HCameraDrv::ini()
{


	if (m_iType == 0)
	{
	}
	else
	{
	}
	return 1;
}

void  HCameraDrv::release()
{
	if (m_iType == 0)
	{
	}
	else
	{
	}
}

int HCameraDrv::setCamType(int t)
{
	return m_iType = t;
}
