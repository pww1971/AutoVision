#include "checkPatch.h"
#include <regex>
class ocrConfig : public CConfig
{
public:
	ocrConfig(string szFile) :CConfig(szFile)
	{
	};
	~ocrConfig() {};
	void getmap(map<string, string>& config)
	{
		config.clear();
		vector< pair<string, string> >::iterator it = m_vctKeyVal.begin();
		for (; it != m_vctKeyVal.end(); ++it)
		{
			string szKeyF = (*it).first;
			string szValF = (*it).second;
			config.insert(pair<string, string>(szKeyF, szValF));
		}
	}
};

checkPatch::checkPatch()
{
	Proc_main_exit = NULL;
	isLoadedOK = false;
	det_db_thresh = 0.3;
	det_db_box_thresh = 0.5;
	m_typeid = 0;
	m_szRule = "";
	m_iniAngle = 0;
	m_pOcr = NULL;
}

checkPatch::~checkPatch()
{
	if (isLoadedOK)
		Proc_main_exit(m_pOcr);
}
bool checkPatch::Load(string dllname)
{
	//		WaitAndSignal m(processLock);
	if (IsLoaded())
		return true;
#if defined(WIN32)
	dllname += ".dll";
#else
	dllname += ".so";
#endif
	if (!DynaLink::Open(dllname.c_str())) {
		viewErr(dllname + "\tFailed to load a library");
		return false;
	}
	if (!GetFunction("main_ini", (Function&)Proc_main_ini)) {
		viewErr(dllname + "\tFailed to load Proc_main_ini");
		return false;
	}
	if (!GetFunction("ImageProcess", (Function&)Proc_ImageProcess)) {
		viewErr(dllname + "\tFailed to load Proc_ImageProcess");
		return false;
	}
	if (!GetFunction("main_exit", (Function&)Proc_main_exit)) {
		viewErr(dllname + "\tFailed to load Proc_main_exit");
		return false;
	}

	isLoadedOK = true;
	return true;
}
void checkPatch::readMode(string file)
{
	if (file != "")
	{
		mkdirs((char*)file.c_str());
	}

	ocrConfig cfg(file + "/PaddleOCR.cfg");
	map<string, string> config;

	cfg.getmap(config);
	try{
		Proc_main_ini(config, m_pOcr);
	}
	catch(...)
	{
		return;
	}
	string modePath = getExePath();
	map<string, string>::iterator it = config.find("det_model_dir");
	if (it != config.end())
		det_model_dir = it->second;
	else
		det_model_dir = modePath + "model/3_xx/en/en_PP-OCRv3_det_infer";
	it = config.find("use_polygon_score");
	if (it != config.end())
		use_polygon_score = atoi(it->second.c_str());
	else
		use_polygon_score = false;
	it = config.find("rec_model_dir");
	if (it != config.end())
		rec_model_dir = it->second;
	else
		rec_model_dir = modePath + "model/3_xx/en/en_PP-OCRv3_rec_infer";
	it = config.find("rec_char_dict_path");
	if (it != config.end())
		rec_char_dict_path = it->second;
	else
		rec_char_dict_path = modePath + "model/3_xx/utils/en_dict.txt";

	use_angle_cls = false;
	it = config.find("use_angle_cls");
	if (it != config.end())
		use_angle_cls = atoi(it->second.c_str());
	if (use_angle_cls) {
		it = config.find("cls_model_dir");
		if (it != config.end())
			cls_model_dir = it->second;
		else
			cls_model_dir = modePath + "model/3_xx/en/ch_ppocr_mobile_v2.0_cls_infer";
	}
	it = config.find("det_db_thresh");
	if (it != config.end())
		det_db_thresh = atof(it->second.c_str());
	it = config.find("det_db_box_thresh");
	if (it != config.end())
		det_db_box_thresh = atof(it->second.c_str());
	it = config.find("m_typeid");
	if (it != config.end())
		m_typeid = atoi(it->second.c_str());
	it = config.find("m_szRule");
	if (it != config.end())
		m_szRule = it->second;
	it = config.find("m_iniAngle");
	if (it != config.end())
		m_iniAngle = atof(it->second.c_str());
}

