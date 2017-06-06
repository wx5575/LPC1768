/**
*@file			
*@brief			main函数和一些驱动函数的调用 设置环境
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








#define TASKStart_STK_SIZE 512       // 任务堆栈长度

OS_STK TaskStartStk[TASKStart_STK_SIZE]; // 开始任务堆栈



void TestCtrlPortInit(void);
void EquipmentInit(void);




int main(void)
{	
//	uint8 i = 0;
//	uint8 strsys[9]={'#'};
	unsigned char volt[20] = {0};
	unsigned char curr[20] = {0};

	SystemInit();                     // 系统初始化
	TestCtrlPortInit();               // 引脚初始化
	EquipmentInit();                  // 初始化各功能

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
	
	OSInit();    // 初始化 uC/OS-II
	OSTaskCreate(TaskStart, (void *)0, &TaskStartStk[TASKStart_STK_SIZE - 1], 0);  // 创建开始任务函数
	OSStart();   // 启动 uC/OS-II
	
	return 0;
}




/**
*@Description: 初始化引脚函数
*@paramreq- 
*@paramind- 
*@return 
*/
void TestCtrlPortInit(void)
{
    // 此处两个信号的初始化操作应尽量早
    BSP_GPIODirectionSet(PORT0, ON_OFF_EN_H_BIT, DIR_OUTPUT, PULL_UP_RES);     //
    BSP_GPIOValueSet(PORT0, ON_OFF_EN_H_BIT, 0);                               //  设置PORT0的P0.1输出低电平 
    BSP_GPIODirectionSet(PORT1, CC_EN_L_BIT, DIR_OUTPUT, PULL_UP_RES);
    BSP_GPIOValueSet(PORT1, CC_EN_L_BIT, 0);                                   //设置P1.22为低电平

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

    // 负载初始化为关
    BSP_GPIOValueSet(PORT0 , ON_OFF_EN_H_BIT, 0);

    // 这些控制口线按照CCH模式的控制信号初始化
    BSP_GPIOValueSet(PORT0 , CV_RANGE_L_BIT, 0);
    
    BSP_GPIOValueSet(PORT1 , CV_EN_L_BIT   , 1);
    BSP_GPIOValueSet(PORT1 , CG_EN_L_BIT   , 1);
    BSP_GPIOValueSet(PORT1 , CR_EN_L_BIT   , 1);
    BSP_GPIOValueSet(PORT1 , DAC_REF_L_BIT , 0);
		
    BSP_GPIOValueSet(PORT0 , CC_RANGE_L_BIT, 0);

    // OVP清除管脚初始化    
    BSP_GPIODirectionSet(PORT1 , OVP_CLR_L_BIT, DIR_OUTPUT, PULL_UP_RES);
    BSP_GPIOValueSet(PORT1 , OVP_CLR_L_BIT, 1);

    // OVP检测管脚初始化    
    BSP_GPIODirectionSet(PORT2 , OVP_INT_L_BIT, DIR_INPUT, PULL_UP_RES);
    
    // 输出信号管脚初始化
    BSP_GPIODirectionSet(PORT2 , OUT_SIGNAL_H_BIT, DIR_OUTPUT, PULL_UP_RES);

    // 初始化成采样电压
    BSP_GPIOValueSet(PORT0 , MEAS_V_L_BIT, 0);
    BSP_GPIOValueSet(PORT1 , MEAS_I_L_BIT, 1);
}




/**
*@Description: 各功能模块初始化
*@paramreq- 
*@paramind- 
*@return 
*/
void EquipmentInit(void)
{	
	TimerInit(TIMER0);                 // 初始化定时器0
  TimerInit(TIMER1);
  TimerInit(TIMER2);
	
  TimerInit(TIMER3);                 // 定时器3
	
	PWMInit();
	PWMStart();
	// 外部中断初始化
    EINT0_Init();
    EINT1_Init();
//    EINT3_Init();                  //  中断函数还没写好  
//    EINT_GPIOINTForUNRInit();      //  与外部中断 3 有关
	BuzzerInit();                      // 初始化蜂鸣器
	
	AT24Cxx_Check();
	Lcd_Init();                        // 初始化LCD
	SoftDelayMs(1000);
	Lcd_Open();                        // 启动LCD功能,能够写
	LCD_Clear();
	Key_Open();                        // 键盘扫描开
	
	
	MainPanel();
	TimerStart(TIMER0);              // 开启定时器
}
