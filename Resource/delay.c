/**
*@file			
*@brief			软件延时函数
*@version		
*@author		
*@date		
*/
#include  "Delay.h"
#include  "BSP_GPIO.h"



/**
*@Description: 延时毫秒
*@paramreq- 
*@paramind- 
*@return 
*/
void SoftDelayMs(uint16 delayMs)
{
	  uint16              i;
    uint32              j;

    for (i = 0; i < delayMs; i++)
    {
        for (j = 0; j < 16590; j++)
        {
            portNOP();
        }
    }
}





/**
*@Description: 延时 us
*@paramreq-  delayUs : 要延时的微秒数
*@paramind- 
*@return 
*/
void SoftDelayUs(uint16 delayUs)
{
    uint16  i;

    for (i = 0; i < delayUs; i++)
    {
			portNOP();
    }
}








/*************     END     *************/	

