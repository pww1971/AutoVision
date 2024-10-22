#include "stdafx.h"

#include "MyData.h"
#include "MyDispLay.h"

using namespace HalconCpp;



void ST_MYDISP::draw_rake(
	HalconCpp::HXLDCont * ho_Regions,
	HalconCpp::HTuple hv_Elements,
	HalconCpp::HTuple hv_DetectHeight,
	HalconCpp::HTuple hv_DetectWidth,
	double * hv_Row1,
	double * hv_Column1,
	double * hv_Row2,
	double * hv_Column2)

{

	// Local iconic variables
	HXLDCont  ho_RegionLines, ho_Rectangle, ho_Arrow1;

	// Local control variables
	HTuple  hv_ATan, hv_i, hv_RowC, hv_ColC, hv_Distance;
	HTuple  hv_RowL2, hv_RowL1, hv_ColL2, hv_ColL1;

	//this->write_message(12, 12, "��������һ��ֱ�ߣ��Ҽ�ȷ��", true);

	
	//����һ������ʾ����������ʾ
	//ho_Regions->GenEmptyObj();
	//GenEmptyObj(&(*ho_Regions));

	//this->m_pMHWinMain->DrawLine(&(*hv_Row1), &(*hv_Column1), &(*hv_Row2), &(*hv_Column2));
	//����ֱ�� XLD
	ho_RegionLines.GenContourPolygonXld(HTuple(*hv_Row1).TupleConcat((*hv_Row2)), HTuple(*hv_Column1).TupleConcat((*hv_Column2)));
	//GenContourPolygonXld(&ho_RegionLines, HTuple(*hv_Row1).TupleConcat((*hv_Row2)), HTuple(*hv_Column1).TupleConcat((*hv_Column2)));
	//�洢����ʾ����
	*ho_Regions = ho_Regions->ConcatObj(ho_RegionLines);
	//ConcatObj((*ho_Regions), ho_RegionLines, &(*ho_Regions));
	//����ֱ����X��ļнǣ���ʱ�뷽��Ϊ����
	AngleLx((*hv_Row1), (*hv_Column1), (*hv_Row2), (*hv_Column2), &hv_ATan);
	//��Ե��ⷽ��ֱ���ֱ�ߣ�ֱ�߷�����ת90��Ϊ��Ե��ⷽ��
	hv_ATan += HTuple(90).TupleRad();
	//���ݼ��ֱ�߰�˳������������򣬲��洢����ʾ����
	{
		HTuple end_val13 = hv_Elements;
		HTuple step_val13 = 1;
		for (hv_i = 1; hv_i.Continue(end_val13, step_val13); hv_i += step_val13)
		{
			//���ֻ��һ���������Σ���Ϊ���߹��ߣ����Ϊ���ֱ�ߵĳ���
			if (0 != (hv_Elements == 1))
			{
				hv_RowC = ((*hv_Row1) + (*hv_Row2))*0.5;
				hv_ColC = ((*hv_Column1) + (*hv_Column2))*0.5;
				DistancePp((*hv_Row1), (*hv_Column1), (*hv_Row2), (*hv_Column2), &hv_Distance);
				ho_Rectangle.GenRectangle2ContourXld(hv_RowC, hv_ColC, hv_ATan, hv_DetectHeight / 2,
					hv_Distance / 2);
				//GenRectangle2ContourXld(&ho_Rectangle, hv_RowC, hv_ColC, hv_ATan, hv_DetectHeight / 2,
				//	hv_Distance / 2);
			}
			else
			{
				//����ж���������Σ������ò�������xld
				hv_RowC = (*hv_Row1) + ((((*hv_Row2) - (*hv_Row1))*(hv_i - 1)) / (hv_Elements - 1));
				hv_ColC = (*hv_Column1) + ((((*hv_Column2) - (*hv_Column1))*(hv_i - 1)) / (hv_Elements - 1));
				ho_Rectangle.GenRectangle2ContourXld(hv_RowC, hv_ColC, hv_ATan, hv_DetectHeight / 2,
					hv_DetectWidth / 2);
				//GenRectangle2ContourXld(&ho_Rectangle1, hv_RowC, hv_ColC, hv_ATan, hv_DetectHeight / 2,
				//	hv_DetectWidth / 2);

			}
			//�Ѳ�������XLD�洢����ʾ����
			*ho_Regions = ho_Regions->ConcatObj(ho_Rectangle);
			//ConcatObj((*ho_Regions), ho_Rectangle, &(*ho_Regions));
			if (0 != (hv_i == 1))
			{
				//�ڵ�һ���������λ���һ����ͷxld,����ֻ�Ǳ�Ե��ⷽ��
				hv_RowL2 = hv_RowC + ((hv_DetectHeight / 2)*((-hv_ATan).TupleSin()));
				hv_RowL1 = hv_RowC - ((hv_DetectHeight / 2)*((-hv_ATan).TupleSin()));
				hv_ColL2 = hv_ColC + ((hv_DetectHeight / 2)*((-hv_ATan).TupleCos()));
				hv_ColL1 = hv_ColC - ((hv_DetectHeight / 2)*((-hv_ATan).TupleCos()));
				
				gen_arrow_contour_xld(&ho_Arrow1, hv_RowL1, hv_ColL1, hv_RowL2, hv_ColL2,
					25, 25);

				//�Ѽ�ͷXLD�洢����ʾ����
				*ho_Regions = ho_Regions->ConcatObj(ho_Arrow1);	
			}
		}
	}
	return;
}



void ST_MYDISP::rake(
	HalconCpp::HImage ho_Image,
	HalconCpp::HXLDCont * ho_Regions,
	HalconCpp::HTuple hv_Elements,
	HalconCpp::HTuple hv_DetectHeight,
	HalconCpp::HTuple hv_DetectWidth,
	HalconCpp::HTuple hv_Sigma,
	HalconCpp::HTuple hv_Threshold,
	HalconCpp::HTuple hv_Transition,
	HalconCpp::HTuple hv_Select,
	HalconCpp::HTuple hv_Row1,
	HalconCpp::HTuple hv_Column1,
	HalconCpp::HTuple hv_Row2,
	HalconCpp::HTuple hv_Column2,
	HalconCpp::HTuple * hv_ResultRow,
	HalconCpp::HTuple * hv_ResultColumn)

