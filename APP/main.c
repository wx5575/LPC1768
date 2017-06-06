/**
*@file			
*@brief			main������һЩ���������ĵ��� ���û���
*@version		
*@author		
*@date		
*/
#include <LPC17xx.h>
#include "bsp_gpio.h"
#include "uart.h"
#include "bsp_timer.h"
#include "buzzer.h"
#include "EINT.h"
#include "lcd.h"
#include "key.h"
#include "TaskCorr.h"
#include "delay.h"
#include "readandwrite.h"
#include "datavalue.h"
#include "panel.h"
 

#include "at24cxx.h"
#include "test.h"
#include "AD.h"
#include "DA.h"
#include "resreadandwrite.h"








#define TASKStart_STK_SIZE 512       // �����ջ����

OS_STK TaskStartStk[TASKStart_STK_SIZE]; // ��ʼ�����ջ



void TestCtrlPortInit(void);
void EquipmentInit(void);




int main(void)
{	
//	uint8 i = 0;
//	uint8 strsys[9]={'#'};
	unsigned char volt[20] = {0};
	unsigned char curr[20] = {0};

	SystemInit();                     // ϵͳ��ʼ��
	TestCtrlPortInit();               // ���ų�ʼ��
	EquipmentInit();                  // ��ʼ��������

/*
LTC2440_Open();
	AD5545_Open();
	SPISendData(0x1FFF);
while(1)
{
//	SPISendData();
	if(SUCCESS == GetSamFinishFlag())
	{
		GetADValue(g_AD_ADValue, volt, curr);
		LCD_Write(volt, 0,0);
		LCD_Write(curr, 9,0);
		SoftDelayMs(800);
		NVIC_EnableIRQ(EINT2_IRQn);
	}

}
*/
	
	OSInit();    // ��ʼ�� uC/OS-II
	OSTaskCreate(TaskStart, (void *)0, &TaskStartStk[TASKStart_STK_SIZE - 1], 0);  // ������ʼ������
	OSStart();   // ���� uC/OS-II
	
	return 0;
}




/**
*@Description: ��ʼ�����ź���
*@paramreq- 
*@paramind- 
*@return 
*/
void TestCtrlPortInit(void)
{
    // �˴������źŵĳ�ʼ������Ӧ������
    BSP_GPIODirectionSet(PORT0, ON_OFF_EN_H_BIT, DIR_OUTPUT, PULL_UP_RES);     //
    BSP_GPIOValueSet(PORT0, ON_OFF_EN_H_BIT, 0);                               //  ����PORT0��P0.1����͵�ƽ 
    BSP_GPIODirectionSet(PORT1, CC_EN_L_BIT, DIR_OUTPUT, PULL_UP_RES);
    BSP_GPIOValueSet(PORT1, CC_EN_L_BIT, 0);                                   //����P1.22Ϊ�͵�ƽ

    BSP_GPIODirectionSet(PORT1 , SENSE_EN_H_BIT , DIR_OUTPUT, PULL_UP_RES);
    BSP_GPIODirectionSet(PORT1 , CR_EN_L_BIT    , DIR_OUTPUT, PULL_UP_RES);
    BSP_GPIODirectionSet(PORT1 , CG_EN_L_BIT    , DIR_OUTPUT, PULL_UP_RES);
    BSP_GPIODirectionSet(PORT1 , HIGH_L_BIT     , DIR_OUTPUT, PULL_UP_RES);
    BSP_GPIODirectionSet(PORT1 , CV_EN_L_BIT    , DIR_OUTPUT, PULL_UP_RES);
    
    BSP_GPIODirectionSet(PORT0 , CC_RANGE_L_BIT , DIR_OUTPUT, NONE_RES);
    BSP_GPIODirectionSet(PORT0 , CV_RANGE_L_BIT , DIR_OUTPUT, NONE_RES);
    BSP_GPIODirectionSet(PORT0 , MEAS_V_L_BIT   , DIR_OUTPUT, PULL_UP_RES);
    BSP_GPIODirectionSet(PORT1 , MEAS_I_L_BIT   , DIR_OUTPUT, PULL_UP_RES);   
    BSP_GPIODirectionSet(PORT1 , DAC_REF_L_BIT  , DIR_OUTPUT, PULL_UP_RES);      
    
    BSP_GPIODirectionSet(PORT0 , SLW1_L_BIT, DIR_OUTPUT, NONE_RES);
    BSP_GPIODirectionSet(PORT0 , SLW2_L_BIT, DIR_OUTPUT, NONE_RES);
    BSP_GPIODirectionSet(PORT3 , SLW3_L_BIT, DIR_OUTPUT, PULL_UP_RES);
    BSP_GPIODirectionSet(PORT3 , SLW4_L_BIT, DIR_OUTPUT, PULL_UP_RES);

    // ���س�ʼ��Ϊ��
    BSP_GPIOValueSet(PORT0 , ON_OFF_EN_H_BIT, 0);

    // ��Щ���ƿ��߰���CCHģʽ�Ŀ����źų�ʼ��
    BSP_GPIOValueSet(PORT0 , CV_RANGE_L_BIT, 0);
    
    BSP_GPIOValueSet(PORT1 , CV_EN_L_BIT   , 1);
    BSP_GPIOValueSet(PORT1 , CG_EN_L_BIT   , 1);
    BSP_GPIOValueSet(PORT1 , CR_EN_L_BIT   , 1);
    BSP_GPIOValueSet(PORT1 , DAC_REF_L_BIT , 0);
		
    BSP_GPIOValueSet(PORT0 , CC_RANGE_L_BIT, 0);

    // OVP����ܽų�ʼ��    
    BSP_GPIODirectionSet(PORT1 , OVP_CLR_L_BIT, DIR_OUTPUT, PULL_UP_RES);
    BSP_GPIOValueSet(PORT1 , OVP_CLR_L_BIT, 1);

    // OVP���ܽų�ʼ��    
    BSP_GPIODirectionSet(PORT2 , OVP_INT_L_BIT, DIR_INPUT, PULL_UP_RES);
    
    // ����źŹܽų�ʼ��
    BSP_GPIODirectionSet(PORT2 , OUT_SIGNAL_H_BIT, DIR_OUTPUT, PULL_UP_RES);

    // ��ʼ���ɲ�����ѹ
    BSP_GPIOValueSet(PORT0 , MEAS_V_L_BIT, 0);
    BSP_GPIOValueSet(PORT1 , MEAS_I_L_BIT, 1);
}




/**
*@Description: ������ģ���ʼ��
*@paramreq- 
*@paramind- 
*@return 
*/
void EquipmentInit(void)
{	
	TimerInit(TIMER0);                 // ��ʼ����ʱ��0
  TimerInit(TIMER1);
  TimerInit(TIMER2);
	
  TimerInit(TIMER3);                 // ��ʱ��3
	
	PWMInit();
	PWMStart();
	// �ⲿ�жϳ�ʼ��
    EINT0_Init();
    EINT1_Init();
//    EINT3_Init();                  //  �жϺ�����ûд��  
//    EINT_GPIOINTForUNRInit();      //  ���ⲿ�ж� 3 �й�
	BuzzerInit();                      // ��ʼ��������
	
	AT24Cxx_Check();
	Lcd_Init();                        // ��ʼ��LCD
	SoftDelayMs(1000);
	Lcd_Open();                        // ����LCD����,�ܹ�д
	LCD_Clear();
	Key_Open();                        // ����ɨ�迪
	
	
	MainPanel();
	TimerStart(TIMER0);              // ������ʱ��
}
