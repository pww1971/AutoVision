#include "stdafx.h"

#include <algorithm>
#include <sstream>

#include "MyData.h"
#include "MyDispLay.h"


using namespace HalconCpp;

using namespace std;
using std::string;

namespace UCI {

	size_t Option::insert_order = 0;
	
	////////////////////////////////////////////////////////////////////////////////////
	/// ��ʼ��ȫ�ֱ�����ȱʡֵ
	void initOptionsMap(OptionsMap& o) {

		Option::insert_order = 0;       // ��Ŀ������ʾ��

		o["��Ŀ���"] << Option("��Ŀ���", 1, 1, 200000,                             // ��ǰ����Ŀ���
			"project ID", false);	

		//o["��ǰ����ID"] << Option("��ǰ����ID", 1, 1, 200000,                         // ��ǰ���������б�����ʾ��ID
		//	"��ǰ����ID", true);

		o["ͼ�����"] << Option("dRadio", 600.0f/800.0f, 0.5f, 2.0f,                            // ͼ�񴰿ڱ���
			"Picture Ration height/width [0.5, 2.0] default 0.75");													

		o["���񱣴��ʽ"] << Option("PictureSaveFormat", "png",                              // ͼ�񱣴��ʽ
			"Picture Save Format",false);

		// �������
		o["GLO_Measure_OK"] << Option("GLO_Measure_OK", 0, 0, 1,                               // ���һ�β������
			"Last Measure Is OK", false);

		o["GLO_Measure_Always_OK"] << Option("GLO_Measure_Always_OK", 0, 0, 1,                 // ���һ�β������
			"Last Measure Always Is OK", false);

		// �ڲα궨ȫ�ֱ���
		o["�궨��Ϣ OK"] << Option("LoadCalibrateOK", 0, 0, 1,                                 // ���ر궨��ϢOK��
			"Load Calibrate Pose and Parameters OK", true);
		//o["ƥ��ģ�� OK"] << Option("Load_ROI_OK", 0, 0, 1,                                     // ����ģ��ƥ����ϢOK��
		//	"����ģ��ƥ����ϢOK��", true);
		
		o["����궨��ʱ"] << Option("iSoftTimeCamCalibrate", 50, 10, 20000,                    // �궨������ʱʱ��
			"In soft trigger mode,Camera Calibrate timer [10,20000] defautl 50ms");

		o["CamCalibratePath"] << Option("CamCalibratePath", "Calibration File",                // ����궨�洢·��
			"Camera Calibrate used file store path",false);

		o["CamCalibrateModel"] << Option("CamCalibrateModel", "area_scan_polynomial",          // ����궨ģ��
			"CamCalibrate Model area_scan_polynomial, ",false);

		o["Զ�ľ�ͷ"] << Option("LensTelecentric", 0, 0, 1,                                   // �Ƿ�Զ�ľ�ͷ
			"is camera lens telecentric: 0: no, 1: yes"	);

		o["��б��ͷ"] << Option("LensSlope", 0, 0, 1,                                         // �Ƿ���б��ͷ
			"is camera lens slope: 0: no, 1: yes");
		o["���ؿ��"] << Option("LensFSx", 2.2f, 1.00f, 200.0f,                                 // ���ؿ��
			"lens pixel x size [1.0, 200.0] um");

		o["���ظ߶�"] << Option("LensFSy", 2.2f, 1.00f, 200.0f,                                 // ���ظ߶�
			"lens pixel y size [1.0, 200.0] um");

		o["��ͷ����"] << Option("LensFocus", 8.0f, 1.00f, 200.0f,                             // ��ͷ����
			"lens Focus [1.0, 200.0] mm");
		o["�궨����"] << Option("CaliPlateThick", 0.001f, 0.000001f, 10.0f,               // �궨����
			"Calibrate Plate Thicnkness [0.000001, 10.0] m");

		//o["ӳ����"] << Option("RectificationWidth", 75.0f, 0.1f, 1000.0f,                      // ӳ����
		//	"RectificationWidth [0.1, 1000.0] mm");

		o["�궨ͼƬ����"] << Option("CaliTotalPicture", 8, 5, 20,                            // ���궨ͼƬ����
			"CaliTotalPicture: [5, 20], default: 8");

		o["�궨�������"] << Option("CaliPicMaxError", 0.55f, 0.01f, 10.0f,                   // �궨�������
			"CaliPicMaxError [0.01, 10.0] pix default: 0.55");

		o["�궨��ǰ���"] << Option("CaliCurrentError", 0.0f, 0.001f, 10.0f,                  // �궨��ǰ���
			"CaliPic Current Error [0.01, 10.0] pix default: 0.55");

		o["�궨�����������"] << Option("�궨�����������", 1.0f, 0.1f, 2.0f,                  
			"�궨gap_tolerance default 1.0f min 0.1 max 2.0");
		
		o["�궨ƽ��alpha"] << Option("�궨ƽ��alpha", 0.50f, 0.1f, 2.0f,                  
			"�궨alpha default 1.0f min 0.1 max 2.0");
		
		o["�궨����־��ֱ��"] << Option("�궨����־��ֱ��", 350.0f, 1.0f, 500.0f,                  
			"�궨����־��ֱ�� default 1.0f min 1.0 max 500.0");

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		/// �궨�ļ�������
		o["ͼ����"] << Option("ORGwidth", 2592, 200, INT_MAX,
			"width [100,2592] pixel");                                                           // "ͼ����"
		o["ͼ��߶�"] << Option("ORGheight", 2073, 200, INT_MAX,
			"height [100,2073] pixel");

		o["�ߴ��ûҶ�"] << Option("�ߴ��ûҶ�", 250, 0, 255,
			"�����ߴ�ʱ�����ͼ��ʱʹ�õĻҶ�");
		o["ORGwhByCamera"] << Option("ORGwhByCamera", 0, 0, 1,
			"ORG width and height is get from Camera ", false);

		o["CaliFileSaveName"] << Option("CaliFileSaveName", "calitabsave",                       // ����궨�����ļ�
			"CamCalibrate save file name ",false);

		o["�궨�����ļ�"] << Option("CaliDescripteName", "caltab_30mm.descr",                   // ����궨�����ļ�
			"CamCalibrate describe file name ");

		o["�������س���"] << Option("OnePixelDis", 0.013f, 0.00000001f, 1.0f,                     // ���ñ궨���ͼ��һ�����ش����ʵ�ʾ��루�ף�
			"Cali One Pixel Distane [0.00000001, 1.0] m default: 0.00013");

		o["�궨ԭ�� X ����"] << Option("CaliOriginPoseX", -0.5f, -2.0f, 2.0f,                // ת��ԭ�����꣬��������£��궨������Ϊԭ�㣬�ѱ궨�����������½��ƶ� 
			"Cali One CaliOriginPose X to Width [FLT_MIN, FLT_MAX] m default: -0.04");

		o["�궨ԭ�� Y ����"] << Option("CaliOriginPoseY", -0.375f, -2.0f, 2.0f,                 // ת��ԭ�����꣬��������£��궨������Ϊԭ�㣬�ѱ궨�����������½��ƶ� 
			"Cali One CaliOriginPose Y of Height [FLT_MIN, FLT_MAX] m default: -0.03");

		o["�궨ԭ�� Z ����"] << Option("CaliOriginPoseZ",  0.00f, -100.9f, 100.1f,                  // ת��ԭ�����꣬��������£��궨������Ϊԭ�㣬�ѱ궨�����������½��ƶ� 
			"Cali One CaliOriginPose Z [FLT_MIN, FLT_MAX] m default: -0.03");

		
		o["�궨 X �� �Ƕ�"] << Option("CalibRotateAngle X", 0.0f, -360.0f, 360.0f,            // insert(poseCalib, poseCalib[5]-90, 5, poseCalibRot) 
			"Calib Rotate Angle X [-180.0, 180.0] degree default: 0.0");                          // http://112.124.6.111/read-6712.html

		o["�궨 Y �� �Ƕ�"] << Option("CalibRotateAngle Y", 0.0f, -360.0f, 360.0f,           // insert(poseCalib, poseCalib[5]-90, 5, poseCalibRot) 
			"Calib Rotate Angle Y [-180.0, 180.0] degree default: 0.0");                          // http://112.124.6.111/read-6712.html

		o["�궨 Z �� �Ƕ�"] << Option("CalibRotateAngle Z", 180.0f, -360.0f, 360.0f,          // insert(poseCalib, poseCalib[5]-90, 5, poseCalibRot) 
			"Calib Rotate Angle Z [-180.0, 180.0] degree default: 0.0");                          // http://112.124.6.111/read-6712.html

		o["ӳ����"] << Option("RectificationWidth", 40.0f, 5.0f, FLT_MAX,                    // insert(poseCalib, poseCalib[5]-90, 5, poseCalibRot) 
			"RectificationWidth [0.01F, FLT_MAX] degree default: 40.0");
		o["�������س���mm"] << Option("OnePixelScalemm", 0.030f, 0.0001f, FLT_MAX,               // ����һ�����ش���ĳߴ�
			"OnePixel Scale mm [0.0001, FLT_MAX] degree default: 0.030");

		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// ��������
		o["TestUseStorePicture"] << Option("TestUseStorePicture", 0, 0, 1,                                                         
			"Test Use prestored Picture", true);

		o["TestUseStorePicName"] << Option("TestUseStorePicName", "roimatch.png",                                                 
			"Test Use prestored Picture Name ", true);

		o["ElementDoubleClickColor"] << Option("ElementDoubleClickColor", "magenta",                                              
			"Suggested values: black, #ffb529 ", true);

		o["ElementDoubleClickLineWidth"] << Option("ElementDoubleClickLineWidth", 3, 1, 2000,                                      
			"Restriction: (Width >= 1) && (Width <= 2000) ", true);

		o["ElementDoubleClickDistanceColor"] << Option("ElementDoubleClickDistanceColor", "red",                                  
			"Suggested values: black, #ffb529 ", true);

		o["ElementDoubleClickDistancWidth"] << Option("ElementDoubleClickDistancWidth", 3, 1, 2000,                                
			"Restriction: (Width >= 1) && (Width <= 2000) ", true);

		o["ElementDoubleClickResultColor"] << Option("ElementDoubleClickResultColor", "medium slate blue",                        
			"Suggested values: black,  #ffb529 ", true);

		o["ElementDoubleClickResultWidth"] << Option("ElementDoubleClickResultWidth", 3, 1, 2000,                                  
			"Restriction: (Width >= 1) && (Width <= 2000) ", true);

		o["ElementDimColor"] << Option("ElementDimColor", "white",                                             
			"Suggested values: #ffb529 ", true);

		o["ElementDimWidth"] << Option("ElementDimWidth", 1, 1, 2000,                                           
			"Restriction: (Width >= 1) && (Width <= 2000) ", true);

		o["ElementHDLXColor"] << Option("ElementHDLXColor", "green",                                           
			"Suggested values:black ffb529", true);

		o["ElementHDLXWidth"] << Option("ElementHDLXWidth", 1, 1, 2000,                                         
			"Restriction: (Width >= 1) && (Width <= 2000) ", true);

		o["ElementArrowSize"] << Option("ElementArrowSize",5.0f, 1.0f, 2000.0f,                                 
			"Element Arrow Size display ", true);

		o["ProdutctBatchNumber"] << Option("ProdutctBatchNumber", "20170816",                                     // ��Ʒ����
			"ProdutctBatchNumber", true);

		o["ProdutctExportStartNo"] << Option("ProdutctExportStartNo", 1, 1, INT_MAX,                             // ������EXCEL�Ĳ�Ʒ��ʼ��� 
			"Produtct Export Start Number", true);

		o["ProdutctOKAutoSave"] << Option("ProdutctOKAutoSave", 1, 0, 1,                                       // ������EXCEL�Ĳ�Ʒ��ʼ��� 
			"Produtct OK AutoSave", true);


		o["��ʾԭͼ"] << Option("��ʾԭͼ", 0, 0, 1,                                                            // 
			"�������ǲ�����ʾ�ɼ���ԭͼ",
			true);

		o["�����ɼ�Ч��ʹ��"] << Option("�����ɼ�Ч��ʹ��", 1, 0, 1,                                            // 
			"�����ɼ�ͼ��, Ӧ�����Ч�����Ӵ��������ʾͼ��",
			true);

		o["Dummy"] << Option("Dummy", "Dummy",                                                                  // ʹ�ñ����ͼƬ���ļ���
			"Dummy why not show ", false);
		
	}	
		