{

	// Local iconic variables
	HXLDCont  ho_RegionLines, ho_Rectangle, ho_Arrow1;

	// Local control variables
	Hlong  hv_Width, hv_Height;
	HTuple  hv_ATan; // 
	HTuple  hv_i, hv_RowC, hv_ColC, hv_Distance, hv_RowL2, hv_RowL1;
	HTuple  hv_ColL2, hv_ColL1,  hv_RowEdge;
	HTuple  hv_ColEdge, hv_Amplitude, hv_tRow, hv_tCol, hv_t;
	HTuple  hv_Number, hv_j;
	//HTuple hv_MsrHandle_Measure;
	HMeasure hv_Measure;

	//��ȡͼ��ߴ�
	ho_Image.GetImageSize(&hv_Width, &hv_Height);
	//GetImageSize(ho_Image, &hv_Width, &hv_Height);
	//����һ������ʾ����������ʾ
	ho_Regions->GenEmptyObj();
	//GenEmptyObj(&(*ho_Regions));
	//��ʼ����Ե��������
	(*hv_ResultRow) = HTuple();
	(*hv_ResultColumn) = HTuple();
	//����ֱ��xld
	ho_RegionLines.GenContourPolygonXld(hv_Row1.TupleConcat(hv_Row2), hv_Column1.TupleConcat(hv_Column2));
	//GenContourPolygonXld(&ho_RegionLines, hv_Row1.TupleConcat(hv_Row2), hv_Column1.TupleConcat(hv_Column2));
	//�洢����ʾ����
	*ho_Regions = ho_Regions->ConcatObj(ho_RegionLines);
	//ConcatObj((*ho_Regions), ho_RegionLines, &(*ho_Regions));   //DispObj(*ho_Regions, this->hv_HWD); return;
	//����ֱ����x��ļнǣ���ʱ�뷽��Ϊ����
	AngleLx(hv_Row1, hv_Column1, hv_Row2, hv_Column2, &hv_ATan);  //write_message(200, 20, hv_ATan);
	//��Ե��ⷽ��ֱ�ڼ��ֱ�ߣ� ֱ�߷���������ת90��Ϊ��Ե��ⷽ��
	hv_ATan += HTuple(90).TupleRad();                            //write_message(200, 220, "90:" + hv_ATan);
	//���ݼ��ֱ�߰�˳���������������Σ����洢����ʾ����
	{
		HTuple end_val16 = hv_Elements;
		HTuple step_val16 = 1;
		for (hv_i = 1; hv_i.Continue(end_val16, step_val16); hv_i += step_val16)
		{
			if (0 != (hv_Elements == 1))
			{
				hv_RowC = (hv_Row1 + hv_Row2)*0.5;
				hv_ColC = (hv_Column1 + hv_Column2)*0.5;
				//�ж��Ƿ񳬳�ͼ�񣬳���������Ե
				if (0 != (HTuple(HTuple(HTuple(hv_RowC>(hv_Height - 1)).TupleOr(hv_RowC<0)).TupleOr(hv_ColC>(hv_Width - 1))).TupleOr(hv_ColC<0)))
				{
					continue;
				}
				DistancePp(hv_Row1, hv_Column1, hv_Row2, hv_Column2, &hv_Distance);
				hv_DetectWidth = hv_Distance;
				ho_Rectangle.GenRectangle2ContourXld(hv_RowC, hv_ColC, hv_ATan, hv_DetectHeight / 2,
					hv_Distance / 2);
				//GenRectangle2ContourXld(&ho_Rectangle, hv_RowC, hv_ColC, hv_ATan, hv_DetectHeight / 2,
				//	hv_Distance / 2);
			}
			else
			{
				//����ж���������Σ������ò�������xld
				hv_RowC = hv_Row1 + (((hv_Row2 - hv_Row1)*(hv_i - 1)) / (hv_Elements - 1));
				hv_ColC = hv_Column1 + (((hv_Column2 - hv_Column1)*(hv_i - 1)) / (hv_Elements - 1));
				if (0 != (HTuple(HTuple(HTuple(hv_RowC>(hv_Height - 1)).TupleOr(hv_RowC<0)).TupleOr(hv_ColC>(hv_Width - 1))).TupleOr(hv_ColC<0)))
				{
					continue;
				}

				//write_message(100, 12, "hv_RowC:" + hv_RowC);
				//write_message(150, 12, "hv_Col:" + hv_ColC);
				//write_message(200, 12, "hv_ATan:" + hv_ATan);

				//write_message(250, 12, "hv_DetectHeight:" + hv_DetectHeight);
				//write_message(300, 12, "hv_Distance:" + hv_Distance);

				ho_Rectangle.GenRectangle2ContourXld(hv_RowC, hv_ColC, hv_ATan, hv_DetectHeight / 2,
					hv_DetectWidth / 2);   //DispObj(ho_Rectangle, this->hv_HWD); return;
				//GenRectangle2ContourXld(&ho_Rectangle, hv_RowC, hv_ColC, hv_ATan, hv_DetectHeight / 2,
				//	hv_DetectWidth / 2);   //DispObj(ho_Rectangle, this->hv_HWD); return;
			}
			//�Ѳ�������xld�洢����ʾ����
			*ho_Regions = ho_Regions->ConcatObj(ho_Rectangle);
			//ConcatObj((*ho_Regions), ho_Rectangle, &(*ho_Regions));
			if (0 != (hv_i == 1))
			{
				//�ڵ�һ���������λ���һ����ͷxld,����ֻ�Ǳ�Ե��ⷽ��
				hv_RowL2 = hv_RowC + ((hv_DetectHeight / 2)*((-hv_ATan).TupleSin()));
				hv_RowL1 = hv_RowC - ((hv_DetectHeight / 2)*((-hv_ATan).TupleSin()));
				hv_ColL2 = hv_ColC + ((hv_DetectHeight / 2)*((-hv_ATan).TupleCos()));
				hv_ColL1 = hv_ColC - ((hv_DetectHeight / 2)*((-hv_ATan).TupleCos()));
				gen_arrow_contour_xld(&ho_Arrow1, hv_RowL1, hv_ColL1, hv_RowL2, hv_ColL2, 25,
					25);
				//��xld�洢����ʾ����
				*ho_Regions = ho_Regions->ConcatObj(ho_Arrow1);
				//ConcatObj((*ho_Regions), ho_Arrow1, &(*ho_Regions));
			}

			// DispObj(*ho_Regions, this->hv_HWD); return;

			//��������������
			hv_Measure.GenMeasureRectangle2(hv_RowC, hv_ColC, hv_ATan, hv_DetectHeight / 2, hv_DetectWidth / 2,
				hv_Width, hv_Height, (HString)"nearest_neighbor");
			//GenMeasureRectangle2(hv_RowC, hv_ColC, hv_ATan, hv_DetectHeight / 2, hv_DetectWidth / 2,
			//	hv_Width, hv_Height, "nearest_neighbor", &hv_MsrHandle_Measure);
			//���ü���
			if (0 != (hv_Transition == HTuple("negative")))
			{
				hv_Transition = "negative";
			}
			else
			{
				if (0 != (hv_Transition == HTuple("positive")))
				{
					hv_Transition = "positive";
				}
				else
				{
					hv_Transition = "all";
				}
			}
			//���ñ�Եλ�ã���ǿ���Ǵ����б�Ե��ѡ����Ⱦ���ֵ���㣬��Ҫ����Ϊ'all'
			if (0 != (hv_Select == HTuple("first")))
			{
				hv_Select = "first";
			}
			else
			{
				if (0 != (hv_Select == HTuple("last")))
				{
					hv_Select = "last";
				}
				else
				{
					hv_Select = "all";
				}
			}

			hv_Measure.MeasurePos(ho_Image, hv_Sigma, hv_Threshold, hv_Transition,
				hv_Select, &hv_RowEdge, &hv_ColEdge, &hv_Amplitude, &hv_Distance);

			//����Ե
			//MeasurePos(ho_Image, hv_MsrHandle_Measure, hv_Sigma, hv_Threshold, hv_Transition,
			//	hv_Select, &hv_RowEdge, &hv_ColEdge, &hv_Amplitude, &hv_Distance);
			
				
			//�������������
			//CloseMeasure(hv_MsrHandle_Measure);


			//��ʱ������ʼ��
			//tRow,tCol�����ҵ�ָ����Ե������
			hv_tRow = 0;
			hv_tCol = 0;
			//t�����Ե�ķ��Ⱦ���ֵ
			hv_t = 0;
			//�ҵ��ı�Ե��������Ϊ1��
			TupleLength(hv_RowEdge, &hv_Number);
			if (0 != (hv_Number<1))
			{
				continue;
			}
			//�ж����Եʱ��ѡ����Ⱦ���ֵ���ı�Ե
			{
				HTuple end_val88 = hv_Number - 1;
				HTuple step_val88 = 1;
				for (hv_j = 0; hv_j.Continue(end_val88, step_val88); hv_j += step_val88)
				{
					if (0 != ((HTuple(hv_Amplitude[hv_j]).TupleAbs())>hv_t))
					{
						hv_tRow = ((const HTuple&)hv_RowEdge)[hv_j];
						hv_tCol = ((const HTuple&)hv_ColEdge)[hv_j];
						hv_t = HTuple(hv_Amplitude[hv_j]).TupleAbs();
					}
				}
			}
			//���ҵ��ı�Ե�������������
			if (0 != (hv_t>0))
			{
				(*hv_ResultRow) = (*hv_ResultRow).TupleConcat(hv_tRow);
				(*hv_ResultColumn) = (*hv_ResultColumn).TupleConcat(hv_tCol);
			}
		}
	}
	return;
}

void ST_MYDISP::pts_to_best_line
(
	HalconCpp::HXLDCont * ho_Line,
	HalconCpp::HTuple hv_Rows,
	HalconCpp::HTuple hv_Cols,
	HalconCpp::HTuple hv_ActiveNum,
	HalconCpp::HTuple * hv_Row1,
	HalconCpp::HTuple * hv_Column1,
	HalconCpp::HTuple * hv_Row2,
	HalconCpp::HTuple * hv_Column2)

