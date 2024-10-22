
#include "pch.h"
#include "checkPatch.h"
#include "LightPlanePose.h"

CThreadMgr g_threadMgr;
//16bit--OpenCV MatתHalcon HObject
HObject MatToHImage16Bit(Mat& imgMat)
{
	HObject Hobj = HObject();
	int height = imgMat.rows;
	int width = imgMat.cols;
	int i;
	//  CV_8UC3
	if (imgMat.type() == CV_16UC3)
	{
		vector<cv::Mat> imgchannel;
		split(imgMat, imgchannel);
		cv::Mat imgB = imgchannel[0];
		cv::Mat imgG = imgchannel[1];
		cv::Mat imgR = imgchannel[2];
		uchar* dataR = new uchar[2 * height * width];
		uchar* dataG = new uchar[2 * height * width];
		uchar* dataB = new uchar[2 * height * width];
		for (i = 0; i < height; i++)
		{
			memcpy(dataR + width * i * 2, imgR.data + imgR.step * i, width * 2);
			memcpy(dataG + width * i * 2, imgG.data + imgG.step * i, width * 2);
			memcpy(dataB + width * i * 2, imgB.data + imgB.step * i, width * 2);
		}
		GenImage3(&Hobj, "uint2", width, height, (Hlong)dataR, (Hlong)dataG, (Hlong)dataB);
		delete[]dataR;
		delete[]dataG;
		delete[]dataB;
	}
	if (imgMat.type() == CV_16UC1)
	{
		uchar* data = new uchar[2 * height * width];
		for (i = 0; i < height; i++)
			memcpy(data + width * i * 2, imgMat.data + imgMat.step * i, width * 2);
		GenImage1(&Hobj, "uint2", width, height, (Hlong)data);
		delete[] data;
	}
	if (imgMat.type() == CV_32FC3)
	{
		vector<cv::Mat> imgchannel;
		split(imgMat, imgchannel);
		cv::Mat imgB = imgchannel[0];
		cv::Mat imgG = imgchannel[1];
		cv::Mat imgR = imgchannel[2];
		uchar* dataR = new uchar[4 * height * width];
		uchar* dataG = new uchar[4 * height * width];
		uchar* dataB = new uchar[4 * height * width];
		for (i = 0; i < height; i++)
		{
			memcpy(dataR + width * i * 4, imgR.data + imgR.step * i, width * 4);
			memcpy(dataG + width * i * 4, imgG.data + imgG.step * i, width * 4);
			memcpy(dataB + width * i * 4, imgB.data + imgB.step * i, width * 4);
		}
		GenImage3(&Hobj, "real", width, height, (Hlong)dataR, (Hlong)dataG, (Hlong)dataB);
		delete[]dataR;
		delete[]dataG;
		delete[]dataB;
	}
	//  CV_8UCU1
	else if (imgMat.type() == CV_32FC1)
	{
		uchar* data = new uchar[4 * height * width];
		for (i = 0; i < height; i++)
			memcpy(data + width * i * 4, imgMat.data + imgMat.step * i, width * 4);
		GenImage1(&Hobj, "real", width, height, (Hlong)data);
		delete[] data;
	}

	return Hobj;
}

//16bit--Halcon HObjectתOpenCV Mat
Mat HImageToMat16Bit(HObject& imgHalcon)
{
	HTuple channels;
	HString cType;
	cv::Mat Image;
	ConvertImageType(imgHalcon, &imgHalcon, "uint2");
	CountChannels(imgHalcon, &channels);
	Hlong width = 0;
	Hlong height = 0;
	if (channels[0].I() == 1)
	{
		HImage hImg(imgHalcon);
		void* ptr = hImg.GetImagePointer1(&cType, &width, &height);//GetImagePointer1(Hobj, &ptr, &cType, &wid, &hgt);
		int W = width;
		int H = height;
		Image.create(H, W, CV_16UC1);
		unsigned char* pdata = static_cast<unsigned char*>(ptr);
		memcpy(Image.data, pdata, W * H);
	}
	else if (channels[0].I() == 3)
	{
		void* Rptr;
		void* Gptr;
		void* Bptr;
		HImage hImg(imgHalcon);
		hImg.GetImagePointer3(&Rptr, &Gptr, &Bptr, &cType, &width, &height);
		int W = width;
		int H = height;
		Image.create(H, W, CV_16UC3);
		vector<cv::Mat> VecM(3);
		VecM[0].create(H, W, CV_16UC1);
		VecM[1].create(H, W, CV_16UC1);
		VecM[2].create(H, W, CV_16UC1);
		unsigned char* R = (unsigned char*)Rptr;
		unsigned char* G = (unsigned char*)Gptr;
		unsigned char* B = (unsigned char*)Bptr;
		memcpy(VecM[2].data, R, W * H);
		memcpy(VecM[1].data, G, W * H);
		memcpy(VecM[0].data, B, W * H);
		cv::merge(VecM, Image);
	}
	return Image;
}
Mat HImageToMat32Bit(HObject& imgHalcon)
{
	HTuple channels;
	HString cType;
	cv::Mat Image;
	ConvertImageType(imgHalcon, &imgHalcon, "uint2");
	CountChannels(imgHalcon, &channels);
	Hlong width = 0;
	Hlong height = 0;
	if (channels[0].I() == 1)
	{
		HImage hImg(imgHalcon);
		void* ptr = hImg.GetImagePointer1(&cType, &width, &height);//GetImagePointer1(Hobj, &ptr, &cType, &wid, &hgt);
		int W = width;
		int H = height;
		Image.create(H, W, CV_32FC1);
		unsigned char* pdata = static_cast<unsigned char*>(ptr);
		memcpy(Image.data, pdata, W * H);
	}
	else if (channels[0].I() == 3)
	{
		void* Rptr;
		void* Gptr;
		void* Bptr;
		HImage hImg(imgHalcon);
		hImg.GetImagePointer3(&Rptr, &Gptr, &Bptr, &cType, &width, &height);
		int W = width;
		int H = height;
		Image.create(H, W, CV_32FC3);
		vector<cv::Mat> VecM(3);
		VecM[0].create(H, W, CV_32FC1);
		VecM[1].create(H, W, CV_32FC1);
		VecM[2].create(H, W, CV_32FC1);
		unsigned char* R = (unsigned char*)Rptr;
		unsigned char* G = (unsigned char*)Gptr;
		unsigned char* B = (unsigned char*)Bptr;
		memcpy(VecM[2].data, R, W * H);
		memcpy(VecM[1].data, G, W * H);
		memcpy(VecM[0].data, B, W * H);
		cv::merge(VecM, Image);
	}
	return Image;
}


HImage Mat2HObject(cv::Mat& image)
{
	HImage Hobj = HImage();
	if (NULL == image.data)
		return Hobj;
	int h = image.rows;
	int w = image.cols;
	//	int al=h*w;
	//	CV_8UC3
	if (image.type() == CV_8UC3)
	{
		cv::Mat imgchannel[3];
		split(image, imgchannel);
		cv::Mat imgB = imgchannel[0];
		cv::Mat imgG = imgchannel[1];
		cv::Mat imgR = imgchannel[2];

		try
		{
			GenImage3(&Hobj, "byte", (Hlong)w, (Hlong)h, (Hlong)imgR.data, (Hlong)imgG.data, (Hlong)imgB.data);
		}
		catch (...)
		{
			//ofstream fp;
			//fp.open("./err.txt",ios::app);
			//fp << "halcon GenImage3 err" << /*"   " << (*(HTuple*)pa.m_dllPa.p).ToString()<<*/endl;
			//fp.close();
		}

	}
	//	CV_8UCU1
	else if (image.type() == CV_8UC1)
	{
		try
		{
			GenImage1(&Hobj, "byte", (Hlong)w, (Hlong)h, (Hlong)image.data);
		}
		catch (...)
		{
			//ofstream fp;
			//fp.open("./err.txt",ios::app);
			//fp << "halcon GenImage1 err" << /*"   " << (*(HTuple*)pa.m_dllPa.p).ToString()<<*/endl;
			//fp.close();
		}

	}
	return Hobj;
}
cv::Mat HObject2Mat(HObject& Hobj)
{
	HTuple htCh = HTuple();
	HTuple cType;
	cv::Mat Image;
	ConvertImageType(Hobj, &Hobj, "byte");
	CountChannels(Hobj, &htCh);
	HTuple wid;
	HTuple hgt;
	int W, H;
	if (htCh[0].I() == 1)
	{
		HTuple ptr;
		GetImagePointer1(Hobj, &ptr, &cType, &wid, &hgt);
		W = wid[0].I();
		H = hgt[0].I();
		Image.create(H, W, CV_8UC1);
		uchar* pdata = (uchar*)(Hlong)ptr;
		memcpy(Image.data, pdata, W * H);
	}
	else if (htCh[0].I() == 3)
	{
		HTuple ptrR, ptrG, ptrB;
		GetImagePointer3(Hobj, &ptrR, &ptrG, &ptrB, &cType, &wid, &hgt);
		W = wid[0].I();
		H = hgt[0].I();

		Image.create(H, W, CV_8UC3);
		vector<cv::Mat> vecM(3);
		vecM[2].create(H, W, CV_8UC1);
		vecM[1].create(H, W, CV_8UC1);
		vecM[0].create(H, W, CV_8UC1);
		uchar* pr = (uchar*)(Hlong)ptrR;
		uchar* pg = (uchar*)(Hlong)ptrG;
		uchar* pb = (uchar*)(Hlong)ptrB;
		memcpy(vecM[2].data, pr, W * H);
		memcpy(vecM[1].data, pg, W * H);
		memcpy(vecM[0].data, pb, W * H);
		merge(vecM, Image);
	}
	return Image;
}

