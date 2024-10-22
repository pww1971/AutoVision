
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


bool myAdapt(unsigned char*&ptr, int iChannel,CRegParam* params)
{
	if (!hsiTable.data)
	{
		ptr+=iChannel;
		viewTxt("hisTable is NULL");
		return false;
	}
	//rgb����256*��256*256���ľ�����в�����Ӧhsiλ��
	uchar * pTab = hsiTable.data + ((ptr[0]<<16)+(ptr[1]<<8)+ptr[2])*iChannel;

	int adaptValue=0;
	for( int i=0;i<iChannel;i++){
		if(params->mode[i])
		{
			if (params->sThdMax[i] < pTab[i] || pTab[i] < params->sThdMin[i])
				adaptValue++;
		}
		else
		{
			if (params->sThdMax[i] >= pTab[i] && pTab[i] >= params->sThdMin[i])
				adaptValue++;
		}
	}
	ptr+=iChannel;
	return adaptValue==iChannel;
};

checkPatch::checkPatch()
{
	if (!hsiTable.data)
	{
		hsiTable = cv::imread(getExePath()+"hsi.bmp");
	}

	m_conAdpt = CpwwRegionConnect::contourFilter;
	m_rgbAdpt = myAdapt;
	m_regAdpt = CpwwRegionConnect::regionFilter;
	m_sPicFeat = &m_PicF;

	m_dlg.m_pMyParam = &m_myParam;
}

checkPatch::~checkPatch()
{
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
	//	m_dlg.ShowWindow(0);
		m_dlg.m_dlgSucc = true;
		return true;
	}
	else
	{
		MessageBoxA(NULL,"����ʧ��","",0);
		return false;
	}
}

void checkPatch::cmpMask(cv::Mat& src,cv::Mat& pwwMask)
{
	int c = src.channels();
	int h = src.rows;
	int w = src.cols;

	unsigned char* ptrMask = pwwMask.data ;
	//������ģ��ʾͼ��
	unsigned char* ptrData = src.data ;
	for (int i=0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if(*ptrMask > 0)//
			{
				;
			}
			else
			{
				ptrData[0]=0;
				ptrData[1]=0;
				ptrData[2]=0;
			}
			ptrData += c;
			ptrMask++;
		}
	}
}

bool checkPatch::verticalPwwTreat(cv::Mat& src,	CTpltParam& pa)
{
	m_sParams = &pa;

	cv::Mat dst;
	cv::transpose(src,dst);//ԭͼ��ֱ
	S_PicMat psrc;
	S_picMsg::picMatPTMat(psrc, dst);
	m_PicF.~CPicFeater();
	m_PicF.mallocRegion();
	treat(psrc);		
	if (m_PicF.curLen == 0)				
	{
		return false;
	}

	cv::Mat mask = cv::Mat(dst.rows,dst.cols,CV_8UC1,cv::Scalar(0));
	getContourMask2(m_PicF,m_sParams->bound,mask,255);//��ȡmask
	cv::transpose(mask,mask);//��ֱ����
		
	/*cv::namedWindow("src", cv::WINDOW_NORMAL);
	cv::imshow("src",mask);
	cv::waitKey(1);*/

	//����
	RGBADAPT rgbAdptBK = m_rgbAdpt;
	short smin = m_sParams->sThdMin[0];
	short smax = m_sParams->sThdMax[0];
	bool mode = m_sParams->mode[0];

	m_sParams->sThdMin[0] = 1;
	m_sParams->sThdMax[0] = 255;
	m_sParams->mode[0] = true;
	m_rgbAdpt = rgbAdapt;

	S_picMsg::picMatPTMat(psrc, mask);//����ֱmaskͼ
	m_PicF.~CPicFeater();
	m_PicF.mallocRegion();
	treat(psrc);	

	//��ԭ
	m_sParams->sThdMin[0] = smin;
	m_sParams->sThdMax[0] = smax;
	m_sParams->mode[0] = mode;
	m_rgbAdpt = rgbAdptBK;

	if (m_PicF.curLen >= pa.iRegNumMin && m_PicF.curLen < pa.iRegNumMax)					//��Ŀ��
	{
		cv::Mat mask2 = cv::Mat(mask.rows,mask.cols,CV_8UC1,cv::Scalar(0));
		getContourMask2(m_PicF,m_sParams->bound,mask2,255);//��ȡmask2
		cmpMask(src,mask2);//����mask��ȡsrc

		/*if (pa.m_drawMask)
		{
			if (pa.m_drawMask == 2)
			{
				cv::Mat mask2 = cv::Mat(mask.rows,mask.cols,CV_8UC1,cv::Scalar(0));
				getContourMask2(m_PicF,m_sParams->bound,mask2,255);//��ȡmask2
				drawMask(src,mask2);
			}
			drawRegion(src, m_PicF);
		}*/
		char sz[128];
		sprintf(sz,"objNum %d",m_PicF.curLen);
		pa.m_dllPa.szRet = sz;
		return true;
	}

	return false;
}

//ͼƬ�Ĵ���	һ����ģ��ͼƬ��������ģ��ͼƬ������ת�ǶȺ�ƥ��
bool checkPatch::treatPicture(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa)
{
	if (!src.data )
		return false;
	/*cv::Mat dst;
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
	}*/

	if (pa.m_dllPa.ini<1)
	{		
		if (pa.m_dllPa.ini == 0 && "" != pa.m_dllPa.szfile)
		{
			readMode(pa.m_dllPa.szfile);
		}
		else//ini == -1
		{
			/*if(1 == m_dlg.DoModal())
			{
				writeMode(pa.m_dllPa.szfile);
			}*/
		}
		pa.m_dllPa.ini=1;
		return true;
	}
	else
	{
		//����ͼƬ
		if(verticalPwwTreat(src,pa))
		{
			return true;
		}
	}

	return false;
}

void checkPatch::readMode(string file)
{
	mkdir(file.c_str());
	file += "\\checkParam.cfg";
	//readParamFromTxt(file,*m_sParams);
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
	//writeParamToTxt(file,*m_sParams);
	CConfig cfg(file);
	//�Ѹ�����д�������ļ�
	cfg.updateItem("\n#��������","���ͺ͸����� ");
	cfg.updateItem("posx",CStrProcess::itoa_radixN(m_myParam.posx));
	char sz[64];
	sprintf(sz, "%f", m_myParam.ret);
	cfg.updateItem("ret",sz);
}

bool checkPatch::drawResult(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa)
{
	return true;
}
