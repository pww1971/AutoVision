#include "stdafx.h"
#include "checkPatch.h"
#include "MyData.h"
#include "MyDispLay.h"
using namespace HalconCpp;
using namespace std;
void gen_arrow_contour_xld(HObject* ho_Arrow, HTuple hv_Row1, HTuple hv_Column1,
	HTuple hv_Row2, HTuple hv_Column2, HTuple hv_HeadLength, HTuple hv_HeadWidth)
{

	// Local iconic variables
	HObject  ho_TempArrow;

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
	GenEmptyObj(&(*ho_Arrow));
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
	hv_DR = (1.0 * (hv_Row2 - hv_Row1)) / hv_Length;
	hv_DC = (1.0 * (hv_Column2 - hv_Column1)) / hv_Length;
	hv_HalfHeadWidth = hv_HeadWidth / 2.0;
	//
	//Calculate end points of the arrow head.
	hv_RowP1 = (hv_Row1 + ((hv_Length - hv_HeadLength) * hv_DR)) + (hv_HalfHeadWidth * hv_DC);
	hv_ColP1 = (hv_Column1 + ((hv_Length - hv_HeadLength) * hv_DC)) - (hv_HalfHeadWidth * hv_DR);
	hv_RowP2 = (hv_Row1 + ((hv_Length - hv_HeadLength) * hv_DR)) - (hv_HalfHeadWidth * hv_DC);
	hv_ColP2 = (hv_Column1 + ((hv_Length - hv_HeadLength) * hv_DC)) + (hv_HalfHeadWidth * hv_DR);
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
				GenContourPolygonXld(&ho_TempArrow, HTuple(hv_Row1[hv_Index]), HTuple(hv_Column1[hv_Index]));
			}
			else
			{
				//Create arrow contour
				GenContourPolygonXld(&ho_TempArrow, ((((HTuple(hv_Row1[hv_Index]).TupleConcat(HTuple(hv_Row2[hv_Index]))).TupleConcat(HTuple(hv_RowP1[hv_Index]))).TupleConcat(HTuple(hv_Row2[hv_Index]))).TupleConcat(HTuple(hv_RowP2[hv_Index]))).TupleConcat(HTuple(hv_Row2[hv_Index])),
					((((HTuple(hv_Column1[hv_Index]).TupleConcat(HTuple(hv_Column2[hv_Index]))).TupleConcat(HTuple(hv_ColP1[hv_Index]))).TupleConcat(HTuple(hv_Column2[hv_Index]))).TupleConcat(HTuple(hv_ColP2[hv_Index]))).TupleConcat(HTuple(hv_Column2[hv_Index])));
			}
			ConcatObj((*ho_Arrow), ho_TempArrow, &(*ho_Arrow));
		}
	}
	return;
}