checkPatch::checkPatch()
{
	m_pDlg = NULL;
	m_curPa = 0;
	m_configAdd = "";
	bIsInROIdrawing = false;
	iCOMBO_MATCH_ROI_TYPE = ROI_UNITE;
	m_runing = false;
}

checkPatch::~checkPatch()
{
}


//ͼƬ�Ĵ���	һ����ģ��ͼƬ��������ģ��ͼƬ������ת�ǶȺ�ƥ��
bool checkPatch::treatPicture(CTpltParam& pa, vector<S_TpltData*>& vctData)
{
	int curid = pa.m_tempPa.m_curPosID;
	vector<cv::Mat>& src = vctData[curid]->m_vctPic;
	if (!src[0].data)
		return false;

	if (pa.m_dllPa.ini < 1)
	{
		if ("" != pa.m_dllPa.szfile)
		{
			readMode(pa.m_dllPa.szfile);
		}
		float h = src[0].rows;
		float w = src[0].cols;
		m_pa.hv_CamParam.Clear();
		m_pa.hv_CamParam[0] = 0.01;//���� m
		m_pa.hv_CamParam[1] = 0;//Kappa(-1/m^2)
		m_pa.hv_CamParam[2] = 1;//�������صĿ�m��
		m_pa.hv_CamParam[3] = 1;//�������صĸߣ�m��
		m_pa.hv_CamParam[4] = w / 2;
		m_pa.hv_CamParam[5] = h / 2;
		m_pa.hv_CamParam[6] = w;
		m_pa.hv_CamParam[7] = h;
		if (pa.m_dllPa.ini == 0)
		{
			;
		}
		else//ini == -1
		{
			if (src.size() < 2)
				return false;
			C3dMeatureDlg dlg;
			dlg.m_pCheck = this;
			m_ImageOrg = Mat2HObject(src[0]);
			int treatid = pa.m_dllPa.mutilMat;
			if (treatid < 0)
			{
				treatid = src.size() - 1;
			}
			m_deepImage = MatToHImage16Bit(src[treatid]);
			if (1 == dlg.DoModal())
			{
				writeMode(pa.m_dllPa.szfile);
			}
		}
		pa.m_dllPa.ini = 1;
		return true;
	}
	else
	{
		if (src.size() < 2)
			return false;
		//����ͼƬ
		if (treatPic(vctData[curid], pa.m_dllPa.mutilMat))
		{
			vctData[curid]->m_checkRet = true;
			return true;
		}
	}
	return false;
}
//ȥ���ƽ �ܶȷ�����ƽ��
void gen_light_adjusting(HTuple& hv_ObjectModel3D, HTuple& hv_ObjectModel3DAffineTrans)
{
	HTuple  hv_WindowHandle;
	HTuple  hv_ObjectModel3DConnected, hv_GenParamValue;
	HTuple  hv_ObjectModel3DSelected, hv_PoseOut1, hv_pose;
	HTuple  hv_PoseInvert, hv_HomMat3D;
	//2 ɸѡģ�ͣ�����Χ����Щ�㶼ɾ��
	ConnectionObjectModel3d(hv_ObjectModel3D, "distance_3d", 1, &hv_ObjectModel3DConnected);

	//������ά����ģ�͵����ԡ�  �õ�����  num_points Ȼ���õ���ɸѡ��ģ��
	GetObjectModel3dParams(hv_ObjectModel3DConnected, "num_points", &hv_GenParamValue);
	SelectObjectModel3d(hv_ObjectModel3DConnected, "num_points", "and", 200000, 2e7,
		&hv_ObjectModel3DSelected);
	//��ʾɸѡ֮���3dģ����ʾ  select3D object
	visualize_object_model_3d(hv_WindowHandle, hv_ObjectModel3DSelected, HTuple(),
		HTuple(), "disp_pose", "true", HTuple(), HTuple(), HTuple(), &hv_PoseOut1);
	//3 �����Ƽ��ϱ任��ԭʼ����ϵ�� ������  x y  Z
	MomentsObjectModel3d(hv_ObjectModel3DSelected, "principal_axes", &hv_pose);
	//�������̬���仯��������ϵ��Ь�ӵ��Ƶ�������
	PoseInvert(hv_pose, &hv_PoseInvert);
	//���Ƶĸ���任
	//rigid_trans_object_model_3d (ObjectModel3DSelected, PoseInvert, ObjectModel3DRigidTrans)
	// �÷���任��ʵ��
	PoseToHomMat3d(hv_PoseInvert, &hv_HomMat3D);
	AffineTransObjectModel3d(hv_ObjectModel3DSelected, hv_HomMat3D, &hv_ObjectModel3DAffineTrans);
	visualize_object_model_3d(hv_WindowHandle, hv_ObjectModel3DAffineTrans, HTuple(),
		HTuple(), "disp_pose", "true", "����任֮��ĵ���", HTuple(), HTuple(),
		&hv_PoseOut1);
}
void fit_3d_plane_xyz1(HTuple hv_X, HTuple hv_Y, HTuple hv_Z, HTuple* hv_Ox, HTuple* hv_Oy,
	HTuple* hv_Oz, HTuple* hv_Nx, HTuple* hv_Ny, HTuple* hv_Nz, HTuple* hv_MeanResidual)
{

	// Local iconic variables

	// Local control variables
	HTuple  hv_Size, hv_MatrixID_Mt, hv_MatrixID_M;
	HTuple  hv_MatrixID_U, hv_MatrixID_S, hv_MatrixID_V, hv_x;
	HTuple  hv_i, hv_j, hv_x1, hv_SingularvaluesOfM, hv_Indices;
	HTuple  hv_N, hv_MatrixID_N, hv_MatrixID_MN, hv_Distances;

	//This procedure fits a 3D-plane into a set of 3D-points
	//
	//The procedure returns the coordinates [Ox, Oy,Oz] of
	//the centroid of the provided input coordinates, and
	//the coordinates [Nx, Ny,Nz] of the normal vector to
	//the fitted plane.
	//
	//WARNING: If the system of equations is under-determined
	//(i.e. if it has too few input coordinates in X, Y, Z),
	//it cannot be solved and the procedure returns empty tuples
	//for X, Y, and Z
	//
	//�˳���һ��3d ƽ����ϳ�һ��3d �㣬�ó��򷵻����ṩ�������������������[ ox��oy��oz ]
	//�Լ����ƽ��ķ���������[ nx��ny��nz ]��
	//����: ���������ȷ������(�� x��y��z �е���������̫��) �����޷���⣬���򷵻� x��y �� z �Ŀ�Ԫ��
	//Perform some initializations
	(*hv_Ox) = HTuple();
	(*hv_Oy) = HTuple();
	(*hv_Oz) = HTuple();
	(*hv_Nx) = HTuple();
	(*hv_Ny) = HTuple();
	(*hv_Nz) = HTuple();
	(*hv_MeanResidual) = HTuple();
	//
	//Test the size of X, Y and Z, and return if necessary
	hv_Size = hv_X.TupleLength();
	if (0 != (HTuple(HTuple(hv_Size < 3).TupleOr(hv_Size != (hv_Y.TupleLength()))).TupleOr(hv_Size != (hv_Z.TupleLength()))))
	{
		return;
	}
	//
	//Compute the coordinates of the centroid point.
	//�������ĵ������.���ֵ
	TupleMean(hv_X, &(*hv_Ox));
	TupleMean(hv_Y, &(*hv_Oy));
	TupleMean(hv_Z, &(*hv_Oz));
	//
	//Setup the equation system as a matrix M and compute
	//its singular value decomposition. The singular vector
	//of M corresponding to its smallest singular value provides
	//the coordinates of the normal vector of the fitted plane.
	//������ʽϵͳ�趨Ϊ���� m ������������ֵ�ֽ⡣
	//��Ӧ����С����ֵ�� m ������ʸ���ṩ�����ƽ�淨���������ꡣ
	//stop ()
	//֮ǰ281* 2  562 *3  1686
	//column := |X|
	CreateMatrix(3, hv_X.TupleLength(), ((hv_X - (*hv_Ox)).TupleConcat(hv_Y - (*hv_Oy))).TupleConcat(hv_Z - (*hv_Oz)),
		&hv_MatrixID_Mt);
	//get_full_matrix (MatrixID_Mt, MatrixID_Mt_value)
	//����ת��
	TransposeMatrix(hv_MatrixID_Mt, &hv_MatrixID_M);
	//����SVD���
	SvdMatrix(hv_MatrixID_M, "reduced", "right", &hv_MatrixID_U, &hv_MatrixID_S, &hv_MatrixID_V);
	//
	//�Ӿ����ڲ��ҵ���Ӧ����ֵ
	hv_x = HTuple();
	//
	for (hv_i = 0; hv_i <= 2; hv_i += 1)
	{
		for (hv_j = 0; hv_j <= 2; hv_j += 1)
		{
			GetValueMatrix(hv_MatrixID_S, hv_i, hv_j, &hv_x1);
			hv_x = hv_x.TupleConcat(hv_x1);
		}
	}
	//stop ()
	//SingularvaluesOfM  ����ֵ
	GetValueMatrix(hv_MatrixID_S, ((HTuple(0).Append(1)).Append(2)), ((HTuple(0).Append(1)).Append(2)),
		&hv_SingularvaluesOfM);
	//
	//��s ��������
	TupleSortIndex(hv_SingularvaluesOfM, &hv_Indices);
	//
	//Test if more than one singular value of M is (nearly) equal
	//to zero. This indicates that the provided 3d points are
	//inappropriate to fit the plane (e.g. they are nearly
	//collinear or reduce to a single point).
	if (0 != (HTuple(HTuple(hv_SingularvaluesOfM[HTuple(hv_Indices[0])]) < 1e-9).TupleAnd(HTuple(hv_SingularvaluesOfM[HTuple(hv_Indices[1])]) < 1e-9)))
	{
		return;
	}
	//
	//Get coordinates of the normal vector to the fitted plane
	//
	hv_x = ((const HTuple&)hv_Indices)[0];
	//*�Ӿ����ڲ��ҵ���Ӧ����ֵ
	GetValueMatrix(hv_MatrixID_V, ((HTuple(0).Append(1)).Append(2)), (HTuple(hv_Indices[0]).TupleConcat(HTuple(hv_Indices[0]))).TupleConcat(HTuple(hv_Indices[0])),
		&hv_N);
	//����3 ��  ��һ�еľ���
	CreateMatrix(3, 1, hv_N, &hv_MatrixID_N);
	(*hv_Nx) = ((const HTuple&)hv_N)[0];
	(*hv_Ny) = ((const HTuple&)hv_N)[1];
	(*hv_Nz) = ((const HTuple&)hv_N)[2];
	//
	//Compute the mean residual distance between the 3d points
	//and the fitted plane, in order to guess the quality of
	//the fitted plane:
	//������ά�������ƽ��֮���ƽ��ʣ����룬�Թ������ƽ�������
	//multiplication �˷�
	MultMatrix(hv_MatrixID_M, hv_MatrixID_N, "AB", &hv_MatrixID_MN);
	//
	//��ȡ���������ֵ
	GetFullMatrix(hv_MatrixID_MN, &hv_Distances);
	hv_Distances = hv_Distances.TupleAbs();
	(*hv_MeanResidual) = (hv_Distances.TupleSum()) / hv_Size;
	//
	//Clear the matrices used in the procedure
	ClearMatrix((((((hv_MatrixID_MN.TupleConcat(hv_MatrixID_N)).TupleConcat(hv_MatrixID_V)).TupleConcat(hv_MatrixID_S)).TupleConcat(hv_MatrixID_U)).TupleConcat(hv_MatrixID_M)).TupleConcat(hv_MatrixID_Mt));
	return;
}
void get_light_plane_pose(HTuple hv_OriginX, HTuple hv_OriginY, HTuple hv_OriginZ,
	HTuple hv_NormalVectorX, HTuple hv_NormalVectorY, HTuple hv_NormalVectorZ, HTuple* hv_LightPlanePose)
{

	// Local iconic variables

	// Local control variables
	HTuple  hv_Norm, hv_HomMat3DIdentity, hv_Alpha;
	HTuple  hv_HomMat3DRotateAlpha, hv_N1x, hv_N1y, hv_N1z;
	HTuple  hv_Beta, hv_HomMat3DRotateBeta, hv_HomMat3DTranslate;
	HTuple  hv_HomMat3DTmp, hv_HomMat3DWorldToLightplane;

	//This procedure determines a lightplane pose,
	//e.g. a pose whose plane defined by z=0 coincides
	//with the physical light plane.
	//
	//
	//������̾�����һ����ƽ������ƣ�
	//����һ���� z 0�����ƽ���������ƽ��һ�µ����ơ�
	//stop ()
	//Test that the vector passed at input is not null
	//���������봦���ݵ������Ƿ�Ϊ��
	(*hv_LightPlanePose) = HTuple();
	hv_Norm = ((hv_NormalVectorX * hv_NormalVectorX) + (hv_NormalVectorY * hv_NormalVectorY)) + (hv_NormalVectorZ * hv_NormalVectorZ);
	if (0 != ((hv_Norm.TupleAbs()) < 1e-8))
	{
		return;
	}
	//
	//In order to compute a light-plane pose, we determine
	//two rotations which align the unit vector of the z-axis
	//to the normal vector of the light plane, when applied
	//successively. For example, we can compute the angles
	//Alpha (rotation around the x-axis) and Beta (subsequent
	//rotation around the y-axis) in this successive order.
	//(The rotation around the z-axis is arbitrarily set
	//to zero).
	//
	//Determine the value of the angle Alpha and rotate the
	//normal vector to the plane y=0. This provides the
	//vector N1.
	//Ϊ�˼���һ����ƽ�����̬������ȷ����������ת��ʹ z ��ĵ�λ�������ƽ��ķ�������һ�£�������ʩ��ʱ��
	//���磬���ǿ��Լ���Ƕ� �� (Χ�� x ����ת)�� �� (���Χ�� y ����ת)�����������˳�� (Χ�� z �����ת����������Ϊ��)��
	//ȷ���� �� ��ֵ����������������ת��ƽ�� y 0�� ���ṩ������ n1��
	//
	HomMat3dIdentity(&hv_HomMat3DIdentity);
	//NY NZ  ������
	TupleAtan2(hv_NormalVectorY, hv_NormalVectorZ, &hv_Alpha);
	HomMat3dRotate(hv_HomMat3DIdentity, hv_Alpha, "x", 0, 0, 0, &hv_HomMat3DRotateAlpha);
	//
	//��NX NY NZ  �任Ϊ N1x, N1y, N1z ��
	AffineTransPoint3d(hv_HomMat3DRotateAlpha, hv_NormalVectorX, hv_NormalVectorY,
		hv_NormalVectorZ, &hv_N1x, &hv_N1y, &hv_N1z);
	//
	//Determine the value of the angle Beta by using the
	//coordinates of N1. Note that the rotation around the
	//y-axis with angle Beta is carried out in the counter
	//trigonometric direction, therefore we apply -Beta.
	//ͨ��ʹ�� n1��������ȷ���� �� ��ֵ�� ע���� y ���ת�������ŷ����Ƿ�����еģ��������Ӧ��-�¡�
	//
	TupleAtan2(hv_N1x, hv_N1z, &hv_Beta);
	HomMat3dRotate(hv_HomMat3DIdentity, -hv_Beta, "y", 0, 0, 0, &hv_HomMat3DRotateBeta);
	//
	//Create the LightPlanePose
	//1. HomMat3DIdentity  ƽ��  ----��omMat3DTranslate
	HomMat3dTranslate(hv_HomMat3DIdentity, -hv_OriginX, -hv_OriginY, -hv_OriginZ, &hv_HomMat3DTranslate);
	//
	//2  omMat3DTranslate +  ���� �� �� ���----��HomMat3DTmp
	HomMat3dCompose(hv_HomMat3DRotateAlpha, hv_HomMat3DTranslate, &hv_HomMat3DTmp);
	//
	//3 HomMat3DTmp +���� �� �� ���----��HomMat3DWorldToLightplane
	HomMat3dCompose(hv_HomMat3DRotateBeta, hv_HomMat3DTmp, &hv_HomMat3DWorldToLightplane);
	//
	//����α任���� ת��Ϊpose
	HomMat3dToPose(hv_HomMat3DWorldToLightplane, &(*hv_LightPlanePose));
	return;
	//
}

