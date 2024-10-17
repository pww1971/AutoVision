#ifndef __CHECKPATCH_H__
#define __CHECKPATCH_H__
/*
1、模板做lbp，计算灰度直方图。
2、调整高低阈值划分直方图为三个部分。保存模板的高低阈值和三部分的值。rgb
3、对整图lbp，以n*n的窗体滑窗，统计直方图，按高低阈值计算出三个部分的值rgb，和模板的rgb的差。
*/

#include "pwwVision.h"
class S_CheckParam :public mouseKeyCtr
{
public:
	S_CheckParam();
	~S_CheckParam();
	virtual bool design(bool add = false, int id = -1);
	void readMode(string file);
	void writeMode(string file);

	void* m_pCheck;
};
class checkPatch1 : public ClineScan
{
public:
	checkPatch1();
	~checkPatch1();
	void rebuildContour(vector<vector<cv::Point> >& contours);//输出纯净轮廓
	double match(vector<vector<cv::Point> >& contours);//输入模板纯净轮廓

public:
	S_LineRegionData m_LRD;

};
class checkPatch2 : public CpwwRegionConnect
{
public:
	checkPatch2();
	~checkPatch2();

	//纵向pww处理 利用连接和不连接扫描消除纵向毛刺 这步要先做这样后期的横向得到的mpicF和图像就是对应的
	bool verticalPwwTreat(cv::Mat& src, CTpltParam& pa, bool bDraw = false);
	bool pwwTreat(cv::Mat& src);
	void cmpMask(cv::Mat& src, cv::Mat& pwwMask, CTpltParam& pa, bool bDraw);
	void rebuildContour(vector<vector<cv::Point> >& contours);//输出纯净轮廓
	double match(vector<vector<cv::Point> >& contours);//输入模板纯净轮廓

public:
	CPicFeater m_PicF;
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
	double getdis(cv::Mat& src);
	void draw(cv::Mat& src);
public:
	double m_f;		// 焦距
	double m_r;   // 被测物体直径
	S_CheckParam m_myParam;
	int m_pixels;//0 就是求轮廓匹配 否则是单目测距
	vector<vector<cv::Point> > m_contours;//模板轮廓
private:
	checkPatch1 m_ck1;
	checkPatch2 m_ck2;
};

#endif
