#ifndef __CHECKPATCH_H__
#define __CHECKPATCH_H__
/*
功能描述：

*/
#include "stdafx.h"
#include "automove.h"
#include "Design_dlg_posDlg.h"
#include <HalconCpp.h>
using namespace HalconCpp;
HImage Mat2HObject(cv::Mat& image);
cv::Mat HObject2Mat(HImage Hobj);

struct S_DeviceParam
{
	S_DeviceParam()
	{
		angleTabId=0;
		m_anglePa = 1;
		m_procAddress="";
	}
	~S_DeviceParam()
	{

	}
	void loadcfg(string cfgDir)
	{
		CConfig cfg(cfgDir+"\\devParam.cfg");
		string szVal = cfg.getItem("procAddress");
		if (""!=szVal)
		{
			m_procAddress = szVal;
		}
		szVal = cfg.getItem("anglePa");
		if ("" != szVal)
		{
			m_anglePa = atof(szVal.c_str());
		}
	};
	void writecfg(string cfgDir)
	{
		CConfig cfg(cfgDir+"\\devParam.cfg");
		cfg.updateItem("\n#制程地址","");
		cfg.updateItem("procAddress",m_procAddress);
		cfg.updateItem("\n#旋转角参数", "");
		char sz[32];
		sprintf(sz, "%f", m_anglePa);
		cfg.updateItem("anglePa", sz);
	}

	string m_procAddress;
	std::vector<float> m_angleTab;//针对阵列运动的旋转方向表
	float m_anglePa;//旋转角参数
	int angleTabId;
};