	//  ֱ�� 
	void stElement::initOptionMapElemnetLine(OptionsMap& o)
	{
		
		// ���������
		o["ROI_Row1"] << Option("ROI_Row1", 0.0f, -FLT_MAX, FLT_MAX,                                  // Ԫ�ص�Rows����
			"ROI_Row1", false);
		o["ROI_Column1"] << Option("ROI_Column1", 0.0f, -FLT_MAX, FLT_MAX,                                  // Ԫ�ص�Rows����
			"ROI_Column1", false);
		o["ROI_Row2"] << Option("ROI_Row2", 0.0f, -FLT_MAX, FLT_MAX,                                  // Ԫ�ص�Rows����
			"ROI_Row2", false);
		o["ROI_Column2"] << Option("ROI_Column2", 0.0f, -FLT_MAX, FLT_MAX,                                  // Ԫ�ص�Rows����
			"ROI_Column2", false);

			
		
		o["Rakeֱ�߼�����"] << Option("Rake_Line_Elements", 10, 1, 1000,                                              // CreateShapeModel double AngleStart
		    "Rake Line Elements number [1 20] default 10 ", true);

		o["Rakeֱ�߼��߶�"] << Option("Rake_Line_Detect_Height", 30.0f, 1.0f, 500.0f,                             // CreateShapeModel double AngleStart
			"Rake_Line_Detect_Height [1.0f, 500.0f] default 30.0 ", true);

		o["Rakeֱ�߼����"] << Option("Rake_Line_Detect_Width", 15.0f, 1.0f, 500.0f,                                // CreateShapeModel double AngleStart
			"Rake_Line_Detect_Height [1.0f, 500.0f] default 15.0 ", true);

		o["Rakeֱ�߼��Sigma"] << Option("Rake_Line_Detect_Sigma", 1.0f, 0.0f, 500.0f,                                // CreateShapeModel double AngleStart
			"Rake_Line_Detect_Sigma [0.0f, 500.0f] default 1.0 ", true);

		o["Rakeֱ�߼����ֵ"] << Option("Rake_Line_Detect_Threshold", 20.0f, 1.0f, 500.0f,                        // CreateShapeModel double AngleStart
			"Rake_Line_Detect_Threshold [1.0f, 500.0f] default 20.0 ", true);

		o["Rake_Line_Detect_Transition"] << Option("Rake_Line_Detect_Transition", "all",                                    // Ԫ�ص������
			"Rake_Line_Detect_Transition", true);

		o["Rake_Line_Detect_Select"] << Option("Rake_Line_Detect_Select", "first",                                           // Ԫ�ص������
			"Rake_Line_Detect_Select", true);

		o["ֱ�������С����"] << Option("pts_to_best_line_ActiveNum", 2, 2, 1000,                                 // CreateShapeModel double AngleStart
			"pts_to_best_line_ActiveNum [2 1000] default 2 ", true);

		// �洢ֱ�ߵĲ�����Ϣ
		o["_RES_Line_Row1"] << Option("RES_Line_Row1", 0.0f, -FLT_MAX, FLT_MAX,                                               // CreateShapeModel double AngleStart
			"RES_Line_Row1 [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o["_RES_Line_Row2"] << Option("RES_Line_Row2", 0.0f, -FLT_MAX, FLT_MAX,                                               // CreateShapeModel double AngleStart
			"RES_Line_Row2 [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o["_RES_Line_Col1"] << Option("RES_Line_Col1", 0.0f, -FLT_MAX, FLT_MAX,                                                // CreateShapeModel double AngleStart
			"RES_Line_Col1 [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o["_RES_Line_Col2"] << Option("RES_Line_Col2", 0.0f, -FLT_MAX, FLT_MAX,                                               // CreateShapeModel double AngleStart
			"RES_Line_Col2 [FLT_MIN, FLT_MAX] default 0.0 ", true);
	}

