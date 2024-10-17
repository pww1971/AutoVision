#ifndef __CHECKPATCH_H__
#define __CHECKPATCH_H__

#include "stdafx.h"
#include "CPaddleOcrParamDlg.h"

struct OCRPredictResult {
	std::vector<std::vector<int>> box;
	std::string text;
	float score = -1.0;
	float cls_score;
	int cls_label = -1;
};

class checkPatch : public DynaLink
{
public:
	checkPatch();
	~checkPatch();
	//name 动态库的文件名
	bool Load(string dllname= "ppocr");
	void readMode(string file);
	void writeMode(string file);
	bool getMapIni();
	//图片的处理	
	bool treatPicture(CTpltParam& pa, vector<S_TpltData*>& vctData);
	//画出ng的标识
	bool drawResult(cv::Mat& src, cv::RotatedRect& roiRect, CTpltParam& pa);
	bool (*Proc_main_ini)(map<string, string>& config, void*& p);
	bool (*Proc_ImageProcess)(cv::Mat& m, std::vector<OCRPredictResult>& ocr_results, int tp, void*& p);
	void (*Proc_main_exit)(void*& p);
	bool isLoadedOK;
	//CriticalSection processLock;
	string det_model_dir;
	string rec_model_dir;
	bool use_angle_cls;
	string cls_model_dir;
	string rec_char_dict_path;
	bool use_polygon_score;
	float det_db_thresh;// , 0.3, "Threshold of det_db_thresh.");
	float det_db_box_thresh;// , 0.5, "Threshold of det_db_box_thresh.");
	int m_typeid;//检测分类识别;检测识别;检测;分类识别;识别;分类
	string m_szRule;//正则化表达式
	float m_iniAngle;//图片正向识别效果好，因此预处理需要旋转图片
	void* m_pOcr;//ocr
};

#endif
