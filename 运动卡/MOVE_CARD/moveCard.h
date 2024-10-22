#ifndef __MOVECAR_H__
#define __MOVECAR_H__
//�����˶����ƿ� ʵ�ֵĻ�������
#include "automove.h"

using namespace GDKT;

//�����ָ�����˶�����ָ��
# define  CHAINFLAG  "@"
# define MVFLAG ":"

#ifdef _LINHUA
#include "APS168.h"
#include "APS_define.h"
#include "ErrorCodeDef.h"
#pragma comment( lib, "APS168x64.lib")
#endif

const int g_ioN  = 128;
class CMoveType
{
public:
	CMoveType();
	virtual ~CMoveType();
	virtual bool ini(string linkAdd);
	virtual bool writeMvCfg(CConfig& cfg);
	virtual bool readMvCfg(CConfig& cfg);
	virtual void release();
	virtual void Rapidstop();
	virtual bool moveAbs(int mtid,float pos);//�����˶�
	virtual bool move(int mtid,float dis);//����˶�
	virtual bool moveV(int mtid,int di);//�����˶� di����1 ����-1
	virtual bool moveStop(int mtid);//��������˶�
	virtual bool mtZero(int mtid);//����
	virtual bool setIO(int ioid,int onoff);//����io�����
	virtual bool getIO(uint state[]);//��ȡ����io�ڵ�״̬
	virtual bool zeroIO();//��ʼ��io�������λ
	virtual bool getMtState(int mtid,int& state);	//��������state=0�˶��� -1û���˶�
	virtual bool getMtPos(int mtid,float& pos);		//�����λ��
	virtual bool getParam(std::vector<string>& vctPa);		//��ȡ��ǰ����
	virtual bool setParam(string szPa);		//��������
	virtual void waitForStop(int mtid);
	virtual void setSpeed(bool debug);
	//��ȡ��ǰλ����Ŀ�ĵ�λ�ò�
	virtual float getDvPos(int iaxis);
	//�ж�ip�ַ��ĸ�ʽ
	virtual bool isIPAddressValid(const char* pszIPAddr);
	bool getIniState();
public:
	std::vector<cv::Point2f> m_MvScope;//�˶���Χ
	std::vector<float> m_fSpeed;//�˶��ٶ�
	std::vector<float> m_fDebugSpeed;//�˶��ٶ�
	//	float m_fUnitMM[16];	//��λ����ת�� ��Ե�Ƭ��  �˶����ƿ����Ե������嵱��
	std::vector<string> m_vctCfgName;
	std::vector<float> m_vctDefaultData;
	int m_ioNum;		//io����Ŀ
	int m_mtNum;		//�����Ŀ
	int* m_ioSoft;	//�����жϼ����� >1������ 0 ���� 
protected:
	CResLock m_Lock;
public:
	bool m_bIni;		//��ʼ��״̬
	string m_linkAdd;//ip�򴮿ڵ�ַ�������ļ���ַ
	string m_szHelp;//����˵��
#ifdef _SIMULATION
	CClkOnTimer m_ontime;
	S_Click* m_MtRunning;//�˶���ʱ �����˶�����
#endif
};
//���˶���
#ifdef _ZMC 
#include "zmotion.h"
#include "zmcaux.h"
#include "zauxdll2.h"
#pragma comment( lib, "zmotion.lib")
typedef  int32(__stdcall* ZAux_Set_i)(ZMC_HANDLE, int, int);
typedef  int32(__stdcall* ZAux_Set_f)(ZMC_HANDLE, int, float);
class CMoveCard:public CMoveType
{
public:
	CMoveCard();
	virtual ~CMoveCard();
	virtual bool ini(string linkAdd);
	virtual bool writeMvCfg(CConfig& cfg);
	virtual bool readMvCfg(CConfig& cfg);
	virtual void release();
	virtual void Rapidstop();
	virtual bool moveAbs(int mtid,float pos);//�����˶�
	virtual bool move(int mtid,float dis);//����˶�
	virtual bool moveV(int mtid,int di);//�����˶� di����1 ����-1
	virtual bool moveStop(int mtid);//��������˶�
	virtual bool mtZero(int mtid);//����
	virtual bool setIO(int ioid,int onoff);//����io�����
	virtual bool getIO( uint state[]);//��ȡ����io�ڵ�״̬
	virtual bool zeroIO(){
		for(int j=0;j<m_ioNum;j++)
			setIO(j,0);
		int ioSoftNum = g_ioN - m_ioNum;
		for (int i=0;i<ioSoftNum;i++)//�������������� 1������
		{
			m_ioSoft[i] = 1;
		}
		return true;
	};//��ʼ��io�������λ
	virtual bool getMtState(int mtid,int& state);	//��������state=0�˶��� -1û���˶�
	virtual bool getMtPos(int mtid,float& pos);		//�����λ��
	virtual bool getParam(vector<string>& vctPa);		//��ȡ��ǰ����
	virtual bool setParam(string szPa);		//��������
	virtual void waitForStop(int mtid);
	virtual void setSpeed(bool debug);

