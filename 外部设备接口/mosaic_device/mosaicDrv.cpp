
#include "pch.h"
#include "mosaicDrv.hpp"
#include "CmosaicDlg.h"

mosaicDrv::mosaicDrv()
{

}

mosaicDrv::~mosaicDrv()
{

}

//相位相关法
int mosaicDrv::mosaicImage2(cv::Mat src1, cv::Mat src2, cv::Mat& dst)
{
	Mat grayL, grayR;
	if (src1.channels() == 3)
	{
		cv::cvtColor(src1, grayL, CV_RGB2GRAY);
		cv::cvtColor(src2, grayR, CV_RGB2GRAY);
	}
	else
	{
		grayL = src1;
		grayR = src2;
	}
	Mat grayL64F, grayR64F;
	grayL.convertTo(grayL64F, CV_64F);
	grayR.convertTo(grayR64F, CV_64F);
	Point2d shiftPt = phaseCorrelate(grayL64F, grayR64F);
	Mat warp_matrix = Mat::eye(2, 3, CV_32F);
	warp_matrix.at<float>(0, 0) = 1;
	warp_matrix.at<float>(0, 1) = 0;
	warp_matrix.at<float>(0, 2) = shiftPt.x;
	warp_matrix.at<float>(1, 0) = 0;
	warp_matrix.at<float>(1, 1) = 1;
	warp_matrix.at<float>(1, 2) = shiftPt.y;
	Mat imgDst;
	warpAffine(src2, imgDst, warp_matrix, cv::Size(src2.cols + fabs(shiftPt.x), src2.rows + fabs(shiftPt.y)), INTER_LINEAR + WARP_INVERSE_MAP, BORDER_REPLICATE);
	int width = src1.cols > imgDst.cols ? src1.cols : imgDst.cols;
	int height = src1.rows > imgDst.rows ? src1.rows : imgDst.rows;
	dst.create(height, width, imgDst.type());//创建一个大的矩阵用于存放两张图像
	//将第一张图和第二张图摆放在合适的位置
	Mat roi2 = dst(Rect(0, 0, imgDst.cols, imgDst.rows));
	imgDst.copyTo(roi2);
	Mat roi1 = dst(Rect(0, 0, src1.cols, src1.rows));
	src1.copyTo(roi1);

	return 1;

}
//模板匹配法
int mosaicDrv::mosaicImage(cv::Mat src1,cv::Mat src2, cv::Mat& dst/*,int lToR*/)
{
	/*Ptr<Stitcher> stitcher = Stitcher::create(Stitcher::PANORAMA);
	auto status = stitcher->stitch(m_vctImage, m);
	if (status == 0)
		return 1;
	else
		return 0;*/

	CoordinatePosXYZ cor;
	cor.vm[0] = src1;
	cor.m_procid = m_Cam->m_picProcId;
	if (-1 != m_Cam->procCheck(&cor))
	{
		float f1 = cor.f;
		int px = cor.px;//拼接点1
		int py = cor.py;
		cor.vm[0] = src2;
		if (-1 != m_Cam->procCheck(&cor))
		{
			cv::Mat image2;
			//拼接点2
			float f2 = cor.f - f1;//相对于图1的旋转角 图1不旋转
			if (f2 != 0)
			{
				if (f2<0)
				{
					f2 += 360;
				}
				S_MosaicParam pa;
				pa.dAngle = f2 * CV_PI / 180;
				pa.dScale = 1;
				image2 = imageRotate(src2, pa);
				
				float sa = sin(pa.dAngle);
				float ca = cos(pa.dAngle);
				int mx = cor.px - src2.cols / 2;
				int my = cor.py - src2.rows / 2;
				cor.py = my * ca - mx * sa + src2.rows / 2;
				cor.px = my * sa + mx * ca + src2.cols / 2;
			}
			else
			{
				image2 = src2;
			}
			Point2d shiftPt;
			shiftPt.x = cor.px - px;
			shiftPt.y = cor.py - py;
			Mat warp_matrix = Mat::eye(2, 3, CV_32F);
			warp_matrix.at<float>(0, 0) = 1;
			warp_matrix.at<float>(0, 1) = 0;
			warp_matrix.at<float>(0, 2) = shiftPt.x;
			warp_matrix.at<float>(1, 0) = 0;
			warp_matrix.at<float>(1, 1) = 1;
			warp_matrix.at<float>(1, 2) = shiftPt.y;
			Mat imgDst;
			warpAffine(src2, imgDst, warp_matrix, cv::Size(src2.cols + fabs(shiftPt.x), src2.rows + fabs(shiftPt.y)), INTER_LINEAR + WARP_INVERSE_MAP, BORDER_REPLICATE);
			int width = src1.cols > imgDst.cols ? src1.cols : imgDst.cols;
			int height = src1.rows > imgDst.rows ? src1.rows : imgDst.rows;
			dst.create(height, width, imgDst.type());//创建一个大的矩阵用于存放两张图像
			//将第一张图和第二张图摆放在合适的位置
			Mat roi2 = dst(Rect(0, 0, imgDst.cols, imgDst.rows));
			imgDst.copyTo(roi2);
			Mat roi1 = dst(Rect(0, 0, src1.cols, src1.rows));
			src1.copyTo(roi1);

			//int width, height;
			//Rect rt1(0, 0, src1.cols, src1.rows), rt2(0, 0, image2.cols, image2.rows);//2图片的roi
			////横向拼接 右边图片上下移动
			//if (lToR)
			//{
			//	if (px > cor.px)//1图拼2图
			//	{
			//		width = px + image2.cols - cor.px;
			//		int uph = py;//定位点上部高度 开始是1图的
			//		rt2.x = px - cor.px;
			//		if (py < cor.py)
			//		{
			//			rt1.y = cor.py - py;
			//			uph = cor.py;//选2图的高度
			//		}
			//		else
			//			rt2.y = py - cor.py;
			//		int downh1 = src1.rows - py;//定位点下部高度 2个图片的
			//		int downh2 = image2.rows - cor.py;
			//		height = uph + (downh1 > downh2 ? downh1 : downh2);
			//	}
			//	else//2图拼1图
			//	{
			//		return 0;
			//	}
			//}
			//else//纵向拼接 下边图片左右移动
			//{
			//	if (py > cor.py)
			//	{
			//		height = py + image2.rows - cor.py;
			//		int leftw = px;
			//		rt2.y = py - cor.py;
			//		if (px < cor.x)
			//		{
			//			rt1.x = cor.px - px;
			//			leftw = cor.px;
			//		}
			//		else
			//			rt2.x = px - cor.px;
			//		int rightw1 = src1.cols - px;
			//		int rightw2 = image2.cols - cor.px;
			//		width = leftw + (rightw1 > rightw2 ? rightw1 : rightw2);
			//	}
			//	else
			//		return 0;
			//}
			//
			//dst.create(height, width, image2.type());//创建一个大的矩阵用于存放两张图像
			////将第一张图和第二张图摆放在合适的位置
			//Mat roi2 = dst(rt2);
			//image2.copyTo(roi2);
			//Mat roi1 = dst(rt1);
			//src1.copyTo(roi1);
			return 1;
		}
	}

	return 0;
}

