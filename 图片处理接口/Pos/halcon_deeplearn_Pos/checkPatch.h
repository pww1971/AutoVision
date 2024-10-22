#ifndef __CHECKPATCH_H__
#define __CHECKPATCH_H__
/*
����������

*/
#include "pwwVision.h"
#include "halcon_deeplearn_Pos.h"
#include "CDLPDlg.h"
#include <HalconCpp.h>
using namespace HalconCpp;
HImage Mat2HObject(cv::Mat& image);
cv::Mat HObject2Mat(HImage Hobj);

struct S_CheckParam 
{
	S_CheckParam() 
	{
		getPic = mytype = 0;
	};

	//ģ���ļ�
	string modelFile;
	//�����ļ�
	string paramFile;

	vector<string> vctClassNames;//������id
	int mytype;//��λ���� 0��λ
	int getPic;//��ͼ
	string szPicPath;//��ͼ�����ַ
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
	static CDLPDlg S_dlg;
	HTuple  hv_ClassIDs, hv_ClassNames, hv_DLModelHandle;
	HTuple  hv_DLPreprocessParam;
	HTuple hv_W,hv_H;//ģ��ͼ��Ĵ�С

	void iniMode();
};

#endif