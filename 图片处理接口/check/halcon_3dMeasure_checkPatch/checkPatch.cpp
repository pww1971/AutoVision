
#include "pch.h"
#include "checkPatch.h"
#include "LightPlanePose.h"

CThreadMgr g_threadMgr;
//16bit--OpenCV Mat转Halcon HObject
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

//16bit--Halcon HObject转OpenCV Mat
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


//图片的处理	一保存模板图片，二加载模板图片进行旋转角度后匹配
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
		m_pa.hv_CamParam[0] = 0.01;//焦距 m
		m_pa.hv_CamParam[1] = 0;//Kappa(-1/m^2)
		m_pa.hv_CamParam[2] = 1;//单个像素的宽（m）
		m_pa.hv_CamParam[3] = 1;//单个像素的高（m）
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
		//处理图片
		if (treatPic(vctData[curid], pa.m_dllPa.mutilMat))
		{
			vctData[curid]->m_checkRet = true;
			return true;
		}
	}
	return false;
}
//去噪调平 密度分析光平面
void gen_light_adjusting(HTuple& hv_ObjectModel3D, HTuple& hv_ObjectModel3DAffineTrans)
{
	HTuple  hv_WindowHandle;
	HTuple  hv_ObjectModel3DConnected, hv_GenParamValue;
	HTuple  hv_ObjectModel3DSelected, hv_PoseOut1, hv_pose;
	HTuple  hv_PoseInvert, hv_HomMat3D;
	//2 筛选模型，将周围的那些点都删掉
	ConnectionObjectModel3d(hv_ObjectModel3D, "distance_3d", 1, &hv_ObjectModel3DConnected);

	//返回三维对象模型的属性。  得到属性  num_points 然后用点来筛选出模型
	GetObjectModel3dParams(hv_ObjectModel3DConnected, "num_points", &hv_GenParamValue);
	SelectObjectModel3d(hv_ObjectModel3DConnected, "num_points", "and", 200000, 2e7,
		&hv_ObjectModel3DSelected);
	//显示筛选之后的3d模型显示  select3D object
	visualize_object_model_3d(hv_WindowHandle, hv_ObjectModel3DSelected, HTuple(),
		HTuple(), "disp_pose", "true", HTuple(), HTuple(), HTuple(), &hv_PoseOut1);
	//3 将点云集合变换到原始坐标系下 的主轴  x y  Z
	MomentsObjectModel3d(hv_ObjectModel3DSelected, "principal_axes", &hv_pose);
	//将这个姿态反变化，让坐标系在鞋子点云的中心上
	PoseInvert(hv_pose, &hv_PoseInvert);
	//点云的刚体变换
	//rigid_trans_object_model_3d (ObjectModel3DSelected, PoseInvert, ObjectModel3DRigidTrans)
	// 用放射变换来实现
	PoseToHomMat3d(hv_PoseInvert, &hv_HomMat3D);
	AffineTransObjectModel3d(hv_ObjectModel3DSelected, hv_HomMat3D, &hv_ObjectModel3DAffineTrans);
	visualize_object_model_3d(hv_WindowHandle, hv_ObjectModel3DAffineTrans, HTuple(),
		HTuple(), "disp_pose", "true", "刚体变换之后的点云", HTuple(), HTuple(),
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
	//此程序将一个3d 平面拟合成一组3d 点，该程序返回所提供的输入坐标的质心坐标[ ox，oy，oz ]
	//以及拟合平面的法向量坐标[ nx，ny，nz ]。
	//警告: 如果方程组确定不足(即 x，y，z 中的输入坐标太少) ，则无法求解，程序返回 x，y 和 z 的空元组
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
	//计算质心点的坐标.求均值
	TupleMean(hv_X, &(*hv_Ox));
	TupleMean(hv_Y, &(*hv_Oy));
	TupleMean(hv_Z, &(*hv_Oz));
	//
	//Setup the equation system as a matrix M and compute
	//its singular value decomposition. The singular vector
	//of M corresponding to its smallest singular value provides
	//the coordinates of the normal vector of the fitted plane.
	//将方程式系统设定为矩阵 m 并计算其奇异值分解。
	//对应于最小奇异值的 m 的奇异矢量提供了拟合平面法向量的坐标。
	//stop ()
	//之前281* 2  562 *3  1686
	//column := |X|
	CreateMatrix(3, hv_X.TupleLength(), ((hv_X - (*hv_Ox)).TupleConcat(hv_Y - (*hv_Oy))).TupleConcat(hv_Z - (*hv_Oz)),
		&hv_MatrixID_Mt);
	//get_full_matrix (MatrixID_Mt, MatrixID_Mt_value)
	//矩阵转置
	TransposeMatrix(hv_MatrixID_Mt, &hv_MatrixID_M);
	//矩阵SVD求解
	SvdMatrix(hv_MatrixID_M, "reduced", "right", &hv_MatrixID_U, &hv_MatrixID_S, &hv_MatrixID_V);
	//
	//从矩阵内部找到对应的数值
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
	//SingularvaluesOfM  奇异值
	GetValueMatrix(hv_MatrixID_S, ((HTuple(0).Append(1)).Append(2)), ((HTuple(0).Append(1)).Append(2)),
		&hv_SingularvaluesOfM);
	//
	//对s 进行排序
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
	//*从矩阵内部找到对应的数值
	GetValueMatrix(hv_MatrixID_V, ((HTuple(0).Append(1)).Append(2)), (HTuple(hv_Indices[0]).TupleConcat(HTuple(hv_Indices[0]))).TupleConcat(HTuple(hv_Indices[0])),
		&hv_N);
	//创建3 行  ，一列的矩阵
	CreateMatrix(3, 1, hv_N, &hv_MatrixID_N);
	(*hv_Nx) = ((const HTuple&)hv_N)[0];
	(*hv_Ny) = ((const HTuple&)hv_N)[1];
	(*hv_Nz) = ((const HTuple&)hv_N)[2];
	//
	//Compute the mean residual distance between the 3d points
	//and the fitted plane, in order to guess the quality of
	//the fitted plane:
	//计算三维点与拟合平面之间的平均剩余距离，以估计拟合平面的质量
	//multiplication 乘法
	MultMatrix(hv_MatrixID_M, hv_MatrixID_N, "AB", &hv_MatrixID_MN);
	//
	//获取矩阵的所有值
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
	//这个过程决定了一个光平面的姿势，
	//例如一个由 z 0定义的平面与物理光平面一致的姿势。
	//stop ()
	//Test that the vector passed at input is not null
	//测试在输入处传递的向量是否为空
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
	//为了计算一个光平面的姿态，我们确定了两个旋转，使 z 轴的单位向量与光平面的法向量相一致，当连续施加时。
	//例如，我们可以计算角度 α (围绕 x 轴旋转)和 β (随后围绕 y 轴旋转)在这个连续的顺序。 (围绕 z 轴的旋转被任意设置为零)。
	//确定角 α 的值，并将正常向量旋转到平面 y 0。 它提供了向量 n1。
	//
	HomMat3dIdentity(&hv_HomMat3DIdentity);
	//NY NZ  反正切
	TupleAtan2(hv_NormalVectorY, hv_NormalVectorZ, &hv_Alpha);
	HomMat3dRotate(hv_HomMat3DIdentity, hv_Alpha, "x", 0, 0, 0, &hv_HomMat3DRotateAlpha);
	//
	//将NX NY NZ  变换为 N1x, N1y, N1z 。
	AffineTransPoint3d(hv_HomMat3DRotateAlpha, hv_NormalVectorX, hv_NormalVectorY,
		hv_NormalVectorZ, &hv_N1x, &hv_N1y, &hv_N1z);
	//
	//Determine the value of the angle Beta by using the
	//coordinates of N1. Note that the rotation around the
	//y-axis with angle Beta is carried out in the counter
	//trigonometric direction, therefore we apply -Beta.
	//通过使用 n1的坐标来确定角 β 的值。 注意绕 y 轴的转动是沿着反三角方向进行的，因此我们应用-β。
	//
	TupleAtan2(hv_N1x, hv_N1z, &hv_Beta);
	HomMat3dRotate(hv_HomMat3DIdentity, -hv_Beta, "y", 0, 0, 0, &hv_HomMat3DRotateBeta);
	//
	//Create the LightPlanePose
	//1. HomMat3DIdentity  平移  ----》omMat3DTranslate
	HomMat3dTranslate(hv_HomMat3DIdentity, -hv_OriginX, -hv_OriginY, -hv_OriginZ, &hv_HomMat3DTranslate);
	//
	//2  omMat3DTranslate +  沿着 α 角 相乘----》HomMat3DTmp
	HomMat3dCompose(hv_HomMat3DRotateAlpha, hv_HomMat3DTranslate, &hv_HomMat3DTmp);
	//
	//3 HomMat3DTmp +沿着 β 角 相乘----》HomMat3DWorldToLightplane
	HomMat3dCompose(hv_HomMat3DRotateBeta, hv_HomMat3DTmp, &hv_HomMat3DWorldToLightplane);
	//
	//将齐次变换矩阵 转换为pose
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
	//计算 线的点 来拟合光平面。
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
	//在先前计算的线点的三维坐标中拟合光平面。 注意这几乎是共面的。
	//为了得到一个明确的解决方案，我们必须提供至少两个不同高度的线点记录。
	//为了获得稳定、准确的测量结果，需要获得测量体积顶部和底部的光线点。
	//
	//stop ()
	//
	//把一个平面的点 所有全部丢进去， 返回的就是 质心坐标和法向量，拟合的质量（其实均值）。
	//MeanResidual  作为平面度 测量。
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
	//计算 线的点 来拟合光平面。
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
	hv_String.Append(("  alpha = " + (HTuple((*hv_LightPlanePose)[3]).TupleString(".4"))) + "°");
	hv_String.Append(("  beta  = " + (HTuple((*hv_LightPlanePose)[4]).TupleString(".4"))) + "°");
	hv_String.Append(("  gamma = " + (HTuple((*hv_LightPlanePose)[5]).TupleString(".4"))) + "°");
	hv_String.Append("  type  = " + HTuple((*hv_LightPlanePose)[6]));
	disp_message(hv_WindowHandle, hv_String, "window", 12, 12, "black", "true");
	disp_continue_message(hv_WindowHandle, "black", "true");
	return true;
}

//去噪调平 主成分分析求光平面
bool checkPatch::gen_self_light_adjusting(HTuple& hv_ObjectModel3D, HTuple& hv_ObjectModel3DAffineTrans, HTuple* hv_WindowHandle)
{
	// Local control variables
	HTuple  hv_Pose, hv_ObjectModel3DConnected;
	HTuple  hv_GenParamValue1, hv_ObjectModel3DX, hv_LightPlanePose;
	HTuple  hv_point_coord_x1, hv_point_coord_y1, hv_point_coord_z1;

	//按照高度  x  y  z ,高度筛选
	SelectPointsObjectModel3d(hv_ObjectModel3D, "point_coord_z", m_pa.m_zMin, m_pa.m_zMax, &hv_ObjectModel3D);
	//联通域  点间距  超过 1 mm  ，分割出来！
	ConnectionObjectModel3d(hv_ObjectModel3D, "distance_3d", m_pa.m_distance, &hv_ObjectModel3DConnected);
	//点云数目的统计
	GetObjectModel3dParams(hv_ObjectModel3DConnected, "num_points", &hv_GenParamValue1);
	if (0 != (hv_GenParamValue1 == 0))
	{
		return false;
	}
	//得到点云个数最大那个object
	SelectObjectModel3d(hv_ObjectModel3DConnected, "num_points", "and", (hv_GenParamValue1.TupleMax()) - 1,
		INT_MAX, &hv_ObjectModel3DX);

	//获取光平面的姿态
	hv_LightPlanePose = HTuple();
	GetObjectModel3dParams(hv_ObjectModel3DX, "point_coord_x", &hv_point_coord_x1);
	GetObjectModel3dParams(hv_ObjectModel3DX, "point_coord_y", &hv_point_coord_y1);
	GetObjectModel3dParams(hv_ObjectModel3DX, "point_coord_z", &hv_point_coord_z1);
	//1.最好的方法是拟合光平面
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

//平面度计算 输出单元法向量和平面度
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
	GetObjectModel3dParams(hv_planeFitted, "primitive_parameter", &ParamValue);//获取单元法向量
	HTuple hvcent;
	GetObjectModel3dParams(hv_planeFitted, "center", &hvcent);//获取平面中心 求参数d
	float ret = 0;
	for (int i = 0; i < 3; i++)
	{
		ret += ParamValue[i].D() * hvcent[i].D();
	}
	ParamValue[3] = -ret;
	GetObjectModel3dParams(hv_planeFitted, "primitive_pose", &ParamValue2);//获取位姿 平面的位姿
	//获取平面度
	DistanceObjectModel3d(hv_ObjectModel3DReduced, hv_planeFitted, HTuple(), 0, HTuple(),
		HTuple());
	GetObjectModel3dParams(hv_ObjectModel3DReduced, "&distance", &hv_pointsDis);
	//平面度=个人感觉应该是误差均值 mean(pointsDis)  或方差deviation(pointsDis)
	//hv_flatness = ((hv_pointsDis.TupleMax()) - (hv_pointsDis.TupleMin())).TupleAbs();
	hv_flatness = hv_pointsDis.TupleDeviation();//TupleMean()
	return hv_flatness.D();
}

//两个法向量的夹角 就是两平面的夹角 180度之内的夹角
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
	if (ret > 90)//转为锐角
	{
		ret = 180 - ret;
	}
	return ret;
}
//平面高度
float getHeigh(HObject  ho_Zmap, HRegion ho_TopRoiRegion)
{
	// Local iconic variables
	HTuple  hv_TopMean;
	HTuple  hv_TopDeviation;
	Intensity(ho_TopRoiRegion, ho_Zmap, &hv_TopMean, &hv_TopDeviation);
	return hv_TopMean.D();
	////方法1  求正切函数  角度结果0.808175
	//HTuple hv_rate = (hv_BottomMean - hv_TopMean) / 1000;
	////rad  弧度  （3.1415926与   角度  （360）
	//HTuple hv_k = hv_rate.TupleAtan();
	//TupleDeg(hv_k, &hv_Deg1);

	////方法2 角度计算  -89.1918   0.808175
	//AngleLx(0, hv_TopMean, 1000, hv_BottomMean, &hv_HightAngle);
	//TupleDeg(hv_HightAngle, &hv_Deg);
	//HTuple hv_Deg2 = hv_Deg + 90;
}
//顶点
float getMax(HObject  ho_Zmap, HRegion ho_TopRoiRegion)
{
	// Local iconic variables
	HTuple  hv_Max;
	HTuple  hv_Min;
	HTuple hv_Range;
	MinMaxGray(ho_TopRoiRegion, ho_Zmap, 0, &hv_Max, &hv_Min, &hv_Range);
	return hv_Max.D();
}

