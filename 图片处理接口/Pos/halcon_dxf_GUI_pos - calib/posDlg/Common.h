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
	APP_READY,                // ԭʼ״̬
	App_CamInner,             // ��ǰ�Ǳ궨��� Inner Parameters ״̬
	App_CamPose,              // ��ǰ�Ǳ궨��� POSE ״̬
	App_MoldMatching,         // ��ǰ����ƥ��
	App_ElementAdding,        // ��ǰ�������Ԫ��
	APP_MANUAL_MEASURING,      // �ֶ�����һ��

	APP_ATTACH_LINE,          // ��ǰ������ֱ����
	APP_ATTACH_CIRCLE,          // ��ǰ������ֱ����
	APP_ATTACH_PEAK,          // ��ǰ������ֱ����
	APP_ATTACH_MEASURE_RECTANGLE2,
	APP_ATTACH_BALLOON_FLAW,
	APP_ATTACH_ROI,

	App_Stat_End
};
static CString STATE_NAME[] =
{
	"ԭʼ״̬",
	"�ڲα궨",
	"��̬�궨",
	"ģ��ƥ��",
	"���Ԫ��",
	"�ֶ�����",
	"����ֱ��",
	"����Բ",
	"�������",
	"���Ҳ���",
	"roi����"
};


enum ePROPERY_TYPE 
{
	PROPRETY_GLOBAL,             // ȫ������ 
	PROPRETY_ELEMENT             // Ԫ������
};



enum eELEMENT_TYPE
{
	ELEMENT_LINE,                     // ��ֱ��      Ԫ��
	ELEMENT_CIRCLE,                   // ��Բ        Ԫ��
	ELEMENT_RECT,                     // �Ҿ���      Ԫ��
	ELEMENT_ELLIPSE,                  // ����Բ      Ԫ��
	ELEMENT_ANGLE,                    // �Ƕ�        Ԫ��
	ELEMENT_DISTANCE,                 // ����
	ELEMENT_RESULT,                   // �������    Ԫ��
	ELEMENT_CAMERA,                   // �������    Ԫ��
	ELEMENT_MM,                       // ����
	ELEMENT_PEAK,                     // �Ҷ���Ԫ��
	ELEMENT_BALLOONLENGTH,            // �Ķ��������ҳ��Ⱥ���
	ELEMENT_FUNCTION,                 // ���Ӵ���
	ELEMENT_MATCH,                    // ƥ��Ԫ��
	ELEMENT_MEASURE_RECTANGLE2,       // һά����������
	ELEMENT_BALLOON_FLAW,             // ���ұ���ȱ��
	ELEMENT_ROI,                      // ȫ��ROI
	ELEMNET_END
};

static CString EleName[] = 
{
	"ֱ��",
	"Բ",
	"����",
	"��Բ",
	"�Ƕ�",
	"����",
	"���",
	"���",
	"У��",
	"����",
	"���ҳ���",
	"����",
	"ƥ��",
	"һά����",
	"���ұ���ȱ��",
	"ROI"
	"����"
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