	//  Բ
	void stElement::initOptionMapElemnetCircle(OptionsMap&  o)
	{
		
		// ROI ����洢��Ϣ

		o["HTupleArcOrCircle"] << Option("HTupleArcOrCircle", 0,0,1,                            // ��Բ�����ǻ� 0: 
			"HTupleArcOrCircle ��Բ�����ǻ� 0: Բ, 1: ��", true);

		o["ROI_RowC"] << Option("ROI_RowC", 0.0f, -FLT_MAX, FLT_MAX,                              // Բ������ Y
			"hv_RowC [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o["ROI_ColumnC"] << Option("ROI_ColumnC", 0.0f, -FLT_MAX, FLT_MAX,                        // 
			"hv_ColumnC [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o["ROI_Radius"] << Option("ROI_Radius", 0.0f, -FLT_MAX, FLT_MAX,                           //
			"hv_Radius [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o["ROI_StartPhi"] << Option("ROI_StartPhi", 0.0f, -FLT_MAX, FLT_MAX,                        //
			"hv_StartPhi [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o["ROI_EndPhi"] << Option("ROI_EndPhi", 0.0f, -FLT_MAX, FLT_MAX,                           //
			"hv_EndPhi [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o["ROI_PointOrder"] << Option("ROI_PointOrder", "positive",                                       // 
			"hv_PointOrder [FLT_MIN, FLT_MAX] default positive", true);

		
		o["Spoke_ʮ�ֳߴ�"] << Option("Spoke_ʮ�ֳߴ�", 10, 1, 1000,                                             // CreateShapeModel double AngleStart
			"Spoke_ʮ�ֳߴ� ��ʾ��С", true);

		// ���������Ϣ		
		o["Spoke_Circle_Direction"] << Option("Spoke_Circle_Direction", "inner",                                             // Ԫ�صķ���
			"Direction of ROI default inner, inner, outer", true);

		o["Spoke_Circle_Elements"] << Option("Spoke_Circle_Elements", 30, 3, 1000,                                             // CreateShapeModel double AngleStart
			"Spoke Circle Elements number [1 20] default 10", true);

		o["Spoke_Circle_Detect_Height"] << Option("Spoke_Circle_Detect_Height", 50.0f, 1.0f, 500.0f,                            // CreateShapeModel double AngleStart
			"Spoke Circle Elements number [1.0f, 500.0f] default 30.0 ", true);

		o["Spoke_Circle_Detect_Width"] << Option("Spoke_Circle_Detect_Width", 35.0f, 1.0f, 500.0f,                             // CreateShapeModel double AngleStart
			"Spoke Circle Elements number [1.0f, 500.0f] default 15.0 ", true);

		o["Spoke_Circle_Detect_Sigma"] << Option("Spoke_Circle_Detect_Sigma", 1.0f, 0.0f, 500.0f,                                // CreateShapeModel double AngleStart
			"Spoke_Circle_Detect_Sigma [0.0f, 500.0f] default 1.0 ", true);

		o["Spoke_Circle_Detect_Threshold"] << Option("Spoke_Circle_Detect_Threshold", 20.0f, 1.0f, 500.0f,                        // CreateShapeModel double AngleStart
			"Spoke_Circle_Detect_Threshold [1.0f, 500.0f] default 20.0 ", true);

		o["Spoke_Circle_Detect_Transition"] << Option("Spoke_Circle_Detect_Transition", "all",                                    
			"Spoke_Circle_Detect_Transition", true);

		o["Spoke_Circle_Detect_Select"] << Option("Spoke_Circle_Detect_Select", "first",                                          
			"Spoke_Circle_Detect_Select", true);

		o["Circle_Fit_Min_Points_Num"] << Option("Circle_Fit_Min_Points_Num", 10, 5, 1000,                                         // ���Բ���ٵ���
			"Circle_Fit_Min_Points_Num [2 1000] default 5 ", true);


		// �洢Բ�Ĳ�����Ϣ
		o["RES_Բ�뾶"] << Option("RES_Բ�뾶", 0.0f, -FLT_MAX, FLT_MAX,                                                         // Բ�뾶
			"RES_Circle_Radius [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o["RES_Բֱ��"] << Option("RES_Բֱ��", 0.0f, -FLT_MAX, FLT_MAX,                                                         // Բ�뾶
			"RES_Circle_Diameter [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o["RES_Circle_Center_Row"] << Option("RES_Circle_Center_Row", 0.0f, -FLT_MAX, FLT_MAX,                                   // Բ������ X
			"RES_Circle_Row [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o["RES_Circle_Center_Column"] << Option("RES_Circle_Center_Column", 0.0f, -FLT_MAX, FLT_MAX,                             // Բ������ Y
			"RES_Circle_Column [FLT_MIN, FLT_MAX] default 0.0 ", true);
		

		o["RES_Circle_StartPhi"] << Option("RES_Circle_StartPhi", 0.0f, -FLT_MAX, FLT_MAX,                                        // Բ����ʼ��
			"RES_Circle_StartPhi [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o["RES_Circle_EndPhi"] << Option("RES_Circle_EndPhi", 0.0f, -FLT_MAX, FLT_MAX,                                            // Բ�Ľ�����
			"RES_Circle_EndPhi [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o["RES_Circle_Type"] << Option("RES_Circle_Type", "",                                                                    // Բ�����ͣ�Բ����
			"RES_Circle_Type: Circle, Arc ", true);

		o["RES_Circle_PointOrders"] << Option("RES_Circle_PointOrders", "",                                                      // Ԫ�ص�Rows����
			"RES_Circle_PointOrders: negative, positive ", true);
	}
	
	// ���
	void stElement::initOptionMapElemnetResult(OptionsMap& o)
	{
		o["���Ԫ������"] << Option("OUT_Elemnt_Name", "",                                           //  Ԫ�ص�����
			"���Ԫ������", true);
		
		o["���Ԫ�صĲ���"] << Option("OUT_Elemnt_Parament_Name", "",                                 // Ԫ�صĲ�����
			"���Ԫ�صĲ���", true);

		o["����Ĳ���ֵ"] << Option(" OUT_Elemnt_Data", 0.0f, -FLT_MAX, FLT_MAX,                      // ����ֵ
			"OUT_Elemnt_Parament_Data", true);

		o["����ı�׼ֵ"] << Option(" OUT_Elemnt_Standard", 0.0f, -FLT_MAX, FLT_MAX,                      // ��׼ֵ
			"OUT_Elemnt_Parament_Standard", true);

		o["������Ϲ���"] << Option("OUT_Elemnt_Upper_Tol", 0.5f, -FLT_MAX, FLT_MAX,                     // ����
			"OUT_Elemnt_Upper_Tolerance", true);

		o["������¹���"] << Option("OUT_Elemnt_Lower_Tol", -0.5f, -FLT_MAX, FLT_MAX,                     // ����
			"OUT_Elemnt_Lower_Tolerance", true);

		o["�����������"] << Option("OUT_Elemnt_Result", "",                                        // ����, OK, �³��ޣ��ϳ���
			"OUT_Elemnt_Result: OK, Lower, Upper", true);

		o["�����"] << Option("OUT_Elemnt_Is_Alarm", 0, 0, 1,                                      // �����ǲ��Ǳ���
			"�����", true);
	}

	// �Ƕ�
	void stElement::initOptionMapElemnetAngle(OptionsMap & o)
	{
		
		o["USE_LINE1_Parament_Name"] << Option("USE_LINE1_Parament_Name", "",                                 // ʹ��ֱ��1����
			"USE_LINE1_Parament_Name", true);

		//o["USE_LINE1_Indentify"] << Option("USE_LINE1_Indentify", 0, 0, INT_MAX,                              // ʹ��ֱ��1 ID
		//	"USE_LINE1_Indentify", true);

		o["USE_LINE2_Parament_Name"] << Option("USE_LINE2_Parament_Name", "",                                 // ʹ��ֱ��2����
			"USE_LINE2_Parament_Name", true);

		//o["USE_LINE2_Indentify"] << Option("USE_LINE2_Indentify", 0, 0, INT_MAX,                              // ʹ��ֱ��2 ID
		//	"USE_LINE2_Indentify", true);

		o["RES_2LINE_Angle_Rad"] << Option("RES_2LINE_Angle_Rad", 0.0f, -FLT_MAX, FLT_MAX,                     // ����ֱ�ߵĽǶ�
			"RES_2LINE_Angle_Rad [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o["RES_�н�"] << Option("RES_�н�", 0.0f, -FLT_MAX, FLT_MAX,                                           // ����ֱ�ߵĽǶ�
			"RES_2LINE_Angle_Deg [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o["RES_Intersection_Row"] << Option("RES_Intersection_Row", 0.0f, -FLT_MAX, FLT_MAX,                   // ����ֱ�ߵĽ���� ROW
			"RES_Intersection_Row ", true);

		o["RES_Intersection_Column"] << Option("RES_Intersection_Column", 0.0f, -FLT_MAX, FLT_MAX,             // ����ֱ�ߵĽ���� Column
			"RES_Intersection_Column ", true);

		o["RES_Intersection_Points"] << Option("RES_Intersection_Points", "",                                 // ����ֱ�ߵĽ���� Points
			"RES_Intersection_Points", true);
	}

