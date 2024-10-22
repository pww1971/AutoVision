#pragma once

#include"MyData.h"
#include <list>

#include "CCameraCPP.h"

class struct_CROSS_LINE
{
public:
	int x;
	int y;
	COLORREF color;
	bool isUse;
};

struct S_Cali
{
	S_Cali()
	{
		//ho_RectMap.GenEmptyObj();
	};
	
	HalconCpp::HTuple hv_RectifiPose;        //== ӳ����̬
	HalconCpp::HPose hv_CamPose;             //== �����̬ 
	HalconCpp::HTuple hv_CamParameters;      //== ����ڲ�����	
	HalconCpp::HImage ho_RectMap;	         //== ����ӳ��ͼ��
};

class ST_MYDISP
{
public:
	ST_MYDISP(CWnd* cwnd);  // CChildView
	~ST_MYDISP();

	void  IPRunCamCaliInner();      // �궨�ڲ��߳�
	void  IPRunCamCaliPose();       // �궨����߳�

	/////////////////////////////////////////////////////////////////////////////
	// Declaration of the thread functions

	void AutoMeasureOnePicture(CList <UCI::OptionsMap, UCI::OptionsMap>& eleList,HalconCpp::HImage& img, HalconCpp::HXLDCont& con);



	void OnDestroy();

	void InitThreadCommunication();


	HalconCpp::HWindow   *m_pMHWinMain;               // HALCON ��ʾ������


	HalconCpp::HTuple hv_DrawID;                     // ���Ӷ���ȫ�ֱ���

	void ResetAllCStaticPart();                              // ������ӳɹ���, ����һ�����е���ʾ�ؼ�����ʾ����
	void ResetAllCStaticWinExtends();                        // ����halcon��ʾ��ͼ��Ĵ�С


	void AppStateChangeTo(eAppState e);                      // �ı����״̬

	HalconCpp::HImage  GrabImageDisp;                        // ������ʾ��ֱ�Ӳɼ�����ͼ��
															 //HalconCpp::HImage  GrabImageERROK;                       // �궨ͼ�����ϸ��ͼ��
	struct ResultContainer m_sIPResult;

	// ======================================================================== 	
	// OnMainDisp() - The OnFGDisp method also allows derived classes to handle the 
	//             event without attaching a delegate. This is the preferred 
	//			   technique for handling the event in a derived class.
	//
	LRESULT OnMainDisp();


	void set_display_font(HalconCpp::HTuple hv_WindowHandle, HalconCpp::HTuple hv_Size,
		HalconCpp::HTuple hv_Font, HalconCpp::HTuple hv_Bold,
		HalconCpp::HTuple hv_Slant);


	BOOL LoadCaliPoseAndParamter(string fileAdd,int prID );                         // ������̬������ڲ�
	BOOL OnDisMeasureOnePic(UCI::OptionsMap * o, HalconCpp::HImage* Img);      // ��o�����ɼ�һ��ͼ����У��

	
	void OnDisButMatchLoadPic();
	void OnDisButLoadPic();

	void DisOnButCaliCorrect(int prID);                            // ���ݱ궨��У��ͼ��


	void DisOnMatchTakeOne();

	void DisDrawAllRoi(HalconCpp::HXLDCont& hv_Cont);       // ��������ROI HXLD
	void DisDrawOneRoi(UCI::OptionsMap *o, HalconCpp::HXLDCont& hv_Cont);                // ����һ��Ԫ�ص�ROI

	void DisOnEleOutReport();                     // ����Ԫ�����
	void DisOnMeaEleMillimeter();                 // Ԫ��ת��mm
	void DisOnEleCameraAdd();                     // ������Ԫ��

	void EditOneElement(UCI::OptionsMap *o);      // �༭һ��Ԫ��
	void EditOneElementLine(UCI::OptionsMap *o);
	void EditOneElementCIRCLE(UCI::OptionsMap *o);
	void EditOneElementPEAK(UCI::OptionsMap *o);
	void EditOneMEASURE_RECTANGLE2(UCI::OptionsMap *o);
	void EditOneBalloonFlaw(UCI::OptionsMap *o);
	void EditOneROI(UCI::OptionsMap *o);

