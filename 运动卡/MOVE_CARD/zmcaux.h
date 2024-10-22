/********************************** ZMCϵ�п�����  ************************************************
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**�ļ���: zmcaux.h
**������: ֣Т��
**ʱ��: 20130621
**����: ZMCDLL ��������

**------------�޶���ʷ��¼----------------------------------------------------------------------------
** �޸���: 
** ��  ��: 
** �ա���: 
** �衡��: 
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/




#ifndef _ZMOTION_DLL_AUX_H
#define _ZMOTION_DLL_AUX_H


//ZAUX֧�ֵ����������
#define MAX_AXIS_AUX   32  
#define MAX_CARD_AUX   16







/*********************************************************
�������Ͷ���
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
typedef unsigned char  uint8;                   /* defined for unsigned 8-bits integer variable     �޷���8λ���ͱ���  */
typedef signed   char  int8;                    /* defined for signed 8-bits integer variable        �з���8λ���ͱ���  */
typedef unsigned short uint16;                  /* defined for unsigned 16-bits integer variable     �޷���16λ���ͱ��� */
typedef signed   short int16;                   /* defined for signed 16-bits integer variable         �з���16λ���ͱ��� */
typedef unsigned int   uint32;                  /* defined for unsigned 32-bits integer variable     �޷���32λ���ͱ��� */
typedef signed   int   int32;                   /* defined for signed 32-bits integer variable         �з���32λ���ͱ��� */
typedef float          fp32;                    /* single precision floating point variable (32bits) �����ȸ�������32λ���ȣ� */
typedef double         fp64;                    /* double precision floating point variable (64bits) ˫���ȸ�������64λ���ȣ� */
typedef unsigned int   uint;                  /* defined for unsigned 32-bits integer variable     �޷���32λ���ͱ��� */



/************************************************/
//������ 
/************************************************/
#define ERR_OK  0
#define ERROR_OK 0
#define ERR_SUCCESS  0

#define ERR_AUX_OFFSET       30000

#define ERR_NOACK               ERR_AUX_OFFSET      //��Ӧ��
#define ERR_ACKERROR            (ERR_AUX_OFFSET+1)  //Ӧ�����
#define ERR_AUX_PARAERR         (ERR_AUX_OFFSET+2)  //��������
#define ERR_AUX_NOTSUPPORT      (ERR_AUX_OFFSET+3)  //��������



/*********************************************************
��������
**********************************************************/