	// ���
	void stElement::initOptionMapElemnetCamera(OptionsMap & o)
	{
		(o)["��ע"] << Option("��ע", "Camera Parameter group description",     // ���������˵��
			"Camera Parameter group description");

		// �ڲα궨ȫ�ֱ���
		o["�궨��Ϣ OK"] << Option("LoadCalibrateOK", 0, 0, 1,                                 // ���ر궨��ϢOK��
			"Load Calibrate Pose and Parameters OK", true);

		o["����궨��ʱ"] << Option("iSoftTimeCamCalibrate", 50, 10, 20000,                    // �궨������ʱʱ��
			"In soft trigger mode,Camera Calibrate timer [10,20000] defautl 50ms");

		o["CamCalibratePath"] << Option("CamCalibratePath", "Calibration File",                // ����궨�洢·��
			"Camera Calibrate used file store path",false);

		o["CamCalibrateModel"] << Option("CamCalibrateModel", "area_scan_polynomial",          // ����궨ģ��
			"CamCalibrate Model area_scan_polynomial, ",false);

		o["Զ�ľ�ͷ"] << Option("LensTelecentric", 0, 0, 1,                                   // �Ƿ�Զ�ľ�ͷ
			"is camera lens telecentric: 0: no, 1: yes"	);

		o["��б��ͷ"] << Option("LensSlope", 0, 0, 1,                                         // �Ƿ���б��ͷ
			"is camera lens slope: 0: no, 1: yes");
		o["���ؿ��"] << Option("LensFSx", 2.2f, 1.00f, 200.0f,                                 // ���ؿ��
			"lens pixel x size [1.0, 200.0] um");

		o["���ظ߶�"] << Option("LensFSy", 2.2f, 1.00f, 200.0f,                                 // ���ظ߶�
			"lens pixel y size [1.0, 200.0] um");

		o["��ͷ����"] << Option("LensFocus", 8.0f, 1.00f, 200.0f,                             // ��ͷ����
			"lens Focus [1.0, 200.0] mm");
		o["�궨����"] << Option("CaliPlateThick", 0.001f, 0.000001f, 10.0f,               // �궨����
			"Calibrate Plate Thicnkness [0.000001, 10.0] m");

		//o["ӳ����"] << Option("RectificationWidth", 75.0f, 0.1f, 1000.0f,                      // ӳ����
		//	"RectificationWidth [0.1, 1000.0] mm");

		o["�궨ͼƬ����"] << Option("CaliTotalPicture", 8, 5, 20,                            // ���궨ͼƬ����
			"CaliTotalPicture: [5, 20], default: 8");

		o["�궨�������"] << Option("CaliPicMaxError", 0.55f, 0.01f, 10.0f,                   // �궨�������
			"CaliPicMaxError [0.01, 10.0] pix default: 0.55");

		o["�궨��ǰ���"] << Option("CaliCurrentError", 0.0f, 0.001f, 10.0f,                  // �궨��ǰ���
			"CaliPic Current Error [0.01, 10.0] pix default: 0.55");

		o["�궨�����������"] << Option("�궨�����������", 1.0f, 0.1f, 2.0f,                  
			"�궨gap_tolerance default 1.0f min 0.1 max 2.0");

		o["�궨ƽ��alpha"] << Option("�궨ƽ��alpha", 0.50f, 0.1f, 2.0f,                  
			"�궨alpha default 1.0f min 0.1 max 2.0");

		o["�궨����־��ֱ��"] << Option("�궨����־��ֱ��", 350.0f, 1.0f, 500.0f,                  
			"�궨����־��ֱ�� default 1.0f min 1.0 max 500.0");

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		/// �궨�ļ�������
		o["ͼ����"] << Option("ORGwidth", 2592, 200, INT_MAX,
			"width [100,2592] pixel");                                                           // "ͼ����"
		o["ͼ��߶�"] << Option("ORGheight", 2073, 200, INT_MAX,
			"height [100,2073] pixel");

		o["�ߴ��ûҶ�"] << Option("�ߴ��ûҶ�", 250, 0, 255,
			"�����ߴ�ʱ�����ͼ��ʱʹ�õĻҶ�");
		o["ORGwhByCamera"] << Option("ORGwhByCamera", 0, 0, 1,
			"ORG width and height is get from Camera ", false);

		o["CaliFileSaveName"] << Option("CaliFileSaveName", "calitabsave",                       // ����궨�����ļ�
			"CamCalibrate save file name ",false);

		o["�궨�����ļ�"] << Option("CaliDescripteName", "caltab_30mm.descr",                   // ����궨�����ļ�
			"CamCalibrate describe file name ");

		o["�������س���"] << Option("OnePixelDis", 0.013f, 0.00000001f, 1.0f,                     // ���ñ궨���ͼ��һ�����ش����ʵ�ʾ��루�ף�
			"Cali One Pixel Distane [0.00000001, 1.0] m default: 0.00013");

		o["�궨ԭ�� X ����"] << Option("CaliOriginPoseX", -0.5f, -2.0f, 2.0f,                // ת��ԭ�����꣬��������£��궨������Ϊԭ�㣬�ѱ궨�����������½��ƶ� 
			"Cali One CaliOriginPose X to Width [FLT_MIN, FLT_MAX] m default: -0.04");

		o["�궨ԭ�� Y ����"] << Option("CaliOriginPoseY", -0.375f, -2.0f, 2.0f,                 // ת��ԭ�����꣬��������£��궨������Ϊԭ�㣬�ѱ궨�����������½��ƶ� 
			"Cali One CaliOriginPose Y of Height [FLT_MIN, FLT_MAX] m default: -0.03");

		o["�궨ԭ�� Z ����"] << Option("CaliOriginPoseZ",  0.00f, -100.9f, 100.1f,                  // ת��ԭ�����꣬��������£��궨������Ϊԭ�㣬�ѱ궨�����������½��ƶ� 
			"Cali One CaliOriginPose Z [FLT_MIN, FLT_MAX] m default: -0.03");


		o["�궨 X �� �Ƕ�"] << Option("CalibRotateAngle X", 0.0f, -360.0f, 360.0f,            // insert(poseCalib, poseCalib[5]-90, 5, poseCalibRot) 
			"Calib Rotate Angle X [-180.0, 180.0] degree default: 0.0");                          // http://112.124.6.111/read-6712.html

		o["�궨 Y �� �Ƕ�"] << Option("CalibRotateAngle Y", 0.0f, -360.0f, 360.0f,           // insert(poseCalib, poseCalib[5]-90, 5, poseCalibRot) 
			"Calib Rotate Angle Y [-180.0, 180.0] degree default: 0.0");                          // http://112.124.6.111/read-6712.html

		o["�궨 Z �� �Ƕ�"] << Option("CalibRotateAngle Z", 180.0f, -360.0f, 360.0f,          // insert(poseCalib, poseCalib[5]-90, 5, poseCalibRot) 
			"Calib Rotate Angle Z [-180.0, 180.0] degree default: 0.0");                          // http://112.124.6.111/read-6712.html

		o["ӳ����"] << Option("RectificationWidth", 40.0f, 5.0f, FLT_MAX,                    // insert(poseCalib, poseCalib[5]-90, 5, poseCalibRot) 
			"RectificationWidth [0.01F, FLT_MAX] degree default: 40.0");
		o["�������س���mm"] << Option("OnePixelScalemm", 0.030f, 0.0001f, FLT_MAX,               // ����һ�����ش���ĳߴ�
			"OnePixel Scale mm [0.0001, FLT_MAX] degree default: 0.030");

	}
	// ����
	void stElement::initOptionMapElemnetDistance(OptionsMap & o)
	{
		
		//o["IsDisplayDimension"] << Option("IsDisplayDimension", 1, 0, 1,                                             // �Ƿ���ʾ�ߴ���ֵ
		//	"IsDisplayDimension", false);

		
		o["����Ԫ������1"] << Option("USE_Distance1_Element_Name", "",                                     // ʹ��ֱ��1����
			"����Ԫ������1", true);

		o["����Ԫ��1������"] << Option(" USE_Distance1_Parament_Name", "",                                 // ʹ��ֱ��1����
			"����Ԫ��1������", true);

		//o["����Ԫ��1 ID"] << Option(" USE_Distance1_Indentify", 0, 0, INT_MAX,                              // ʹ��ֱ��1 ID
		//	"����Ԫ��1 ID", true);

		o["����Ԫ������2"] << Option("USE_Distance2_Element_Name", "",                                     // ʹ��ֱ��2����
			"����Ԫ������2", true);
		
		o["����Ԫ��2������"] << Option("USE_Distance2_Parament_Name","",                                   // ʹ��ֱ��2����
			"����Ԫ��2������", true);

		//o["����Ԫ��2 ID"] << Option("USE_Distance2_Indentify", 0, 0, INT_MAX,                               // ʹ��ֱ��2 ID
		//	"����Ԫ��2 ID", true);

		// �������Ƕ�����
		o["����Row1"] << Option("����Row1", 0.0f, -FLT_MAX, FLT_MAX,                        // 
			"����Row1", false);
		o["����Col1"] << Option("����Col1", 0.0f, -FLT_MAX, FLT_MAX,                        // 
			"����Col1", false);

		o["����Row2"] << Option("����Row2", 0.0f, -FLT_MAX, FLT_MAX,                        // 
			"����Row1", false);
		o["����Col2"] << Option("����Col2", 0.0f, -FLT_MAX, FLT_MAX,                        // 
			"����Col2", false);

		o["�ߴ���ʾƫ��X"] << Option("�ߴ���ʾƫ��X", 0.0f, -FLT_MAX, FLT_MAX,                        // 
			"�ߴ���ʾƫ��X", true);
		o["�ߴ���ʾƫ��Y"] << Option("�ߴ���ʾƫ��Y", 0.0f, -FLT_MAX, FLT_MAX,                        // 
			"�ߴ���ʾƫ��Y", true);


		o["�ߴ��ͷ����"] << Option("�ߴ��ͷ����", 20.0f, 2.0f, FLT_MAX,                                      // gen_arrow_contour_xld
			"Arrow_HeadLength Default Value: 5 ����ֵ: [2,3,5,10,20]", true);

		o["�ߴ��ͷ���"] << Option("�ߴ��ͷ���", 20.0f, 2.0f, FLT_MAX,                                      // gen_arrow_contour_xld
			"The width of the arrow head in pixels Default Value: 5 ����ֵ: [2,3,5,10,20]", true);

		o["RES_����"] << Option("RES_Distance", 0.0f, -FLT_MAX, FLT_MAX,                                            // ����ֱ�ߵľ���
			"RES_Distance [FLT_MIN, FLT_MAX] default 0.0 ", true);

	

	}

