#include "pch.h"
#include "device.h"
#include "detect_deep_learning.h"
CDLPDlg CDevice::S_dlg;
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
void S_CheckParam::readMode(CConfig& cfg)
{
	string szVal = cfg.getItem("modelFile");
	if ("" != szVal)
	{
		//	replace(szVal.begin(), szVal.end(), '\\', '/');
		modelFile = szVal;
	}
	szVal = cfg.getItem("paramFile");
	if ("" != szVal)
	{
		//	replace(szVal.begin(), szVal.end(), '\\', '/');
		paramFile = szVal;
	}
	szVal = cfg.getItem("m_procName");
	if ("" != szVal)
	{
		m_procName = szVal;
	}
	szVal = cfg.getItem("vctClassNames");
	if ("" != szVal)
	{
		vctClassNames.clear();
		CStrProcess::Split(szVal, ",", vctClassNames, true);
	}
	szVal = cfg.getItem("minConfidence");
	if ("" != szVal)
	{
		minConfidence = atof(szVal.c_str());
	}
	szVal = cfg.getItem("maxOverlap");
	if ("" != szVal)
	{
		maxOverlap = atof(szVal.c_str());
	}
	szVal = cfg.getItem("maxOverlapClassAgnostic");
	if ("" != szVal)
	{
		maxOverlapClassAgnostic = atof(szVal.c_str());
	}
	szVal = cfg.getItem("m_dlType");
	if ("" != szVal)
	{
		m_dlType = atoi(szVal.c_str());
	}
	iniMode();
}

void S_CheckParam::writeMode(CConfig& cfg)
{
	replace(modelFile.begin(), modelFile.end(), '\\', '/');
	cfg.updateItem("modelFile", modelFile);
	replace(paramFile.begin(), paramFile.end(), '\\', '/');
	cfg.updateItem("paramFile", paramFile);
	string szname = "";
	for (int i = 0; i < vctClassNames.size(); i++)
	{
		szname += vctClassNames[i] + ",";
	}
	cfg.updateItem("vctClassNames", szname);
	cfg.updateItem("m_procName", m_procName);
	char sz[32];
	sprintf(sz, "%f", minConfidence);
	cfg.updateItem("minConfidence", sz);
	sprintf(sz, "%f", maxOverlap);
	cfg.updateItem("maxOverlap", sz);
	sprintf(sz, "%f", maxOverlapClassAgnostic);
	cfg.updateItem("maxOverlapClassAgnostic", sz);
	sprintf(sz, "%d", m_dlType);
	cfg.updateItem("m_dlType", sz);
}

