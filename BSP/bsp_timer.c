/**
*@file			
*@brief			定时器和pwm等控制
*@version		
*@author		
*@date		
*/
#include "bsp_timer.h"
#include <LPC17xx.h>
#include "key.h"
#include "keyfuntion.h"
#include "lcd.h"





/** 定时器宏设置时间*/
#define     TIMER0_INT_TIME                 (10)
/** 控制寄存器位宏定义*/
#define     TIMER_EN                        (0)
#define     TIMER_RESET                     (1)

/** 中断标志位宏定义 */
#define         MR0_INT                                         (0)
#define         MR1_INT                                         (1)
#define         MR2_INT                                         (2)
#define         MR3_INT                                         (3)

//static uint8 flag = 0;


/**
*@Description: 初始化定时器
*@paramreq-   CTCR: 计数控制寄存器   CR: 捕获寄存器  PR: 预分频寄存器   MR: 匹配寄存器
*@paramind- 
*@return 无
*/
portuBASE_TYPE  TimerInit(TimerNumb timerNumb)
{	
    if (TIMER0 == timerNumb)
    {
        // 复位后功率控制寄存器中Timer0默认使能
        // 复位后定时器时钟源为1/4内核时钟
        // 计数控制寄存器最低两位为0选择定时器模式
        LPC_TIM0->CTCR         &= 0xFFFFFFFC;             
        // 预分频寄存器 不分频
        LPC_TIM0->PR            = 0;
        // 匹配寄存器 除以4000因为定时器时钟源为内核时钟1/4分频
        LPC_TIM0->MR0           = TIMER0_INT_TIME*SystemFrequency/4000-1;
        // 匹配控制寄存器 匹配时中断 并复位TC值
        LPC_TIM0->MCR          |= (1 << 0)|(1 << 1);

        NVIC_SetPriority(TIMER0_IRQn, (1<<__NVIC_PRIO_BITS) - 3);
        NVIC_EnableIRQ(TIMER0_IRQn);
    }
    else if (TIMER1 == timerNumb)
    {
        // 复位后功率控制寄存器中Timer1默认使能
        // 复位后定时器时钟源为1/4内核时钟
        // 计数控制寄存器最低两位为0选择定时器模式
        LPC_TIM1->CTCR         &= 0xFFFFFFFC;             // 定时器模式
        // 预分频寄存器 不分频
        LPC_TIM1->PR            = 0;                      // PR=0:定时器计数器每个 PLCK 周期加1    PR=1:每2个 PCLK 周期加 1
        // 根据需要调用BSP_TimerXTimeValSet()函数重新进行定时值设定
        LPC_TIM1->MR0           = 123456;        // 123456 = ...000b  禁止中断  禁止MR0引发的复位  禁止MR0引发的停止
        // 匹配控制寄存器 匹配时中断 并复位TC值
        LPC_TIM1->MCR          |= (1 << 0)|(1 << 1);    // 匹配控制寄存器   11 ==> MR0与TC值匹配时产生中断  MR0与TC值匹配时TC复位  

        NVIC_SetPriority(TIMER1_IRQn, (1<<__NVIC_PRIO_BITS) - 15);    
        NVIC_EnableIRQ(TIMER1_IRQn);
    }
    else if (TIMER2 == timerNumb)
    {
        // 功率控制寄存器中打开Timer2
        LPC_SC->PCONP          |= (1 << 22);
        // 复位后定时器时钟源为1/4内核时钟
        // 计数控制寄存器最低两位为0选择定时器模式
        LPC_TIM2->CTCR         &= 0xFFFFFFFC;
        // 预分频寄存器 不分频
        LPC_TIM2->PR            = 0;
        // 根据需要调用BSP_TimerXTimeValSet()函数重新进行定时值设定
        LPC_TIM2->MR0           = 123456;
        // 匹配控制寄存器 匹配时中断 并复位TC值
        LPC_TIM2->MCR          |= (1 << 0)|(1 << 1);

        NVIC_SetPriority(TIMER2_IRQn, (1<<__NVIC_PRIO_BITS) - 15);
        NVIC_EnableIRQ(TIMER2_IRQn);
    }
    else
    {
        // 功率控制寄存器中打开Timer3
        LPC_SC->PCONP          |= (1 << 23);
        // 复位后定时器时钟源为1/4内核时钟
        // 计数控制寄存器最低两位为0选择定时器模式
        LPC_TIM3->CTCR         &= 0xFFFFFFFC;
        // 预分频寄存器 不分频
        LPC_TIM3->PR            = 0;
        // 根据需要调用BSP_TimerXTimeValSet()函数重新进行定时值设定
        LPC_TIM3->MR0           = 123456;
        // 匹配控制寄存器 匹配时中断 并复位TC值 停止定时器
        LPC_TIM3->MCR          |= (1 << 0)|(1 << 1)|(1 << 2);  

        NVIC_SetPriority(TIMER3_IRQn, (1<<__NVIC_PRIO_BITS) - 10);
        NVIC_EnableIRQ(TIMER3_IRQn);
    }

    return TRUE;
}




