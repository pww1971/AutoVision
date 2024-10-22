#ifndef __CHECKPATCH_H__
#define __CHECKPATCH_H__
/*
����������

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
		// ��ÿһ��Ԫ�ؽ���
		UCI::OptionsMap::iterator it = o->begin();
		while(it != o->end())
		{
			UCI::Option& p = it->second;
			CString name = it->first;

			updateItem(name.GetBuffer(0), p.getOptionStr());//����
			it++;
		}
	};
	BOOL isCamParaCanOut(CString Name)
	{
		if (Name == "��ע") {
			return FALSE;
		}
		else if (Name == "��Ŀ���") {
			return FALSE;
		}
		else if (Name == "Ԫ������") {
			return FALSE;
		}
		else if (Name == "Ԫ������") {
			return FALSE;
		}
		else if (Name == "Ԫ����ƥ��") {
			return FALSE;
		}
		else if (Name == "Ԫ�ر�ע") {
			return FALSE;
		}
		else if (Name == "������") {
			return FALSE;
		}
		else if (Name == "������ʱ") {
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
	string m_cfgAdd;//�����ļ�Ŀ¼
	CList <UCI::OptionsMap, UCI::OptionsMap> m_EleList;
	vector <S_Cali*> m_CaliList;
	static CDesign_dlg_posDlg S_dlg;
//	CCameraCPP m_halconCam; ��ʱû��halcon����� �Ժ������ͺ������ֱ��ʹ���������
};

#endif