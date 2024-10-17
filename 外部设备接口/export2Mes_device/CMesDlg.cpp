// CMesDlg.cpp: 实现文件
//

#include "pch.h"
#include "export2Mes_device.h"
#include "CMesDlg.h"
#include "afxdialogex.h"
string sysUsecTime4()
{
	//printf("====sysUsecTime4\n");
	struct timeb tv;
	struct tm* t;

	ftime(&tv);
	char sz[64];
	t = localtime(&tv.time);
	sprintf(sz, "%d-%d-%d %d:%d:%d:%ld", 1900 + t->tm_year, 1 + t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, tv.millitm);
	return sz;
}

void runhttpClient(std::string body, std::string szuri,std::vector<std::string>& szret)
{
	try
	{
		// json
	//	std::string body("{\"DATAFLAG\":\"01\",\"SN\":\"99093527**H231015AA10A01061\",\"PRID\":\"H005\",\"PNUMBERr\":\"HL06301\",\"GXID\":\"B001\",\"GXNAME\":\"螺母浮高\",\"RESULT\":\"True\",\"CheckTime\":\"2023-10-04 00:00 : 00\",\"SubmitTime\":\"2023-10-04 00:00:00\",\"CreateTime\":\"2023-10-04 17:41:10\"}");

		// uri
		//Poco::URI uri("http://58.253.84.177:36118/execute?action=HLSEARCH");
		//Poco::URI uri("http://58.253.84.177:36118/execute?action=HLQC");
		Poco::URI uri(szuri);
		// session
		Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());
		session.setKeepAlive(true);

		// request
		Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, uri.getPathAndQuery(), Poco::Net::HTTPRequest::HTTP_1_1);
		request.setContentType("application/json");
		request.add("Accept", "Agent-007");
		request.add("User-Agent", "xxxxoooo");
		request.setContentLength(body.length());
		//
		session.sendRequest(request) << body;


		// response
		Poco::Net::HTTPResponse res;
		std::string recv_string;
		std::istream& is = session.receiveResponse(res);
		int result = (int)res.getStatus();
	//	std::cout << "result:" << result << ", reason:" << res.getReason() << std::endl;
		szret.push_back("result:" + CStrProcess::itoa_radixN(result) + ", reason:" + res.getReason());
		Poco::StreamCopier::copyToString(is, recv_string);
		//std::cout << "recv : " << std::endl << UTF_82ASCII(recv_string) << std::endl;
		szret.push_back(UTF_82ASCII(recv_string));
		std::string recv_body = res.get("body");
		//	std::cout << "body : " << std::endl << recv_body << std::endl;
		szret.push_back(recv_body);
	}
	catch (Poco::Exception& exc)
	{
		szret.push_back(exc.displayText());
	}
}
void objectValue(Json::Value& value)
{
	Json::Value::Members mem = value.getMemberNames();
	for (auto iter = mem.begin(); iter != mem.end(); iter++) {
		printf("%s : ", (*iter).c_str());       // 打印键名
		typeSwitch(value[*iter]);
	}
}

void typeSwitch(Json::Value& value)
{
	switch (value.type())
	{
	case Json::objectValue:
		objectValue(value);
		break;

	case Json::arrayValue:
	{
		auto count = value.size();
		for (unsigned int i = 0; i < count; i++)
			typeSwitch(value[i]);
	}
	break;

	case Json::stringValue:
		printf("%s\n", value.asString().c_str());
		break;

	case Json::realValue:
		printf("%lf\n", value.asDouble());
		break;

	case Json::uintValue:
		printf("%u\n", value.asUInt());
		break;

	case Json::intValue:
		printf("%d\n", value.asInt());
		break;

	case Json::booleanValue:
		printf("%d\n", value.asBool());
		break;

	default:
		break;
	}
}

// CMesDlg 对话框

IMPLEMENT_DYNAMIC(CMesDlg, CDialogEx)

CMesDlg::CMesDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	m_pData = NULL;
	m_auth = -1;
}

CMesDlg::~CMesDlg()
{
}

void CMesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CMesDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMesDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CMesDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMesDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMesDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMesDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_CHECK1, &CMesDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CMesDlg::OnBnClickedCheck2)
END_MESSAGE_MAP()


// CMesDlg 消息处理程序


