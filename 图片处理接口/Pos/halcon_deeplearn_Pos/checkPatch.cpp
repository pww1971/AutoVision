#include "pch.h"
#include "checkPatch.h"
#include "deeplearn.h"
#include <sys/timeb.h>
CDLPDlg checkPatch::S_dlg;
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
		pww_mkdir(file.c_str());
		m_cfgDir = file;
	}
	CConfig cfg0(m_cfgDir + "\\checkParam.cfg");
	
	//	replace(szVal.begin(), szVal.end(), '\\', '/');
	m_pa.modelFile = m_cfgDir + "\\model.hdl";
	//	replace(szVal.begin(), szVal.end(), '\\', '/');
	m_pa.paramFile = m_cfgDir + "\\param.hdict";
	
	string szVal = cfg0.getItem("vctClassNames");
	if ("" != szVal)
	{
		CStrProcess::Split(szVal, ",", m_pa.vctClassNames,true);
	}
	szVal = cfg0.getItem("mytype");
	if ("" != szVal)
	{
		m_pa.mytype = atoi(szVal.c_str());
	}
	szVal = cfg0.getItem("getPic");
	if ("" != szVal)
	{
		m_pa.getPic = atoi(szVal.c_str());
	}
	szVal = cfg0.getItem("szPicPath");
	if ("" != szVal)
	{
		m_pa.szPicPath = szVal;
	}
	iniMode();
}

void checkPatch::writeMode(string file)
{
	if (file != "")
	{
		pww_mkdir(file.c_str());
		m_cfgDir = file;
	}
	delCfgFiles(m_cfgDir.c_str(), 1);

	CConfig cfg(m_cfgDir + "\\checkParam.cfg");
	//	replace(szVal.begin(), szVal.end(), '\\', '/');
	m_pa.modelFile = m_cfgDir + "\\model.hdl";
	//	replace(szVal.begin(), szVal.end(), '\\', '/');
	m_pa.paramFile = m_cfgDir + "\\param.hdict";
	string szname = "";
	for (int i = 0; i < m_pa.vctClassNames.size(); i++)
	{
		szname += m_pa.vctClassNames[i] + ",";
	}
	cfg.updateItem("vctClassNames", szname);
	cfg.updateItem("mytype", CStrProcess::itoa_radixN(m_pa.mytype));
	cfg.updateItem("getPic", CStrProcess::itoa_radixN(m_pa.getPic));
	cfg.updateItem("szPicPath", m_pa.szPicPath);

}

void checkPatch::iniMode()
{
	//初始化
	hv_DLModelHandle.Clear();
	hv_DLPreprocessParam.Clear();
	// Local control variables
	HTuple  hv_UsePretrainedModel;
	HTuple  hv_DLDeviceHandles, hv_DLDevice, hv_ExampleInternals;
	HTuple  hv_ExampleDataDir, hv_PreprocessParamFileName;
	HTuple  hv_RetrainedModelFileName, hv_DataDirectory;
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
		replace(m_pa.paramFile.begin(), m_pa.paramFile.end(), '\\', '/');
		hv_PreprocessParamFileName = (HTuple)(m_pa.paramFile.c_str());
		//Path of the retrained segmentation model.
		replace(m_pa.modelFile.begin(), m_pa.modelFile.end(), '\\', '/');
		hv_RetrainedModelFileName = (HTuple)(m_pa.modelFile.c_str());
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
//	GetDlModelParam(hv_DLModelHandle, "class_names", &hv_ClassNames);
//	GetDlModelParam(hv_DLModelHandle, "class_ids", &hv_ClassIDs);

	for (int i = 0; i < m_pa.vctClassNames.size(); i++)
	{
		hv_ClassNames[i] = (HTuple)(m_pa.vctClassNames[i].c_str());
		hv_ClassIDs[i] = i;
	}

	//********************
	//**   Inference   ***
	//********************
	//
	//Check if all necessary files exist.
/*	check_data_availability(hv_ExampleDataDir, hv_PreprocessParamFileName, hv_RetrainedModelFileName,
		hv_UsePretrainedModel);*/
		//
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
	//Set the batch size.
	SetDlModelParam(hv_DLModelHandle, "batch_size", 1);
	//
	//Initialize the model for inference.
	SetDlModelParam(hv_DLModelHandle, "device", hv_DLDevice);
	//
	//Get the parameters used for preprocessing.
	ReadDict(hv_PreprocessParamFileName, HTuple(), HTuple(), &hv_DLPreprocessParam);

	GetDlModelParam(hv_DLModelHandle, "image_height", &hv_H);
	GetDlModelParam(hv_DLModelHandle, "image_width", &hv_W);

}
bool checkPatch::EditModel(cv::Mat& src)
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