{

	// Local iconic variables
	HObject  ho_Contour;

	// Local control variables
	HTuple  hv_Length, hv_Nr, hv_Nc, hv_Dist, hv_Length1;

	//��ʼ��
	(*hv_Row1) = 0;
	(*hv_Column1) = 0;
	(*hv_Row2) = 0;
	(*hv_Column2) = 0;
	//����һ���յ�ֱ�߶���
	GenEmptyObj(&(*ho_Line));
	//�����Ե����
	TupleLength(hv_Cols, &hv_Length);
	//����Ե������С����Ч����ʱ�������
	if (0 != (HTuple(hv_Length >= hv_ActiveNum).TupleAnd(hv_ActiveNum>1)))
	{
		//halcon������ǻ���xld�ģ���Ҫ�ѱ�Ե���ӳ�xld
		GenContourPolygonXld(&ho_Contour, hv_Rows, hv_Cols);
		//���ֱ�ߣ�ʹ�õ��㷨��'tukey',�����㷨��ο�fit_line_contour_xld����������
		FitLineContourXld(ho_Contour, "tukey", -1, 0, 5, 2, &(*hv_Row1), &(*hv_Column1),
			&(*hv_Row2), &(*hv_Column2), &hv_Nr, &hv_Nc, &hv_Dist);
		//�ж����ֱ���Ƿ�Ч�������ϳɹ���������Ԫ�ص���������0
		TupleLength(hv_Dist, &hv_Length1);
		if (0 != (hv_Length<1))
		{
			return;
		}
		//������Ͻ��������ֱ��xld
		GenContourPolygonXld(&(*ho_Line), (*hv_Row1).TupleConcat((*hv_Row2)), (*hv_Column1).TupleConcat((*hv_Column2)));
	}
	return;
}

void ST_MYDISP::draw_spoke(UCI::OptionsMap * o, HalconCpp::HXLDCont &Cont)
{
	
	try {

		double ROI_RowC = (*o)["ROI_RowC"];
		double ROI_ColumnC = (*o)["ROI_ColumnC"];
		double ROI_Radius = (*o)["ROI_Radius"];
		double ROI_StartPhi = (*o)["ROI_StartPhi"];
		double ROI_EndPhi = (*o)["ROI_EndPhi"];
		CString ROI_PointOrder = (*o)["ROI_PointOrder"];

		HXLDCont ho_ContCircle;
		ho_ContCircle.GenCircleContourXld(ROI_RowC,
			ROI_ColumnC,
			ROI_Radius,
			ROI_StartPhi,
			ROI_EndPhi, ROI_PointOrder.GetBuffer(), 3.0f);

		HTuple hv_RowXLD, hv_ColXLD;
		ho_ContCircle.GetContourXld(&hv_RowXLD, &hv_ColXLD);

		//���xld��Բ������Length2���㣬����㿪ʼ���ȼ�ࣨ���ΪLength2/(Element2-1),ȡELements���㣬��Ϊ���߹��ߵ��е�
		//���xld��Բ����Length2���㣬��0��Ϊ��㣬�ȼ�ࣨ���ΪLength2/(Element2-1),ȡELements���㣬��Ϊ���߹��ߵ��е�

		int elements = (*o)["Spoke_Circle_Elements"];
		double detect_height = (*o)["Spoke_Circle_Detect_Height"];
		double detect_width = (*o)["Spoke_Circle_Detect_Width"];
		//�õ�Բ��Բ���ϵ������
		HTuple hv_Length2 = hv_ColXLD.Length();

		HTuple end_val42 = elements - 1;
		HTuple step_val42 = 1;
		HTuple hv_i, hv_j;
		HTuple hv_RowE, hv_ColE;
		HTuple hv_ATan;

		for (hv_i = 0; hv_i.Continue(end_val42, step_val42); hv_i += step_val42)
		{

			if (0 != (HTuple(hv_RowXLD[0]) == HTuple(hv_RowXLD[hv_Length2 - 1])))
			{
				//xld�������յ�������ԣ�ΪԲ
				TupleInt(((1.0*hv_Length2) / elements)*hv_i, &hv_j);

				(*o)["HTupleArcOrCircle"] = 0;
			}
			else
			{
				//����ΪԲ��
				TupleInt(((1.0*hv_Length2) / (elements - 1))*hv_i, &hv_j);

				(*o)["HTupleArcOrCircle"] = 1;
			}
			// ����Խ��,ǿ�Ƹ�ֵΪ���һ������
			if (0 != (hv_j >= hv_Length2))
			{
				hv_j = hv_Length2 - 1;
				//	continue;
			}
			//��ȡ��������
			hv_RowE = ((const HTuple&)hv_RowXLD)[hv_j];
			hv_ColE = ((const HTuple&)hv_ColXLD)[hv_j];


			//��������xld,�����ֵ���ʾ����
			HXLDCont ho_Rectangle1;
			ho_Rectangle1.GenRectangle2ContourXld(hv_RowE, hv_ColE, hv_ATan, HTuple(detect_height / 2),
				HTuple(detect_width / 2));

			Cont = Cont.ConcatObj(ho_Rectangle1);

			if (0 != (hv_i == 0))
			{
				HTuple hv_RowL2 = hv_RowE + ((detect_height / 2)*((-hv_ATan).TupleSin()));
				HTuple hv_RowL1 = hv_RowE - ((detect_height / 2)*((-hv_ATan).TupleSin()));
				HTuple hv_ColL2 = hv_ColE + ((detect_height / 2)*((-hv_ATan).TupleCos()));
				HTuple hv_ColL1 = hv_ColE - ((detect_height / 2)*((-hv_ATan).TupleCos()));

				HXLDCont ho_Arrow1;
				gen_arrow_contour_xld(&ho_Arrow1, hv_RowL1, hv_ColL1, hv_RowL2, hv_ColL2, 25,
					25);
				Cont = Cont.ConcatObj(ho_Arrow1);
			}
		}
	}
	catch (HException& except) {
//		UCI::sDB.Log(except, "draw_spoke()��");

	}
	

}



// ������Բ��
void ST_MYDISP::draw_spoke(
	HalconCpp::HXLDCont * ho_Regions,
	HalconCpp::HTuple hv_Elements,
	HalconCpp::HTuple hv_DetectHeight,
	HalconCpp::HTuple hv_DetectWidth,
	HalconCpp::HTuple * hv_ROIRows,
	HalconCpp::HTuple * hv_ROICols,
	HalconCpp::HTuple * hv_Direct)

