#include "stdafx.h"

#include "MyData.h"
#include "MyDispLay.h"
#include "checkPatch.h"

using namespace HalconCpp;
using namespace std;

// �궨�ڲδ����߳�
void ST_MYDISP::IPRunCamCaliInner()
{
	HImage         Image;
	HXLDCont       Contours;
	Contours.GenEmptyObj();

	HTuple T1, T2;
	
	int id = checkPatch::S_dlg.GetCurrentSelElementID();
	if(id > -1)
	{
		UCI::pDis->hv_Cali = ((checkPatch*)checkPatch::S_dlg.m_pCheck)->m_CaliList[id-1];
#ifdef HCAM
		vector<HImage> vm;
		if (-1 != ((checkPatch*)checkPatch::S_dlg.m_pCheck)->HCameraDrv::grapImage(id - 1, vm))
			Image = vm[0];
#else
		vector<cv::Mat> vm;
		if(-1 != ((checkPatch*)checkPatch::S_dlg.m_pCheck)->CameraDrv::grapImage(id-1,vm))
			Image = Mat2HObject(vm[0]);
#endif
		else
			return;
		UCI::pDis->m_sIPResult.result_img = Image;
		CountSeconds(&T1);
		DisCamliInnerOnePicture(&Image, &Contours,id);
		CountSeconds(&T2);

		m_sIPResult.time_needed = 1000 * (T2[0].D() - T1[0].D());

		HTuple time = m_sIPResult.time_needed;

		CString str;
		str.Format(_T("�ڲα궨 >��ǰ����ʱ��Ϊ: %4.2f ms"), time[0].D());
		AfxMessageBox(str);

	}
}