bool checkPatch::checkLocateCheck(cv::Mat& src, vector<S_TpltData*>& vctData, CTpltParam& Pa)
{
	if (m_pa.getPic)//截图
	{
		timeb t;
		ftime(&t);//获取毫秒
		char sz[64];
		sprintf(sz, "\\%d%d.bmp", t.time,t.millitm);
		imwrite(m_pa.szPicPath+sz,src);
		vctData[0]->m_checkRet = true;
		return true;
	}
	//S_DllParam& halPa = Pa.m_dllPa;
	try
	{
		HObject  ho_ImageBatch, ho_Image, ho_SegmentationImage;
		ho_ImageBatch = Mat2HObject(src);
		HTuple  hv_DLSampleBatch, hv_DLResultBatch;
		HTuple  hv_Areas, hv_ClassIndex, hv_Area;
		HTuple  hv_Row, hv_Column, hv_ConnectIndex;
		
		// Local iconic variables
		HObject  ho_ClassRegions, ho_ClassRegion, ho_ConnectedRegions;
		HObject  ho_CurrentRegion;

		//Generate the DLSampleBatch.
		gen_dl_samples_from_images(ho_ImageBatch, &hv_DLSampleBatch);
		//
		//Preprocess the DLSampleBatch.
		preprocess_dl_samples(hv_DLSampleBatch, hv_DLPreprocessParam);
		//
		//Apply the DL model on the DLSampleBatch.
		ApplyDlModel(hv_DLModelHandle, hv_DLSampleBatch, HTuple(),&hv_DLResultBatch);
		//
		//Postprocessing and visualization.
		int objN = 0;
		//Get image.
//		GetDictObject(&ho_Image, HTuple(hv_DLSampleBatch[0]), "image");
//		GetImageSize(ho_Image, &hv_H, &hv_W);
		double rw = src.cols/hv_W.D();//图像缩小了 要还原坐标 求坐标缩放系数
		double rh = src.rows/hv_H.D();

		//获取置信度
	//	HObject ho_confidence;
	//	GetDictObject(&ho_confidence, HTuple(hv_DLResultBatch[0]), "segmentation_confidence");
		//Get result image.
		GetDictObject(&ho_SegmentationImage, HTuple(hv_DLResultBatch[0]),"segmentation_image");
		//
		//Postprocessing: Get segmented regions for each class.
		Threshold(ho_SegmentationImage, &ho_ClassRegions, hv_ClassIDs, hv_ClassIDs);
		//

		//Separate any components of the class regions
		//and display result regions as well as their area.
		//
		//Get area of class regions.
		RegionFeatures(ho_ClassRegions, "area", &hv_Areas);
		//
		//Display whether the pill is OK, or not.
		bool bret = true;
		
		//
		//Here, we do not display the first class, since it is the class 'good'也可以设置类名进行过滤来定位
		//and we only want to display the defect regions.
		if (0 == m_pa.mytype)//定位 有缺损目标
		{
			if (0 != (int(((hv_Areas.TupleSum()) - HTuple(hv_Areas[0])) > 0)))
			{
				//hv_Text = "NOK";
				bret = false;
			}
		}
		HTuple end_val190 = (hv_Areas.TupleLength()) - 1;
		HTuple step_val190 = 1;
		HTuple hv_left, hv_top, hv_right, hv_bottom;
		for (hv_ClassIndex = 1; hv_ClassIndex.Continue(end_val190, step_val190); hv_ClassIndex += step_val190)
		{
			if (0 != (int(HTuple(hv_Areas[hv_ClassIndex]) > 0)))//这里可以设置面积的大小范围 进行过滤
			{
				SelectObj(ho_ClassRegions, &ho_ClassRegion, hv_ClassIndex + 1);
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
						vctData[a]->m_szRet = hv_ClassNames[hv_ClassIndex];
						vctData[a]->m_roiRect.center.x = hv_Column[hv_ConnectIndex]*rw;
						vctData[a]->m_roiRect.center.y = hv_Row[hv_ConnectIndex]*rh;
						vctData[a]->m_roiRect.size = cv::Size(pt2.x-pt1.x, pt2.y-pt1.y);
						cv::Rect rt = vctData[a]->m_roiRect.boundingRect();
						rt &= cv::Rect(0, 0, src.cols, src.rows);
						vctData[a]->m_vctPic[0] = cv::Mat(src, rt).clone();
						vctData[a]->m_checkRet = false;
					}
				}
			}
		}
		//第一个类放最后 肯定是ok的类
		if (0 < m_pa.mytype)//检测 不不需要ok的类
			return bret;
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
					objN++;
					if (vctData.size() < objN)
					{
						vctData.push_back(new S_TpltData());
					}
					int a = objN - 1;
					vctData[a]->m_szRet = hv_ClassNames[0];
					vctData[a]->m_roiRect.center.x = hv_Column[hv_ConnectIndex] * rw;
					vctData[a]->m_roiRect.center.y = hv_Row[hv_ConnectIndex] * rh;
					vctData[a]->m_roiRect.size = cv::Size(pt2.x - pt1.x, pt2.y - pt1.y);
					cv::Rect rt = vctData[a]->m_roiRect.boundingRect();
					rt &= cv::Rect(0, 0, src.cols, src.rows);
					vctData[a]->m_vctPic[0] = cv::Mat(src, rt).clone();
					vctData[a]->m_checkRet = bret;
				}
			}
		}

		return bret;
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


bool checkPatch::drawResult(cv::Mat& src, cv::RotatedRect& roiRect, CTpltParam& pa)
{
	return true;
}
