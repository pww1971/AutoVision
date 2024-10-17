#include "stdafx.h"

#include "MyData.h"
#include "MyDispLay.h"
using namespace HalconCpp;
using namespace std;

//void ST_MYDISP::OnDisKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//
//}


void ST_MYDISP::DisDisplay(CRect rect)
{
	this->ChangeImageCRect(rect);
	this->SetPartHvWnd();

	// this->DisDisplay();

	/*WaitForSingleObject(this->hMutex_disp, 500);
	ClearWindow(this->hv_HWD);
	this->DisDisplay();
	ReleaseMutex(this->hMutex_disp);*/
}



// 更新图像的显示区域为当前的 Rect
void ST_MYDISP::SetPartHvWnd()
{
	CRect* rc = &rcImage;
	this->m_pMHWinMain->SetPart(rc->top, rc->left, rc->bottom, rc->right);

}






void ST_MYDISP::write_message(HalconCpp::HTuple hv_Row, HalconCpp::HTuple hv_Column,
	HalconCpp::HTuple hv_String, bool isclear)
{

	// Local control variables
	HTuple  hv_Red, hv_Green, hv_Blue;
	Hlong hv_Row1Part, hv_Column1Part, hv_Row2Part, hv_Column2Part;
	Hlong   hv_RowWin, hv_ColumnWin, hv_WidthWin, hv_HeightWin;
	HTuple  hv_Width;
	HTuple  hv_Index;
	Hlong hv_ExpDefaultCtrlDummyVar, hv_W;
	HTuple hv_Function;
	HTuple  hv_Sequence, hv_Chr, hv_Ascent;
	Hlong hv_Descent, hv_Height;
	HTuple  hv_FrameHeight, hv_FrameWidth, hv_FactorRow, hv_FactorColumn;
	HTuple  hv_DrawMode, hv_R1_S, hv_C1_S, hv_R2_S, hv_C2_S;
	HTuple  hv_R1, hv_C1, hv_R2, hv_C2;

	//
	//Estimate extentions of text depending on font size, 
	//draw rectangle at the specified position
	//and write text in this rectangle
	//
	//HDevWindowStack::SetActive(hv_HWD);

	this->m_pMHWinMain->GetRgb(&hv_Red, &hv_Green, &hv_Blue);
	this->m_pMHWinMain->GetPart(&hv_Row1Part, &hv_Column1Part, &hv_Row2Part, &hv_Column2Part);
	this->m_pMHWinMain->GetWindowExtents(&hv_RowWin, &hv_ColumnWin, &hv_WidthWin, &hv_HeightWin);

	if (0 != (hv_Row == -1))
	{
		hv_Row = 12;
	}
	if (0 != (hv_Column == -1))
	{
		hv_Column = 12;
	}
	hv_String = (" " + ((("" + hv_String) + "").TupleSplit("\n"))) + " ";
	hv_Width = HTuple();
	{
		HTuple end_val17 = (hv_String.TupleLength()) - 1;
		HTuple step_val17 = 1;
		for (hv_Index = 0; hv_Index.Continue(end_val17, step_val17); hv_Index += step_val17)
		{
			this->m_pMHWinMain->GetStringExtents(HTuple(hv_String[hv_Index]), &hv_ExpDefaultCtrlDummyVar,
				&hv_W, &hv_ExpDefaultCtrlDummyVar);
			hv_Width = hv_Width.TupleConcat(hv_W);
		}
	}
	CreateFunct1dArray(HTuple(256, 0), &hv_Function);
	Funct1dToPairs(hv_Function, &hv_Sequence, &HTuple(hv_ExpDefaultCtrlDummyVar));
	hv_Chr = hv_Sequence.TupleChr();

	this->m_pMHWinMain->GetStringExtents(hv_Chr, &hv_Descent, &hv_ExpDefaultCtrlDummyVar,
		&hv_Height);
	hv_Height = (HTuple(hv_Height).TupleConcat(hv_Descent)).TupleMax();
	//
	hv_FrameHeight = hv_Height*(hv_String.TupleLength());
	hv_FrameWidth = hv_Width.TupleMax();
	//
	//Adapt text position and extents because of image zooming
	hv_FactorRow = (6.*((hv_Row2Part - hv_Row1Part) + 1)) / hv_HeightWin;

	//double f = hv_FactorRow.D();

	hv_FactorColumn = (1.*((hv_Column2Part - hv_Column1Part) + 1)) / hv_WidthWin;

	hv_DrawMode = this->m_pMHWinMain->GetDraw();
	//GetDraw(hv_HWD, &hv_DrawMode);

	this->m_pMHWinMain->SetDraw("fill");

	HTuple hv_WindowCoordinates = 0;

	if (0 != hv_WindowCoordinates)
	{
		this->m_pMHWinMain->SetColor("light gray");
		//SetColor(hv_HWD, "light gray");
		hv_R1_S = (((hv_Row + 3)*hv_FactorRow) + 0.5) + hv_Row1Part;
		hv_C1_S = (((hv_Column + 3)*hv_FactorColumn) + 0.5) + hv_Column1Part;
		hv_R2_S = (((((hv_Row + 3) + hv_FrameHeight) - 1)*hv_FactorRow) + 0.5) + hv_Row1Part;
		hv_C2_S = (((((hv_Column + 3) + hv_FrameWidth) - 1)*hv_FactorColumn) + 0.5) + hv_Column1Part;

		this->m_pMHWinMain->DispRectangle1(hv_R1_S, hv_C1_S, hv_R2_S, hv_C2_S);
		//DispRectangle1(hv_HWD, hv_R1_S, hv_C1_S, hv_R2_S, hv_C2_S);
		this->m_pMHWinMain->SetColor("white");
		//SetColor(hv_HWD, "white");
		hv_R1 = ((hv_Row*hv_FactorRow) + 0.5) + hv_Row1Part;
		hv_C1 = ((hv_Column*hv_FactorColumn) + 0.5) + hv_Column1Part;
		hv_R2 = ((((hv_Row + hv_FrameHeight) - 1)*hv_FactorRow) + 0.5) + hv_Row1Part;
		hv_C2 = ((((hv_Column + hv_FrameWidth) - 1)*hv_FactorColumn) + 0.5) + hv_Column1Part;

		this->m_pMHWinMain->DispRectangle1(hv_R1, hv_C1, hv_R2, hv_C2);
		//DispRectangle1(hv_HWD, hv_R1, hv_C1, hv_R2, hv_C2);
		this->m_pMHWinMain->SetColor("black");
		//SetColor(hv_HWD, "black");
	}
	else
	{
		this->m_pMHWinMain->SetColor("light gray");
		//SetColor(hv_HWD, "light gray");
		hv_R1_S = hv_Row + 3;
		hv_C1_S = hv_Column + 3;
		hv_R2_S = ((hv_Row + 3) + ((hv_FrameHeight - 1)*hv_FactorRow)) + 0.5;
		hv_C2_S = ((hv_Column + 3) + ((hv_FrameWidth - 1)*hv_FactorColumn)) + 0.5;

		this->m_pMHWinMain->DispRectangle1(hv_R1_S, hv_C1_S, hv_R2_S, hv_C2_S);
		//DispRectangle1(hv_HWD, hv_R1_S, hv_C1_S, hv_R2_S, hv_C2_S);
		this->m_pMHWinMain->SetColor("white");
		//SetColor(hv_HWD, "white");
		hv_R1 = hv_Row;
		hv_C1 = hv_Column;
		hv_R2 = (hv_Row + ((hv_FrameHeight - 1)*hv_FactorRow)) + 0.5;
		hv_C2 = (hv_Column + ((hv_FrameWidth - 1)*hv_FactorColumn)) + 0.5;

		this->m_pMHWinMain->DispRectangle1(hv_R1, hv_C1, hv_R2, hv_C2);
		//DispRectangle1(hv_HWD, hv_R1, hv_C1, hv_R2, hv_C2);
		this->m_pMHWinMain->SetColor("black");
	}
	//Write text into rectangle
	{
		HTuple end_val66 = (hv_String.TupleLength()) - 1;
		HTuple step_val66 = 1;
		for (hv_Index = 0; hv_Index.Continue(end_val66, step_val66); hv_Index += step_val66)
		{
			this->m_pMHWinMain->SetTposition(hv_R1.I() + int((int(hv_Height)*hv_FactorRow.D())*hv_Index.I()), hv_C1);
			//this->m_pMHWinMain->SetTposition(hv_R1 + ((hv_Height*hv_FactorRow)*hv_Index), hv_C1);
			//SetTposition(hv_HWD, hv_R1 + ((hv_Height*hv_FactorRow)*hv_Index), hv_C1);
			this->m_pMHWinMain->WriteString(HTuple(hv_String[hv_Index]));
			//WriteString(hv_HWD, HTuple(hv_String[hv_Index]));
		}
	}
	this->m_pMHWinMain->SetDraw(hv_DrawMode);
	//if (HDevWindowStack::IsOpen())
	//	SetDraw(HDevWindowStack::GetActive(), hv_DrawMode);

	this->m_pMHWinMain->SetRgb(hv_Red, hv_Green, hv_Blue);
	//SetRgb(hv_HWD, hv_Red, hv_Green, hv_Blue);
	return;
}