	CString GetMatchPathFile(int pr);               // �õ���ǰ��Ŀ��ƥ��ͼƬ�ļ���
	CString GetShapeModelPathFile(int pr);          // �õ� shm ���ļ���
	CString GetShapeModelRegionPathFile(int pr);    // �õ� ���� ShapeModel��Region�ļ���

	int GetHeight() {
		int h = UCI::OPTIONS["ͼ��߶�"]; return h;
	};

	int GetWidth() {
		int w = UCI::OPTIONS["ͼ����"]; return w;
	};

	eAppState GetAppState() { return this->mAppState; };

public:
	BOOL MeasureOneElement(CList <UCI::OptionsMap, UCI::OptionsMap>& eleList,UCI::OptionsMap *o, HalconCpp::HImage &hImg, HalconCpp::HXLDCont&);     // ����һ��Ԫ�ء�����


private:
	void Zoomfit();//�ָ�����
	void scaleState(bool s );//���Ŵ���
	void ControlFunction(HalconCpp::HTuple WindowHandleOrg,HalconCpp::HTuple ZoomTrans,HalconCpp::HTuple Row,HalconCpp::HTuple Col,HalconCpp::HTuple RowShif,HalconCpp::HTuple ColShif);
	float m_Zoom;					//���ż�¼
	float m_ZoomOrg;


	BOOL MeasureOneLine(UCI::OptionsMap *o, HalconCpp::HImage &hImg, HalconCpp::HXLDCont&);        // ����һ��ֱ�ߡ���
	BOOL MeasureOneCircle(UCI::OptionsMap *o, HalconCpp::HImage &hImg, HalconCpp::HXLDCont&);      // ����һ��Բ��
	BOOL MeasureOneAngle(CList <UCI::OptionsMap, UCI::OptionsMap>& eleList,UCI::OptionsMap *o, HalconCpp::HImage &hImg, HalconCpp::HXLDCont&);
	BOOL MeasureOneDistance(CList <UCI::OptionsMap, UCI::OptionsMap>& eleList,UCI::OptionsMap *o, HalconCpp::HImage&hImg, HalconCpp::HXLDCont&);

	BOOL MeasureOneCamera(UCI::OptionsMap *o, HalconCpp::HImage&hImg, HalconCpp::HXLDCont&);       // ����һ�����Ԫ�� 
	BOOL MeasureOneROI(UCI::OptionsMap *o, HalconCpp::HImage&hImg, HalconCpp::HXLDCont&);          // ����һ��ROIԪ�� 

	BOOL MeasureOneFUNCTION(UCI::OptionsMap *o, HalconCpp::HImage&hImg, HalconCpp::HXLDCont&);       // 
	BOOL MeasureOneFUNCTION2(UCI::OptionsMap *o, HalconCpp::HImage&hImg, HalconCpp::HXLDCont&);       // 
	BOOL MeasureOneELEMENT_MEASURE_RECTANGLE2(UCI::OptionsMap *o, HalconCpp::HImage&hImg, HalconCpp::HXLDCont&);
	BOOL MeasureOneELEMENT_MEASURE_BalloonFlaw(UCI::OptionsMap *o, HalconCpp::HImage&hImg, HalconCpp::HXLDCont&);

	BOOL MeasureOneResult(CList <UCI::OptionsMap, UCI::OptionsMap>& eleList,UCI::OptionsMap *o, HalconCpp::HXLDCont&);
	BOOL MeasureOneMM(CList <UCI::OptionsMap, UCI::OptionsMap>& eleList,UCI::OptionsMap *o, HalconCpp::HXLDCont&);
	BOOL MeasureOnePeak(UCI::OptionsMap *o, HalconCpp::HImage&hImg, HalconCpp::HXLDCont&);
	BOOL MeasureOneBalloonLenght(CList <UCI::OptionsMap, UCI::OptionsMap>& eleList,UCI::OptionsMap *o, HalconCpp::HXLDCont&);

	// ��ʾ�ߴ���
	void p_disp_dimensions(HalconCpp::HTuple hv_RowEdgeFirst,
		HalconCpp::HTuple hv_ColumnEdgeFirst,
		HalconCpp::HTuple hv_RowEdgeSecond,
		HalconCpp::HTuple hv_ColumnEdgeSecond,
		HalconCpp::HTuple hv_IntraDistance,
		HalconCpp::HTuple hv_InterDistance,
		HalconCpp::HTuple hv_Phi,
		HalconCpp::HTuple hv_Length, double scalemm,
		HalconCpp::HXLDCont&);

