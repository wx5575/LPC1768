#include <LPC17xx.h>
#include <ucos_ii.h>
/*
#define TASK_STK_SIZE 512

OS_STK TaskStartStk[TASK_STK_SIZE];

void TaskStart(void *data);

int main(void)
{
	OSInit();            // ��ʼ�� uC/OS-II

	// �����û����� TaskStart
	OSTaskCreate(TaskStart, (void *)0, &TaskStartStk[TASK_STK_SIZE - 1], 0);

	OSStart();           // ���� uC/OS-II

	return 0;
}

void  TaskStart(void *data)
{
	unsigned int cpu_sr = 0;               // ���� cpu_sr
	data=data;
	
	OS_CPU_SysTickInit(SystemFrequency/100);

	for(;;)
	{
		OSCtxSwCtr = 0;
		OSTimeDlyHMSM(0,0,0,10);
//		OS_ENTER_CRITICAL();           // �����ٽ��(���ж�)
//    �����Ա��жϵ��û�����		
//		OS_EXIT_CRITICAL();            // �˳��ٽ��(���ж�)
		cpu_sr++;
		if(cpu_sr>=100)
		{
			cpu_sr = 0;
		}
	}
}
*/




