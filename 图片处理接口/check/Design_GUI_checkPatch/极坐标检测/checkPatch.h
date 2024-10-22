#ifndef __CHECKPATCH_H__
#define __CHECKPATCH_H__
/*
������ͷ���߼�⣬���Ƶ�dll���  ģ����������ķ�ʽ
������ת�Ƕ�
����ÿ���������������ĽǶ� ��ѯ �ҵ��Ƕ�һ�¾��������  �γ����� �����������ߵĽǶ���Ϊ��ת�Ƕ�
*/

#include "pwwVision.h"
#include "GuiContrlDlg.h"
struct S_CheckParam:CTpltParam
{
	S_CheckParam()
	{
	}
};
class checkPatch :public PolarCheck
{
public:
	checkPatch();
	~checkPatch();
	virtual bool ini(cv::Mat& tpltImg,cv::Mat& mask,CTpltParam& pa);
	virtual bool PolarMatch(cv::Mat& src);
	//ͼƬ�Ĵ���	
	bool treatPicture(CTpltParam& pa, vector<S_TpltData*>& vctData, int curid);
	bool drawResult(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa);

	void readMode(string file);
	void writeMode(string file);

private:

public:
	static CGuiContrlDlg m_dlg;

};

#endif