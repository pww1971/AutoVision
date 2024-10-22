#include "stdafx.h"

#include "MyData.h"
#include "MyDispLay.h"

using namespace HalconCpp;
using namespace std;


void ST_MYDISP::DisOnMatchTakeOne(UCI::OptionsMap *om)
{
	try {
		if (nullptr == om)
		{
			AfxMessageBox("û��ƥ�����,��ѡ��ƥ�䷽��");
			return ;
		}
		this->m_pMHWinMain->ClearWindow();

		// ���ֶ����������ǰ����
		HalconCpp::HXLDCont Contours;
		Contours.GenEmptyObj();
		HalconCpp::HImage hImg = UCI::pDis->GrabImageDisp;

		//UCI::OptionsMap *om = UCI::ELE.GetOptionsMapByEletype(*UCI::ELE.EleList,ELEMENT_CAMERA);
		//this->MeasureOneCamera(om, hImg, Contours);

		////	this->m_pMHWinMain->DispObj(hImg);

		//om = UCI::ELE.GetOptionsMapByEletype(*UCI::ELE.EleList,ELEMENT_ROI);
		//if (om == NULL)
		//{
		//	return;
		//}

		//this->MeasureOneROI(om, hImg, Contours);

		//	this->m_pMHWinMain->ClearWindow();
		//	this->m_pMHWinMain->DispObj(hImg);

		//om = UCI::ELE.GetOptionsMapByEletype(*UCI::ELE.EleList,ELEMENT_FUNCTION);
		//this->MeasureOneFUNCTION(om, hImg, Contours);
		//om = UCI::ELE.GetOptionsMapByEletype(ELEMENT_MATCH);
		this->MeasureOneMATCH(om, hImg, Contours);
		//	this->m_pMHWinMain->ClearWindow();
		//	this->m_pMHWinMain->DispObj(hImg);

//		EnterCriticalSection(&m_csIPDisp);
		this->m_sIPResult.result_img = hImg;
		this->m_sIPResult.result_cont = Contours;
//		LeaveCriticalSection(&m_csIPDisp);
		this->OnMainDisp();              // ��������ʾһ��

	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox("DisOnMatchTakeOne()��"+(CString)except.ErrorText().Text());
	}
}

// ��������ROI HXLD
void ST_MYDISP::DisDrawAllRoi(HalconCpp::HXLDCont& hv_Cont)
{
	POSITION pos = UCI::ELE.EleList->GetHeadPosition();
	while (pos != NULL) {
		UCI::OptionsMap *o = &UCI::ELE.EleList->GetNext(pos);
		this->DisDrawOneRoi(o, hv_Cont);
	}
}