bool getLightPlanePose(HTuple hv_X, HTuple hv_Y, HTuple hv_Z, HTuple* hv_LightPlanePose)
{
	// Local control variables
	HTuple  hv_Ox, hv_Oy, hv_Oz, hv_Nx, hv_Ny, hv_Nz;
	HTuple  hv_MeanResidual, hv_String;

	fit_3d_plane_xyz1(hv_X, hv_Y, hv_Z, &hv_Ox, &hv_Oy, &hv_Oz, &hv_Nx, &hv_Ny, &hv_Nz,
		&hv_MeanResidual);
	if (0 != (HTuple(HTuple((hv_Nx.TupleLength()) == 0).TupleOr((hv_Ny.TupleLength()) == 0)).TupleOr((hv_Nz.TupleLength()) == 0)))
	{
		return false;
	}
	//���� �ߵĵ� ����Ϲ�ƽ�档
	get_light_plane_pose(hv_Ox, hv_Oy, hv_Oz, hv_Nx, hv_Ny, hv_Nz, &(*hv_LightPlanePose));
	if (0 != (((*hv_LightPlanePose).TupleLength()) != 7))
	{
		return false;
	}
	return true;
}

bool gen_LightPlanePose(HTuple hv_X, HTuple hv_Y, HTuple hv_Z, HTuple hv_WindowHandle,
	HTuple* hv_LightPlanePose)
{

	// Local iconic variables

	// Local control variables
	HTuple  hv_Ox, hv_Oy, hv_Oz, hv_Nx, hv_Ny, hv_Nz;
	HTuple  hv_MeanResidual, hv_String;

	//Fit the light plane in the 3D coordinates of the line
	//points computed previously. Note that this requires
	//nearly coplanar points. We must provide line points
	//recorded at -at least- two different heights, in order
	//to get an unambiguous solution. To obtain stable and
	//accurate results, acquire the light line points at the
	//bottom and at the top of the measurement volume.
	//����ǰ������ߵ����ά��������Ϲ�ƽ�档 ע���⼸���ǹ���ġ�
	//Ϊ�˵õ�һ����ȷ�Ľ�����������Ǳ����ṩ����������ͬ�߶ȵ��ߵ��¼��
	//Ϊ�˻���ȶ���׼ȷ�Ĳ����������Ҫ��ò�����������͵ײ��Ĺ��ߵ㡣
	//
	//stop ()
	//
	//��һ��ƽ��ĵ� ����ȫ������ȥ�� ���صľ��� ��������ͷ���������ϵ���������ʵ��ֵ����
	//MeanResidual  ��Ϊƽ��� ������
	fit_3d_plane_xyz1(hv_X, hv_Y, hv_Z, &hv_Ox, &hv_Oy, &hv_Oz, &hv_Nx, &hv_Ny, &hv_Nz,
		&hv_MeanResidual);
	if (0 != (HTuple(HTuple((hv_Nx.TupleLength()) == 0).TupleOr((hv_Ny.TupleLength()) == 0)).TupleOr((hv_Nz.TupleLength()) == 0)))
	{
		disp_message(hv_WindowHandle, "Too few 3d points have been provided to fit the light plane,\nor the points are (nearly) collinear!\nThe program will exit.",
			"window", 12, 12, "black", "true");
		return false;
	}
	//if (MeanResidual > 8e-5)
	  //disp_message (WindowHandle, 'The light plane could not be fitted accurately!\nThe mean residual distance between the 3d-points and the\nfitted plane is too high (' + (MeanResidual * 1000)$'.3' + 'mm). Please check the\nquality and the correctness of those points.\nThe program will exit!', 'window', 12, 21, 'black', 'true')
	  //return ()

	//endif
	//
	//Compute the light plane pose: this pose must be oriented
	//such that the plane defined by z=0 coincides with the
	//light plane.
	//���� �ߵĵ� ����Ϲ�ƽ�档
	get_light_plane_pose(hv_Ox, hv_Oy, hv_Oz, hv_Nx, hv_Ny, hv_Nz, &(*hv_LightPlanePose));
	if (0 != (((*hv_LightPlanePose).TupleLength()) != 7))
	{
		disp_message(hv_WindowHandle, "The pose of the light plane could not be\ndetermined. Please verify that the vector\npassed at input of the procedure\nget_light_plane_pose() is not null.\nThe program will exit!",
			"window", -1, -2, "black", "true");
		return false;
	}
	hv_String.Clear();
	hv_String[0] = "LightPlanePose: ";
	hv_String.Append(("  Tx    = " + (HTuple((*hv_LightPlanePose)[0]).TupleString(".3"))) + " m");
	hv_String.Append(("  Ty    = " + (HTuple((*hv_LightPlanePose)[1]).TupleString(".3"))) + " m");
	hv_String.Append(("  Tz    = " + (HTuple((*hv_LightPlanePose)[2]).TupleString(".3"))) + " m");
	hv_String.Append(("  alpha = " + (HTuple((*hv_LightPlanePose)[3]).TupleString(".4"))) + "��");
	hv_String.Append(("  beta  = " + (HTuple((*hv_LightPlanePose)[4]).TupleString(".4"))) + "��");
	hv_String.Append(("  gamma = " + (HTuple((*hv_LightPlanePose)[5]).TupleString(".4"))) + "��");
	hv_String.Append("  type  = " + HTuple((*hv_LightPlanePose)[6]));
	disp_message(hv_WindowHandle, hv_String, "window", 12, 12, "black", "true");
	disp_continue_message(hv_WindowHandle, "black", "true");
	return true;
}

