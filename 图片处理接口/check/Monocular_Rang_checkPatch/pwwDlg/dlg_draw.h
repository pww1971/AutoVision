#ifndef DRAW_H
#define DRAW_H

#include "pwwVision.h"
#include "../resource.h"

#define m_his_h    80 
#define m_his_w    256
#define m_his_w2   100

#define DLLVIEW "pwwDlg"//�����޸���ͼ���� ��ֹ����dll����һ����ͼ
//����¼���Ӧ
void onMouse(int event, int x, int y, int flag=0, void*p=NULL);
//�Ҷ�ֱ��ͼ����¼���Ӧ
#define ONMOUSE(funname) static void funname(int event, int x, int y, int flag = 0, void* p = NULL){	\
hisMouse* pdata = (hisMouse*)p;	\
pdata->onMouse(event, x, y, flag, p);}

#endif //HISTOGRAM_INCLUDE_H