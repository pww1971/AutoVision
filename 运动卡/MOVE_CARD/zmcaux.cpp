
/********************************** ZMCϵ�п�����  ************************************************
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**�ļ���: zmcaux.c
**������: ֣Т��
**ʱ��: 20130621
**����: ZMCDLL ��������

�����ṩ��EXCUTE����������ִ�к����ķ�װ�����Կ����������ִ�г�������޸Ĳ���.



**------------�޶���ʷ��¼----------------------------------------------------------------------------
** �޸���: 
** ��  ��: 
** �ա���: 
** �衡��: 
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
    //�����˶�����
#endif


/*************************************************************
Description:    //BASEָ�����

�����޸����������BASE�б����Կ����������������BASE�����޸�.
�޸ĺ󣬺���������MOVE��ָ��������BASEΪ����

Input:          //������handle
Output:         //
Return:         //������
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

    //��������
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

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //����DPOS
Input:          //������handle
Output:         //
Return:         //������
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

    //��������
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

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //��Բ岹
Input:          //������handle
Output:         //
Return:         //������
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

    //��������
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

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //��Բ岹
Input:          //������handle
Output:         //
Return:         //������
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

    //��������
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

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //�岹
Input:          //������handle
Output:         //
Return:         //������
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

    //��������
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

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //�岹
Input:          //������handle
Output:         //
Return:         //������
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

    //��������
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

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //�岹
Input:          //������handle
Output:         //
Return:         //������
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

    //��������
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

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //�岹
Input:          //������handle

end1              ��һ�����˶�����
end2              �ڶ������˶�����
centre1    ��һ�����˶�Բ�ģ��������ʼ�㡣
centre2    �ڶ������˶�Բ�ģ��������ʼ�㡣
direction  0-��ʱ�룬1-˳ʱ��

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MoveCirc(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MOVECIRC(%f,%f,%f,%f,%d)", fend1,fend2,fcenter1,fcenter2,idirection);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //�岹
Input:          //������handle

end1              ��һ�����˶�����
end2              �ڶ������˶�����
centre1    ��һ�����˶�Բ�ģ��������ʼ�㡣
centre2    �ڶ������˶�Բ�ģ��������ʼ�㡣
direction  0-��ʱ�룬1-˳ʱ��

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MoveCircSp(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MOVECIRCSP(%f,%f,%f,%f,%d)", fend1,fend2,fcenter1,fcenter2,idirection);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //�岹
Input:          //������handle

end1              ��һ�����˶����꣬����λ��
end2              �ڶ������˶����꣬����λ��
centre1    ��һ�����˶�Բ�ģ�����λ��
centre2    �ڶ������˶�Բ�ģ�����λ��
direction  0-��ʱ�룬1-˳ʱ��

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MoveCircAbs(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MOVECIRCABS(%f,%f,%f,%f,%d)", fend1,fend2,fcenter1,fcenter2,idirection);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //�岹
Input:          //������handle

end1              ��һ�����˶����꣬����λ��
end2              �ڶ������˶����꣬����λ��
centre1    ��һ�����˶�Բ�ģ�����λ��
centre2    �ڶ������˶�Բ�ģ�����λ��
direction  0-��ʱ�룬1-˳ʱ��

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MoveCircAbsSp(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MOVECIRCABSSP(%f,%f,%f,%f,%d)", fend1,fend2,fcenter1,fcenter2,idirection);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}



/*************************************************************
Description:    //�岹
Input:          //������handle

mid1       ��һ�����м�㣬�����ʼ�����
mid2       �ڶ������м�㣬�����ʼ�����
end1              ��һ��������㣬�����ʼ�����
end2              �ڶ���������㣬�����ʼ�����

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MoveCirc2(ZMC_HANDLE handle, float fmid1, float fmid2, float fend1, float fend2)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MOVECIRC2(%f,%f,%f,%f)", fmid1,fmid2,fend1,fend2);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //�岹
Input:          //������handle

mid1       ��һ�����м�㣬����λ��
mid2       �ڶ������м�㣬����λ��
end1              ��һ��������㣬����λ��
end2              �ڶ���������㣬����λ��

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MoveCirc2Abs(ZMC_HANDLE handle, float fmid1, float fmid2, float fend1, float fend2)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MOVECIRC2ABS(%f,%f,%f,%f)", fmid1,fmid2,fend1,fend2);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //�岹
Input:          //������handle

mid1       ��һ�����м�㣬�����ʼ�����
mid2       �ڶ������м�㣬�����ʼ�����
end1              ��һ��������㣬�����ʼ�����
end2              �ڶ���������㣬�����ʼ�����

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MoveCirc2Sp(ZMC_HANDLE handle, float fmid1, float fmid2, float fend1, float fend2)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MOVECIRC2SP(%f,%f,%f,%f)", fmid1,fmid2,fend1,fend2);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //�岹
Input:          //������handle

mid1       ��һ�����м�㣬����λ��
mid2       �ڶ������м�㣬����λ��
end1              ��һ��������㣬����λ��
end2              �ڶ���������㣬����λ��

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MoveCirc2AbsSp(ZMC_HANDLE handle, float fmid1, float fmid2, float fend1, float fend2)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MOVECIRC2ABSSP(%f,%f,%f,%f)", fmid1,fmid2,fend1,fend2);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //�岹
Input:          //������handle

end1              ��һ�����˶�����

end2              �ڶ������˶�����

centre1    ��һ�����˶�Բ�ģ��������ʼ��

centre2    �ڶ������˶�Բ�ģ��������ʼ��

direction  0-��ʱ�룬1-˳ʱ��

distance3���������˶����롣

mode      ��������ٶȼ���:
0(ȱʡ)
 ����������ٶȼ��㡣
1
 �����᲻�����ٶȼ��㡣
 

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MHelical(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fDistance3, int imode)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MHELICAL(%f,%f,%f,%f,%d,%f,%d)", fend1,fend2,fcenter1,fcenter2,idirection, fDistance3, imode);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //�岹
Input:          //������handle

end1              ��һ�����˶�����

end2              �ڶ������˶�����

centre1    ��һ�����˶�Բ�ģ��������ʼ��

centre2    �ڶ������˶�Բ�ģ��������ʼ��

direction  0-��ʱ�룬1-˳ʱ��

distance3���������˶����롣

mode      ��������ٶȼ���:
0(ȱʡ)
 ����������ٶȼ��㡣
1
 �����᲻�����ٶȼ��㡣
 

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MHelicalAbs(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fDistance3, int imode)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MHELICALABS(%f,%f,%f,%f,%d,%f,%d)", fend1,fend2,fcenter1,fcenter2,idirection, fDistance3, imode);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //�岹
Input:          //������handle

end1              ��һ�����˶�����

end2              �ڶ������˶�����

centre1    ��һ�����˶�Բ�ģ��������ʼ��

centre2    �ڶ������˶�Բ�ģ��������ʼ��

direction  0-��ʱ�룬1-˳ʱ��

distance3���������˶����롣

mode      ��������ٶȼ���:
0(ȱʡ)
 ����������ٶȼ��㡣
1
 �����᲻�����ٶȼ��㡣
 

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MHelicalSp(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fDistance3, int imode)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MHELICALSP(%f,%f,%f,%f,%d,%f,%d)", fend1,fend2,fcenter1,fcenter2,idirection, fDistance3, imode);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //�岹
Input:          //������handle

end1              ��һ�����˶�����

end2              �ڶ������˶�����

centre1    ��һ�����˶�Բ�ģ��������ʼ��

centre2    �ڶ������˶�Բ�ģ��������ʼ��

direction  0-��ʱ�룬1-˳ʱ��

distance3���������˶����롣

mode      ��������ٶȼ���:
0(ȱʡ)
 ����������ٶȼ��㡣
1
 �����᲻�����ٶȼ��㡣
 

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MHelicalAbsSp(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fDistance3, int imode)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MHELICALABSSP(%f,%f,%f,%f,%d,%f,%d)", fend1,fend2,fcenter1,fcenter2,idirection, fDistance3, imode);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //�岹
Input:          //������handle

mid1       ��һ�����м��

mid2       �ڶ������м��

end1              ��һ���������

end2              �ڶ����������

distance3���������˶�����

mode      ��������ٶȼ���:

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MHelical2(ZMC_HANDLE handle, float fmid1, float fmid2, float fend1, float fend2, float fDistance3, int imode)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MHELICAL2(%f,%f,%f,%f,%f,%d)", fmid1,fmid2,fend1,fend2, fDistance3, imode);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //�岹
Input:          //������handle

mid1       ��һ�����м��

mid2       �ڶ������м��

end1              ��һ���������

end2              �ڶ����������

distance3   ���������˶�������

mode      ��������ٶȼ���:

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MHelical2Abs(ZMC_HANDLE handle, float fmid1, float fmid2, float fend1, float fend2, float fDistance3, int imode)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MHELICAL2ABS(%f,%f,%f,%f,%f,%d)", fmid1,fmid2,fend1,fend2, fDistance3, imode);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //�岹
Input:          //������handle

mid1       ��һ�����м��

mid2       �ڶ������м��

end1              ��һ���������

end2              �ڶ����������

distance3���������˶�����

mode      ��������ٶȼ���:

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MHelical2Sp(ZMC_HANDLE handle, float fmid1, float fmid2, float fend1, float fend2, float fDistance3, int imode)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MHELICAL2SP(%f,%f,%f,%f,%f,%d)", fmid1,fmid2,fend1,fend2, fDistance3, imode);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //�岹
Input:          //������handle

mid1       ��һ�����м��

mid2       �ڶ������м��

end1              ��һ���������

end2              �ڶ����������

distance3   ���������˶�������

mode      ��������ٶȼ���:

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MHelical2AbsSp(ZMC_HANDLE handle, float fmid1, float fmid2, float fend1, float fend2, float fDistance3, int imode)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MHELICAL2ABSSP(%f,%f,%f,%f,%f,%d)", fmid1,fmid2,fend1,fend2, fDistance3, imode);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}



/*************************************************************
Description:    //�岹
Input:          //������handle

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MEclipse(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fADis, float fBDis)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MECLIPSE(%f,%f,%f,%f,%d,%f,%f)", fend1,fend2,fcenter1,fcenter2,idirection, fADis, fBDis);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //�岹
Input:          //������handle

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MEclipseAbs(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fADis, float fBDis)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MECLIPSEABS(%f,%f,%f,%f,%d,%f,%f)", fend1,fend2,fcenter1,fcenter2,idirection, fADis, fBDis);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //�岹
Input:          //������handle

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MEclipseSp(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fADis, float fBDis)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MECLIPSESP(%f,%f,%f,%f,%d,%f,%f)", fend1,fend2,fcenter1,fcenter2,idirection, fADis, fBDis);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //�岹
Input:          //������handle

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MEclipseAbsSp(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fADis, float fBDis)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MECLIPSEABSSP(%f,%f,%f,%f,%d,%f,%f)", fend1,fend2,fcenter1,fcenter2,idirection, fADis, fBDis);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //�岹 ��Բ + ����
Input:          //������handle

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MEclipseHelical(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fADis, float fBDis, float fDistance3)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MECLIPSE(%f,%f,%f,%f,%d,%f,%f,%f)", fend1,fend2,fcenter1,fcenter2,idirection, fADis, fBDis, fDistance3);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //�岹  ��Բ + ����
Input:          //������handle

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MEclipseHelicalAbs(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fADis, float fBDis,float fDistance3)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MECLIPSEABS(%f,%f,%f,%f,%d,%f,%f,%f)", fend1,fend2,fcenter1,fcenter2,idirection, fADis, fBDis,fDistance3);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //�岹 ��Բ + ����
Input:          //������handle

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MEclipseHelicalSp(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fADis, float fBDis, float fDistance3)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MECLIPSESP(%f,%f,%f,%f,%d,%f,%f,%f)", fend1,fend2,fcenter1,fcenter2,idirection, fADis, fBDis, fDistance3);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //�岹  ��Բ + ����
Input:          //������handle

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MEclipseHelicalAbsSp(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fADis, float fBDis,float fDistance3)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MECLIPSEABSSP(%f,%f,%f,%f,%d,%f,%f,%f)", fend1,fend2,fcenter1,fcenter2,idirection, fADis, fBDis,fDistance3);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //�岹  �ռ�Բ��
Input:          //������handle
end1              ��1�����˶��������1

end2              ��2�����˶��������1

end3              ��3�����˶��������1

centre1    ��1�����˶��������2

centre2    ��2�����˶��������2

centre3    ��3�����˶��������2

mode      ָ��ǰ�����������
0 ��ǰ�㣬�м�㣬�յ����㶨Բ�����������1Ϊ�յ���룬�������2Ϊ�м����롣
1 ����С��Բ�����������1Ϊ�յ���룬�������2ΪԲ�ĵľ��롣
2 ��ǰ�㣬�м�㣬�յ����㶨Բ���������1Ϊ�յ���룬�������2Ϊ�м����롣
3 ������С��Բ�����ټ���������Բ���������1Ϊ�յ���룬�������2ΪԲ�ĵľ��롣
 

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MSpherical(ZMC_HANDLE handle, float fend1, float fend2, float fend3, float fcenter1, float fcenter2, float fcenter3, int imode)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MSPHERICAL(%f,%f,%f,%f,%f,%f,%d)", fend1,fend2,fend3,fcenter1,fcenter2,fcenter3,imode);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //�岹  �ռ�Բ��
Input:          //������handle
end1              ��1�����˶��������1

end2              ��2�����˶��������1

end3              ��3�����˶��������1

centre1    ��1�����˶��������2

centre2    ��2�����˶��������2

centre3    ��3�����˶��������2

mode      ָ��ǰ�����������
0 ��ǰ�㣬�м�㣬�յ����㶨Բ�����������1Ϊ�յ���룬�������2Ϊ�м����롣
1 ����С��Բ�����������1Ϊ�յ���룬�������2ΪԲ�ĵľ��롣
2 ��ǰ�㣬�м�㣬�յ����㶨Բ���������1Ϊ�յ���룬�������2Ϊ�м����롣
3 ������С��Բ�����ټ���������Բ���������1Ϊ�յ���룬�������2ΪԲ�ĵľ��롣
 

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MSphericalSp(ZMC_HANDLE handle, float fend1, float fend2, float fend3, float fcenter1, float fcenter2, float fcenter3, int imode)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MSPHERICALSP(%f,%f,%f,%f,%f,%f,%d)", fend1,fend2,fend3,fcenter1,fcenter2,fcenter3,imode);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //�˶���ͣ
Input:          //������handle  

0��ȱʡ�� ��ͣ��ǰ�˶��� 
1 �ڵ�ǰ�˶���ɺ���׼��ִ����һ���˶�ָ��ʱ��ͣ�� 
2 �ڵ�ǰ�˶���ɺ���׼��ִ����һ���˶�ָ��ʱ����������ָ���MARK��ʶ��һ��ʱ��ͣ�����ģʽ��������һ�������ɶ��ָ����ʵ��ʱ��������һ����������ɺ���ͣ�� 

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MovePause(ZMC_HANDLE handle, int imode)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MOVE_PAUSE(%d)", imode);


	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //�˶���ͣ
Input:          //������handle  

0��ȱʡ�� ��ͣ��ǰ�˶��� 
1 �ڵ�ǰ�˶���ɺ���׼��ִ����һ���˶�ָ��ʱ��ͣ�� 
2 �ڵ�ǰ�˶���ɺ���׼��ִ����һ���˶�ָ��ʱ����������ָ���MARK��ʶ��һ��ʱ��ͣ�����ģʽ��������һ�������ɶ��ָ����ʵ��ʱ��������һ����������ɺ���ͣ�� 

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MoveResume(ZMC_HANDLE handle)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MOVE_RESUME");


	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //���˶������м������ָ��
Input:          //������handle  

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MoveOp(ZMC_HANDLE handle, int ioutnum, int ivalue)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MOVE_OP(%d,%d)", ioutnum, ivalue);


	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //���˶������м������ָ��
Input:          //������handle  

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MoveOpMulti(ZMC_HANDLE handle, int ioutnumfirst, int ioutnumend, int ivalue)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MOVE_OP(%d,%d,%d)", ioutnumfirst, ioutnumend, ivalue);


	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //���˶������м������ָ��
Input:          //������handle  

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MoveOp2(ZMC_HANDLE handle, int ioutnum, int ivalue, int iofftimems)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MOVE_OP2(%d,%d,%d)", ioutnum, ivalue, iofftimems);


	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //���˶������м�����ʱָ��
Input:          //������handle  

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MoveDelay(ZMC_HANDLE handle, int itimems)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MOVE_WA(%d)", itimems);


	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //����͹��
Input:          //������handle  

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Cam(ZMC_HANDLE handle, int istartpoint, int iendpoint, float ftablemulti, float fDistance)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "CAM(%d,%d,%f,%f)", istartpoint,iendpoint,ftablemulti,fDistance);


	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //����͹��
Input:          //������handle  

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Cambox(ZMC_HANDLE handle, int istartpoint, int iendpoint, float ftablemulti, float fDistance, int ilinkaxis, int ioption, float flinkstartpos)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "CAMBOX(%d,%d,%f,%f,%d,%d,%f)", istartpoint,iendpoint,ftablemulti,fDistance, ilinkaxis, ioption, flinkstartpos);


	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}



/*************************************************************
Description:    //���� ���ӳ���
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Connpath(ZMC_HANDLE handle, float fratio, int idringaxis)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "CONNPATH(%f,%d)", fratio, idringaxis);


	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //���� ���ӳ���
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Connect(ZMC_HANDLE handle, float fratio, int idringaxis)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "CONNECT(%f,%d)", fratio, idringaxis);


	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //���� ����͹��
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Movelink(ZMC_HANDLE handle, float fDistance, float fLinkDis, float fLinkAcc, float fLinkDec,int iLinkaxis, int ioption, float flinkstartpos)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MOVELINK(%f,%f,%f,%f,%d,%d,%f)", fDistance, fLinkDis, fLinkAcc,fLinkDec,iLinkaxis,ioption,flinkstartpos);


	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //λ������ָ��
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Regist(ZMC_HANDLE handle, int imode)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "REGIST(%d)", imode);


	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //����������ֹͣ
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Rapidstop(ZMC_HANDLE handle, int imode)
{
	//int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "RAPIDSTOP(%d)", imode);


	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //SPָ���ٶȵĲ岹�˶�, ����ָ����ʼ�ٶȣ������ٶȣ� �����ٶ�
                  �˶������ǵ�ǰ��BASE���б�
Input:          //����
Output:         //
Return:         //������
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

    //���� ���� ����
	sprintf(cmdbuff, "STARTMOVE_SPEED=%.4f\nENDMOVE_SPEED=%.4f\nFORCE_SPEED=%.4f", fStartSpeed, fEndSpeed, fForceSpeed);

	//��������ִ�к���
	iresult = ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
    if(ERR_OK != iresult)
    {
        return iresult;
    }
    
    //
    return ZAux_MoveSp(handle, imaxaxis, pfDisancelist);
    
}



#if 0
    //�����˶�
#endif

/*************************************************************
Description:    //�����˶�
Input:          //������handle  ��ţ� ����
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Singl_Vmove(ZMC_HANDLE handle, int iaxis, int idir)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];

	ASSERT(iaxis < MAX_AXIS_AUX);

    //��������
	sprintf(cmdbuff, "VMOVE(%d) AXIS(%d)", idir, iaxis);


	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //�����˶�
Input:          //������handle  ��ţ� ����
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Singl_Move(ZMC_HANDLE handle, int iaxis, float fdistance)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];

	ASSERT(iaxis < MAX_AXIS_AUX);

    //��������
	sprintf(cmdbuff, "MOVE(%f) AXIS(%d)", fdistance, iaxis);


	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //�����˶�
Input:          //������handle  ��ţ� ����
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Singl_MoveAbs(ZMC_HANDLE handle, int iaxis, float fdistance)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];

	ASSERT(iaxis < MAX_AXIS_AUX);

    //��������
	sprintf(cmdbuff, "MOVEABS(%f) AXIS(%d)", fdistance, iaxis);


	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}



/*************************************************************
Description:    //�����˶�
Input:          //������handle  ��ţ� ����
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Singl_MoveModify(ZMC_HANDLE handle, int iaxis, float fdistance)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];

	ASSERT(iaxis < MAX_AXIS_AUX);

    //��������
	sprintf(cmdbuff, "MOVEMODIFY(%f) AXIS(%d)", fdistance, iaxis);


	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Singl_Addax(ZMC_HANDLE handle, int iaxis, int iaddaxis)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];

	ASSERT(iaxis < MAX_AXIS_AUX);

    //��������
	sprintf(cmdbuff, "ADDAX(%d) AXIS(%d)", iaddaxis, iaxis);


	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //cancel
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Singl_Cancel(ZMC_HANDLE handle, int iaxis, int imode)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //��������
	sprintf(cmdbuff, "CANCEL(%d) AXIS(%d)", imode, iaxis);


	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Singl_Datum(ZMC_HANDLE handle, int iaxis, int imode)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //��������
	sprintf(cmdbuff, "DATUM(%d) AXIS(%d)", imode, iaxis);


	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //���ӳ��ֱ�����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Singl_EncoderRatio(ZMC_HANDLE handle, int iaxis, int imposcount, int inputcount)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //��������
	sprintf(cmdbuff, "ENCODER_RATIO(%d,%d) AXIS(%d)", imposcount, inputcount, iaxis);


	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //���ӳ��ֱ�����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Singl_StepRatio(ZMC_HANDLE handle, int iaxis, int ioutcount, int idposcount)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //��������
	sprintf(cmdbuff, "STEP_RATIO(%d,%d) AXIS(%d)", ioutcount, idposcount, iaxis);


	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}




#if 0
    //��������
#endif

/*************************************************************
Description:    //���� ���������ã� ������һ�㲻������
Input:          //������handle  merge: 0/1
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetAtype(ZMC_HANDLE handle,int iaxis, int iset)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "ATYPE(%d)=%d", iaxis, iset);


	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}



/*************************************************************
Description:    //���� �����Ƿ������岹
Input:          //������handle  merge: 0/1
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetMerge(ZMC_HANDLE handle,int iaxis,  int iset)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "MERGE(%d)=%d",iaxis, iset);


	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //����
Input:          //������handle  merge: 0/1
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetUnits(ZMC_HANDLE handle, int iaxis, float fset)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "UNITS(%d)=%f", iaxis, fset);


	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}



/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetSpeed(ZMC_HANDLE handle, int iaxis, float fspeed)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //��������
	sprintf(cmdbuff, "SPEED(%d)=%f", iaxis, fspeed);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}



/*************************************************************
Description:    //����
Input:          //������handle  merge: 0/1
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetAccel(ZMC_HANDLE handle, int iaxis, float fspeed)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //��������
	sprintf(cmdbuff, "ACCEL(%d)=%f", iaxis, fspeed);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetDecel(ZMC_HANDLE handle, int iaxis, float fspeed)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //��������
	sprintf(cmdbuff, "DECEL(%d)=%f", iaxis, fspeed);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}
int32 __stdcall ZAux_SetDatumin(ZMC_HANDLE handle, int iaxis, int id)
{
	int i;
	int32 iresult;

	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

	//��������
	sprintf(cmdbuff, "DATUM_IN(%d)=%d", iaxis, id);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}
/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetFastDec(ZMC_HANDLE handle, int iaxis, float fspeed)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //��������
	sprintf(cmdbuff, "FASTDEC(%d)=%f", iaxis, fspeed);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetForceSpeed(ZMC_HANDLE handle, int iaxis, float fspeed)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //��������
	sprintf(cmdbuff, "FORCE_SPEED(%d)=%f", iaxis, fspeed);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetStartMoveSpeed(ZMC_HANDLE handle, int iaxis, float fspeed)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //��������
	sprintf(cmdbuff, "STARTMOVE_SPEED(%d)=%f", iaxis, fspeed);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetEndMoveSpeed(ZMC_HANDLE handle, int iaxis, float fspeed)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //��������
	sprintf(cmdbuff, "ENDMOVE_SPEED(%d)=%f", iaxis, fspeed);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetLSpeed(ZMC_HANDLE handle, int iaxis, float fspeed)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //��������
	sprintf(cmdbuff, "LSPEED(%d)=%f", iaxis, fspeed);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetCreepSpeed(ZMC_HANDLE handle, int iaxis, float fspeed)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //��������
	sprintf(cmdbuff, "CREEP(%d)=%f", iaxis, fspeed);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //����  �սǼ�������
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetCornerMode(ZMC_HANDLE handle, int iaxis, int iset)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //��������
	sprintf(cmdbuff, "CORNER_MODE(%d)=%d", iaxis, iset);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //����  ���������
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetInvertStep(ZMC_HANDLE handle, int iaxis, int iset)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //��������
	sprintf(cmdbuff, "INVERT_STEP(%d)=%d", iaxis, iset);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}



/*************************************************************
Description:    //����  S��������
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetDecelAngle(ZMC_HANDLE handle, int iaxis, float fset)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //��������
	sprintf(cmdbuff, "DECEL_ANGLE(%d)=%.3f", iaxis, fset);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //����  S��������
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetStopAngle(ZMC_HANDLE handle, int iaxis, float fset)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //��������
	sprintf(cmdbuff, "STOP_ANGLE(%d)=%.3f", iaxis, fset);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //����  СԲ���ٵİ뾶����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetSpRadius(ZMC_HANDLE handle, int iaxis, float fset)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //��������
	sprintf(cmdbuff, "FULL_SP_RADIUS(%d)=%.3f", iaxis, fset);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //����  S��������
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetSramp(ZMC_HANDLE handle, int iaxis, int iset)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //��������
	sprintf(cmdbuff, "SRAMP(%d)=%d", iaxis, iset);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //����  DPOS�޸�
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetDpos(ZMC_HANDLE handle, int iaxis, float fpos)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //��������
	sprintf(cmdbuff, "DPOS(%d)=%f", iaxis, fpos);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

/*************************************************************
Description:    //����  MPOS�޸�
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetMpos(ZMC_HANDLE handle, int iaxis, float fpos)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //��������
	sprintf(cmdbuff, "MPOS(%d)=%f", iaxis, fpos);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //����  ����޸�����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetOffpos(ZMC_HANDLE handle, int iaxis, float fpos)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	ASSERT(iaxis < MAX_AXIS_AUX);

    //��������
	sprintf(cmdbuff, "OFFPOS(%d)=%f", iaxis, fpos);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}



#if 0
    //״̬��ȡ
#endif


/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
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

    //��������
	sprintf(cmdbuff, "?DPOS(%d)", iaxis);

	//��������ִ�к���
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
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
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

    //��������
	sprintf(cmdbuff, "?MPOS(%d)", iaxis);

	//��������ִ�к���
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
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
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

    //��������
	sprintf(cmdbuff, "?VP_SPEED(%d)", iaxis);

	//��������ִ�к���
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
Description:    //���� ��ǰ���˶�����
Input:          //������handle  
Output:         //
Return:         //������
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

    //��������
	sprintf(cmdbuff, "?MTYPE(%d)", iaxis);

	//��������ִ�к���
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
Description:    //����  ���Ƿ��˶����
Input:          //������handle  
Output:         //
Return:         //������
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

    //��������
	sprintf(cmdbuff, "?IDLE(%d)", iaxis);

	//��������ִ�к���
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
Description:    //���� ��ǰ������ɺ���յ�����, ��������Ĳ���
Input:          //������handle  
Output:         //
Return:         //������
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

    //��������
	sprintf(cmdbuff, "?ENDMOVE(%d)", iaxis);

	//��������ִ�к���
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
Description:    //���� ��ǰ����������ɺ���յ����� ����ת����Ծ���
Input:          //������handle  
Output:         //
Return:         //������
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

    //��������
	sprintf(cmdbuff, "?ENDMOVE_BUFFER(%d)", iaxis);

	//��������ִ�к���
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
Description:    //���� 
Input:          //������handle  
Output:         //
Return:         //������
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

    //��������
	sprintf(cmdbuff, "?AXISSTATUS(%d)", iaxis);

	//��������ִ�к���
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
Description:    //����  �ڲ��ı�����λ��
Input:          //������handle  
Output:         //
Return:         //������
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

    //��������
	sprintf(cmdbuff, "?ENCODER(%d)", iaxis);

	//��������ִ�к���
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
Description:    //����  ��ʣ��Ļ���, ��ֱ��������
Input:          //������handle  
Output:         //
Return:         //������
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

    //��������
	sprintf(cmdbuff, "?REMAIN_BUFFER(1) AXIS(%d)", iaxis);

	//��������ִ�к���
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
Description:    //����  ��ʣ��Ļ���, ����ӵĿռ�Բ��������
Input:          //������handle  
Output:         //
Return:         //������
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

    //��������
	sprintf(cmdbuff, "?REMAIN_BUFFER() AXIS(%d)", iaxis);

	//��������ִ�к���
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
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
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

    //��������
	sprintf(cmdbuff, "?REG_POS(%d)", iaxis);

	//��������ִ�к���
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
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
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

    //��������
	sprintf(cmdbuff, "?MARK(%d)", iaxis);

	//��������ִ�к���
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
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
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

    //��������
	sprintf(cmdbuff, "?MARKB(%d)", iaxis);

	//��������ִ�к���
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
Description:    //ȫ�ֱ�����ȡ, Ҳ�����ǲ����ȵ�
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_GetVariablef(ZMC_HANDLE handle, const char *pname, float *pfValue)
{
	int i;
	float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != pfValue);

    //��������
	sprintf(cmdbuff, "?%s", pname);

	//��������ִ�к���
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
Description:    //ȫ�ֱ�����ȡ, Ҳ�����ǲ����ȵ�
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_GetVariableInt(ZMC_HANDLE handle, const char *pname, int *piValue)
{
	int i;
	//float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != piValue);

    //��������
	sprintf(cmdbuff, "?%s", pname);

	//��������ִ�к���
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
    //IOָ��
    // ����ʹ�� ZMC_GetIn ZMC_GetOutput ��
#endif


/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_GetIn(ZMC_HANDLE handle, int ionum, uint32 *piValue)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != piValue);

    //��������
	sprintf(cmdbuff, "?IN(%d)", ionum);

	//��������ִ�к���
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
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetInvertIN(ZMC_HANDLE handle, int ionum, int bifInvert)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "INVERT_IN(%d,%d)", ionum, bifInvert);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}


/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetOp(ZMC_HANDLE handle, int ionum, uint32 iValue)
{
	int i;
	int32 iresult;
	
	char  cmdbuff[2048];

    //��������
	sprintf(cmdbuff, "OP(%d,%d)", ionum, iValue);

	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
}

#if 0
    //ͨ��modbus���ٶ�ȡ����Ĵ���
#endif

/*************************************************************
Description:    //���� ���ٶ�ȡ�������
Input:          //������handle  
Output:         //��λ�洢
Return:         //������
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
Description:    //���� ���ٶ�ȡ�����ǰ�����״̬
Input:          //������handle  
Output:         //��λ�洢
Return:         //������
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
Description:    //���� ���ٶ�ȡ�����ǰ��DPOS
Input:          //������handle  
Output:         //���洢
Return:         //������
*************************************************************/
int32 __stdcall ZAux_GetModbusDpos(ZMC_HANDLE handle, int imaxaxises, float *pValueList)
{
    return ZMC_Modbus_Get4x(handle, 10000, imaxaxises*2, (uint16 *)pValueList);
}

