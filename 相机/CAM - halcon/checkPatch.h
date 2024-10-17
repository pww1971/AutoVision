#ifndef __CHECKPATCH_H__
#define __CHECKPATCH_H__
/*
功能描述：

*/
#include "CAM.h"
#include "CameraDrv.h"
#include "Design_dlg_posDlg.h"
#include <HalconCpp.h>
#include "CCameraCPP.h"
using namespace HalconCpp;
HImage Mat2HObject(cv::Mat& image);
cv::Mat HObject2Mat(HImage Hobj);
Mat HImageToMat32Bit(HObject& imgHalcon);

struct S_CheckParam : public CConfig
{
	S_CheckParam(string szFile) :CConfig(szFile) 
	{
		
	};
	void readcfgfile(UCI::OptionsMap& omp)
	{
		vector< pair<string, string> >::iterator it = m_vctKeyVal.begin();
		for (; it != m_vctKeyVal.end(); ++it)
		{
			string szKeyF = (*it).first;
			string szValF = (*it).second;
			UCI::Option op;
			op.strToOption(szValF);

			omp.insert(pair<CString,UCI::Option>(szKeyF.c_str(),op));			
		}
	};
	void writecfgfile( UCI::OptionsMap *o)
	{
		// 对每一个元素进行
		UCI::OptionsMap::iterator it = o->begin();
		while(it != o->end())
		{
			UCI::Option& p = it->second;
			CString name = it->first;

			updateItem(name.GetBuffer(0), p.getOptionStr());//内容
			it++;
		}
	};
	BOOL isCamParaCanOut(CString Name)
	{
		if (Name == "备注") {
			return FALSE;
		}
		else if (Name == "项目编号") {
			return FALSE;
		}
		else if (Name == "元素类型") {
			return FALSE;
		}
		else if (Name == "元素名称") {
			return FALSE;
		}
		else if (Name == "元素已匹配") {
			return FALSE;
		}
		else if (Name == "元素备注") {
			return FALSE;
		}
		else if (Name == "命令行") {
			return FALSE;
		}
		else if (Name == "采样延时") {
			return FALSE;
		}

		return TRUE;
	}
};
#ifdef HCAM
class checkPatch :public HCameraDrv
#else
class checkPatch :public CameraDrv
#endif
{
public:
	checkPatch();
	~checkPatch();
#ifdef HCAM
	virtual int grapImage(int id, vector<HImage>& vm);
#else
	virtual int grapImage(int id, vector<cv::Mat>& vm);
#endif
	int ini();
	void readMode(string file="");
	void writeMode( );

public:
	string m_cfgAdd;//配置文件目录
	CList <UCI::OptionsMap, UCI::OptionsMap> m_EleList;
	vector <S_Cali*> m_CaliList;
	static CDesign_dlg_posDlg S_dlg;
//	CCameraCPP m_halconCam; 暂时没用halcon的相机 以后其他型号相机就直接使用这个方便
};

#endif