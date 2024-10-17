#ifndef __CHECKPATCH_H__
#define __CHECKPATCH_H__
/*
功能描述：

*/

#include "pwwVision.h"
#include "TestHalconDlg.h"
#include "halconLocal.h"
#include "HalconCpp.h"
#include "HOperatorSetLegacy.h"
using namespace HalconCpp;
#pragma comment( lib, "../../../halcon\\Lib\\x64-win64\\halcon.lib")
#pragma comment( lib, "../../../halcon\\Lib\\x64-win64\\HalconCpp.lib")
struct S_CheckParam
{
	S_CheckParam()
	{
		index = 1;
		index2 = 2;
		index3 = 0;
		iBarCode = 0;
	};
	~S_CheckParam()
	{
		clearmode();
	};
	void clearmode()
	{
		if (0 == iBarCode)
			ClearDataCode2dModel(pos);
		else
			ClearBarCodeModel(pos);
	}
	void readcfgfile(string fileadd)
	{
		CConfig cfg(fileadd);		
		string szVal = cfg.getItem("index");
		if ("" != szVal)
		{
			index = atoi(szVal.c_str());
		}
		szVal = cfg.getItem("index2");
		if ("" != szVal)
		{
			index2 = atoi(szVal.c_str());
		}
		szVal = cfg.getItem("index3");
		if ("" != szVal)
		{
			index3 = atoi(szVal.c_str());
		}
		szVal = cfg.getItem("regex");
		if ("" != szVal)
		{
			szregex = szVal;
		}
		szVal = cfg.getItem("iBarCode");
		if ("" != szVal)
		{
			iBarCode = atoi(szVal.c_str());
		}
		changemodel();
	};
	void changemodel()
	{
		if (0 == iBarCode)
		{
			HTuple hv_Message[] = { "standard_recognition", "enhanced_recognition","maximum_recognition" };
			HTuple hv_SymbolType[] = {
				HTuple("Aztec Code"),
				HTuple("Data Matrix ECC 200"),
				HTuple("GS1 Aztec Code"),
				HTuple("GS1 DataMatrix"),
				HTuple("GS1 QR Code"),
				HTuple("Micro QR Code"),
				HTuple("PDF417"),
				HTuple("QR Code")
			};
			if (pos.TupleLength() > 0)
				clear_data_code_2d_model(pos);
			create_data_code_2d_model(hv_SymbolType[index], "default_parameters", hv_Message[index2], &pos);
		}
		else
		{
			HTuple hv_SymbolType[] = {
				HTuple("2/5 Industrial"),
				HTuple("2/5 Interleaved"),
				HTuple("Codabar"),
				HTuple("Code 128"),
				HTuple("Code 39"),
				HTuple("Code 93"),
				HTuple("EAN-13 Add-On 2"),
				HTuple("EAN-13 Add-On 5"),
				HTuple("EAN-13"),
				HTuple("EAN-8 Add-On 2"),
				HTuple("EAN-8 Add-On 5"),
				HTuple("EAN-8"),
				HTuple("GS1 DataBar Expanded Stacked"),
				HTuple("GS1 DataBar Expanded"),
				HTuple("GS1 DataBar Limited"),
				HTuple("GS1 DataBar Omnidir"),
				HTuple("GS1 DataBar Stacked Omnidir"),
				HTuple("GS1 DataBar Stacked"),
				HTuple("GS1 DataBar Truncated"),
				HTuple("GS1-128"),
				HTuple("MSI"),
				HTuple("PharmaCode"),
				HTuple("UPC-A Add-On 2"),
				HTuple("UPC-A Add-On 5"),
				HTuple("UPC-A"),
				HTuple("UPC-E Add-On 2"),
				HTuple("UPC-E Add-On 5"),
				HTuple("UPC-E"),
				HTuple("auto")
			};

			if (pos.TupleLength() > 0)
				ClearBarCodeModel(pos);
			//Read bar codes of type 2/5 Industrial
	//
			CreateBarCodeModel(HTuple(), HTuple(), &pos);
			//We expect to decode a single bar code per image
			SetBarCodeParam(pos, "stop_after_result_num", 1);

			//Some codes show a minimal code length of 1 digit. Hence, we need to decrease the
			//default setting for this application.
			bartype = hv_SymbolType[index3];
			if ("auto" != bartype)
			{
				SetBarCodeParamSpecific(pos, bartype, "min_code_length",1);
			}
		}
	};
	void writecfgfile(string fileadd)
	{
		CConfig cfg(fileadd);
		cfg.updateItem("index",CStrProcess::itoa_radixN(index));
		cfg.updateItem("index2",CStrProcess::itoa_radixN(index2));
		cfg.updateItem("index3", CStrProcess::itoa_radixN(index3));
		cfg.updateItem("regex",szregex);
		cfg.updateItem("iBarCode", CStrProcess::itoa_radixN(iBarCode));
	};

	HTuple pos;
	int index,index2,index3;
	string szregex;
	int iBarCode;//二维码0还是条码1
	HTuple bartype;//当前条码类型
};

class checkPatch 
{
public:
	checkPatch();
	~checkPatch();
	//图片的处理	
	//
	bool treatPicture(CTpltParam& pa, vector<S_TpltData*>& vctData);
	//具体的算法分析
	bool treatPic(HObject& ho_Image0,string& str);
	//画出ng的标识
	bool drawResult(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa);

	void readMode(string file);
	void writeMode(string file);

public:
	static CTestHalconDlg m_dlg;
	S_CheckParam m_pa;
};

#endif