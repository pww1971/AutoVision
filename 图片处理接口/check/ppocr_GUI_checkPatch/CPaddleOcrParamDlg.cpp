// CPaddleOcrParamDlg.cpp: 实现文件
//
#include "stdafx.h"
#include "CPaddleOcrParamDlg.h"
#include "afxdialogex.h"
#include "checkPatch.h"
#include <regex>

// CPaddleOcrParamDlg 对话框

IMPLEMENT_DYNAMIC(CPaddleOcrParamDlg, CDialogEx)

CPaddleOcrParamDlg::CPaddleOcrParamDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	m_pCheck = NULL;
}

CPaddleOcrParamDlg::~CPaddleOcrParamDlg()
{
}

void CPaddleOcrParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPaddleOcrParamDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPaddleOcrParamDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CPaddleOcrParamDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BT_DET, &CPaddleOcrParamDlg::OnBnClickedBtDet)
	ON_BN_CLICKED(IDC_BT_REC, &CPaddleOcrParamDlg::OnBnClickedBtRec)
	ON_BN_CLICKED(IDC_BT_CLS, &CPaddleOcrParamDlg::OnBnClickedBtCls)
	ON_BN_CLICKED(IDC_BT_Dict, &CPaddleOcrParamDlg::OnBnClickedBtDict)
	ON_BN_CLICKED(IDC_BUTTON2, &CPaddleOcrParamDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CPaddleOcrParamDlg 消息处理程序


void CPaddleOcrParamDlg::OnBnClickedOk()
{
	if (!setParam())
		return;
	CDialogEx::OnOK();
}

bool CPaddleOcrParamDlg::setParam()
{
	checkPatch* pCheck = (checkPatch*)m_pCheck;
	CString str;
	GetDlgItem(IDC_EDIT_Det)->GetWindowTextA(str);
	pCheck->det_model_dir = str.GetBuffer(0);
	GetDlgItem(IDC_EDIT_Rec)->GetWindowTextA(str);
	pCheck->rec_model_dir = str.GetBuffer(0);
	GetDlgItem(IDC_EDIT_Cls)->GetWindowTextA(str);
	pCheck->cls_model_dir = str.GetBuffer(0);

	if (((CButton*)GetDlgItem(IDC_CHECK_angle))->GetCheck() == 1)
	{
		pCheck->use_angle_cls = 1;
	}
	else
	{
		pCheck->use_angle_cls = 0;
		if ("" == pCheck->cls_model_dir)
		{
			pCheck->cls_model_dir = getExePath();
		}
	}
	if (((CButton*)GetDlgItem(IDC_CHECK_polygon))->GetCheck() == 1)
	{
		pCheck->use_polygon_score = 1;
	}
	else
	{
		pCheck->use_polygon_score = 0;
	}

	GetDlgItem(IDC_EDIT_db_thresh)->GetWindowTextA(str);
	pCheck->det_db_thresh = atof(str.GetBuffer(0));
	GetDlgItem(IDC_EDIT_db_box_thresh)->GetWindowTextA(str);
	pCheck->det_db_box_thresh = atof(str.GetBuffer(0));
	int id = ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();
	if (id >= 0)
	{
		pCheck->m_typeid = id;
	}
	GetDlgItem(IDC_EDIT2)->GetWindowTextA(str);
	pCheck->m_szRule = str.GetBuffer(0);
	GetDlgItem(IDC_EDIT_Dict)->GetWindowTextA(str);
	pCheck->rec_char_dict_path = str.GetBuffer(0);

	if (pCheck->m_typeid < 3 && pCheck->det_model_dir.empty())
	{
		MessageBox("det_model_dir目录空");
		return false;
	}
	if (pCheck->m_typeid != 2 && pCheck->m_typeid != 5 && pCheck->rec_model_dir.empty()) 
	{
		MessageBox("rec_model_dir目录空");
		return false;
	}
	if (pCheck->m_typeid != 1 && pCheck->m_typeid != 4 && pCheck->use_angle_cls) {
		if (pCheck->cls_model_dir.empty()) {
			MessageBox("cls_model_dir目录空 ");
			return false;
		}
	}
	ifstream f(pCheck->rec_char_dict_path.c_str());
	if (!f.good())
	{
		MessageBox("字典文件不存在.\n");
	}
	return true;
}
string dllWinName = "ppocrObjPic";
BOOL CPaddleOcrParamDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	checkPatch* pCheck = (checkPatch*)m_pCheck;
	cv::namedWindow(dllWinName, cv::WINDOW_NORMAL);
	HWND hWnd = (HWND)cvGetWindowHandle(dllWinName.c_str());
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_STATIC_PIC)->GetSafeHwnd());
	::ShowWindow(hParent, SW_HIDE);
	RECT rect;
	GetDlgItem(IDC_STATIC_PIC)->GetClientRect(&rect);
	cv::resizeWindow(dllWinName, rect.right, rect.bottom);
	if (!m_pic.empty())
		imshow(dllWinName, m_pic);

	((CButton*)GetDlgItem(IDC_CHECK_angle))->SetCheck(pCheck->use_angle_cls);
	((CButton*)GetDlgItem(IDC_CHECK_polygon))->SetCheck(pCheck->use_polygon_score);
	
	CString str = pCheck->det_model_dir.c_str();
	GetDlgItem(IDC_EDIT_Det)->SetWindowTextA(str);
	str = pCheck->rec_model_dir.c_str();
	GetDlgItem(IDC_EDIT_Rec)->SetWindowTextA(str);
	str = pCheck->cls_model_dir.c_str();
	GetDlgItem(IDC_EDIT_Cls)->SetWindowTextA(str);
	str = pCheck->rec_char_dict_path.c_str();
	GetDlgItem(IDC_EDIT_Dict)->SetWindowTextA(str);
	char sz[32];
	sprintf(sz, "%f", pCheck->det_db_thresh);
	GetDlgItem(IDC_EDIT_db_thresh)->SetWindowTextA(sz);
	sprintf(sz, "%f", pCheck->det_db_box_thresh);
	GetDlgItem(IDC_EDIT_db_box_thresh)->SetWindowTextA(sz);
	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(pCheck->m_typeid);
	GetDlgItem(IDC_EDIT2)->SetWindowTextA(pCheck->m_szRule.c_str());
	sprintf(sz, "%f", pCheck->m_iniAngle);
	GetDlgItem(IDC_EDIT3)->SetWindowTextA(sz);

	return TRUE; 
}