void S_CheckParam::iniMode()
{
	try
	{
		//初始化
		hv_DLModelHandle.Clear();
		hv_DLPreprocessParam.Clear();
		// Local control variables
		HTuple  hv_UsePretrainedModel;
		HTuple  hv_DLDeviceHandles, hv_DLDevice, hv_ExampleInternals;
		HTuple  hv_ExampleDataDir, hv_PreprocessParamFileName;
		HTuple  hv_RetrainedModelFileName, hv_DataDirectory;
		HTuple  hv_GenParamDisplay;
		HTuple  hv_MinConfidence, hv_MaxOverlap, hv_MaxOverlapClassAgnostic;

		//
		//This example is part of a series of examples, which summarizes
		//the workflow for DL segmentation. It uses the MVTec pill dataset.
		//The four parts are:
		//1. Dataset preprocessing.
		//2. Training of the model.
		//3. Evaluation of the trained model.
		//4. Inference on new images.
		//
		//This example covers part 4: Inference (application)
		//of a trained model on new images.
		//
		//Please note: This script uses a pretrained model. To use the output
		//of part 1 and part 2 of this example series, set UsePretrainedModel
		//to false below.
		//
		//By default, this example uses a model pretrained by MVTec. To use the model
		//which was trained in part 2 of this example series, set the following
		//variable to false.
		hv_UsePretrainedModel = 1;
		//
		//Inference can be done on a GPU or CPU.
		//See the respective system requirements in the Installation Guide.
		//If possible a GPU is used in this example.
		//In case you explicitely wish to run this example on the CPU,
		//choose the CPU device instead.
		QueryAvailableDlDevices((HTuple("runtime").Append("runtime")), (HTuple("gpu").Append("cpu")),
			&hv_DLDeviceHandles);
		if (0 != (int((hv_DLDeviceHandles.TupleLength()) == 0)))
		{
			throw HException("No supported device found to continue this example.");
		}
		//Due to the filter used in query_available_dl_devices, the first device is a GPU, if available.
		hv_DLDevice = ((const HTuple&)hv_DLDeviceHandles)[0];
		//
		//
		//
		//*************************************************
		//**   Set paths and parameters for inference   ***
		//*************************************************
		//
		//We will demonstrate the inference on the example images.
		//In a real application newly incoming images (not used for training or evaluation)
		//would be used here.
		//

		if (0 != hv_UsePretrainedModel)
		{
			//Use the pretrained model shipping with HALCON.
			//
			//File name of dict containing parameters used for preprocessing.
			hv_PreprocessParamFileName = (HTuple)(paramFile.c_str());
			//Path of the retrained segmentation model.
			hv_RetrainedModelFileName = (HTuple)(modelFile.c_str());
			//	hv_PreprocessParamFileName = "D:/code/AutoVision/x64/Debug/Segmentation/segment_pill_defects_preprocess_param.hdict";
			//	hv_RetrainedModelFileName = "D:/code/AutoVision/x64/Debug/Segmentation/segment_pill_defects.hdl";

		}
		else
		{
			//
		//Example data folder containing the outputs of the previous example series.
			hv_ExampleDataDir = (HTuple)((getExePath() + "segment_pill_defects_data").c_str());
			//
			//File name of dict containing parameters used for preprocessing.
			//Note: Adapt DataDirectory after preprocessing with another image size.
			hv_DataDirectory = hv_ExampleDataDir + "/dldataset_pill_400x400";
			hv_PreprocessParamFileName = hv_DataDirectory + "/dl_preprocess_param.hdict";
			//Path of the retrained segmentation model.
			hv_RetrainedModelFileName = hv_ExampleDataDir + "/best_dl_model_segmentation.hdl";
		}
		//
		//Provide the class names and IDs.
		//Class names.
		hv_ClassNames.Clear();
		/*hv_ClassNames[0] = "good";
		hv_ClassNames[1] = "contamination";
		hv_ClassNames[2] = "crack";*/
		//Respective class IDs.
		hv_ClassIDs.Clear();
		/* hv_ClassIDs[0] = 0;
		 hv_ClassIDs[1] = 1;
		 hv_ClassIDs[2] = 2;*/
		for (int i = 0; i < vctClassNames.size(); i++)
		{
			hv_ClassNames[i] = (HTuple)(vctClassNames[i].c_str());
			hv_ClassIDs[i] = i;
		}

		//********************
		//**   Inference   ***
		//********************
		//
		//Check if all necessary files exist.
	/*	check_data_availability(hv_ExampleDataDir, hv_PreprocessParamFileName, hv_RetrainedModelFileName,
			hv_UsePretrainedModel);*/
		HTuple  hv_FileExists;
		FileExists(hv_PreprocessParamFileName, &hv_FileExists);
		if (0 != (hv_FileExists.TupleNot()))
		{
			throw HException(hv_PreprocessParamFileName + " does not exist. Please run part 1 of example series.");
		}
		//
		FileExists(hv_RetrainedModelFileName, &hv_FileExists);
		if (0 != (hv_FileExists.TupleNot()))
		{
			throw HException(hv_RetrainedModelFileName + " does not exist. Please run the HALCON Deep Learning installer.");
		}

		//
		//Read in the retrained model.
		ReadDlModel(hv_RetrainedModelFileName, &hv_DLModelHandle);
		//
		//Set the batch size.
		SetDlModelParam(hv_DLModelHandle, "batch_size", 1);
		//
		//Initialize the model for inference.
		SetDlModelParam(hv_DLModelHandle, "device", hv_DLDevice);
		//不同之处
		//Postprocessing parameters for the detection model.
		hv_MinConfidence = minConfidence;
		hv_MaxOverlap = maxOverlap;
		hv_MaxOverlapClassAgnostic = maxOverlapClassAgnostic;
		if (m_dlType == 0)
		{
			//Set postprocessing parameters for model.
			SetDlModelParam(hv_DLModelHandle, "min_confidence", hv_MinConfidence);
			SetDlModelParam(hv_DLModelHandle, "max_overlap", hv_MaxOverlap);
			SetDlModelParam(hv_DLModelHandle, "max_overlap_class_agnostic", hv_MaxOverlapClassAgnostic);
		}
		//
		//Get the parameters used for preprocessing.
		ReadDict(hv_PreprocessParamFileName, HTuple(), HTuple(), &hv_DLPreprocessParam);

		GetDlModelParam(hv_DLModelHandle, "image_height", &hv_H);
		GetDlModelParam(hv_DLModelHandle, "image_width", &hv_W);

		m_iniSucc = true;
	}
	catch (HException& except)
	{
		AfxMessageBox(except.ErrorText().Text());
		m_iniSucc = false;
	}
}

