#include "pch.h"
#include "device.h"
#include "3d_gripping_point_detection_workflow.h"
GripingDlg CDevice::S_dlg;
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
	string szVal = cfg.getItem("image_width");
	if ("" != szVal)
	{
		image_width = atoi(szVal.c_str());
	}
	szVal = cfg.getItem("image_height");
	if ("" != szVal)
	{
		image_height = atoi(szVal.c_str());
	}
	szVal = cfg.getItem("min_area_size");
	if ("" != szVal)
	{
		min_area_size = atoi(szVal.c_str());
	}
	szVal = cfg.getItem("hv_SortResultsTiltedCamera");
	if ("" != szVal)
	{
		hv_SortResultsTiltedCamera = atoi(szVal.c_str());
	}
	
	iniMode();
}

void S_CheckParam::writeMode(CConfig& cfg)
{
	char sz[32];
	sprintf(sz, "%d", image_width);
	cfg.updateItem("image_width", sz);
	sprintf(sz, "%d", image_height);
	cfg.updateItem("image_height", sz);
	sprintf(sz, "%d", min_area_size);
	cfg.updateItem("min_area_size", sz);
	sprintf(sz, "%d", hv_SortResultsTiltedCamera);
	cfg.updateItem("hv_SortResultsTiltedCamera", sz);
	
}

void S_CheckParam::iniMode()
{
	try
	{
		HTuple  hv_DeviceRuntime, hv___Tmp_Ctrl_0;
		HObject  ho_Z, ho_X, ho_Y;
		//初始化
		hv_DeviceRuntime.Clear();
		hv_DeviceRuntime[0] = "gpu";
		hv_DeviceRuntime[1] = "cpu";
		string file = getExePath() + "anomalies/pretrained_dl_3d_gripping_point.hdl";
		replace(file.begin(), file.end(), '\\', '/');//halcon要求/
		ReadDlModel(file.c_str(), &hv_DLModelHandle);
		//
		//Optionally the input image size of the model can be
		//changed here.
		//Please refer to the chapter reference for details.
		SetDlModelParam(hv_DLModelHandle, "image_width", image_width);
		SetDlModelParam(hv_DLModelHandle, "image_height", image_height);
		//
		//Determine deep learning device to work with
		//(prefer GPU over CPU).
		set_suitable_inference_device(hv_DLModelHandle, hv_DeviceRuntime);
		//
		create_dl_preprocess_param_from_model(hv_DLModelHandle, "none", "full_domain",
			HTuple(), HTuple(), HTuple(), &hv_DLPreprocessParam);
		//
		CreateDict(&hv_WindowDict);
		CreateDict(&hv_DLDatasetInfo);
		//See documentation of dev_display_dl_data for visualization
		//parameter options.
		CreateDict(&hv_DisplayParams);
		SetDictTuple(hv_DisplayParams, "gripping_point_size", 25);
		CreateDict(&hv_DisplayParams3D);
		SetDictTuple(hv_DisplayParams3D, "arrow_thickness", 0.003);
		SetDictTuple(hv_DisplayParams3D, "arrow_length", 0.05);
		//See documentation of gen_dl_3d_gripping_points_and_poses for
		//parameter options.
		CreateDict(&hv_DLGrippingPointParams);
		SetDictTuple(hv_DLGrippingPointParams, "min_area_size", min_area_size);
		if (0 != hv_SortResultsTiltedCamera)
		{
			Decompose3(ho_XYZImage, &ho_X, &ho_Y, &ho_Z);
			estimate_dl_3d_sorting_direction(ho_X, ho_Y, ho_Z, &hv___Tmp_Ctrl_0);
			SetDictTuple(hv_DLGrippingPointParams, "sorting_direction", hv___Tmp_Ctrl_0);
			//if (0 != hv_Visualization3D)
			{
				visualize_dl_3d_sorting_direction(ho_X, ho_Y, ho_Z, ho_GrayImage, hv_DLGrippingPointParams.TupleGetDictTuple("sorting_direction"));
			}
		}

		m_iniSucc = true;
	}
	catch (...)
	{
		m_iniSucc = false;
	}
}

CDevice::CDevice()
{
#if defined(_WIN32)
	SetSystem("use_window_thread", "true");
#endif

	// file was stored with local-8-bit encoding
	//   -> set the interface encoding accordingly
	SetHcppInterfaceStringEncodingIsUtf8(false);

	// Default settings used in HDevelop (can be omitted)
	SetSystem("width", 512);
	SetSystem("height", 512);
	m_cfgDir = "";
	m_hImage.resize(2);
	m_runing = false;
}