void ST_MYDISP::DisDrawOneRoi(UCI::OptionsMap * o, HalconCpp::HXLDCont& hv_Cont)
{
	try {

		int type = (*o)["Ԫ������"];

		HalconCpp::HXLDCont hv_Out;

		switch (type) {
		case ELEMENT_LINE:
			{
				HTuple ht_Rows = (*o)["HTupleRows"];
				HTuple ht_Columns = (*o)["HTupleCols"];
				hv_Out.GenContourPolygonXld(ht_Rows, ht_Columns);

				//HTuple hv_Length = hv_Out.LengthXld();
				//this->write_message(300, 30, "hv_Length" + hv_Length);

				hv_Cont = hv_Cont.ConcatObj(hv_Out);
			}
			break;
		case ELEMENT_CIRCLE:
			{
				HTuple ht_Rows = (*o)["HTupleRows"];
				HTuple ht_Columns = (*o)["HTupleCols"];

				HTuple hv_RowC, hv_ColumnC, hv_Radius, hv_StartPhi, hv_EndPhi, hv_PointOrder;


				hv_Out.GenContourPolygonXld(ht_Rows, ht_Columns);
				//GenContourPolygonXld(&ho_Contour, (*hv_ROIRows), (*hv_ROICols));
				hv_Out.FitCircleContourXld("algebraic", -1, 0, 0, 3, 2, &hv_RowC, &hv_ColumnC,
					&hv_Radius, &hv_StartPhi, &hv_EndPhi, &hv_PointOrder);

				hv_Out.GenCircleContourXld(hv_RowC, hv_ColumnC, hv_Radius, hv_StartPhi,
					hv_EndPhi, hv_PointOrder, 3);
				//HTuple hv_Length = hv_Out.LengthXld();
				//this->write_message(300, 30, "hv_Length" + hv_Length);

			hv_Cont = hv_Cont.ConcatObj(hv_Out);

		}
		break;
		case ELEMENT_ELLIPSE:
			{
				HTuple ht_Rows = (*o)["HTupleRows"];
				HTuple ht_Columns = (*o)["HTupleCols"];

				HTuple hv_RowC, hv_ColumnC, hv_Phi, hv_Radius1, hv_Radius2, hv_StartPhi, hv_EndPhi, hv_PointOrder;


				hv_Out.GenContourPolygonXld(ht_Rows, ht_Columns);
				
				hv_Out.FitEllipseContourXld("fitzgibbon", -1, 0, 0, 200,3, 2, &hv_RowC, &hv_ColumnC,&hv_Phi,
					&hv_Radius1,&hv_Radius2, &hv_StartPhi, &hv_EndPhi, &hv_PointOrder);

				hv_Out.GenEllipseContourXld(hv_RowC, hv_ColumnC, hv_Phi,hv_Radius1,hv_Radius2, hv_StartPhi,
					hv_EndPhi, hv_PointOrder, 3);

				hv_Cont = hv_Cont.ConcatObj(hv_Out);

			}
			break;
			// ��ʾһ������Ԫ��
		case ELEMENT_DISTANCE:
			{
				// �õ�������������
				CString idDis1 = (*o)["����Ԫ������1"];
				CString idDis2 = (*o)["����Ԫ������1"];

				if (idDis1 == "" || idDis2 == "") return;

				if (idDis1 == idDis2) return;			

				UCI::OptionsMap * oDIS1 = UCI::ELE.GetOptionsMapByEleName(*UCI::ELE.EleList,idDis1);
				UCI::OptionsMap * oDIS2 = UCI::ELE.GetOptionsMapByEleName(*UCI::ELE.EleList,idDis2);

				CString eleParameter1 = (*o)["����Ԫ��1������"];
				CString eleParameter2 = (*o)["����Ԫ��2������"];

				int etype1 = (*oDIS1)["Ԫ������"];
				int etype2 = (*oDIS2)["Ԫ������"];


				double scalemm = UCI::OPTIONS["�������س���mm"];    // �������ص�mm�ı���

				HalconCpp::HXLDCont hx_tmp;

				// 1. �������ζ�û��ѡ��������
				if (eleParameter1 == "" && eleParameter2 == "")
				{
					if (etype1 == ELEMENT_CIRCLE && etype2 == ELEMENT_CIRCLE)
					{
						//// ����Բ, �������Բ��Բ�ľ�
						//double e1row = (*oDIS1)["RES_Circle_Center_Row"];
						//double e1col = (*oDIS1)["RES_Circle_Center_Column"];

						//double e2row = (*oDIS2)["RES_Circle_Center_Row"];
						//double e2col = (*oDIS2)["RES_Circle_Center_Column"];

						//this->gen_double_arrow_contour_xld(     // ����˫ͷ��ͷ
						//	&hx_tmp,
						//	e1row/scalemm,
						//	e1col / scalemm,
						//	e2row / scalemm,
						//	e2col / scalemm,
						//	alength,
						//	awidth);

						//hv_Cont = hv_Cont.ConcatObj(hx_tmp);
					}
					//else if ((etype1 == ELEMENT_LINE && etype2 == ELEMENT_CIRCLE)
					//	|| (etype2 == ELEMENT_LINE && etype1 == ELEMENT_CIRCLE))
					//{
					//	// Բ�ĵ�ֱ�ߵĴ�ֱ����
					//	double Rrow, Rcol;                     // Բ��
					//	double Lrow1, Lcol1, Lrow2, Lcol2;     // ֱ�߶����˵�

					//	UCI::OptionsMap * oCir = oDIS1;
					//	UCI::OptionsMap * oLine = oDIS2;

					//	if (etype1 == ELEMENT_LINE) {
					//		oCir = oDIS2;
					//		oLine = oDIS1;
					//	}


					//}
				}
			}
			break;
		case ELEMENT_ANGLE:
			break;
		default:
			break;
		}
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox("DisDrawOneRoi �� ROI ����"+(CString)except.ErrorText().Text());
	}
}

// ɾ�����е�ƥ�����
void ST_MYDISP::DisOnMatchDelAllRoi()
{

	this->m_sIPResult.result_cont.GenEmptyObj();
	hv_Model->ho_ModelRegion.GenEmptyRegion();
	hv_Model->hv_Model.Clear();
	hv_Model->hv_nccModel.Clear();
	// ��Ҫɾ������������ļ���ROI�ļ�
	//int prID = UCI::OPTIONS["��Ŀ���"];

	//CString stRegion = this->GetShapeModelRegionPathFile(prID);
	//CString stModel = this->GetShapeModelPathFile(prID);

	//CFile TempFile;
	//if (GetFileAttributes(stRegion) != -1) { // �ļ�������
	//	TempFile.Remove(stRegion);
	//}
	//if (GetFileAttributes(stModel) != -1) { // �ļ�������
	//	TempFile.Remove(stModel);
	//}

	this->OnMainDisp();
}
void ST_MYDISP::EditMatchROI(UCI::OptionsMap *o)
{
	HalconCpp::HXLDCont Contours;
	Contours.GenEmptyObj();
	Contours = Contours.ConcatObj(hv_Model->ho_ModelRegion);

//	EnterCriticalSection(&m_csIPDisp);
	this->m_sIPResult.result_cont = Contours;
//	LeaveCriticalSection(&m_csIPDisp);

	this->OnMainDisp();              // ��������ʾһ��
}