	//��ȡ��ǰλ����Ŀ�ĵ�λ�ò�
	virtual float getDvPos(int iaxis);
	//�ж�ip�ַ��ĸ�ʽ
	bool isIPAddressValid(const char* pszIPAddr);
	std::vector<float> m_fZeroMode;//����ģʽ
protected:
	ZMC_HANDLE  m_handle;//�Ŀ�

};
#endif
//�軪
class CLinHuaCard:public CMoveType
{
#ifdef _LINHUA
public:
	CLinHuaCard();
	virtual ~CLinHuaCard();
	virtual bool ini(string linkAdd);
	virtual bool writeMvCfg(CConfig& cfg);
	virtual bool readMvCfg(CConfig& cfg);
	virtual void release();
	virtual void Rapidstop();
	virtual bool moveAbs(int mtid,float pos);//�����˶�
	virtual bool move(int mtid,float dis);//����˶�
	virtual bool moveV(int mtid,int di);//�����˶� di����1 ����-1
	virtual bool moveStop(int mtid);//��������˶�
	virtual bool mtZero(int mtid);//����
	virtual bool setIO(int ioid,int onoff);//����io�����
	virtual bool getIO( uint state[]);//��ȡ����io�ڵ�״̬
	virtual bool zeroIO(){
		int ioTal = m_ioNum*v_card_num;//�˶�����io������
		for(int j=0;j<ioTal;j++)
			setIO(j,0);
		return true;
	};//��ʼ��io�������λ
	virtual bool getMtState(int mtid,int& state);	//��������state=0�˶��� -1û���˶�
	virtual bool getMtPos(int mtid,float& pos);		//�����λ��
	virtual bool getParam(vector<string>& vctPa);		//��ȡ��ǰ����
	virtual bool setParam(string szPa);		//��������
	virtual void waitForStop(int mtid);
	virtual void setSpeed(bool debug);
	//��ȡ��ǰλ����Ŀ�ĵ�λ�ò�
	virtual float getDvPos(int iaxis);
	const char *stop_code_to_string( I32 Stop_Code );
protected:
	I32 v_card_num;//�˶�����Ŀ
	I32 v_card_id[10];
	I32 v_first_axis_id[10];
	I32 v_card_name;
	I32 v_total_axis;
	vector<float> m_iMvSpeed;
	vector<int> m_mvScale;//һ���׵�������
#endif
};
#ifdef _LINHUA
//�˶���+io��չ��
//#define IOEXT
#ifdef IOEXT
#include "Dask64.h"
#pragma comment( lib, "pci-dask64.lib");
#endif
#endif
class CLinHuaCardExt:public CLinHuaCard
{
#ifdef _LINHUA
public:
	CLinHuaCardExt();
	virtual ~CLinHuaCardExt();
	virtual bool ini(string linkAdd);
	virtual bool setIO(int ioid,int onoff);//����io�����
	virtual bool getIO( uint state[]);//��ȡ����io�ڵ�״̬
	virtual bool zeroIO();//��ʼ��io�������λ
protected:
	int m_ioExtNum;//��չio����Ŀ ����������ǶԳƵ���
	I16 m_hCard;
	U32 m_dwOutData;
	U32 m_dwInData;
	uint* m_ioExtInState;//������չ����һ������io״̬
#endif
};

#ifdef _LTD
#include "LTDMC.h"
#pragma comment( lib, "LTDMC.lib")
class CLTDCard:public CMoveType
{
public:
	CLTDCard();
	virtual ~CLTDCard();
	virtual bool ini(string linkAdd);
	virtual bool writeMvCfg(CConfig& cfg);
	virtual bool readMvCfg(CConfig& cfg);
	virtual void release();
	virtual void Rapidstop();
	virtual bool moveAbs(int mtid, float pos);//�����˶�
	virtual bool move(int mtid, float dis);//����˶�
	virtual bool moveV(int mtid, int di);//�����˶� di����1 ����-1
	virtual bool moveStop(int mtid);//��������˶�
	virtual bool mtZero(int mtid);//����
	virtual bool setIO(int ioid, int onoff);//����io�����
	virtual bool getIO(uint state[]);//��ȡ����io�ڵ�״̬
	virtual bool zeroIO() {
		int ioTal = m_ioNum * m_CardNum;//�˶�����io������
		for (int j = 0; j < ioTal; j++)
			setIO(j, 0);
		return true;
	};//��ʼ��io�������λ
	virtual bool getMtState(int mtid, int& state);	//��������state=0�˶��� -1û���˶�
	virtual bool getMtPos(int mtid, float& pos);		//�����λ��
	virtual bool getParam(vector<string>& vctPa);		//��ȡ��ǰ����
	virtual bool setParam(string szPa);		//��������
	virtual void waitForStop(int mtid);
	virtual void setSpeed(bool debug);
	//��ȡ��ǰλ����Ŀ�ĵ�λ�ò�
	virtual float getDvPos(int mtid);
protected:
	void getMtId(WORD& cardid,int& mtid);
	WORD m_CardNum;      //���忨��
	WORD m_CardList[8];   //���忨������

