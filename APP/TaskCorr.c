#include "TaskCorr.h"
#include "lcd.h"
#include <LPC17xx.h>
#include "datavalue.h"
#include "panel.h"
#include "keyfuntion.h"
#include "readandwrite.h"
#include "uart.h"



#define TASK_STK_SIZE 512      // 任务堆栈长度

OS_STK TaskStk[TASK_STK_SIZE]; // 任务堆栈




/**
*@Description: 系统操作任务   通信和键盘操作
*@paramreq- 
*@paramind- 
*@return 
*/
void Opera_Task(void *data)
{
/*	
	uint32 value = 0;
	uint8 local_flag; // 远近程状态标志
	uint8 loadValue = 0;
	
	data=data;
	while(1)
	{	
		local_flag = Read_Local(); // 获取远近程状态
	  loadValue = Read_Load();
	  if(loadValue != 0)   
	  {
	    BSP_PWMDutySet(40);  // 负载打开,处于测试状态  风扇转   (原程序 TS_TestSched.c的1655)
    }
		value = ReadKeyValue();
		if(local_flag !=1) // 远程状态 通信
		{
			if(value == KEY_VAL_9)
			{
				Reversal_Local(); // 如果按下Local键 翻转标志
				WriteKeyValue(0x00);
				value = KEY_NULL;
			}
		}
		else // 本地状态 键盘操作
		{
			if(value != KEY_NULL)
			{
				KeyFunction(value);   // 如果按键不为空 则执行
				WriteKeyValue(0x00);
				value = KEY_NULL;
			}
		}
//		OSTimeDlyHMSM(0,0,0,10); // 延时 10ms
	}
*/	
}









/**
*@Description: 系统开始任务  只用于创建运行任务  之后删除
*@paramreq- 
*@paramind- 
*@return 
*/
void TaskStart(void *data)
{
	OS_TMR *pTimer0 = 0;
	INT8U err = 0;
	
	data=data;
	OS_CPU_SysTickInit(SystemFrequency/100); // 系统时钟 1M
	OSTaskCreate(Opera_Task, (void *)0, &TaskStk[TASK_STK_SIZE], 2); // 创建运行任务 优先级2
	
/*	
	pTimer0 = OSTmrCreate(0,
	                      OS_TMR_CFG_TICKS_PER_SEC,
	                      OS_TMR_OPT_PERIODIC,
	                      (OS_TMR_CALLBACK)OS_Timer0,
*/	                      
}
















