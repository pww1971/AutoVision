
#include "pch.h"
#include "checkPatch.h"

HImage Mat2HObject(cv::Mat& image)
{
	HImage Hobj = HImage();
	if (NULL == image.data)
		return Hobj;
	int h = image.rows;
	int w = image.cols;
	//	int al=h*w;
	//	CV_8UC3
	if (image.type() == CV_8UC3)
	{
		cv::Mat imgchannel[3];
		split(image, imgchannel);
		cv::Mat imgB = imgchannel[0];
		cv::Mat imgG = imgchannel[1];
		cv::Mat imgR = imgchannel[2];

		try
		{
			GenImage3(&Hobj, "byte", (Hlong)w, (Hlong)h, (Hlong)imgR.data, (Hlong)imgG.data, (Hlong)imgB.data);
		}
		catch (...)
		{
			//ofstream fp;
			//fp.open("./err.txt",ios::app);
			//fp << "halcon GenImage3 err" << /*"   " << (*(HTuple*)pa.m_dllPa.p).ToString()<<*/endl;
			//fp.close();
		}

	}
	//	CV_8UCU1
	else if (image.type() == CV_8UC1)
	{
		try
		{
			GenImage1(&Hobj, "byte", (Hlong)w, (Hlong)h, (Hlong)image.data);
		}
		catch (...)
		{
			//ofstream fp;
			//fp.open("./err.txt",ios::app);
			//fp << "halcon GenImage1 err" << /*"   " << (*(HTuple*)pa.m_dllPa.p).ToString()<<*/endl;
			//fp.close();
		}

	}
	return Hobj;
}
cv::Mat HObject2Mat(HImage& Hobj)
{
	HTuple htCh = HTuple();
	HTuple cType;
	cv::Mat Image;
	ConvertImageType(Hobj, &Hobj, "byte");
	CountChannels(Hobj, &htCh);
	HTuple wid;
	HTuple hgt;
	int W, H;
	if (htCh[0].I() == 1)
	{
		HTuple ptr;
		GetImagePointer1(Hobj, &ptr, &cType, &wid, &hgt);
		W = wid[0].I();
		H = hgt[0].I();
		Image.create(H, W, CV_8UC1);
		uchar* pdata = (uchar*)(Hlong)ptr;
		memcpy(Image.data, pdata, W * H);
	}
	else if (htCh[0].I() == 3)
	{
		HTuple ptrR, ptrG, ptrB;
		GetImagePointer3(Hobj, &ptrR, &ptrG, &ptrB, &cType, &wid, &hgt);
		W = wid[0].I();
		H = hgt[0].I();

		Image.create(H, W, CV_8UC3);
		vector<cv::Mat> vecM(3);
		vecM[2].create(H, W, CV_8UC1);
		vecM[1].create(H, W, CV_8UC1);
		vecM[0].create(H, W, CV_8UC1);
		uchar* pr = (uchar*)(Hlong)ptrR;
		uchar* pg = (uchar*)(Hlong)ptrG;
		uchar* pb = (uchar*)(Hlong)ptrB;
		memcpy(vecM[2].data, pr, W * H);
		memcpy(vecM[1].data, pg, W * H);
		memcpy(vecM[0].data, pb, W * H);
		merge(vecM, Image);
	}
	return Image;
}

checkPatch::checkPatch()
{
	m_configAdd = "";
}

checkPatch::~checkPatch()
{
}


