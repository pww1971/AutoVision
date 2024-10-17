//用户使用的头文件
#pragma once

#define LA_AXIS_NUM				8
#define LA_WORK_AXIS_NUM		6
#define LA_MACHINE_AXIS_NUM   5

#define AXIS_LIMIT_NONE       0       //轴无限制
#define AXIS_LIMIT_MAX_VEL    1       //轴最大速度限制
#define AXIS_LIMIT_MAX_ACC    2       //轴最大加速度限制
#define AXIS_LIMIT_MAX_DV     4       //轴最大速度跳变量限制

#define KIN_MSG_BUFFER_SIZE   32

//工件坐标系下轨迹是否限制速度模式
enum EWorkLimitMode
{
	WORK_LIMIT_INVALID=0,		//不限制
	WORK_LIMIT_VALID,			//限制生效
};

//设置的速度定义规则
enum EVelSettingDef
{
	NORMAL_DEF_VEL=0,			//输入为轴坐标系所有轴的合成速度
	NUM_DEF_VEL,				//以NUM系统的规则定义
	CUT_DEF_VEL,				//速度为切削速度
};

//设置的加速度定义规则
enum EAccSettingDef
{
	NORMAL_DEF_ACC=0,             //输入即输出
	LONG_AXIS_ACC,                //长轴最大速度
};

//机床类型
enum EMachineMode
{
	NORMAL_THREE_AXIS=0,		//标准三轴机床模式
	MULTI_AXES,					//多轴联动模式
	FIVE_AXIS,					//五轴机床模式，轴坐标系为主，工件坐标系为辅
	FIVE_AXIS_WORK,				//五轴机床模式，工件坐标系为主，轴坐标系为辅
	ROBOT,
};

//前瞻参数结构体
struct TLookAheadParameter
{
	int lookAheadNum;					//前瞻段数
	double time;						//时间常数
	double radiusRatio;					//曲率限制调节参数
	double vMax[LA_AXIS_NUM];			//各轴的最大速度
	double aMax[LA_AXIS_NUM];			//各轴的最大加速度
	double DVMax[LA_AXIS_NUM];			//各轴的最大速度变化量（在时间常数内）
	double scale[LA_AXIS_NUM];			//各轴的脉冲当量
	short axisRelation[LA_AXIS_NUM];	//输入坐标和内部坐标的对应关系
	char machineCfgFileName[128];		//机床配置文件名
};

//速度规划模式
enum EVelMode
{
	T_CURVE=0,
	S_CURVE,
	S_CURVE_NEW,                  //根据加加速度、最大加速度进行S曲线速度前瞻，2015.11.16

	VEL_MODE_MAX=0x10000,         //确保长度为4Byte
};

//////////////////////////////////////
typedef struct
{
	short RobotType;
	short reserved1;

	short KinParUse[18];
	double KinPar[18];
	short KinLimitUse[12];
	double KinLimitMin[12];
	double KinLimitMax[12];
	double KinLimitMinShift[12];
	double KinLimitMaxShift[12];

	short AxisUse[8];
	char AxisPosSignSwitch[8];
	double AxisPosOffset[8];

	short CartUnitUse[6];
	char CartPosKCSSignSwitch[6];
	short reserved2[3];
	double CartPosKCSOffset[6];
}RC_KIN_CONFIG;

typedef struct
{
	char Error;
	short ErrorID;
	char Message[129];
}RC_ERROR_INTERFACE;

typedef struct  
{
	short ErrorID;
	char Message[129];
	char LogTime[32];
	long InternalID;
}RC_MSG_BUFFER_ELEMENT;

typedef struct  
{
	short LastMsgIndex;
	RC_MSG_BUFFER_ELEMENT MsgElement[KIN_MSG_BUFFER_SIZE];
	long LastMsgID;
}RC_MSG_BUFFER;

//旋转轴范围设置
struct TRotationAxisRange
{
	int primaryAxisRangeOn;              //第一旋转轴限定范围是否生效，0：不生效，1：生效
	int slaveAxisRangeOn;                //第二旋转轴限定范围是否生效，0：不生效，1：生效
	double maxPrimaryAngle;              //第一旋转轴最大值
	double minPrimaryAngle;              //第一旋转轴最小值
	double maxSlaveAngle;                //第二旋转轴最大值
	double minSlaveAgnle;                //第二旋转轴最小值
};

//选解参数
enum EGroupSelect
{
	Continuous=0,
	Group_1,
	Group_2,
};

