#ifndef __CHECKPATCH_H__
#define __CHECKPATCH_H__
/*
功能描述：

*/
#include "C3dMeatureDlg.h"
#include "pwwVision.h"
#include <HalconCpp.h>
using namespace HalconCpp;
HObject MatToHImage16Bit(Mat& imgMat);
Mat HImageToMat16Bit(HObject& imgHalcon);
HImage Mat2HObject(cv::Mat& image);
cv::Mat HObject2Mat(HObject& Hobj);
struct S_CheckProb
{
	S_CheckProb()
	{
		m_type=0;//检测项目 0、高度 1、顶点 2、平面度 3、两平面角度 4、高度差 5、切片
		m_bl= 0, m_bh=1;//误差标准值上下范围
		m_dret = -1;
		m_objId=0, m_objPairId=0;//角度 高度差 需要两个目标的id
		m_szRet=m_procName="";
		m_hvReg.GenEmptyRegion();
		hv_Pose.Clear();
		hv_Pose[0] = 0;
		hv_Pose[1] = 0;
		hv_Pose[2] = 0;
		hv_Pose[3] = 0;
		hv_Pose[4] = 90;
		hv_Pose[5] = 0;
		hv_Pose[6] = 0;
	};
	~S_CheckProb()
	{

	};
	void readcfgfile(string fileadd)
	{
		CConfig cfg(fileadd + ".cfg");
		string szVal = cfg.getItem("m_type");
		if ("" != szVal)
		{
			m_type = atoi(szVal.c_str());
		}
		szVal = cfg.getItem("m_objId");
		if ("" != szVal)
		{
			m_objId = atoi(szVal.c_str());
		}
		szVal = cfg.getItem("m_objPairId");
		if ("" != szVal)
		{
			m_objPairId = atoi(szVal.c_str());
		}
		szVal = cfg.getItem("m_bl");
		if ("" != szVal)
		{
			m_bl = atof(szVal.c_str());
		}
		szVal = cfg.getItem("m_bh");
		if ("" != szVal)
		{
			m_bh = atof(szVal.c_str());
		}
		szVal = cfg.getItem("m_procName");
		if ("" != szVal)
		{
			m_procName = szVal;
		}
		try
		{
			string str = fileadd + "sharp.hobj";
			m_hvReg.GenEmptyRegion();
			ReadRegion(&m_hvReg, str.c_str());          // 读出匹配区域文件
			str = fileadd + "Pose.tup";
			ReadPose(str.c_str(), &hv_Pose);
		}
		catch (HalconCpp::HException& except) {
			AfxMessageBox(except.ErrorText().Text());
		}
	};
	void writecfgfile(string fileadd)
	{
		CConfig cfg(fileadd + ".cfg");
		char sz[32];
		sprintf(sz, "%d", m_type);
		cfg.updateItem("m_type", sz);
		sprintf(sz, "%d", m_objId);
		cfg.updateItem("m_objId", sz);
		sprintf(sz, "%d", m_objPairId);
		cfg.updateItem("m_objPairId", sz);
		sprintf(sz, "%f", m_bl);
		cfg.updateItem("m_bl", sz);
		sprintf(sz, "%f", m_bh);
		cfg.updateItem("m_bh", sz);
		cfg.updateItem("m_procName", m_procName);
		try
		{
			string str = fileadd + "sharp.hobj";
			remove(str.c_str());
			WriteRegion(m_hvReg, str.c_str());          // 匹配区域文件
			str = fileadd + "Pose.tup";
			WritePose(hv_Pose, str.c_str());
		}
		catch (HalconCpp::HException& except) {
			AfxMessageBox(except.ErrorText().Text());
		}
	};
	int m_type;//检测项目 0、高度 1、顶点 2、平面度 3、两平面角度 4、高度差 5、切片 6、平面到点的距离
	HRegion m_hvReg;//Roi
	HTuple hv_Pose;//切片位姿
	float m_bl, m_bh;//误差标准值上下范围
	int m_objId, m_objPairId;//角度 高度差 需要两个目标的id
	float m_dret;//计算结果
	HTuple hv_Value;//单元法向量 0 0 1 表示和水平面的夹角
	HTuple hv_Value2;//平面位姿
	string m_procName;
	string m_szRet;//返回结果字符
};
struct S_CheckParam
{
	S_CheckParam()
	{
		m_vctPa.push_back(S_CheckProb());
		m_vctPa[0].m_procName = CDateTime::GetNow().ToString();
		m_distance = 100;
		m_zMax = 1000, m_zMin = 0;//zmap的范围

		hv_Pose.Clear();
		hv_Pose[0] = 0;
		hv_Pose[1] = 0;
		hv_Pose[2] = 0;
		hv_Pose[3] = 0;
		hv_Pose[4] = 0;
		hv_Pose[5] = 0;
		hv_Pose[6] = 0;

	};
	~S_CheckParam()
	{
	};
	void readcfgfile(string fileadd)
	{
		CConfig cfg(fileadd + ".cfg");
		string szVal = cfg.getItem("num");
		if ("" != szVal)
		{
			int n = atoi(szVal.c_str());
			m_vctPa.resize(n);
			for (int i = 0; i < m_vctPa.size(); i++)
			{
				string filename = fileadd + CStrProcess::itoa_radixN(i);
				m_vctPa[i].readcfgfile(filename);
			}
			//hv_CamParam.ReadTuple((fileadd + "caminner.cal").c_str());

		}
		szVal = cfg.getItem("m_distance");
		if ("" != szVal)
		{
			m_distance = atoi(szVal.c_str());
		}
		szVal = cfg.getItem("m_zMax");
		if ("" != szVal)
		{
			m_zMax = atof(szVal.c_str());
		}
		szVal = cfg.getItem("m_zMin");
		if ("" != szVal)
		{
			m_zMin = atof(szVal.c_str());
		}
	};
	void writecfgfile(string fileadd)
	{
		CConfig cfg(fileadd+".cfg");
		cfg.updateItem("num", CStrProcess::itoa_radixN(m_vctPa.size()));
		cfg.updateItem("m_distance", CStrProcess::itoa_radixN(m_distance));
		for (int i = 0; i < m_vctPa.size(); i++)
		{
			string filename = fileadd + CStrProcess::itoa_radixN(i);
			m_vctPa[i].writecfgfile(filename);
		}
		char sz[32];
		sprintf(sz, "%f", m_zMax);
		cfg.updateItem("m_zMax", sz);
		sprintf(sz, "%f", m_zMin);
		cfg.updateItem("m_zMin", sz);
		//hv_CamParam.ReadTuple((fileadd + "caminner.cal").c_str());
	};
	void clone(S_CheckParam& src)
	{
		hv_CamParam = src.hv_CamParam;
		hv_Pose = src.hv_Pose;
		m_distance = src.m_distance;
		m_zMin = src.m_zMin;
		m_zMax = src.m_zMax;
		m_vctPa.assign(src.m_vctPa.begin(), src.m_vctPa.end());
	};
	HTuple hv_CamParam, hv_Pose;//相机参数
	vector< S_CheckProb> m_vctPa;
	int m_distance;//点云筛选距离
	float m_zMax, m_zMin;//zmap的范围
};
enum eCOMBO_MATCH_ROI_TYPE
{
	ROI_UNITE,
	ROI_AND,
	ROI_SUB,
	ROI_XOR
};
class checkPatch :public CThread
{
public:
	checkPatch();
	~checkPatch();
	virtual int svc();

