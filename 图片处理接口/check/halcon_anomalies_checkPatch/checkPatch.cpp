#include "pch.h"
#include "checkPatch.h"
#include "dl_anomaly_detection_workflow.h"

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

checkPatch::checkPatch()
{
	m_cfgDir = "";
	//Folder where the preprocessed samples will be stored.所有样本预处理后的保存路径
	hv_OutputDir = "./anomaly_data";
	//
	//Dataset specific preprocessing.数据集特定的预处理
	hv_ExampleSpecificPreprocessing = 1;
	//
	//Set image size. It should be large enough if the defects are small.
	//Please refer to the documentation of read_dl_model for possible restrictions
	//depending on the network.缩放后的大小（32的倍数）
	hv_ImageWidth = 320;
	hv_ImageHeight = 320;
	//
	//Set the complexity of the model which roughly describes the capability of the model
	//to handle complex application images. A higher value can improve the performance but
	//increases the time needed to train the model.复杂度，越大准确率越高，训练越耗时
	hv_Complexity = 15;
	//设置训练终止条件，错误率、次数，满足其一则终止
	//Set a threshold for the training error and a maximum number of training epochs.
	//If the training error falls below this threshold, the training is finished.
	//Otherwise the training continues until the maximum number of epochs is reached.
	hv_ErrorThreshold = 0.001;
	hv_MaxNumEpochs = 15;
	//
	//Set the domain ratio which controls the fraction of each image used for training.
	//The training result might be improved by setting a greater value, but this also
	//increases the training time.训练集中用于训练的样本比
	hv_DomainRatio = 0.25;
	//
	//Regularization noise can make the training more robust. In case the training fails,
	//setting a higher value might help.正则化噪声，使得训练更健壮。为防止训练失败，可以设置大些
	hv_RegularizationNoise = 0.01;
	//value might be suitable.标准差因子（如果缺陷很小，推荐较大值）
	hv_StandardDeviationFactor = 1.0;
	//设置阈值（模型训练后得到）
	hv_InferenceClassificationThreshold = 0.183618;
	hv_InferenceSegmentationThreshold = 0.236205;
	hv_use_ok_samples = 1;
}

checkPatch::~checkPatch()
{
	clearMode();
}

void checkPatch::clearMode()
{

}

void checkPatch::readMode(string file)
{
	clearMode();
	if (file != "")
	{
		replace(file.begin(), file.end(), '\\', '/');//halcon要求/
		mkdirs((char*)file.c_str());
		pww_mkdir((file + "/ok").c_str());
		pww_mkdir((file + "/ng").c_str());
		m_cfgDir = file;
	}
	CConfig cfg0(m_cfgDir + "/checkParam.cfg");
	string szVal = cfg0.getItem("modelFile");
	if ("" != szVal)
	{
		m_pa.modelFile = szVal;
	}
	else
		m_pa.modelFile = m_cfgDir + "/model_final.hdl";

	szVal = cfg0.getItem("hv_ExampleSpecificPreprocessing");
	if ("" != szVal)
	{
		hv_ExampleSpecificPreprocessing = atoi(szVal.c_str());//
	}
	szVal = cfg0.getItem("hv_ImageWidth");
	if ("" != szVal)
	{
		hv_ImageWidth = atoi(szVal.c_str());//
	}
	szVal = cfg0.getItem("hv_ImageHeight");
	if ("" != szVal)
	{
		hv_ImageHeight = atoi(szVal.c_str());//
	}
	szVal = cfg0.getItem("hv_Complexity");
	if ("" != szVal)
	{
		hv_Complexity = atoi(szVal.c_str());//
	}
	szVal = cfg0.getItem("hv_use_ok_samples");
	if ("" != szVal)
	{
		hv_use_ok_samples = atoi(szVal.c_str());//
	}
	szVal = cfg0.getItem("hv_ErrorThreshold");
	if ("" != szVal)
	{
		hv_ErrorThreshold = atof(szVal.c_str());//
	}
	szVal = cfg0.getItem("hv_MaxNumEpochs");
	if ("" != szVal)
	{
		hv_MaxNumEpochs = atoi(szVal.c_str());//
	}
	szVal = cfg0.getItem("hv_DomainRatio");
	if ("" != szVal)
	{
		hv_DomainRatio = atof(szVal.c_str());//
	}
	szVal = cfg0.getItem("hv_RegularizationNoise");
	if ("" != szVal)
	{
		hv_RegularizationNoise = atof(szVal.c_str());// 
	}
	szVal = cfg0.getItem("hv_StandardDeviationFactor");
	if ("" != szVal)
	{
		hv_StandardDeviationFactor = atof(szVal.c_str());//
	}
	szVal = cfg0.getItem("hv_InferenceClassificationThreshold");
	if ("" != szVal)
	{
		hv_InferenceClassificationThreshold = atof(szVal.c_str());// 
	}
	szVal = cfg0.getItem("hv_InferenceSegmentationThreshold");
	if ("" != szVal)
	{
		hv_InferenceSegmentationThreshold = atof(szVal.c_str());//
	}
	
	iniMode();
}

