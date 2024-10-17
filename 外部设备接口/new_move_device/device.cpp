#ifdef WIN32
#include <Winsock2.h>
#endif
#include "device.h"



CDllPicTreat::CDllPicTreat()
{
	m_bStop = false;
	m_runing = false;
	m_mvb=NULL;
	open();
}

CDllPicTreat::~CDllPicTreat(void)
{
	m_bStop = true;

	for(int t=0;t<50000;t++)
	{
		if(m_runing)
			mysleep(10);
		else
			break;
	}
	stop();
	m_mvb = NULL;
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
			quLen = recvMsg(quMsg);
			while (0 != quLen)
			{
				pMsg = quMsg.front();
				delete pMsg;
				quMsg.pop();
				--quLen;
			}
			break;
		}
		if (NULL==m_mvb)
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

		//	m_mvb->m_getRet->pushMsg(pMsg);

			quMsg.pop();
			--quLen;
		}
	}

	m_runing = false;
	return 0;
}

bool CDllPicTreat::treat(S_Msg* pMsg)
{
	if (NULL==m_mvb)
		return false;
	CProcDll* totdll=m_mvb->m_pCheck;
	bool bR=true;
	int ret =  totdll->procCheck(pMsg );
	return bR;
}

bool CDllPicTreat::getData(S_Msg* pMsg)
{
	if (NULL==m_mvb)
		return false;
	pMsg = m_trans.getData();
	if (NULL != pMsg)
	{
		CProcDll* totdll=m_mvb->m_pCheck;
		CArrayMove& pmtc = m_mvb->m_mtc;
		totdll->viewWin(pMsg->m_procid,pMsg->vm);
		if (pMsg->m_nc_ok == 3)//ok
		{
			pmtc.viewOK(*pMsg);
		}
		else//ng
		{
			pmtc.viewNG(*pMsg);
		}

		if (NULL != m_mvb->m_tbReport)
			m_mvb->m_tbReport->pushMsg(pMsg);
		//if (m_mvb->m_writeDir && m_mvb->m_writeDir->m_saveOrNot)
		//	m_mvb->m_writeDir->pushNG(pMsg->m,pMsg);
		return true;
	}
	return false;
}

CDevice::CDevice()
{
}
CDevice::~CDevice()
{
	releaseArrRet();
}
void CDevice::releaseArrRet()
{
	std::deque<CoordinatePosXYZ*>::iterator itor = m_deqRet.begin();
	while (itor != m_deqRet.end())
	{
		delete* itor;
		itor++;
	}
	m_deqRet.clear();
}

bool CDevice::ini()
{
	return false;
}

