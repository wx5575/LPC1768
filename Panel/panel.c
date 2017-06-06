/**
*@file			panel.c
*@brief			画开机界面等
*@version
*@author		
*@date			
*/

#include "lcd.h"
#include "test.h"
#include "stringhandle.h"
#include "panel.h"
#include "datavalue.h"
#include "keyfuntion.h"


/**
*@Description: 主面板 显示电压、电流、电阻 测试等
*@paramreq-     无
*@paramind- 
*@return 无
*/
void MainPanel(void)
{
//	float value = 0.0;
	unsigned char value;
	unsigned char load;
	unsigned char volstr[6] = {'0','.','0','0','0','0'};             // 电压
	unsigned char elestr[6] = {'1','0','.','0','0','0'};             // 电流
	unsigned char resstr[6] = {'1','0','0','.','0','0'};             // 电阻
  unsigned char str[2] = {'1','0'};

	LCD_Clear();                                  // 清屏
	
//	value = VoltageGet();                         // 获取电压
//	FloatToStr(volstr, value, 4);                // 转换为字符串 4位小数
	LCD_Write(volstr,0,0);                        // 显示
	LCD_Write("V",6,0);                           // 显示 V
	
//	value = EletricityGet();                      // 电流
//	FloatToStr(elestr, value, 4);                
	LCD_Write(elestr,9,0);                        
	LCD_Write("A",15,0);                          
	
	
	load = Read_Load();
	if(load == 0)                       
	{
		LCD_Write("ON",17,0);                        // LCD显示负载状态
	}
	else
	{
		LCD_Write("OFF",17,0);                      
	}
	
//	value = ResistanceGet();                       // 电阻
//	FloatToStr(resstr, value, 4);             
	LCD_Write(resstr,0,1);                        
	LCD_Write("m",6,1);
	WriteLcdData(0xF4);                           // 显示 Ω
	
	LCD_Write("Step",9,1);                        // 测试步
	value = StepGet();
	IntToStr(str, value);               
	LCD_Write(str,13,1);                        // 显示 测试步
	
	LCD_Write("F",16,1);
	value = FileGet();
	IntToStr(str, value);                     
	LCD_Write(str,17,1);                        // 显示 测试文件
}







