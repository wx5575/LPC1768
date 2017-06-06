/**
*@file			
*@brief			LCD 驱动、开启LCD、清屏、写字符串等
*@version		
*@author		
*@date		
*/
#include "lcd.h"
#include "bsp_gpio.h"



/******************************************************************************
 *                            本文件内部宏定义
******************************************************************************/ 
#define         WRITE_INSTRUCTION               (0x00)              // 定义液晶屏的地址，其中隐含液晶屏的读写操作时序,写指令
#define         READ_INSTRUCTION                (0x02)              // 读指令
#define         WRITE_DATA                      (0x01)              // 写数据

#define         LCD_RAM_ADDR_CAL(x, y)          (128 + y*64 + x)    // 根据X Y坐标计算LCD的RAM地址

#define         port_LCD_WRT_RD_PORT            (PORT0)             // 液晶屏WR RD信号 实际上这两个信号根本就没有用处 就是一直处于低电平 
#define         port_LCD_WR_BIT                 (5)
#define         port_LCD_RD_BIT                 (4)
#define         port_LCD_RS_RW_PORT             (PORT0)             // 液晶屏R/W RS信号管脚
#define         port_LCD_RS_BIT                 (8)
#define         port_LCD_RW_BIT                 (7)
#define         port_LCD_EN_PORT                (PORT0)             // 液晶屏EN信号管脚
#define         port_LCD_EN_BIT                 (6)
#define         port_LCD_DATA_BUS_PORT          (PORT2)             // 液晶屏数据线D0-D7 目前的程序只能适应这些数据线在同一端口的连续位号上的情况
#define         port_LCD_DATA_BUS_LOW_BIT       (0)                 // 数据线位于P1.18-P1.25 故最低位号为18

#define         port_LCD_EN                     (0)                 // LCD高电平使能(单片机输出低 LCD得到高)
#define         port_LCD_DISEN                  (1)                 // LCD低电平禁能(单片机输出高 LCD得到低)


static uint32 ReadLcdRegister(uint32 dataType);
static void WriteLcdRegister(uint32 dataType, uint32 data);



/**
*@Description: 设置读LCD模式
*@paramreq- 
*@paramind- 
*@return 
*/
static uint32 ReadLcdInstruction(void)
{
    return ReadLcdRegister(READ_INSTRUCTION);                  
}



/**
*@Description: 写LCD
*@paramreq- 
*@paramind- 
*@return 
*/
void WriteLcdInstruction(uint32 instruction)
{
    uint32                  i   = 0;

    // 查忙状态 查询指定的次数后若还处于忙状态 则跳过此步
    while (((ReadLcdInstruction() & 0x80) != 0) && (i++ < 1000));
    // 送命令字
    WriteLcdRegister(WRITE_INSTRUCTION,instruction);
}




/**
*@Description: 
*@paramreq- 
*@paramind- 
*@return 
*/
static uint32 ReadLcdRegister(uint32 dataType)
{
    uint32                  data;
	  uint32                  i;
    portuBASE_TYPE          portAddrVal = 0;

	// 寄存器功能选择
    BSP_GPIOValueSet(port_LCD_RS_RW_PORT, port_LCD_RS_BIT, dataType & 0x01);
    BSP_GPIOValueSet(port_LCD_RS_RW_PORT, port_LCD_RW_BIT, (dataType & 0x02) >> 1);

    portNOP();                   // 延时
    portNOP();
    
    // 数据口方向置为输入脚
    portAddrVal                 = BSP_GPIOPortDDRGet(port_LCD_DATA_BUS_PORT);
    BSP_GPIOPortDDRSet(port_LCD_DATA_BUS_PORT, portAddrVal & ~(0xFF << port_LCD_DATA_BUS_LOW_BIT));

	// 液晶屏操作使能
    BSP_GPIOValueSet(port_LCD_EN_PORT, port_LCD_EN_BIT, port_LCD_EN);
	for (i = 0; i < 5; i++);
	// 读数据     
    data                        = (BSP_GPIOPortValueGet(port_LCD_DATA_BUS_PORT) >> port_LCD_DATA_BUS_LOW_BIT) & 0xFF;
    // 液晶屏操作禁能
    BSP_GPIOValueSet(port_LCD_EN_PORT, port_LCD_EN_BIT, port_LCD_DISEN);
	
    // 数据口方向置为输出脚
	BSP_GPIOPortDDRSet(port_LCD_DATA_BUS_PORT, portAddrVal | (0xFF << port_LCD_DATA_BUS_LOW_BIT));

	   return data;
}