{

	// Local iconic variables
	HXLDCont  ho_ContOut1, ho_Contour, ho_ContCircle;
	HXLDCont  ho_Cross, ho_Rectangle1, ho_Arrow1;

	// Local control variables
	HTuple  hv_Rows, hv_Cols, hv_Weights, hv_Length1;
	HTuple  hv_RowC, hv_ColumnC, hv_Radius, hv_StartPhi, hv_EndPhi;
	HTuple  hv_PointOrder, hv_RowXLD, hv_ColXLD;
	double	hv_Row1, hv_Column1;
	double  hv_Row2, hv_Column2;
	HTuple  hv_DistanceStart, hv_DistanceEnd;
	HTuple  hv_Length2, hv_i, hv_j, hv_RowE, hv_ColE, hv_ATan;
	HTuple  hv_RowL2, hv_RowL1, hv_ColL2, hv_ColL1;

	write_message(12, 12, "1.��4�����ϵĵ�ȷ��һ��Բ�����Ҽ�ȷ��", true);
	ho_Regions->GenEmptyObj();
	//GenEmptyObj(&(*ho_Regions));
	//DrawNurbs(&ho_ContOut1, hv_WindowHandle, "true", "true", "true", "true", 3, &hv_Rows,
	//	&hv_Cols, &hv_Weights);

	ho_ContOut1 = this->m_pMHWinMain->DrawNurbs("true", "true", "true", "true", 3, &hv_Rows,
		&hv_Cols, &hv_Weights);

	TupleLength(hv_Weights, &hv_Length1);
	if (0 != (hv_Length1<4))
	{
		write_message(62, 12, "��ʾ������̫�٣����ػ�", true);
		(*hv_ROIRows) = HTuple();
		(*hv_ROICols) = HTuple();
		return;
	}
	(*hv_ROIRows) = hv_Rows;
	(*hv_ROICols) = hv_Cols;
	ho_Contour.GenContourPolygonXld((*hv_ROIRows), (*hv_ROICols));
	//GenContourPolygonXld(&ho_Contour, (*hv_ROIRows), (*hv_ROICols));
	ho_Contour.FitCircleContourXld("algebraic", -1, 0, 0, 3, 2, &hv_RowC, &hv_ColumnC,
		&hv_Radius, &hv_StartPhi, &hv_EndPhi, &hv_PointOrder);
	//FitCircleContourXld(ho_Contour, "algebraic", -1, 0, 0, 3, 2, &hv_RowC, &hv_ColumnC,
	//	&hv_Radius, &hv_StartPhi, &hv_EndPhi, &hv_PointOrder);
	ho_ContCircle.GenCircleContourXld(hv_RowC, hv_ColumnC, hv_Radius, hv_StartPhi,
		hv_EndPhi, hv_PointOrder, 3);
	//GenCircleContourXld(&ho_ContCircle, hv_RowC, hv_ColumnC, hv_Radius, hv_StartPhi,
	//	hv_EndPhi, hv_PointOrder, 3);
	*ho_Regions = ho_Regions->ConcatObj(ho_ContCircle);
	//ConcatObj((*ho_Regions), ho_ContCircle, &(*ho_Regions));
	ho_ContCircle.GetContourXld(&hv_RowXLD, &hv_ColXLD);
	// GetContourXld(ho_ContCircle, &hv_RowXLD, &hv_ColXLD);

	//this->m_pMHWinMain->DispObj(ho_Image);
	//if (HDevWindowStack::IsOpen())
	//	DispObj(ho_Image, HDevWindowStack::GetActive());
	this->m_pMHWinMain->DispObj(ho_ContCircle);
	//if (HDevWindowStack::IsOpen())
	//	DispObj(ho_ContCircle, HDevWindowStack::GetActive());
	ho_Cross.GenCrossContourXld(hv_RowC, hv_ColumnC, HTuple(60), 0.785398);
	//GenCrossContourXld(&ho_Cross, hv_RowC, hv_ColumnC, 60, 0.785398);
	this->m_pMHWinMain->DispObj(ho_Cross);
	//if (HDevWindowStack::IsOpen())
	//	DispObj(ho_Cross, HDevWindowStack::GetActive());

	write_message(12, 12, "2) Զ��Բ�ģ�����ͷȷ����Ե��ⷽ���Ҽ�ȷ��",true);
	//����ȷ�ϼ�ⷽ��
	this->m_pMHWinMain->DrawLine(&hv_Row1, &hv_Column1, &hv_Row2, &hv_Column2);

	DistancePp(hv_RowC, hv_ColumnC, hv_Row1, hv_Column1, &hv_DistanceStart);
	//��Բ�ĵ���ⷽ��ֱ���յ�ľ���
	DistancePp(hv_RowC, hv_ColumnC, hv_Row2, hv_Column2, &hv_DistanceEnd);
	//�õ�Բ��Բ���ϵ������
	TupleLength(hv_ColXLD, &hv_Length2);

	if (0 != (hv_Elements<3))
	{
		(*hv_ROIRows) = HTuple();
		(*hv_ROICols) = HTuple();
		write_message(62, 12, "���ı�Ե����̫�٣�����������", true);
		return;
	}
	//���xld��Բ������Length2���㣬����㿪ʼ���ȼ�ࣨ���ΪLength2/(Element2-1),ȡELements���㣬��Ϊ���߹��ߵ��е�
	//���xld��Բ����Length2���㣬��0��Ϊ��㣬�ȼ�ࣨ���ΪLength2/(Element2-1),ȡELements���㣬��Ϊ���߹��ߵ��е�
	{
		HTuple end_val42 = hv_Elements - 1;
		HTuple step_val42 = 1;
		for (hv_i = 0; hv_i.Continue(end_val42, step_val42); hv_i += step_val42)
		{

			if (0 != (HTuple(hv_RowXLD[0]) == HTuple(hv_RowXLD[hv_Length2 - 1])))
			{
				//xld�������յ�������ԣ�ΪԲ
				TupleInt(((1.0*hv_Length2) / hv_Elements)*hv_i, &hv_j);
			}
			else
			{
				//����ΪԲ��
				TupleInt(((1.0*hv_Length2) / (hv_Elements - 1))*hv_i, &hv_j);
			}
			// ����Խ��,ǿ�Ƹ�ֵΪ���һ������
			if (0 != (hv_j >= hv_Length2))
			{
				hv_j = hv_Length2 - 1;
		  	   //	continue;
			}
			//��ȡ��������
			hv_RowE = ((const HTuple&)hv_RowXLD)[hv_j];
			hv_ColE = ((const HTuple&)hv_ColXLD)[hv_j];

			//���Բ�ĵ���ⷽ��ֱ�ߵ����������Բ�ĵ���ⷽ��ֱ�ߵ��յ�ľ��룬����������Բ��ָ��Բ��
			if (0 != (hv_DistanceStart>hv_DistanceEnd))
			{
				TupleAtan2((-hv_RowE) + hv_RowC, hv_ColE - hv_ColumnC, &hv_ATan);
				hv_ATan = (HTuple(180).TupleRad()) + hv_ATan;
				(*hv_Direct) = "inner";
			}
			else
			{
				TupleAtan2((-hv_RowE) + hv_RowC, hv_ColE - hv_ColumnC, &hv_ATan);
				(*hv_Direct) = "outer";
			}
			//��������xld,�����ֵ���ʾ����
			ho_Rectangle1.GenRectangle2ContourXld(hv_RowE, hv_ColE, hv_ATan, hv_DetectHeight / 2,
				hv_DetectWidth / 2);
			//GenRectangle2ContourXld(&ho_Rectangle1, hv_RowE, hv_ColE, hv_ATan, hv_DetectHeight / 2,
			//	hv_DetectWidth / 2);
			*ho_Regions = ho_Regions->ConcatObj(ho_Rectangle1);
			//ConcatObj((*ho_Regions), ho_Rectangle1, &(*ho_Regions));

			if (0 != (hv_i == 0))
			{
				hv_RowL2 = hv_RowE + ((hv_DetectHeight / 2)*((-hv_ATan).TupleSin()));
				hv_RowL1 = hv_RowE - ((hv_DetectHeight / 2)*((-hv_ATan).TupleSin()));
				hv_ColL2 = hv_ColE + ((hv_DetectHeight / 2)*((-hv_ATan).TupleCos()));
				hv_ColL1 = hv_ColE - ((hv_DetectHeight / 2)*((-hv_ATan).TupleCos()));
				gen_arrow_contour_xld(&ho_Arrow1, hv_RowL1, hv_ColL1, hv_RowL2, hv_ColL2, 25,
					25);
				*ho_Regions = ho_Regions->ConcatObj(ho_Arrow1);
				//ConcatObj((*ho_Regions), ho_Arrow1, &(*ho_Regions));
			}
		}
	}
	return;
}

