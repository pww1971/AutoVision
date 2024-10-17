#include "stdafx.h"

#include "MyData.h"
#include "MyDispLay.h"
#include "checkPatch.h"
using namespace HalconCpp;

using namespace std;
using namespace Gdiplus;

// 添加顶点
void ST_MYDISP::DisOnAddFindPeekElement()
{
	this->AppStateChangeTo(App_ElementAdding);

	try {
		int curProject = UCI::OPTIONS["项目编号"];
		UCI::OptionsMap *o = UCI::ELE.add_One_Element(curProject, ELEMENT_PEAK);

		double dRow, dColumn, dPhi, dLength1, dLength2;

		write_message(100, 10, "请画仿射矩形表明查找顶点区域，箭头表示检测方向");
		this->m_pMHWinMain->DrawRectangle2(&dRow, &dColumn, &dPhi, &dLength1, &dLength2);
		
		// 保存
		(*o)["Peak_Rectangle2_Row"] = dRow;
		(*o)["Peak_Rectangle2_Colum"] = dColumn;
		(*o)["Peak_Rectangle2_Phi"] = dPhi;
		(*o)["Peak_Rectangle2_Length1"] = dLength1;
		(*o)["Peak_Rectangle2_Length2"] = dLength2;


		// 下面显示这个查找到的顶点
		double hv_DetectWidth = (*o)["Peak_DetectWidth"];
		double hv_Sigma = (*o)["Peak_Sigma"];
		double hv_Threshold = (*o)["Peak_Threshold"];
		CString hv_Transition = (*o)["Peak_Transition"];
		CString hv_Select = (*o)["Peak_Select"];


		HImage Image = this->m_sIPResult.result_img;
		HTuple hv_EdgesY, hv_EdgesX, hv_ResultRow, hv_ResultColumn;

		peak(Image, dRow, dColumn, dPhi, dLength1, dLength2, hv_DetectWidth, hv_Sigma, hv_Threshold,
			CT2A(hv_Transition).m_psz, CT2A(hv_Select).m_psz,
			&hv_EdgesY, &hv_EdgesX, &hv_ResultRow, &hv_ResultColumn);

		HObject  ho_Cross;

		if (hv_EdgesY.Length() > 0) {
			this->m_pMHWinMain->SetColor("yellow");
			GenCrossContourXld(&ho_Cross, hv_EdgesY, hv_EdgesX, 3, 0);

			this->m_pMHWinMain->DispObj(ho_Cross);

			this->m_pMHWinMain->SetColor("green");
			GenCrossContourXld(&ho_Cross, hv_ResultRow, hv_ResultColumn, 10, 0);

			this->m_pMHWinMain->DispObj(ho_Cross);
		}

	}
	catch (HalconCpp::HException& except) {
//		UCI::sDB.Log(except, "DisOnFindPeekElement：找顶点元素 ");

	}
	

	this->AppStateChangeTo(APP_READY);
}

// 球囊表面缺陷检测
void ST_MYDISP::DisOnMeaEleBalloonFlaw()
{
	int curProject = UCI::OPTIONS["项目编号"];
	UCI::OptionsMap *o = UCI::ELE.add_One_Element(curProject, ELEMENT_BALLOON_FLAW);

	// 下面设定一下缺陷检测范围
	double dRow, dColumn, dPhi, dLength1, dLength2;
	write_message(100, 10, "请画仿射矩形指出要检测的球囊表面缺陷范围,右键确认");
	this->m_pMHWinMain->DrawRectangle2(&dRow, &dColumn, &dPhi, &dLength1, &dLength2);

	// 保存
	(*o)["Flaw_Rectangle2_Row"] = dRow;
	(*o)["Flaw_Rectangle2_Column"] = dColumn;
	(*o)["Flaw_Rectangle2_Phi"] = dPhi;
	(*o)["Flaw_Rectangle2_Length1"] = dLength1;
	(*o)["Flaw_Rectangle2_Length2"] = dLength2;

	// 这儿也要变换的
	this->MeasureOneELEMENT_MEASURE_BalloonFlaw(o, this->m_sIPResult.result_img, this->m_sIPResult.result_cont);
	this->OnMainDisp();
}

void ST_MYDISP::DisOnMeaEleAddRoi()
{
	int curProject = UCI::OPTIONS["项目编号"];
	UCI::OptionsMap *o = UCI::ELE.add_One_Element(curProject, ELEMENT_ROI);

	// 下面设定一下缺陷检测范围
	double dRow, dColumn, dPhi, dLength1, dLength2;
	write_message(100, 10, "请画仿射矩形指出要检测的图像区域范围,右键确认");
	this->m_pMHWinMain->DrawRectangle2(&dRow, &dColumn, &dPhi, &dLength1, &dLength2);

	// 保存
	(*o)["ROI_Rectangle2_Row"] = dRow;
	(*o)["ROI_Rectangle2_Column"] = dColumn;
	(*o)["ROI_Rectangle2_Phi"] = dPhi;
	(*o)["ROI_Rectangle2_Length1"] = dLength1;
	(*o)["ROI_Rectangle2_Length2"] = dLength2;

	// 这儿也要变换的
	this->MeasureOneROI(o, this->m_sIPResult.result_img, this->m_sIPResult.result_cont);
	this->OnMainDisp();
}