CDevice::~CDevice()
{
	clearMode();
}

void CDevice::clearMode()
{
	
}
void CDevice::readMode(string file)
{
	string szdir = file + "3dGrippingcfg";
	if (file != "")
	{
		pww_mkdir(szdir.c_str());
		m_cfgDir = szdir;
	}
	CConfig cfg(m_cfgDir + "\\checkParam.cfg");
	m_DLPa.readMode(cfg);
	
	szdir = file + "eyeinhandcalicfg";
	m_CaliParam.loadcfg(szdir);//加载手眼标定
}

void CDevice::writeMode(string file)
{
	string szdir = file + "3dGrippingcfg";
	if (file != "")
	{
		pww_mkdir(szdir.c_str());
		m_cfgDir = szdir;
	}
	delCfgFiles(m_cfgDir.c_str(), 1);

	CConfig cfg(m_cfgDir + "\\checkParam.cfg");
	m_DLPa.writeMode(cfg);
}

void CDevice::iniMode()
{
	m_DLPa.iniMode();
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
void display_segmentation_regions(HObject ho_SegmentationImage, HTuple hv_ClassIDs,
	HTuple hv_ColorsSegmentation, HTuple hv_ExcludeClassIDs, HTuple* hv_ImageClassIDs, HTuple* hv_WindowHandle)
{

	// Local iconic variables
	HObject  ho_Regions, ho_SelectedRegion;

	// Local control variables
	HTuple  hv_IncludedClassIDs, hv_Area, hv_Index;
	HTuple  hv_ClassID, hv_IndexColor;

	//
	//This procedure displays the ground truth/result segmentation
	//given in SegmentationImage as regions. The ClassIDs are necessary to
	//display ground truth/result segmentations from the same class
	//always with the same color. It is possible to exclude certain ClassIDs
	//from being displayed. The displayed classes are returned in ImageClassIDs.
	//
	//
	//Remove excluded class IDs from the list.
	hv_IncludedClassIDs = hv_ClassIDs.TupleDifference(hv_ExcludeClassIDs);
	//
	//Get a region for each class ID.
	Threshold(ho_SegmentationImage, &ho_Regions, hv_IncludedClassIDs, hv_IncludedClassIDs);
	//
	//Get classes with non-empty regions.
	RegionFeatures(ho_Regions, "area", &hv_Area);
	if (0 != (int((hv_Area.TupleLength()) != (hv_IncludedClassIDs.TupleLength()))))
	{
		throw HException("No equal number of class IDs and segmentation regions.");
	}
	TupleSelectMask(hv_IncludedClassIDs, hv_Area.TupleGreaterElem(0), &(*hv_ImageClassIDs));
	//
	//Display all non-empty class regions in distinct colors.
	{
		HTuple end_val22 = (hv_IncludedClassIDs.TupleLength()) - 1;
		HTuple step_val22 = 1;
		for (hv_Index = 0; hv_Index.Continue(end_val22, step_val22); hv_Index += step_val22)
		{
			if (0 != (int(HTuple(hv_Area[hv_Index]) > 0)))
			{
				//Use class ID to determine region color.
				hv_ClassID = HTuple(hv_IncludedClassIDs[hv_Index]);
				hv_IndexColor = hv_ClassIDs.TupleFindFirst(hv_ClassID);
				if (0 != hv_WindowHandle)
					SetColor(*hv_WindowHandle, HTuple(hv_ColorsSegmentation[hv_IndexColor]));
				//Display the segmentation region.
				SelectObj(ho_Regions, &ho_SelectedRegion, hv_Index + 1);
				if (0 != hv_WindowHandle)
					DispObj(ho_SelectedRegion, *hv_WindowHandle);
			}
		}
	}
	return;
}

bool CDevice::get_dl_data(HTuple hv_DLSample, HTuple hv_DLResult, HTuple hv_DLDatasetInfo, HTuple* hv_WindowHandle)
{
	// Local iconic variables
	HObject  ho_Image, ho_Domain, ho_GrippingMapImageResult;
	HObject  ho_Cross, ho_GrippingMapGroundTruth, ho_X, ho_Y;
	HObject  ho_Z, ho_AnomalyImage, ho_AnomalyRegion, ho_ConfidenceImage;


	// Local control variables
	HTuple  hv_Params, hv_GenParamNames, hv_ParamIndex;
	HTuple  hv_GenParamName, hv_KeyExists, hv_SampleKeys, hv_ResultKeys;
	HTuple  hv_ImageIDExists, hv_ImageID, hv_ImageIDString, hv_ImageClassIDs;
	HTuple  hv_ColorsGrippingGroundTruth,hv_GrippingPointsExists, hv_Rows, hv_Columns;


	//
	//This procedure displays the content of the provided DLSample and/or DLResult
	//depending on the input string KeysForDisplay.
	//DLDatasetInfo is a dictionary containing the information about the dataset.
	//The visualization can be adapted with GenParam.
	//
	//** Set the default values: ***
	CreateDict(&hv_Params);
	//
	//Define the screen width when a new window row is started.
	SetDictTuple(hv_Params, "threshold_width", 1024);
	//Since potentially a lot of windows are opened,
	//scale the windows consistently.
	SetDictTuple(hv_Params, "scale_windows", 0.8);
	//Set a font and a font size.
	SetDictTuple(hv_Params, "font", "mono");
	SetDictTuple(hv_Params, "font_size", 14);
	//
	SetDictTuple(hv_Params, "line_width", 2);
	SetDictTuple(hv_Params, "map_transparency", "cc");
	SetDictTuple(hv_Params, "map_color_bar_width", 140);
	//
	//Define parameter values specifically for 3d_gripping_point_detection
	SetDictTuple(hv_Params, "gripping_point_color", "#00FF0099");
	SetDictTuple(hv_Params, "gripping_point_size", 6);
	SetDictTuple(hv_Params, "region_color", "#FF000040");
	SetDictTuple(hv_Params, "gripping_point_map_color", "#83000080");
	SetDictTuple(hv_Params, "gripping_point_background_color", "#00007F80");
	//
	//Define parameter values specifically for anomaly detection
	//and Global Context Anomaly Detection.
	SetDictTuple(hv_Params, "anomaly_region_threshold", -1);
	SetDictTuple(hv_Params, "anomaly_classification_threshold", -1);
	SetDictTuple(hv_Params, "anomaly_region_label_color", "#40e0d0");
	SetDictTuple(hv_Params, "anomaly_color_transparency", "40");
	SetDictTuple(hv_Params, "anomaly_region_result_color", "#ff0000c0");
	//
	//Define segmentation-specific parameter values.
	SetDictTuple(hv_Params, "segmentation_max_weight", 0);
	SetDictTuple(hv_Params, "segmentation_draw", "fill");
	SetDictTuple(hv_Params, "segmentation_transparency", "aa");
	SetDictTuple(hv_Params, "segmentation_exclude_class_ids", HTuple());
	//
	//Define bounding box-specific parameter values.
	SetDictTuple(hv_Params, "bbox_label_color", HTuple("#000000") + "99");
	SetDictTuple(hv_Params, "bbox_display_confidence", 1);
	SetDictTuple(hv_Params, "bbox_text_color", "#eeeeee");
	//
	//By default, display a description on the bottom.
	SetDictTuple(hv_Params, "display_bottom_desc", 1);
	//
	//By default, show a legend with class IDs.
	SetDictTuple(hv_Params, "display_legend", 1);
	//
	//By default, show the anomaly ground truth regions.
	SetDictTuple(hv_Params, "display_ground_truth_anomaly_regions", 1);
	//
	//By default, show class IDs and color frames for classification ground truth/results.
	SetDictTuple(hv_Params, "display_classification_ids", 1);
	SetDictTuple(hv_Params, "display_classification_color_frame", 1);
	//
	//By default, show class labels for detection ground truth/results.
	SetDictTuple(hv_Params, "display_labels", 1);
	//
	//By default, show direction of the ground truth/results instances for detection with instance_type 'rectangle2'.
	SetDictTuple(hv_Params, "display_direction", 1);
	//
	//By default, use color scheme 'Jet' for the heatmap display.
	SetDictTuple(hv_Params, "heatmap_color_scheme", "jet");
	//
	//Get the dictionary keys.
	if (0 != (int(hv_DLResult != HTuple())))
	{
		GetDictParam(hv_DLResult, "keys", HTuple(), &hv_ResultKeys);
	}

	//Check if DLDatasetInfo is valid.
	//
	
	
		//Image.
	get_dl_sample_image(&ho_Image, hv_SampleKeys, hv_DLSample, "image");
	//
	//Gripping map image result.
	if (0 != (int(hv_DLResult == HTuple())))
	{
		throw HException("DLResult dict is empty.");
	}
	if (0 != (int((hv_ResultKeys.TupleFind("gripping_map")) != -1)))
	{
		GetDictObject(&ho_GrippingMapImageResult, hv_DLResult, "gripping_map");
	}
	else
	{
		throw HException(HTuple(HTuple("Image with key '") + "gripping_map") + "' could not be found in DLResult.");
	}
	//
	//Visualization.
	SetWindowParam(*hv_WindowHandle, "background_color", "#000000");
	GetDomain(ho_GrippingMapImageResult, &ho_Domain);
	ReduceDomain(ho_Image, ho_Domain, &ho_Image);
	//
	if (0 != hv_WindowHandle)
		DispObj(ho_Image, *hv_WindowHandle);

	//Display gripping regions.
	hv_ColorsGrippingGroundTruth.Clear();
	hv_ColorsGrippingGroundTruth.Append(hv_Params.TupleGetDictTuple("gripping_point_map_color"));
	hv_ColorsGrippingGroundTruth.Append(hv_Params.TupleGetDictTuple("gripping_point_background_color"));
	display_segmentation_regions(ho_GrippingMapImageResult, (HTuple(1).Append(0)),
		hv_ColorsGrippingGroundTruth, HTuple(), &hv_ImageClassIDs, hv_WindowHandle);
	//Display gripping points.
	GetDictParam(hv_DLResult, "key_exists", "gripping_points", &hv_GrippingPointsExists);
	if (0 != hv_GrippingPointsExists)
	{
		get_gripping_points_from_dict(hv_DLResult, &hv_Rows, &hv_Columns);
		GenCrossContourXld(&ho_Cross, hv_Rows, hv_Columns, hv_Params.TupleGetDictTuple("gripping_point_size"),
			0.785398);
		if (0 != hv_WindowHandle)
		{
			SetLineWidth(*hv_WindowHandle, hv_Params.TupleGetDictTuple("line_width"));
			SetColor(*hv_WindowHandle, hv_Params.TupleGetDictTuple("gripping_point_color"));
			DispObj(ho_Cross, *hv_WindowHandle);
		}
	}

//Display results.
	if (0 != hv_WindowHandle)
	{
		FlushBuffer(*hv_WindowHandle);
		//	SetWindowParam(*hv_WindowHandle, "flush", "false");
	}
	return true;
}

bool CDevice::LocateCheck()
{
	try
	{
		HObject  ho_X, ho_Y;
		HObject  ho_Z, ho_Normals;
		HTuple  hv_DLSample, hv_DLResult;

		Decompose3(m_hImage[1], &ho_X, &ho_Y, &ho_Z);
		GenEmptyObj(&ho_Normals);
		gen_dl_samples_3d_gripping_point_detection(m_hImage[0], ho_X, ho_Y, ho_Z, ho_Normals,
			&hv_DLSample);
		//
		preprocess_dl_samples(hv_DLSample, m_DLPa.hv_DLPreprocessParam);
		//
		ApplyDlModel(m_DLPa.hv_DLModelHandle, hv_DLSample, HTuple(), &hv_DLResult);
		//
		//Generate the result containing poses of possible gripping points.
		gen_dl_3d_gripping_points_and_poses(hv_DLSample, m_DLPa.hv_DLGrippingPointParams, hv_DLResult);
		//

		/*dev_display_dl_data(hv_DLSample, hv_DLResult, m_DLPa.hv_DLDatasetInfo, "gripping_map",
			m_DLPa.hv_DisplayParams, m_DLPa.hv_WindowDict);*/
		dev_display_dl_3d_data(hv_DLSample, hv_DLResult, m_DLPa.hv_DLDatasetInfo, "gripping_point_cloud",
			m_DLPa.hv_DisplayParams3D, m_DLPa.hv_WindowDict, &hv_WindowHandle);
		return true;
	}
	catch (HException& except)
	{
		AfxMessageBox(except.ErrorText().Text());
	}
	return false;
}

bool get_dl_3d_data( HTuple hv_DLResult,vector<HTuple>& hvec_Poses)
{
	HTuple hv_KeyExists, hv_GrippingPoints, hv_GrippingPointCoordDict;
	HTuple  hv_OM3DToolOrigin, hv_OM3DBase;
	HTuple  hv_OM3DTool;
	HTuple hv_NumberOfResults = 0;
	if (0 != (int(hv_DLResult != HTuple())))
	{
		GetDictParam(hv_DLResult, "key_exists", "gripping_points", &hv_KeyExists);
		if (0 != hv_KeyExists)
		{
			GetDictTuple(hv_DLResult, "gripping_points", &hv_GrippingPoints);
			hv_NumberOfResults = hv_GrippingPoints.TupleLength();
			HTuple hv_IndexGrippingPose, end_val65 = (hv_GrippingPoints.TupleLength()) - 1;
			HTuple step_val65 = 1;
			for (hv_IndexGrippingPose = 0; hv_IndexGrippingPose.Continue(end_val65, step_val65); hv_IndexGrippingPose += step_val65)
			{
				hv_GrippingPointCoordDict = HTuple(hv_GrippingPoints[hv_IndexGrippingPose]);
				hvec_Poses.push_back(hv_GrippingPointCoordDict.TupleGetDictTuple("pose"));
			}
		}
	}
	
	return true;
}
bool CDevice::checkLocateCheck(S_Msg& msg)
{
	try
	{
		HObject  ho_X, ho_Y;
		HObject  ho_Z, ho_Normals;
		HTuple  hv_DLSample, hv_DLResult;
		HTuple hv_GrippingPointsExists;
		Decompose3(m_hImage[1], &ho_X, &ho_Y, &ho_Z);
		//Normals are optional.
		//If the sensor provides normals the processing time will be
		//shorter. Otherwise, the preprocessing will automatically
		//compute normals based on the given X, Y and Z images.
		GenEmptyObj(&ho_Normals);
		gen_dl_samples_3d_gripping_point_detection(m_hImage[0], ho_X, ho_Y, ho_Z, ho_Normals,
			&hv_DLSample);
		//
		preprocess_dl_samples(hv_DLSample, m_DLPa.hv_DLPreprocessParam);
		
		//计算比例 训练模型的图片大小和实际图片的大小比例
		//GetDlModelParam(m_DLPa.hv_DLModelHandle, "image_height", &hv_ImageHeight);
		//GetDlModelParam(m_DLPa.hv_DLModelHandle, "image_width", &hv_ImageWidth);
		HTuple hv_Width, hv_Height;
		GetImageSize(m_hImage[1], &hv_Width, &hv_Height);
		float dx = hv_Width.D()/ m_DLPa.image_width;
		float dy = hv_Height.D()/ m_DLPa.image_height;
		ApplyDlModel(m_DLPa.hv_DLModelHandle, hv_DLSample, HTuple(), &hv_DLResult);
		//
		//Generate the result containing poses of possible gripping points.
		gen_dl_3d_gripping_points_and_poses(hv_DLSample, m_DLPa.hv_DLGrippingPointParams, hv_DLResult);
		//

		GetDictParam(hv_DLResult, "key_exists", "gripping_points", &hv_GrippingPointsExists);
		if (0 != hv_GrippingPointsExists)
		{
			hv_Rows.Clear();
			hv_Columns.Clear();
			get_gripping_points_from_dict(hv_DLResult, &hv_Rows, &hv_Columns);
		}
		hv_CPose.clear();
		get_dl_3d_data( hv_DLResult, hv_CPose);

		msg.vm[0] = HObject2Mat(m_hImage[0]);
		if (1 == msg.vm[0].channels())
		{
			cvtColor(msg.vm[0], msg.vm[0], CV_GRAY2RGB);
		}
		int size = 0.5 + msg.vm[0].cols > msg.vm[0].rows ? msg.vm[0].rows / g_lineRate : msg.vm[0].cols / g_lineRate;
		int thickness = 1 + size;
		size = (thickness << 3);

		int len = (hv_Rows.TupleLength());
		for (int i = 0; i<len;i++)
		{
			//转换为机器人的pose
			ConvertPoseType(hv_CPose[i], "Rp+T", "abg", "point", &hv_CPose[i]);

			char sz[256];
			sprintf(sz, "pose:%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%d",  hv_CPose[i][0].D(),
				hv_CPose[i][1].D(), hv_CPose[i][2].D(), hv_CPose[i][3].D(), hv_CPose[i][4].D(), hv_CPose[i][5].D(), hv_CPose[i][6].I());
			msg.str += sz;
			msg.x = hv_CPose[i][0].D();//x平移
			msg.y = hv_CPose[i][1].D();//y平移
			msg.z = hv_CPose[i][2].D();//z平移
			msg.xr = hv_CPose[i][3].D();//x旋转
			msg.yr = hv_CPose[i][4].D();//y旋转
			msg.zr = hv_CPose[i][5].D();//z旋转
			//转换到平面图片上显示
			cv::Point pt( hv_Columns[i].D()* dx, hv_Rows[i].D()*dy);
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
