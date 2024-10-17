#include "procInterface.h"
//CThreadMgr g_threadMgr;
CProcGUIDll::CProcGUIDll()
{
}
CProcGUIDll::~CProcGUIDll()
{
	Proc_exitOpencvGui();
}
//name 动态库的文件名
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
	//	MessageBox(0,"加载动态库（opencv_gui.dll）失败", "系统提示", MB_YESNO | MB_ICONQUESTION) ;

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
		//viewErr("Proc_SetOpencvGuiDir 调用出错");
	}
}

// 设置初始化的制程窗体和位置
void CProcDll::RunOpencvGuiDlg( void* hwnd  )
{
	try  
	{ 
        m_guidll.Proc_RunOpencvGuiDlg(hwnd);
	}
	catch(...) 
	{  
		//viewErr("Proc_RunOpencvGuiDlg 调用出错");
	}
}

//显示和关闭制程界面窗体
void CProcDll::VisibleOpencvGuiDlg( int b )
{
	try  
	{ 
		m_guidll.Proc_VisibleOpencvGuiDlg(b);
	}
	catch(...) 
	{  
		//viewErr("Proc_VisibleOpencvGuiDlg 调用出错");
	}
}
//更新主视图和制程视图
void CProcDll::updateMain(vector<cv::Mat>& vm)
{
	try  
	{ 
		m_guidll.Proc_updateMain(vm);
	}
	catch(...) 
	{  
		//viewErr("Proc_updateMain 调用出错");
	}
}

//更新第二制程 m.data为空 就还原为单制程
void CProcDll::updateCurProc(vector<cv::Mat>& vm)
{
	try  
	{ 
		m_guidll.Proc_updateCurProc(vm);
	}
	catch(...) 
	{  
		//viewErr("Proc_updateCurProc 调用出错");
	}
}

//主窗体图片显示
void CProcDll::viewMainWin(vector<cv::Mat>& vm)
{
	try  
	{ 
		m_guidll.Proc_viewMainWin(vm);
	}
	catch(...) 
	{  
		//viewErr("Proc_viewMainWin 调用出错");
	}
}
//直方图三通道控件重写需要 鼠标键盘事件的回传
int CProcDll::DllPreTranslateMessage_opencv_gui(MSG *pMsg)
{
	try  
	{
		return m_guidll.Proc_DllPreTranslateMessage_opencv_gui(pMsg);
	}
	catch(...)
	{  
		//viewErr("Proc_DllPreTranslateMessage_opencv_gui 调用出错");
	}

	return -1;
}

//显式调用退出接口
void CProcDll::exitOpencvGui()
{
	try  
	{ 
		m_guidll.Proc_exitOpencvGui();
		dll_ExitDll(m_pCheck);
	}
	catch(...)
	{  
		//viewErr("Proc_exitOpencvGui 调用出错");
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
		//viewErr("Proc_DllOnSize_opencv_gui 调用出错");
	}
}

//制程i的配置加载
bool CProcDll::loadprocCheck(int i, string  dir)
{
	try  
	{ 
		return CCheckDll::dll_loadprocCheck(i,dir,m_pCheck);
	}
	catch(...) 
	{  
		//viewErr("Proc_loadprocCheck 调用出错");
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
		//viewErr("setViewName 调用出错");
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
		//viewErr("procCheck 调用出错");
	}
	return -1;
}