//ȥ���ƽ ���ɷַ������ƽ��
bool checkPatch::gen_self_light_adjusting(HTuple& hv_ObjectModel3D, HTuple& hv_ObjectModel3DAffineTrans, HTuple* hv_WindowHandle)
{
	// Local control variables
	HTuple  hv_Pose, hv_ObjectModel3DConnected;
	HTuple  hv_GenParamValue1, hv_ObjectModel3DX, hv_LightPlanePose;
	HTuple  hv_point_coord_x1, hv_point_coord_y1, hv_point_coord_z1;

	//���ո߶�  x  y  z ,�߶�ɸѡ
	SelectPointsObjectModel3d(hv_ObjectModel3D, "point_coord_z", m_pa.m_zMin, m_pa.m_zMax, &hv_ObjectModel3D);
	//��ͨ��  ����  ���� 1 mm  ���ָ������
	ConnectionObjectModel3d(hv_ObjectModel3D, "distance_3d", m_pa.m_distance, &hv_ObjectModel3DConnected);
	//������Ŀ��ͳ��
	GetObjectModel3dParams(hv_ObjectModel3DConnected, "num_points", &hv_GenParamValue1);
	if (0 != (hv_GenParamValue1 == 0))
	{
		return false;
	}
	//�õ����Ƹ�������Ǹ�object
	SelectObjectModel3d(hv_ObjectModel3DConnected, "num_points", "and", (hv_GenParamValue1.TupleMax()) - 1,
		INT_MAX, &hv_ObjectModel3DX);

	//��ȡ��ƽ�����̬
	hv_LightPlanePose = HTuple();
	GetObjectModel3dParams(hv_ObjectModel3DX, "point_coord_x", &hv_point_coord_x1);
	GetObjectModel3dParams(hv_ObjectModel3DX, "point_coord_y", &hv_point_coord_y1);
	GetObjectModel3dParams(hv_ObjectModel3DX, "point_coord_z", &hv_point_coord_z1);
	//1.��õķ�������Ϲ�ƽ��
	if (NULL == hv_WindowHandle)
	{
		if (!getLightPlanePose(hv_point_coord_x1, hv_point_coord_y1, hv_point_coord_z1,
			&hv_LightPlanePose))
			return false;
	}
	else
		if(!gen_LightPlanePose(hv_point_coord_x1, hv_point_coord_y1, hv_point_coord_z1, *hv_WindowHandle,
			&hv_LightPlanePose))
			return false;

	RigidTransObjectModel3d(hv_ObjectModel3DX, hv_LightPlanePose, &hv_ObjectModel3DAffineTrans);
	//	SelectPointsObjectModel3d(hv_ObjectModel3DAffineTrans, "point_coord_z", -1, 1, &hv_ObjectModel3DAffineTrans);
		//WriteTuple(hv_LightPlanePose, "./Camera_Pose.tup");
	if (NULL == hv_WindowHandle)
	{
		return true;
	}
	HTuple  hv_GenParamNames, hv_GenParamValues;
	HTuple  hv_Instructions;
	hv_GenParamNames.Clear();
	hv_GenParamNames[0] = "disp_pose";
	hv_GenParamNames[1] = "alpha";
	hv_GenParamValues.Clear();
	hv_GenParamValues[0] = "true";
	hv_GenParamValues[1] = 0.9;

	hv_Instructions[0] = "Rotate: Left button";
	hv_Instructions[1] = "Zoom:   Shift + left button";
	hv_Instructions[2] = "Move:   Ctrl  + left button";

	visualize_object_model_3d(*hv_WindowHandle, hv_ObjectModel3DAffineTrans, HTuple(), HTuple(),
		hv_GenParamNames, hv_GenParamValues, "gen_self_light_adjusting", "", HTuple(),
		&hv_Pose);
	return true;
}