	//图片的处理	
	//
	bool treatPicture(CTpltParam& pa, vector<S_TpltData*>& vctData);
	//具体的算法分析
	bool treatPic(S_TpltData* data,int treatid);
	bool treatPic();
	bool treatOnce();
	bool treatProb(S_CheckProb& prob, HObject& ho_Zm, HTuple& hv_ObjectModel3DAffineTrans, HTuple* hv_WindowHandle = NULL);
	//画出ng的标识
	bool drawResult(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa);
	bool iniHalWin();
	void readMode(string file="");
	void writeMode(string file="");
	void clone(checkPatch& src);
	bool gen_self_light_adjusting(HTuple& hv_ObjectModel3D, HTuple& hv_ObjectModel3DAffineTrans, HTuple* hv_WindowHandle = NULL);
	double genFitness(HTuple& hv_ObjectModel3D, HRegion& ho_ROI, HTuple& ParamValue, HTuple& ParamValue2);
	void DisProcessROIRegion(HRegion& HXdis, HRegion& HXsrc);
	void DisProcessROImodel();
	// ROI Rectangle2
	void DisOnRoiDrawRectangle2();
	void DisOnRoiDrawCircle();
	void DisOnRoiDrawEllipse();
	void DisOnRoiDrawRectangle();
	void DisOnRoiDrawAnyLine();
	void DisOnMatchDelAllRoi();
	void DisOnElementDelOne();
public:
	S_CheckParam m_pa;
	int m_curPa;
	HImage m_ImageOrg;
	HImage m_deepImage;
	HTuple hv_WindowHandle;
	bool bIsInROIdrawing;                                 // 当前是不是在 ROI 绘图中
	eCOMBO_MATCH_ROI_TYPE iCOMBO_MATCH_ROI_TYPE;          // 当前的类型
	string m_configAdd;//配置文件

	bool m_runing;
	void* m_pDlg;
};

#endif