// �ڲ�ȡͼ
void ST_MYDISP::DisCamliInnerOnePicture(HalconCpp::HImage * Image, HalconCpp::HXLDCont*  Contours,int prID)
{
	try
	{
		CString str;
		str.Format("��� %d",prID);
		UCI::OptionsMap *om = UCI::ELE.GetOptionsMapByEleName(*UCI::ELE.EleList,str);
		if (nullptr == om)
		{
			AfxMessageBox("û������궨����");
			return ;
		}
		// ����Ĳ���ҲҪ����һ�¡�
		HTuple Width, Height;
		GetImageSize(*Image, &Width, &Height);
		UCI::OPTIONS["ͼ����"] = Width.I();
		UCI::OPTIONS["ͼ��߶�"] = Height.I();

		HTuple hv_TmpCtrl_ReferenceIndex;
		HTuple hv_StartParameters;
		HTuple hv_TmpCtrl_FindCalObjParNames;
		HTuple hv_TmpCtrl_FindCalObjParValues;
		//HTuple hv_CalibHandle;
		HTuple hv_CameraParameters;
		HTuple hv_CameraPose;
		HTuple hv_TmpCtrl_Errors;
		HTuple hv_Index;

		Contours->GenEmptyObj();

		char sz[32];
		sprintf(sz,"%d\\",prID);
		string dir = getExePath()+"Cali\\"+string(sz);
		pww_mkdir(dir.c_str());
		dir = dir + "inner\\";
		pww_mkdir(dir.c_str());
		vector<string> vctFile;
		getFileNameVec(dir.c_str(),vctFile);

		int nPic =vctFile.size();
	int nTotal = UCI::OPTIONS["�궨ͼƬ����"];

		if (nPic >= nTotal) {                      // ͼƬ���������㣬���Լ��㲢����궨�ڲΡ�		
			this->DisGetCameraInnerParaFromPic(om,dir,vctFile);  // ȡ�õ�������ڲ�������
			this->AppStateChangeTo(APP_READY);
			return;
		}

		hv_TmpCtrl_ReferenceIndex = 0;

		// �궨�����ļ�	
		CString descFile = UCI::OPTIONS["�궨�����ļ�"];
		HTuple hv_TmpCtrl_PlateDescription = CT2A(descFile).m_psz;

		// �궨��ʼ������� 
		DisSetInnerAndPosStartPara(om,hv_StartParameters,
			hv_TmpCtrl_FindCalObjParNames,
			hv_TmpCtrl_FindCalObjParValues);

		// �����궨���
		HCalibData m_HCali;
		m_HCali.CreateCalibData("calibration_object", 1, 1);

		// ���ñ궨��ʼ����
		CString calimodel = UCI::OPTIONS["CamCalibrateModel"];
		m_HCali.SetCalibDataCamParam(0,
			CT2A(calimodel).m_psz,
			(HCamPar)hv_StartParameters);

		// ���ñ궨�����ļ�
		m_HCali.SetCalibDataCalibObject(0, hv_TmpCtrl_PlateDescription);

		hv_Index = 0;
		// ����ͼ���еı궨��
		m_HCali.FindCalibObject(*Image, 0, 0, hv_Index, hv_TmpCtrl_FindCalObjParNames,
			hv_TmpCtrl_FindCalObjParValues);

		// ����һ��ͼ�����ɫ
		this->m_pMHWinMain->SetColor("magenta");

		// �õ��궨�����̬
		HPose hv_pose = m_HCali.GetCalibDataObservPose(0, 0, hv_Index);

		// ��ʾͼ��
		this->m_pMHWinMain->DispObj(*Image);

		// ��ʾ�궨��Բ����
		this->m_pMHWinMain->DispCaltab(hv_TmpCtrl_PlateDescription,
			(HCamPar)hv_StartParameters, hv_pose, 1.0);

		// ��ʾ�궨����������
		this->m_pMHWinMain->SetColor("red");
		this->m_pMHWinMain->SetLineWidth(2);

		*Contours = m_HCali.GetCalibDataObservContours("caltab", 0, 0, 0);

		this->m_pMHWinMain->DispObj(*Contours);

		// ����궨����
		hv_TmpCtrl_Errors = m_HCali.CalibrateCameras();

		write_message(8, 8, "���ڲɼ��궨ͼ�񣬵�ǰ���:" + hv_TmpCtrl_Errors
			+ "���أ��Ѿ��ɼ���" + HTuple(nPic)
			+ "��������" + HTuple(nTotal - nPic) + "��ͼ��", 1);

		double picError = UCI::OPTIONS["�궨�������"];
		if (hv_TmpCtrl_Errors <  picError) {
			// ����ͼ��
			//this->GrabImageERROK = *Image;
			write_message(100, 8, "���½�ͼ��ť����ɱ���ͼ���뾡��ѡȡ��ͬ��λ�ã�", 0);

			if (this->PushCaptureDown == 1) {
				this->PushCaptureDown = 0;
				sprintf(sz,"%d",nPic); 
				if (Write_Image(*Image, (dir+string(sz)+".png").c_str()))
				{
				/*	CString str;
					str.Format("�ɹ�������һ���궨�ڲ�ͼ��. ��ǰ���� %d �궨OK��ͼ��",nPic+1);
					AfxMessageBox(str);*/
				}
			}
			else {
				//Sleep(50);
			}

		}
		else {
			write_message(100, 8, "��ǰͼ�����̫������һ�����û�ͷ��", 0);
		}
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
	// ��ʾһ��
//	UCI::pDis->OnMainDisp();
	//PostMessage(UCI::pMain->m_hWnd, UM_UPDATE_RIBBON, (WPARAM)0, (LPARAM)0);
}

void ST_MYDISP::DisSetInnerAndPosStartPara(UCI::OptionsMap *om,
	HalconCpp::HTuple & hv_StartParameters,
	HalconCpp::HTuple & hv_TmpCtrl_FindCalObjParNames, 
	HalconCpp::HTuple & hv_TmpCtrl_FindCalObjParValues)
{

	// �궨��ʼ������� ��ɨ�裬����ʽ
	hv_StartParameters.Clear();     // 
	double focus = UCI::OPTIONS["��ͷ����"];
	hv_StartParameters[0] = focus / 1000.0;
	hv_StartParameters[1] = 0;
	hv_StartParameters[2] = 0;
	hv_StartParameters[3] = 0;
	hv_StartParameters[4] = 0;
	hv_StartParameters[5] = 0;

	double fsx = UCI::OPTIONS["���ؿ��"];
	double fsy = UCI::OPTIONS["���ظ߶�"];
	hv_StartParameters[6] = fsx / 1e+006;
	hv_StartParameters[7] = fsy / 1e+006;

	hv_StartParameters[8] = this->GetWidth() / 2;
	hv_StartParameters[9] = this->GetHeight() / 2;
	hv_StartParameters[10] = this->GetWidth();
	hv_StartParameters[11] = this->GetHeight();

	// FindCalObjParNames;

	hv_TmpCtrl_FindCalObjParNames.Clear();
	hv_TmpCtrl_FindCalObjParNames[0] = "gap_tolerance";
	hv_TmpCtrl_FindCalObjParNames[1] = "alpha";
	hv_TmpCtrl_FindCalObjParNames[2] = "max_diam_marks";
	hv_TmpCtrl_FindCalObjParNames[3] = "skip_find_caltab";

	// FindCalObjParValues

	double gap_tolerance = UCI::OPTIONS["�궨�����������"];
	double alpha = UCI::OPTIONS["�궨ƽ��alpha"];
	double max_diam_marks = UCI::OPTIONS["�궨����־��ֱ��"];

	hv_TmpCtrl_FindCalObjParValues.Clear();
	hv_TmpCtrl_FindCalObjParValues[0] = gap_tolerance;
	hv_TmpCtrl_FindCalObjParValues[1] = alpha;
	hv_TmpCtrl_FindCalObjParValues[2] = max_diam_marks;
	hv_TmpCtrl_FindCalObjParValues[3] = "false";
}



// ��ͼƬ��ȡ���ڲΣ�������
void ST_MYDISP::DisGetCameraInnerParaFromPic(UCI::OptionsMap *om,string fileAdd,vector<string>& vctFile)
{
	HTuple hv_TmpCtrl_ReferenceIndex;
	HTuple hv_StartParameters;
	HTuple hv_TmpCtrl_FindCalObjParNames;
	HTuple hv_TmpCtrl_FindCalObjParValues;
	HTuple hv_CameraPose;
	HTuple hv_TmpCtrl_Errors;

	int nPic = vctFile.size();
	int nTotal = UCI::OPTIONS["�궨ͼƬ����"];        // 

	if (nPic < nTotal) {                       // ͼƬ����������
		write_message(8, 8, "�궨ͼ������̫С������" + HTuple(nTotal - nPic) + "��ͼ��", 0);
		return;
	}
	try {

		hv_TmpCtrl_ReferenceIndex = 0;

		// �궨�����ļ�
		CString descFile = UCI::OPTIONS["�궨�����ļ�"];
		HTuple hv_TmpCtrl_PlateDescription = CT2A(descFile).m_psz;

		DisSetInnerAndPosStartPara(om,hv_StartParameters,
			hv_TmpCtrl_FindCalObjParNames,
			hv_TmpCtrl_FindCalObjParValues);

		// �����궨���
		HCalibData m_HCali;
		m_HCali.CreateCalibData("calibration_object", 1, 1);

		// ���ñ궨��ʼ����
		CString calimodel = UCI::OPTIONS["CamCalibrateModel"];
		m_HCali.SetCalibDataCamParam(0,
			CT2A(calimodel).m_psz,
			(HCamPar)hv_StartParameters);
		// ���ñ궨�����ļ�
		m_HCali.SetCalibDataCalibObject(0, hv_TmpCtrl_PlateDescription);

		HalconCpp::HImage Image;

		int nTpic = vctFile.size();
		for (int i = 0; i < nTpic; i++) {

			Image.ReadImage((fileAdd+vctFile[i]).c_str());

			//EnterCriticalSection(&this->m_csFGDisp);
			//this->GrabImageDisp = Image;
			//LeaveCriticalSection(&this->m_csFGDisp);		
			this->m_pMHWinMain->DispObj(Image);
			//this->DisDisplay();
			write_message(12, 12, "���ڴ�" + HTuple(i) + "�ű궨���ռ����ݡ�����", 0);

			// ����ͼ���еı궨ͼ��
			m_HCali.FindCalibObject(Image, 0, 0, i, hv_TmpCtrl_FindCalObjParNames,
				hv_TmpCtrl_FindCalObjParValues);
		}

		// ����һ��ͼ�����ɫ
		this->m_pMHWinMain->SetColor("green");

		// ȡ�ñ궨��������Ϣ
		HPose hv_pose = m_HCali.GetCalibDataObservPose(0, 0, nTpic - 1);

		// ��ʾ�궨��Բ����
		this->m_pMHWinMain->DispCaltab(hv_TmpCtrl_PlateDescription,
			(HCamPar)hv_StartParameters, hv_pose, 1.0);

		// ȡ�ñ궨����������
		HalconCpp::HXLDCont  Contours
			= m_HCali.GetCalibDataObservContours("caltab", 0, 0, nTpic - 1);

		// ��ʾ�궨����������
		this->m_pMHWinMain->SetColor("green");
		this->m_pMHWinMain->SetLineWidth(2);

		this->m_pMHWinMain->DispObj(Contours);

		// ����궨����
		hv_TmpCtrl_Errors = m_HCali.CalibrateCameras();

		double picError = UCI::OPTIONS["�궨�������"];
		if (hv_TmpCtrl_Errors < picError) {
			write_message(8, 8, "�ܹ��궨��" + HTuple(nTpic) + "��ͼ��,"
				+ "�궨�ɹ����궨���:" + hv_TmpCtrl_Errors, 1);

			// ȡ��������ڲ�
			hv_Cali->hv_CamParameters = m_HCali.GetCalibData("camera", 0, "params");
			//��һ��ͼƬ��Ϊ�ο�λ��
			GetCalibData(m_HCali, "calib_obj_pose", HTuple(0).TupleConcat(0),"pose", &hv_CameraPose);
			////Calibration 01: Adjust origin for plate thickness
			double thick = UCI::OPTIONS["�궨����"];
			SetOriginPose(hv_CameraPose, 0.0, 0.0, thick, &hv_CameraPose);
			write_message(600, 600, "�����̬pre��" + hv_CameraPose, 0);
			HalconCpp::WritePose(hv_CameraPose,(fileAdd+"pos1.dat").c_str());
			hv_Cali->hv_CamPose.ReadPose((fileAdd+"pos1.dat").c_str());
			remove((fileAdd+"pos1.dat").c_str());
			//write_message(60, 8, "��������ڲ�������ļ��ɹ���", 0);
			//HalconCpp::WriteCamPar(hv_Cali->hv_CamParameters,(fileAdd+"inner.cal").c_str());
			write_message(300, 8, "����ڲΣ�" + hv_Cali->hv_CamParameters, 0);

			double  curErr = hv_TmpCtrl_Errors.D();
			UCI::OPTIONS["�궨��ǰ���"] = curErr;
			UCI::OPTIONS["�궨��Ϣ OK"] = 1;
		}
		else {
			write_message(60, 8, "�ܹ�" + HTuple(nTotal - nPic) + "��ͼ��,"
				+ "�궨ʧ�ܣ��궨����:" + hv_TmpCtrl_Errors, 1);

			write_message(120, 8, "�����²ɼ�ͼ��궨��", 0);
		}
		//Calibration 01: Clear calibration model when done
		//ClearCalibData(m_HCali);
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}