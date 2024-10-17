#ifndef __CHECKPATCH_H__
#define __CHECKPATCH_H__
/*
功能描述：

*/
#include "automove.h"


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
		int iret = pCheck->procCheck( &msg);
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
class checkPatch
{
public:
	checkPatch();
	~checkPatch();
	void readMode(string dir);
	void writeMode(string dir);

public:
	string m_cfgDir;//配置文件目录
	S_CVCaliParam m_CaliParam;//标定参数
};

#endif
