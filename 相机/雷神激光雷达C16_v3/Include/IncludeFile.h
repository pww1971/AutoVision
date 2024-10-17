#pragma once

#include <iostream>
#include "GetLidarData.cpp"

#define GetLidarData_C16_v3_0_MACRO												//预定义 对应下面的雷达型号 填入即将使用的雷达类型

//创建HS1的类
#ifdef GetLidarData_HS1_MACRO
#include "GetLidarData_HS1.h"
#include "GetLidarData_HS1.cpp"													//项目 *.vcxproj 内不增加 .cpp文件； 在这部分增加； 两个地方都增加会导致重复定义
GetLidarData* m_GetLidarData = new GetLidarData_HS1;							//创建HS1的类
#endif

//创建CH16的类
#ifdef GetLidarData_CH16_MACRO
#include "GetLidarData_CH16.h"
#include "GetLidarData_CH16.cpp"
	 GetLidarData* m_GetLidarData = new GetLidarData_CH16;						//创建CH16的类
#endif

//创建CH32的类
#ifdef GetLidarData_CH32_MACRO
#include "GetLidarData_CH32.h"
#include "GetLidarData_CH32.cpp"
	 GetLidarData* m_GetLidarData = new GetLidarData_CH32;						//创建CH32的类
#endif
 
//创建_CH32_WideAngle的类
#ifdef GetLidarData_CH32_WideAngle_MACRO
#include "GetLidarData_CH32_WideAngle.h"
#include "GetLidarData_CH32_WideAngle.cpp"
	 GetLidarData* m_GetLidarData = new GetLidarData_CH32_WideAngle;			//创建_CH32_WideAngle的类
#endif

//创建CH64的类
#ifdef GetLidarData_CH64_MACRO
#include "GetLidarData_CH64.h"
#include "GetLidarData_CH64.cpp"
	 GetLidarData* m_GetLidarData = new GetLidarData_CH64;						//创建CH64的类
#endif

//创建CH120的类
#ifdef GetLidarData_CH120_MACRO
#include "GetLidarData_CH120.h"
#include "GetLidarData_CH120.cpp"
	 GetLidarData* m_GetLidarData = new GetLidarData_CH120;						//创建CH120的类
#endif

//创建CH128的类
#ifdef GetLidarData_CH128_MACRO
#include "GetLidarData_CH128.h"
#include "GetLidarData_CH128.cpp"
	 GetLidarData* m_GetLidarData = new GetLidarData_CH128;						//创建CH128的类
#endif

//创建CH128x1的类
#ifdef GetLidarData_CH128x1_MACRO
#include "GetLidarData_CH128x1.h"
#include "GetLidarData_CH128x1.cpp"
		 GetLidarData* m_GetLidarData = new GetLidarData_CH128x1;				//创建CH128x1的类
#endif

//创建CH256的类
#ifdef GetLidarData_CH256_MACRO
#include "GetLidarData_CH256.h"
#include "GetLidarData_CH256.cpp"
		 GetLidarData* m_GetLidarData = new GetLidarData_CH256;				//创建CH256的类
#endif

// 创建CH16x1的类
#ifdef GetLidarData_CH16x1_MACRO
#include "GetLidarData_CH16x1.h"
#include "GetLidarData_CH16x1.cpp"
			 GetLidarData* m_GetLidarData = new GetLidarData_CH16x1;			// 创建CH16x1的类
#endif

// 创建CH128S1的类
#ifdef GetLidarData_CH128S1_MACRO
#include "GetLidarData_CH128S1.h"
#include "GetLidarData_CH128S1.cpp"
			 GetLidarData* m_GetLidarData = new GetLidarData_CH128S1;			// 创建CH128S1的类
#endif

// 创建CX128S2的类
#ifdef GetLidarData_CX128S2_MACRO
#include "GetLidarData_CX128S2.h"
#include "GetLidarData_CX128S2.cpp"
			 GetLidarData* m_GetLidarData = new GetLidarData_CX128S2;			// 创建CX128S2的类
#endif

// 创建CX126S3的类
#ifdef GetLidarData_CX126S3_MACRO
#include "GetLidarData_CX126S3.h"
#include "GetLidarData_CX126S3.cpp"
			 GetLidarData* m_GetLidarData = new GetLidarData_CX126S3;			// 创建CX126S3的类
#endif

// 创建CX1S3的类  
#ifdef GetLidarData_CX1S3_MACRO
#include "GetLidarData_CX1S3.h"
#include "GetLidarData_CX1S3.cpp"
			 GetLidarData* m_GetLidarData = new GetLidarData_CX1S3;			// 创建CX1S3的类
#endif

 // 创建CX6S3的类  只显示s3的19、20线数据
#ifdef GetLidarData_CX6S3_MACRO
#include "GetLidarData_CX6S3.h"
#include "GetLidarData_CX6S3.cpp"
			 GetLidarData* m_GetLidarData = new GetLidarData_CX6S3;			// 创建CX6S3的类
#endif

// 创建CB64S1的类
#ifdef GetLidarData_CB64S1_MACRO
#include "GetLidarData_CB64S1.h"
#include "GetLidarData_CB64S1.cpp"
			 GetLidarData* m_GetLidarData = new GetLidarData_CB64S1;				// 创建CB64S1的类
#endif

 // 创建CB64S1_A的类
