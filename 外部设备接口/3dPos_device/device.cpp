#include "pch.h"
#include "device.h"
#include "find_surface_model.h"
threeDMatch CDevice::S_dlg;
//CThreadMgr g_threadMgr;
void deepToRgb(cv::Mat m, int Low, cv::Mat& rgbm)//深度图转3通道图 
{
	//20cm的测量范围
	int fanwei = 1000;// um
	//int Low = 650;//去背景
	double base = 256.0 / fanwei;//根据范围求基数
//	m.convertTo(m, CV_32FC1, 1000);//放大1000倍转为深度图
	rgbm = cv::Mat(m.rows, m.cols, CV_8UC3);//转换为彩色图  
	double intpart = 0;
	float* pm = (float*)m.data;
	uchar* pm3d = rgbm.data;
	int len = m.total();
	for (int i = 0; i < len; i++)
	{
		int j = 3 * i;
		double val = modf((pm[i] - Low) * base, &intpart);//基数*深度的整数部分是通道1  定位和显示用这个通道
		pm3d[j] = intpart;
		val = modf(val * 256, &intpart);//细分256后 整数部分通道2
		pm3d[j + 1] = intpart;
		pm3d[j + 2] = 0.5 + val * 256;//细分256后 四舍五入 通道3
	}

}

//16bit--OpenCV Mat转Halcon HObject
HObject MatToHImage16Bit(Mat& imgMat)
{
	HObject Hobj = HObject();
	int height = imgMat.rows;
	int width = imgMat.cols;
	int i;
	//  CV_8UC3
	if (imgMat.type() == CV_16UC3)
	{
		std::vector<cv::Mat> imgchannel;
		split(imgMat, imgchannel);
		cv::Mat imgB = imgchannel[0];
		cv::Mat imgG = imgchannel[1];
		cv::Mat imgR = imgchannel[2];
		uchar* dataR = new uchar[2 * height * width];
		uchar* dataG = new uchar[2 * height * width];
		uchar* dataB = new uchar[2 * height * width];
		for (i = 0; i < height; i++)
		{
			memcpy(dataR + width * i * 2, imgR.data + imgR.step * i, width * 2);
			memcpy(dataG + width * i * 2, imgG.data + imgG.step * i, width * 2);
			memcpy(dataB + width * i * 2, imgB.data + imgB.step * i, width * 2);
		}
		GenImage3(&Hobj, "uint2", width, height, (Hlong)dataR, (Hlong)dataG, (Hlong)dataB);
		delete[]dataR;
		delete[]dataG;
		delete[]dataB;
	}
	if (imgMat.type() == CV_16UC1)
	{
		uchar* data = new uchar[2 * height * width];
		for (i = 0; i < height; i++)
			memcpy(data + width * i * 2, imgMat.data + imgMat.step * i, width * 2);
		GenImage1(&Hobj, "uint2", width, height, (Hlong)data);
		delete[] data;
	}
	if (imgMat.type() == CV_32FC3)
	{
		std::vector<cv::Mat> imgchannel;
		split(imgMat, imgchannel);
		cv::Mat imgB = imgchannel[0];
		cv::Mat imgG = imgchannel[1];
		cv::Mat imgR = imgchannel[2];
		uchar* dataR = new uchar[4 * height * width];
		uchar* dataG = new uchar[4 * height * width];
		uchar* dataB = new uchar[4 * height * width];
		for (i = 0; i < height; i++)
		{
			memcpy(dataR + width * i * 4, imgR.data + imgR.step * i, width * 4);
			memcpy(dataG + width * i * 4, imgG.data + imgG.step * i, width * 4);
			memcpy(dataB + width * i * 4, imgB.data + imgB.step * i, width * 4);
		}
		GenImage3(&Hobj, "real", width, height, (Hlong)dataR, (Hlong)dataG, (Hlong)dataB);
		delete[]dataR;
		delete[]dataG;
		delete[]dataB;
	}
	//  CV_8UCU1
	else if (imgMat.type() == CV_32FC1)
	{
		uchar* data = new uchar[4 * height * width];
		for (i = 0; i < height; i++)
			memcpy(data + width * i * 4, imgMat.data + imgMat.step * i, width * 4);
		GenImage1(&Hobj, "real", width, height, (Hlong)data);
		delete[] data;
	}

	return Hobj;
}

//16bit--Halcon HObject转OpenCV Mat
Mat HImageToMat16Bit(HObject& imgHalcon)
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
		Image.create(H, W, CV_16UC1);
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
		Image.create(H, W, CV_16UC3);
		std::vector<cv::Mat> VecM(3);
		VecM[0].create(H, W, CV_16UC1);
		VecM[1].create(H, W, CV_16UC1);
		VecM[2].create(H, W, CV_16UC1);
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
		std::vector<cv::Mat> VecM(3);
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
		std::vector<cv::Mat> vecM(3);
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
void S_CheckParam::readMode(CConfig& cfg)
{
	string szVal = cfg.getItem("m_procName");
	if ("" != szVal)
	{
		m_procName = szVal;
	}
	szVal = cfg.getItem("RelSamplingDistance1");
	if ("" != szVal)
	{
		RelSamplingDistance1 = atof(szVal.c_str());
	}
	szVal = cfg.getItem("RelSamplingDistance2");
	if ("" != szVal)
	{
		RelSamplingDistance2 = atof(szVal.c_str());
	}
	szVal = cfg.getItem("KeyPointFraction");
	if ("" != szVal)
	{
		KeyPointFraction = atof(szVal.c_str());
	}
	szVal = cfg.getItem("MinScore");
	if ("" != szVal)
	{
		MinScore = atof(szVal.c_str());
	}
	szVal = cfg.getItem("GenParamValue");
	if ("" != szVal)
	{
		GenParamValue = atoi(szVal.c_str());
	}
	szVal = cfg.getItem("bgL");
	if ("" != szVal)
	{
		bgL = atof(szVal.c_str());
	}
	szVal = cfg.getItem("bgH");
	if ("" != szVal)
	{
		bgH = atof(szVal.c_str());
	}
	szVal = cfg.getItem("m_winH");
	if ("" != szVal)
	{
		m_winH = atof(szVal.c_str());
	}
	szVal = cfg.getItem("m_winW");
	if ("" != szVal)
	{
		m_winW = atof(szVal.c_str());
	}
	szVal = cfg.getItem("m_pose");
	if ("" != szVal)
	{
		m_pose = atoi(szVal.c_str());
	}
	szVal = cfg.getItem("m_grapsize");
	if ("" != szVal)
	{
		m_grapsize = atof(szVal.c_str());
	}
	iniMode();
}

