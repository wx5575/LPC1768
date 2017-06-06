/**
*@file		
*@brief			
*@version		
*@author		
*@date		
*/
#include "system_parameter.h"








/** 保存system的选项下标或值 */
//static uint8 systemvalue[9]={0,0,0,0,0,0,0,0,0};    
//const uint8 sys_default[9] = {99, 1, 1, 1, 0, 2, 0, 0, 1}; // 默认值




/**
 *@Description: 读取SYSTEM参数值保存到str数组    
 *@paramreq-  str:将参数保存到数组        opt:从第几个开始读   num:读取的个数
 *@paramind- 
 *@return 返回读取的个数
 */
/*
uint8 ReadSystemPara(uint8* str, uint8 opt, uint8 num)
{
//	uint8 total = 0; // system选项个数
	uint8 i;  // 计数
	
	i = GET_ARRAY_COUNT(systemvalue);
	
	if(num > i)
	{
		num = i;
	}
	
	for(i = opt;i < num; i++)
	{
		str[i] = systemvalue[i];
	}
	
	return i;
}
*/




/**
 *@Description: 设置SYSTEM参数值
 *@paramreq-  str:要设置的参数数组   opt:从第几个开始设置   num:设置的个数
 *@paramind- 
 *@return 
 */
 /*
uint8 WriteSystemPara(uint8* str, uint8 opt, uint8 num)
{
	uint8 i;  // 计数
	
	i = GET_ARRAY_COUNT(systemvalue);
	if(num > i)
	{
		return 0;
	}
	for(i = opt;i < num; i++)
	{
		systemvalue[i] = str[i];
	}
	
	return i;
}

*/



/**
 *@Description:  
 *@paramreq-   
 *@paramind- 
 *@return  
 */
uint8 Set_PowRecall(uint8* powcal)
{
	return 0;
}









