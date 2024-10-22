// new_move_device.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "new_move_device.h"
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
	string szHelp = "\r\ncheck, id, id = ���ռ��, ���id, �Ƴ�id\r\ngetdata, ok, ng ������ȡ, ok, ng�Ķ�λλ���ƶ����� - 1������\r\n\
checkdata, ioid, up, �ж����������Ƿ������, io�߳�id�����߳�ioid = -1��up = 1������ = 0�½��ر�io\r\n\
�����˶�, ioid, up = ��ȡ������ngλ���γ��µ�����";
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
		if (vct.size() != 3)
		{
			data = "error format:" + data;
			return false;
		}
		else if (std::string::npos != vct[0].find(ARRAYMOVE))//����ng���������µ������˶�
		{
			int ioid = atoi(vct[1].c_str());
			int up = atoi(vct[2].c_str());
			CMoveBase* mbtemp = NULL;
			if (ioid == -1)
			{
				mbtemp = mb->m_mainMB;
			}
			else
			{
				if (up > 0)
					mbtemp = ((CVisionSystem*)mb->m_mainMB)->m_io.m_ioTrigger[ioid];
				else
					mbtemp = ((CVisionSystem*)mb->m_mainMB)->m_io.m_ioTrigger[g_ioNum + ioid];
			}
			if (NULL == mbtemp)
			{
				return false;
			}
			CArrayMove& mtcraw = mbtemp->m_mtc;//Դ��������
			CArrayMove& mtc = mb->m_mtc;//����������
			if (vct[0] == TRIGARRAYMOVE)//���������˶�
				mb->m_prob = 0;//��������

			mtc.m_table.clear();
			mtc.m_prob = 1;//��ϵ��
			//��ȡngλ��   
			map<myPoint2f, int>::iterator itor = mtcraw.m_mpArrRet.begin();
			while (itor != mtcraw.m_mpArrRet.end())
			{
				if (itor->second == 0)//ng
					mtc.m_table.push_back(itor->first);
				itor++;
			}
			mtc.pointView();
		}
		else if (vct[0] == "check" )//���ռ�� 
		{
			S_Msg msg;
			cv::Point2f pt = mb->m_mtc.getCurPos();
			msg.pan_id = mb->m_prob + 1;//�������ĸ����̵���
			msg.x = pt.x;
			msg.y = pt.y;
			if (mb->m_mtc.m_prob == 0)
			{
				msg.pan_x = msg.x / mb->m_mtc.m_mtxl;
				msg.pan_y = msg.y / mb->m_mtc.m_mtyl;
			}
			msg.arrMoveId = mb->m_arrMoveId;//����id 
			msg.m_procid = atoi(vct[2].c_str());
			if (msg.m_procid == -1)
			{
				msg.m_procid = mb->m_pCheck->m_picProcId;
			}
			int camid = atoi(vct[1].c_str());
			if (-1 == camid)
			{
				camid = mb->m_pCheck->m_curCamId;
			}
			mb->m_pCheck->camPic(camid, msg.vm);
			if (-1 == mb->m_pCheck->procCheckwait(&msg))
			{
				data = "cam check fail��" + data;
				return false;
			}
		}
		else if (vct[0] == "checkdata" )//������������Ƿ������
		{
			data = "";
			int ioid = atoi(vct[1].c_str());
			int up = atoi(vct[2].c_str());
			CMoveBase* mbtemp = NULL;
			if (ioid == -1)
			{
				mbtemp = mb->m_mainMB;
			}
			else
			{
				if (up > 0)
					mbtemp = ((CVisionSystem*)mb->m_mainMB)->m_io.m_ioTrigger[ioid];
				else
					mbtemp = ((CVisionSystem*)mb->m_mainMB)->m_io.m_ioTrigger[g_ioNum + ioid];
			}
			if (NULL == mbtemp)
			{
				return false;
			}
			return mbtemp->m_mtc.ArrIsEnd();
		}
		else if (vct[0] == "getdata" )//������ȡ
		{
			static CgetResultView getret(mb->m_mainMB);
			S_Msg* pmsg = getret.treatMsg();
			int limittime = 5000;//50�볬ʱ����
			while (NULL == pmsg)//����ʽ�ж�
			{
				if (((CVisionSystem*)mb->m_mainMB)->ArrIsEnd())
					return false;
				Sleep(10);
				if (limittime-- <= 0)
				{
					data = "check time out:" + data;
					return false;
				}
				pmsg = getret.treatMsg();
			}
			data = "";
			int iok = atoi(vct[1].c_str());
			int ing = atoi(vct[2].c_str());
			//���ݶ�λ�������
			if (pmsg->m_nc_ok == 3 && iok > 0)//ok����
			{
				mb->m_handle->moveAbs(mb->m_mtc.m_ix, pmsg->x);
				mb->m_handle->moveAbs(mb->m_mtc.m_iy, pmsg->y);
			}
			else if (pmsg->m_nc_ok == 2 && ing > 0)//ng
			{
				mb->m_handle->moveAbs(mb->m_mtc.m_ix, pmsg->x);
				mb->m_handle->moveAbs(mb->m_mtc.m_iy, pmsg->y);
			}
			delete pmsg;
			pmsg = NULL;
			return true;

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