	// ����
	void stElement::initOptionMapFUNCTION(OptionsMap & o)
	{		

		//==========================================
		
		
		//==========================================
		o["USE_mean_image"] << Option("USE_mean_image", 1, 0, 1,                                              // FindShapeModleMinScore
			"Smooth by averaging. ");

		(o)["mean_image_MaskWidth"] << Option("mean_image_MaskWidth", 9, 1, 501,                           // 
			"Width of filter mask. Default value: 9 Suggested values: 3, 5, 7, 9, 11, 15, 23, 31, 43, 61, 101",
			true);

		(o)["mean_image_MaskHeight"] << Option("mean_image_MaskHeight", 9, 1, 501,                           // 
			"Height of filter mask. Default value: 9 Suggested values: 3, 5, 7, 9, 11, 15, 23, 31, 43, 61, 101",
			true);


		//==========================================
		//o["USE_scale_image"] << Option("USE_scale_image", 0, 0, 1,                                          // FindShapeModleMinScore
		//	"ʹ��ͼ��Ҷ����칦�� Scale the gray values of an image scale_image(Image : ImageScaled : Mult, Add : ) g1 := g * Mult + Add");

		//(o)["scale_image_Mult"] << Option("scale_image_Mult", 0.01f, 0.001f, 1.0f,                           // 
		//	"Scale factor. Default value: 0.01 Suggested values: 0.001, 0.003, 0.005, 0.008, 0.01, 0.02, 0.03, 0.05, 0.08, 0.1, 0.5, 1.0",
		//	true);

		//(o)["scale_image_Add"] << Option("scale_image_Add", 50.0f, 0.01f, 500.0f,                           // 
		//	"scale_image_Add. Default value: 0 Suggested values: 0, 10, 50, 100, 200, 500",
		//	true);
		//==========================================
		o["USE_equ_histo_image"] << Option("USE_equ_histo_image", 1, 0, 1,                                              // FindShapeModleMinScore
			"Histogram linearisation of images The operator equ_histo_image primarily serves for optical processing of images for a human viewer. For example, the (local) contrast spreading can lead to a detection of fictitious edges.");
		

		//==========================================
		o["USE_emphasize"] << Option("USE_emphasize", 1, 0, 1,                                              // FindShapeModleMinScore
			"ʹ��ͼ����ǿ���� Enhance contrast of the image. emphasize(Image : ImageEmphasize : MaskWidth, MaskHeight, Factor : ) res := round((orig - mean) * Factor) + orig");

		(o)["emphasize_MaskWidth"] << Option("emphasize_MaskWidth", 7, 3, 201,                           // 
			"Width of low pass mask.efault value: 7 Suggested values: 3, 5, 7, 9, 11, 15, 21, 25, 31, 39  Typical range of values: 3 �� MaskWidth �� 201 Minimum increment: 2",
			true);
		
		(o)["emphasize_MaskHeight"] << Option("emphasize_MaskHeight", 7, 3, 201,                           // 
			"Width of low pass mask.efault value: 7 Suggested values: 3, 5, 7, 9, 11, 15, 21, 25, 31, 39  Typical range of values: 3 �� MaskHeight �� 201 Minimum increment: 2",
			true);

		(o)["emphasize_Factor"] << Option("emphasize_Factor", 1.0f, 0.01f, 20.0f,                           // 
			"Intensity of contrast emphasis. Default value: 1.0 Suggested values: 0.3, 0.5, 0.7, 1.0, 1.4, 1.8, 2.0 Minimum increment: 0.01",
			true);

		


		//==========================================
		o["USE_threshold"] << Option("USE_threshold", 0, 0, 1,                                              // FindShapeModleMinScore
			"USE_threshold Segment an image using global threshold. threshold(Image : Region : MinGray, MaxGray : ) ");

		(o)["threshold_MinGray"] << Option("threshold_MinGray", 180.0f, 0.00f, 255.0f,                           // 
			"Lower threshold for the gray values. Default value: 128.0 Suggested values: 0.0, 10.0, 30.0, 64.0, 128.0, 200.0, 220.0, 255.0",
			true);

		(o)["threshold_MaxGray"] << Option("threshold_MaxGray", 255.0f, 0.00f, 255.0f,                           // 
			"Lower threshold for the gray values. Default value: 128.0 Suggested values: 0.0, 10.0, 30.0, 64.0, 128.0, 200.0, 220.0, 255.0",
			true);

		
		o["USE_dilation_circle"] << Option("USE_dilation_circle", 0, 0, 1,                                              // FindShapeModleMinScore
			"USE_dilation_circle");
		(o)["dilation_circle_radius"] << Option("dilation_circle_radius", 3.5f, 1.50f, 511.0f,                           // 
			"Radius of the circular structuring element.. efault value: 3.5 uggested values: 1.5, 2.5, 3.5, 4.5, 5.5, 7.5, 9.5, 12.5, 15.5, 19.5, 25.5, 33.5, 45.5, 60.5, 110.5",
			true);

		o["USE_fill_up"] << Option("USE_fill_up", 0, 0, 1,                                              // FindShapeModleMinScore
			"USE_fill_up fill_up fills up holes in regions. The number of regions remains unchanged. The neighborhood type is set via set_system(neighborhood,<4/8>) (default: 8-neighborhood).");

		o["USE_erosion_circle"] << Option("USE_erosion_circle", 0, 0, 1,                                              // FindShapeModleMinScore
			"USE_erosion_circle");
		(o)["erosion_circle_radius"] << Option("erosion_circle_radius", 3.5f, 1.50f, 511.0f,                           // 
			"Radius of the circular structuring element.. efault value: 3.5 uggested values: 1.5, 2.5, 3.5, 4.5, 5.5, 7.5, 9.5, 12.5, 15.5, 19.5, 25.5, 33.5, 45.5, 60.5, 110.5",
			true);


		//==========================================
		o["USE_reduce_domain"] << Option("USE_reduce_domain", 0, 0, 1,                                      // 
			"USE_reduce_domain reduce_domain(Image, Region : ImageReduced : : )");

		o["USE_reduce_domain_Gray"] << Option("USE_reduce_domain_Gray", 0, 0, 1,                           // 
			"USE_reduce_domain_Gray image to replace the product to measure dimension");
		
		
	}

