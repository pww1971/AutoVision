#include "stdafx.h"

#include "MyData.h"
#include "MyDispLay.h"
#include "checkPatch.h"
using namespace HalconCpp;

using namespace std;
using namespace Gdiplus;

// ��Ӷ���
void ST_MYDISP::DisOnAddFindPeekElement()
{
	this->AppStateChangeTo(App_ElementAdding);

	try {
		int curProject = UCI::OPTIONS["��Ŀ���"];
		UCI::OptionsMap *o = UCI::ELE.add_One_Element(curProject, ELEMENT_PEAK);

		double dRow, dColumn, dPhi, dLength1, dLength2;

		write_message(100, 10, "�뻭������α������Ҷ������򣬼�ͷ��ʾ��ⷽ��");
		this->m_pMHWinMain->DrawRectangle2(&dRow, &dColumn, &dPhi, &dLength1, &dLength2);
		
		// ����
		(*o)["Peak_Rectangle2_Row"] = dRow;
		(*o)["Peak_Rectangle2_Colum"] = dColumn;
		(*o)["Peak_Rectangle2_Phi"] = dPhi;
		(*o)["Peak_Rectangle2_Length1"] = dLength1;
		(*o)["Peak_Rectangle2_Length2"] = dLength2;


		// ������ʾ������ҵ��Ķ���
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
//		UCI::sDB.Log(except, "DisOnFindPeekElement���Ҷ���Ԫ�� ");

	}
	

	this->AppStateChangeTo(APP_READY);
}

// ���ұ���ȱ�ݼ��
void ST_MYDISP::DisOnMeaEleBalloonFlaw()
{
	int curProject = UCI::OPTIONS["��Ŀ���"];
	UCI::OptionsMap *o = UCI::ELE.add_One_Element(curProject, ELEMENT_BALLOON_FLAW);

	// �����趨һ��ȱ�ݼ�ⷶΧ
	double dRow, dColumn, dPhi, dLength1, dLength2;
	write_message(100, 10, "�뻭�������ָ��Ҫ�������ұ���ȱ�ݷ�Χ,�Ҽ�ȷ��");
	this->m_pMHWinMain->DrawRectangle2(&dRow, &dColumn, &dPhi, &dLength1, &dLength2);

	// ����
	(*o)["Flaw_Rectangle2_Row"] = dRow;
	(*o)["Flaw_Rectangle2_Column"] = dColumn;
	(*o)["Flaw_Rectangle2_Phi"] = dPhi;
	(*o)["Flaw_Rectangle2_Length1"] = dLength1;
	(*o)["Flaw_Rectangle2_Length2"] = dLength2;

	// ���ҲҪ�任��
	this->MeasureOneELEMENT_MEASURE_BalloonFlaw(o, this->m_sIPResult.result_img, this->m_sIPResult.result_cont);
	this->OnMainDisp();
}

void ST_MYDISP::DisOnMeaEleAddRoi()
{
	int curProject = UCI::OPTIONS["��Ŀ���"];
	UCI::OptionsMap *o = UCI::ELE.add_One_Element(curProject, ELEMENT_ROI);

	// �����趨һ��ȱ�ݼ�ⷶΧ
	double dRow, dColumn, dPhi, dLength1, dLength2;
	write_message(100, 10, "�뻭�������ָ��Ҫ����ͼ������Χ,�Ҽ�ȷ��");
	this->m_pMHWinMain->DrawRectangle2(&dRow, &dColumn, &dPhi, &dLength1, &dLength2);

	// ����
	(*o)["ROI_Rectangle2_Row"] = dRow;
	(*o)["ROI_Rectangle2_Column"] = dColumn;
	(*o)["ROI_Rectangle2_Phi"] = dPhi;
	(*o)["ROI_Rectangle2_Length1"] = dLength1;
	(*o)["ROI_Rectangle2_Length2"] = dLength2;

	// ���ҲҪ�任��
	this->MeasureOneROI(o, this->m_sIPResult.result_img, this->m_sIPResult.result_cont);
	this->OnMainDisp();
}

