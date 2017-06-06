/**
*@file			key.h
*@brief			与按键有个的一些宏定义或者变量
*@version		
*@author		
*@date		
*/
#ifndef  KEY_H
#define  KEY_H


#include "BSP_CpuConfig.h"

     






/** 静态变量区域 */
//static  uint32  s_keyvalue = 0;   // 记录按键值



/** 按键状态结构体 */
typedef struct STATE
{
	uint8 press;
	uint8 pop;
}KEYSTATE;
static KEYSTATE keystate;       // 按键按下弹起状态







/** 函数声明 */
uint32 Key_Scan(void);           // 键盘扫描,返回按键位置

long Key_Open(void);


#endif
