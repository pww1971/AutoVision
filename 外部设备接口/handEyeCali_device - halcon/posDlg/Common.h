#pragma once
#include <HalconCpp.h>
//#define MAX_CAM_PARA_GROUP 10

enum eCOMBO_MATCH_ROI_TYPE 
{
	ROI_UNITE,
	ROI_AND,
	ROI_SUB,
	ROI_XOR
};

enum eLogType
{
	LOG_LOG,
	LOG_HALCON_ERR,
	LOG_DATABASE_ERR,
	LOG_OTHER
};

enum eAppState
{
	APP_READY,                // 原始状态
	App_CamInner,             // 当前是标定相机 Inner Parameters 状态
	App_CamPose,              // 当前是标定相机 POSE 状态
	App_MoldMatching,         // 当前正在匹配
	App_ElementAdding,        // 当前正在添加元素
	APP_MANUAL_MEASURING,      // 手动测量一次

	APP_ATTACH_LINE,          // 当前附加了直线了
	APP_ATTACH_CIRCLE,          // 当前附加了直线了
	APP_ATTACH_PEAK,          // 当前附加了直线了
	APP_ATTACH_MEASURE_RECTANGLE2,
	APP_ATTACH_BALLOON_FLAW,
	APP_ATTACH_ROI,

	App_Stat_End
};
static CString STATE_NAME[] =
{
	"原始状态",
	"内参标定",
	"姿态标定",
	"模型匹配",
	"添加元素",
	"手动测量",
	"附加直线",
	"附加圆",
	"测量宽度",
	"气囊测量",
	"roi测量"
};


enum ePROPERY_TYPE 
{
	PROPRETY_GLOBAL,             // 全局属性 
	PROPRETY_ELEMENT             // 元素属性
};



enum eELEMENT_TYPE
{
	ELEMENT_LINE,                     // 找直线      元素
	ELEMENT_CIRCLE,                   // 找圆        元素
	ELEMENT_RECT,                     // 找矩形      元素
	ELEMENT_ELLIPSE,                  // 找椭圆      元素
	ELEMENT_ANGLE,                    // 角度        元素
	ELEMENT_DISTANCE,                 // 距离
	ELEMENT_RESULT,                   // 输出报表    元素
	ELEMENT_CAMERA,                   // 相机参数    元素
	ELEMENT_MM,                       // 毫米
	ELEMENT_PEAK,                     // 找顶点元素
	ELEMENT_BALLOONLENGTH,            // 四顶点找球囊长度函数
	ELEMENT_FUNCTION,                 // 算子处理
	ELEMENT_MATCH,                    // 匹配元素
	ELEMENT_MEASURE_RECTANGLE2,       // 一维测量，矩形
	ELEMENT_BALLOON_FLAW,             // 球囊表面缺陷
	ELEMENT_ROI,                      // 全局ROI
	ELEMNET_END
};

static CString EleName[] = 
{
	"直线",
	"圆",
	"矩形",
	"椭圆",
	"角度",
	"距离",
	"结果",
	"相机",
	"校正",
	"顶点",
	"球囊长度",
	"算子",
	"匹配",
	"一维矩形",
	"球囊表面缺陷",
	"ROI"
	"其它"
};


static int ELEMTEN_PIC_INDEX_NOSEL[] = 
{
	2,
	3,
	4,
	4,
	6,
	5,
	7,
	8,
	9,
	10,
	11,
	12,
	13,
	14,
	15,
	16,
	3
};

static int ELEMTEN_PIC_INDEX_SEL[] = 
{
	2,
	3,
	4,
	4,
	6,
	5,
	7,
	8,
	9,
	10,
	11,
	12,
	13,
	14,
	15,
	16,
	3
};

typedef struct CheckError
{
	int iErrorType;
	CString strErrorInfo;
}s_CheckError;


enum ePROP_ID
{
	OUT_Element_Name_ID = 1000,
	OUT_Element_Note_ID,
	OUT_Element_Name_Identify,
	USE_LINE1_Name_Identify,
	USE_LINE2_Name_Identify,
	USE_Distance1_Name_Identify,
	USE_Distance2_Name_Identify,
	USE_Distance1_Parament_Name_ID,
	USE_Distance2_Parament_Name_ID,
	OUT_Element_Name_IdentifyX,
	OUT_Element_Name_IdentifyY,
	OUT_Element_Name_IDX,
	OUT_Element_Name_IDY,

	USE_Peek1_Element_Name_ID,
	USE_Peek2_Element_Name_ID,
	USE_Peek3_Element_Name_ID,
	USE_Peek4_Element_Name_ID,

	ePROP_END
};


enum ePROPLIST {
	PROPLIST_OTHER,
	//PROPLIST_CAMERA,	
	PROPLIST_GLOBAL,

	ePROPLIST_END
};