struct TPos
{
	double machinePos[LA_MACHINE_AXIS_NUM];
	double workPos[LA_WORK_AXIS_NUM];
};


//////////////////////////////////
typedef enum{OPT_OFF, OPT_ON}OptimizeState;
typedef enum{NO_OPT, OPT_BLENDING, OPT_CIRCLEFITTING, OPT_CUBICSPLINE, OPT_BSPLINE}OptimizeMethod;

typedef enum{INIT_ERROR=1,		//没有进行参数初始化
			PASSWORD_ERROR,		//密码错误，请在固高运动控制平台上运行
			INDATA_ERROR,		//输入数据错误（检查圆弧数据是否正确）
			PRE_PROCESS_ERROR,	//
			TOOL_RADIUS_COMPENSATE_ERROR_INOUT,		//刀具半径补偿错误：进入/结束刀补处不能是圆弧
			TOOL_RADIUS_COMPENSATE_ERROR_NOCROSS,	//刀具半径补偿错误：数据不合理，无法计算交点
			USERDATA_ERROR,
}ErrorID;

//轨迹优化参数结构体
typedef struct OptimizeParamUser 
{
	OptimizeState usePathOptimize;	//是否使用路径优化：OPT_OFF:不使用	OPT_ON:使用

	float tolerance;				//公差(suggest: rough:0.1, pre-finish:0.05, finish:0.01)

	OptimizeMethod optimizeMethod;	//选择曲线优化方式

	OptimizeState keepLargeArc;		//是否保留大圆弧：OPT_OFF：不保留， OPT_ON：保留

	float blendingMinError;			//blending的最小设定误差

	float blendingMaxAngle;			//blending的最大角度限制（即当线段向量角度大于该角度时，不做blending，单位：度）

}TOptimizeParamUser;

struct TErrorInfo 
{
	ErrorID errorID;		//错误号(INIT_ERROR:未初始化参数；PRE_PROCESS_ERROR:预处理模块错误；
							//TOOL_RADIUS_COMPENSATE_ERROR:刀具半径补偿错误；)
	long errorRowNum;		//错误行号
};

struct TPreStartPos 
{
 double Pos[LA_AXIS_NUM];
};

typedef struct  
{
	unsigned short doType;
	unsigned short doMask;
	unsigned short doValue;
	unsigned short delayTime;
	short fifo;
}TBufIoDelayData;

typedef struct  
{
	short doType;
	short doIndex;
	unsigned short highLevelTime;
	unsigned short lowLevelTime;
	long pulseNum;
	short firstLevel;
	short fifo;
}TBufDoBitPulseData;

typedef struct  
{
	short channel;
	short daValue;
	short fifo;
}TBufDaData;

typedef struct  
{
	short channel;
	short fifo;
	short source;
	double laserPower;
	double ratio;
	double minPower;
	double maxPower;
	short tableId;
}TBufLaserData;

typedef struct  
{
	short axis;
	double deltaPos;
	short fifo;
	short smoothFlag;
	short accPercent;
	short decPercent;
}TBufGearData;

typedef struct  
{
	short axis;
	double pos;
	double vel;
	double acc;
	short modal;
	short fifo;
}TBufMoveData;

typedef struct
{
	long segNum;
	short fifo;
}TBufSegNumData;

typedef struct  
{
	short crdAxis;
	short masterIndex;
	short masterType;
	short fifo;
} TBufFollowMasterData;

typedef struct  
{
	long masterPos;
	long pad;
	short fifo;
} TBufFollowEventCrossData;

typedef struct  
{
	short triggerIndex;
	long triggerOffset;
	long pad;
	short fifo;
} TBufFollowEventTriggerData;

typedef struct
{
	long masterSegment;
	long slaveSegment;
	long masterFrameWidth;
	short fifo;
}TBufFollowStartData;

typedef struct
{
	long width;
	short fifo;

}TBufFollowNextData;

typedef struct
{
	double vel;
	double acc;
	short smoothPercent;
	short fifo;
}TBufFollowReturnData;

typedef struct  
{
	short index;
	short enable;
	short fifo;
}TBufSmartCutterData;

typedef struct
{
	short eventIndex;
	short count;
	short fifo;
}TBufEventData;

