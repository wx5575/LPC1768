#ifndef  AD_H
#define  AD_H


#include "BSP_CpuConfig.h"




#define  AD_SAMPLE_NUMBS_MAX_VAL          (50)   // ��ѹ���������������ֵ

// AD���ٲ�������궨��
#define  LTC2440_QUICK_PROC               (0)    // һ��ȡ��������ɲ�ѯ




extern uint32   g_AD_ADValue[2*AD_SAMPLE_NUMBS_MAX_VAL];   // ���ⲿ��ȡ��ѹ��������ֵ












uint8 DEV_AD_SelfChk(void);
uint8 LTC2440_Open(void);
uint8 GetSamFinishFlag(void);





#endif


