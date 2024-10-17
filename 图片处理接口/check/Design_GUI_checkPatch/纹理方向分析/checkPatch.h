#ifndef __CHECKPATCH_H__
#define __CHECKPATCH_H__
/*
1、模板做lbp，计算灰度直方图。
2、调整高低阈值划分直方图为三个部分。保存模板的高低阈值和三部分的值。rgb
3、对整图lbp，以n*n的窗体滑窗，统计直方图，按高低阈值计算出三个部分的值rgb，和模板的rgb的差。
*/

#include "GuiContrlDlg.h"

class S_CheckParam:public mouseKeyCtr
{
public:
	S_CheckParam();
	~S_CheckParam();
	void readParam(string file);
	void writeParam(string file);
	virtual bool design(bool add = false, int id = -1);

	int m_iHisType;//0,lbp直方图，1elbp直方图
	int m_N;//扫距大小
	int m_channel;//通道选择
	void* m_pCheck;
};
class checkPatch : CpwwRegionConnect
{
public:
	checkPatch();
	~checkPatch();
	S_CheckParam m_myParam;
	//图片的处理	
	bool treatPicture(CTpltParam& pa, vector<S_TpltData*>& vctData, int curid);
	bool drawResult(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa);
	//纵向pww处理 利用连接和不连接扫描消除纵向毛刺 这步要先做这样后期的横向得到的mpicF和图像就是对应的
	bool verticalPwwTreat(cv::Mat& src,	CTpltParam& pa,bool bDraw=false);
	bool pwwTreat(cv::Mat& src);
	//检测
	bool checkDug(cv::Mat&src);
	void readMode(string file);
	void writeMode(string file);
	//pww产生模板
	void genMode();
	//过滤点 对pww数据插值 过滤超过2个像素长的点
	bool filterPoint(cv::Point& pt1,cv::Point& pt2);
	//掩码 过滤点
	bool isMaskScope(cv::Point& pt);
private:
	void cmpMask(cv::Mat& src,cv::Mat& pwwMask,CTpltParam& pa,bool bDraw);
	CPicFeater m_PicF;

	
public:
	
	static CGuiContrlDlg m_dlg;
	static bool iniDlg();//非模式对话框
	string m_szRet;
};

#endif