	// Displays a marker, which indicates the position of an edge 
	void p_disp_edge_marker(HalconCpp::HTuple hv_Rows,
		HalconCpp::HTuple hv_Cols,
		HalconCpp::HTuple hv_Phi,
		HalconCpp::HTuple hv_Length,
		HalconCpp::HTuple hv_Color,
		HalconCpp::HTuple hv_LineWidth,
		HalconCpp::HXLDCont&);

	void p_disp_text_right_of_center(HalconCpp::HTuple hv_Text,
		HalconCpp::HTuple hv_RowFirst,
		HalconCpp::HTuple hv_ColFirst,
		HalconCpp::HTuple hv_RowSecond,
		HalconCpp::HTuple hv_ColSecond,
		HalconCpp::HTuple hv_Phi,
		HalconCpp::HTuple hv_Distance, double scalemm,
		HalconCpp::HXLDCont&);

	void p_disp_text_left_of_center(HalconCpp::HTuple hv_Text,
		HalconCpp::HTuple hv_RowFirst,
		HalconCpp::HTuple hv_ColFirst,
		HalconCpp::HTuple hv_RowSecond,
		HalconCpp::HTuple hv_ColSecond,
		HalconCpp::HTuple hv_Phi,
		HalconCpp::HTuple hv_Distance, double scalemm,
		HalconCpp::HXLDCont&);


	CStatic*    PMainStatic;

	//////////////////////////////////////////////////////////////////////////////
	/// 
public:
	S_Cali* hv_Cali;
//	HalconCpp::HImage ho_RectMap;	         //== ����ӳ��ͼ��
	//HalconCpp::HShapeModel hv_Model;         //==  ��λģ��
	//HalconCpp::HRegion ho_ModelRegion;       //==  ģ������
	//HalconCpp::HHomMat2D HomMat2D;           //==  ROI �任����

	bool m_bRectMapOK;                       //== ����ӳ��ͼ����������OK��


	CWnd* pCView;                             // ���ڱ���
	CRect rcImage;							  // ͼ�����ʾ����

	struct_CROSS_LINE CrossLine[3];           // ʮ�ֲο���

	eAppState mAppState;                      // ��ǰ��ͼ������״̬	

	int  PushCaptureDown;                      // �궨�ڲλ����ʱ���µ���һ��	

public:

	CStatic* GetpMainStatic() { return this->PMainStatic; };

	void SetPMainStatic(CStatic* pcs) { this->PMainStatic = pcs; };

	void DisCamliInnerOnePicture(HalconCpp::HImage *img, HalconCpp::HXLDCont*  Contours,int prID);
	void DisCamliPoseOnePicture(HalconCpp::HImage *img, HalconCpp::HXLDCont*  Contours,int prID);

//	void MyLog(CString str) { return UCI::MyLog(str); };
//	void MyLog(std::string str) { return UCI::MyLog(str.c_str()); };

	bool SaveImgToFile(CString stSavePath, CString stFormat, CString& st);

	bool SaveImgToROI();

	bool ReadImageFromFile(CString stFileName);
	bool LoadDefaultPicture();
	void ChangeImageCRect(CRect cr);

	CString  GetTimeMsToString();                                             // �õ���ǰʱ���ms�����ַ���


//	int GetTotalCariInnerPicNum(int pr);                                      // ���ӵ�ǰ�ɼ�ͼ���ǵ�����
//	int GetTotalCariPosePicNum(int pr);                                        // 

	//void DelTotalCariInnerPic(int pr);                // ɾ���궨ͼ���ļ�	
	//void DelTotalCariPosePic(int pr);                 // ɾ���궨ͼ���ļ�	


													  ///--------------------------------------------------------
	bool Write_Image(HalconCpp::HImage Image, CString Path);

	bool GetCameraPoseFromPictre();

	void OnRButtonDown(MSG* pMsg);                     // ����Ҽ�����
	void OnRButtonDownAPP_ATTACH_LINE();
	void OnRButtonDownAPP_ATTACH_CIRCLE();
	void OnRButtonDownAPP_ATTACH_PEAK();
	void OnRButtonDownAPP_ATTACH_MEASURE_RECTANGLE2();
	void OnRButtonDownAPP_ATTACH_BALLOON_FLAW();
	void OnRButtonDownAPP_ATTACH_ROI();



