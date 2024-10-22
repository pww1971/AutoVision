#include "stdafx.h"

#include "MyData.h"
#include "MyDispLay.h"
using namespace HalconCpp;


// Procedures 
// Chapter: Graphics / Text
// Short Description: Set font independent of OS 
void ST_MYDISP::set_display_font(HTuple hv_WindowHandle, HTuple hv_Size, HTuple hv_Font, HTuple hv_Bold,
	HTuple hv_Slant)
{

	// Local iconic variables

	// Local control variables
	HTuple  hv_OS, hv_BufferWindowHandle, hv_Ascent;
	HTuple  hv_Descent, hv_Width, hv_Height, hv_Scale, hv_Exception;
	HTuple  hv_SubFamily, hv_Fonts, hv_SystemFonts, hv_Guess;
	HTuple  hv_I, hv_Index, hv_AllowedFontSizes, hv_Distances;
	HTuple  hv_Indices, hv_FontSelRegexp, hv_FontsCourier;

	//This procedure sets the text font of the current window with
	//the specified attributes.
	//It is assumed that following fonts are installed on the system:
	//Windows: Courier New, Arial Times New Roman
	//Mac OS X: CourierNewPS, Arial, TimesNewRomanPS
	//Linux: courier, helvetica, times
	//Because fonts are displayed smaller on Linux than on Windows,
	//a scaling factor of 1.25 is used the get comparable results.
	//For Linux, only a limited number of font sizes is supported,
	//to get comparable results, it is recommended to use one of the
	//following sizes: 9, 11, 14, 16, 20, 27
	//(which will be mapped internally on Linux systems to 11, 14, 17, 20, 25, 34)
	//
	//Input parameters:
	//WindowHandle: The graphics window for which the font will be set
	//Size: The font size. If Size=-1, the default of 16 is used.
	//Bold: If set to 'true', a bold font is used
	//Slant: If set to 'true', a slanted font is used
	//
	
	// dev_get_preferences(...); only in hdevelop
	// dev_set_preferences(...); only in hdevelop
	if (0 != (HTuple(hv_Size == HTuple()).TupleOr(hv_Size == -1)))
	{
		hv_Size = 16;
	}

		//Set font on Windows systems
		//try
		//{
		//	//Check, if font scaling is switched on
		//	OpenWindow(0, 0, 256, 256, 0, "buffer", "", &hv_BufferWindowHandle);
		//	SetFont(hv_BufferWindowHandle, "-Consolas-16-*-0-*-*-1-");
		//	GetStringExtents(hv_BufferWindowHandle, "test_string", &hv_Ascent, &hv_Descent,
		//		&hv_Width, &hv_Height);
		//	//Expected width is 110
		//	hv_Scale = 110.0 / hv_Width;
		//	hv_Size = (hv_Size*hv_Scale).TupleInt();
		//	CloseWindow(hv_BufferWindowHandle);
		//}
		//// catch (Exception) 
		//catch (HalconCpp::HException &HDevExpDefaultException)
		//{
		//	HDevExpDefaultException.ToHTuple(&hv_Exception);
		//	//throw (Exception)
		//}
		if (0 != (HTuple(hv_Font == HTuple("Courier")).TupleOr(hv_Font == HTuple("courier"))))
		{
			hv_Font = "Courier New";
		}
		else if (0 != (hv_Font == HTuple("mono")))
		{
			hv_Font = "Consolas";
		}
		else if (0 != (hv_Font == HTuple("sans")))
		{
			hv_Font = "Arial";
		}
		else if (0 != (hv_Font == HTuple("serif")))
		{
			hv_Font = "Times New Roman";
		}
		if (0 != (hv_Bold == HTuple("true")))
		{
			hv_Bold = 1;
		}
		else if (0 != (hv_Bold == HTuple("false")))
		{
			hv_Bold = 0;
		}
		else
		{
			hv_Exception = "Wrong value of control parameter Bold";
			throw HalconCpp::HException(hv_Exception);
		}
		if (0 != (hv_Slant == HTuple("true")))
		{
			hv_Slant = 1;
		}
		else if (0 != (hv_Slant == HTuple("false")))
		{
			hv_Slant = 0;
		}
		else
		{
			hv_Exception = "Wrong value of control parameter Slant";
			throw HalconCpp::HException(hv_Exception);
		}
		try
		{
			SetFont(hv_WindowHandle, ((((((("-" + hv_Font) + "-") + hv_Size) + "-*-") + hv_Slant) + "-*-*-") + hv_Bold) + "-");
		}
		// catch (Exception) 
		catch (HalconCpp::HException &HDevExpDefaultException)
		{
			HDevExpDefaultException.ToHTuple(&hv_Exception);
			//throw (Exception)
		}


	// dev_set_preferences(...); only in hdevelop
	return;
}

