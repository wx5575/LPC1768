#ifndef  LCD_H
#define  LCD_H

#include "bsp_cpuconfig.h"



//����������
#define             CLR_LCD_CMD_CODE                    (0x01)    // ���LCD��ʾ���� ��ACΪ��


#define             FUNC_SET_CMD                        (FUNC_SET_CMD_BASE_CODE|FUNC_SET_CMD_CPU_8BIT|FUNC_SET_CMD_SCREEN_TWO_ROW)
#define             DISP_CTRL_CMD                       (DISP_CTRL_CMD_BASE_CODE|DISP_CTRL_CMD_DISP_ON)
#define             ENTER_MODE_CMD                      (ENTER_MODE_CMD_BASE_CODE|ENTER_MODE_CMD_PTR_INC)
//��������������
#define             FUNC_SET_CMD_BASE_CODE              (0x20)
#define             FUNC_SET_CMD_CPU_8BIT               (0x10)    //CPU����
#define             FUNC_SET_CMD_SCREEN_TWO_ROW         (0x08)    //������ʾ

#define             DISP_CTRL_CMD_BASE_CODE             (0x08)
#define             DISP_CTRL_CMD_DISP_ON               (0x04)    // ����ʾ
#define             ENTER_MODE_CMD_BASE_CODE            (0x04)
#define             ENTER_MODE_CMD_PTR_INC              (0x02)    // ����ƶ�����




void WriteLcdData(uint32 data);

int16 Lcd_Init(void);

int16 Lcd_Open(void);

void LCD_Write(unsigned  char*  buffer, unsigned short x,unsigned short y);   //size: Ҫ��ʾ�ַ��� buffer ��ǰ size ���ַ�,���ܳ��� buffer �Ĵ�С                 

void LCD_Empty(uint8 x, uint8 y, uint8 num);

void LCD_Clear(void);

#endif
