/**
*@file			key.c
*@brief			读取按键码 实现按键功能 驱动按键
*@version		
*@author		
*@date		
*/

#include "key.h"
#include "bsp_gpio.h"
#include "keyfuntion.h"





// 端口宏定义
#define                     port_KNOB_LEFT_PORT                 (PORT0)
#define                     port_KNOB_LEFT_BIT                  (24)
#define                     port_KNOB_RIGHT_PORT                (PORT0)
#define                     port_KNOB_RIGHT_BIT                 (23)




static  uint8    s_row ;               ///< 行
static  uint8    s_column ;            ///< 列
/** 键盘矩阵数组 */
const port gpio_keycolumn[] = {{1, 1}, {1, 4}, {1, 8}};

/** 键盘矩阵值 */
const port gpio_keyrow[]    = {{4, 28}, {4, 29}, {1, 17}, {1, 16},
                               {1, 15}, {1, 14}, {1, 10}, {1, 9}};





															 
/**
*@Description: 键盘扫描
*@paramreq- 
*@paramind- 
*@return uint32 按键值
*/
uint32 Key_Scan(void)
{
	  uint32 row_val = KEY_NULL; // 行值
    uint32 column_val = KEY_NULL; // 列值
    ulong i = 0;

    //-----------------------------------------------------------------------//
    // 先扫行再扫列，具体做法：行扫描口线置为输入，下拉使能，列扫描输出，上拉
    // 使能，输出高电平，然后读取行扫描各口线电平，此时可确定按下键所在行；然
    // 后，行扫描口线置为输出，上拉使能，输出高电平，列扫描口线置为输入，下拉
    // 使能，然后读取列扫描各口线电平，此时可确定按下键所在列，至此按下的按键
    // 在矩阵键盘上的坐标则确定。无键按下时，扫描到的行列口线值全为低。
    //-----------------------------------------------------------------------//

    // 所有行扫描管脚置为输入脚 下拉电阻使能
    for (i = 0; i < s_row; i++)
    {
        BSP_GPIODirectionSet(gpio_keyrow[i].port, gpio_keyrow[i].bit, DIR_INPUT, PULL_DOWN_RES);
    }
		
    // 所有列扫描管脚置为输出脚 上拉电阻使能 并输出高电平
    for (i = 0; i < s_column; i++)
    {
        BSP_GPIODirectionSet(gpio_keycolumn[i].port, gpio_keycolumn[i].bit, DIR_OUTPUT, PULL_UP_RES);
        BSP_GPIOValueSet(gpio_keycolumn[i].port, gpio_keycolumn[i].bit, 1);
    }
		
    portNOP();
		
    /* 扫描所有行 */
    for (i = 0; i < s_row; i++)
    {
        row_val      <<= 1;//
        row_val      |= BSP_GPIOValueGet(gpio_keyrow[i].port, gpio_keyrow[i].bit);
    }

    // 所有行扫描管脚置为输出脚 上拉电阻使能 并输出高电平
    for (i = 0; i < s_row; i++)
    {
        BSP_GPIODirectionSet(gpio_keyrow[i].port, gpio_keyrow[i].bit, DIR_OUTPUT, PULL_UP_RES);
        BSP_GPIOValueSet(gpio_keyrow[i].port, gpio_keyrow[i].bit, 1);
    }
		
    // 所有列扫描管脚置为输入脚 下拉电阻使能
    for (i = 0; i < s_column; i++)
    {
        BSP_GPIODirectionSet(gpio_keycolumn[i].port, gpio_keycolumn[i].bit, DIR_INPUT, PULL_DOWN_RES);
    }
    portNOP();
    // 扫描所有列
    for (i = 0; i < s_column; i++)
    {
        column_val       <<= 1;
        column_val       |= BSP_GPIOValueGet(gpio_keycolumn[i].port, gpio_keycolumn[i].bit);
    }
    return ((row_val << s_column) | column_val);
}




/**
*@Description: 开键盘 开键盘后才能扫描
*@paramreq- 
*@paramind- 
*@return 错误码
*/
long Key_Open(void)
{
	 uint8      i;
	 s_row    = GET_ARRAY_COUNT(gpio_keyrow);                // 行
	 s_column = GET_ARRAY_COUNT(gpio_keycolumn);             // 列
	 InfoValue();
	 /**  */
	keystate.press = 0;
	keystate.pop   = 0;
	
    // 所有列扫描管脚置为输入脚
    for (i = 0; i < s_column; i++)
    {
        BSP_GPIODirectionSet(gpio_keycolumn[i].port, gpio_keycolumn[i].bit, DIR_INPUT, PULL_UP_RES);
    }

    // 所有行扫描管脚置为输出脚
    for (i = 0; i < s_row; i++)
    {
        BSP_GPIODirectionSet(gpio_keyrow[i].port, gpio_keyrow[i].bit, DIR_OUTPUT, PULL_UP_RES);
    }

    // 拨盘的端口初始化为中断口
    BSP_GPIODirectionSet(port_KNOB_LEFT_PORT, port_KNOB_LEFT_BIT, DIR_INPUT, PULL_UP_RES);
    BSP_GPIODirectionSet(port_KNOB_RIGHT_PORT, port_KNOB_RIGHT_BIT, DIR_INPUT, PULL_UP_RES);
    BSP_GPIOIntEnable(port_KNOB_LEFT_PORT, port_KNOB_LEFT_BIT, GPIO_INT_R);
    BSP_GPIOIntEnable(port_KNOB_RIGHT_PORT, port_KNOB_RIGHT_BIT, GPIO_INT_R);
    NVIC_SetPriority(EINT3_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
    BSP_GPIOIntClear(port_KNOB_LEFT_PORT, port_KNOB_LEFT_BIT);
    BSP_GPIOIntClear(port_KNOB_RIGHT_PORT, port_KNOB_RIGHT_BIT);
    NVIC_EnableIRQ(EINT3_IRQn);

    return RT_EOK;      
}





/*************     END     *************/	







