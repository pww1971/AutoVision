// TcpIpDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TcpIpDlg.h"
#include "afxdialogex.h"
// CTcpIpDlg 对话框

IMPLEMENT_DYNAMIC(CTcpIpDlg, CDialog)

CTcpIpDlg::CTcpIpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTcpIpDlg::IDD, pParent)
{
	/*if (NULL == g_totdll)
	{
		g_totdll=new CTotDll();
	}*/
}

CTcpIpDlg::~CTcpIpDlg()
{
	/*if (NULL != g_totdll)
	{
		delete g_totdll;
		g_totdll=NULL;
	}*/
}

void CTcpIpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_socketList);
}

BEGIN_MESSAGE_MAP(CTcpIpDlg, CDialog)
	ON_BN_CLICKED(IDC_BT_SERVERIP, &CTcpIpDlg::OnBnClickedBtServerip)
	ON_BN_CLICKED(IDC_BT_AUTO, &CTcpIpDlg::OnBnClickedBtAuto)
	ON_BN_CLICKED(IDC_BT_DELAUTO, &CTcpIpDlg::OnBnClickedBtDelauto)
END_MESSAGE_MAP()


// CTcpIpDlg 消息处理程序
BOOL CTcpIpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	//m_socketList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//m_socketList.InsertColumn(0, "客户端ip", LVCFMT_CENTER,150);
	//CIPAddressCtrl * pIP=(CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS1);
	//CConfig cfg(getExePath()+"tot2.cfg");
	//string szIp = cfg.getItem("SERVER_IP");	
	//if (szIp=="")
	//{
	//	cfg.updateItem("SERVER_IP","192.168.0.142");
	//	pIP->SetAddress(192,168,0,142);
	//	//	pIP->SetAddress((DWORD)0x7F000001);//这两行的效果都是一样的
	//}
	//else
	//{
	//	vector<string> vct;
	//	CStrProcess::Split(szIp,".",vct);
	//	pIP->SetAddress(atoi(vct[0].c_str()),atoi(vct[1].c_str()),atoi(vct[2].c_str()),atoi(vct[3].c_str()));

	//	string szVal = cfg.getItem("OPEN_ServeORCient");	
	//	if (szVal=="server")
	//	{
	//		if (g_totdll->m_server == NULL)
	//		{
	//			g_totdll->openServer();
	//		}
	//		if(g_totdll->m_server->openServer(szIp,6868)==1)
	//		{
	//			MessageBox("开启服务器成功");
	//		}
	//	}

	//}

	return TRUE;  
}

void CTcpIpDlg::OnBnClickedBtServerip()
{
	//CIPAddressCtrl * pIP=(CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS1);
	//BYTE nf1,nf2,nf3,nf4;
	//pIP->GetAddress(nf1,nf2,nf3,nf4);
	//CString str;
	//str.Format("%d.%d.%d.%d",nf1,nf2,nf3,nf4);//这里的nf得到的值是IP值了
	////	MessageBox(str);
	//if (g_totdll->m_server == NULL)
	//{
	//	g_totdll->openServer();
	//}
	//string szIp = str.GetBuffer(0);
	//if(g_totdll->m_server->openServer(szIp,6868)==1)
	//{
	//	CConfig cfg(getExePath()+"tot2.cfg");
	//	string szcfgip = cfg.getItem("SERVER_IP");	
	//	if (szIp!=szcfgip)
	//	{
	//		cfg.updateItem("SERVER_IP",szIp);
	//	}
	//	MessageBox("开启服务器成功");
	//}
}

void CTcpIpDlg::viewClientList()
{
	/*if (g_totdll->m_server == NULL)
	{
		return;
	}
	m_socketList.DeleteAllItems();
	vector<string > cont;
	CStrProcess::Split(g_totdll->m_server->getClientIPStr(),CHAINFLAG,cont,true);
	int len = cont.size();
	if(0==len)
		return ;
	for (int i=0;i<len;i++)
	{
		m_socketList.InsertItem(i,cont[i].c_str(),0);
	}*/
}

void CTcpIpDlg::OnBnClickedBtAuto()
{
	/*if (g_totdll->m_server != NULL)
	{
		CConfig cfg(getExePath()+"tot2.cfg");
		cfg.updateItem("OPEN_ServeORCient","server");
	}*/
}

void CTcpIpDlg::OnBnClickedBtDelauto()
{
	CConfig cfg(getExePath()+"tot2.cfg");
	cfg.updateItem("OPEN_ServeORCient","");
}
