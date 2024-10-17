/********************************** ZMC系列控制器  ************************************************
**--------------文件信息--------------------------------------------------------------------------------
**文件名: zmcaux.h
**创建人: 郑孝洋
**时间: 20130621
**描述: ZMCDLL 辅助函数

**------------修订历史记录----------------------------------------------------------------------------
** 修改人: 
** 版  本: 
** 日　期: 
** 描　述: 
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/




#ifndef _ZMOTION_DLL_AUX_H
#define _ZMOTION_DLL_AUX_H


//ZAUX支持的最大轴数宏
#define MAX_AXIS_AUX   32  
#define MAX_CARD_AUX   16







/*********************************************************
数据类型定义
**********************************************************/

typedef unsigned __int64   uint64;  
typedef __int64   int64;  


//#define BYTE           INT8U
//#define WORD           INT16U
//#define DWORD          INT32U
typedef unsigned char  BYTE;
typedef unsigned short  WORD;
//typedef unsigned int  DWORD;
//#define __stdcall 
typedef unsigned char  uint8;                   /* defined for unsigned 8-bits integer variable     无符号8位整型变量  */
typedef signed   char  int8;                    /* defined for signed 8-bits integer variable        有符号8位整型变量  */
typedef unsigned short uint16;                  /* defined for unsigned 16-bits integer variable     无符号16位整型变量 */
typedef signed   short int16;                   /* defined for signed 16-bits integer variable         有符号16位整型变量 */
typedef unsigned int   uint32;                  /* defined for unsigned 32-bits integer variable     无符号32位整型变量 */
typedef signed   int   int32;                   /* defined for signed 32-bits integer variable         有符号32位整型变量 */
typedef float          fp32;                    /* single precision floating point variable (32bits) 单精度浮点数（32位长度） */
typedef double         fp64;                    /* double precision floating point variable (64bits) 双精度浮点数（64位长度） */
typedef unsigned int   uint;                  /* defined for unsigned 32-bits integer variable     无符号32位整型变量 */



/************************************************/
//错误码 
/************************************************/
#define ERR_OK  0
#define ERROR_OK 0
#define ERR_SUCCESS  0

#define ERR_AUX_OFFSET       30000

#define ERR_NOACK               ERR_AUX_OFFSET      //无应答
#define ERR_ACKERROR            (ERR_AUX_OFFSET+1)  //应答错误
#define ERR_AUX_PARAERR         (ERR_AUX_OFFSET+2)  //参数错误
#define ERR_AUX_NOTSUPPORT      (ERR_AUX_OFFSET+3)  //参数错误



/*********************************************************
函数声明
**********************************************************/

