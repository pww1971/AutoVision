#include "stdafx.h"
#include "checkPatch.h"
#include "MyData.h"
#include "MyDispLay.h"
using namespace HalconCpp;
using namespace std;


void ST_MYDISP::DisProcessROIRegion(HalconCpp::HRegion &HXdis, HalconCpp::HRegion &HXsrc)
{
		switch (UCI::RIBmenu.iCOMBO_MATCH_ROI_TYPE)
		{
		case ROI_UNITE:
			HXdis = HXdis.Union2(HXsrc);
			break;
		case ROI_AND:
			HXdis = HXdis.Intersection(HXsrc);
			break;
		case ROI_SUB:
			HXdis = HXdis.Difference(HXsrc);
			break;
		case ROI_XOR:
			HXdis = HXdis.SymmDifference(HXsrc);
			break;
		default:
			break;
		}

}

void ST_MYDISP::DisProcessROImodel(HalconCpp::HImage &Img, HRegion &HXdis)
{
	try {
		HalconCpp::HImage ImageReduced;
		ImageReduced = Img.ReduceDomain(HXdis);

		UCI::OptionsMap* o = UCI::ELE.GetOptionsMapByEletype(*UCI::ELE.EleList,ELEMENT_MATCH);

		CString szCreatNumLevels = (*o)[CST_PosNumLevels];
		HTuple CreatNumLevels;
		if (szCreatNumLevels == "auto")
		{
			CreatNumLevels = "auto";
		}
		else
			CreatNumLevels = atoi(szCreatNumLevels.GetBuffer(0));
		//double CreatAngleStart = (*o)["起始角弧度"];
		double CreatAngleExtend = (*o)[CST_PosAngleExtend];
		CreatAngleExtend *= PI / 180;
		CString CreateAngleStep = (*o)[CST_PosStep];
		CString CreateOptimization = (*o)["CreateModleOptimization"];
		CString CreateMetric = (*o)["CreateModleMetric"];
		CString CreateContrast = (*o)[CST_PosContrast];
		CString CreateMinContrast = (*o)["CreateModleMinContrast"];
		int modeType = (*o)[CST_PosModeType];
		if (0 == modeType)
		{
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
			hv_Model->hv_Model.CreateShapeModel(
				ImageReduced,
				CreatNumLevels,
				-CreatAngleExtend/2,
				CreatAngleExtend,
				AngleStep,
				CT2A(CreateOptimization).m_psz,
				CT2A(CreateMetric).m_psz,
				hCreateContrast,
				CT2A(CreateMinContrast).m_psz);
		}
		else
		{
			HTuple channels;
			CountChannels(ImageReduced, &channels);
			if (3 == channels[0].I())	//三通道彩色
				Rgb1ToGray(ImageReduced, &ImageReduced);
			hv_Model->hv_nccModel.CreateNccModel(
				ImageReduced,
				CreatNumLevels,
				-CreatAngleExtend/2,
				CreatAngleExtend,
				CT2A(CreateAngleStep).m_psz,
				CT2A(CreateMetric).m_psz);
		}
		//double FindAngleStart = (*o)["起始角弧度"];
		double FindAngleExtend = (*o)[CST_PosAngleExtend];
		FindAngleExtend *= PI / 180;
		double FindMinScore = (*o)[CST_PosMinScroe];
		int FindNumMatches = (*o)[CST_PosNumMatches];
		double FindOverlap = (*o)["FindModleMaxOverlap"];
		CString FindSubPixel = (*o)["FindModleSubPixel"];
		CString szNumLevels = (*o)[CST_PosNumLevels];
		if ("auto" == szNumLevels)
		{
			szNumLevels = "4";
		}
		int FindNumLevels = atoi(szNumLevels.GetBuffer(0));
		double FindGreediness = (*o)["FindModleGreediness"];

		HTuple Row3, Column3, Angle1, Score1;
		if (0 == modeType)
		{
			hv_Model->hv_Model.FindShapeModel(
				Img,
				-FindAngleExtend/2,
				FindAngleExtend,
				FindMinScore,
				FindNumMatches,
				FindOverlap,
				CT2A(FindSubPixel).m_psz,
				FindNumLevels,
				FindGreediness,
				&Row3,
				&Column3,
				&Angle1,
				&Score1
			);
		}
		else
		{
			HTuple channels;
			CountChannels(Img, &channels);
			if (3 == channels[0].I())	//三通道彩色
				Rgb1ToGray(Img, &Img);
			hv_Model->hv_nccModel.FindNccModel(
				Img,
				-FindAngleExtend/2,
				FindAngleExtend,
				FindMinScore,
				FindNumMatches,
				FindOverlap,
				"true",
				FindNumLevels,
				&Row3,
				&Column3,
				&Angle1,
				&Score1
			);
		}

//		EnterCriticalSection(&m_csIPDisp);
		m_sIPResult.result_cont = hv_Model->ho_ModelRegion;   // region 可转换成 xld
//		LeaveCriticalSection(&m_csIPDisp);

		this->OnMainDisp();              // 主窗口显示一下

		CString DispShapeC = (*o)["DispShapeColor"];
		if (0 == modeType)
		{
			dev_display_shape_matching_results(
				hv_Model->hv_Model,
				CT2A(DispShapeC).m_psz,
				Row3,
				Column3,
				Angle1,
				1,
				1,
				0);
		}
		else
		{
			dev_display_ncc_matching_results(
				hv_Model->hv_nccModel,
				CT2A(DispShapeC).m_psz,
				Row3,
				Column3,
				Angle1,
				1,
				1,
				0);
		}

	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}

// ROI Rectangle2
void ST_MYDISP::DisOnRoiDrawRectangle2()
{
	if (UCI::RIBmenu.bIsInROIdrawing) {
		AfxMessageBox(CST_PosWord7);
		return;
	}
	
	try {
		double Row, Column, Phi, Length1, Length2;
		
		HalconCpp::HRegion Hreg;                     // 区域
		HalconCpp::HImage hImg;

//		EnterCriticalSection(&m_csIPDisp);  
		hImg = m_sIPResult.result_img;
//		LeaveCriticalSection(&m_csIPDisp);    

		// 提示信息
		this->write_message(20, 20, CST_PosWord8);

		UCI::RIBmenu.bIsInROIdrawing = true;
		// 画区域
		this->m_pMHWinMain->DrawRectangle2(&Row, &Column, &Phi, &Length1, &Length2);

		UCI::RIBmenu.bIsInROIdrawing = false;

		Hreg.GenRectangle2(Row, Column, Phi, Length1, Length2);

		// 产生区域
		DisProcessROIRegion(hv_Model->ho_ModelRegion, Hreg);	

		DisProcessROImodel(hImg, hv_Model->ho_ModelRegion);   // 查找模型

	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}

void ST_MYDISP::DisOnRoiDrawCircle()
{
	if (UCI::RIBmenu.bIsInROIdrawing) {
		AfxMessageBox(CST_PosWord7);
		return;
	}
	
	try {
		double Row, Column, Radius;

		HalconCpp::HRegion Hreg;                     // 区域
		HalconCpp::HImage hImg;

//		EnterCriticalSection(&m_csIPDisp);	
		hImg = m_sIPResult.result_img;
//		LeaveCriticalSection(&m_csIPDisp);

		// 提示信息
		this->write_message(20, 20, CST_PosWord8);

		UCI::RIBmenu.bIsInROIdrawing = true;
		// 画区域
		this->m_pMHWinMain->DrawCircle(&Row, &Column, &Radius);	

		UCI::RIBmenu.bIsInROIdrawing = false;


		// 产生区域
		Hreg.GenCircle(Row, Column, Radius);

		DisProcessROIRegion(hv_Model->ho_ModelRegion, Hreg);

		DisProcessROImodel(hImg, hv_Model->ho_ModelRegion);   // 查找模型
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}

void ST_MYDISP::DisOnRoiDrawEllipse()
{
	if (UCI::RIBmenu.bIsInROIdrawing) {
		AfxMessageBox(CST_PosWord7);
		return;
	}
	
	try {
		double Row, Column, Phi,Radius1, Radius2;

		HalconCpp::HRegion Hreg;                     // 区域
		HalconCpp::HImage hImg;

//		EnterCriticalSection(&m_csIPDisp);
		hImg = m_sIPResult.result_img;
//		LeaveCriticalSection(&m_csIPDisp);

		// 提示信息
		this->write_message(20, 20, CST_PosWord8);

		UCI::RIBmenu.bIsInROIdrawing = true;
		// 画区域
		this->m_pMHWinMain->DrawEllipse(&Row, &Column, &Phi, &Radius1, &Radius2);

		UCI::RIBmenu.bIsInROIdrawing = false;

		// 产生区域
		Hreg.GenEllipse(Row, Column, Phi, Radius1, Radius2);

		// 产生区域
		DisProcessROIRegion(hv_Model->ho_ModelRegion, Hreg);


		DisProcessROImodel(hImg, hv_Model->ho_ModelRegion);   // 查找模型
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}

void ST_MYDISP::DisOnRoiDrawRectangle()
{
	if (UCI::RIBmenu.bIsInROIdrawing) {
		AfxMessageBox(CST_PosWord7);
		return;
	}
	
	try {
		double Row1, Column1, Row2, Column2;

		HalconCpp::HRegion Hreg;                     // 区域
		HalconCpp::HImage hImg;

//		EnterCriticalSection(&m_csIPDisp);
		hImg = m_sIPResult.result_img;
//		LeaveCriticalSection(&m_csIPDisp);

		// 提示信息
		this->write_message(20, 20, CST_PosWord8);

		UCI::RIBmenu.bIsInROIdrawing = true;
		// 画区域
		this->m_pMHWinMain->DrawRectangle1(&Row1, &Column1, &Row2, &Column2);
		UCI::RIBmenu.bIsInROIdrawing = false;

		// 产生区域
		Hreg.GenRectangle1(Row1, Column1, Row2, Column2);

		// 与原来的区域进行运算
		DisProcessROIRegion(hv_Model->ho_ModelRegion, Hreg);

		//this->m_pMHWinMain->ClearWindow();	   
		//this->m_pMHWinMain->DispObj(hImg);
		//this->m_pMHWinMain->DispObj(this->ho_ModelRegion);

		DisProcessROImodel(hImg, hv_Model->ho_ModelRegion);   // 查找模型

	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}

void ST_MYDISP::DisOnRoiDrawAnyLine()
{
	if (UCI::RIBmenu.bIsInROIdrawing) {
		AfxMessageBox(CST_PosWord7);
		return;
	}
	
	try {
		
		HalconCpp::HImage hImg;                      // 显示的图像
		HalconCpp::HRegion Hreg;                     // 区域

//		EnterCriticalSection(&m_csIPDisp);
		hImg = m_sIPResult.result_img;
//		LeaveCriticalSection(&m_csIPDisp);

		// 提示信息
		this->write_message(20, 20, CST_PosWord8);

		UCI::RIBmenu.bIsInROIdrawing = true;
		Hreg = this->m_pMHWinMain->DrawRegion();
		UCI::RIBmenu.bIsInROIdrawing = false;

		DisProcessROIRegion(hv_Model->ho_ModelRegion,Hreg);	

		DisProcessROImodel(hImg, hv_Model->ho_ModelRegion);   // 查找模型

	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}


// 显示当前的模板信息\n模板信息
void ST_MYDISP::DisOnRoiInfo(int id)
{
	try {
		CString str;
		str.Format(CST_PosWord18,id);
		UCI::OptionsMap *om = UCI::ELE.GetOptionsMapByEleName(*UCI::ELE.EleList,str);
		if (nullptr == om)
		{
			AfxMessageBox(CST_PosWord4);
			return ;
		}
		// 打开当前保存的图片

		this->OnDisButMatchLoadPic();
		CString szdir = ((checkPatch*)checkPatch::S_dlg.m_pCheck)->m_cfgDir.c_str();
		CString stRegion;
		stRegion.Format("%d",id);
		stRegion = szdir+"\\region"+stRegion+".hobj";
		CString stModel;
		stModel.Format("%d",id);
		stModel = szdir+"\\model"+stModel;

		if (GetFileAttributes(stRegion) == -1) { // 文件不存在
			AfxMessageBox(stRegion);
			AfxMessageBox(CST_PosWord10);
			return;
		}
		hv_Model->ho_ModelRegion.ReadRegion(stRegion.GetBuffer(0));
		//
		/*HXLDCont HXdis;
		HXdis.ReadObject(CT2A(stRegion).m_psz);
		hv_Model->ho_ModelRegion = HXdis;*/
		if (GetFileAttributes(stModel) == -1) { // 文件不存在
			AfxMessageBox(stModel);
			AfxMessageBox(CST_PosWord11);
			return;
		}
		int modeType = (*om)[CST_PosModeType];
		if (0==modeType)
			hv_Model->hv_Model.ReadShapeModel(CT2A(stModel).m_psz);
		else
			hv_Model->hv_nccModel.ReadNccModel(CT2A(stModel).m_psz);
//		EnterCriticalSection(&m_csIPDisp);
		m_sIPResult.result_cont = hv_Model->ho_ModelRegion;
//		LeaveCriticalSection(&m_csIPDisp);

		this->OnMainDisp();

		//double AngleStart = (*om)["起始角弧度"];
		double AngleExtend = (*om)[CST_PosAngleExtend];
		AngleExtend *= PI/180;
		HTuple Row3, Column3, Angle1, Score1;
		HalconCpp::HImage hImg;
		hImg = UCI::pDis->GrabImageDisp;
		if (0 == modeType)
		{
			hv_Model->hv_Model.FindShapeModel(
				hImg,
				-AngleExtend/2,
				AngleExtend,
				0.5,
				1,
				0.5,
				"least_squares",
				0,
				0.9,
				&Row3,
				&Column3,
				&Angle1,
				&Score1
			);
			dev_display_shape_matching_results(
				hv_Model->hv_Model,
				"red",
				Row3,
				Column3,
				Angle1,
				1,
				1,
				0);
		}
		else
		{
			HTuple channels;
			CountChannels(hImg, &channels);
			if (3 == channels[0].I())	//三通道彩色
				Rgb1ToGray(hImg, &hImg);
			hv_Model->hv_nccModel.FindNccModel(
				hImg,
				-AngleExtend/2,
				AngleExtend,
				0.5,
				1,
				0.5,
				"true",
				0,
				&Row3,
				&Column3,
				&Angle1,
				&Score1
			);
			dev_display_ncc_matching_results(
				hv_Model->hv_nccModel,
				"red",
				Row3,
				Column3,
				Angle1,
				1,
				1,
				0);
		}

	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}

void ST_MYDISP::DisOnRoiFindModel(UCI::OptionsMap *om)
{
	try {		
		if (nullptr == om)
		{
			AfxMessageBox(CST_PosWord4);
			return ;
		}
		this->m_pMHWinMain->ClearWindow();

		HalconCpp::HImage hImg=this->GrabImageDisp;
		HalconCpp::HXLDCont Cont;
		Cont.GenEmptyObj();
	//	(*om)["匹配模板 OK"] = FALSE;

		/*UCI::OptionsMap *om;
		om = UCI::ELE.GetOptionsMapByEletype(*UCI::ELE.EleList,ELEMENT_CAMERA);
		this->MeasureOneCamera(om, hImg, Cont);

		om = UCI::ELE.GetOptionsMapByEletype(*UCI::ELE.EleList,ELEMENT_ROI);
		this->MeasureOneROI(om, hImg, Cont);

		om = UCI::ELE.GetOptionsMapByEletype(*UCI::ELE.EleList,ELEMENT_FUNCTION);
		this->MeasureOneFUNCTION(om, hImg, Cont);
		om = UCI::ELE.GetOptionsMapByEletype(*UCI::ELE.EleList,ELEMENT_MATCH);*/
		if (this->MeasureOneMATCH(om, hImg, Cont) == FALSE) {
			write_message(10, 10, CST_PosWord5);
			return;
		}

		Cont = Cont.ConcatObj(hv_Model->ho_ModelRegion);

//		EnterCriticalSection(&m_csIPDisp);
		this->m_sIPResult.result_img = hImg;
		this->m_sIPResult.result_cont = Cont;
//		LeaveCriticalSection(&m_csIPDisp);
		this->OnMainDisp();              // 主窗口显示一下
		// 显示一下模板结果
		om = UCI::ELE.GetOptionsMapByEletype(*UCI::ELE.EleList,ELEMENT_MATCH);
		HTuple Row = (*om)["RES_FindRow"];
		HTuple Column = (*om)["RES_FindColumn"];
		HTuple Angle = (*om)["RES_FindAngle"];
		HTuple Score = (*om)[CST_PosCheckScore];

		if (Row.Length() > 0) {

			write_message(100, 20, CST_PosWord6 + Score, true);

			CString mcolor = (*om)["DispShapeColor"];
			int modeType = (*om)[CST_PosModeType];
			if (0 == modeType)
			{
				dev_display_shape_matching_results(
					hv_Model->hv_Model,
					CT2A(mcolor).m_psz,
					Row,
					Column,
					Angle,
					1,
					1,
					0);
			}
			else
			{
				dev_display_ncc_matching_results(
					hv_Model->hv_nccModel,
					CT2A(mcolor).m_psz,
					Row,
					Column,
					Angle,
					1,
					1,
					0);
			}
		}


		//int prID = UCI::OPTIONS[CST_PosProcID];
		//// 采集一个图片
		//CString stModel = this->GetShapeModelPathFile(prID);		//

		//HalconCpp::HImage hImg;
		//if (GetFileAttributes(stModel) == -1) { // 文件不存在
		//	MyLog(stModel);
		//	MyLog("DisOnRoiInfo() 文件不存在！ ");
		//	return;
		//}
		//this->hv_Model.ReadShapeModel(CT2A(stModel).m_psz);

		//EnterCriticalSection(&m_csIPDisp);
		////m_sIPResult.result_cont = HXdis;
		//hImg = m_sIPResult.result_img;
		//LeaveCriticalSection(&m_csIPDisp);

		//this->OnMainDisp(NULL, NULL);

		//double AngleStart = UCI::OPTIONS["起始角弧度"];
		//double AngleExtend = UCI::OPTIONS[CST_PosAngleExtend];

		//HTuple Row3, Column3, Angle1, Score1;
		//this->hv_Model.FindShapeModel(
		//	hImg,
		//	AngleStart,
		//	AngleExtend,
		//	0.5,
		//	1,
		//	0.5,
		//	"least_squares",
		//	0,
		//	0.9,
		//	&Row3,
		//	&Column3,
		//	&Angle1,
		//	&Score1
		//);

		//if (Row3.Length() < 1) {
		//	MyLog("DisOnRoiFindModel() 没有找到对应的产品形状");
		//	return;
		//}
		//

		//double dMin = UCI::OPTIONS[CST_PosMinScroe];

		//this->write_message(20, 20, "当前查找得分：" + Score1 + " 设置值：" + dMin);
		//if (Score1 >= dMin) {
		//	dev_display_shape_matching_results(
		//		this->hv_Model,
		//		"red",
		//		Row3,
		//		Column3,
		//		Angle1,
		//		1,
		//		1,
		//		0);
		//}
		//else {
		//	MyLog("DisOnRoiFindModel() 没有找到对应的产品形状");
		//}

	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}

// 保存ROI 
void ST_MYDISP::DisOnButMatchSaveModel()
{

	try {
		//int prID = UCI::OPTIONS[CST_PosProcID];
		//// 先判断一下文件是否存在
		//CString str = this->GetMatchPathFile(prID);
		//if (GetFileAttributes(str) != -1) { // 文件存在
		//	int res = ::MessageBox(NULL,
		//		"点击 确定  重新实现采样一副！\n \
		//		点击 取消  返回。",
		//		"匹配文件已存在！",
		//		MB_OKCANCEL | MB_ICONSTOP);
		//	if (res == IDCANCEL) return;
		//}

		//// 1. 保存当前的图像
		//this->SaveImgToROI();

		//// 保存一下这个查找的区域 ROI
		//hv_Model->ho_ModelRegion.WriteRegion(CT2A(this->GetShapeModelRegionPathFile(prID)).m_psz);

		//// 保存一下这个查找的模型 
		//hv_Model->hv_Model.WriteShapeModel(CT2A(this->GetShapeModelPathFile(prID)).m_psz);
		((checkPatch*)checkPatch::S_dlg.m_pCheck)->writeMode();
		this->write_message(12, 12, CST_PosWord12);
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
		this->write_message(12, 12, CST_PosWord13);
	}
}

// 加载上次保存的匹配的图
void ST_MYDISP::OnDisButMatchLoadPic()
{	
	try {
		this->m_pMHWinMain->ClearWindow();

		//int prID = UCI::OPTIONS[CST_PosProcID];

		HalconCpp::HImage hImg;
		HalconCpp::HXLDCont Contours;

	//	hv_Model->ho_ModelRegion.GenEmptyObj();            // 清空一下模板区域
	//	UCI::OPTIONS["匹配模板 OK"] = FALSE;
		Contours.GenEmptyObj();

		HTuple hv_ImageWidth, hv_ImageHeight;

		//CString str = this->GetMatchPathFile(prID);    // 得到匹配文件名		
		//hImg.ReadImage(CT2A(str).m_psz);
		hImg=this->GrabImageDisp;
		hImg.GetImageSize(&hv_ImageWidth, &hv_ImageHeight);		

		int iHeight = hv_ImageHeight.I();
		int iWidth = hv_ImageWidth.I();

		/*int oldH = UCI::OPTIONS[CST_PosImageHeigh];
		int oldW = UCI::OPTIONS["图像宽度"];
		if (oldH != iHeight || oldW != iWidth) {

			UCI::OPTIONS["图像宽度"] = iWidth;
			UCI::OPTIONS[CST_PosImageHeigh] = iHeight;
			UCI::OPTIONS["ORGwhByCamera"] = 0;
		}*/

		this->m_pMHWinMain->SetPart(0, 0, iHeight - 1, iWidth - 1);


		//// 同时也保存到抓取图像变量中去
		//this->GrabImageDisp = hImg;

		// 下面走后面二步
		/*UCI::OptionsMap * om;
		if(nullptr != (om = UCI::ELE.GetOptionsMapByEletype(*UCI::ELE.EleList,ELEMENT_ROI)))
			this->MeasureOneROI(om, hImg, Contours);

		if(nullptr != (om = UCI::ELE.GetOptionsMapByEletype(*UCI::ELE.EleList,ELEMENT_FUNCTION)))
			this->MeasureOneFUNCTION(om, hImg, Contours);

		if(nullptr != (om = UCI::ELE.GetOptionsMapByEletype(*UCI::ELE.EleList,ELEMENT_MATCH)))
			this->MeasureOneMATCH(om, hImg, Contours);*/


		Contours = Contours.ConcatObj(hv_Model->ho_ModelRegion);

//		EnterCriticalSection(&m_csIPDisp);
		this->m_sIPResult.result_img = hImg;
		this->m_sIPResult.result_cont = Contours;
//		LeaveCriticalSection(&m_csIPDisp);
		this->OnMainDisp();              // 主窗口显示一下


		// 显示一下模板结果
		/*if(nullptr != (om = UCI::ELE.GetOptionsMapByEletype(*UCI::ELE.EleList,ELEMENT_MATCH)))
		{
			HTuple Row = (*om)["RES_FindRow"];
			HTuple Column = (*om)["RES_FindColumn"];
			HTuple Angle = (*om)["RES_FindAngle"];
			HTuple Score = (*om)[CST_PosCheckScore];

			if (Row.Length() > 0) {

				write_message(100, 20, "找到了对象，得分：" + Score, true);

				CString mcolor = (*om)["DispShapeColor"];

				dev_display_shape_matching_results(
					hv_Model->hv_Model,
					CT2A(mcolor).m_psz,
					Row,
					Column,
					Angle,
					1,
					1,
					0);
			}
		}*/

		//HXLDCont ho_RoiHxld;
		//ho_RoiHxld.GenEmptyObj();
		//this->DisDrawAllRoi(ho_RoiHxld);            // 显示所有的元素的 ROI
		//this->m_pMHWinMain->DispObj(ho_RoiHxld);		

		return;
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
		return;
	}
}

// 只是加载原图像
void ST_MYDISP::OnDisButLoadPic()
{

	try {

		this->m_pMHWinMain->ClearWindow();

		// 做手动测量程序的前三步
		HalconCpp::HXLDCont Contours;
		HalconCpp::HImage hImg;

		UCI::OptionsMap *om = UCI::ELE.GetOptionsMapByEletype(*UCI::ELE.EleList,ELEMENT_CAMERA);
		this->MeasureOneCamera(om, hImg, Contours);

		this->m_pMHWinMain->DispObj(hImg);

		om = UCI::ELE.GetOptionsMapByEletype(*UCI::ELE.EleList,ELEMENT_ROI);
		this->MeasureOneROI(om, hImg, Contours);
		
		return;
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
		return;
	}
}

void ST_MYDISP::DisOnElementDeleteAll()
{
	
	int res = CST_PosDelMsg
		/*::MessageBox(NULL,
		"点击 确定  删除所有测量元素！\n \
		点击 取消  返回。",
		"警告！亲，您真的要删除所有测量元素吗？",
		MB_OKCANCEL | MB_ICONSTOP | MB_ICONWARNING|MB_SYSTEMMODAL);*/
	if (res == IDCANCEL) return;

	//int prID = UCI::OPTIONS[CST_PosProcID];

	//// 从第三个开始一个一个删除啊！
	//int size = (int)UCI::ELE.EleList->GetCount();

	//int i = size;
	//POSITION pos = UCI::ELE.EleList->GetTailPosition();
	//while (pos != NULL) {
	//	UCI::OptionsMap *ot = &UCI::ELE.EleList->GetPrev(pos);
	//	if (i > 4) {
	//		UCI::ELE.EleList->RemoveTail();
	//	}
	//	i--;
	//}

	UCI::ELE.EleList->RemoveAll();
	checkPatch::S_dlg.UpdataProjectTreeByList();

}

void ST_MYDISP::DisOnElementDelOne()
{
	try
	{
		if (UCI::RIBmenu.bIsInROIdrawing) {
			AfxMessageBox(CST_PosWord7);
			return;
		}
		HalconCpp::HImage hImg;
//		EnterCriticalSection(&m_csIPDisp);  
		hImg = m_sIPResult.result_img;
//		LeaveCriticalSection(&m_csIPDisp);    

		HalconCpp::HXLDCont hv_cont,hv_cont2,hv_cont3;
		hv_cont.GenEmptyObj();
		hv_cont2.GenEmptyObj();
		hv_cont3.GenEmptyObj();
		hv_cont= hv_Model->ho_ModelRegion;
		m_pMHWinMain->DispObj(hv_cont);
		// 提示信息
		this->write_message(20, 20, CST_PosWord8);
		double Row, Column;
		UCI::RIBmenu.bIsInROIdrawing = true;
		// 画区域
		this->m_pMHWinMain->DrawPoint(&Row, &Column);
		UCI::RIBmenu.bIsInROIdrawing = false;

		/*m_pMHWinMain->ClearWindow();
		m_pMHWinMain->DispObj(hImg);*/

		hv_cont2=hv_cont.SelectXldPoint(Row, Column);
		int n = hv_cont.CountObj();
		for (int i=0;i<n;i++)
		{
			hv_cont2=hv_cont[i+1].SelectXldPoint(Row, Column);
			if (hv_cont2.CountObj()==0)
				hv_cont3=hv_cont3.ConcatObj(hv_cont[i+1]);
		}
		//m_pMHWinMain->DispObj(hv_cont3);
		hv_Model->ho_ModelRegion = hv_cont3;
		if (hv_cont3.CountObj()>0)
			DisProcessROImodel(hImg, hv_Model->ho_ModelRegion);   // 查找模型
		else
			m_pMHWinMain->DispObj(hImg);
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}
