#pragma once

#define GT_API extern "C" short __stdcall

#define DLL_VERSION_0                   2
#define DLL_VERSION_1                   1
#define DLL_VERSION_2                   0

#define DLL_VERSION_3                   1
#define DLL_VERSION_4                   5
#define DLL_VERSION_5                   0
#define DLL_VERSION_6                   6
#define DLL_VERSION_7                   0
#define DLL_VERSION_8                   7

#define MC_NONE                         -1

#define MC_LIMIT_POSITIVE               0
#define MC_LIMIT_NEGATIVE               1
#define MC_ALARM                        2
#define MC_HOME                         3
#define MC_GPI                          4
#define MC_ARRIVE                       5
#define MC_MPG                          6

#define MC_ENABLE                       10
#define MC_CLEAR                        11
#define MC_GPO                          12

#define MC_DAC                          20
#define MC_STEP                         21
#define MC_PULSE                        22
#define MC_ENCODER                      23
#define MC_ADC                          24

#define MC_AXIS                         30
#define MC_PROFILE                      31
#define MC_CONTROL                      32

#define MC_SMART_CUTTER                 400

#define CAPTURE_SCALE_TRANSFORM_ON      0
#define CAPTURE_SCALE_TRANSFORM_OFF     1

#define CAPTURE_HOME                    1
#define CAPTURE_INDEX                   2
#define CAPTURE_PROBE                   3
#define CAPTURE_HSIO0                   6
#define CAPTURE_HSIO1                   7
#define CAPTURE_HOME_GPI                8

#define PT_MODE_STATIC                  0
#define PT_MODE_DYNAMIC                 1

#define PT_SEGMENT_NORMAL               0
#define PT_SEGMENT_EVEN                 1
#define PT_SEGMENT_STOP                 2

#define GEAR_MASTER_ENCODER             1
#define GEAR_MASTER_PROFILE             2
#define GEAR_MASTER_AXIS                3

#define FOLLOW_MASTER_ENCODER           1
#define FOLLOW_MASTER_PROFILE           2
#define FOLLOW_MASTER_AXIS              3

#define FOLLOW_EVENT_START              1
#define FOLLOW_EVENT_PASS               2

#define GEAR_EVENT_START                1
#define GEAR_EVENT_PASS                 2
#define GEAR_EVENT_AREA                 5

#define FOLLOW_SEGMENT_NORMAL           0
#define FOLLOW_SEGMENT_EVEN             1
#define FOLLOW_SEGMENT_STOP             2
#define FOLLOW_SEGMENT_CONTINUE         3

#define CRD_FIFO_MAX                    4096
#define FIFO_MAX						2
#define CRD_MAX                         2
#define CRD_OPERATION_DATA_EXT_MAX      2

#define CRD_OPERATION_TYPE_NONE                   0
#define CRD_OPERATION_TYPE_BUF_IO_DELAY           1
#define CRD_OPERATION_TYPE_LASER_ON               2
#define CRD_OPERATION_TYPE_LASER_OFF              3
#define CRD_OPERATION_TYPE_BUF_DA                 4
#define CRD_OPERATION_TYPE_LASER_CMD              5
#define CRD_OPERATION_TYPE_LASER_FOLLOW_RATIO     6
#define CRD_OPERATION_TYPE_LMTS_ON                7
#define CRD_OPERATION_TYPE_LMTS_OFF               8
#define CRD_OPERATION_TYPE_SET_STOP_IO            9
#define CRD_OPERATION_TYPE_BUF_MOVE               10
#define CRD_OPERATION_TYPE_BUF_GEAR               11
#define CRD_OPERATION_TYPE_SET_SEG_NUM            12
#define CRD_OPERATION_TYPE_STOP_MOTION            13
#define CRD_OPERATION_TYPE_SET_VAR_VALUE          14
#define CRD_OPERATION_TYPE_JUMP_NEXT_SEG          15
#define CRD_OPERATION_TYPE_SYNCH_PRF_POS          16
#define CRD_OPERATION_TYPE_VIRTUAL_TO_ACTUAL      17
#define CRD_OPERATION_TYPE_SET_USER_VAR           18
#define CRD_OPERATION_TYPE_SET_DO_BIT_PULSE       19
#define CRD_OPERATION_TYPE_BUF_COMPAREPULSE       20
#define CRD_OPERATION_TYPE_LASER_ON_EX            21
#define CRD_OPERATION_TYPE_LASER_OFF_EX           22
#define CRD_OPERATION_TYPE_LASER_CMD_EX           23
#define CRD_OPERATION_TYPE_LASER_FOLLOW_RATIO_EX  24
#define CRD_OPERATION_TYPE_LASER_FOLLOW_MODE      25
#define CRD_OPERATION_TYPE_LASER_FOLLOW_OFF		  26
#define CRD_OPERATION_TYPE_LASER_FOLLOW_OFF_EX	  27
#define CRD_OPERATION_TYPE_LASER_FOLLOW_SPLINE	  28
#define CRD_OPERATION_TYPE_MOTION_DATA			  29
#define CRD_OPERATION_TYPE_SET_LASER_DIS_ENABLE  (30)
#define CRD_OPERATION_TYPE_SELECT_LASER_DIS_TABLE (31)
#define CRD_OPERATION_TYPE_BUF_STOP				 (36)
#define CRD_OPERATION_TYPE_BUF_MOVE_JOG			 (37)
#define CRD_OPERATION_TYPE_BUF_TREND			  50

#define CRD_OPERATION_TYPE_BUF_FOLLOW_MASTER     (60)
#define CRD_OPERATION_TYPE_BUF_FOLLOW_EVENT      (61)
#define CRD_OPERATION_TYPE_BUF_FOLLOW_EVENT_TRIGGER	 (62)	//新前瞻指令需要通过这些宏进行区分，GT_BufFollowEventCross和GT_BufFollowEventTrigger共用CRD_OPERATION_TYPE_BUF_FOLLOW_EVENT，因此增加此宏进行区分
#define CRD_OPERATION_TYPE_BUF_FOLLOW_START      (63)
#define CRD_OPERATION_TYPE_BUF_FOLLOW_NEXT       (66)	
#define CRD_OPERATION_TYPE_BUF_FOLLOW_RETURN     (68)

#define CRD_OPERATION_TYPE_BUF_EVENT_ON			 (70)
#define CRD_OPERATION_TYPE_BUF_EVENT_OFF		 (71)

#define CRD_OPERATION_TYPE_WORK_LASER_FOLLOW_RATIO	(72)

#define CRD_OPERATION_TYPE_BUF_SMART_CUTTER_ENABLE (80)

#define CRD_OPERATION_TYPE_BUF_POS_COMPARE_START (90)
#define CRD_OPERATION_TYPE_BUF_POS_COMPARE_STOP (91)
#define CRD_OPERATION_TYPE_BUF_POS_COMPARE_PSO_PRM (97)
#define CRD_OPERATION_TYPE_BUF_SET_COMPARE_PORT (98)

#define CRD_OPERATION_TYPE_BUF_2D_COMPARE_DATA		(100)
#define CRD_OPERATION_TYPE_BUF_2D_COMPARE_PULSE		(101)

#define CRD_OPERATION_TYPE_LASER_FOLLOW_TABLE	 (200)

#define INTERPOLATION_MOTION_TYPE_LINE  0
#define INTERPOLATION_MOTION_TYPE_CIRCLE 1
#define INTERPOLATION_MOTION_TYPE_HELIX 2
#define INTERPOLATION_MOTION_TYPE_CIRCLE_3D 3
#define INTERPOLATION_MOTION_TYPE_ELLIPSE               (4)
#define INTERPOLATION_MOTION_TYPE_ELLIPSE_DATA          (5)
#define INTERPOLATION_MOTION_TYPE_HELIX_AU             (50)
#define INTERPOLATION_MOTION_TYPE_CIRCLE_VARIABLE      (60)

#define INTERPOLATION_CIRCLE_PLAT_XY    0
#define INTERPOLATION_CIRCLE_PLAT_YZ    1
#define INTERPOLATION_CIRCLE_PLAT_ZX    2

#define INTERPOLATION_HELIX_CIRCLE_XY_LINE_Z    0
#define INTERPOLATION_HELIX_CIRCLE_YZ_LINE_X    1
#define INTERPOLATION_HELIX_CIRCLE_ZX_LINE_Y    2

#define INTERPOLATION_CIRCLE_DIR_CW     0
#define INTERPOLATION_CIRCLE_DIR_CCW    1
#define CRD_OPERATION_TYPE_LASER_FOLLOW_RATIO     6

#define COMPARE_PORT_HSIO                 (0)             //??zW??HSIO?
#define COMPARE_PORT_GPO                  (1)             //??zW??GPO?

#define  COMPARE2D_MODE_2D            1
#define  COMPARE2D_MODE_1D            0

#define  INTERFACEBOARD20                2
#define  INTERFACEBOARD30                3


#define AXIS_LASER                       7
#define AXIS_LASER_EX                    8

#define LASER_CTRL_MODE_PWM1            (0)                 // ??:PWM1
#define LASER_CTRL_FREQUENCY            (1)                 // ??:?
#define LASER_CTRL_VOLTAGE              (2)                 // ??:?
#define LASER_CTRL_MODE_PWM2            (3)                 // ??:PWM2

#define CRD_BUFFER_MODE_DYNAMIC_DEFAULT  (0)
#define CRD_BUFFER_MODE_DYNAMIC_KEEP     (1)
#define CRD_BUFFER_MODE_STATIC_INPUT     (11)
#define CRD_BUFFER_MODE_STATIC_READY     (12)
#define CRD_BUFFER_MODE_STATIC_START     (13)

#define CRD_SMOOTH_MODE_NONE                   (0)
#define CRD_SMOOTH_MODE_PERCENT                (1)
#define CRD_SMOOTH_MODE_JERK                   (2)

typedef struct TrapPrm
{
    double acc;
    double dec;
    double velStart;
    short  smoothTime;
} TTrapPrm;

typedef struct TTrapTime
{
	double totalTime;
	double remainderTime;
	double pad[2];
} TTrapTime;

typedef struct JogPrm
{
    double acc;
    double dec;
    double smooth;
} TJogPrm;

typedef struct Pid
{
    double kp;
    double ki;
    double kd;
    double kvff;
    double kaff;
    long   integralLimit;
    long   derivativeLimit;
    short  limit;
} TPid;

typedef struct ThreadSts
{
    short  run;
    short  error;
    double result;
    short  line;
} TThreadSts;

typedef struct VarInfo
{
    short id;
    short dataType;
    char  name[32];
} TVarInfo;

typedef struct CompileInfo
{
    char  *pFileName;
    short *pLineNo;
    char  *pMessage;
} TCompileInfo;

//速度形态类型
const long TYPE_ACC = 1;
const long TYPE_EVEN = 2;
const long TYPE_DCC = 4;

//速度形态信息
typedef struct 
{
	long segVelType;             //速度形态类型
	long endVelType;             //最后一个速度形态
	double segTime;              //整段的运动时间
	double segAccTime;           //加速段时间
	double segEvenTime;          //匀速段时间
	double segDccTime;           //减速段时间
	double segAccLength;         //加速段距离
	double segEvenLength;        //匀速段距离
	double segDccLength;         //减速段距离
	double segMaxVel;            //最大速度
	double segStartVel;          //起点速度
	double segEndVel;            //终点速度
}TVelInfo;

typedef struct CrdPrm
{
    short dimension;                              // 坐标系维数
    short profile[8];                             // 关联profile和坐标轴
    double synVelMax;                             // 最大合成速度
    double synAccMax;                             // 最大合成加速度
    short evenTime;                               // 最小匀速时间
    short setOriginFlag;                          // 设置原点坐标值标志,0:默认当前规划位置为原点位置;1:用户指定原点位置
    long originPos[8];                            // 用户指定的原点位置
}TCrdPrm;

typedef struct CrdBufOperation
{
    short flag;                                   // 标志该插补段是否含有IO和延时
    unsigned short delay;                         // 延时时间
    short doType;                                 // 缓存区IO的类型,0:不输出IO
    unsigned short doMask;                        // 缓存区IO的输出控制掩码
    unsigned short doValue;                       // 缓存区IO的输出值
    unsigned short dataExt[CRD_OPERATION_DATA_EXT_MAX];     // 辅助操作扩展数据
}TCrdBufOperation;

typedef struct CrdData
{
    short motionType;                             // 运动类型,0:直线插补,1:圆弧插补
    short circlePlat;                             // 圆弧插补的平面
    long pos[8];							      // 当前段各轴终点位置
    double radius;                                // 圆弧插补的半径
    short circleDir;                              // 圆弧旋转方向,0:顺时针;1:逆时针
    double center[3];                             // 圆弧插补的圆心坐标
    double vel;                                   // 当前段合成目标速度
    double acc;                                   // 当前段合成加速度
    short velEndZero;                             // 标志当前段的终点速度是否强制为0,0:不强制为0;1:强制为0
    TCrdBufOperation operation;                   // 缓存区延时和IO结构体

    double cos[8];								  // 当前段各轴对应的余弦值
    double velEnd;                                // 当前段合成终点速度
    double velEndAdjust;                          // 调整终点速度时用到的变量(前瞻模块)
    double r;                                     // 当前段合成位移量
}TCrdData;

