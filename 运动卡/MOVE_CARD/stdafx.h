// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

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


#ifdef nouseEN
#define CST_INIError "��ʼ������"
#define CST_MoveRun "�˶�"
#define CST_MoveStop "ֹͣ"
#define CST_Word401 "�����˶��� 4�������� 16����io,16-128����io"

#define CST_AxisID "���"
#define CST_NegLimit "������λ"
#define CST_PosLimit "������λ"
#define CST_AxisPulseMode "������ģʽ"
#define CST_SCurve "s����"
#define CST_PulseEquivalent "���嵱��"
#define CST_Acc "���ٶ�"
#define CST_Dec "���ٶ�"
#define CST_Speed "�ٶ�"
#define CST_ZeroReverSpeed "���㷴���ٶ�"
#define CST_StartSpeed "��ʼ�ٶ�"
#define CST_ZeroMode "����ģʽ"
#define CST_BearGuide "����"
#define CST_ZeroSpeed "�����ٶ�"
#define CST_ManualSpeed "�ֶ��ٶ�"
#define CST_AxisNegLimit "�Ḻ����λ"
#define CST_AxisPosLimit "��������λ"
#define CST_ZeroTriggerMode "���㴥��ģʽ"

#else
#define CST_INIError "ini error"
#define CST_MoveRun "Run"
#define CST_MoveStop "Stop"
#define CST_Word401 "Simulated motion card 4 virtual motors 16 virtual IO, 16-128 soft trigger IO"

#define CST_AxisID "axis id"
#define CST_NegLimit "Negative limit"
#define CST_PosLimit "Positive limit"
#define CST_AxisPulseMode "Axis pulse mode"
#define CST_SCurve "s curve"
#define CST_PulseEquivalent "Pulse equivalent"
#define CST_Acc "acceleration"
#define CST_Dec "Deceleration"
#define CST_Speed "Speed"
#define CST_ZeroReverSpeed "Zero reversing speed"
#define CST_StartSpeed "Start speed"
#define CST_ZeroMode "Zero mode"
#define CST_BearGuide "Bearing guide"
#define CST_ZeroSpeed "Zero speed"
#define CST_ManualSpeed "Manual speed"
#define CST_AxisNegLimit "Axis negative limit"
#define CST_AxisPosLimit "Axis positive limit"
#define CST_ZeroTriggerMode "Zero trigger mode"

#endif