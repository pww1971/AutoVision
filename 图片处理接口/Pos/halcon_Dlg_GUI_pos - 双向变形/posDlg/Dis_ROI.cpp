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
	//	(*om)["ƥ��ģ�� OK"] = FALSE;

		/*UCI::OptionsMap *om;
		om = UCI::ELE.GetOptionsMapByEletype(*UCI::ELE.EleList,ELEMENT_CAMERA);
		this->MeasureOneCamera(om, hImg, Cont);

		om = UCI::ELE.GetOptionsMapByEletype(*UCI::ELE.EleList,ELEMENT_ROI);
		this->MeasureOneROI(om, hImg, Cont);

		om = UCI::ELE.GetOptionsMapByEletype(*UCI::ELE.EleList,ELEMENT_FUNCTION);
		this->MeasureOneFUNCTION(om, hImg, Cont);
		om = UCI::ELE.GetOptionsMapByEletype(*UCI::ELE.EleList,ELEMENT_MATCH);*/
		if (this->MeasureOneMATCH(om, hImg, Cont) == FALSE) {
			write_message(10, 10, "û���ҵ���Ӧ��ģ��");
			return;
		}

		Cont = Cont.ConcatObj(hv_Model->ho_ModelRegion);

//		EnterCriticalSection(&m_csIPDisp);
		this->m_sIPResult.result_img = hImg;
		this->m_sIPResult.result_cont = Cont;
//		LeaveCriticalSection(&m_csIPDisp);
		this->OnMainDisp();              // ��������ʾһ��
		// ��ʾһ��ģ����
		om = UCI::ELE.GetOptionsMapByEletype(*UCI::ELE.EleList,ELEMENT_MATCH);
		HTuple Row = (*om)["RES_FindRow"];
		HTuple Column = (*om)["RES_FindColumn"];
		HTuple Angle = (*om)["RES_FindAngle"];
		HTuple Score = (*om)["������ƶ�"];

		if (Row.Length() > 0) {

			write_message(100, 20, "�ҵ��˶��󣬵÷֣�" + Score, true);

			CString mcolor = (*om)["DispShapeColor"];
			int modeType = (*om)["ƥ��ģ��"];
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


		//int prID = UCI::OPTIONS["��Ŀ���"];
		//// �ɼ�һ��ͼƬ
		//CString stModel = this->GetShapeModelPathFile(prID);		//

		//HalconCpp::HImage hImg;
		//if (GetFileAttributes(stModel) == -1) { // �ļ�������
		//	MyLog(stModel);
		//	MyLog("DisOnRoiInfo() �ļ������ڣ� ");
		//	return;
		//}
		//this->hv_Model.ReadShapeModel(CT2A(stModel).m_psz);

		//EnterCriticalSection(&m_csIPDisp);
		////m_sIPResult.result_cont = HXdis;
		//hImg = m_sIPResult.result_img;
		//LeaveCriticalSection(&m_csIPDisp);

		//this->OnMainDisp(NULL, NULL);

		//double AngleStart = UCI::OPTIONS["��ʼ�ǻ���"];
		//double AngleExtend = UCI::OPTIONS["��ת��Χ"];

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
		//	MyLog("DisOnRoiFindModel() û���ҵ���Ӧ�Ĳ�Ʒ��״");
		//	return;
		//}
		//

		//double dMin = UCI::OPTIONS["���ƶ�"];

		//this->write_message(20, 20, "��ǰ���ҵ÷֣�" + Score1 + " ����ֵ��" + dMin);
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
		//	MyLog("DisOnRoiFindModel() û���ҵ���Ӧ�Ĳ�Ʒ��״");
		//}

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