	//---------------------------------------------------------------------------
	// ����Ϊ����

	void DisDisplay(CRect rect);                               // ����ͼ�����ʾ��������ʾһ��


	void SetPartHvWnd();

	void DisSetInnerAndPosStartPara(UCI::OptionsMap *om,
		HalconCpp::HTuple& hv_StartParameters,
		HalconCpp::HTuple& hv_TmpCtrl_FindCalObjParNames,
		HalconCpp::HTuple& hv_TmpCtrl_FindCalObjParValues);

	void DisGetCameraInnerParaFromPic(UCI::OptionsMap *om,string fileAdd,vector<string>& vctFile);                        // �ӱ궨��ͼ����ȡ��������ڲ�
	void DisGetCameraPoseParaFromPic(UCI::OptionsMap *om,string fileAdd,vector<string>& vctFile);                         // �ӱ궨��ͼ����ȡ��������ڲ�


	void write_message(HalconCpp::HTuple hv_Row,
		HalconCpp::HTuple hv_Column,
		HalconCpp::HTuple hv_String,
		bool isclear = false);

	bool GetRefMapFromSaveFile(string fileAdd,int prID);                               // 

																//==================================================================================
																// ROI

	void draw_rake(
		HalconCpp::HXLDCont *ho_Regions,
		HalconCpp::HTuple hv_Elements,
		HalconCpp::HTuple hv_DetectHeight,
		HalconCpp::HTuple hv_DetectWidth,
		double *hv_Row1,
		double *hv_Column1,
		double *hv_Row2,
		double *hv_Column2);


	void rake(
		HalconCpp::HImage ho_Image,
		HalconCpp::HXLDCont *ho_Regions,
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
		HalconCpp::HTuple *hv_ResultRow,
		HalconCpp::HTuple *hv_ResultColumn);

	void pts_to_best_line(
		HalconCpp::HXLDCont *ho_Line,
		HalconCpp::HTuple hv_Rows,
		HalconCpp::HTuple hv_Cols,
		HalconCpp::HTuple hv_ActiveNum,
		HalconCpp::HTuple *hv_Row1,
		HalconCpp::HTuple *hv_Column1,
		HalconCpp::HTuple *hv_Row2,
		HalconCpp::HTuple *hv_Column2);

	void draw_spoke(UCI::OptionsMap* o, HalconCpp::HXLDCont&);

	void draw_spoke(
		HalconCpp::HXLDCont *ho_Regions,
		HalconCpp::HTuple hv_Elements,
		HalconCpp::HTuple hv_DetectHeight,
		HalconCpp::HTuple hv_DetectWidth,
		HalconCpp::HTuple *hv_ROIRows,
		HalconCpp::HTuple *hv_ROICols,
		HalconCpp::HTuple *hv_Direct);



	void spoke(
		HalconCpp::HImage ho_Image,
		HalconCpp::HXLDCont *ho_Regions,
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
		HalconCpp::HTuple *hv_ResultRow,
		HalconCpp::HTuple *hv_ResultColumn,
		HalconCpp::HTuple *hv_ArcType);

	void pts_to_best_circle(
		HalconCpp::HXLDCont *ho_Circle,
		HalconCpp::HTuple hv_Rows,
		HalconCpp::HTuple hv_Cols,
		HalconCpp::HTuple hv_ActiveNum,
		HalconCpp::HTuple hv_ArcType,
		HalconCpp::HTuple *hv_RowCenter,
		HalconCpp::HTuple *hv_ColCenter,
		HalconCpp::HTuple *hv_Radius,
		HalconCpp::HTuple *hv_StartPhi,
		HalconCpp::HTuple *hv_EndPhi,
		HalconCpp::HTuple *hv_PointOrder);

	void gen_arrow_contour_xld(
		HalconCpp::HXLDCont *ho_Arrow,
		HalconCpp::HTuple hv_Row1,
		HalconCpp::HTuple hv_Column1,
		HalconCpp::HTuple hv_Row2,
		HalconCpp::HTuple hv_Column2,
		HalconCpp::HTuple hv_HeadLength,
		HalconCpp::HTuple hv_HeadWidth);

