/**
*@file		
*@brief			字符串处理函数
*@version		
*@author		
*@date		
*/
#include "stringhandle.h"
#include <math.h>
#include <string.h>




/**
 *@Description: long int 型数据转换为字符串 因为电流电压等显示6位，所以判断到99999
 *@paramreq- str保存的数组指针  data 转换的数据
 *@paramind- 
 *@return 转换后的字符串指针
 */
void IntToStr(unsigned char* str, long int data)
{
	long int temp = data;
	unsigned char site = 0;                          // 数组下标
	unsigned char i = 0; // 用作 count 、num 的下标
	unsigned char k = 0;
	unsigned char num[5] = {0};    // 记录非零数据
  unsigned char count[5] = {0};  // 记录非零数据的位置
	
	
	if(temp<=99999&&temp>9999)                      // 只转换5位
	{
		i = temp/10000 + 0x30;                        // 0-9 转为字符
		num[site] = i;
		temp = temp%10000;
		count[site] = 5;        //  : 记录数据的位数
		site++;
	}
	if(temp>999 && temp<=9999)
	{
		num[site] = temp/1000 + 0x30;
		temp = temp%1000;
		count[site] = 4;
		site++;
	}
	if(temp>99 && temp<=999)
	{
		num[site] = temp/100 + 0x30;
		temp = temp%100;
		count[site] = 3;
		site++;
	}
	if(temp>9 && temp<=99)
	{
		num[site] = temp/10 + 0x30;
		temp = temp%10;
		count[site] = 2;
		site++;
		num[site] = temp + 0x30;
		count[site] = 1;
		site++;
	}
	else
	{
		num[site] = temp + 0x30;
		count[site] = 1;
		site++;
	}
	
	for(i=0,k=0;k<site;i++,k++)
	{
		str[i] = num[k]; // 
		for(temp = 1; temp<(count[i]-count[i+1]); temp++) // 0 的个数大于 1
		{
			i = i+1;
			str[i] = 0x30;
		}
	}
	str[site+1] = '\0'; // 结束
}



/**
*@Description: float 型数据转换为字符串
*@paramreq-   result: 保存的指针  num:数值   pre: 精度
*@paramind- 
*@return wu
*/
void FloatToStr(unsigned char* result,float num, unsigned char pre)
{
	float data    = 0.0;
	unsigned char  buffer[12] = {'\0'};
	unsigned char  place = 0;
	long int       num_temp = num;
	
	if(num < 0)                               // 小于 0 转换为整数 加 - 号
	{
		data -= num;
		*(result+place) = '-';                     // 为负数
		place++;
	}
	else
	{
		data = num;
	}	
	IntToStr(buffer,num_temp);                // 转换
	data = data - num_temp;                    // 小数
	while(buffer[place] != '\0')
	{
		*(result+place) = buffer[place]; //
		buffer[place] = '\0';
		place++;
	}
	
	/** 根据精度确定小数点后小数位数*/
	if(pre>0)                                  // 小数位
	{
		if(pre>4)
		{
			pre = 4;                                // 只转换 4 位小数位
		}
//		result[place] = '.';                       // 添加小数点
		*(result+place) = '.';
		place++;
		num_temp = data*pow(10,pre);            // 转换为整数
		IntToStr(buffer,num_temp);                // 转换
		num_temp = 0;
		while(buffer[num_temp] != '\0')
			{
				*(result+place) = buffer[num_temp];   // 保存
				num_temp++;
				place++;
      }
	}
	
	*(result+place) = '\0';
}




