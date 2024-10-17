#pragma once

#include "afxdialogex.h"
#include <json/json.h>
#pragma comment(lib,"jsoncpp.lib")
#pragma comment(lib,"MinMesSock - x64.lib")
extern "C" __declspec(dllimport) bool ValidateBoardWorkState(const char* IpAddr, const char* barcode, const char* StationName, const char* ProductName, char* outputMsg, int nLengthMsg);
extern "C" __declspec(dllimport) bool SubmitBoardWorkState(const char* IpAddr, const char* barcode, const char* StationName, const char* ProductName, int Slot_ID, bool result, char* outputMsg, int nLengthMsg);

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/DNS.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include <Poco/Net/HTTPCredentials.h>
#include "Poco/StreamCopier.h"
#include "Poco/NullStream.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include "Poco/Net/HTMLForm.h"

#include <sys/timeb.h>
string sysUsecTime4();
void runhttpClient(std::string body, std::string szuri, std::vector<std::string>& szret);
void objectValue(Json::Value& value);
void typeSwitch(Json::Value& value);
// CMesDlg 对话框

class CMesDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMesDlg)

public:
	CMesDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMesDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_list;
	void* m_pData;
	int m_auth;//管理权限
	void getparam();
	void updateCtrl(string szjson);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
};
