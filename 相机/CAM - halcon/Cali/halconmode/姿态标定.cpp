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
		str.Format(_T("外参姿态标定 >当前处理时间为: %4.2f ms"), time[0].D());
		AfxMessageBox(str);
	}
}

// 姿态取图
void ST_MYDISP::DisCamliPoseOnePicture(HalconCpp::HImage * Image, HalconCpp::HXLDCont*  Contours,int prID)
{
	try
	{

		CString str;
		str.Format("相机 %d",prID);
		UCI::OptionsMap *om = UCI::ELE.GetOptionsMapByEleName(*UCI::ELE.EleList,str);
		if (nullptr == om)
		{
			AfxMessageBox("没有相机标定对象");
			return ;
		}	
		// 相机的参数也要更新一下。
		HTuple Width, Height;
		GetImageSize(*Image, &Width, &Height);
		UCI::OPTIONS["图像宽度"] = Width.I();
		UCI::OPTIONS["图像高度"] = Height.I();

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
		//int nTotal = 1;         // pose 只要一张

		//if (nPic >= nTotal) {                      // 图片数量已满足，可以计算并保存标定内参。		
		//this->DisGetCameraPoseParaFromPic(om,dir,vctFile);   // 取得的相机的外部参数。
		//	//	this->StopContiouscap();
		//	//	Sleep(100);
		//	this->AppStateChangeTo(APP_READY);
		//	return;
		//}

		hv_TmpCtrl_ReferenceIndex = 0;

		// 标定描述文件	
		CString descFile = UCI::OPTIONS["标定描述文件"];
		HTuple hv_TmpCtrl_PlateDescription = CT2A(descFile).m_psz;

		// 标定初始相机参数 
		DisSetInnerAndPosStartPara(om,hv_StartParameters,
			hv_TmpCtrl_FindCalObjParNames,
			hv_TmpCtrl_FindCalObjParValues);

		// 建立标定句柄
		HCalibData m_HCali;
		m_HCali.CreateCalibData("calibration_object", 1, 1);
		// 设置标定初始参数
		CString calimodel = UCI::OPTIONS["CamCalibrateModel"];
		m_HCali.SetCalibDataCamParam(0,
			CT2A(calimodel).m_psz,
			(HCamPar)hv_StartParameters);
		// 设置标定描述文件
		m_HCali.SetCalibDataCalibObject(0, hv_TmpCtrl_PlateDescription);

		hv_Index = 0;
		// 查找图像中的标定图标
		m_HCali.FindCalibObject(*Image, 0, 0, hv_Index, hv_TmpCtrl_FindCalObjParNames,
			hv_TmpCtrl_FindCalObjParValues);

		// 设置一下图像的颜色
		this->m_pMHWinMain->SetColor("magenta");
		HPose hv_pose = m_HCali.GetCalibDataObservPose(0, 0, hv_Index);

		// 显示图像
		this->m_pMHWinMain->DispObj(*Image);
		// 显示标定板圆坐标
		this->m_pMHWinMain->DispCaltab(hv_TmpCtrl_PlateDescription,
			(HCamPar)hv_StartParameters, hv_pose, 1.0);

		// 显示标定板外形轮廓
		this->m_pMHWinMain->SetColor("red");
		this->m_pMHWinMain->SetLineWidth(2);

		*Contours = m_HCali.GetCalibDataObservContours("caltab", 0, 0, 0);

		this->m_pMHWinMain->DispObj(*Contours);

		// 计算标定数据
		hv_TmpCtrl_Errors = m_HCali.CalibrateCameras();

		write_message(8, 8, "正在采集标定图像，当前误差:" + hv_TmpCtrl_Errors
			/*+ "像素，已经采集：" + HTuple(nPic)
			+ "个，尚需" + HTuple(nTotal - nPic) + "个图像"*/, 1);

		double picError = UCI::OPTIONS["标定充许误差"];
		if (hv_TmpCtrl_Errors <  picError) {
			// 保存姿态图像
			//this->GrabImageERROK = *Image;
			write_message(100, 8, "按下截图按钮保存可保存图像！请尽量选取不同的位置！", 0);

			if (this->PushCaptureDown == 1) {
				this->PushCaptureDown = 0;
				sprintf(sz,"%d",0); 
				string posepicfile = dir+string(sz)+".png";
				if (Write_Image(*Image, posepicfile.c_str()))
				{
					//AfxMessageBox(("成功保存标定外参图像:"+posepicfile).c_str());
				}
			}
			else {
				//Sleep(50);
			}
		}
		else {
			write_message(100, 8, "当前图像误差太大，请检查一下设置或镜头！", 1);
		}
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
	// 显示一下
	//UCI::pDis->OnMainDisp();
//	PostMessage(UCI::pMain->m_hWnd, UM_UPDATE_RIBBON, (WPARAM)0, (LPARAM)0);
}

// 从图片中取得姿态，并保存
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

	//this->DisDisplay();                        // 显示一下当前的图片
	if (nPic < nTotal) {                       // 图片数量不满足
		write_message(8, 8, "标定姿态图像数量太小，尚需" + HTuple(nTotal - nPic) + "个图像", 1);
		return;
	}
	try {

//		hv_TmpCtrl_ReferenceIndex = 0;

		// 标定描述文件
		CString descFile = UCI::OPTIONS["标定描述文件"];
		HTuple hv_TmpCtrl_PlateDescription = CT2A(descFile).m_psz;

		// 标定初始相机参数 面扫描，多项式
		DisSetInnerAndPosStartPara(om,hv_StartParameters,
			hv_TmpCtrl_FindCalObjParNames,
			hv_TmpCtrl_FindCalObjParValues);

		// 建立标定句柄
		HCalibData m_HCali;
		m_HCali.CreateCalibData("calibration_object", 1, 1);

		// 设置标定初始参数
		CString calimodel = UCI::OPTIONS["CamCalibrateModel"];
		m_HCali.SetCalibDataCamParam(0,
			CT2A(calimodel).m_psz,
			(HCamPar)hv_StartParameters);

		// 设置标定描述文件
		m_HCali.SetCalibDataCalibObject(0, hv_TmpCtrl_PlateDescription);

		HalconCpp::HImage Image;

		// 取得所有的文件名
		int nTpic = vctFile.size();
		for (int i = 0; i < nTpic; i++) {
			Image.ReadImage((fileAdd+vctFile[i]).c_str());

			//EnterCriticalSection(&this->m_csFGDisp);
			//this->GrabImageDisp = Image;
			//LeaveCriticalSection(&this->m_csFGDisp);
			this->m_pMHWinMain->DispObj(Image);
			//this->DisDisplay();
			write_message(12, 12, "正在从" + HTuple(i) + "号标定板收集数据。。。", 1);

			// 查找图像中的标定图标
			m_HCali.FindCalibObject(Image, 0, 0, i, hv_TmpCtrl_FindCalObjParNames,
				hv_TmpCtrl_FindCalObjParValues);
		}

		// 设置一下图像的颜色
		this->m_pMHWinMain->SetColor("green");

		// 取得标定的坐标信息
		HPose hv_pose = m_HCali.GetCalibDataObservPose(0, 0, nTpic - 1);

		// 显示标定板圆坐标
		this->m_pMHWinMain->DispCaltab(hv_TmpCtrl_PlateDescription,
			(HCamPar)hv_StartParameters, hv_pose, 1.0);

		// 取得标定板外形轮廓
		HalconCpp::HXLDCont  Contours
			= m_HCali.GetCalibDataObservContours("caltab", 0, 0, nTpic - 1);

		// 显示标定板外形轮廓
		this->m_pMHWinMain->SetColor("green");
		this->m_pMHWinMain->SetLineWidth(2);
		this->m_pMHWinMain->DispObj(Contours);

		// 计算标定数据
		hv_TmpCtrl_Errors = m_HCali.CalibrateCameras();

		double picError = UCI::OPTIONS["标定充许误差"];
		if (hv_TmpCtrl_Errors < picError) {
			write_message(8, 8, "总共标定了" + HTuple(nTpic) + "个图像,"
				+ "标定成功，标定误差:" + hv_TmpCtrl_Errors, 1);

			// 取得相机的姿态
			//m_HCali.GetCalibDataObservPose

			//hv_CameraPose = m_HCali.GetCalibData("calib_obj_pose", (Hlong)HTuple(0).TupleConcat(0), "pose");
			GetCalibData(m_HCali, "calib_obj_pose", HTuple(0).TupleConcat(0),"pose", &hv_CameraPose);
			////Calibration 01: Adjust origin for plate thickness
			double thick = UCI::OPTIONS["标定板厚度"];
			SetOriginPose(hv_CameraPose, 0.0, 0.0, thick, &hv_CameraPose);
			write_message(600, 600, "相机姿态pre：" + hv_CameraPose, 0);
			HalconCpp::WritePose(hv_CameraPose,(fileAdd+"pos.dat").c_str());

			//	write_message(160, 8, "保存相机外参文件成功了", 1);
			/*hv_pose.SetOriginPose( 0.0, 0.0, thick);
			hv_pose.WritePose((fileAdd+"pos.dat").c_str());	
			write_message(600, 32, "相机姿态：" + hv_pose, 0);*/

		}
		else {
			write_message(8, 8, "总共" + HTuple(nTotal - nPic) + "个图像,"
				+ "标定姿态失败，标定误差超差:" + hv_TmpCtrl_Errors, 1);

			write_message(32, 8, "请重新采集图像标定！", 0);
		}

		//Calibration 01: Clear calibration model when done
		//ClearCalibData(m_HCali);
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}

}