
#include "stdafx.h"
#include "checkPatch.h"

CGuiContrlDlg checkPatch::m_dlg;

typedef struct _WND_INFO
{
	HWND hWnd;
	DWORD dwProcessId;
}WNDINFO, *PWNDINFO;

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	WNDINFO* pInfo = (WNDINFO*)lParam;
	DWORD dwProcessId = 0;
	GetWindowThreadProcessId(hWnd, &dwProcessId);//�õ�ö�ٳ��Ĵ��ھ����Ӧ�Ľ���ID.

	if(dwProcessId == pInfo->dwProcessId)//���ǲ�������Ҫ�����Ľ��̵�ID
	{
		pInfo->hWnd = hWnd;//�Ǿ��������������
		return FALSE;//ֹͣö��.
	}
	return TRUE;
}

HWND GetHwndByProcessId(DWORD dwProcessId)
{
	WNDINFO info = {0};
	info.hWnd = NULL;
	info.dwProcessId = dwProcessId;//��������Ҫ�ҵĽ��̵�ID
	EnumWindows(EnumWindowsProc, (LPARAM)&info);//��ʼö�ٴ���.
	return info.hWnd;
}

bool checkPatch::iniDlg()
{
	if (m_dlg.m_dlgSucc)
	{
		m_dlg.ShowWindow(1);
		return true;
	}
	HWND hMainWnd=FindWindowA("#32770","TOT�Ӿ�����ϵͳ");//ͨ���������ڷ�ʽ��ȡĿ����������ھ��
	//HWND hMainWnd=GetHwndByProcessId(GetCurrentProcessId());//ͨ�����̻�ȡĿ�������ڽ���.ע��:GetCurrentProcessIdΪĿ�����Pid
	if(hMainWnd!=0)
	{
		m_dlg.Create(IDD_DIALOG1,CWnd::FromHandle(hMainWnd));
		//m_dlg.ShowWindow(0);
		m_dlg.m_dlgSucc = true;
		return true;
	}
	else
	{
		MessageBoxA(NULL,"����ʧ��","",0);
		return false;
	}
}


checkPatch::checkPatch()
{
	//dst=5*img1;//�����ع�
	//dst=img1/5;//�����ع�
	//bitwise_and(img1,img2,dst);//�߼��룬�󽻼�
	//bitwise_or(img1,img2,dst);//�߼����󲢼�
	//bitwise_not(img1,dst);//�߼��ǣ��󲹼�
	//bitwise_xor(img1,img2,dst);//�����ͬΪ0������Ϊ1
}

checkPatch::~checkPatch()
{
}
void checkPatch::readMode(string file)
{
	mkdir(file.c_str());
	file += "\\checkParam.cfg";
	CConfig cfg(file);
	//�������ļ��ж�ȡ������
	string szVal = cfg.getItem("posx");	
	if (szVal != "")
	{
		m_myParam.posx = atoi(szVal.c_str());
	}
	szVal = cfg.getItem("ret");	
	if (szVal != "")
	{
		m_myParam.ret = atof(szVal.c_str());
	}
}

void checkPatch::writeMode(string file)
{
	mkdir(file.c_str());
	file += "\\checkParam.cfg";
	CConfig cfg(file);
	//�Ѹ�����д�������ļ�
	cfg.updateItem("\n#��������","���ͺ͸����� ");
	cfg.updateItem("posx",CStrProcess::itoa_radixN(m_myParam.posx));
	char sz[64];
	sprintf(sz, "%f", m_myParam.ret);
	cfg.updateItem("ret",sz);
}


//ͼƬ�Ĵ���	
bool checkPatch::treatPicture(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa)
{
	if (!src.data )
		return false;
	cv::Mat dst;
	if (src.channels()>1)
	{
		if (pa.m_dllPa.szParam!="")
		{
			cv::Mat m[3];
			cv::split(src,m);
			int id = atoi(pa.m_dllPa.szParam.c_str());
			if (id >-1 && id < 3)
			{
				dst = m[id];
			}
			else
				dst = m[0];
		}
		else
			cv::cvtColor(src, dst, CV_RGB2GRAY);
	}

	Hobject ho_Image0 = Mat2HObject(dst);
	if (ho_Image0.Id() == H_EMPTY_REGION)	
		return  false;
	if (pa.m_dllPa.ini<1)
	{		
		if (pa.m_dllPa.ini == 0 && "" != pa.m_dllPa.szfile)
		{
			readMode(pa.m_dllPa.szfile);
		}
		else//ini == -1
		{
			m_dlg.m_pMyParam = &m_myParam;
			if(1 == m_dlg.DoModal())
			{
				writeMode(pa.m_dllPa.szfile);
			}
		}
		pa.m_dllPa.ini=1;
		return true;
	}
	else
	{
		//����ͼƬ
		return true;
	}

	return false;
}

bool checkPatch::drawResult(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa)
{
	return true;
}
