#ifndef  AD_H
#define  AD_H


#include "BSP_CpuConfig.h"




#define  AD_SAMPLE_NUMBS_MAX_VAL          (50)   // 电压电流采样组数最大值

// AD快速操作命令宏定义
#define  LTC2440_QUICK_PROC               (0)    // 一轮取样过程完成查询




extern uint32   g_AD_ADValue[2*AD_SAMPLE_NUMBS_MAX_VAL];   // 供外部获取电压电流采样值












uint8 DEV_AD_SelfChk(void);
uint8 LTC2440_Open(void);
uint8 GetSamFinishFlag(void);





#endif