/*************************************************************
Description:    //BASE指令调用

仅仅修改在线命令的BASE列表，不对控制器的运行任务的BASE进行修改.
修改后，后续的所有MOVE等指令都是以这个BASE为基础

Input:          //卡链接handle
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Base(ZMC_HANDLE handle, int imaxaxises, int *piAxislist);


/*************************************************************
Description:    //定义DPOS
Input:          //卡链接handle
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Defpos(ZMC_HANDLE handle, int imaxaxises, float *pfDposlist);



/*************************************************************
Description:    //相对插补
Input:          //卡链接handle
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Move(ZMC_HANDLE handle, int imaxaxises, float *pfDisancelist);

/*************************************************************
Description:    //相对插补
Input:          //卡链接handle
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MoveSp(ZMC_HANDLE handle, int imaxaxises, float *pfDisancelist);


/*************************************************************
Description:    //插补
Input:          //卡链接handle
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MoveAbs(ZMC_HANDLE handle, int imaxaxises, float *pfDisancelist);

/*************************************************************
Description:    //插补
Input:          //卡链接handle
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MoveAbsSp(ZMC_HANDLE handle, int imaxaxises, float *pfDisancelist);


/*************************************************************
Description:    //插补
Input:          //卡链接handle
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MoveModify(ZMC_HANDLE handle, int imaxaxises, float *pfDisancelist);



/*************************************************************
Description:    //插补
Input:          //卡链接handle

end1              第一个轴运动坐标
end2              第二个轴运动坐标
centre1    第一个轴运动圆心，相对与起始点。
centre2    第二个轴运动圆心，相对与起始点。
direction  0-逆时针，1-顺时针

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MoveCirc(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection);

/*************************************************************
Description:    //插补
Input:          //卡链接handle

end1              第一个轴运动坐标
end2              第二个轴运动坐标
centre1    第一个轴运动圆心，相对与起始点。
centre2    第二个轴运动圆心，相对与起始点。
direction  0-逆时针，1-顺时针

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MoveCircSp(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection);


/*************************************************************
Description:    //插补
Input:          //卡链接handle

end1              第一个轴运动坐标，绝对位置
end2              第二个轴运动坐标，绝对位置
centre1    第一个轴运动圆心，绝对位置
centre2    第二个轴运动圆心，绝对位置
direction  0-逆时针，1-顺时针

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MoveCircAbs(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection);

/*************************************************************
Description:    //插补
Input:          //卡链接handle

end1              第一个轴运动坐标，绝对位置
end2              第二个轴运动坐标，绝对位置
centre1    第一个轴运动圆心，绝对位置
centre2    第二个轴运动圆心，绝对位置
direction  0-逆时针，1-顺时针

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MoveCircAbsSp(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection);


/*************************************************************
Description:    //插补
Input:          //卡链接handle

mid1       第一个轴中间点，相对起始点距离
mid2       第二个轴中间点，相对起始点距离
end1              第一个轴结束点，相对起始点距离
end2              第二个轴结束点，相对起始点距离

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MoveCirc2(ZMC_HANDLE handle, float fmid1, float fmid2, float fend1, float fend2);

/*************************************************************
Description:    //插补
Input:          //卡链接handle

mid1       第一个轴中间点，相对起始点距离
mid2       第二个轴中间点，相对起始点距离
end1              第一个轴结束点，相对起始点距离
end2              第二个轴结束点，相对起始点距离

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MoveCirc2Sp(ZMC_HANDLE handle, float fmid1, float fmid2, float fend1, float fend2);


/*************************************************************
Description:    //插补
Input:          //卡链接handle

mid1       第一个轴中间点，绝对位置
mid2       第二个轴中间点，绝对位置
end1              第一个轴结束点，绝对位置
end2              第二个轴结束点，绝对位置

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MoveCirc2AbsSp(ZMC_HANDLE handle, float fmid1, float fmid2, float fend1, float fend2);


/*************************************************************
Description:    //插补
Input:          //卡链接handle

mid1       第一个轴中间点，绝对位置
mid2       第二个轴中间点，绝对位置
end1              第一个轴结束点，绝对位置
end2              第二个轴结束点，绝对位置

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MoveCirc2Abs(ZMC_HANDLE handle, float fmid1, float fmid2, float fend1, float fend2);


/*************************************************************
Description:    //插补
Input:          //卡链接handle

end1              第一个轴运动坐标

end2              第二个轴运动坐标

centre1    第一个轴运动圆心，相对与起始点

centre2    第二个轴运动圆心，相对与起始点

direction  0-逆时针，1-顺时针

distance3第三个轴运动距离。

mode      第三轴的速度计算:
0(缺省)
 第三轴参与速度计算。
1
 第三轴不参与速度计算。
 

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MHelical(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fDistance3, int imode);


/*************************************************************
Description:    //插补
Input:          //卡链接handle

end1              第一个轴运动坐标

end2              第二个轴运动坐标

centre1    第一个轴运动圆心，相对与起始点

centre2    第二个轴运动圆心，相对与起始点

direction  0-逆时针，1-顺时针

distance3第三个轴运动距离。

mode      第三轴的速度计算:
0(缺省)
 第三轴参与速度计算。
1
 第三轴不参与速度计算。
 

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MHelicalAbs(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fDistance3, int imode);


/*************************************************************
Description:    //插补
Input:          //卡链接handle

end1              第一个轴运动坐标

end2              第二个轴运动坐标

centre1    第一个轴运动圆心，相对与起始点

centre2    第二个轴运动圆心，相对与起始点

direction  0-逆时针，1-顺时针

distance3第三个轴运动距离。

mode      第三轴的速度计算:
0(缺省)
 第三轴参与速度计算。
1
 第三轴不参与速度计算。
 

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MHelicalSp(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fDistance3, int imode);

/*************************************************************
Description:    //插补
Input:          //卡链接handle

end1              第一个轴运动坐标

end2              第二个轴运动坐标

centre1    第一个轴运动圆心，相对与起始点

centre2    第二个轴运动圆心，相对与起始点

direction  0-逆时针，1-顺时针

distance3第三个轴运动距离。

mode      第三轴的速度计算:
0(缺省)
 第三轴参与速度计算。
1
 第三轴不参与速度计算。
 

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MHelicalAbsSp(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fDistance3, int imode);


/*************************************************************
Description:    //插补
Input:          //卡链接handle

mid1       第一个轴中间点

mid2       第二个轴中间点

end1              第一个轴结束点

end2              第二个轴结束点

distance3第三个轴运动距离

mode      第三轴的速度计算:

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MHelical2(ZMC_HANDLE handle, float fmid1, float fmid2, float fend1, float fend2, float fDistance3, int imode);


/*************************************************************
Description:    //插补
Input:          //卡链接handle

mid1       第一个轴中间点

mid2       第二个轴中间点

end1              第一个轴结束点

end2              第二个轴结束点

distance3   第三个轴运动结束点

mode      第三轴的速度计算:

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MHelical2Abs(ZMC_HANDLE handle, float fmid1, float fmid2, float fend1, float fend2, float fDistance3, int imode);

/*************************************************************
Description:    //插补
Input:          //卡链接handle

mid1       第一个轴中间点

mid2       第二个轴中间点

end1              第一个轴结束点

end2              第二个轴结束点

distance3第三个轴运动距离

mode      第三轴的速度计算:

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MHelical2Sp(ZMC_HANDLE handle, float fmid1, float fmid2, float fend1, float fend2, float fDistance3, int imode);

/*************************************************************
Description:    //插补
Input:          //卡链接handle

mid1       第一个轴中间点

mid2       第二个轴中间点

end1              第一个轴结束点

end2              第二个轴结束点

distance3   第三个轴运动结束点

mode      第三轴的速度计算:

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MHelical2AbsSp(ZMC_HANDLE handle, float fmid1, float fmid2, float fend1, float fend2, float fDistance3, int imode);


/*************************************************************
Description:    //插补
Input:          //卡链接handle

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MEclipse(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fADis, float fBDis);


/*************************************************************
Description:    //插补
Input:          //卡链接handle

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MEclipseAbs(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fADis, float fBDis);



/*************************************************************
Description:    //插补
Input:          //卡链接handle

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MEclipseSp(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fADis, float fBDis);


/*************************************************************
Description:    //插补
Input:          //卡链接handle

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MEclipseAbsSp(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fADis, float fBDis);


/*************************************************************
Description:    //插补 椭圆 + 螺旋
Input:          //卡链接handle

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MEclipseHelical(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fADis, float fBDis, float fDistance3);


/*************************************************************
Description:    //插补  椭圆 + 螺旋
Input:          //卡链接handle

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MEclipseHelicalAbs(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fADis, float fBDis,float fDistance3);


/*************************************************************
Description:    //插补 椭圆 + 螺旋
Input:          //卡链接handle

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MEclipseHelicalSp(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fADis, float fBDis, float fDistance3);


/*************************************************************
Description:    //插补  椭圆 + 螺旋
Input:          //卡链接handle

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MEclipseHelicalAbsSp(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fADis, float fBDis,float fDistance3);


/*************************************************************
Description:    //插补  空间圆弧
Input:          //卡链接handle
end1              第1个轴运动距离参数1

end2              第2个轴运动距离参数1

end3              第3个轴运动距离参数1

centre1    第1个轴运动距离参数2

centre2    第2个轴运动距离参数2

centre3    第3个轴运动距离参数2

mode      指定前面参数的意义
0 当前点，中间点，终点三点定圆弧，距离参数1为终点距离，距离参数2为中间点距离。
1 走最小的圆弧，距离参数1为终点距离，距离参数2为圆心的距离。
2 当前点，中间点，终点三点定圆，距离参数1为终点距离，距离参数2为中间点距离。
3 先走最小的圆弧，再继续走完整圆，距离参数1为终点距离，距离参数2为圆心的距离。
 

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MSpherical(ZMC_HANDLE handle, float fend1, float fend2, float fend3, float fcenter1, float fcenter2, float fcenter3, int imode);


/*************************************************************
Description:    //插补  空间圆弧
Input:          //卡链接handle
end1              第1个轴运动距离参数1

end2              第2个轴运动距离参数1

end3              第3个轴运动距离参数1

centre1    第1个轴运动距离参数2

centre2    第2个轴运动距离参数2

centre3    第3个轴运动距离参数2

mode      指定前面参数的意义
0 当前点，中间点，终点三点定圆弧，距离参数1为终点距离，距离参数2为中间点距离。
1 走最小的圆弧，距离参数1为终点距离，距离参数2为圆心的距离。
2 当前点，中间点，终点三点定圆，距离参数1为终点距离，距离参数2为中间点距离。
3 先走最小的圆弧，再继续走完整圆，距离参数1为终点距离，距离参数2为圆心的距离。
 

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MSphericalSp(ZMC_HANDLE handle, float fend1, float fend2, float fend3, float fcenter1, float fcenter2, float fcenter3, int imode);


/*************************************************************
Description:    //运动暂停
Input:          //卡链接handle  

0（缺省） 暂停当前运动。 
1 在当前运动完成后正准备执行下一条运动指令时暂停。 
2 在当前运动完成后正准备执行下一条运动指令时，并且两条指令的MARK标识不一样时暂停。这个模式可以用于一个动作由多个指令来实现时，可以在一整个动作完成后暂停。 

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MovePause(ZMC_HANDLE handle, int imode);


/*************************************************************
Description:    //运动暂停
Input:          //卡链接handle  

0（缺省） 暂停当前运动。 
1 在当前运动完成后正准备执行下一条运动指令时暂停。 
2 在当前运动完成后正准备执行下一条运动指令时，并且两条指令的MARK标识不一样时暂停。这个模式可以用于一个动作由多个指令来实现时，可以在一整个动作完成后暂停。 

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MoveResume(ZMC_HANDLE handle);



/*************************************************************
Description:    //在运动缓冲中加入输出指令
Input:          //卡链接handle  

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MoveOp(ZMC_HANDLE handle, int ioutnum, int ivalue);


/*************************************************************
Description:    //在运动缓冲中加入输出指令
Input:          //卡链接handle  

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MoveOpMulti(ZMC_HANDLE handle, int ioutnumfirst, int ioutnumend, int ivalue);



/*************************************************************
Description:    //在运动缓冲中加入输出指令
Input:          //卡链接handle  

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MoveOp2(ZMC_HANDLE handle, int ioutnum, int ivalue, int iofftimems);


/*************************************************************
Description:    //在运动缓冲中加入延时指令
Input:          //卡链接handle  

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MoveDelay(ZMC_HANDLE handle, int itimems);


/*************************************************************
Description:    //电子凸轮
Input:          //卡链接handle  

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Cam(ZMC_HANDLE handle, int istartpoint, int iendpoint, float ftablemulti, float fDistance);


/*************************************************************
Description:    //电子凸轮
Input:          //卡链接handle  

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Cambox(ZMC_HANDLE handle, int istartpoint, int iendpoint, float ftablemulti, float fDistance, int ilinkaxis, int ioption, float flinkstartpos);




/*************************************************************
Description:    //连接 电子齿轮
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Connpath(ZMC_HANDLE handle, float fratio, int idringaxis);


/*************************************************************
Description:    //连接 电子齿轮
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Connect(ZMC_HANDLE handle, float fratio, int idringaxis);



/*************************************************************
Description:    //连接 特殊凸轮
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Movelink(ZMC_HANDLE handle, float fDistance, float fLinkDis, float fLinkAcc, float fLinkDec,int iLinkaxis, int ioption, float flinkstartpos);


/*************************************************************
Description:    //位置锁存指令
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Regist(ZMC_HANDLE handle, int imode);



/*************************************************************
Description:    //所有轴立即停止
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Rapidstop(ZMC_HANDLE handle, int imode);

/*************************************************************
Description:    //SP指定速度的插补运动, 可以指定起始速度，最终速度， 运行速度
                  运动的轴是当前的BASE轴列表
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_LineNSp(ZMC_HANDLE handle, int imaxaxis, float *pfDisancelist, float fStartSpeed, float fEndSpeed, float fForceSpeed);




#if 0
    //单轴运动
#endif

/*************************************************************
Description:    //连续运动
Input:          //卡链接handle  轴号， 方向
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Singl_Vmove(ZMC_HANDLE handle, int iaxis, int idir);

/*************************************************************
Description:    //单轴运动
Input:          //卡链接handle  轴号， 距离
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Singl_Move(ZMC_HANDLE handle, int iaxis, float fdistance);

/*************************************************************
Description:    //单轴运动
Input:          //卡链接handle  轴号， 距离
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Singl_MoveAbs(ZMC_HANDLE handle, int iaxis, float fdistance);

/*************************************************************
Description:    //单轴运动
Input:          //卡链接handle  轴号， 距离
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Singl_MoveModify(ZMC_HANDLE handle, int iaxis, float fdistance);


/*************************************************************
Description:    //叠加
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Singl_Addax(ZMC_HANDLE handle, int iaxis, int iaddaxis);


/*************************************************************
Description:    //cancel
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Singl_Cancel(ZMC_HANDLE handle, int iaxis, int imode);


/*************************************************************
Description:    //回零
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Singl_Datum(ZMC_HANDLE handle, int iaxis, int imode);



/*************************************************************
Description:    //电子齿轮比设置
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Singl_EncoderRatio(ZMC_HANDLE handle, int iaxis, int imposcount, int inputcount);



/*************************************************************
Description:    //电子齿轮比设置
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Singl_StepRatio(ZMC_HANDLE handle, int iaxis, int ioutcount, int idposcount);





#if 0
    //参数部分
#endif




/*************************************************************
Description:    //参数 轴类型设置， 步进轴一般不用设置
Input:          //卡链接handle  merge: 0/1
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetAtype(ZMC_HANDLE handle,int iaxis, int iset);
/*************************************************************
Description:    //参数 设置是否连续插补
Input:          //卡链接handle  merge: 0/1
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetMerge(ZMC_HANDLE handle,int iaxis,  int iset);

/*************************************************************
Description:    //参数
Input:          //卡链接handle  merge: 0/1
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetUnits(ZMC_HANDLE handle, int iaxis, float fset);


/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetSpeed(ZMC_HANDLE handle, int iaxis, float fspeed);



/*************************************************************
Description:    //参数
Input:          //卡链接handle  merge: 0/1
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetAccel(ZMC_HANDLE handle, int iaxis, float fspeed);

/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetDecel(ZMC_HANDLE handle, int iaxis, float fspeed);
int32 __stdcall ZAux_SetDatumin(ZMC_HANDLE handle, int iaxis, int id);
/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetFastDec(ZMC_HANDLE handle, int iaxis, float fspeed);
/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetForceSpeed(ZMC_HANDLE handle, int iaxis, float fspeed);

/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetStartMoveSpeed(ZMC_HANDLE handle, int iaxis, float fspeed);

/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetEndMoveSpeed(ZMC_HANDLE handle, int iaxis, float fspeed);

/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetLSpeed(ZMC_HANDLE handle, int iaxis, float fspeed);

/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetCreepSpeed(ZMC_HANDLE handle, int iaxis, float fspeed);

/*************************************************************
Description:    //参数  拐角减速设置
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetCornerMode(ZMC_HANDLE handle, int iaxis, int iset);
/*************************************************************
Description:    //参数  脉冲的设置
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetInvertStep(ZMC_HANDLE handle, int iaxis, int iset);


/*************************************************************
Description:    //参数  S曲线设置
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetDecelAngle(ZMC_HANDLE handle, int iaxis, float fset);


/*************************************************************
Description:    //参数  S曲线设置
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetStopAngle(ZMC_HANDLE handle, int iaxis, float fset);

/*************************************************************
Description:    //参数  小圆限速的半径设置
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetSpRadius(ZMC_HANDLE handle, int iaxis, float fset);


/*************************************************************
Description:    //参数  S曲线设置
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetSramp(ZMC_HANDLE handle, int iaxis, int iset);

/*************************************************************
Description:    //参数  DPOS修改
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetDpos(ZMC_HANDLE handle, int iaxis, float fpos);

/*************************************************************
Description:    //参数  MPOS修改
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetMpos(ZMC_HANDLE handle, int iaxis, float fpos);

/*************************************************************
Description:    //参数  相对修改坐标
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetOffpos(ZMC_HANDLE handle, int iaxis, float fpos);






#if 0
    //状态读取
#endif


/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetDpos(ZMC_HANDLE handle, int iaxis, float *pfValue);


/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetMpos(ZMC_HANDLE handle, int iaxis, float *pfValue);



/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetCurSpeed(ZMC_HANDLE handle, int iaxis, float *pfValue);


/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetMtype(ZMC_HANDLE handle, int iaxis, int *piValue);

/*************************************************************
Description:    //参数  轴是否运动完成
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetIfIdle(ZMC_HANDLE handle, int iaxis, int *piValue);




/*************************************************************
Description:    //参数 当前运行完成后的终点坐标, 缓冲里面的不算
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetEndMove(ZMC_HANDLE handle, int iaxis, float *pfValue);



/*************************************************************
Description:    //参数 当前缓冲运行完成后的终点坐标 用于转换相对绝对
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetEndMoveBuff(ZMC_HANDLE handle, int iaxis, float *pfValue);


/*************************************************************
Description:    //参数 
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetAxisStatus(ZMC_HANDLE handle, int iaxis, int *piValue);



/*************************************************************
Description:    //参数  内部的编码器位置
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetEncoder(ZMC_HANDLE handle, int iaxis, int *piValue);


/*************************************************************
Description:    //参数  轴剩余的缓冲, 按直线来计算
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetRemain_LineBuffer(ZMC_HANDLE handle, int iaxis, int *piValue);



/*************************************************************
Description:    //参数  轴剩余的缓冲, 按最复杂的空间圆弧来计算
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetRemain_Buffer(ZMC_HANDLE handle, int iaxis, int *piValue);




/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetRegPos(ZMC_HANDLE handle, int iaxis, float *pfValue);


/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetMark(ZMC_HANDLE handle, int iaxis, int *piValue);



/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetMarkB(ZMC_HANDLE handle, int iaxis, int *piValue);



/*************************************************************
Description:    //全局变量读取
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetVariablef(ZMC_HANDLE handle, const char *pname, float *pfValue);

/*************************************************************
Description:    //全局变量读取, 也可以是参数等等
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetVariableInt(ZMC_HANDLE handle, const char *pname, int *piValue);



#if 0
    //IO指令
    // 可以使用 ZMC_GetIn ZMC_GetOutput 等
#endif

/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetIn(ZMC_HANDLE handle, int ionum, uint32 *piValue);


/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetInvertIN(ZMC_HANDLE handle, int ionum, int bifInvert);



/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetOp(ZMC_HANDLE handle, int ionum, uint32 iValue);


#if 0
    //通过modbus快速读取特殊寄存器
#endif

/*************************************************************
Description:    //参数 快速读取多个输入
Input:          //卡链接handle  
Output:         //按位存储
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetModbusIn(ZMC_HANDLE handle, int ionumfirst, int ionumend, uint8 *pValueList);


/*************************************************************
Description:    //参数 快速读取多个当前的输出状态
Input:          //卡链接handle  
Output:         //按位存储
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetModbusOut(ZMC_HANDLE handle, int ionumfirst, int ionumend, uint8 *pValueList);


/*************************************************************
Description:    //参数 快速读取多个当前的DPOS
Input:          //卡链接handle  
Output:         //按存储
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetModbusDpos(ZMC_HANDLE handle, int imaxaxises, float *pValueList);


/*************************************************************
Description:    //参数 快速读取多个当前的MPOS
Input:          //卡链接handle  
Output:         //按存储
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetModbusMpos(ZMC_HANDLE handle, int imaxaxises, float *pValueList);


/*************************************************************
Description:    //参数 快速读取多个当前的速度
Input:          //卡链接handle  
Output:         //按存储
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetModbusCurSpeed(ZMC_HANDLE handle, int imaxaxises, float *pValueList);



#if 0
    //采用ZMC_DirectCommand 来快速获取一些状态, ZMC_DirectCommand的执行比ZMC_Execute要快
	// 只有参数，变量，数组元素等能使用ZMC_DirectCommand
#endif

/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Direct_GetDpos(ZMC_HANDLE handle, int iaxis, float *pfValue);


/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Direct_GetMpos(ZMC_HANDLE handle, int iaxis, float *pfValue);



/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Direct_GetCurSpeed(ZMC_HANDLE handle, int iaxis, float *pfValue);



/*************************************************************
Description:    //参数 当前的运动类型
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Direct_GetMtype(ZMC_HANDLE handle, int iaxis, int *piValue);


/*************************************************************
Description:    //参数  轴是否运动完成
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Direct_GetIfIdle(ZMC_HANDLE handle, int iaxis, int *piValue);




/*************************************************************
Description:    //参数 当前运行完成后的终点坐标, 缓冲里面的不算
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Direct_GetEndMove(ZMC_HANDLE handle, int iaxis, float *pfValue);



/*************************************************************
Description:    //参数 当前缓冲运行完成后的终点坐标 用于转换相对绝对
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Direct_GetEndMoveBuff(ZMC_HANDLE handle, int iaxis, float *pfValue);


/*************************************************************
Description:    //参数 
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Direct_GetAxisStatus(ZMC_HANDLE handle, int iaxis, int *piValue);



/*************************************************************
Description:    //参数  内部的编码器位置
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Direct_GetEncoder(ZMC_HANDLE handle, int iaxis, int *piValue);


/*************************************************************
Description:    //参数  轴剩余的缓冲, 按直线段来计算
				  REMAIN_BUFFER为唯一一个可以加AXIS并用ZMC_DirectCommand获取的.
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Direct_GetRemain_LineBuffer(ZMC_HANDLE handle, int iaxis, int *piValue);



/*************************************************************
Description:    //参数  轴剩余的缓冲, 按最复杂的空间圆弧来计算
				  REMAIN_BUFFER为唯一一个可以加AXIS并用ZMC_DirectCommand获取的.
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Direct_GetRemain_Buffer(ZMC_HANDLE handle, int iaxis, int *piValue);




/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Direct_GetRegPos(ZMC_HANDLE handle, int iaxis, float *pfValue);


/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Direct_GetMark(ZMC_HANDLE handle, int iaxis, int *piValue);



/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Direct_GetMarkB(ZMC_HANDLE handle, int iaxis, int *piValue);



/*************************************************************
Description:    //全局变量读取, 也可以是参数等等
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Direct_GetVariablef(ZMC_HANDLE handle, const char *pname, float *pfValue);


/*************************************************************
Description:    //全局变量读取, 也可以是参数等等
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Direct_GetVariableInt(ZMC_HANDLE handle, const char *pname, int *piValue);



#if 0
    //辅助函数
#endif


/*************************************************************
Description:    //字符串转为float
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_TransStringtoFloat(const char* pstringin, int inumes,  float* pfvlaue);


/*************************************************************
Description:    //字符串转为int
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_TransStringtoInt(const char* pstringin, int inumes,  int* pivlaue);



//
/*************************************************************
Description:    //把卡的handle 与 卡号关联起来, 这样后面就可以直接用卡号来操作了
Input:          //卡链接handle
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_Init();



/*************************************************************
Description:    //把卡的handle 与 卡号关联起来, 这样后面就可以直接用卡号来操作了
Input:          //卡链接handle
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_SetCardHandle(WORD card, ZMC_HANDLE handle);


/*************************************************************
Description:    //把卡的handle 与 卡号关联起来, 这样后面就可以直接用卡号来操作了
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_UnSetCardHandle(WORD card);




/*************************************************************
Description:    //单轴点位运动
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_GetTotalAxises(WORD card);



/*************************************************************
Description:    // 设置轨迹的运动参数
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_SetVectProfile(WORD card, double spara, int32 speed, int32 acc, int32 dec );



/*************************************************************
Description:    // 设置单轴的运动参数
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_SetProfile(WORD card, int iaxis, int32 speed, int32 acc, int32 dec);



/*************************************************************
Description:    // 读取坐标
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_SetSProfile(WORD card, int iaxis, double spara);




/*************************************************************
Description:    //单轴点位运动
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_PMove(WORD card, int iaxis, int Length);


/*************************************************************
Description:    //单轴点位运动
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_PMoveAbs(WORD card, int iaxis, int Length);



/*************************************************************
Description:    //PMOVE时动态调整位置
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_ResetTargetPostion(WORD card, int iaxis, int Length);





/*************************************************************
Description:    //SP指定速度的运动
Input:          //卡号   dir 1- 正向， 2-负向
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_VMove(WORD card, int iaxis, int dir);




/*************************************************************
Description:    // 脉冲模式设置
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_SetPulOutMode(WORD card, int iaxis, int iset);




/*************************************************************
Description:    // 编码器模式设置，这个对应到不同的ATYPE，无法映射
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_SetCounterMode(WORD card, int iaxis, int iset);



/*************************************************************
Description:    // 手轮， 用编码器来模拟
Input:          //卡号  AB类型还是 脉冲方向 类型
                  这里只配置ATYPE， 当使用扩展轴时，ATYPE类型不一样.

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_SetHandle(WORD card, int iaxiscoder, int ifabmode);



/*************************************************************
Description:    // 手轮， 用编码器来模拟
Input:          //卡号  必须先配置手轮的轴
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_HandMove(WORD card, int iaxis, float fratio);



/*************************************************************
Description:    // 减速停止， 立即停止时，把FASTDEC设置很大的值，再调用减速停止
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_DeclStop(WORD card, int iaxis);


/*************************************************************
Description:    // 读取坐标
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_GetPosition(WORD card, int iaxis);


/*************************************************************
Description:    // 读取坐标
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_SetPosition(WORD card, int iaxis, int32 current_position);



/*************************************************************
Description:    // 读取坐标
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_GetCurSpeed(WORD card, int iaxis);


/*************************************************************
Description:    // 读取坐标
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_SetCurSpeed(WORD card, int iaxis, int32 speed);



/*************************************************************
Description:    // 读取坐标
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_ConfigHomeMode(WORD card, int iaxis, int home_dir, int velspeed, int Mode);



/*************************************************************
Description:    // 读取坐标
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_HomeMove(WORD card, int iaxis);




/*************************************************************
Description:    // 减速停止 插补指令, 因为涉及到多个轴，必须修改BASE轴列表
                   注意连续插补的时候不要切换主轴,即BASE的第一个轴

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_Line2(WORD card, int iaxis1, int iaxis2, int distance1, int distance2);



/*************************************************************
Description:    // 减速停止 插补指令, 因为涉及到多个轴，必须修改BASE轴列表
                   注意连续插补的时候不要切换主轴,即BASE的第一个轴

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_LineN(WORD card, int imaxaxis, int* iaxisList, int *distanceList);




#if 0
    //额外的函数
    //合成特殊的函数
    //采用卡号来操作
#endif


/*************        每个卡的内部数据定义,              ***************/

