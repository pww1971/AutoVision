
/********************************** ZMC系列控制器  ************************************************
**--------------文件信息--------------------------------------------------------------------------------
**文件名: zmcaux.c
**创建人: 郑孝洋
**时间: 20130621
**描述: ZMCDLL 辅助函数

本库提供对EXCUTE等在线命令执行函数的封装，不对控制器本身的执行程序进行修改操作.



**------------修订历史记录----------------------------------------------------------------------------
** 修改人: 
** 版  本: 
** 日　期: 
** 描　述: 
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#ifdef _ZMC 

#include "stdafx.h"

#include "stdio.h"


#include "zmotion.h"
#include "zmcaux.h"


#ifdef Z_DEBUG
#undef THIS_FILE
static const char THIS_FILE[] = "zmcaux";
#endif

int g_ZMC_MaxExcuteWaitms = 1000;



#if 0
    //基本运动函数
#endif


/*************************************************************
Description:    //BASE指令调用

仅仅修改在线命令的BASE列表，不对控制器的运行任务的BASE进行修改.
修改后，后续的所有MOVE等指令都是以这个BASE为基础

Input:          //卡链接handle
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Base(ZMC_HANDLE handle, int imaxaxises, int *piAxislist)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  tempbuff[2048];
	
	//
	ASSERT(imaxaxises < MAX_AXIS_AUX);
	ASSERT(imaxaxises > 0);

    //生成命令
	strcpy(cmdbuff, "BASE(");

	for(i = 0; i< imaxaxises-1; i++)
	{
		//
		sprintf(tempbuff, "%d,",piAxislist[i]);
		strcat(cmdbuff, tempbuff);
	}

	//
	sprintf(tempbuff, "%d)",piAxislist[imaxaxises-1]);
    strcat(cmdbuff, tempbuff);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //定义DPOS
Input:          //卡链接handle
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Defpos(ZMC_HANDLE handle, int imaxaxises, float *pfDposlist)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  tempbuff[2048];
	
	//
	ASSERT(imaxaxises < MAX_AXIS_AUX);
	ASSERT(imaxaxises > 0);

    //生成命令
	strcpy(cmdbuff, "DEFPOS(");

	for(i = 0; i< imaxaxises-1; i++)
	{
		//
		sprintf(tempbuff, "%f,",pfDposlist[i]);
		strcat(cmdbuff, tempbuff);
	}

	//
	sprintf(tempbuff, "%f)",pfDposlist[imaxaxises-1]);
    strcat(cmdbuff, tempbuff);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //相对插补
Input:          //卡链接handle
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Move(ZMC_HANDLE handle, int imaxaxises, float *pfDisancelist)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  tempbuff[2048];
	
	//
	ASSERT(imaxaxises < MAX_AXIS_AUX);
	ASSERT(imaxaxises > 0);

    //生成命令
	strcpy(cmdbuff, "MOVE(");

	for(i = 0; i< imaxaxises-1; i++)
	{
		//
		sprintf(tempbuff, "%f,",pfDisancelist[i]);
		strcat(cmdbuff, tempbuff);
	}

	//
	sprintf(tempbuff, "%f)",pfDisancelist[imaxaxises-1]);
    strcat(cmdbuff, tempbuff);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //相对插补
Input:          //卡链接handle
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MoveSp(ZMC_HANDLE handle, int imaxaxises, float *pfDisancelist)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  tempbuff[2048];
	
	//
	ASSERT(imaxaxises < MAX_AXIS_AUX);
	ASSERT(imaxaxises > 0);

    //生成命令
	strcpy(cmdbuff, "MOVESP(");

	for(i = 0; i< imaxaxises-1; i++)
	{
		//
		sprintf(tempbuff, "%f,",pfDisancelist[i]);
		strcat(cmdbuff, tempbuff);
	}

	//
	sprintf(tempbuff, "%f)",pfDisancelist[imaxaxises-1]);
    strcat(cmdbuff, tempbuff);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //插补
Input:          //卡链接handle
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MoveAbs(ZMC_HANDLE handle, int imaxaxises, float *pfDisancelist)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  tempbuff[2048];
	
	//
	ASSERT(imaxaxises < MAX_AXIS_AUX);
	ASSERT(imaxaxises > 0);

    //生成命令
	strcpy(cmdbuff, "MOVEABS(");

	for(i = 0; i< imaxaxises-1; i++)
	{
		//
		sprintf(tempbuff, "%f,",pfDisancelist[i]);
		strcat(cmdbuff, tempbuff);
	}

	//
	sprintf(tempbuff, "%f)",pfDisancelist[imaxaxises-1]);
    strcat(cmdbuff, tempbuff);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //插补
Input:          //卡链接handle
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MoveAbsSp(ZMC_HANDLE handle, int imaxaxises, float *pfDisancelist)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  tempbuff[2048];
	
	//
	ASSERT(imaxaxises < MAX_AXIS_AUX);
	ASSERT(imaxaxises > 0);

    //生成命令
	strcpy(cmdbuff, "MOVEABSSP(");

	for(i = 0; i< imaxaxises-1; i++)
	{
		//
		sprintf(tempbuff, "%f,",pfDisancelist[i]);
		strcat(cmdbuff, tempbuff);
	}

	//
	sprintf(tempbuff, "%f)",pfDisancelist[imaxaxises-1]);
    strcat(cmdbuff, tempbuff);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //插补
Input:          //卡链接handle
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MoveModify(ZMC_HANDLE handle, int imaxaxises, float *pfDisancelist)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  tempbuff[2048];
	
	//
	ASSERT(imaxaxises < MAX_AXIS_AUX);
	ASSERT(imaxaxises > 0);

    //生成命令
	strcpy(cmdbuff, "MOVEMODIFY(");

	for(i = 0; i< imaxaxises-1; i++)
	{
		//
		sprintf(tempbuff, "%f,",pfDisancelist[i]);
		strcat(cmdbuff, tempbuff);
	}

	//
	sprintf(tempbuff, "%f)",pfDisancelist[imaxaxises-1]);
    strcat(cmdbuff, tempbuff);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


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
int32 __stdcall ZAux_MoveCirc(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MOVECIRC(%f,%f,%f,%f,%d)", fend1,fend2,fcenter1,fcenter2,idirection);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

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
int32 __stdcall ZAux_MoveCircSp(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MOVECIRCSP(%f,%f,%f,%f,%d)", fend1,fend2,fcenter1,fcenter2,idirection);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


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
int32 __stdcall ZAux_MoveCircAbs(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MOVECIRCABS(%f,%f,%f,%f,%d)", fend1,fend2,fcenter1,fcenter2,idirection);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

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
int32 __stdcall ZAux_MoveCircAbsSp(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MOVECIRCABSSP(%f,%f,%f,%f,%d)", fend1,fend2,fcenter1,fcenter2,idirection);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}



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
int32 __stdcall ZAux_MoveCirc2(ZMC_HANDLE handle, float fmid1, float fmid2, float fend1, float fend2)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MOVECIRC2(%f,%f,%f,%f)", fmid1,fmid2,fend1,fend2);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


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
int32 __stdcall ZAux_MoveCirc2Abs(ZMC_HANDLE handle, float fmid1, float fmid2, float fend1, float fend2)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MOVECIRC2ABS(%f,%f,%f,%f)", fmid1,fmid2,fend1,fend2);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

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
int32 __stdcall ZAux_MoveCirc2Sp(ZMC_HANDLE handle, float fmid1, float fmid2, float fend1, float fend2)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MOVECIRC2SP(%f,%f,%f,%f)", fmid1,fmid2,fend1,fend2);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


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
int32 __stdcall ZAux_MoveCirc2AbsSp(ZMC_HANDLE handle, float fmid1, float fmid2, float fend1, float fend2)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MOVECIRC2ABSSP(%f,%f,%f,%f)", fmid1,fmid2,fend1,fend2);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


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
int32 __stdcall ZAux_MHelical(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fDistance3, int imode)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MHELICAL(%f,%f,%f,%f,%d,%f,%d)", fend1,fend2,fcenter1,fcenter2,idirection, fDistance3, imode);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

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
int32 __stdcall ZAux_MHelicalAbs(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fDistance3, int imode)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MHELICALABS(%f,%f,%f,%f,%d,%f,%d)", fend1,fend2,fcenter1,fcenter2,idirection, fDistance3, imode);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

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
int32 __stdcall ZAux_MHelicalSp(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fDistance3, int imode)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MHELICALSP(%f,%f,%f,%f,%d,%f,%d)", fend1,fend2,fcenter1,fcenter2,idirection, fDistance3, imode);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

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
int32 __stdcall ZAux_MHelicalAbsSp(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fDistance3, int imode)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MHELICALABSSP(%f,%f,%f,%f,%d,%f,%d)", fend1,fend2,fcenter1,fcenter2,idirection, fDistance3, imode);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


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
int32 __stdcall ZAux_MHelical2(ZMC_HANDLE handle, float fmid1, float fmid2, float fend1, float fend2, float fDistance3, int imode)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MHELICAL2(%f,%f,%f,%f,%f,%d)", fmid1,fmid2,fend1,fend2, fDistance3, imode);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

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
int32 __stdcall ZAux_MHelical2Abs(ZMC_HANDLE handle, float fmid1, float fmid2, float fend1, float fend2, float fDistance3, int imode)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MHELICAL2ABS(%f,%f,%f,%f,%f,%d)", fmid1,fmid2,fend1,fend2, fDistance3, imode);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

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
int32 __stdcall ZAux_MHelical2Sp(ZMC_HANDLE handle, float fmid1, float fmid2, float fend1, float fend2, float fDistance3, int imode)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MHELICAL2SP(%f,%f,%f,%f,%f,%d)", fmid1,fmid2,fend1,fend2, fDistance3, imode);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

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
int32 __stdcall ZAux_MHelical2AbsSp(ZMC_HANDLE handle, float fmid1, float fmid2, float fend1, float fend2, float fDistance3, int imode)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MHELICAL2ABSSP(%f,%f,%f,%f,%f,%d)", fmid1,fmid2,fend1,fend2, fDistance3, imode);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}



/*************************************************************
Description:    //插补
Input:          //卡链接handle

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MEclipse(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fADis, float fBDis)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MECLIPSE(%f,%f,%f,%f,%d,%f,%f)", fend1,fend2,fcenter1,fcenter2,idirection, fADis, fBDis);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //插补
Input:          //卡链接handle

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MEclipseAbs(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fADis, float fBDis)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MECLIPSEABS(%f,%f,%f,%f,%d,%f,%f)", fend1,fend2,fcenter1,fcenter2,idirection, fADis, fBDis);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //插补
Input:          //卡链接handle

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MEclipseSp(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fADis, float fBDis)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MECLIPSESP(%f,%f,%f,%f,%d,%f,%f)", fend1,fend2,fcenter1,fcenter2,idirection, fADis, fBDis);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //插补
Input:          //卡链接handle

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MEclipseAbsSp(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fADis, float fBDis)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MECLIPSEABSSP(%f,%f,%f,%f,%d,%f,%f)", fend1,fend2,fcenter1,fcenter2,idirection, fADis, fBDis);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //插补 椭圆 + 螺旋
Input:          //卡链接handle

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MEclipseHelical(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fADis, float fBDis, float fDistance3)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MECLIPSE(%f,%f,%f,%f,%d,%f,%f,%f)", fend1,fend2,fcenter1,fcenter2,idirection, fADis, fBDis, fDistance3);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //插补  椭圆 + 螺旋
Input:          //卡链接handle

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MEclipseHelicalAbs(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fADis, float fBDis,float fDistance3)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MECLIPSEABS(%f,%f,%f,%f,%d,%f,%f,%f)", fend1,fend2,fcenter1,fcenter2,idirection, fADis, fBDis,fDistance3);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //插补 椭圆 + 螺旋
Input:          //卡链接handle

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MEclipseHelicalSp(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fADis, float fBDis, float fDistance3)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MECLIPSESP(%f,%f,%f,%f,%d,%f,%f,%f)", fend1,fend2,fcenter1,fcenter2,idirection, fADis, fBDis, fDistance3);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //插补  椭圆 + 螺旋
Input:          //卡链接handle

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MEclipseHelicalAbsSp(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fADis, float fBDis,float fDistance3)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MECLIPSEABSSP(%f,%f,%f,%f,%d,%f,%f,%f)", fend1,fend2,fcenter1,fcenter2,idirection, fADis, fBDis,fDistance3);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


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
int32 __stdcall ZAux_MSpherical(ZMC_HANDLE handle, float fend1, float fend2, float fend3, float fcenter1, float fcenter2, float fcenter3, int imode)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MSPHERICAL(%f,%f,%f,%f,%f,%f,%d)", fend1,fend2,fend3,fcenter1,fcenter2,fcenter3,imode);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

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
int32 __stdcall ZAux_MSphericalSp(ZMC_HANDLE handle, float fend1, float fend2, float fend3, float fcenter1, float fcenter2, float fcenter3, int imode)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MSPHERICALSP(%f,%f,%f,%f,%f,%f,%d)", fend1,fend2,fend3,fcenter1,fcenter2,fcenter3,imode);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //运动暂停
Input:          //卡链接handle  

0（缺省） 暂停当前运动。 
1 在当前运动完成后正准备执行下一条运动指令时暂停。 
2 在当前运动完成后正准备执行下一条运动指令时，并且两条指令的MARK标识不一样时暂停。这个模式可以用于一个动作由多个指令来实现时，可以在一整个动作完成后暂停。 

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MovePause(ZMC_HANDLE handle, int imode)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MOVE_PAUSE(%d)", imode);


	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //运动暂停
Input:          //卡链接handle  

0（缺省） 暂停当前运动。 
1 在当前运动完成后正准备执行下一条运动指令时暂停。 
2 在当前运动完成后正准备执行下一条运动指令时，并且两条指令的MARK标识不一样时暂停。这个模式可以用于一个动作由多个指令来实现时，可以在一整个动作完成后暂停。 

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MoveResume(ZMC_HANDLE handle)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MOVE_RESUME");


	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //在运动缓冲中加入输出指令
Input:          //卡链接handle  

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MoveOp(ZMC_HANDLE handle, int ioutnum, int ivalue)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MOVE_OP(%d,%d)", ioutnum, ivalue);


	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //在运动缓冲中加入输出指令
Input:          //卡链接handle  

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MoveOpMulti(ZMC_HANDLE handle, int ioutnumfirst, int ioutnumend, int ivalue)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MOVE_OP(%d,%d,%d)", ioutnumfirst, ioutnumend, ivalue);


	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //在运动缓冲中加入输出指令
Input:          //卡链接handle  

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MoveOp2(ZMC_HANDLE handle, int ioutnum, int ivalue, int iofftimems)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MOVE_OP2(%d,%d,%d)", ioutnum, ivalue, iofftimems);


	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //在运动缓冲中加入延时指令
Input:          //卡链接handle  

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_MoveDelay(ZMC_HANDLE handle, int itimems)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MOVE_WA(%d)", itimems);


	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //电子凸轮
Input:          //卡链接handle  

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Cam(ZMC_HANDLE handle, int istartpoint, int iendpoint, float ftablemulti, float fDistance)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "CAM(%d,%d,%f,%f)", istartpoint,iendpoint,ftablemulti,fDistance);


	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //电子凸轮
Input:          //卡链接handle  

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Cambox(ZMC_HANDLE handle, int istartpoint, int iendpoint, float ftablemulti, float fDistance, int ilinkaxis, int ioption, float flinkstartpos)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "CAMBOX(%d,%d,%f,%f,%d,%d,%f)", istartpoint,iendpoint,ftablemulti,fDistance, ilinkaxis, ioption, flinkstartpos);


	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}



/*************************************************************
Description:    //连接 电子齿轮
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Connpath(ZMC_HANDLE handle, float fratio, int idringaxis)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "CONNPATH(%f,%d)", fratio, idringaxis);


	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //连接 电子齿轮
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Connect(ZMC_HANDLE handle, float fratio, int idringaxis)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "CONNECT(%f,%d)", fratio, idringaxis);


	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //连接 特殊凸轮
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Movelink(ZMC_HANDLE handle, float fDistance, float fLinkDis, float fLinkAcc, float fLinkDec,int iLinkaxis, int ioption, float flinkstartpos)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MOVELINK(%f,%f,%f,%f,%d,%d,%f)", fDistance, fLinkDis, fLinkAcc,fLinkDec,iLinkaxis,ioption,flinkstartpos);


	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //位置锁存指令
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Regist(ZMC_HANDLE handle, int imode)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "REGIST(%d)", imode);


	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //所有轴立即停止
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Rapidstop(ZMC_HANDLE handle, int imode)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "RAPIDSTOP(%d)", imode);


	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //SP指定速度的插补运动, 可以指定起始速度，最终速度， 运行速度
                  运动的轴是当前的BASE轴列表
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_LineNSp(ZMC_HANDLE handle, int imaxaxis, float *pfDisancelist, float fStartSpeed, float fEndSpeed, float fForceSpeed)
{
    int i;
    int32 iresult;

    if(0 == imaxaxis || imaxaxis > MAX_AXIS_AUX)
    {
        return ERR_AUX_PARAERR;
    }


	char  cmdbuff[2048];

    //生成 参数 命令
	sprintf(cmdbuff, "STARTMOVE_SPEED=%.4f\nENDMOVE_SPEED=%.4f\nFORCE_SPEED=%.4f", fStartSpeed, fEndSpeed, fForceSpeed);

	//调用命令执行函数
	iresult = ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
    if(ERR_OK != iresult)
    {
        return iresult;
    }
    
    //
    return ZAux_MoveSp(handle, imaxaxis, pfDisancelist);
    
}



#if 0
    //单轴运动
#endif

/*************************************************************
Description:    //连续运动
Input:          //卡链接handle  轴号， 方向
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Singl_Vmove(ZMC_HANDLE handle, int iaxis, int idir)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];

	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "VMOVE(%d) AXIS(%d)", idir, iaxis);


	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //单轴运动
Input:          //卡链接handle  轴号， 距离
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Singl_Move(ZMC_HANDLE handle, int iaxis, float fdistance)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];

	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "MOVE(%f) AXIS(%d)", fdistance, iaxis);


	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //单轴运动
Input:          //卡链接handle  轴号， 距离
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Singl_MoveAbs(ZMC_HANDLE handle, int iaxis, float fdistance)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];

	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "MOVEABS(%f) AXIS(%d)", fdistance, iaxis);


	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}



/*************************************************************
Description:    //单轴运动
Input:          //卡链接handle  轴号， 距离
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Singl_MoveModify(ZMC_HANDLE handle, int iaxis, float fdistance)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];

	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "MOVEMODIFY(%f) AXIS(%d)", fdistance, iaxis);


	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //叠加
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Singl_Addax(ZMC_HANDLE handle, int iaxis, int iaddaxis)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];

	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "ADDAX(%d) AXIS(%d)", iaddaxis, iaxis);


	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //cancel
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Singl_Cancel(ZMC_HANDLE handle, int iaxis, int imode)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "CANCEL(%d) AXIS(%d)", imode, iaxis);


	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //回零
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Singl_Datum(ZMC_HANDLE handle, int iaxis, int imode)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "DATUM(%d) AXIS(%d)", imode, iaxis);


	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //电子齿轮比设置
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Singl_EncoderRatio(ZMC_HANDLE handle, int iaxis, int imposcount, int inputcount)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "ENCODER_RATIO(%d,%d) AXIS(%d)", imposcount, inputcount, iaxis);


	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //电子齿轮比设置
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Singl_StepRatio(ZMC_HANDLE handle, int iaxis, int ioutcount, int idposcount)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "STEP_RATIO(%d,%d) AXIS(%d)", ioutcount, idposcount, iaxis);


	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}




#if 0
    //参数部分
#endif

/*************************************************************
Description:    //参数 轴类型设置， 步进轴一般不用设置
Input:          //卡链接handle  merge: 0/1
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetAtype(ZMC_HANDLE handle,int iaxis, int iset)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "ATYPE(%d)=%d", iaxis, iset);


	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}



/*************************************************************
Description:    //参数 设置是否连续插补
Input:          //卡链接handle  merge: 0/1
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetMerge(ZMC_HANDLE handle,int iaxis,  int iset)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "MERGE(%d)=%d",iaxis, iset);


	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //参数
Input:          //卡链接handle  merge: 0/1
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetUnits(ZMC_HANDLE handle, int iaxis, float fset)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "UNITS(%d)=%f", iaxis, fset);


	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}



/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetSpeed(ZMC_HANDLE handle, int iaxis, float fspeed)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "SPEED(%d)=%f", iaxis, fspeed);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}



/*************************************************************
Description:    //参数
Input:          //卡链接handle  merge: 0/1
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetAccel(ZMC_HANDLE handle, int iaxis, float fspeed)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "ACCEL(%d)=%f", iaxis, fspeed);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetDecel(ZMC_HANDLE handle, int iaxis, float fspeed)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "DECEL(%d)=%f", iaxis, fspeed);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}
int32 __stdcall ZAux_SetDatumin(ZMC_HANDLE handle, int iaxis, int id)
{
	int i;
	int32 iresult;

	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

	//生成命令
	sprintf(cmdbuff, "DATUM_IN(%d)=%d", iaxis, id);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}
/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetFastDec(ZMC_HANDLE handle, int iaxis, float fspeed)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "FASTDEC(%d)=%f", iaxis, fspeed);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetForceSpeed(ZMC_HANDLE handle, int iaxis, float fspeed)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "FORCE_SPEED(%d)=%f", iaxis, fspeed);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetStartMoveSpeed(ZMC_HANDLE handle, int iaxis, float fspeed)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "STARTMOVE_SPEED(%d)=%f", iaxis, fspeed);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetEndMoveSpeed(ZMC_HANDLE handle, int iaxis, float fspeed)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "ENDMOVE_SPEED(%d)=%f", iaxis, fspeed);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetLSpeed(ZMC_HANDLE handle, int iaxis, float fspeed)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "LSPEED(%d)=%f", iaxis, fspeed);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetCreepSpeed(ZMC_HANDLE handle, int iaxis, float fspeed)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "CREEP(%d)=%f", iaxis, fspeed);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //参数  拐角减速设置
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetCornerMode(ZMC_HANDLE handle, int iaxis, int iset)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "CORNER_MODE(%d)=%d", iaxis, iset);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //参数  脉冲的设置
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetInvertStep(ZMC_HANDLE handle, int iaxis, int iset)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "INVERT_STEP(%d)=%d", iaxis, iset);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}



/*************************************************************
Description:    //参数  S曲线设置
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetDecelAngle(ZMC_HANDLE handle, int iaxis, float fset)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "DECEL_ANGLE(%d)=%.3f", iaxis, fset);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //参数  S曲线设置
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetStopAngle(ZMC_HANDLE handle, int iaxis, float fset)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "STOP_ANGLE(%d)=%.3f", iaxis, fset);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //参数  小圆限速的半径设置
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetSpRadius(ZMC_HANDLE handle, int iaxis, float fset)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "FULL_SP_RADIUS(%d)=%.3f", iaxis, fset);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //参数  S曲线设置
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetSramp(ZMC_HANDLE handle, int iaxis, int iset)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "SRAMP(%d)=%d", iaxis, iset);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //参数  DPOS修改
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetDpos(ZMC_HANDLE handle, int iaxis, float fpos)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "DPOS(%d)=%f", iaxis, fpos);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //参数  MPOS修改
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetMpos(ZMC_HANDLE handle, int iaxis, float fpos)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "MPOS(%d)=%f", iaxis, fpos);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //参数  相对修改坐标
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetOffpos(ZMC_HANDLE handle, int iaxis, float fpos)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "OFFPOS(%d)=%f", iaxis, fpos);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}



#if 0
    //状态读取
#endif


/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetDpos(ZMC_HANDLE handle, int iaxis, float *pfValue)
{
	int i;
	float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != pfValue);
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "?DPOS(%d)", iaxis);

	//调用命令执行函数
	iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%f",pfValue);

	return ERR_OK;
}

/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetMpos(ZMC_HANDLE handle, int iaxis, float *pfValue)
{
	int i;
	float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != pfValue);
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "?MPOS(%d)", iaxis);

	//调用命令执行函数
	iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%f",pfValue);

	return ERR_OK;
}


/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetCurSpeed(ZMC_HANDLE handle, int iaxis, float *pfValue)
{
	int i;
	float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != pfValue);
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "?VP_SPEED(%d)", iaxis);

	//调用命令执行函数
	iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%f",pfValue);

	return ERR_OK;
}


/*************************************************************
Description:    //参数 当前的运动类型
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetMtype(ZMC_HANDLE handle, int iaxis, int *piValue)
{
	int i;
	float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != piValue);
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "?MTYPE(%d)", iaxis);

	//调用命令执行函数
	iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%d",piValue);

	return ERR_OK;
}

/*************************************************************
Description:    //参数  轴是否运动完成
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetIfIdle(ZMC_HANDLE handle, int iaxis, int *piValue)
{
	int i;
	float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != piValue);
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "?IDLE(%d)", iaxis);

	//调用命令执行函数
	iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%d",piValue);

	return ERR_OK;
}



/*************************************************************
Description:    //参数 当前运行完成后的终点坐标, 缓冲里面的不算
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetEndMove(ZMC_HANDLE handle, int iaxis, float *pfValue)
{
	int i;
	float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != pfValue);
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "?ENDMOVE(%d)", iaxis);

	//调用命令执行函数
	iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%f",pfValue);

	return ERR_OK;
}


/*************************************************************
Description:    //参数 当前缓冲运行完成后的终点坐标 用于转换相对绝对
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetEndMoveBuff(ZMC_HANDLE handle, int iaxis, float *pfValue)
{
	int i;
	float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != pfValue);
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "?ENDMOVE_BUFFER(%d)", iaxis);

	//调用命令执行函数
	iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%f",pfValue);

	return ERR_OK;
}

/*************************************************************
Description:    //参数 
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetAxisStatus(ZMC_HANDLE handle, int iaxis, int *piValue)
{
	int i;
	float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != piValue);
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "?AXISSTATUS(%d)", iaxis);

	//调用命令执行函数
	iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%d",piValue);

	return ERR_OK;
}


/*************************************************************
Description:    //参数  内部的编码器位置
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetEncoder(ZMC_HANDLE handle, int iaxis, int *piValue)
{
	int i;
	float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != piValue);
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "?ENCODER(%d)", iaxis);

	//调用命令执行函数
	iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%d",piValue);

	return ERR_OK;
}

/*************************************************************
Description:    //参数  轴剩余的缓冲, 按直线来计算
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetRemain_LineBuffer(ZMC_HANDLE handle, int iaxis, int *piValue)
{
	int i;
	float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != piValue);
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "?REMAIN_BUFFER(1) AXIS(%d)", iaxis);

	//调用命令执行函数
	iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%d",piValue);

	return ERR_OK;
}


/*************************************************************
Description:    //参数  轴剩余的缓冲, 按最复杂的空间圆弧来计算
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetRemain_Buffer(ZMC_HANDLE handle, int iaxis, int *piValue)
{
	int i;
	float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != piValue);
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "?REMAIN_BUFFER() AXIS(%d)", iaxis);

	//调用命令执行函数
	iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%d",piValue);

	return ERR_OK;
}



/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetRegPos(ZMC_HANDLE handle, int iaxis, float *pfValue)
{
	int i;
	float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != pfValue);
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "?REG_POS(%d)", iaxis);

	//调用命令执行函数
	iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%f",pfValue);

	return ERR_OK;
}

/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetMark(ZMC_HANDLE handle, int iaxis, int *piValue)
{
	int i;
	float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != piValue);
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "?MARK(%d)", iaxis);

	//调用命令执行函数
	iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%d",piValue);

	return ERR_OK;
}


/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetMarkB(ZMC_HANDLE handle, int iaxis, int *piValue)
{
	int i;
	float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != piValue);
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "?MARKB(%d)", iaxis);

	//调用命令执行函数
	iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%d",piValue);

	return ERR_OK;
}


/*************************************************************
Description:    //全局变量读取, 也可以是参数等等
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetVariablef(ZMC_HANDLE handle, const char *pname, float *pfValue)
{
	int i;
	float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != pfValue);

    //生成命令
	sprintf(cmdbuff, "?%s", pname);

	//调用命令执行函数
	iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	//iresult = ZMC_DirectCommand(handle, cmdbuff, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%f",pfValue);

	return ERR_OK;
}

/*************************************************************
Description:    //全局变量读取, 也可以是参数等等
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetVariableInt(ZMC_HANDLE handle, const char *pname, int *piValue)
{
	int i;
	//float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != piValue);

    //生成命令
	sprintf(cmdbuff, "?%s", pname);

	//调用命令执行函数
	iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	//iresult = ZMC_DirectCommand(handle, cmdbuff, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%d",piValue);

	return ERR_OK;
}


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
int32 __stdcall ZAux_GetIn(ZMC_HANDLE handle, int ionum, uint32 *piValue)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != piValue);

    //生成命令
	sprintf(cmdbuff, "?IN(%d)", ionum);

	//调用命令执行函数
	iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%d",piValue);

	return ERR_OK;
}

/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetInvertIN(ZMC_HANDLE handle, int ionum, int bifInvert)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "INVERT_IN(%d,%d)", ionum, bifInvert);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_SetOp(ZMC_HANDLE handle, int ionum, uint32 iValue)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "OP(%d,%d)", ionum, iValue);

	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

#if 0
    //通过modbus快速读取特殊寄存器
#endif

/*************************************************************
Description:    //参数 快速读取多个输入
Input:          //卡链接handle  
Output:         //按位存储
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetModbusIn(ZMC_HANDLE handle, int ionumfirst, int ionumend, uint8 *pValueList)
{
    if(ionumend < ionumfirst)
    {
        return ERR_AUX_PARAERR;
    }
    
    return ZMC_Modbus_Get0x(handle, 10000+ionumfirst, ionumend-ionumfirst +1, pValueList);
}

/*************************************************************
Description:    //参数 快速读取多个当前的输出状态
Input:          //卡链接handle  
Output:         //按位存储
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetModbusOut(ZMC_HANDLE handle, int ionumfirst, int ionumend, uint8 *pValueList)
{
    if(ionumend < ionumfirst)
    {
        return ERR_AUX_PARAERR;
    }
    
    return ZMC_Modbus_Get0x(handle, 20000+ionumfirst, ionumend-ionumfirst +1, pValueList);
}

/*************************************************************
Description:    //参数 快速读取多个当前的DPOS
Input:          //卡链接handle  
Output:         //按存储
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetModbusDpos(ZMC_HANDLE handle, int imaxaxises, float *pValueList)
{
    return ZMC_Modbus_Get4x(handle, 10000, imaxaxises*2, (uint16 *)pValueList);
}

/*************************************************************
Description:    //参数 快速读取多个当前的MPOS
Input:          //卡链接handle  
Output:         //按存储
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetModbusMpos(ZMC_HANDLE handle, int imaxaxises, float *pValueList)
{
    return ZMC_Modbus_Get4x(handle, 11000, imaxaxises*2, (uint16 *)pValueList);
}

/*************************************************************
Description:    //参数 快速读取多个当前的速度
Input:          //卡链接handle  
Output:         //按存储
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_GetModbusCurSpeed(ZMC_HANDLE handle, int imaxaxises, float *pValueList)
{
    return ZMC_Modbus_Get4x(handle, 12000, imaxaxises*2, (uint16 *)pValueList);
}

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
int32 __stdcall ZAux_Direct_GetDpos(ZMC_HANDLE handle, int iaxis, float *pfValue)
{
	int i;
	float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != pfValue);
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "?DPOS(%d)", iaxis);

	//调用命令执行函数
	//iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	iresult = ZMC_DirectCommand(handle, cmdbuff, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%f",pfValue);

	return ERR_OK;
}

/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Direct_GetMpos(ZMC_HANDLE handle, int iaxis, float *pfValue)
{
	int i;
	float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != pfValue);
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "?MPOS(%d)", iaxis);

	//调用命令执行函数
	//iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	iresult = ZMC_DirectCommand(handle, cmdbuff, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%f",pfValue);

	return ERR_OK;
}


/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Direct_GetCurSpeed(ZMC_HANDLE handle, int iaxis, float *pfValue)
{
	int i;
	float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != pfValue);
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "?VP_SPEED(%d)", iaxis);

	//调用命令执行函数
	//iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	iresult = ZMC_DirectCommand(handle, cmdbuff, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%f",pfValue);

	return ERR_OK;
}


/*************************************************************
Description:    //参数 当前的运动类型
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Direct_GetMtype(ZMC_HANDLE handle, int iaxis, int *piValue)
{
	int i;
	float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != piValue);
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "?MTYPE(%d)", iaxis);

	//调用命令执行函数
	//iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	iresult = ZMC_DirectCommand(handle, cmdbuff, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%d",piValue);

	return ERR_OK;
}

/*************************************************************
Description:    //参数  轴是否运动完成
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Direct_GetIfIdle(ZMC_HANDLE handle, int iaxis, int *piValue)
{
	int i;
	float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != piValue);
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "?IDLE(%d)", iaxis);

	//调用命令执行函数
	//iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	iresult = ZMC_DirectCommand(handle, cmdbuff, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%d",piValue);

	return ERR_OK;
}



/*************************************************************
Description:    //参数 当前运行完成后的终点坐标, 缓冲里面的不算
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Direct_GetEndMove(ZMC_HANDLE handle, int iaxis, float *pfValue)
{
	int i;
	float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != pfValue);
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "?ENDMOVE(%d)", iaxis);

	//调用命令执行函数
	//iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	iresult = ZMC_DirectCommand(handle, cmdbuff, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%f",pfValue);

	return ERR_OK;
}


/*************************************************************
Description:    //参数 当前缓冲运行完成后的终点坐标 用于转换相对绝对
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Direct_GetEndMoveBuff(ZMC_HANDLE handle, int iaxis, float *pfValue)
{
	int i;
	float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != pfValue);
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "?ENDMOVE_BUFFER(%d)", iaxis);

	//调用命令执行函数
	//iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	iresult = ZMC_DirectCommand(handle, cmdbuff, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%f",pfValue);

	return ERR_OK;
}

/*************************************************************
Description:    //参数 
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Direct_GetAxisStatus(ZMC_HANDLE handle, int iaxis, int *piValue)
{
	int i;
	float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != piValue);
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "?AXISSTATUS(%d)", iaxis);

	//调用命令执行函数
	//iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	iresult = ZMC_DirectCommand(handle, cmdbuff, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%d",piValue);

	return ERR_OK;
}


/*************************************************************
Description:    //参数  内部的编码器位置
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Direct_GetEncoder(ZMC_HANDLE handle, int iaxis, int *piValue)
{
	int i;
	float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != piValue);
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "?ENCODER(%d)", iaxis);

	//调用命令执行函数
	//iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	iresult = ZMC_DirectCommand(handle, cmdbuff, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%d",piValue);

	return ERR_OK;
}

/*************************************************************
Description:    //参数  轴剩余的缓冲, 按直线段来计算
				  REMAIN_BUFFER为唯一一个可以加AXIS并用ZMC_DirectCommand获取的.
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Direct_GetRemain_LineBuffer(ZMC_HANDLE handle, int iaxis, int *piValue)
{
	int i;
	float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != piValue);
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "?REMAIN_BUFFER(1) AXIS(%d)", iaxis);

	//调用命令执行函数
	//iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	iresult = ZMC_DirectCommand(handle, cmdbuff, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%d",piValue);

	return ERR_OK;
}


/*************************************************************
Description:    //参数  轴剩余的缓冲, 按最复杂的空间圆弧来计算
				  REMAIN_BUFFER为唯一一个可以加AXIS并用ZMC_DirectCommand获取的.
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Direct_GetRemain_Buffer(ZMC_HANDLE handle, int iaxis, int *piValue)
{
	int i;
	float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != piValue);
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "?REMAIN_BUFFER() AXIS(%d)", iaxis);

	//调用命令执行函数
	//iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	iresult = ZMC_DirectCommand(handle, cmdbuff, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%d",piValue);

	return ERR_OK;
}



/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Direct_GetRegPos(ZMC_HANDLE handle, int iaxis, float *pfValue)
{
	int i;
	float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != pfValue);
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "?REG_POS(%d)", iaxis);

	//调用命令执行函数
	//iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	iresult = ZMC_DirectCommand(handle, cmdbuff, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%f",pfValue);

	return ERR_OK;
}

/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Direct_GetMark(ZMC_HANDLE handle, int iaxis, int *piValue)
{
	int i;
	float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != piValue);
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "?MARK(%d)", iaxis);

	//调用命令执行函数
	//iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	iresult = ZMC_DirectCommand(handle, cmdbuff, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%d",piValue);

	return ERR_OK;
}


/*************************************************************
Description:    //参数
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Direct_GetMarkB(ZMC_HANDLE handle, int iaxis, int *piValue)
{
	int i;
	float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != piValue);
	ASSERT(iaxis < MAX_AXIS_AUX);

    //生成命令
	sprintf(cmdbuff, "?MARKB(%d)", iaxis);

	//调用命令执行函数
	//iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	iresult = ZMC_DirectCommand(handle, cmdbuff, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%d",piValue);

	return ERR_OK;
}


/*************************************************************
Description:    //全局变量读取, 也可以是参数等等
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Direct_GetVariablef(ZMC_HANDLE handle, const char *pname, float *pfValue)
{
	int i;
	float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != pfValue);

    //生成命令
	sprintf(cmdbuff, "?%s", pname);

	//调用命令执行函数
	//iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	iresult = ZMC_DirectCommand(handle, cmdbuff, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%f",pfValue);

	return ERR_OK;
}

/*************************************************************
Description:    //全局变量读取, 也可以是参数等等
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Direct_GetVariableInt(ZMC_HANDLE handle, const char *pname, int *piValue)
{
	int i;
	//float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != piValue);

    //生成命令
	sprintf(cmdbuff, "?%s", pname);

	//调用命令执行函数
	//iresult = ZMC_Execute(handle, cmdbuff, g_ZMC_MaxExcuteWaitms, cmdbuffAck, 2048);
	iresult = ZMC_DirectCommand(handle, cmdbuff, cmdbuffAck, 2048);
	if(ERR_OK != iresult)
	{
        return iresult;
	}

	//
	if(0 == strlen(cmdbuffAck))
	{
        return ERR_NOACK;
	}

	//
	sscanf(cmdbuffAck, "%d",piValue);

	return ERR_OK;
}




#if 0
    //辅助函数
#endif

/*************************************************************
Description:    //字符串转为float
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_TransStringtoFloat(const char* pstringin, int inumes,  float* pfvlaue)
{
    char *ptemp;

    ptemp = (char*)pstringin;
    while(' ' == *ptemp)
    {
        ptemp++;
    }
    
    if(!(isdigit(ptemp[0]) || ('-' == ptemp[0])))
    {
        return ERR_ACKERROR;
    }

    char *pstringnew = ptemp;
    
    //读取所有的轴的参数状态                
    for(int i =0; i < inumes;i++)
    {
    
        while((' ' == *pstringnew) || ('\t' == *pstringnew))
        {
            pstringnew++;
        }
        if(('\0' == pstringnew[0]) || ('\r' == pstringnew[0]) || ('\n' == pstringnew[0]) || !(isdigit(pstringnew[0]) || ('-' == pstringnew[0])))
        {
            break;
        }
    
        double dvalue = strtod(pstringnew, &ptemp);
        if((pstringnew == ptemp))
        {
            //break;
            return ERR_ACKERROR;
        }    

        pfvlaue[i] = dvalue;
        
        //跳过上次的
        pstringnew = ptemp;
    }


    return ERR_OK;
}

/*************************************************************
Description:    //字符串转为int
Input:          //卡链接handle  
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_TransStringtoInt(const char* pstringin, int inumes,  int* pivlaue)
{
    char *ptemp;

    ptemp = (char*)pstringin;
    while(' ' == *ptemp)
    {
        ptemp++;
    }
    
    if(!(isdigit(ptemp[0]) || ('-' == ptemp[0])))
    {
        return ERR_ACKERROR;
    }

    char *pstringnew = ptemp;
    
    //读取所有的轴的参数状态                
    for(int i =0; i < inumes;i++)
    {
    
        while((' ' == *pstringnew) || ('\t' == *pstringnew))
        {
            pstringnew++;
        }
        if(('\0' == pstringnew[0]) || ('\r' == pstringnew[0]) || ('\n' == pstringnew[0]) || !(isdigit(pstringnew[0]) || ('-' == pstringnew[0])))
        {
            break;
        }
    
        double dvalue = strtod(pstringnew, &ptemp);
        if((pstringnew == ptemp))
        {
            //break;
            return ERR_ACKERROR;
        }    

        pivlaue[i] = dvalue;    //转换成整数
        
        //跳过上次的
        pstringnew = ptemp;
    }


    return ERR_OK;
}



#if 0
    //额外的函数
    //合成特殊的函数
    //采用卡号来操作
#endif

//保存一些临时状态
struct_ZAuxCardSpecial ZAux_Spec_m_CardInfo[MAX_CARD_AUX];


//
/*************************************************************
Description:    //把卡的handle 与 卡号关联起来, 这样后面就可以直接用卡号来操作了
Input:          //卡链接handle
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_Init()
{
    int i,j;

    for(i=0; i< MAX_CARD_AUX; i++)
    {
        ZAux_Spec_m_CardInfo[i].m_cardnum = i;
        ZAux_Spec_m_CardInfo[i].m_handle= NULL;
        ZAux_Spec_m_CardInfo[i].m_iaxisHandle= -1;

        for(j=0; j< MAX_AXIS_AUX; j++)
        {
            ZAux_Spec_m_CardInfo[i].m_aSpeed[j]= 100;
            ZAux_Spec_m_CardInfo[i].m_aAccel[j]= 1000;            
            ZAux_Spec_m_CardInfo[i].m_aDecel[j]= 1000;
            
            ZAux_Spec_m_CardInfo[i].m_aSRampSet[j]= 0;  // 默认T
            
            ZAux_Spec_m_CardInfo[i].m_aHomeSpeed[j]= 30;
            ZAux_Spec_m_CardInfo[i].m_aHomeDir[j]= 1;
            ZAux_Spec_m_CardInfo[i].m_aHomeMode[j]= 2;  //回零反找
        }

        for(j=0; j< MAX_VECT_AUX_SPECIAL; j++)
        {
            ZAux_Spec_m_CardInfo[i].m_aVectSpeed[j]= 100;
            ZAux_Spec_m_CardInfo[i].m_aVectAccel[j]= 1000;            
            ZAux_Spec_m_CardInfo[i].m_aVectDecel[j]= 1000;
            
            ZAux_Spec_m_CardInfo[i].m_aVectSRampSet[j]= 0;  // 默认T
        
        }

    }

    return ERR_OK;
}


/*************************************************************
Description:    //把卡的handle 与 卡号关联起来, 这样后面就可以直接用卡号来操作了
Input:          //卡链接handle
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_SetCardHandle(WORD card, ZMC_HANDLE handle)
{
    if(card >= MAX_CARD_AUX)
    {
        return ERR_AUX_PARAERR;
    }

    //
    ZAux_Spec_m_CardInfo[card].m_cardnum = card;
    ZAux_Spec_m_CardInfo[card].m_handle= handle;

    return ERR_OK;
}

/*************************************************************
Description:    //把卡的handle 与 卡号关联起来, 这样后面就可以直接用卡号来操作了
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_UnSetCardHandle(WORD card)
{
    if(card >= MAX_CARD_AUX)
    {
        return ERR_AUX_PARAERR;
    }

    //
    ZAux_Spec_m_CardInfo[card].m_cardnum = card;
    ZAux_Spec_m_CardInfo[card].m_handle= NULL;

    return ERR_OK;
}



/*************************************************************
Description:    //单轴点位运动
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_GetTotalAxises(WORD card)
{
    if(card >= MAX_CARD_AUX)
    {
        return 0;
    }
    
    return ZMC_GetAxises(ZAux_Spec_m_CardInfo[card].m_handle);
}


/*************************************************************
Description:    // 设置轨迹的运动参数
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_SetVectProfile(WORD card, double spara, int32 speed, int32 acc, int32 dec )
{
    //
    if(card >= MAX_CARD_AUX)
    {
        return ERR_AUX_PARAERR;
    }
    
    ZAux_Spec_m_CardInfo[card].m_aVectSpeed[0]= speed;
    ZAux_Spec_m_CardInfo[card].m_aVectAccel[0]= acc;
    ZAux_Spec_m_CardInfo[card].m_aVectDecel[0]= dec;
    ZAux_Spec_m_CardInfo[card].m_aVectSRampSet[0]= spara;
    
    return ERR_OK;    
}


/*************************************************************
Description:    // 设置单轴的运动参数
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_SetProfile(WORD card, int iaxis, int32 speed, int32 acc, int32 dec)
{
    if(card >= MAX_CARD_AUX)
    {
        return ERR_AUX_PARAERR;
    }

    ZAux_Spec_m_CardInfo[card].m_aSpeed[0]= speed;
    ZAux_Spec_m_CardInfo[card].m_aAccel[0]= acc;
    ZAux_Spec_m_CardInfo[card].m_aDecel[0]= dec;
    //ZAux_Spec_m_CardInfo[card].m_aSRampSet[0]= spara;
    
    ZAux_SetSpeed(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, (float)speed);
    ZAux_SetAccel(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, (float)acc);
    ZAux_SetDecel(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, (float)speed);

    return ERR_OK;        
}


/*************************************************************
Description:    // 读取坐标
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_SetSProfile(WORD card, int iaxis, double spara)
{
    if(card >= MAX_CARD_AUX)
    {
        return ERR_AUX_PARAERR;
    }

    ZAux_Spec_m_CardInfo[card].m_aSRampSet[0]= spara;

    
    return ZAux_SetSramp(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, spara);
    
}



/*************************************************************
Description:    //单轴点位运动
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_PMove(WORD card, int iaxis, int Length)
{
    if(card >= MAX_CARD_AUX)
    {
        return ERR_AUX_PARAERR;
    }

    if(card >= MAX_CARD_AUX)
    {
        return ERR_AUX_PARAERR;
    }

    //设置为单轴的速度
    ZAux_SetSpeed(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, ZAux_Spec_m_CardInfo[card].m_aSpeed[iaxis]);

    //
    return ZAux_Singl_Move(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, (float)Length);

}

/*************************************************************
Description:    //单轴点位运动
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_PMoveAbs(WORD card, int iaxis, int Length)
{
    if(card >= MAX_CARD_AUX)
    {
        return ERR_AUX_PARAERR;
    }

    if(card >= MAX_CARD_AUX)
    {
        return ERR_AUX_PARAERR;
    }

    //速度
    ZAux_SetSpeed(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, ZAux_Spec_m_CardInfo[card].m_aSpeed[iaxis]);

    //
    return ZAux_Singl_MoveAbs(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, (float)Length);

}


/*************************************************************
Description:    //PMOVE时动态调整位置
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_ResetTargetPostion(WORD card, int iaxis, int Length)
{
    if(card >= MAX_CARD_AUX)
    {
        return ERR_AUX_PARAERR;
    }

    //
    return ZAux_Singl_MoveModify(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, (float)Length);

}




/*************************************************************
Description:    //SP指定速度的运动
Input:          //卡号   dir 1- 正向， 2-负向
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_VMove(WORD card, int iaxis, int dir)
{
    if(card >= MAX_CARD_AUX)
    {
        return ERR_AUX_PARAERR;
    }

    if(2 == dir)
    {
        dir = -1;
    }

    //速度
    ZAux_SetSpeed(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, ZAux_Spec_m_CardInfo[card].m_aSpeed[iaxis]);

    return ZAux_Singl_Vmove(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, dir);
}



/*************************************************************
Description:    // 脉冲模式设置
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_SetPulOutMode(WORD card, int iaxis, int iset)
{
    if(card >= MAX_CARD_AUX)
    {
        return ERR_AUX_PARAERR;
    }

    //
    return ZAux_SetInvertStep(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, iset);   

}



/*************************************************************
Description:    // 编码器模式设置，这个对应到不同的ATYPE，无法映射
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_SetCounterMode(WORD card, int iaxis, int iset)
{
    if(card >= MAX_CARD_AUX)
    {
        return ERR_AUX_PARAERR;
    }

    //
    return ERR_AUX_NOTSUPPORT;
}


/*************************************************************
Description:    // 手轮， 用编码器来模拟
Input:          //卡号  AB类型还是 脉冲方向 类型
                  这里只配置ATYPE， 当使用扩展轴时，ATYPE类型不一样.

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_SetHandle(WORD card, int iaxiscoder, int ifabmode)
{
    if(card >= MAX_CARD_AUX)
    {
        return ERR_AUX_PARAERR;
    }
    ZMC_HANDLE handle;

    handle = ZAux_Spec_m_CardInfo[card].m_handle;

    //保存手轮接入的轴
    ZAux_Spec_m_CardInfo[card].m_iaxisHandle = iaxiscoder;

    //
    if(ifabmode)
    {
        return ZAux_SetAtype(handle,iaxiscoder, 3);
    }
    else
    {
        return ZAux_SetAtype(handle,iaxiscoder, 6);        
    }

}


/*************************************************************
Description:    // 手轮， 用编码器来模拟
Input:          //卡号  必须先配置手轮的轴
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_HandMove(WORD card, int iaxis, float fratio)
{
    if(card >= MAX_CARD_AUX)
    {
        return ERR_AUX_PARAERR;
    }
    ZMC_HANDLE handle;

    handle = ZAux_Spec_m_CardInfo[card].m_handle;

    //
	char  cmdbuff[2048];

    //生成命令
	sprintf(cmdbuff, "CONNECT(%f,%d) AXIS(%d)", fratio,ZAux_Spec_m_CardInfo[card].m_iaxisHandle, iaxis);


	//调用命令执行函数
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);

}


/*************************************************************
Description:    // 减速停止， 立即停止时，把FASTDEC设置很大的值，再调用减速停止
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_DeclStop(WORD card, int iaxis)
{
    if(card >= MAX_CARD_AUX)
    {
        return ERR_AUX_PARAERR;
    }

    //
    return ZAux_Singl_Cancel(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, 2);   //取消当前的运动，缓冲的也取消
}

/*************************************************************
Description:    // 读取坐标
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_GetPosition(WORD card, int iaxis)
{
    float pos;

    if(card >= MAX_CARD_AUX)
    {
        return 0;
    }

    ZAux_GetDpos(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, &pos);

    return (int32)pos;
}

/*************************************************************
Description:    // 读取坐标
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_SetPosition(WORD card, int iaxis, int32 current_position)
{
    if(card >= MAX_CARD_AUX)
    {
        return ERR_AUX_PARAERR;
    }
    return ZAux_SetDpos(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, (float)current_position);
}


/*************************************************************
Description:    // 读取坐标
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_GetCurSpeed(WORD card, int iaxis)
{
    float fvalue;

    if(card >= MAX_CARD_AUX)
    {
        return 0;
    }

    ZAux_GetCurSpeed(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, &fvalue);

    return (int32)fvalue;
}

/*************************************************************
Description:    // 读取坐标
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_SetCurSpeed(WORD card, int iaxis, int32 speed)
{
    if(card >= MAX_CARD_AUX)
    {
        return ERR_AUX_PARAERR;
    }
    
    return ZAux_SetSpeed(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, (float)speed);
}


/*************************************************************
Description:    // 读取坐标
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_ConfigHomeMode(WORD card, int iaxis, int home_dir, int velspeed, int Mode)
{
    if(card >= MAX_CARD_AUX)
    {
        return ERR_AUX_PARAERR;
    }

    //保存速度
    ZAux_Spec_m_CardInfo[card].m_aHomeSpeed[iaxis] = velspeed;
    ZAux_Spec_m_CardInfo[card].m_aHomeDir[iaxis] = home_dir;
    ZAux_Spec_m_CardInfo[card].m_aHomeMode[iaxis] = Mode;


    return ERR_OK;    
}


/*************************************************************
Description:    // 读取坐标
Input:          //卡号
Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_HomeMove(WORD card, int iaxis)
{
    int32 iresult;
    
    if(card >= MAX_CARD_AUX)
    {
        return ERR_AUX_PARAERR;
    }

    //回零速度
    iresult = ZAux_SetSpeed(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, (float)ZAux_Spec_m_CardInfo[card].m_aHomeSpeed[iaxis]);
    if(ERR_OK != iresult)
    {
        return iresult;
    }

    //正向
    if(1 == ZAux_Spec_m_CardInfo[card].m_aHomeDir[iaxis])
    {
        //EZ回零
        if(ZAux_Spec_m_CardInfo[card].m_aHomeMode[iaxis] >= 10)
        {
            //
            return ZAux_Singl_Datum(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, 5);
        }
        else    //普通回零
        {
            return ZAux_Singl_Datum(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, 3);
        }
    }
    else    // 负向
    {
        //EZ回零
        if(ZAux_Spec_m_CardInfo[card].m_aHomeMode[iaxis] >= 10)
        {
            //
            return ZAux_Singl_Datum(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, 6);
        }
        else    //普通回零
        {
            return ZAux_Singl_Datum(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, 4);
        }
    }
    
}



/*************************************************************
Description:    // 减速停止 插补指令, 因为涉及到多个轴，必须修改BASE轴列表
                   注意连续插补的时候不要切换主轴,即BASE的第一个轴

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_Line2(WORD card, int iaxis1, int iaxis2, int distance1, int distance2)
{
    int32 iresult;
    int iaxises[2];
    float fDistancelist[2];
    ZMC_HANDLE handle;

    handle = ZAux_Spec_m_CardInfo[card].m_handle;
    iaxises[0] = iaxis1;
    iaxises[1] = iaxis2;
    fDistancelist[0] = distance1;
    fDistancelist[1] = distance2;
    
    //
    iresult = ZAux_Base(handle, 2, iaxises);
    if(ERR_OK != iresult)
    {
        return iresult;
    }

	char  cmdbuff[2048];
    //生成 参数 命令
	sprintf(cmdbuff, "SPEED=%.4f\nACCEL=%.4f\nDECEL=%.4f\nSRAMP=%.4f", (float)ZAux_Spec_m_CardInfo[card].m_aVectSpeed[0], 
	                            (float)ZAux_Spec_m_CardInfo[card].m_aVectAccel[0], 
            	                (float)ZAux_Spec_m_CardInfo[card].m_aVectDecel[0], 
            	                (float)ZAux_Spec_m_CardInfo[card].m_aVectSRampSet[0]);

	//调用命令执行函数
	iresult = ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
    if(ERR_OK != iresult)
    {
        return iresult;
    }

    //
    return ZAux_Move(handle, 2, fDistancelist);
    
}


/*************************************************************
Description:    // 减速停止 插补指令, 因为涉及到多个轴，必须修改BASE轴列表
                   注意连续插补的时候不要切换主轴,即BASE的第一个轴

Output:         //
Return:         //错误码
*************************************************************/
int32 __stdcall ZAux_Spec_LineN(WORD card, int imaxaxis, int* iaxisList, int *distanceList)
{
    int i;
    int32 iresult;
    float fDistancelist[MAX_AXIS_AUX];
    ZMC_HANDLE handle;

    if(0 == imaxaxis || imaxaxis > MAX_AXIS_AUX)
    {
        return ERR_AUX_PARAERR;
    }

    handle = ZAux_Spec_m_CardInfo[card].m_handle;
    for(i=0; i< imaxaxis; i++)
    {
        fDistancelist[i] = distanceList[i];
        
    }
    
    //
    iresult = ZAux_Base(handle, imaxaxis, iaxisList);
    if(ERR_OK != iresult)
    {
        return iresult;
    }


	char  cmdbuff[2048];
    //生成 参数 命令
	sprintf(cmdbuff, "SPEED=%.4f\nACCEL=%.4f\nDECEL=%.4f\nSRAMP=%.4f", ZAux_Spec_m_CardInfo[card].m_aVectSpeed[0], 
	                ZAux_Spec_m_CardInfo[card].m_aVectAccel[0], ZAux_Spec_m_CardInfo[card].m_aVectDecel[0], ZAux_Spec_m_CardInfo[card].m_aVectSRampSet[0]);

	//调用命令执行函数
	iresult = ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
    if(ERR_OK != iresult)
    {
        return iresult;
    }

    //
    return ZAux_Move(handle, imaxaxis, fDistancelist);
    
}


#endif