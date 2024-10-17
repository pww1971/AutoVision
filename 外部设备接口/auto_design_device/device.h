#ifndef __DEVICE_H__
#define __DEVICE_H__
/*
外部运动模块
自定义图像处理算法 
功能：
1、针对每一个目标都自动建模，形成独特的时间识别标记。并可以遍历检测。
2、对每一个目标都有一个二维码字符进行标记和根据二维码字符进行检测识别。
*/
#include "automove.h"
#include <opencv2/wechat_qrcode.hpp>

class CDevice 
{
public:
	CDevice();
	~CDevice();
	bool searchObj(std::vector<cv::Mat>& vm);//遍历主目录下各个子目录制程 检测
	bool autoDesign(std::vector<cv::Mat>& vm);//根据时间在主目录下形成子目录，模板检测新图像，然后重新保存制程
	void release();

	bool ini(string modelcfg,string newModelDir);//加载modelcfg模板制程，设置主目录
	bool wechatIni();
	bool checkObj(std::vector<cv::Mat>& vm);//微信二维码检测，根据字符找到对应文件夹，加载该制程 检测
	bool wechatDesign(std::vector<cv::Mat>& vm);//微信二维码定位，根据二维码字符形成子目录，模板检测新图像，然后重新保存制程。
	mouseKeyCtr m_mkc;
	string m_mainDir;//制程主目录
	CTemplateProc m_bkProc;
	Ptr<wechat_qrcode::WeChatQRCode> m_wecharDecode;
};

#endif