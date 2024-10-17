#include "pch.h"
#include "checkPatch.h"

checkPatch::checkPatch()
{

}

checkPatch::~checkPatch()
{
}

void checkPatch::readMode(string dir)
{
	if (dir != "")
	{
		pww_mkdir(dir.c_str());
		m_cfgDir = dir;
	}
	
	m_CaliParam.loadcfg(m_cfgDir);
}

void checkPatch::writeMode(string dir)
{
	if (dir != "")
	{
		pww_mkdir(dir.c_str());
		m_cfgDir = dir;
	}
	
}