// 添加一维测量
void ST_MYDISP::DisOnAddMEASURE_RECTANGLE2()
{
	try {
		int curProject = UCI::OPTIONS["项目编号"];
		UCI::OptionsMap *o = UCI::ELE.add_One_Element(curProject, ELEMENT_MEASURE_RECTANGLE2);

		double dRow, dColumn, dPhi, dLength1, dLength2;
		write_message(100, 10, "请画仿射矩形表明1维测量区域，箭头表示检测方向");
		this->m_pMHWinMain->DrawRectangle2(&dRow, &dColumn, &dPhi, &dLength1, &dLength2);

		// 保存
		(*o)["Measure_Rectangle2_Row"] = dRow;
		(*o)["Measure_Rectangle2_Column"] = dColumn;
		(*o)["Measure_Rectangle2_Phi"] = dPhi;
		(*o)["Measure_Rectangle2_Length1"] = dLength1;
		(*o)["Measure_Rectangle2_Length2"] = dLength2;


		// 这儿也要变换的
		this->MeasureOneELEMENT_MEASURE_RECTANGLE2(o, this->m_sIPResult.result_img, this->m_sIPResult.result_cont);
		this->OnMainDisp();

	}
	catch (HalconCpp::HException& except) {
//		UCI::sDB.Log(except, "DisOnMEASURE_RECTANGLE2():");

	}

	this->AppStateChangeTo(APP_READY);
}

//#include "GdiPlusImageCodec.h"

void ST_MYDISP::DisOnMeaEleEllispe()
{
	Gdiplus::Graphics g(checkPatch::S_dlg.GetSafeHwnd());

	CRect rc;
	this->pCView->GetWindowRect(rc);

	CRect wrc;
	checkPatch::S_dlg.GetWindowRect(wrc);

	CString str = "楷体";
	FontFamily fontFamily(str.AllocSysString());
	Gdiplus::Font myFont(&fontFamily, 40, FontStyleRegular, UnitPoint); //第二个是字体大小
	SolidBrush blackBrush(Color(255, 255, 0, 0));  //半透明+文字RGB颜色
	//Gdiplus::PointF school_site(100.0f, 100.0f);//文字放置的像素坐标
	StringFormat format;
	format.SetAlignment(StringAlignmentNear);    //文本排列方式，即在对应位置居中、靠左、靠右

	str = "椭圆形元素";
	Gdiplus::PointF school_site(float(rc.left-wrc.left), float(rc.top-wrc.top));//文字放置的像素坐标
	g.DrawString(str.AllocSysString(), str.GetLength(), &myFont, school_site, &format, &blackBrush);//把string绘制到图上
}

// 显示大字体，这个不随着图像大小而改变
void ST_MYDISP::DisFontLarge(CString str, float x, float y, float size, SolidBrush &br)
{
	CRect rc;
	this->pCView->GetWindowRect(rc);

	CRect wrc;
	checkPatch::S_dlg.GetWindowRect(wrc);

	float fx = rc.left - wrc.left + x;
	float fy = rc.top - wrc.top + y;

	Gdiplus::Graphics g(checkPatch::S_dlg.GetSafeHwnd());
	CString szFont = "楷体";
	FontFamily fontFamily(szFont.AllocSysString());

	Gdiplus::Font myFont(&fontFamily, size, FontStyleRegular, UnitPoint); //第二个是字体大小

	StringFormat format;
	format.SetAlignment(StringAlignmentNear);    //文本排列方式，即在对应位置居中、靠左、靠右

	Gdiplus::PointF school_site(fx, fy);//文字放置的像素坐标

	g.DrawString(str.AllocSysString(), str.GetLength(), &myFont, school_site, &format, &br);//把string绘制到图上
}

//  直线 元素
void ST_MYDISP::DisOnEleLineAdd()
{
	this->AppStateChangeTo(App_ElementAdding);

	try {

		int curProject = UCI::OPTIONS["项目编号"];
		UCI::OptionsMap* o = UCI::ELE.add_One_Element(curProject, ELEMENT_LINE);

		//HalconCpp::HXLDCont RoiLineHXLD;
		double Row1, Column1, Row2, Column2;		
	
		this->write_message(12, 12, "鼠标左键画一条直线，右键确认", true);
		this->m_pMHWinMain->DrawLine(&(Row1), &(Column1), &(Row2), &(Column2));

		// 保存ROI
		(*o)["ROI_Row1"] = Row1;
		(*o)["ROI_Column1"] = Column1;
		(*o)["ROI_Row2"] = Row2;
		(*o)["ROI_Column2"] = Column2;

		int hv_Elements = (*o)["Rake直线检测点数"];
		double hv_DetectHeight = (*o)["Rake直线检测高度"];
		double hv_DetectWidth = (*o)["Rake直线检测宽度"];
		//画出检测区域 
		draw_rake(&this->m_sIPResult.result_cont,
			hv_Elements,
			hv_DetectHeight,
			hv_DetectWidth,
			&Row1,
			&Column1,
			&Row2,
			&Column2);

		// 测量这个直线
		this->MeasureOneLine(o, this->m_sIPResult.result_img, this->m_sIPResult.result_cont);
		this->OnMainDisp();

	}
	catch (HalconCpp::HException& except) {
//		UCI::sDB.Log(except, "DisOnEleLineAdd()：");
	}

	this->AppStateChangeTo(APP_READY);
}

