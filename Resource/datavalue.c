/**
*@file		
*@brief			����һЩ���ݴ���ĺ��� ��Ӳ���޹�
*@version		
*@author		
*@date		
*/
#include "datavalue.h"
#include "math.h"







uint8 ReadSystemPara(uint8* str, uint8 opt, uint8 num)
{
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





/**
 *@Description: ����SYSTEM����ֵ
 *@paramreq-  str:Ҫ���õĲ�������   opt:�ӵڼ�����ʼ����   num:���õĸ���
 *@paramind- 
 *@return 
 */
uint8 WriteSystemPara(uint8* str, uint8 opt, uint8 num)
{
	uint8 i;  // ����
	uint8 k = 0;
	
	i = GET_ARRAY_COUNT(systemvalue);
	if(num > i || opt>i)
	{
		return 0;
	}
	for(i = opt;i < num+opt; i++)
	{
		systemvalue[i] = str[k];
		k++;
	}
	
	return i;
}





/**
 *@Description: ����FILE ����    FILEֵ��һ��uint8 ����  ,��Ҫ����ֵתΪ�ַ���  С��0 ʱǰ����� '0'
 *@paramreq-  file_value:Ҫ���õĲ�������   
 *@paramind- 
 *@return 
 */
void Write_FILEvalue(uint8 file_value)
{	
	FILE[0] = file_value / 10 + 0x30;     // ʮλ
	FILE[1] = file_value % 10 + 0x30;
	FILE[2] = '\0';
}

uint8* Read_FILEvalue(void)
{
	return(FILE);
}



void Write_STEPvalue(uint8 step_value)
{
	STEP[0] = step_value / 10 + 0x30;
	STEP[1] = step_value % 10 + 0x30;
	STEP[2] = '\0';
}



uint8* Read_STEPvalue(void)
{
	return(STEP);
}





/**
 *@Description: ����STEP_RESֵ  �ȱȽ�����ֵ�Ƿ�Ϸ�  Ȼ��ֵ
 *@paramreq-  res:Ҫ���õĲ�������     num:���õĸ���
 *@paramind- 
 *@return 1:���óɹ�    0:����ʧ��,STEP_RES����
 */
uint8 Write_RESvalue(uint8* res, uint8 num)
{
	int8 i = 0;
	
	if(num == 6)                   // ��6λ ����
	{
		if(res[0] == 6)              // ��λΪ6 ����λֻ��Ϊ0
		{
			for(i = 1; i < num; i++)
			{
				if((res[i] > 0)&&(res[i] != '.'))
				{
					return 0;
				}
			}
		}
		else if(res[0] > 6)         // ����600 ����ֵ��Ч
		{
			return 0;
		}
	}
	else if(num > 6)
	{
		return 0;
	}
	else
	{
	}
	
	for(i = 0; i < num-1; i++)
	{
		if((res[i] > 9)&&(res[i] != '.'))
		{
			return 0;                      // λֵΪ 0-9
	  }
  }
	
	for(i = num-1; i > 0; i--)
	{
		STEP_RES[i] = res[i];
	}
	STEP_RES[num] = '\0';
	
	return 1;
}




uint8* Read_RESvalue(void)
{
	return(STEP_RES);
}






float SetValue(float temp, uint8 opt, uint8 type)
{
	if(type != 0)
	{
		s_setvalue[opt] = temp;
		return 1;
	}
	else
	{
		return(s_setvalue[opt]);
	}
}









/**
 *@Description: �� float ֵ���浽����   
 *@paramreq-  teststr: Ҫ��д������ָ��     opt:λ��   type:��д���� 0:��  1:д
 *@paramind- 
 *@return   
 */
uint8 TestValue(float* teststr, uint8 opt, uint8 type)
{
	uint8 i;
	i = GET_ARRAY_COUNT(s_setvalue);
	if(i <= opt)
	{
		return 0;
	}
	if(type != 0)
	{
		s_setvalue[opt] = *teststr;
	}
	else
	{
		return (s_setvalue[opt]);
	}
	return 1;
}








/**
 *@Description: �� float ֵ���浽����   
 *@paramreq-  str: ���������ָ��   data: float ��ָ��
 *@paramind- 
 *@return   
 */
uint8 FloatToUint8(uint8* str, float* data)
{
	uint8 i = 0;
	uint8* value = (uint8*)data;
	for(i=0;i<4;i++)
	{
		*(str+i) = *(value+i);
	}
	return 1;
}



float Uint8ToFloat(unsigned char* str)
{
	int i = 0;
	float* temp;
//	uint8* pot;
//	pot = temp;
	for(i=0;i<4;i++)
	{
//		temp = *(str+i) ;
	}
	return (*temp);
}






/**
 *@Description: �� float ת��Ϊ BCD �룬��ʮ�����Ʊ���    
 *@paramreq-  str: ���������ָ��   data: ת��������
 *@paramind- 
 *@return   
 */
void FloatToBCDCode(unsigned char* str,float data)
{
	int i = 0;
	unsigned int  temp = 0;

	temp = data; // �� float ת��Ϊ int
	data = data - temp; // ����С������
	for(i=1;i>=0;i--)
	{
		*(str+i) = temp & 0xFF;   // ��ȡ 8 λֵ����Ϊ BCD��
		temp = temp>>8;  // ȥ���ѱ���� 8 λֵ
	}
	temp = data*1000;
	for(i=3;i>=2;i--)
	{
		*(str+i) = temp & 0xFF;
		temp = temp>>8;
	}
}



/**
 *@Description: �� BCD�� ת��Ϊ float   ����ΪС�������λ
 *@paramreq-  str: ���������ָ��  
 *@paramind- 
 *@return   ����ת���������
 */
float BCDCodeToFloat(unsigned char* str)
{
	float temp = 0;
	float data = 0;
	unsigned int  t = 0;

	t = t | str[0]; // ��ֵ
	t = t<<8; // ����λ��λ
	t = t | str[1]; // ��λ��ֵ , ��Ϊ temp & 0xFF
	temp = t;
	t = 0;

	t = t | str[2];
	t = t<<8;
	t = t | str[3];
	
	data = t;
	if(t>=10)
	{
		data =data/1000;  // С�������λ , ��Ϊ 8 λ���������ɳ��� 100
	}
	else
	{
		data =data/10; // С�����ֻ��һλ 
	}
	temp = temp+data;
	return temp;
}

