
#include "stdafx.h"
#include "checkPatch.h"
#include "MyData.h"
CDesign_dlg_posDlg checkPatch::S_dlg;
//CThreadMgr g_threadMgr;
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
			ofstream fp;
			fp.open("./err.txt", ios::app);
			fp << "halcon GenImage3 err" << /*"   " << (*(HTuple*)pa.m_dllPa.p).ToString()<<*/endl;
			fp.close();
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
			ofstream fp;
			fp.open("./err.txt", ios::app);
			fp << "halcon GenImage1 err" << /*"   " << (*(HTuple*)pa.m_dllPa.p).ToString()<<*/endl;
			fp.close();
		}

	}
	return Hobj;
}
cv::Mat HObject2Mat(HImage Hobj)
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
Mat HImageToMat32Bit(HObject& imgHalcon)
{
	HTuple channels;
	HString cType;
	cv::Mat Image;
	ConvertImageType(imgHalcon, &imgHalcon, "uint2");
	CountChannels(imgHalcon, &channels);
	Hlong width = 0;
	Hlong height = 0;
	if (channels[0].I() == 1)
	{
		HImage hImg(imgHalcon);
		void* ptr = hImg.GetImagePointer1(&cType, &width, &height);//GetImagePointer1(Hobj, &ptr, &cType, &wid, &hgt);
		int W = width;
		int H = height;
		Image.create(H, W, CV_32FC1);
		unsigned char* pdata = static_cast<unsigned char*>(ptr);
		memcpy(Image.data, pdata, W * H);
	}
	else if (channels[0].I() == 3)
	{
		void* Rptr;
		void* Gptr;
		void* Bptr;
		HImage hImg(imgHalcon);
		hImg.GetImagePointer3(&Rptr, &Gptr, &Bptr, &cType, &width, &height);
		int W = width;
		int H = height;
		Image.create(H, W, CV_32FC3);
		vector<cv::Mat> VecM(3);
		VecM[0].create(H, W, CV_32FC1);
		VecM[1].create(H, W, CV_32FC1);
		VecM[2].create(H, W, CV_32FC1);
		unsigned char* R = (unsigned char*)Rptr;
		unsigned char* G = (unsigned char*)Gptr;
		unsigned char* B = (unsigned char*)Bptr;
		memcpy(VecM[2].data, R, W * H);
		memcpy(VecM[1].data, G, W * H);
		memcpy(VecM[0].data, B, W * H);
		cv::merge(VecM, Image);
	}
	return Image;
}


checkPatch::checkPatch()
{
	m_cfgAdd = getExePath() + "Cali\\";
	mkdir(m_cfgAdd.c_str());
}

checkPatch::~checkPatch()
{
	for (int i = 0; i < m_CaliList.size(); i++)
	{
		delete m_CaliList[i];
	}
}

int checkPatch::ini()
{
	UCI::ELE.EleList = &m_EleList;
	S_dlg.m_pCheck = this;
	S_dlg.m_hasIni = false;

	readMode(m_cfgAdd);

#ifdef HCAM
	vector<HImage> vm;
	if (-1 == grapImage(0, vm))
	{
		AfxMessageBox("获取图像失败");
		return 0;
	}
	UCI::pDis->m_sIPResult.result_img = UCI::pDis->GrabImageDisp = vm[0];
	/*UCI::OPTIONS["图像宽度"] = vm[0].cols;
	UCI::OPTIONS["图像高度"] = vm[0].rows;
	UCI::OPTIONS["图像宽长比"] = (double)vm[0].rows / vm[0].cols;*/

#else
	vector<cv::Mat> vm;
	grapImage(0, vm);
	if (vm[0].empty())
	{
		AfxMessageBox("获取图像失败");
		return 0;
	}
	UCI::pDis->m_sIPResult.result_img = UCI::pDis->GrabImageDisp = Mat2HObject(vm[0]);
	UCI::OPTIONS["图像宽度"] = vm[0].cols;
	UCI::OPTIONS["图像高度"] = vm[0].rows;
	UCI::OPTIONS["图像宽长比"] = (double)vm[0].rows / vm[0].cols;

#endif
	

	INT_PTR nResponse = S_dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	return m_EleList.GetSize();
}
#ifdef HCAM
int checkPatch::grapImage(int id, vector<HImage>& vm)
{
	if (S_dlg.m_pCheck == NULL)
	{
		S_dlg.m_pCheck = this;
		UCI::ELE.EleList = &m_EleList;
		readMode(m_cfgAdd);
	}
	/*if (m_CaliList.size()==0)
	{
		return -1;
	}*/
	if (-1 != HCameraDrv::grapImage(id, vm))
	{
#ifndef _SIMULATION
		CString str;
		str.Format("相机 %d", id + 1);
		UCI::OptionsMap* om = UCI::ELE.GetOptionsMapByEleName(m_EleList, str);
		if (nullptr == om)
		{
			return -1;
		}
		UCI::pDis->GrabImageDisp = vm[0];
		BOOL isLoad = UCI::OPTIONS["标定信息 OK"];
		if (isLoad == FALSE) {
			UCI::pDis->hv_Cali = m_CaliList[id];
			if (UCI::pDis->LoadCaliPoseAndParamter(m_cfgAdd, id + 1) == FALSE) {
			//	MessageBox(NULL, "标定配置错误", _T("Warning"), MB_SYSTEMMODAL | MB_ICONEXCLAMATION | MB_OK);
				return 1;
			}
		}
		try
		{
			//int w = (*om)["图像宽度"];
			//int h = (*om)["图像高度"];
			//if (w != m.cols || h != m.rows)
			//{
			//	AfxMessageBox("标定配置和实际图像大小不同");
			//}
			//else// 校正图像
			{
				MapImage(vm[0], UCI::pDis->hv_Cali->ho_RectMap, &vm[0]);
				/*hImg.WriteImage("bmp", 0,
					"d:\\temp.bmp");
				m = imread("d:\\temp.bmp",0);*/
			}
		}
		catch (...)
		{
			//AfxMessageBox("标定配置和实际图像大小不同");
		}
#endif
		return 1;
	}
	else
		return -1;
}
#else
int checkPatch::grapImage(int id, vector<cv::Mat>& vm)
{
	if (S_dlg.m_pCheck == NULL)
	{
		S_dlg.m_pCheck = this;
		UCI::ELE.EleList = &m_EleList;
		readMode(m_cfgAdd);
	}
	/*if (m_CaliList.size()==0)
	{
		return -1;
	}*/
	if (-1 != CameraDrv::grapImage(id, vm))
	{
		CString str;
		str.Format("相机 %d", id + 1);
		UCI::OptionsMap* om = UCI::ELE.GetOptionsMapByEleName(m_EleList, str);
		if (nullptr == om)
		{
			return -1;
		}
		HalconCpp::HImage hImg = UCI::pDis->GrabImageDisp = Mat2HObject(vm[0]);
		BOOL isLoad = UCI::OPTIONS["标定信息 OK"];
		if (isLoad == FALSE) {
			UCI::pDis->hv_Cali = m_CaliList[id];
			if (UCI::pDis->LoadCaliPoseAndParamter(m_cfgAdd, id + 1) == FALSE) {
			//	MessageBox(NULL, "标定配置错误", _T("Warning"), MB_SYSTEMMODAL | MB_ICONEXCLAMATION | MB_OK);
				return 1;
			}
		}
		try
		{
			//int w = (*om)["图像宽度"];
			//int h = (*om)["图像高度"];
			//if (w != m.cols || h != m.rows)
			//{
			//	AfxMessageBox("标定配置和实际图像大小不同");
			//}
			//else// 校正图像
			{
				MapImage(hImg, UCI::pDis->hv_Cali->ho_RectMap, &hImg);
				/*hImg.WriteImage("bmp", 0,
					"d:\\temp.bmp");
				m = imread("d:\\temp.bmp",0);*/
				vm[0] = HObject2Mat(hImg);
			}
		}
		catch (...)
		{
			//AfxMessageBox("标定配置和实际图像大小不同");
		}

		return 1;
	}
	else
		return -1;
}
#endif

