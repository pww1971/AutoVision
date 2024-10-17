// CfgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MOVE_CARD.h"
#include "CfgDlg.h"
#include "afxdialogex.h"
#include "io.h"

// CCfgDlg 对话框

IMPLEMENT_DYNAMIC(CCfgDlg, CDialog)

CCfgDlg::CCfgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCfgDlg::IDD, pParent)
{
	 m_x=0;
	 m_y=1;
}

CCfgDlg::~CCfgDlg()
{
}

void CCfgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_listCarParam);
}


BEGIN_MESSAGE_MAP(CCfgDlg, CDialog)
	ON_BN_CLICKED(IDC_BT_MTSET, &CCfgDlg::OnBnClickedBtMtset)
	ON_BN_CLICKED(IDC_BT_AddMvPa, &CCfgDlg::OnBnClickedBtAddmvpa)
	ON_BN_CLICKED(IDC_BT_DelMvPa, &CCfgDlg::OnBnClickedBtDelmvpa)
	ON_BN_CLICKED(IDOK, &CCfgDlg::OnBnClickedOk)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CCfgDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_CHECK2, &CCfgDlg::OnBnClickedCheck2)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CCfgDlg::OnCbnSelchangeCombo2)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CCfgDlg::OnCbnSelchangeCombo3)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CCfgDlg 消息处理程序