/*************************************************************
Description:    //���� ���ٶ�ȡ�����ǰ��MPOS
Input:          //������handle  
Output:         //���洢
Return:         //������
*************************************************************/
int32 __stdcall ZAux_GetModbusMpos(ZMC_HANDLE handle, int imaxaxises, float *pValueList)
{
    return ZMC_Modbus_Get4x(handle, 11000, imaxaxises*2, (uint16 *)pValueList);
}

/*************************************************************
Description:    //���� ���ٶ�ȡ�����ǰ���ٶ�
Input:          //������handle  
Output:         //���洢
Return:         //������
*************************************************************/
int32 __stdcall ZAux_GetModbusCurSpeed(ZMC_HANDLE handle, int imaxaxises, float *pValueList)
{
    return ZMC_Modbus_Get4x(handle, 12000, imaxaxises*2, (uint16 *)pValueList);
}

#if 0
    //����ZMC_DirectCommand �����ٻ�ȡһЩ״̬, ZMC_DirectCommand��ִ�б�ZMC_ExecuteҪ��
	// ֻ�в���������������Ԫ�ص���ʹ��ZMC_DirectCommand
#endif

/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
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

    //��������
	sprintf(cmdbuff, "?DPOS(%d)", iaxis);

	//��������ִ�к���
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
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
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

    //��������
	sprintf(cmdbuff, "?MPOS(%d)", iaxis);

	//��������ִ�к���
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
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
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

    //��������
	sprintf(cmdbuff, "?VP_SPEED(%d)", iaxis);

	//��������ִ�к���
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
Description:    //���� ��ǰ���˶�����
Input:          //������handle  
Output:         //
Return:         //������
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

    //��������
	sprintf(cmdbuff, "?MTYPE(%d)", iaxis);

	//��������ִ�к���
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
Description:    //����  ���Ƿ��˶����
Input:          //������handle  
Output:         //
Return:         //������
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

    //��������
	sprintf(cmdbuff, "?IDLE(%d)", iaxis);

	//��������ִ�к���
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
Description:    //���� ��ǰ������ɺ���յ�����, ��������Ĳ���
Input:          //������handle  
Output:         //
Return:         //������
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

    //��������
	sprintf(cmdbuff, "?ENDMOVE(%d)", iaxis);

	//��������ִ�к���
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
Description:    //���� ��ǰ����������ɺ���յ����� ����ת����Ծ���
Input:          //������handle  
Output:         //
Return:         //������
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

    //��������
	sprintf(cmdbuff, "?ENDMOVE_BUFFER(%d)", iaxis);

	//��������ִ�к���
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
Description:    //���� 
Input:          //������handle  
Output:         //
Return:         //������
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

    //��������
	sprintf(cmdbuff, "?AXISSTATUS(%d)", iaxis);

	//��������ִ�к���
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
Description:    //����  �ڲ��ı�����λ��
Input:          //������handle  
Output:         //
Return:         //������
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

    //��������
	sprintf(cmdbuff, "?ENCODER(%d)", iaxis);

	//��������ִ�к���
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
Description:    //����  ��ʣ��Ļ���, ��ֱ�߶�������
				  REMAIN_BUFFERΪΨһһ�����Լ�AXIS����ZMC_DirectCommand��ȡ��.