#ifdef GetLidarData_CB64S1_A_MACRO
#include "GetLidarData_CB64S1_A.h"
#include "GetLidarData_CB64S1_A.cpp"
			 GetLidarData* m_GetLidarData = new GetLidarData_CB64S1_A;				// 创建CB64S1_A的类
#endif


// 创建CH1W的类
#ifdef GetLidarData_CH1W_MACRO
#include "GetLidarData_CH1W.h"
#include "GetLidarData_CH1W.cpp"
			 GetLidarData* m_GetLidarData = new GetLidarData_CH1W;				// 创建CH1W的类
#endif

// 创建C16_v3_0的类
#ifdef GetLidarData_C16_v3_0_MACRO
#include "GetLidarData_C16_v3_0.h"
#include "GetLidarData_C16_v3_0.cpp"
			 GetLidarData* m_GetLidarData = new GetLidarData_C16_v3_0;			// 创建C16_v3_0的类
#endif

// 创建C32_v3_0的类
#ifdef GetLidarData_C32_v3_0_MACRO
#include "GetLidarData_C32_v3_0.h"
#include "GetLidarData_C32_v3_0.cpp"
			 GetLidarData* m_GetLidarData = new GetLidarData_C32_v3_0;			// 创建C32_v3_0的类
#endif

// 创建C16_v4_0的类
#ifdef GetLidarData_C16_v4_0_MACRO
#include "GetLidarData_C16_v4_0.h"
#include "GetLidarData_C16_v4_0.cpp"
			 GetLidarData* m_GetLidarData = new GetLidarData_C16_v4_0;			// 创建C16_v4_0的类
#endif

// 创建C32_v4_0的类
#ifdef GetLidarData_C32_v4_0_MACRO
#include "GetLidarData_C32_v4_0.h"
#include "GetLidarData_C32_v4_0.cpp"
			 GetLidarData* m_GetLidarData = new GetLidarData_C32_v4_0;			// 创建C32_v4_0的类
#endif

// 创建C8_v4_0的类
#ifdef GetLidarData_C8_v4_0_MACRO
#include "GetLidarData_C8_v4_0.h"
#include "GetLidarData_C8_v4_0.cpp"
			 GetLidarData* m_GetLidarData = new GetLidarData_C8_v4_0;			// 创建C8_v4_0的类
#endif

// 创建C1_v4_0的类
#ifdef GetLidarData_C1_v4_0_MACRO
#include "GetLidarData_C1_v4_0.h"
#include "GetLidarData_C1_v4_0.cpp"
			 GetLidarData* m_GetLidarData = new GetLidarData_C1_v4_0;			// 创建C1_v4_0的类
#endif

// 创建C32_70D_v4_0的类
#ifdef GetLidarData_C32_70D_v4_0_MACRO
#include "GetLidarData_C32_70D_v4_0.h"
#include "GetLidarData_C32_70D_v4_0.cpp"
			 GetLidarData* m_GetLidarData = new GetLidarData_C32_70D_v4_0;		// 创建C32_70D_v4_0的类
#endif

// 创建C32W_v4_0的类
#ifdef GetLidarData_C32W_v4_0_MACRO
#include "GetLidarData_C32W_v4_0.h"
#include "GetLidarData_C32W_v4_0.cpp"
			 GetLidarData* m_GetLidarData = new GetLidarData_C32W_v4_0;		// 创建C32W_v4_0的类
#endif

// 创建C32_90D_v4_0的类
#ifdef GetLidarData_CH32R_v4_0_MACRO
#include "GetLidarData_CH32R_v4_0.h"
#include "GetLidarData_CH32R_v4_0.cpp"
			 GetLidarData* m_GetLidarData = new GetLidarData_C32_90D_v4_0;		// 创建C32_90D_v4_0的类
#endif

// 创建LS的类
#ifdef GetLidarData_LS_MACRO
#include "GetLidarData_LS.h"
#include "GetLidarData_LS.cpp"
			 GetLidarData* m_GetLidarData = new GetLidarData_LS;				// 创建LS的类
#endif

// 创建LS500W1的类
#ifdef GetLidarData_LS500W1_MACRO
#include "GetLidarData_LS500W1.h"
#include "GetLidarData_LS500W1.cpp"
			 GetLidarData* m_GetLidarData = new GetLidarData_LS500W1;				// 创建LS500W1的类
#endif

//创建N301的类 //6.0雷达1.7协议
#ifdef GetLidarData_N301_L6_v1_7MACRO      
#include "GetLidarData_N301.h"
#include "GetLidarData_N301.cpp"
			 GetLidarData* m_GetLidarData = new GetLidarData_N301(4, 4, 7);						//创建N301的类 //6.0雷达1.7协议
#endif

//创建N301的类 //5.0雷达1.7协议
#ifdef GetLidarData_N301_L5_v1_7MACRO     
#include "GetLidarData_N301.h"
#include "GetLidarData_N301.cpp"
			 GetLidarData* m_GetLidarData = new GetLidarData_N301(4, 1, 7);					//创建N301的类 //5.0雷达1.7协议
#endif

//创建N301的类 //5.0雷达1.6协议
#ifdef GetLidarData_N301_L5_v1_6MACRO      
#include "GetLidarData_N301.h"
#include "GetLidarData_N301.cpp"
			 GetLidarData* m_GetLidarData = new GetLidarData_N301(2, 1, 6);						//创建N301的类 //5.0雷达1.6协议
#endif