//Look ahead Ex
GT_API GT_SetupLookAheadCrd(short cardNum,short crd,EMachineMode machineMode);
GT_API GT_SetFollowAxisParaLa(short cardNum,short crd, int *pAxisLimitMode, double *pVmax, double *pAmax, double *pDVmax);
GT_API GT_SetVelDefineModeLa(short cardNum,short crd,EVelSettingDef velDefMode);
GT_API GT_SetAxisLimitModeLa(short cardNum,short crd,int *pAxisLimitMode);
GT_API GT_SetWorkLimitModeLa(short cardNum,short crd,EWorkLimitMode workLimitMode);
GT_API GT_SetAxisVelValidModeLa(short cardNum,short crd,int velValidAxis);
GT_API GT_SetVelSmoothMode(short cardNum,int crd,int smoothMode);
GT_API GT_InitLookAheadEx(short cardNum,short crd,TLookAheadParameter *pLookAheadPara,short fifo=0, short motionMode =0,TPreStartPos *pPreStartPos =NULL);
GT_API GT_PrintLACmdLa(short cardNum,short crd,int printFlag, int clearFile);
GT_API GT_InitLookAheadPara(short cardNum,short crd,long lookAheadNum,double time,double radiusRatio,double scale,short fifo=0);
GT_API GT_SetAxisFollowModeLa(short cardNum,short crd,int *pFollowMode);
GT_API GT_SetBlendingParaLa(short cardNum,short crd,short paraType,double para,double minAngle,double maxAngle);

GT_API GT_LnXYEx(short cardNum,short crd,double x,double y,double synVel,double synAcc,long segNum,short override2,short fifo=0);
GT_API GT_LnXYG0Ex(short cardNum,short crd,double x,double y,double synVel,double synAcc,long segNum,short override2,short fifo=0);

GT_API GT_LnXYZEx(short cardNum,short crd,double x,double y,double z,double synVel,double synAcc,long segNum,short override2,short fifo=0);
GT_API GT_LnXYZG0Ex(short cardNum,short crd,double x,double y,double z,double synVel,double synAcc,long segNum,short override2,short fifo=0);

GT_API GT_LnXYZAEx(short cardNum,short crd,double x,double y,double z,double a,double synVel,double synAcc,long segNum,short override2,short fifo=0);
GT_API GT_LnXYZAG0Ex(short cardNum,short crd,double x,double y,double z,double a,double synVel,double synAcc,long segNum,short override2,short fifo=0);

GT_API GT_LnXYZACEx(short cardNum,short crd,double *pPos,short posMask,double synVel,double synAcc,long segNum,short override2,short fifo);
GT_API GT_LnXYZACG0Ex(short cardNum,short crd,double *pPos,short posMask,double synVel,double synAcc,long segNum,short override2,short fifo);

GT_API GT_LnXYZACUVWEx(short cardNum,short crd,double *pPos,short posMask,double synVel,double synAcc,long segNum,short override2,short fifo);
GT_API GT_LnXYZACUVWG0Ex(short cardNum,short crd,double *pPos,short posMask,double synVel,double synAcc,long segNum,short override2,short fifo);

GT_API GT_ArcXYREx(short cardNum,short crd,double x,double y,double radius,short circleDir,double synVel,double synAcc,long segNum,short override2,short fifo=0);
GT_API GT_ArcYZREx(short cardNum,short crd,double y,double z,double radius,short circleDir,double synVel,double synAcc,long segNum,short override2,short fifo=0);
GT_API GT_ArcZXREx(short cardNum,short crd,double z,double x,double radius,short circleDir,double synVel,double synAcc,long segNum,short override2,short fifo=0);

GT_API GT_ArcXYCEx(short cardNum,short crd,double x,double y,double xCenter,double yCenter,short circleDir,double synVel,double synAcc,long segNum,short override2,short fifo=0);
GT_API GT_ArcYZCEx(short cardNum,short crd,double y,double z,double yCenter,double zCenter,short circleDir,double synVel,double synAcc,long segNum,short override2,short fifo=0);
GT_API GT_ArcZXCEx(short cardNum,short crd,double z,double x,double zCenter,double xCenter,short circleDir,double synVel,double synAcc,long segNum,short override2,short fifo=0);

GT_API GT_ArcXYZEx(short cardNum,short crd,double x,double y,double z,double interX,double interY,double interZ,double synVel,double synAcc,long segNum,short override2,short fifo=0);

GT_API GT_HelixXYRZEx(short cardNum,short crd,double x,double y,double z,double radius,short circleDir,double synVel,double synAcc,long segNum,short override2,short fifo=0);
GT_API GT_HelixXYCZEx(short cardNum,short crd,double x,double y,double z,double xCenter,double yCenter,short circleDir,double synVel,double synAcc,long segNum,short override2,short fifo=0);

