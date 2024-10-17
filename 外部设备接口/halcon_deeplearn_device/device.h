#ifndef __CHECKPATCH_H__
#define __CHECKPATCH_H__
/*
功能描述：

*/
#include "pwwVision.h"
#include "halcon_deeplearn_device.h"
#include "CDLPDlg.h"
#include <HalconCpp.h>
using namespace HalconCpp;
HImage Mat2HObject(cv::Mat& image);
cv::Mat HObject2Mat(HImage Hobj);

struct S_CheckParam
{
	S_CheckParam() 
	{
		m_dlType = 0;
		m_iniSucc = false;
		minConfidence = 0.6;
		maxOverlap = 0.2;
		maxOverlapClassAgnostic = 0.7;
	};
	void readMode(CConfig& cfg);
	void writeMode(CConfig& cfg);
	void iniMode();
	//模型文件
	string modelFile;
	//参数文件
	string paramFile;
	float minConfidence ;
	float maxOverlap ;
	float maxOverlapClassAgnostic;
	vector<string> vctClassNames;//类名和id
	HTuple  hv_ClassIDs, hv_ClassNames, hv_DLModelHandle;
	HTuple  hv_DLPreprocessParam;
	HTuple hv_W, hv_H;//模板图像的大小
	bool m_iniSucc;//初始化成功
	string m_procName;
	int m_dlType;//0 目标检测，1语义分割
};

class CDevice 
{
public:
	CDevice();
	~CDevice();
	void clearMode();

	void readMode(string file="");
	void writeMode( string file="");

	bool EditModel(cv::Mat& src);

	bool checkLocateCheck(S_Msg& msg, int modeId);

public:
	string m_cfgDir;//配置文件目录
	vector<S_CheckParam> m_vctDLPa;
	static CDLPDlg S_dlg;
	int m_curId;//当前制程id
	void iniMode();
};

#endif