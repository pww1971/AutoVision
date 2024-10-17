#include "checkPatch.h"
#include "MyData.h"
CDesign_dlg_posDlg checkPatch::S_dlg;
//CThreadMgr g_threadMgr;
HImage Mat2HObject(cv::Mat& image)
{
	HImage Hobj = HImage();
	if (NULL == image.data)
		return Hobj;
	int h = image.rows;
	int w = image.cols;
	//	int al=h*w;
	//	CV_8UC3
	if (image.type() == CV_8UC3)
	{
		cv::Mat imgchannel[3];
		split(image, imgchannel);
		cv::Mat imgB = imgchannel[0];
		cv::Mat imgG = imgchannel[1];
		cv::Mat imgR = imgchannel[2];

		try
		{
			GenImage3(&Hobj, "byte", (Hlong)w, (Hlong)h, (Hlong)imgR.data, (Hlong)imgG.data, (Hlong)imgB.data);
		}
		catch (...)
		{
			//ofstream fp;
			//fp.open("./err.txt",ios::app);
			//fp << "halcon GenImage3 err" << /*"   " << (*(HTuple*)pa.m_dllPa.p).ToString()<<*/endl;
			//fp.close();
		}

	}
	//	CV_8UCU1
	else if (image.type() == CV_8UC1)
	{
		try
		{
			GenImage1(&Hobj, "byte", (Hlong)w, (Hlong)h, (Hlong)image.data);
		}
		catch (...)
		{
			//ofstream fp;
			//fp.open("./err.txt",ios::app);
			//fp << "halcon GenImage1 err" << /*"   " << (*(HTuple*)pa.m_dllPa.p).ToString()<<*/endl;
			//fp.close();
		}

	}
	return Hobj;
}
cv::Mat HObject2Mat(HImage Hobj)
{
	HTuple htCh = HTuple();
	HTuple cType;
	cv::Mat Image;
	ConvertImageType(Hobj, &Hobj, "byte");
	CountChannels(Hobj, &htCh);
	HTuple wid;
	HTuple hgt;
	int W, H;
	if (htCh[0].I() == 1)
	{
		HTuple ptr;
		GetImagePointer1(Hobj, &ptr, &cType, &wid, &hgt);
		W = wid[0].I();
		H = hgt[0].I();
		Image.create(H, W, CV_8UC1);
		uchar* pdata = (uchar*)(Hlong)ptr;
		memcpy(Image.data, pdata, W * H);
	}
	else if (htCh[0].I() == 3)
	{
		HTuple ptrR, ptrG, ptrB;
		GetImagePointer3(Hobj, &ptrR, &ptrG, &ptrB, &cType, &wid, &hgt);
		W = wid[0].I();
		H = hgt[0].I();

		Image.create(H, W, CV_8UC3);
		vector<cv::Mat> vecM(3);
		vecM[2].create(H, W, CV_8UC1);
		vecM[1].create(H, W, CV_8UC1);
		vecM[0].create(H, W, CV_8UC1);
		uchar* pr = (uchar*)(Hlong)ptrR;
		uchar* pg = (uchar*)(Hlong)ptrG;
		uchar* pb = (uchar*)(Hlong)ptrB;
		memcpy(vecM[2].data, pr, W * H);
		memcpy(vecM[1].data, pg, W * H);
		memcpy(vecM[0].data, pb, W * H);
		merge(vecM, Image);
	}
	return Image;
}

checkPatch::checkPatch()
{
	//	SetSystem("border_shape_models", "false");
	//	SetSystem("store_empty_region", "false");
}

checkPatch::~checkPatch()
{
	clearMode();
}

void checkPatch::clearMode()
{
	if (!m_EleList.IsEmpty())
		m_EleList.RemoveAll();
	for (int i = 0; i < m_modelList.size(); i++)
	{
		m_modelList[i]->ho_ModelRegion.GenEmptyObj();
		m_modelList[i]->hv_Model.Clear();
		m_modelList[i]->hv_nccModel.Clear();
		delete m_modelList[i];
	}
	m_modelList.clear();
}

