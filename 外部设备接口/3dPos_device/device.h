#ifndef __CHECKPATCH_H__
#define __CHECKPATCH_H__
/*
功能描述：

*/
#include "automove.h"
#include "threeDMatch.h"
#include <HalconCpp.h>
using namespace HalconCpp;
HObject MatToHImage16Bit(Mat& imgMat);
Mat HImageToMat16Bit(HObject& imgHalcon);
HImage Mat2HObject(cv::Mat& image);
cv::Mat HObject2Mat(HImage& Hobj);
class S_CaliParam
{
public:
	S_CaliParam()
	{
		dSh = dSw = -1;
		CaliNum = -1;//9点标定
		pCheck = NULL;
	}
public:
	void loadcfg(string cfgDir)
	{
		string str = cfgDir + "\\caliobjpt";
		CConfig cfg(str);
		string szVal = cfg.getItem("CaliNum");
		if ("" != szVal)
		{
			object.clear();
			CaliNum = atoi(szVal.c_str());
			szVal = cfg.getItem("object");
			std::vector<string> vct;
			CStrProcess::Split(szVal, ",", vct, true);
			for (int i = 0; i < CaliNum; i++)
			{
				int id = i * 2;
				object.push_back(cv::Point2f(atof(vct[id].c_str()), atof(vct[id + 1].c_str())));
			}
		}
		szVal = cfg.getItem("Sh");
		if ("" != szVal)
		{
			dSh = atof(szVal.c_str());
		}
		szVal = cfg.getItem("Sw");
		if ("" != szVal)
		{
			dSw = atof(szVal.c_str());
		}
	};
	void writecfg(string cfgDir)
	{
		string str = cfgDir + "\\caliobjpt";
		CConfig cfg(str);
		cfg.updateItem("CaliNum", CStrProcess::itoa_radixN(CaliNum));
		char sz[64];
		string szVal = "";
		for (int i = 0; i < CaliNum; i++)
		{
			sprintf(sz, "%f,%f,", object[i].x, object[i].y);
			szVal += sz;
		}
		cfg.updateItem("object", szVal);
		sprintf(sz, "%f", dSh);
		cfg.updateItem("Sh", sz);
		sprintf(sz, "%f", dSw);
		cfg.updateItem("Sw", sz);
	};
	void setObj(std::vector<cv::Point2f>& obj)
	{
		object.swap(obj);
		CaliNum = object.size();
	};
	bool getScenePos()
	{
		S_Msg msg;
		if (-1 == pCheck->camPic(pCheck->m_curCamId, msg.vm))
		{
			return false;
		}
		scene.clear();
		//加载制程和检测另外写，制程过程还是界面完成。
		msg.m_procid = pCheck->m_picProcId;
		int iret = pCheck->procCheck(&msg);
		if (1 == iret)//ok
		{
			std::vector<string> vct;
			CStrProcess::Split(msg.str, "|", vct);
			for (int ik = 1; ik < vct.size(); ik++)
			{
				std::vector<string> vct2;
				CStrProcess::Split(msg.str, ",", vct2);
				if (vct2.size() != 3)
				{
					continue;
				}
				float m_posX = atof(vct2[0].c_str());
				float m_posY = atof(vct2[1].c_str());
				scene.push_back(cv::Point2f(m_posX, m_posY));
			}
			return true;
		}
		return false;
	};
	bool getUs()//计算单个像素距离
	{
		if (CaliNum != scene.size())
		{
			return false;
		}
		dSh = dSw = 0;
		double dx, dy;
		for (int i = 1; i < CaliNum; i++)
		{
			dx = scene[i].x - scene[i - 1].x;
			dy = scene[i].y - scene[i - 1].y;
			dSh += sqrt(dx * dx + dy * dy);
			dx = object[i].x - object[i - 1].x;
			dy = object[i].y - object[i - 1].y;
			dSw += sqrt(dx * dx + dy * dy);
		}
		dx = scene[0].x - scene[CaliNum - 1].x;
		dy = scene[0].y - scene[CaliNum - 1].y;
		dSh += sqrt(dx * dx + dy * dy);
		dx = object[0].x - object[CaliNum - 1].x;
		dy = object[0].y - object[CaliNum - 1].y;
		dSw += sqrt(dx * dx + dy * dy);

		dSw /= dSh;
		dSh = dSw;
		return true;
	};
	virtual bool setHomMat() = NULL;
	CProcDll* pCheck;
	std::vector<cv::Point2f> scene;//标定板目标像素位置
	std::vector<cv::Point2f> object;//实际距离
	int CaliNum;//标定点数
	double dSh, dSw;//像素点距离
};

