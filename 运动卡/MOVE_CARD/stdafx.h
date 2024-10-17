// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE 类
#include <afxodlgs.h>       // MFC OLE 对话框类
#include <afxdisp.h>        // MFC 自动化类
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC 数据库类
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO 数据库类
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT


#ifdef nouseEN
#define CST_INIError "初始化错误"
#define CST_MoveRun "运动"
#define CST_MoveStop "停止"
#define CST_Word401 "仿真运动卡 4个虚拟电机 16虚拟io,16-128软触发io"

#define CST_AxisID "轴号"
#define CST_NegLimit "负向限位"
#define CST_PosLimit "正向限位"
#define CST_AxisPulseMode "轴脉冲模式"
#define CST_SCurve "s曲线"
#define CST_PulseEquivalent "脉冲当量"
#define CST_Acc "加速度"
#define CST_Dec "减速度"
#define CST_Speed "速度"
#define CST_ZeroReverSpeed "回零反找速度"
#define CST_StartSpeed "起始速度"
#define CST_ZeroMode "回零模式"
#define CST_BearGuide "导承"
#define CST_ZeroSpeed "回零速度"
#define CST_ManualSpeed "手动速度"
#define CST_AxisNegLimit "轴负向限位"
#define CST_AxisPosLimit "轴正向限位"
#define CST_ZeroTriggerMode "回零触发模式"

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