#ifndef  BSP_TIMER_H_
#define  BSP_TIMER_H_



#include "BSP_CpuConfig.h"





typedef enum TIMER_NUMB {TIMER0=0, TIMER1, TIMER2, TIMER3} TimerNumb;



portuBASE_TYPE  TimerInit(TimerNumb timerNumb);

portuBASE_TYPE  PWMInit(void);

portuBASE_TYPE  PWMStart(void);

portuBASE_TYPE  TimerStart(TimerNumb timerNumb);

uint8 BSP_PWMDutySet(portuBASE_TYPE duty);







#endif