typedef struct Trigger
{
    short encoder;
    short probeType;
    short probeIndex;
	short sense;
    short offset;
	unsigned long loop;
    short windowOnly;
    long firstPosition;
    long lastPosition;
}TTrigger;

typedef struct TriggerStatus
{
    short execute;
    short done;
    long position;
}TTriggerStatus;

typedef struct TriggerStatusEx
{
	short execute;
	short done;
	long position;
	unsigned long clock;
	unsigned long loopCount;
}TTriggerStatusEx;

typedef struct
{
	short fifoFull;
	short pad1[3];
	double pad2[2];
}TLatchValueInfo;

typedef struct
{
    long px;
    long py;
} T2DCompareData;

typedef struct
{
    double px;
    double py;
} T2DCompareDataEx;

typedef struct
{
    short encx;
    short ency;
    short source;
    short outputType;
    short startLevel;
    short time;
    short maxerr;
    short threshold;
} T2DComparePrm;

typedef struct  
{
	short mode;
	short encoderX;
	short encoderY;
	short source;
	short outputType;
	short startLevel;
	long pulseTime;
	short maxerr;
	long pulseCount;
	long pulseCycle;
	long interval;
	unsigned short pad1[2];
	long pad2[2];
}T2DComparePrmEx;
typedef struct
{
	long px;
	long py;
	long intervalX;
	long intervalY;
	long count;
}T2DCompareDataGroup;

typedef struct
{
	long startPosX;
	long startPosY;
	long intervalX;
	long intervalY;
	unsigned long count;
}T2DCompareLinear;

typedef struct  
{
	double time;
	long segmentUsed;
	long segmentHead;
	long segmentTail;
} TCrdTime;

typedef struct  
{
	short enable;
	short smoothMode;

	short percent;
	short accStartPercent;
	short decEndPercent;

	double jerkMax;
} TCrdSmoothInfo;

typedef struct  
{
	short percent;
	short accStartPercent;
	short decEndPercent;
	double reserve;
} TCrdSmooth;

typedef struct
{
	short          mdlType;                       // 扩展模块的类型

	short          mdlAddressBegin;               // 该模块的起始地址偏移

	short          adChannels;                    // 如果为AD时，通道的数量
	double         adMaxVoltage;                  // 如果为AD时，最大转换位对应的电压值
	double         adMinVoltage;                  // 如果为AD时，最小转换位对应的电压值

	short          daChannels;                    // 如果为DA时，通道的数量
	double         daMaxVoltage;                  // 如果为DA时，最大转换位对应的电压值
	double         daMinVoltage;                  // 如果为DA时，最小转换位对应的电压值

	unsigned short outSave;                       // 保留当前IO输出的值
}TExtMdlCfgInfo;

GT_API GT_GetDllVersion(short cardNum,char **pDllVersion);

GT_API GT_SetCardNo(short index);
GT_API GT_GetCardNo(short *pIndex);

GT_API GT_GetVersion(short cardNum,char **pVersion);
GT_API GT_GetInterfaceBoardSts(short cardNum,short *pStatus);
GT_API GT_SetInterfaceBoardSts(short cardNum,short type);
GT_API GT_Open(short cardNum,short channel=0,short param=1);
GT_API GT_Close(short cardNum);

GT_API GT_LoadConfig(short cardNum,char *pFile);

GT_API GT_AlarmOff(short cardNum,short axis);
GT_API GT_AlarmOn(short cardNum,short axis);
GT_API GT_LmtsOn(short cardNum,short axis,short limitType=-1);
GT_API GT_LmtsOff(short cardNum,short axis,short limitType=-1);
GT_API GT_ProfileScale(short cardNum,short axis,short alpha,short beta);
GT_API GT_EncScale(short cardNum,short axis,short alpha,short beta);
GT_API GT_StepDir(short cardNum,short step);
GT_API GT_StepPulse(short cardNum,short step);
GT_API GT_StepOrthogonal(short cardNum,short step);
GT_API GT_SetMtrBias(short cardNum,short dac,short bias);
GT_API GT_GetMtrBias(short cardNum,short dac,short *pBias);
GT_API GT_SetMtrLmt(short cardNum,short dac,short limit);
GT_API GT_GetMtrLmt(short cardNum,short dac,short *pLimit);
GT_API GT_EncSns(short cardNum,unsigned short sense);
GT_API GT_EncOn(short cardNum,short encoder);
GT_API GT_EncOff(short cardNum,short encoder);
GT_API GT_SetPosErr(short cardNum,short control,long error);
GT_API GT_GetPosErr(short cardNum,short control,long *pError);
GT_API GT_SetStopDec(short cardNum,short profile,double decSmoothStop,double decAbruptStop);
GT_API GT_GetStopDec(short cardNum,short profile,double *pDecSmoothStop,double *pDecAbruptStop);
GT_API GT_LmtSns(short cardNum,unsigned short sense);
GT_API GT_CtrlMode(short cardNum,short axis,short mode);
GT_API GT_SetStopIo(short cardNum,short axis,short stopType,short inputType,short inputIndex);
GT_API GT_GpiSns(short cardNum,unsigned short sense);
GT_API GT_SetAdcFilter(short cardNum,short adc,short filterTime);
GT_API GT_SetAxisInputShaping(short cardNum,short axis,short enable,short count,double k);

GT_API GT_SetDo(short cardNum,short doType,long value);
GT_API GT_SetDoBit(short cardNum,short doType,short doIndex,short value);
GT_API GT_GetDo(short cardNum,short doType,long *pValue);
GT_API GT_SetDoBitReverse(short cardNum,short doType,short doIndex,short value,short reverseTime);
GT_API GT_SetDoMask(short cardNum,short doType,unsigned short doMask,long value);
GT_API GT_EnableDoBitPulse(short cardNum,short doType,short doIndex,unsigned short highLevelTime,unsigned short lowLevelTime,long pulseNum,short firstLevel);
GT_API GT_DisableDoBitPulse(short cardNum,short doType,short doIndex);

GT_API GT_GetDi(short cardNum,short diType,long *pValue);
GT_API GT_GetDiReverseCount(short cardNum,short diType,short diIndex,unsigned long *pReverseCount,short count=1);
GT_API GT_SetDiReverseCount(short cardNum,short diType,short diIndex,unsigned long *pReverseCount,short count=1);
GT_API GT_GetDiRaw(short cardNum,short diType,long *pValue);

GT_API GT_SetDac(short cardNum,short dac,short *pValue,short count=1);
GT_API GT_GetDac(short cardNum,short dac,short *pValue,short count=1,unsigned long *pClock=NULL);

GT_API GT_GetAdc(short cardNum,short adc,double *pValue,short count=1,unsigned long *pClock=NULL);
GT_API GT_GetAdcValue(short cardNum,short adc,short *pValue,short count=1,unsigned long *pClock=NULL);

GT_API GT_SetEncPos(short cardNum,short encoder,long encPos);
GT_API GT_GetEncPos(short cardNum,short encoder,double *pValue,short count=1,unsigned long *pClock=NULL);
GT_API GT_GetEncPosPre(short cardNum,short encoder,double *pValue,short count=1,unsigned long *pClock=NULL);
GT_API GT_GetEncVel(short cardNum,short encoder,double *pValue,short count=1,unsigned long *pClock=NULL);

GT_API GT_SetCaptureMode(short cardNum,short encoder,short mode);
GT_API GT_GetCaptureMode(short cardNum,short encoder,short *pMode,short count=1);
GT_API GT_StopCapture(short cardNum,short encoder);
GT_API GT_GetCaptureStatus(short cardNum,short encoder,short *pStatus,long *pValue,short count=1,unsigned long *pClock=NULL);
GT_API GT_GetCaptureStatusEx(short cardNum,short encoder,short *pStatus,long *pValue,double *pDoubleVar,unsigned long *pCaptureClock,short count=1,unsigned long *pClock=NULL);
GT_API GT_SetCaptureSense(short cardNum,short encoder,short mode,short sense);
GT_API GT_ClearCaptureStatus(short cardNum,short encoder);
GT_API GT_SetCaptureRepeat(short cardNum,short encoder,short count);
GT_API GT_GetCaptureRepeatStatus(short cardNum,short encoder,short *pCount);
GT_API GT_GetCaptureRepeatPos(short cardNum,short encoder,long *pValue,short startNum,short count);
GT_API GT_GetCaptureRepeatPosEx(short cardNum,short encoder,long *pValue,double *pDoubleVar,unsigned long *pCaptureClock,short startNum,short count);

GT_API GT_SetCaptureEncoder(short cardNum,short trigger,short encoder);
GT_API GT_GetCaptureWidth(short cardNum,short trigger,short *pWidth,short count=1);
GT_API GT_GetCaptureHomeGpi(short cardNum,short trigger,short pHomeSts,short *pHomePos,short pGpiSts,short *pGpiPos,short count=1);

GT_API GT_Reset(short cardNum);
GT_API GT_GetClock(short cardNum,unsigned long *pClock,unsigned long *pLoop=NULL);
GT_API GT_GetClockHighPrecision(short cardNum,unsigned long *pClock);

GT_API GT_GetSts(short cardNum,short axis,long *pSts,short count=1,unsigned long *pClock=NULL);
GT_API GT_ClrSts(short cardNum,short axis,short count=1);
GT_API GT_AxisOn(short cardNum,short axis);
GT_API GT_AxisOff(short cardNum,short axis);
GT_API GT_Stop(short cardNum,long mask,long option);
GT_API GT_SetPrfPos(short cardNum,short profile,long prfPos);
GT_API GT_SynchAxisPos(short cardNum,long mask);
GT_API GT_ZeroPos(short cardNum,short axis,short count=1);

GT_API GT_SetSoftLimit(short cardNum,short axis,long positive,long negative);
GT_API GT_GetSoftLimit(short cardNum,short axis,long *pPositive,long *pNegative);
GT_API GT_SetAxisBand(short cardNum,short axis,long band,long time);
GT_API GT_GetAxisBand(short cardNum,short axis,long *pBand,long *pTime);
GT_API GT_SetBacklash(short cardNum,short axis,long compValue,double compChangeValue,long compDir);
GT_API GT_GetBacklash(short cardNum,short axis,long *pCompValue,double *pCompChangeValue,long *pCompDir);
GT_API GT_SetLeadScrewComp(short cardNum,short axis,short n,long startPos,long lenPos,long *pCompPos,long *pCompNeg);
GT_API GT_EnableLeadScrewComp(short cardNum,short axis,short mode);
GT_API GT_GetCompensate(short cardNum,short axis,double *pPitchError,double *pCrossError,double *pBacklashError,double *pEncPos,double *pPrfPos);

GT_API GT_EnableGantry(short cardNum,short gantryMaster,short gantrySlave,double masterKp,double slaveKp);
GT_API GT_DisableGantry(short cardNum);
GT_API GT_SetGantryErrLmt(short cardNum,long gantryErrLmt);
GT_API GT_GetGantryErrLmt(short cardNum,long *pGantryErrLmt);
GT_API GT_ZeroGantryPos(short cardNum,short gantryMaster,short gantrySlave);

GT_API GT_GetPrfPos(short cardNum,short profile,double *pValue,short count=1,unsigned long *pClock=NULL);
GT_API GT_GetPrfVel(short cardNum,short profile,double *pValue,short count=1,unsigned long *pClock=NULL);
GT_API GT_GetPrfAcc(short cardNum,short profile,double *pValue,short count=1,unsigned long *pClock=NULL);
GT_API GT_GetPrfMode(short cardNum,short profile,long *pValue,short count=1,unsigned long *pClock=NULL);

GT_API GT_GetAxisPrfPos(short cardNum,short axis,double *pValue,short count=1,unsigned long *pClock=NULL);
GT_API GT_GetAxisPrfVel(short cardNum,short axis,double *pValue,short count=1,unsigned long *pClock=NULL);
GT_API GT_GetAxisPrfAcc(short cardNum,short axis,double *pValue,short count=1,unsigned long *pClock=NULL);
GT_API GT_GetAxisEncPos(short cardNum,short axis,double *pValue,short count=1,unsigned long *pClock=NULL);
GT_API GT_GetAxisEncVel(short cardNum,short axis,double *pValue,short count=1,unsigned long *pClock=NULL);
GT_API GT_GetAxisEncAcc(short cardNum,short axis,double *pValue,short count=1,unsigned long *pClock=NULL);
GT_API GT_GetAxisError(short cardNum,short axis,double *pValue,short count=1,unsigned long *pClock=NULL);

GT_API GT_SetLongVar(short cardNum,short index,long value);
GT_API GT_GetLongVar(short cardNum,short index,long *pValue);
GT_API GT_SetDoubleVar(short cardNum,short index,double pValue);
GT_API GT_GetDoubleVar(short cardNum,short index,double *pValue);

GT_API GT_SetControlFilter(short cardNum,short control,short index);
GT_API GT_GetControlFilter(short cardNum,short control,short *pIndex);

GT_API GT_SetPid(short cardNum,short control,short index,TPid *pPid);
GT_API GT_GetPid(short cardNum,short control,short index,TPid *pPid);

GT_API GT_SetKvffFilter(short cardNum,short control,short index,short kvffFilterExp,double accMax);
GT_API GT_GetKvffFilter(short cardNum,short control,short index,short *pKvffFilterExp,double *pAccMax);

