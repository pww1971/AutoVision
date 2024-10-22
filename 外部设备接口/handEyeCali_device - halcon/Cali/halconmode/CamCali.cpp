#include "stdafx.h"

#include "MyData.h"
#include "MyDispLay.h"

using namespace HalconCpp;
using namespace std;



// �궨��ť����, ֻ�Ǳ궨�ڲ�
void ST_MYDISP::DisOnButCamCaliInner(int prID)
{
	if (this->mAppState == App_CamInner) {
		//this->StopContiouscap();
		this->AppStateChangeTo(APP_READY);
		AfxMessageBox("��ֹͣ�궨����");
		return;
	}

	//	int prID = UCI::OPTIONS["��Ŀ���"];
	char sz[32];
	sprintf(sz,"%d\\inner\\",prID);
	string dir = getExePath()+"Cali\\"+(string)sz;
	CString CaliPathFile = (dir+"inner.cal").c_str();

	if (PathFileExists(CaliPathFile))
	{
		int res = ::MessageBox(NULL,
			"��� ȷ��  ���±궨��\n"
			"��� ȡ��  ���ء�",
			"����ڲα궨�ļ��Ѵ��ڣ�",
			MB_OKCANCEL | MB_ICONSTOP | MB_ICONWARNING | MB_SYSTEMMODAL);
		if (res == IDCANCEL) return;
		if (res == IDOK) {
			CFile TempFile;
			TempFile.Remove(CaliPathFile);
			remove((dir+"pos.dat").c_str());
		}
	}

	int total = UCI::OPTIONS["�궨ͼƬ����"];
	// ��ǰͼƬ������������
	vector<string> vctFile;
	getFileNameVec(dir.c_str(),vctFile);
	if (vctFile.size() >= total && PushCaptureDown == 1) {
		int res = ::MessageBox(NULL,
			"��� ȷ��  ���²ɼ�ͼƬ��\n"
			"��� ȡ��  ���ء�",
			"����궨�ڲ�ͼƬ�����ѴﵽҪ���ˡ���",
			MB_OKCANCEL | MB_ICONSTOP | MB_ICONWARNING | MB_SYSTEMMODAL);
		if (res == IDCANCEL)
		{
			PushCaptureDown = 0;
			return;
		}
		else if (res == IDOK) {
			UCI::FindSpecialFile(dir.c_str(), "*.png", NULL, true);
			//this->DelTotalCariInnerPic(prID); // ��ɾ�����еĲɼ�ͼ���ļ�
		}
	}		

	//	this->DisOpenCamera();                                 // �����

	//	UCI::OptionsMap * camo = &UCI::ELE.EleList->GetHead();
	//	UCI::sDB.SetCamPara(*camo);


	// �趨һ�µ�ǰ������궨����	
	this->AppStateChangeTo(App_CamInner);
	UCI::pDis->IPRunCamCaliInner();
	this->AppStateChangeTo(APP_READY);
}

// ���水ť
void ST_MYDISP::DisOnSaveOnCaliPicture()
{
	// �����ǰ�Ǳ궨ģʽ����Ҫ����ͼƬ
	this->PushCaptureDown = 1;
}