void checkPatch::writeMode(string file)
{
	pww_mkdir(file.c_str());
	file = file + "/PaddleOCR.cfg";
	remove(file.c_str());
	CConfig cfg(file);
	cfg.updateItem("#", "检测定位模型文件");
	cfg.updateItem("det_model_dir", det_model_dir);
	cfg.updateItem("#", "识别模型文件");
	cfg.updateItem("rec_model_dir", rec_model_dir);
	cfg.updateItem("#", "方向分类模型文件");
	cfg.updateItem("cls_model_dir", cls_model_dir);
	cfg.updateItem("#", "字典路径");
	cfg.updateItem("rec_char_dict_path", rec_char_dict_path);
	if (use_angle_cls)
		cfg.updateItem("use_angle_cls", "1");
	else
		cfg.updateItem("use_angle_cls", "0");
	if (use_polygon_score)
		cfg.updateItem("use_polygon_score", "1");
	else
		cfg.updateItem("use_polygon_score", "0");
	char sz[32];
	sprintf(sz, "%f", det_db_thresh);
	cfg.updateItem("det_db_thresh", sz);
	sprintf(sz, "%f", det_db_box_thresh);
	cfg.updateItem("det_db_box_thresh", sz);
	sprintf(sz, "%d", m_typeid);
	cfg.updateItem("m_typeid", sz);
	cfg.updateItem("#", "正则化表达式");
	cfg.updateItem("m_szRule", m_szRule);
	sprintf(sz, "%f", m_iniAngle);
	cfg.updateItem("m_iniAngle", sz);
}
bool checkPatch::getMapIni()
{
	map<string, string> config;
	config.insert(pair<string, string>("det_model_dir", det_model_dir));
	config.insert(pair<string, string>("rec_model_dir", rec_model_dir));
	config.insert(pair<string, string>("cls_model_dir", cls_model_dir));
	config.insert(pair<string, string>("rec_char_dict_path", rec_char_dict_path));
	if (use_angle_cls)
		config.insert(pair<string, string>("use_angle_cls", "1"));
	else
		config.insert(pair<string, string>("use_angle_cls", "0"));
	if (use_polygon_score)
		config.insert(pair<string, string>("use_polygon_score", "1"));
	else
		config.insert(pair<string, string>("use_polygon_score", "0"));
	
	char sz[32];
	sprintf(sz, "%f", det_db_thresh);
	config.insert(pair<string, string>("det_db_thresh", sz));
	sprintf(sz, "%f", det_db_box_thresh);
	config.insert(pair<string, string>("det_db_box_thresh", sz));
	sprintf(sz, "%d", m_typeid);
	config.insert(pair<string, string>("m_typeid", sz));	
	config.insert(pair<string, string>("m_szRule", m_szRule));
	try {
		return Proc_main_ini(config, m_pOcr);
	}
	catch (...)
	{
		return false;
	}
	
}

