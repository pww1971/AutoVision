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
	void delMap();
	//图片的处理	
	bool treatPicture(CTpltParam& pa, vector <S_TpltData*>& tpltData);
	//画出ng的标识
	bool drawResult(cv::Mat& src, cv::RotatedRect& roiRect, CTpltParam& pa);
	void writeFile(CTpltParam& pa);
private:
	Ptr<wechat_qrcode::WeChatQRCode> detector;
public:
	string m_szRule;//正则化表达式
	static safePwwStrMap<string, string> m_qrDiskMp;//硬盘不能共享 必须是静态的
	int m_mapSign;
	int m_iHashPrefix;
	int m_BN;
	int m_wBufLen;
};

#endif