CDevice::CDevice()
{
	SetHcppInterfaceStringEncodingIsUtf8(false);
	// Default settings used in HDevelop (can be omitted)
	SetSystem("width", 512);
	SetSystem("height", 512);
	m_vctDLPa.push_back(S_CheckParam());
	m_curId = 0;
	m_cfgDir = "";
	
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
	if (file != "")
	{
		pww_mkdir(file.c_str());
		m_cfgDir = file;
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
		m_vctDLPa[i].readMode(cfg);
	}
}

void CDevice::writeMode(string file)
{
	if (file != "")
	{
		pww_mkdir(file.c_str());
		m_cfgDir = file;
	}
	delCfgFiles(m_cfgDir.c_str(), 1);

	CConfig cfg(m_cfgDir + "\\checkParam.cfg");
	cfg.updateItem("DPProcNum", CStrProcess::itoa_radixN(m_vctDLPa.size()));
	cfg.updateItem("m_curId", CStrProcess::itoa_radixN(m_curId));

	for (int i = 0; i < m_vctDLPa.size(); i++)
	{
		CConfig cfg(m_cfgDir + "\\checkParam" + CStrProcess::itoa_radixN(i) + ".cfg");
		m_vctDLPa[i].writeMode(cfg);
	}
}

void CDevice::iniMode()
{
	for (int i = 0; i < m_vctDLPa.size(); i++)
		m_vctDLPa[i].iniMode();

}