void ST_MYDISP::OnDestroy()
{
	if (m_pMHWinMain!=NULL)
	{
		delete m_pMHWinMain;
		m_pMHWinMain=NULL;
	}
}

void ST_MYDISP::InitThreadCommunication()
{
	if (NULL != UCI::pDis->m_pMHWinMain)
		delete m_pMHWinMain;
	CRect rect;
	this->PMainStatic->GetClientRect(&rect); 
	m_pMHWinMain = new HWindow(0, 0, rect.Width(), rect.Height(), this->PMainStatic->m_hWnd, "visible", "");

	m_pMHWinMain->SetDraw("fill");
	m_pMHWinMain->SetColor("green");
	m_pMHWinMain->SetLineWidth(1);
	m_pMHWinMain->SetFont("Arial-30");

}

void ST_MYDISP::ResetAllCStaticPart()
{

	int h = UCI::OPTIONS["图像高度"];
	int w = UCI::OPTIONS["图像宽度"];

	if (h < 50) return;
	if (w < 50) return;

	this->m_pMHWinMain->SetPart(0, 0, h - 1, w - 1);
}

void ST_MYDISP::ResetAllCStaticWinExtends()
{
	CRect rect;
	//this->pIPWinStatic->GetClientRect(&rect);
	//this->m_pHWinIP->SetWindowExtents(0, 0, rect.Width(), rect.Height());

	this->PMainStatic->GetClientRect(&rect);

	CRect cr( 0,0,rect.Width() - 1, rect.Height() - 1 );
	this->ChangeImageCRect(cr);

	this->m_pMHWinMain->SetWindowExtents(0, 0, rect.Width(), rect.Height());
}
void ST_MYDISP::AppStateChangeTo(eAppState e)
{
	this->mAppState = e;
	if (e == APP_READY) {
		
	}
	
}

