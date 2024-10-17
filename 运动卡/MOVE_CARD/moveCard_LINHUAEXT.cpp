#include "stdafx.h"
#include "moveCard.h"

#ifdef _LINHUA
CLinHuaCardExt::CLinHuaCardExt()
{
#ifdef IOEXT
	m_ioExtNum = 32;
	m_ioExtInState = new uint[m_ioExtNum];
	m_szHelp = "两个凌华+io扩展运动卡 24个电机 24*2+32个io输出 24*2+32个io触发输入 80至128是软触发io";
#else
	m_ioExtNum = 0;
	m_szHelp = "凌华运动卡 24个电机 24个io输出 24个io触发输入 24至128是软触发io";
#endif
	m_dwInData = 0x00000000;
	m_dwOutData = 0x00000000;
	m_hCard = -1;
}

CLinHuaCardExt::~CLinHuaCardExt()
{
#ifdef IOEXT
	if (m_hCard >= 0)
		Release_Card(m_hCard);
	m_hCard = -1;
	delete[] m_ioExtInState;
#endif
}

bool CLinHuaCardExt::ini(string linkAdd)
{
#ifdef IOEXT
	if (m_hCard < 0)
	{
		int nCardID = 0;
		if ((m_hCard=Register_Card(PCI_7230, nCardID)) < 0) 
		{
			/*strTemp.Format( TEXT("Register_Card Error = %d"), hCard);
			AfxMessageBox( strTemp );*/
			return false;
		}
		DO_WritePort( m_hCard, 0, m_dwOutData );
	}

#endif	
	if (CLinHuaCard::ini(linkAdd))
	{
		//软触发中断器
		int ioTal = m_ioNum*v_card_num;//运动卡的io口数量
		int ioSoftPos = ioTal+m_ioExtNum;
		int ioSoftNum = g_ioN - ioSoftPos;
		if (ioSoftNum>0)
		{
			if (m_ioSoft != NULL)
			{
				delete[] m_ioSoft;
			}
			m_ioSoft = new int[ioSoftNum];
			//软触发计数器清零 
			memset(m_ioSoft,0,sizeof(int)*ioSoftNum);
		}
		return true;
	}
	else
		return false;
}

bool CLinHuaCardExt::setIO(int ioid,int onoff)
{	
	if ( ioid >= g_ioN)
	{
		return false;
	}
	int ioTal = m_ioNum*v_card_num;//运动卡的io口数量
	if( ioid > ioTal )
	{ 
		m_Lock.lock();
		if (!m_bIni)
		{
			m_Lock.unLock();
			return false;
		}
		//软触发中断器
		int ioSoftPos = ioTal+m_ioExtNum;
		int ioSoftNum = g_ioN - ioSoftPos;
		if (ioSoftNum>0 && ioid >= ioSoftPos)
		{
			if (onoff<=0)
			{
				m_ioSoft[ioid-ioSoftPos]--;//计数一次
			}
			else// if (onoff > 0 )//计数设置
			{
				m_ioSoft[ioid-ioSoftPos]=onoff;
			}
			m_Lock.unLock();
			return true;
		}
#ifdef IOEXT
		U32 sign = (1<<(ioid-ioTal));
		if( onoff == BST_CHECKED )
		{
			m_dwOutData |= sign;
		}
		else if( onoff == BST_UNCHECKED )
		{
			m_dwOutData &= (~sign);
		}

		DO_WritePort( m_hCard, 0, m_dwOutData );
#endif
		m_Lock.unLock();
		return true;
	}
	else
	{
		return CLinHuaCard::setIO(ioid,onoff);
	}
}

bool CLinHuaCardExt::getIO(uint state[])
{
	int ioTal = m_ioNum*v_card_num;//运动卡的io口数量
	m_Lock.lock();
	if (!m_bIni)
	{
		m_Lock.unLock();
		return false;
	}
#ifdef IOEXT
	U32 dwPortData, dwChangeMask;

    DI_ReadPort(m_hCard, PORT_DI_LOW, &dwPortData);

	dwChangeMask = dwPortData ^ m_dwInData;

	if( dwChangeMask != 0x00 ) // Input had been changed 
	{
		 m_dwInData = dwPortData; // update the DI data
		 U32 sign = 1;
		 for (int i=0;i<m_ioExtNum;i++)
		 {
			 if( (dwChangeMask & sign) == sign ) // if the target bit is changed 
			 {
				 if( (dwPortData & sign) == sign )
				 {
					 m_ioExtInState[i] = 1;
					 state[i+ioTal] = 1;
				 }
				 else
				 {
					 m_ioExtInState[i] = 0;
					 state[i+ioTal] = 0;
				 }
			 }
			 sign = (sign << 1);
		 }
	}
	else
		memcpy(state+ioTal,m_ioExtInState,m_ioExtNum*sizeof(uint));
#endif
	//软触发计数器
	int ioSoftPos = ioTal+m_ioExtNum;
	int ioSoftNum = g_ioN - ioSoftPos;
	for (int i=0;i<ioSoftNum;i++)
	{
		if (m_ioSoft[i] > 0)//次数小于等于0 就触发
			state[i+ioSoftPos] = 1;
		else
			state[i+ioSoftPos] = 0;
	}
	m_Lock.unLock();

	return CLinHuaCard::getIO(state);
}

bool CLinHuaCardExt::zeroIO()
{
	int ioTal = m_ioNum*v_card_num;//运动卡的io口数量
	m_Lock.lock();
	if (!m_bIni)
	{
		m_Lock.unLock();
		return false;
	}
	int ioSoftNum = g_ioN - ioTal - m_ioExtNum;
	//软触发计数器清零 
	memset(m_ioSoft,0,sizeof(int)*ioSoftNum);
#ifdef IOEXT
	//扩展卡清零
	m_dwOutData = 0x00000000;
	DO_WritePort( m_hCard, 0, m_dwOutData );
#endif
	m_Lock.unLock();
	return CLinHuaCard::zeroIO();
}
#endif