bool ST_MYDISP::GetRefMapFromSaveFile(string fileAdd,int prID)
{
	CString str;
	str.Format("��� %d",prID);
	UCI::OptionsMap *om = UCI::ELE.GetOptionsMapByEleName(*UCI::ELE.EleList,str);
	if (nullptr == om)
	{
		AfxMessageBox("û������궨����");
		return false;
	}
	//int prID = UCI::OPTIONS["��Ŀ���"];

	// ��������ڲ�
	/*if (!this->pCAM->Read_CaliCal(hv_CamParameters,prID)) {
		this->write_message(8, 8, "��ȡ����ڲ��ļ�ʧ��!", true);
		return false;
	}

	if (!this->pCAM->Read_CaliPose(hv_CamPose,prID)) {
		this->write_message(8, 8, "��ȡ�����̬�ļ�ʧ��", true);
		return false;
	}*/
		char sz[32];
		sprintf(sz,"%d\\inner\\",prID);
		// ��������ڲ�
		HalconCpp::ReadCamPar((fileAdd+(string)sz+"inner.cal").c_str(), &hv_Cali->hv_CamParameters);
		/*if (!this->pCAM->Read_CaliCal(this->hv_CamParameters, prID)) {
			MyLog((string)"��ȡ����ڲ��ļ�ʧ��!");
			return FALSE;
		}*/
	//	sprintf(sz,"%d\\pos\\",prID);
		hv_Cali->hv_CamPose.ReadPose((fileAdd+(string)sz+"pos.dat").c_str());

	double RectificationWidth = 75.0 / 1000.0;  // mm
	try {

		/*	SetOriginPose(this->hv_CamPose,
		-0.5*RectificationWidth,
		-0.4*RectificationWidth,
		0,
		&this->hv_RectifiPose);*/

		SetOriginPose(this->hv_Cali->hv_CamPose,
			0,
			0,
			0,
			&this->hv_Cali->hv_RectifiPose);

		// HObject  ho_TmpObj_RectificationMap;

		int hv_width = (*om)["ͼ����"];
		int hv_height = (*om)["ͼ��߶�"];


		GenImageToWorldPlaneMap(&this->hv_Cali->ho_RectMap,
			this->hv_Cali->hv_CamParameters,
			this->hv_Cali->hv_RectifiPose,
			hv_width,
			hv_height,
			hv_width,
			hv_height,
			RectificationWidth / hv_width,
			"bilinear"
		);
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
		return false;
	}

	this->m_bRectMapOK = true;  // 

	return true;
}

// MAP
void ST_MYDISP::DisOnButtonGenRectifyMap()
{
	
	// �ȶ�һ��ͼ, ��ʼ��һ��
	
//	if (!this->GetRefMapFromSaveFile()) return;

	this->m_pMHWinMain->DispObj(this->hv_Cali->ho_RectMap);
	this->write_message(200, 8, "�ڲ�:" + hv_Cali->hv_CamParameters, true);
	this->write_message(200, 400, "CamPose ��̬:" + hv_Cali->hv_CamPose, true);
	this->write_message(600, 400, "Recti  ��̬:" + hv_Cali->hv_RectifiPose, true);
}

void ST_MYDISP::DisOnOpenImage()
{
	
	
	TCHAR szFilter[] = _T("ͼ���ļ�|*.bmp;*.jpg;*.jpeg;*.png|�����ļ�(*.*)|*.*||");

	// ������ļ��Ի���   
	CFileDialog fileDlg(true, _T("��ͼ���ļ�"), NULL, 0, szFilter, NULL);

	CString strFilePath;

	// ��ʾ���ļ��Ի���   
	if (IDOK == fileDlg.DoModal()) {

		strFilePath = fileDlg.GetPathName();
	}
	else {
		return;
	}
	
	ReadImageFromFile(strFilePath);
	SetPartHvWnd();
	if (NULL != this->m_pMHWinMain)
		this->m_pMHWinMain->DispObj(this->GrabImageDisp);
	
	write_message(12, 12, CT2A(strFilePath).m_psz, true);

	// ����һ�´��ڵı���
	// this->pCView->SetWindowText(strFilePath);
}

