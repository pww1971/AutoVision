// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

//#define _AFX_ALL_WARNINGS
#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE ��
#include <afxodlgs.h>       // MFC OLE �Ի�����
#include <afxdisp.h>        // MFC �Զ�����
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC ���ݿ���
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO ���ݿ���
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��


#include <afxsock.h>            // MFC �׽�����չ
#include <afxcview.h>


#define ON_NOTIFY_REFLECT_EX1(wNotifyCode, memberFxn) \
{ WM_NOTIFY + WM_REFLECT_BASE, (WORD)(int)wNotifyCode, 0, 0, AfxSigNotify_b, (AFX_PMSG) \
(static_cast<void (AFX_MSG_CALL CCmdTarget::*)(NMHDR*, LRESULT*) > (memberFxn)) },

//#define ON_NOTIFY_REFLECT(wNotifyCode, memberFxn) \
//	{ WM_NOTIFY+WM_REFLECT_BASE, (WORD)(int)wNotifyCode, 0, 0, AfxSigNotify_v, \
//		(AFX_PMSG) \
//		(static_cast<void (AFX_MSG_CALL CCmdTarget::*)(NMHDR*, LRESULT*) > \
//		(memberFxn)) },


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


#ifdef nouseEN
#define CST_PosModeType "ƥ��ģ��"
#define CST_PosImageWidth "ͼ����"
#define CST_PosImageHeigh "ͼ��߶�"
#define CST_PosImageWHR "ͼ�����"
#define CST_PosAngleExtend "��ת��Χ"
#define CST_PosMinScroe "���ƶ�"
#define CST_PosNumMatches "Ŀ����"
#define CST_PosNumMatchesLimit "Ŀ��������"
#define CST_PosNumLevels "����������"
#define CST_PosCheckScore "������ƶ�"
#define CST_PosStep "����"
#define CST_PosContrast "��С�߶�����"
#define CST_PosProcID "��Ŀ���"
#define CST_PosProcName "��Ŀ����"
#define CST_PosEleName "Ԫ������"
#define CST_PosEleType "Ԫ������"

#define CST_PosWord1  "Ŀ����%d,���%d,���ƶ�=%f,��ת��=%.4f "
#define CST_PosWord2  "���%d,���ƶ�=%f,��ת��=%.4f "
#define CST_PosWord3 "û�з��ֶ���, �����²ɼ�����"
#define CST_PosWord4 "û��ƥ�����,��ѡ��ƥ�䷽��"
#define CST_PosWord5 "û���ҵ���Ӧ��ģ��"
#define CST_PosWord6 "�ҵ��˶��󣬵÷֣�"
#define CST_PosWord7 "��ǰ���ڻ�ROI�С�����"
#define CST_PosWord8 "��ģ������,�������Ҽ�ȷ��"
#define CST_PosWord9 "Ԫ��������������������Ԫ��"
#define CST_PosWord10 "DisOnRoiInfo() ROI�������ļ������ڣ� "
#define CST_PosWord11 "DisOnRoiInfo()ROI��ģ���ļ������ڣ� "
#define CST_PosWord12 "����ƥ����Ϣ�ɹ��ˣ�"
#define CST_PosWord13 "����ƥ����Ϣʧ�ܣ� ������鿴��־"
#define CST_PosWord14 "����"
#define CST_PosWord15 "δ�ܴ��������ԡ�����\n"
#define CST_PosWord16 "�޷�����λͼ: %x\n"
#define CST_PosWord17 "������Ŀ"
#define CST_PosWord18 "ƥ�� %d"
#define CST_PosWord19 "��ѡ��ƥ�䷽��"
#define CST_PosWord20 "ȫ�ֱ���"
#define CST_PosWord21 "Ԫ������"
#define CST_PosWord22 "ģ��ƥ��"
#define CST_PosWord23 "ƥ��"

#define CST_PosDelMsg ::MessageBox(NULL,"��� ȷ��  ɾ�����в���Ԫ�أ�\n ��� ȡ��  ���ء�", "���棡�ף������Ҫɾ�����в���Ԫ����",MB_OKCANCEL | MB_ICONSTOP | MB_ICONWARNING|MB_SYSTEMMODAL);

#define CST_MeasureWord41 "δ�ܴ���������� \n"
#define CST_MeasureWord42 "��������"
#define CST_MeasureWord43 "Ӧ�ó���"
#define CST_MeasureWord44 "δ�ܴ�����������\n"

#define CST_GLBPACommandLine "������"
#define CST_GLBPAEleNote "Ԫ�ر�ע"

#else
#define CST_PosModeType "Match Type"
#define CST_PosImageWidth "Image width"
#define CST_PosImageHeigh "Image heigh"
#define CST_PosImageWHR "Image width and heigh ratio"
#define CST_PosAngleExtend "angle"
#define CST_PosMinScroe "Similarity"
#define CST_PosNumMatches "Targets num"
#define CST_PosNumMatchesLimit "Targets num limit"
#define CST_PosNumLevels "Pyramid levels"
#define CST_PosCheckScore "Detect Similarity"
#define CST_PosStep "Step"
#define CST_PosContrast "Minimum scale setting"
#define CST_PosProcID "project id"
#define CST_PosProcName "project name"
#define CST_PosEleName "Element name"
#define CST_PosEleType "Element type"

#define CST_PosWord1  "targets Num%d,ID%d,Similarity=%f,angle=%.4f "
#define CST_PosWord2  "ID%d,Similarity=%f,angle=%.4f "
#define CST_PosWord3 "No object found, please recollect the object"
#define CST_PosWord4 "No matching objects, Please select a matching scheme"
#define CST_PosWord5 "No corresponding model found"
#define CST_PosWord6 "Found the object, score��"
#define CST_PosWord7 "Currently in the drawing ROI..."
#define CST_PosWord8 "Draw the template area and right-click to confirm"
#define CST_PosWord9 "Elements have duplicate names, please name the elements separately"
#define CST_PosWord10 "DisOnRoiInfo() ROI��The shapes file does not exist! "
#define CST_PosWord11 "DisOnRoiInfo()ROI��The model file does not exist! "
#define CST_PosWord12 "Saving the match information is successful!"
#define CST_PosWord13 "Failed to save match information! For details, please check the logs"
#define CST_PosWord14 "property"
#define CST_PosWord15 "Failed to create 'Properties' window\n"
#define CST_PosWord16 "Unable to load bitmap: %x\n"
#define CST_PosWord17 "Measurement project"
#define CST_PosWord18 "match %d"
#define CST_PosWord19 "Please select a matching scheme"
#define CST_PosWord20 "Global variables"
#define CST_PosWord21 "element attributes"
#define CST_PosWord22 "Template matching"
#define CST_PosWord23 "match"

#define CST_PosDelMsg ::MessageBox(NULL,"Click OK to remove all measurement elements!\n Click CANCEL to return", "Warn! Dear, do you really want to remove all the measurement elements?",MB_OKCANCEL | MB_ICONSTOP | MB_ICONWARNING|MB_SYSTEMMODAL);

#define CST_MeasureWord41 "Failed to create attribute combination \n"
#define CST_MeasureWord42 "property settings"
#define CST_MeasureWord43 "application"
#define CST_MeasureWord44 "Failed to create attribute grid\n"

#define CST_GLBPACommandLine "Command_Line"
#define CST_GLBPAEleNote "Element_Notes"

#endif