//额外的函数支持的插补个数, 一般的PCI卡只有一个
#define MAX_VECT_AUX_SPECIAL   1


typedef struct str_ZAuxCardSpecial
{
    uint8       m_cardnum;   //卡号
    ZMC_HANDLE  m_handle;    //句柄

    //手轮的轴
    int  m_iaxisHandle; // 手轮

    //单轴设置
    double      m_aSpeed[MAX_AXIS_AUX];
    double      m_aAccel[MAX_AXIS_AUX];
    double      m_aDecel[MAX_AXIS_AUX];
    
    double      m_aSRampSet[MAX_AXIS_AUX];  // S曲线的设置

    double      m_aHomeSpeed[MAX_AXIS_AUX]; // 回零的速度
    int      m_aHomeDir[MAX_AXIS_AUX]; // 回零方向
    int      m_aHomeMode[MAX_AXIS_AUX]; // 回零模式
    
    
    //插补的设置
    double      m_aVectSpeed[MAX_VECT_AUX_SPECIAL];
    double      m_aVectAccel[MAX_VECT_AUX_SPECIAL];
    double      m_aVectDecel[MAX_VECT_AUX_SPECIAL];
    
    double      m_aVectSRampSet[MAX_VECT_AUX_SPECIAL];  // S曲线的设置

    //
    

}struct_ZAuxCardSpecial;

























#endif