/**
*@Description: 控制LCD端口
*@paramreq- 
*@paramind- 
*@return 
*/
static void WriteLcdRegister(uint32 dataType, uint32 data)
{
    portuBASE_TYPE          i;
    portuBASE_TYPE          portVal     = 0;

      
    // 寄存器功能选择 虽然时序图上没有体现出两个信号的先后关系 但是若把这两者在现在基础上调换顺序
    // 就会出错——有时写屏对 有时错；按照现在顺序或者两者同时送出去都没问题 
    BSP_GPIOValueSet(port_LCD_RS_RW_PORT, port_LCD_RW_BIT, (dataType & 0x02) >> 1);
    BSP_GPIOValueSet(port_LCD_RS_RW_PORT, port_LCD_RS_BIT, dataType & 0x01);

    portNOP();
    portNOP();

	// 液晶屏操作使能
    BSP_GPIOValueSet(port_LCD_EN_PORT, port_LCD_EN_BIT, port_LCD_EN);
	for(i = 0; i < 5; i++);
    // 写数据
    portVal       = BSP_GPIOPortValueGet(port_LCD_DATA_BUS_PORT);
    portVal       = (portVal & ~(0xFF << port_LCD_DATA_BUS_LOW_BIT)) | (data << port_LCD_DATA_BUS_LOW_BIT);
    BSP_GPIOPortValueSet(port_LCD_DATA_BUS_PORT, portVal);
	// 液晶屏操作禁能
    BSP_GPIOValueSet(port_LCD_EN_PORT, port_LCD_EN_BIT, port_LCD_DISEN); 
}





/**
*@Description: LCD写一个字
*@paramreq- data: 16进制数据
*@paramind- 
*@return 
*/
void WriteLcdData(uint32 data)
{
    uint32    i = 0;

    // 查忙状态 查询指定的次数后若还处于忙状态 则跳过此步
    while (((ReadLcdInstruction() & 0x80) != 0) && (i++ < 1000));   // 查忙状态
    // 写数据
    WriteLcdRegister(WRITE_DATA,data);
}




/**
*@Description: 写字符串到LCD
*@paramreq- buffer: 字符串指针    x:列(0-19)   y: 行 (0:第一行)
*@paramind- 
*@return 
*/
void LCD_Write(unsigned  char*  buffer, unsigned short x,unsigned short y)                
{
    WriteLcdInstruction(LCD_RAM_ADDR_CAL(x, y)); 
	
	while(*buffer != '\0')                   // 字符串结尾
	{
				WriteLcdData(*((char *)buffer));   // 写一个字符
        portNOP();
        portNOP();
		    buffer++;
	}
	
}




/**
*@Description: LCD初始化  先初始化后开启才能用
*@paramreq- 
*@paramind- 
*@return 
*/
int16 Lcd_Init(void)
{
    portBASE_TYPE       i;

    // 设置 LCD 数据总线 D0-D7 为输出口 且上拉电阻使能
    for (i = 0; i < 8; i++)
    {
        BSP_GPIODirectionSet(port_LCD_DATA_BUS_PORT, port_LCD_DATA_BUS_LOW_BIT + i, DIR_OUTPUT, PULL_UP_RES);
    }

    // 设置 WR RD 信号管脚为输出 且上拉电阻使能
    BSP_GPIODirectionSet(port_LCD_WRT_RD_PORT, port_LCD_WR_BIT, DIR_OUTPUT, PULL_UP_RES);
    BSP_GPIODirectionSet(port_LCD_WRT_RD_PORT, port_LCD_RD_BIT, DIR_OUTPUT, PULL_UP_RES);

    // 设置控制液晶屏 RS RW 引脚的管脚为输出 且上拉电阻使能    
    BSP_GPIODirectionSet(port_LCD_RS_RW_PORT, port_LCD_RS_BIT, DIR_OUTPUT, PULL_UP_RES);
    BSP_GPIODirectionSet(port_LCD_RS_RW_PORT, port_LCD_RW_BIT, DIR_OUTPUT, PULL_UP_RES);

    // 设置控制液晶屏 EN 引脚的管脚为输出 且上拉电阻使能
    BSP_GPIODirectionSet(port_LCD_EN_PORT, port_LCD_EN_BIT, DIR_OUTPUT, PULL_UP_RES);

    // WR 和 RD信号置为低电平 且以后无需改变
    BSP_GPIOValueSet(port_LCD_WRT_RD_PORT, port_LCD_WR_BIT, 0);
    BSP_GPIOValueSet(port_LCD_WRT_RD_PORT, port_LCD_RD_BIT, 0);
		
    return RT_EOK;
}




/**
*@Description: LCD开启 , 只有开启后LCD才能写
*@paramreq- 
*@paramind- 
*@return 
*/
int16 Lcd_Open(void)
{
    // 清屏
    WriteLcdInstruction(CLR_LCD_CMD_CODE);
	// 器件功能设置
    WriteLcdInstruction(FUNC_SET_CMD);
    // 显示模式设置
    WriteLcdInstruction(DISP_CTRL_CMD);
	// 输入模式设置
    WriteLcdInstruction(ENTER_MODE_CMD);
	
    #if (LCD_DEBUG_SERV > 0)
        
        // 128 + y*64 + x
        WriteLcdInstruction(128 + 4);                                   
    #endif
    return RT_EOK;
}



/**
*@Description: LCD清除字符
*@paramreq-    x:第几格    y: 行   num: 清除的个数
*@paramind- 
*@return 
*/
void LCD_Empty(uint8 x, uint8 y, uint8 num)
{
	uint8 i;
	for(i=0;i<=num; i++)
	{
		LCD_Write(" ", x+i, y);
	}
}


/**
*@Description: 清屏
*@paramreq-    
*@paramind- 
*@return 
*/
void LCD_Clear(void)
{
	WriteLcdInstruction(CLR_LCD_CMD_CODE);
}


