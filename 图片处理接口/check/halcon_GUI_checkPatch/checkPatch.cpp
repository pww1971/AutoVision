
#include "stdafx.h"
#include "checkPatch.h"
#include <regex>

CTestHalconDlg checkPatch::m_dlg;


checkPatch::checkPatch()
{


}

checkPatch::~checkPatch()
{
}


//图片的处理	一保存模板图片，二加载模板图片进行旋转角度后匹配
bool checkPatch::treatPicture(CTpltParam& pa, vector<S_TpltData*>& vctData)
{
	int curid = pa.m_tempPa.m_curPosID;
	vector<cv::Mat>& src = vctData[curid]->m_vctPic;
	if (!src[0].data )
		return false;

	if (pa.m_dllPa.ini<1)
	{		
		if ("" != pa.m_dllPa.szfile)
		{
			readMode(pa.m_dllPa.szfile);
		}
		if (pa.m_dllPa.ini == 0 )
		{
			;
		}
		else//ini == -1
		{
			m_dlg.m_pCheck = this;
			m_dlg.m_ImageOrg = Mat2HObject(src[0]);
			if(1 == m_dlg.DoModal())
			{
				writeMode(pa.m_dllPa.szfile);
			}
		}
		pa.m_dllPa.ini=1;
		return true;
	}
	else
	{
		//处理图片
		cv::Mat dst = src[0].clone();
		if (dst.channels()>1)
			cv::cvtColor(dst, dst, cv::COLOR_RGB2GRAY);
		HObject ho_Image0 = Mat2HObject(dst);

		if(treatPic(ho_Image0, vctData[curid]->m_szRet))
		{
			vctData[curid]->m_checkRet = true;
			return true;
		}
	}
	return false;
}

bool checkPatch::treatPic(HObject& ho_Image0,string& str)
{
	//写处理流程如下
	HObject ho_SymbolXLDs;
	HTuple   hv_ResultHandles, hv_DecodedDataStrings;
	HTuple  hv_FoundStandard;
	if (0 == m_pa.iBarCode)
	{
		FindDataCode2d(ho_Image0, &ho_SymbolXLDs, m_pa.pos, "train", "all",
			&hv_ResultHandles, &hv_DecodedDataStrings);
	}
	else
	{
		//Read bar code, the resulting string includes the check character
		SetBarCodeParam(m_pa.pos, "check_char", "absent");
		FindBarCode(ho_Image0, &ho_SymbolXLDs, m_pa.pos, m_pa.bartype,
			&hv_DecodedDataStrings);
	}
	hv_FoundStandard = hv_DecodedDataStrings.TupleLength();
	if (0 != (hv_FoundStandard==0))
	{
		return false;
	}
	else
	{
		str = hv_DecodedDataStrings[0].S();
		str = CStrProcess::Trim(str,"\"");
		if("" != m_pa.szregex )
		{
			//std::smatch match;
			//regex r(m_pa.szregex);
			//if (!std::regex_search(str, match, r, regex_constants::match_default)) //regex_search "(C562415)(\\dddd)"  头文字C562415和末尾四个数字
			//	return false;
			regex r(m_pa.szregex);
			bool ret = std::regex_match(str, r); //regex_search "(C562415)(\\dddd)" 
			if (ret)
			{
			}
			else
			{
				return false;
			}
		}
	}
	return true;
}

void checkPatch::readMode(string file)
{
	mkdir(file.c_str());
	m_pa.readcfgfile(file + "\\checkParam1.cfg");
}

void checkPatch::writeMode(string file)
{
	delCfgFiles(file.c_str(),1);
	mkdir(file.c_str());
	m_pa.writecfgfile(file + "\\checkParam1.cfg");
}

bool checkPatch::drawResult(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa)
{
	
	return true;
}