/*************************************************************
Description:    //BASEָ�����

�����޸����������BASE�б����Կ����������������BASE�����޸�.
�޸ĺ󣬺���������MOVE��ָ��������BASEΪ����

Input:          //������handle
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Base(ZMC_HANDLE handle, int imaxaxises, int *piAxislist);


/*************************************************************
Description:    //����DPOS
Input:          //������handle
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Defpos(ZMC_HANDLE handle, int imaxaxises, float *pfDposlist);



/*************************************************************
Description:    //��Բ岹
Input:          //������handle
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Move(ZMC_HANDLE handle, int imaxaxises, float *pfDisancelist);

/*************************************************************
Description:    //��Բ岹
Input:          //������handle
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MoveSp(ZMC_HANDLE handle, int imaxaxises, float *pfDisancelist);


/*************************************************************
Description:    //�岹
Input:          //������handle
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MoveAbs(ZMC_HANDLE handle, int imaxaxises, float *pfDisancelist);

/*************************************************************
Description:    //�岹
Input:          //������handle
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MoveAbsSp(ZMC_HANDLE handle, int imaxaxises, float *pfDisancelist);


/*************************************************************
Description:    //�岹
Input:          //������handle
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MoveModify(ZMC_HANDLE handle, int imaxaxises, float *pfDisancelist);



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
int32 __stdcall ZAux_MoveCirc(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection);

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
int32 __stdcall ZAux_MoveCircSp(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection);


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
int32 __stdcall ZAux_MoveCircAbs(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection);

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
int32 __stdcall ZAux_MoveCircAbsSp(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection);


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
int32 __stdcall ZAux_MoveCirc2(ZMC_HANDLE handle, float fmid1, float fmid2, float fend1, float fend2);

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
int32 __stdcall ZAux_MoveCirc2Sp(ZMC_HANDLE handle, float fmid1, float fmid2, float fend1, float fend2);


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
int32 __stdcall ZAux_MoveCirc2AbsSp(ZMC_HANDLE handle, float fmid1, float fmid2, float fend1, float fend2);


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
int32 __stdcall ZAux_MoveCirc2Abs(ZMC_HANDLE handle, float fmid1, float fmid2, float fend1, float fend2);


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
int32 __stdcall ZAux_MHelical(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fDistance3, int imode);


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
int32 __stdcall ZAux_MHelicalAbs(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fDistance3, int imode);


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
int32 __stdcall ZAux_MHelicalSp(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fDistance3, int imode);

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
int32 __stdcall ZAux_MHelicalAbsSp(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fDistance3, int imode);


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
int32 __stdcall ZAux_MHelical2(ZMC_HANDLE handle, float fmid1, float fmid2, float fend1, float fend2, float fDistance3, int imode);


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
int32 __stdcall ZAux_MHelical2Abs(ZMC_HANDLE handle, float fmid1, float fmid2, float fend1, float fend2, float fDistance3, int imode);

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
int32 __stdcall ZAux_MHelical2Sp(ZMC_HANDLE handle, float fmid1, float fmid2, float fend1, float fend2, float fDistance3, int imode);

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
int32 __stdcall ZAux_MHelical2AbsSp(ZMC_HANDLE handle, float fmid1, float fmid2, float fend1, float fend2, float fDistance3, int imode);


/*************************************************************
Description:    //�岹
Input:          //������handle

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MEclipse(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fADis, float fBDis);


/*************************************************************
Description:    //�岹
Input:          //������handle

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MEclipseAbs(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fADis, float fBDis);



/*************************************************************
Description:    //�岹
Input:          //������handle

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MEclipseSp(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fADis, float fBDis);


/*************************************************************
Description:    //�岹
Input:          //������handle

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MEclipseAbsSp(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fADis, float fBDis);


/*************************************************************
Description:    //�岹 ��Բ + ����
Input:          //������handle

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MEclipseHelical(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fADis, float fBDis, float fDistance3);


/*************************************************************
Description:    //�岹  ��Բ + ����
Input:          //������handle

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MEclipseHelicalAbs(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fADis, float fBDis,float fDistance3);


/*************************************************************
Description:    //�岹 ��Բ + ����
Input:          //������handle

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MEclipseHelicalSp(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fADis, float fBDis, float fDistance3);


/*************************************************************
Description:    //�岹  ��Բ + ����
Input:          //������handle

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MEclipseHelicalAbsSp(ZMC_HANDLE handle, float fend1, float fend2, float fcenter1, float fcenter2, int idirection, float fADis, float fBDis,float fDistance3);


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
int32 __stdcall ZAux_MSpherical(ZMC_HANDLE handle, float fend1, float fend2, float fend3, float fcenter1, float fcenter2, float fcenter3, int imode);


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
int32 __stdcall ZAux_MSphericalSp(ZMC_HANDLE handle, float fend1, float fend2, float fend3, float fcenter1, float fcenter2, float fcenter3, int imode);


/*************************************************************
Description:    //�˶���ͣ
Input:          //������handle  

0��ȱʡ�� ��ͣ��ǰ�˶��� 
1 �ڵ�ǰ�˶���ɺ���׼��ִ����һ���˶�ָ��ʱ��ͣ�� 
2 �ڵ�ǰ�˶���ɺ���׼��ִ����һ���˶�ָ��ʱ����������ָ���MARK��ʶ��һ��ʱ��ͣ�����ģʽ��������һ�������ɶ��ָ����ʵ��ʱ��������һ����������ɺ���ͣ�� 

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MovePause(ZMC_HANDLE handle, int imode);


/*************************************************************
Description:    //�˶���ͣ
Input:          //������handle  

0��ȱʡ�� ��ͣ��ǰ�˶��� 
1 �ڵ�ǰ�˶���ɺ���׼��ִ����һ���˶�ָ��ʱ��ͣ�� 
2 �ڵ�ǰ�˶���ɺ���׼��ִ����һ���˶�ָ��ʱ����������ָ���MARK��ʶ��һ��ʱ��ͣ�����ģʽ��������һ�������ɶ��ָ����ʵ��ʱ��������һ����������ɺ���ͣ�� 

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MoveResume(ZMC_HANDLE handle);



/*************************************************************
Description:    //���˶������м������ָ��
Input:          //������handle  

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MoveOp(ZMC_HANDLE handle, int ioutnum, int ivalue);


/*************************************************************
Description:    //���˶������м������ָ��
Input:          //������handle  

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MoveOpMulti(ZMC_HANDLE handle, int ioutnumfirst, int ioutnumend, int ivalue);



/*************************************************************
Description:    //���˶������м������ָ��
Input:          //������handle  

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MoveOp2(ZMC_HANDLE handle, int ioutnum, int ivalue, int iofftimems);


/*************************************************************
Description:    //���˶������м�����ʱָ��
Input:          //������handle  

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_MoveDelay(ZMC_HANDLE handle, int itimems);


/*************************************************************
Description:    //����͹��
Input:          //������handle  

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Cam(ZMC_HANDLE handle, int istartpoint, int iendpoint, float ftablemulti, float fDistance);


/*************************************************************
Description:    //����͹��
Input:          //������handle  

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Cambox(ZMC_HANDLE handle, int istartpoint, int iendpoint, float ftablemulti, float fDistance, int ilinkaxis, int ioption, float flinkstartpos);




/*************************************************************
Description:    //���� ���ӳ���
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Connpath(ZMC_HANDLE handle, float fratio, int idringaxis);


/*************************************************************
Description:    //���� ���ӳ���
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Connect(ZMC_HANDLE handle, float fratio, int idringaxis);



/*************************************************************
Description:    //���� ����͹��
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Movelink(ZMC_HANDLE handle, float fDistance, float fLinkDis, float fLinkAcc, float fLinkDec,int iLinkaxis, int ioption, float flinkstartpos);


/*************************************************************
Description:    //λ������ָ��
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Regist(ZMC_HANDLE handle, int imode);



/*************************************************************
Description:    //����������ֹͣ
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Rapidstop(ZMC_HANDLE handle, int imode);

/*************************************************************
Description:    //SPָ���ٶȵĲ岹�˶�, ����ָ����ʼ�ٶȣ������ٶȣ� �����ٶ�
                  �˶������ǵ�ǰ��BASE���б�
Input:          //����
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_LineNSp(ZMC_HANDLE handle, int imaxaxis, float *pfDisancelist, float fStartSpeed, float fEndSpeed, float fForceSpeed);




#if 0
    //�����˶�
#endif

/*************************************************************
Description:    //�����˶�
Input:          //������handle  ��ţ� ����
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Singl_Vmove(ZMC_HANDLE handle, int iaxis, int idir);

/*************************************************************
Description:    //�����˶�
Input:          //������handle  ��ţ� ����
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Singl_Move(ZMC_HANDLE handle, int iaxis, float fdistance);

/*************************************************************
Description:    //�����˶�
Input:          //������handle  ��ţ� ����
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Singl_MoveAbs(ZMC_HANDLE handle, int iaxis, float fdistance);

/*************************************************************
Description:    //�����˶�
Input:          //������handle  ��ţ� ����
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Singl_MoveModify(ZMC_HANDLE handle, int iaxis, float fdistance);


/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Singl_Addax(ZMC_HANDLE handle, int iaxis, int iaddaxis);


/*************************************************************
Description:    //cancel
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Singl_Cancel(ZMC_HANDLE handle, int iaxis, int imode);


/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Singl_Datum(ZMC_HANDLE handle, int iaxis, int imode);



/*************************************************************
Description:    //���ӳ��ֱ�����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Singl_EncoderRatio(ZMC_HANDLE handle, int iaxis, int imposcount, int inputcount);



/*************************************************************
Description:    //���ӳ��ֱ�����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Singl_StepRatio(ZMC_HANDLE handle, int iaxis, int ioutcount, int idposcount);





#if 0
    //��������
#endif




/*************************************************************
Description:    //���� ���������ã� ������һ�㲻������
Input:          //������handle  merge: 0/1
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetAtype(ZMC_HANDLE handle,int iaxis, int iset);
/*************************************************************
Description:    //���� �����Ƿ������岹
Input:          //������handle  merge: 0/1
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetMerge(ZMC_HANDLE handle,int iaxis,  int iset);

/*************************************************************
Description:    //����
Input:          //������handle  merge: 0/1
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetUnits(ZMC_HANDLE handle, int iaxis, float fset);


/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetSpeed(ZMC_HANDLE handle, int iaxis, float fspeed);



/*************************************************************
Description:    //����
Input:          //������handle  merge: 0/1
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetAccel(ZMC_HANDLE handle, int iaxis, float fspeed);

/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetDecel(ZMC_HANDLE handle, int iaxis, float fspeed);
int32 __stdcall ZAux_SetDatumin(ZMC_HANDLE handle, int iaxis, int id);
/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetFastDec(ZMC_HANDLE handle, int iaxis, float fspeed);
/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetForceSpeed(ZMC_HANDLE handle, int iaxis, float fspeed);

/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetStartMoveSpeed(ZMC_HANDLE handle, int iaxis, float fspeed);

/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetEndMoveSpeed(ZMC_HANDLE handle, int iaxis, float fspeed);

/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetLSpeed(ZMC_HANDLE handle, int iaxis, float fspeed);

/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetCreepSpeed(ZMC_HANDLE handle, int iaxis, float fspeed);

/*************************************************************
Description:    //����  �սǼ�������
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetCornerMode(ZMC_HANDLE handle, int iaxis, int iset);
/*************************************************************
Description:    //����  ���������
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetInvertStep(ZMC_HANDLE handle, int iaxis, int iset);


/*************************************************************
Description:    //����  S��������
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetDecelAngle(ZMC_HANDLE handle, int iaxis, float fset);


/*************************************************************
Description:    //����  S��������
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetStopAngle(ZMC_HANDLE handle, int iaxis, float fset);

/*************************************************************
Description:    //����  СԲ���ٵİ뾶����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetSpRadius(ZMC_HANDLE handle, int iaxis, float fset);


/*************************************************************
Description:    //����  S��������
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetSramp(ZMC_HANDLE handle, int iaxis, int iset);

/*************************************************************
Description:    //����  DPOS�޸�
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetDpos(ZMC_HANDLE handle, int iaxis, float fpos);

/*************************************************************
Description:    //����  MPOS�޸�
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetMpos(ZMC_HANDLE handle, int iaxis, float fpos);

/*************************************************************
Description:    //����  ����޸�����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetOffpos(ZMC_HANDLE handle, int iaxis, float fpos);






#if 0
    //״̬��ȡ
#endif


/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_GetDpos(ZMC_HANDLE handle, int iaxis, float *pfValue);


/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_GetMpos(ZMC_HANDLE handle, int iaxis, float *pfValue);



/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_GetCurSpeed(ZMC_HANDLE handle, int iaxis, float *pfValue);


/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_GetMtype(ZMC_HANDLE handle, int iaxis, int *piValue);

/*************************************************************
Description:    //����  ���Ƿ��˶����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_GetIfIdle(ZMC_HANDLE handle, int iaxis, int *piValue);




/*************************************************************
Description:    //���� ��ǰ������ɺ���յ�����, ��������Ĳ���
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_GetEndMove(ZMC_HANDLE handle, int iaxis, float *pfValue);



/*************************************************************
Description:    //���� ��ǰ����������ɺ���յ����� ����ת����Ծ���
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_GetEndMoveBuff(ZMC_HANDLE handle, int iaxis, float *pfValue);


/*************************************************************
Description:    //���� 
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_GetAxisStatus(ZMC_HANDLE handle, int iaxis, int *piValue);



/*************************************************************
Description:    //����  �ڲ��ı�����λ��
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_GetEncoder(ZMC_HANDLE handle, int iaxis, int *piValue);


/*************************************************************
Description:    //����  ��ʣ��Ļ���, ��ֱ��������
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_GetRemain_LineBuffer(ZMC_HANDLE handle, int iaxis, int *piValue);



/*************************************************************
Description:    //����  ��ʣ��Ļ���, ����ӵĿռ�Բ��������
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_GetRemain_Buffer(ZMC_HANDLE handle, int iaxis, int *piValue);




/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_GetRegPos(ZMC_HANDLE handle, int iaxis, float *pfValue);


/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_GetMark(ZMC_HANDLE handle, int iaxis, int *piValue);



/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_GetMarkB(ZMC_HANDLE handle, int iaxis, int *piValue);



/*************************************************************
Description:    //ȫ�ֱ�����ȡ
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_GetVariablef(ZMC_HANDLE handle, const char *pname, float *pfValue);

/*************************************************************
Description:    //ȫ�ֱ�����ȡ, Ҳ�����ǲ����ȵ�
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_GetVariableInt(ZMC_HANDLE handle, const char *pname, int *piValue);



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
int32 __stdcall ZAux_GetIn(ZMC_HANDLE handle, int ionum, uint32 *piValue);


/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetInvertIN(ZMC_HANDLE handle, int ionum, int bifInvert);



/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_SetOp(ZMC_HANDLE handle, int ionum, uint32 iValue);


#if 0
    //ͨ��modbus���ٶ�ȡ����Ĵ���
#endif

/*************************************************************
Description:    //���� ���ٶ�ȡ�������
Input:          //������handle  
Output:         //��λ�洢
Return:         //������
*************************************************************/
int32 __stdcall ZAux_GetModbusIn(ZMC_HANDLE handle, int ionumfirst, int ionumend, uint8 *pValueList);


