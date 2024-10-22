#ifndef __CHECKPATCH_H__
#define __CHECKPATCH_H__
/*
����������

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

	//ģ���ļ�
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
	//����ng�ı�ʶ
	bool drawResult(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa);

public:
	string m_cfgDir;//�����ļ�Ŀ¼
	S_CheckParam m_pa;
	CDLPDlg S_dlg;

	void iniMode();
	bool get_dl_data(HTuple hv_DLSample, HTuple hv_DLResult, HTuple hv_DLDatasetInfo,HTuple* hv_WindowHandle);
	void Train();
	bool predict(HObject& ho_Image);
	HTuple hv_ExampleSpecificPreprocessing;//���ݼ��ض���Ԥ����
	HTuple hv_use_ok_samples;//��ʹ��ok����ѵ��
	HTuple hv_InferenceClassificationThreshold, hv_InferenceSegmentationThreshold;//������ֵ��ģ��ѵ����õ���
	HTuple hv_DLModelHandle;//ģ�;��
	HTuple hv_OutputDir;//��������Ԥ�����ı���·��
	HTuple hv_ImageWidth, hv_ImageHeight;//���ź�Ĵ�С��32�ı�����
	HTuple hv_Complexity;//���Ӷȣ�Խ��׼ȷ��Խ�ߣ�ѵ��Խ��ʱ
	//����ѵ����ֹ�����������ʡ�������������һ����ֹ
	HTuple hv_ErrorThreshold, hv_MaxNumEpochs, hv_DomainRatio, hv_RegularizationNoise;
	HTuple hv_StandardDeviationFactor;//��׼�����ӣ����ȱ�ݺ�С���Ƽ��ϴ�ֵ��

	HObject ho_AnomalyRegion;//�쳣�ľ���λ��
};

#endif