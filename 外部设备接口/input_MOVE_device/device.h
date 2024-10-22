#ifndef __DEVICE_H__
#define __DEVICE_H__
/*
�ⲿ�˶�ģ��
*/
#include "automove.h"
/*
���ݴ���� �����������߳��а�ȫ����
*/
#define MAX_MSG_ONE_RECV 1024


class CDevice 
{
public:
	class CpicTreat:public CtransMsg
	{
	public:
		//�²㴦����߳�ָ�봫��pBuzInterface 
		CpicTreat(CtransMsg* pBuzInterface);
		virtual ~CpicTreat(void);

	protected:
		// -------------------------------------------------------------------
		/// @����:  svc --- �̴߳�����
		// -------------------------------------------------------------------
		virtual int svc();
	private:
		bool treat(S_Msg *pMsg);			//ͼ����

	public:
		CProcDll* m_dll;
		bool m_runing;						//�߳�����״̬
		bool m_bLoad;//��������
		bool m_bcanLoad;//���Լ���
		bool m_bEmptyData;
	};
	CpicTreat* m_picTreat;
	CGetMsg m_trans;		//ͼ������ɺ���ṩ
	int m_msgNum;
public:
	CDevice();
	~CDevice();

	//ͼƬ�Ĵ���	
	bool treatPicture();
	std::vector<cv::Mat> m_vctm;
private:
	bool ini();
};

#endif