void CPaddleOcrParamDlg::OnBnClickedButton1()
{
	if(!setParam())
		return;
	checkPatch* pCheck = (checkPatch*)m_pCheck;		
	if (!pCheck->getMapIni())
	{
		GetDlgItem(IDC_EDIT1)->SetWindowTextA("初始化调用错误");
		return;
	}
	string szRet = "";
	vector<OCRPredictResult> ocr_results;
	try
	{
		S_MosaicParam pa2;
		pa2.dScale = 1;
		pa2.iCenterX = m_pic.cols / 2;			//就取中点
		pa2.iCenterY = m_pic.rows / 2;
		pa2.dAngle = pCheck->m_iniAngle * PI / 180;//度
		cv::Mat dst = imageRotate(m_pic, pa2);		//旋转后图像会放大
		pCheck->Proc_ImageProcess(dst,ocr_results,pCheck->m_typeid, pCheck->m_pOcr);
	}
	catch (...)
	{
		GetDlgItem(IDC_EDIT1)->SetWindowTextA("调用错误");
		return;
	}
	
	if (3 > pCheck->m_typeid )
	{
		char sz[128];
		for (int i = 0; i < ocr_results.size(); i++)
		{
			std::vector<std::vector<int>> box = ocr_results[i].box;
			if (box.size() < 4) {
				continue;
			}

			int x_collect[4] = { box[0][0], box[1][0], box[2][0], box[3][0] };
			int y_collect[4] = { box[0][1], box[1][1], box[2][1], box[3][1] };
			int left = int(*std::min_element(x_collect, x_collect + 4));
			int right = int(*std::max_element(x_collect, x_collect + 4));
			int top = int(*std::min_element(y_collect, y_collect + 4));
			int bottom = int(*std::max_element(y_collect, y_collect + 4));
			float angle = fastAtan2((box[1][1] - box[0][1]), (box[1][0] - box[0][0]));
			if (ocr_results[i].cls_label != -1)
			{
				angle += ocr_results[i].cls_label;
			}
			cv::Rect ret(left, top, right - left, bottom - top);
			sprintf(sz, "Box[%d]: x=%d, y=%d, w=%d, h=%d, angle=%f\t\r\n",
				i, left, top, ret.width, ret.height,angle);//输出坐标框的信息
			szRet += sz;
			// rec
			if (ocr_results[i].score != -1.0) {
				szRet += "rec text: " + ocr_results[i].text;
				sprintf(sz, "\r\n rec score: %f", ocr_results[i].score);
				szRet += sz;
			}

		}
	}
	else
	{
		for (int i = 0; i < ocr_results.size(); i++)
		{
			if (ocr_results[i].score != -1.0)
				szRet += ocr_results[i].text;
		}
		CString str;
		GetDlgItem(IDC_EDIT2)->GetWindowTextA(str);
		regex r(str.GetBuffer(0));
		if (!std::regex_match(szRet, r)) //regex_search "(C562415)(\\dddd)"  头文字C562415和末尾四个数字
		{
			szRet += "\r\n正则化匹配失败";
		}
	}
	GetDlgItem(IDC_EDIT1)->SetWindowTextA(szRet.c_str());
}