BOOL CCfgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	(GetDlgItem(IDC_EDIT_IPADD))->SetWindowText(m_handle.m_linkAdd.c_str());
	m_listCarParam.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	viewCarParam();
	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(2);
	((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(1);
#ifdef _SIMULATION
	(GetDlgItem(IDC_EDIT_IONUM))->SetWindowText(GDKT::CStrProcess::itoa_radixN(m_handle.m_ioNum).c_str());
#else
	GetDlgItem(IDC_EDIT_IONUM)->ShowWindow(0);
	GetDlgItem(IDC_STATIC_IONUM)->ShowWindow(0);
#endif
	SetTimer(1,300,NULL);
	SetTimer(2,500,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CCfgDlg::viewCarParam()
{
	m_listCarParam.DeleteAllItems();
	CHeaderCtrl* pHeader=m_listCarParam.GetHeaderCtrl();
	int Count=pHeader->GetItemCount();
	for (int i=Count-1;i>=0;i--)
	{
		m_listCarParam.DeleteColumn(i);
	}
	//初始化显示字符表
	map<string,bool> mpName;
	char* szContent[] = { CST_AxisID,CST_NegLimit,CST_PosLimit,\
		CST_ManualSpeed, CST_Speed,\
		CST_ZeroSpeed, CST_ZeroMode/*,CST_BearGuide*/};
	int mplen = sizeof(szContent)/sizeof(char*);
	for (int i=0;i<mplen;i++)
	{
		mpName.insert(pair<string,bool>(szContent[i],true));
	}
	
	std::vector<string>& vctCfgName = m_handle.m_vctCfgName;
	int cLen = vctCfgName.size();
	for (int i=0;i<cLen;i++)
	{
		if (mpName.find(vctCfgName[i]) != mpName.end())
		{
			m_listCarParam.InsertColumn(i, vctCfgName[i].c_str(), LVCFMT_CENTER, 80);
		}
		else
			m_listCarParam.InsertColumn(i, vctCfgName[i].c_str(), LVCFMT_CENTER, 0);
		m_listCarParam.SetColumnCtrlType(i, CCT_EDITBOX);
	}

	std::vector<string> mvpa;
	if(m_handle.m_bIni)
		m_handle.getParam(mvpa);
	int palen = mvpa.size();
	for (int i=0;i<palen;i++)
	{
		std::vector<string> command;
		CStrProcess::Split(mvpa[i],MVFLAG,command,true);
		INT comlen = command.size()+1;
		if(2>comlen)
			continue;
		m_listCarParam.InsertItem(i,CStrProcess::itoa_radixN(i).c_str(),0);
		for (int j=1;j<comlen;j++)
		{
			m_listCarParam.SetItemText(i,j,command[j-1].c_str());
		}
		//if (m_handle.m_bIni )
		//{
		//	m_handle.setSpeed(i,atof(command[comlen-1].c_str()));//最后一个必须是手动速度
		//}
	}
}

void CCfgDlg::OnBnClickedBtMtset()
{
	CString str ;
	(GetDlgItem(IDC_EDIT_IPADD))->GetWindowText(str);
	if(m_handle.ini(str.GetBuffer(0)))
	{
		CHeaderCtrl* pHeaderCtrl = m_listCarParam.GetHeaderCtrl();
		if(pHeaderCtrl)
		{
			int count = m_listCarParam.GetItemCount();
			int colnum=0;
			string content="";
			colnum = pHeaderCtrl->GetItemCount();
			for (int i=0;i<count;i++)
			{
				for (int j=1;j<colnum;j++)//第一个是编号
				{
					content += m_listCarParam.GetItemText(i, j)+MVFLAG;
				}
				content += CHAINFLAG;
			}
			if(count>0)
			{
				m_handle.m_mtNum = count;
				if (m_handle.m_bIni)
					m_handle.setParam(content);
			}

			CConfig cfg(getExePath()+MOVECFG);
			string szVal = cfg.getItem("CurDir");
			if (_access(szVal.c_str(),0) != -1)
				updateMoveCard(szVal + "MoveCfg");
		}
	}
}

void CCfgDlg::updateMoveCard(string cfgfile)
{
	if (m_handle.m_bIni)
	{
		std::vector<string> mvParam;
		m_handle.getParam(mvParam);//读取当前运动参数

		CConfig cfg(cfgfile);
		cfg.updateItem("\n#设置运动卡参数","");
		int len = mvParam.size();
		cfg.updateItem("MoveParamNum",GDKT::CStrProcess::itoa_radixN(len));
		for (int i=0;i<len;i++)
		{
			cfg.updateItem("MoveParam"+GDKT::CStrProcess::itoa_radixN(i),mvParam[i]);
		}
#ifdef _SIMULATION
		CString str;
		(GetDlgItem(IDC_EDIT_IONUM))->GetWindowText(str);
		m_handle.m_ioNum = atoi(str.GetBuffer(0));
		cfg.updateItem("ioNum", GDKT::CStrProcess::itoa_radixN(m_handle.m_ioNum));
#endif
	}
}

void CCfgDlg::OnBnClickedBtAddmvpa()
{
	std::vector<float>& vctData = m_handle.m_vctDefaultData;
	int comlen = vctData.size()+1;
	int count = m_listCarParam.GetItemCount();
	m_listCarParam.InsertItem(count,CStrProcess::itoa_radixN(count).c_str(),0);
	char sz[32];
	for (int j=1;j<comlen;j++)
	{
		sprintf(sz,"%.3f",vctData[j-1]);
		m_listCarParam.SetItemText(count,j,sz);
	}
}

void CCfgDlg::OnBnClickedBtDelmvpa()
{
	m_listCarParam.DeleteItem(m_listCarParam.GetItemCount()-1);
}

void CCfgDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	(GetDlgItem(IDC_EDIT_IPADD))->GetWindowText(str);
	if(m_handle.ini(str.GetBuffer(0)))
	{
		;
	}
	else
	{
		MessageBox(CST_INIError);
		return;
	}
	CDialog::OnOK();
}



void CCfgDlg::OnDestroy()
{
	CDialog::OnDestroy();
	KillTimer(1);
	KillTimer(2);
	m_handle.setSpeed(true);

}


void CCfgDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent==1)
	{
		if (m_handle.m_bIni && m_handle.m_mtNum > m_y && m_handle.m_mtNum > m_x)
		{
			CString str;
			float xpos=0;
			m_handle.getMtPos(m_x,xpos);
			float ypos=0;
			m_handle.getMtPos(m_y,ypos);
			int xstate=-1,ystate=-1;
			try
			{
				m_handle.getMtState(m_x,xstate);
				m_handle.getMtState(m_y,ystate);
				char* xinfo = xstate==0? CST_MoveRun : CST_MoveStop;
				char* yinfo = ystate==0? CST_MoveRun : CST_MoveStop;
				str.Format("x:=%f\tx:%s\r\ny:=%f\ty:%s",xpos,xinfo,ypos,yinfo);
				GetDlgItem(IDC_STATIC_State)->SetWindowText(str);
			}
			catch (char* e)
			{
				GetDlgItem(IDC_STATIC_State)->SetWindowText(e);
			}
		}
	}
	else if (nIDEvent==2)
	{
		if (m_handle.m_bIni)
		{
			static int nID[] = {IDC_RADIO1,IDC_RADIO2,IDC_RADIO3,IDC_RADIO4,IDC_RADIO15,IDC_RADIO6,IDC_RADIO7,IDC_RADIO8,IDC_RADIO9,IDC_RADIO10,IDC_RADIO11,IDC_RADIO12,IDC_RADIO13,IDC_RADIO14,IDC_RADIO15,IDC_RADIO16};
			u_int iostate[g_ioN];
			m_handle.getIO(iostate);
			for (int i=0;i<16;i++)
			{
				((CButton*)GetDlgItem(nID[i]))->SetCheck(iostate[i]);
			}
		}
	}
	CDialog::OnTimer(nIDEvent);
}