void checkPatch::readMode(string file)
{
	try
	{
		if (!m_EleList.IsEmpty())
			m_EleList.RemoveAll();
		if (m_CaliList.size() > 0)
		{
			for (int i = 0; i < m_CaliList.size(); i++)
			{
				delete m_CaliList[i];
			}
			m_CaliList.clear();
		}
		if (file != "")
		{
			pww_mkdir(file.c_str());
			m_cfgAdd = file;
		}
		S_CheckParam cfg0(m_cfgAdd + "\\checkParam.cfg");
		string szVal = cfg0.getItem("optCount");
		if ("" != szVal)
		{
			int count = atoi(szVal.c_str()) + 1;
			char sz[32];
			for (int i = 1; i < count; i++)
			{
				sprintf(sz, "%d", i);
				S_CheckParam cfg(m_cfgAdd + "\\checkParam" + sz + ".cfg");
				UCI::OptionsMap omp;
				cfg.readcfgfile(omp);
				m_EleList.AddTail(omp);

				S_Cali* cali = new S_Cali();
				UCI::pDis->hv_Cali = cali;
				m_CaliList.push_back(cali);
				if (UCI::pDis->LoadCaliPoseAndParamter(m_cfgAdd, i) == TRUE)
					;
				//UCI::pDis->GetRefMapFromSaveFile(m_cfgAdd,i);
				//UCI::pDis->DisOnButtonGenRectifyMap();
			}
			S_CheckParam cfg2(m_cfgAdd + "\\OPTIONSParam.cfg");
			cfg2.readcfgfile(UCI::OPTIONS);

		}
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}

void checkPatch::writeMode()
{

	delCfgFiles(m_cfgAdd.c_str(), 1);

	char sz[32];
	S_CheckParam cfg0(m_cfgAdd + "\\checkParam.cfg");
	sprintf(sz, "%ld", m_EleList.GetCount());
	cfg0.updateItem("optCount", sz);

	int i = 1;
	POSITION pos = m_EleList.GetHeadPosition();
	while (pos != NULL) {
		UCI::OptionsMap* o = &m_EleList.GetNext(pos);

		sprintf(sz, "%d", i++);
		S_CheckParam cfg(m_cfgAdd + "\\checkParam" + sz + ".cfg");
		cfg.writecfgfile(o);
	}
	string szfile;
	for (i = 0; i < m_CaliList.size(); i++)
	{
		sprintf(sz, "%d\\inner\\", i + 1);
		szfile = m_cfgAdd + (string)sz + "inner.cal";
		remove(szfile.c_str());
		try
		{
			if (m_CaliList[i]->hv_CamParameters.Length()>0)
			{
			}
			else
				continue;
			HalconCpp::WriteCamPar(m_CaliList[i]->hv_CamParameters, szfile.c_str());
			//	sprintf(sz,"%d\\pos\\",i+1);
			szfile = m_cfgAdd + (string)sz + "pos.dat";
			remove(szfile.c_str());
			m_CaliList[i]->hv_CamPose.WritePose(szfile.c_str());
		}
		catch (HalconCpp::HException& except) {
			AfxMessageBox(except.ErrorText().Text());
		}
	}

	S_CheckParam cfg2(m_cfgAdd + "\\OPTIONSParam.cfg");
	cfg2.writecfgfile(&UCI::OPTIONS);
}
