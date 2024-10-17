#ifndef __CHECKPATCH_H__
#define __CHECKPATCH_H__



#include "pwwVision.h"

class checkPatch
{
public:
	checkPatch();
	~checkPatch();
	void readMode(string file);
	void writeMode(string file);

	//图片的处理	
	bool treatPicture(CTpltParam& pa, vector <S_TpltData*>& tpltData, int curid);
	//画出ng的标识
	bool drawResult(cv::Mat& src, cv::RotatedRect& roiRect, CTpltParam& pa);
	void writeFile(CTpltParam& pa);
private:
	Ptr<wechat_qrcode::WeChatQRCode> detector;
		string m_szRule;//正则化表达式
};

#endif