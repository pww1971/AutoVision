// new_move_device.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "new_move_device.h"
#include "excel9.h"
#include "comdef.h" 
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// Cnew_move_deviceApp

BEGIN_MESSAGE_MAP(Cnew_move_deviceApp, CWinApp)
END_MESSAGE_MAP()


// Cnew_move_deviceApp 构造

Cnew_move_deviceApp::Cnew_move_deviceApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 Cnew_move_deviceApp 对象

Cnew_move_deviceApp theApp;
#ifdef MYSQLTable
class myReportTable :public CTable
{
public:
	myReportTable(){};
	bool find(string sql)
	{
		mysql_real_query(m_DB,sql.c_str(),sql.size());
		MYSQL_RES *result = mysql_store_result(m_DB);
		if (mysql_num_rows(result))
		{
			MYSQL_ROW row=mysql_fetch_row(result);
			u_long *lengths = mysql_fetch_lengths(result);
			char *p = row[4];
			u_long len = lengths[4];
			return true;
		}
		else
			return false;
	}

};
myReportTable g_DB;
#endif

void exceloutput(void* p2) 
{
	static bool comhasini=false;
	if (!comhasini)
	{
		if (CoInitialize(NULL)!=0) 
		{ 
			AfxMessageBox("初始化COM支持库失败!"); 
			return;
		} 		
		else
			comhasini=true; 
	}

	// TODO: Add your control notification handler code here
	_Application ExcelApp; 
	Workbooks wbsMyBooks; 
	_Workbook wbMyBook; 
	Worksheets wssMysheets; 
	_Worksheet wsMysheet; 
	::Range rgMyRge; 	
	//创建Excel 2000服务器(启动Excel) 
	if (!ExcelApp.CreateDispatch("Excel.Application",NULL)) 
	{ 
		AfxMessageBox("创建Excel服务失败!"); 
		return; 
	} 
	ExcelApp.SetVisible(false); 
	//利用模板文件建立新文档 
	CString strPath = ((S_DeviceParam*)p2)->m_reportAddress.c_str();
	wbsMyBooks.AttachDispatch(ExcelApp.GetWorkbooks(),true); 
	wbMyBook.AttachDispatch(wbsMyBooks.Add(_variant_t(strPath)));
	//得到Worksheets 
	wssMysheets.AttachDispatch(wbMyBook.GetWorksheets(),true);
	//得到sheet1 
	wsMysheet.AttachDispatch(wssMysheets.GetItem(_variant_t("sheet1")),true);
	//添加模板个数
	int i=0;
	for(;i<4;i++){
		wsMysheet.Copy(vtMissing,_variant_t(wsMysheet));
	}
	CString str1;
	wsMysheet.AttachDispatch(wssMysheets.GetItem(_variant_t("sheet1")),true);
	str1 = "第1页";
	wsMysheet.SetName(str1);
	for(i=0;i<wssMysheets.GetCount()-1;i++){
		wsMysheet = wsMysheet.GetNext();
		str1.Format("第%d页",i+2);
		wsMysheet.SetName(str1);
	}
	wsMysheet.AttachDispatch(wssMysheets.GetItem(_variant_t("第1页")),true);
	//得到全部Cells，此时,rgMyRge是cells的集合 
	rgMyRge.AttachDispatch(wsMysheet.GetCells(),true); 
	//设置1行1列的单元的值 
	string src = "中国";
	string dst;
	ConvertGb2312ToBig5(dst,src);
	CString str=dst.c_str();

	rgMyRge.SetItem(_variant_t((long)1),_variant_t((long)1),_variant_t(str)); 
	rgMyRge.SetItem(_variant_t((long)2),_variant_t((long)2),_variant_t(str)); 


	//将表格保存
	strPath = getExePath().c_str();
	strPath += "report.xlsx";
	wsMysheet.SaveAs(strPath,vtMissing,vtMissing,vtMissing,vtMissing,
		vtMissing,vtMissing,vtMissing,vtMissing);
	ExcelApp.SetVisible(true); 
	wbMyBook.PrintPreview(_variant_t(false)); 
	//释放对象 
	rgMyRge.ReleaseDispatch(); 
	wsMysheet.ReleaseDispatch(); 
	wssMysheets.ReleaseDispatch(); 
	wbMyBook.ReleaseDispatch(); 
	wbsMyBooks.ReleaseDispatch(); 
	ExcelApp.ReleaseDispatch(); 	
}
// Cnew_move_deviceApp 初始化