//ƽ��ȼ��� �����Ԫ��������ƽ���
double checkPatch::genFitness(HTuple& hv_ObjectModel3D, HRegion& ho_ROI, HTuple& ParamValue, HTuple& ParamValue2)
{
	// Local control variables
	HTuple  hv_ObjectModel3DReduced;
	HTuple  hv_paraName, hv_paraVal, hv_planeFitted, hv_pointsDis;
	HTuple hv_flatness;

	/*visualize_object_model_3d(hv_WindowHandle, hv_ObjectModel3D, hv_CamParam,
		HTuple(), HTuple(), HTuple(), "", HTuple(), "",
		&hv_Pose,false);*/
	ReduceObjectModel3dByView(ho_ROI, hv_ObjectModel3D, m_pa.hv_CamParam, m_pa.hv_Pose, &hv_ObjectModel3DReduced);
	hv_paraName.Clear();
	hv_paraName[0] = "primitive_type";
	hv_paraName[1] = "fitting_algorithm";
	hv_paraVal.Clear();
	hv_paraVal[0] = "plane";
	hv_paraVal[1] = "least_squares_tukey";
	FitPrimitivesObjectModel3d(hv_ObjectModel3DReduced, hv_paraName, hv_paraVal, &hv_planeFitted);
	GetObjectModel3dParams(hv_planeFitted, "primitive_parameter", &ParamValue);//��ȡ��Ԫ������
	HTuple hvcent;
	GetObjectModel3dParams(hv_planeFitted, "center", &hvcent);//��ȡƽ������ �����d
	float ret = 0;
	for (int i = 0; i < 3; i++)
	{
		ret += ParamValue[i].D() * hvcent[i].D();
	}
	ParamValue[3] = -ret;
	GetObjectModel3dParams(hv_planeFitted, "primitive_pose", &ParamValue2);//��ȡλ�� ƽ���λ��
	//��ȡƽ���
	DistanceObjectModel3d(hv_ObjectModel3DReduced, hv_planeFitted, HTuple(), 0, HTuple(),
		HTuple());
	GetObjectModel3dParams(hv_ObjectModel3DReduced, "&distance", &hv_pointsDis);
	//ƽ���=���˸о�Ӧ��������ֵ mean(pointsDis)  �򷽲�deviation(pointsDis)
	//hv_flatness = ((hv_pointsDis.TupleMax()) - (hv_pointsDis.TupleMin())).TupleAbs();
	hv_flatness = hv_pointsDis.TupleDeviation();//TupleMean()
	return hv_flatness.D();
}

//�����������ļн� ������ƽ��ļн� 180��֮�ڵļн�
float getAngle(HTuple  hval, HTuple  hval2)
{
	float ret = 0;
	for (int i = 0; i < 3; i++)
		ret += hval[i].D() * hval2[i].D();
	/*if (ret < -1 || ret > 1)
	{
		return 0;
	}*/
	ret = acos(ret) * 180 / PI;
	if (ret > 90)//תΪ���
	{
		ret = 180 - ret;
	}
	return ret;
}
//ƽ��߶�
float getHeigh(HObject  ho_Zmap, HRegion ho_TopRoiRegion)
{
	// Local iconic variables
	HTuple  hv_TopMean;
	HTuple  hv_TopDeviation;
	Intensity(ho_TopRoiRegion, ho_Zmap, &hv_TopMean, &hv_TopDeviation);
	return hv_TopMean.D();
	////����1  �����к���  �ǶȽ��0.808175
	//HTuple hv_rate = (hv_BottomMean - hv_TopMean) / 1000;
	////rad  ����  ��3.1415926��   �Ƕ�  ��360��
	//HTuple hv_k = hv_rate.TupleAtan();
	//TupleDeg(hv_k, &hv_Deg1);

	////����2 �Ƕȼ���  -89.1918   0.808175
	//AngleLx(0, hv_TopMean, 1000, hv_BottomMean, &hv_HightAngle);
	//TupleDeg(hv_HightAngle, &hv_Deg);
	//HTuple hv_Deg2 = hv_Deg + 90;
}
//����
float getMax(HObject  ho_Zmap, HRegion ho_TopRoiRegion)
{
	// Local iconic variables
	HTuple  hv_Max;
	HTuple  hv_Min;
	HTuple hv_Range;
	MinMaxGray(ho_TopRoiRegion, ho_Zmap, 0, &hv_Max, &hv_Min, &hv_Range);
	return hv_Max.D();
}