//void ST_MYDISP::draw_spoke(
//	HalconCpp::HXLDCont * ho_Regions, 
//	HalconCpp::HTuple hv_Elements, 
//	HalconCpp::HTuple hv_DetectHeight, 
//	HalconCpp::HTuple hv_DetectWidth, 
//	HalconCpp::HTuple * hv_ROIRows, 
//	HalconCpp::HTuple * hv_ROICols, 
//	HalconCpp::HTuple * hv_Direct, 
//	HalconCpp::HTuple * hvv_Radius, 
//	HalconCpp::HTuple * hvv_StartPhi, 
//	HalconCpp::HTuple * hvv_EndPhi, 
//	HalconCpp::HTuple * hvv_PointOrder)
//{
//	// Local iconic variables
//	HXLDCont  ho_ContOut1, ho_Contour, ho_ContCircle;
//	HXLDCont  ho_Cross, ho_Rectangle1, ho_Arrow1;
//
//	// Local control variables
//	HTuple  hv_Rows, hv_Cols, hv_Weights, hv_Length1;
//	HTuple  hv_RowC, hv_ColumnC;
//
//	//HTuple  hv_Radius, hv_StartPhi, hv_EndPhi;
//	//HTuple  hv_PointOrder;
//
//	HTuple  hv_RowXLD, hv_ColXLD;
//	double	hv_Row1, hv_Column1;
//	double  hv_Row2, hv_Column2;
//	HTuple  hv_DistanceStart, hv_DistanceEnd;
//	HTuple  hv_Length2, hv_i, hv_j, hv_RowE, hv_ColE, hv_ATan;
//	HTuple  hv_RowL2, hv_RowL1, hv_ColL2, hv_ColL1;
//
//	write_message(12, 12, "1.��4�����ϵĵ�ȷ��һ��Բ�����Ҽ�ȷ��", true);
//	ho_Regions->GenEmptyObj();
//	//GenEmptyObj(&(*ho_Regions));
//	//DrawNurbs(&ho_ContOut1, hv_WindowHandle, "true", "true", "true", "true", 3, &hv_Rows,
//	//	&hv_Cols, &hv_Weights);
//
//	ho_ContOut1 = this->m_pMHWinMain->DrawNurbs("true", "true", "true", "true", 3, &hv_Rows,
//		&hv_Cols, &hv_Weights);
//
//	TupleLength(hv_Weights, &hv_Length1);
//	if (0 != (hv_Length1<4))
//	{
//		write_message(62, 12, "��ʾ������̫�٣����ػ�", true);
//		(*hv_ROIRows) = HTuple();
//		(*hv_ROICols) = HTuple();
//		return;
//	}
//	(*hv_ROIRows) = hv_Rows;
//	(*hv_ROICols) = hv_Cols;
//	ho_Contour.GenContourPolygonXld((*hv_ROIRows), (*hv_ROICols));
//	//GenContourPolygonXld(&ho_Contour, (*hv_ROIRows), (*hv_ROICols));
//	ho_Contour.FitCircleContourXld("algebraic", -1, 0, 0, 3, 2, &hv_RowC, &hv_ColumnC,
//		hvv_Radius, hvv_StartPhi, hvv_EndPhi, hvv_PointOrder);
//	//FitCircleContourXld(ho_Contour, "algebraic", -1, 0, 0, 3, 2, &hv_RowC, &hv_ColumnC,
//	//	&hv_Radius, &hv_StartPhi, &hv_EndPhi, &hv_PointOrder);
//	ho_ContCircle.GenCircleContourXld(hv_RowC, hv_ColumnC, *hvv_Radius, *hvv_StartPhi,
//		*hvv_EndPhi, *hvv_PointOrder, 3);
//
//	// �ٱ���һ��
//	(*hv_ROIRows) = hv_RowC;
//	(*hv_ROICols) = hv_ColumnC;
//
//	//GenCircleContourXld(&ho_ContCircle, hv_RowC, hv_ColumnC, hv_Radius, hv_StartPhi,
//	//	hv_EndPhi, hv_PointOrder, 3);
//	*ho_Regions = ho_Regions->ConcatObj(ho_ContCircle);
//	//ConcatObj((*ho_Regions), ho_ContCircle, &(*ho_Regions));
//	ho_ContCircle.GetContourXld(&hv_RowXLD, &hv_ColXLD);
//	// GetContourXld(ho_ContCircle, &hv_RowXLD, &hv_ColXLD);
//
//	//this->m_pMHWinMain->DispObj(ho_Image);
//	//if (HDevWindowStack::IsOpen())
//	//	DispObj(ho_Image, HDevWindowStack::GetActive());
//	this->m_pMHWinMain->DispObj(ho_ContCircle);
//	//if (HDevWindowStack::IsOpen())
//	//	DispObj(ho_ContCircle, HDevWindowStack::GetActive());
//	ho_Cross.GenCrossContourXld(hv_RowC, hv_ColumnC, HTuple(60), 0.785398);
//	//GenCrossContourXld(&ho_Cross, hv_RowC, hv_ColumnC, 60, 0.785398);
//	this->m_pMHWinMain->DispObj(ho_Cross);
//	//if (HDevWindowStack::IsOpen())
//	//	DispObj(ho_Cross, HDevWindowStack::GetActive());
//
//	write_message(12, 12, "2) Զ��Բ�ģ�����ͷȷ����Ե��ⷽ���Ҽ�ȷ��", true);
//	//����ȷ�ϼ�ⷽ��
//	this->m_pMHWinMain->DrawLine(&hv_Row1, &hv_Column1, &hv_Row2, &hv_Column2);
//
//	DistancePp(hv_RowC, hv_ColumnC, hv_Row1, hv_Column1, &hv_DistanceStart);
//	//��Բ�ĵ���ⷽ��ֱ���յ�ľ���
//	DistancePp(hv_RowC, hv_ColumnC, hv_Row2, hv_Column2, &hv_DistanceEnd);
//	//�õ�Բ��Բ���ϵ������
//	TupleLength(hv_ColXLD, &hv_Length2);
//
//	if (0 != (hv_Elements<3))
//	{
//		(*hv_ROIRows) = HTuple();
//		(*hv_ROICols) = HTuple();
//		write_message(62, 12, "���ı�Ե����̫�٣�����������", true);
//		return;
//	}
//	//���xld��Բ������Length2���㣬����㿪ʼ���ȼ�ࣨ���ΪLength2/(Element2-1),ȡELements���㣬��Ϊ���߹��ߵ��е�
//	//���xld��Բ����Length2���㣬��0��Ϊ��㣬�ȼ�ࣨ���ΪLength2/(Element2-1),ȡELements���㣬��Ϊ���߹��ߵ��е�
//	{
//		HTuple end_val42 = hv_Elements - 1;
//		HTuple step_val42 = 1;
//		for (hv_i = 0; hv_i.Continue(end_val42, step_val42); hv_i += step_val42)
//		{
//
//			if (0 != (HTuple(hv_RowXLD[0]) == HTuple(hv_RowXLD[hv_Length2 - 1])))
//			{
//				//xld�������յ�������ԣ�ΪԲ
//				TupleInt(((1.0*hv_Length2) / hv_Elements)*hv_i, &hv_j);
//			}
//			else
//			{
//				//����ΪԲ��
//				TupleInt(((1.0*hv_Length2) / (hv_Elements - 1))*hv_i, &hv_j);
//			}
//			// ����Խ��,ǿ�Ƹ�ֵΪ���һ������
//			if (0 != (hv_j >= hv_Length2))
//			{
//				hv_j = hv_Length2 - 1;
//				//	continue;
//			}
//			//��ȡ��������
//			hv_RowE = ((const HTuple&)hv_RowXLD)[hv_j];
//			hv_ColE = ((const HTuple&)hv_ColXLD)[hv_j];
//
//			//���Բ�ĵ���ⷽ��ֱ�ߵ����������Բ�ĵ���ⷽ��ֱ�ߵ��յ�ľ��룬����������Բ��ָ��Բ��
//			if (0 != (hv_DistanceStart>hv_DistanceEnd))
//			{
//				TupleAtan2((-hv_RowE) + hv_RowC, hv_ColE - hv_ColumnC, &hv_ATan);
//				hv_ATan = (HTuple(180).TupleRad()) + hv_ATan;
//				(*hv_Direct) = "inner";
//			}
//			else
//			{
//				TupleAtan2((-hv_RowE) + hv_RowC, hv_ColE - hv_ColumnC, &hv_ATan);
//				(*hv_Direct) = "outer";
//			}
//			//��������xld,�����ֵ���ʾ����
//			ho_Rectangle1.GenRectangle2ContourXld(hv_RowE, hv_ColE, hv_ATan, hv_DetectHeight / 2,
//				hv_DetectWidth / 2);
//			//GenRectangle2ContourXld(&ho_Rectangle1, hv_RowE, hv_ColE, hv_ATan, hv_DetectHeight / 2,
//			//	hv_DetectWidth / 2);
//			*ho_Regions = ho_Regions->ConcatObj(ho_Rectangle1);
//			//ConcatObj((*ho_Regions), ho_Rectangle1, &(*ho_Regions));
//
//			if (0 != (hv_i == 0))
//			{
//				hv_RowL2 = hv_RowE + ((hv_DetectHeight / 2)*((-hv_ATan).TupleSin()));
//				hv_RowL1 = hv_RowE - ((hv_DetectHeight / 2)*((-hv_ATan).TupleSin()));
//				hv_ColL2 = hv_ColE + ((hv_DetectHeight / 2)*((-hv_ATan).TupleCos()));
//				hv_ColL1 = hv_ColE - ((hv_DetectHeight / 2)*((-hv_ATan).TupleCos()));
//				gen_arrow_contour_xld(&ho_Arrow1, hv_RowL1, hv_ColL1, hv_RowL2, hv_ColL2, 25,
//					25);
//				*ho_Regions = ho_Regions->ConcatObj(ho_Arrow1);
//				//ConcatObj((*ho_Regions), ho_Arrow1, &(*ho_Regions));
//			}
//		}
//	}
//	return;
//}

void ST_MYDISP::spoke(
	HalconCpp::HImage ho_Image,
	HalconCpp::HXLDCont * ho_Regions,
	//HalconCpp::HTuple hv_WindowHandle,
	HalconCpp::HTuple hv_Elements,
	HalconCpp::HTuple hv_DetectHeight,
	HalconCpp::HTuple hv_DetectWidth,
	HalconCpp::HTuple hv_Sigma,
	HalconCpp::HTuple hv_Threshold,
	HalconCpp::HTuple hv_Transition,
	HalconCpp::HTuple hv_Select,
	HalconCpp::HTuple hv_ROIRows,
	HalconCpp::HTuple hv_ROICols,
	HalconCpp::HTuple hv_Direct,
	HalconCpp::HTuple * hv_ResultRow,
	HalconCpp::HTuple * hv_ResultColumn,
	HalconCpp::HTuple * hv_ArcType)

