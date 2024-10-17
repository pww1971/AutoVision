#ifndef __CHECKPATCH_H__
#define __CHECKPATCH_H__
/*
功能描述：

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

	//模型文件
	string modelFile;
	//参数文件
	string paramFile;

	vector<string> vctClassNames;//类名和id
	int mytype;//定位或检测 0定位
	int getPic;//截图
	string szPicPath;//截图保存地址
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
	static CDLPDlg S_dlg;
	HTuple  hv_ClassIDs, hv_ClassNames, hv_DLModelHandle;
	HTuple  hv_DLPreprocessParam;
	HTuple hv_W,hv_H;//模板图像的大小

	void iniMode();
};

#endif