//点到平面的距离
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
//切片图
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

	//4、三角化   三维重建 表面比较光滑的  密度一样的
	TriangulateObjectModel3d(hv_ObjectModel3DAffineTrans, "greedy", HTuple(), HTuple(),
		&hv_TriangulatedObjectModel3D, &hv_Information);

	//5、求最小外接立方体
	SmallestBoundingBoxObjectModel3d(hv_TriangulatedObjectModel3D, "oriented", &hv_PoseOutbox,
		&hv_Length1, &hv_Length2, &hv_Length3);
	PoseInvert(hv_PoseOutbox, &hv_PoseInvertBox);
	RigidTransObjectModel3d(hv_TriangulatedObjectModel3D, hv_PoseInvertBox, &hv_TransObjectModel3D);
	//将box 和 点云模型都显示一下,为的是将点云模型的坐标系和立方体的坐标 做一个标准位置
	SmallestBoundingBoxObjectModel3d(hv_TransObjectModel3D, "oriented", &hv_PoseBox,
		&hv_Length1, &hv_Length2, &hv_Length3);
	GenBoxObjectModel3d(hv_PoseBox, hv_Length1, hv_Length2, hv_Length3, &hv_PoseBoxOriented);
	//6 做切平面
	//设置切割面
	{
		hv_CutPlanePose = hv_PoseBox;
		if (hv_CutPose[0] > 0)
			hv_CutPlanePose[0] = (HTuple(hv_PoseBox[0]) - (hv_Length1 / 2)) + hv_CutPose[0];//x方向切割
		if (hv_CutPose[1] > 0)
			hv_CutPlanePose[1] = (HTuple(hv_PoseBox[0]) - (hv_Length2 / 2)) + hv_CutPose[1];//y方向切割
		if (hv_CutPose[2] > 0)
			hv_CutPlanePose[2] = (HTuple(hv_PoseBox[0]) - (hv_Length3 / 2)) + hv_CutPose[2];//z方向切割
		hv_CutPlanePose[3] = hv_CutPose[3];
		hv_CutPlanePose[4] = hv_CutPose[4];
		hv_CutPlanePose[5] = hv_CutPose[5];

		//生成一个平面
		hv_Length = hv_Length3 > hv_Length2 ? hv_Length3 : hv_Length2;
		hv_Length = hv_Length1 > hv_Length ? hv_Length1 : hv_Length;
		GenImageConst(&ho_cutImage, "byte", hv_Length, hv_Length);
		hv_Length1 = hv_Length / 2;
		GenPlaneObjectModel3d(hv_CutPlanePose, (((HTuple(-1).Append(-1)).Append(1)).Append(1)) * hv_Length1,
			(((HTuple(-1).Append(1)).Append(1)).Append(-1)) * hv_Length1, &hv_ObjectModel3D1);

		//显示这个切平面
		if (NULL != hv_WinHandle)
		{
			visualize_object_model_3d(*hv_WinHandle, hv_TransObjectModel3D.TupleConcat(hv_PoseBoxOriented).TupleConcat(hv_ObjectModel3D1),
				HTuple(), HTuple(), (((HTuple("color_0").Append("color_1").Append("color_2")).Append("alpha_1")).Append("disp_pose")),
				(((HTuple("green").Append("gray").Append("blue")).Append(0.5)).Append("true")), "最小外接立方体-点云-切面",
				HTuple(), HTuple(), &hv_Pose);
		}
		//求这个平面和 三角化模型的焦点
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
		//显示我们通过切平面得到的交线模型
		if (NULL != hv_WinHandle)
			visualize_object_model_3d(*hv_WinHandle, hv_ObjectModel3DIntersection, HTuple(),
				HTuple(), HTuple(), HTuple(), HTuple(), HTuple(), HTuple(), &hv_PoseOut2);

		//通过投影得到轮廓   轮廓线的 row  对于的是交线模型的Y轴  column 对应的是交线模型的Z轴
		PoseInvert(hv_CutPlanePose, &hv_PoseInvertXLD);
		GetObjectModel3dParams(hv_ObjectModel3DIntersection, "diameter_axis_aligned_bounding_box",
			&hv_Diameter);
		hv_PoseInvertXLD[2] = HTuple(hv_PoseInvertXLD[2]) + hv_Diameter;
		//用平行于投影平面的相机（1:1的比例）
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

		// 将所有的轮廓的的点都放到数组当中
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

		//将点按照 Row 来排序
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
					hv_OrderRow = hv_OrderRow.TupleConcat(HTuple(hv_Rows[HTuple(hv_Indices[hv_Row_Index])] + hv_Length1));//hv_Length1转到图片左顶位置
					hv_OrderColumn = hv_OrderColumn.TupleConcat(HTuple(hv_Columns[hv_Row_Index] + hv_Length1));
				}
			}
		}
		//形成图片
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
			data->m_szRet = "处理图编号超范围";
			return false;
		}
		//写处理流程如下
		HTuple  hv_ObjectModel3DZ;
		HObject  ho_Xm, ho_Ym, ho_Zm;
		HImage ho_Image = MatToHImage16Bit(data->m_vctPic[treatid]);
		Decompose3(ho_Image, &ho_Xm, &ho_Ym, &ho_Zm);
		XyzToObjectModel3d(ho_Xm, ho_Ym, ho_Zm, &hv_ObjectModel3DZ);
		//去噪调平
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
			data->m_szRet += "测量结果"+m_pa.m_vctPa[i].m_szRet;
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
		//写处理流程如下
		HTuple  hv_ObjectModel3DZ;
		HObject  ho_Xm, ho_Ym, ho_Zm;
		Decompose3(m_deepImage, &ho_Xm, &ho_Ym, &ho_Zm);
		XyzToObjectModel3d(ho_Xm, ho_Ym, ho_Zm, &hv_ObjectModel3DZ);

		//去噪调平
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
		//写处理流程如下
		HTuple  hv_ObjectModel3DZ;
		HObject  ho_Xm, ho_Ym, ho_Zm;
		Decompose3(m_deepImage, &ho_Xm, &ho_Ym, &ho_Zm);
		XyzToObjectModel3d(ho_Xm, ho_Ym, ho_Zm, &hv_ObjectModel3DZ);
		//去噪调平
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
		//计算中心点
		HTuple hvArea,hvRow, hvCol;
		AreaCenter(prob.m_hvReg,&hvArea,&hvRow,&hvCol);
		prob.hv_Pose[0] = hvCol;
		prob.hv_Pose[1] = hvRow;
		prob.hv_Pose[2] = prob.m_dret;
	}
	else if (1 == prob.m_type)
	{
		prob.m_dret = getMax(ho_Zm, prob.m_hvReg);
		//计算中心点
		HTuple hvArea, hvRow, hvCol;
		AreaCenter(prob.m_hvReg, &hvArea, &hvRow, &hvCol);
		prob.hv_Pose[0] = hvCol;
		prob.hv_Pose[1] = hvRow;
		prob.hv_Pose[2] = prob.m_dret;
		char sz[128];
		sprintf(sz, "顶点%f顶点%f顶点%f", prob.hv_Pose[0].D(), prob.hv_Pose[1].D(), prob.hv_Pose[2].D());
		prob.m_szRet = sz;
	}
	else if (2 == prob.m_type)
	{
		prob.m_dret = genFitness(hv_ObjectModel3DAffineTrans, prob.m_hvReg, prob.hv_Value, prob.hv_Value2);
	}
	else if (3 == prob.m_type)
	{
		HTuple hv_Value1,hv_Value2;//单元法向量
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
			HTuple hv_Value;//单元法向量
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
		AfxMessageBox("当前已在绘ROI中。。。");
		return;
	}
	try {
		DisProcessROImodel();
		HTuple Row, Column, Phi, Length1, Length2;
		HalconCpp::HRegion Hreg;                     // 区域
				// 提示信息
		disp_message(hv_WindowHandle, "画模板区域,点击鼠标右键确认", "window",
			12, 12, "black", "true");
		bIsInROIdrawing = true;
		// 画区域
		DrawRectangle2(hv_WindowHandle, &Row, &Column, &Phi, &Length1, &Length2);

		bIsInROIdrawing = false;

		GenRectangle2(&Hreg, Row, Column, Phi, Length1, Length2);

		// 产生区域
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
		AfxMessageBox("当前已在绘ROI中。。。");
		return;
	}
	try {
		DisProcessROImodel();
		HTuple Row, Column, Radius;
		HRegion Hreg;                     // 区域
		disp_message(hv_WindowHandle, "画模板区域,点击鼠标右键确认", "window",
			12, 12, "black", "true");
		bIsInROIdrawing = true;
		// 画区域
		DrawCircle(hv_WindowHandle, &Row, &Column, &Radius);
		bIsInROIdrawing = false;

		// 产生区域
		GenCircle(&Hreg, Row, Column, Radius);

		DisProcessROIRegion(m_pa.m_vctPa[m_curPa].m_hvReg, Hreg);

		DisProcessROImodel();   // 查找模型
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}

