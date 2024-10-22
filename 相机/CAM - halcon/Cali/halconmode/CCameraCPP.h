#pragma once

#include "afxcmn.h"
#include "afxwin.h"
#include <HalconCpp.h>
#include "pwwVision.h"
#include <list>
#define MAX_BUFFERS 200
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
		SetParameterHTuple(std::string(str.GetBuffer(0)), ht);
	};

	HalconCpp::HFramegrabber* GetPFG();

	BOOL Connect(CString str = "NotFiles");                           // ����޲������ǵ�������ͷ
	BOOL close_camera();

	void SetbOnePIcMaxErrorOK(bool b);

	void SetfError(double f);

	void OneShot();     // ����ģʽ�ɼ�һ��ͼ�������ع������������Ч������Ҫ��ʱ��

	HBOOL GetIsFGRuning();
	void SetFGRuning(HBOOL b);

	void GetBufferImage(HalconCpp::HImage*);
	void GetLastBufferImage(HalconCpp::HImage*);//��ȡ��ǰ��ͼƬ
	void ClearBufImage();
	void GrabOneImageAsync(HalconCpp::HImage *);  // �첽�ɼ�ͼ��
	void ResetIPNewImageEvent();

	HANDLE GetNewImageEvent();


private:

	std::list<HalconCpp::HImage> imgList;        // �ɼ���ͼ�񻺳�  

	CRITICAL_SECTION       newImageMutex;        /* Local mutex to protect the access on the image buffer  */
	HANDLE                 newImageEven;         /* Event to tell the IP thread of a new image        */
	double fError;							     // ͼ������	 
	bool bOnePIcMaxErrorOK;						 // ���α궨ͼ����Ч
	double RectificationWidth;					 // width of the visible area in world coordinates[mm]

												 //-----------------------------------------------------------------------
												 // ����������Ĺ�������
	HBOOL     m_bFGIsRunning;                    // ������ڲɼ�ͼ��
	HalconCpp::HFramegrabber  *m_pFG;            // Frame grabber 
	BOOL    bIsOpened;                           // ����Ƿ����.



};
