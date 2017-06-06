/**
*@file		
*@brief			包含一些数据处理的函数 与硬件无关
*@version		
*@author		
*@date		
*/
#include "datavalue.h"
#include "math.h"







uint8 ReadSystemPara(uint8* str, uint8 opt, uint8 num)
{
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





/**
 *@Description: 设置SYSTEM参数值
 *@paramreq-  str:要设置的参数数组   opt:从第几个开始设置   num:设置的个数
 *@paramind- 
 *@return 
 */
uint8 WriteSystemPara(uint8* str, uint8 opt, uint8 num)
{
	uint8 i;  // 计数
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
 *@Description: 设置FILE 数组    FILE值以一个uint8 保存  ,需要对数值转为字符串  小于0 时前面添加 '0'
 *@paramreq-  file_value:要设置的参数数组   
 *@paramind- 
 *@return 
 */
void Write_FILEvalue(uint8 file_value)
{	
	FILE[0] = file_value / 10 + 0x30;     // 十位
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
 *@Description: 设置STEP_RES值  先比较数组值是否合法  然后赋值
 *@paramreq-  res:要设置的参数数组     num:设置的个数
 *@paramind- 
 *@return 1:设置成功    0:设置失败,STEP_RES不变
 */
uint8 Write_RESvalue(uint8* res, uint8 num)
{
	int8 i = 0;
	
	if(num == 6)                   // 有6位 数据
	{
		if(res[0] == 6)              // 百位为6 其他位只能为0
		{
			for(i = 1; i < num; i++)
			{
				if((res[i] > 0)&&(res[i] != '.'))
				{
					return 0;
				}
			}
		}
		else if(res[0] > 6)         // 超过600 则数值无效
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
			return 0;                      // 位值为 0-9
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
 *@Description: 将 float 值保存到数组   
 *@paramreq-  teststr: 要读写的数据指针     opt:位置   type:读写类型 0:读  1:写
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
 *@Description: 将 float 值保存到数组   
 *@paramreq-  str: 保存的数组指针   data: float 型指针
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
 *@Description: 将 float 转换为 BCD 码，用十六进制保存    
 *@paramreq-  str: 保存的数组指针   data: 转换的数据
 *@paramind- 
 *@return   
 */
void FloatToBCDCode(unsigned char* str,float data)
{
	int i = 0;
	unsigned int  temp = 0;

	temp = data; // 将 float 转换为 int
	data = data - temp; // 保存小数部分
	for(i=1;i>=0;i--)
	{
		*(str+i) = temp & 0xFF;   // 获取 8 位值保存为 BCD码
		temp = temp>>8;  // 去掉已保存的 8 位值
	}
	temp = data*1000;
	for(i=3;i>=2;i--)
	{
		*(str+i) = temp & 0xFF;
		temp = temp>>8;
	}
}



/**
 *@Description: 将 BCD码 转换为 float   精度为小数点后三位
 *@paramreq-  str: 保存的数组指针  
 *@paramind- 
 *@return   返回转换后的数据
 */
float BCDCodeToFloat(unsigned char* str)
{
	float temp = 0;
	float data = 0;
	unsigned int  t = 0;

	t = t | str[0]; // 赋值
	t = t<<8; // 将高位移位
	t = t | str[1]; // 低位赋值 , 因为 temp & 0xFF
	temp = t;
	t = 0;

	t = t | str[2];
	t = t<<8;
	t = t | str[3];
	
	data = t;
	if(t>=10)
	{
		data =data/1000;  // 小数点后三位 , 因为 8 位二进制最大可超过 100
	}
	else
	{
		data =data/10; // 小数点后只有一位 
	}
	temp = temp+data;
	return temp;
}

