#include <LPC17xx.h>
#include <ucos_ii.h>
/*
#define TASK_STK_SIZE 512

OS_STK TaskStartStk[TASK_STK_SIZE];

void TaskStart(void *data);

int main(void)
{
	OSInit();            // 初始化 uC/OS-II

	// 创建用户任务 TaskStart
	OSTaskCreate(TaskStart, (void *)0, &TaskStartStk[TASK_STK_SIZE - 1], 0);

	OSStart();           // 启动 uC/OS-II

	return 0;
}

void  TaskStart(void *data)
{
	unsigned int cpu_sr = 0;               // 定义 cpu_sr
	data=data;
	
	OS_CPU_SysTickInit(SystemFrequency/100);

	for(;;)
	{
		OSCtxSwCtr = 0;
		OSTimeDlyHMSM(0,0,0,10);
//		OS_ENTER_CRITICAL();           // 进入临界段(关中断)
//    不可以被中断的用户代码		
//		OS_EXIT_CRITICAL();            // 退出临界段(开中断)
		cpu_sr++;
		if(cpu_sr>=100)
		{
			cpu_sr = 0;
		}
	}
}
*/




