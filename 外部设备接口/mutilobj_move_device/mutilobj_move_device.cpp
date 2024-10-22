// new_move_device.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "mutilobj_move_device.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
//#include "checkPatch.h"
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

BEGIN_MESSAGE_MAP(Cnew_move_deviceApp, CWinAppEx)
END_MESSAGE_MAP()


// Cnew_move_deviceApp ����

Cnew_move_deviceApp::Cnew_move_deviceApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
	//	SetSystem("parallelize_operators", "true");
}


// Ψһ��һ�� Cnew_move_deviceApp ����

Cnew_move_deviceApp theApp;


// Cnew_move_deviceApp ��ʼ��

BOOL Cnew_move_deviceApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	/*UCI::pDis = new ST_MYDISP(&checkPatch::S_dlg);
	UCI::initFirst();*/
	return TRUE;
}

string MoveDLLHelp()
{
	string szHelp = "\r\n�����˶�, camid = ���ռ���Ŀ�궨λ���okλ���γ��µ�����\r\nmove,mvid = mvid�����ת�˶�	\
		\r\nmaskObjArr,camid = ���ռ��λ���γ�����";
	return szHelp;
}

bool MoveDLLIni(void* pmv, void*& p2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (pmv == NULL)
	{
		return false;
	}
	if (NULL == p2)
	{
		p2 = new CDevice();
	}
	return true;
}

