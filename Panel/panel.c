/**
*@file			panel.c
*@brief			�����������
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
*@Description: ����� ��ʾ��ѹ������������ ���Ե�
*@paramreq-     ��
*@paramind- 
*@return ��
*/
void MainPanel(void)
{
//	float value = 0.0;
	unsigned char value;
	unsigned char load;
	unsigned char volstr[6] = {'0','.','0','0','0','0'};             // ��ѹ
	unsigned char elestr[6] = {'1','0','.','0','0','0'};             // ����
	unsigned char resstr[6] = {'1','0','0','.','0','0'};             // ����
  unsigned char str[2] = {'1','0'};

	LCD_Clear();                                  // ����
	
//	value = VoltageGet();                         // ��ȡ��ѹ
//	FloatToStr(volstr, value, 4);                // ת��Ϊ�ַ��� 4λС��
	LCD_Write(volstr,0,0);                        // ��ʾ
	LCD_Write("V",6,0);                           // ��ʾ V
	
//	value = EletricityGet();                      // ����
//	FloatToStr(elestr, value, 4);                
	LCD_Write(elestr,9,0);                        
	LCD_Write("A",15,0);                          
	
	
	load = Read_Load();
	if(load == 0)                       
	{
		LCD_Write("ON",17,0);                        // LCD��ʾ����״̬
	}
	else
	{
		LCD_Write("OFF",17,0);                      
	}
	
//	value = ResistanceGet();                       // ����
//	FloatToStr(resstr, value, 4);             
	LCD_Write(resstr,0,1);                        
	LCD_Write("m",6,1);
	WriteLcdData(0xF4);                           // ��ʾ ��
	
	LCD_Write("Step",9,1);                        // ���Բ�
	value = StepGet();
	IntToStr(str, value);               
	LCD_Write(str,13,1);                        // ��ʾ ���Բ�
	
	LCD_Write("F",16,1);
	value = FileGet();
	IntToStr(str, value);                     
	LCD_Write(str,17,1);                        // ��ʾ �����ļ�
}







