/**
*@file			
*@brief			移动用的一些变量标记申明
*@version		
*@author		
*@date		
*/
#ifndef  MOVE_H
#define  MOVE_H

#include "datavalue.h"



/* 移动标志结构体 */
typedef struct MOVE
{
	uint8 position;        // 第一行数组的下标
	uint8 point;           // 移动指针指向的行数  0:第一行  1: 第二行
}MOVEPOSITION;
static MOVEPOSITION s_display;         // 移动记录


void UpAndDown_One(INFO infomation, uint8* str, MOVEPOSITION* movepos, uint8 move_type);
uint8 LeftAndRight_One(uint8** infodata, uint8 datanum, uint8 option, uint8 move_type);

#endif