void checkPatch::writeMode(string file)
{
	if (file != "")
	{
		replace(file.begin(), file.end(), '\\', '/');//halcon要求/
		mkdirs((char*)file.c_str());
		pww_mkdir((file + "/ok").c_str());
		pww_mkdir((file + "/ng").c_str());
		m_cfgDir = file;
	}
	delCfgFiles(m_cfgDir.c_str(), 1);
	CConfig cfg(m_cfgDir + "/checkParam.cfg");
	replace(m_pa.modelFile.begin(), m_pa.modelFile.end(), '\\', '/');
	cfg.updateItem("modelFile", m_pa.modelFile);

	char sz[32];
	sprintf(sz, "%d", hv_ExampleSpecificPreprocessing.I());
	cfg.updateItem("hv_ExampleSpecificPreprocessing", sz);//
	sprintf(sz, "%d", hv_ImageWidth.I());
	cfg.updateItem("hv_ImageWidth", sz);//
	sprintf(sz, "%d", hv_ImageHeight.I());
	cfg.updateItem("hv_ImageHeight", sz);//
	sprintf(sz, "%d", hv_Complexity.I());
	cfg.updateItem("hv_Complexity", sz);//
	sprintf(sz, "%d", hv_use_ok_samples.I());
	cfg.updateItem("hv_use_ok_samples", sz);//
	sprintf(sz, "%f", hv_ErrorThreshold.D());
	cfg.updateItem("hv_ErrorThreshold", sz);//
	sprintf(sz, "%d", hv_MaxNumEpochs.I());
	cfg.updateItem("hv_MaxNumEpochs", sz);//
	sprintf(sz, "%f", hv_DomainRatio.D());
	cfg.updateItem("hv_DomainRatio", sz);//
	sprintf(sz, "%f", hv_RegularizationNoise.D());
	cfg.updateItem("hv_RegularizationNoise", sz);//
	sprintf(sz, "%f", hv_StandardDeviationFactor.D());
	cfg.updateItem("hv_StandardDeviationFactor", sz);//
	//Store the trained model.保存模型
	sprintf(sz, "%f", hv_InferenceClassificationThreshold.D());
	cfg.updateItem("hv_InferenceClassificationThreshold", sz);//
	sprintf(sz, "%f", hv_InferenceSegmentationThreshold.D());
	cfg.updateItem("hv_InferenceSegmentationThreshold", sz);//
	//Store the trained model.保存模型

	if (m_pa.modelFile == "");
		m_pa.modelFile = m_cfgDir + "/model_final.hdl";

	WriteDlModel(hv_DLModelHandle, m_pa.modelFile.c_str());
}

void checkPatch::iniMode()
{
	//初始化
	hv_DLModelHandle.Clear();
	// Local control variables
	HTuple  hv_DLDeviceHandles;
	HTuple  hv_RetrainedModelFileName;

	//Use the pretrained model shipping with HALCON.
	//
	//Path of the retrained segmentation model.
	hv_RetrainedModelFileName = (HTuple)(m_pa.modelFile.c_str());
	hv_OutputDir = (m_cfgDir+"/anomaly_data").c_str();
	//Read in the retrained model.
	HTuple  hv_FileExists;
	FileExists(hv_RetrainedModelFileName, &hv_FileExists);
	if (0 != (hv_FileExists.TupleNot()))
	{
		return;
		//	throw HException(hv_RetrainedModelFileName + " does not exist. Please run the HALCON Deep Learning installer.");
	}
	ReadDlModel(hv_RetrainedModelFileName, &hv_DLModelHandle);
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
	SetDlModelParam(hv_DLModelHandle, "device", hv_DLDeviceHandles);
//	GetDlModelParam(hv_DLModelHandle, "image_height", &hv_ImageHeight);
//	GetDlModelParam(hv_DLModelHandle, "image_width", &hv_ImageWidth);
}

