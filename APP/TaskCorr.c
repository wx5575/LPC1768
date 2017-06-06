#include "TaskCorr.h"
#include "lcd.h"
#include <LPC17xx.h>
#include "datavalue.h"
#include "panel.h"
#include "keyfuntion.h"
#include "readandwrite.h"
#include "uart.h"



#define TASK_STK_SIZE 512      // �����ջ����

OS_STK TaskStk[TASK_STK_SIZE]; // �����ջ




/**
*@Description: ϵͳ��������   ͨ�źͼ��̲���
*@paramreq- 
*@paramind- 
*@return 
*/
void Opera_Task(void *data)
{
/*	
	uint32 value = 0;
	uint8 local_flag; // Զ����״̬��־
	uint8 loadValue = 0;
	
	data=data;
	while(1)
	{	
		local_flag = Read_Local(); // ��ȡԶ����״̬
	  loadValue = Read_Load();
	  if(loadValue != 0)   
	  {
	    BSP_PWMDutySet(40);  // ���ش�,���ڲ���״̬  ����ת   (ԭ���� TS_TestSched.c��1655)
    }
		value = ReadKeyValue();
		if(local_flag !=1) // Զ��״̬ ͨ��
		{
			if(value == KEY_VAL_9)
			{
				Reversal_Local(); // �������Local�� ��ת��־
				WriteKeyValue(0x00);
				value = KEY_NULL;
			}
		}
		else // ����״̬ ���̲���
		{
			if(value != KEY_NULL)
			{
				KeyFunction(value);   // ���������Ϊ�� ��ִ��
				WriteKeyValue(0x00);
				value = KEY_NULL;
			}
		}
//		OSTimeDlyHMSM(0,0,0,10); // ��ʱ 10ms
	}
*/	
}









/**
*@Description: ϵͳ��ʼ����  ֻ���ڴ�����������  ֮��ɾ��
*@paramreq- 
*@paramind- 
*@return 
*/
void TaskStart(void *data)
{
	OS_TMR *pTimer0 = 0;
	INT8U err = 0;
	
	data=data;
	OS_CPU_SysTickInit(SystemFrequency/100); // ϵͳʱ�� 1M
	OSTaskCreate(Opera_Task, (void *)0, &TaskStk[TASK_STK_SIZE], 2); // ������������ ���ȼ�2
	
/*	
	pTimer0 = OSTmrCreate(0,
	                      OS_TMR_CFG_TICKS_PER_SEC,
	                      OS_TMR_OPT_PERIODIC,
	                      (OS_TMR_CALLBACK)OS_Timer0,
*/	                      
}
















