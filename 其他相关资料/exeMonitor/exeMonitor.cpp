// totMonitor.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "afxdialogex.h"
#include <Tlhelp32.h>
bool IsExistProcess(const CHAR* szProcessName)
{
	PROCESSENTRY32 processEntry32; 
	HANDLE toolHelp32Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,  0);
	if (((int)toolHelp32Snapshot) != -1)
	{
		processEntry32.dwSize = sizeof(processEntry32);
		if (Process32First(toolHelp32Snapshot, &processEntry32))
		{
			do
			{
				if(strcmp(szProcessName ,processEntry32.szExeFile) == 0)
				{
					return TRUE;
				}
			}while (Process32Next(toolHelp32Snapshot, &processEntry32));
		}
		CloseHandle(toolHelp32Snapshot);
	}

	return false;
}

int _tmain(int argc, _TCHAR* argv[])
{
	while(1)
	{
		if (IsExistProcess("AutoVision.exe"))
			Sleep(1000);
		else
			WinExec("AutoVision.exe open", SW_SHOW);
	}
	return 0;
}

