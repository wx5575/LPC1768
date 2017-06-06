/**
*@file			
*@brief			与蜂鸣器有关 蜂鸣器驱动和开关
*@version		
*@author		
*@date		
*/
#include    "Buzzer.h"
#include    "bsp_gpio.h"

#define             BUZZER_PORT                    (PORT0)
#define             BUZZER_BIT                     (25)


/**
*@Description: 蜂鸣器引脚初始化
*@paramreq-   
*@paramind-     
*@return 无
*/
void BuzzerInit(void)
{
	BSP_GPIODirectionSet(BUZZER_PORT, BUZZER_BIT, DIR_OUTPUT, PULL_UP_RES);    // 
}



/**
*@Description: 关
*@paramreq-   
*@paramind-     
*@return 无
*/
void Buzzer_Off(void)
{
    BSP_GPIOValueSet(BUZZER_PORT, BUZZER_BIT, 0);
}


/**
*@Description: 开
*@paramreq-   
*@paramind-     
*@return 无
*/
void Buzzer_On(void)
{
    BSP_GPIOValueSet(BUZZER_PORT, BUZZER_BIT, 1);
}
