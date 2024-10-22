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
	//ͼƬ�Ĵ���	
	bool treatPicture(CTpltParam& pa, vector <S_TpltData*>& tpltData);
	//����ng�ı�ʶ
	bool drawResult(cv::Mat& src, cv::RotatedRect& roiRect, CTpltParam& pa);
	void writeFile(CTpltParam& pa);
private:
	Ptr<wechat_qrcode::WeChatQRCode> detector;
public:
	string m_szRule;//���򻯱��ʽ
	static safePwwStrMap<string, string> m_qrDiskMp;//Ӳ�̲��ܹ��� �����Ǿ�̬��
	int m_mapSign;
	int m_iHashPrefix;
	int m_BN;
	int m_wBufLen;
};

#endif