bool mosaicDrv::EditModel()
{
	CmosaicDlg S_dlg;
	S_dlg.m_pCheck = this;
	/*if (!S_dlg.GetSafeHwnd())
	{
		S_dlg.Create(IDD_DIALOG1);
	}
	S_dlg.ShowWindow(1);*/
	INT_PTR nResponse = S_dlg.DoModal();
	if (nResponse == IDOK)
	{
		return true;
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	return false;
}
void mosaicDrv::showAllImage()
{
	if (0 == m_vctImage.size())
	{
		return;
	}
	int iN = ceil(sqrt(m_vctImage.size()));
	int num = iN * iN;
	int scalew = m_vctImage[0].cols / 10;
	int scaleh = m_vctImage[0].rows / 10;
	cv::Mat outm;
	std::vector<cv::Mat> vm;
	int count = 0;
	for (int i = 0; i < iN; i++)
	{
		if (count >= m_vctImage.size())
		{
			vm.push_back(cv::Mat(scaleh, scalew * iN, m_vctImage[0].type()));
		}
		else
		{
			std::vector<cv::Mat> hm;
			for (int j = 0; j < iN; j++)
			{
				if (count >= m_vctImage.size())
				{
					hm.push_back(cv::Mat(scaleh, scalew, m_vctImage[0].type()));
				}
				else
				{
					resize(m_vctImage[count], outm, cv::Size(scalew, scaleh));
					hm.push_back(outm.clone());
				}
				count++;
			}
			hconcat(hm, outm);
			vm.push_back(outm.clone());
		}
	}
	vconcat(vm, outm);
	imshow(mosaitcView, outm);
}


mosaicHarris::mosaicHarris()
{
	hv_SigmaGrad = 1;
	hv_SigmaSmooth = 3;
	hv_Alpha = 0.04;
	hv_Threshold = 0;

	//Define initial values for the position of the search window
	hv_MaskSize = 21;
	hv_RowTolerance = 20;
	hv_ColTolerance = 20;
	hv_Rotation = 0;
	hv_MatchThreshold = 50;
	hv_DistanceThreshold = 0.4;
	hv_StartImage = 3;

	m_PossibleTransformations.resize(5);
	m_szPtf = m_PossibleTransformations[0] = "projective";
	m_PossibleTransformations[1] = "affine";
	m_PossibleTransformations[2] = "similarity";
	m_PossibleTransformations[3] = "rigid";
	m_PossibleTransformations[4] = "bundle_adjusted";
	m_Vertically = 5;
}

mosaicHarris::~mosaicHarris()
{
	clearMode();
}

int mosaicHarris::ini()
{

	return 1;
}

int mosaicHarris::mosaicImage(cv::Mat& m)
{
	if (0 == m_vctImage.size())
	{
		return -1;
	}
	//return mosaicImage3(m_vctImage[0],m_vctImage[1],m);相位相关法
	if (m_szPtf != m_PossibleTransformations[4])
	{
		return mosaicImage2(m);
	}
	// Local iconic variables
	HObject  ho_Images, ho_ImageF;
	HObject  ho_ImageT;
	HObject  ho_MosaicImage;

	// Local control variables
	HTuple  hv_UseRigidTransformation;
	HTuple  hv_Num;
	HTuple  hv_ProjMatrices, hv_F, hv_T;
	HTuple  hv_RowFAll, hv_ColFAll;
	HTuple  hv_RowTAll, hv_ColTAll, hv_FShiftR, hv_FShiftC;
	HTuple  hv_TShiftR, hv_TShiftC, hv_RowMove, hv_ColMove;
	HTuple  hv_RandSeed;
	HTuple  hv_ProjMatrix, hv_Points1, hv_Points2, hv_RowF;
	HTuple  hv_ColF, hv_RowT, hv_ColT, hv_HomMat2D;
	HTuple hv_MosaicMatrices2D, hv_Pointer;
	HTuple  hv_Width, hv_Height;
	hv_RandSeed = 4364537;
	hv_UseRigidTransformation = 1;

	GenEmptyObj(&ho_Images);
	for (int i = 0; i < m_vctImage.size(); i++)
	{
		ConcatObj(ho_Images, Mat2HObject(m_vctImage[i]), &ho_Images);
	}

	hv_Num = hv_From.TupleLength();
	//We need a variable to accumulate the projective transformation matrices.
	hv_ProjMatrices = HTuple();
	//Now we can determine the transformations between the image pairs.

	hv_Width = m_vctImage[0].cols;
	hv_Height = m_vctImage[0].rows;
	//Define initial values for the position of the search window
	double dRowMoveRadio = (2.0 * hv_Width / hv_Height);
	HTuple iShiftR = (hv_Height + hv_RowTolerance);
	HTuple iShiftC = (hv_Width + hv_ColTolerance);
	HTuple end_val86 = hv_Num - 1;
	HTuple hv_J, step_val86 = 1;
	for (hv_J = 0; hv_J.Continue(end_val86, step_val86); hv_J += step_val86)
	{
		hv_F = ((const HTuple&)hv_From)[hv_J];
		hv_T = ((const HTuple&)hv_To)[hv_J];
		SelectObj(ho_Images, &ho_ImageF, hv_F);
		SelectObj(ho_Images, &ho_ImageT, hv_T);
		//Extract the points in both images.
		PointsHarris(ho_ImageF, hv_SigmaGrad, hv_SigmaSmooth, hv_Alpha, hv_Threshold,
			&hv_RowFAll, &hv_ColFAll);
		PointsHarris(ho_ImageT, hv_SigmaGrad, hv_SigmaSmooth, hv_Alpha, hv_Threshold,
			&hv_RowTAll, &hv_ColTAll);
		//Perform the matching
		if (0 != (hv_F > m_Vertically))
		{
			hv_FShiftR = iShiftR;
			hv_FShiftC = (hv_F - m_Vertically - 1) * iShiftC;
		}
		else
		{
			hv_FShiftR = 0;
			hv_FShiftC = (hv_F - 1) * iShiftC;
		}
		if (0 != (hv_T > m_Vertically))
		{
			hv_TShiftR = iShiftR;
			hv_TShiftC = (hv_T - m_Vertically - 1) * iShiftC;
		}
		else
		{
			hv_TShiftR = 0;
			hv_TShiftC = (hv_T - 1) * iShiftC;
		}
		hv_RowMove = (hv_FShiftR - hv_TShiftR) / dRowMoveRadio;
		hv_ColMove = (hv_FShiftC - hv_TShiftC) / 2;
		ProjMatchPointsRansac(ho_ImageF, ho_ImageT, hv_RowFAll, hv_ColFAll, hv_RowTAll,
			hv_ColTAll, "sad", hv_MaskSize, hv_RowMove, hv_ColMove, hv_RowTolerance,
			hv_ColTolerance, hv_Rotation, hv_MatchThreshold, "gold_standard", hv_DistanceThreshold,
			hv_RandSeed, &hv_ProjMatrix, &hv_Points1, &hv_Points2);
		if (0 != hv_UseRigidTransformation)
		{
			//Determine rigid transformation
			hv_RowF = hv_RowFAll.TupleSelect(hv_Points1);
			hv_ColF = hv_ColFAll.TupleSelect(hv_Points1);
			hv_RowT = hv_RowTAll.TupleSelect(hv_Points2);
			hv_ColT = hv_ColTAll.TupleSelect(hv_Points2);
			VectorToRigid(hv_RowF + 0.5, hv_ColF + 0.5, hv_RowT + 0.5, hv_ColT + 0.5, &hv_HomMat2D);
			hv_ProjMatrix.Clear();
			hv_ProjMatrix.Append(hv_HomMat2D);
			hv_ProjMatrix.Append(0);
			hv_ProjMatrix.Append(0);
			hv_ProjMatrix.Append(1);
		}
		//Accumulate the transformation matrices
		hv_ProjMatrices = hv_ProjMatrices.TupleConcat(hv_ProjMatrix);

	}
	if (m_vctImage.size() != hv_StackingOrder.TupleLength().I())
	{
		hv_StackingOrder.Clear();
		int l = m_vctImage.size();
		for (int k = 0; k < l; k++)
		{
			hv_StackingOrder[k] = l - k;
		}
	}
	//Finally, we can generate the mosaic image from the projective transformations.
	GenProjectiveMosaic(ho_Images, &ho_MosaicImage, hv_StartImage, hv_From, hv_To,
		hv_ProjMatrices, hv_StackingOrder, "false", &hv_MosaicMatrices2D);
	//	GetImagePointer1(ho_MosaicImage, &hv_Pointer, &hv_Type, &hv_Width, &hv_Height);
	m = HObject2Mat(ho_MosaicImage);

	return 1;
}


int mosaicHarris::mosaicImage2(cv::Mat& m)
{
	// Local iconic variables
	HObject  ho_Images, ho_ImageF;
	HObject  ho_ImageT;
	HObject  ho_MosaicImage;

	// Local control variables
	HTuple  hv_Num;
	HTuple  hv_ProjMatrices, hv_F, hv_T;
	HTuple  hv_RowFAll, hv_ColFAll;
	HTuple  hv_RowTAll, hv_ColTAll, hv_FShiftR, hv_FShiftC;
	HTuple  hv_TShiftR, hv_TShiftC, hv_RowMove, hv_ColMove;
	HTuple  hv_RandSeed;
	HTuple  hv_ProjMatrix, hv_Points1, hv_Points2;
	HTuple hv_MosaicMatrices2D;
	HTuple  hv_Width, hv_Height;
	hv_RandSeed = 4364537;

	GenEmptyObj(&ho_Images);
	for (int i = 0; i < m_vctImage.size(); i++)
	{
		ConcatObj(ho_Images, Mat2HObject(m_vctImage[i]), &ho_Images);
	}
	HTuple  hv_Transformation, hv_Rows;
	HTuple  hv_Cols, hv_Error, hv_TransMat2D, hv_Type;
	HTuple  hv_Rows1, hv_Cols1, hv_Rows2;
	HTuple  hv_Cols2, hv_NumCorrespondences;

	hv_Num = hv_From.TupleLength();
	//We need a variable to accumulate the projective transformation matrices.
	hv_ProjMatrices = HTuple();
	hv_Rows1 = HTuple();
	hv_Cols1 = HTuple();
	hv_Rows2 = HTuple();
	hv_Cols2 = HTuple();
	hv_NumCorrespondences = HTuple();
	//Now we can determine the transformations between the image pairs.

	hv_Width = m_vctImage[0].cols;
	hv_Height = m_vctImage[0].rows;
	//Define initial values for the position of the search window
	double dRowMoveRadio = (2.0 * hv_Width / hv_Height);
	HTuple iShiftR = (hv_Height + hv_RowTolerance);
	HTuple iShiftC = (hv_Width + hv_ColTolerance);
	HTuple end_val86 = hv_Num - 1;
	HTuple hv_J, step_val86 = 1;
	for (hv_J = 0; hv_J.Continue(end_val86, step_val86); hv_J += step_val86)
	{
		hv_F = ((const HTuple&)hv_From)[hv_J];
		hv_T = ((const HTuple&)hv_To)[hv_J];
		SelectObj(ho_Images, &ho_ImageF, hv_F);
		SelectObj(ho_Images, &ho_ImageT, hv_T);
		//Extract the points in both images.
		PointsHarris(ho_ImageF, hv_SigmaGrad, hv_SigmaSmooth, hv_Alpha, hv_Threshold,
			&hv_RowFAll, &hv_ColFAll);
		PointsHarris(ho_ImageT, hv_SigmaGrad, hv_SigmaSmooth, hv_Alpha, hv_Threshold,
			&hv_RowTAll, &hv_ColTAll);
		//Perform the matching
		if (0 != (hv_F > m_Vertically))
		{
			hv_FShiftR = iShiftR;
			hv_FShiftC = (hv_F - m_Vertically - 1) * iShiftC;
		}
		else
		{
			hv_FShiftR = 0;
			hv_FShiftC = (hv_F - 1) * iShiftC;
		}
		if (0 != (hv_T > m_Vertically))
		{
			hv_TShiftR = iShiftR;
			hv_TShiftC = (hv_T - m_Vertically - 1) * iShiftC;
		}
		else
		{
			hv_TShiftR = 0;
			hv_TShiftC = (hv_T - 1) * iShiftC;
		}
		hv_RowMove = (hv_FShiftR - hv_TShiftR) / dRowMoveRadio;
		hv_ColMove = (hv_FShiftC - hv_TShiftC) / 2;
		ProjMatchPointsRansac(ho_ImageF, ho_ImageT, hv_RowFAll, hv_ColFAll, hv_RowTAll,
			hv_ColTAll, "sad", hv_MaskSize, hv_RowMove, hv_ColMove, hv_RowTolerance,
			hv_ColTolerance, hv_Rotation, hv_MatchThreshold, "gold_standard", hv_DistanceThreshold,
			hv_RandSeed, &hv_ProjMatrix, &hv_Points1, &hv_Points2);
		//Accumulate the transformation matrices
		hv_ProjMatrices = hv_ProjMatrices.TupleConcat(hv_ProjMatrix);
		hv_Rows1 = hv_Rows1.TupleConcat(hv_RowFAll.TupleSelect(hv_Points1));
		hv_Cols1 = hv_Cols1.TupleConcat(hv_ColFAll.TupleSelect(hv_Points1));
		hv_Rows2 = hv_Rows2.TupleConcat(hv_RowTAll.TupleSelect(hv_Points2));
		hv_Cols2 = hv_Cols2.TupleConcat(hv_ColTAll.TupleSelect(hv_Points2));
		hv_NumCorrespondences = hv_NumCorrespondences.TupleConcat(hv_Points1.TupleLength());
	}

	//Finally, we can generate the mosaic image from the projective transformations.
	HTuple hv_imgN = (int)m_vctImage.size();
	//Finally, we can generate the bundle adjusted mosaic image from the projective transformations.
	hv_Transformation = m_szPtf.c_str();
	BundleAdjustMosaic(hv_imgN, hv_StartImage, hv_From, hv_To, hv_ProjMatrices, hv_Rows1,
		hv_Cols1, hv_Rows2, hv_Cols2, hv_NumCorrespondences, hv_Transformation, &hv_MosaicMatrices2D,
		&hv_Rows, &hv_Cols, &hv_Error);
	if (m_vctImage.size() != hv_StackingOrder.TupleLength().I())
	{
		hv_StackingOrder.Clear();
		int l = m_vctImage.size();
		for (int k = 0; k < l; k++)
		{
			hv_StackingOrder[k] = l - k;
		}
	}
	GenBundleAdjustedMosaic(ho_Images, &ho_MosaicImage, hv_MosaicMatrices2D, hv_StackingOrder,
		"false", &hv_TransMat2D);
	//	GetImagePointer1(ho_MosaicImage, &hv_Pointer, &hv_Type, &hv_Width, &hv_Height);

	m = HObject2Mat(ho_MosaicImage);

	return 1;
}

void mosaicHarris::clearMode()
{

}
void mosaicHarris::readMode(string file)
{
	string szdir = file + "mosaiccfg";
	if (file != "")
	{
		pww_mkdir(szdir.c_str());
		m_cfgDir = szdir;
	}
	CConfig cfg(m_cfgDir + "\\checkParam.cfg");
	string szVal = cfg.getItem("hv_Alpha");
	if ("" != szVal)
	{
		hv_Alpha = atof(szVal.c_str());
	}
	szVal = cfg.getItem("hv_Threshold");
	if ("" != szVal)
	{
		hv_Threshold = atoi(szVal.c_str());
	}
	szVal = cfg.getItem("hv_SigmaGrad");
	if ("" != szVal)
	{
		hv_SigmaGrad = atoi(szVal.c_str());
	}
	szVal = cfg.getItem("hv_SigmaSmooth");
	if ("" != szVal)
	{
		hv_SigmaSmooth = atoi(szVal.c_str());
	}
	szVal = cfg.getItem("hv_MaskSize");
	if ("" != szVal)
	{
		hv_MaskSize = atoi(szVal.c_str());
	}
	szVal = cfg.getItem("hv_RowTolerance");
	if ("" != szVal)
	{
		hv_RowTolerance = atoi(szVal.c_str());
	}
	szVal = cfg.getItem("hv_ColTolerance");
	if ("" != szVal)
	{
		hv_ColTolerance = atoi(szVal.c_str());
	}
	szVal = cfg.getItem("hv_Rotation");
	if ("" != szVal)
	{
		hv_Rotation = atoi(szVal.c_str());
	}
	szVal = cfg.getItem("hv_MatchThreshold");
	if ("" != szVal)
	{
		hv_MatchThreshold = atoi(szVal.c_str());
	}
	szVal = cfg.getItem("hv_DistanceThreshold");
	if ("" != szVal)
	{
		hv_DistanceThreshold = atof(szVal.c_str());
	}
	szVal = cfg.getItem("hv_From");
	if ("" != szVal)
	{
		std::vector<string> vct;
		CStrProcess::Split(szVal, ",", vct, true);
		for (int i=0;i<vct.size();i++)
		{
			hv_From[i] = atoi(vct[i].c_str());
		}
	}
	szVal = cfg.getItem("hv_To");
	if ("" != szVal)
	{
		std::vector<string> vct;
		CStrProcess::Split(szVal, ",", vct, true);
		for (int i = 0; i < vct.size(); i++)
		{
			hv_To[i] = atoi(vct[i].c_str());
		}
	}
	szVal = cfg.getItem("m_szPtf");
	if ("" != szVal)
	{
		m_szPtf = szVal.c_str();
	}
	szVal = cfg.getItem("hv_StartImage");
	if ("" != szVal)
	{
		hv_StartImage = atoi(szVal.c_str());
	}
	szVal = cfg.getItem("m_Vertically");
	if ("" != szVal)
	{
		m_Vertically = atoi(szVal.c_str());
	}
	szVal = cfg.getItem("hv_StackingOrder");
	if ("" != szVal)
	{
		std::vector<string> vct;
		hv_StackingOrder.Clear();
		CStrProcess::Split(szVal, ",", vct, true);
		for (int i = 0; i < vct.size(); i++)
			hv_StackingOrder[i] = atoi(vct[i].c_str());//
	}
	
}

void mosaicHarris::writeMode(string file)
{
	string szdir = file + "mosaiccfg";
	if (file != "")
	{
		pww_mkdir(szdir.c_str());
		m_cfgDir = szdir;
	}
	delCfgFiles(m_cfgDir.c_str(), 1);

	CConfig cfg(m_cfgDir + "\\checkParam.cfg");
	char sz[32];
	sprintf(sz, "%f", hv_Alpha.D());
	cfg.updateItem("hv_Alpha", sz);
	sprintf(sz, "%d", hv_Threshold.I());
	cfg.updateItem("hv_Threshold", sz);
	sprintf(sz, "%d", hv_SigmaGrad.I());
	cfg.updateItem("hv_SigmaGrad", sz);
	sprintf(sz, "%d", hv_SigmaSmooth.I());
	cfg.updateItem("hv_SigmaSmooth", sz);
	sprintf(sz, "%d", hv_MaskSize.I());
	cfg.updateItem("hv_MaskSize", sz);
	sprintf(sz, "%d", hv_RowTolerance.I());
	cfg.updateItem("hv_RowTolerance", sz);
	sprintf(sz, "%d", hv_ColTolerance.I());
	cfg.updateItem("hv_ColTolerance", sz);
	sprintf(sz, "%d", hv_Rotation.I());
	cfg.updateItem("hv_Rotation", sz);
	sprintf(sz, "%d", hv_MatchThreshold.I());
	cfg.updateItem("hv_MatchThreshold", sz);
	sprintf(sz, "%f", hv_DistanceThreshold.D());
	cfg.updateItem("hv_DistanceThreshold", sz);
	
	HTuple hv_Num = hv_From.TupleLength();
	HTuple end_val86 = hv_Num - 1;
	HTuple hv_J, step_val86 = 1;
	string str = "";
	string str2 = "";
	for (hv_J = 0; hv_J.Continue(end_val86, step_val86); hv_J += step_val86)
	{
		sprintf(sz, "%d", hv_From[hv_J].I());
		str += sz;
		str += ",";
		sprintf(sz, "%d", hv_To[hv_J].I());
		str2 += sz;
		str2 += ",";
	}
	cfg.updateItem("hv_From", str);
	cfg.updateItem("hv_To", str2);
	cfg.updateItem("m_szPtf", m_szPtf);
	sprintf(sz, "%d", hv_StartImage.I());
	cfg.updateItem("hv_StartImage", sz);
	sprintf(sz, "%d", m_Vertically);
	cfg.updateItem("m_Vertically", sz);
	int len = hv_StackingOrder.TupleLength();
	str = "";
	for (int i = 0; i < len; i++)
	{
		sprintf(sz, "%d,", hv_StackingOrder[i].I());
		str += sz;
	}
	cfg.updateItem("hv_StackingOrder", str);
}