{

	// Local iconic variables
	HXLDCont  ho_Contour, ho_ContCircle, ho_Rectangle1;
	HXLDCont  ho_Arrow1;

	// Local control variables
	HTuple  hv_Width, hv_Height, hv_RowC, hv_ColumnC;
	HTuple  hv_Radius, hv_StartPhi, hv_EndPhi, hv_PointOrder;
	HTuple  hv_RowXLD, hv_ColXLD, hv_Length2, hv_i, hv_j, hv_RowE;
	HTuple  hv_ColE, hv_ATan, hv_RowL2, hv_RowL1, hv_ColL2;
	HTuple  hv_ColL1, hv_RowEdge, hv_ColEdge;
	HTuple  hv_Amplitude, hv_Distance, hv_tRow, hv_tCol, hv_t;
	HTuple  hv_Number, hv_k;

	HMeasure hv_Measure;

	ho_Image.GetImageSize(&hv_Width, &hv_Height);
	//GetImageSize(ho_Image, &hv_Width, &hv_Height);
	ho_Regions->GenEmptyObj();
	//GenEmptyObj(ho_Regions);

	(*hv_ResultRow) = HTuple();
	(*hv_ResultColumn) = HTuple();

	ho_Contour.GenContourPolygonXld(hv_ROIRows, hv_ROICols);
	//GenContourPolygonXld(&ho_Contour, hv_ROIRows, hv_ROICols);
	ho_Contour.FitCircleContourXld("algebraic", -1, 0, 0, 3, 2, &hv_RowC, &hv_ColumnC,
		&hv_Radius, &hv_StartPhi, &hv_EndPhi, &hv_PointOrder);
	//FitCircleContourXld(ho_Contour, "algebraic", -1, 0, 0, 3, 2, &hv_RowC, &hv_ColumnC,
	//	&hv_Radius, &hv_StartPhi, &hv_EndPhi, &hv_PointOrder);
	ho_ContCircle.GenCircleContourXld(hv_RowC, hv_ColumnC, hv_Radius, hv_StartPhi,
		hv_EndPhi, hv_PointOrder, 3);
	//GenCircleContourXld(&ho_ContCircle, hv_RowC, hv_ColumnC, hv_Radius, hv_StartPhi,
	//	hv_EndPhi, hv_PointOrder, 3);
	*ho_Regions = ho_Regions->ConcatObj(ho_ContCircle);
	//ConcatObj((*ho_Regions), ho_ContCircle, &(*ho_Regions));
	// ��ȡԲ��Բ�� xld �ϵĵ�����
	ho_ContCircle.GetContourXld(&hv_RowXLD, &hv_ColXLD);
	//GetContourXld(ho_ContCircle, &hv_RowXLD, &hv_ColXLD);
	// ��Բ��Բ���ϵ������
	TupleLength(hv_ColXLD, &hv_Length2);

	if (0 != (hv_Elements<3))
	{
		this->write_message(62, 12, "����Ե����̫��,����������", true);
		return;
	}

	{
		HTuple end_val20 = hv_Elements - 1;
		HTuple step_val20 = 1;
		for (hv_i = 0; hv_i.Continue(end_val20, step_val20); hv_i += step_val20)
		{
			if (0 != (HTuple(hv_RowXLD[0]) == HTuple(hv_RowXLD[hv_Length2 - 1])))
			{
				// xld �������յ�������ͬΪԲ
				TupleInt(((1.0*hv_Length2) / hv_Elements)*hv_i, &hv_j);
				(*hv_ArcType) = "circle";
			}
			else
			{
				// ����ΪԲ��
				TupleInt(((1.0*hv_Length2) / (hv_Elements - 1))*hv_i, &hv_j);
				(*hv_ArcType) = "arc";
			}
			// ����Խ��, ǿ�Ƹ�ֵΪ���һ������
			if (0 != (hv_j >= hv_Length2))
			{
				hv_j = hv_Length2 - 1;
				//continue;
			}
			// ��ȡ���߹�������
			hv_RowE = ((const HTuple&)hv_RowXLD)[hv_j];
			hv_ColE = ((const HTuple&)hv_ColXLD)[hv_j];

			// ����ͼ������,�����,�������ױ��쳣
			if (0 != (HTuple(HTuple(HTuple(hv_RowE>(hv_Height - 1)).TupleOr(hv_RowE<0)).TupleOr(hv_ColE>(hv_Width - 1))).TupleOr(hv_ColE<0)))
			{
				continue;
			}
			// ��Ե������������: inner ����������Բ��ָ��Բ��, outer ����������Բ��ָ��Բ��
			if (0 != (hv_Direct == HTuple("inner")))
			{
				// �󿨳߹��ߵı�Ե��������
				// ��Բ��ָ���Ե��ʸ���ĽǶ�
				TupleAtan2((-hv_RowE) + hv_RowC, hv_ColE - hv_ColumnC, &hv_ATan);
				hv_ATan = (HTuple(180).TupleRad()) + hv_ATan;
			}
			else
			{
				TupleAtan2((-hv_RowE) + hv_RowC, hv_ColE - hv_ColumnC, &hv_ATan);
			}

			//this->write_message(20, 20, hv_RowE);
			//this->write_message(80, 20, hv_ColE);
			//this->write_message(140, 20, hv_ATan);
			// �������� xld, �����ֵ���ʾ����
			ho_Rectangle1.GenRectangle2ContourXld(hv_RowE, hv_ColE, hv_ATan, hv_DetectHeight / 2,
				hv_DetectWidth / 2);
			//GenRectangle2ContourXld(&ho_Rectangle1, hv_RowE, hv_ColE, hv_ATan, hv_DetectHeight / 2,
			//	hv_DetectWidth / 2);
			*ho_Regions = ho_Regions->ConcatObj(ho_Rectangle1);
			//ConcatObj((*ho_Regions), ho_Rectangle1, &(*ho_Regions));

			// �ü�ͷ xld ָʾ��Ե��������, �����ֵ���ʾ����
			if (0 != (hv_i == 0))
			{
				hv_RowL2 = hv_RowE + ((hv_DetectHeight / 2)*((-hv_ATan).TupleSin()));
				hv_RowL1 = hv_RowE - ((hv_DetectHeight / 2)*((-hv_ATan).TupleSin()));
				hv_ColL2 = hv_ColE + ((hv_DetectHeight / 2)*((-hv_ATan).TupleCos()));
				hv_ColL1 = hv_ColE - ((hv_DetectHeight / 2)*((-hv_ATan).TupleCos()));

				gen_arrow_contour_xld(&ho_Arrow1, hv_RowL1, hv_ColL1, hv_RowL2, hv_ColL2, 25,
					25);
				*ho_Regions = ho_Regions->ConcatObj(ho_Arrow1);
				//ConcatObj((*ho_Regions), ho_Arrow1, &(*ho_Regions));
			}

			//��������������
			hv_Measure.GenMeasureRectangle2(hv_RowE, hv_ColE, hv_ATan, hv_DetectHeight / 2, hv_DetectWidth / 2,
				hv_Width, hv_Height, (HString)"nearest_neighbor");
			//GenMeasureRectangle2(hv_RowE, hv_ColE, hv_ATan, hv_DetectHeight / 2, hv_DetectWidth / 2,
			//	hv_Width, hv_Height, "nearest_neighbor", &hv_MrsHandle_Measure);

			if (0 != (hv_Transition == HTuple("negative")))
			{
				hv_Transition = "negative";
			}
			else
			{
				if (0 != (hv_Transition == HTuple("positive")))
				{
					hv_Transition = "positive";
				}
				else
				{
					hv_Transition = "all";
				}
			}

			if (0 != (hv_Select == HTuple("first")))
			{
				hv_Select = "first";
			}
			else
			{
				if (0 != (hv_Select == HTuple("last")))
				{
					hv_Select = "last";
				}
				else
				{
					hv_Select = "all";
				}
			}
			//����Ե
			hv_Measure.MeasurePos(ho_Image, hv_Sigma, hv_Threshold, hv_Transition,
				hv_Select, &hv_RowEdge, &hv_ColEdge, &hv_Amplitude, &hv_Distance);
			//MeasurePos(ho_Image, hv_MrsHandle_Measure, hv_Sigma, hv_Threshold, hv_Transition,
			//	hv_Select, &hv_RowEdge, &hv_ColEdge, &hv_Amplitude, &hv_Distance);
			//CloseMeasure(hv_MrsHandle_Measure);
			hv_tRow = 0;
			hv_tCol = 0;
			hv_t = 0;
			TupleLength(hv_RowEdge, &hv_Number);
			if (0 != (hv_Number<1))
			{
				continue;
			}
			//�ж����Եʱ��ѡ����Ⱦ���ֵ���ı�Ե
			{
				HTuple end_val93 = hv_Number - 1;
				HTuple step_val93 = 1;
				for (hv_k = 0; hv_k.Continue(end_val93, step_val93); hv_k += step_val93)
				{
					if (0 != ((HTuple(hv_Amplitude[hv_k]).TupleAbs())>hv_t))
					{
						hv_tRow = ((const HTuple&)hv_RowEdge)[hv_k];
						hv_tCol = ((const HTuple&)hv_ColEdge)[hv_k];
						hv_t = HTuple(hv_Amplitude[hv_k]).TupleAbs();
					}
				}
			}
			//���ҵ��ı�Ե�������������
			if (0 != (hv_t>0))
			{
				(*hv_ResultRow) = (*hv_ResultRow).TupleConcat(hv_tRow);
				(*hv_ResultColumn) = (*hv_ResultColumn).TupleConcat(hv_tCol);
			}
		}
	}
	return;
}

