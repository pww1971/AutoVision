#ifndef __CHECKPATCH_H__
#define __CHECKPATCH_H__
/*
给摄像头排线检测，定制的dll检测  模板设计派生的方式
计算旋转角度
计算每个区域和其他区域的角度 轮询 找到角度一致距离相符的  形成排线 计算两个排线的角度作为旋转角度
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
	//图片的处理	
	bool treatPicture(CTpltParam& pa, vector<S_TpltData*>& vctData, int curid);
	bool drawResult(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa);

	void readMode(string file);
	void writeMode(string file);

private:

public:
	static CGuiContrlDlg m_dlg;

};

#endif