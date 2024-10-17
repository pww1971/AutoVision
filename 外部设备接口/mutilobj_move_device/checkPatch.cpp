#include "checkPatch.h"
#include "MyData.h"

Ctot_dlg_posDlg checkPatch::S_dlg;

HImage Mat2HObject(cv::Mat& image)
{
	HImage Hobj=HImage();
	if(NULL == image.data)
		return Hobj;
	int h=image.rows;
	int w=image.cols;
	//	int al=h*w;
	//	CV_8UC3
	if(image.type() == CV_8UC3)
	{
		cv::Mat imgchannel[3];
		split(image,imgchannel);
		cv::Mat imgB=imgchannel[0];
		cv::Mat imgG=imgchannel[1];
		cv::Mat imgR=imgchannel[2];

		try
		{
			GenImage3(&Hobj,"byte",(Hlong)w,(Hlong)h,(Hlong)imgR.data,(Hlong)imgG.data,(Hlong)imgB.data);
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
	else if(image.type() == CV_8UC1)
	{
		try
		{
			GenImage1(&Hobj,"byte",(Hlong)w,(Hlong)h,(Hlong)image.data);
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
	HTuple htCh=HTuple();
	HTuple cType;
	cv::Mat Image;
	ConvertImageType(Hobj,&Hobj,"byte");
	CountChannels(Hobj,&htCh);
	HTuple wid;
	HTuple hgt;
	int W,H;
	if(htCh[0].I()==1)
	{
		HTuple ptr;
		GetImagePointer1(Hobj,&ptr,&cType,&wid,&hgt);
		W=wid[0].I();
		H=hgt[0].I();
		Image.create(H,W,CV_8UC1);
		uchar* pdata=(uchar*)ptr[0].I();
		memcpy(Image.data,pdata,W*H);
	}
	else if(htCh[0].I()==3)
	{
		HTuple ptrR, ptrG, ptrB;
		GetImagePointer3(Hobj,&ptrR,&ptrG,&ptrB,&cType,&wid,&hgt);
		W=wid[0].I();
		H=hgt[0].I();

		Image.create(H,W,CV_8UC3);
		vector<cv::Mat> vecM(3);
		vecM[2].create(H,W,CV_8UC1);
		vecM[1].create(H,W,CV_8UC1);
		vecM[0].create(H,W,CV_8UC1);
		uchar* pr=(uchar*)ptrR[0].I();
		uchar* pg=(uchar*)ptrG[0].I();
		uchar* pb=(uchar*)ptrB[0].I();
		memcpy(vecM[2].data,pr,W*H);
		memcpy(vecM[1].data,pg,W*H);
		memcpy(vecM[0].data,pb,W*H);
		merge(vecM,Image);
	}
	return Image;
}

checkPatch::checkPatch()
{

}

checkPatch::~checkPatch()
{
	clearMode();
}

void checkPatch::clearMode()
{
	if(!m_EleList.IsEmpty())
		m_EleList.RemoveAll();
	for (int i=0;i<m_modelList.size();i++)
	{
		m_modelList[i]->ho_ModelRegion.GenEmptyObj();
		m_modelList[i]->hv_Model.Clear();
		delete m_modelList[i];
	}
	m_modelList.clear();
}

void checkPatch::readMode(string dir)
{	
	clearMode();
	if(dir != "")
	{
		mkdir(dir.c_str());
		m_cfgDir = dir;
	}
	S_CheckParam cfg0(m_cfgDir + "\\checkParam.cfg");
	string szVal = cfg0.getItem("optCount");
	if("" != szVal)
	{
		int count = atoi(szVal.c_str())+1;
		char sz[32];
		for(int i=1;i<count;i++)
		{
			sprintf(sz,"%d",i);
			S_CheckParam cfg(m_cfgDir + "\\checkParam" +sz +".cfg");
			UCI::OptionsMap omp;
			cfg.readcfgfile(omp);
			m_EleList.AddTail(omp);

			S_Model* model = new S_Model();
			UCI::pDis->hv_Model = model;
			model->hv_Model.ReadShapeModel((m_cfgDir+"\\model"+sz).c_str());       // 读出模板文件
			model->ho_ModelRegion.ReadRegion((m_cfgDir+"\\region"+sz+".hobj").c_str());          // 读出匹配区域文件
			
			m_modelList.push_back(model);
		}
	}
	m_CaliParam.loadcfg(m_cfgDir);
	m_DevParam.loadcfg(m_cfgDir);
}

void checkPatch::writeMode(string dir)
{
	if (m_modelList.size()<1)
	{
		return;
	}
	if(dir != "")
	{
		mkdir(dir.c_str());
		m_cfgDir = dir;
	}
	delCfgFiles(m_cfgDir.c_str(),1);

	char sz[32];
	S_CheckParam cfg0(m_cfgDir + "\\checkParam.cfg");
	sprintf(sz,"%d",m_EleList.GetCount());
	cfg0.updateItem("optCount",sz);

	int i = 1;
	POSITION pos = m_EleList.GetHeadPosition();
	while (pos != NULL) {
		UCI::OptionsMap *o = &m_EleList.GetNext(pos);

		sprintf(sz,"%d",i);
		S_CheckParam cfg(m_cfgDir + "\\checkParam" +sz +".cfg");
		cfg.writecfgfile(o);

		S_Model* model = m_modelList[i-1];
		UCI::pDis->hv_Model = model;
		string str = m_cfgDir+"\\model"+sz;
		remove(str.c_str());
		model->hv_Model.WriteShapeModel(str.c_str());       // 读出模板文件
		str = m_cfgDir+"\\region"+sz+".hobj";
		remove(str.c_str());
		model->ho_ModelRegion.WriteRegion(str.c_str());          // 读出匹配区域文件
		
		i++;
	}
	m_CaliParam.writecfg(m_cfgDir);
	m_DevParam.writecfg(m_cfgDir);
}


bool checkPatch::EditModel(cv::Mat& src)
{
	UCI::ELE.EleList = &m_EleList;
	UCI::pDis->m_sIPResult.result_img = UCI::pDis->GrabImageDisp = Mat2HObject(src);
	UCI::OPTIONS["图像宽度"] = src.cols;
	UCI::OPTIONS["图像高度"] = src.rows;
	UCI::OPTIONS["图像宽长比"] = (double)src.rows/src.cols;
	
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

bool checkPatch::checkLocateCheck(HImage& ho_Image0)
{	
	int i = 0;
	POSITION pos = m_EleList.GetHeadPosition();
	while (pos != NULL) {
		UCI::OptionsMap *o = &m_EleList.GetNext(pos);
		try
		{
			S_Model *hv_Model = m_modelList[i++];
			HalconCpp::HXLDCont Cont;

			// 1. 查找图像中的产品
			double AngleStart = (*o)["FindShapeModleAngleStart"];
			double AngleExtend = (*o)["FindShapeModleAngleExtend"];
			double minScroe = (*o)["FindShapeModleMinScore"];
			int NumMatches = (*o)["FindShapeModleNumMatches"];
			double MaxOverlap = (*o)["FindShapeModleMaxOverlap"];
			CString SubPixel = (*o)["FindShapeModleSubPixel"];
			int NumLevels = (*o)["FindShapeModleNumLevels"];
			double Greediness = (*o)["FindShapeModleGreediness"];

			HTuple Row, Column, Angle, Score;
			hv_Model->hv_Model.FindShapeModel(
				ho_Image0,
				AngleStart,
				AngleExtend,
				minScroe,
				NumMatches,
				MaxOverlap,
				CT2A(SubPixel).m_psz,
				NumLevels,
				Greediness,
				&Row,
				&Column,
				&Angle,
				&Score
				);
			if (Row.Length() == m_CaliParam.CaliNum ) //3点标定 9点标定
			{
				std::vector<cv::Point2f>& scene = m_CaliParam.scene;
				scene.clear();
				for (int i=0;i<m_CaliParam.CaliNum;i++)
					scene.push_back(cv::Point2f(Row[i].D(),Column[i].D()));
				// 产生ROI仿射变换矩阵  	
				//HTuple hv_x,hv_y;//实际距离
				//for (int i=0;i<m_CaliParam.CaliNum;i++)
				//{
				//	hv_x[i] = m_CaliParam.object[i].x;
				//	hv_y[i] = m_CaliParam.object[i].y;
				//}
				//VectorToHomMat2d(Row, Column,hv_y, hv_x,&m_CaliParam.HomMat2D);
				return true;
			}
			/*if (Row.Length() > 0) {
				roiRect.angle = Angle.D()*180/PI;
				roiRect.center.x = Column.D();
				roiRect.center.y = Row.D();
				double dret = Score.D();
				char sz[32];
				sprintf(sz," value=%lf",dret);
				if (Pa.m_polarPa.multiobjstr)
				{
					halPa.szRet="";
					char sz[64];
					int len = Row.Length();
					for (int a=0;a<len;a++)
					{
						sprintf(sz,"|%f,%f,%f",Column[a].D(),Row[a].D(),Angle[a].D()*180/PI);
						halPa.szRet += sz;
					}					
				}
				else
					halPa.szRet = sz;
				return true;
			}*/
			else
				return false;
		}
		catch (HException &except)
		{
			/*ofstream fp;
			fp.open("./err.txt",ios::app);
			fp << except.ErrorText().Text() << endl;
			fp.close();*/
		}
	}
	return false;
}

