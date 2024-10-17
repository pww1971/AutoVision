// halconPos.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "PwwRegionConnect.h"
#include "templateDesign.h"
#include "halSelectRoi.h"
#include "opencv2/opencv.hpp"

#include <regex>

#ifdef WIN32

#ifdef _DEBUG
#pragma comment( lib, "opencv_world310d.lib")
#else
#pragma comment( lib, "opencv_world310.lib")
#endif

#endif

#if (defined WIN32 || defined _WIN32 || defined WINCE)
#ifdef TOTLIB_EXPORTS
#define TOTLIB_API __declspec(dllexport)
#else
#define TOTLIB_API __declspec(dllimport)
#endif

#else
#define TOTLIB_API 
#endif


#include "halconLocal.h"
//#define  HALCON_SHAPE_POS
static CResLock _m2oLock;

struct S_PAndC: string
{
	S_PAndC(){
	};
	~S_PAndC(){
		clearMode();
	};
	void clearMode()
	{
		clear_data_code_2d_model(pos);
	};

	S_PAndC* Clone()
	{
		S_PAndC* tmp = new S_PAndC();
		tmp->pos = pos;//??????
		return tmp;
	};
	HTuple pos;
	
};

HTuple hv_Message[] = {"standard_recognition", "enhanced_recognition","maximum_recognition"};
HTuple hv_SymbolType[] ={
	HTuple("Aztec Code"),	
	HTuple("Data Matrix ECC 200"),
	HTuple("GS1 Aztec Code"),
	HTuple("GS1 DataMatrix"),
	HTuple("GS1 QR Code"),
	HTuple("Micro QR Code"),
	HTuple("PDF417"),
	HTuple("QR Code")
};

extern "C" TOTLIB_API bool TotDLLTreatPicture(cv::Mat& src,CTpltParam& pa, cv::RotatedRect& roiRect);
bool TotDLLTreatPicture(cv::Mat& src,CTpltParam& pa, cv::RotatedRect& roiRect)
{
	try
	{
		S_DllParam& halPa = pa.m_dllPa;

		_m2oLock.lock();
		if (halPa.ini<1)//
		{
			halPa.type = 1;//���halcon�㷨

			if (halPa.p==NULL)
				halPa.p = new S_PAndC();
			int index = pa.m_polarPa.scope;
			if (index >= 8)
			{
				index = 1;
			}
			create_data_code_2d_model(hv_SymbolType[index], "default_parameters", hv_Message[2], &((S_PAndC*)halPa.p)->pos);

			halPa.ini=1;
			_m2oLock.unLock();
			return true;
		}
		else
		{
			if (halPa.ini == 2)//��¡�Լ�
			{
				halPa.p = ((S_PAndC*)halPa.p)->Clone();
				halPa.ini=1;
				_m2oLock.unLock();
				return true;
			}
			cv::Mat dst = src.clone();
			if (dst.channels()>1)
				cv::cvtColor(dst, dst, CV_RGB2GRAY);
			Hobject ho_SymbolXLDs,ho_Image0 = Mat2HObject(dst);
			//	WriteImage(ho_Image0,"bmp",0,"./raw.bmp");
			//F ������ ģ��
			//��������
			//1. Score �ɹ�
			//2. Row, Column, Angle ==�� λ�� x , y , �Ƕ�
			HTuple   hv_ResultHandles, hv_DecodedDataStrings;
			HTuple  hv_FoundStandard;

			find_data_code_2d(ho_Image0, &ho_SymbolXLDs, ((S_PAndC*)halPa.p)->pos, "train", "all", 
				 &hv_ResultHandles, &hv_DecodedDataStrings);
			 hv_FoundStandard = hv_DecodedDataStrings.Num();
			 if (0 != (hv_FoundStandard==0))
			 {
				 _m2oLock.unLock();
				 return false;
			 }
			 else
			{
				pa.m_dllPa.szRet = hv_DecodedDataStrings[0].S();
				pa.m_dllPa.szRet = CStrProcess::Trim(pa.m_dllPa.szRet,"\"");
				//src = HObject2Mat(ho_SymbolXLDs);
				//�����ļ���ƥ�������ƥ��
				if("" != pa.m_dllPa.szParam )
				{
					regex r(pa.m_dllPa.szParam);
					bool ret = std::regex_match(pa.m_dllPa.szRet, r); //regex_search "(C562415)(\\dddd)"  ͷ����C562415��ĩβ�ĸ�����
					if (ret)
					{
					}
					else
					{
						_m2oLock.unLock();
						return false;
					}
				}
			}
		}
	}
	catch (HException &except)
	{
		//ofstream fp;
		//fp.open("./err.txt",ios::app);
		//fp << except.ErrorText() << /*"   " << (*(HTuple*)pa.m_dllPa.p).ToString()<<*/endl;
		//fp.close();
		/*HObject ho_Image0 = Mat2HObject(src);
		WriteImage(ho_Image0,"bmp",0,"./raw.bmp");*/
		_m2oLock.unLock();
		return false;
	}
	_m2oLock.unLock();
	return true;
}

extern "C" TOTLIB_API void TotDllRelease(CTpltParam& pa);
void TotDllRelease(CTpltParam& pa)
{
	if (pa.m_dllPa.ini == 1)
	{
		_m2oLock.lock();
		try
		{
			if (NULL!=pa.m_dllPa.p)
			{
				delete (S_PAndC*)pa.m_dllPa.p;
				pa.m_dllPa.p = NULL;
			}
		}
		catch (HException &except)
		{
			/*ofstream fp;
			fp.open("./err.txt",ios::app);
			fp << except.ErrorText() << endl;
			fp.close();*/
			_m2oLock.unLock();
			return;
		}
		pa.m_dllPa.sel = NULL;
		pa.m_dllPa.ini = 0;
		pa.m_dllPa.type = 0;
		_m2oLock.unLock();
	}

}


extern "C" TOTLIB_API void TotDllWriteFile(CTpltParam& pa);
void TotDllWriteFile(CTpltParam& pa)
{
	
}