	// ƥ��Ԫ��
	void stElement::initOptionMapMATCH(OptionsMap & o)
	{
		
		o["����������"] << Option("NumLevels", "auto",                                              // FindShapeModleMinScore
			"Default value: auto  List of values: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 auto");
		
		o["��ʼ�ǻ���"] << Option("AngleStart", -0.39f, -3.14f, 3.14f,                             // CreateShapeModel double AngleStart
			"CreateModle double AngleStart RAD Default value: -0.39 Suggested values: -3.14, -1.57, -0.78, -0.39, -0.20, 0.0 ");
		
		o["��ת��Χ"] << Option("AngleExtend", 0.78f, 0.0f, 6.29f,                               // CreateShapeModel double AngleExtend
			"CreateModle double AngleExtend Default value: 0.78 Suggested values: 6.29, 3.14, 1.57, 0.78, 0.39, 0.0 ");
		o["���������С�߶�����"] << Option("ScaleRMin", 0.9f, 0.5f,1.0f,                                        // FindShapeModleMinScore
			"Default value: 0.9 Suggested values : 0.5, 0.6, 0.7, 0.8, 0.9, 1.0");
		o["����������߶�����"] << Option("ScaleRMax", 1.1f,1.0f,1.5f,                                         // FindShapeModleMinScore
			"Default value: 1.1 Suggested values : 1.0, 1.1, 1.2, 1.3, 1.4, 1.5");
		o["������β�������"] << Option("ScaleRStep", "auto",                                         // FindShapeModleMinScore
			"Default value:  'auto' Suggested values : 'auto', 0.01, 0.02, 0.05, 0.1, 0.15, 0.2");
		o["���������С�߶�����"] << Option("ScaleCMin", 0.9f, 0.5f, 1.0f,                                        // FindShapeModleMinScore
			"Default value: 0.9 Suggested values : 0.5, 0.6, 0.7, 0.8, 0.9, 1.0");
		o["����������߶�����"] << Option("ScaleCMax", 1.1f, 1.0f, 1.5f,                                         // FindShapeModleMinScore
			"Default value: 1.1 Suggested values : 1.0, 1.1, 1.2, 1.3, 1.4, 1.5");
		o["������β�������"] << Option("ScaleCStep", "auto",                                         // FindShapeModleMinScore
			"Default value:  'auto' Suggested values : 'auto', 0.01, 0.02, 0.05, 0.1, 0.15, 0.2");

		o["����"] << Option("AngleStep", "auto",                                               // CreateShapeModel double AngleExtend
			"CreateModle double AngleStep Default value: auto Suggested values: 0.0175, 0.0349, 0.0524, 0.0698, 0.0873 Restriction: (AngleStep >= 0) && (AngleStep <= (pi / 16))");

		o["CreateModleOptimization"] << Option("Optimization", "auto",                                         // FindShapeModleMinScore
			"Kind of optimization and optionally method used for generating the model. List of values: auto, no_pregeneration, none, point_reduction_high, point_reduction_low, point_reduction_medium, pregeneration");

		o["CreateModleMetric"] << Option("Metric", "use_polarity",                                         // FindShapeModleMinScore
			"Match metric. Default value: use_polarityList of values: ignore_color_polarity, ignore_global_polarity, ignore_local_polarity, use_polarity");

		o["��С�߶�����"] << Option("Contrast", "auto",                                         // FindShapeModleMinScore
			"Threshold or hysteresis thresholds for the contrast of the object in the template image and optionally minimum size of the object parts. Default value: auto Suggested values: auto, auto_contrast, auto_contrast_hyst, auto_min_size, 10, 20, 30, 40, 60, 80, 100, 120, 140, 160");
		
		o["CreateModleMinContrast"] << Option("MinContrast", "auto",                                         // FindShapeModleMinScore
			"Minimum contrast of the objects in the search images. Default value: auto Suggested values: auto, 1, 2, 3, 5, 7, 10, 20, 30, 40 estriction: MinContrast < Contrast");


		//o["FindShapeModleAngleStart"] << Option("AngleStart", -0.39f, -3.14f, 3.14f,                             // CreateShapeModel double AngleStart
		//	"FindShapeModle double AngleStart RAD Default value: -0.39 Suggested values: -3.14, -1.57, -0.78, -0.39, -0.20, 0.0 ");

		//o["FindShapeModleAngleExtend"] << Option("AngleExtend", 0.78f, 0.0f, 6.29f,                               // CreateShapeModel double AngleExtend
		//	"FindShapeModle double AngleExtend Default value: 0.78 Suggested values: 6.29, 3.14, 1.57, 0.78, 0.39, 0.0 ");

		o["���ƶ�"] << Option("MinScore", 0.5f, 0.00f, 1.0f,                                      // FindShapeModleMinScore
			"Default value: 0.5 Suggested values: 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0");

		o["Ŀ����"] << Option("NumMatches", 1, 0, 20,                                              // FindShapeModleMinScore
			"Default value: 1 Suggested values: 0, 1, 2, 3, 4, 5, 10, 20");
		
		o["Ŀ��������"] << Option("NumMatchesLimit", 2, 0, 99999999,                                              // FindShapeModleMinScore
			"Default value: 2 Suggested values: 2, 99999999");

		o["FindModleMaxOverlap"] << Option("MaxOverlap", 0.5f, 0.0f, 1.00f,                                      // FindShapeModleMinScore
			"MaxOverlap double  Default value: 0.5 Suggested values: 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0");

		o["FindModleSubPixel"] << Option("SubPixel", "least_squares",                                         // FindShapeModleMinScore
			"least_squares Default value: least_squares List of values: interpolation, least_squares, least_squares_high, least_squares_very_high, max_deformation 1, max_deformation 2, max_deformation 3, max_deformation 4, max_deformation 5, max_deformation 6, none");

		//o["FindShapeModleNumLevels"] << Option("NumLevels", 0, 0, 10,                                              // FindShapeModleMinScore
		//	"FindShapeModle NumLevels Default value: 0  List of values: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10");

		o["ƥ��ģ��"] << Option("ModelTypes", 0, 0, 1,                                              // FindShapeModleMinScore
			"ƥ��ģ�� ������nccƥ�� Default value: 0  List of values: 0, 1");

		o["FindModleGreediness"] << Option("Greediness", 0.9f, 0.0f, 1.0f,                                              // FindShapeModleMinScore
			"Default value: 0.9 Suggested values: 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0 Greediness of the search heuristic (0: safe but slow; 1: fast but matches may be missed).");
		
		o["DispShapeColor"] << Option("DispShapeColor", "red",                                         // FindShapeModleMinScore
			"dev_display_shape_matching_results color");

		o["RES_FindRow"] << Option("RES_FindRow", "",                                         // FindShapeModleMinScore
			"RES_FindRow");

		o["RES_FindColumn"] << Option("RES_FindColumn", "",                                         // FindShapeModleMinScore
			"RES_FindColumn");

		o["RES_FindAngle"] << Option("RES_FindAngle", "",                                         // FindShapeModleMinScore
			"RES_FindAngle");

		o["������ƶ�"] << Option("RES_FindScore", "",                                         // FindShapeModleMinScore
			"������ƶ�");

		o["RES_FindCenterRow"] << Option("RES_FindCenterRow", "",                                         // FindShapeModleMinScore
			"RES_FindCenterRow");

		o["RES_FindCenterColumn"] << Option("RES_FindCenterColumn", "",                                         // FindShapeModleMinScore
			"RES_FindCenterColumn");

	}

	// У�� mm 
	void stElement::initOptionMapElemnetMM(OptionsMap & o)
	{
		o["У��Ԫ�� X"] << Option("OUT_Elemnt_NameX", "",                                                 //  Ԫ�ص�����
			"У��Ԫ�� X", true);

		//o["У��Ԫ�صĲ��� X"] << Option("OUT_Elemnt_Parament_NameX", "",                               // Ԫ�صĲ�����
		//	"���Ԫ�صĲ���", true);

		//o["У��Ԫ�ص�ID X"] << Option("OUT_Elemnt_IndentifyX", 0, 0, INT_MAX,                             // ���Ԫ�ص�Ψһ���
		//	"У��Ԫ�ص�ID X", true);

		//o["У��Ԫ�صĲ���ֵ X"] << Option("OUT_Elemnt_DataX", 0.0f, -FLT_MAX, FLT_MAX,                             // ����ֵ
		//	"OUT_Elemnt_Parament_DataX", true);

		//o["У��Ԫ�صı�׼ֵ X"] << Option("OUT_Elemnt_StandardX", 0.0f, -FLT_MAX, FLT_MAX,                     // ��׼ֵ
		//	"OUT_Elemnt_Parament_StandardX", true);

		o["У��Ԫ�� Y"] << Option(" OUT_Elemnt_NameY", "",                                                 //  Ԫ�ص�����
			"У��Ԫ�� Y", true);

		//o["У��Ԫ�صĲ��� Y"] << Option("OUT_Elemnt_Parament_NameY", "",                               // Ԫ�صĲ�����
		//	"���Ԫ�صĲ���", true);

		//o["У��Ԫ�ص�ID Y"] << Option("OUT_Elemnt_IndentifyY", 0, 0, INT_MAX,                            // ���Ԫ�ص�Ψһ���
		//	"У��Ԫ�ص�ID Y", true);

		//o["У��Ԫ�صĲ���ֵ Y"] << Option("OUT_Elemnt_DataY", 0.0f, -FLT_MAX, FLT_MAX,                            // ����ֵ
		//	"OUT_Elemnt_Parament_DataY", true);

		//o["У��Ԫ�صı�׼ֵ Y"] << Option("OUT_Elemnt_StandardY", 0.0f, -FLT_MAX, FLT_MAX,                    // ��׼ֵ
		//	"OUT_Elemnt_Parament_StandardY", true);
	}

	// ����Ԫ��
	void stElement::initOptionMapElemnetPeek(OptionsMap & o)
	{
		o["Peak_Rectangle2_Row"] << Option("Peak_Rectangle2_Row", 0.0f, -FLT_MAX, FLT_MAX,                                        // 
			"Peak Rectangle2 Row  [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o["Peak_Rectangle2_Colum"] << Option("Peak_Rectangle2_Colum", 0.0f, -FLT_MAX, FLT_MAX,                                        // 
			"Peak Rectangle2 Colum  [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o["Peak_Rectangle2_Phi"] << Option("Peak_Rectangle2_Phi", 0.0f, -FLT_MAX, FLT_MAX,                                        // 
			"Peak Rectangle2 Phi  [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o["Peak_Rectangle2_Length1"] << Option("Peak_Rectangle2_Length1", 0.0f, -FLT_MAX, FLT_MAX,                                        // 
			"Peak Rectangle2 Length1  [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o["Peak_Rectangle2_Length2"] << Option("Peak_Rectangle2_Length2", 0.0f, -FLT_MAX, FLT_MAX,                                        // 
			"Peak Rectangle2 Length2  [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o["Peak_DetectWidth"] << Option("Peak_DetectWidth", 2.0f, -FLT_MAX, FLT_MAX,                                        // 
			"Peak_DetectWidth  [FLT_MIN, FLT_MAX] default 2.0 ", true);

		o["Peak_Sigma"] << Option("Peak_Sigma", 0.41f, -FLT_MAX, FLT_MAX,                                        // 
			"Peak_Sigma [FLT_MIN, FLT_MAX] default 0.41 ", true);

		o["Peak_Threshold"] << Option("Peak_Threshold", 20.0f, -FLT_MAX, FLT_MAX,                                        // 
			"Peak_Threshold[FLT_MIN, FLT_MAX] default 20.0 ", true);


		// �洢���,��������ж��
		o["Peak_cross_size"] << Option("Peak_cross_size", 20.0f, -FLT_MAX, FLT_MAX,
			"����ʮ���ߵĳߴ�", true);

		o["Peak_cross_angle"] << Option("Peak_cross_angle", 0.785398f, -FLT_MAX, FLT_MAX,
			"����ʮ���ߵĽǶ� Default value: 0.785398 Suggested values: 0.0, 0.785398", true);

		// �洢���,��������ж��
		o["Peak_Transition"] << Option("Peak_Transition", "positive",                                                     
			"Peak ransition negative: �ڶ��� positive���׶���", true);

		o["Peak_Select"] << Option("Peak_Select", "first",
			"Peak Select", true);
		

		// �洢���,��������ж��
		//o["nRES_hv_EdgesY"] << Option("RES_hv_EdgesY", "",                                                      // Ԫ�صĽ�����б�Ե
		//	"RES_hv EdgesY ", true);

		//o["nRES_hv_EdgesX"] << Option("RES_hv_EdgesX", "",                                                      // Ԫ�صĽ������
		//	"RES_hv EdgesX ", true);

		o["RES_hv_ResultRow"] << Option("RES_hv_ResultRow", "",                                                  // Ԫ�صĽ������
			"RES_hv ResultRow ", true);

		o["RES_hv_ResultColumn"] << Option("RES_hv_ResultColumn", "",                                            // Ԫ�صĽ������
			"RES_hv ResultColumn ", true);

	}