void CPaddleOcrParamDlg::OnBnClickedBtDet()
{
	string localDir = getExePath();
	CFolderPickerDialog fd(localDir.c_str(), 0, this, 0);
	if (fd.DoModal() == IDOK)
	{
		CString str = fd.GetPathName() + "/";		
		checkPatch* pCheck = (checkPatch*)m_pCheck;
		pCheck->det_model_dir = str.GetBuffer(0);
		GetDlgItem(IDC_EDIT_Det)->SetWindowTextA(str);
	}
	else
		GetDlgItem(IDC_EDIT_Det)->SetWindowTextA("");
}

void CPaddleOcrParamDlg::OnBnClickedBtRec()
{
	string localDir = getExePath();
	CFolderPickerDialog fd(localDir.c_str(), 0, this, 0);
	if (fd.DoModal() == IDOK)
	{
		CString str = fd.GetPathName() + "/"; 
		checkPatch* pCheck = (checkPatch*)m_pCheck;
		pCheck->rec_model_dir = str.GetBuffer(0);
		GetDlgItem(IDC_EDIT_Rec)->SetWindowTextA(str);
	}
	else
		GetDlgItem(IDC_EDIT_Rec)->SetWindowTextA("");
}

void CPaddleOcrParamDlg::OnBnClickedBtCls()
{
	string localDir = getExePath();
	CFolderPickerDialog fd(localDir.c_str(), 0, this, 0);
	if (fd.DoModal() == IDOK)
	{
		CString str = fd.GetPathName() + "/";
		checkPatch* pCheck = (checkPatch*)m_pCheck;
		pCheck->cls_model_dir = str.GetBuffer(0);
		GetDlgItem(IDC_EDIT_Cls)->SetWindowTextA(str);
	}
	else
		GetDlgItem(IDC_EDIT_Cls)->SetWindowTextA("");
}


void CPaddleOcrParamDlg::OnBnClickedBtDict()
{
	ShowWindow(0);
	string localDir = getExePath();
	CFileDialog fdlg1(true, NULL, localDir.c_str(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "open file (*.txt)|*.txt||");
	checkPatch* pCheck = (checkPatch*)m_pCheck;
	if (fdlg1.DoModal() == IDOK)
	{
		pCheck->rec_char_dict_path = fdlg1.GetPathName().GetBuffer(0);
	}
	ShowWindow(1);
	GetDlgItem(IDC_EDIT_Dict)->SetWindowTextA(pCheck->rec_char_dict_path.c_str());
	/*
	PaddleOCR内置了一部分字典，可以按需使用。
ppocr/utils/ppocr_keys_v1.txt 是一个包含6623个字符的中文字典
ppocr/utils/ic15_dict.txt 是一个包含36个字符的英文字典
ppocr/utils/dict/french_dict.txt 是一个包含118个字符的法文字典
ppocr/utils/dict/japan_dict.txt 是一个包含4399个字符的日文字典
ppocr/utils/dict/korean_dict.txt 是一个包含3636个字符的韩文字典
ppocr/utils/dict/german_dict.txt 是一个包含131个字符的德文字典
ppocr/utils/en_dict.txt 是一个包含96个字符的英文字典
	*/

}


void CPaddleOcrParamDlg::OnBnClickedButton2()
{
	CString str;
	GetDlgItem(IDC_EDIT3)->GetWindowTextA(str);
	checkPatch* pCheck = (checkPatch*)m_pCheck;
	pCheck->m_iniAngle = atof(str.GetBuffer());

	S_MosaicParam pa2;
	pa2.dScale = 1;
	pa2.iCenterX = m_pic.cols / 2;			//就取中点
	pa2.iCenterY = m_pic.rows / 2;
	pa2.dAngle = pCheck->m_iniAngle * PI / 180;//度
	cv::Mat dst = imageRotate(m_pic, pa2);		//旋转后图像会放大
	showMat(dllWinName,dst);

}
