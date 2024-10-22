#ifndef __CHECKPATCH_H__
#define __CHECKPATCH_H__
/*
����������

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
	//ģ���ļ�
	string modelFile;
	//�����ļ�
	string paramFile;
	float minConfidence ;
	float maxOverlap ;
	float maxOverlapClassAgnostic;
	vector<string> vctClassNames;//������id
	HTuple  hv_ClassIDs, hv_ClassNames, hv_DLModelHandle;
	HTuple  hv_DLPreprocessParam;
	HTuple hv_W, hv_H;//ģ��ͼ��Ĵ�С
	bool m_iniSucc;//��ʼ���ɹ�
	string m_procName;
	int m_dlType;//0 Ŀ���⣬1����ָ�
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
	string m_cfgDir;//�����ļ�Ŀ¼
	vector<S_CheckParam> m_vctDLPa;
	static CDLPDlg S_dlg;
	int m_curId;//��ǰ�Ƴ�id
	void iniMode();
};

#endif