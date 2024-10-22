#include "procInterface.h"
//CThreadMgr g_threadMgr;
CProcGUIDll::CProcGUIDll()
{
}
CProcGUIDll::~CProcGUIDll()
{
	Proc_exitOpencvGui();
}
//name ��̬����ļ���
bool CProcGUIDll::Load(string dllname)
{
	m_dllname = dllname;
	//		WaitAndSignal m(processLock);
	if (IsLoaded())
		return true;
#if defined(WIN32)
	dllname += ".dll";
#else
	dllname += ".so";
#endif
	if (!DynaLink::Open(dllname.c_str())) {
	//	viewErr(dllname+"\tFailed to load a library");
		return false;
	}

	if (!GetFunction("SetOpencvGuiDir", (Function &)Proc_SetOpencvGuiDir)) {
	//	viewErr(dllname+"\tFailed to load Proc_SetOpencvGuiDir");
		return false;
	}
	if (!GetFunction("RunOpencvGuiDlg", (Function &)Proc_RunOpencvGuiDlg)) {
	//	viewErr(dllname+"\tFailed to load Proc_RunOpencvGuiDlg");
		return false;
	}
	if (!GetFunction("VisibleOpencvGuiDlg", (Function &)Proc_VisibleOpencvGuiDlg)) {
	//	viewErr(dllname+"\tFailed to load Proc_VisibleOpencvGuiDlg");
		return false;
	}
	if (!GetFunction("updateMain", (Function &)Proc_updateMain)) {
	//	viewErr(dllname+"\tFailed to load Proc_updateMain");
		return false;
	}
	if (!GetFunction("updateCurProc", (Function &)Proc_updateCurProc)) {
	//	viewErr(dllname+"\tFailed to load Proc_updateCurProc");
		return false;
	}
	if (!GetFunction("viewMainWin", (Function &)Proc_viewMainWin)) {
	//	viewErr(dllname+"\tFailed to load Proc_viewMainWin");
		return false;
	}
	if (!GetFunction("DllPreTranslateMessage_opencv_gui", (Function &)Proc_DllPreTranslateMessage_opencv_gui)) {
	//	viewErr(dllname+"\tFailed to load Proc_DllPreTranslateMessage_opencv_gui");
		return false;
	}
	if (!GetFunction("exitOpencvGui", (Function &)Proc_exitOpencvGui)) {
	//	viewErr(dllname+"\tFailed to load Proc_exitOpencvGui");
		return false;
	}
	if (!GetFunction("DllOnSize_opencv_gui", (Function &)Proc_DllOnSize_opencv_gui)) {
	//	viewErr(dllname+"\tFailed to load Proc_DllOnSize_opencv_gui");
		return false;
	}
	isLoadedOK = true;
	return true;
}




CProcDll::CProcDll()
{
	bool b = m_guidll.Load("opencv_gui");
	b = Load("dllCheck");
	//if(b==false)
	//	MessageBox(0,"���ض�̬�⣨opencv_gui.dll��ʧ��", "ϵͳ��ʾ", MB_YESNO | MB_ICONQUESTION) ;

}

CProcDll::~CProcDll()
{
	exitOpencvGui();
}

void CProcDll::SetOpencvGuiDir( std::string str)
{
	try  
	{ 
		m_guidll.Proc_SetOpencvGuiDir(str);
	}
	catch(...) 
	{  
		//viewErr("Proc_SetOpencvGuiDir ���ó���");
	}
}

// ���ó�ʼ�����Ƴ̴����λ��
void CProcDll::RunOpencvGuiDlg( void* hwnd  )
{
	try  
	{ 
        m_guidll.Proc_RunOpencvGuiDlg(hwnd);
	}
	catch(...) 
	{  
		//viewErr("Proc_RunOpencvGuiDlg ���ó���");
	}
}

//��ʾ�͹ر��Ƴ̽��洰��
void CProcDll::VisibleOpencvGuiDlg( int b )
{
	try  
	{ 
		m_guidll.Proc_VisibleOpencvGuiDlg(b);
	}
	catch(...) 
	{  
		//viewErr("Proc_VisibleOpencvGuiDlg ���ó���");
	}
}
//��������ͼ���Ƴ���ͼ
void CProcDll::updateMain(vector<cv::Mat>& vm)
{
	try  
	{ 
		m_guidll.Proc_updateMain(vm);
	}
	catch(...) 
	{  
		//viewErr("Proc_updateMain ���ó���");
	}
}

//���µڶ��Ƴ� m.dataΪ�� �ͻ�ԭΪ���Ƴ�
void CProcDll::updateCurProc(vector<cv::Mat>& vm)
{
	try  
	{ 
		m_guidll.Proc_updateCurProc(vm);
	}
	catch(...) 
	{  
		//viewErr("Proc_updateCurProc ���ó���");
	}
}

//������ͼƬ��ʾ
void CProcDll::viewMainWin(vector<cv::Mat>& vm)
{
	try  
	{ 
		m_guidll.Proc_viewMainWin(vm);
	}
	catch(...) 
	{  
		//viewErr("Proc_viewMainWin ���ó���");
	}
}
//ֱ��ͼ��ͨ���ؼ���д��Ҫ �������¼��Ļش�
int CProcDll::DllPreTranslateMessage_opencv_gui(MSG *pMsg)
{
	try  
	{
		return m_guidll.Proc_DllPreTranslateMessage_opencv_gui(pMsg);
	}
	catch(...)
	{  
		//viewErr("Proc_DllPreTranslateMessage_opencv_gui ���ó���");
	}

	return -1;
}

//��ʽ�����˳��ӿ�
void CProcDll::exitOpencvGui()
{
	try  
	{ 
		m_guidll.Proc_exitOpencvGui();
		dll_ExitDll(m_pCheck);
	}
	catch(...)
	{  
		//viewErr("Proc_exitOpencvGui ���ó���");
	}
}
void CProcDll::DllOnSize_opencv_gui(float fsp[2])
{
	try  
	{ 
		m_guidll.Proc_DllOnSize_opencv_gui(fsp);
	}
	catch(...)
	{  
		//viewErr("Proc_DllOnSize_opencv_gui ���ó���");
	}
}

//�Ƴ�i�����ü���
bool CProcDll::loadprocCheck(int i, string  dir)
{
	try  
	{ 
		return CCheckDll::dll_loadprocCheck(i,dir,m_pCheck);
	}
	catch(...) 
	{  
		//viewErr("Proc_loadprocCheck ���ó���");
	}
	return false;
}

void CProcDll::setViewName(string winname)
{
	try
	{
		CCheckDll::dll_setViewName(winname, m_pCheck);
	}
	catch (...)
	{
		//viewErr("setViewName ���ó���");
	}
}

int CProcDll::procCheck(CoordinatePosXYZ* pd)
{
	try
	{
		return CCheckDll::dll_procCheck( pd, m_pCheck);
	}
	catch (...)
	{
		//viewErr("procCheck ���ó���");
	}
	return -1;
}