class S_CVCaliParam :public S_CaliParam
{
public:
	S_CVCaliParam()
	{
	};
	~S_CVCaliParam()
	{

	};
	bool getDistance(void* p1x, void* p1y, void* p2x, void* p2y)
	{
		if (H.empty())
		{
			return false;
		}
		std::vector<cv::Point2f> obj_corners(1);
		obj_corners[0] = cv::Point2f(*(float*)p1x, *(float*)p1y);
		std::vector<cv::Point2f> scene_corners(1);

		perspectiveTransform(obj_corners, scene_corners, H);
		*(float*)p2x = scene_corners[0].x;
		*(float*)p2y = scene_corners[0].y;
		return true;
	};
	void loadcfg(string cfgDir)
	{
		//读取变换矩阵
		string str = cfgDir + "\\calihom2d.mat";
		FileStorage fs(str, FileStorage::READ);
		fs["H"] >> H;
		fs.release();
		S_CaliParam::loadcfg(cfgDir);
	};
	void writecfg(string cfgDir)
	{
		//保存变换矩阵
		string str = cfgDir + "\\calihom2d.mat";
		FileStorage fs(str, FileStorage::WRITE);
		fs << "H" << H;
		fs.release();
		S_CaliParam::writecfg(cfgDir);
	};

	bool setHomMat()
	{
		if (CaliNum != scene.size())
		{
			return false;
		}

		H = findHomography(object, scene, cv::RANSAC);
		return true;
	};
	cv::Mat H;//变换矩阵
};

struct S_CheckParam
{
	S_CheckParam() 
	{
		m_iniSucc = false;
		RelSamplingDistance1 = 0.03;
		RelSamplingDistance2 = 0.05;
		KeyPointFraction = 0.15;
		MinScore = 0.3;
		GenParamValue = 10;
		bgL = 0;
		bgH = 1;
		m_winH = 100;
		m_winW = 100;
		ho_ModelRegion.GenEmptyRegion();
		hv_Pose.Clear();
		hv_Pose[0] = 0;
		hv_Pose[1] = 0;
		hv_Pose[2] = 0;
		hv_Pose[3] = 0;
		hv_Pose[4] = 0;
		hv_Pose[5] = 0;
		hv_Pose[6] = 0;
		m_pose = 0;
		m_grapsize = 40;
		hv_CamParam.Clear();
		hv_CamParam[0] = 0.01;//焦距 m
		hv_CamParam[1] = 0;//Kappa(-1/m^2)
		hv_CamParam[2] = 7e-6;//单个像素的宽（m）
		hv_CamParam[3] = 7e-6;//单个像素的高（m）
		hv_CamParam[4] = 220;//中心点x坐标(像素)
		hv_CamParam[5] = 220;
		hv_CamParam[6] = 440;//像素宽
		hv_CamParam[7] = 440;

	}
	~S_CheckParam()
	{
		
	}
	void readMode(CConfig& cfg);
	void writeMode(CConfig& cfg);
	void iniMode();
	
	//模型文件 
	string modelFile;
	float RelSamplingDistance1, RelSamplingDistance2;
	float KeyPointFraction;
	float MinScore;
	int GenParamValue;
	float bgL, bgH;//过滤背景 
	float m_winH, m_winW;//视图宽高 输出位姿校正为实际位置用
	bool m_iniSucc;//初始化成功
	string m_procName;
	HTuple  hv_ObjectModel3DModel;
	HTuple  hv_SFM;
	HRegion ho_ModelRegion;
	HTuple hv_Pose;//机械手的抓取点
	int m_pose;//抓取面 //正面;反面;后侧面;前侧面;左面;右面
	float m_grapsize;//抓取点显示大小
	HTuple hv_CamParam;//相机参数
};
enum eCOMBO_MATCH_ROI_TYPE
{
	ROI_UNITE,
	ROI_AND,
	ROI_SUB,
	ROI_XOR
};
class CDevice :public CThread
{
public:
	CDevice();
	~CDevice();
	void clearMode();
	virtual int svc()
	{//halcon新版本visualize_object_model_3d里面无法刷新点云 因此必须在线程里完成
		m_runing = true;

		int sign = *(int*)m_thread_args;
		if (1 == sign)
			set3dTran(false);
		else if (2 == sign)
			openDevWin();
		else if (3 == sign)
			LocateCheck(m_curId);
		else
			;
		stop();
		m_runing = false;
		return true;
	};

	void readMode(string file="");
	void writeMode( string file="");

	bool EditModel();
	bool openDevWin(bool ini=false);
	bool LocateCheck( int modeId);
	bool checkLocateCheck(S_Msg& msg, int modeId);

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
	void set3dTran(bool bsel = true);
	void setPoseRota(int p);
	void setPoseX(float val);
	void setPoseY(float val);
	void setPoseZ(float val);
public:
	string m_cfgDir;//配置文件目录
	std::vector<S_CheckParam> m_vctDLPa;
	static threeDMatch S_dlg;
	int m_curId;//当前制程id
	void iniMode();

	HTuple  hv_WindowHandle;
	std::vector<HImage>   m_hImage;

	bool bIsInROIdrawing;                                 // 当前是不是在 ROI 绘图中

	eCOMBO_MATCH_ROI_TYPE iCOMBO_MATCH_ROI_TYPE;          // 当前的类型
	S_CVCaliParam m_CaliParam;//标定参数

	bool m_runing;
};

#endif