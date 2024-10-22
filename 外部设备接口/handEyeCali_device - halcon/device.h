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
	CProcDll* m_dll;
	bool m_runing;						//�߳�����״̬
	CGetMsg m_trans;		//ͼ������ɺ���ṩ
};

class CDevice 
{
public:
	CDevice();
	~CDevice();


private:
	bool ini();

};

#endif