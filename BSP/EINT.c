#include "EINT.h"
#include <LPC17xx.h>




/******************************************************************************
 *                            ���ļ��ڲ��궨��
******************************************************************************/

#define             EINT0                       (0)
#define             EINT1                       (1)
#define             EINT2                       (2)
#define             EINT3                       (3)



/**
*@Description: �ж�0��ʼ��
*@paramreq-   PINSEL : ���Ź���ѡ��          EXTMODE:�ⲿ�ж�ģʽ�Ĵ���(0:��ƽ����  1:���ش���)          
*@paramind-   EXTPOLAR : �ⲿ�жϼ��ԼĴ��� (0:�͵�ƽ���½���)
*@return ��
*/
void EINT0_Init(void)
{
    // ѡ���ⲿ�ж�0����
    LPC_PINCON->PINSEL4     = (LPC_PINCON->PINSEL4 & 0xFFCFFFFF)|(0x00100000);
    // �ⲿ�ж�0���ش��� �½��ش���
    LPC_SC->EXTMODE        |= (1 << EINT0);
    LPC_SC->EXTPOLAR       &= (~(1 << EINT0));
    NVIC_SetPriority(EINT0_IRQn, (1<<__NVIC_PRIO_BITS) - 2);        // �ж�����
    NVIC_EnableIRQ(EINT0_IRQn);                                     // ʹ��
}


// ������һ��
void EINT1_Init(void)
{
    // ѡ���ⲿ�ж�1����
    LPC_PINCON->PINSEL4     = (LPC_PINCON->PINSEL4 & 0xFF3FFFFF)|(0x00400000);
    // �ⲿ�ж�1���ش��� �½��ش���
    LPC_SC->EXTMODE        |= (1 << EINT1);
    LPC_SC->EXTPOLAR       &= (~(1 << EINT1));
    NVIC_SetPriority(EINT1_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
    NVIC_EnableIRQ(EINT1_IRQn);   
}



void EINT3_Init(void)
{
    // ѡ���ⲿ�ж�1����
    LPC_PINCON->PINSEL4     = (LPC_PINCON->PINSEL4 & 0xF3FFFFFF)|(0x04000000);
    // �ⲿ�ж�3���ش��� �½��ش���
    LPC_SC->EXTMODE        |= (1 << EINT3);
    LPC_SC->EXTPOLAR       &= (~(1 << EINT3));
    NVIC_SetPriority(EINT3_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
    NVIC_EnableIRQ(EINT3_IRQn);   
}



/**
*@Description: �ⲿ�ж�0�жϺ���
*@paramreq-   
*@paramind-     
*@return ��
*/
void EINT0_IRQHandler(void)
{
    // ����жϱ�־
    LPC_SC->EXTINT        |= (1 << EINT0);
}



/**
*@Description: �ⲿ�ж�1�жϺ���
*@paramreq-   
*@paramind-     
*@return ��
*/
void EINT1_IRQHandler(void)
{
    // ����жϱ�־
    LPC_SC->EXTINT       |= (1 << EINT1);
}


/**
*@Description: �ⲿ�ж�2�жϺ���
*@paramreq-   
*@paramind-     
*@return ��
*/
void EINT3_IRQHandler(void)
{
	LPC_SC->EXTINT        |= (1 << EINT3);
}