void checkPatch::readMode(string file)
{
	clearMode();
	if (file != "")
	{
		pww_mkdir(file.c_str());
		m_cfgDir = file;
	}
	S_CheckParam cfg0(m_cfgDir + "\\checkParam.cfg");
	string szVal = cfg0.getItem("optCount");
	if ("" != szVal)
	{
		int count = atoi(szVal.c_str()) + 1;
		char sz[32];
		for (int i = 1; i < count; i++)
		{
			sprintf(sz, "%d", i);
			S_CheckParam cfg(m_cfgDir + "\\checkParam" + sz + ".cfg");
			UCI::OptionsMap omp;
			cfg.readcfgfile(omp);
			m_EleList.AddTail(omp);
			int modeType = omp["匹配模型"];
			S_Model* model = new S_Model();
			UCI::pDis->hv_Model = model;
			if (0 == modeType)
				model->hv_Model.ReadShapeModel((m_cfgDir + "\\model" + sz).c_str());       // 读出模板文件
			else
				model->hv_nccModel.ReadNccModel((m_cfgDir + "\\model" + sz).c_str());
			model->ho_ModelRegion.ReadRegion((m_cfgDir + "\\region" + sz + ".hobj").c_str());          // 读出匹配区域文件
			m_modelList.push_back(model);
		}
	}
}

void checkPatch::writeMode(string file)
{
	if (m_modelList.size() < 1)
	{
		return;
	}
	if (file != "")
	{
		pww_mkdir(file.c_str());
		m_cfgDir = file;
	}
	delCfgFiles(m_cfgDir.c_str(), 1);

	char sz[32];
	S_CheckParam cfg0(m_cfgDir + "\\checkParam.cfg");
	sprintf(sz, "%d", m_EleList.GetCount());
	cfg0.updateItem("optCount", sz);

	int i = 1;
	POSITION pos = m_EleList.GetHeadPosition();
	while (pos != NULL) {
		UCI::OptionsMap* o = &m_EleList.GetNext(pos);

		sprintf(sz, "%d", i);
		S_CheckParam cfg(m_cfgDir + "\\checkParam" + sz + ".cfg");
		cfg.writecfgfile(o);
		int modeType = (*o)["匹配模型"];
		S_Model* model = m_modelList[i - 1];
		UCI::pDis->hv_Model = model;
		string str = m_cfgDir + "\\model" + sz;
		remove(str.c_str());
		if (0 == modeType)
			model->hv_Model.WriteShapeModel(str.c_str());       // 读出模板文件
		else
			model->hv_nccModel.WriteNccModel(str.c_str());
		str = m_cfgDir + "\\region" + sz + ".hobj";
		remove(str.c_str());
		model->ho_ModelRegion.WriteRegion(str.c_str());          // 读出匹配区域文件
		i++;
	}
}


