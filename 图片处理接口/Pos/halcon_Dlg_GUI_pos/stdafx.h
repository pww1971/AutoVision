// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

//#define _AFX_ALL_WARNINGS
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

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持


#include <afxsock.h>            // MFC 套接字扩展
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
#define CST_PosModeType "匹配模型"
#define CST_PosImageWidth "图像宽度"
#define CST_PosImageHeigh "图像高度"
#define CST_PosImageWHR "图像宽长比"
#define CST_PosAngleExtend "旋转范围"
#define CST_PosMinScroe "相似度"
#define CST_PosNumMatches "目标数"
#define CST_PosNumMatchesLimit "目标数限制"
#define CST_PosNumLevels "金字塔级数"
#define CST_PosCheckScore "检测相似度"
#define CST_PosStep "步数"
#define CST_PosContrast "最小尺度设置"
#define CST_PosProcID "项目编号"
#define CST_PosProcName "项目名称"
#define CST_PosEleName "元素名称"
#define CST_PosEleType "元素类型"

#define CST_PosWord1  "目标数%d,编号%d,相似度=%f,旋转角=%.4f "
#define CST_PosWord2  "编号%d,相似度=%f,旋转角=%.4f "
#define CST_PosWord3 "没有发现对象, 请重新采集对象"
#define CST_PosWord4 "没有匹配对象,请选择匹配方案"
#define CST_PosWord5 "没有找到相应的模型"
#define CST_PosWord6 "找到了对象，得分："
#define CST_PosWord7 "当前已在绘ROI中。。。"
#define CST_PosWord8 "画模板区域,点击鼠标右键确认"
#define CST_PosWord9 "元素有重名，请另外命名元素"
#define CST_PosWord10 "DisOnRoiInfo() ROI　区域文件不存在！ "
#define CST_PosWord11 "DisOnRoiInfo()ROI　模型文件不存在！ "
#define CST_PosWord12 "保存匹配信息成功了！"
#define CST_PosWord13 "保存匹配信息失败！ 详情请查看日志"
#define CST_PosWord14 "属性"
#define CST_PosWord15 "未能创建“属性”窗口\n"
#define CST_PosWord16 "无法加载位图: %x\n"
#define CST_PosWord17 "测量项目"
#define CST_PosWord18 "匹配 %d"
#define CST_PosWord19 "请选择匹配方案"
#define CST_PosWord20 "全局变量"
#define CST_PosWord21 "元素属性"
#define CST_PosWord22 "模板匹配"
#define CST_PosWord23 "匹配"

#define CST_PosDelMsg ::MessageBox(NULL,"点击 确定  删除所有测量元素！\n 点击 取消  返回。", "警告！亲，您真的要删除所有测量元素吗？",MB_OKCANCEL | MB_ICONSTOP | MB_ICONWARNING|MB_SYSTEMMODAL);

#define CST_MeasureWord41 "未能创建属性组合 \n"
#define CST_MeasureWord42 "属性设置"
#define CST_MeasureWord43 "应用程序"
#define CST_MeasureWord44 "未能创建属性网格\n"

#define CST_GLBPACommandLine "命令行"
#define CST_GLBPAEleNote "元素备注"

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
#define CST_PosWord6 "Found the object, score："
#define CST_PosWord7 "Currently in the drawing ROI..."
#define CST_PosWord8 "Draw the template area and right-click to confirm"
#define CST_PosWord9 "Elements have duplicate names, please name the elements separately"
#define CST_PosWord10 "DisOnRoiInfo() ROI　The shapes file does not exist! "
#define CST_PosWord11 "DisOnRoiInfo()ROI　The model file does not exist! "
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