	void gen_double_arrow_contour_xld(
		HalconCpp::HXLDCont *ho_Arrow,
		HalconCpp::HTuple hv_Row1,
		HalconCpp::HTuple hv_Column1,
		HalconCpp::HTuple hv_Row2,
		HalconCpp::HTuple hv_Column2,
		HalconCpp::HTuple hv_HeadLength,
		HalconCpp::HTuple hv_HeadWidth);

	//////////////////////////////////////////////////////////////
	/// dis ROI
	//void DisOnRoiDrawRectangle2();
	//void DisOnRoiDrawCircle();
	//void DisOnRoiDrawEllipse();
	//void DisOnRoiDrawRectangle();
	//void DisOnRoiDrawAnyLine();
	//void DisOnRoiInfo();
//	void DisOnRoiFindModel();
//	void DisOnButMatchSaveModel();
	void DisOnElementMeasureManual();
	void DisOnAddFindPeekElement();             // ����Ҷ���Ԫ��
	void DisOnAddMEASURE_RECTANGLE2();                 // 1ά����Ԫ��
	void DisOnMeaEleEllispe();

	void DisFontLarge(CString str, float x, float y, float size, Gdiplus::SolidBrush &br);

	void DisElementDimension(UCI::OptionsMap *o);      // ��ʾԪ�صĳߴ�
	void DisElemnetDimension(UCI::OptionsMap *o, double dis, double midrow, double midcol);

	void DisOnElementDeleteAll();

	void peak(
		HalconCpp::HImage ho_Image,
		HalconCpp::HTuple hv_Row,
		HalconCpp::HTuple hv_Coloumn,
		HalconCpp::HTuple hv_Phi,
		HalconCpp::HTuple hv_Length1,
		HalconCpp::HTuple hv_Length2,
		HalconCpp::HTuple hv_DetectWidth,
		HalconCpp::HTuple hv_Sigma,
		HalconCpp::HTuple hv_Threshold,
		HalconCpp::HTuple hv_Transition,
		HalconCpp::HTuple hv_Select,
		HalconCpp::HTuple *hv_EdgeRows,
		HalconCpp::HTuple *hv_EdgeColumns,
		HalconCpp::HTuple *hv_ResultRow,
		HalconCpp::HTuple *hv_ResultColumn);

	//////////////////////////////////////////////////////////////
	// DIS��ROI��ELEMENT Ԫ��
	void DisOnEleLineAdd();
	void DisOnEleCircleAdd();
	void DisOnEleAngle();
	void DisOnEleDistance();
	void DisMeaEleFunction();
	void DisMeaEleMatching();
	void DisOnMeaEleBalloonLength();
	void DisOnMeaEleBalloonFlaw();  // ���ұ���ȱ�ݼ��
	void DisOnMeaEleAddRoi();       // ȫ��ROI

	//void DisProcessROIRegion(HalconCpp::HRegion&, HalconCpp::HRegion&);
	//void DisProcessROImodel(HalconCpp::HImage&, HalconCpp::HRegion&);


	void DisOnButtonGenRectifyMap();

	void DisOnSaveOnCaliPicture();                        // ����һ���궨ͼ��
																// ��������ӿ�
	void DisOnOpenImage();                                      // ��һ��ͼ��
	void DisOnSaveCurrentDISPtoFile();                          // ���浱ǰ��ͼ��
	void DisOnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	void DisOnMouseLeftDown(UINT nFlags, CPoint point);
	void DisOnMouseMove(UINT nFlags, CPoint point);
	void DisOnRButtonDown(UINT nFlags, CPoint point);
	void DisOnRButtonUp(UINT nFlags, CPoint pointC);

	void DisOnCamCapMulti();

	void DisOnButCaliShowInfo(int i); // ��ʾ��ǰ�ı궨��Ϣ

	void DisOnButCamCaliInner(int prID);					            // ����궨�ڲ�����
	void DisOnButCamCaliPose(int prID);                                 // ����궨pose����     


	void dev_display_shape_matching_results(
		HalconCpp::HShapeModel hv_ModelID,
		HalconCpp::HTuple hv_Color,
		HalconCpp::HTuple hv_Row,
		HalconCpp::HTuple hv_Column,
		HalconCpp::HTuple hv_Angle,
		HalconCpp::HTuple hv_ScaleR,
		HalconCpp::HTuple hv_ScaleC,
		HalconCpp::HTuple hv_Model);

	//=============================================================================
	// HALCON ͨ�ú���						            


};
