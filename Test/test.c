#include "test.h"
#include "readandwrite.h"
#include "at24cxx.h"
#include "bsp_timer.h"
#include "AD.h"
#include <string.h>
#include "stringhandle.h"




static uint32 temperature = 0;  // ��¼�¶�




#define   AD_ERR                        (0)
#define   AD_SECCESS                    (1)
#define   DA_ERR                        (2)
#define   DA_SECCESS                    (3)
#define   AT24Cxx_READ_ERR              (4)
#define   AT24Cxx_READ_SECCESS          (5)
#define   AT24Cxx_WRITE_ERR             (6)
#define   AT24Cxx_WRITE_SECCESS         (7)










/**
*@Description: ��ȡ��ѹ
*@paramreq-     
*@paramind- 
*@return ��
*/
float VoltageGet(void)
{
	float v = 0.0;
	return v;
}



/**
*@Description: ��ȡ����
*@paramreq-     
*@paramind- 
*@return ��
*/
float EletricityGet(void)
{
	float e = 0.0;
	return e;
}



/**
*@Description: ��ȡ����
*@paramreq-     
*@paramind- 
*@return ��
*/
float ResistanceGet(void)
{
	float res = 0.0;
	return res;
}



/**
*@Description: ��ȡ���Բ�
*@paramreq-     
*@paramind- 
*@return ��
*/
unsigned char StepGet(void)
{
	unsigned char s = 99;
	return s;
}




/**
*@Description: ��ȡ�����ļ�
*@paramreq-     
*@paramind- 
*@return ��
*/
unsigned char FileGet(void)
{
	unsigned char f = 10;
	return f;
}





/**
 *@Description: �����Լ���� ��� AT24Cxx ��д , AD��DA ���AD/DA ������ ��������  ���ֻ��AT24Cxx
 *              ��������������set���� ֻ����ʱ����
 *@paramreq- 
 *@paramind- 
 *@return ���쳣���سɹ�
 */
uint8 ExamineSelf(void)
{
	return SUCCESS;
}





/**
*@Description: ���Ʒ���ת�ٳ���
*@paramreq-     samp:�����Ƿ���� 0:��  load:����״̬��־
*@paramind- 
*@return 
*/
uint8 FAN_Ctrol(uint32 samp, uint8 load)
{	
	if (temperature < 2407)     // ����35�� ���Ȳ�ת
	{
		if (load != 0)      // ���ؿ�
    {
			BSP_PWMDutySet(40);  // ֻҪ���ؿ� ���Ⱦ�Ҫת
    }
		else
		{
			BSP_PWMDutySet(0);
    }
  }
	else if (temperature > 3772)   // ���±���
  {
		BSP_PWMDutySet(90);          // ����ת�ټӴ�
  }
	else                           // �����¶ȵ��ڷ���ת��
  {
		BSP_PWMDutySet((456*temperature)/10000 - 70);
  }
	
	if(samp != 0)                  // ���������Ƿ����
	{
		LPC_ADC->CR  |= (1 << 24);   // ���������ڲ�ADת��
	}
	return 1;
}



/**
*@Description: �����¶�ֵ   ��A/D�ж����潫����ֵ�����¶�
*@paramreq-     value: �¶�   (A/D����������)
*@paramind- 
*@return 
*/
void TemperatureSet(uint32 value)
{
	temperature = (value & 0x00FFFFFF);
}







/**
 *@Description:  ��ȡ���Ե�ѹֵ   ��ʾ  �Ե�ѹֵ���д���   
 *@paramreq-    adVal:AD��������     volt:�����ѹֵ        curr:�������ֵ
 *@paramind- 
 *@return 
 */
