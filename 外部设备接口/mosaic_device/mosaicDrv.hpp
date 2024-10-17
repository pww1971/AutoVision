#ifndef __MOSAICDRV_H__
#define __MOSAICDRV_H__

#include "pch.h"
#include "automove.h"
#include "halconLocal.h"
#define mosaitcView "win_mosaic"
//图像拼接
class mosaicDrv
{
public:
	mosaicDrv();
	~mosaicDrv();
	//左或上src1拼接右或下src2 仅限于左右和上下拼接 一旦定位点交错 右拼左 下拼上 就返回0  这样保证拼接不会错 宁可错 不能误判
	virtual int mosaicImage(cv::Mat src1, cv::Mat src2, cv::Mat& dst/*,int lToR=1*/);
	int mosaicImage2(cv::Mat src1, cv::Mat src2, cv::Mat& dst);
	bool EditModel();
	void showAllImage();

public:
	string m_cfgDir;//配置文件目录
	CProcDll* m_Cam;//相机
	std::vector<cv::Mat> m_vctImage;
	map<int, int> m_camPair;//相机图片拼接的匹配对
};

class mosaicHarris :public mosaicDrv
{
public:
	mosaicHarris();
	~mosaicHarris();
	virtual int mosaicImage( cv::Mat& m);
	virtual int mosaicImage2(cv::Mat& m);
	int ini();
	void clearMode();
	void readMode(string file = "");
	void writeMode(string file = "");
	HTuple  hv_Alpha, hv_Threshold, hv_SigmaGrad, hv_SigmaSmooth;
	HTuple  hv_MaskSize, hv_RowTolerance, hv_ColTolerance, hv_Rotation;
	HTuple  hv_MatchThreshold, hv_DistanceThreshold;
	HTuple  hv_StartImage, hv_StackingOrder;
	HTuple  hv_From, hv_To;
	std::vector<string> m_PossibleTransformations;
	string m_szPtf;
	int m_Vertically;//垂直拼接位置
};

#endif