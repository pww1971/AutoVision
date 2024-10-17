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
		ClearDeformableModel(m_modelList[i]->m_dxfModel);
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
	try
	{
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
				ReadDeformableModel((m_cfgDir + "\\model" + sz + ".dfm").c_str(), &model->m_dxfModel);
				GetDeformableModelContours(&model->ho_ModelContours, model->m_dxfModel, 1);
				HTuple  hv_Row1, hv_Column1, hv_Row2, hv_Column2;
				SmallestRectangle1Xld(model->ho_ModelContours, &hv_Row1, &hv_Column1, &hv_Row2, &hv_Column2);
				model->hv_CentY = hv_Row2.TupleMax().Append(hv_Row1.TupleMin());
				model->hv_CentX = hv_Column2.TupleMax().Append(hv_Column1.TupleMin());

				m_modelList.push_back(model);
			}
		}
	}
	catch (...)
	{
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
		string str = m_cfgDir + "\\model" + sz + ".dfm";
		remove(str.c_str());
		WriteDeformableModel(model->m_dxfModel, str.c_str());
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
			HTuple hv_HomMat2D,  Score;
			double ScaleRMin = (*o)["纵向变形最小尺度设置"];
			double ScaleRMax = (*o)["纵向变形最大尺度设置"];
			double ScaleCMin = (*o)["横向变形最小尺度设置"];
			double ScaleCMax = (*o)["横向变形最大尺度设置"];
			HTuple RScale, CScale;
			FindPlanarUncalibDeformableModel(
				ho_Image0,
				hv_Model->m_dxfModel,
				AngleStart,
				AngleExtend,
				ScaleRMin, ScaleRMax,
				ScaleCMin, ScaleCMax,
				minScroe,
				NumMatches,
				MaxOverlap,
				NumLevels,
				Greediness,
				"subpixel",
				CT2A(SubPixel).m_psz,
				&hv_HomMat2D,
				&Score
			);
			HTuple hv_ObjectModel3DRigidTrans, hv_ParamValue;
			HTuple Row, Column, Angle, hv_Zc;
			int len = Score.Length();
			if (NumMatchesLimit > len) {
				for (int pi = vctData.size(); pi < len; pi++)
					vctData.push_back(new S_TpltData());
				HTuple  hv_HomMatSelected;
				char sz[128];
				for (int a = 0; a < len; a++)
				{
					TupleSelectRange(hv_HomMat2D, a * 9, ((a + 1) * 9) - 1, &hv_HomMatSelected);
					ProjectiveTransPoint2d(hv_HomMatSelected, hv_Model->hv_CentX, hv_Model->hv_CentY, HTuple(1).Append(1), &Column, &Row, &hv_Zc);

					AngleLl(hv_Model->hv_CentY[0], hv_Model->hv_CentX[0],
						hv_Model->hv_CentY[1], hv_Model->hv_CentX[1],
						Row[0], Column[0],
						Row[1], Column[1],
						&Angle);

					vctData[a]->m_roiRect.angle = 360-Angle.TupleDeg().D();
					if (vctData[a]->m_roiRect.angle<0)
					{
						vctData[a]->m_roiRect.angle += 360;
					}
					else if (vctData[a]->m_roiRect.angle >= 360)
					{
						vctData[a]->m_roiRect.angle -= 360;
					}
					if (a == 0)
						sprintf(sz, "目标数%d,编号%d,相似度=%f,旋转角=%.4f ", len, a, Score.D(), vctData[a]->m_roiRect.angle);
					else
						sprintf(sz, "编号%d,相似度=%f,旋转角=%.4f ", a, Score.D(), vctData[a]->m_roiRect.angle);
					vctData[a]->m_szRet = sz;
					vctData[a]->m_roiRect.center.x = Row.TupleMean().D();
					vctData[a]->m_roiRect.center.y = Column.TupleMean().D();
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

bool checkPatch::creatDxfModel(string dxffile, CTpltParam& pa,cv::RotatedRect& rrt)
{
	try {
		UCI::ELE.EleList = &m_EleList;
		UCI::OptionsMap* o = UCI::ELE.GetOptionsMapByEleName(*UCI::ELE.EleList, "匹配 1");
		HObject  ho_DXFModelContours, ho_Contours, ho_ObjectSelected;
		HTuple hv_DXFStatus;
		HTuple  hv_I, hv_Y, hv_X, hv_Z, hv_Xc, hv_Yc, hv_Zc, hv_R, hv_C;
		ReadContourXldDxf(&ho_DXFModelContours, dxffile.c_str(),
			HTuple(), HTuple(), &hv_DXFStatus);

		HTuple  hv_Row1, hv_Column1, hv_Row2, hv_Column2;
		{
			SmallestRectangle1Xld(ho_DXFModelContours, &hv_Row1, &hv_Column1, &hv_Row2, &hv_Column2);
			HTuple  hv_HomMat2DIdentity, hv_HomMat2DScale;
			HomMat2dIdentity(&hv_HomMat2DIdentity);
			double dscale = pa.m_scale;//缩放比例
			(*o)["cad缩放比例"]= dscale;
			HomMat2dScale(hv_HomMat2DIdentity, dscale, dscale,
				(hv_Row2.TupleMax() - hv_Row1.TupleMin()) / 2, (hv_Column2.TupleMax() - hv_Column1.TupleMin()) / 2,
				&hv_HomMat2DScale);
			AffineTransContourXld(ho_DXFModelContours, &ho_DXFModelContours, hv_HomMat2DScale);
			//	this->m_pMHWinMain->DispObj(ho_DXFModelContours);
			//	return;
		}

		CString szCreatNumLevels = "auto";// (*o)["金字塔级数"];
		HTuple CreatNumLevels;
		if (szCreatNumLevels == "auto")
		{
			CreatNumLevels = "auto";
		}
		else
			CreatNumLevels = atoi(szCreatNumLevels.GetBuffer(0));
		double CreatAngleStart = -3.14;//(*o)["起始角弧度"];
		double CreatAngleExtend = 6.29; //(*o)["旋转范围"];
		(*o)["起始角弧度"] = CreatAngleStart;
		(*o)["旋转范围"] = CreatAngleExtend;
		CString CreateAngleStep = "auto";// (*o)["步数"];
		CString CreateOptimization = "auto";// (*o)["CreateModleOptimization"];
		CString CreateMetric = "use_polarity";// (*o)["CreateModleMetric"];
		CString CreateContrast = "auto";// (*o)["最小尺度设置"];
		CString CreateMinContrast = "auto";// (*o)["CreateModleMinContrast"];
		(*o)["纵向变形最小尺度设置"] = 1;
		(*o)["纵向变形最小尺度设置"] = 1;
		double ScaleRMin = 1;// (*o)["纵向变形最小尺度设置"];
		double ScaleRMax = 1;// (*o)["纵向变形最大尺度设置"];
		CString szScaleRStep = "auto";// (*o)["纵向变形步数设置"];
		HTuple ScaleRStep;
		if (szScaleRStep == "auto")
		{
			ScaleRStep = "auto";
		}
		else
			ScaleRStep = atof(szScaleRStep.GetBuffer(0));
		(*o)["横向变形最小尺度设置"] = 1;
		(*o)["横向变形最大尺度设置"] = 1;
		double ScaleCMin = 1;// (*o)["横向变形最小尺度设置"];
		double ScaleCMax = 1;// (*o)["横向变形最大尺度设置"];
		CString szScaleCStep = "auto";//(*o)["横向变形步数设置"];
		HTuple ScaleCStep;
		if (szScaleCStep == "auto")
		{
			ScaleCStep = "auto";
		}
		else
			ScaleCStep = atof(szScaleCStep.GetBuffer(0));

		HTuple AngleStep;
		if (CreateAngleStep == "auto")
			AngleStep = "auto";
		else
			AngleStep = atoi(CreateAngleStep.GetBuffer(0));
		HTuple hCreateContrast;
		if (CreateContrast == "auto")
		{
			hCreateContrast = "auto";
		}
		else
		{
			hCreateContrast[0] = "auto_contrast";
			hCreateContrast[1] = atoi(CreateContrast.GetBuffer(0));
		}
		if (m_modelList.size() == 0) 
		{
			S_Model* model = new S_Model();
			UCI::pDis->hv_Model = model;
			m_modelList.push_back(model);
		}
		S_Model* hv_Model = m_modelList[0];
		CreatePlanarUncalibDeformableModelXld(
			ho_DXFModelContours,
			CreatNumLevels,
			CreatAngleStart,
			CreatAngleExtend,
			AngleStep,
			ScaleRMin, ScaleRMax, ScaleRStep,
			ScaleCMin, ScaleCMax, ScaleCStep,
			/*CT2A(CreateOptimization).m_psz,
			CT2A(CreateMetric).m_psz,
			CT2A(CreateMinContrast).m_psz,*/
			"point_reduction_high",
			"ignore_part_polarity",
			5,
			HTuple(), HTuple(), &hv_Model->m_dxfModel);

		GetDeformableModelContours(&hv_Model->ho_ModelContours, hv_Model->m_dxfModel, 1);
		SmallestRectangle1Xld(hv_Model->ho_ModelContours, &hv_Row1, &hv_Column1, &hv_Row2, &hv_Column2);
		hv_Model->hv_CentY.Clear();
		hv_Model->hv_CentX.Clear();
		hv_Model->hv_CentY = hv_Row2.TupleMax().Append(hv_Row1.TupleMin());
		hv_Model->hv_CentX = hv_Column2.TupleMax().Append(hv_Column1.TupleMin());
		rrt.angle = 0;
		rrt.size.width = hv_Model->hv_CentX[0] - hv_Model->hv_CentX[1];
		rrt.size.height = hv_Model->hv_CentY[0] - hv_Model->hv_CentY[1];
		rrt.center.x = rrt.size.width/2;
		rrt.center.y = rrt.size.height/2;
		//	this->m_pMHWinMain->DispObj(hv_Model->ho_ModelContours);
		return true;
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
	return false;
}

