/**
*@file		
*@brief			
*@version		
*@author		
*@date		
*/
#include "system_parameter.h"








/** ����system��ѡ���±��ֵ */
//static uint8 systemvalue[9]={0,0,0,0,0,0,0,0,0};    
//const uint8 sys_default[9] = {99, 1, 1, 1, 0, 2, 0, 0, 1}; // Ĭ��ֵ




/**
 *@Description: ��ȡSYSTEM����ֵ���浽str����    
 *@paramreq-  str:���������浽����        opt:�ӵڼ�����ʼ��   num:��ȡ�ĸ���
 *@paramind- 
 *@return ���ض�ȡ�ĸ���
 */
/*
uint8 ReadSystemPara(uint8* str, uint8 opt, uint8 num)
{
//	uint8 total = 0; // systemѡ�����
	uint8 i;  // ����
	
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
 *@Description: ����SYSTEM����ֵ
 *@paramreq-  str:Ҫ���õĲ�������   opt:�ӵڼ�����ʼ����   num:���õĸ���
 *@paramind- 
 *@return 
 */
 /*
uint8 WriteSystemPara(uint8* str, uint8 opt, uint8 num)
{
	uint8 i;  // ����
	
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