GT_API GT_Update(short cardNum,long mask);
GT_API GT_SetPos(short cardNum,short profile,long pos);
GT_API GT_GetPos(short cardNum,short profile,long *pPos);
GT_API GT_SetVel(short cardNum,short profile,double vel);
GT_API GT_GetVel(short cardNum,short profile,double *pVel);

GT_API GT_PrfTrap(short cardNum,short profile);
GT_API GT_SetTrapPrm(short cardNum,short profile,TTrapPrm *pPrm);
GT_API GT_GetTrapPrm(short cardNum,short profile,TTrapPrm *pPrm);
GT_API GT_GetTrapTime(short cardNum,short profile,TTrapTime *pTime);

GT_API GT_PrfJog(short cardNum,short profile);
GT_API GT_SetJogPrm(short cardNum,short profile,TJogPrm *pPrm);
GT_API GT_GetJogPrm(short cardNum,short profile,TJogPrm *pPrm);

GT_API GT_PrfPt(short cardNum,short profile,short mode=PT_MODE_STATIC);
GT_API GT_SetPtLoop(short cardNum,short profile,long loop);
GT_API GT_GetPtLoop(short cardNum,short profile,long *pLoop);
GT_API GT_PtSpace(short cardNum,short profile,short *pSpace,short fifo=0);
GT_API GT_PtData(short cardNum,short profile,double pos,long time,short type=PT_SEGMENT_NORMAL,short fifo=0);
GT_API GT_PtDataWN(short cardNum,short profile,double pos,long time,short type=PT_SEGMENT_NORMAL,long segNum=0,short fifo=0);
GT_API GT_PtClear(short cardNum,short profile,short fifo=0);
GT_API GT_PtStart(short cardNum,long mask,long option=0);
GT_API GT_SetPtMemory(short cardNum,short profile,short memory);
GT_API GT_GetPtMemory(short cardNum,short profile,short *pMemory);
GT_API GT_PtGetSegNum(short cardNum,short profile,long *pSegNum);
GT_API GT_SetPtPrecisionMode(short cardNum,short profile,short precisionMode);
GT_API GT_GetPtPrecisionMode(short cardNum,short profile,short *pPrecisionMode);
GT_API GT_SetPtVel(short cardNum,short profile,double velLast,short fifo);
GT_API GT_SetPtLink(short cardNum,short profile,short fifo,short list);
GT_API GT_GetPtLink(short cardNum,short profile,short fifo,short *pList);
GT_API GT_PtDoBit(short cardNum,short profile,short doType,short index,short value,short fifo);
GT_API GT_PtAo(short cardNum,short profile,short aoType,short index,double value,short fifo);

GT_API GT_PrfGear(short cardNum,short profile,short dir=0);
GT_API GT_SetGearMaster(short cardNum,short profile,short masterIndex,short masterType=GEAR_MASTER_PROFILE,short masterItem=0);
GT_API GT_GetGearMaster(short cardNum,short profile,short *pMasterIndex,short *pMasterType=NULL,short *pMasterItem=NULL);
GT_API GT_SetGearRatio(short cardNum,short profile,long masterEven,long slaveEven,long masterSlope=0);
GT_API GT_GetGearRatio(short cardNum,short profile,long *pMasterEven,long *pSlaveEven,long *pMasterSlope=NULL);
GT_API GT_GearStart(short cardNum,long mask);
GT_API GT_SetGearEvent(short cardNum,short profile,short event,long startPara0,long startPara1);
GT_API GT_GetGearEvent(short cardNum,short profile,short *pEvent,long *pStartPara0,long *pStartPara1);

GT_API GT_PrfFollow(short cardNum,short profile,short dir=0);
GT_API GT_SetFollowMaster(short cardNum,short profile,short masterIndex,short masterType=FOLLOW_MASTER_PROFILE,short masterItem=0);
GT_API GT_GetFollowMaster(short cardNum,short profile,short *pMasterIndex,short *pMasterType=NULL,short *pMasterItem=NULL);
GT_API GT_SetFollowLoop(short cardNum,short profile,long loop);
GT_API GT_GetFollowLoop(short cardNum,short profile,long *pLoop);
GT_API GT_SetFollowEvent(short cardNum,short profile,short event,short masterDir,long pos=0);
GT_API GT_GetFollowEvent(short cardNum,short profile,short *pEvent,short *pMasterDir,long *pPos=NULL);
GT_API GT_FollowSpace(short cardNum,short profile,short *pSpace,short fifo=0);
GT_API GT_FollowData(short cardNum,short profile,long masterSegment,double slaveSegment,short type=FOLLOW_SEGMENT_NORMAL,short fifo=0);
GT_API GT_FollowClear(short cardNum,short profile,short fifo=0);
GT_API GT_FollowStart(short cardNum,long mask,long option=0);
GT_API GT_FollowSwitch(short cardNum,long mask);
GT_API GT_SetFollowMemory(short cardNum,short profile,short memory);
GT_API GT_GetFollowMemory(short cardNum,short profile,short *pMemory);
GT_API GT_GetFollowStatus(short cardNum,short profile,short *pFifoNum,short *pSwitchStatus);
GT_API GT_SetFollowPhasing(short cardNum,short profile,short profilePhasing);
GT_API GT_GetFollowPhasing(short cardNum,short profile,short *pProfilePhasing);

GT_API GT_Compile(short cardNum,char *pFileName,TCompileInfo *pWrongInfo);
GT_API GT_Download(short cardNum,char *pFileName);

GT_API GT_GetFunId(short cardNum,char *pFunName,short *pFunId);
GT_API GT_Bind(short cardNum,short thread,short funId,short page);

GT_API GT_RunThread(short cardNum,short thread);
GT_API GT_StopThread(short cardNum,short thread);
GT_API GT_PauseThread(short cardNum,short thread);

GT_API GT_GetThreadSts(short cardNum,short thread,TThreadSts *pThreadSts);

GT_API GT_GetVarId(short cardNum,char *pFunName,char *pVarName,TVarInfo *pVarInfo);
GT_API GT_SetVarValue(short cardNum,short page,TVarInfo *pVarInfo,double *pValue,short count=1);
GT_API GT_GetVarValue(short cardNum,short page,TVarInfo *pVarInfo,double *pValue,short count=1);

