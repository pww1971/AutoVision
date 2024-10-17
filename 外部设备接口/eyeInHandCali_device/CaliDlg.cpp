// CaliDlg.cpp: 实现文件
//

#include "pch.h"
#include "eyeInHandCali_device.h"
#include "CaliDlg.h"
#include "afxdialogex.h"


// CaliDlg 对话框

IMPLEMENT_DYNAMIC(CaliDlg, CDialogEx)


CaliDlg::CaliDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_Cali, pParent)
{

}

CaliDlg::~CaliDlg()
{
}

void CaliDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ppList);

}


BEGIN_MESSAGE_MAP(CaliDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BT_Cali2, &CaliDlg::OnBnClickedBtCali2)
	ON_BN_CLICKED(IDC_BT_Cali, &CaliDlg::OnBnClickedBtCali)
	ON_BN_CLICKED(IDC_BT_Cali3, &CaliDlg::OnBnClickedBtCali3)
	ON_BN_CLICKED(IDOK, &CaliDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CaliDlg 消息处理程序


void CaliDlg::OnBnClickedBtCali2()
{
	// 打开文件夹 读取图片 定位 获取定位坐标 写入list控件 
	CFolderPickerDialog fd(getExePath().c_str(), 0, this, 0);
	if (fd.DoModal() == IDOK)
	{
		CProcDll* pCheck = ((checkPatch*)m_pData)->m_CaliParam.pCheck;
		S_CaliParam* CaliParam = &((checkPatch*)m_pData)->m_CaliParam;
		std::vector<cv::Point2f>& scene = CaliParam->scene;
		scene.clear();
		CoordinatePosXYZ pd;
		pd.m_procid = pCheck->m_picProcId;
		std::string pattern_jpg = fd.GetPathName().GetBuffer(0);
		std::vector<cv::String> image_files;
		pattern_jpg += "/*.png";
		cv::glob(pattern_jpg, image_files);
		for (int ii = 0; ii < image_files.size(); ii++)
		{
			pd.vm[0] = imread(image_files[ii]);
			int ret = pCheck->procCheck(&pd);
			if ( -1 < ret )//定位成功
			{
				scene.push_back(move(Point2f(pd.px, pd.py)));
			}
		}
		viewList();
	}
}

void CaliDlg::viewList()
{
	m_ppList.DeleteAllItems();
	S_CaliParam* CaliParam = &((checkPatch*)m_pData)->m_CaliParam;
	std::vector<cv::Point2f>& scene = CaliParam->scene;
	int len = scene.size();
	if (0 == len)
		return;
	/*int len2 = CaliParam->CaliNum;
	if (len != len2)
		return;*/
	std::vector<cv::Point2f>& obj = CaliParam->object;
	if (obj.empty())
	{
		for (int i = 0; i < len; i++)
			obj.push_back(cv::Point2f(0, 0));
	}

	char sz[64];
	for (int i = 0; i < len; i++)
	{
		sprintf(sz, "%f", scene[i].x);
		m_ppList.InsertItem(i, sz, 0);
		sprintf(sz, "%f", scene[i].y);
		m_ppList.SetItemText(i, 1, sz);

		sprintf(sz, "%f", obj[i].x);
		m_ppList.SetItemText(i, 2, sz);
		sprintf(sz, "%f", obj[i].y);
		m_ppList.SetItemText(i, 3, sz);
	}
	
}

void CaliDlg::OnBnClickedBtCali()
{
	// 读取list控件的xy位姿和定位坐标 仿射变换  保存结果
	BtCali();
	
}

//void CaliDlg::listToParam()
//{
//	S_CaliParam* CaliParam = &((checkPatch*)m_pData)->m_CaliParam;
//	vector<cv::Point2f>& obj = CaliParam->object;
//	vector<cv::Point2f>& scene = CaliParam->scene;
//	obj.clear();
//	scene.clear();
//	CString str;
//	float x, y;
//	for (int i = 0; i < CaliParam->CaliNum; i++)
//	{
//		str = m_ppList.GetItemText(i, 0);
//		x = atof(str.GetBuffer(0));
//		str = m_ppList.GetItemText(i, 1);
//		y = atof(str.GetBuffer(0));
//		scene.push_back(cv::Point2f(x, y));
//		str = m_ppList.GetItemText(i, 2);
//		x = atof(str.GetBuffer(0));
//		str = m_ppList.GetItemText(i, 3);
//		y = atof(str.GetBuffer(0));
//		obj.push_back(cv::Point2f(x, y));
//	}
//}

void CaliDlg::OnBnClickedBtCali3()
{
	// list清空  拍照读取图片 定位后根据仿射结果计算 机械手臂的要去的xy绝对位置
	CProcDll* pCheck = ((checkPatch*)m_pData)->m_CaliParam.pCheck;
	CoordinatePosXYZ pd;
	if (-1 == pCheck->camPic(pCheck->m_curCamId, pd.vm))
	{
		return;
	}
	
	pd.m_procid = pCheck->m_picProcId;
	if (-1 < pCheck->procCheck(&pd))//定位成功
	{
		Point2f pt;
		if (((checkPatch*)m_pData)->m_CaliParam.getDistance(&pd.px, &pd.py, &pt.x, &pt.y))
		{
			CString str;
			str.Format("%f,%f", pt.x, pt.y);
			MessageBox(str, "目标位置");
		}
	}
}

void CaliDlg::BtCali()
{
	int len = m_ppList.GetItemCount();
	if (len == 0)
	{
		return;
	}
	S_CaliParam& CaliParam = ((checkPatch*)m_pData)->m_CaliParam;
	{
		std::vector<cv::Point2f>& obj = CaliParam.object;
		std::vector<cv::Point2f>& scene = CaliParam.scene;
		obj.clear();
		scene.clear();
		CString str;
		float x, y;
		for (int i = 0; i < len; i++)
		{
			str = m_ppList.GetItemText(i, 0);
			x = atof(str.GetBuffer(0));
			str = m_ppList.GetItemText(i, 1);
			y = atof(str.GetBuffer(0));
			scene.push_back(cv::Point2f(x, y));

			str = m_ppList.GetItemText(i, 2);
			x = atof(str.GetBuffer(0));
			str = m_ppList.GetItemText(i, 3);
			y = atof(str.GetBuffer(0));
			obj.push_back(cv::Point2f(x, y));
		}
		CaliParam.CaliNum = len;
		CaliParam.setHomMat();		
	}
}

BOOL CaliDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ppList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ppList.InsertColumn(0, "像素x", LVCFMT_CENTER, 80);
	m_ppList.InsertColumn(1, "像素y", LVCFMT_LEFT, 80);
	m_ppList.InsertColumn(2, "距离x", LVCFMT_CENTER, 80);
	m_ppList.InsertColumn(3, "距离y", LVCFMT_LEFT, 80);
	m_ppList.SetColumnCtrlType(0, CCT_EDITBOX);
	m_ppList.SetColumnCtrlType(1, CCT_EDITBOX);
	m_ppList.SetColumnCtrlType(2, CCT_EDITBOX);
	m_ppList.SetColumnCtrlType(3, CCT_EDITBOX);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CaliDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	((checkPatch*)m_pData)->m_CaliParam.writecfg(((checkPatch*)m_pData)->m_cfgDir);
	CDialogEx::OnOK();
}
