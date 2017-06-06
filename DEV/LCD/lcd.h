#ifndef  LCD_H
#define  LCD_H

#include "bsp_cpuconfig.h"



//清屏命令码
#define             CLR_LCD_CMD_CODE                    (0x01)    // 清除LCD显示内容 置AC为零


#define             FUNC_SET_CMD                        (FUNC_SET_CMD_BASE_CODE|FUNC_SET_CMD_CPU_8BIT|FUNC_SET_CMD_SCREEN_TWO_ROW)
#define             DISP_CTRL_CMD                       (DISP_CTRL_CMD_BASE_CODE|DISP_CTRL_CMD_DISP_ON)
#define             ENTER_MODE_CMD                      (ENTER_MODE_CMD_BASE_CODE|ENTER_MODE_CMD_PTR_INC)
//功能设置命令码
#define             FUNC_SET_CMD_BASE_CODE              (0x20)
#define             FUNC_SET_CMD_CPU_8BIT               (0x10)    //CPU类型
#define             FUNC_SET_CMD_SCREEN_TWO_ROW         (0x08)    //两行显示

#define             DISP_CTRL_CMD_BASE_CODE             (0x08)
#define             DISP_CTRL_CMD_DISP_ON               (0x04)    // 开显示
#define             ENTER_MODE_CMD_BASE_CODE            (0x04)
#define             ENTER_MODE_CMD_PTR_INC              (0x02)    // 光标移动方向




void WriteLcdData(uint32 data);

int16 Lcd_Init(void);

int16 Lcd_Open(void);

void LCD_Write(unsigned  char*  buffer, unsigned short x,unsigned short y);   //size: 要显示字符串 buffer 的前 size 个字符,不能超过 buffer 的大小                 

void LCD_Empty(uint8 x, uint8 y, uint8 num);

void LCD_Clear(void);

#endif