GT_API GT_BufDelayEx(short cardNum,short crd,unsigned short delayTime,short fifo=0);
GT_API GT_BufGearEx(short cardNum,short crd,short gearAxis,double deltaPos,short fifo=0);
GT_API GT_BufGearPercentEx(short cardNum,short crd,short gearAxis,double deltaPos,short accPercent,short decPercent,short fifo=0);
GT_API GT_BufMoveEx(short cardNum,short crd,short moveAxis,double pos,double vel,double acc,short modal,short fifo=0);
GT_API GT_BufIOEx(short cardNum,short crd,unsigned short doType,unsigned short doMask,unsigned short doValue,short fifo=0);
GT_API GT_BufDAEx(short cardNum,short crd,short chn,short daValue,short fifo=0);
GT_API GT_SetUserSegNumEx(short cardNum,short crd,long segNum,short fifo=0);

//Add By lin.ga 20150330
GT_API GT_BufLaserOnEx(short cardNum,short crd,short fifo=0,short channel=0);
GT_API GT_BufLaserOffEx(short cardNum,short crd,short fifo=0,short channel=0);
GT_API GT_BufLaserPrfCmdEx(short cardNum,short crd,double laserPower,short fifo=0,short channel=0);
GT_API GT_BufLaserFollowRatioEx(short cardNum,short crd,double ratio,double minPower,double maxPower,short fifo=0,short channel=0);
GT_API GT_BufLaserFollowModeEx(short cardNum,short crd,short source =0,short fifo=0,short channel=0);
GT_API GT_BufLaserFollowSplineEx(short cardNum,short crd,short tableId,double minPower,double maxPower,short fifo=0,short channel=0);
GT_API GT_BufLaserFollowOffEx(short cardNum,short crd,short fifo=0,short channel=0);

GT_API GT_BufDisableDoBitPulseEx(short cardNum,short crd,short doType,short doIndex,short fifo);
GT_API GT_BufEnableDoBitPulseEx(short cardNum,short crd,short doType,short doIndex,unsigned short highLevelTime,unsigned short lowLevelTime,long pulseNum,short firstLevel,short fifo);

GT_API GT_CrdDataEx(short cardNum,short crd,TCrdData *pCrdData,short fifo=0);
GT_API GT_GetLookAheadSegCountEx(short cardNum,short crd,long *pSegCount,short fifo=0);

GT_API GT_GetMotionTimeEx(short cardNum,short crd,double *pTime,short fifo);

GT_API GT_BufFollowMasterEx(short cardNum,short crd,TBufFollowMaster *pBufFollowMaster,short fifo=0);
GT_API GT_BufFollowEventCrossEx(short cardNum,short crd,TBufFollowEventCross *pEventCross,short fifo=0);
GT_API GT_BufFollowEventTriggerEx(short cardNum,short crd,TBufFollowEventTrigger *pEventTrigger,short fifo=0);
GT_API GT_BufFollowStartEx(short cardNum,short crd,long masterSegment,long slaveSegment,long masterFrameWidth,short fifo=0);
GT_API GT_BufFollowNextEx(short cardNum,short crd,long width,short fifo=0);
GT_API GT_BufFollowReturnEx(short cardNum,short crd,double vel,double acc,short smoothPercent,short fifo=0);

GT_API GT_BufSmartCutterOnEx(short cardNum,short crd,short smartCutterIndex,short fifo=0);
GT_API GT_BufSmartCutterOffEx(short cardNum,short crd,short smartCutterIndex,short fifo=0);

GT_API GT_BufEventOnEx(short cardNum,short crd,short eventIndex,short count,short fifo);
GT_API GT_BufEventOffEx(short cardNum,short crd,short eventIndex,short count,short fifo);
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////lin.ga 20150701 Add PathOpt Fuction/////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
GT_API GT_SetPathOptPara(short cardNum,short crd, TOptimizeParamUser *optPrm,short fifo);
GT_API GT_GetPathOptErrorInfo(short cardNum,short crd,TErrorInfo *errorInfo,short fifo);