/**
*@Description: 字符串转换为float数值
*@paramreq-   str: 字符串指针   number:转换后的数值   pre: 小数点后面位数
*@paramind- 
*@return  转换结果 成功为1 失败为0 
*/
uint8 StrToFloat(uint8* str, float* number , uint8 pre)
{
	float result = 0;  // 临时保存数据
	int j = 0;      // 
	int i = 0;    // 计数
	int k = 0; // 小数点出现的次数

	while(str[i] != '\0')
	{
		if(str[i] == '.')
		{
			j = i; // 记录小数点的位置
			k++; // 记录小数点的次数
			i++;
			continue;
		}
		else if(str[i] >= 0x30 && str[i] <= 0x39) 
		{
//			str[i] = str[i] - 0x30;
			i++;
		}
		else
		{
			return 0;// 非法字符
		}
		
		if(k>1) // 小数点太多
		{
			return 0;
		}
	}
	
	/**  读数 */
	if((i==0)||(k==1 && i == j+1)||(k>1))
	{
		return 0;
	}
	else if(k == 0) // 没有点
	{
		for(k = i-1;k>=0;k--)
		{
			result = result + (str[k] - 0x30)*pow(10,i-k-1);
		}
	}
	else
	{
		for(k = j-1;k>=0;k--)
		{
			result = result + (str[k] - 0x30)*pow(10,j-k-1); // 整数部分
		}
		for(k=j+1;k<i;k++)
		{
			result = result + (str[k] - 0x30)/pow(10,k-j); // 小数部分
		}
	}
	*number = result;
	
	return 1;
}




/**
*@Description: 字符串转换为uint8数值 在输入时最好把size的值求准确 , 在函数中没法求 str 数组的大小
*@paramreq-   str: 字符串指针   number:转换后的数值  pre: 要转换的个数 size : str的大小
*@paramind- 
*@return  转换结果 成功为1 失败为0 
*/
uint8 StrToUint(uint8* str, unsigned int* number, uint8 pre, uint8 size)
{
	uint8 result = 0;
	int i = 0;
//	uint8 k = 0;
	
	if(size==0 || pre>=size || pre==0)
	{
		return 0;
	}
	
	if(str[0]<0x30 || str[0]>0x39)
	{
		return 0;
	}
	
	for(i=0;i<pre;i++)
	{
		if((str[i] >= 0x30)&&(str[i] <= 0x39))  // 字符为 0 - 9
		{
			result += (str[i]-0x30)*pow(10,pre-i-1);
		}
	}
	*number = result;
	
	return 1;
}












/**
 *@Description:   求10的n次幂
 *@paramreq-     
 *@paramind- 
 *@return 
 */
uint32 LIB_Get10nData(portuBASE_TYPE n)
{
    uint32 result = 1;
	
    for (; n > 0; n--)
    {
        result *= 10;    
    }
    return result;
}



/**
 *@Description:    将uint32型数字转换为字符串
 *@paramreq- rscData:要转换的数值    splitLen:转换的字符个数   pdestSaveAddr:转换后保存的数组指针
 *@paramind- 
 *@return 
 */
void LIB_DataSplit(uint32 rscData, portuBASE_TYPE splitLen, uint8 *pdestSaveAddr)
{
    uint32 TmpFor10n     = 0;
    splitLen--;

    for (; splitLen > 0; splitLen--)
    {
        TmpFor10n        = LIB_Get10nData(splitLen);		//取10的n次幂
        *pdestSaveAddr   = rscData/TmpFor10n;
        rscData         %= TmpFor10n; 
        pdestSaveAddr++;   
    }

    *pdestSaveAddr = rscData;
}





/**
 *@Description:   将数组中元素转为ASIIC码
 *@paramreq- prscSaveAddr:数组指针     convertLen:转换的位数
 *@paramind- 
 *@return 
 */
void LIB_ConvertNmubToChar(uint8 *prscSaveAddr, uint8 convertLen)
{
    uint8 i = 0;
	
    for (; i < convertLen; i++)
    {
			*(prscSaveAddr + i)  += '0';    
    }        
}




portuBASE_TYPE LIB_StrInsert(int8 *rscStr, uint8 insertPosIndex, uint8 insertChar)
{
    portuBASE_TYPE   i     = 0;
    portuBASE_TYPE   len   = strlen((const char *)rscStr);              //转换为索引号
    portuBASE_TYPE   j     = (portuBASE_TYPE)insertPosIndex - 1;

   if (insertPosIndex > len)
	 {
		 return FALSE;
	 }
	 for (i = len; i != j; i--)
   {
		 rscStr[i + 1] = rscStr[i];
   }
   rscStr[insertPosIndex] = insertChar;
	
	return TRUE;
}




/*************     END     *************/	