// 圆 元素, 
void ST_MYDISP::DisOnEleCircleAdd()
{
	this->AppStateChangeTo(App_ElementAdding);

	try {
		
		//HTuple ROIRows, ROICols, Direct;

		int curProject = UCI::OPTIONS["项目编号"];
		UCI::OptionsMap* o = UCI::ELE.add_One_Element(curProject, ELEMENT_CIRCLE);


	    // 准备相应的素材。 1. 是圆上的点，2，是一个线段，表明取样方向
		HalconCpp::HXLDCont RoiCircleHXLD;
		HTuple  hv_Rows, hv_Cols, hv_Weights;

ReDrawCircle:
		write_message(12, 12, "1.画4个以上的点确定一个圆弧，右键确认", true);		
		HXLDCont ho_ContOut1 = this->m_pMHWinMain->DrawNurbs("true", "true", "true", "true", 3, &hv_Rows,
			&hv_Cols, &hv_Weights);

		HTuple hv_Length1 = hv_Weights.Length();
		if (hv_Length1 < 4) {
			write_message(62, 12, "提示，点数太少，请重画", true);
			goto ReDrawCircle;
		}

		//ROIRows = hv_Rows;
		//ROICols = hv_Cols;

		HXLDCont ho_Contour;
		ho_Contour.GenContourPolygonXld(hv_Rows, hv_Cols);    // 生成多边形
		

		HTuple  hv_RowC, hv_ColumnC, hv_Radius, hv_StartPhi, hv_EndPhi;
		HTuple  hv_PointOrder;
		ho_Contour.FitCircleContourXld("algebraic", -1, 0, 0, 3, 2, &hv_RowC, &hv_ColumnC,
			&hv_Radius, &hv_StartPhi, &hv_EndPhi, &hv_PointOrder);  // 转换成圆来存储

		// 保存原来的点
		(*o)["ROI_RowC"] = hv_RowC[0].D();
		(*o)["ROI_ColumnC"] = hv_ColumnC[0].D();
		(*o)["ROI_Radius"] = hv_Radius[0].D();
		(*o)["ROI_StartPhi"] = hv_StartPhi[0].D();
		(*o)["ROI_EndPhi"] = hv_EndPhi[0].D();
		CString str = hv_PointOrder[0].S();
		(*o)["ROI_PointOrder"] = str;

		draw_spoke(o, this->m_sIPResult.result_cont);

		// 测量这个圆
		this->MeasureOneCircle(o, this->m_sIPResult.result_img, this->m_sIPResult.result_cont);
		this->OnMainDisp();

	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());

	}

	this->AppStateChangeTo(APP_READY);
}

//  相机 元素
void ST_MYDISP::DisOnEleCameraAdd()
{
	int curProject = UCI::OPTIONS["项目编号"];
	UCI::ELE.add_One_Element(curProject, ELEMENT_CAMERA);
}

// 二直线角度测量 
void ST_MYDISP::DisOnEleAngle()
{
	int curProject = UCI::OPTIONS["项目编号"];
	UCI::ELE.add_One_Element(curProject, ELEMENT_ANGLE);
}

// 距离 元素
void ST_MYDISP::DisOnEleDistance()
{
	int curProject = UCI::OPTIONS["项目编号"];
	UCI::ELE.add_One_Element(curProject, ELEMENT_DISTANCE);
}

// 算子
void ST_MYDISP::DisMeaEleFunction()
{
	int curProject = UCI::OPTIONS["项目编号"];
	UCI::ELE.add_One_Element(curProject, ELEMENT_FUNCTION);
}

// 匹配
void ST_MYDISP::DisMeaEleMatching()
{
	int curProject = UCI::OPTIONS["项目编号"];
	UCI::ELE.add_One_Element(curProject, ELEMENT_MATCH);
}

// 球囊长度元素
void ST_MYDISP::DisOnMeaEleBalloonLength()
{
	int curProject = UCI::OPTIONS["项目编号"];
	UCI::ELE.add_One_Element(curProject, ELEMENT_BALLOONLENGTH);
}



// 报表 元素
void ST_MYDISP::DisOnEleOutReport()
{
	int curProject = UCI::OPTIONS["项目编号"];
	UCI::ELE.add_One_Element(curProject, ELEMENT_RESULT);
}

// mm
void ST_MYDISP::DisOnMeaEleMillimeter()
{
	int curProject = UCI::OPTIONS["项目编号"];
	UCI::ELE.add_One_Element(curProject, ELEMENT_MM);
}








