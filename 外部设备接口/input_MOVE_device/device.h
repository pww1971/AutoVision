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


class CDevice 
{
public:
	class CpicTreat:public CtransMsg
	{
	public:
		//下层处理的线程指针传递pBuzInterface 
		CpicTreat(CtransMsg* pBuzInterface);
		virtual ~CpicTreat(void);

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
		bool m_bLoad;//加载需求
		bool m_bcanLoad;//可以加载
		bool m_bEmptyData;
	};
	CpicTreat* m_picTreat;
	CGetMsg m_trans;		//图像处理完成后的提供
	int m_msgNum;
public:
	CDevice();
	~CDevice();

	//图片的处理	
	bool treatPicture();
	std::vector<cv::Mat> m_vctm;
private:
	bool ini();
};

#endif