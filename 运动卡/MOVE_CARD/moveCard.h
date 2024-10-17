#ifndef __MOVECAR_H__
#define __MOVECAR_H__
//各种运动控制卡 实现的基本功能
#include "automove.h"

using namespace GDKT;

//连锁分割符和运动编码分割符
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
	virtual bool moveAbs(int mtid,float pos);//绝对运动
	virtual bool move(int mtid,float dis);//相对运动
	virtual bool moveV(int mtid,int di);//连续运动 di正向1 反向-1
	virtual bool moveStop(int mtid);//针对连续运动
	virtual bool mtZero(int mtid);//归零
	virtual bool setIO(int ioid,int onoff);//设置io口输出
	virtual bool getIO(uint state[]);//获取输入io口的状态
	virtual bool zeroIO();//初始化io口输出低位
	virtual bool getMtState(int mtid,int& state);	//检测电机情况state=0运动中 -1没有运动
	virtual bool getMtPos(int mtid,float& pos);		//检测电机位置
	virtual bool getParam(std::vector<string>& vctPa);		//获取当前配置
	virtual bool setParam(string szPa);		//设置配置
	virtual void waitForStop(int mtid);
	virtual void setSpeed(bool debug);
	//获取当前位置与目的的位置差
	virtual float getDvPos(int iaxis);
	//判断ip字符的格式
	virtual bool isIPAddressValid(const char* pszIPAddr);
	bool getIniState();
public:
	std::vector<cv::Point2f> m_MvScope;//运动范围
	std::vector<float> m_fSpeed;//运动速度
	std::vector<float> m_fDebugSpeed;//运动速度
	//	float m_fUnitMM[16];	//单位毫米转换 针对单片机  运动控制卡可以调节脉冲当量
	std::vector<string> m_vctCfgName;
	std::vector<float> m_vctDefaultData;
	int m_ioNum;		//io口数目
	int m_mtNum;		//电机数目
	int* m_ioSoft;	//软触发中断计数器 >1不触发 0 触发 
protected:
	CResLock m_Lock;
public:
	bool m_bIni;		//初始化状态
	string m_linkAdd;//ip或串口地址或配置文件地址
	string m_szHelp;//帮助说明
#ifdef _SIMULATION
	CClkOnTimer m_ontime;
	S_Click* m_MtRunning;//运动定时 仿真运动过程
#endif
};
//正运动卡
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
	virtual bool moveAbs(int mtid,float pos);//绝对运动
	virtual bool move(int mtid,float dis);//相对运动
	virtual bool moveV(int mtid,int di);//连续运动 di正向1 反向-1
	virtual bool moveStop(int mtid);//针对连续运动
	virtual bool mtZero(int mtid);//归零
	virtual bool setIO(int ioid,int onoff);//设置io口输出
	virtual bool getIO( uint state[]);//获取输入io口的状态
	virtual bool zeroIO(){
		for(int j=0;j<m_ioNum;j++)
			setIO(j,0);
		int ioSoftNum = g_ioN - m_ioNum;
		for (int i=0;i<ioSoftNum;i++)//软触发计数器清零 1不触发
		{
			m_ioSoft[i] = 1;
		}
		return true;
	};//初始化io口输出低位
	virtual bool getMtState(int mtid,int& state);	//检测电机情况state=0运动中 -1没有运动
	virtual bool getMtPos(int mtid,float& pos);		//检测电机位置
	virtual bool getParam(vector<string>& vctPa);		//获取当前配置
	virtual bool setParam(string szPa);		//设置配置
	virtual void waitForStop(int mtid);
	virtual void setSpeed(bool debug);

	//获取当前位置与目的的位置差
	virtual float getDvPos(int iaxis);
	//判断ip字符的格式
	bool isIPAddressValid(const char* pszIPAddr);
	std::vector<float> m_fZeroMode;//归零模式
protected:
	ZMC_HANDLE  m_handle;//的卡

};
#endif
//凌华
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
	virtual bool moveAbs(int mtid,float pos);//绝对运动
	virtual bool move(int mtid,float dis);//相对运动
	virtual bool moveV(int mtid,int di);//连续运动 di正向1 反向-1
	virtual bool moveStop(int mtid);//针对连续运动
	virtual bool mtZero(int mtid);//归零
	virtual bool setIO(int ioid,int onoff);//设置io口输出
	virtual bool getIO( uint state[]);//获取输入io口的状态
	virtual bool zeroIO(){
		int ioTal = m_ioNum*v_card_num;//运动卡的io口数量
		for(int j=0;j<ioTal;j++)
			setIO(j,0);
		return true;
	};//初始化io口输出低位
	virtual bool getMtState(int mtid,int& state);	//检测电机情况state=0运动中 -1没有运动
	virtual bool getMtPos(int mtid,float& pos);		//检测电机位置
	virtual bool getParam(vector<string>& vctPa);		//获取当前配置
	virtual bool setParam(string szPa);		//设置配置
	virtual void waitForStop(int mtid);
	virtual void setSpeed(bool debug);
	//获取当前位置与目的的位置差
	virtual float getDvPos(int iaxis);
	const char *stop_code_to_string( I32 Stop_Code );