BOOL CMesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(((S_DeviceParam*)m_pData)->m_basePa.nSentAuth);
	((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(((S_DeviceParam*)m_pData)->m_basePa.nCheckRe);
	if (0 == m_auth || 1 == m_auth)
	{
		((CButton*)GetDlgItem(IDC_CHECK1))->ShowWindow(1);
		GetDlgItem(IDC_BUTTON4)->ShowWindow(1);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK1))->ShowWindow(0);
		GetDlgItem(IDC_BUTTON4)->ShowWindow(0);
	}
	// TODO:  在此添加额外的初始化
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.InsertColumn(0, "指令", LVCFMT_CENTER, 120);
	CRect crt;
	m_list.GetWindowRect(&crt);//获取控件的屏幕坐标
	m_list.InsertColumn(1, "数据", LVCFMT_LEFT, crt.Width() - 25);

	CIPAddressCtrl* pIP = (CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS1);
	std::vector<string> vct;
	CStrProcess::Split(((S_DeviceParam*)m_pData)->m_basePa.strIP, ".", vct);
	pIP->SetAddress(atoi(vct[0].c_str()), atoi(vct[1].c_str()), atoi(vct[2].c_str()), atoi(vct[3].c_str()));
	GetDlgItem(IDC_EDIT1)->SetWindowTextA(((S_DeviceParam*)m_pData)->m_basePa.strSN.c_str());
	GetDlgItem(IDC_EDIT2)->SetWindowTextA(((S_DeviceParam*)m_pData)->m_basePa.strProID.c_str());
	GetDlgItem(IDC_EDIT3)->SetWindowTextA(((S_DeviceParam*)m_pData)->m_basePa.strStaionName.c_str());
	GetDlgItem(IDC_EDIT4)->SetWindowTextA(((S_DeviceParam*)m_pData)->m_basePa.strProductName.c_str());
	GetDlgItem(IDC_EDIT5)->SetWindowTextA(CStrProcess::itoa_radixN(((S_DeviceParam*)m_pData)->m_basePa.nSlotID).c_str());
	
	GetDlgItem(IDC_EDIT8)->SetWindowTextA(((S_DeviceParam*)m_pData)->m_basePa.strPNUMBER.c_str());
	GetDlgItem(IDC_EDIT9)->SetWindowTextA(((S_DeviceParam*)m_pData)->m_basePa.strGXID.c_str());
	GetDlgItem(IDC_EDIT10)->SetWindowTextA(((S_DeviceParam*)m_pData)->m_basePa.strGXNAME.c_str());

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CMesDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	getparam();
	CDialogEx::OnOK();
}

void CMesDlg::OnBnClickedButton4()
{
	if (0 == ((S_DeviceParam*)m_pData)->m_basePa.nSentAuth)
		return;
	getparam();
	Json::Value value;
	((S_DeviceParam*)m_pData)->m_basePa.getValue(value);
	
	std::vector<std::string> szret;
	runhttpClient(value.toStyledString(),\
		"http://58.253.84.177:36118/execute?action=HLQC", szret);
	GetDlgItem(IDC_EDIT7)->SetWindowTextA(szret[0].c_str());
	GetDlgItem(IDC_EDIT6)->SetWindowTextA(szret[2].c_str());
	updateCtrl(szret[1]);
}

void CMesDlg::OnBnClickedButton1()
{
	getparam();
	Json::Value value;
	value["DATAFLAG"] = "02";//查询
	value["SN"] = ((S_DeviceParam*)m_pData)->m_basePa.strSN;
	value["PRID"] = ((S_DeviceParam*)m_pData)->m_basePa.strProID;

	std::vector<std::string> szret;
	runhttpClient(value.toStyledString(), \
		"http://58.253.84.177:36118/execute?action=HLSEARCH", szret);
	GetDlgItem(IDC_EDIT7)->SetWindowTextA(szret[0].c_str());
	GetDlgItem(IDC_EDIT6)->SetWindowTextA(szret[2].c_str());
	updateCtrl(szret[1]);
}

