#ifndef __CHECKPATCH_H__
#define __CHECKPATCH_H__
/*
给摄像头排线检测，定制的dll检测  模板设计派生的方式
计算旋转角度
计算每个区域和其他区域的角度 轮询 找到角度一致距离相符的  形成排线 计算两个排线的角度作为旋转角度
*/

#include "templateDesign.h"
#include "GuiContrlDlg.h"
#include "halconLocal.h"
struct S_CheckParam
{
	S_CheckParam()
	{
		posx = 9;
		ret = 0.9;
	}
	int posx;
	float ret;
};
class checkPatch
{
public:
	checkPatch();
	~checkPatch();
	S_CheckParam m_myParam;
	//图片的处理	
	bool treatPicture(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa);
	bool drawResult(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa);
	void readMode(string file);
	void writeMode(string file);

public:
	static CGuiContrlDlg m_dlg;
	static bool iniDlg();//非模式对话框

};

#endif