Input:          //������handle  
Output:         //
Return:         //������
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

    //��������
	sprintf(cmdbuff, "?REMAIN_BUFFER(1) AXIS(%d)", iaxis);

	//��������ִ�к���
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
Description:    //����  ��ʣ��Ļ���, ����ӵĿռ�Բ��������
				  REMAIN_BUFFERΪΨһһ�����Լ�AXIS����ZMC_DirectCommand��ȡ��.
Input:          //������handle  
Output:         //
Return:         //������
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

    //��������
	sprintf(cmdbuff, "?REMAIN_BUFFER() AXIS(%d)", iaxis);

	//��������ִ�к���
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
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
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

    //��������
	sprintf(cmdbuff, "?REG_POS(%d)", iaxis);

	//��������ִ�к���
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
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
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

    //��������
	sprintf(cmdbuff, "?MARK(%d)", iaxis);

	//��������ִ�к���
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
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
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

    //��������
	sprintf(cmdbuff, "?MARKB(%d)", iaxis);

	//��������ִ�к���
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
Description:    //ȫ�ֱ�����ȡ, Ҳ�����ǲ����ȵ�
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Direct_GetVariablef(ZMC_HANDLE handle, const char *pname, float *pfValue)
{
	int i;
	float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != pfValue);

    //��������
	sprintf(cmdbuff, "?%s", pname);

	//��������ִ�к���
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
Description:    //ȫ�ֱ�����ȡ, Ҳ�����ǲ����ȵ�
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Direct_GetVariableInt(ZMC_HANDLE handle, const char *pname, int *piValue)
{
	int i;
	//float ftemp;
	int32 iresult;
	
	char  cmdbuff[2048];
	char  cmdbuffAck[2048];

    ASSERT(NULL != piValue);

    //��������
	sprintf(cmdbuff, "?%s", pname);

	//��������ִ�к���
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
    //��������
#endif

/*************************************************************
Description:    //�ַ���תΪfloat
Input:          //������handle  
Output:         //
Return:         //������
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
    
    //��ȡ���е���Ĳ���״̬                
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
        
        //�����ϴε�
        pstringnew = ptemp;
    }


    return ERR_OK;
}

/*************************************************************
Description:    //�ַ���תΪint
Input:          //������handle  
Output:         //
Return:         //������
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
    
    //��ȡ���е���Ĳ���״̬                
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

        pivlaue[i] = dvalue;    //ת��������
        
        //�����ϴε�
        pstringnew = ptemp;
    }


    return ERR_OK;
}



#if 0
    //����ĺ���
    //�ϳ�����ĺ���
    //���ÿ���������
#endif

//����һЩ��ʱ״̬
struct_ZAuxCardSpecial ZAux_Spec_m_CardInfo[MAX_CARD_AUX];


//
/*************************************************************
Description:    //�ѿ���handle �� ���Ź�������, ��������Ϳ���ֱ���ÿ�����������
Input:          //������handle
Output:         //
Return:         //������
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
            
            ZAux_Spec_m_CardInfo[i].m_aSRampSet[j]= 0;  // Ĭ��T
            
            ZAux_Spec_m_CardInfo[i].m_aHomeSpeed[j]= 30;
            ZAux_Spec_m_CardInfo[i].m_aHomeDir[j]= 1;
            ZAux_Spec_m_CardInfo[i].m_aHomeMode[j]= 2;  //���㷴��
        }

        for(j=0; j< MAX_VECT_AUX_SPECIAL; j++)
        {
            ZAux_Spec_m_CardInfo[i].m_aVectSpeed[j]= 100;
            ZAux_Spec_m_CardInfo[i].m_aVectAccel[j]= 1000;            
            ZAux_Spec_m_CardInfo[i].m_aVectDecel[j]= 1000;
            
            ZAux_Spec_m_CardInfo[i].m_aVectSRampSet[j]= 0;  // Ĭ��T
        
        }

    }

    return ERR_OK;
}


/*************************************************************
Description:    //�ѿ���handle �� ���Ź�������, ��������Ϳ���ֱ���ÿ�����������
Input:          //������handle
Output:         //
Return:         //������
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
Description:    //�ѿ���handle �� ���Ź�������, ��������Ϳ���ֱ���ÿ�����������
Input:          //����
Output:         //
Return:         //������
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
Description:    //�����λ�˶�
Input:          //����
Output:         //
Return:         //������
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
Description:    // ���ù켣���˶�����
Input:          //����
Output:         //
Return:         //������
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
Description:    // ���õ�����˶�����
Input:          //����
Output:         //
Return:         //������
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
Description:    // ��ȡ����
Input:          //����
Output:         //
Return:         //������
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
Description:    //�����λ�˶�
Input:          //����
Output:         //
Return:         //������
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

    //����Ϊ������ٶ�
    ZAux_SetSpeed(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, ZAux_Spec_m_CardInfo[card].m_aSpeed[iaxis]);

    //
    return ZAux_Singl_Move(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, (float)Length);

}

/*************************************************************
Description:    //�����λ�˶�
Input:          //����
Output:         //
Return:         //������
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

    //�ٶ�
    ZAux_SetSpeed(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, ZAux_Spec_m_CardInfo[card].m_aSpeed[iaxis]);

    //
    return ZAux_Singl_MoveAbs(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, (float)Length);

}


/*************************************************************
Description:    //PMOVEʱ��̬����λ��
Input:          //����
Output:         //
Return:         //������
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
Description:    //SPָ���ٶȵ��˶�
Input:          //����   dir 1- ���� 2-����
Output:         //
Return:         //������
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

    //�ٶ�
    ZAux_SetSpeed(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, ZAux_Spec_m_CardInfo[card].m_aSpeed[iaxis]);

    return ZAux_Singl_Vmove(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, dir);
}



/*************************************************************
Description:    // ����ģʽ����
Input:          //����
Output:         //
Return:         //������
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
Description:    // ������ģʽ���ã������Ӧ����ͬ��ATYPE���޷�ӳ��
Input:          //����
Output:         //
Return:         //������
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
Description:    // ���֣� �ñ�������ģ��
Input:          //����  AB���ͻ��� ���巽�� ����
                  ����ֻ����ATYPE�� ��ʹ����չ��ʱ��ATYPE���Ͳ�һ��.

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Spec_SetHandle(WORD card, int iaxiscoder, int ifabmode)
{
    if(card >= MAX_CARD_AUX)
    {
        return ERR_AUX_PARAERR;
    }
    ZMC_HANDLE handle;

    handle = ZAux_Spec_m_CardInfo[card].m_handle;

    //�������ֽ������
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
Description:    // ���֣� �ñ�������ģ��
Input:          //����  �������������ֵ���
Output:         //
Return:         //������
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

    //��������
	sprintf(cmdbuff, "CONNECT(%f,%d) AXIS(%d)", fratio,ZAux_Spec_m_CardInfo[card].m_iaxisHandle, iaxis);


	//��������ִ�к���
	return ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);

}


/*************************************************************
Description:    // ����ֹͣ�� ����ֹͣʱ����FASTDEC���úܴ��ֵ���ٵ��ü���ֹͣ
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Spec_DeclStop(WORD card, int iaxis)
{
    if(card >= MAX_CARD_AUX)
    {
        return ERR_AUX_PARAERR;
    }

    //
    return ZAux_Singl_Cancel(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, 2);   //ȡ����ǰ���˶��������Ҳȡ��
}

/*************************************************************
Description:    // ��ȡ����
Input:          //����
Output:         //
Return:         //������
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
Description:    // ��ȡ����
Input:          //����
Output:         //
Return:         //������
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
Description:    // ��ȡ����
Input:          //����
Output:         //
Return:         //������
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
Description:    // ��ȡ����
Input:          //����
Output:         //
Return:         //������
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
Description:    // ��ȡ����
Input:          //����
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Spec_ConfigHomeMode(WORD card, int iaxis, int home_dir, int velspeed, int Mode)
{
    if(card >= MAX_CARD_AUX)
    {
        return ERR_AUX_PARAERR;
    }

    //�����ٶ�
    ZAux_Spec_m_CardInfo[card].m_aHomeSpeed[iaxis] = velspeed;
    ZAux_Spec_m_CardInfo[card].m_aHomeDir[iaxis] = home_dir;
    ZAux_Spec_m_CardInfo[card].m_aHomeMode[iaxis] = Mode;


    return ERR_OK;    
}


/*************************************************************
Description:    // ��ȡ����
Input:          //����
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Spec_HomeMove(WORD card, int iaxis)
{
    int32 iresult;
    
    if(card >= MAX_CARD_AUX)
    {
        return ERR_AUX_PARAERR;
    }

    //�����ٶ�
    iresult = ZAux_SetSpeed(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, (float)ZAux_Spec_m_CardInfo[card].m_aHomeSpeed[iaxis]);
    if(ERR_OK != iresult)
    {
        return iresult;
    }

    //����
    if(1 == ZAux_Spec_m_CardInfo[card].m_aHomeDir[iaxis])
    {
        //EZ����
        if(ZAux_Spec_m_CardInfo[card].m_aHomeMode[iaxis] >= 10)
        {
            //
            return ZAux_Singl_Datum(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, 5);
        }
        else    //��ͨ����
        {
            return ZAux_Singl_Datum(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, 3);
        }
    }
    else    // ����
    {
        //EZ����
        if(ZAux_Spec_m_CardInfo[card].m_aHomeMode[iaxis] >= 10)
        {
            //
            return ZAux_Singl_Datum(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, 6);
        }
        else    //��ͨ����
        {
            return ZAux_Singl_Datum(ZAux_Spec_m_CardInfo[card].m_handle, iaxis, 4);
        }
    }
    
}



/*************************************************************
Description:    // ����ֹͣ �岹ָ��, ��Ϊ�漰������ᣬ�����޸�BASE���б�
                   ע�������岹��ʱ��Ҫ�л�����,��BASE�ĵ�һ����

Output:         //
Return:         //������
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
    //���� ���� ����
	sprintf(cmdbuff, "SPEED=%.4f\nACCEL=%.4f\nDECEL=%.4f\nSRAMP=%.4f", (float)ZAux_Spec_m_CardInfo[card].m_aVectSpeed[0], 
	                            (float)ZAux_Spec_m_CardInfo[card].m_aVectAccel[0], 
            	                (float)ZAux_Spec_m_CardInfo[card].m_aVectDecel[0], 
            	                (float)ZAux_Spec_m_CardInfo[card].m_aVectSRampSet[0]);

	//��������ִ�к���
	iresult = ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
    if(ERR_OK != iresult)
    {
        return iresult;
    }

    //
    return ZAux_Move(handle, 2, fDistancelist);
    
}


/*************************************************************
Description:    // ����ֹͣ �岹ָ��, ��Ϊ�漰������ᣬ�����޸�BASE���б�
                   ע�������岹��ʱ��Ҫ�л�����,��BASE�ĵ�һ����

Output:         //
Return:         //������
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
    //���� ���� ����
	sprintf(cmdbuff, "SPEED=%.4f\nACCEL=%.4f\nDECEL=%.4f\nSRAMP=%.4f", ZAux_Spec_m_CardInfo[card].m_aVectSpeed[0], 
	                ZAux_Spec_m_CardInfo[card].m_aVectAccel[0], ZAux_Spec_m_CardInfo[card].m_aVectDecel[0], ZAux_Spec_m_CardInfo[card].m_aVectSRampSet[0]);

	//��������ִ�к���
	iresult = ZMC_ExecuteNoAck(handle, cmdbuff, g_ZMC_MaxExcuteWaitms);
    if(ERR_OK != iresult)
    {
        return iresult;
    }

    //
    return ZAux_Move(handle, imaxaxis, fDistancelist);
    
}


#endif