uint32 GetADValue(uint32 *adVal, uint8 *volt, uint8 *curr)
{
	int32 voltSampleVal = 0;
	int32 currSampleVal = 0;
	uint8 i = 0;
	uint8 dotPos = 1;            // С����λ��
	
	
	for (i = 0; i < AD_SAMPLE_NUMBS_MAX_VAL; i++)   
  {
		if ((1 << 23) & adVal[2*i])
    {
			// ����λΪ1 ����ֵ �����ԵĽ���
      // ȥ������λ ȥ����4λ(��8λ����һֱ����) ����ʵ���Ͼ�ֻʣ19λ��
      voltSampleVal  -= (~((adVal[2*i] & 0x007FFFF0) >> 4)) & 0x0007FFFF;
    }
		else
		{
			// ����λΪ0 ����ֵ �����ԵĽ���(���Խӷ�)
      voltSampleVal  += (((adVal[2*i] & 0x007FFFF0) >> 4)) & 0x0007FFFF;
    }
		if ((1 << 23) & adVal[2*i+1])
    {
			// ����λΪ1 ����ֵ �����ԵĽ���
      // ȥ������λ ȥ����4λ(��8λ����һֱ����) ����ʵ���Ͼ�ֻʣ19λ��
      currSampleVal  -= (~((adVal[2*i+1] & 0x007FFFF0) >> 4)) & 0x0007FFFF;
    }
		else
    {
			// ����λΪ0 ����ֵ �����ԵĽ���(���Խӷ�)
      currSampleVal  += (((adVal[2*i+1] & 0x007FFFF0) >> 4)) & 0x0007FFFF;
    }
  }
	
	// ��ֵС�����ʾ�ⲿ��Դ�����뼫��Ϊ��
	if (voltSampleVal < 0)
  {
		voltSampleVal = ~voltSampleVal + 1;
  }
	else
  {
		voltSampleVal = 0 - voltSampleVal;
  }
	voltSampleVal  /= AD_SAMPLE_NUMBS_MAX_VAL;      // ��ƽ����ѹ
	
	if (currSampleVal < 0)
  {
		currSampleVal = ~currSampleVal + 1;
  }
	else
  {
		currSampleVal = 0 - currSampleVal;
  }
	currSampleVal  /= AD_SAMPLE_NUMBS_MAX_VAL;      // ��ƽ������
	
	voltSampleVal   = ((int64)voltSampleVal*0x00022E4A)/100000 + 0xFFFFFF98;   // 
  currSampleVal   = ((int64)currSampleVal*0x00023CE4)/100000 + 0x0000004E;
	
	// ʵ�ʼ�����ĵ�ѹֵС���㣬��������Ϊ���Խӷ�����
  if (voltSampleVal < 0)
	{
		if(voltSampleVal < -4000)
    {
			;
    }
		voltSampleVal  = 0;
  }
	// ����72V����ѹ����
	else if (voltSampleVal > 720000)
  {
		;
  }
	if (voltSampleVal > 999990)
  {
		voltSampleVal   = 999990;
  }
	
	// ����ѹת��Ϊ�ַ���
	if (voltSampleVal < 100000)
  {
		*(volt+6) = '\0';	
//    voltSampleVal  /= 10;       // ��ȷ��0.001V  ���Ҫ��ȷ��0.0001���ó�10		
		LIB_DataSplit(voltSampleVal, 5, (void *)volt);
    LIB_ConvertNmubToChar((void *)volt, 5);
    LIB_StrInsert((void *)volt, dotPos, '.');
    volt[strlen((void *)volt)] = 'V';
    *(volt+6) = '\0';	
  }
	else
  {
		voltSampleVal /= 10;
  }	
	
	// ������ת��Ϊ�ַ���
	if (currSampleVal < 0)
  {
		currSampleVal = 0;
  }
	if (currSampleVal < 100000)
  {
		dotPos = 1;
    currSampleVal  /= 10;

		// ��ֵ���
		LIB_DataSplit(currSampleVal, 4, (void *)curr);
    LIB_ConvertNmubToChar((void *)curr, 4);
    LIB_StrInsert((void *)curr, dotPos, '.');
    curr[strlen((void *)curr)]  = 'A';
    curr[strlen((void *)curr)]  = '\0';
		*(curr+6) = '\0';	
  }
	else
  {
		dotPos = 2;
    currSampleVal /= 10;

		// ��ֵ���
    LIB_DataSplit(currSampleVal, 5, (void *)curr);
    LIB_ConvertNmubToChar((void *)curr, 5);
    LIB_StrInsert((void *)curr, dotPos, '.');
    curr[strlen((void *)curr)]  = 'A';
		*(curr+6) = '\0';
  }
		
	return 0;
}








/**
 *@Description:  �������ֵ�͵���ֵ �����ѹֵ �����õ�ѹ���ó������õ�ѹ 
 *@paramreq-   curr_value:����ֵ       res_value: ����ֵ
 *@paramind- 
 *@return 
 */























/******************************************************************************
 *                             END  OF  FILE                                   
******************************************************************************/