void ST_MYDISP::pts_to_best_circle(
	HalconCpp::HXLDCont * ho_Circle,
	HalconCpp::HTuple hv_Rows,
	HalconCpp::HTuple hv_Cols,
	HalconCpp::HTuple hv_ActiveNum,
	HalconCpp::HTuple hv_ArcType,
	HalconCpp::HTuple * hv_RowCenter,
	HalconCpp::HTuple * hv_ColCenter,
	HalconCpp::HTuple * hv_Radius,
	HalconCpp::HTuple * hv_StartPhi,
	HalconCpp::HTuple * hv_EndPhi,
	HalconCpp::HTuple * hv_PointOrder)

{

	// Local iconic variables
	HXLDCont  ho_Contour;

	// Local control variables
	HTuple  hv_Length, hv_Length1;

	//��ʼ��
	(*hv_RowCenter) = 0;
	(*hv_ColCenter) = 0;
	(*hv_Radius) = 0;
	ho_Circle->GenEmptyObj();
	//GenEmptyObj(&(*ho_Circle));
	TupleLength(hv_Cols, &hv_Length);

	if (0 != (HTuple(hv_Length >= hv_ActiveNum).TupleAnd(hv_ActiveNum>2)))
	{
		if (0 != (hv_ArcType == HTuple("circle")))
		{
			ho_Contour.GenContourPolygonXld(hv_Rows.TupleConcat(HTuple(hv_Rows[0])),
				hv_Cols.TupleConcat(HTuple(hv_Cols[0])));
			//GenContourPolygonXld(&ho_Contour, hv_Rows.TupleConcat(HTuple(hv_Rows[0])),
			//	hv_Cols.TupleConcat(HTuple(hv_Cols[0])));
		}
		else
		{
			ho_Contour.GenContourPolygonXld(hv_Rows, hv_Cols);
			//GenContourPolygonXld(&ho_Contour, hv_Rows, hv_Cols);
		}
		ho_Contour.FitCircleContourXld("geotukey", -1, 0, 0, 3, 2, &(*hv_RowCenter),
			&(*hv_ColCenter), &(*hv_Radius), &(*hv_StartPhi), &(*hv_EndPhi), &(*hv_PointOrder));
		//FitCircleContourXld(ho_Contour, "geotukey", -1, 0, 0, 3, 2, &(*hv_RowCenter),
		//	&(*hv_ColCenter), &(*hv_Radius), &(*hv_StartPhi), &(*hv_EndPhi), &(*hv_PointOrder));
		TupleLength((*hv_StartPhi), &hv_Length1);
		if (0 != (hv_Length1<1))
		{
			return;
		}
		//������Ͻ��������Բxld
		if (0 != (hv_ArcType == HTuple("arc")))
		{
			ho_Circle->GenCircleContourXld((*hv_RowCenter), (*hv_ColCenter), (*hv_Radius),
				(*hv_StartPhi), (*hv_EndPhi), (*hv_PointOrder), 1);
			//GenCircleContourXld(&(*ho_Circle), (*hv_RowCenter), (*hv_ColCenter), (*hv_Radius),
			//	(*hv_StartPhi), (*hv_EndPhi), (*hv_PointOrder), 1);
		}
		else
		{
			(*hv_StartPhi) = 0;
			(*hv_EndPhi) = HTuple(360).TupleRad();
			ho_Circle->GenCircleContourXld((*hv_RowCenter), (*hv_ColCenter), (*hv_Radius),
				(*hv_StartPhi), (*hv_EndPhi), (*hv_PointOrder), 1);
			//GenCircleContourXld(&(*ho_Circle), (*hv_RowCenter), (*hv_ColCenter), (*hv_Radius),
			//	(*hv_StartPhi), (*hv_EndPhi), (*hv_PointOrder), 1);
		}
	}
	return;
}


// ����ͷ

void ST_MYDISP::gen_arrow_contour_xld(
	HalconCpp::HXLDCont * ho_Arrow,
	HalconCpp::HTuple hv_Row1,
	HalconCpp::HTuple hv_Column1,
	HalconCpp::HTuple hv_Row2,
	HalconCpp::HTuple hv_Column2,
	HalconCpp::HTuple hv_HeadLength,
	HalconCpp::HTuple hv_HeadWidth)

{

	// Local iconic variables
	HXLDCont  ho_TempArrow;

	// Local control variables
	HTuple  hv_Length, hv_ZeroLengthIndices, hv_DR;
	HTuple  hv_DC, hv_HalfHeadWidth, hv_RowP1, hv_ColP1, hv_RowP2;
	HTuple  hv_ColP2, hv_Index;

	//This procedure generates arrow shaped XLD contours,
	//pointing from (Row1, Column1) to (Row2, Column2).
	//If starting and end point are identical, a contour consisting
	//of a single point is returned.
	//
	//input parameteres:
	//Row1, Column1: Coordinates of the arrows' starting points
	//Row2, Column2: Coordinates of the arrows' end points
	//HeadLength, HeadWidth: Size of the arrow heads in pixels
	//
	//output parameter:
	//Arrow: The resulting XLD contour
	//
	//The input tuples Row1, Column1, Row2, and Column2 have to be of
	//the same length.
	//HeadLength and HeadWidth either have to be of the same length as
	//Row1, Column1, Row2, and Column2 or have to be a single element.
	//If one of the above restrictions is violated, an error will occur.
	//
	//
	//Init
	ho_Arrow->GenEmptyObj();
	//GenEmptyObj(&(*ho_Arrow));
	//
	//Calculate the arrow length
	DistancePp(hv_Row1, hv_Column1, hv_Row2, hv_Column2, &hv_Length);
	//
	//Mark arrows with identical start and end point
	//(set Length to -1 to avoid division-by-zero exception)
	hv_ZeroLengthIndices = hv_Length.TupleFind(0);
	if (0 != (hv_ZeroLengthIndices != -1))
	{
		hv_Length[hv_ZeroLengthIndices] = -1;
	}
	//
	//Calculate auxiliary variables.
	hv_DR = (1.0*(hv_Row2 - hv_Row1)) / hv_Length;
	hv_DC = (1.0*(hv_Column2 - hv_Column1)) / hv_Length;
	hv_HalfHeadWidth = hv_HeadWidth / 2.0;
	//
	//Calculate end points of the arrow head.
	hv_RowP1 = (hv_Row1 + ((hv_Length - hv_HeadLength)*hv_DR)) + (hv_HalfHeadWidth*hv_DC);
	hv_ColP1 = (hv_Column1 + ((hv_Length - hv_HeadLength)*hv_DC)) - (hv_HalfHeadWidth*hv_DR);
	hv_RowP2 = (hv_Row1 + ((hv_Length - hv_HeadLength)*hv_DR)) - (hv_HalfHeadWidth*hv_DC);
	hv_ColP2 = (hv_Column1 + ((hv_Length - hv_HeadLength)*hv_DC)) + (hv_HalfHeadWidth*hv_DR);
	//
	//Finally create output XLD contour for each input point pair
	{
		HTuple end_val45 = (hv_Length.TupleLength()) - 1;
		HTuple step_val45 = 1;
		for (hv_Index = 0; hv_Index.Continue(end_val45, step_val45); hv_Index += step_val45)
		{
			if (0 != (HTuple(hv_Length[hv_Index]) == -1))
			{
				//Create_ single points for arrows with identical start and end point
				ho_TempArrow.GenContourPolygonXld(HTuple(hv_Row1[hv_Index]), HTuple(hv_Column1[hv_Index]));
				//GenContourPolygonXld(&ho_TempArrow, HTuple(hv_Row1[hv_Index]), HTuple(hv_Column1[hv_Index]));
			}
			else
			{
				//Create arrow contour
				ho_TempArrow.GenContourPolygonXld(((((HTuple(hv_Row1[hv_Index]).TupleConcat(HTuple(hv_Row2[hv_Index]))).TupleConcat(HTuple(hv_RowP1[hv_Index]))).TupleConcat(HTuple(hv_Row2[hv_Index]))).TupleConcat(HTuple(hv_RowP2[hv_Index]))).TupleConcat(HTuple(hv_Row2[hv_Index])),
					((((HTuple(hv_Column1[hv_Index]).TupleConcat(HTuple(hv_Column2[hv_Index]))).TupleConcat(HTuple(hv_ColP1[hv_Index]))).TupleConcat(HTuple(hv_Column2[hv_Index]))).TupleConcat(HTuple(hv_ColP2[hv_Index]))).TupleConcat(HTuple(hv_Column2[hv_Index])));
				//GenContourPolygonXld(&ho_TempArrow, ((((HTuple(hv_Row1[hv_Index]).TupleConcat(HTuple(hv_Row2[hv_Index]))).TupleConcat(HTuple(hv_RowP1[hv_Index]))).TupleConcat(HTuple(hv_Row2[hv_Index]))).TupleConcat(HTuple(hv_RowP2[hv_Index]))).TupleConcat(HTuple(hv_Row2[hv_Index])),
				//	((((HTuple(hv_Column1[hv_Index]).TupleConcat(HTuple(hv_Column2[hv_Index]))).TupleConcat(HTuple(hv_ColP1[hv_Index]))).TupleConcat(HTuple(hv_Column2[hv_Index]))).TupleConcat(HTuple(hv_ColP2[hv_Index]))).TupleConcat(HTuple(hv_Column2[hv_Index])));
			}
			*ho_Arrow = ho_Arrow->ConcatObj(ho_TempArrow);
			//ConcatObj((*ho_Arrow), ho_TempArrow, &(*ho_Arrow));
		}
	}
	return;
}

