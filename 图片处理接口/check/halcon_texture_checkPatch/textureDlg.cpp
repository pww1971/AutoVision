// textureDlg.cpp: 实现文件
//

#include "pch.h"
#include "halcon_texture_checkPatch.h"
#include "textureDlg.h"
#include "afxdialogex.h"
#include "checkPatch.h"

// textureDlg 对话框

IMPLEMENT_DYNAMIC(textureDlg, CDialogEx)

textureDlg::textureDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	m_pCheck = NULL;
}

textureDlg::~textureDlg()
{
}

void textureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(textureDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &textureDlg::OnBnClickedButton1)

	ON_BN_CLICKED(IDC_BUTTON2, &textureDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_CHECK1, &textureDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &textureDlg::OnBnClickedCheck2)
	ON_EN_CHANGE(IDC_EDIT1, &textureDlg::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &textureDlg::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &textureDlg::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, &textureDlg::OnEnChangeEdit4)
	ON_BN_CLICKED(IDC_BUTTON3, &textureDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &textureDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// textureDlg 消息处理程序


void textureDlg::OnBnClickedButton1()
{
	try {
		HImage ho_Mask=m_ImageOrg;
		((checkPatch*)m_pCheck)->treatPic(ho_Mask);
		DispObj(ho_Mask, hv_WindowHandle);
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
}


BOOL textureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_halhWnd = GetDlgItem(IDC_STATIC_POSPIC)->GetSafeHwnd();//获取控件句柄
	iniHalWin();

	S_CheckProb& pa = ((checkPatch*)m_pCheck)->m_pa;
	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(pa.patch_normalization);
	((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(pa.patch_rotational_robustness);
	CString str;
	str.Format("%f", pa.sensitivity);
	(GetDlgItem(IDC_EDIT1))->SetWindowText(str);
	str.Format("%f", pa.gmm_em_threshold);
	(GetDlgItem(IDC_EDIT4))->SetWindowText(str);
	int n = pa.hv_NoveltyThreshold.TupleLength();
	string szthr = "";
	char sz[32];
	for (int i = 0; i < n; i++)
	{
		sprintf(sz, "%d,", pa.hv_NoveltyThreshold[i].I());
		szthr += sz;
	}
	(GetDlgItem(IDC_EDIT2))->SetWindowText(szthr.c_str());
	
	(GetDlgItem(IDC_EDIT3))->SetWindowText(pa.szLevel.c_str());
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
void textureDlg::dev_open_window_fit_size(HTuple hv_Row, HTuple hv_Column, HTuple hv_Width,
	HTuple hv_Height, HTuple hv_WidthLimit, HTuple hv_HeightLimit, HTuple* hv_WindowHandle)
{

	// Local iconic variables

	// Local control variables
	HTuple  hv_MinWidth, hv_MaxWidth, hv_MinHeight;
	HTuple  hv_MaxHeight, hv_ResizeFactor, hv_TempWidth, hv_TempHeight;
	HTuple  hv_WindowWidth, hv_WindowHeight;

	//This procedure open a new graphic window
	//such that it fits into the limits specified by WidthLimit
	//and HeightLimit, but also maintains the correct aspect ratio
	//given by Width and Height.
	//
	//If it is impossible to match the minimum and maximum extent requirements
	//at the same time (f.e. if the image is very long but narrow),
	//the maximum value gets a higher priority.
	//
	//Parse input tuple WidthLimit
	if (0 != (HTuple(int((hv_WidthLimit.TupleLength()) == 0)).TupleOr(int(hv_WidthLimit < 0))))
	{
		hv_MinWidth = 500;
		hv_MaxWidth = 800;
	}
	else if (0 != (int((hv_WidthLimit.TupleLength()) == 1)))
	{
		hv_MinWidth = 0;
		hv_MaxWidth = hv_WidthLimit;
	}
	else
	{
		hv_MinWidth = ((const HTuple&)hv_WidthLimit)[0];
		hv_MaxWidth = ((const HTuple&)hv_WidthLimit)[1];
	}
	//Parse input tuple HeightLimit
	if (0 != (HTuple(int((hv_HeightLimit.TupleLength()) == 0)).TupleOr(int(hv_HeightLimit < 0))))
	{
		hv_MinHeight = 400;
		hv_MaxHeight = 600;
	}
	else if (0 != (int((hv_HeightLimit.TupleLength()) == 1)))
	{
		hv_MinHeight = 0;
		hv_MaxHeight = hv_HeightLimit;
	}
	else
	{
		hv_MinHeight = ((const HTuple&)hv_HeightLimit)[0];
		hv_MaxHeight = ((const HTuple&)hv_HeightLimit)[1];
	}
	//
	//Test, if window size has to be changed.
	hv_ResizeFactor = 1;
	//First, expand window to the minimum extents (if necessary).
	if (0 != (HTuple(int(hv_MinWidth > hv_Width)).TupleOr(int(hv_MinHeight > hv_Height))))
	{
		hv_ResizeFactor = (((hv_MinWidth.TupleReal()) / hv_Width).TupleConcat((hv_MinHeight.TupleReal()) / hv_Height)).TupleMax();
	}
	hv_TempWidth = hv_Width * hv_ResizeFactor;
	hv_TempHeight = hv_Height * hv_ResizeFactor;
	//Then, shrink window to maximum extents (if necessary).
	if (0 != (HTuple(int(hv_MaxWidth < hv_TempWidth)).TupleOr(int(hv_MaxHeight < hv_TempHeight))))
	{
		hv_ResizeFactor = hv_ResizeFactor * ((((hv_MaxWidth.TupleReal()) / hv_TempWidth).TupleConcat((hv_MaxHeight.TupleReal()) / hv_TempHeight)).TupleMin());
	}
	hv_WindowWidth = hv_Width * hv_ResizeFactor;
	hv_WindowHeight = hv_Height * hv_ResizeFactor;
	//Resize window
	SetWindowAttr("background_color", "black");
	OpenWindow(hv_Row, hv_Column, hv_WindowWidth, hv_WindowHeight, (Hlong)m_halhWnd, "", "", &(*hv_WindowHandle));
	HDevWindowStack::Push((*hv_WindowHandle));
	if (HDevWindowStack::IsOpen())
		SetPart(HDevWindowStack::GetActive(), 0, 0, hv_Height - 1, hv_Width - 1);
	return;
}

bool textureDlg::iniHalWin()
{
	try
	{
		if (HDevWindowStack::IsOpen())
			HalconCpp::CloseWindow(HDevWindowStack::Pop());
		HTuple hv_Width,  hv_Height;
		GetImageSize(m_ImageOrg, &hv_Width, &hv_Height);
		dev_open_window_fit_size(0, 0, hv_Width, hv_Height, -1, -1, &hv_WindowHandle);

		DispObj(m_ImageOrg, hv_WindowHandle);
	}
	catch (HalconCpp::HException& except) {
		AfxMessageBox(except.ErrorText().Text());
	}
	return true;
}


void textureDlg::OnBnClickedButton2()
{
	CFolderPickerDialog fd(getExePath().c_str(), 0, this, 0);
	if (fd.DoModal() == IDOK)
	{
		string dir = fd.GetPathName().GetBuffer(0);
		vector<string> vct;
		getFileNameVec(dir.c_str(), vct);
		if (vct.size() == 0)
			return;
		BeginWaitCursor();
		try {
			S_CheckProb& pa = ((checkPatch*)m_pCheck)->m_pa;
			HObject  ho_Image;
			HTuple   hv_Indices;

			for (int i = 0; i < vct.size(); i++)
			{
				ReadImage(&ho_Image, (dir+"/"+vct[i]).c_str());
				if (HDevWindowStack::IsOpen())
					DispObj(ho_Image, HDevWindowStack::GetActive());
				//Use the whole image as texture sample
				//In different applications you may want to reduce the image domain first.
				AddTextureInspectionModelImage(ho_Image, pa.hv_TextureInspectionModel, &hv_Indices);
			}

			if (pa.patch_normalization)//亮度变化
				SetTextureInspectionModelParam(pa.hv_TextureInspectionModel, "patch_normalization", "weber");
			else
				SetTextureInspectionModelParam(pa.hv_TextureInspectionModel, "patch_normalization", "none");
			if (pa.patch_rotational_robustness)//旋转变化
				SetTextureInspectionModelParam(pa.hv_TextureInspectionModel, "patch_rotational_robustness", "true");
			else
				SetTextureInspectionModelParam(pa.hv_TextureInspectionModel, "patch_rotational_robustness", "false");
			//gmm期望阈值变化
			SetTextureInspectionModelParam(pa.hv_TextureInspectionModel, "gmm_em_threshold", pa.gmm_em_threshold);
			//灵敏度
			SetTextureInspectionModelParam(pa.hv_TextureInspectionModel, "sensitivity", pa.sensitivity);
			//金字塔
			if("auto"==pa.szLevel)
				SetTextureInspectionModelParam(pa.hv_TextureInspectionModel, "levels", "auto");
			else
			{
				vector<string> vct;
				CStrProcess::Split(pa.szLevel, ",", vct, true);
				HTuple hv_Levels;
				for (int i = 0; i < vct.size(); i++)
					hv_Levels[i] = atoi(vct[i].c_str());
				SetTextureInspectionModelParam(pa.hv_TextureInspectionModel, "levels", hv_Levels);
			}
			//Start the training.
			TrainTextureInspectionModel(pa.hv_TextureInspectionModel);
			//
			//Write the texture model to disc
			/*hv_TextureModelFilename = ((checkPatch*)m_pCheck)->m_configAdd.c_str();
			WriteTextureInspectionModel(pa.hv_TextureInspectionModel, hv_TextureModelFilename);*/
		}
		catch (HalconCpp::HException& except) {
			AfxMessageBox(except.ErrorText().Text());
		}
		EndWaitCursor();
	}
}


void textureDlg::OnBnClickedCheck1()
{
	S_CheckProb& pa = ((checkPatch*)m_pCheck)->m_pa;
	if (((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() == 1)
		pa.patch_normalization = 1;
	else
		pa.patch_normalization = 0;
	DispObj(m_ImageOrg, hv_WindowHandle);
}


void textureDlg::OnBnClickedCheck2()
{
	S_CheckProb& pa = ((checkPatch*)m_pCheck)->m_pa;
	if (((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck() == 1)
		pa.patch_rotational_robustness = 1;
	else
		pa.patch_rotational_robustness = 0;
	DispObj(m_ImageOrg, hv_WindowHandle);
}


void textureDlg::OnEnChangeEdit1()
{
	S_CheckProb& pa = ((checkPatch*)m_pCheck)->m_pa;
	CString str;
	(GetDlgItem(IDC_EDIT1))->GetWindowText(str);
	pa.sensitivity = atof(str.GetBuffer());
	DispObj(m_ImageOrg, hv_WindowHandle);
}


void textureDlg::OnEnChangeEdit2()
{
	S_CheckProb& pa = ((checkPatch*)m_pCheck)->m_pa;
	CString str;
	(GetDlgItem(IDC_EDIT2))->GetWindowText(str);
	vector<string> vct;
	string szlevel = str.GetBuffer(0);
	CStrProcess::Split(szlevel, ",", vct, true);
	pa.hv_NoveltyThreshold.Clear();
	for (int i = 0; i < vct.size(); i++)
		pa.hv_NoveltyThreshold[i] = atoi(vct[i].c_str());
	DispObj(m_ImageOrg, hv_WindowHandle);
}


void textureDlg::OnEnChangeEdit3()
{
	S_CheckProb& pa = ((checkPatch*)m_pCheck)->m_pa;
	CString str;
	(GetDlgItem(IDC_EDIT3))->GetWindowText(str);
	pa.szLevel =str.GetBuffer();
	DispObj(m_ImageOrg, hv_WindowHandle);
}


void textureDlg::OnEnChangeEdit4()
{
	S_CheckProb& pa = ((checkPatch*)m_pCheck)->m_pa;
	CString str;
	(GetDlgItem(IDC_EDIT4))->GetWindowText(str);
	pa.gmm_em_threshold = atof(str.GetBuffer());
	DispObj(m_ImageOrg, hv_WindowHandle);
}


void textureDlg::OnBnClickedButton3()
{
	CFolderPickerDialog fd(getExePath().c_str(), 0, this, 0);
	if (fd.DoModal() == IDOK)
	{
		string dir = fd.GetPathName().GetBuffer(0);
		CDateTime dt;
		dir += "/"+dt.ToString("%Y-%m-%d-%H-%M-%S");
		WriteImage(m_ImageOrg, "bmp", 0, dir.c_str());
	}
}


void textureDlg::OnBnClickedButton4()
{
	((checkPatch*)m_pCheck)->writeMode();
}