bool CDevice::EditModel(cv::Mat& src)
{
	//	HImage ho_Image = Mat2HObject(src);

	S_dlg.m_pCheck = this;
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

bool CDevice::checkLocateCheck(S_Msg& msg, int modeId)
{
	cv::Mat& src = msg.vm[0];
	if (m_vctDLPa.size() <= modeId)
		return false;

	S_CheckParam& pa = m_vctDLPa[modeId];
	try
	{
		HObject  ho_ImageBatch, ho_Image, ho_SegmentationImage;
		ho_ImageBatch = Mat2HObject(src);
		HTuple  hv_DLSampleBatch, hv_DLResultBatch;
		HTuple  hv_Areas, hv_Index, hv_Area;
		HTuple  hv_Row, hv_Column, hv_ConnectIndex;
		// Local iconic variables
		HObject  ho_ClassRegions, ho_ClassRegion, ho_ConnectedRegions;
		HObject  ho_CurrentRegion;

		//Generate the DLSampleBatch.
		gen_dl_samples_from_images(ho_ImageBatch, &hv_DLSampleBatch);
		//
		//Preprocess the DLSampleBatch.
		preprocess_dl_samples(hv_DLSampleBatch, pa.hv_DLPreprocessParam);

		//开始不同了
		//Apply the DL model on the DLSampleBatch.
		ApplyDlModel(pa.hv_DLModelHandle, hv_DLSampleBatch, HTuple(), &hv_DLResultBatch);//结果包含所有内容
		//Postprocessing and visualization.
		//Loop over each sample in the batch.
		HTuple  hv_DLSample, hv_DLResult, hv_DetectedClassIDs, hv_NumberDetectionsPerClass;
		//
		//Get sample and according results.
		hv_DLSample = HTuple(hv_DLSampleBatch[0]);
		hv_DLResult = HTuple(hv_DLResultBatch[0]);

		//Get image.
	//	GetDictObject(&ho_Image, hv_DLSample, "image");
	//	GetImageSize(ho_Image, &hv_H, &hv_W);
		double rw = src.cols / pa.hv_W.D();//图像缩小了 要还原坐标 求坐标缩放系数
		double rh = src.rows / pa.hv_H.D();

		string szOk = OKSIGNSTR, szNg = NGSIGNSTR;
		tinyxml2::XMLDocument retXmlDoc;
		tinyxml2::XMLElement* RootElement = retXmlDoc.NewElement("POS");

		if (pa.m_dlType == 0)
		{
			HTuple  hv_Row2, hv_Column2, hv_confiidence;
			GetDictTuple(hv_DLResult, "bbox_row1", &hv_Row);
			GetDictTuple(hv_DLResult, "bbox_col1", &hv_Column);
			GetDictTuple(hv_DLResult, "bbox_row2", &hv_Row2);
			GetDictTuple(hv_DLResult, "bbox_col2", &hv_Column2);
			//	GetDictTuple(hv_DLResult, "bbox_confidence", &hv_confiidence);
			GetDictTuple(hv_DLResult, "bbox_class_id", &hv_DetectedClassIDs);

			//分析所有目标
			{
				HTuple end_val190 = (hv_Row.TupleLength()) - 1;
				HTuple step_val190 = 1;
				HTuple hv_left, hv_top, hv_right, hv_bottom;
				char sz[64];
				for (hv_Index = 0; hv_Index.Continue(end_val190, step_val190); hv_Index += step_val190)
				{
					hv_left = hv_Column[hv_Index];
					hv_top = hv_Row[hv_Index];
					hv_right = hv_Column2[hv_Index];
					hv_bottom = hv_Row2[hv_Index];
					cv::Point pt1(hv_left.D() * rw, hv_top.D() * rh), pt2(hv_right.D() * rw, hv_bottom.D() * rh);
					cv::Scalar color(0, 0, 255);
					sprintf(sz, "%d,%d,%d,%d", pt1.x, pt1.y, pt2.x, pt2.y);
					int classID = hv_DetectedClassIDs[hv_Index].I() - 1;
					if (0 == hv_Index)
					{
						RootElement->SetAttribute("AnotherName", pa.vctClassNames[classID].c_str());
						if (0 == classID)//第一个类是ok的
							RootElement->SetAttribute(szOk.c_str(), sz), color = cv::Scalar(0, 255, 0);
						else
							RootElement->SetAttribute(szNg.c_str(), sz);;
						retXmlDoc.InsertFirstChild(RootElement);

					}
					else
					{
						tinyxml2::XMLElement* newRootElement = retXmlDoc.NewElement("mask");
						newRootElement->SetAttribute("AnotherName", pa.vctClassNames[classID].c_str());
						RootElement->InsertEndChild(newRootElement);
						if (0 == classID)//第一个类是ok的
							newRootElement->SetAttribute(szOk.c_str(), sz), color = cv::Scalar(0, 255, 0);
						else
							newRootElement->SetAttribute(szNg.c_str(), sz);;

					}
					cv::rectangle(src, pt1, pt2, color, 2);
				}
			}
			//统计每个类的数量		
			TupleGenConst(pa.hv_ClassIDs.TupleLength(), 0, &hv_NumberDetectionsPerClass);
			msg.m_nc_ok = 3;
			//Count detected pills for each class.
			{
				HTuple end_val199 = (pa.hv_ClassIDs.TupleLength()) - 1;
				HTuple step_val199 = 1;
				for (hv_Index = 0; hv_Index.Continue(end_val199, step_val199); hv_Index += step_val199)
				{
					hv_NumberDetectionsPerClass[hv_Index] = (hv_DetectedClassIDs.TupleEqualElem(HTuple(pa.hv_ClassIDs[hv_Index]))).TupleSum();
					if (hv_NumberDetectionsPerClass[hv_Index] == 0 || hv_NumberDetectionsPerClass[hv_Index] >1)
					{
						msg.m_nc_ok = 2;
					}
				}
			}
		}
		else//语义分割
		{
			//获取置信度
		//	HObject ho_confidence;
		//	GetDictObject(&ho_confidence, HTuple(hv_DLResultBatch[0]), "segmentation_confidence");
			//Get result image.
			GetDictObject(&ho_SegmentationImage, HTuple(hv_DLResultBatch[0]), "segmentation_image");
			//
			//Postprocessing: Get segmented regions for each class.
			Threshold(ho_SegmentationImage, &ho_ClassRegions, pa.hv_ClassIDs, pa.hv_ClassIDs);
			//

			//Separate any components of the class regions
			//and display result regions as well as their area.
			//
			//Get area of class regions.
			RegionFeatures(ho_ClassRegions, "area", &hv_Areas);
			if (0 != (int(((hv_Areas.TupleSum()) - HTuple(hv_Areas[0])) > 0)))
			{
				//hv_Text = "NOK";
				msg.m_nc_ok = 2;
			}
			else
				msg.m_nc_ok = 3;
			char sz[64];
			//第一个类 肯定是ok的类
			cv::Scalar color(0, 255, 0);
			HTuple hv_left, hv_top, hv_right, hv_bottom;
			if (0 != (int(HTuple(hv_Areas[0]) > 0)))
			{
				SelectObj(ho_ClassRegions, &ho_ClassRegion, 1);
				//Get connected components of the segmented class region.
				Connection(ho_ClassRegion, &ho_ConnectedRegions);
				AreaCenter(ho_ConnectedRegions, &hv_Area, &hv_Row, &hv_Column);
				{
					HTuple end_val196 = (hv_Area.TupleLength()) - 1;
					HTuple step_val196 = 1;
					for (hv_ConnectIndex = 0; hv_ConnectIndex.Continue(end_val196, step_val196); hv_ConnectIndex += step_val196)
					{
						SelectObj(ho_ConnectedRegions, &ho_CurrentRegion, hv_ConnectIndex + 1);
						SmallestRectangle1(ho_CurrentRegion, &hv_top, &hv_left, &hv_bottom, &hv_right);
						cv::Point pt1(hv_left.D() * rw, hv_top.D() * rh), pt2(hv_right.D() * rw, hv_bottom.D() * rh);
						if (3== msg.m_nc_ok)
							cv::rectangle(src, pt1, pt2, color, 2);

						RootElement->SetAttribute("AnotherName", pa.hv_ClassNames[0].S());
						sprintf(sz, "%d,%d,%d,%d", pt1.x, pt1.y, pt2.x, pt2.y);
						RootElement->SetAttribute(szOk.c_str(), sz), color = cv::Scalar(0, 255, 0);
						retXmlDoc.InsertFirstChild(RootElement);
					}
				}

			}
			//
			HTuple end_val190 = (hv_Areas.TupleLength()) - 1;
			HTuple step_val190 = 1;				
			color = cv::Scalar(0, 0, 255);
			for (hv_Index = 1; hv_Index.Continue(end_val190, step_val190); hv_Index += step_val190)
			{
				if (0 != (int(HTuple(hv_Areas[hv_Index]) > 0)))//这里可以设置面积的大小范围 进行过滤
				{
					SelectObj(ho_ClassRegions, &ho_ClassRegion, hv_Index + 1);
					//Get connected components of the segmented class region.
					Connection(ho_ClassRegion, &ho_ConnectedRegions);
					AreaCenter(ho_ConnectedRegions, &hv_Area, &hv_Row, &hv_Column);
					{
						HTuple end_val196 = (hv_Area.TupleLength()) - 1;
						HTuple step_val196 = 1;
						for (hv_ConnectIndex = 0; hv_ConnectIndex.Continue(end_val196, step_val196); hv_ConnectIndex += step_val196)
						{
							SelectObj(ho_ConnectedRegions, &ho_CurrentRegion, hv_ConnectIndex + 1);
							SmallestRectangle1(ho_CurrentRegion, &hv_top, &hv_left, &hv_bottom, &hv_right);
							cv::Point pt1(hv_left.D() * rw, hv_top.D() * rh), pt2(hv_right.D() * rw, hv_bottom.D() * rh);
							cv::rectangle(src, pt1, pt2, color, 2);

							//string szRet = hv_ClassNames[hv_ClassIndex];
							//cv::Point2f pt = cv::Point2f(hv_Column[hv_ConnectIndex]*rw, hv_Row[hv_ConnectIndex]*rh);
							//cv::line(src, pt, pt, cv::Scalar(0, 0, 255), 8);
							//cv::putText(src, szRet, pt, cv::FONT_HERSHEY_TRIPLEX, 4, color, 2);
							//imshow("sfs", src);

							tinyxml2::XMLElement* newRootElement = retXmlDoc.NewElement("mask");
							newRootElement->SetAttribute("AnotherName", pa.hv_ClassNames[0].S());
							RootElement->InsertEndChild(newRootElement);
							sprintf(sz, "%d,%d,%d,%d", pt1.x, pt1.y, pt2.x, pt2.y);
							newRootElement->SetAttribute(szNg.c_str(), sz);
						}
					}
				}
			}
			
		}
		tinyxml2::XMLPrinter printer(0, true);
		if (retXmlDoc.Accept(&printer))//retXmlDoc.Print(&printer);
			msg.str = printer.CStr();
		else
			msg.str = "";
		return 3 == msg.m_nc_ok;
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