void ST_MYDISP::DisOnSaveCurrentDISPtoFile()
{
	
	CString format = UCI::OPTIONS["���񱣴��ʽ"];

	CString strName;
	if (this->SaveImgToFile(UCI::stPictureSavePath, format, strName)) {
		write_message(12, 12, "����ԭʼͼƬ�ɹ���", true);
		write_message(32, 12, CT2A(strName).m_psz, true);

		AfxMessageBox(strName);
	}
	else {
		write_message(12, 12, "����ͼƬʧ�ܣ�", true);
	}
}

void ST_MYDISP::Zoomfit()
{
	int hv_width = UCI::OPTIONS["ͼ����"];
	int hv_height = UCI::OPTIONS["ͼ��߶�"];

	this->DisDisplay(CRect(0, 0, hv_width - 1, hv_height - 1));



	m_Zoom=1;
	m_ZoomOrg=1;

	// ��ʾһ��
	UCI::pDis->OnMainDisp();

	/*
	UpdateData(FALSE);
	clear_window(m_WindowHandleOrg);
	disp_obj(m_ImageOrg,m_WindowHandleOrg);*/
}
void ST_MYDISP::scaleState(bool s )
{	
	if (s)
	{
		m_Zoom *= 0.8;
		if (m_Zoom < 1)
		{
			m_Zoom = 1;
			Zoomfit();
		}
	}
	else
	{
		m_Zoom /= 0.8;
		if (m_Zoom > 100)
		{
			m_Zoom = 100;
		}
	}
}
void ST_MYDISP::ControlFunction(HTuple WindowHandleOrg,HTuple ZoomTrans,HTuple Row,HTuple Col,HTuple RowShif,HTuple ColShif)
{
	HTuple  hv_Row0, hv_Column0,hv_Row00, hv_Column00,hv_Wt,hv_Ht;
	GetPart(WindowHandleOrg,&hv_Row0, &hv_Column0,&hv_Row00, &hv_Column00);
	hv_Ht=hv_Row00-hv_Row0;
	hv_Wt=hv_Column00-hv_Column0;	
	HTuple r1,c1,r2,c2;
	r1 = hv_Row0+((1-(1/ZoomTrans))*(Row-hv_Row0))- RowShif/ZoomTrans;
	c1 = hv_Column0+((1-(1/ZoomTrans))*(Col-hv_Column0))-ColShif/ZoomTrans;
	r2 = r1+(hv_Ht/ZoomTrans);
	c2= c1+(hv_Wt/ZoomTrans);
	this->DisDisplay(CRect((int)c1.D(), (int)r1.D(), (int)c2.D(), (int)r2.D()));
	
}

// �������¼��� �Ŵ���С�� ȡ�Ҷ�ֵ����ԭͼ���С
void ST_MYDISP::DisOnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	try {
		CRect WndRect;

		//Hlong ImagePtX, ImagePtY;
		//Hlong Row0_1, Col0_1, Row1_1, Col1_1;

		CString str;

		this->PMainStatic->GetWindowRect(&WndRect);
;
		this->m_pMHWinMain->ClearWindow();

		double dRatio = UCI::OPTIONS["ͼ�����"];

		int raHeight = int(WndRect.Width()* dRatio);
		WndRect.bottom = WndRect.top + raHeight;

		if (!WndRect.PtInRect(pt))
			return;

		// �ж�����Ҽ��ǲ��ǰ����ˣ���ָ�ͼ�����ʾ����
		if (MK_RBUTTON == nFlags) {
			Zoomfit();
			return;
		}
	
				
		HTuple   hv_Button, hv_Row, hv_Column;
		SetCheck("~give_error");
		GetMposition(*m_pMHWinMain, &hv_Row, &hv_Column, &hv_Button);
		SetCheck("give_error");
		{
			if (zDelta>0)
			{
				scaleState(0);
			}
			else
				scaleState(1);
			
			//��ʾ
			HTuple ZoomTrans=m_Zoom/m_ZoomOrg;
			m_ZoomOrg=m_Zoom;
			ControlFunction(*m_pMHWinMain,ZoomTrans,hv_Row,hv_Column,0,0);
			// ��ʾһ��
			UCI::pDis->OnMainDisp();
		}


	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}

}

