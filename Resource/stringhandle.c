/**
*@file		
*@brief			�ַ���������
*@version		
*@author		
*@date		
*/
#include "stringhandle.h"
#include <math.h>
#include <string.h>




/**
 *@Description: long int ������ת��Ϊ�ַ��� ��Ϊ������ѹ����ʾ6λ�������жϵ�99999
 *@paramreq- str���������ָ��  data ת��������
 *@paramind- 
 *@return ת������ַ���ָ��
 */
void IntToStr(unsigned char* str, long int data)
{
	long int temp = data;
	unsigned char site = 0;                          // �����±�
	unsigned char i = 0; // ���� count ��num ���±�
	unsigned char k = 0;
	unsigned char num[5] = {0};    // ��¼��������
  unsigned char count[5] = {0};  // ��¼�������ݵ�λ��
	
	
	if(temp<=99999&&temp>9999)                      // ֻת��5λ
	{
		i = temp/10000 + 0x30;                        // 0-9 תΪ�ַ�
		num[site] = i;
		temp = temp%10000;
		count[site] = 5;        //  : ��¼���ݵ�λ��
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
		for(temp = 1; temp<(count[i]-count[i+1]); temp++) // 0 �ĸ������� 1
		{
			i = i+1;
			str[i] = 0x30;
		}
	}
	str[site+1] = '\0'; // ����
}



/**
*@Description: float ������ת��Ϊ�ַ���
*@paramreq-   result: �����ָ��  num:��ֵ   pre: ����
*@paramind- 
*@return wu
*/
void FloatToStr(unsigned char* result,float num, unsigned char pre)
{
	float data    = 0.0;
	unsigned char  buffer[12] = {'\0'};
	unsigned char  place = 0;
	long int       num_temp = num;
	
	if(num < 0)                               // С�� 0 ת��Ϊ���� �� - ��
	{
		data -= num;
		*(result+place) = '-';                     // Ϊ����
		place++;
	}
	else
	{
		data = num;
	}	
	IntToStr(buffer,num_temp);                // ת��
	data = data - num_temp;                    // С��
	while(buffer[place] != '\0')
	{
		*(result+place) = buffer[place]; //
		buffer[place] = '\0';
		place++;
	}
	
	/** ���ݾ���ȷ��С�����С��λ��*/
	if(pre>0)                                  // С��λ
	{
		if(pre>4)
		{
			pre = 4;                                // ֻת�� 4 λС��λ
		}
//		result[place] = '.';                       // ���С����
		*(result+place) = '.';
		place++;
		num_temp = data*pow(10,pre);            // ת��Ϊ����
		IntToStr(buffer,num_temp);                // ת��
		num_temp = 0;
		while(buffer[num_temp] != '\0')
			{
				*(result+place) = buffer[num_temp];   // ����
				num_temp++;
				place++;
      }
	}
	
	*(result+place) = '\0';
}




/**
*@Description: �ַ���ת��Ϊfloat��ֵ
*@paramreq-   str: �ַ���ָ��   number:ת�������ֵ   pre: С�������λ��
*@paramind- 
*@return  ת����� �ɹ�Ϊ1 ʧ��Ϊ0 
*/
uint8 StrToFloat(uint8* str, float* number , uint8 pre)
{
	float result = 0;  // ��ʱ��������
	int j = 0;      // 
	int i = 0;    // ����
	int k = 0; // С������ֵĴ���

	while(str[i] != '\0')
	{
		if(str[i] == '.')
		{
			j = i; // ��¼С�����λ��
			k++; // ��¼С����Ĵ���
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
			return 0;// �Ƿ��ַ�
		}
		
		if(k>1) // С����̫��
		{
			return 0;
		}
	}
	
	/**  ���� */
	if((i==0)||(k==1 && i == j+1)||(k>1))
	{
		return 0;
	}
	else if(k == 0) // û�е�
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
			result = result + (str[k] - 0x30)*pow(10,j-k-1); // ��������
		}
		for(k=j+1;k<i;k++)
		{
			result = result + (str[k] - 0x30)/pow(10,k-j); // С������
		}
	}
	*number = result;
	
	return 1;
}




/**
*@Description: �ַ���ת��Ϊuint8��ֵ ������ʱ��ð�size��ֵ��׼ȷ , �ں�����û���� str ����Ĵ�С
*@paramreq-   str: �ַ���ָ��   number:ת�������ֵ  pre: Ҫת���ĸ��� size : str�Ĵ�С
*@paramind- 
*@return  ת����� �ɹ�Ϊ1 ʧ��Ϊ0 
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
		if((str[i] >= 0x30)&&(str[i] <= 0x39))  // �ַ�Ϊ 0 - 9
		{
			result += (str[i]-0x30)*pow(10,pre-i-1);
		}
	}
	*number = result;
	
	return 1;
}












/**
 *@Description:   ��10��n����
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
 *@Description:    ��uint32������ת��Ϊ�ַ���
 *@paramreq- rscData:Ҫת������ֵ    splitLen:ת�����ַ�����   pdestSaveAddr:ת���󱣴������ָ��
 *@paramind- 
 *@return 
 */
void LIB_DataSplit(uint32 rscData, portuBASE_TYPE splitLen, uint8 *pdestSaveAddr)
{
    uint32 TmpFor10n     = 0;
    splitLen--;

    for (; splitLen > 0; splitLen--)
    {
        TmpFor10n        = LIB_Get10nData(splitLen);		//ȡ10��n����
        *pdestSaveAddr   = rscData/TmpFor10n;
        rscData         %= TmpFor10n; 
        pdestSaveAddr++;   
    }

    *pdestSaveAddr = rscData;
}





/**
 *@Description:   ��������Ԫ��תΪASIIC��
 *@paramreq- prscSaveAddr:����ָ��     convertLen:ת����λ��
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
    portuBASE_TYPE   len   = strlen((const char *)rscStr);              //ת��Ϊ������
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