	// һά����
	void stElement::initOptionMapELEMENT_MEASURE_RECTANGLE2(OptionsMap & o)
	{
		// ����
		
		o["Measure_Rectangle2_Row"] << Option("Measure_Rectangle2_Row", 20.0f, 0.0f, FLT_MAX,                  // Row coordinate of the center of the rectangle.
			"Measure Rectangle2 Row  [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o["Measure_Rectangle2_Column"] << Option("Measure_Rectangle2_Column", 20.0f, 0.0f, FLT_MAX,           // Column coordinate of the center of the rectangle.
			"Measure Rectangle2 Column  [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o["Measure_Rectangle2_Phi"] << Option("Measure_Rectangle2_Phi", 0.0f, -FLT_MAX, FLT_MAX,           // Angle of longitudinal axis of the rectangle to horizontal (radians).
			"Measure Rectangle2 Phi  Typical range of values: -1.178097 �� Phi �� 1.178097 (lin)", true);

		o["Measure_Rectangle2_Length1"] << Option("Measure_Rectangle2_Length1", 20.0f, 1.0f, FLT_MAX,           // Angle of longitudinal axis of the rectangle to horizontal (radians).
			"Measure Rectangle2 Length1  Restriction: Length1 >= 1.0", true);

		o["Measure_Rectangle2_Length2"] << Option("Measure_Rectangle2_Length2", 10.0f, 1.0f, FLT_MAX,           // Angle of longitudinal axis of the rectangle to horizontal (radians).
			"Measure Rectangle2 Length2  Restriction: Length1 >= 1.0", true);

		o["Measure_Rectangle2_Interpolation"] << Option("Measure_Rectangle2_Interpolation", "bilinear",            // Angle of longitudinal axis of the rectangle to horizontal (radians).
			"Measure Rectangle2 Interpolation  bicubic, bilinear, nearest_neighbor", true);

		// ���
		// measure_pairs ����
		o["measure_pairs_Sigma"] << Option("measure_pairs_Sigma", 1.0f, 0.4f, 100.0f,           // Angle of longitudinal axis of the rectangle to horizontal (radians).
			"Sigma of gaussian smoothing.Restriction: Sigma >= 0.4 ", true);
		
		o["measure_pairs_Threshold"] << Option("measure_pairs_Threshold", 30.0f, 1.0f, 255.0f,           // Angle of longitudinal axis of the rectangle to horizontal (radians).
			"Minimum edge amplitude.Default value: 30.0 Suggested values: 5.0, 10.0, 20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 90.0, 110.0  ", true);
		
		o["measure_pairs_Transition"] << Option("measure_pairs_Transition", "all",            // Angle of longitudinal axis of the rectangle to horizontal (radians).
			"Type of gray value transition that determines how edges are grouped to edge pairs. List of values: all, all_strongest, negative, negative_strongest, positive, positive_strongest", true);
	
		o["measure_pairs_Select"] << Option("measure_pairs_Select", "all",            // Angle of longitudinal axis of the rectangle to horizontal (radians).
			"Selection of edge pairs. Default value: all. List of values: all, first, last", true);

		// ���
		o["measure_pairs_RowEdgeFirst"] << Option("measure_pairs_RowEdgeFirst", "",            
			"Row coordinate of the center of the first edge.", true);

		o["measure_pairs_ColumnEdgeFirst"] << Option("measure_pairs_ColumnEdgeFirst", "",      
			"Column coordinate of the center of the first edge.", true);

		o["measure_pairs_AmplitudeFirst"] << Option("measure_pairs_AmplitudeFirst", "",        
			"Edge amplitude of the first edge (with sign).", true);

		o["measure_pairs_RowEdgeSecond"] << Option("measure_pairs_RowEdgeSecond", "",
			"Row coordinate of the center of the second edge.", true);

		o["measure_pairs_ColumnEdgeSecond"] << Option("measure_pairs_ColumnEdgeSecond", "",
			"Column coordinate of the center of the second edge.", true);

		o["measure_pairs_AmplitudeSecond"] << Option("measure_pairs_AmplitudeSecond", "",
			"Edge amplitude of the second edge (with sign).", true);

		o["RES_measure_pairs_IntraDistance"] << Option("RES_measure_pairs_IntraDistance", "",
			"Distance between edges of an edge pair.", true);

		o["RES_measure_pairs_InterDistance"] << Option("RES_measure_pairs_InterDistance", "",
			"Distance between consecutive edge pairs.", true);

		o["RES_һά�ܳ�"] << Option("RES_һά�ܳ�", 0.0f, -FLT_MAX, FLT_MAX,
			"RES_measure_pairs_Balloon.", true);

		o["�ߴ���ʾƫ��X"] << Option("�ߴ���ʾƫ��X", 0.0f, -FLT_MAX, FLT_MAX,                        // 
			"�ߴ���ʾƫ��X", true);
		o["�ߴ���ʾƫ��Y"] << Option("�ߴ���ʾƫ��Y", 0.0f, -FLT_MAX, FLT_MAX,                        // 
			"�ߴ���ʾƫ��Y", true);
	}

	// ����ȱ�� 
	void stElement::initOptionMapBalloonFlaw(OptionsMap & o)
	{
		// ����
		o["Flaw_Rectangle2_Row"] << Option("Flaw_Rectangle2_Row", 400.0f, 0.0f, FLT_MAX,                 
			"Flaw Rectangle2 Row  [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o["Flaw_Rectangle2_Column"] << Option("Flaw_Rectangle2_Column", 400.0f, 0.0f, FLT_MAX,           
			"Flaw Rectangle2 Column  [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o["Flaw_Rectangle2_Phi"] << Option("Flaw_Rectangle2_Phi", 0.0f, -FLT_MAX, FLT_MAX,              
			"Typical range of values: -1.178097 �� Phi �� 1.178097 (lin)", true);

		o["Flaw_Rectangle2_Length1"] << Option("Flaw_Rectangle2_Length1", 400.0f, 10.0f, FLT_MAX,         
			"Restriction: Length1 >= 1.0", true);

		o["Flaw_Rectangle2_Length2"] << Option("Flaw_Rectangle2_Length2", 300.0f, 10.0f, FLT_MAX,         
			"Restriction: Length1 >= 1.0", true);

		// mean_image ��ֵ
		o["Flaw_mean_image_MaskWidth"] << Option("Flaw_mean_image_MaskWidth", 43, 1, 501,
			"Typical, Default value: 9, range of values: 1 �� MaskHeight �� 501", true);

		o["Flaw_mean_image_MaskHeight"] << Option("Flaw_mean_image_MaskHeight", 43, 1, 501,
			"Typical, Default value: 9, range of values: 1 �� MaskHeight �� 501", true);

		// dyn_threshold ��̬��ֵ
		o["Flaw_dyn_threshold_Offset"] << Option("Flaw_dyn_threshold_Offset", 50.0, -255.0f, 255.0f,
			"Default value: 5.0, Suggested values: 1.0, 3.0, 5.0, 7.0, 10.0, 20.0, 30.0 Typical range of values: -255.0 �� Offset �� 255.0 (lin) ", true);

		o["Flaw_dyn_threshold_LightDark"] << Option("Flaw_dyn_threshold_LightDark", "dark",
			"Extract light, dark or similar areas? Default value: light, List of values: dark, equal, light, not_equal  ", true);

		// closing_circle
		o["Flaw_closing_circle_Radius"] << Option("Flaw_closing_circle_Radius", 3.5f, 0.5f, 511.0f,
			"Radius of the circular structuring element.  Default value: 3.5 Suggested values: 1.5, 2.5, 3.5, 4.5, 5.5, 7.5, 9.5, 12.5, 15.5, 19.5, 25.5, 33.5, 45.5, 60.5, 110.5", true);

		// select_shape
		o["Flaw_select_shape_Features"] << Option("Flaw_select_shape_Features", "area",
			"Shape features to be checked. Default value: area, List of values: anisometry, area, area_holes, bulkiness, circularity, column, column1, column2, compactness, connect_num, contlength, convexity, dist_deviation, dist_mean, euler_number, height, holes_num, inner_height, inner_radius, inner_width, max_diameter, moments_i1, ", true);
		
		o["Flaw_select_shape_Operation"] << Option("Flaw_select_shape_Operation", "and",
			"Linkage type of the individual features.Default value: and, List of values: and, or ", true);


		o["Flaw_select_shape_Min"] << Option("Flaw_select_shape_Min", 15.0f, 1.0f, FLT_MAX,
			"Lower limits of the features or min.", true);

		o["Flaw_select_shape_Max"] << Option("Flaw_select_shape_Max", 99999.0f, 1.0f, FLT_MAX,
			"Lower limits of the features or max.", true);


		o["RES_ȱ������"] << Option("RES_ȱ������", 0.0f, 0.0f, FLT_MAX,
			"�����RES_ȱ������", true);

		o["RES_ȱ��������"] << Option("RES_ȱ��������", 0.0f, 0.0f, FLT_MAX,
			"RES_ȱ��������", true);

		o["RES_ȱ����С���"] << Option("RES_ȱ����С���", 0.0f, 0.0f, FLT_MAX,
			"RES_ȱ����С���", true);
	}