void CCfgDlg::OnCbnSelchangeCombo1()
{
	//int id = ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();
	CString str;
	((CComboBox*)GetDlgItem(IDC_COMBO1))->GetWindowTextA(str);
	m_step = atof(str.GetBuffer(0));
}


BOOL CCfgDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_handle.m_bIni)
	{
		if(pMsg->message == WM_LBUTTONDOWN)
		{
			if(pMsg->hwnd == GetDlgItem(IDC_BUTTON1)->m_hWnd)
			{
				if(((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck())
					m_handle.moveV(m_x,-1);
				else
					m_handle.move(m_x,-m_step);
			}
			else if(pMsg->hwnd == GetDlgItem(IDC_BUTTON2)->m_hWnd)
			{
				if(((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck())
					m_handle.moveV(m_x,1);
				else
					m_handle.move(m_x,m_step);
			}
			else if(pMsg->hwnd == GetDlgItem(IDC_BUTTON3)->m_hWnd)
			{
				if(((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck())
					m_handle.moveV(m_y,-1);
				else
					m_handle.move(m_y,-m_step);
			}
			else if(pMsg->hwnd == GetDlgItem(IDC_BUTTON4)->m_hWnd)
			{
				if(((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck())
					m_handle.moveV(m_y,1);
				else
					m_handle.move(m_y,m_step);
			}
		}
		else if(pMsg->message == WM_LBUTTONUP)
		{
			if(((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck())
			{
				if(pMsg->hwnd == GetDlgItem(IDC_BUTTON1)->m_hWnd)
				{
					m_handle.moveStop(m_x);
				}
				else if(pMsg->hwnd == GetDlgItem(IDC_BUTTON2)->m_hWnd)
				{
					m_handle.moveStop(m_x);
				}
				else if(pMsg->hwnd == GetDlgItem(IDC_BUTTON3)->m_hWnd)
				{
					m_handle.moveStop(m_y);
				}
				else if(pMsg->hwnd == GetDlgItem(IDC_BUTTON4)->m_hWnd)
				{
					m_handle.moveStop(m_y);
				}
			}
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}


void CCfgDlg::OnBnClickedCheck2()
{
	if(m_handle.m_bIni)
	{
		m_handle.setSpeed(((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck());
	}
}


void CCfgDlg::OnCbnSelchangeCombo2()
{
	m_y = ((CComboBox*)GetDlgItem(IDC_COMBO2))->GetCurSel();
}


void CCfgDlg::OnCbnSelchangeCombo3()
{
	m_x = ((CComboBox*)GetDlgItem(IDC_COMBO3))->GetCurSel();
}


void CCfgDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	MessageBox(m_handle.m_szHelp.c_str());
	CDialog::OnLButtonDblClk(nFlags, point);
}
