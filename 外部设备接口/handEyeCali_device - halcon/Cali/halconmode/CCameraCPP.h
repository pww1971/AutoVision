#pragma once

#include "afxcmn.h"
#include "afxwin.h"
#include <HalconCpp.h>
#include "pwwVision.h"
#include <list>
static int WM_FGDISP = RegisterWindowMessage(_T("MYFGDisp"));
static int WM_IPDISP = RegisterWindowMessage(_T("MYIPDisp"));                  // 线程让主窗口显示
static int UM_UPDATESTATUS = RegisterWindowMessage(_T("MYUM_UPDATESTATUS"));   // 线程更新状栏文字
static int UM_UPDATE_RIBBON = RegisterWindowMessage(_T("UM_UPDATE_RIBBON"));

static int UM_UPDATESTATUS_APPSTATE = RegisterWindowMessage(_T("UM_UPDATESTATUS_APPSTATE"));
static int UM_LOGOUT = RegisterWindowMessage(_T("UM_LOGOUT"));                  // 显示日志信息

#define MAX_BUFFERS 2
//#define INC_BUFFER( IDX)  ((IDX) < (MAX_BUFFERS-1)) ? (IDX)++ : (IDX)=0

/////////////////////////////////////////////////////////////////////////////
// ResultContainer - data structure to share the result data between the
//                   worker threads and the display thread. 
/////////////////////////////////////////////////////////////////////////////
struct ResultContainer
{
	//HalconCpp::HImage   disp_img;
	//HalconCpp::HXLDCont disp_cont;
	HalconCpp::HImage   result_img;
	HalconCpp::HXLDCont result_cont;
	HalconCpp::HTuple   time_needed;
	HalconCpp::HTuple   result_handle;
	HalconCpp::HTuple   decoded_data;
};

class CChildView;


class CCameraCPP
{
public:

	CCameraCPP();
	~CCameraCPP();


	BOOL GetIsOpen();


	int GetParameterInt(CString sPara);
	CString GetParameterString(CString sPara);
	double GetParameterDouble(CString sPara);

	void SetParameterHTuple(std::string str, HalconCpp::HTuple ht);

	void SetParameterHTuple(CString str, HalconCpp::HTuple ht) {
		SetParameterHTuple(string(str.GetBuffer(0)), ht);
	};

	HalconCpp::HFramegrabber* GetPFG();

	BOOL Connect(CString str = "NotFiles");                           // 如果无参数就是调用摄像头
	BOOL close_camera();

	void SetbOnePIcMaxErrorOK(bool b);

	void SetfError(double f);

	void OneShot();     // 软触发模式采集一帖图像（设置曝光和增益马上生效，不需要延时）

	HBOOL GetIsFGRuning();
	void SetFGRuning(HBOOL b);

	;
	void GetBufferImage(HalconCpp::HImage*);
	void GrabOneImageAsync(HalconCpp::HImage *);  // 异步采集图像
	void ResetIPNewImageEvent();

	HANDLE GetNewImageEvent();


private:

	std::list<HalconCpp::HImage> imgList;        // 采集的图像缓冲  

	CRITICAL_SECTION       newImageMutex;        /* Local mutex to protect the access on the image buffer  */
	HANDLE                 newImageEven;         /* Event to tell the IP thread of a new image        */
	double fError;							     // 图像的误差	 
	bool bOnePIcMaxErrorOK;						 // 本次标定图像有效
	double RectificationWidth;					 // width of the visible area in world coordinates[mm]

												 //-----------------------------------------------------------------------
												 // 以上是相机的公共参数
	HBOOL     m_bFGIsRunning;                    // 相机正在采集图像
	HalconCpp::HFramegrabber  *m_pFG;            // Frame grabber 
	BOOL    bIsOpened;                           // 相机是否打开了.



};