void S_CheckParam::writeMode(CConfig& cfg)
{
	if (hv_SFM.Length() > 0)
	{
		replace(modelFile.begin(), modelFile.end(), '\\', '/');
		WriteSurfaceModel(hv_SFM, modelFile.c_str());
		WriteObjectModel3d(hv_ObjectModel3DModel, "om3", (modelFile + "s").c_str(), HTuple(), HTuple());
		string str = modelFile + "sharp.hobj";
		remove(str.c_str());
		WriteRegion(ho_ModelRegion, str.c_str());          // 匹配区域文件
		str = modelFile + "grab_Pose.tup";
		WritePose(hv_Pose, str.c_str());
		/*str = modelFile + "caminner.cal";
		WriteCamPar(hv_CamParam,str.c_str());*/
	}
	cfg.updateItem("m_procName", m_procName);
	char sz[32];
	sprintf(sz, "%f", RelSamplingDistance1);
	cfg.updateItem("RelSamplingDistance1", sz);
	sprintf(sz, "%f", RelSamplingDistance2);
	cfg.updateItem("RelSamplingDistance2", sz);
	sprintf(sz, "%f", KeyPointFraction);
	cfg.updateItem("KeyPointFraction", sz);
	sprintf(sz, "%f", MinScore);
	cfg.updateItem("MinScore", sz);
	sprintf(sz, "%d", GenParamValue);
	cfg.updateItem("GenParamValue", sz);
	sprintf(sz, "%f", bgL);
	cfg.updateItem("bgL", sz);
	sprintf(sz, "%f", bgH);
	cfg.updateItem("bgH", sz);
	sprintf(sz, "%f", m_winH);
	cfg.updateItem("m_winH", sz);
	sprintf(sz, "%f", m_winW);
	cfg.updateItem("m_winW", sz);
	sprintf(sz,"%d", m_pose);
	cfg.updateItem("m_pose", sz);
	sprintf(sz, "%f", m_grapsize);
	cfg.updateItem("m_grapsize", sz);
}

void S_CheckParam::iniMode()
{
	try
	{
		//初始化
	//	ClearSurfaceModel(hv_SFM);
		replace(modelFile.begin(), modelFile.end(), '\\', '/');
		ReadSurfaceModel(modelFile.c_str(), &hv_SFM);
		HTuple hv_Status;
		ReadObjectModel3d((modelFile + "s").c_str(), "mm", HTuple(), HTuple(), &hv_ObjectModel3DModel,
			&hv_Status);
		string str = modelFile + "sharp.hobj";
		ho_ModelRegion.GenEmptyRegion();
		ReadRegion(&ho_ModelRegion,str.c_str());          // 读出匹配区域文件
		str = modelFile + "grab_Pose.tup";
		ReadPose( str.c_str(), &hv_Pose);
		/*str = modelFile + "caminner.cal";
		ReadCamPar(str.c_str(), &hv_CamParam);*/
		m_iniSucc = true;
	}
	catch (...)
	{
		m_iniSucc = false;
	}
}

CDevice::CDevice()
{	// Default settings used in HDevelop (can be omitted)
	SetSystem("width", 512);
	SetSystem("height", 512);

#if defined(_WIN32)
	SetSystem("use_window_thread", "true");
#elif defined(__linux__)
	XInitThreads();
#endif
	//	HalconCpp::SetHcppInterfaceStringEncodingIsUtf8(false);
	m_vctDLPa.push_back(S_CheckParam());
	m_curId = 0;
	m_cfgDir = "";
	bIsInROIdrawing = false;
	iCOMBO_MATCH_ROI_TYPE = ROI_UNITE;
	m_hImage.resize(2);

	m_runing = false;
}

CDevice::~CDevice()
{
	clearMode();
}

void CDevice::clearMode()
{
	for (int i = 0; i < m_vctDLPa.size(); i++)
	{
		S_CheckParam& pa = m_vctDLPa[i];
		ClearObjectModel3d(pa.hv_ObjectModel3DModel);
		ClearSurfaceModel(pa.hv_SFM);
	}
}
void CDevice::readMode(string file)
{
	string szdir = file + "3dMatchcfg";
	if (file != "")
	{
		pww_mkdir(szdir.c_str());
		m_cfgDir = szdir;
	}
	CConfig cfg(m_cfgDir + "\\checkParam.cfg");
	string szVal = cfg.getItem("DPProcNum");
	int num = 1;
	if ("" != szVal)
	{
		num = atoi(szVal.c_str());
	}
	szVal = cfg.getItem("m_curId");
	if ("" != szVal)
	{
		m_curId = atoi(szVal.c_str());
	}
	m_vctDLPa.resize(num);
	for (int i = 0; i < num; i++)
	{
		CConfig cfg(m_cfgDir + "\\checkParam" + CStrProcess::itoa_radixN(i) + ".cfg");
		m_vctDLPa[i].modelFile = m_cfgDir + "\\model" + CStrProcess::itoa_radixN(i);
		m_vctDLPa[i].readMode(cfg);
	}
	szdir = file + "eyeinhandcalicfg";
	m_CaliParam.loadcfg(szdir);//加载手眼标定
}