	int v_first_axis_id[8];
	int v_total_axis;
	vector<float> m_iMvSpeed;
	vector<int> m_mvScale;//һ���׵�������
};
#endif

#ifdef _GTS
#include "gts.h"
#pragma comment( lib, "gts.lib")
class CGTSCard :public CMoveType
{
public:
	CGTSCard();
	virtual ~CGTSCard();
	virtual bool ini(string linkAdd);
	virtual bool writeMvCfg(CConfig& cfg);
	virtual bool readMvCfg(CConfig& cfg);
	virtual void release();
	virtual void Rapidstop();
	virtual bool moveAbs(int mtid, float pos);//�����˶�
	virtual bool move(int mtid, float dis);//����˶�
	virtual bool moveV(int mtid, int di);//�����˶� di����1 ����-1
	virtual bool moveStop(int mtid);//��������˶�
	virtual bool mtZero(int mtid);//����
	virtual bool setIO(int ioid, int onoff);//����io�����
	virtual bool getIO(uint state[]);//��ȡ����io�ڵ�״̬
	virtual bool zeroIO() {
		int ioTal = m_ioNum * m_CardNum;//�˶�����io������
		for (int j = 0; j < ioTal; j++)
			setIO(j, 0);
		return true;
	};//��ʼ��io�������λ
	virtual bool getMtState(int mtid, int& state);	//��������state=0�˶��� -1û���˶�
	virtual bool getMtPos(int mtid, float& pos);		//�����λ��
	virtual bool getParam(vector<string>& vctPa);		//��ȡ��ǰ����
	virtual bool setParam(string szPa);		//��������
	virtual void waitForStop(int mtid);
	virtual void setSpeed(bool debug);
	//��ȡ��ǰλ����Ŀ�ĵ�λ�ò�
	virtual float getDvPos(int mtid);
protected:
	void getMtId(WORD& cardid, int& mtid);
	WORD m_CardNum;      //���忨��

	int v_first_axis_id[8];
	int v_total_axis;
	vector<float> m_iMvSpeed;
	vector<int> m_mvScale;//һ���׵�������
};
#endif

//�����˶���
#ifdef _MC_BP
#include "MultiCardCPP.h"
#pragma comment(lib,"MultiCard.lib")
class CMCBPMoveCard :public CMoveType
{
public:
	CMCBPMoveCard();
	virtual ~CMCBPMoveCard();
	virtual bool ini(string linkAdd);
	virtual bool writeMvCfg(CConfig& cfg);
	virtual bool readMvCfg(CConfig& cfg);
	virtual void release();
	virtual void Rapidstop();
	virtual bool moveAbs(int mtid, float pos);//�����˶�
	virtual bool move(int mtid, float dis);//����˶�
	virtual bool moveV(int mtid, int di);//�����˶� di����1 ����-1
	virtual bool moveStop(int mtid);//��������˶�
	virtual bool mtZero(int mtid);//����
	virtual bool setIO(int ioid, int onoff);//����io�����
	virtual bool getIO(uint state[]);//��ȡ����io�ڵ�״̬
	virtual bool zeroIO() {
		for (int j = 0; j < m_ioNum; j++)
			setIO(j, 0);
		int ioSoftNum = g_ioN - m_ioNum;
		for (int i = 0; i < ioSoftNum; i++)//�������������� 1������
		{
			m_ioSoft[i] = 1;
		}
		return true;
	};//��ʼ��io�������λ
	virtual bool getMtState(int mtid, int& state);	//��������state=0�˶��� -1û���˶�
	virtual bool getMtPos(int mtid, float& pos);		//�����λ��
	virtual bool getParam(vector<string>& vctPa);		//��ȡ��ǰ����
	virtual bool setParam(string szPa);		//��������
	virtual void waitForStop(int mtid);
	virtual void setSpeed(bool debug);

	//��ȡ��ǰλ����Ŀ�ĵ�λ�ò�
	virtual float getDvPos(int iaxis);
	//�ж�ip�ַ��ĸ�ʽ
	bool isIPAddressValid(const char* pszIPAddr);

protected:
	MultiCard m_handle;//��  ����� ������������µĶ���ʵ�ֶ���
};
#endif

#endif

