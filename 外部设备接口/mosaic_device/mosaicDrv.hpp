#ifndef __MOSAICDRV_H__
#define __MOSAICDRV_H__

#include "pch.h"
#include "automove.h"
#include "halconLocal.h"
#define mosaitcView "win_mosaic"
//ͼ��ƴ��
class mosaicDrv
{
public:
	mosaicDrv();
	~mosaicDrv();
	//�����src1ƴ���һ���src2 ���������Һ�����ƴ�� һ����λ�㽻�� ��ƴ�� ��ƴ�� �ͷ���0  ������֤ƴ�Ӳ���� ���ɴ� ��������
	virtual int mosaicImage(cv::Mat src1, cv::Mat src2, cv::Mat& dst/*,int lToR=1*/);
	int mosaicImage2(cv::Mat src1, cv::Mat src2, cv::Mat& dst);
	bool EditModel();
	void showAllImage();

public:
	string m_cfgDir;//�����ļ�Ŀ¼
	CProcDll* m_Cam;//���
	std::vector<cv::Mat> m_vctImage;
	map<int, int> m_camPair;//���ͼƬƴ�ӵ�ƥ���
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
	int m_Vertically;//��ֱƴ��λ��
};

#endif