void CDevice::writeMode(string file)
{
	string szdir = file + "3dMatchcfg";
	if (file != "")
	{
		pww_mkdir(szdir.c_str());
		m_cfgDir = szdir;
	}
	delCfgFiles(m_cfgDir.c_str(), 1);

	CConfig cfg(m_cfgDir + "\\checkParam.cfg");
	cfg.updateItem("DPProcNum", CStrProcess::itoa_radixN(m_vctDLPa.size()));
	cfg.updateItem("m_curId", CStrProcess::itoa_radixN(m_curId));

	for (int i = 0; i < m_vctDLPa.size(); i++)
	{
		CConfig cfg(m_cfgDir + "\\checkParam" + CStrProcess::itoa_radixN(i) + ".cfg");
		m_vctDLPa[i].modelFile = m_cfgDir + "\\model" + CStrProcess::itoa_radixN(i);
		m_vctDLPa[i].writeMode(cfg);
	}
}

void CDevice::iniMode()
{
	for (int i = 0; i < m_vctDLPa.size(); i++)
	{
		m_vctDLPa[i].iniMode();
	}
}

bool CDevice::openDevWin(bool ini)
{
	try
	{
		HTuple  hv_RowNotUsed, hv_ColumnNotUsed, hv_Width, hv_Height;
		GetImageSize(m_hImage[1], &hv_Width, &hv_Height);
		float h = hv_Height.I();
		float w = hv_Width.I();
		S_CheckParam& pa = m_vctDLPa[m_curId];
		pa.hv_CamParam.Clear();
		pa.hv_CamParam[0] = 0.01;
		pa.hv_CamParam[1] = 0;
		pa.hv_CamParam[2] = 7e-6;
		pa.hv_CamParam[3] = 7e-6;
		pa.hv_CamParam[4] = w / 2;
		pa.hv_CamParam[5] = h / 2;
		pa.hv_CamParam[6] = w;
		pa.hv_CamParam[7] = h;

		if (ini)
		{
			/*dev_update_off();

			if (HDevWindowStack::IsOpen())
				CloseWindow(HDevWindowStack::Pop());

			dev_open_window_fit_image(m_hImage[1], 0, 0, -1, -1, &hv_WindowHandle);*/
			set_display_font(hv_WindowHandle, 14, "mono", "true", "false");
			if (HDevWindowStack::IsOpen())
				SetLineWidth(HDevWindowStack::GetActive(), 2);
			if (HDevWindowStack::IsOpen())
				SetDraw(HDevWindowStack::GetActive(), "margin");
			if (HDevWindowStack::IsOpen())
				SetColor(HDevWindowStack::GetActive(), "green");
			disp_message(hv_WindowHandle, "Create surface model from XYZ image region", "window",
				12, 12, "black", "true");
			disp_continue_message(hv_WindowHandle, "black", "true");

			set3dTran();
			return true;
		}

		HObject  ho_Xm, ho_Ym, ho_Zm;
		HTuple  hv_pra_name, hv_pra_value, hv_Pose, hv_HomMat3D;
		HTuple  hv_Instructions, hv_Message, hv_ObjectModel3DModel;
		hv_Instructions[0] = "Rotate: Left button";
		hv_Instructions[1] = "Zoom:   Shift + left button";
		hv_Instructions[2] = "Move:   Ctrl  + left button";
		//
		hv_Message = "点continue后画矩形，然后点右键确认";
		hv_Pose.Clear();
		hv_Pose[0] = 0;
		hv_Pose[1] = 0;
		hv_Pose[2] = 0;
		hv_Pose[3] = 0;
		hv_Pose[4] = 0;
		hv_Pose[5] = 0;
		hv_Pose[6] = 0;
		hv_pra_name.Clear();
		hv_pra_name[0] = "alpha";
		hv_pra_name[1] = "disp_pose";
		hv_pra_value.Clear();
		hv_pra_value[0] = 0.1;
		hv_pra_value[1] = "true";

		//Initialization
		Decompose3(m_hImage[1], &ho_Xm, &ho_Ym, &ho_Zm);
		//Create the surface model from the reference view
		XyzToObjectModel3d(ho_Xm, ho_Ym, ho_Zm, &hv_ObjectModel3DModel);
		//RigidTransObjectModel3d(hv_ObjectModel3DModel, hv_Pose, &hv_ObjectModel3DModel);
		/*PoseToHomMat3d(hv_Pose, &hv_HomMat3D);
		AffineTransObjectModel3d(hv_ObjectModel3DModel, hv_HomMat3D, &hv_ObjectModel3DModel);*/
		visualize_object_model_3d(hv_WindowHandle, hv_ObjectModel3DModel, pa.hv_CamParam,
			HTuple(), hv_pra_name, hv_pra_value, hv_Message, HTuple(), hv_Instructions,
			&hv_Pose);

		ReduceObjectModel3dByView(pa.ho_ModelRegion, hv_ObjectModel3DModel, pa.hv_CamParam, hv_Pose, &hv_ObjectModel3DModel);
		SelectPointsObjectModel3d(hv_ObjectModel3DModel, "point_coord_z", pa.bgL, pa.bgH, &pa.hv_ObjectModel3DModel);

		CreateSurfaceModel(pa.hv_ObjectModel3DModel, pa.RelSamplingDistance1, HTuple(), HTuple(), &pa.hv_SFM);
		visualize_object_model_3d(hv_WindowHandle, pa.hv_ObjectModel3DModel, pa.hv_CamParam,
			HTuple(), HTuple(), HTuple(), hv_Message, HTuple(), hv_Instructions, &hv_Pose);
		//DispObjectModel3d(hv_WindowHandle, pa.hv_ObjectModel3DModel, hv_CamParam, HTuple(), hv_pra_name, hv_pra_value);
		GetWindowExtents(hv_WindowHandle, &hv_RowNotUsed, &hv_ColumnNotUsed, &hv_Width, &hv_Height);
		pa.m_winH = h / hv_Height.I();
		pa.m_winW = w / hv_Width.I();

		return true;
	}
	catch (HException& except)
	{
	}
	return false;
}

