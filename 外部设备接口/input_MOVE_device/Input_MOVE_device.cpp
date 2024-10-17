/* 定义 DLL 应用程序的导出函数。
针对AD转换和插补运动的相关接口 可以根据需求自行添加
1、在MOVE_CARD.cpp 文件里添加并实现
extern "C" PROCLIB_API void myDefineMoveType();
void myDefineMoveType()
{
查询运动卡关于AD转换和插补运动的api接口实现
}
2、外部导入设备中写下代码
void (*myDefineMove)()=NULL;//先定义函数指针
extern "C" PROCLIB_API bool MoveDLLMove(void* pmv, void*& p2, string & data);
bool MoveDLLMove(void* pmv, void*& p2, string& data)
{
	CMoveBase* mb = (CMoveBase*)pmv;
	if (NULL != mb)
	{
		CMoveDllType* handle = mb->m_mtc.m_handle;
		if (NULL == myDefineMove)
			if (!handle->GetFunction("myDefineMoveType", myDefineMove)) //从dll中加载AD或插补运动函数
				return false;
		if (data == "check")
		{
			myDefineMove();//直接调用该函数
		}
	}

}
*/
//机器人或机械手臂一般采用通讯方式，也可以封装成扩展接口的形式 让外部导入设备调用
#include "stdafx.h"

#include "device.h"
#if (defined WIN32 || defined _WIN32 || defined WINCE)
#ifdef PROCLIB_EXPORTS
#define PROCLIB_API __declspec(dllexport)
#else
#define PROCLIB_API __declspec(dllimport)
#endif

#else
#define PROCLIB_API 
#endif
extern "C" PROCLIB_API string MoveDLLHelp();
extern "C" PROCLIB_API bool MoveDLLIni(void* pmv, void*& p2);
extern "C" PROCLIB_API void MoveDLLStop();
//动态库中提取的标函数
extern "C" PROCLIB_API bool MoveDLLMove(void* pmv, void*& p2, string & data);

//必须显示的调用释放内存
extern "C" PROCLIB_API void MoveDllRelease(void*& p2);
extern "C" PROCLIB_API void MoveDllWriteFile();

string MoveDLLHelp()
{
#ifdef nouseEN
	string szHelp = "\r\ncheckio,ioid,n = 检测io是否高位 n检测时间\r\nviewArrCur = 显示阵列当前位置\r\ncam = 系列拍照,check = 多图检测";
#else
	string szHelp = "\r\ncheckio,ioid,n = Check whether the IO is high, n Detection time\r\nviewArrCur = Displays the current position of the array\r\ncam = series of cam,check = Multi-image detection";
#endif
	return szHelp;
}

bool MoveDLLIni(void* pmv, void*& p2)
{
	if (pmv == NULL)
	{
		return false;
	}
	/*CVisionSystem* mb = (CVisionSystem*)pmv;
	CDevice* devParam;
	if (p2 == NULL)
	{
		devParam = new CDevice();
		p2 = devParam;
	}
	else
		devParam = (CDevice*)p2;*/
	return true;
}

void MoveDLLStop()
{

}

//动态库中提取的标函数
bool MoveDLLMove(void* pmv, void*& p2, string& data)
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CVisionSystem* mb = (CVisionSystem*)pmv;
	if (NULL != mb)
	{
		if (data == "cam")
		{
			CDevice* devParam;
			if (p2 == NULL)
			{
				devParam = new CDevice();
				p2 = devParam;
			}
			else
				devParam = (CDevice*)p2;
			std::vector<cv::Mat> vm;
			if (-1 == mb->m_pCheck->camPic(mb->m_pCheck->m_curCamId, vm))
			{
				data = "";
				return false;
			}
			devParam->m_vctm.insert(devParam->m_vctm.end(),vm.begin(), vm.end());
		}
		else if (data == "check")//拍照检测 推人后台就返回
		{
			CDevice* devParam;
			if (p2 == NULL)
			{
				devParam = new CDevice();
				p2 = devParam;
			}
			else
				devParam = (CDevice*)p2;
			if (devParam->m_vctm.size()<1)
			{
				return false;
			}
			S_Msg* pmsg = new S_Msg;
			pmsg->vm.swap(devParam->m_vctm);
			devParam->m_picTreat->m_dll = mb->m_pCheck;
			pmsg->m_procid = mb->m_pCheck->m_picProcId;
			devParam->m_picTreat->pushMsg(pmsg);
			//mb->m_pCheck->procCheck(mb->m_pTotCheck->m_picProcId,m,&cor);
		}
		else if (data == "getdata")//提取数据 并重新赋值返回定位或检测结果
		{
			CDevice* devParam;
			if (p2 == NULL)
			{
				devParam = new CDevice();
				p2 = devParam;
			}
			else
				devParam = (CDevice*)p2;
			S_Msg* pmsg = devParam->m_trans.getData();
			if (NULL != pmsg)
			{
				data = pmsg->str;
				char sz[128];
				sprintf(sz, ",%d,%d,%.3f", pmsg->px, pmsg->py, pmsg->f);
				data += sz;
			}
		}
		else if (data == "viewArrCur")
		{
			mb->m_mtc.viewMtCurPos();
		}
		else
		{
			static uint32 iostatus[g_ioNum];
			std::vector<string > cont;
			CStrProcess::Split(data, ",", cont, true);
			if (cont[0] == "checkio" && cont.size() > 0)
			{
				CMoveDllType* handle = mb->m_mtc.m_handle;
				int ioid = 7;
				if (cont.size() > 1)
					ioid = atoi(cont[1].c_str());
				else
				{
					data += "param error";
					return false;
				}

				int settime = 10;
				if (cont.size() > 2)
				{
					settime = atoi(cont[2].c_str());
				}
				double m_stime = clock();
				while (clock() - m_stime < settime)//settime毫秒
				{
					memset(iostatus, 0, sizeof(iostatus));
					handle->getIO(iostatus);
					if (iostatus[ioid])
					{
						data = "";
						return true;
					}
					Sleep(10);
				}
				data = "";
				return false;
			}
			else
			{
				data = "error format:" + data;
				return false;
			}
		}
	}
	return true;
}

//必须显示的调用释放内存
void MoveDllRelease(void*& p2)
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (NULL != p2)
	{
		delete (CDevice*)p2;
		p2 = NULL;
	}
}

void MoveDllWriteFile()
{

}