void disp_3d_coord_system(HalconCpp::HWindow* hv_WindowHandle, HTuple hv_CamParam, HTuple hv_Pose,
	HTuple hv_CoordAxesLength)
{

	// Local iconic variables
	HObject  ho_Arrows;

	// Local control variables
	HTuple  hv_TransWorld2Cam, hv_OrigCamX, hv_OrigCamY;
	HTuple  hv_OrigCamZ, hv_Row0, hv_Column0, hv_X, hv_Y, hv_Z;
	HTuple  hv_RowAxX, hv_ColumnAxX, hv_RowAxY, hv_ColumnAxY;
	HTuple  hv_RowAxZ, hv_ColumnAxZ, hv_Distance, hv_HeadLength;
	HTuple  hv_Red, hv_Green, hv_Blue;

	//This procedure displays a 3D coordinate system.
	//It needs the procedure gen_arrow_contour_xld.
	//
	//Input parameters:
	//WindowHandle: The window where the coordinate system shall be displayed
	//CamParam: The camera paramters
	//Pose: The pose to be displayed
	//CoordAxesLength: The length of the coordinate axes in world coordinates
	//
	//Check, if Pose is a correct pose tuple.
	if (0 != ((hv_Pose.TupleLength()) != 7))
	{
		return;
	}
	if (0 != (HTuple(HTuple(hv_Pose[2]) == 0.0).TupleAnd(HTuple(hv_CamParam[0]) != 0)))
	{
		//For projective cameras:
		//Poses with Z position zero cannot be projected
		//(that would lead to a division by zero error).
		return;
	}
	//Convert to pose to a transformation matrix
	PoseToHomMat3d(hv_Pose, &hv_TransWorld2Cam);
	//Project the world origin into the image
	AffineTransPoint3d(hv_TransWorld2Cam, 0, 0, 0, &hv_OrigCamX, &hv_OrigCamY, &hv_OrigCamZ);
	Project3dPoint(hv_OrigCamX, hv_OrigCamY, hv_OrigCamZ, hv_CamParam, &hv_Row0, &hv_Column0);
	//Project the coordinate axes into the image
	AffineTransPoint3d(hv_TransWorld2Cam, hv_CoordAxesLength, 0, 0, &hv_X, &hv_Y, &hv_Z);
	Project3dPoint(hv_X, hv_Y, hv_Z, hv_CamParam, &hv_RowAxX, &hv_ColumnAxX);
	AffineTransPoint3d(hv_TransWorld2Cam, 0, hv_CoordAxesLength, 0, &hv_X, &hv_Y, &hv_Z);
	Project3dPoint(hv_X, hv_Y, hv_Z, hv_CamParam, &hv_RowAxY, &hv_ColumnAxY);
	AffineTransPoint3d(hv_TransWorld2Cam, 0, 0, hv_CoordAxesLength, &hv_X, &hv_Y, &hv_Z);
	Project3dPoint(hv_X, hv_Y, hv_Z, hv_CamParam, &hv_RowAxZ, &hv_ColumnAxZ);
	//
	//Generate an XLD contour for each axis
	DistancePp((hv_Row0.TupleConcat(hv_Row0)).TupleConcat(hv_Row0), (hv_Column0.TupleConcat(hv_Column0)).TupleConcat(hv_Column0),
		(hv_RowAxX.TupleConcat(hv_RowAxY)).TupleConcat(hv_RowAxZ), (hv_ColumnAxX.TupleConcat(hv_ColumnAxY)).TupleConcat(hv_ColumnAxZ),
		&hv_Distance);
	hv_HeadLength = ((((hv_Distance.TupleMax()) / 12.0).TupleConcat(5.0)).TupleMax()).TupleInt();
	gen_arrow_contour_xld(&ho_Arrows, (hv_Row0.TupleConcat(hv_Row0)).TupleConcat(hv_Row0),
		(hv_Column0.TupleConcat(hv_Column0)).TupleConcat(hv_Column0), (hv_RowAxX.TupleConcat(hv_RowAxY)).TupleConcat(hv_RowAxZ),
		(hv_ColumnAxX.TupleConcat(hv_ColumnAxY)).TupleConcat(hv_ColumnAxZ), hv_HeadLength,
		hv_HeadLength);
	//
	//Display coordinate system
	hv_WindowHandle->DispXld(ho_Arrows);
	//
	/*hv_WindowHandle->GetRgb( &hv_Red, &hv_Green, &hv_Blue);
	hv_WindowHandle->SetRgb( HTuple(hv_Red[0]), HTuple(hv_Green[0]), HTuple(hv_Blue[0]));
	hv_WindowHandle->SetTposition( hv_RowAxX + 3, hv_ColumnAxX + 3);
	hv_WindowHandle->WriteString( "X");
	hv_WindowHandle->SetRgb( HTuple(hv_Red[1 % (hv_Red.TupleLength())]), HTuple(hv_Green[1 % (hv_Green.TupleLength())]),
		HTuple(hv_Blue[1 % (hv_Blue.TupleLength())]));
	hv_WindowHandle->SetTposition( hv_RowAxY + 3, hv_ColumnAxY + 3);
	hv_WindowHandle->WriteString( "Y");
	hv_WindowHandle->SetRgb( HTuple(hv_Red[2 % (hv_Red.TupleLength())]), HTuple(hv_Green[2 % (hv_Green.TupleLength())]),
		HTuple(hv_Blue[2 % (hv_Blue.TupleLength())]));
	hv_WindowHandle->SetTposition( hv_RowAxZ + 3, hv_ColumnAxZ + 3);
	hv_WindowHandle->WriteString( "Z");
	hv_WindowHandle->SetRgb( hv_Red, hv_Green, hv_Blue);*/
	return;
}

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

		CString szCreatNumLevels = (*o)["����������"];
		HTuple CreatNumLevels;
		if (szCreatNumLevels == "auto")
		{
			CreatNumLevels = "auto";
		}
		else
			CreatNumLevels = atoi(szCreatNumLevels.GetBuffer(0));
		double CreatAngleStart = (*o)["��ʼ�ǻ���"];
		double CreatAngleExtend = (*o)["��ת��Χ"];
		CString CreateAngleStep = (*o)["����"];
		CString CreateOptimization = (*o)["CreateModleOptimization"];
		CString CreateMetric = (*o)["CreateModleMetric"];
		CString CreateContrast = (*o)["��С�߶�����"];
		CString CreateMinContrast = (*o)["CreateModleMinContrast"];
		int modeType = (*o)["ƥ��ģ��"];
		double ScaleRMin = (*o)["���������С�߶�����"];
		double ScaleRMax = (*o)["����������߶�����"];
		CString szScaleRStep = (*o)["������β�������"];
		HTuple ScaleRStep;
		if (szScaleRStep == "auto")
		{
			ScaleRStep = "auto";
		}
		else
			ScaleRStep = atof(szScaleRStep.GetBuffer(0));
		double ScaleCMin = (*o)["���������С�߶�����"];
		double ScaleCMax = (*o)["����������߶�����"];
		CString szScaleCStep = (*o)["������β�������"];
		HTuple ScaleCStep;
		if (szScaleCStep == "auto")
		{
			ScaleCStep = "auto";
		}
		else
			ScaleCStep = atof(szScaleCStep.GetBuffer(0));

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
			hv_Model->hv_Model.CreateAnisoShapeModel(
				ImageReduced,
				CreatNumLevels,
				CreatAngleStart,
				CreatAngleExtend,
				AngleStep,
				ScaleRMin, ScaleRMax, ScaleRStep,
				ScaleCMin, ScaleCMax, ScaleCStep,
				CT2A(CreateOptimization).m_psz,
				CT2A(CreateMetric).m_psz,
				hCreateContrast,
				CT2A(CreateMinContrast).m_psz);
		}
		else
		{
			hv_Model->hv_nccModel.CreateNccModel(
				ImageReduced,
				CreatNumLevels,
				CreatAngleStart,
				CreatAngleExtend,
				CT2A(CreateAngleStep).m_psz,
				CT2A(CreateMetric).m_psz);
		}
		double FindAngleStart = (*o)["��ʼ�ǻ���"];
		double FindAngleExtend = (*o)["��ת��Χ"];
		double FindMinScore = (*o)["���ƶ�"];
		int FindNumMatches = (*o)["Ŀ����"];
		double FindOverlap = (*o)["FindModleMaxOverlap"];
		CString FindSubPixel = (*o)["FindModleSubPixel"];
		CString szNumLevels = (*o)["����������"];
		if ("auto" == szNumLevels)
		{
			szNumLevels = "4";
		}
		int FindNumLevels = atoi(szNumLevels.GetBuffer(0));
		double FindGreediness = (*o)["FindModleGreediness"];

		HTuple Row3, Column3, Angle1, Score1;
		HTuple RScale,CScale;
		if (0 == modeType)
		{
			hv_Model->hv_Model.FindAnisoShapeModel(
				Img,
				FindAngleStart,
				FindAngleExtend,
				ScaleRMin, ScaleRMax,
				ScaleCMin, ScaleCMax,
				FindMinScore,
				FindNumMatches,
				FindOverlap,
				CT2A(FindSubPixel).m_psz,
				FindNumLevels,
				FindGreediness,
				&Row3,
				&Column3,
				&Angle1,
				&RScale,
				&CScale,
				&Score1
			);
		}
		else
		{
			hv_Model->hv_nccModel.FindNccModel(
				Img,
				FindAngleStart,
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
		m_sIPResult.result_cont = hv_Model->ho_ModelRegion;   // region ��ת���� xld
//		LeaveCriticalSection(&m_csIPDisp);

		this->OnMainDisp();              // ��������ʾһ��

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
void ST_MYDISP::CreatDxfModel(UCI::OptionsMap* o, CString  dxffile)
{
	try {
		HObject  ho_DXFModelContours, ho_Contours, ho_ObjectSelected;
		HTuple hv_DXFStatus;
		HTuple  hv_I, hv_Y, hv_X, hv_Z, hv_Xc, hv_Yc, hv_Zc, hv_R, hv_C;
		ReadContourXldDxf(&ho_DXFModelContours, dxffile.GetBuffer(0),
			HTuple(), HTuple(), &hv_DXFStatus);

		HTuple  hv_Row1, hv_Column1, hv_Row2, hv_Column2;
		{
			SmallestRectangle1Xld(ho_DXFModelContours, &hv_Row1, &hv_Column1, &hv_Row2, &hv_Column2);
			HTuple  hv_HomMat2DIdentity, hv_HomMat2DScale;
			HomMat2dIdentity(&hv_HomMat2DIdentity);
			double dscale = (*o)["cad���ű���"];
			HomMat2dScale(hv_HomMat2DIdentity, dscale, dscale, 
				(hv_Row2.TupleMax()-hv_Row1.TupleMin())/2, (hv_Column2.TupleMax()-hv_Column1.TupleMin())/2,
				&hv_HomMat2DScale);
			AffineTransContourXld(ho_DXFModelContours, &ho_DXFModelContours, hv_HomMat2DScale);
			//	this->m_pMHWinMain->DispObj(ho_DXFModelContours);
			//	return;
		}

		CString szCreatNumLevels = (*o)["����������"];
		HTuple CreatNumLevels;
		if (szCreatNumLevels == "auto")
		{
			CreatNumLevels = "auto";
		}
		else
			CreatNumLevels = atoi(szCreatNumLevels.GetBuffer(0));
		double CreatAngleStart = (*o)["��ʼ�ǻ���"];
		double CreatAngleExtend = (*o)["��ת��Χ"];
		CString CreateAngleStep = (*o)["����"];
		CString CreateOptimization = (*o)["CreateModleOptimization"];
		CString CreateMetric = (*o)["CreateModleMetric"];
		CString CreateContrast = (*o)["��С�߶�����"];
		CString CreateMinContrast = (*o)["CreateModleMinContrast"];
		int modeType = (*o)["ƥ��ģ��"];
		double ScaleRMin = (*o)["���������С�߶�����"];
		double ScaleRMax = (*o)["����������߶�����"];
		CString szScaleRStep = (*o)["������β�������"];
		HTuple ScaleRStep;
		if (szScaleRStep == "auto")
		{
			ScaleRStep = "auto";
		}
		else
			ScaleRStep = atof(szScaleRStep.GetBuffer(0));
		double ScaleCMin = (*o)["���������С�߶�����"];
		double ScaleCMax = (*o)["����������߶�����"];
		CString szScaleCStep = (*o)["������β�������"];
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

	//	this->m_pMHWinMain->DispObj(hv_Model->ho_ModelContours);
	//	return;

		double minScroe = (*o)["���ƶ�"];
		int NumMatches = (*o)["Ŀ����"];
		int NumMatchesLimit = (*o)["Ŀ��������"];
		double MaxOverlap = (*o)["FindModleMaxOverlap"];
		CString SubPixel = (*o)["FindModleSubPixel"];
		double Greediness = (*o)["FindModleGreediness"];
		CString szNumLevels = (*o)["����������"];
		if ("auto" == szNumLevels)
		{
			szNumLevels = "4";
		}
		int NumLevels = atoi(szNumLevels.GetBuffer(0));
		HTuple hv_HomMat2D, CovPose, Score;

		FindPlanarUncalibDeformableModel(
			m_sIPResult.result_img,
			hv_Model->m_dxfModel,
			CreatAngleStart,
			CreatAngleExtend,
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
		if (Score.Length() > 0)
		{
			HTuple end_val81 = (Score.TupleLength()) - 1;
			HTuple step_val81 = 1;
			HTuple hv_J, hv_HomMatSelected;
			for (hv_J = 0; hv_J.Continue(end_val81, step_val81); hv_J += step_val81)
			{
				//Select the respective match.
				TupleSelectRange(hv_HomMat2D, hv_J * 9, ((hv_J + 1) * 9) - 1, &hv_HomMatSelected);
				ProjectiveTransContourXld(hv_Model->ho_ModelContours, &ho_Contours, hv_HomMatSelected);
				
			}
			
			//��ͼ������Ӽ���
			SetPlanarUncalibDeformableModelMetric(m_sIPResult.result_img, hv_Model->m_dxfModel, hv_HomMatSelected, "use_polarity");
			//����ԭ��
		//	SetDeformableModelOrigin(hv_Model->m_dxfModel, 0.010, 0.0425);
			GetDeformableModelContours(&hv_Model->ho_ModelContours, hv_Model->m_dxfModel, 1);

			this->m_pMHWinMain->DispObj(ho_Contours);
			disp_3d_coord_system(this->m_pMHWinMain, hv_Model->hv_CamParam, hv_HomMatSelected, 0.05);
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
		AfxMessageBox("��ǰ���ڻ�ROI�С�����");
		return;
	}
	
	try {
		double Row, Column, Phi, Length1, Length2;
		
		HalconCpp::HRegion Hreg;                     // ����
		HalconCpp::HImage hImg;

//		EnterCriticalSection(&m_csIPDisp);  
		hImg = m_sIPResult.result_img;
//		LeaveCriticalSection(&m_csIPDisp);    

		// ��ʾ��Ϣ
		this->write_message(20, 20, "��ģ������,�������Ҽ�ȷ��");

		UCI::RIBmenu.bIsInROIdrawing = true;
		// ������
		this->m_pMHWinMain->DrawRectangle2(&Row, &Column, &Phi, &Length1, &Length2);

		UCI::RIBmenu.bIsInROIdrawing = false;

		Hreg.GenRectangle2(Row, Column, Phi, Length1, Length2);

		// ��������
		DisProcessROIRegion(hv_Model->ho_ModelRegion, Hreg);	

		DisProcessROImodel(hImg, hv_Model->ho_ModelRegion);   // ����ģ��

	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}

void ST_MYDISP::DisOnRoiDrawCircle()
{
	if (UCI::RIBmenu.bIsInROIdrawing) {
		AfxMessageBox("��ǰ���ڻ�ROI�С�����");
		return;
	}
	
	try {
		double Row, Column, Radius;

		HalconCpp::HRegion Hreg;                     // ����
		HalconCpp::HImage hImg;

//		EnterCriticalSection(&m_csIPDisp);	
		hImg = m_sIPResult.result_img;
//		LeaveCriticalSection(&m_csIPDisp);

		// ��ʾ��Ϣ
		this->write_message(20, 20, "��ģ������,�������Ҽ�ȷ��");

		UCI::RIBmenu.bIsInROIdrawing = true;
		// ������
		this->m_pMHWinMain->DrawCircle(&Row, &Column, &Radius);	

		UCI::RIBmenu.bIsInROIdrawing = false;


		// ��������
		Hreg.GenCircle(Row, Column, Radius);

		DisProcessROIRegion(hv_Model->ho_ModelRegion, Hreg);

		DisProcessROImodel(hImg, hv_Model->ho_ModelRegion);   // ����ģ��
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}

void ST_MYDISP::DisOnRoiDrawEllipse()
{
	if (UCI::RIBmenu.bIsInROIdrawing) {
		AfxMessageBox("��ǰ���ڻ�ROI�С�����");
		return;
	}
	
	try {
		double Row, Column, Phi,Radius1, Radius2;

		HalconCpp::HRegion Hreg;                     // ����
		HalconCpp::HImage hImg;

//		EnterCriticalSection(&m_csIPDisp);
		hImg = m_sIPResult.result_img;
//		LeaveCriticalSection(&m_csIPDisp);

		// ��ʾ��Ϣ
		this->write_message(20, 20, "��ģ������,�������Ҽ�ȷ��");

		UCI::RIBmenu.bIsInROIdrawing = true;
		// ������
		this->m_pMHWinMain->DrawEllipse(&Row, &Column, &Phi, &Radius1, &Radius2);

		UCI::RIBmenu.bIsInROIdrawing = false;

		// ��������
		Hreg.GenEllipse(Row, Column, Phi, Radius1, Radius2);

		// ��������
		DisProcessROIRegion(hv_Model->ho_ModelRegion, Hreg);


		DisProcessROImodel(hImg, hv_Model->ho_ModelRegion);   // ����ģ��
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}

void ST_MYDISP::DisOnRoiDrawRectangle()
{
	if (UCI::RIBmenu.bIsInROIdrawing) {
		AfxMessageBox("��ǰ���ڻ�ROI�С�����");
		return;
	}
	
	try {
		double Row1, Column1, Row2, Column2;

		HalconCpp::HRegion Hreg;                     // ����
		HalconCpp::HImage hImg;

//		EnterCriticalSection(&m_csIPDisp);
		hImg = m_sIPResult.result_img;
//		LeaveCriticalSection(&m_csIPDisp);

		// ��ʾ��Ϣ
		this->write_message(20, 20, "��ģ������,�������Ҽ�ȷ��");

		UCI::RIBmenu.bIsInROIdrawing = true;
		// ������
		this->m_pMHWinMain->DrawRectangle1(&Row1, &Column1, &Row2, &Column2);
		UCI::RIBmenu.bIsInROIdrawing = false;

		// ��������
		Hreg.GenRectangle1(Row1, Column1, Row2, Column2);

		// ��ԭ���������������
		DisProcessROIRegion(hv_Model->ho_ModelRegion, Hreg);

		//this->m_pMHWinMain->ClearWindow();	   
		//this->m_pMHWinMain->DispObj(hImg);
		//this->m_pMHWinMain->DispObj(this->ho_ModelRegion);

		DisProcessROImodel(hImg, hv_Model->ho_ModelRegion);   // ����ģ��

	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}

void ST_MYDISP::DisOnRoiDrawAnyLine()
{
	if (UCI::RIBmenu.bIsInROIdrawing) {
		AfxMessageBox("��ǰ���ڻ�ROI�С�����");
		return;
	}
	
	try {
		
		HalconCpp::HImage hImg;                      // ��ʾ��ͼ��
		HalconCpp::HRegion Hreg;                     // ����

//		EnterCriticalSection(&m_csIPDisp);
		hImg = m_sIPResult.result_img;
//		LeaveCriticalSection(&m_csIPDisp);

		// ��ʾ��Ϣ
		this->write_message(20, 20, "��ģ������,�������Ҽ�ȷ��");

		UCI::RIBmenu.bIsInROIdrawing = true;
		Hreg = this->m_pMHWinMain->DrawRegion();
		UCI::RIBmenu.bIsInROIdrawing = false;

		DisProcessROIRegion(hv_Model->ho_ModelRegion,Hreg);	

		DisProcessROImodel(hImg, hv_Model->ho_ModelRegion);   // ����ģ��

	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}


// ��ʾ��ǰ��ģ����Ϣ\nģ����Ϣ
void ST_MYDISP::DisOnRoiInfo(int id)
{
	try {
		CString str;
		str.Format("ƥ�� %d",id);
		UCI::OptionsMap *om = UCI::ELE.GetOptionsMapByEleName(*UCI::ELE.EleList,str);
		if (nullptr == om)
		{
			AfxMessageBox("û��ƥ�����,��ѡ��ƥ�䷽��");
			return ;
		}
		// �򿪵�ǰ�����ͼƬ

		this->OnDisButMatchLoadPic();
		CString szdir = ((checkPatch*)checkPatch::S_dlg.m_pCheck)->m_cfgDir.c_str();
		CString stRegion;
		stRegion.Format("%d",id);
		stRegion = szdir+"\\region"+stRegion+".hobj";
		CString stModel;
		stModel.Format("%d",id);
		stModel = szdir+"\\model"+stModel;

		if (GetFileAttributes(stRegion) == -1) { // �ļ�������
			AfxMessageBox(stRegion);
			AfxMessageBox("DisOnRoiInfo() ROI�������ļ������ڣ� ");
			return;
		}
		hv_Model->ho_ModelRegion.ReadRegion(stRegion.GetBuffer(0));
		//
		/*HXLDCont HXdis;
		HXdis.ReadObject(CT2A(stRegion).m_psz);
		hv_Model->ho_ModelRegion = HXdis;*/
		if (GetFileAttributes(stModel) == -1) { // �ļ�������
			AfxMessageBox(stModel);
			AfxMessageBox("DisOnRoiInfo()ROI��ģ���ļ������ڣ� ");
			return;
		}
		int modeType = (*om)["ƥ��ģ��"];
		if (0==modeType)
			hv_Model->hv_Model.ReadShapeModel(CT2A(stModel).m_psz);
		else
			hv_Model->hv_nccModel.ReadNccModel(CT2A(stModel).m_psz);
//		EnterCriticalSection(&m_csIPDisp);
		m_sIPResult.result_cont = hv_Model->ho_ModelRegion;
//		LeaveCriticalSection(&m_csIPDisp);

		this->OnMainDisp();

		double AngleStart = (*om)["��ʼ�ǻ���"];
		double AngleExtend = (*om)["��ת��Χ"];

		HTuple Row3, Column3, Angle1, Score1;
		HalconCpp::HImage hImg;
		hImg = UCI::pDis->GrabImageDisp;
		double ScaleMin = (*om)["������С�߶�����"];
		double ScaleMax = (*om)["�������߶�����"];
		HTuple Scale;
		if (0 == modeType)
		{
			hv_Model->hv_Model.FindScaledShapeModel(
				hImg,
				AngleStart,
				AngleExtend,
				ScaleMin, ScaleMax,
				0.5,
				1,
				0.5,
				"least_squares",
				0,
				0.9,
				&Row3,
				&Column3,
				&Angle1,
				&Scale,
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
			hv_Model->hv_nccModel.FindNccModel(
				hImg,
				AngleStart,
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
			AfxMessageBox("û��ƥ�����,��ѡ��ƥ�䷽��");
			return ;
		}
		this->m_pMHWinMain->ClearWindow();

		HalconCpp::HImage hImg=this->GrabImageDisp;
		HalconCpp::HXLDCont Cont;
		Cont.GenEmptyObj();
		if (this->MeasureOneMATCH(om, hImg, Cont) == FALSE) {
			write_message(10, 10, "û���ҵ���Ӧ��ģ��");
			return;
		}


	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}

// ����ROI 
void ST_MYDISP::DisOnButMatchSaveModel()
{

	try {
		//int prID = UCI::OPTIONS["��Ŀ���"];
		//// ���ж�һ���ļ��Ƿ����
		//CString str = this->GetMatchPathFile(prID);
		//if (GetFileAttributes(str) != -1) { // �ļ�����
		//	int res = ::MessageBox(NULL,
		//		"��� ȷ��  ����ʵ�ֲ���һ����\n \
		//		��� ȡ��  ���ء�",
		//		"ƥ���ļ��Ѵ��ڣ�",
		//		MB_OKCANCEL | MB_ICONSTOP);
		//	if (res == IDCANCEL) return;
		//}

		//// 1. ���浱ǰ��ͼ��
		//this->SaveImgToROI();

		//// ����һ��������ҵ����� ROI
		//hv_Model->ho_ModelRegion.WriteRegion(CT2A(this->GetShapeModelRegionPathFile(prID)).m_psz);

		//// ����һ��������ҵ�ģ�� 
		//hv_Model->hv_Model.WriteShapeModel(CT2A(this->GetShapeModelPathFile(prID)).m_psz);
		((checkPatch*)checkPatch::S_dlg.m_pCheck)->writeMode();
		this->write_message(12, 12, "����ƥ����Ϣ�ɹ��ˣ�");
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
		this->write_message(12, 12, "����ƥ����Ϣʧ�ܣ� ������鿴��־");
	}
}

// �����ϴα����ƥ���ͼ
void ST_MYDISP::OnDisButMatchLoadPic()
{	
	try {
		this->m_pMHWinMain->ClearWindow();

		//int prID = UCI::OPTIONS["��Ŀ���"];

		HalconCpp::HImage hImg;
		HalconCpp::HXLDCont Contours;

	//	hv_Model->ho_ModelRegion.GenEmptyObj();            // ���һ��ģ������
	//	UCI::OPTIONS["ƥ��ģ�� OK"] = FALSE;
		Contours.GenEmptyObj();

		HTuple hv_ImageWidth, hv_ImageHeight;

		//CString str = this->GetMatchPathFile(prID);    // �õ�ƥ���ļ���		
		//hImg.ReadImage(CT2A(str).m_psz);
		hImg=this->GrabImageDisp;
		hImg.GetImageSize(&hv_ImageWidth, &hv_ImageHeight);		

		int iHeight = hv_ImageHeight.I();
		int iWidth = hv_ImageWidth.I();

		/*int oldH = UCI::OPTIONS["ͼ��߶�"];
		int oldW = UCI::OPTIONS["ͼ����"];
		if (oldH != iHeight || oldW != iWidth) {

			UCI::OPTIONS["ͼ����"] = iWidth;
			UCI::OPTIONS["ͼ��߶�"] = iHeight;
			UCI::OPTIONS["ORGwhByCamera"] = 0;
		}*/

		this->m_pMHWinMain->SetPart(0, 0, iHeight - 1, iWidth - 1);


		//// ͬʱҲ���浽ץȡͼ�������ȥ
		//this->GrabImageDisp = hImg;

		// �����ߺ������
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
		this->OnMainDisp();              // ��������ʾһ��


		// ��ʾһ��ģ����
		/*if(nullptr != (om = UCI::ELE.GetOptionsMapByEletype(*UCI::ELE.EleList,ELEMENT_MATCH)))
		{
			HTuple Row = (*om)["RES_FindRow"];
			HTuple Column = (*om)["RES_FindColumn"];
			HTuple Angle = (*om)["RES_FindAngle"];
			HTuple Score = (*om)["������ƶ�"];

			if (Row.Length() > 0) {

				write_message(100, 20, "�ҵ��˶��󣬵÷֣�" + Score, true);

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
		//this->DisDrawAllRoi(ho_RoiHxld);            // ��ʾ���е�Ԫ�ص� ROI
		//this->m_pMHWinMain->DispObj(ho_RoiHxld);		

		return;
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
		return;
	}
}

// ֻ�Ǽ���ԭͼ��
void ST_MYDISP::OnDisButLoadPic()
{

	try {

		this->m_pMHWinMain->ClearWindow();

		// ���ֶ����������ǰ����
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
	
	int res = ::MessageBox(NULL,
		"��� ȷ��  ɾ�����в���Ԫ�أ�\n \
		��� ȡ��  ���ء�",
		"���棡�ף������Ҫɾ�����в���Ԫ����",
		MB_OKCANCEL | MB_ICONSTOP | MB_ICONWARNING|MB_SYSTEMMODAL);
	if (res == IDCANCEL) return;

	//int prID = UCI::OPTIONS["��Ŀ���"];

	//// �ӵ�������ʼһ��һ��ɾ������
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
			AfxMessageBox("��ǰ���ڻ�ROI�С�����");
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
		// ��ʾ��Ϣ
		this->write_message(20, 20, "��ģ������,�������Ҽ�ȷ��");
		double Row, Column;
		UCI::RIBmenu.bIsInROIdrawing = true;
		// ������
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
			DisProcessROImodel(hImg, hv_Model->ho_ModelRegion);   // ����ģ��
		else
			m_pMHWinMain->DispObj(hImg);
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}
