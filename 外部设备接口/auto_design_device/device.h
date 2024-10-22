#ifndef __DEVICE_H__
#define __DEVICE_H__
/*
�ⲿ�˶�ģ��
�Զ���ͼ�����㷨 
���ܣ�
1�����ÿһ��Ŀ�궼�Զ���ģ���γɶ��ص�ʱ��ʶ���ǡ������Ա�����⡣
2����ÿһ��Ŀ�궼��һ����ά���ַ����б�Ǻ͸��ݶ�ά���ַ����м��ʶ��
*/
#include "automove.h"
#include <opencv2/wechat_qrcode.hpp>

class CDevice 
{
public:
	CDevice();
	~CDevice();
	bool searchObj(std::vector<cv::Mat>& vm);//������Ŀ¼�¸�����Ŀ¼�Ƴ� ���
	bool autoDesign(std::vector<cv::Mat>& vm);//����ʱ������Ŀ¼���γ���Ŀ¼��ģ������ͼ��Ȼ�����±����Ƴ�
	void release();

	bool ini(string modelcfg,string newModelDir);//����modelcfgģ���Ƴ̣�������Ŀ¼
	bool wechatIni();
	bool checkObj(std::vector<cv::Mat>& vm);//΢�Ŷ�ά���⣬�����ַ��ҵ���Ӧ�ļ��У����ظ��Ƴ� ���
	bool wechatDesign(std::vector<cv::Mat>& vm);//΢�Ŷ�ά�붨λ�����ݶ�ά���ַ��γ���Ŀ¼��ģ������ͼ��Ȼ�����±����Ƴ̡�
	mouseKeyCtr m_mkc;
	string m_mainDir;//�Ƴ���Ŀ¼
	CTemplateProc m_bkProc;
	Ptr<wechat_qrcode::WeChatQRCode> m_wecharDecode;
};

#endif