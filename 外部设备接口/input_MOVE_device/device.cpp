#ifdef WIN32
#include <Winsock2.h>
#endif
#include "device.h"



CDevice::CpicTreat::CpicTreat(CtransMsg* pBuzInterface)
{
	m_bStop = false;
	m_pBuzInterface = pBuzInterface;
	m_runing = false;
	m_dll=NULL;
}

CDevice::CpicTreat::~CpicTreat(void)
{
	m_bStop = true;

	for(int t=0;t<500;t++)
	{
		if(m_runing)
			mysleep(10);
		else
			break;
	}
	stop();
	m_dll = NULL;
}

//下层调用，传消息过来
int CDevice::CpicTreat::svc()
{
	int quLen;
	queue<S_Msg *> quMsg;
	S_Msg* pMsg = NULL;
	m_bLoad = true;
	m_runing = true;
	m_bEmptyData = false;
	while (true)
	{
		sendHeartBeat();
		if (m_bStop)
		{
			break;
		}
		if (m_bLoad)
		{
			m_bcanLoad = true;//通知接口可以加载了
			mysleep(5);
			continue;
		}

		//从自己的消息队列里面获取100条消息用来解析
		quLen = recvMsg(quMsg);
		if (0 == quLen)
		{
			m_bEmptyData = false;
			mysleep(5);
			continue;
		}
		while (0 != quLen)
		{
			pMsg = (S_Msg*)quMsg.front();
			if (m_bStop)
			{
				delete pMsg;
				quMsg.pop();
				--quLen;
				continue;
			}
			if (m_bEmptyData)
			{
				delete pMsg;
				quMsg.pop();
				--quLen;
				continue;
			}
			if (!treat(pMsg))
			{

			}


			m_pBuzInterface->pushMsg(pMsg);

			quMsg.pop();
			--quLen;
		}
	}

	m_runing = false;

	return 0;
}

bool CDevice::CpicTreat::treat(S_Msg* pMsg)
{
	if (NULL==m_dll)
		return false;

	int ret = -1;
	for (int i=0;i<2;i++)
	{
		pMsg->m_procid = i;
		ret =  m_dll->procCheck( pMsg );
		if (1 != ret)
		{
			pMsg->m_nc_ok = 2;
			return false;
		}
		else
		{
			pMsg->m_nc_ok = 3;
		}
	}
	return true;
}


CDevice::CDevice()
{
	m_picTreat = new CpicTreat(&m_trans);
	m_picTreat->open();
}

CDevice::~CDevice()
{
}


bool CDevice::ini()
{

	return false;
}


//图片的处理	一保存模板图片，二加载模板图片进行旋转角度后匹配
bool CDevice::treatPicture()
{
	
	return false;
}