//图片的处理	一保存模板图片，二加载模板图片进行旋转角度后匹配
bool checkPatch::treatPicture(CTpltParam& pa, vector<S_TpltData*>& vctData)
{
	int curid = pa.m_tempPa.m_curPosID;
	cv::Mat& src = vctData[curid]->m_vctPic[0];
	if (!src.data)
		return false;
	if (pa.m_dllPa.ini < 1)
	{
		if(!Load())
			return false;
		if ("" != pa.m_dllPa.szfile)
		{
			readMode(pa.m_dllPa.szfile);
		}
		if (pa.m_dllPa.ini == 0)
		{
			;
		}
		else if (pa.m_dllPa.ini == -1)
		{
			CPaddleOcrParamDlg S_dlg;
			S_dlg.m_pCheck = this;
			S_dlg.m_pic = src;
			INT_PTR nResponse = S_dlg.DoModal();
			if (nResponse == IDOK)
			{
				writeMode(pa.m_dllPa.szfile);
			}
			else// if (nResponse == IDCANCEL)
			{
				return false;
			}
		}

		pa.m_dllPa.ini = 1;
		return true;
	}
	else
	{
		vector<OCRPredictResult> ocr_results;
		S_MosaicParam pa2;
		pa2.dScale = 1;
		pa2.iCenterX = src.cols / 2;			//就取中点
		pa2.iCenterY = src.rows / 2;
		pa2.dAngle = m_iniAngle* PI / 180;//度
		cv::Mat dst = imageRotate(src, pa2);		//旋转后图像会放大
		try
		{
			Proc_ImageProcess(dst, ocr_results, m_typeid,m_pOcr);
		}
		catch (...)
		{
		}
		
		if (2 == m_typeid)//检测 定位
		{
			int retNum = 0;
			for (int i = 0; i < ocr_results.size(); i++)
			{
				std::vector<std::vector<int>> box = ocr_results[i].box;
				if (box.size() >= 4)
					retNum++;
			}
			int NumMatchesLimit = pa.iRegNumMax;
			if (NumMatchesLimit > retNum) {
				for (int pi = vctData.size(); pi < ocr_results.size(); pi++)
					vctData.push_back(new S_TpltData());
				char sz[128];
				for (int i = 0; i < ocr_results.size(); i++)
				{
					std::vector<std::vector<int>> box = ocr_results[i].box;
					if (box.size() < 4) {
						continue;
					}

					int x_collect[4] = { box[0][0], box[1][0], box[2][0], box[3][0] };
					int y_collect[4] = { box[0][1], box[1][1], box[2][1], box[3][1] };
					int left = int(*std::min_element(x_collect, x_collect + 4));
					int right = int(*std::max_element(x_collect, x_collect + 4));
					int top = int(*std::min_element(y_collect, y_collect + 4));
					int bottom = int(*std::max_element(y_collect, y_collect + 4));

					cv::Rect ret(left, top, right - left, bottom - top);
					vctData[i]->m_roiRect.center.x = left + ret.width / 2;
					vctData[i]->m_roiRect.center.y = top + ret.height / 2;
					vctData[i]->m_roiRect.size.width = ret.width;
					vctData[i]->m_roiRect.size.height = ret.height;
					
					vctData[i]->m_roiRect.angle = fastAtan2((box[1][1]- box[0][1]),(box[1][0]- box[0][0]));
					if (ocr_results[i].cls_label != -1)
					{
						vctData[i]->m_roiRect.angle += ocr_results[i].cls_label;
					}
					ret &= cv::Rect(0, 0, dst.cols, dst.rows);
					sprintf(sz, "Box[%d]: x=%d, y=%d, w=%d, h=%d, angle=%f",
						i, left, top, ret.width, ret.height, vctData[i]->m_roiRect.angle);//输出坐标框的信息
					vctData[i]->m_szRet = sz;
					// rec
					if (ocr_results[i].score != -1.0) {
						vctData[i]->m_szRet += "\r\n rec text: " + ocr_results[i].text;
						sprintf(sz, "\r\n rec score: %f" , ocr_results[i].score);
						vctData[i]->m_szRet += sz; 
					}

					vctData[i]->m_vctPic[0] = cv::Mat(dst, ret).clone();
					vctData[i]->m_checkRet = true;
				}
				return true;
			}
		}
		else//识别
		{			
			vctData[curid]->m_szRet = "";
			for (int i = 0; i < ocr_results.size(); i++)
			{
				if (ocr_results[i].score != -1.0)
					vctData[curid]->m_szRet += ocr_results[i].text;
			}
			vctData[curid]->m_szRet = CStrProcess::Trim(vctData[curid]->m_szRet, " \t\f\v\n\r");
			//配置文件的匹配规则做匹配
			if (vctData[curid]->m_szRet != "")
			{
				//if ("" != m_szRule)
				//{
				//	std::smatch match;
				//	regex r(m_szRule);
				//	if (!std::regex_search(vctData[curid]->m_szRet, match, r, regex_constants::match_default)) //regex_search "(C562415)(\\dddd)"  头文字C562415和末尾四个数字
				//		return false;
				//}
				if ("" != m_szRule)
				{
					regex r(m_szRule);
					if (!std::regex_match(vctData[curid]->m_szRet,  r)) //regex_search "(C562415)(\\dddd)"  头文字C562415和末尾四个数字
						return false;
				}
				vctData[curid]->m_checkRet = true;
				return true;
			}
		}
		
		return false;
	}

	return false;
}

//画出ng的标识
bool checkPatch::drawResult(cv::Mat& src, cv::RotatedRect& roiRect, CTpltParam& pa)
{
	return true;
}