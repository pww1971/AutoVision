#ifndef __CHECKPATCH_H__
#define __CHECKPATCH_H__
/*
����������

*/
#include "textureDlg.h"
#include "pwwVision.h"
#include <HalconCpp.h>
using namespace HalconCpp;
HImage Mat2HObject(cv::Mat& image);
cv::Mat HObject2Mat(HImage& Hobj);
struct S_CheckProb
{
	S_CheckProb()
	{
		patch_normalization=0;
		patch_rotational_robustness=0;
		gmm_em_threshold = 0.001;
		szLevel ="auto";
		sensitivity=0;
		hv_NoveltyThreshold.Clear();

		CreateTextureInspectionModel("basic", &hv_TextureInspectionModel);
	};
	~S_CheckProb()
	{
		ClearTextureInspectionModel(hv_TextureInspectionModel);
	};
	void readcfgfile(string fileadd)
	{
		CConfig cfg(fileadd + ".cfg");
		string szVal = cfg.getItem("patch_normalization");
		if ("" != szVal)
		{
			patch_normalization = atoi(szVal.c_str());
		}
		szVal = cfg.getItem("patch_rotational_robustness");
		if ("" != szVal)
		{
			patch_rotational_robustness = atoi(szVal.c_str());
		}
		szVal = cfg.getItem("gmm_em_threshold");
		if ("" != szVal)
		{
			gmm_em_threshold = atof(szVal.c_str());
		}
		szVal = cfg.getItem("sensitivity");
		if ("" != szVal)
		{
			sensitivity = atof(szVal.c_str());
		}
		szVal = cfg.getItem("szLevel");
		if ("" != szVal)
		{
			szLevel = szVal;
		}
		szVal = cfg.getItem("hv_NoveltyThreshold");
		if ("" != szVal)
		{
			vector<string> vct;
			CStrProcess::Split(szVal, ",", vct, true);
			hv_NoveltyThreshold.Clear();
			for (int i = 0; i < vct.size(); i++)
				hv_NoveltyThreshold[i] = atoi(vct[i].c_str());
		}
		
		try
		{
			string str = fileadd + "model.htim";
			HTuple hv_FileExists;
			HTuple hv_TextureModelFilename = str.c_str();
			FileExists(hv_TextureModelFilename, &hv_FileExists);
			if (0 != hv_FileExists)
			{
				//ClearTextureInspectionModel(hv_TextureInspectionModel);
				ReadTextureInspectionModel(hv_TextureModelFilename, &hv_TextureInspectionModel);
				//HTuple hv_Level;
				//GetTextureInspectionModelParam(hv_TextureInspectionModel, "levels", &hv_Level);
			}
		}
		catch (HalconCpp::HException& except) {
			AfxMessageBox(except.ErrorText());
		}
	};
	void writecfgfile(string fileadd)
	{
		CConfig cfg(fileadd + ".cfg");
		char sz[32];
		sprintf(sz, "%d", patch_normalization);
		cfg.updateItem("patch_normalization", sz);
		sprintf(sz, "%d", patch_rotational_robustness);
		cfg.updateItem("patch_rotational_robustness", sz);
		sprintf(sz, "%f", gmm_em_threshold);
		cfg.updateItem("gmm_em_threshold", sz);
		sprintf(sz, "%f", sensitivity);
		cfg.updateItem("sensitivity", sz);
		cfg.updateItem("szLevel", szLevel);
		int n = hv_NoveltyThreshold.TupleLength();
		string szthr = "";
		for (int i = 0; i < n; i++)
		{
			sprintf(sz, "%d,", hv_NoveltyThreshold[i].I());
			szthr += sz;
		}
		cfg.updateItem("hv_NoveltyThreshold", szthr);

		try
		{
			string str = fileadd + "model.htim";
			remove(str.c_str());
			HTuple hv_TextureModelFilename = str.c_str();
			WriteTextureInspectionModel(hv_TextureInspectionModel, hv_TextureModelFilename);
		}
		catch (HalconCpp::HException& except) {
			AfxMessageBox(except.ErrorText());
		}
	};
	int patch_normalization;
	//'patch_normalization'��'weber'������³������none����Ҫ������Ϊ���У�Ĭ�ϣ�
	int patch_rotational_robustness;
	//	'patch_rotational_robustness'��'true'����ת³����'false'��Ҫ��ת��Ϊ���У�Ĭ�ϣ�
	float gmm_em_threshold;
	//	�ӿ�ѵ���ķ�����

	//	�� ����ѵ��������zoom����ͨ��ģ�ͶԵͷֱ�������覴ü��Ч�����á�

	//	�� ���� 'gmm_em_threshold'�����ή��ģ��׼ȷ�ȣ�ѵ����ֹ������֮һ��0.001��Ĭ�ϣ���

	//	����Ҫ������
	string szLevel;
	//	'levels'�����þ���Ľ����������ѵ��������Խ�ֲڣ���ϵ͵Ľ������㼶Խ��ʡ�ԡ�Ĭ��auto��
	float sensitivity;
	//	'sensitivity'�������ȣ�Ӱ��'novelty_threshold'�ļ���������ֵ�ᵼ�¸��ߵ���ֵ���Ӷ����ٵķ���ȱ�ݡ�Ĭ��0��
	HTuple hv_NoveltyThreshold;
	//	'novelty_threshold'����ֵ΢����

	HTuple  hv_TextureInspectionModel;
		/*"gen_result_handle",
		"true");�������н��������*/
};
class checkPatch 
{
public:
	checkPatch();
	~checkPatch();
	//ͼƬ�Ĵ���	
	//
	bool treatPicture(CTpltParam& pa, vector<S_TpltData*>& vctData);
	//������㷨����
	bool treatPic(S_TpltData* data,int treatid);
	bool treatPic(HImage& ho_Image);
	//����ng�ı�ʶ
	bool drawResult(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa);
	void readMode(string file="");
	void writeMode(string file="");
	void clone(void*p);
public:
	S_CheckProb m_pa;
	string m_configAdd;//�����ļ�
	
	HObject  ho_ConnectedRegions;
};

#endif