#ifndef __CHECKPATCH_H__
#define __CHECKPATCH_H__
/*
功能描述：

*/
#include "pwwVision.h"
#include "halcon_anomalies_checkPatch.h"
#include "CDLPDlg.h"
#include <HalconCpp.h>
using namespace HalconCpp;
HImage Mat2HObject(cv::Mat& image);
cv::Mat HObject2Mat(HImage Hobj);

struct S_CheckParam 
{
	S_CheckParam() 
	{
	};

	//模型文件
	string modelFile;
	
};

class checkPatch 
{
public:
	checkPatch();
	~checkPatch();
	void clearMode();

	void readMode(string file="");
	void writeMode( string file="");

	bool EditModel(cv::Mat& src);

	bool checkLocateCheck(cv::Mat& src, vector<S_TpltData*>& vctData,CTpltParam& Pa);
	//画出ng的标识
	bool drawResult(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa);

public:
	string m_cfgDir;//配置文件目录
	S_CheckParam m_pa;
	CDLPDlg S_dlg;

	void iniMode();
	bool get_dl_data(HTuple hv_DLSample, HTuple hv_DLResult, HTuple hv_DLDatasetInfo,HTuple* hv_WindowHandle);
	void Train();
	bool predict(HObject& ho_Image);
	HTuple hv_ExampleSpecificPreprocessing;//数据集特定的预处理
	HTuple hv_use_ok_samples;//仅使用ok样本训练
	HTuple hv_InferenceClassificationThreshold, hv_InferenceSegmentationThreshold;//设置阈值（模型训练后得到）
	HTuple hv_DLModelHandle;//模型句柄
	HTuple hv_OutputDir;//所有样本预处理后的保存路径
	HTuple hv_ImageWidth, hv_ImageHeight;//缩放后的大小（32的倍数）
	HTuple hv_Complexity;//复杂度，越大准确率越高，训练越耗时
	//设置训练终止条件，错误率、次数，满足其一则终止
	HTuple hv_ErrorThreshold, hv_MaxNumEpochs, hv_DomainRatio, hv_RegularizationNoise;
	HTuple hv_StandardDeviationFactor;//标准差因子（如果缺陷很小，推荐较大值）

	HObject ho_AnomalyRegion;//异常的具体位置
};

#endif