/*************************************************************
Description:    //���� ���ٶ�ȡ�����ǰ�����״̬
Input:          //������handle  
Output:         //��λ�洢
Return:         //������
*************************************************************/
int32 __stdcall ZAux_GetModbusOut(ZMC_HANDLE handle, int ionumfirst, int ionumend, uint8 *pValueList);


/*************************************************************
Description:    //���� ���ٶ�ȡ�����ǰ��DPOS
Input:          //������handle  
Output:         //���洢
Return:         //������
*************************************************************/
int32 __stdcall ZAux_GetModbusDpos(ZMC_HANDLE handle, int imaxaxises, float *pValueList);


/*************************************************************
Description:    //���� ���ٶ�ȡ�����ǰ��MPOS
Input:          //������handle  
Output:         //���洢
Return:         //������
*************************************************************/
int32 __stdcall ZAux_GetModbusMpos(ZMC_HANDLE handle, int imaxaxises, float *pValueList);


/*************************************************************
Description:    //���� ���ٶ�ȡ�����ǰ���ٶ�
Input:          //������handle  
Output:         //���洢
Return:         //������
*************************************************************/
int32 __stdcall ZAux_GetModbusCurSpeed(ZMC_HANDLE handle, int imaxaxises, float *pValueList);



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
int32 __stdcall ZAux_Direct_GetDpos(ZMC_HANDLE handle, int iaxis, float *pfValue);