bool CDevice::EditModel()
{
	S_dlg.m_pCheck = this;
	/*if (!S_dlg.GetSafeHwnd())
	{
		S_dlg.Create(IDD_DIALOG1);
	}
	S_dlg.ShowWindow(1);*/
	INT_PTR nResponse = S_dlg.DoModal();
	if (nResponse == IDOK)
	{
		return true;
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	return false;
}

bool CDevice::LocateCheck(int modeId)
{
	if (m_vctDLPa.size() <= modeId)
		return false;

	S_CheckParam& pa = m_vctDLPa[modeId];
	if (pa.hv_SFM.Length() == 0)
		return false;
	try
	{

		// Local iconic variables
		HObject  ho_X, ho_Y, ho_Z, ho_SceneGood, ho_XReduced;

		// Local control variables
		HTuple  hv_Message, hv_Instructions;

		HTuple  hv_ObjectModel3DSceneReduced;
		HTuple  hv_Pose, hv_Score, hv_SurfaceMatchingResultID;
		HTuple  hv_ObjectModel3DResult,hv_GrabPose1, hv_GrabPose2;
		HTuple  hv_Index2, hv_CPose, hv_ObjectModel3DRigidTrans;
		HTuple  hv_ObjectModel3DScene, hv_SampledScene, hv_KeyPoints;
		HTuple  hv_ScoreString, hv_NumResult, hv_Colors, hv_Colors1, hv_Colors2;
		HTuple  hv_Radii, hv_Indices;
		HTuple  hv_PoseOut;

		Decompose3(m_hImage[1], &ho_X, &ho_Y, &ho_Z);
		Threshold(ho_Z, &ho_SceneGood, pa.bgL, pa.bgH);
		ReduceDomain(ho_X, ho_SceneGood, &ho_XReduced);
		XyzToObjectModel3d(ho_XReduced, ho_Y, ho_Z, &hv_ObjectModel3DSceneReduced);
		//
		//***************************************
		//Match: Find the reference model in the
		//       3D scene
		//***************************************
		FindSurfaceModel(pa.hv_SFM, hv_ObjectModel3DSceneReduced, pa.RelSamplingDistance2, pa.MinScore, pa.KeyPointFraction, "true",
			"num_matches", pa.GenParamValue, &hv_Pose, &hv_Score, &hv_SurfaceMatchingResultID);
		//***************************************
		//Display: Visualize the result(s)
		//***************************************
		//Prepare the visualization of the result(s)
		hv_ObjectModel3DResult = HTuple();
		HTuple  hv_CatchSphere, hv_Left_Box;
		{
			HTuple end_val96 = (hv_Score.TupleLength()) - 1;
			HTuple step_val96 = 1;
			for (hv_Index2 = 0; hv_Index2.Continue(end_val96, step_val96); hv_Index2 += step_val96)
			{
				if (0 != (HTuple(hv_Score[hv_Index2]) < 0.11))
				{
					continue;
				}
				hv_CPose = hv_Pose.TupleSelectRange(hv_Index2 * 7, (hv_Index2 * 7) + 6);
				RigidTransObjectModel3d(pa.hv_ObjectModel3DModel, hv_CPose, &hv_ObjectModel3DRigidTrans);
				hv_ObjectModel3DResult = hv_ObjectModel3DResult.TupleConcat(hv_ObjectModel3DRigidTrans);
				//转换为抓取点
				hv_CPose[2] += pa.bgH;
				PoseCompose(hv_CPose, pa.hv_Pose, &hv_CPose);
				//抓取圆柱
				GenBoxObjectModel3d(hv_CPose, pa.m_grapsize, pa.m_grapsize, 5 * pa.m_grapsize, &hv_Left_Box);
				//抓取点
				GenSphereObjectModel3d(hv_CPose, pa.m_grapsize, &hv_CatchSphere);
			
				hv_GrabPose1 = hv_GrabPose1.TupleConcat(hv_Left_Box);
				hv_GrabPose2 = hv_GrabPose2.TupleConcat(hv_CatchSphere);
			}
			
		}
		//
		XyzToObjectModel3d(ho_X, ho_Y, ho_Z, &hv_ObjectModel3DScene);
		//
		//Visualize matching scene and key points
		hv_Message = "Original scene points (white)";
		hv_Message[1] = "Sampled scene points (cyan)";
		hv_Message[2] = "Key points (yellow)";
		GetSurfaceMatchingResult(hv_SurfaceMatchingResultID, "sampled_scene", HTuple(),
			&hv_SampledScene);
		GetSurfaceMatchingResult(hv_SurfaceMatchingResultID, "key_points", HTuple(),
			&hv_KeyPoints);
		if (HDevWindowStack::IsOpen())
			ClearWindow(HDevWindowStack::GetActive());
		visualize_object_model_3d(hv_WindowHandle, (hv_ObjectModel3DScene.TupleConcat(hv_SampledScene)).TupleConcat(hv_KeyPoints),
			pa.hv_CamParam, HTuple(), HTuple(HTuple("color_") + ((HTuple(0).Append(1)).Append(2))).TupleConcat(HTuple("point_size_") + ((HTuple(0).Append(1)).Append(2))),
			(((((HTuple("gray").Append("cyan")).Append("yellow")).Append(1.0)).Append(3.0)).Append(5.0)),
			hv_Message, HTuple(), hv_Instructions, &hv_PoseOut);
		//
		//Visualize result(s)
		hv_Message = "Scene: ";
		hv_Message[1] = (HTuple(HTuple("Found ") + (hv_ObjectModel3DResult.TupleLength())) + " object(s)");
		hv_ScoreString = ((hv_Score.TupleString(".2f")) + " / ").TupleSum();
		hv_Message[2] = "Score(s): " + (hv_ScoreString.TupleSubstr(0, (hv_ScoreString.TupleStrlen()) - 4));
		hv_NumResult = hv_ObjectModel3DResult.TupleLength();
		TupleGenConst(hv_NumResult, "yellow", &hv_Colors);
		TupleGenConst(hv_NumResult, "green", &hv_Colors1);
		TupleGenConst(hv_NumResult, "blue", &hv_Colors2);
		hv_Colors = hv_Colors.TupleConcat(hv_Colors1).TupleConcat(hv_Colors2);
		hv_NumResult = 3* hv_NumResult;
		hv_Indices = HTuple::TupleGenSequence(1, hv_NumResult, 1);
		//
		if (HDevWindowStack::IsOpen())
			ClearWindow(HDevWindowStack::GetActive());
		visualize_object_model_3d(hv_WindowHandle, hv_ObjectModel3DScene.TupleConcat(hv_ObjectModel3DResult).TupleConcat(hv_GrabPose1).TupleConcat(hv_GrabPose2),
			pa.hv_CamParam, HTuple(), ("color_" + (HTuple(0).TupleConcat(hv_Indices))).TupleConcat("point_size_0").TupleConcat("alpha_0").TupleConcat("disp_pose"),
			(HTuple("gray").TupleConcat(hv_Colors)).TupleConcat(1.0).TupleConcat(0.5).TupleConcat("true"), hv_Message, HTuple(),
			hv_Instructions, &hv_PoseOut);
		//
		//***************************************
		//Clear: Free result(s)
		//***************************************
		ClearObjectModel3d(hv_ObjectModel3DSceneReduced);
		ClearObjectModel3d(hv_ObjectModel3DScene);
		ClearObjectModel3d(hv_ObjectModel3DResult);
		ClearSurfaceMatchingResult(hv_SurfaceMatchingResultID);

		return true;
	}
	catch (HException& except)
	{
		/*ofstream fp;
		fp.open("./err.txt",ios::app);
		fp << except.ErrorText().Text() << endl;
		fp.close();*/
	}
	return false;
}
bool CDevice::checkLocateCheck(S_Msg& msg, int modeId)
{
	if (m_vctDLPa.size() <= modeId)
		return false;

	S_CheckParam& pa = m_vctDLPa[modeId];
	if (pa.hv_SFM.Length() == 0)
		return false;
	try
	{

		// Local iconic variables
		HObject  ho_X, ho_Y, ho_Z, ho_SceneGood, ho_XReduced;

		// Local control variables
		HTuple  hv_ObjectModel3DSceneReduced;
		HTuple  hv_Pose, hv_Score, hv_SurfaceMatchingResultID, hv_ObjectModel3DRigidTrans;
		HTuple  hv_Index2, hv_CPose, hv_ParamValue;

		//
		//***********************************************************************
		//Online-Phase: Match the reference object in 3D scenes
		//***********************************************************************
		//
		Decompose3(m_hImage[1], &ho_X, &ho_Y, &ho_Z);
		//Remove the background plane to increase the speed and
		//robustness of the surface-based matching
		Threshold(ho_Z, &ho_SceneGood, pa.bgL, pa.bgH);
		ReduceDomain(ho_X, ho_SceneGood, &ho_XReduced);
		XyzToObjectModel3d(ho_XReduced, ho_Y, ho_Z, &hv_ObjectModel3DSceneReduced);
		//
		//***************************************
		//Match: Find the reference model in the
		//       3D scene
		//***************************************
		FindSurfaceModel(pa.hv_SFM, hv_ObjectModel3DSceneReduced, pa.RelSamplingDistance2, pa.MinScore, pa.KeyPointFraction, "true",
			"num_matches", pa.GenParamValue, &hv_Pose, &hv_Score, &hv_SurfaceMatchingResultID);

		//
		//***************************************
		//Clear: Free result(s)
		//***************************************
		ClearObjectModel3d(hv_ObjectModel3DSceneReduced);
		ClearSurfaceMatchingResult(hv_SurfaceMatchingResultID);

		msg.vm[0] = HObject2Mat(m_hImage[0]);
		if (1 == msg.vm[0].channels())
		{
			cvtColor(msg.vm[0], msg.vm[0], CV_GRAY2RGB);
		}
		int size = 0.5 + msg.vm[0].cols > msg.vm[0].rows ? msg.vm[0].rows / g_lineRate : msg.vm[0].cols / g_lineRate;
		int thickness = 1 + size;
		size = (thickness << 3);

		HTuple end_val96 = (hv_Score.TupleLength()) - 1;
		HTuple step_val96 = 1;
		for (hv_Index2 = 0; hv_Index2.Continue(end_val96, step_val96); hv_Index2 += step_val96)
		{
			if (0 != (HTuple(hv_Score[hv_Index2]) < 0.11))
			{
				continue;
			}
			hv_CPose = hv_Pose.TupleSelectRange(hv_Index2 * 7, (hv_Index2 * 7) + 6);
			RigidTransObjectModel3d(pa.hv_ObjectModel3DModel, hv_CPose, &hv_ObjectModel3DRigidTrans);
			/*HTuple hv_HomMat3D;
			PoseToHomMat3d(hv_CPose, &hv_HomMat3D);
			AffineTransObjectModel3d(pa.hv_ObjectModel3DModel, hv_HomMat3D, &hv_ObjectModel3DRigidTrans);*/
			GetObjectModel3dParams(hv_ObjectModel3DRigidTrans, "center", &hv_ParamValue);
			//转换为抓取点
			hv_CPose[2] += pa.bgH;
			PoseCompose(hv_CPose, pa.hv_Pose, &hv_CPose);
			//转换为机器人的pose
			ConvertPoseType(hv_CPose, "Rp+T", "abg", "point", &hv_CPose);
			char sz[256];
			sprintf(sz, "score:%.3f,pose:%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%d", hv_Score[hv_Index2].D(), hv_CPose[0].D(),
				hv_CPose[1].D(), hv_CPose[2].D(), hv_CPose[3].D(), hv_CPose[4].D(), hv_CPose[5].D(), hv_CPose[6].I());
			msg.str += sz;
			msg.x = hv_CPose[0].D();//x平移
			msg.y = hv_CPose[1].D();//y平移
			msg.z = hv_CPose[2].D();//z平移
			msg.xr = hv_CPose[3].D();//x旋转
			msg.yr = hv_CPose[4].D();//y旋转
			msg.zr = hv_CPose[5].D();//z旋转
			//转换到平面图片上显示
			cv::Point pt(msg.vm[0].cols / 2 + hv_ParamValue[0].D() * pa.m_winW, msg.vm[0].rows / 2 + hv_ParamValue[1].D() * pa.m_winH);
			cv::line(msg.vm[0], cv::Point(pt.x - size, pt.y), cv::Point(pt.x + size, pt.y), cv::Scalar(0, 255, 0), thickness, 8, 0);
			//绘制竖线
			cv::line(msg.vm[0], cv::Point(pt.x, pt.y - size), cv::Point(pt.x, pt.y + size), cv::Scalar(0, 255, 0), thickness, 8, 0);
			//仿射变换为机器人的xy坐标
			m_CaliParam.getDistance(&msg.x, &msg.y, &pt.x, &pt.y);
		}


		/*msg.m = HImageToMat32Bit(m_hImage);
		double minv = 0.0, maxv = 0.0;
		double* minp = &minv;
		double* maxp = &maxv;
		minMaxIdx(msg.m, minp, maxp);
		msg.m.convertTo(msg.m, CV_8U, 255.0 /(maxv - minv));*/

		//	deepToRgb(HImageToMat32Bit(m_hImage),pa.bgH, msg.m);

		return true;
	}
	catch (HException& except)
	{
		/*ofstream fp;
		fp.open("./err.txt",ios::app);
		fp << except.ErrorText().Text() << endl;
		fp.close();*/
	}
	return false;
}
void CDevice::set3dTran(bool bsel)
{
	HObject  ho_Xm, ho_Ym, ho_Zm;
	HTuple  hv_pra_name, hv_pra_value,  hv_Pose, hv_CatchSphere;
	HTuple  hv_Instructions, hv_Message, hv_ObjectModel3DModel;
	hv_Instructions[0] = "Rotate: Left button";
	hv_Instructions[1] = "Zoom:   Shift + left button";
	hv_Instructions[2] = "Move:   Ctrl  + left button";
	//
	hv_Message = "点continue后画矩形，然后点右键确认";
	S_CheckParam& pa = m_vctDLPa[m_curId];
	hv_Pose.Clear();
	hv_Pose[0] = 0;
	hv_Pose[1] = 0;
	hv_Pose[2] = -pa.bgH;
	hv_Pose[3] = 0;
	hv_Pose[4] = 0;
	hv_Pose[5] = 0;
	hv_Pose[6] = 0;
	hv_pra_name.Clear();
	hv_pra_name[0] = "color_0";
	hv_pra_name[1] = "color_1";
	hv_pra_name[2] = "color_2";
	hv_pra_name[3] = "color_3";
	hv_pra_name[4] = "alpha_0";
	hv_pra_name[5] = "disp_pose";
	hv_pra_value.Clear();
	hv_pra_value[0] = "yellow";
	hv_pra_value[1] = "gray";
	hv_pra_value[2] = "green";
	hv_pra_value[3] = "blue";
	hv_pra_value[4] = 0.1;
	hv_pra_value[5] = "true";

	Decompose3(m_hImage[1], &ho_Xm, &ho_Ym, &ho_Zm);
	XyzToObjectModel3d(ho_Xm, ho_Ym, ho_Zm, &hv_ObjectModel3DModel);
	RigidTransObjectModel3d(hv_ObjectModel3DModel, hv_Pose, &hv_ObjectModel3DModel);
	/*HTuple hv_HomMat3D;
	PoseToHomMat3d(hv_Pose, &hv_HomMat3D);
	AffineTransObjectModel3d(hv_ObjectModel3DModel, hv_HomMat3D, &hv_ObjectModel3DModel);*/

	HTuple hv_Length1, hv_Length2, hv_Length3, hv_ObjectBox, hv_disobjects, hv_Left_Box;
	SmallestBoundingBoxObjectModel3d(hv_ObjectModel3DModel, "axis_aligned", &hv_Pose, &hv_Length1,
		&hv_Length2, &hv_Length3);
	GenBoxObjectModel3d(hv_Pose, hv_Length1, hv_Length2, hv_Length3, &hv_ObjectBox);

	PoseCompose(hv_Pose, pa.hv_Pose, &hv_Pose);
	//抓取圆柱
	GenBoxObjectModel3d(hv_Pose, pa.m_grapsize, pa.m_grapsize, 5* pa.m_grapsize, &hv_Left_Box);
	//抓取点
	GenSphereObjectModel3d(hv_Pose, pa.m_grapsize, &hv_CatchSphere);
	hv_disobjects.Clear();
	hv_disobjects.Append(hv_ObjectBox);
	hv_disobjects.Append(hv_ObjectModel3DModel);
	hv_disobjects.Append(hv_Left_Box);
	hv_disobjects.Append(hv_CatchSphere);
	visualize_object_model_3d(hv_WindowHandle, hv_disobjects, pa.hv_CamParam,
		HTuple(), hv_pra_name, hv_pra_value, hv_Message, HTuple(), hv_Instructions,
		&hv_Pose, bsel);
	//DispObjectModel3d(hv_WindowHandle, hv_ObjectModel3DModel, pa.hv_CamParam, HTuple(), hv_pra_name, hv_pra_value);
	ClearObjectModel3d(hv_disobjects);
	DisProcessROImodel();
}

void CDevice::setPoseRota(int p)
{//正面;反面;后侧面;前侧面;左面;右面
	S_CheckParam& pa = m_vctDLPa[m_curId];
	switch (p)
	{		
		//正面
	case 0:
		pa.hv_Pose[3] = 0;
		pa.hv_Pose[4] = 0;
		pa.hv_Pose[5] = 0;
		break;
		//反面
	case 1:
		pa.hv_Pose[3] = 0;
		pa.hv_Pose[4] = 180;
		pa.hv_Pose[5] = 0;
		break;
	case 2://后侧面
		pa.hv_Pose[3] = -90;
		pa.hv_Pose[4] = 0;
		pa.hv_Pose[5] = 0;
		break;
	case 3://前侧面
		pa.hv_Pose[3] = 90;
		pa.hv_Pose[4] = 0;
		pa.hv_Pose[5] = 0;
		break;
	case 4://左面
		pa.hv_Pose[3] = 0;
		pa.hv_Pose[4] = 90;
		pa.hv_Pose[5] = 0;
		break;
	case 5: // 右侧面
		pa.hv_Pose[3] = 0;
		pa.hv_Pose[4] = -90;
		pa.hv_Pose[5] = 0;
		break;
	default:
		break;
	}
}

void CDevice::setPoseX(float val)
{
	S_CheckParam& pa = m_vctDLPa[m_curId];
	pa.hv_Pose[0] = val;
}

void CDevice::setPoseY(float val)
{
	S_CheckParam& pa = m_vctDLPa[m_curId];
	pa.hv_Pose[1] = val;
}

void CDevice::setPoseZ(float val)
{
	S_CheckParam& pa = m_vctDLPa[m_curId];
	pa.hv_Pose[2] = val;
}

void CDevice::DisProcessROIRegion(HRegion& HXdis, HRegion& HXsrc)
{
	switch (iCOMBO_MATCH_ROI_TYPE)
	{
	case ROI_UNITE:
		HXdis = HXdis.Union2(HXsrc);
		break;
	case ROI_AND:
		HXdis = HXdis.Intersection(HXsrc);
		break;
	case ROI_SUB:
		HXdis = HXdis.Difference(HXsrc);
		break;
	case ROI_XOR:
		HXdis = HXdis.SymmDifference(HXsrc);
		break;
	default:
		break;
	}

}

void CDevice::DisProcessROImodel()
{
	S_CheckParam& pa = m_vctDLPa[m_curId];

	HXLDCont Contours;
	Contours.GenEmptyObj();
	Contours = pa.ho_ModelRegion;
//	DispObj(m_hImage, hv_WindowHandle);
	DispObj(Contours, hv_WindowHandle);
}

// ROI Rectangle2
void CDevice::DisOnRoiDrawRectangle2()
{
	if (bIsInROIdrawing) {
		AfxMessageBox("当前已在绘ROI中。。。");
		return;
	}
	try {
		set3dTran();

		HTuple Row, Column, Phi, Length1, Length2;
		S_CheckParam& pa = m_vctDLPa[m_curId];
		HalconCpp::HRegion Hreg;                     // 区域
				// 提示信息
		disp_message(hv_WindowHandle, "画模板区域,点击鼠标右键确认", "window",
			12, 12, "black", "true");
		bIsInROIdrawing = true;
		// 画区域
		DrawRectangle2(hv_WindowHandle, &Row, &Column, &Phi, &Length1, &Length2);

		bIsInROIdrawing = false;

		GenRectangle2(&Hreg,Row, Column, Phi, Length1, Length2);

		// 产生区域
		DisProcessROIRegion(pa.ho_ModelRegion, Hreg);

		DisProcessROImodel();

	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}

void CDevice::DisOnRoiDrawCircle()
{
	if (bIsInROIdrawing) {
		AfxMessageBox("当前已在绘ROI中。。。");
		return;
	}
	try {
		set3dTran();

		HTuple Row, Column, Radius;
		S_CheckParam& pa = m_vctDLPa[m_curId];
		HRegion Hreg;                     // 区域
		disp_message(hv_WindowHandle, "画模板区域,点击鼠标右键确认", "window",
			12, 12, "black", "true");
		bIsInROIdrawing = true;
		// 画区域
		DrawCircle(hv_WindowHandle, &Row, &Column, &Radius);
		bIsInROIdrawing = false;

		// 产生区域
		GenCircle(&Hreg,Row, Column, Radius);

		DisProcessROIRegion(pa.ho_ModelRegion, Hreg);

		DisProcessROImodel();   // 查找模型
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}

void CDevice::DisOnRoiDrawEllipse()
{
	if (bIsInROIdrawing) {
		AfxMessageBox("当前已在绘ROI中。。。");
		return;
	}
	try {
		set3dTran();

		HTuple Row, Column, Phi, Radius1, Radius2;
		S_CheckParam& pa = m_vctDLPa[m_curId];
		HRegion Hreg;                     // 区域
		disp_message(hv_WindowHandle, "画模板区域,点击鼠标右键确认", "window",
			12, 12, "black", "true");

		bIsInROIdrawing = true;
		// 画区域
		DrawEllipse(hv_WindowHandle, &Row, &Column, &Phi, &Radius1, &Radius2);
		bIsInROIdrawing = false;

		// 产生区域
		GenEllipse(&Hreg,Row, Column, Phi, Radius1, Radius2);

		// 产生区域
		DisProcessROIRegion(pa.ho_ModelRegion, Hreg);

		DisProcessROImodel();   // 查找模型
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}

void CDevice::DisOnRoiDrawRectangle()
{
	if (bIsInROIdrawing) {
		AfxMessageBox("当前已在绘ROI中。。。");
		return;
	}
	try {
		set3dTran();

		HTuple Row1, Column1, Row2, Column2;
		S_CheckParam& pa = m_vctDLPa[m_curId];
		HRegion Hreg;                     // 区域
		disp_message(hv_WindowHandle, "画模板区域,点击鼠标右键确认", "window",
			12, 12, "black", "true");

		bIsInROIdrawing = true;
		// 画区域
		DrawRectangle1(hv_WindowHandle, &Row1, &Column1, &Row2, &Column2);
		bIsInROIdrawing = false;

		// 产生区域
		GenRectangle1(&Hreg,Row1, Column1, Row2, Column2);

		// 与原来的区域进行运算
		DisProcessROIRegion(pa.ho_ModelRegion, Hreg);

		//this->m_pMHWinMain->ClearWindow();	   
		//this->m_pMHWinMain->DispObj(hImg);
		//this->m_pMHWinMain->DispObj(this->ho_ModelRegion);

		DisProcessROImodel();   // 查找模型

	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}

void CDevice::DisOnRoiDrawAnyLine()
{
	if (bIsInROIdrawing) {
		AfxMessageBox("当前已在绘ROI中。。。");
		return;
	}
	try {
		set3dTran();

		disp_message(hv_WindowHandle, "画模板区域,点击鼠标右键确认", "window",
			12, 12, "black", "true");
		HRegion Hreg;                     // 区域
		bIsInROIdrawing = true;
		DrawRegion(&Hreg, hv_WindowHandle);
		bIsInROIdrawing = false;
		S_CheckParam& pa = m_vctDLPa[m_curId];
		DisProcessROIRegion(pa.ho_ModelRegion, Hreg);

		DisProcessROImodel();   // 查找模型

	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}

void CDevice::DisOnMatchDelAllRoi()
{
	S_CheckParam& pa = m_vctDLPa[m_curId];
	pa.ho_ModelRegion.GenEmptyRegion();
	try
	{
		set3dTran();
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
	//DispObj(m_hImage, hv_WindowHandle);
}

void CDevice::DisOnElementDelOne()
{
	try
	{
		if (bIsInROIdrawing) {
			AfxMessageBox("当前已在绘ROI中。。。");
			return;
		}
		S_CheckParam& pa = m_vctDLPa[m_curId];
		HalconCpp::HXLDCont hv_cont, hv_cont2, hv_cont3;
		hv_cont.GenEmptyObj();
		hv_cont2.GenEmptyObj();
		hv_cont3.GenEmptyObj();
		hv_cont = pa.ho_ModelRegion;
		DispObj(hv_cont, hv_WindowHandle);
		// 提示信息
		disp_message(hv_WindowHandle, "点模板区域,点击鼠标右键确认", "window",
			12, 12, "black", "true");
		HTuple Row, Column;
		bIsInROIdrawing = true;
		// 画区域
		DrawPoint(hv_WindowHandle, &Row, &Column);
		bIsInROIdrawing = false;

		/*m_pMHWinMain->ClearWindow();
		m_pMHWinMain->DispObj(hImg);*/

		hv_cont2 = hv_cont.SelectXldPoint(Row, Column);
		int n = hv_cont.CountObj();
		for (int i = 0; i < n; i++)
		{
			hv_cont2 = hv_cont[i + 1].SelectXldPoint(Row, Column);
			if (hv_cont2.CountObj() == 0)
				hv_cont3 = hv_cont3.ConcatObj(hv_cont[i + 1]);
		}
		//m_pMHWinMain->DispObj(hv_cont3);
		pa.ho_ModelRegion = hv_cont3;
		set3dTran();
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}
