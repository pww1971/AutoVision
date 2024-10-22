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
	string szHelp = "check,id,id = ���ռ��,���id,�Ƴ�id getdata = �ȴ�������ȡ���� �����˶��򴥷������˶�=ngλ���γ�����\r\n";
	return szHelp;
}

bool MoveDLLIni(void* pmv,void*& p2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (pmv == NULL)
	{
		return false;
	}
	CVisionSystem* mb = (CVisionSystem*)pmv;
	if (NULL==p2)
	{
		p2=new CDllPicTreat();
	}
	
	return true;
}

void MoveDLLStop()
{

}
//��̬������ȡ�ı꺯��
bool MoveDLLMove(void* pmv,void*& p2,string& data)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CMoveBase* mb = (CMoveBase*)pmv;
	if (NULL != mb)
	{
		CServerConnect* myserver = mb->m_pCheck->m_server;
		if (NULL==p2)
		{
			p2=new CDllPicTreat();
		}
		CDllPicTreat* s_picTreat = (CDllPicTreat*)p2;
		if (NULL == s_picTreat->m_mvb)
			s_picTreat->m_mvb = mb;
		vector<string> vct;
		CStrProcess::Split(data,",",vct);
		if (vct[0] == "check" && vct.size()==3)//���ռ�� 
		{
			S_Msg* pmsg = new S_Msg;
			cv::Point2f pt = mb->m_mtc.getCurPos();
			pmsg->pan_id = mb->m_prob+1;//�������ĸ����̵���
			pmsg->x = pt.x;
			pmsg->y = pt.y;
			if (mb->m_mtc.m_prob==0)
			{
				pmsg->pan_x = pmsg->x/mb->m_mtc.m_mtxl;
				pmsg->pan_y = pmsg->y/mb->m_mtc.m_mtyl;
			}
			pmsg->arrMoveId = mb->m_arrMoveId;//����id 
			pmsg->m_procid = atoi(vct[2].c_str());
			int camid = atoi(vct[1].c_str());
			if (-1==camid)
			{
				camid = mb->m_pCheck->m_curCamId;
			}
			if(-1 == mb->m_pCheck->camPic(camid,pmsg->m) )
			{
				data="";
				return false;
			}
			
			//����ֻ�ȡͼƬ���
			static int  s_count2=1;
			if (myserver!=NULL && myserver->m_tcpNum > 0  && s_count2++%(myserver->m_tcpNum+1) > 0)
			{
				myserver->sendMsg(e_CAM_CHECK,pmsg);
				delete pmsg;
			}
			else
			{
				//���˺�̨�ͷ���
				pmsg->pic = pmsg->m.clone();
				s_picTreat->pushMsg(pmsg);
				
				//mb->m_pTotCheck->procCheck(mb->m_pTotCheck->m_picProcId,m,&cor);
			}
		}
		else if (data == "getdata")//�ȴ�ȫ��������ȡ���
		{
			//while ( !mb->m_mtc.mb->m_mtc.ArrIsEnd())//���н���ɼ����
			//{
			//	Sleep(10);
			//}
			//deque<CoordinatePosXYZ*>::iterator itor = mb->m_mtc.m_deqRet.begin();
			//while (itor != mb->m_mtc.m_deqRet.end())
			//{
			//	if(3 != (*itor)->m_nc_ok)
			//		s_picTreat->m_vctNGPoint.push_back(cv::Point2f((*itor)->x, (*itor)->y));
			//	itor++;
			//}
			
			//Ҳ���Ե�����ȡ���
			CoordinatePosXYZ* msg = mb->m_mtc.popArrMsg();
			if (NULL != msg)
			{
				if (3 != msg->m_nc_ok)
					s_picTreat->m_vctNGPoint.push_back(cv::Point2f(msg->x, msg->y));
				else
					delete msg;
			}
			else if (mb->m_mtc.ArrIsEnd())//���н���ɼ����� �����Ѿ���ȡ����
			{
			}
		}
		else if (std::string::npos != data.find(ARRAYMOVE))//����ng���������µ������˶�
		{
			if (s_picTreat->m_vctNGPoint.size()>0)
			{
				if (data == TRIGARRAYMOVE)//���������˶�
					mb->m_prob = 0;//��������
				CArrayMove& mtc = mb->m_mtc;
				mtc.m_table.clear();
				mtc.m_prob = 1;//��ϵ��
				mtc.m_table.assign(s_picTreat->m_vctNGPoint.begin(),s_picTreat->m_vctNGPoint.end());
				s_picTreat->m_vctNGPoint.clear();
				mtc.pointView();
			}
			else
			{
				data="";
				return false;
			}
		}
		else
		{
			
		}
	}
	data="";
	return true;
}

//������ʾ�ĵ����ͷ��ڴ�
void MoveDllRelease(void*& p2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (NULL!=p2)
	{
		delete (CDllPicTreat*)p2;
		p2=NULL;
	}
}

void MoveDllWriteFile()
{
	
}