GT_API GT_SetCrdMapBase(short cardNum,short crd,short base);
GT_API GT_GetCrdMapBase(short cardNum,short crd,short *pBase);
GT_API GT_SetCrdSmooth(short cardNum,short crd,TCrdSmooth *pCrdSmooth);
GT_API GT_GetCrdSmooth(short cardNum,short crd,TCrdSmooth *pCrdSmooth);
GT_API GT_SetCrdJerk(short cardNum,short crd,double jerkMax);
GT_API GT_GetCrdJerk(short cardNum,short crd,double *pJerkMax);
GT_API GT_SetCrdPrm(short cardNum,short crd,TCrdPrm *pCrdPrm);
GT_API GT_GetCrdPrm(short cardNum,short crd,TCrdPrm *pCrdPrm);
GT_API GT_SetArcAllowError(short cardNum,short crd,double error);
GT_API GT_CrdSpace(short cardNum,short crd,long *pSpace,short fifo=0);
GT_API GT_CrdData(short cardNum,short crd,TCrdData *pCrdData,short fifo=0);
GT_API GT_CrdDataCircle(short cardNum,short crd,TCrdData *pCrdData,short fifo=0);
GT_API GT_LnXY(short cardNum,short crd,long x,long y,double synVel,double synAcc,double velEnd=0,short fifo=0);
GT_API GT_LnXYZ(short cardNum,short crd,long x,long y,long z,double synVel,double synAcc,double velEnd=0,short fifo=0);
GT_API GT_LnXYZA(short cardNum,short crd,long x,long y,long z,long a,double synVel,double synAcc,double velEnd=0,short fifo=0);
GT_API GT_LnXYG0(short cardNum,short crd,long x,long y,double synVel,double synAcc,short fifo=0);
GT_API GT_LnXYZG0(short cardNum,short crd,long x,long y,long z,double synVel,double synAcc,short fifo=0);
GT_API GT_LnXYZAG0(short cardNum,short crd,long x,long y,long z,long a,double synVel,double synAcc,short fifo=0);
GT_API GT_ArcXYR(short cardNum,short crd,long x,long y,double radius,short circleDir,double synVel,double synAcc,double velEnd=0,short fifo=0);
GT_API GT_ArcXYC(short cardNum,short crd,long x,long y,double xCenter,double yCenter,short circleDir,double synVel,double synAcc,double velEnd=0,short fifo=0);
GT_API GT_ArcYZR(short cardNum,short crd,long y,long z,double radius,short circleDir,double synVel,double synAcc,double velEnd=0,short fifo=0);
GT_API GT_ArcYZC(short cardNum,short crd,long y,long z,double yCenter,double zCenter,short circleDir,double synVel,double synAcc,double velEnd=0,short fifo=0);
GT_API GT_ArcZXR(short cardNum,short crd,long z,long x,double radius,short circleDir,double synVel,double synAcc,double velEnd=0,short fifo=0);
GT_API GT_ArcZXC(short cardNum,short crd,long z,long x,double zCenter,double xCenter,short circleDir,double synVel,double synAcc,double velEnd=0,short fifo=0);
GT_API GT_ArcXYZ(short cardNum,short crd,long x,long y,long z,double interX,double interY,double interZ,double synVel,double synAcc,double velEnd=0,short fifo=0);
GT_API GT_LnXYOverride2(short cardNum,short crd,long x,long y,double synVel,double synAcc,double velEnd=0,short fifo=0);
GT_API GT_LnXYZOverride2(short cardNum,short crd,long x,long y,long z,double synVel,double synAcc,double velEnd=0,short fifo=0);
GT_API GT_LnXYZAOverride2(short cardNum,short crd,long x,long y,long z,long a,double synVel,double synAcc,double velEnd=0,short fifo=0);
GT_API GT_LnXYG0Override2(short cardNum,short crd,long x,long y,double synVel,double synAcc,short fifo=0);
GT_API GT_LnXYZG0Override2(short cardNum,short crd,long x,long y,long z,double synVel,double synAcc,short fifo=0);
GT_API GT_LnXYZAG0Override2(short cardNum,short crd,long x,long y,long z,long a,double synVel,double synAcc,short fifo=0);
GT_API GT_ArcXYROverride2(short cardNum,short crd,long x,long y,double radius,short circleDir,double synVel,double synAcc,double velEnd=0,short fifo=0);
GT_API GT_ArcXYCOverride2(short cardNum,short crd,long x,long y,double xCenter,double yCenter,short circleDir,double synVel,double synAcc,double velEnd=0,short fifo=0);
GT_API GT_ArcYZROverride2(short cardNum,short crd,long y,long z,double radius,short circleDir,double synVel,double synAcc,double velEnd=0,short fifo=0);
GT_API GT_ArcYZCOverride2(short cardNum,short crd,long y,long z,double yCenter,double zCenter,short circleDir,double synVel,double synAcc,double velEnd=0,short fifo=0);
GT_API GT_ArcZXROverride2(short cardNum,short crd,long z,long x,double radius,short circleDir,double synVel,double synAcc,double velEnd=0,short fifo=0);
GT_API GT_ArcZXCOverride2(short cardNum,short crd,long z,long x,double zCenter,double xCenter,short circleDir,double synVel,double synAcc,double velEnd=0,short fifo=0);
GT_API GT_HelixXYRZ(short cardNum,short crd,long x,long y,long z,double radius,short circleDir,double synVel,double synAcc,double velEnd,short fifo=0);
GT_API GT_HelixXYCZ(short cardNum,short crd,long x,long y,long z,double xCenter,double yCenter,short circleDir,double synVel,double synAcc,double velEnd=0,short fifo=0);
GT_API GT_HelixYZRX(short cardNum,short crd,long x,long y,long z,double radius,short circleDir,double synVel,double synAcc,double velEnd=0,short fifo=0);
GT_API GT_HelixYZCX(short cardNum,short crd,long x,long y,long z,double yCenter,double zCenter,short circleDir,double synVel,double synAcc,double velEnd=0,short fifo=0);
GT_API GT_HelixZXRY(short cardNum,short crd,long x,long y,long z,double radius,short circleDir,double synVel,double synAcc,double velEnd=0,short fifo=0);
GT_API GT_HelixZXCY(short cardNum,short crd,long x,long y,long z,double zCenter,double xCenter,short circleDir,double synVel,double synAcc,double velEnd=0,short fifo=0);
GT_API GT_HelixXYRZOverride2(short cardNum,short crd,long x,long y,long z,double radius,short circleDir,double synVel,double synAcc,double velEnd=0,short fifo=0);
GT_API GT_HelixXYCZOverride2(short cardNum,short crd,long x,long y,long z,double xCenter,double yCenter,short circleDir,double synVel,double synAcc,double velEnd=0,short fifo=0);
GT_API GT_HelixYZRXOverride2(short cardNum,short crd,long x,long y,long z,double radius,short circleDir,double synVel,double synAcc,double velEnd=0,short fifo=0);
GT_API GT_HelixYZCXOverride2(short cardNum,short crd,long x,long y,long z,double yCenter,double zCenter,short circleDir,double synVel,double synAcc,double velEnd=0,short fifo=0);
GT_API GT_HelixZXRYOverride2(short cardNum,short crd,long x,long y,long z,double radius,short circleDir,double synVel,double synAcc,double velEnd=0,short fifo=0);
GT_API GT_HelixZXCYOverride2(short cardNum,short crd,long x,long y,long z,double zCenter,double xCenter,short circleDir,double synVel,double synAcc,double velEnd=0,short fifo=0);
GT_API GT_LnXYWN(short cardNum,short crd,long x,long y,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_LnXYZWN(short cardNum,short crd,long x,long y,long z,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_LnXYZAWN(short cardNum,short crd,long x,long y,long z,long a,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_LnXYG0WN(short cardNum,short crd,long x,long y,double synVel,double synAcc,long segNum=0,short fifo=0);
GT_API GT_LnXYZG0WN(short cardNum,short crd,long x,long y,long z,double synVel,double synAcc,long segNum=0,short fifo=0);
GT_API GT_LnXYZAG0WN(short cardNum,short crd,long x,long y,long z,long a,double synVel,double synAcc,long segNum=0,short fifo=0);
GT_API GT_ArcXYRWN(short cardNum,short crd,long x,long y,double radius,short circleDir,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_ArcXYCWN(short cardNum,short crd,long x,long y,double xCenter,double yCenter,short circleDir,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_ArcYZRWN(short cardNum,short crd,long y,long z,double radius,short circleDir,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_ArcYZCWN(short cardNum,short crd,long y,long z,double yCenter,double zCenter,short circleDir,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_ArcZXRWN(short cardNum,short crd,long z,long x,double radius,short circleDir,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_ArcZXCWN(short cardNum,short crd,long z,long x,double zCenter,double xCenter,short circleDir,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_ArcXYZWN(short cardNum,short crd,long x,long y,long z,double interX,double interY,double interZ,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_LnXYOverride2WN(short cardNum,short crd,long x,long y,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_LnXYZOverride2WN(short cardNum,short crd,long x,long y,long z,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_LnXYZAOverride2WN(short cardNum,short crd,long x,long y,long z,long a,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_LnXYG0Override2WN(short cardNum,short crd,long x,long y,double synVel,double synAcc,long segNum=0,short fifo=0);
GT_API GT_LnXYZG0Override2WN(short cardNum,short crd,long x,long y,long z,double synVel,double synAcc,long segNum=0,short fifo=0);
GT_API GT_LnXYZAG0Override2WN(short cardNum,short crd,long x,long y,long z,long a,double synVel,double synAcc,long segNum=0,short fifo=0);
GT_API GT_ArcXYROverride2WN(short cardNum,short crd,long x,long y,double radius,short circleDir,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_ArcXYCOverride2WN(short cardNum,short crd,long x,long y,double xCenter,double yCenter,short circleDir,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_ArcYZROverride2WN(short cardNum,short crd,long y,long z,double radius,short circleDir,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_ArcYZCOverride2WN(short cardNum,short crd,long y,long z,double yCenter,double zCenter,short circleDir,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_ArcZXROverride2WN(short cardNum,short crd,long z,long x,double radius,short circleDir,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_ArcZXCOverride2WN(short cardNum,short crd,long z,long x,double zCenter,double xCenter,short circleDir,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_ArcXYZOverride2WN(short cardNum,short crd,long x,long y,long z,double interX,double interY,double interZ,double synVel,double synAcc,double velEnd,long segNum,short fifo=0);
GT_API GT_HelixXYRZWN(short cardNum,short crd,long x,long y,long z,double radius,short circleDir,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_HelixXYCZWN(short cardNum,short crd,long x,long y,long z,double xCenter,double yCenter,short circleDir,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_HelixYZRXWN(short cardNum,short crd,long x,long y,long z,double radius,short circleDir,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_HelixYZCXWN(short cardNum,short crd,long x,long y,long z,double yCenter,double zCenter,short circleDir,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_HelixZXRYWN(short cardNum,short crd,long x,long y,long z,double radius,short circleDir,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_HelixZXCYWN(short cardNum,short crd,long x,long y,long z,double zCenter,double xCenter,short circleDir,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_HelixXYRZOverride2WN(short cardNum,short crd,long x,long y,long z,double radius,short circleDir,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_HelixXYCZOverride2WN(short cardNum,short crd,long x,long y,long z,double xCenter,double yCenter,short circleDir,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_HelixYZRXOverride2WN(short cardNum,short crd,long x,long y,long z,double radius,short circleDir,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_HelixYZCXOverride2WN(short cardNum,short crd,long x,long y,long z,double yCenter,double zCenter,short circleDir,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_HelixZXRYOverride2WN(short cardNum,short crd,long x,long y,long z,double radius,short circleDir,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_HelixZXCYOverride2WN(short cardNum,short crd,long x,long y,long z,double zCenter,double xCenter,short circleDir,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_BufTrend(short cardNum,short crd,unsigned long trendSegNum,double trendDistance,double trendVelEnd,short fifo=0);
GT_API GT_BufIO(short cardNum,short crd,unsigned short doType,unsigned short doMask,unsigned short doValue,short fifo=0);
GT_API GT_BufEnableDoBitPulse(short cardNum,short crd,short doType,short doIndex,unsigned short highLevelTime,unsigned short lowLevelTime,long pulseNum,short firstLevel,short fifo=0);
GT_API GT_BufDisableDoBitPulse(short cardNum,short crd,short doType,short doIndex,short fifo=0);
GT_API GT_BufDelay(short cardNum,short crd,unsigned short delayTime,short fifo=0);
GT_API GT_BufComparePulse(short cardNum,short crd,short level,short outputType,short time,short fifo=0);
GT_API GT_BufDA(short cardNum,short crd,short chn,short daValue,short fifo=0);
GT_API GT_BufLmtsOn(short cardNum,short crd,short axis,short limitType,short fifo=0);
GT_API GT_BufLmtsOff(short cardNum,short crd,short axis,short limitType,short fifo=0);
GT_API GT_BufSetStopIo(short cardNum,short crd,short axis,short stopType,short inputType,short inputIndex,short fifo=0);
GT_API GT_BufMove(short cardNum,short crd,short moveAxis,long pos,double vel,double acc,short modal,short fifo=0);
GT_API GT_BufGear(short cardNum,short crd,short gearAxis,long pos,short fifo=0);
GT_API GT_BufGearPercent(short cardNum,short crd,short gearAxis,long pos,short accPercent,short decPercent,short fifo=0);
GT_API GT_BufStopMotion(short cardNum,short crd,short fifo=0);
GT_API GT_BufSetVarValue(short cardNum,short crd,short pageId,TVarInfo *pVarInfo,double value,short fifo=0);
GT_API GT_BufJumpNextSeg(short cardNum,short crd,short axis,short limitType,short fifo=0);
GT_API GT_BufSynchPrfPos(short cardNum,short crd,short encoder,short profile,short fifo=0);
GT_API GT_BufVirtualToActual(short cardNum,short crd,short fifo=0);
GT_API GT_BufSetLongVar(short cardNum,short crd,short index,long value,short fifo=0);
GT_API GT_BufSetDoubleVar(short cardNum,short crd,short index,double value,short fifo=0);
GT_API GT_CrdStart(short cardNum,short mask,short option);
GT_API GT_CrdStartStep(short cardNum,short mask,short option);
GT_API GT_CrdStepMode(short cardNum,short mask,short option);
GT_API GT_SetOverride(short cardNum,short crd,double synVelRatio);
GT_API GT_SetOverride2(short cardNum,short crd,double synVelRatio);
GT_API GT_SetMaxOverrideLA(short cardNum,double maxSynVelRatio);
GT_API GT_InitLookAhead(short cardNum,short crd,short fifo,double T,double accMax,short n,TCrdData *pLookAheadBuf);
GT_API GT_GetLookAheadSpace(short cardNum,short crd,long *pSpace,short fifo=0);
GT_API GT_GetLookAheadSegCount(short cardNum,short crd,long *pSegCount,short fifo=0);
GT_API GT_CrdClear(short cardNum,short crd,short fifo);
GT_API GT_CrdStatus(short cardNum,short crd,short *pRun,long *pSegment,short fifo=0);
GT_API GT_SetUserSegNum(short cardNum,short crd,long segNum,short fifo=0);
GT_API GT_GetUserSegNum(short cardNum,short crd,long *pSegment,short fifo=0);
GT_API GT_GetUserSegNumWN(short cardNum,short crd,long *pSegment,short fifo=0);
GT_API GT_GetRemainderSegNum(short cardNum,short crd,long *pSegment,short fifo=0);
GT_API GT_SetCrdStopDec(short cardNum,short crd,double decSmoothStop,double decAbruptStop);
GT_API GT_GetCrdStopDec(short cardNum,short crd,double *pDecSmoothStop,double *pDecAbruptStop);
GT_API GT_SetCrdLmtStopMode(short cardNum,short crd,short lmtStopMode);
GT_API GT_GetCrdLmtStopMode(short cardNum,short crd,short *pLmtStopMode);
GT_API GT_GetUserTargetVel(short cardNum,short crd,double *pTargetVel);
GT_API GT_GetSegTargetPos(short cardNum,short crd,long *pTargetPos);
GT_API GT_GetCrdPos(short cardNum,short crd,double *pPos);
GT_API GT_GetCrdVel(short cardNum,short crd,double *pSynVel);
GT_API GT_SetCrdSingleMaxVel(short cardNum,short crd,double *pMaxVel);
GT_API GT_GetCrdSingleMaxVel(short cardNum,short crd,double *pMaxVel);
GT_API GT_GetCmdCount(short cardNum,short crd,short *pResult,short fifo);
GT_API GT_BufLaserOn(short cardNum,short crd,short fifo=0,short channel=0);
GT_API GT_BufLaserOff(short cardNum,short crd,short fifo=0,short channel=0);
GT_API GT_BufLaserPrfCmd(short cardNum,short crd,double laserPower,short fifo=0,short channel=0);
GT_API GT_BufLaserFollowRatio(short cardNum,short crd,double ratio,double minPower,double maxPower,short fifo=0,short channel=0);
GT_API GT_BufLaserFollowMode(short cardNum,short crd,short source =0,short fifo=0,short channel=0,double startPower =0);
GT_API GT_BufLaserFollowOff(short cardNum,short crd,short fifo=0,short channel=0);
GT_API GT_BufLaserFollowSpline(short cardNum,short crd,short tableId,double minPower,double maxPower,short fifo=0,short channel=0);
GT_API GT_SetCrdBufferMode(short cardNum,short crd,short staticMode,short fifo);
GT_API GT_GetCrdBufferMode(short cardNum,short crd,short *pStaticMode,short fifo);
GT_API GT_GetCrdSegmentTime(short cardNum,short crd,long segmentIndex,double *pSegmentTime,long *pSegmentNumber,short fifo);
GT_API GT_GetCrdTime(short cardNum,short crd,TCrdTime *pTime,short fifo);
GT_API GT_LnXYZACUVW(short cardNum,short crd,long *pPos,short posMask,double synVel,double synAcc,double velEnd=0,short fifo=0);
GT_API GT_LnXYZACUVWWN(short cardNum,short crd,long *pPos,short posMask,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_LnXYZACUVWOverride2(short cardNum,short crd,long *pPos,short posMask,double synVel,double synAcc,double velEnd=0,short fifo=0);
GT_API GT_LnXYZACUVWOverride2WN(short cardNum,short crd,long *pPos,short posMask,double synVel,double synAcc,double velEnd=0,long segNum=0,short fifo=0);
GT_API GT_BufWaitLongVar(short cardNum,short crd, short index, long value, long overTime, short flagMode, long segNum, short fifo);
GT_API GT_GetBufWaitLongVarStatus(short cardNum,short crd,short *pIndex,long *pValue,short *pFlagMode,long *pSegNum,short *pEnable,short *pStatus,short fifo);
GT_API GT_ClearBufWaitStatus(short cardNum,short crd,short fifo);

GT_API GT_PrfPvt(short cardNum,short profile);
GT_API GT_SetPvtLoop(short cardNum,short profile,long loop);
GT_API GT_GetPvtLoop(short cardNum,short profile,long *pLoopCount,long *pLoop);
GT_API GT_PvtStatus(short cardNum,short profile,short *pTableId,double *pTime,short count=1);
GT_API GT_PvtTableClear(short cardNum,short tableId);
GT_API GT_PvtStart(short cardNum,long mask);
GT_API GT_PvtTableSelect(short cardNum,short profile,short tableId);

GT_API GT_PvtTable(short cardNum,short tableId,long count,double *pTime,double *pPos,double *pVel);
GT_API GT_PvtTableEx(short cardNum,short tableId,long count,double *pTime,double *pPos,double *pVelBegin,double *pVelEnd);
GT_API GT_PvtTableComplete(short cardNum,short tableId,long count,double *pTime,double *pPos,double *pA,double *pB,double *pC,double velBegin=0,double velEnd=0);
GT_API GT_PvtTablePercent(short cardNum,short tableId,long count,double *pTime,double *pPos,double *pPercent,double velBegin=0);
GT_API GT_PvtPercentCalculate(short cardNum,long n,double *pTime,double *pPos,double *pPercent,double velBegin,double *pVel);
GT_API GT_PvtTableContinuous(short cardNum,short tableId,long count,double *pPos,double *pVel,double *pPercent,double *pVelMax,double *pAcc,double *pDec,double timeBegin);
GT_API GT_PvtContinuousCalculate(short cardNum,long n,double *pPos,double *pVel,double *pPercent,double *pVelMax,double *pAcc,double *pDec,double *pTime);
GT_API GT_PvtTableMovePercent(short cardNum,short tableId,long distance,double vm,double acc,double pa1,double pa2,double dec,double pd1,double pd2,double *pVel,double *pAcc,double *pDec,double *pTime);
GT_API GT_PvtTableMovePercentEx(short cardNum,short tableId,long distance,double vm,double acc,double pa1,double pa2,double ma,double dec,double pd1,double pd2,double md,double *pVel,double *pAcc,double *pDec,double *pTime);

GT_API GT_HomeInit(short cardNum);
GT_API GT_Home(short cardNum,short axis,long pos,double vel,double acc,long offset);
GT_API GT_Index(short cardNum,short axis,long pos,long offset);
GT_API GT_HomeStop(short cardNum,short axis,long pos,double vel,double acc);
GT_API GT_HomeSts(short cardNum,short axis,unsigned short *pStatus);

GT_API GT_HandwheelInit(short cardNum);
GT_API GT_SetHandwheelStopDec(short cardNum,short slave,double decSmoothStop,double decAbruptStop);
GT_API GT_StartHandwheel(short cardNum,short slave,short master,short masterEven,short slaveEven,short intervalTime,double acc,double dec,double vel,short stopWaitTime);
GT_API GT_EndHandwheel(short cardNum,short slave);

GT_API GT_SetTrigger(short cardNum,short i,TTrigger *pTrigger);
GT_API GT_GetTrigger(short cardNum,short i,TTrigger *pTrigger);
GT_API GT_GetTriggerStatus(short cardNum,short i,TTriggerStatus *pTriggerStatus,short count=1);
GT_API GT_GetTriggerStatusEx(short cardNum,short i,TTriggerStatusEx *pTriggerStatusEx,short count=1);
GT_API GT_ClearTriggerStatus(short cardNum,short i);
GT_API GT_GetTriggerLatchValue(short cardNum,short i,long count,long *pValue,long *pCount,TLatchValueInfo *pInfo);

GT_API GT_SetComparePort(short cardNum,short channel,short hsio0,short hsio1);

GT_API GT_ComparePulse(short cardNum,short level,short outputType,short time);
GT_API GT_CompareStop(short cardNum);
GT_API GT_CompareStatus(short cardNum,short *pStatus,long *pCount);
GT_API GT_CompareData(short cardNum,short encoder,short source,short pulseType,short startLevel,short time,
                      long *pBuf1,short count1,long *pBuf2,short count2);
GT_API GT_CompareLinear(short cardNum,short encoder,short channel,long startPos,long repeatTimes,long interval,short time,short source);
GT_API GT_CompareContinuePulseMode(short cardNum,short mode,short count,short standTime);

GT_API GT_SetEncResponseCheck(short cardNum,short control,short dacThreshold,double minEncVel,long time);
GT_API GT_GetEncResponseCheck(short cardNum,short control,short *pDacThreshold,double *pMinEncVel,long *pTime);
GT_API GT_EnableEncResponseCheck(short cardNum,short control);
GT_API GT_DisableEncResponseCheck(short cardNum,short control);

GT_API GT_SetAxisFollowErrorMode(short cardNum,short axis,short followErrorMode);
GT_API GT_GetAxisFollowErrorMode(short cardNum,short axis,short *pFollowErrorMode);

GT_API GT_2DCompareMode(short cardNum,short chn,short mode);
GT_API GT_2DComparePulse(short cardNum,short chn,short level,short outputType,short time);
GT_API GT_2DCompareStop(short cardNum,short chn);
GT_API GT_2DCompareClear(short cardNum,short chn);
GT_API GT_2DCompareStatus(short cardNum,short chn,short *pStatus,long *pCount,short *pFifo,short *pFifoCount,short *pBufCount =NULL);
GT_API GT_2DCompareSetPrm(short cardNum,short chn,T2DComparePrm *pPrm);
GT_API GT_2DCompareData(short cardNum,short chn,short count,T2DCompareData *pBuf,short fifo);
GT_API GT_2DCompareStart(short cardNum,short chn);
GT_API GT_2DCompareClearData(short cardNum,short chn);
GT_API GT_2DCompareSetPreOutTime(short cardNum,short chn,double preOutputTime);
GT_API GT_2DCompareSetPrmEx(short cardNum,short chn,T2DComparePrmEx *pMode);
GT_API GT_2DCompareDataGroup(short cardNum,short chn,short count,T2DCompareDataGroup *pDataGroup,short fifo);
GT_API GT_2DCompareLinear(short cardNum,short chn,T2DCompareLinear *pPrm);

GT_API GT_SetAxisMode(short cardNum,short axis,short mode);
GT_API GT_GetAxisMode(short cardNum,short axis,short *pMode);
GT_API GT_SetHSIOOpt(short cardNum,unsigned short value,short channel=0);
GT_API GT_GetHSIOOpt(short cardNum,unsigned short *pValue,short channel=0);
GT_API GT_LaserPowerMode(short cardNum,short laserPowerMode,double maxValue,double minValue,short channel=0,short delaymode =0);
GT_API GT_LaserPrfCmd(short cardNum,double outputCmd,short channel=0);
GT_API GT_LaserOutFrq(short cardNum,double outFrq,short channel=0);
GT_API GT_SetPulseWidth(short cardNum,unsigned long width,short channel=0);
GT_API GT_SetWaitPulse(short cardNum,unsigned short mode,double waitPulseFrq,double waitPulseDuty,short channel=0);
GT_API GT_SetPreVltg(short cardNum,unsigned short mode,double voltageValue,short channel=0);
GT_API GT_SetLevelDelay(short cardNum,unsigned short offDelay,unsigned short onDelay,short channel=0);
GT_API GT_EnaFPK(short cardNum,unsigned short time1,unsigned short time2,unsigned short laserOffDelay,short channel=0);
GT_API GT_DisFPK(short cardNum,short channel=0);
GT_API GT_SetLaserDisMode(short cardNum,short mode,short source,long *pPos,double *pScale,short channel=0);
GT_API GT_SetLaserDisRatio(short cardNum,double *pRatio,double minPower,double maxPower,short channel=0);
GT_API GT_SetWaitPulseEx(short cardNum,unsigned short mode,double waitPulseFrq,double waitPulseDuty);
GT_API GT_SetLaserMode(short cardNum,short mode);
GT_API GT_SetLaserFollowSpline(short cardNum,short tableId,long n,double *pX,double *pY,double beginValue,double endValue,short channel=0);
GT_API GT_GetLaserFollowSpline(short cardNum,short tableId,long n,double *pX,double *pY,double *pA,double *pB,double *pC,long *pCount,short channel=0);

GT_API GT_OpenExtMdl(short cardNum,char *pDllName=NULL);
GT_API GT_CloseExtMdl(short cardNum);
GT_API GT_SwitchtoCardNoExtMdl(short card);

GT_API GT_ResetExtMdl(short cardNum);
GT_API GT_LoadExtConfig(short cardNum,char *pFileName);
GT_API GT_SetExtIoValue(short cardNum,short mdl,unsigned short value);
GT_API GT_GetExtIoValue(short cardNum,short mdl,unsigned short *pValue);
GT_API GT_SetExtIoBit(short cardNum,short mdl,short index,unsigned short value);
GT_API GT_GetExtIoBit(short cardNum,short mdl,short index,unsigned short *pValue);
GT_API GT_GetExtAdValue(short cardNum,short mdl,short chn,unsigned short *pValue);
GT_API GT_GetExtAdVoltage(short cardNum,short mdl,short chn,double *pValue);
GT_API GT_SetExtDaValue(short cardNum,short mdl,short chn,unsigned short value);
GT_API GT_SetExtDaVoltage(short cardNum,short mdl,short chn,double value);
GT_API GT_GetStsExtMdl(short cardNum,short mdl,short chn,unsigned short *pStatus);
GT_API GT_GetExtDoValue(short cardNum,short mdl,unsigned short *pValue);
GT_API GT_GetExtAdCurrent(short cardNum,short mdl,short chn,double *pValue);

GT_API GT_GetExtMdlMode(short cardNum,short *pMode);
GT_API GT_SetExtMdlMode(short cardNum,short mode);
GT_API GT_UploadConfig(short cardNum);
GT_API GT_DownloadConfig(short cardNum);
GT_API GT_GetConfig(short cardNum,short mdl,TExtMdlCfgInfo *pExtMdlCfgInfo);
GT_API GT_SetConfig(short cardNum,short mdl,TExtMdlCfgInfo *pExtMdlCfgInfo);
GT_API GT_GetDriverVersion(short cardNum,unsigned short *mainVer,unsigned short *slaveVer);
GT_API GT_SaveConfigToFile(short cardNum,char *pFile);
GT_API GT_GetChnStsExtMdl(short cardNum,unsigned short *pStatus);

GT_API GT_GetUuid(short cardNum,char *pCode,short count);
GT_API GT_SetUuid(short cardNum,char *pCode,short count);

//////////////////////////////////////////////////////////////////////////
//2D Compensate
//////////////////////////////////////////////////////////////////////////
typedef struct  
{
	short count[2];                               // 行数和列数，最小值为2
	long posBegin[2];                             // 起点位置
	long step[2];                                 // 步长
} TCompensate2DTable;

typedef struct  
{
	short enable;                                  // 2D补偿使能标志
	short tableIndex;                              // 所使用的2D补偿表
	short axisType[2];	                           // 查表轴类型
	short axisIndex[2];	                           // 查表轴索引
} TCompensate2D;

GT_API GT_SetCompensate2DTable(short cardNum,short tableIndex,TCompensate2DTable *pTable,long *pData,short externComp=0);
GT_API GT_GetCompensate2DTable(short cardNum,short tableIndex,TCompensate2DTable *pTable,short *pExternComp=NULL);
GT_API GT_SetCompensate2D(short cardNum,short axis,TCompensate2D *pComp2d);
GT_API GT_GetCompensate2D(short cardNum,short axis,TCompensate2D *pComp2d);
GT_API GT_GetCompensate2DValue(short cardNum,short axis,double *pValue);

//////////////////////////////////////////////////////////////////////////
//Smart Home
//////////////////////////////////////////////////////////////////////////
#define HOME_STAGE_IDLE						(0)
#define HOME_STAGE_START					(1)
#define HOME_STAGE_ON_HOME_LIMIT_ESCAPE		(2)

#define HOME_STAGE_SEARCH_LIMIT				(10)
#define HOME_STAGE_SEARCH_LIMIT_STOP		(11)

#define HOME_STAGE_SEARCH_LIMIT_ESCAPE		(13)

#define HOME_STAGE_SEARCH_LIMIT_RETURN		(15)
#define HOME_STAGE_SEARCH_LIMIT_RETURN_STOP	(16)

#define HOME_STAGE_SEARCH_HOME				(20)

#define HOME_STAGE_SEARCH_HOME_STOP			(22)

#define HOME_STAGE_SEARCH_HOME_RETURN		(25)

#define HOME_STAGE_SEARCH_INDEX				(30)

#define HOME_STAGE_SEARCH_GPI				(40)

#define HOME_STAGE_SEARCH_GPI_RETURN		(45)

#define HOME_STAGE_GO_HOME					(80)

#define HOME_STAGE_END						(100)

#define HOME_ERROR_NONE						(0)
#define HOME_ERROR_NOT_TRAP_MODE			(1)
#define HOME_ERROR_DISABLE					(2)
#define HOME_ERROR_ALARM					(3)
#define HOME_ERROR_STOP						(4)
#define HOME_ERROR_STAGE					(5)
#define HOME_ERROR_HOME_MODE				(6)
#define HOME_ERROR_SET_CAPTURE_HOME			(7)
#define HOME_ERROR_NO_HOME					(8)
#define HOME_ERROR_SET_CAPTURE_INDEX		(9)
#define HOME_ERROR_NO_INDEX					(10)

#define HOME_MODE_LIMIT						(10)
#define HOME_MODE_LIMIT_HOME				(11)
#define HOME_MODE_LIMIT_INDEX				(12)
#define HOME_MODE_LIMIT_HOME_INDEX			(13)

#define HOME_MODE_HOME						(20)

#define HOME_MODE_HOME_INDEX				(22)

#define HOME_MODE_INDEX						(30)

#define HOME_MODE_FORCED_HOME				(40)
#define HOME_MODE_FORCED_HOME_INDEX			(41)

typedef struct  
{
	short mode;						// 回零模式
	short moveDir;					// 设置启动搜索时的运动方向
	short indexDir;					// 设置Index搜索方向
	short edge;						// 设置捕获沿
	short triggerIndex;				// 用于设置触发器，为-1表示和轴号对应
	short pad1[3];					// 保留1
	double velHigh;					// Home搜索速度
	double velLow;					// Index搜索速度
	double acc;					
	double dec;
	short smoothTime;
	short pad2[3];					// 保留2
	long homeOffset;				// 原点偏移
	long searchHomeDistance;		// Home最大搜索距离，为0表示不限制
	long searchIndexDistance;		// Index最大搜索距离，为0表示不限制
	long escapeStep;				// 脱离步长
	long pad3[2];					// 保留3
} THomePrm;

typedef struct  
{
	short run;
	short stage;
	short error;
	short pad1;
	long capturePos;
	long targetPos;
} THomeStatus;

GT_API GT_GoHome(short cardNum,short axis,THomePrm *pHomePrm);
GT_API GT_GetHomePrm(short cardNum,short axis,THomePrm *pHomePrm);
GT_API GT_GetHomeStatus(short cardNum,short axis,THomeStatus *pHomeStatus);
//////////////////////////////////////////////////////////////////////////
//Extern Control
//////////////////////////////////////////////////////////////////////////
typedef struct ControlConfigEx
{
	short refType;			//输出参考类型，只能取MC_AXIS
	short refIndex;			//输出轴号
	short feedbackType;		//反馈类型，为MC_ENCODER时表示反馈类型为编码器，为MC_ADC时表示反馈类型为电压
	short feedbackIndex;	//反馈通道号
	long errorLimit;		//误差极限，暂时不起作用
	short feedbackSmooth;	//反馈平滑系数，暂时不起作用
	short controlSmooth;	//控制器平滑系数，暂时不起作用

}TControlConfigEx;

GT_API GT_SetControlConfigEx(short cardNum,short control,TControlConfigEx *pControl);
GT_API GT_GetControlConfigEx(short cardNum,short control,TControlConfigEx *pControl);
//////////////////////////////////////////////////////////////////////////
//Adc filter
//////////////////////////////////////////////////////////////////////////
typedef struct AdcConfig
{
	short active;	
	short reverse;
	double a;			//输入电压线性变换a值，取值为1
	double b;			//输入电压线性变换b值，取值为0
	short filterMode;	//滤波模式，0-FIR滤波模式，1-IIR滤波模式

} TAdcConfig;

GT_API GT_SetAdcConfig(short cardNum,short adc,TAdcConfig *pAdc);
GT_API GT_GetAdcConfig(short cardNum,short adc,TAdcConfig *pAdc);

GT_API GT_SetAdcFilterPrm(short cardNum,short adc,double k);
GT_API GT_GetAdcFilterPrm(short cardNum,short adc,double *pk);

//////////////////////////////////////////////////////////////////////////
//Superimposed
//////////////////////////////////////////////////////////////////////////
GT_API GT_SetControlSuperimposed(short cardNum,short control,short superimposedType,short superimposedIndex);
GT_API GT_GetControlSuperimposed(short cardNum,short control,short *pSuperimposedType,short *pSuperimposedIndex);

//////////////////////////////////////////////////////////////////////////
//Pci Interrupt
//////////////////////////////////////////////////////////////////////////
#define PCI_INTERRUPT_SOURCE_USER		(1)				//PCI中断源为用户自定义
#define PCI_INTERRUPT_SOURCE_2DCOMPARE	(2)				//PCI中断源为二维位置比较
typedef struct
{
	short source;	//interrupt source of pci device
	short param;	//param coming with pci interrupt
	short reserve;
}TInterrupt;

typedef short (*TInterruptCallback)(TInterrupt);

GT_API GT_InterruptOn(short cardNum,TInterruptCallback pCallback);
GT_API GT_InterruptOff(short cardNum);
GT_API GT_SetInterruptMask(short cardNum,long mask);
GT_API GT_GetInterruptMask(short cardNum,long *pMask);
GT_API GT_InterruptTrigger(short cardNum,short param);

//////////////////////////////////////////////////////////////////////////
GT_API GT_ZeroLaserOnTime(short cardNum,short channel);
GT_API GT_GetLaserOnTime(short cardNum,short channel,unsigned long *pTime);

typedef struct
{
	short work;			//是否已经设置参数
	short n;			//补偿点数
	long startPos;		//起始位置
	long lenPos;		//总长度
	long compPos[256];	//正向补偿值
	long compNeg[256];	//负向补偿值

}TLeadScrewPrm;

GT_API GT_GetLeadScrewCompInnerValue(short cardNum,short axis,short dir,long pos,long *pCompValue);

GT_API GT_SetProfileScale(short cardNum,short axis,long alpha,long beta);
GT_API GT_GetProfileScale(short cardNum,short axis,long *pAlpha,long *pBeta);
GT_API GT_SetEncoderScale(short cardNum,short encoder,long alpha,long beta);
GT_API GT_GetEncoderScale(short cardNum,short encoder,long *pAlpha,long *pBeta);

GT_API GT_MultiAxisOn(short cardNum,unsigned long mask);
GT_API GT_MultiAxisOff(short cardNum,unsigned long mask);
GT_API GT_SetAxisOnDelayTime(short cardNum,unsigned short ms);
GT_API GT_GetAxisOnDelayTime(short cardNum,unsigned short *pMs);

GT_API GT_SetLaserDisTable1D(short cardNum,short count,double *pRatio,long *pPos,double minPower,double maxPower,double *pLimitPower,short channel=0);
GT_API GT_SetLaserDisTable2D(short cardNum,short axisIndex[2],short count[2],double *pRatio,long *pXPos,long *pYPos, 
							 double minPower,double maxPower,double *pLimitPower,short channel=0);
GT_API GT_SetLaserDisTable2DEx(short cardNum,short axisIndex[2],short count[2],double *pRatio,long posBegin[2],long posStep[2], 
							   double minPower,double maxPower,double *pLimitPower,short channel=0);
GT_API GT_SetLaserCrdMap(short cardNum,short channel,short map);
GT_API GT_GetLaserCrdMap(short cardNum,short channel,short *pMap);

//////////////////////////////////////////////////////////////////////////
//AutoFocus
//////////////////////////////////////////////////////////////////////////
GT_API GT_AutoFocus(short cardNum,unsigned short mode,double kp,short reverse,short chanel);
GT_API GT_SetAutoFocusRefVol(short cardNum,double refVol,double maxVol,double minVol,short chanel);
GT_API GT_GetAutoFocusStatus(short cardNum,unsigned short *pStatus,short count);
GT_API GT_ConfigAutoFocus(short cardNum,short chnAdc,short chanel);
GT_API GT_SetAutoFocusAuxPrm(short cardNum,double kf,double kd,double limitKd,short chanel);
GT_API GT_SetAutoFocusDeadVoltage(short cardNum,double voltage,short chanel);

GT_API GT_Delay(short cardNum,unsigned short time);
GT_API GT_DelayHighPrecision(short cardNum,unsigned short time);

//////////////////////////////////////////////////////////////////////////
//Random Code
//////////////////////////////////////////////////////////////////////////
GT_API GT_SetRandCode(short cardNum,short addr,short *pData);
GT_API GT_GetRandCode(short cardNum,short addr,short *pData);

#define CRD_BUFFER_MODE_DYNAMIC_DEFAULT  (0)
#define CRD_BUFFER_MODE_DYNAMIC_KEEP     (1)

#define CRD_BUFFER_MODE_STATIC_INPUT     (11)
#define CRD_BUFFER_MODE_STATIC_READY     (12)
#define CRD_BUFFER_MODE_STATIC_START     (13)

GT_API GT_SetLeadScrewLink(short cardNum,short axis,short link);
GT_API GT_GetLeadScrewLink(short cardNum,short axis,short *pLink);

//////////////////////////////////////////////////////////////////////////
//Crd Follow
//////////////////////////////////////////////////////////////////////////
typedef struct  
{
	short crdAxis;
	short masterIndex;
	short masterType;
} TBufFollowMaster;

typedef struct  
{
	long masterPos;
	long pad;
} TBufFollowEventCross;

typedef struct  
{
	short triggerIndex;
	long triggerOffset;
	long pad;
} TBufFollowEventTrigger;


typedef struct  
{
	short stage;
	double slavePos;
	double slaveVel;
	unsigned long loopCount;
} TCrdFollowStatus;

typedef struct  
{
	double velRatioMax;
	double accRatioMax;
	long masterLead;
	long masterEven;
	long slaveEven;
	short dir;
	short smoothPercent;
	short synchAlign;
} TCrdFollowPrm;

GT_API GT_BufFollowMaster(short cardNum,short crd,TBufFollowMaster *pBufFollowMaster,short fifo=0);
GT_API GT_BufFollowEventCross(short cardNum,short crd,TBufFollowEventCross *pEventCross,short fifo=0);
GT_API GT_BufFollowEventTrigger(short cardNum,short crd,TBufFollowEventTrigger *pEventTrigger,short fifo=0);
GT_API GT_BufFollowStart(short cardNum,short crd,long masterSegment,long slaveSegment,long masterFrameWidth,short fifo=0);
GT_API GT_BufFollowReturn(short cardNum,short crd,double vel,double acc,short smoothPercent,short fifo=0);
GT_API GT_BufFollowNext(short cardNum,short crd,long width,short fifo=0);
GT_API GT_GetCrdFollowStatus(short cardNum,short crd,TCrdFollowStatus *pStatus);

GT_API GT_SetCrdFollowLoop(short cardNum,short crd,unsigned long loop);
GT_API GT_GetCrdFollowLoop(short cardNum,short crd,unsigned long *pLoop);
GT_API GT_SetCrdFollowPrm(short cardNum,short crd,TCrdFollowPrm *pPrm);
GT_API GT_GetCrdFollowPrm(short cardNum,short crd,TCrdFollowPrm *pPrm);

typedef struct  
{
	short source;
	short enable;
	short x;
	short y;
	double theta;		// degree
} TTransformOrthogonal;

GT_API GT_SetTransformOrthogonal(short cardNum,short index,TTransformOrthogonal *pOrthogonal);
GT_API GT_GetTransformOrthogonal(short cardNum,short index,TTransformOrthogonal *pOrthogonal);
GT_API GT_GetTransformOrthogonalPosition(short cardNum,short index,double *pPositionX,double *pPositionY);

typedef struct  
{
	short source;
	short enable;
	short x;
	short y;
	short z;
	double alpha;		// Z轴和XY平面的夹角
	double beta;		// Z轴在XY平面的投影和X轴的夹角
	double gama;		// XY轴之间的夹角
} TTransformPerpendicularity;

GT_API GT_SetTransformPerpendicularity(short cardNum,short index,TTransformPerpendicularity *pPerpendicularity);
GT_API GT_GetTransformPerpendicularity(short cardNum,short index,TTransformPerpendicularity *pPerpendicularity);
GT_API GT_GetTransformPerpendicularityPosition(short cardNum,short index,double *pPositionX,double *pPositionY,double *pPositionZ);

GT_API GT_SetLaserPwmDac(short cardNum,double pwmWidth,double frq,short *pDacValue,long count,short channel);
GT_API GT_LaserPwmDacOn(short cardNum,short channel);
GT_API GT_LaserPwmDacOff(short cardNum,short channel);


typedef struct
{
	short mode;
	short prm1;
	short prm2;
	short prm3;
	short prm4;
	double reserve1;
	double reserve2;
	double reserve3;
	double reserve4;

}TMotionSmooth;

GT_API GT_SetMotionSmooth(short cardNum,short axis,TMotionSmooth *pSmooth);
GT_API GT_GetMotionSmooth(short cardNum,short axis,TMotionSmooth *pSmooth);

typedef struct  
{
	short master[2];
	short masterValueSource[2];
	long gearRatioNumerator[2];
	long gearRatioDenominator[2];
} TCombineAxes;

typedef struct  
{
	short enable;
	double slavePos;
	double slaveVel;
} TCombineAxesStatus;

GT_API GT_SetCombineAxes(short cardNum,short index,TCombineAxes *pCombineAxes);
GT_API GT_GetCombineAxes(short cardNum,short index,TCombineAxes *pCombineAxes);
GT_API GT_CombineAxesOn(short cardNum,short index);
GT_API GT_CombineAxesOff(short cardNum,short index);
GT_API GT_GetCombineAxesStatus(short cardNum,short index,TCombineAxesStatus *pCombineAxesStatus);
GT_API GT_SetAxisAddition(short cardNum,short axis,short dataType,short additionIndex,short additionType);
GT_API GT_GetAxisAddition(short cardNum,short axis,short dataType,short *pAdditionIndex,short *pAdditionType);

GT_API GT_SetLeadScrewCrossComp(short cardNum,short axis,short n,long startPos,long lenPos,long *pCompPos,long *pCompNeg,short link);
GT_API GT_EnableLeadScrewCrossComp(short cardNum,short axis,short mode);

typedef enum 
{
	MC_POSITIVE_DIRECTION,
	MC_NEGATIVE_DIRECTION,
	MC_CURRENT_DIRECTION,
	MC_SHORTEST_WAY,
} EMcDirection;

typedef struct
{
	long pos;
	double vel;
	double acc;
	double dec;
	short percent;
} TMoveAbsolutePrm;

typedef struct
{
	long pos;
	double vel;
	double acc;
	double dec;
	short percent;
	double velStart;
	double velEnd;
	short accStartPercent;
	short decEndPercent;
} TMoveAbsolutePrmEx;

GT_API GT_MoveAbsolute(short cardNum,short profile,TMoveAbsolutePrm *pPrm);
GT_API GT_GetMoveAbsolute(short cardNum,short profile,TMoveAbsolutePrm *pPrm);
GT_API GT_MoveAbsoluteEx(short cardNum,short profile,TMoveAbsolutePrmEx *pPrm);
GT_API GT_GetMoveAbsoluteEx(short cardNum,short profile,TMoveAbsolutePrmEx *pPrm);

typedef struct
{
	double vel;
	double acc;
	double dec;
	double jerkBegin;
	double jerkEnd;
	short direction;
} TMoveVelocityPrm;

GT_API GT_MoveVelocity(short cardNum,short profile,TMoveVelocityPrm *pPrm);
GT_API GT_GetMoveVelocity(short cardNum,short profile,TMoveVelocityPrm *pPrm);

//////////////////////////////////////////////////////////////////////////
//Smart Cutter
//////////////////////////////////////////////////////////////////////////
typedef struct  
{
	short x;						// X轴对应的规划轴
	short y;						// Y轴对应的规划轴
	short c;						// C轴对应的规划轴

	short tableRadiusIndex;			// 半径补偿表索引
	short tableAngleIndex;			// C轴补偿表索引

	short directionReverse;			// 默认规划位置增大时角度也是增大的

	long offset;		            // C轴旋转角度为0时的规划位置
	long resolution;				// C轴每转脉冲数

	short adcIndex;					// ADC索引
	double adcThreshold;			// ADC触发补偿的阈值
} TSmartCutterPrm;

typedef struct  
{
	short enable;
	short execute;
	double radiusValue;
	double angleValue;
} TSmartCutterInfo;

GT_API GT_SetSmartCutterPrm(short cardNum,short index,TSmartCutterPrm *pPrm);
GT_API GT_GetSmartCutterPrm(short cardNum,short index,TSmartCutterPrm *pPrm);
GT_API GT_SmartCutterOn(short cardNum,short index);
GT_API GT_SmartCutterOff(short cardNum,short index);
GT_API GT_BufSmartCutterOn(short cardNum,short crd,short smartCutterIndex,short fifo=0);
GT_API GT_BufSmartCutterOff(short cardNum,short crd,short smartCutterIndex,short fifo=0);
GT_API GT_GetSmartCutterInfo(short cardNum,short index,TSmartCutterInfo *pInfo);

GT_API GT_SetSmartCutterValue(short cardNum,short index,double radiusValue,double angleValue);
GT_API GT_SmartCutterStart(short cardNum,short index);


//////////////////////////////////////////////////////////////////////////
//Gantry
//////////////////////////////////////////////////////////////////////////
#define GANTRY_MODE_NONE								(-1)
#define GANTRY_MODE_OPEN_LOOP_GANTRY					(1)
#define GANTRY_MODE_DECOUPLE_POSITION_LOOP				(2)

GT_API GT_SetGantryMode(short cardNum,short group,short master,short slave,short mode,long syncErrorLimit=1000);
GT_API GT_GetGantryMode(short cardNum,short group,short *pMaster,short *pSlave,short *pMode,long *pSyncErrorLimit);
GT_API GT_SetGantryPid(short cardNum,short group,TPid *pGantryPid,TPid *pYawPid);
GT_API GT_GetGantryPid(short cardNum,short group,TPid *pGantryPid,TPid *pYawPid);
GT_API GT_GantryAxisOn(short cardNum,short group);
GT_API GT_GantryAxisOff(short cardNum,short group);

GT_API GT_SetEncoderOutSelect(short cardNum,short mode);
GT_API GT_GetEncoderOutSelect(short cardNum,short *pMode);

//////////////////////////////////////////////////////////////////////////
//Standard Home
//////////////////////////////////////////////////////////////////////////

#define STANDARD_HOME_STAGE_IDLE  (0) //未启动回原点
#define STANDARD_HOME_STAGE_START (1) //启动回原点
#define STANDARD_HOME_STAGE_SEARCH_HOME (20) //正在搜索Home
#define STANDARD_HOME_STAGE_SEARCH_INDEX  (30) //正在搜索Index
#define STANDARD_HOME_STAGE_GO_HOME       (80) //正在运动到原点
#define STANDARD_HOME_STAGE_END           (100) //回原点结束
#define STANDARD_HOME_STAGE_START_CHECK (-1) //启动回原点前自检
#define STANDARD_HOME_STAGE_CHECKING (-2) //自检中


#define STANDARD_HOME_ERROR_NONE		(0) //未发生错误
#define STANDARD_HOME_ERROR_DISABLE	(10) //执行回原点的轴未使能
#define STANDARD_HOME_ERROR_ALARM		(20) //执行回原点的轴报警
#define STANDARD_HOME_ERROR_STOP		(30) //未完成回原点，被停止运动
#define STANDARD_HOME_ERROR_ON_LIMIT   (40) //触发了限位无法继续
#define STANDARD_HOME_ERROR_NO_HOME	(50) //未找到Home
#define STANDARD_HOME_ERROR_NO_INDEX  (60) //未找到Index
#define STANDARD_HOME_ERROR_NO_LIMIT (70) //未找到限位
#define STANDARD_HOME_ERROR_ENCODER_DIR_SCALE (-1) //规划器与编码器方向方向相反或者当量不一致


typedef struct  
{
	short mode;		 // 回原点模式取值范围1~36
	double highSpeed; // 搜索Home的速度，单位pulse/ms
	double lowSpeed;	 // 搜索Index的速度，单位pulse/ms
	double acc;		 // 回零加速度，单位pulse/ms^2
	long offset;       // 回零偏移量，单位pulse
	short check; // 是否启用自检功能，1-启用，其它值-不启用
	short autoZeroPos; // 回零完毕是否自动清零，1-自动清零，其它值-不清零
	long motorStopDelay; //电机到位延时，单位：控制周期
	short pad1[3];	 // 保留（不需要设置）
} TStandardHomePrm;

typedef struct  
{
	short run;     // 是正在进行回原点，0—已停止运动，1-正在回原点
	short stage;   // 回原点运动的阶段
	short error;    // 回原点过程的发生的错误
	short pad1[3];       // 保留（无具体含义）
	long capturePos;  // 捕获到Home或Index时刻的编码器位置
	long targetPos;    // 需要运动到的目标位置（原点位置或者原点位置+偏移量），在搜索Limit时或者搜索Home或Index时，设置的搜索距离为0，那么该值显示为805306368
} TStandardHomeStatus;

GT_API GT_ExecuteStandardHome(short cardNum,short axis,TStandardHomePrm *pHomePrm);
GT_API GT_GetStandardHomePrm(short cardNum,short axis,TStandardHomePrm *pHomePrm);
GT_API GT_GetStandardHomeStatus(short cardNum,short axis,TStandardHomeStatus *pHomeStatus);

typedef struct  
{
	short year;
	short month;
	short day;
	short version;
	short user;
	short reserve1;
	short reserve2;
	short chip;
} TVersion;

GT_API GT_GetVersionEx(short cardNum,short type,TVersion *pVersion);

GT_API GT_CrdHsOn(short cardNum,short crd,short fifo,short link=1,unsigned short threshold=200,short lookAheadInMc=0);
GT_API GT_CrdHsOff(short cardNum,short crd,short fifo);

#define AUTO_HOME_MODE_HOME (1)
#define AUTO_HOME_MODE_HOME_INDEX (2)
#define AUTO_HOME_MODE_HOME_STOP (3)

typedef struct
{
	short mode;
	double homeSearchVel;
	double acc;
	long homeSearchDis;
	long indexSearchDis;
	long homeReturnOffset;
	long indexReturnOffest;
}TAutoHomePrm;

typedef struct
{
	short run;
	short stage;
	short result;
	long capturePos;
	long targetPos;
}TAutoHomeStatus;

GT_API GT_CheckPciCommandFlag(short cardNum,short *pFlag);
GT_API GT_ResetPciCommandFlag(short cardNum);

GT_API GT_SetStepOutLevel(short cardNum,short axis,short type,short reverse);
GT_API GT_GetStepOutLevel(short cardNum,short axis,short type,short *pReverse);

//////////////////////////////////////////////////////////////////////////
//兼容GSN位置比较指令的位置比较输出
//////////////////////////////////////////////////////////////////////////
typedef struct
{
	short mode;                       // FIFO模式；Linear模式
	short dimension;                  // 1D,2D
	short sourceMode;                 // 编码器、脉冲计数器
	short sourceX;                    // X轴比较源
	short sourceY;                    // Y轴比较源
	short outputMode;                 // 输出模式：脉冲、电平
	short outputCounter;              // 输出脉冲计数源
	unsigned short outputPulseWidth;  // 输出脉冲宽度，电平模式无效
	unsigned short errorBand;         // 二维位置比较输出误差带
} TPosCompareMode;


typedef struct
{
	short mode;                       // 0:时间模式，1：脉冲模式
	unsigned short count;             // 输出个数
	short highLevel;//脉冲模式下，无效电平持续时间，电平模式下指高电平持续时间
	short lowLevel;//电平模式下，低电平持续时间，脉冲模式下无效
	short resver[20];//保留
} TPosCompareContinueMode;

typedef struct
{
	unsigned long count;
	unsigned short hso;
	unsigned short gpo;

	long startPos;
	long interval;
} TPosCompareLinear;

typedef struct
{
	unsigned long count;
	unsigned short hso;
	unsigned short gpo;

	long startPosX;
	long startPosY;
	long intervalX;
	long intervalY;
} TPosCompareLinear2D;

typedef struct
{
	unsigned long count;
	unsigned short hso;
	unsigned short gpo;
	long startPosX;
	long startPosY;
	long syncPos;
	long time;
	short reserve[20];
} TPosComparePsoPrm;

typedef struct
{
	long pos;
	unsigned short hso;
	unsigned short gpo;
	unsigned long segmentNumber;
} TPosCompareData;

typedef struct
{
	long posX;
	long posY;
	unsigned short hso;
	unsigned short gpo;
	unsigned long segmentNumber;
} TPosCompareData2D;

typedef struct
{
	unsigned short mode;      // 0:FIFO模式；1：Linear模式
	unsigned short run;
	unsigned short space;
	unsigned long pulseCount;
	unsigned short hso;
	unsigned short gpo;
	unsigned long segmentNumber;
} TPosCompareStatus;

typedef struct
{
	unsigned short config;
	unsigned short fifoEmpty;
	unsigned short head;
	unsigned short tail;
	unsigned long commandReceive;
	unsigned long commandSend;
	long posX;
	long posY;
} TPosCompareInfo;


typedef struct
{
	unsigned short config;
	unsigned short fifoEmpty;
	unsigned short head;
	unsigned short tail;
	unsigned long commandReceive;
	unsigned long commandSend;
	long posX;
	long posY;
	short sourceReverseX;
	short sourceReverseY;
	short reserve[12];
} TPosCompareInfoEx;

GT_API GT_SetPosCompareMode(short cardNum,short index,TPosCompareMode *pMode);
GT_API GT_GetPosCompareMode(short cardNum,short index,TPosCompareMode *pMode);
GT_API GT_PosCompareStart(short cardNum,short index);
GT_API GT_PosCompareStop(short cardNum,short index);
GT_API GT_PosCompareClear(short cardNum,short index);
GT_API GT_PosCompareStatus(short cardNum,short index,TPosCompareStatus *pStatus);
GT_API GT_PosCompareData(short cardNum,short index,TPosCompareData *pData);
GT_API GT_PosCompareData2D(short cardNum,short index,TPosCompareData2D *pData);
GT_API GT_SetPosCompareLinear(short cardNum,short index,TPosCompareLinear *pLinear);
GT_API GT_GetPosCompareLinear(short cardNum,short index,TPosCompareLinear *pLinear);
GT_API GT_SetPosCompareLinear2D(short cardNum,short index,TPosCompareLinear2D *pLinear);
GT_API GT_GetPosCompareLinear2D(short cardNum,short index,TPosCompareLinear2D *pLinear);
GT_API GT_SetPosComparePsoPrm(short cardNum,short index,TPosComparePsoPrm *pPrm);
GT_API GT_GetPosComparePsoPrm(short cardNum,short index,TPosComparePsoPrm *pPrm);
GT_API GT_SetPosCompareContinueMode(short cardNum,short index,TPosCompareContinueMode *pMode);
GT_API GT_GetPosCompareContinueMode(short cardNum,short index,TPosCompareContinueMode *pMode);
GT_API GT_PosCompareInfo(short cardNum,short index,TPosCompareInfo *pInfo);
GT_API GT_PosCompareInfoEx(short cardNum,short index,TPosCompareInfoEx *pInfo);
GT_API GT_PosCompareSpace(short cardNum,short index,unsigned short *pSpace);
GT_API GT_SetPosCompareStartLevel(short cardNum,short index,short type,short startLevel);
GT_API GT_PosComparePulse(short cardNum,short index,short outputMode,short level,unsigned short outputPulseWidth);
GT_API GT_PosComapreContinuePusleMode(short cardNum,short index,short mode,short count,unsigned short pulseLowerWidth);
GT_API GT_BufPosCompareStart(short cardNum,short crd,short fifo,short index);
GT_API GT_BufPosCompareStop(short cardNum,short crd,short fifo,short index);
GT_API GT_BufPosComparePsoPrm(short cardNum,short crd,short index,TPosComparePsoPrm *pPrm,short fifo);
GT_API GT_BufSetComparePort(short cardNum,short crd,short fifo,short channel,short hsio0,short hsio1);

GT_API GT_SelectLaserDisTable(short cardNum,short channel,short index);
GT_API GT_GetCurrentLaserDisTable(short cardNum,short channel,short *pIndex);
GT_API GT_BufSetLaserDisEnable(short cardNum,short crd,short fifo,short channel,short enable);
GT_API GT_BufSelectLaserDisTable(short cardNum,short crd,short fifo,short channel,short index);

GT_API GT_SetFlagVar(short cardNum,short index,short mode,short value);
GT_API GT_GetFlagVar(short cardNum,short index,short *pMode,short *pValue);


//////////////////////////////////////////////////////////////////////////
//手摇杆——兆丰
//////////////////////////////////////////////////////////////////////////
#define  MC_JOYSTICK_AXIS_COUNT_MAX 3//摇杆最大轴数
#define  MC_JOYSTICK_VEL_COUNT_MAX 3//速度最大阶数

typedef struct
{
	long scale[MC_JOYSTICK_AXIS_COUNT_MAX];//脉冲当量：pulse/mm
	double motionAcc[MC_JOYSTICK_AXIS_COUNT_MAX];//加速度： mm/s^2
	double motionDec[MC_JOYSTICK_AXIS_COUNT_MAX];//减速度： mm/s^2
	double motionSmooth[MC_JOYSTICK_AXIS_COUNT_MAX];//平滑系数： 0——1
}TJoystickMoitonPrm;

typedef struct
{
	double xMotionVel[MC_JOYSTICK_VEL_COUNT_MAX];//X速度：mm/s
	double yMotionVel[MC_JOYSTICK_VEL_COUNT_MAX];//Y速度：mm/s
	double zMotionVel[MC_JOYSTICK_VEL_COUNT_MAX];//Z速度：mm/s
}TJoystickMoitonVel;


typedef struct
{
	long thresholdValue[MC_JOYSTICK_AXIS_COUNT_MAX];//摇杆阈值
	long joystickMaxValue[MC_JOYSTICK_AXIS_COUNT_MAX];//最大值
	long joystickMedianValue[MC_JOYSTICK_AXIS_COUNT_MAX];//中间值
	long joystickMinValue[MC_JOYSTICK_AXIS_COUNT_MAX];//最小值
}TJoystickCriticalValue;

typedef struct
{
	long baudRate;
	short byteSize;
	short parity;
	short stopBits;
	short reserve;
}TSericalPortPrm;

GT_API GT_SetJoystickMapAxis(short cardNum,short joystickIndex,short *axis);
GT_API GT_GetJoystickMapAxis(short cardNum,short joystickIndex,short *pAxis);

GT_API GT_JoystickEnable(short cardNum,short joystickIndex,short enable);//mode  0:禁用，1：启用

GT_API GT_SetJoystickSericalPortPrm(short cardNum,short joystickIndex,TSericalPortPrm *sericalPortPrm);
GT_API GT_GetJoystickSericalPortPrm(short cardNum,short joystickIndex,TSericalPortPrm *pSericalPortPrm);

GT_API GT_SetJoystickSendDataPacket(short cardNum,short joystickIndex,short *dataPacket,short dataCount);
GT_API GT_GetJoystickSendDataPacket(short cardNum,short joystickIndex,short *pDataPacket,short *pDataCount);

GT_API GT_JoystickCheckEnable(short cardNum,short joystickIndex,short checkEnable);//checkEnable  0:使能校验，1：校验关闭
GT_API GT_GetJoystickSts(short cardNum,short joystickIndex,long *pSts);//pSts  bit0:摇杆状态（0:禁用，1：启用），bit1：摇杆校验状态（ 0:使能校验，1：校验关闭）
GT_API GT_GetJoystickValue(short cardNum,short joystickIndex,long *pValue);
GT_API GT_SetJoystickMotionPrm(short cardNum,short joystickIndex,TJoystickMoitonPrm *joystickMotionPrm);
GT_API GT_GetJoystickMotionPrm(short cardNum,short joystickIndex,TJoystickMoitonPrm *pJoystickMotionPrm);
GT_API GT_SetJoystickMotionVel(short cardNum,short joystickIndex,TJoystickMoitonVel *joystickMotionVel);
GT_API GT_GetJoystickMotionVel(short cardNum,short joystickIndex,TJoystickMoitonVel *joystickMotionVel);
GT_API GT_SetJoystickCriticalValue(short cardNum,short joystickIndex,TJoystickCriticalValue *joystickCriticalValue);
GT_API GT_GetJoystickCriticalValue(short cardNum,short joystickIndex,TJoystickCriticalValue *pJoystickCriticalValue);

GT_API GT_JoystickReset(short cardNum,short joystickIndex);


GT_API GT_SetDisplayDip(short cardNum,short mode);
GT_API GT_GetDisplayDip(short cardNum,short *pMode);

GT_API GT_Buf2DCompareData(short cardNum,short crd,short fifo,short chn,T2DCompareData data,short compareFifo);
GT_API GT_2DCompareSetHoldTime(short cardNum,short chn,short time);
GT_API GT_2DCompareGetHoldTimeStatus(short cardNum,short chn,short *pTime,short *pSts);

GT_API GT_SetLaserFollowTable(short cardNum,short tableId,long n,double *pVel,double *pPower,short channel);
GT_API GT_GetLaserFollowTable(short cardNum,short tableId,long n,double *pVel,double *pPower,long *pCount,short channel);
GT_API GT_BufLaserFollowTable(short cardNum,short crd,short tableId,double minPower,double maxPower,short fifo=0,short channel=0);

GT_API GT_BufStop(short cardNum,short crd,long mask,long option,short fifo=0);
GT_API GT_BufMoveJog(short cardNum,short crd,short moveAxis,double vel,double acc,short modal,short fifo=0);

GT_API GT_GetCardInfo(short *pCardNum,short *pCardType);

GT_API GT_SetDeviceShareMax(short cardNum,short count);

GT_API GT_Buf2DComparePulse(short cardNum,short crd,short fifo,short chn,short level,short outputType,short time);

GT_API GT_CrdEnableVariableCircle(short cardNum,short crd,short fifo);
GT_API GT_CrdDisableVariableCircle(short cardNum,short crd,short fifo);

//////////////////////////////////////////////////////////////////////////
//MovePosPlc
//////////////////////////////////////////////////////////////////////////
typedef struct  
{
	long pos1;
	long pos2;
	double vel1;
	double vel2;
	double acc;
	double dec;
	short direction;
	short overrideSelect;
	short mode;
	short percent;
}TMovePosTwoSegmentParameterPlc;

//////////////////////////////////////////////////////////////////////////
//椭圆插补
//////////////////////////////////////////////////////////////////////////
#define ELLIPSE_AUX_POINT_COUNT                 (5)

#define ELLIPSE_MODE_AUX_POINT                  (0)
#define ELLIPSE_MODE_STANDARD                   (1)

typedef struct
{
	double pos[ELLIPSE_AUX_POINT_COUNT][8];    // 椭圆上辅助点坐标
} TEllipseAuxPoint;

typedef struct  
{
	double centerPoint[8];                     // 椭圆圆心坐标
	double theta;                              // 椭圆旋转角度
	double a;                                  // 椭圆长轴
	double b;                                  // 椭圆短轴
}TEllipseStandard;

typedef union
{
	TEllipseAuxPoint auxPoint;                 // 辅助点模式参数
	TEllipseStandard standard;                 // 标准模式参数
	double reserve[60];
} TEllipseParameterUnion;

typedef struct  
{
	double endPoint[8];                        // 终点坐标
	short plane;		                       // 椭圆平面选择，0：XY；1：YZ；2：ZX
	short dir;			                       // 椭圆方向，0：顺时针；1：逆时针
	short overrideSelect;	                   // 速度倍率选择，0：第1组倍率；1：第2组倍率
	short mode;			                       // 椭圆模式，目前支持：0-辅助点模式，1-标准模式

	TEllipseParameterUnion data;	           // 保留参数
} TEllipseParameter;

GT_API GT_Ellipse(short cardNum,short crd,TEllipseParameter *pEllipse,double synVel,double synAcc,double velEnd,long segNum,short fifo);
GT_API GT_EllipseEx(short cardNum,short crd,TEllipseParameter *pEllipse,double synVel,double synAcc,double velEnd,long segNum,short fifo);


#define ELLIPSE_MODE_AUX_POINT_2D               (0)
#define ELLIPSE_MODE_STANDARD_2D                (1)
//-------------------------------------------------------
//功能说明：椭圆插补描述参数,模式：ELLIPSE_MODE_AU_POINT_2D
//plane--------------椭圆平面选择，INTERPOLATION_CIRCLE_PLAT_XY(0)：XY；INTERPOLATION_CIRCLE_PLAT_YZ(1)：YZ；INTERPOLATION_CIRCLE_PLAT_ZX(2)：ZX
//dir----------------椭圆方向，0：顺时针；1：逆时针
//overrideSelect-----速度倍率选择，0：第1组倍率；1：第2组倍率
//pad----------------占位变量，不需要传入
//endPoint1----------终点坐标1,意义根据plane来定，如果palne为XY平面，则endPoint1、pos1为X坐标，endPoint2、pos2为Y坐标
//endPoint2----------终点坐标2
//pos1---------------椭圆上辅助点坐标1
//pos2---------------椭圆上辅助点坐标2
//-------------------------------------------------------
typedef struct  
{
	short plane;		                     
	short dir;			                     
	short overrideSelect;	                 
	short pad;		                         

	double endPoint1;
	double endPoint2;

	double pos1[ELLIPSE_AUX_POINT_COUNT];    
	double pos2[ELLIPSE_AUX_POINT_COUNT];    
} TEllipseAuxPoint2D;

//-------------------------------------------------------
//功能说明：椭圆插补描述参数,模式：ELLIPSE_MODE_STANDARD_2D
//plane--------------椭圆平面选择，INTERPOLATION_CIRCLE_PLAT_XY(0)：XY；INTERPOLATION_CIRCLE_PLAT_YZ(1)：YZ；INTERPOLATION_CIRCLE_PLAT_ZX(2)：ZX
//dir----------------椭圆方向，0：顺时针；1：逆时针
//overrideSelect-----速度倍率选择，0：第1组倍率；1：第2组倍率
//pad----------------占位变量，不需要传入
//endPoint1----------终点坐标1,意义根据plane来定，如果palne为XY平面，则endPoint1为X坐标，endPoint2为Y坐标
//endPoint2----------终点坐标2
//centerPoint1-------椭圆圆心坐标1，意义根据plane来定，如果palne为XY平面，则centerPoint1为X坐标，centerPoint2为Y坐标
//centerPoint2-------椭圆圆心坐标2
//theta--------------椭圆旋转角度，单位：度
//a------------------椭圆长轴
//b------------------椭圆短轴，短轴必须比长轴短
//-------------------------------------------------------
typedef struct
{
	short plane;		                     
	short dir;			                     
	short overrideSelect;	                 
	short pad;		                         

	double endPoint1;
	double endPoint2;

	double centerPoint1;
	double centerPoint2;

	double theta;
	double a;
	double b;
}TEllipseStandard2D;

GT_API GT_EllipsePro(short cardNum,short crd,short mode,void *pData,double synVel,double synAcc,double velEnd,long segNum,short fifo);
GT_API GT_EllipseProEx(short cardNum,short crd,short mode,void *pData,double synVel,double synAcc,double velEnd,long segNum,short fifo);

//////////////////////////////////////////////////////////////////////////
//计算插补时间
//////////////////////////////////////////////////////////////////////////

GT_API GT_CheckCrdLineTime(short cardNum,short crd,double x,double y,double z,double *pVelMax,double *pAcc,double *pVelEnd,double *pTime,long *pErrorCode);

//////////////////////////////////////////////////////////////////////////
//五轴PSO
//////////////////////////////////////////////////////////////////////////
typedef struct
{
	short mode;						// 0：FIFO模式，1：Linear模式 2：等间距输出模式 3:等间距精确到位模式
	short dimension;					// 五轴模式选择5维
	short sourceMode;					// 0：编码器； 1：脉冲计数器
	short source[8];					//编码器比较源
	short outputMode;					/*输出模式：0:脉冲 1:电平2：电平自
										动翻转(电平模式并且输出电平自动反转，不受posCompareData电平影响)*/
	short outputCounter;				// 保留
	unsigned short outputPulseWidth;		/*输出脉冲宽度,单位为1us，电平模式该参数无效*/
	unsigned short errorBand;			// 二维位置比较输出误差带
	short reserve1[2];
	double reserve2[16];
} TPosCompareModeEx;

GT_API GT_SetPosCompareModeEx(short cardNum,short index,TPosCompareModeEx *pMode);
GT_API GT_GetPosCompareModeEx(short cardNum,short index,TPosCompareModeEx *pMode);

typedef struct
{
	short probeType;			// 捕获类型
	short probeIndex;			// 捕获索引
	short latchType;			// 锁存类型
	short latchIndex;			// 锁存索引

	short sense;				// 捕获电平
	short pad[3];
	double filterWidth;			// 捕获最小宽度
}TSignalDetect;

typedef struct
{
	short status;				// 完成标志,0，未触发，1计数中，2计数完成
	short pad[3];
	double latchWidth;			// 锁存脉宽值
}TSignalDetectStatus;

GT_API GT_SetSignalTimeFilter(short cardNum,short type,short index,double filterWidth);
GT_API GT_GetSignalTimeFilter(short cardNum,short type,short index,double *pFilterWidth);

GT_API GT_ClearSignalDetect(short cardNum,short index);
GT_API GT_SetSignalDetect(short cardNum,short index,short enable,TSignalDetect *pPrm);
GT_API GT_GetSignalDetect(short cardNum,short index,short *pEnable,TSignalDetect *pPrm);
GT_API GT_GetSignalDetectStatus(short cardNum,short index,TSignalDetectStatus *pPrm);