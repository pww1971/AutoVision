#ifndef __DEVICE_H__
#define __DEVICE_H__
/*
�ⲿ�˶�ģ��
�Զ���ͼ�����㷨 
*/
#include "automove.h"

/*
���ݴ���� �����������߳��а�ȫ����
*/
#define MAX_MSG_ONE_RECV 1024

class CDllPicTreat:public CtransMsg
{
public:
	//�²㴦����߳�ָ�봫��pBuzInterface 
	CDllPicTreat();
	virtual ~CDllPicTreat(void);

protected:
	// -------------------------------------------------------------------
	/// @����:  svc --- �̴߳�����
	// -------------------------------------------------------------------
	virtual int svc();
private:
	bool treat(S_Msg *pMsg);			//ͼ����
	
public:
	bool getData(S_Msg* pMsg);
	bool m_runing;						//�߳�����״̬
	CGetMsg m_trans;		//ͼ������ɺ���ṩ
	CMoveBase* m_mvb;
	std::vector<cv::Point2f> m_vctNGPoint;//������ngλ��
};

class CDevice 
{
public:
	CDevice();
	~CDevice();

	void releaseArrRet();
public:
	bool ini();
	std::deque<CoordinatePosXYZ*> m_deqRet;//��̨ͳ�� �����˶��Ķ�μ������
};

#endif