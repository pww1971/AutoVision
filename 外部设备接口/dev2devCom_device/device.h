#ifndef __DEVICE_H__
#define __DEVICE_H__
/*
外部运动模块
自定义图像处理算法 
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
	virtual bool connectionAnalyse(TcpMsg* pTcpMsg);//只接收到注册的消息类型和模块类型
		//从本层消息队列中取出100条消息进行处理
	u_int getRet(string& szRet);

public:
	bool m_Sync;//同步阻塞

protected:

protected:
	queue<string> m_quMsg;					//下层传过来的消息队列
	CResLock m_Lock;

};


#endif