/**
*@Description: 初始化定时器
*@paramreq-   MCR: 匹配控制寄存器   PR: 预分频寄存器  PCR: PWM控制存器   MR: 匹配寄存器
*@paramind-   PCONP : 外设功率控制寄存器      TCR: 定时器控制寄存器     
*@return 成功标志
*/
portuBASE_TYPE  PWMInit(void)
{
    // 引脚初始化为PWM输出功能 PWM1.6
    LPC_PINCON->PINSEL3     = (LPC_PINCON->PINSEL3 & 0xFFCFFFFF)|(0x00200000);
    LPC_PINCON->PINMODE3    = (LPC_PINCON->PINMODE3 & 0xFFCFFFFF)|(0x00200000);

    LPC_SC->PCONP          |= (1 << 6);
    // 匹配控制寄存器设置 MR0匹配时复位TC
    LPC_PWM1->MCR           = (1 << 0)|(1 << 1);
    // PWM1.6单边沿输出 并使能输出
    LPC_PWM1->PCR           = (1 << 14);
    // 外设时钟不分频
    LPC_PWM1->PR            = 0;
    // 必须在PWM模式使能前设置该寄存器 设定MR0值 即设定PWM输出频率(定时间隔1ms 则输出频率1kHz)
    LPC_PWM1->MR0           = 1*SystemFrequency/4000-1;
    //LPC_PWM1->MR0           = 2000;
    // 设定占空比
    LPC_PWM1->MR6           = 1*(SystemFrequency/4000-1)/100;
    // PWM模式使能
    LPC_PWM1->TCR           = (1 << 3);

    //NVIC_SetPriority(PWM1_IRQn, (1<<__NVIC_PRIO_BITS) - 10);
    //NVIC_EnableIRQ(PWM1_IRQn);
    return SUCCESS;
}


/**
*@Description: 
*@paramreq-   
*@paramind-    TCR: 定时器控制寄存器     
*@return 无
*/
portuBASE_TYPE  PWMStart(void)
{
    LPC_PWM1->TCR |= (1 << TIMER_EN);          // 定时器使能
    return SUCCESS;
} 


/**
*@Description: 
*@paramreq-   
*@paramind-    TCR: 定时器控制寄存器     
*@return 无
*/
portuBASE_TYPE  PWMClose(void)
{
    LPC_PWM1->TCR  |= (1 << TIMER_RESET);    // 同步复位    

    return SUCCESS;
}


/**
*@Description: 定时器0中断
*@paramreq-   
*@paramind-     
*@return 无
*/
void TIMER0_IRQHandler(void)
{
	uint32 key_value = KEY_NULL;
	
	key_value = Key_Scan();
	if(key_value != KEY_NULL)
	{
		if(keystate.press <= SCAN_NUM)
		{
			keystate.press++;
			WriteKeyValue(key_value);
		}
	}
	else
	{
		if(keystate.press >= SCAN_NUM)
		{
			if(keystate.pop <= SCAN_NUM)
			{
				keystate.pop++;
			}
			else if(keystate.pop >= SCAN_NUM)
			{		
				key_value = ReadKeyValue();
				WriteKeyValue(key_value);
				KeyFunction(key_value);
				keystate.press = 0;
				keystate.pop   = 0;
			}
			else
				;
		}
	}
  // 清除中断标志    向对应的 IR 位写入 1 会复位中断 . 写入 0 无效
	LPC_TIM0->IR |= (uint32)1 << MR0_INT;
}



/**
*@Description: 定时器1中断
*@paramreq-   
*@paramind-     
*@return 无
*/
void TIMER1_IRQHandler(void)
{
    // 清除中断标志
	LPC_TIM1->IR |= (uint32)1 << MR0_INT;
}



/**
*@Description: 定时器2中断
*@paramreq-   
*@paramind-     
*@return 无
*/
void TIMER2_IRQHandler(void)
{	
    // 清除中断标志
    LPC_TIM2->IR |= (uint32)1 << MR0_INT;
}



/**
*@Description: 定时器3中断
*@paramreq-   
*@paramind-     
*@return 无
*/
void TIMER3_IRQHandler(void)
{	
    // 清除中断标志
	LPC_TIM3->IR |= (uint32)1 << MR3_INT;
}



/**
*@Description: 定时器使能
*@paramreq-   TCR: 定时器控制寄存器
*@paramind-     
*@return 无
*/
portuBASE_TYPE  TimerStart(TimerNumb timerNumb)     
{
    if (TIMER0 == timerNumb)
    {
        LPC_TIM0->TCR          |= (1 << TIMER_EN);
    }
    else if (TIMER1 == timerNumb)
    {
        LPC_TIM1->TCR          |= (1 << TIMER_EN);
    }
    else if (TIMER2 == timerNumb)
    {
        LPC_TIM2->TCR          |= (1 << TIMER_EN);
    }
    else
    {
        LPC_TIM3->TCR          = (1 << TIMER_EN);
    }

    return TRUE;
}



/**
*@Description: 定时器禁能
*@paramreq-   TCR: 定时器控制寄存器
*@paramind-     
*@return 无
*/
portuBASE_TYPE  TimerClose(TimerNumb timerNumb)   
{
    if (TIMER0 == timerNumb)
    {
        LPC_TIM0->TCR          |= (1 << TIMER_RESET);
    }
    else if (TIMER1 == timerNumb)
    {
        LPC_TIM1->TCR          |= (1 << TIMER_RESET);
    }
    else if (TIMER2 == timerNumb)
    {
        LPC_TIM2->TCR          |= (1 << TIMER_RESET);
    }
    else
    {
        LPC_TIM3->TCR          = (1 << TIMER_RESET);
    }

    return TRUE;         
}




/**
*@Description: 设置风扇转速    
*@paramreq-  duty:比例   0-100 之间 ==>值越大风扇转速越快 
*@paramind-     
*@return 
*/
uint8 BSP_PWMDutySet(portuBASE_TYPE duty)
{
    if (duty > 100)
    {
			duty = 100;
    }
    LPC_PWM1->MR6  = duty*(SystemFrequency/4000-1)/100;
    LPC_PWM1->LER  = (1 << 6);

    return SUCCESS;
}