// ���һά����
void ST_MYDISP::DisOnAddMEASURE_RECTANGLE2()
{
	try {
		int curProject = UCI::OPTIONS["��Ŀ���"];
		UCI::OptionsMap *o = UCI::ELE.add_One_Element(curProject, ELEMENT_MEASURE_RECTANGLE2);

		double dRow, dColumn, dPhi, dLength1, dLength2;
		write_message(100, 10, "�뻭������α���1ά�������򣬼�ͷ��ʾ��ⷽ��");
		this->m_pMHWinMain->DrawRectangle2(&dRow, &dColumn, &dPhi, &dLength1, &dLength2);

		// ����
		(*o)["Measure_Rectangle2_Row"] = dRow;
		(*o)["Measure_Rectangle2_Column"] = dColumn;
		(*o)["Measure_Rectangle2_Phi"] = dPhi;
		(*o)["Measure_Rectangle2_Length1"] = dLength1;
		(*o)["Measure_Rectangle2_Length2"] = dLength2;


		// ���ҲҪ�任��
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

	CString str = "����";
	FontFamily fontFamily(str.AllocSysString());
	Gdiplus::Font myFont(&fontFamily, 40, FontStyleRegular, UnitPoint); //�ڶ����������С
	SolidBrush blackBrush(Color(255, 255, 0, 0));  //��͸��+����RGB��ɫ
	//Gdiplus::PointF school_site(100.0f, 100.0f);//���ַ��õ���������
	StringFormat format;
	format.SetAlignment(StringAlignmentNear);    //�ı����з�ʽ�����ڶ�Ӧλ�þ��С����󡢿���

	str = "��Բ��Ԫ��";
	Gdiplus::PointF school_site(float(rc.left-wrc.left), float(rc.top-wrc.top));//���ַ��õ���������
	g.DrawString(str.AllocSysString(), str.GetLength(), &myFont, school_site, &format, &blackBrush);//��string���Ƶ�ͼ��
}

// ��ʾ�����壬���������ͼ���С���ı�
void ST_MYDISP::DisFontLarge(CString str, float x, float y, float size, SolidBrush &br)
{
	CRect rc;
	this->pCView->GetWindowRect(rc);

	CRect wrc;
	checkPatch::S_dlg.GetWindowRect(wrc);

	float fx = rc.left - wrc.left + x;
	float fy = rc.top - wrc.top + y;

	Gdiplus::Graphics g(checkPatch::S_dlg.GetSafeHwnd());
	CString szFont = "����";
	FontFamily fontFamily(szFont.AllocSysString());

	Gdiplus::Font myFont(&fontFamily, size, FontStyleRegular, UnitPoint); //�ڶ����������С

	StringFormat format;
	format.SetAlignment(StringAlignmentNear);    //�ı����з�ʽ�����ڶ�Ӧλ�þ��С����󡢿���

	Gdiplus::PointF school_site(fx, fy);//���ַ��õ���������

	g.DrawString(str.AllocSysString(), str.GetLength(), &myFont, school_site, &format, &br);//��string���Ƶ�ͼ��
}

//  ֱ�� Ԫ��
void ST_MYDISP::DisOnEleLineAdd()
{
	this->AppStateChangeTo(App_ElementAdding);

	try {

		int curProject = UCI::OPTIONS["��Ŀ���"];
		UCI::OptionsMap* o = UCI::ELE.add_One_Element(curProject, ELEMENT_LINE);

		//HalconCpp::HXLDCont RoiLineHXLD;
		double Row1, Column1, Row2, Column2;		
	
		this->write_message(12, 12, "��������һ��ֱ�ߣ��Ҽ�ȷ��", true);
		this->m_pMHWinMain->DrawLine(&(Row1), &(Column1), &(Row2), &(Column2));

		// ����ROI
		(*o)["ROI_Row1"] = Row1;
		(*o)["ROI_Column1"] = Column1;
		(*o)["ROI_Row2"] = Row2;
		(*o)["ROI_Column2"] = Column2;

		int hv_Elements = (*o)["Rakeֱ�߼�����"];
		double hv_DetectHeight = (*o)["Rakeֱ�߼��߶�"];
		double hv_DetectWidth = (*o)["Rakeֱ�߼����"];
		//����������� 
		draw_rake(&this->m_sIPResult.result_cont,
			hv_Elements,
			hv_DetectHeight,
			hv_DetectWidth,
			&Row1,
			&Column1,
			&Row2,
			&Column2);

		// �������ֱ��
		this->MeasureOneLine(o, this->m_sIPResult.result_img, this->m_sIPResult.result_cont);
		this->OnMainDisp();

	}
	catch (HalconCpp::HException& except) {
//		UCI::sDB.Log(except, "DisOnEleLineAdd()��");
	}

	this->AppStateChangeTo(APP_READY);
}

// Բ Ԫ��, 
void ST_MYDISP::DisOnEleCircleAdd()
{
	this->AppStateChangeTo(App_ElementAdding);

	try {
		
		//HTuple ROIRows, ROICols, Direct;

		int curProject = UCI::OPTIONS["��Ŀ���"];
		UCI::OptionsMap* o = UCI::ELE.add_One_Element(curProject, ELEMENT_CIRCLE);


	    // ׼����Ӧ���زġ� 1. ��Բ�ϵĵ㣬2����һ���߶Σ�����ȡ������
		HalconCpp::HXLDCont RoiCircleHXLD;
		HTuple  hv_Rows, hv_Cols, hv_Weights;

ReDrawCircle:
		write_message(12, 12, "1.��4�����ϵĵ�ȷ��һ��Բ�����Ҽ�ȷ��", true);		
		HXLDCont ho_ContOut1 = this->m_pMHWinMain->DrawNurbs("true", "true", "true", "true", 3, &hv_Rows,
			&hv_Cols, &hv_Weights);

		HTuple hv_Length1 = hv_Weights.Length();
		if (hv_Length1 < 4) {
			write_message(62, 12, "��ʾ������̫�٣����ػ�", true);
			goto ReDrawCircle;
		}

		//ROIRows = hv_Rows;
		//ROICols = hv_Cols;

		HXLDCont ho_Contour;
		ho_Contour.GenContourPolygonXld(hv_Rows, hv_Cols);    // ���ɶ����
		

		HTuple  hv_RowC, hv_ColumnC, hv_Radius, hv_StartPhi, hv_EndPhi;
		HTuple  hv_PointOrder;
		ho_Contour.FitCircleContourXld("algebraic", -1, 0, 0, 3, 2, &hv_RowC, &hv_ColumnC,
			&hv_Radius, &hv_StartPhi, &hv_EndPhi, &hv_PointOrder);  // ת����Բ���洢

		// ����ԭ���ĵ�
		(*o)["ROI_RowC"] = hv_RowC[0].D();
		(*o)["ROI_ColumnC"] = hv_ColumnC[0].D();
		(*o)["ROI_Radius"] = hv_Radius[0].D();
		(*o)["ROI_StartPhi"] = hv_StartPhi[0].D();
		(*o)["ROI_EndPhi"] = hv_EndPhi[0].D();
		CString str = hv_PointOrder[0].S();
		(*o)["ROI_PointOrder"] = str;

		draw_spoke(o, this->m_sIPResult.result_cont);

		// �������Բ
		this->MeasureOneCircle(o, this->m_sIPResult.result_img, this->m_sIPResult.result_cont);
		this->OnMainDisp();

	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());

	}

	this->AppStateChangeTo(APP_READY);
}

