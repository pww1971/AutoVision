
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

void checkPatch::cmpMask(cv::Mat& src,cv::Mat& pwwMask,CTpltParam& pa,bool bDraw)
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
			if(*ptrMask > 0 || i<pa.xMinConnNum || i>h-pa.xMinConnNum || j<pa.xMinConnNum || j>w-pa.xMinConnNum)//
			{
				if (bDraw)
				{
					ptrData[0]=0;
					ptrData[1]=0;
					ptrData[2]=0;
				}
			}
			else
			{
				ptrData[0]=255;
				ptrData[1]=255;
				ptrData[2]=255;
			}
			ptrData += c;
			ptrMask++;
		}
	}
}

bool checkPatch::verticalPwwTreat(cv::Mat& src,	CTpltParam& pa,bool bDraw)
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
		
	/*cv::imshow(DLLVIEW,mask);
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

	char sz[128];
	sprintf(sz,"objNum %d",m_PicF.curLen);
	pa.m_dllPa.szRet = sz;

	if (m_PicF.curLen >= pa.iRegNumMin && m_PicF.curLen < pa.iRegNumMax)					//��Ŀ��
	{
		cv::Mat mask2 = cv::Mat(mask.rows,mask.cols,CV_8UC1,cv::Scalar(0));
		getContourMask2(m_PicF,m_sParams->bound,mask2,255);//��ȡmask2
		cmpMask(src,mask2,pa,bDraw);//����mask��ȡsrc

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
		int id = m_tempPa.m_preTreat;
		if (id > 6 && id < 10)
		{
			cv::Mat m[3];
			cv::split(srcImg, m);
			srcImg = m[id - 7];
		}
		else
			cv::cvtColor(srcImg, srcImg, cv::COLOR_RGB2GRAY);
	}*/

	if (pa.m_dllPa.ini<1)
	{		
		if ("" != pa.m_dllPa.szfile)
		{
			readMode(pa.m_dllPa.szfile);
		}
		if (pa.m_dllPa.ini == 0 )
		{
			;
		}
		else//ini == -1
		{
			m_dlg.m_pCheck = this;
			g_MKC.updateFrame(src);
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
		if(soldered_dot(src,m_myParam))
		{
			return true;
		}
	}
	return false;
}

bool checkPatch::soldered_dot(cv::Mat&src,CTpltParam pa[])
{
	//1���Ⱥ��̷��� 
	cv::Mat m1 = src.clone();
	verticalPwwTreat(m1,m_myParam[0],true);
	//2���ٺ������ 
	cv::Mat m2 = src.clone();
	verticalPwwTreat(m2,m_myParam[1],true);
	//3�������� �ཻ����
	cv::Mat m3;
	cv::bitwise_not(m1,m3);//�õ�����ͼ��Ĳ� �����󺸵��ཻ
	//cv::imshow("sdf1",m1);
	//cv::imshow("sdf2",m2);
	//cv::imshow("sdf3",m3);
	cv::bitwise_and(m2,m3,src);//�󺸵��ཻ
	//4�����ཻ�� ����false
	
	return true;
}

void checkPatch::readMode(string file)
{
	mkdir(file.c_str());
	readParamFromTxt(file + "\\checkParam1.cfg",m_myParam[0]);
	readParamFromTxt(file + "\\checkParam2.cfg",m_myParam[1]);
}

void checkPatch::writeMode(string file)
{
	mkdir(file.c_str());
	writeParamToTxt(file + "\\checkParam1.cfg",m_myParam[0]);
	writeParamToTxt(file + "\\checkParam2.cfg",m_myParam[1]);
}

bool checkPatch::drawResult(cv::Mat& src,cv::RotatedRect& roiRect,CTpltParam& pa)
{
	//cv::Rect rect = roiRect.boundingRect();
	//int angle = 360-roiRect.angle;

	//rect &= cv::Rect(0, 0, src.cols, src.rows);
	//cv::Scalar c=cv::Scalar(0, 0, 255);
	//int size = src.cols*src.rows/(g_lineRate>>3);
	//int thickness = 1+size/8;
	//{
	//	float anglePI;
	//	float sa;
	//	float ca;
	//	if (angle != 0)//���¼�����ת���λ��
	//	{
	//		if (angle<0)
	//		{
	//			angle += 360;
	//		}
	//		if (angle>=360)
	//		{
	//			angle -= 360;
	//		}
	//		anglePI = (float) (-angle * CV_PI / 180);
	//		sa = sin(anglePI);
	//		ca = cos(anglePI);
	//	}
	//	int piccentx = rect.width/2;
	//	int piccenty = rect.height/2;
	//	float rrdx = roiRect.center.x - roiRect.size.width/2;
	//	float rrdy = roiRect.center.y - roiRect.size.height/2;
	//	for (int i = 0; i < m_PicF.curLen; i++)
	//	{
	//		CRegion& reg = m_PicF.region[i];
	//		int oL = reg.outline->curLenIR;
	//		CRegion::S_Ring* ring = reg.outline->sRingArray[reg.outline->outSign];//��Ӧ�Ļ�
	//		if (NULL == ring)
	//			continue;
	//		int len = ring->curLen;
	//		if (0 == len)
	//			continue;
	//		if (angle != 0)//���¼�����ת���λ��
	//		{
	//			float yDis = reg.y-piccentx;
	//			float xDis = reg.x-piccenty;

	//			reg.y = yDis*ca - xDis*sa ;
	//			reg.x = yDis*sa + xDis*ca ;

	//			reg.y += piccentx;
	//			reg.x += piccenty;
	//		}
	//		int ix = reg.x + rrdx;
	//		int iy = reg.y + rrdy;
	//		//���ƺ���
	//		int ib = (size>>2);
	//		cv::line(src, cv::Point(ix - ib, iy), cv::Point(ix + ib, iy), c, thickness, 8, 0);
	//		//��������
	//		cv::line(src, cv::Point(ix, iy - ib), cv::Point(ix, iy + ib), c, thickness, 8, 0);
	//	}
	//}
	return true;
}
