// new_move_device.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "new_move_device.h"
#include "excel9.h"
#include "comdef.h" 
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// Cnew_move_deviceApp

BEGIN_MESSAGE_MAP(Cnew_move_deviceApp, CWinApp)
END_MESSAGE_MAP()


// Cnew_move_deviceApp ����

Cnew_move_deviceApp::Cnew_move_deviceApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� Cnew_move_deviceApp ����

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
			AfxMessageBox("��ʼ��COM֧�ֿ�ʧ��!"); 
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
	//����Excel 2000������(����Excel) 
	if (!ExcelApp.CreateDispatch("Excel.Application",NULL)) 
	{ 
		AfxMessageBox("����Excel����ʧ��!"); 
		return; 
	} 
	ExcelApp.SetVisible(false); 
	//����ģ���ļ��������ĵ� 
	CString strPath = ((S_DeviceParam*)p2)->m_reportAddress.c_str();
	wbsMyBooks.AttachDispatch(ExcelApp.GetWorkbooks(),true); 
	wbMyBook.AttachDispatch(wbsMyBooks.Add(_variant_t(strPath)));
	//�õ�Worksheets 
	wssMysheets.AttachDispatch(wbMyBook.GetWorksheets(),true);
	//�õ�sheet1 
	wsMysheet.AttachDispatch(wssMysheets.GetItem(_variant_t("sheet1")),true);
	//���ģ�����
	int i=0;
	for(;i<4;i++){
		wsMysheet.Copy(vtMissing,_variant_t(wsMysheet));
	}
	CString str1;
	wsMysheet.AttachDispatch(wssMysheets.GetItem(_variant_t("sheet1")),true);
	str1 = "��1ҳ";
	wsMysheet.SetName(str1);
	for(i=0;i<wssMysheets.GetCount()-1;i++){
		wsMysheet = wsMysheet.GetNext();
		str1.Format("��%dҳ",i+2);
		wsMysheet.SetName(str1);
	}
	wsMysheet.AttachDispatch(wssMysheets.GetItem(_variant_t("��1ҳ")),true);
	//�õ�ȫ��Cells����ʱ,rgMyRge��cells�ļ��� 
	rgMyRge.AttachDispatch(wsMysheet.GetCells(),true); 
	//����1��1�еĵ�Ԫ��ֵ 
	string src = "�й�";
	string dst;
	ConvertGb2312ToBig5(dst,src);
	CString str=dst.c_str();

	rgMyRge.SetItem(_variant_t((long)1),_variant_t((long)1),_variant_t(str)); 
	rgMyRge.SetItem(_variant_t((long)2),_variant_t((long)2),_variant_t(str)); 


	//����񱣴�
	strPath = getExePath().c_str();
	strPath += "report.xlsx";
	wsMysheet.SaveAs(strPath,vtMissing,vtMissing,vtMissing,vtMissing,
		vtMissing,vtMissing,vtMissing,vtMissing);
	ExcelApp.SetVisible(true); 
	wbMyBook.PrintPreview(_variant_t(false)); 
	//�ͷŶ��� 
	rgMyRge.ReleaseDispatch(); 
	wsMysheet.ReleaseDispatch(); 
	wssMysheets.ReleaseDispatch(); 
	wbMyBook.ReleaseDispatch(); 
	wbsMyBooks.ReleaseDispatch(); 
	ExcelApp.ReleaseDispatch(); 	
}
// Cnew_move_deviceApp ��ʼ��


BOOL Cnew_move_deviceApp::InitInstance()
{
	CWinApp::InitInstance();
	return TRUE;
}


string MoveDLLHelp()
{
	string szHelp = "\r\nreport=�������\r\nsetUPH=UPH����\r\ntimebegin=��ʱ��ʼ \r\ntimeend=��ʱ����\r\nokplus,ngplus=OK,NG����\r\n";
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

//��̬������ȡ�ı꺯��
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
			sprintf(sz,"����:ÿʱ%dƬ,\tOK:%d, NG:%d\n",int(0.5+devParam->m_count*3600/dur),devParam->m_countOK,devParam->m_countNG);
			data = sz;//���طǿ����������ʾ

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
	
	data="";//���طǿ����������ʾ
	return true;
}

//������ʾ�ĵ����ͷ��ڴ�
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