void CMesDlg::getparam()
{
	CIPAddressCtrl* pIP = (CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS1);
	BYTE nf1, nf2, nf3, nf4;
	pIP->GetAddress(nf1, nf2, nf3, nf4);
	CString str;
	str.Format("%d.%d.%d.%d", nf1, nf2, nf3, nf4);//这里的nf得到的值是IP值了
	((S_DeviceParam*)m_pData)->m_basePa.strIP = str;
	GetDlgItem(IDC_EDIT1)->GetWindowTextA(str);
	((S_DeviceParam*)m_pData)->m_basePa.strSN = str;
	GetDlgItem(IDC_EDIT2)->GetWindowTextA(str);
	((S_DeviceParam*)m_pData)->m_basePa.strProID = str;
	GetDlgItem(IDC_EDIT3)->GetWindowTextA(str);
	((S_DeviceParam*)m_pData)->m_basePa.strStaionName = str;
	GetDlgItem(IDC_EDIT4)->GetWindowTextA(str);
	((S_DeviceParam*)m_pData)->m_basePa.strProductName = str;
	GetDlgItem(IDC_EDIT5)->GetWindowTextA(str);
	((S_DeviceParam*)m_pData)->m_basePa.nSlotID = atoi(str.GetBuffer(0));

	GetDlgItem(IDC_EDIT8)->GetWindowTextA(str);
	((S_DeviceParam*)m_pData)->m_basePa.strPNUMBER = str;
	GetDlgItem(IDC_EDIT9)->GetWindowTextA(str);
	((S_DeviceParam*)m_pData)->m_basePa.strGXID = str;
	GetDlgItem(IDC_EDIT10)->GetWindowTextA(str);
	((S_DeviceParam*)m_pData)->m_basePa.strGXNAME = str;

}
void CMesDlg::OnBnClickedButton2()
{
	char strMSGInfo[MAX_PATH] = { 0 }; // 服务端接口执行返回的具体信息
	bool result;                  // 服务端接口执行的结果：true 成功，false 失败
	S_DeviceParam* devParam = (S_DeviceParam*)m_pData;
	result =ValidateBoardWorkState(devParam->m_basePa.strIP.c_str(), devParam->m_basePa.strSN.c_str(), devParam->m_basePa.strStaionName.c_str(), \
		devParam->m_basePa.strProductName.c_str(), strMSGInfo, sizeof(strMSGInfo));
	GetDlgItem(IDC_EDIT6)->SetWindowTextA(strMSGInfo);
	if (result)
		GetDlgItem(IDC_EDIT7)->SetWindowTextA("OK");
	else
		GetDlgItem(IDC_EDIT7)->SetWindowTextA("NG");
}

void CMesDlg::OnBnClickedButton3()
{
	bool bTestResult = false;   // true 成功，false 失败
	char strMSGInfo[MAX_PATH] = { 0 }; // 服务端接口执行返回的具体信息
	bool result;                  // 服务端接口执行的结果：true 成功，false 失败
	S_DeviceParam* devParam = (S_DeviceParam*)m_pData;
	result = SubmitBoardWorkState(devParam->m_basePa.strIP.c_str(), devParam->m_basePa.strSN.c_str(), devParam->m_basePa.strStaionName.c_str(), \
		devParam->m_basePa.strProductName.c_str(), devParam->m_basePa.nSlotID, bTestResult, \
		strMSGInfo, sizeof(strMSGInfo));
	GetDlgItem(IDC_EDIT6)->SetWindowTextA(strMSGInfo);
	if(bTestResult)
		GetDlgItem(IDC_EDIT7)->SetWindowTextA("OK");
	else 
		GetDlgItem(IDC_EDIT7)->SetWindowTextA("NG");
}

void CMesDlg::updateCtrl(string szjson)
{
	m_list.DeleteAllItems();
	// Json格式的字符串
	Json::Value value2;
	Json::Reader reader;
	if (!reader.parse(szjson, value2)) {
		// 解析失败
		return;
	}
	Json::Value::Members mem = value2.getMemberNames();
	int i = 0;
	for (auto iter = mem.begin(); iter != mem.end(); iter++) {
		//	printf("%s : ", (*iter).c_str());       // 打印键名
		//	typeSwitch(value2[*iter]);
		m_list.InsertItem(i, (*iter).c_str(), 0);
		m_list.SetItemText(i++, 1, value2[*iter].asString().c_str());
	}
}

void CMesDlg::OnBnClickedCheck1()
{
	((S_DeviceParam*)m_pData)->m_basePa.nSentAuth = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
}


void CMesDlg::OnBnClickedCheck2()
{
	((S_DeviceParam*)m_pData)->m_basePa.nCheckRe = ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck();
}