//图片的处理	一保存模板图片，二加载模板图片进行旋转角度后匹配
bool checkPatch::treatPicture(CTpltParam& pa, vector<S_TpltData*>& vctData)
{
	int curid = pa.m_tempPa.m_curPosID;
	vector<cv::Mat>& src = vctData[curid]->m_vctPic;
	if (!src[0].data)
		return false;

	if (pa.m_dllPa.ini < 1)
	{
		if ("" != pa.m_dllPa.szfile)
		{
			readMode(pa.m_dllPa.szfile);
		}		
		if (pa.m_dllPa.ini == 0)
		{
			;
		}
		else//ini == -1
		{
			textureDlg dlg;
			dlg.m_pCheck = this;
			dlg.m_ImageOrg = Mat2HObject(src[0]);
			int treatid = pa.m_dllPa.mutilMat;
			if (treatid < 0)
			{
				treatid = src.size() - 1;
			}
			if (1 == dlg.DoModal())
			{
				//writeMode(pa.m_dllPa.szfile);
			}
		}
		pa.m_dllPa.ini = 1;
		return true;
	}
	else
	{
		//处理图片
		if (treatPic(vctData[curid], pa.m_dllPa.mutilMat))
		{
			vctData[curid]->m_checkRet = true;
			return true;
		}
	}
	return false;
}

bool checkPatch::treatPic(S_TpltData* data,int treatid)
{
	try
	{
		if (treatid < 0)
		{
			treatid = data->m_vctPic.size() - 1;
		}
		if (treatid >= data->m_vctPic.size())
		{
			data->m_szRet = "处理图编号超范围";
			return false;
		}
		HImage ho_Image = Mat2HObject(data->m_vctPic[treatid]);
		if (treatPic(ho_Image))
		{
			data->m_vctPic[treatid] = HObject2Mat(ho_Image);
			return true;
		}
		else
			return false;
	}
	catch (HException& except)
	{
		data->m_szRet = except.ErrorText().Text();
		return false;
	}
}

bool checkPatch::treatPic(HImage& ho_Image)
{
	//写处理流程如下
	if (m_pa.hv_NoveltyThreshold.TupleLength() > 0)//阈值
	{
		HTuple hv_Threshold;
		GetTextureInspectionModelParam(m_pa.hv_TextureInspectionModel, "novelty_threshold",
			&hv_Threshold);
		SetTextureInspectionModelParam(m_pa.hv_TextureInspectionModel, "novelty_threshold",
			hv_Threshold + m_pa.hv_NoveltyThreshold);
	}
	HObject ho_NoveltyRegion;
	HTuple hv_TextureInspectionResultID;
	ApplyTextureInspectionModel(ho_Image, &ho_NoveltyRegion, m_pa.hv_TextureInspectionModel,
		&hv_TextureInspectionResultID);
	
	HTuple  hv_Width, hv_Height;
	GetImageSize(ho_Image, &hv_Width, &hv_Height);
	HTuple htCh = HTuple();
	CountChannels(ho_Image, &htCh);
	Connection(ho_NoveltyRegion, &ho_ConnectedRegions);
	if (htCh[0].I() == 1)
	{
		RegionToBin(ho_ConnectedRegions, &ho_Image, 255, 0, hv_Width, hv_Height);
	}
	else
	{
		HImage ho_Mask;
		RegionToBin(ho_ConnectedRegions, &ho_Mask, 255, 0, hv_Width, hv_Height);
		Compose3(ho_Mask, ho_Mask, ho_Mask, &ho_Image);
	}
	return true;
}

void checkPatch::readMode(string file)
{
	if (file != "")
	{
		mkdirs((char*)file.c_str());
		m_configAdd = file;
	}

	m_pa.readcfgfile(m_configAdd + "/checkParam");
}

void checkPatch::writeMode(string file)
{
	if (file != "")
	{
		m_configAdd = file;
	}
	delCfgFiles(m_configAdd.c_str(), 1);
	mkdirs((char*)m_configAdd.c_str());
	m_pa.writecfgfile(m_configAdd + "/checkParam");
}

void checkPatch::clone(void* p)
{
	m_pa = ((checkPatch*)p)->m_pa;	
	readMode(((checkPatch*)p)->m_configAdd);	
}

bool checkPatch::drawResult(cv::Mat& src, cv::RotatedRect& roiRect, CTpltParam& pa)
{
	return true;
}