LRESULT ST_MYDISP::OnMainDisp()
{
	try
	{
		HImage   Image;
		HXLDCont Cont;
		Cont.GenEmptyObj();
		Image.GenEmptyObj();
		//		EnterCriticalSection(&m_csIPDisp);      // CriticalSect
		Image = m_sIPResult.result_img;         // CriticalSect
		Cont = m_sIPResult.result_cont;         // CriticalSect
		//		LeaveCriticalSection(&m_csIPDisp);      // CriticalSect

		// 设置一下ROI的颜色及线宽
		CString HDLXcolor = UCI::OPTIONS["ElementHDLXColor"];
		int HDLXWidth = UCI::OPTIONS["ElementHDLXWidth"];
		this->m_pMHWinMain->SetColor(CT2A(HDLXcolor).m_psz);
		this->m_pMHWinMain->SetLineWidth(HDLXWidth);

		this->m_pMHWinMain->DispObj(Image);
		this->m_pMHWinMain->DispObj(Cont);

	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}

	return 0L;
}


CString ST_MYDISP::GetMatchPathFile(int prID)
{
	CString format = "相像保存格式";
	CString stDeviceName = "相机名称";

	CString str;
	str.Format("%d", prID);


	CString res = UCI::strMoudlePath + "\\project\\"
		+ str + "\\ROI\\"
		+ stDeviceName + "\\roimatch."
		+ format;
	return res;
}

CString ST_MYDISP::GetShapeModelPathFile(int prID)
{

	CString stDeviceName = UCI::OPTIONS["相机名称"];

	CString str;
	str.Format("%d", prID);

	CString res = UCI::strMoudlePath + "\\project\\"
		+ str + "\\ROI\\" +
		stDeviceName + "\\roishapemodel.shm";

	return res;
}

CString ST_MYDISP::GetShapeModelRegionPathFile(int prID)
{

	CString stDeviceName = UCI::OPTIONS["相机名称"];

	CString str;
	str.Format("%d", prID);

	CString res = UCI::strMoudlePath + "\\project\\"
		+ str + "\\ROI\\" +
		stDeviceName + "\\roishmRegion.hobj";

	return res;
}


// 初始化
ST_MYDISP::ST_MYDISP(CWnd* cwnd)
{
	hv_Model=NULL;

	pCView = cwnd;
//	IsSpaceKeyDown = 0; 
	rcImage = CRect( 0,0,600,800 );
//	m_bRectMapOK = false;
	PMainStatic = NULL;
	m_pMHWinMain = NULL;
//	m_hIPThrHandle = NULL;

//	pCAM = new CCameraCPP();

	GrabImageDisp.GenEmptyObj();


	this->mAppState = APP_READY;            // 第一次初始化

	this->CrossLine[0].x = 100;
	this->CrossLine[0].y = 100;
	this->CrossLine[0].color = RGB(255,0,0);
	this->CrossLine[0].isUse = true ;
	this->CrossLine[1].x = 1900;
	this->CrossLine[1].y = 1400;
	this->CrossLine[1].color = RGB(0,255,0);
	this->CrossLine[1].isUse = true ;
	this->CrossLine[2].x = 1000;
	this->CrossLine[2].y = 700;
	this->CrossLine[2].color = RGB(0,0,255);
	this->CrossLine[2].isUse = true ;
	m_Zoom=1;
	m_ZoomOrg=1;

}

// 
ST_MYDISP::~ST_MYDISP()
{
	this->OnDestroy();
//	delete this->pCAM;
}




CString ST_MYDISP::GetTimeMsToString()
{
	CTime t = CTime::GetCurrentTime();
	CString s = t.Format("%Y%m%d%H%M%S");

	return s;
}

