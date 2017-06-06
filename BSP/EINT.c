#include "EINT.h"
#include <LPC17xx.h>




/******************************************************************************
 *                            本文件内部宏定义
******************************************************************************/

#define             EINT0                       (0)
#define             EINT1                       (1)
#define             EINT2                       (2)
#define             EINT3                       (3)



/**
*@Description: 中断0初始化
*@paramreq-   PINSEL : 引脚功能选择          EXTMODE:外部中断模式寄存器(0:电平触发  1:边沿触发)          
*@paramind-   EXTPOLAR : 外部中断极性寄存器 (0:低电平或下降沿)
*@return 无
*/
void EINT0_Init(void)
{
    // 选择外部中断0功能
    LPC_PINCON->PINSEL4     = (LPC_PINCON->PINSEL4 & 0xFFCFFFFF)|(0x00100000);
    // 外部中断0边沿触发 下降沿触发
    LPC_SC->EXTMODE        |= (1 << EINT0);
    LPC_SC->EXTPOLAR       &= (~(1 << EINT0));
    NVIC_SetPriority(EINT0_IRQn, (1<<__NVIC_PRIO_BITS) - 2);        // 中断向量
    NVIC_EnableIRQ(EINT0_IRQn);                                     // 使能
}


// 和上面一样
void EINT1_Init(void)
{
    // 选择外部中断1功能
    LPC_PINCON->PINSEL4     = (LPC_PINCON->PINSEL4 & 0xFF3FFFFF)|(0x00400000);
    // 外部中断1边沿触发 下降沿触发
    LPC_SC->EXTMODE        |= (1 << EINT1);
    LPC_SC->EXTPOLAR       &= (~(1 << EINT1));
    NVIC_SetPriority(EINT1_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
    NVIC_EnableIRQ(EINT1_IRQn);   
}



void EINT3_Init(void)
{
    // 选择外部中断1功能
    LPC_PINCON->PINSEL4     = (LPC_PINCON->PINSEL4 & 0xF3FFFFFF)|(0x04000000);
    // 外部中断3边沿触发 下降沿触发
    LPC_SC->EXTMODE        |= (1 << EINT3);
    LPC_SC->EXTPOLAR       &= (~(1 << EINT3));
    NVIC_SetPriority(EINT3_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
    NVIC_EnableIRQ(EINT3_IRQn);   
}



/**
*@Description: 外部中断0中断函数
*@paramreq-   
*@paramind-     
*@return 无
*/
void EINT0_IRQHandler(void)
{
    // 清除中断标志
    LPC_SC->EXTINT        |= (1 << EINT0);
}



/**
*@Description: 外部中断1中断函数
*@paramreq-   
*@paramind-     
*@return 无
*/
void EINT1_IRQHandler(void)
{
    // 清除中断标志
    LPC_SC->EXTINT       |= (1 << EINT1);
}


/**
*@Description: 外部中断2中断函数
*@paramreq-   
*@paramind-     
*@return 无
*/
void EINT3_IRQHandler(void)
{
	LPC_SC->EXTINT        |= (1 << EINT3);
}





