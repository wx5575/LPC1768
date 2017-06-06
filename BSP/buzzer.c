/**
*@file			
*@brief			��������й� �����������Ϳ���
*@version		
*@author		
*@date		
*/
#include    "Buzzer.h"
#include    "bsp_gpio.h"

#define             BUZZER_PORT                    (PORT0)
#define             BUZZER_BIT                     (25)


/**
*@Description: ���������ų�ʼ��
*@paramreq-   
*@paramind-     
*@return ��
*/
void BuzzerInit(void)
{
	BSP_GPIODirectionSet(BUZZER_PORT, BUZZER_BIT, DIR_OUTPUT, PULL_UP_RES);    // 
}



/**
*@Description: ��
*@paramreq-   
*@paramind-     
*@return ��
*/
void Buzzer_Off(void)
{
    BSP_GPIOValueSet(BUZZER_PORT, BUZZER_BIT, 0);
}


/**
*@Description: ��
*@paramreq-   
*@paramind-     
*@return ��
*/
void Buzzer_On(void)
{
    BSP_GPIOValueSet(BUZZER_PORT, BUZZER_BIT, 1);
}
