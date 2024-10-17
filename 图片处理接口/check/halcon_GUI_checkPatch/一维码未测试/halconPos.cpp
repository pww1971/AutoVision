// halconPos.cpp : 定义 DLL 应用程序的导出函数。
//

#include "PwwRegionConnect.h"
#include "templateDesign.h"

#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>

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
		ClearBarCodeModel(pos);
	};

	S_PAndC* Clone()
	{
		S_PAndC* tmp = new S_PAndC();
		tmp->pos = pos.Clone();//??????
		return tmp;
	};
	HTuple pos;
	
};



extern "C" TOTLIB_API bool TotDLLTreatPicture(cv::Mat& src,CTpltParam& pa, cv::RotatedRect& roiRect);
bool TotDLLTreatPicture(cv::Mat& src,CTpltParam& pa, cv::RotatedRect& roiRect)
{
	static string S_szReg="";// "(C562415)(\\dddd)"  头文字C562415和末尾四个数字
	try
	{
		S_DllParam& halPa = pa.m_dllPa;

		//	float startTime = omp_get_wtime();
		_m2oLock.lock();
		if (halPa.ini<1)//
		{
			halPa.type = 1;//标记halcon算法

			if (halPa.p==NULL)
				halPa.p = new S_PAndC();
		
			//读取配置文件 读出字符匹配规则
			CConfig cfg(getExePath()+"barconf.txt");
			string szVal = cfg.getItem("regex");
			if ("" != szVal)
			{
				S_szReg = szVal;
			}
			//Read bar codes of type 2/5 Industrial
			//
			CreateBarCodeModel(HTuple(), HTuple(), &((S_PAndC*)halPa.p)->pos);
			//We expect to decode a single bar code per image
			SetBarCodeParam(((S_PAndC*)halPa.p)->pos, "stop_after_result_num", 1);

			//Some codes show a minimal code length of 1 digit. Hence, we need to decrease the
			//default setting for this application.
			SetBarCodeParamSpecific(((S_PAndC*)halPa.p)->pos, "2/5 Industrial", "min_code_length", 
				1);

			halPa.ini=1;
			_m2oLock.unLock();
			return true;
		}
		else
		{
			if (halPa.ini == 2)//克隆自己
			{
				halPa.p = ((S_PAndC*)halPa.p)->Clone();
				halPa.ini=1;
				_m2oLock.unLock();
				return true;
			}
			cv::Mat dst = src.clone();
			if (dst.channels()>1)
				cv::cvtColor(dst, dst, CV_RGB2GRAY);
			HObject ho_SymbolRegions,ho_Image0 = Mat2HObject(dst);
			HTuple  hv_DecodedDataStrings;
			//	WriteImage(ho_Image0,"bmp",0,"./raw.bmp");
			//F 。查找 模板
			//Read bar code, the resulting string includes the check character
			SetBarCodeParam(((S_PAndC*)halPa.p)->pos, "check_char", "absent");
			FindBarCode(ho_Image0, &ho_SymbolRegions, ((S_PAndC*)halPa.p)->pos, "2/5 Industrial", 
				&hv_DecodedDataStrings);

			HTuple  hv_FoundStandard;
			hv_FoundStandard = hv_DecodedDataStrings.TupleLength();
			if (0 != (hv_FoundStandard==0))
			{
				_m2oLock.unLock();
				return false;
			}
			else
			{
				pa.m_dllPa.szRet = hv_DecodedDataStrings.ToString();
				//src = HObject2Mat(ho_SymbolXLDs);
				//配置文件的匹配规则做匹配
				if ("" != S_szReg)
				{
					regex r(S_szReg);
					bool ret = std::regex_match(pa.m_dllPa.szRet, r); //regex_search
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

		//	float endTime = omp_get_wtime();
		//	std::cout << "使用消耗时间： " << endTime - startTime << std::endl;
	}
	catch (HalconCpp::HException &except)
	{
		ofstream fp;
		fp.open("./err.txt",ios::app);
		fp << except.ErrorText() << /*"   " << (*(HTuple*)pa.m_dllPa.p).ToString()<<*/endl;
		fp.close();
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
		catch (HalconCpp::HException &except)
		{
			ofstream fp;
			fp.open("./err.txt",ios::app);
			fp << except.ErrorText() << endl;
			fp.close();
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