//�㵽ƽ��ľ���
float point2planeDis(HTuple  hvPoint, HTuple  hvPlane)
{
	float ret = hvPlane[3].D();
	float fdv = 0;
	for (int i = 0; i < 3; i++)
	{
		ret += hvPoint[i].D() * hvPlane[i].D();
		fdv += hvPlane[i].D() * hvPlane[i].D();
	}
	return fabs(ret) / fdv;
}
//��Ƭͼ
void section(HTuple  hv_ObjectModel3DAffineTrans, HObject& ho_cutImage, HTuple hv_CutPose, HTuple* hv_WinHandle = NULL)
{
	// Local iconic variables
	HObject  ho_IntersectionXld, ho_EdgeContour;
	HObject ho_Intersection;
	// Local control variables
	HTuple  hv_TriangulatedObjectModel3D, hv_Information;
	HTuple  hv_PoseOutbox, hv_Length1, hv_Length2, hv_Length3;
	HTuple  hv_PoseInvertBox, hv_TransObjectModel3D;
	HTuple  hv_PoseBox, hv_PoseBoxOriented, hv_Pose;
	HTuple  hv_CutPlanePose, hv_ObjectModel3D1, hv_ObjectModel3DIntersection;
	HTuple  hv_GenParamValue1, hv_PoseOut2, hv_PoseInvertXLD, hv_Diameter;
	HTuple  hv_Scale, hv_CamParam, hv_Number, hv_Rows, hv_Columns;
	HTuple  hv_Row, hv_Column, hv_I, hv_Indices, hv_Length;
	HTuple  hv_OrderRow, hv_OrderColumn, hv_Row_Index;

	//4�����ǻ�   ��ά�ؽ� ����ȽϹ⻬��  �ܶ�һ����
	TriangulateObjectModel3d(hv_ObjectModel3DAffineTrans, "greedy", HTuple(), HTuple(),
		&hv_TriangulatedObjectModel3D, &hv_Information);

	//5������С���������
	SmallestBoundingBoxObjectModel3d(hv_TriangulatedObjectModel3D, "oriented", &hv_PoseOutbox,
		&hv_Length1, &hv_Length2, &hv_Length3);
	PoseInvert(hv_PoseOutbox, &hv_PoseInvertBox);
	RigidTransObjectModel3d(hv_TriangulatedObjectModel3D, hv_PoseInvertBox, &hv_TransObjectModel3D);
	//��box �� ����ģ�Ͷ���ʾһ��,Ϊ���ǽ�����ģ�͵�����ϵ������������� ��һ����׼λ��
	SmallestBoundingBoxObjectModel3d(hv_TransObjectModel3D, "oriented", &hv_PoseBox,
		&hv_Length1, &hv_Length2, &hv_Length3);
	GenBoxObjectModel3d(hv_PoseBox, hv_Length1, hv_Length2, hv_Length3, &hv_PoseBoxOriented);
	//6 ����ƽ��
	//�����и���
	{
		hv_CutPlanePose = hv_PoseBox;
		if (hv_CutPose[0] > 0)
			hv_CutPlanePose[0] = (HTuple(hv_PoseBox[0]) - (hv_Length1 / 2)) + hv_CutPose[0];//x�����и�
		if (hv_CutPose[1] > 0)
			hv_CutPlanePose[1] = (HTuple(hv_PoseBox[0]) - (hv_Length2 / 2)) + hv_CutPose[1];//y�����и�
		if (hv_CutPose[2] > 0)
			hv_CutPlanePose[2] = (HTuple(hv_PoseBox[0]) - (hv_Length3 / 2)) + hv_CutPose[2];//z�����и�
		hv_CutPlanePose[3] = hv_CutPose[3];
		hv_CutPlanePose[4] = hv_CutPose[4];
		hv_CutPlanePose[5] = hv_CutPose[5];

		//����һ��ƽ��
		hv_Length = hv_Length3 > hv_Length2 ? hv_Length3 : hv_Length2;
		hv_Length = hv_Length1 > hv_Length ? hv_Length1 : hv_Length;
		GenImageConst(&ho_cutImage, "byte", hv_Length, hv_Length);
		hv_Length1 = hv_Length / 2;
		GenPlaneObjectModel3d(hv_CutPlanePose, (((HTuple(-1).Append(-1)).Append(1)).Append(1)) * hv_Length1,
			(((HTuple(-1).Append(1)).Append(1)).Append(-1)) * hv_Length1, &hv_ObjectModel3D1);

		//��ʾ�����ƽ��
		if (NULL != hv_WinHandle)
		{
			visualize_object_model_3d(*hv_WinHandle, hv_TransObjectModel3D.TupleConcat(hv_PoseBoxOriented).TupleConcat(hv_ObjectModel3D1),
				HTuple(), HTuple(), (((HTuple("color_0").Append("color_1").Append("color_2")).Append("alpha_1")).Append("disp_pose")),
				(((HTuple("green").Append("gray").Append("blue")).Append(0.5)).Append("true")), "��С���������-����-����",
				HTuple(), HTuple(), &hv_Pose);
		}
		//�����ƽ��� ���ǻ�ģ�͵Ľ���
		IntersectPlaneObjectModel3d(hv_TransObjectModel3D, hv_CutPlanePose, &hv_ObjectModel3DIntersection);
		ClearObjectModel3d(hv_TriangulatedObjectModel3D);
		ClearObjectModel3d(hv_ObjectModel3D1);
		ClearObjectModel3d(hv_PoseBoxOriented);
		ClearObjectModel3d(hv_TransObjectModel3D);
		GetObjectModel3dParams(hv_ObjectModel3DIntersection, "num_points", &hv_GenParamValue1);
		if (0 != (hv_GenParamValue1 == 0))
		{
			ClearObjectModel3d(hv_ObjectModel3DIntersection);
			return;
		}
		//��ʾ����ͨ����ƽ��õ��Ľ���ģ��
		if (NULL != hv_WinHandle)
			visualize_object_model_3d(*hv_WinHandle, hv_ObjectModel3DIntersection, HTuple(),
				HTuple(), HTuple(), HTuple(), HTuple(), HTuple(), HTuple(), &hv_PoseOut2);

		//ͨ��ͶӰ�õ�����   �����ߵ� row  ���ڵ��ǽ���ģ�͵�Y��  column ��Ӧ���ǽ���ģ�͵�Z��
		PoseInvert(hv_CutPlanePose, &hv_PoseInvertXLD);
		GetObjectModel3dParams(hv_ObjectModel3DIntersection, "diameter_axis_aligned_bounding_box",
			&hv_Diameter);
		hv_PoseInvertXLD[2] = HTuple(hv_PoseInvertXLD[2]) + hv_Diameter;
		//��ƽ����ͶӰƽ��������1:1�ı�����
		hv_Scale = 1;
		hv_CamParam.Clear();
		hv_CamParam[0] = 0;
		hv_CamParam[1] = 0;
		hv_CamParam.Append(1.0 / hv_Scale);
		hv_CamParam.Append(1.0 / hv_Scale);
		hv_CamParam.Append(0);
		hv_CamParam.Append(0);
		hv_CamParam.Append(500);
		hv_CamParam.Append(500);
		ProjectObjectModel3d(&ho_IntersectionXld, hv_ObjectModel3DIntersection, hv_CamParam,
			hv_PoseInvertXLD, "data", "lines");
		ClearObjectModel3d(hv_ObjectModel3DIntersection);

		// �����е������ĵĵ㶼�ŵ����鵱��
		CountObj(ho_IntersectionXld, &hv_Number);
		hv_Rows = HTuple();
		hv_Columns = HTuple();
		hv_Row = HTuple();
		hv_Column = HTuple();
		{
			HTuple end_val91 = hv_Number;
			HTuple step_val91 = 1;
			for (hv_I = 1; hv_I.Continue(end_val91, step_val91); hv_I += step_val91)
			{
				SelectObj(ho_IntersectionXld, &ho_EdgeContour, hv_I);
				GetContourXld(ho_EdgeContour, &hv_Row, &hv_Column);
				hv_Rows = hv_Rows.TupleConcat(hv_Row);
				hv_Columns = hv_Columns.TupleConcat(hv_Column);
			}
		}

		//���㰴�� Row ������
		TupleSortIndex(hv_Rows, &hv_Indices);
		TupleLength(hv_Rows, &hv_Length2);
		hv_OrderRow = HTuple();
		hv_OrderColumn = HTuple();
		if (0 != (hv_Length2 >= 1))
		{
			{
				HTuple end_val106 = hv_Length2 - 1;
				HTuple step_val106 = 1;
				for (hv_Row_Index = 0; hv_Row_Index.Continue(end_val106, step_val106); hv_Row_Index += step_val106)
				{
					hv_OrderRow = hv_OrderRow.TupleConcat(HTuple(hv_Rows[HTuple(hv_Indices[hv_Row_Index])] + hv_Length1));//hv_Length1ת��ͼƬ��λ��
					hv_OrderColumn = hv_OrderColumn.TupleConcat(HTuple(hv_Columns[hv_Row_Index] + hv_Length1));
				}
			}
		}
		//�γ�ͼƬ
		/*GenContourPolygonXld(&ho_Intersection, hv_OrderRow, hv_OrderColumn);
		GenRegionContourXld(ho_Intersection, &ho_Intersection,"margin");
		GenImageConst(&ho_cutImage, "byte", hv_Length, hv_Length);
		PaintRegion(ho_Intersection, ho_cutImage, &ho_cutImage, 255, "fill");
		return;*/
		GenRegionPolygon(&ho_Intersection, hv_OrderRow, hv_OrderColumn);
		PaintRegion(ho_Intersection, ho_cutImage, &ho_cutImage, 255, "fill");
		if (NULL != hv_WinHandle)
		{
			SetPart(*hv_WinHandle, 0, 0, hv_Length - 1, hv_Length - 1);
			DispObj(ho_cutImage, *hv_WinHandle);
		}
	}
}

bool checkPatch::treatPic(S_TpltData* data,int treatid)
{
	try
	{
		if (treatid < 0)
		{
			treatid = data->m_vctPic.size() - 1;
		}
		if (treatid >= data->m_vctPic.size())
		{
			data->m_szRet = "����ͼ��ų���Χ";
			return false;
		}
		//д������������
		HTuple  hv_ObjectModel3DZ;
		HObject  ho_Xm, ho_Ym, ho_Zm;
		HImage ho_Image = MatToHImage16Bit(data->m_vctPic[treatid]);
		Decompose3(ho_Image, &ho_Xm, &ho_Ym, &ho_Zm);
		XyzToObjectModel3d(ho_Xm, ho_Ym, ho_Zm, &hv_ObjectModel3DZ);
		//ȥ���ƽ
		HTuple hv_ObjectModel3DAffineTrans;
		if (!gen_self_light_adjusting(hv_ObjectModel3DZ, hv_ObjectModel3DAffineTrans))
		{
			ClearObjectModel3d(hv_ObjectModel3DZ);
			return false;
		}
		bool bret = true;
		int len = m_pa.m_vctPa.size();
		for (int i = 0; i < len; i++)
		{
			if(!treatProb(m_pa.m_vctPa[i], ho_Zm, hv_ObjectModel3DAffineTrans))
				bret = false;
			if (5 == m_pa.m_vctPa[i].m_type)
			{
				data->m_vctPic[0] = HObject2Mat(ho_Zm);
			}
			data->m_szRet += "�������"+m_pa.m_vctPa[i].m_szRet;
		}
		ClearObjectModel3d(hv_ObjectModel3DZ);
		ClearObjectModel3d(hv_ObjectModel3DAffineTrans);
		return bret;
	}
	catch (HException& except)
	{
		data->m_szRet = except.ErrorText().Text();
		return false;
	}
}