	// ROI
	void stElement::initOptionMapROI(OptionsMap & o)
	{
		
		// ��У��������ROI
		o["USE_map_image"] << Option("USE_map_image", 1, 0, 1,                               // �����ȴ�����У��
			"ʹ�������У����Ϣ��ͼ����л���У��.");
		
		// ����
		o["ROI_Rectangle2_Row"] << Option("ROI_Rectangle2_Row", 400.0f, 0.0f, FLT_MAX,
			"ROI Rectangle2 Row  [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o["ROI_Rectangle2_Column"] << Option("ROI_Rectangle2_Column", 400.0f, 0.0f, FLT_MAX,
			"ROI Rectangle2 Column  [FLT_MIN, FLT_MAX] default 0.0 ", true);

		o["ROI_Rectangle2_Phi"] << Option("ROI_Rectangle2_Phi", 0.0f, -FLT_MAX, FLT_MAX,
			"Typical range of values: -1.178097 �� Phi �� 1.178097 (lin)", true);

		o["ROI_Rectangle2_Length1"] << Option("ROI_Rectangle2_Length1", 200.0f, 1.0f, FLT_MAX,
			"Restriction: Length1 >= 1.0", true);

		o["ROI_Rectangle2_Length2"] << Option("ROI_Rectangle2_Length2", 200.0f, 1.0f, FLT_MAX,
			"Restriction: Length1 >= 1.0", true);
	}

	// ���ҳ���
	void stElement::initOptionMapBalloonLength(OptionsMap & o)
	{
		o["IsDisplayDimension"] << Option("IsDisplayDimension", 1, 0, 1,                                     // �Ƿ���ʾ�ߴ���ֵ
			"IsDisplayDimension", false);

		o["���ö���1"] << Option("���ö���1", "",                                  // ʹ�ö���1����
			"���ö���1", true);

		//o["USE_Peek1_Indentify"] << Option("USE_Peek1_Indentify", 0, 0, INT_MAX,                              // ʹ�ö���1 ID
		//	"USE_Peek1_Indentify", true);
		
		o["���ö���2"] << Option("���ö���2", "",                                  // ʹ�ö���1����
			"���ö���2", true);

		//o["USE_Peek2_Indentify"] << Option("USE_Peek2_Indentify", 0, 0, INT_MAX,                              // ʹ�ö���1 ID
		//	"USE_Peek2_Indentify", true);

		o["���ö���3"] << Option("���ö���3", "",                                  // ʹ�ö���1����
			"���ö���3", true);

		//o["USE_Peek3_Indentify"] << Option("USE_Peek3_Indentify", 0, 0, INT_MAX,                              // ʹ�ö���1 ID
		//	"USE_Peek3_Indentify", true);

		o["���ö���4"] << Option("���ö���4", "",                                  // ʹ�ö���4����
			"���ö���4", true);

		//o["USE_Peek4_Indentify"] << Option("USE_Peek4_Indentify", 0, 0, INT_MAX,                              // ʹ�ö���4 ID
		//	"USE_Peek4_Indentify", true);

		o["�ߴ���ʾƫ��X"] << Option("�ߴ���ʾƫ��X", 0.0f, -FLT_MAX, FLT_MAX,                        // 
			"�ߴ���ʾƫ��X", true);
		o["�ߴ���ʾƫ��Y"] << Option("�ߴ���ʾƫ��Y", 0.0f, -FLT_MAX, FLT_MAX,                        // 
			"�ߴ���ʾƫ��Y", true);

		o["Balloon����Row1"] << Option("����Row1", 0.0f, -FLT_MAX, FLT_MAX,                        // 
			"����Row1", false);
		o["Balloon����Col1"] << Option("����Col1", 0.0f, -FLT_MAX, FLT_MAX,                        // 
			"����Col1", false);

		o["Balloon����Row2"] << Option("����Row2", 0.0f, -FLT_MAX, FLT_MAX,                        // 
			"����Row1", false);
		o["Balloon����Col2"] << Option("����Col2", 0.0f, -FLT_MAX, FLT_MAX,                        // 
			"����Col2", false);

		o["�ߴ��ͷ����"] << Option("�ߴ��ͷ����", 10.0f, 2.0f, FLT_MAX,                                      // gen_arrow_contour_xld
			"Arrow_HeadLength Default Value: 5 ����ֵ: [2,3,5,10,20]", true);

		o["�ߴ��ͷ���"] << Option("�ߴ��ͷ���", 10.0f, 2.0f, FLT_MAX,                                      // gen_arrow_contour_xld
			"The width of the arrow head in pixels Default Value: 5 ����ֵ: [2,3,5,10,20]", true);

		o["RES_���ҳ���"] << Option("RES_Balloon_Length", 0.0f, -FLT_MAX, FLT_MAX,                            // ���ҵĳ���
			"RES_Balloon_Length [FLT_MIN, FLT_MAX] default 0.0 ", true);

	}
	BOOL stElement::isHaveSaveElementName(CString str)
	{

		POSITION pos = UCI::ELE.EleList->GetHeadPosition();
		while (pos != NULL) {
			UCI::OptionsMap *ot = &UCI::ELE.EleList->GetNext(pos);
			CString name = (*ot)["Ԫ������"];
			if (str == name) return TRUE;
		}

		return FALSE;
	}
	////////////////////////////////////////////////////////////////////////////////////
	// ��ʼ��һ��Ԫ�ر���
	OptionsMap* stElement::initOptionMapElemnet(eELEMENT_TYPE t, int iPID)
	{
		
		OptionsMap o;
		Option::insert_order = 0;

		
		// ����Ψһ��ţ�ֻ��Ψһ����
		CString name = ""; 
		for (int i = 1; i < INT_MAX; i++) {
			name.Format("%s %d", EleName[t], i);
			if (!UCI::ELE.isHaveSaveElementName(name)) {
				break;
			}
		}

		//int index = (int)ELE.EleList->GetSize() + 1;

		// ��ʼ��һЩ��������                                                   
		o["��Ŀ���"] << Option("��Ŀ���", iPID, 1, INT_MAX,                         // ��ǰ�����������Ŀ���
			"Current Project ID", false);

		o["Ԫ������"] << Option("Ԫ������", t, 0, ELEMNET_END - 1,                    // ��ǰ����Ԫ������
			"Current Elemnet Type",false);
		
		o["Ԫ������"] << Option("Ԫ������", name,                                     // Ԫ�ص�����
			"Element Name", true);	
		

		o["Ԫ����ƥ��"] << Option("Ԫ����ƥ��", 0, 0, 1,                           // Ԫ�ص����ҵ�
			"Element is Finded", false);

		switch (t) {
		case ELEMENT_LINE:      // ֱ��
			initOptionMapElemnetLine(o);
			break;
		case ELEMENT_CIRCLE:    // Բ 
			initOptionMapElemnetCircle(o);
			break;

		case ELEMENT_ELLIPSE:   // ��Բ

			break;
		case ELEMENT_ANGLE:     // �Ƕ�
			initOptionMapElemnetAngle(o);
			break;
		case ELEMENT_RESULT:    // ������
		//{
		//	o["����������"] << Option("����������", name,             // Ҫ�����Ԫ�ص�����
		//		"���������ʱ��������ʾ������", true);
		//}

			initOptionMapElemnetResult(o);	
			break;

		case ELEMENT_MM:
			initOptionMapElemnetMM(o);
			break;
		case ELEMENT_DISTANCE:
			initOptionMapElemnetDistance(o);
			break;
		case ELEMENT_CAMERA:
			initOptionMapElemnetCamera(o);
			break;
		case ELEMENT_PEAK:
			initOptionMapElemnetPeek(o);
			break;
		case ELEMENT_BALLOONLENGTH:
			initOptionMapBalloonLength(o);
			break;
		case ELEMENT_FUNCTION:
			initOptionMapFUNCTION(o);
			break;
		case ELEMENT_MATCH:
			initOptionMapMATCH(o);
			break;
		case ELEMENT_MEASURE_RECTANGLE2:
			initOptionMapELEMENT_MEASURE_RECTANGLE2(o);
			break;
		case ELEMENT_BALLOON_FLAW:
			initOptionMapBalloonFlaw(o);
			break;
		case ELEMENT_ROI:
			initOptionMapROI(o);
			break;
		default:			
			break;
		}

		o["������"] << Option("������", "none", "Command Macro", true);                           // Ԫ�ص������
		o["Ԫ�ر�ע"] << Option("Ԫ�ر�ע", "note", "Element Note", true);                        // Ԫ�صı�ע
		
		ELE.EleList->AddTail(o);
		return &ELE.EleList->GetTail();
	}

	
}




