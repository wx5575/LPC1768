/**
*@file			
*@brief			��ʱ����pwm�ȿ���
*@version		
*@author		
*@date		
*/
#include "bsp_timer.h"
#include <LPC17xx.h>
#include "key.h"
#include "keyfuntion.h"
#include "lcd.h"





/** ��ʱ��������ʱ��*/
#define     TIMER0_INT_TIME                 (10)
/** ���ƼĴ���λ�궨��*/
#define     TIMER_EN                        (0)
#define     TIMER_RESET                     (1)

/** �жϱ�־λ�궨�� */
#define         MR0_INT                                         (0)
#define         MR1_INT                                         (1)
#define         MR2_INT                                         (2)
#define         MR3_INT                                         (3)

//static uint8 flag = 0;


/**
*@Description: ��ʼ����ʱ��
*@paramreq-   CTCR: �������ƼĴ���   CR: ����Ĵ���  PR: Ԥ��Ƶ�Ĵ���   MR: ƥ��Ĵ���
*@paramind- 
*@return ��
*/
portuBASE_TYPE  TimerInit(TimerNumb timerNumb)
{	
    if (TIMER0 == timerNumb)
    {
        // ��λ���ʿ��ƼĴ�����Timer0Ĭ��ʹ��
        // ��λ��ʱ��ʱ��ԴΪ1/4�ں�ʱ��
        // �������ƼĴ��������λΪ0ѡ��ʱ��ģʽ
        LPC_TIM0->CTCR         &= 0xFFFFFFFC;             
        // Ԥ��Ƶ�Ĵ��� ����Ƶ
        LPC_TIM0->PR            = 0;
        // ƥ��Ĵ��� ����4000��Ϊ��ʱ��ʱ��ԴΪ�ں�ʱ��1/4��Ƶ
        LPC_TIM0->MR0           = TIMER0_INT_TIME*SystemFrequency/4000-1;
        // ƥ����ƼĴ��� ƥ��ʱ�ж� ����λTCֵ
        LPC_TIM0->MCR          |= (1 << 0)|(1 << 1);

        NVIC_SetPriority(TIMER0_IRQn, (1<<__NVIC_PRIO_BITS) - 3);
        NVIC_EnableIRQ(TIMER0_IRQn);
    }
    else if (TIMER1 == timerNumb)
    {
        // ��λ���ʿ��ƼĴ�����Timer1Ĭ��ʹ��
        // ��λ��ʱ��ʱ��ԴΪ1/4�ں�ʱ��
        // �������ƼĴ��������λΪ0ѡ��ʱ��ģʽ
        LPC_TIM1->CTCR         &= 0xFFFFFFFC;             // ��ʱ��ģʽ
        // Ԥ��Ƶ�Ĵ��� ����Ƶ
        LPC_TIM1->PR            = 0;                      // PR=0:��ʱ��������ÿ�� PLCK ���ڼ�1    PR=1:ÿ2�� PCLK ���ڼ� 1
        // ������Ҫ����BSP_TimerXTimeValSet()�������½��ж�ʱֵ�趨
        LPC_TIM1->MR0           = 123456;        // 123456 = ...000b  ��ֹ�ж�  ��ֹMR0�����ĸ�λ  ��ֹMR0������ֹͣ
        // ƥ����ƼĴ��� ƥ��ʱ�ж� ����λTCֵ
        LPC_TIM1->MCR          |= (1 << 0)|(1 << 1);    // ƥ����ƼĴ���   11 ==> MR0��TCֵƥ��ʱ�����ж�  MR0��TCֵƥ��ʱTC��λ  

        NVIC_SetPriority(TIMER1_IRQn, (1<<__NVIC_PRIO_BITS) - 15);    
        NVIC_EnableIRQ(TIMER1_IRQn);
    }
    else if (TIMER2 == timerNumb)
    {
        // ���ʿ��ƼĴ����д�Timer2
        LPC_SC->PCONP          |= (1 << 22);
        // ��λ��ʱ��ʱ��ԴΪ1/4�ں�ʱ��
        // �������ƼĴ��������λΪ0ѡ��ʱ��ģʽ
        LPC_TIM2->CTCR         &= 0xFFFFFFFC;
        // Ԥ��Ƶ�Ĵ��� ����Ƶ
        LPC_TIM2->PR            = 0;
        // ������Ҫ����BSP_TimerXTimeValSet()�������½��ж�ʱֵ�趨
        LPC_TIM2->MR0           = 123456;
        // ƥ����ƼĴ��� ƥ��ʱ�ж� ����λTCֵ
        LPC_TIM2->MCR          |= (1 << 0)|(1 << 1);

        NVIC_SetPriority(TIMER2_IRQn, (1<<__NVIC_PRIO_BITS) - 15);
        NVIC_EnableIRQ(TIMER2_IRQn);
    }
    else
    {
        // ���ʿ��ƼĴ����д�Timer3
        LPC_SC->PCONP          |= (1 << 23);
        // ��λ��ʱ��ʱ��ԴΪ1/4�ں�ʱ��
        // �������ƼĴ��������λΪ0ѡ��ʱ��ģʽ
        LPC_TIM3->CTCR         &= 0xFFFFFFFC;
        // Ԥ��Ƶ�Ĵ��� ����Ƶ
        LPC_TIM3->PR            = 0;
        // ������Ҫ����BSP_TimerXTimeValSet()�������½��ж�ʱֵ�趨
        LPC_TIM3->MR0           = 123456;
        // ƥ����ƼĴ��� ƥ��ʱ�ж� ����λTCֵ ֹͣ��ʱ��
        LPC_TIM3->MCR          |= (1 << 0)|(1 << 1)|(1 << 2);  

        NVIC_SetPriority(TIMER3_IRQn, (1<<__NVIC_PRIO_BITS) - 10);
        NVIC_EnableIRQ(TIMER3_IRQn);
    }

    return TRUE;
}