void ST_MYDISP::gen_double_arrow_contour_xld(
	HalconCpp::HXLDCont * ho_Arrow,
	HalconCpp::HTuple hv_Row1,
	HalconCpp::HTuple hv_Column1, 
	HalconCpp::HTuple hv_Row2, 
	HalconCpp::HTuple hv_Column2,
	HalconCpp::HTuple hv_HeadLength, 
	HalconCpp::HTuple hv_HeadWidth)
{
	// Local iconic variables
	HXLDCont  ho_TempArrow, ho2_TempArrow;

	// Local control variables
	HTuple  hv_Length, hv_ZeroLengthIndices, hv_DR;
	HTuple  hv_DC, hv_HalfHeadWidth, hv_RowP1, hv_ColP1, hv_RowP2;
	HTuple  hv_ColP2, hv_Index;
	
	HTuple hv2_RowP1, hv2_ColP1, hv2_RowP2, hv2_ColP2;    // ��һ����ļ�ͷ
	HTuple hv2_DR, hv2_DC;
	//
	//Init
	ho_Arrow->GenEmptyObj();
	//GenEmptyObj(&(*ho_Arrow));
	//
	//Calculate the arrow length
	DistancePp(hv_Row1, hv_Column1, hv_Row2, hv_Column2, &hv_Length);
	//
	//Mark arrows with identical start and end point
	//(set Length to -1 to avoid division-by-zero exception)
	hv_ZeroLengthIndices = hv_Length.TupleFind(0);
	if (0 != (hv_ZeroLengthIndices != -1))
	{
		hv_Length[hv_ZeroLengthIndices] = -1;
	}
	//
	//Calculate auxiliary variables.
	hv_DR = (1.0*(hv_Row2 - hv_Row1)) / hv_Length;
	hv_DC = (1.0*(hv_Column2 - hv_Column1)) / hv_Length;

	hv2_DR = (1.0*(hv_Row1 - hv_Row2)) / hv_Length;
	hv2_DC = (1.0*(hv_Column1 - hv_Column2)) / hv_Length;

	hv_HalfHeadWidth = hv_HeadWidth / 2.0;
	//
	//Calculate end points of the arrow head.
	hv_RowP1 = (hv_Row1 + ((hv_Length - hv_HeadLength)*hv_DR)) + (hv_HalfHeadWidth*hv_DC);
	hv_ColP1 = (hv_Column1 + ((hv_Length - hv_HeadLength)*hv_DC)) - (hv_HalfHeadWidth*hv_DR);
	hv_RowP2 = (hv_Row1 + ((hv_Length - hv_HeadLength)*hv_DR)) - (hv_HalfHeadWidth*hv_DC);
	hv_ColP2 = (hv_Column1 + ((hv_Length - hv_HeadLength)*hv_DC)) + (hv_HalfHeadWidth*hv_DR);

	hv2_RowP1 = (hv_Row2 + ((hv_Length - hv_HeadLength)*hv2_DR)) + (hv_HalfHeadWidth*hv2_DC);
	hv2_ColP1 = (hv_Column2 + ((hv_Length - hv_HeadLength)*hv2_DC)) - (hv_HalfHeadWidth*hv2_DR);
	hv2_RowP2 = (hv_Row2 + ((hv_Length - hv_HeadLength)*hv2_DR)) - (hv_HalfHeadWidth*hv2_DC);
	hv2_ColP2 = (hv_Column2 + ((hv_Length - hv_HeadLength)*hv2_DC)) + (hv_HalfHeadWidth*hv2_DR);

	//
	//Finally create output XLD contour for each input point pair
	{
		HTuple end_val45 = (hv_Length.TupleLength()) - 1;
		HTuple step_val45 = 1;
		for (hv_Index = 0; hv_Index.Continue(end_val45, step_val45); hv_Index += step_val45)
		{
			if (0 != (HTuple(hv_Length[hv_Index]) == -1))
			{
				//Create_ single points for arrows with identical start and end point
				ho_TempArrow.GenContourPolygonXld(HTuple(hv_Row1[hv_Index]), HTuple(hv_Column1[hv_Index]));
			}
			else
			{
				//Create arrow contour
				ho_TempArrow.GenContourPolygonXld(((((HTuple(hv_Row1[hv_Index]).TupleConcat(HTuple(hv_Row2[hv_Index]))).TupleConcat(HTuple(hv_RowP1[hv_Index]))).TupleConcat(HTuple(hv_Row2[hv_Index]))).TupleConcat(HTuple(hv_RowP2[hv_Index]))).TupleConcat(HTuple(hv_Row2[hv_Index])),
					((((HTuple(hv_Column1[hv_Index]).TupleConcat(HTuple(hv_Column2[hv_Index]))).TupleConcat(HTuple(hv_ColP1[hv_Index]))).TupleConcat(HTuple(hv_Column2[hv_Index]))).TupleConcat(HTuple(hv_ColP2[hv_Index]))).TupleConcat(HTuple(hv_Column2[hv_Index])));

				ho2_TempArrow.GenContourPolygonXld(((((HTuple(hv_Row2[hv_Index]).TupleConcat(HTuple(hv_Row1[hv_Index]))).TupleConcat(HTuple(hv2_RowP1[hv_Index]))).TupleConcat(HTuple(hv_Row1[hv_Index]))).TupleConcat(HTuple(hv2_RowP2[hv_Index]))).TupleConcat(HTuple(hv_Row1[hv_Index])),
					((((HTuple(hv_Column2[hv_Index]).TupleConcat(HTuple(hv_Column1[hv_Index]))).TupleConcat(HTuple(hv2_ColP1[hv_Index]))).TupleConcat(HTuple(hv_Column1[hv_Index]))).TupleConcat(HTuple(hv2_ColP2[hv_Index]))).TupleConcat(HTuple(hv_Column1[hv_Index])));

			}
			*ho_Arrow = ho_Arrow->ConcatObj(ho_TempArrow);
			*ho_Arrow = ho_Arrow->ConcatObj(ho2_TempArrow);
			//ConcatObj((*ho_Arrow), ho_TempArrow, &(*ho_Arrow));
		}
	}
	return;
}