bool checkPatch::treatPic()
{
	try
	{
		//д������������
		HTuple  hv_ObjectModel3DZ;
		HObject  ho_Xm, ho_Ym, ho_Zm;
		Decompose3(m_deepImage, &ho_Xm, &ho_Ym, &ho_Zm);
		XyzToObjectModel3d(ho_Xm, ho_Ym, ho_Zm, &hv_ObjectModel3DZ);

		//ȥ���ƽ
		HTuple hv_ObjectModel3DAffineTrans;
		if (!gen_self_light_adjusting(hv_ObjectModel3DZ, hv_ObjectModel3DAffineTrans))
		{
			ClearObjectModel3d(hv_ObjectModel3DZ);
			return false;
		}
		string szinfo="traeatPic: \r\n";
		for (int i = 0; i < m_pa.m_vctPa.size(); i++)
		{
			if(!treatProb(m_pa.m_vctPa[i], ho_Zm, hv_ObjectModel3DAffineTrans))
				szinfo += CStrProcess::itoa_radixN(i) + "-" + m_pa.m_vctPa[i].m_procName+ " NG\r\n";
			else
				szinfo += CStrProcess::itoa_radixN(i) + "-" + m_pa.m_vctPa[i].m_procName + " OK\r\n";
		}
		HTuple  hv_GenParamNames, hv_GenParamValues, hv_Pose;
		hv_GenParamNames.Clear();
		hv_GenParamNames[0] = "disp_pose";
		hv_GenParamNames[1] = "alpha";
		hv_GenParamValues.Clear();
		hv_GenParamValues[0] = "true";
		hv_GenParamValues[1] = 0.9;
		visualize_object_model_3d(hv_WindowHandle, hv_ObjectModel3DZ, HTuple(), HTuple(),
			hv_GenParamNames, hv_GenParamValues, szinfo.c_str(), "", HTuple(),
			&hv_Pose);
		ClearObjectModel3d(hv_ObjectModel3DZ);
		ClearObjectModel3d(hv_ObjectModel3DAffineTrans);
	}
	catch (HException& except)
	{
		//string str = except.ErrorText().Text();
		disp_message(hv_WindowHandle, except.ErrorText(),
			"window", 12, 12, "black", "true");
		return false;
	}
	return true;
}

bool checkPatch::treatOnce()
{
	try
	{
		//д������������
		HTuple  hv_ObjectModel3DZ;
		HObject  ho_Xm, ho_Ym, ho_Zm;
		Decompose3(m_deepImage, &ho_Xm, &ho_Ym, &ho_Zm);
		XyzToObjectModel3d(ho_Xm, ho_Ym, ho_Zm, &hv_ObjectModel3DZ);
		//ȥ���ƽ
		HTuple hv_ObjectModel3DAffineTrans;
		if (!gen_self_light_adjusting(hv_ObjectModel3DZ, hv_ObjectModel3DAffineTrans, &hv_WindowHandle))
		{
			ClearObjectModel3d(hv_ObjectModel3DZ);
			return false;
		}
		bool bret = treatProb(m_pa.m_vctPa[m_curPa], ho_Zm, hv_ObjectModel3DAffineTrans, &hv_WindowHandle);
		if (5 == m_pa.m_vctPa[m_curPa].m_type)
			return true;
		HTuple  hv_GenParamNames, hv_GenParamValues, hv_Pose;
		hv_GenParamNames.Clear();
		hv_GenParamNames[0] = "disp_pose";
		hv_GenParamNames[1] = "alpha";
		hv_GenParamValues.Clear();
		hv_GenParamValues[0] = "true";
		hv_GenParamValues[1] = 0.9;
		HTuple hv_info;
		if (bret)
			hv_info = "treatOnce OK";
		else
			hv_info = "treatOnec NG";
		visualize_object_model_3d(hv_WindowHandle, hv_ObjectModel3DZ, HTuple(), HTuple(),
			hv_GenParamNames, hv_GenParamValues, hv_info, "", HTuple(),
			&hv_Pose);
		ClearObjectModel3d(hv_ObjectModel3DZ);
		ClearObjectModel3d(hv_ObjectModel3DAffineTrans);
	}
	catch (HException& except)
	{
		disp_message(hv_WindowHandle, except.ErrorText(),
			"window", 12, 12, "black", "true");
		return false;
	}
	return true;
}

bool checkPatch::treatProb(S_CheckProb& prob, HObject& ho_Zm, HTuple& hv_ObjectModel3DAffineTrans, HTuple* hv_WindowHandle)
{
	if (0 == prob.m_type)
	{
		prob.m_dret = getHeigh(ho_Zm, prob.m_hvReg);
		//�������ĵ�
		HTuple hvArea,hvRow, hvCol;
		AreaCenter(prob.m_hvReg,&hvArea,&hvRow,&hvCol);
		prob.hv_Pose[0] = hvCol;
		prob.hv_Pose[1] = hvRow;
		prob.hv_Pose[2] = prob.m_dret;
	}
	else if (1 == prob.m_type)
	{
		prob.m_dret = getMax(ho_Zm, prob.m_hvReg);
		//�������ĵ�
		HTuple hvArea, hvRow, hvCol;
		AreaCenter(prob.m_hvReg, &hvArea, &hvRow, &hvCol);
		prob.hv_Pose[0] = hvCol;
		prob.hv_Pose[1] = hvRow;
		prob.hv_Pose[2] = prob.m_dret;
		char sz[128];
		sprintf(sz, "����%f����%f����%f", prob.hv_Pose[0].D(), prob.hv_Pose[1].D(), prob.hv_Pose[2].D());
		prob.m_szRet = sz;
	}
	else if (2 == prob.m_type)
	{
		prob.m_dret = genFitness(hv_ObjectModel3DAffineTrans, prob.m_hvReg, prob.hv_Value, prob.hv_Value2);
	}
	else if (3 == prob.m_type)
	{
		HTuple hv_Value1,hv_Value2;//��Ԫ������
		int id1 = prob.m_objId;
		int id2 = prob.m_objPairId;
		if (-1 != id1)
		{
			hv_Value1 = m_pa.m_vctPa[id1].hv_Value;
		}
		else
		{
			hv_Value1[0] = 0;
			hv_Value1[1] = 0;
			hv_Value1[2] = 1;
		}
		if (-1 != id2)
		{
			hv_Value2 = m_pa.m_vctPa[id2].hv_Value;
		}
		else
		{
			hv_Value2[0] = 0;
			hv_Value2[1] = 0;
			hv_Value2[2] = 1;
		}
		prob.m_dret = getAngle(hv_Value1, hv_Value2);
	}
	else if (4 == prob.m_type)
	{
		int id1 = prob.m_objId;
		int id2 = prob.m_objPairId;
		prob.m_dret = fabs(m_pa.m_vctPa[id1].m_dret - m_pa.m_vctPa[id2].m_dret);
	}
	else if (5 == prob.m_type)
	{
		section(hv_ObjectModel3DAffineTrans, ho_Zm, prob.hv_Pose, hv_WindowHandle);
		return true;
	}
	else if (6 == prob.m_type)
	{
		int id1 = prob.m_objId;
		int id2 = prob.m_objPairId;
		if (-1 == id2)
		{
			HTuple hv_Value;//��Ԫ������
			hv_Value[0] = 0;
			hv_Value[1] = 0;
			hv_Value[2] = 1;
			prob.m_dret = point2planeDis(m_pa.m_vctPa[id1].hv_Pose, hv_Value);
		}
		else
			prob.m_dret = point2planeDis(m_pa.m_vctPa[id1].hv_Pose, m_pa.m_vctPa[id2].hv_Value);
	}
	else
		return false;
	if (prob.m_dret < prob.m_bh && prob.m_dret >= prob.m_bl)
	{
		return true;
	}
	else
		return false;
} 

