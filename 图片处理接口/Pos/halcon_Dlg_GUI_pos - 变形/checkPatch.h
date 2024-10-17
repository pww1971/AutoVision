#ifndef __CHECKPATCH_H__
#define __CHECKPATCH_H__
/*
功能描述：

*/
#include "Design_GUI_pos.h"
#include "Design_dlg_posDlg.h"
#include <HalconCpp.h>
using namespace HalconCpp;
HImage Mat2HObject(cv::Mat& image);
cv::Mat HObject2Mat(HImage Hobj);

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


};

class checkPatch 
{
public:
	checkPatch();
	~checkPatch();
	void clearMode();

	void readMode(string file="");
	void writeMode( string file="");

	bool EditModel(cv::Mat& src);

	bool checkLocateCheck(cv::Mat& src, vector<S_TpltData*>& vctData,CTpltParam& Pa);
	//画出ng的标识
	bool drawResult(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa);

public:
	string m_cfgDir;//配置文件目录
	CList <UCI::OptionsMap, UCI::OptionsMap> m_EleList;
	vector <S_Model*> m_modelList;
	static CDesign_dlg_posDlg S_dlg;
};

#endif