//  ��� Ԫ��
void ST_MYDISP::DisOnEleCameraAdd()
{
	int curProject = UCI::OPTIONS["��Ŀ���"];
	UCI::ELE.add_One_Element(curProject, ELEMENT_CAMERA);
}

// ��ֱ�߽ǶȲ��� 
void ST_MYDISP::DisOnEleAngle()
{
	int curProject = UCI::OPTIONS["��Ŀ���"];
	UCI::ELE.add_One_Element(curProject, ELEMENT_ANGLE);
}

// ���� Ԫ��
void ST_MYDISP::DisOnEleDistance()
{
	int curProject = UCI::OPTIONS["��Ŀ���"];
	UCI::ELE.add_One_Element(curProject, ELEMENT_DISTANCE);
}

// ����
void ST_MYDISP::DisMeaEleFunction()
{
	int curProject = UCI::OPTIONS["��Ŀ���"];
	UCI::ELE.add_One_Element(curProject, ELEMENT_FUNCTION);
}

// ƥ��
void ST_MYDISP::DisMeaEleMatching()
{
	int curProject = UCI::OPTIONS["��Ŀ���"];
	UCI::ELE.add_One_Element(curProject, ELEMENT_MATCH);
}

// ���ҳ���Ԫ��
void ST_MYDISP::DisOnMeaEleBalloonLength()
{
	int curProject = UCI::OPTIONS["��Ŀ���"];
	UCI::ELE.add_One_Element(curProject, ELEMENT_BALLOONLENGTH);
}



// ���� Ԫ��
void ST_MYDISP::DisOnEleOutReport()
{
	int curProject = UCI::OPTIONS["��Ŀ���"];
	UCI::ELE.add_One_Element(curProject, ELEMENT_RESULT);
}

// mm
void ST_MYDISP::DisOnMeaEleMillimeter()
{
	int curProject = UCI::OPTIONS["��Ŀ���"];
	UCI::ELE.add_One_Element(curProject, ELEMENT_MM);
}








