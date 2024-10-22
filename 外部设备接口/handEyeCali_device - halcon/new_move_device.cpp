/* new_move_device.cpp : ���� DLL �ĳ�ʼ�����̡�
n��궨�����ݱ궨����ʵ�ֵ�ϵ�����С�
���԰ѱ궨���ݣ�������ͼ���Ƴ̵�unit�����������صĺ��ף���������dll���Զ�ʵ���˺��׵�ת��,����Ĭ�������ؾ���.
*/

#include "new_move_device.h"
#include "automove.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "checkPatch.h"
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
	SetSystem("parallelize_operators", "true");
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

	UCI::pDis = new ST_MYDISP(&checkPatch::S_dlg);
	UCI::initFirst();
	return TRUE;
}

string MoveDLLHelp()
{
	string szHelp = "\r\n�����˶�=��Ŀ�궨λ����\r\nanglemove,n = ��n��ת���˶�\r\nanglemm, f = ������ת����ǶȲ���";
	return szHelp;
}

bool MoveDLLIni(void* pmv, void*& p2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (pmv == NULL)
	{
		return false;
	}
	CMoveBase* mb = (CMoveBase*)pmv;
	checkPatch* devCheck;
	if (p2 == NULL)
	{
		devCheck = new checkPatch();
		devCheck->m_CaliParam.pCheck = mb->m_pCheck;
		string szDir = mb->m_szCfgAdd;
		int pos = szDir.size() - strlen("MoveCfg");
		szDir = szDir.substr(0, pos);
		devCheck->readMode(szDir + "arrmovedllcfg");
		p2 = devCheck;
	}
	else
		devCheck = (checkPatch*)p2;
	CNewMoveDlg mydlg;
	mydlg.m_pData = p2;
	std::vector<cv::Mat> vm;
	if (-1 == mb->m_pCheck->camPic(mb->m_pCheck->m_curCamId, vm))
	{
		return false;
	}
	devCheck->m_rawPic = vm[0];
	if (IDOK == mydlg.DoModal())
	{
		string szDir = mb->m_szCfgAdd;
		int pos = szDir.size() - strlen("MoveCfg");
		szDir = szDir.substr(0, pos);
		devCheck->writeMode(szDir + "arrmovedllcfg");
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
		checkPatch* devCheck;
		if (p2 == NULL)
		{
			devCheck = new checkPatch();
			devCheck->m_CaliParam.pCheck = mb->m_pCheck;
			string szDir = mb->m_szCfgAdd;
			int pos = szDir.size() - strlen("MoveCfg");
			szDir = szDir.substr(0, pos);
			devCheck->readMode(szDir + "arrmovedllcfg");
			p2 = devCheck;
		}
		else
			devCheck = (checkPatch*)p2;
		if (data == ARRAYMOVE)//���ݶ�Ŀ�궨λ���������˶�
		{
			CArrayMove& mtc = mb->m_mtc;
			std::vector<cv::Mat> vm;
			if (-1 == mb->m_pCheck->camPic(mb->m_pCheck->m_curCamId, vm))
			{
				data += "open cam error";
				return false;
			}
			//��Ŀ�궨λ
			if (devCheck->m_Proc.parallelMatchTplt(vm) > -1)//ok
			{
				mtc.m_table.clear();
				mtc.m_prob = 1;
				devCheck->m_DevParam.m_angleTab.clear();
				devCheck->m_DevParam.angleTabId = 0;
				double mm = devCheck->m_CaliParam.dSh;//ÿ���غ��׵�λ
				std::vector<S_TpltData*>& vctData = devCheck->m_Proc.m_vcTplt[0]->m_vctData;
				for (int ik = 0; ik < vctData.size(); ik++)
				{
					float posX = vctData[ik]->m_roiRect.center.x;
					float posY = vctData[ik]->m_roiRect.center.y;
					float posZ = vctData[ik]->m_roiRect.angle;//��ת��
					devCheck->m_DevParam.m_angleTab.push_back(posZ);
					mtc.m_table.push_back(cv::Point2f(posX * mm, posY * mm));
				}
				mtc.pointView();
				mtc.m_prob = 1;//��ϵ��
			}
			else
			{
				data = "";
				return false;
			}
		}
		else
		{
			CMoveDllType* handle = mb->m_mtc.m_handle;
			if (NULL == handle)
			{
				data += " no movecard";
				return false;
			}
			std::vector<string> vct;
			CStrProcess::Split(data, ",", vct);
			if (vct[0] == "anglemove" && vct.size()==2)//���ݶ�Ŀ�궨λ�������˶�����ת�ǵ��˶�
			{
				if (devCheck->m_DevParam.m_angleTab.size() == 0)
				{
					data += " angle table no data";
					return false;
				}
				int mvid = atoi(vct[1].c_str());
				handle->moveAbs(mvid, devCheck->m_DevParam.m_anglePa * \
					devCheck->m_DevParam.m_angleTab[devCheck->m_DevParam.angleTabId]);
				devCheck->m_DevParam.angleTabId++;
				if (devCheck->m_DevParam.angleTabId == devCheck->m_DevParam.m_angleTab.size())
				{
					devCheck->m_DevParam.angleTabId = 0;
				}
			}
			else if (vct[0] == "cali" && vct.size() == 3)//���ݱ궨���λ���˶�
			{
				float px = atof(vct[1].c_str());
				float py = atof(vct[2].c_str());
				cv::Point2f pt;
				if (devCheck->m_CaliParam.getDistance(&px, &py, &pt.x, &pt.y))
					;
			}
			else if (vct[0] == "anglemm" && vct.size() == 2)
			{
				devCheck->m_DevParam.m_anglePa = atof(vct[1].c_str());
			}
			else
			{
				data = "error format:" + data;
				return false;
			}
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
		delete (checkPatch*)p2;
		p2 = NULL;
	}
	delete UCI::pDis; UCI::pDis = NULL;

}

void MoveDllWriteFile()
{

}


