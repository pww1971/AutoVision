

#include "pch.h"
#include "device.h"

S_SocketDevice::S_SocketDevice()
{
	m_szDescription = "�Ǳ��豸��ͨѶ�߳�";//�߳�����
	subscribe("CMVRet", "Measure");//ע��CMVģ�����ͺ���Ϣ����
	subscribe("CMVRet", "AutoMeasure");//ע��CMVģ�����ͺ���Ϣ����
}

S_SocketDevice::~S_SocketDevice()
{
	string quMsg;
	while (1)
	{
		if (0 == getRet(quMsg))
			break;
	}
}

bool S_SocketDevice::connectionAnalyse(TcpMsg* pTcpMsg)//ֻ���յ�ע�����Ϣ���ͺ�ģ������
{
	if (CSubscribeClient::connectionAnalyse(pTcpMsg))//������Ϣ
	{
		return true;
	}

	S_SubscribeMsg msg;
	msg.decodeMsg(pTcpMsg->buf);
	if (msg.szModelType == "CMVRet")
	{
		if (msg.szMsgType == "AutoMeasure" || msg.szMsgType == "Measure")//�յ�CMV��������
		{
			m_Lock.lock();
			m_quMsg.push(msg.szMsgCont);
			m_Lock.unLock();
		}
	}
	else
	{
		string actionMsg = msg.szMsgCont;
		if ("��������"==actionMsg)
		{
			m_Sync = true;//�յ������ź� ȡ������
		}
	}
	return true;
}
//��ȡcmv�Ľ��
u_int S_SocketDevice::getRet(string& szRet)
{
	u_int uiReceived;

	m_Lock.lock();
	uiReceived = m_quMsg.size();
	if (0 == uiReceived)
	{
		m_Lock.unLock();
		return 0;
	}

	szRet = m_quMsg.front();
	m_quMsg.pop();
	m_Lock.unLock();

	return uiReceived;
}