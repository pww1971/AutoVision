#ifndef __CHECKPATCH_H__
#define __CHECKPATCH_H__
/*
1、模板做lbp，计算灰度直方图。
2、调整高低阈值划分直方图为三个部分。保存模板的高低阈值和三部分的值。rgb
3、对整图lbp，以n*n的窗体滑窗，统计直方图，按高低阈值计算出三个部分的值rgb，和模板的rgb的差。
*/

#include "pwwVision.h"
class S_CheckParam 
{
public:
	S_CheckParam();
	~S_CheckParam();
	void readMode(string file);
	void writeMode(string file);
	int type;//0 logpolar 1 linePolar
	double m;//
};

class checkPatch
{
public:
	checkPatch();
	~checkPatch();
	//图片的处理	
	bool treatPicture(CTpltParam& pa, vector<S_TpltData*>& vctData);
	bool drawResult(cv::Mat& src, cv::RotatedRect& roiRect, CTpltParam& pa);
	void readMode(string file);
	void writeMode(string file);

public:
	S_CheckParam m_myParam;
private:

};

#endif