void checkPatch::DisOnRoiDrawEllipse()
{
	if (bIsInROIdrawing) {
		AfxMessageBox("当前已在绘ROI中。。。");
		return;
	}
	try {
		DisProcessROImodel();
		HTuple Row, Column, Phi, Radius1, Radius2;
		HRegion Hreg;                     // 区域
		disp_message(hv_WindowHandle, "画模板区域,点击鼠标右键确认", "window",
			12, 12, "black", "true");

		bIsInROIdrawing = true;
		// 画区域
		DrawEllipse(hv_WindowHandle, &Row, &Column, &Phi, &Radius1, &Radius2);
		bIsInROIdrawing = false;

		// 产生区域
		GenEllipse(&Hreg, Row, Column, Phi, Radius1, Radius2);

		// 产生区域
		DisProcessROIRegion(m_pa.m_vctPa[m_curPa].m_hvReg, Hreg);

		DisProcessROImodel();   // 查找模型
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}

void checkPatch::DisOnRoiDrawRectangle()
{
	if (bIsInROIdrawing) {
		AfxMessageBox("当前已在绘ROI中。。。");
		return;
	}
	try {
		DisProcessROImodel();
		HTuple Row1, Column1, Row2, Column2;
		HRegion Hreg;                     // 区域
		disp_message(hv_WindowHandle, "画模板区域,点击鼠标右键确认", "window",
			12, 12, "black", "true");

		bIsInROIdrawing = true;
		// 画区域
		DrawRectangle1(hv_WindowHandle, &Row1, &Column1, &Row2, &Column2);
		bIsInROIdrawing = false;

		// 产生区域
		GenRectangle1(&Hreg, Row1, Column1, Row2, Column2);

		// 与原来的区域进行运算
		DisProcessROIRegion(m_pa.m_vctPa[m_curPa].m_hvReg, Hreg);

		DisProcessROImodel();   // 查找模型

	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}

void checkPatch::DisOnRoiDrawAnyLine()
{
	if (bIsInROIdrawing) {
		AfxMessageBox("当前已在绘ROI中。。。");
		return;
	}
	try {
		DisProcessROImodel();
		disp_message(hv_WindowHandle, "画模板区域,点击鼠标右键确认", "window",
			12, 12, "black", "true");
		HRegion Hreg;                     // 区域
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
			AfxMessageBox("当前已在绘ROI中。。。");
			return;
		}
		HalconCpp::HXLDCont hv_cont, hv_cont2, hv_cont3;
		hv_cont.GenEmptyObj();
		hv_cont2.GenEmptyObj();
		hv_cont3.GenEmptyObj();
		hv_cont = m_pa.m_vctPa[m_curPa].m_hvReg;
		DispObj(hv_cont, hv_WindowHandle);
		// 提示信息
		disp_message(hv_WindowHandle, "点模板区域,点击鼠标右键确认", "window",
			12, 12, "black", "true");
		HTuple Row, Column;
		bIsInROIdrawing = true;
		// 画区域
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
{//halcon新版本visualize_object_model_3d里面无法刷新点云 因此必须在线程里完成
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