/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Direct_GetMpos(ZMC_HANDLE handle, int iaxis, float *pfValue);



/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Direct_GetCurSpeed(ZMC_HANDLE handle, int iaxis, float *pfValue);



/*************************************************************
Description:    //���� ��ǰ���˶�����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Direct_GetMtype(ZMC_HANDLE handle, int iaxis, int *piValue);


/*************************************************************
Description:    //����  ���Ƿ��˶����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Direct_GetIfIdle(ZMC_HANDLE handle, int iaxis, int *piValue);




/*************************************************************
Description:    //���� ��ǰ������ɺ���յ�����, ��������Ĳ���
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Direct_GetEndMove(ZMC_HANDLE handle, int iaxis, float *pfValue);



/*************************************************************
Description:    //���� ��ǰ����������ɺ���յ����� ����ת����Ծ���
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Direct_GetEndMoveBuff(ZMC_HANDLE handle, int iaxis, float *pfValue);


/*************************************************************
Description:    //���� 
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Direct_GetAxisStatus(ZMC_HANDLE handle, int iaxis, int *piValue);



/*************************************************************
Description:    //����  �ڲ��ı�����λ��
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Direct_GetEncoder(ZMC_HANDLE handle, int iaxis, int *piValue);


/*************************************************************
Description:    //����  ��ʣ��Ļ���, ��ֱ�߶�������
				  REMAIN_BUFFERΪΨһһ�����Լ�AXIS����ZMC_DirectCommand��ȡ��.
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Direct_GetRemain_LineBuffer(ZMC_HANDLE handle, int iaxis, int *piValue);



/*************************************************************
Description:    //����  ��ʣ��Ļ���, ����ӵĿռ�Բ��������
				  REMAIN_BUFFERΪΨһһ�����Լ�AXIS����ZMC_DirectCommand��ȡ��.
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Direct_GetRemain_Buffer(ZMC_HANDLE handle, int iaxis, int *piValue);




/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Direct_GetRegPos(ZMC_HANDLE handle, int iaxis, float *pfValue);


/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Direct_GetMark(ZMC_HANDLE handle, int iaxis, int *piValue);



/*************************************************************
Description:    //����
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Direct_GetMarkB(ZMC_HANDLE handle, int iaxis, int *piValue);



/*************************************************************
Description:    //ȫ�ֱ�����ȡ, Ҳ�����ǲ����ȵ�
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Direct_GetVariablef(ZMC_HANDLE handle, const char *pname, float *pfValue);


/*************************************************************
Description:    //ȫ�ֱ�����ȡ, Ҳ�����ǲ����ȵ�
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Direct_GetVariableInt(ZMC_HANDLE handle, const char *pname, int *piValue);



#if 0
    //��������
#endif


/*************************************************************
Description:    //�ַ���תΪfloat
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_TransStringtoFloat(const char* pstringin, int inumes,  float* pfvlaue);


/*************************************************************
Description:    //�ַ���תΪint
Input:          //������handle  
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_TransStringtoInt(const char* pstringin, int inumes,  int* pivlaue);



//
/*************************************************************
Description:    //�ѿ���handle �� ���Ź�������, ��������Ϳ���ֱ���ÿ�����������
Input:          //������handle
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Spec_Init();



/*************************************************************
Description:    //�ѿ���handle �� ���Ź�������, ��������Ϳ���ֱ���ÿ�����������
Input:          //������handle
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Spec_SetCardHandle(WORD card, ZMC_HANDLE handle);


/*************************************************************
Description:    //�ѿ���handle �� ���Ź�������, ��������Ϳ���ֱ���ÿ�����������
Input:          //����
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Spec_UnSetCardHandle(WORD card);




/*************************************************************
Description:    //�����λ�˶�
Input:          //����
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Spec_GetTotalAxises(WORD card);



/*************************************************************
Description:    // ���ù켣���˶�����
Input:          //����
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Spec_SetVectProfile(WORD card, double spara, int32 speed, int32 acc, int32 dec );



/*************************************************************
Description:    // ���õ�����˶�����
Input:          //����
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Spec_SetProfile(WORD card, int iaxis, int32 speed, int32 acc, int32 dec);



/*************************************************************
Description:    // ��ȡ����
Input:          //����
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Spec_SetSProfile(WORD card, int iaxis, double spara);




/*************************************************************
Description:    //�����λ�˶�
Input:          //����
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Spec_PMove(WORD card, int iaxis, int Length);


/*************************************************************
Description:    //�����λ�˶�
Input:          //����
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Spec_PMoveAbs(WORD card, int iaxis, int Length);



/*************************************************************
Description:    //PMOVEʱ��̬����λ��
Input:          //����
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Spec_ResetTargetPostion(WORD card, int iaxis, int Length);





/*************************************************************
Description:    //SPָ���ٶȵ��˶�
Input:          //����   dir 1- ���� 2-����
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Spec_VMove(WORD card, int iaxis, int dir);




/*************************************************************
Description:    // ����ģʽ����
Input:          //����
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Spec_SetPulOutMode(WORD card, int iaxis, int iset);




/*************************************************************
Description:    // ������ģʽ���ã������Ӧ����ͬ��ATYPE���޷�ӳ��
Input:          //����
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Spec_SetCounterMode(WORD card, int iaxis, int iset);



/*************************************************************
Description:    // ���֣� �ñ�������ģ��
Input:          //����  AB���ͻ��� ���巽�� ����
                  ����ֻ����ATYPE�� ��ʹ����չ��ʱ��ATYPE���Ͳ�һ��.

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Spec_SetHandle(WORD card, int iaxiscoder, int ifabmode);



/*************************************************************
Description:    // ���֣� �ñ�������ģ��
Input:          //����  �������������ֵ���
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Spec_HandMove(WORD card, int iaxis, float fratio);



/*************************************************************
Description:    // ����ֹͣ�� ����ֹͣʱ����FASTDEC���úܴ��ֵ���ٵ��ü���ֹͣ
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Spec_DeclStop(WORD card, int iaxis);


/*************************************************************
Description:    // ��ȡ����
Input:          //����
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Spec_GetPosition(WORD card, int iaxis);


/*************************************************************
Description:    // ��ȡ����
Input:          //����
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Spec_SetPosition(WORD card, int iaxis, int32 current_position);



/*************************************************************
Description:    // ��ȡ����
Input:          //����
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Spec_GetCurSpeed(WORD card, int iaxis);


/*************************************************************
Description:    // ��ȡ����
Input:          //����
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Spec_SetCurSpeed(WORD card, int iaxis, int32 speed);



/*************************************************************
Description:    // ��ȡ����
Input:          //����
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Spec_ConfigHomeMode(WORD card, int iaxis, int home_dir, int velspeed, int Mode);



/*************************************************************
Description:    // ��ȡ����
Input:          //����
Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Spec_HomeMove(WORD card, int iaxis);




/*************************************************************
Description:    // ����ֹͣ �岹ָ��, ��Ϊ�漰������ᣬ�����޸�BASE���б�
                   ע�������岹��ʱ��Ҫ�л�����,��BASE�ĵ�һ����

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Spec_Line2(WORD card, int iaxis1, int iaxis2, int distance1, int distance2);



/*************************************************************
Description:    // ����ֹͣ �岹ָ��, ��Ϊ�漰������ᣬ�����޸�BASE���б�
                   ע�������岹��ʱ��Ҫ�л�����,��BASE�ĵ�һ����

Output:         //
Return:         //������
*************************************************************/
int32 __stdcall ZAux_Spec_LineN(WORD card, int imaxaxis, int* iaxisList, int *distanceList);