//5 Axis
GT_API GT_CrdRTCPOn(short cardNum,short crd,short fifo=0);
GT_API GT_CrdRTCPOff(short cardNum,short crd,short fifo=0);
GT_API GT_UpdateMachineBuildingFile(short cardNum,short crd, int update);
GT_API GT_InitialMachineBuilding(short cardNum,short crd, char *pMachineCfgFileName,double *machineCoordCenter, double *workCoordCenter, double toolLength);
GT_API GT_SetRotationAxisRange(short cardNum,short crd, TRotationAxisRange *pRotationAxisRange);
GT_API GT_SetInverseSolutionSelectPara(short cardNum,short crd, EGroupSelect groupSelect, int priorAxisSet);
GT_API GT_MachineForwardTrans(short cardNum,short crd, double *pMachinePos, double *pWorkPos);
GT_API GT_MachineRTCPTrans(short cardNum,short crd, double *pInputPos, double *pMachinePos, double *pWorkPos);
GT_API GT_MachineTransformation(short cardNum,short crd, int posType, double *pPrePos, double *pPos, int *pPosNum, TPos **pReturnPos);
GT_API GT_MachineTransformationToLabView(short cardNum,short crd, int posType, double *pPrePos, double *pPos, int *pPosNum,double *machinePos,double *workPos);
GT_API GT_SetCompToolLength(short cardNum,short crd, double compToolLength);
GT_API GT_SetCompWorkCoordOffset(short cardNum,short crd, double *pCompWorkOffset);
GT_API GT_SetNonlinearErrorControl(short cardNum,short crd, int enable, double nonlinearError);
GT_API GT_EnableDiscreateArc(short cardNum,short crd,short enable,double arcError);
GT_API GT_StartXYCMachineMode(short cardNum,short crd, short dir, double contactAngle, double rotationAngle, double *pTranslation, double aValue,short fifo);//启动XYC模式
GT_API GT_EndXYCMachineMode(short cardNum,short crd,short fifo);                                                   //退出XYC模式
GT_API GT_SetWorkCrdPlane(short cardNum,short crd, short enable, double *pNormVector, double *pPoint,short fifo);    //设置玻璃加工平面参数
GT_API GT_SetStartPointProcessMode(short cardNum,short crd, short enable, double z,short fifo);                      //预处理第一段xyc数据
GT_API GT_InitialMachineBuildingEx(short cardNum,short crd, char *pMachineCfgFileName,double *machineCoordCenter, double *workCoordCenter, double toolLength);
GT_API GT_SetWorkCrdLaserFollowMode(short cardNum,short crd,short enbale,short fifo,short chn);
GT_API GT_ArcXYRACEx(short cardNum,short crd,double x,double y,double a,double c,double radius,short circleDir,double synVel,double synAcc,long segNum,short override2,short fifo);
GT_API GT_ArcXYCACEx(short cardNum,short crd,double x,double y,double a,double c,double xCenter,double yCenter,short circleDir,double synVel,double synAcc,long segNum,short override2,short fifo);
GT_API GT_ArcXYZACEx(short cardNum,short crd,double x,double y,double z,double a,double c,double interX,double interY,double interZ,double interA,double interC,double synVel,double synAcc,long segNum,short override2,short fifo);
GT_API GT_ArcYZRACEx(short cardNum,short crd,double y,double z,double a,double c,double radius,short circleDir,double synVel,double synAcc,long segNum,short override2,short fifo);
GT_API GT_ArcYZCACEx(short cardNum,short crd,double y,double z,double a,double c,double yCenter,double zCenter,short circleDir,double synVel,double synAcc,long segNum,short override2,short fifo);
GT_API GT_ArcZXRACEx(short cardNum,short crd,double z,double x,double a,double c,double radius,short circleDir,double synVel,double synAcc,long segNum,short override2,short fifo);
GT_API GT_ArcZXCACEx(short cardNum,short crd,double z,double x,double a,double c,double zCenter,double xCenter,short circleDir,double synVel,double synAcc,long segNum,short override2,short fifo);

GT_API GT_BufPosCompareStartEx(short cardNum,short crd,short fifo,short index);
GT_API GT_BufPosCompareStopEx(short cardNum,short crd,short fifo,short index);
GT_API GT_BufPosComparePsoPrmEx(short cardNum,short crd,short index,TPosComparePsoPrm *pPrm,short fifo);
GT_API GT_BufSetComparePortEx(short cardNum,short crd,short fifo,short channel,short hsio0,short hsio1);

GT_API GT_Buf2DCompareDataEx(short cardNum,short crd,short fifo,short chn,T2DCompareDataEx data,short compareFifo);

GT_API GT_BufStopEx(short cardNum,short crd,long mask,long option,short fifo=0);
GT_API GT_BufMoveJogEx(short cardNum,short crd,short moveAxis,double vel,double acc,short modal,short fifo=0);

GT_API GT_BufWaitLongVarEx(short cardNum,short crd,short index,long value,long overTime,short flagMode,long segNum,short fifo);