bool checkPatch::iniHalWin()
{
	try
	{
		dev_update_off();

		if (HDevWindowStack::IsOpen())
			CloseWindow(HDevWindowStack::Pop());

		dev_open_window_fit_image(m_ImageOrg, 0, 0, -1, -1, &hv_WindowHandle);

		DisProcessROImodel();
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
	return true;
}
void checkPatch::readMode(string file)
{
	if (file != "")
	{
		mkdirs((char*)file.c_str());
		m_configAdd = file;
	}

	m_pa.readcfgfile(m_configAdd + "/checkParam");
}

void checkPatch::writeMode(string file)
{
	if (file != "")
	{
		m_configAdd = file;
	}
	delCfgFiles(m_configAdd.c_str(), 1);
	mkdirs((char*)m_configAdd.c_str());
	m_pa.writecfgfile(m_configAdd + "/checkParam");
}

void checkPatch::clone(checkPatch& src)
{
	m_curPa = src.m_curPa;
	m_pa.clone(src.m_pa);
}

bool checkPatch::drawResult(cv::Mat& src, cv::RotatedRect& roiRect, CTpltParam& pa)
{

	return true;
}

void checkPatch::DisProcessROIRegion(HRegion& HXdis, HRegion& HXsrc)
{
	switch (iCOMBO_MATCH_ROI_TYPE)
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

void checkPatch::DisProcessROImodel()
{
	HXLDCont Contours;
	Contours.GenEmptyObj();
	Contours = m_pa.m_vctPa[m_curPa].m_hvReg;
	DispObj(m_ImageOrg, hv_WindowHandle);
	DispObj(Contours, hv_WindowHandle);
}

// ROI Rectangle2
void checkPatch::DisOnRoiDrawRectangle2()
{
	if (bIsInROIdrawing) {
		AfxMessageBox("��ǰ���ڻ�ROI�С�����");
		return;
	}
	try {
		DisProcessROImodel();
		HTuple Row, Column, Phi, Length1, Length2;
		HalconCpp::HRegion Hreg;                     // ����
				// ��ʾ��Ϣ
		disp_message(hv_WindowHandle, "��ģ������,�������Ҽ�ȷ��", "window",
			12, 12, "black", "true");
		bIsInROIdrawing = true;
		// ������
		DrawRectangle2(hv_WindowHandle, &Row, &Column, &Phi, &Length1, &Length2);

		bIsInROIdrawing = false;

		GenRectangle2(&Hreg, Row, Column, Phi, Length1, Length2);

		// ��������
		DisProcessROIRegion(m_pa.m_vctPa[m_curPa].m_hvReg, Hreg);

		DisProcessROImodel();

	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}

void checkPatch::DisOnRoiDrawCircle()
{
	if (bIsInROIdrawing) {
		AfxMessageBox("��ǰ���ڻ�ROI�С�����");
		return;
	}
	try {
		DisProcessROImodel();
		HTuple Row, Column, Radius;
		HRegion Hreg;                     // ����
		disp_message(hv_WindowHandle, "��ģ������,�������Ҽ�ȷ��", "window",
			12, 12, "black", "true");
		bIsInROIdrawing = true;
		// ������
		DrawCircle(hv_WindowHandle, &Row, &Column, &Radius);
		bIsInROIdrawing = false;

		// ��������
		GenCircle(&Hreg, Row, Column, Radius);

		DisProcessROIRegion(m_pa.m_vctPa[m_curPa].m_hvReg, Hreg);

		DisProcessROImodel();   // ����ģ��
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}

void checkPatch::DisOnRoiDrawEllipse()
{
	if (bIsInROIdrawing) {
		AfxMessageBox("��ǰ���ڻ�ROI�С�����");
		return;
	}
	try {
		DisProcessROImodel();
		HTuple Row, Column, Phi, Radius1, Radius2;
		HRegion Hreg;                     // ����
		disp_message(hv_WindowHandle, "��ģ������,�������Ҽ�ȷ��", "window",
			12, 12, "black", "true");

		bIsInROIdrawing = true;
		// ������
		DrawEllipse(hv_WindowHandle, &Row, &Column, &Phi, &Radius1, &Radius2);
		bIsInROIdrawing = false;

		// ��������
		GenEllipse(&Hreg, Row, Column, Phi, Radius1, Radius2);

		// ��������
		DisProcessROIRegion(m_pa.m_vctPa[m_curPa].m_hvReg, Hreg);

		DisProcessROImodel();   // ����ģ��
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}

void checkPatch::DisOnRoiDrawRectangle()
{
	if (bIsInROIdrawing) {
		AfxMessageBox("��ǰ���ڻ�ROI�С�����");
		return;
	}
	try {
		DisProcessROImodel();
		HTuple Row1, Column1, Row2, Column2;
		HRegion Hreg;                     // ����
		disp_message(hv_WindowHandle, "��ģ������,�������Ҽ�ȷ��", "window",
			12, 12, "black", "true");

		bIsInROIdrawing = true;
		// ������
		DrawRectangle1(hv_WindowHandle, &Row1, &Column1, &Row2, &Column2);
		bIsInROIdrawing = false;

		// ��������
		GenRectangle1(&Hreg, Row1, Column1, Row2, Column2);

		// ��ԭ���������������
		DisProcessROIRegion(m_pa.m_vctPa[m_curPa].m_hvReg, Hreg);

		DisProcessROImodel();   // ����ģ��

	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}

void checkPatch::DisOnRoiDrawAnyLine()
{
	if (bIsInROIdrawing) {
		AfxMessageBox("��ǰ���ڻ�ROI�С�����");
		return;
	}
	try {
		DisProcessROImodel();
		disp_message(hv_WindowHandle, "��ģ������,�������Ҽ�ȷ��", "window",
			12, 12, "black", "true");
		HRegion Hreg;                     // ����
		bIsInROIdrawing = true;
		DrawRegion(&Hreg, hv_WindowHandle);
		bIsInROIdrawing = false;
		DisProcessROIRegion(m_pa.m_vctPa[m_curPa].m_hvReg, Hreg);

		DisProcessROImodel();

	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}

void checkPatch::DisOnMatchDelAllRoi()
{
	m_pa.m_vctPa[m_curPa].m_hvReg.GenEmptyRegion();
	try
	{
		DisProcessROImodel();
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
	//DispObj(m_hImage, hv_WindowHandle);
}

void checkPatch::DisOnElementDelOne()
{
	try
	{
		if (bIsInROIdrawing) {
			AfxMessageBox("��ǰ���ڻ�ROI�С�����");
			return;
		}
		HalconCpp::HXLDCont hv_cont, hv_cont2, hv_cont3;
		hv_cont.GenEmptyObj();
		hv_cont2.GenEmptyObj();
		hv_cont3.GenEmptyObj();
		hv_cont = m_pa.m_vctPa[m_curPa].m_hvReg;
		DispObj(hv_cont, hv_WindowHandle);
		// ��ʾ��Ϣ
		disp_message(hv_WindowHandle, "��ģ������,�������Ҽ�ȷ��", "window",
			12, 12, "black", "true");
		HTuple Row, Column;
		bIsInROIdrawing = true;
		// ������
		DrawPoint(hv_WindowHandle, &Row, &Column);
		bIsInROIdrawing = false;

		/*m_pMHWinMain->ClearWindow();
		m_pMHWinMain->DispObj(hImg);*/

		hv_cont2 = hv_cont.SelectXldPoint(Row, Column);
		int n = hv_cont.CountObj();
		for (int i = 0; i < n; i++)
		{
			hv_cont2 = hv_cont[i + 1].SelectXldPoint(Row, Column);
			if (hv_cont2.CountObj() == 0)
				hv_cont3 = hv_cont3.ConcatObj(hv_cont[i + 1]);
		}
		//m_pMHWinMain->DispObj(hv_cont3);
		m_pa.m_vctPa[m_curPa].m_hvReg = hv_cont3;
		DisProcessROImodel();
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}

int checkPatch::svc()
{//halcon�°汾visualize_object_model_3d�����޷�ˢ�µ��� ��˱������߳������
	m_runing = true;

	int sign = *(int*)m_thread_args;
	if (1 == sign)
		treatOnce();
	else if (2 == sign)
		treatPic();
	else
		;
	stop();
	m_runing = false;
	((C3dMeatureDlg*)m_pDlg)->SetTimer(1,50,NULL);
	return true;
};
