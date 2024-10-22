#include "stdafx.h"
#include "MyData.h"
#include "MyDispLay.h"
#include "checkPatch.h"

using namespace HalconCpp;
using namespace std;

void ST_MYDISP::IPRunCamCaliPose()
{
	HImage         Image;
	HXLDCont       Contours;
	Contours.GenEmptyObj();

	HTuple T1, T2;
	vector<cv::Mat> vm;
	int id = checkPatch::S_dlg.GetCurrentSelElementID();
	if (id > -1)
	{
		UCI::pDis->hv_Cali = ((checkPatch*)checkPatch::S_dlg.m_pCheck)->m_CaliList[id-1];
#ifdef HCAM
		vector<HImage> vm;
		if (-1 != ((checkPatch*)checkPatch::S_dlg.m_pCheck)->HCameraDrv::grapImage(id - 1, vm))
			Image = vm[0];
#else
		vector<cv::Mat> vm;
		if (-1 != ((checkPatch*)checkPatch::S_dlg.m_pCheck)->CameraDrv::grapImage(id - 1, vm))
			Image = Mat2HObject(vm[0]);
#endif
		else
			return;
		UCI::pDis->m_sIPResult.result_img = Image;

		CountSeconds(&T1);
		DisCamliPoseOnePicture(&Image, &Contours,id);
		CountSeconds(&T2);

		m_sIPResult.time_needed = 1000 * (T2[0].D() - T1[0].D());

		HTuple time = UCI::pDis->m_sIPResult.time_needed;
		CString str;
		str.Format(_T("�����̬�궨 >��ǰ����ʱ��Ϊ: %4.2f ms"), time[0].D());
		AfxMessageBox(str);
	}
}

// ��̬ȡͼ
void ST_MYDISP::DisCamliPoseOnePicture(HalconCpp::HImage * Image, HalconCpp::HXLDCont*  Contours,int prID)
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
		//vector<string> vctFile;
		//getFileNameVec(dir.c_str(),vctFile);
		//int nPic = vctFile.size();//this->GetTotalCariPosePicNum(prID);
		//int nTotal = 1;         // pose ֻҪһ��

		//if (nPic >= nTotal) {                      // ͼƬ���������㣬���Լ��㲢����궨�ڲΡ�		
		//this->DisGetCameraPoseParaFromPic(om,dir,vctFile);   // ȡ�õ�������ⲿ������
		//	//	this->StopContiouscap();
		//	//	Sleep(100);
		//	this->AppStateChangeTo(APP_READY);
		//	return;
		//}

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
		// ����ͼ���еı궨ͼ��
		m_HCali.FindCalibObject(*Image, 0, 0, hv_Index, hv_TmpCtrl_FindCalObjParNames,
			hv_TmpCtrl_FindCalObjParValues);

		// ����һ��ͼ�����ɫ
		this->m_pMHWinMain->SetColor("magenta");
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
			/*+ "���أ��Ѿ��ɼ���" + HTuple(nPic)
			+ "��������" + HTuple(nTotal - nPic) + "��ͼ��"*/, 1);

		double picError = UCI::OPTIONS["�궨�������"];
		if (hv_TmpCtrl_Errors <  picError) {
			// ������̬ͼ��
			//this->GrabImageERROK = *Image;
			write_message(100, 8, "���½�ͼ��ť����ɱ���ͼ���뾡��ѡȡ��ͬ��λ�ã�", 0);

			if (this->PushCaptureDown == 1) {
				this->PushCaptureDown = 0;
				sprintf(sz,"%d",0); 
				string posepicfile = dir+string(sz)+".png";
				if (Write_Image(*Image, posepicfile.c_str()))
				{
					//AfxMessageBox(("�ɹ�����궨���ͼ��:"+posepicfile).c_str());
				}
			}
			else {
				//Sleep(50);
			}
		}
		else {
			write_message(100, 8, "��ǰͼ�����̫������һ�����û�ͷ��", 1);
		}
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
	// ��ʾһ��
	//UCI::pDis->OnMainDisp();
//	PostMessage(UCI::pMain->m_hWnd, UM_UPDATE_RIBBON, (WPARAM)0, (LPARAM)0);
}

// ��ͼƬ��ȡ����̬��������
void ST_MYDISP::DisGetCameraPoseParaFromPic(UCI::OptionsMap *om,string fileAdd,vector<string>& vctFile)
{
//	HTuple hv_TmpCtrl_ReferenceIndex;
	HTuple hv_StartParameters;
	HTuple hv_TmpCtrl_FindCalObjParNames;
	HTuple hv_TmpCtrl_FindCalObjParValues;
	// HTuple hv_CalibHandle;
	//	HTuple hv_CameraParameters;
	HTuple hv_CameraPose;
	HTuple hv_TmpCtrl_Errors;
	//HTuple hv_Index;
	int nPic = vctFile.size();
	int nTotal = 1;        // 

	//this->DisDisplay();                        // ��ʾһ�µ�ǰ��ͼƬ
	if (nPic < nTotal) {                       // ͼƬ����������
		write_message(8, 8, "�궨��̬ͼ������̫С������" + HTuple(nTotal - nPic) + "��ͼ��", 1);
		return;
	}
	try {

//		hv_TmpCtrl_ReferenceIndex = 0;

		// �궨�����ļ�
		CString descFile = UCI::OPTIONS["�궨�����ļ�"];
		HTuple hv_TmpCtrl_PlateDescription = CT2A(descFile).m_psz;

		// �궨��ʼ������� ��ɨ�裬����ʽ
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

		// ȡ�����е��ļ���
		int nTpic = vctFile.size();
		for (int i = 0; i < nTpic; i++) {
			Image.ReadImage((fileAdd+vctFile[i]).c_str());

			//EnterCriticalSection(&this->m_csFGDisp);
			//this->GrabImageDisp = Image;
			//LeaveCriticalSection(&this->m_csFGDisp);
			this->m_pMHWinMain->DispObj(Image);
			//this->DisDisplay();
			write_message(12, 12, "���ڴ�" + HTuple(i) + "�ű궨���ռ����ݡ�����", 1);

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

			// ȡ���������̬
			//m_HCali.GetCalibDataObservPose

			//hv_CameraPose = m_HCali.GetCalibData("calib_obj_pose", (Hlong)HTuple(0).TupleConcat(0), "pose");
			GetCalibData(m_HCali, "calib_obj_pose", HTuple(0).TupleConcat(0),"pose", &hv_CameraPose);
			////Calibration 01: Adjust origin for plate thickness
			double thick = UCI::OPTIONS["�궨����"];
			SetOriginPose(hv_CameraPose, 0.0, 0.0, thick, &hv_CameraPose);
			write_message(600, 600, "�����̬pre��" + hv_CameraPose, 0);
			HalconCpp::WritePose(hv_CameraPose,(fileAdd+"pos.dat").c_str());

			//	write_message(160, 8, "�����������ļ��ɹ���", 1);
			/*hv_pose.SetOriginPose( 0.0, 0.0, thick);
			hv_pose.WritePose((fileAdd+"pos.dat").c_str());	
			write_message(600, 32, "�����̬��" + hv_pose, 0);*/

		}
		else {
			write_message(8, 8, "�ܹ�" + HTuple(nTotal - nPic) + "��ͼ��,"
				+ "�궨��̬ʧ�ܣ��궨����:" + hv_TmpCtrl_Errors, 1);

			write_message(32, 8, "�����²ɼ�ͼ��궨��", 0);
		}

		//Calibration 01: Clear calibration model when done
		//ClearCalibData(m_HCali);
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}

}