/**
*@file	���� at24cxx.h	
*@brief			at24c512-2.7 ����������
*@version		
*@author		
*@date		
*/
#include "at24cxx.h"
#include "LPC17xx.h"
#include "delay.h"





uint8 Readaxx(void)
{
	return axx;
}




void writeaxx(uint8 value)
{
	axx = value;
}



















































/*********** ����ľ�ͷ **********/



























/**
 *@Description: AT24Cxx   
 *@paramreq-  
 *@paramind- 
 *@return   
 */
 /*
void AT24Cxx_Init(void)
{
	WP_PORT_INIT();
	SCL_PORT_INIT();
}
*/


/**
 *@Description:  AT24Cxx  ָ����ַ����һ������ 
 *@paramreq-  
 *@paramind- 
 *@return   ����������
 */
 /*
uint8 AT24Cxx_ReadOneByte(uint16 ReadAddr)
{
	uint8 temp = 0;
	I2C_Start();
	I2C_SendByte(0xA0);
	I2C_WaitAck();
	I2C_SendByte(ReadAddr>>8);
	I2C_WaitAck();
	I2C_SendByte(ReadAddr%256);
	I2C_WaitAck();
	I2C_Start();
	I2C_SendByte(0xA1);
	I2C_WaitAck();
	temp = I2C_ReadByte();
	I2C_Stop();
	return temp;
}



void AT24Cxx_WriteOneByte(uint16 addr, uint8 data)
{
	I2C_Start();
	I2C_SendByte(0xA0);
	I2C_WaitAck();
	I2C_SendByte(addr>>8);
	I2C_WaitAck();
	I2C_SendByte(addr%256);
	I2C_WaitAck();
	I2C_SendByte(data);
	I2C_WaitAck();
	I2C_Stop();
	SoftDelayMs(10);
}
*/



/**
 *@Description: ��� AT24Cxx  �Ƿ����� 
 *@paramreq-  
 *@paramind- 
 *@return   1: ����    0: ʧ��
 */ 
/*
uint8 AT24Cxx_Check(void)
{
	uint8 temp = 0;
	temp = AT24Cxx_ReadOneByte(255);
	if(temp == 0x55)
	{
		return 1;
	}
	else
	{
		AT24Cxx_WriteOneByte(255,0x55);
		temp = AT24Cxx_ReadOneByte(255);
		if(temp == 0x55)
		{
			return 1;
		}
	}
	return 0;
}
*/















/*************     END     *************/	