bool checkPatch::EditModel(cv::Mat& src)
{
	//	HImage ho_Image = Mat2HObject(src);

	S_dlg.m_pCheck = this;
	S_dlg.m_ImageOrg = Mat2HObject(src);
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

bool checkPatch::get_dl_data(HTuple hv_DLSample, HTuple hv_DLResult, HTuple hv_DLDatasetInfo,HTuple* hv_WindowHandle)
{
		// Local iconic variables
	HObject  ho_AnomalyImage,ho_Image;

	// Local control variables
	HTuple  hv_ResultKeys, hv_NeededKeys,hv_Index,hv_DLDatasetInfoKeys,hv_ClassNames,hv_ClassIDs;
	HTuple hv_Colors, hv_SampleKeys, hv_AnomalyClassID, hv_AnomalyClassThresholdDisplay, hv_CurrentWindowHandle;
	HTuple hv_AnomalyScore, hv_AnomalyRegionThresholdDisplay;
	HTuple hv_AnomalyRegionResultColor, hv_LineWidth, hv_AnomalyRegionThreshold, hv_AnomalyClassificationThreshold;

	//Get the dictionary keys.
	if (0 != (int(hv_DLResult != HTuple())))
	{
		GetDictParam(hv_DLResult, "keys", HTuple(), &hv_ResultKeys);
	}

	hv_AnomalyRegionThreshold = -1;
	hv_AnomalyClassificationThreshold = -1;
	//
	//** Display the data: ***
	//Get image.
	get_image(&ho_Image, hv_SampleKeys, hv_DLSample);
	ho_AnomalyRegion.Clear();
	//Get the anomaly results either by applying the specified thresholds or out of DLResult.
	get_anomaly_result(&ho_AnomalyImage, &ho_AnomalyRegion, hv_DLResult, hv_ResultKeys,
		hv_AnomalyClassificationThreshold, hv_AnomalyRegionThreshold, &hv_AnomalyScore,
		&hv_AnomalyClassID, &hv_AnomalyClassThresholdDisplay, &hv_AnomalyRegionThresholdDisplay);

	if(0!= hv_WindowHandle)
		DispObj(ho_Image, *hv_WindowHandle);
	if (0 != (int(hv_AnomalyClassID == 0)))
	{
		return true;// hv_Text[hv_Text.TupleLength()] = hv_AnomalyClassID + " : 'ok'";
	}
	//Display anomaly regions defined by the specified threshold or from DLResult.
	if (0 != (HTuple(int(hv_AnomalyRegionThreshold != -1)).TupleOr(int((hv_ResultKeys.TupleFind("anomaly_region")) != -1))))
	{
		hv_AnomalyRegionResultColor = "#ff0000c0";
		hv_LineWidth = 2;
		dev_display_result_anomaly_regions(ho_AnomalyRegion, hv_CurrentWindowHandle,
			hv_LineWidth, hv_AnomalyRegionResultColor, hv_WindowHandle);
	}
	
	//Display results.
	if (0 != hv_WindowHandle)
	{
		FlushBuffer(*hv_WindowHandle);
	//	SetWindowParam(*hv_WindowHandle, "flush", "false");
	}
	
	if (0 != (int(hv_AnomalyClassID == 1)))
	{
		return false; //hv_Text[hv_Text.TupleLength()] = hv_AnomalyClassID + " : 'nok'";
	}
	else
	{
		return false;// hv_Text[hv_Text.TupleLength()] = "No classification result found";
	}
}

bool checkPatch::predict(HObject& ho_Image)
{
	HTuple  hv_DLDatasetInfo, hv_DLSample, hv_DLPreprocessParam;
	HTuple  hv_DLResult;
	create_dl_preprocess_param("anomaly_detection", hv_ImageWidth, hv_ImageHeight,
		3, HTuple(), HTuple(), "constant_values", "full_domain", HTuple(), HTuple(),
		HTuple(), HTuple(), &hv_DLPreprocessParam);
	//创建类别标签字典(不起作用，但是必须有)
	//Create dictionary with dataset parameters used for display.
	CreateDict(&hv_DLDatasetInfo);
	SetDictTuple(hv_DLDatasetInfo, "class_names", (HTuple("ok").Append("ng")));
	SetDictTuple(hv_DLDatasetInfo, "class_ids", (HTuple(0).Append(1)));
	gen_dl_samples_from_images(ho_Image, &hv_DLSample);
	preprocess_dl_samples(hv_DLSample, hv_DLPreprocessParam);
	//与训练时相同的特定处理
	//Use the same dataset specific preprocessing as for training.
	if (0 != hv_ExampleSpecificPreprocessing)
	{
		preprocess_dl_samples_bottle(hv_DLSample);
	}
	//
	ApplyDlModel(hv_DLModelHandle, hv_DLSample, HTuple(), &hv_DLResult);
	//
	//Apply thresholds to classify regions and the entire image.
	threshold_dl_anomaly_results(hv_InferenceSegmentationThreshold, hv_InferenceClassificationThreshold,
		hv_DLResult);
	//
	//Display the inference result.
	if (S_dlg.m_hWnd && S_dlg.IsWindowVisible())
	{
		//这里面研究一下怎么显示异常到图像中
		return get_dl_data(hv_DLSample, hv_DLResult, hv_DLDatasetInfo,  &S_dlg.hv_WindowHandle);
	}
	else
		return get_dl_data(hv_DLSample, hv_DLResult, hv_DLDatasetInfo, NULL);
}

void checkPatch::Train()
{
	hv_DLModelHandle.Clear();
	HTuple hv_GenParamDataset, hv_DLDevice, hv_PreprocessSettings, hv_DLPreprocessParam;
	HTuple hv_DLDataset, hv_DLDatasetFileName, hv_DatasetSamples, hv_DLSampleBatch, hv_TrainParamAnomaly;

	HTuple hv_TrainParam, hv_TrainResults, hv_TrainInfos, hv_EvaluationInfos, hv_ImageDir, hv_ImageSubDirs, hv_AnomalyDir;
	HTuple hv_GenParamThreshold, hv_AnomalySegmentationThreshold, hv_AnomalyClassificationThresholds;
	HTuple hv_ClassificationThresholdIndex;
	//训练只需ok文件夹，其他文件夹用于之后的评估
	hv_ImageDir = m_cfgDir.c_str();
	hv_ImageSubDirs.Clear();
	hv_ImageSubDirs[0] = "ok";
	hv_ImageSubDirs[1] = "ng";
	//If there is no ground truth data available, set AnomalyDir to [].缺陷区域的二值图路径（无）
	//hv_AnomalyDir = hv_HalconImages + "/labels/bottles";
	//
	//
	//***   1.) PREPARE   ***
	//
	//Read and preprocess an anomaly detection dataset.
	//For optimal performance it is beneficial to do a custom
	//preprocessing step that defines input domains.
	//
	//Load and split the dataset.
	//CreateDict(&hv_GenParamDataset);
	//SetDictTuple(hv_GenParamDataset, "image_sub_dirs", hv_ImageSubDirs);
	read_dl_dataset_anomaly(hv_ImageDir, hv_AnomalyDir, HTuple(), HTuple(), hv_GenParamDataset,
		&hv_DLDataset);
	//
	//Note: make sure that every split contains at least one 'ok' image.
	//For a small number of images in the dataset you might need to increase
	//the validation fraction. 拆分样本集为训练集(50%)、验证集(10%)、测试集(剩余的)
	if (1 == hv_use_ok_samples.I())
		split_dl_dataset(hv_DLDataset, 90, 10, HTuple());
	else
		split_dl_dataset(hv_DLDataset, 50, 10, HTuple());
	//
	//Load the anomaly detection model and set parameters.
	//For documentation see set_dl_model_param () and get_dl_model_param ().
	string file = getExePath() + "anomalies\\initial_dl_anomaly_medium.hdl";
	replace(file.begin(), file.end(), '\\', '/');//halcon要求/
	ReadDlModel(file.c_str(), &hv_DLModelHandle);
	//initial_dl_anomaly_large.hdl大模型
	SetDlModelParam(hv_DLModelHandle, "image_width", hv_ImageWidth);
	SetDlModelParam(hv_DLModelHandle, "image_height", hv_ImageHeight);
	SetDlModelParam(hv_DLModelHandle, "complexity", hv_Complexity);
	//In this example, the training of the anomaly detection model is done on the CPU.
	//QueryAvailableDlDevices((HTuple("runtime").Append("id")), (HTuple("cpu").Append(0)),
	//	&hv_DLDevice);
	QueryAvailableDlDevices((HTuple("runtime").Append("runtime")), (HTuple("gpu").Append("cpu")),
		&hv_DLDevice);
	if (0 != (int((hv_DLDevice.TupleLength()) == 0)))
	{
		throw HException("No supported device found to continue this example.");
	}
	SetDlModelParam(hv_DLModelHandle, "device", hv_DLDevice);
	//
	//Set preprocessing parameters and preprocess.
	CreateDict(&hv_PreprocessSettings);
	SetDictTuple(hv_PreprocessSettings, "overwrite_files", 1);
	create_dl_preprocess_param("anomaly_detection", hv_ImageWidth, hv_ImageHeight,
		3, HTuple(), HTuple(), "constant_values", "full_domain", HTuple(), HTuple(),
		HTuple(), HTuple(), &hv_DLPreprocessParam);
	preprocess_dl_dataset(hv_DLDataset, hv_OutputDir, hv_DLPreprocessParam, hv_PreprocessSettings,
		&hv_DLDatasetFileName);
	//
	//Run a specific preprocessing for this example.
	//Note: For your own application, this step needs to be adapted.
	//In case the possible areas for defects (ROI) on the images can be restricted,
	//we recommend to define these areas as domains. For this example, this is
	//done in the exemplary procedure preprocess_dl_sample_bottle.获取样本集DLDataset中的样本
	GetDictTuple(hv_DLDataset, "samples", &hv_DatasetSamples);
	if (0 != hv_ExampleSpecificPreprocessing)
	{
		read_dl_samples(hv_DLDataset, HTuple::TupleGenSequence(0, (hv_DatasetSamples.TupleLength()) - 1, 1),
			&hv_DLSampleBatch);
		preprocess_dl_samples_bottle(hv_DLSampleBatch);
		write_dl_samples(hv_DLDataset, HTuple::TupleGenSequence(0, (hv_DatasetSamples.TupleLength()) - 1, 1),
			hv_DLSampleBatch, HTuple(), HTuple());
	}

	//***   2.) TRAIN   ***
	//
	//Create training parameters.
	//是否展示训练过程
	//Control whether the training progress is displayed (true/false).
	HTuple hv_EnableDisplay = 0;

	//创建字典，并存储键-值对
	CreateDict(&hv_TrainParamAnomaly);
	SetDictTuple(hv_TrainParamAnomaly, "regularization_noise", hv_RegularizationNoise);
	SetDictTuple(hv_TrainParamAnomaly, "error_threshold", hv_ErrorThreshold);
	SetDictTuple(hv_TrainParamAnomaly, "domain_ratio", hv_DomainRatio);
	create_dl_train_param(hv_DLModelHandle, hv_MaxNumEpochs, HTuple(), hv_EnableDisplay,
		73, "anomaly", hv_TrainParamAnomaly, &hv_TrainParam);//创建训练参数
	//
	//The training and thus the call of train_dl_model_anomaly_dataset ()
	//is done using the following procedure. This may take some time.
	train_dl_model(hv_DLDataset, hv_DLModelHandle, hv_TrainParam, 0, &hv_TrainResults,
		&hv_TrainInfos, &hv_EvaluationInfos);


	SetDlModelParam(hv_DLModelHandle, "standard_deviation_factor", hv_StandardDeviationFactor);

	//fine-tuning may be beneficial.计算阈值
	CreateDict(&hv_GenParamThreshold);
//	SetDictTuple(hv_GenParamThreshold, "enable_display", "true");
	if( 1 == hv_use_ok_samples.I() )//仅仅使用ok样本
		SetDictTuple(hv_GenParamThreshold, "use_ok_samples_only", "true");
	compute_dl_anomaly_thresholds(hv_DLModelHandle, hv_DLDataset, hv_GenParamThreshold,
		&hv_AnomalySegmentationThreshold, &hv_AnomalyClassificationThresholds);
	//设置阈值（模型训练后得到）
//Choose thresholds for inference.
	hv_ClassificationThresholdIndex = (hv_AnomalyClassificationThresholds.TupleLength()) - 1;
	hv_InferenceClassificationThreshold = HTuple(hv_AnomalyClassificationThresholds[hv_ClassificationThresholdIndex]);
	hv_InferenceSegmentationThreshold = hv_AnomalySegmentationThreshold;

}

bool checkPatch::checkLocateCheck(cv::Mat& src, vector<S_TpltData*>& vctData, CTpltParam& Pa)
{
	try
	{
		int curid = Pa.m_tempPa.m_curPosID;
		HObject  ho_ImageBatch;
		ho_ImageBatch = Mat2HObject(src);
		bool ret =  predict(ho_ImageBatch);
		if (!ret)
		{
			float dx = src.cols / hv_ImageWidth.D();
			float dy = src.rows / hv_ImageHeight.D();
			int objN = 0;
			HTuple hv_Areas, hv_ClassIndex, hv_ConnectIndex;
			RegionFeatures(ho_AnomalyRegion, "area", &hv_Areas);
			HObject ho_ClassRegion, ho_ConnectedRegions, ho_CurrentRegion;
			HTuple   hv_Area,hv_Row, hv_Column;
			HTuple end_val190 = (hv_Areas.TupleLength()) - 1;
			HTuple step_val190 = 1;
			HTuple hv_left, hv_top, hv_right, hv_bottom;
			for (hv_ClassIndex = 0; hv_ClassIndex.Continue(end_val190, step_val190); hv_ClassIndex += step_val190)
			{
				if (0 != (int(HTuple(hv_Areas[hv_ClassIndex]) > 0)))//这里可以设置面积的大小范围 进行过滤
				{
					SelectObj(ho_AnomalyRegion, &ho_ClassRegion, hv_ClassIndex + 1);
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
							cv::Point pt1(hv_left.D() * dx, hv_top.D() * dy), pt2(hv_right.D() * dx, hv_bottom.D() * dy);
							cv::rectangle(src, pt1, pt2, cv::Scalar(0, 0, 255), 2);

							//string szRet = hv_ClassNames[hv_ClassIndex];
							//cv::Point2f pt = cv::Point2f(hv_Column[hv_ConnectIndex]*rw, hv_Row[hv_ConnectIndex]*rh);
							//cv::line(src, pt, pt, cv::Scalar(0, 0, 255), 8);
							//cv::putText(src, szRet, pt, cv::FONT_HERSHEY_TRIPLEX, 4, cv::Scalar(0, 0, 255), 2);
							//imshow("sfs", src);

							objN++;
							if (vctData.size() < objN)
							{
								vctData.push_back(new S_TpltData());
							}
							int a = objN - 1;
							//vctData[a]->m_szRet = hv_ClassNames[hv_ClassIndex];
							vctData[a]->m_roiRect.center.x = hv_Column[hv_ConnectIndex] * dx;
							vctData[a]->m_roiRect.center.y = hv_Row[hv_ConnectIndex] * dy;
							vctData[a]->m_roiRect.size = cv::Size(pt2.x - pt1.x, pt2.y - pt1.y);
							cv::Rect rt = vctData[a]->m_roiRect.boundingRect();
							rt &= cv::Rect(0, 0, src.cols, src.rows);
							vctData[a]->m_vctPic[0] = cv::Mat(src, rt).clone();
							vctData[a]->m_checkRet = false;
						}
					}
				}
			}
		}
		vctData[curid]->m_checkRet = ret;
		return ret;
	}
	catch (HException& except)
	{
		AfxMessageBox(except.ErrorText().Text());
	}
	return false;
}


bool checkPatch::drawResult(cv::Mat& src, cv::RotatedRect& roiRect, CTpltParam& pa)
{
	return true;
}
