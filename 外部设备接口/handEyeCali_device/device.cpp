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

#ifdef AUTODEVICE
	int ret = -1;
	int i=0;
	int len = g_procNum-1;
	for (;i<len;i++)
	{
		cv::Mat m2 = pMsg->pic.clone();
		pMsg->m_procid = i;
		ret = m_dll->procCheck(i,m2,&pMsg->ret);
		if (pMsg->ret.m_nc_ok == 3)//无料判断
		{
			pMsg->ret.z = 2;
			pMsg->ret.px+=m2.cols/2;
			pMsg->ret.py+=m2.rows/2;

			//pback->m_have_material = 2;
			break;
		}
	}
	cv::Mat m2 = pMsg->pic.clone();
	if (i==len)
	{
		//有料检测流程
		pMsg->m_procid = i;
		ret = g_totdll->procCheck(i,m2,&pMsg->ret);

		pMsg->ret.z = 1;
		pMsg->ret.px+=m2.cols/2;
		pMsg->ret.py+=m2.rows/2;

		//pback->m_have_material = 1;

	}


	int centx = pMsg->ret.px;
	int centy = pMsg->ret.py;

	cv::line(pMsg->pic,cv::Point(centx-30,centy),cv::Point(centx+30,centy),cv::Scalar(0,255,0),2);
	cv::line(pMsg->pic,cv::Point(centx,centy-30),cv::Point(centx,centy+30),cv::Scalar(0,255,0),2);
	cv::rectangle(pMsg->pic,cv::Rect(  centx - 850 ,
		centy - 850 ,
		850*2,
		850*2),cv::Scalar(0,255,0),3);
	char sz[128];
	sprintf(sz,"%d,%d",centx,centy);
	cv::putText(pMsg->pic, sz, cv::Point(25, 125), cv::FONT_HERSHEY_TRIPLEX, 4, cv::Scalar(0,255,0),2);
	cv::namedWindow("win_opencv",cv::WINDOW_NORMAL);
	cv::imshow("win_opencv",pMsg->pic);
	cv::waitKey(1);


	return ret == 1 ;

#else

	//cv::Mat m;
	//if (m_saveOrNot)//保存原始图片
	//{
	//	m = pMsg->m.clone();
	//}
	int ret = -1;
	for (int i=0;i<2;i++)
	{
		pMsg->m_procid = i;
		ret =  m_dll->procCheck(pMsg );
		if (1 != ret)
		{
			pMsg->m_nc_ok = 2;
			//if (m_saveOrNot)//保存错误图片
			//	{
			//		//原始的图片
			//		S_Msg* pMsg3 = new S_Msg();
			//		pMsg3->pic = m.clone();
			//		pMsg3->procid = 0;
			//		pMsg3->pan_id = pMsg->pan_id;
			//		pMsg3->pan_x = pMsg->pan_x;
			//		pMsg3->pan_y = pMsg->pan_y;
			//		pMsg3->m_check_id = pMsg->m_check_id;
			//		GDKT::CDateTime dt;
			//		pMsg3->str = dt.ToString("%Y-%m-%d-%H-%M-%S")+"_" + \
			//			GDKT::CStrProcess::itoa_radixN(pMsg->pan_id)  \
			//			+"_"+GDKT::CStrProcess::itoa_radixN(pMsg->pan_x) \
			//			+"_" + GDKT::CStrProcess::itoa_radixN(pMsg->pan_y) \
			//			+"_" + GDKT::CStrProcess::itoa_radixN(pMsg->m_check_id);
			//		m_writeDir.pushMsg(pMsg3);
			//		//保存错误图片
			//		S_Msg* pMsg2 = new S_Msg();
			//		pMsg2->pic=pMsg->m;
			//		pMsg2->procid = 1;
			//		pMsg2->pan_id = pMsg->pan_id;
			//		pMsg2->pan_x = pMsg->pan_x;
			//		pMsg2->pan_y = pMsg->pan_y;
			//		pMsg2->m_check_id = pMsg->m_check_id;
			//		pMsg2->str = pMsg3->str/* + "_" + pMsg->ret.str*/;
			//		m_writeDir.pushMsg(pMsg2);
			return false;
		}
		else
		{
			pMsg->m_nc_ok = 3;
		}
	}
	return true;
#endif
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

