#ifdef WIN32
#include <Winsock2.h>
#endif
#include "device.h"




CDllPicTreat::CDllPicTreat()
{
	m_bStop = false;
	m_runing = false;
	m_dll=NULL;
	open();
}

CDllPicTreat::~CDllPicTreat(void)
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
int CDllPicTreat::svc()
{
	int quLen;
	queue<S_Msg *> quMsg;
	S_Msg* pMsg = NULL;
	m_runing = true;
	while (true)
	{
		sendHeartBeat();
		if (m_bStop)
		{
			break;
		}
		if (NULL==m_dll)
		{
			mysleep(50);
			continue;
		}
		//从自己的消息队列里面获取100条消息用来解析
		quLen = recvMsg(quMsg);
		if (0 == quLen)
		{
			mysleep(50);
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
			if (!treat(pMsg))
			{

			}

			m_trans.pushMsg(pMsg);

			quMsg.pop();
			--quLen;
		}
	}

	m_runing = false;
	return 0;
}

bool CDllPicTreat::treat(S_Msg* pMsg)
{
	if (NULL==m_dll)
		return false;

	int ret = -1;	
	for (int i=0;i<2;i++)
	{
		pMsg->m_procid = i;
		ret =  m_dll->procCheck(pMsg );
		if (1 != ret)
		{
			pMsg->m_nc_ok = 2;
			return false;
		}
		else
		{
			pMsg->m_nc_ok = 3;
		}
		if (i<2)
			pMsg->vm[0] = pMsg->raw.clone();//必须克隆 否则出现定位框到下一个检测中
	}
	return true;
}

CDevice::CDevice()
{
}

CDevice::~CDevice()
{
}

bool CDevice::ini()
{
	return false;
}