// �궨��ť����, ֻ�Ǳ궨��̬
void ST_MYDISP::DisOnButCamCaliPose(int prID)
{
	if (this->mAppState == App_CamPose) {
		//		this->StopContiouscap();
		this->AppStateChangeTo(APP_READY);
		AfxMessageBox("��ֹͣ�궨��̬��");
	}

	char sz[32];
	sprintf(sz,"%d\\inner\\",prID);
	string dir = getExePath()+"Cali\\"+(string)sz;
	CString CaliPathFile = (dir+"pos.dat").c_str();

	// �����ǲ����Ѵ����˱궨�ڲ��ļ��������̬�ļ�  PathFileExists
	if (PathFileExists(CaliPathFile))
	{
		int res = ::MessageBox(NULL,
			"��� ȷ��  ���±궨��\n"
			"��� ȡ��  ���ء�",
			"�����̬�궨�ļ��Ѵ��ڣ�",
			MB_OKCANCEL | MB_ICONSTOP | MB_ICONWARNING | MB_SYSTEMMODAL);
		if (res == IDCANCEL) return;
		if (res == IDOK) {
			CFile TempFile;
			TempFile.Remove(CaliPathFile);
		}
	}

	// ��ǰͼƬ������������
	int total = 1;//UCI::OPTIONS["�궨ͼƬ����"];
	// ��ǰͼƬ������������
	vector<string> vctFile;
	getFileNameVec(dir.c_str(),vctFile);
	if (vctFile.size() >= total && PushCaptureDown == 1) {

		int res = ::MessageBox(NULL,
			"��� ȷ��  ���²ɼ�ͼƬ��\n"
			"��� ȡ��  ���ء�",
			"����궨��̬ͼƬ�����ѴﵽҪ���ˡ���",
			MB_OKCANCEL | MB_ICONSTOP | MB_ICONWARNING | MB_SYSTEMMODAL);
		if (res == IDCANCEL)
		{
			PushCaptureDown = 0;
			return;
		}
		else if (res == IDOK) {
			UCI::FindSpecialFile(dir.c_str(), "*.png", NULL, true);
			//	this->DelTotalCariPosePic(prID); // ��ɾ�����еĲɼ�ͼ���ļ� 
		}
	}

	//	this->DisOpenCamera();   // �����

	//	UCI::OptionsMap * camo = &UCI::ELE.EleList->GetHead();
	//	UCI::sDB.SetCamPara(*camo);

	this->AppStateChangeTo(App_CamPose);
	UCI::pDis->IPRunCamCaliPose();
	this->AppStateChangeTo(APP_READY);
}
// ���ݱ궨��У��ͼ��
void ST_MYDISP::DisOnButCaliCorrect(int prID)
{
	CString str;
	str.Format("��� %d",prID);
	UCI::OptionsMap *om = UCI::ELE.GetOptionsMapByEleName(*UCI::ELE.EleList,str);
	if (nullptr == om)
	{
		AfxMessageBox("û������궨����,��ѡ�����");
		return ;
	}
	BOOL isLoad = (*om)["�궨��Ϣ OK"];
	if (isLoad == FALSE) {
		if (this->LoadCaliPoseAndParamter(getExePath()+"Cali\\",prID) == FALSE) {
			write_message(200, 20, "���ر궨����û�гɹ������ȱ궨���");
			return;
		}
	}

	try{
		int width = (*om)["ͼ����"] = m_sIPResult.result_img.Width();
		int height = (*om)["ͼ��߶�"] = m_sIPResult.result_img.Height();
		
		this->m_pMHWinMain->SetPart(0, 0, height - 1, width - 1);
		MapImage(m_sIPResult.result_img, this->hv_Cali->ho_RectMap, &this->m_sIPResult.result_img);
		
	//	GrabImageDisp = m_sIPResult.result_img;
	//	this->m_sIPResult.result_img.WriteImage("bmp",0,"c:\gray.bmp");
		this->m_pMHWinMain->DispObj(this->m_sIPResult.result_img);
		OnMainDisp();
		DisOnSaveCurrentDISPtoFile();  // �ٱ���һ��	

	}
	catch (HalconCpp::HException& except) {
		(*om)["�궨��Ϣ OK"] = 0;
		AfxMessageBox(except.ErrorText().Text());
	}

}
void ST_MYDISP::DisOnMatchTakeOne()
{
	try {

		this->m_pMHWinMain->ClearWindow();

		// ���ֶ����������ǰ����
		HalconCpp::HXLDCont Contours;
		HalconCpp::HImage hImg;

		UCI::OptionsMap *om = UCI::ELE.GetOptionsMapByEletype(*UCI::ELE.EleList,ELEMENT_CAMERA);
		this->MeasureOneCamera(om, hImg, Contours);

		//	this->m_pMHWinMain->DispObj(hImg);

		om = UCI::ELE.GetOptionsMapByEletype(*UCI::ELE.EleList,ELEMENT_ROI);
		if (om == NULL)
		{
			return;
		}

		this->MeasureOneROI(om, hImg, Contours);

		//	this->m_pMHWinMain->ClearWindow();
		//	this->m_pMHWinMain->DispObj(hImg);

		om = UCI::ELE.GetOptionsMapByEletype(*UCI::ELE.EleList,ELEMENT_FUNCTION);
		this->MeasureOneFUNCTION(om, hImg, Contours);
		//om = UCI::ELE.GetOptionsMapByEletype(ELEMENT_MATCH);
		//this->MeasureOneMATCH(om, hImg, Contours);
		//	this->m_pMHWinMain->ClearWindow();
		//	this->m_pMHWinMain->DispObj(hImg);

//		EnterCriticalSection(&m_csIPDisp);
		this->m_sIPResult.result_img = hImg;
		this->m_sIPResult.result_cont = Contours;
//		LeaveCriticalSection(&m_csIPDisp);
		this->OnMainDisp();              // ��������ʾһ��

	}
	catch (HalconCpp::HException& except) {
		//		UCI::sDB.Log(except, "DisOnMatchTakeOne()��");
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
		//		UCI::sDB.Log(except, "DisDrawOneRoi �� ROI ����");
	}
}

// ɾ�����е�ƥ�����
//void ST_MYDISP::DisOnMatchDelAllRoi()
//{
//
//	this->m_sIPResult.result_cont.GenEmptyObj();
//	this->ho_ModelRegion.GenEmptyRegion();
//	this->hv_Model.Clear();
//
//	// ��Ҫɾ������������ļ���ROI�ļ�
//	int prID = UCI::OPTIONS["��Ŀ���"];
//
//	CString stRegion = this->GetShapeModelRegionPathFile(prID);
//	CString stModel = this->GetShapeModelPathFile(prID);
//
//	CFile TempFile;
//	if (GetFileAttributes(stRegion) != -1) { // �ļ�������
//		TempFile.Remove(stRegion);
//	}
//	if (GetFileAttributes(stModel) != -1) { // �ļ�������
//		TempFile.Remove(stModel);
//	}
//
//	this->OnMainDisp(NULL, NULL);
//}
