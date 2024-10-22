#ifndef __CHECKPATCH_H__
#define __CHECKPATCH_H__
/*
1��ģ����lbp������Ҷ�ֱ��ͼ��
2�������ߵ���ֵ����ֱ��ͼΪ�������֡�����ģ��ĸߵ���ֵ�������ֵ�ֵ��rgb
3������ͼlbp����n*n�Ĵ��廬����ͳ��ֱ��ͼ�����ߵ���ֵ������������ֵ�ֵrgb����ģ���rgb�Ĳ
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
	//ͼƬ�Ĵ���	
	bool treatPicture(CTpltParam& pa, vector<S_TpltData*>& vctData);
	bool drawResult(cv::Mat& src, cv::RotatedRect& roiRect, CTpltParam& pa);
	void readMode(string file);
	void writeMode(string file);

public:
	S_CheckParam m_myParam;
private:

};

#endif