bool checkPatch::EditModel(cv::Mat& src)
{
	UCI::ELE.EleList = &m_EleList;
	UCI::pDis->m_sIPResult.result_img = UCI::pDis->GrabImageDisp = Mat2HObject(src);
	UCI::OPTIONS["图像宽度"] = src.cols;
	UCI::OPTIONS["图像高度"] = src.rows;
	UCI::OPTIONS["图像宽长比"] = (double)src.rows / src.cols;

	S_dlg.m_pCheck = this;
	S_dlg.m_hasIni = false;
	INT_PTR nResponse = S_dlg.DoModal();
	if (nResponse == IDOK)
	{
		return true;
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	return false;
}

bool checkPatch::checkLocateCheck(cv::Mat& src, vector<S_TpltData*>& vctData, CTpltParam& Pa)
{
	S_DllParam& halPa = Pa.m_dllPa;
	int i = 0;
	HImage ho_Image0 = Mat2HObject(src);
	POSITION pos = m_EleList.GetHeadPosition();
	while (pos != NULL) {
		UCI::OptionsMap* o = &m_EleList.GetNext(pos);
		try
		{
			S_Model* hv_Model = m_modelList[i++];
			HalconCpp::HXLDCont Cont;

			// 1. 查找图像中的产品
			double AngleStart = (*o)["起始角弧度"];
			double AngleExtend = (*o)["旋转范围"];
			double minScroe = (*o)["相似度"];
			int NumMatches = (*o)["目标数"];
			int NumMatchesLimit = (*o)["目标数限制"];
			double MaxOverlap = (*o)["FindModleMaxOverlap"];
			CString SubPixel = (*o)["FindModleSubPixel"];
			CString szNumLevels = (*o)["金字塔级数"];
			if ("auto" == szNumLevels)
			{
				szNumLevels = "4";
			}
			int NumLevels = atoi(szNumLevels.GetBuffer(0));
			double Greediness = (*o)["FindModleGreediness"];
			int modeType = (*o)["匹配模型"];
			HTuple Row, Column, Angle, Score;
			double ScaleRMin = (*o)["纵向变形最小尺度设置"];
			double ScaleRMax = (*o)["纵向变形最大尺度设置"];
			double ScaleCMin = (*o)["横向变形最小尺度设置"];
			double ScaleCMax = (*o)["横向变形最大尺度设置"];
			HTuple RScale, CScale;
			if (0 == modeType)
			{
				hv_Model->hv_Model.FindAnisoShapeModel(
					ho_Image0,
					AngleStart,
					AngleExtend,
					ScaleRMin, ScaleRMax,
					ScaleCMin, ScaleCMax,
					minScroe,
					NumMatches,
					MaxOverlap,
					CT2A(SubPixel).m_psz,
					NumLevels,
					Greediness,
					&Row,
					&Column,
					&Angle,
					&RScale,
					&CScale,
					&Score
				);
			}
			else
			{
				hv_Model->hv_nccModel.FindNccModel(
					ho_Image0,
					AngleStart,
					AngleExtend,
					minScroe,
					NumMatches,
					MaxOverlap,
					"true",
					NumLevels,
					&Row,
					&Column,
					&Angle,
					&Score
				);
			}
			int len = Row.Length();
			if (NumMatchesLimit > len) {
				for (int pi = vctData.size(); pi < len; pi++)
					vctData.push_back(new S_TpltData());

				char sz[128];
				for (int a = 0; a < len; a++)
				{
					vctData[a]->m_roiRect.angle = Angle[a].D() * 180 / PI;
					if (a == 0)
						sprintf(sz, "目标数%d,编号%d,相似度=%f,旋转角=%.4f ", len, a, Score.D(), vctData[a]->m_roiRect.angle);
					else
						sprintf(sz, "编号%d,相似度=%f,旋转角=%.4f ", a, Score.D(), vctData[a]->m_roiRect.angle);
					vctData[a]->m_szRet = sz;
					vctData[a]->m_roiRect.center.x = Column[a].D();
					vctData[a]->m_roiRect.center.y = Row[a].D();
					vctData[a]->m_roiRect.size = vctData[0]->m_roiRect.size;
					cv::Rect rt = vctData[a]->m_roiRect.boundingRect();
					rt &= cv::Rect(0, 0, src.cols, src.rows);
					vctData[a]->m_vctPic[0] = cv::Mat(src, rt).clone();
					vctData[a]->m_checkRet = true;
				}
				return true;
			}
		}

		catch (HException& except)
		{
			/*ofstream fp;
			fp.open("./err.txt",ios::app);
			fp << except.ErrorText().Text() << endl;
			fp.close();*/
		}
	}
	return false;
}


bool checkPatch::drawResult(cv::Mat& src, cv::RotatedRect& roiRect, CTpltParam& pa)
{
	return true;

}
