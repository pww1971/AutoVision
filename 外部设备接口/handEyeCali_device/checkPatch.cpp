#include "checkPatch.h"
mouseKeyCtr checkPatch::S_caliProc;
void onMouse(int event, int x, int y, int flag, void* p)
{
	/*if (event == CV_EVENT_MBUTTONDOWN)
	{
		*(string*)p = g_threadMgr.checkThread();
		return;
	}*/
	if (event == cv::EVENT_LBUTTONDOWN || event == cv::EVENT_LBUTTONUP || event == cv::EVENT_MOUSEMOVE)
	{
		checkPatch::S_caliProc.onMouse(event, x, y, flag, p);
	}
}

checkPatch::checkPatch()
{

}

checkPatch::~checkPatch()
{
#ifdef USE_HALCON
	clearMode();
#endif
}

void checkPatch::readMode(string dir)
{
	if (dir != "")
	{
		pww_mkdir(dir.c_str());
		m_cfgDir = dir;
	}
	S_CheckParam cfg0(dir + "\\checkParam.cfg");
	string szVal = cfg0.getItem("fileadd");
	if ("" != szVal)
	{
		m_caliDir = szVal;
		if (S_caliProc.m_curProc.loadTplt(szVal + PROCFILE))
		{
			S_caliProc.m_curProc.m_unit = -1;//输出原始像素坐标 m_DevParam.dSh
		}
	}
	m_CaliParam.loadcfg(m_cfgDir);
	m_DevParam.loadcfg(m_cfgDir);
	loadProc(m_DevParam.m_procAddress);
}

void checkPatch::writeMode(string dir)
{
	if (dir != "")
	{
		pww_mkdir(dir.c_str());
		m_cfgDir = dir;
	}
	remove((m_cfgDir + "\\checkParam.cfg").c_str());
	m_DevParam.writecfg(m_cfgDir);
	S_CheckParam cfg0(m_cfgDir + "\\checkParam.cfg");
	cfg0.updateItem("fileadd", m_caliDir);
}

bool checkPatch::loadProc(string szdir)
{
	m_DevParam.m_procAddress = szdir;
	if(m_Proc.loadTplt(szdir+PROCFILE))
	{
		m_Proc.m_unit = -1;//输出原始像素坐标 m_DevParam.dSh
		return true;
	}
	else
		return false;
}
