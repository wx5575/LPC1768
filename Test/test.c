#include "test.h"
#include "readandwrite.h"
#include "at24cxx.h"
#include "bsp_timer.h"
#include "AD.h"
#include <string.h>
#include "stringhandle.h"




static uint32 temperature = 0;  // 记录温度




#define   AD_ERR                        (0)
#define   AD_SECCESS                    (1)
#define   DA_ERR                        (2)
#define   DA_SECCESS                    (3)
#define   AT24Cxx_READ_ERR              (4)
#define   AT24Cxx_READ_SECCESS          (5)
#define   AT24Cxx_WRITE_ERR             (6)
#define   AT24Cxx_WRITE_SECCESS         (7)










/**
*@Description: 获取电压
*@paramreq-     
*@paramind- 
*@return 无
*/
float VoltageGet(void)
{
	float v = 0.0;
	return v;
}



/**
*@Description: 获取电流
*@paramreq-     
*@paramind- 
*@return 无
*/
float EletricityGet(void)
{
	float e = 0.0;
	return e;
}



/**
*@Description: 获取电阻
*@paramreq-     
*@paramind- 
*@return 无
*/
float ResistanceGet(void)
{
	float res = 0.0;
	return res;
}



/**
*@Description: 获取测试步
*@paramreq-     
*@paramind- 
*@return 无
*/
unsigned char StepGet(void)
{
	unsigned char s = 99;
	return s;
}




/**
*@Description: 获取测试文件
*@paramreq-     
*@paramind- 
*@return 无
*/
unsigned char FileGet(void)
{
	unsigned char f = 10;
	return f;
}





/**
 *@Description: 开机自检程序 检查 AT24Cxx 读写 , AD、DA 如果AD/DA 有问题 则不能运行  如果只有AT24Cxx
 *              有问题则不能设置set参数 只能临时测试
 *@paramreq- 
 *@paramind- 
 *@return 无异常返回成功
 */
uint8 ExamineSelf(void)
{
	return SUCCESS;
}





/**
*@Description: 控制风扇转速程序
*@paramreq-     samp:控制是否采样 0:否  load:测试状态标志
*@paramind- 
*@return 
*/
uint8 FAN_Ctrol(uint32 samp, uint8 load)
{	
	if (temperature < 2407)     // 低于35度 风扇不转
	{
		if (load != 0)      // 负载开
    {
			BSP_PWMDutySet(40);  // 只要负载开 风扇就要转
    }
		else
		{
			BSP_PWMDutySet(0);
    }
  }
	else if (temperature > 3772)   // 过温报警
  {
		BSP_PWMDutySet(90);          // 风扇转速加大
  }
	else                           // 根据温度调节风扇转速
  {
		BSP_PWMDutySet((456*temperature)/10000 - 70);
  }
	
	if(samp != 0)                  // 用来控制是否采样
	{
		LPC_ADC->CR  |= (1 << 24);   // 重新启动内部AD转换
	}
	return 1;
}



/**
*@Description: 设置温度值   在A/D中断里面将采样值设置温度
*@paramreq-     value: 温度   (A/D采样得来的)
*@paramind- 
*@return 
*/
void TemperatureSet(uint32 value)
{
	temperature = (value & 0x00FFFFFF);
}







/**
 *@Description:  获取测试电压值   显示  对电压值进行处理   
 *@paramreq-    adVal:AD采样数组     volt:保存电压值        curr:保存电流值
 *@paramind- 
 *@return 
 */
uint32 GetADValue(uint32 *adVal, uint8 *volt, uint8 *curr)
{
	int32 voltSampleVal = 0;
	int32 currSampleVal = 0;
	uint8 i = 0;
	uint8 dotPos = 1;            // 小数点位置
	
	
	for (i = 0; i < AD_SAMPLE_NUMBS_MAX_VAL; i++)   
  {
		if ((1 << 23) & adVal[2*i])
    {
			// 符号位为1 负码值 正极性的接入
      // 去掉符号位 去掉低4位(低8位数据一直在跳) 这样实际上就只剩19位了
      voltSampleVal  -= (~((adVal[2*i] & 0x007FFFF0) >> 4)) & 0x0007FFFF;
    }
		else
		{
			// 符号位为0 正码值 负极性的接入(极性接反)
      voltSampleVal  += (((adVal[2*i] & 0x007FFFF0) >> 4)) & 0x0007FFFF;
    }
		if ((1 << 23) & adVal[2*i+1])
    {
			// 符号位为1 负码值 正极性的接入
      // 去掉符号位 去掉低4位(低8位数据一直在跳) 这样实际上就只剩19位了
      currSampleVal  -= (~((adVal[2*i+1] & 0x007FFFF0) >> 4)) & 0x0007FFFF;
    }
		else
    {
			// 符号位为0 正码值 负极性的接入(极性接反)
      currSampleVal  += (((adVal[2*i+1] & 0x007FFFF0) >> 4)) & 0x0007FFFF;
    }
  }
	
	// 码值小于零表示外部电源的输入极性为正
	if (voltSampleVal < 0)
  {
		voltSampleVal = ~voltSampleVal + 1;
  }
	else
  {
		voltSampleVal = 0 - voltSampleVal;
  }
	voltSampleVal  /= AD_SAMPLE_NUMBS_MAX_VAL;      // 算平均电压
	
	if (currSampleVal < 0)
  {
		currSampleVal = ~currSampleVal + 1;
  }
	else
  {
		currSampleVal = 0 - currSampleVal;
  }
	currSampleVal  /= AD_SAMPLE_NUMBS_MAX_VAL;      // 算平均电流
	
	voltSampleVal   = ((int64)voltSampleVal*0x00022E4A)/100000 + 0xFFFFFF98;   // 
  currSampleVal   = ((int64)currSampleVal*0x00023CE4)/100000 + 0x0000004E;
	
	// 实际计算出的电压值小于零，可能是因为极性接反导致
  if (voltSampleVal < 0)
	{
		if(voltSampleVal < -4000)
    {
			;
    }
		voltSampleVal  = 0;
  }
	// 大于72V，过压报警
	else if (voltSampleVal > 720000)
  {
		;
  }
	if (voltSampleVal > 999990)
  {
		voltSampleVal   = 999990;
  }
	
	// 将电压转换为字符串
	if (voltSampleVal < 100000)
  {
		*(volt+6) = '\0';	
//    voltSampleVal  /= 10;       // 精确到0.001V  如果要精确到0.0001则不用除10		
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
	
	// 将电流转换为字符串
	if (currSampleVal < 0)
  {
		currSampleVal = 0;
  }
	if (currSampleVal < 100000)
  {
		dotPos = 1;
    currSampleVal  /= 10;

		// 拆分电流
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

		// 拆分电流
    LIB_DataSplit(currSampleVal, 5, (void *)curr);
    LIB_ConvertNmubToChar((void *)curr, 5);
    LIB_StrInsert((void *)curr, dotPos, '.');
    curr[strlen((void *)curr)]  = 'A';
		*(curr+6) = '\0';
  }
		
	return 0;
}








/**
 *@Description:  输入电流值和电阻值 计算电压值 并调用电压设置程序设置电压 
 *@paramreq-   curr_value:电流值       res_value: 电阻值
 *@paramind- 
 *@return 
 */























/******************************************************************************
 *                             END  OF  FILE                                   
******************************************************************************/












