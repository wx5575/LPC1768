/**
*@file			
*@brief			�����ʱ����
*@version		
*@author		
*@date		
*/
#include  "Delay.h"
#include  "BSP_GPIO.h"



/**
*@Description: ��ʱ����
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
*@Description: ��ʱ us
*@paramreq-  delayUs : Ҫ��ʱ��΢����
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

