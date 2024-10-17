#ifndef __DEVICE_H__
#define __DEVICE_H__
/*
外部运动模块
*/
#include "automove.h"

/*
数据传输层 负责数据在线程中安全传递
*/
#define MAX_MSG_ONE_RECV 1024

class CDllPicTreat:public CtransMsg
{
public:
	//下层处理的线程指针传递pBuzInterface 
	CDllPicTreat();
	virtual ~CDllPicTreat(void);

protected:
	// -------------------------------------------------------------------
	/// @描述:  svc --- 线程处理函数
	// -------------------------------------------------------------------
	virtual int svc();
private:
	bool treat(S_Msg *pMsg);			//图像处理

public:
	CProcDll* m_dll;
	bool m_runing;						//线程运行状态
	CGetMsg m_trans;		//图像处理完成后的提供
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