struct S_CaliParam
{
	S_CaliParam()
	{
		dSh=dSw=-1;
		CaliNum = -1;//9点标定
		pCheck=NULL;
	}
	void loadcfg(string cfgDir)
	{
		string str = cfgDir+"\\caliobjpt";
		CConfig cfg(str);
		string szVal = cfg.getItem("CaliNum");
		if ("" != szVal)
		{		
			object.clear();
			CaliNum = atoi(szVal.c_str());
			szVal = cfg.getItem("object");
			std::vector<string> vct;
			CStrProcess::Split(szVal,",",vct,true);
			for (int i=0;i<CaliNum;i++)
			{
				int id = i*2;
				object.push_back(cv::Point2f(atof(vct[id].c_str()),atof(vct[id+1].c_str())));
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
		string str = cfgDir+"\\caliobjpt";
		CConfig cfg(str);
		cfg.updateItem("CaliNum",CStrProcess::itoa_radixN(CaliNum));
		char sz[64];
		string szVal="";
		for (int i=0;i<CaliNum;i++)
		{
			sprintf(sz,"%f,%f,",object[i].x,object[i].y);
			szVal += sz;
		}
		cfg.updateItem("object",szVal);
		sprintf(sz,"%f",dSh);
		cfg.updateItem("Sh",sz);
		sprintf(sz,"%f",dSw);
		cfg.updateItem("Sw",sz);
	};
	void setObj(std::vector<cv::Point2f>& obj)
	{
		object.swap(obj);
		CaliNum=object.size();
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
		if (CaliNum!=scene.size())
		{
			return false;
		}
		dSh=dSw=0;
		double dx,dy;
		for (int i=1;i<CaliNum;i++)
		{
			dx = scene[i].x-scene[i-1].x;
			dy = scene[i].y-scene[i-1].y;
			dSh += sqrt(dx*dx+dy*dy);
			dx = object[i].x-object[i-1].x;
			dy = object[i].y-object[i-1].y;
			dSw += sqrt(dx*dx+dy*dy);
		}
		dx = scene[0].x-scene[CaliNum-1].x;
		dy = scene[0].y-scene[CaliNum-1].y;
		dSh += sqrt(dx*dx+dy*dy);
		dx = object[0].x-object[CaliNum-1].x;
		dy = object[0].y-object[CaliNum-1].y;
		dSw += sqrt(dx*dx+dy*dy);

		dSw /= dSh;
		dSh = dSw;
		return true;
	};
	CProcDll* pCheck;
	std::vector<cv::Point2f> scene;//标定板目标像素位置
	std::vector<cv::Point2f> object;//实际距离
	int CaliNum;//标定点数
	double dSh,dSw;//像素点距离

};

struct S_HaCaliParam:S_CaliParam
{
	S_HaCaliParam()
	{

	};
	~S_HaCaliParam()
	{

	};
	void loadcfg(string cfgDir)
	{  
		//读取变换矩阵
		string str = cfgDir+"\\calihom2d.mat";
		if(0 == rename(str.c_str(),"./temp.mat") )
		{		
			HalconCpp::HTuple  FileName, FileHandle, SerializedItemHandle;
			FileName = "./temp.mat";
			OpenFile(FileName, "input_binary", &FileHandle); 
			FreadSerializedItem(FileHandle, &SerializedItemHandle); 
			DeserializeHomMat2d(SerializedItemHandle,&HomMat2D ); 
			CloseFile(FileHandle);
			rename("./temp.mat",str.c_str());
		}
		S_CaliParam::loadcfg(cfgDir);
	}
	void writecfg(string cfgDir)
	{
		//保存变换矩阵
		if (HomMat2D.Length()>0)
		{
			HalconCpp::HTuple  FileName, FileHandle, SerializedItemHandle;
			string str = cfgDir+"\\calihom2d.mat";
			remove(str.c_str());
			FileName = "./temp.mat";
			OpenFile(FileName, "output_binary", &FileHandle); 
			SerializeHomMat2d(HomMat2D, &SerializedItemHandle);
			FwriteSerializedItem(FileHandle, SerializedItemHandle); 
			CloseFile(FileHandle);
			rename("./temp.mat",str.c_str());
		}
		S_CaliParam::writecfg(cfgDir);
	}
	bool getDistance(void* p1x,void* p1y,void* p2x,void* p2y)
	{
		if (HomMat2D.Length()==0)
		{
			return false;
		}
		AffineTransPoint2d(HomMat2D,*(HTuple*)p1y, *(HTuple*)p1x,(HTuple*)p2y,(HTuple*)p2x);
		return true;
	}
	bool setHomMat()
	{
		if (CaliNum!=scene.size())
		{
			return false;
		}
		HTuple hv_x,hv_y;//实际距离
		HTuple Row, Column;
		for (int i=0;i<CaliNum;i++)
		{
			hv_x[i] = object[i].x;
			hv_y[i] = object[i].y;
			Row[i] = scene[i].x;
			Column[i]= scene[i].y;
		}
		try {
			VectorToHomMat2d(Row, Column, hv_y, hv_x, &HomMat2D);
		}
		catch(...)
		{
			return false;
		}
		return true;
	};
	
	HalconCpp::HTuple HomMat2D;           //==  ROI 变换矩阵
};
struct S_CVCaliParam:S_CaliParam
{
	S_CVCaliParam()
	{
	};
	~S_CVCaliParam()
	{

	};
	bool getDistance(void* p1x,void* p1y,void* p2x,void* p2y)
	{
		if (H.empty())
		{
			return false;
		}
		std::vector<cv::Point2f> obj_corners(1);
		obj_corners[0] = cv::Point2f(*(float*)p1x,*(float*)p1y);
		std::vector<cv::Point2f> scene_corners(1);

		perspectiveTransform( obj_corners, scene_corners, H);
		*(float*)p2x = scene_corners[0].x;
		*(float*)p2y = scene_corners[0].y;
		return true;
	};
	void loadcfg(string cfgDir)
	{
		//读取变换矩阵
		string str = cfgDir+"\\calihom2d.mat";
		FileStorage fs(str,FileStorage::READ);
		fs["H"]>>H;
		fs.release();
		S_CaliParam::loadcfg(cfgDir);
	};
	void writecfg(string cfgDir)
	{
		//保存变换矩阵
		string str = cfgDir+"\\calihom2d.mat";
		FileStorage fs(str,FileStorage::WRITE);
		fs<<"H"<<H;
		fs.release();
		S_CaliParam::writecfg(cfgDir);
	};
	
	bool setHomMat()
	{
		if (CaliNum!=scene.size())
		{
			return false;
		}
		
		H = findHomography( object,scene,  cv::RANSAC  );
		return true;
	};
	cv::Mat H;//变换矩阵
};

struct S_CheckParam : public CConfig
{
	S_CheckParam(string szFile) :CConfig(szFile) 
	{

	};
	void readcfgfile(UCI::OptionsMap& omp)
	{
		std::vector< pair<string, string> >::iterator it = m_vctKeyVal.begin();
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

	void readMode(string dir="");
	void writeMode( string dir="");

	bool EditModel(cv::Mat& src);
	bool checkLocateCheck(HImage& ho_Image0);
	
public:
	string m_cfgDir;//配置文件目录
	CList <UCI::OptionsMap, UCI::OptionsMap> m_EleList;//标定定位制程
	std::vector <S_Model*> m_modelList;
	static CDesign_dlg_posDlg S_dlg;
	S_DeviceParam m_DevParam;//设备参数
	S_HaCaliParam m_CaliParam;//标定参数

	cv::Mat m_rawPic;
	CTemplateProc m_Proc;//阵列制程
	bool loadProc(string szdir);
};

#endif