/**
*@Description: ��ʼ����ʱ��
*@paramreq-   MCR: ƥ����ƼĴ���   PR: Ԥ��Ƶ�Ĵ���  PCR: PWM���ƴ���   MR: ƥ��Ĵ���
*@paramind-   PCONP : ���蹦�ʿ��ƼĴ���      TCR: ��ʱ�����ƼĴ���     
*@return �ɹ���־
*/
portuBASE_TYPE  PWMInit(void)
{
    // ���ų�ʼ��ΪPWM������� PWM1.6
    LPC_PINCON->PINSEL3     = (LPC_PINCON->PINSEL3 & 0xFFCFFFFF)|(0x00200000);
    LPC_PINCON->PINMODE3    = (LPC_PINCON->PINMODE3 & 0xFFCFFFFF)|(0x00200000);

    LPC_SC->PCONP          |= (1 << 6);
    // ƥ����ƼĴ������� MR0ƥ��ʱ��λTC
    LPC_PWM1->MCR           = (1 << 0)|(1 << 1);
    // PWM1.6��������� ��ʹ�����
    LPC_PWM1->PCR           = (1 << 14);
    // ����ʱ�Ӳ���Ƶ
    LPC_PWM1->PR            = 0;
    // ������PWMģʽʹ��ǰ���øüĴ��� �趨MR0ֵ ���趨PWM���Ƶ��(��ʱ���1ms �����Ƶ��1kHz)
    LPC_PWM1->MR0           = 1*SystemFrequency/4000-1;
    //LPC_PWM1->MR0           = 2000;
    // �趨ռ�ձ�
    LPC_PWM1->MR6           = 1*(SystemFrequency/4000-1)/100;
    // PWMģʽʹ��
    LPC_PWM1->TCR           = (1 << 3);

    //NVIC_SetPriority(PWM1_IRQn, (1<<__NVIC_PRIO_BITS) - 10);
    //NVIC_EnableIRQ(PWM1_IRQn);
    return SUCCESS;
}


/**
*@Description: 
*@paramreq-   
*@paramind-    TCR: ��ʱ�����ƼĴ���     
*@return ��
*/
portuBASE_TYPE  PWMStart(void)
{
    LPC_PWM1->TCR |= (1 << TIMER_EN);          // ��ʱ��ʹ��
    return SUCCESS;
} 


/**
*@Description: 
*@paramreq-   
*@paramind-    TCR: ��ʱ�����ƼĴ���     
*@return ��
*/
portuBASE_TYPE  PWMClose(void)
{
    LPC_PWM1->TCR  |= (1 << TIMER_RESET);    // ͬ����λ    

    return SUCCESS;
}


/**
*@Description: ��ʱ��0�ж�
*@paramreq-   
*@paramind-     
*@return ��
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
  // ����жϱ�־    ���Ӧ�� IR λд�� 1 �Ḵλ�ж� . д�� 0 ��Ч
	LPC_TIM0->IR |= (uint32)1 << MR0_INT;
}



/**
*@Description: ��ʱ��1�ж�
*@paramreq-   
*@paramind-     
*@return ��
*/
void TIMER1_IRQHandler(void)
{
    // ����жϱ�־
	LPC_TIM1->IR |= (uint32)1 << MR0_INT;
}



/**
*@Description: ��ʱ��2�ж�
*@paramreq-   
*@paramind-     
*@return ��
*/
void TIMER2_IRQHandler(void)
{	
    // ����жϱ�־
    LPC_TIM2->IR |= (uint32)1 << MR0_INT;
}



/**
*@Description: ��ʱ��3�ж�
*@paramreq-   
*@paramind-     
*@return ��
*/
void TIMER3_IRQHandler(void)
{	
    // ����жϱ�־
	LPC_TIM3->IR |= (uint32)1 << MR3_INT;
}



/**
*@Description: ��ʱ��ʹ��
*@paramreq-   TCR: ��ʱ�����ƼĴ���
*@paramind-     
*@return ��
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
*@Description: ��ʱ������
*@paramreq-   TCR: ��ʱ�����ƼĴ���
*@paramind-     
*@return ��
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
*@Description: ���÷���ת��    
*@paramreq-  duty:����   0-100 ֮�� ==>ֵԽ�����ת��Խ�� 
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











