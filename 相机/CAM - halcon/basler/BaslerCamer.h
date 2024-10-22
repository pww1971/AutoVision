#pragma once
#ifdef _BASLER_CAM
#include "SampleImageEventHandler.h" //�ص���ͷ�ļ�

#include <pylon/PylonIncludes.h> //Pylonͷ�ļ�


#ifdef PYLON_WIN_BUILD
#include <pylon/PylonGUI.h>
#endif


using namespace std;
using namespace Pylon;//pylon���ƿռ�

using namespace GenApi;//pylon SDKͨ����д�����ƿռ�

class BaslerCamer
{
public:
	BaslerCamer(void);
	~BaslerCamer(void);

public:
	CInstantCamera camera;//ʵ�����������
	
	INodeMap *nodemap;//������Խڵ�

	CDeviceInfo di;//�豸��Ϣ������UserID��SN�Ŵ������

public:
	void CamInit();//�����ʼ��
	void CamStart();//��ʼȡͼ
	void CamStop();//ֹͣȡͼ
	void CamDestroy();//�˳�ʱ�ر����

	string CamName();//��ȡ�����
	string CamSerialNum();//��ȡ���SN��
	string CamFormat();//��ȡ���ͼ���ʽ
	double CamFrameRate();//��ȡ���֡��
	int CamWidth();//�ֱ��ʿ�
	int CamHeight();//�ֱ��ʸ�

	double GetExpMin();//��ȡ�ع���Сֵ
	double GetExpMax();//��ȡ�ع����ֵ
	double GetExpCur();//��ȡ��ǰ�ع�ֵ
	void SetExp(double ExpTime);//�����ع�ֵ

	int GetGainMin();//��ȡ������Сֵ
	int GetGainMax();//��ȡ�������ֵ
	int GetGainCur();//��ȡ��ǰ����ֵ
	void SetGain(int GainValue);//��������ֵ

	void SoftTrigEnable(string flag);//����ʹ��
	void ExecuteSoftTrig();//ִ������
	void ExtTrigEnable(string flag);//�ⴥ��ʹ��
};
#endif