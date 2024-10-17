

#include "pch.h"
#include "device.h"

S_SocketDevice::S_SocketDevice()
{
	m_szDescription = "非标设备间通讯线程";//线程描述
	subscribe("CMVRet", "Measure");//注册CMV模块类型和消息类型
	subscribe("CMVRet", "AutoMeasure");//注册CMV模块类型和消息类型
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

bool S_SocketDevice::connectionAnalyse(TcpMsg* pTcpMsg)//只接收到注册的消息类型和模块类型
{
	if (CSubscribeClient::connectionAnalyse(pTcpMsg))//心跳消息
	{
		return true;
	}

	S_SubscribeMsg msg;
	msg.decodeMsg(pTcpMsg->buf);
	if (msg.szModelType == "CMVRet")
	{
		if (msg.szMsgType == "AutoMeasure" || msg.szMsgType == "Measure")//收到CMV的运算结果
		{
			m_Lock.lock();
			m_quMsg.push(msg.szMsgCont);
			m_Lock.unLock();
		}
	}
	else
	{
		string actionMsg = msg.szMsgCont;
		if ("阻塞测试"==actionMsg)
		{
			m_Sync = true;//收到返回信号 取消阻塞
		}
	}
	return true;
}
//提取cmv的结果
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