protected:
	I32 v_card_num;//运动卡数目
	I32 v_card_id[10];
	I32 v_first_axis_id[10];
	I32 v_card_name;
	I32 v_total_axis;
	vector<float> m_iMvSpeed;
	vector<int> m_mvScale;//一毫米的脉冲数
#endif
};
#ifdef _LINHUA
//运动卡+io扩展卡
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
	virtual bool setIO(int ioid,int onoff);//设置io口输出
	virtual bool getIO( uint state[]);//获取输入io口的状态
	virtual bool zeroIO();//初始化io口输出低位
protected:
	int m_ioExtNum;//扩展io口数目 输入输出都是对称的算
	I16 m_hCard;
	U32 m_dwOutData;
	U32 m_dwInData;
	uint* m_ioExtInState;//保留扩展卡上一次输入io状态
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
	virtual bool moveAbs(int mtid, float pos);//绝对运动
	virtual bool move(int mtid, float dis);//相对运动
	virtual bool moveV(int mtid, int di);//连续运动 di正向1 反向-1
	virtual bool moveStop(int mtid);//针对连续运动
	virtual bool mtZero(int mtid);//归零
	virtual bool setIO(int ioid, int onoff);//设置io口输出
	virtual bool getIO(uint state[]);//获取输入io口的状态
	virtual bool zeroIO() {
		int ioTal = m_ioNum * m_CardNum;//运动卡的io口数量
		for (int j = 0; j < ioTal; j++)
			setIO(j, 0);
		return true;
	};//初始化io口输出低位
	virtual bool getMtState(int mtid, int& state);	//检测电机情况state=0运动中 -1没有运动
	virtual bool getMtPos(int mtid, float& pos);		//检测电机位置
	virtual bool getParam(vector<string>& vctPa);		//获取当前配置
	virtual bool setParam(string szPa);		//设置配置
	virtual void waitForStop(int mtid);
	virtual void setSpeed(bool debug);
	//获取当前位置与目的的位置差
	virtual float getDvPos(int mtid);
protected:
	void getMtId(WORD& cardid,int& mtid);
	WORD m_CardNum;      //定义卡数
	WORD m_CardList[8];   //定义卡号数组

	int v_first_axis_id[8];
	int v_total_axis;
	vector<float> m_iMvSpeed;
	vector<int> m_mvScale;//一毫米的脉冲数
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
	virtual bool moveAbs(int mtid, float pos);//绝对运动
	virtual bool move(int mtid, float dis);//相对运动
	virtual bool moveV(int mtid, int di);//连续运动 di正向1 反向-1
	virtual bool moveStop(int mtid);//针对连续运动
	virtual bool mtZero(int mtid);//归零
	virtual bool setIO(int ioid, int onoff);//设置io口输出
	virtual bool getIO(uint state[]);//获取输入io口的状态
	virtual bool zeroIO() {
		int ioTal = m_ioNum * m_CardNum;//运动卡的io口数量
		for (int j = 0; j < ioTal; j++)
			setIO(j, 0);
		return true;
	};//初始化io口输出低位
	virtual bool getMtState(int mtid, int& state);	//检测电机情况state=0运动中 -1没有运动
	virtual bool getMtPos(int mtid, float& pos);		//检测电机位置
	virtual bool getParam(vector<string>& vctPa);		//获取当前配置
	virtual bool setParam(string szPa);		//设置配置
	virtual void waitForStop(int mtid);
	virtual void setSpeed(bool debug);
	//获取当前位置与目的的位置差
	virtual float getDvPos(int mtid);
protected:
	void getMtId(WORD& cardid, int& mtid);
	WORD m_CardNum;      //定义卡数

	int v_first_axis_id[8];
	int v_total_axis;
	vector<float> m_iMvSpeed;
	vector<int> m_mvScale;//一毫米的脉冲数
};
#endif

//博派运动卡
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
	virtual bool moveAbs(int mtid, float pos);//绝对运动
	virtual bool move(int mtid, float dis);//相对运动
	virtual bool moveV(int mtid, int di);//连续运动 di正向1 反向-1
	virtual bool moveStop(int mtid);//针对连续运动
	virtual bool mtZero(int mtid);//归零
	virtual bool setIO(int ioid, int onoff);//设置io口输出
	virtual bool getIO(uint state[]);//获取输入io口的状态
	virtual bool zeroIO() {
		for (int j = 0; j < m_ioNum; j++)
			setIO(j, 0);
		int ioSoftNum = g_ioN - m_ioNum;
		for (int i = 0; i < ioSoftNum; i++)//软触发计数器清零 1不触发
		{
			m_ioSoft[i] = 1;
		}
		return true;
	};//初始化io口输出低位
	virtual bool getMtState(int mtid, int& state);	//检测电机情况state=0运动中 -1没有运动
	virtual bool getMtPos(int mtid, float& pos);		//检测电机位置
	virtual bool getParam(vector<string>& vctPa);		//获取当前配置
	virtual bool setParam(string szPa);		//设置配置
	virtual void waitForStop(int mtid);
	virtual void setSpeed(bool debug);

	//获取当前位置与目的的位置差
	virtual float getDvPos(int iaxis);
	//判断ip字符的格式
	bool isIPAddressValid(const char* pszIPAddr);

protected:
	MultiCard m_handle;//卡  多个卡 就派生类添加新的对象实现定义
};
#endif

#endif