void ST_MYDISP::DisOnMouseLeftDown(UINT nFlags, CPoint point)
{
	
}

void ST_MYDISP::DisOnMouseMove(UINT nFlags, CPoint point)
{
	//this->pImg->bMustPaint = true;	

	//this->ptMouseLdownNow = point;


	// ���ֻ�����������д
	//CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	//CClientDC DC(pMain);

	//CPen pen(PS_SOLID, 1, RGB(255, 0, 0));	
	//CPen *oldpen = pDC->SelectObject(&pen);


	//int len = 10;
	//pDC->MoveTo(point.x - len, point.y - len);
	//pDC->LineTo(point.x + len, point.y + len);
	//pDC->MoveTo(point.x - len, point.y + len);
	//pDC->LineTo(point.x + len, point.y - len);

	// pDC->SelectObject(oldpen);
}

void ST_MYDISP::DisOnRButtonDown(UINT nFlags, CPoint point)
{
}

void ST_MYDISP::DisOnRButtonUp(UINT nFlags, CPoint point)
{
	//ClearWindow(this->hv_HWD);
	//// �ָ�ԭ���ı���
	//CRect rc = { 0,0,this->pImg->iWidth,this->pImg->iHeight };
	//this->pImg->ChangeImageCRect(rc);
	//this->SetPartHvWnd();
	//this->bMouseLBdown = false;
}

// ����ɼ�������Ƭ,��ͬ���ع�,����
void ST_MYDISP::DisOnCamCapMulti()
{
	//DisOpenCamera();

	HImage      GrabImage;

	// TODO: �ڴ���������������
	HTuple hv_Start, hv_End;
	CString str;

	// ���òɼ���֡��
	int nFrameNum = 7;

	// ��¼��ʼ��ʱ
	CountSeconds(&hv_Start);

	// ѭ���ɼ������漰��ʾ
	for (int i = 0; i < nFrameNum; i++) {

		//// �����ع�
		//this->pCAM->SetShutter(500 * (i)+1);
		//// ��������
		//this->pCAM->SetGain(5 * i);

		// �ɼ�һ֡ͼ��
	//	this->DisOnOneShot();

		CString format = UCI::OPTIONS["���񱣴��ʽ"];

		CString strName;
		bool res = SaveImgToFile("UCI::stShutterSavePath", format, strName);

		if (res) {
			AfxMessageBox(strName+" ����ͼƬ�ɹ��ˣ�");
		}

		Sleep(10);
		
	}
	// ��¼����ʱ��
	CountSeconds(&hv_End);

	str.Format("�����عⲢ�ɼ��� %d ֡ͼ�� ��ʱ��(s) %f",
		nFrameNum, (hv_End - hv_Start).D());
	AfxMessageBox(str);
}
void ST_MYDISP::DisOnButCaliShowInfo(int i)
{
	HPose hv_pose;
	int prID = UCI::OPTIONS["��Ŀ���"];
	HTuple hv_CameraParameters;
	char sz[32];
	string filedir = getExePath()+"Cali\\";
	POSITION pos = UCI::ELE.EleList->GetHeadPosition();
	while (pos != NULL) {
		UCI::OptionsMap *o = &UCI::ELE.EleList->GetNext(pos);

		sprintf(sz,"%d\\inner\\",i);
		string filename = filedir+(string)sz+"inner.cal";
		if (!PathFileExists(filename.c_str()))
		{
			write_message(600, 200, "���"+ HTuple(i) +"�ڲ�û�����ݣ�");
			return;
		}
		HalconCpp::ReadCamPar(filename.c_str(), &hv_CameraParameters);
		write_message(600, 200, "����ڲΣ�" + hv_CameraParameters, 0);

	//	sprintf(sz,"%d\\pos\\",i++);
		filename = filedir+(string)sz+"pos.dat";
		if (!PathFileExists(filename.c_str()))
		{
			write_message(40, 200, "���"+ HTuple(i) +"��̬û�����ݣ�");
			return;
		}
		hv_pose.ReadPose(filename.c_str());
		write_message(40, 200, "�����̬��" + hv_pose, 0);
		Sleep(500);
	}

}




