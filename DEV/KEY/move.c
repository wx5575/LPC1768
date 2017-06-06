/**
 *@file		
 *@brief			选项的上下左右移动函数 
 *@version		
 *@author		
 *@date		
 */
#include "move.h"
#include "datavalue.h"
#include "lcd.h"



/**
*@Description: 上下移动 
*@paramreq- str:当前选项的位置  infomation: 移动的值   move_type: 1下移 0上移  movepos:移动标记
*@paramind-  第二行数据和第一行数据一样 当等于2时下移有问题
*@return 无  
*/
void UpAndDown_One(INFO infomation, uint8* str, MOVEPOSITION* movepos, uint8 move_type)
{
	uint8 num = 0;
	uint8 count = infomation.amount;
	uint8 *opt;
	
	for(num = 0;num<infomation.amount;num++)
	{
		if(str[num]>infomation.num[num]) // 下标不能超限
		{
			str[num] = 0;
		}
	}
	if(count<2)
	{
		return ;
	}
	
	if(move_type == 0)   // 上移
	{
		if(movepos->point == 0)    // 指针在第一行
		{
			if(movepos->position == 0)//数组下标为0
			{
				if(count%2 == 1)//数组个数为单
				{
					LCD_Clear(); // 清屏
					num = 0x7E;
					movepos->point = 0;
					movepos->position = count-1; //最后一个
					opt = infomation.option[movepos->position];
					LCD_Write(&num,0,movepos->point);  // 第一行画箭头
					LCD_Write(infomation.title[count-1],1,movepos->point);  // 标题
					LCD_Write(opt,15,movepos->point);  // 数据
					return ;
				}
				else  // 标题数为双
				{
					movepos->point = 1;  // 第二行
					movepos->position = count-2;
				}
			}
			else
			{
				movepos->point = 1; // 指向第二行
				movepos->position -= 2;   // 移动2个
			}
		}
		else    // 指针在第二行
		{
			num = 0x7E;
			movepos->point = 0;   // 指向第一行
			LCD_Write(&num,0,0);  // 第一行画箭头
			LCD_Write(" ",0,1);   // 清除第二行的箭头
			return ;
		}
	}

	/** 下移 */	
	else
	{
		if((movepos->position == count-1)&&((movepos->position+movepos->point) == count-1)) //指向最后一个数据
		{
			movepos->point = 0;  // 第1行
			movepos->position = 0;
		}
		else
		{
			if(movepos->point == 0)//第一行
			{
				movepos->point = 1;  // 第2行
				num = 0x7E;
				LCD_Write(&num,0,1);  // 画箭头
			  LCD_Write(" ",0,0);   // 清除箭头
			  return ;
			}
			else
			{
				movepos->point = 0;
				if(movepos->position == count-3)// 
				{
					LCD_Clear(); // 清屏
					num = 0x7E;
					movepos->position = count-1; //最后一个
					opt = infomation.option[movepos->position];
					LCD_Write(&num,0,movepos->point);  // 第一行画箭头
					LCD_Write(infomation.title[count-1],1,movepos->point);  // 最后一个标题  有问题
					LCD_Write(opt,15,movepos->point);  // 
					return ;
				}
				else
				{
					movepos->position += 2;
				}
			}
		}
	}
	
	LCD_Clear(); // 清屏
	num = 0x7E;
	opt = infomation.option[movepos->position];
	LCD_Write(&num,0,movepos->point);  // 画箭头
	LCD_Write(infomation.title[movepos->position],1,0);  // 第一行标题
	LCD_Write(opt,15,0);  // 画数据
	
	opt = infomation.option[movepos->position+1]; // 下一个选项
	LCD_Write(infomation.title[movepos->position+1],1,1);  // 第二行标题
	LCD_Write(opt,15,1);  // 画数据
}






/**
*@Description: 左右移动 用于固定选项的左右移动
*@paramreq- infodata:选项数组  datanum: 选项数组大小  move_type: 1右移 0左移  option:选项当前下标值
*@paramind- 
*@return 移动后的选项下标值
*/
uint8 LeftAndRight_One(uint8** infodata, uint8 datanum, uint8 option, uint8 move_type)
{
	uint8 result = 0;  // 位置
	
	if(datanum <= 1)  // 数量太少
	{
		return 0;
	}
	else
	{
		if(move_type == 0)  // 左移
		{
			if(option == 0)  // 第一个数据
			{
				result = datanum-1;
      }
			else
			{
				result = option - 1;
			}
	  }
		else  // 右移
		{
			if(option == datanum-1)// 最后一个数据
			{
				result = 0;
			}
			else
			{
				result = option+1;
			}
	  }
  }
	
	return result;  // 数组下标
}
	
	
	



/**
 *@Description: set参数选中后选择选项
 *@paramreq- type: 1:选项值加一  0:选项值减一
 *@paramind-  
 *@return  
 */
void UpAndDown_Two(INFO infomation, uint8 type)
{
}






















	
/*************     END     *************/	
	
	
	
	