#if 0
    //����ĺ���
    //�ϳ�����ĺ���
    //���ÿ���������
#endif


/*************        ÿ�������ڲ����ݶ���,              ***************/

//����ĺ���֧�ֵĲ岹����, һ���PCI��ֻ��һ��
#define MAX_VECT_AUX_SPECIAL   1


typedef struct str_ZAuxCardSpecial
{
    uint8       m_cardnum;   //����
    ZMC_HANDLE  m_handle;    //���

    //���ֵ���
    int  m_iaxisHandle; // ����

    //��������
    double      m_aSpeed[MAX_AXIS_AUX];
    double      m_aAccel[MAX_AXIS_AUX];
    double      m_aDecel[MAX_AXIS_AUX];
    
    double      m_aSRampSet[MAX_AXIS_AUX];  // S���ߵ�����

    double      m_aHomeSpeed[MAX_AXIS_AUX]; // ������ٶ�
    int      m_aHomeDir[MAX_AXIS_AUX]; // ���㷽��
    int      m_aHomeMode[MAX_AXIS_AUX]; // ����ģʽ
    
    
    //�岹������
    double      m_aVectSpeed[MAX_VECT_AUX_SPECIAL];
    double      m_aVectAccel[MAX_VECT_AUX_SPECIAL];
    double      m_aVectDecel[MAX_VECT_AUX_SPECIAL];
    
    double      m_aVectSRampSet[MAX_VECT_AUX_SPECIAL];  // S���ߵ�����

    //
    

}struct_ZAuxCardSpecial;

























#endif


