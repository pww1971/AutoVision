#ifndef __DEVICE_H__
#define __DEVICE_H__
/*
�ⲿ�˶�ģ��
�Զ���ͼ�����㷨 
*/
#include "automove.h"

using namespace std;

/*

*/
class S_SocketDevice :public CSubscribeClient
{
public:
	S_SocketDevice();
	~S_SocketDevice();
	virtual bool connectionAnalyse(TcpMsg* pTcpMsg);//ֻ���յ�ע�����Ϣ���ͺ�ģ������
		//�ӱ�����Ϣ������ȡ��100����Ϣ���д���
	u_int getRet(string& szRet);

public:
	bool m_Sync;//ͬ������

protected:

protected:
	queue<string> m_quMsg;					//�²㴫��������Ϣ����
	CResLock m_Lock;

};


#endif