void MoveDLLStop()
{

}
//��̬������ȡ�ı꺯��
bool MoveDLLMove(void* pmv, void*& p2, string& data)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CMoveBase* mb = (CMoveBase*)pmv;
	if (NULL != mb)
	{
		if (NULL == p2)
		{
			p2 = new CDevice();
		}

		std::vector<string> vct;
		CStrProcess::Split(data, ",", vct);
		if (vct.size() < 2)
		{
			data = "error format:" + data;
			return false;
		}
		else if (std::string::npos != vct[0].find(ARRAYMOVE))//����ok���������µ������˶�
		{
			CoordinatePosXYZ msg;
			int camid = atoi(vct[1].c_str());
			if (-1 == camid)
			{
				camid = mb->m_pCheck->m_curCamId;
			}
			if (-1 == mb->m_pCheck->camPic(camid, msg.vm))
			{
				data = "cam  fail��" + data;
				return false;
			}
			msg.m_procid = mb->m_pCheck->m_picProcId;
			S_MultiObjRet mtRet;
			if (-1 == mb->m_pCheck->procCheck(&msg, mtRet))
			{
				data = "cam check fail��" + data;
				return false;
			}
			CArrayMove& mtc = mb->m_mtc;//����������
			if (vct[0] == TRIGARRAYMOVE)//���������˶�
				mb->m_prob = 0;//��������

			mtc.m_table.clear();
			mtc.m_tbid = 0;//��ǰλ�ô��¿�ʼ
			mtc.m_prob = 1;//��ϵ��
			//��ȡokλ��   
			cv::Point2f* dis = mtc.m_handle->getMvScope();//���ݵ�����з�Χ������λ��
			float wr = (dis[mtc.m_ix].y - dis[mtc.m_ix].x) / msg.vm[0].cols;//��������ͼ��ı���
			float hr = (dis[mtc.m_iy].y - dis[mtc.m_iy].x) / msg.vm[0].rows;
			for (int i = 0; i < mtRet.objNum; i++)
			{
				if (mtRet.label[i] == 1)//ok
				{
					//��������λ�ã����Ǹ���λ�ľ���
					mtc.m_table.push_back(cv::Point2f(mtRet.x[i] * wr + dis[mtc.m_ix].x, mtRet.y[i] * hr + dis[mtc.m_iy].x));
				}
			}
			S_MultiObjRet& mtRet2 = ((CDevice*)p2)->m_mtRet;
			mtRet2.rememini(mtc.m_table.size());
			int j = 0;
			for (int i = 0; i < mtRet.objNum; i++)
			{
				if (mtRet.label[i] == 1)//ok
				{
					mtRet2.x[j] = mtc.m_table[j].x;
					mtRet2.y[j] = mtc.m_table[j].y;
					mtRet2.z[j] = mtRet.z[i];
					j++;
				}
			}
			mtc.pointView();
		}
		else if ( "move" == vct[0])
		{
			int mtid = atoi(vct[1].c_str());
			S_MultiObjRet& mtRet = ((CDevice*)p2)->m_mtRet;
			CArrayMove& mtc = mb->m_mtc;//����������
			CMoveDllType* handle = mb->m_mtc.m_handle;
			int tbid = mtc.getCurPosId();
			if (-1 == tbid)
			{
				data = "û���˶�λ�� " + data;
				return false;
			}
			int step = mtRet.z[tbid];
			handle->moveAbs(mtid, step);
			CoordinatePosXYZ pd;
			pd.x = mtRet.x[tbid];
			pd.y = mtRet.y[tbid];
			mtc.viewPt(true,pd);
		}
		else if ("maskObjArr"== vct[0])//����ok���������µ������˶�
		{
			CoordinatePosXYZ msg;
			int camid = atoi(vct[1].c_str());
			if (-1 == camid)
			{
				camid = mb->m_pCheck->m_curCamId;
			}
			if (-1 == mb->m_pCheck->camPic(camid, msg.vm))
			{
				data = "cam  fail��" + data;
				return false;
			}
			msg.m_procid = mb->m_pCheck->m_picProcId;
			if (-1 == mb->m_pCheck->procCheck(&msg))
			{
				data = "cam check fail��" + data;
				return false;
			}
			CArrayMove& mtc = mb->m_mtc;//����������
			if (vct[0] == TRIGARRAYMOVE)//���������˶�
				mb->m_prob = 0;//��������

			mtc.m_table.clear();
			mtc.m_tbid = 0;//��ǰλ�ô��¿�ʼ
			mtc.m_prob = 1;//��ϵ��
			//��ȡokλ��   
			cv::Point2f* dis = mtc.m_handle->getMvScope();//���ݵ�����з�Χ������λ��
			float wr = (dis[mtc.m_ix].y - dis[mtc.m_ix].x) / msg.vm[0].cols;//��������ͼ��ı���
			float hr = (dis[mtc.m_iy].y - dis[mtc.m_iy].x) / msg.vm[0].rows;
			std::vector<std::vector<cv::RotatedRect>> vvRRect;
			mb->m_pCheck->m_vctMkc[msg.m_procid]->m_curProc.getMaskPos(vvRRect, true);
			for (int i = 0; i < vvRRect[0].size(); i++)//ֻ�õ�һ��λ���Ŀ��
			{
				//��������λ�ã����Ǹ���λ�ľ���
				mtc.m_table.push_back(cv::Point2f(vvRRect[0][i].center.x * wr + dis[mtc.m_ix].x, vvRRect[0][i].center.y * hr + dis[mtc.m_iy].x));
			}
			S_MultiObjRet& mtRet2 = ((CDevice*)p2)->m_mtRet;
			mtRet2.rememini(mtc.m_table.size());
			int j = 0;
			for (int i = 0; i < vvRRect[0].size(); i++)
			{
		//		if (mtRet.label[i] == 1)//ok
				{
					mtRet2.x[j] = mtc.m_table[j].x;
					mtRet2.y[j] = mtc.m_table[j].y;
		//			mtRet2.z[j] = mtRet.z[i];
					j++;
				}
			}
			mtc.pointView();
		}

		else
		{
			data = "error format:" + data;
			return false;
		}
	}

	data = "";
	return true;
}

//������ʾ�ĵ����ͷ��ڴ�
void MoveDllRelease(void*& p2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (NULL != p2)
	{
		delete (CDevice*)p2;
		p2 = NULL;
	}
}

void MoveDllWriteFile()
{

}