BOOL Cnew_move_deviceApp::InitInstance()
{
	CWinApp::InitInstance();
	return TRUE;
}


string MoveDLLHelp()
{
	string szHelp = "\r\nreport=输出报表\r\nsetUPH=UPH清零\r\ntimebegin=计时开始 \r\ntimeend=计时结束\r\nokplus,ngplus=OK,NG计数\r\n";
	return szHelp;
}

bool MoveDLLIni(void* p,void*& p2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	S_DeviceParam* devParam;
	if (p2==NULL)
	{
		if (p==NULL)
		{
			return false;
		}
		CMoveBase* mb = (CMoveBase*)p;
		devParam = new S_DeviceParam();
		string szDir = mb->m_szCfgAdd;
		int pos = szDir.size()-strlen("MoveCfg");
		szDir = szDir.substr(0,pos);
		devParam->m_cfgAdd = szDir+"reportdllcfg";
		devParam->loadcfg();
		p2 = devParam;
	}
	else
		devParam = (S_DeviceParam*)p2;
	CTcpIpDlg mydlg;
	mydlg.m_pData = p2;
	if (IDOK == mydlg.DoModal())
	{
		if (mydlg.m_fNGRatio>0)
			devParam->m_fNGRatio = mydlg.m_fNGRatio;
		if (mydlg.m_reportAddress!="")
			devParam->m_reportAddress = mydlg.m_reportAddress;
		devParam->writecfg();
	}
	return true;
}

void MoveDLLStop()
{

}

//动态库中提取的标函数
bool MoveDLLMove(void* p,void*& p2,string& data)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CMoveBase* mb = (CMoveBase*)p;
	/*CMoveType*  handle = mb->m_mtc.m_handle;
	if (NULL!=handle)
	{
		handle->moveAbs(0,50);
	}*/
	if (NULL != mb)
	{
		S_DeviceParam* devParam;
		if (p2==NULL)
		{
			devParam = new S_DeviceParam();
			string szDir = mb->m_szCfgAdd;
			int pos = szDir.size()-strlen("MoveCfg");
			szDir = szDir.substr(0,pos);
			devParam->m_cfgAdd = szDir+"reportdllcfg";
			devParam->loadcfg();
			p2 = devParam;
		}
		else
			devParam = (S_DeviceParam*)p2;
		if (data == "setUPH")
		{
			devParam->uphZero();
		}
		else if (data == "report")
		{
			exceloutput(p2);
		}
		else if (data == "timebegin")
		{
			devParam->m_lock.lock();
			devParam->m_timer = clock();
			devParam->m_lock.unLock();
		}
		else if (data == "timeend")
		{
			devParam->m_lock.lock();
			devParam->m_count++;
			double dur = (double)(clock() - devParam->m_timer)/CLOCKS_PER_SEC;
			char sz[64];
			sprintf(sz,"产能:每时%d片,\tOK:%d, NG:%d\n",int(0.5+devParam->m_count*3600/dur),devParam->m_countOK,devParam->m_countNG);
			data = sz;//返回非空主界面会显示

			if( (float)devParam->m_countNG/devParam->m_count > devParam->m_fNGRatio )
			{
				devParam->m_lock.unLock();
				return false;
			}
			devParam->m_lock.unLock();
			return true;
		}
		else if (data == "okplus")
		{
			devParam->m_lock.lock();
			devParam->m_countOK++;
			devParam->m_lock.unLock();
		}
		else if (data == "ngplus")
		{
			devParam->m_countNG++;
		}
		else
		{
			data = "error format:" + data;
			return false;
		}
	}
	
	data="";//返回非空主界面会显示
	return true;
}

//必须显示的调用释放内存
void MoveDllRelease(void*& p)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (NULL!=p)
	{
		delete (S_DeviceParam*)p;
		p=NULL;
	}
}

void MoveDllWriteFile()
{

}


