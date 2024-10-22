#ifndef __CHECKPATCH_H__
#define __CHECKPATCH_H__
/*
1��ģ����lbp������Ҷ�ֱ��ͼ��
2�������ߵ���ֵ����ֱ��ͼΪ�������֡�����ģ��ĸߵ���ֵ�������ֵ�ֵ��rgb
3������ͼlbp����n*n�Ĵ��廬����ͳ��ֱ��ͼ�����ߵ���ֵ������������ֵ�ֵrgb����ģ���rgb�Ĳ
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
	void rebuildContour(vector<vector<cv::Point> >& contours);//�����������
	double match(vector<vector<cv::Point> >& contours);//����ģ�崿������

public:
	S_LineRegionData m_LRD;

};
class checkPatch2 : public CpwwRegionConnect
{
public:
	checkPatch2();
	~checkPatch2();

	//����pww���� �������ӺͲ�����ɨ����������ë�� �ⲽҪ�����������ڵĺ���õ���mpicF��ͼ����Ƕ�Ӧ��
	bool verticalPwwTreat(cv::Mat& src, CTpltParam& pa, bool bDraw = false);
	bool pwwTreat(cv::Mat& src);
	void cmpMask(cv::Mat& src, cv::Mat& pwwMask, CTpltParam& pa, bool bDraw);
	void rebuildContour(vector<vector<cv::Point> >& contours);//�����������
	double match(vector<vector<cv::Point> >& contours);//����ģ�崿������

public:
	CPicFeater m_PicF;
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
	double getdis(cv::Mat& src);
	void draw(cv::Mat& src);
public:
	double m_f;		// ����
	double m_r;   // ��������ֱ��
	S_CheckParam m_myParam;
	int m_pixels;//0 ����������ƥ�� �����ǵ�Ŀ���
	vector<vector<cv::Point> > m_contours;//ģ������
private:
	checkPatch1 m_ck1;
	checkPatch2 m_ck2;
};

#endif
