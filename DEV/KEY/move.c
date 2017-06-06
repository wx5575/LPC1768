/**
 *@file		
 *@brief			ѡ������������ƶ����� 
 *@version		
 *@author		
 *@date		
 */
#include "move.h"
#include "datavalue.h"
#include "lcd.h"



/**
*@Description: �����ƶ� 
*@paramreq- str:��ǰѡ���λ��  infomation: �ƶ���ֵ   move_type: 1���� 0����  movepos:�ƶ����
*@paramind-  �ڶ������ݺ͵�һ������һ�� ������2ʱ����������
*@return ��  
*/
void UpAndDown_One(INFO infomation, uint8* str, MOVEPOSITION* movepos, uint8 move_type)
{
	uint8 num = 0;
	uint8 count = infomation.amount;
	uint8 *opt;
	
	for(num = 0;num<infomation.amount;num++)
	{
		if(str[num]>infomation.num[num]) // �±겻�ܳ���
		{
			str[num] = 0;
		}
	}
	if(count<2)
	{
		return ;
	}
	
	if(move_type == 0)   // ����
	{
		if(movepos->point == 0)    // ָ���ڵ�һ��
		{
			if(movepos->position == 0)//�����±�Ϊ0
			{
				if(count%2 == 1)//�������Ϊ��
				{
					LCD_Clear(); // ����
					num = 0x7E;
					movepos->point = 0;
					movepos->position = count-1; //���һ��
					opt = infomation.option[movepos->position];
					LCD_Write(&num,0,movepos->point);  // ��һ�л���ͷ
					LCD_Write(infomation.title[count-1],1,movepos->point);  // ����
					LCD_Write(opt,15,movepos->point);  // ����
					return ;
				}
				else  // ������Ϊ˫
				{
					movepos->point = 1;  // �ڶ���
					movepos->position = count-2;
				}
			}
			else
			{
				movepos->point = 1; // ָ��ڶ���
				movepos->position -= 2;   // �ƶ�2��
			}
		}
		else    // ָ���ڵڶ���
		{
			num = 0x7E;
			movepos->point = 0;   // ָ���һ��
			LCD_Write(&num,0,0);  // ��һ�л���ͷ
			LCD_Write(" ",0,1);   // ����ڶ��еļ�ͷ
			return ;
		}
	}

	/** ���� */	
	else
	{
		if((movepos->position == count-1)&&((movepos->position+movepos->point) == count-1)) //ָ�����һ������
		{
			movepos->point = 0;  // ��1��
			movepos->position = 0;
		}
		else
		{
			if(movepos->point == 0)//��һ��
			{
				movepos->point = 1;  // ��2��
				num = 0x7E;
				LCD_Write(&num,0,1);  // ����ͷ
			  LCD_Write(" ",0,0);   // �����ͷ
			  return ;
			}
			else
			{
				movepos->point = 0;
				if(movepos->position == count-3)// 
				{
					LCD_Clear(); // ����
					num = 0x7E;
					movepos->position = count-1; //���һ��
					opt = infomation.option[movepos->position];
					LCD_Write(&num,0,movepos->point);  // ��һ�л���ͷ
					LCD_Write(infomation.title[count-1],1,movepos->point);  // ���һ������  ������
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
	
	LCD_Clear(); // ����
	num = 0x7E;
	opt = infomation.option[movepos->position];
	LCD_Write(&num,0,movepos->point);  // ����ͷ
	LCD_Write(infomation.title[movepos->position],1,0);  // ��һ�б���
	LCD_Write(opt,15,0);  // ������
	
	opt = infomation.option[movepos->position+1]; // ��һ��ѡ��
	LCD_Write(infomation.title[movepos->position+1],1,1);  // �ڶ��б���
	LCD_Write(opt,15,1);  // ������
}






/**
*@Description: �����ƶ� ���ڹ̶�ѡ��������ƶ�
*@paramreq- infodata:ѡ������  datanum: ѡ�������С  move_type: 1���� 0����  option:ѡ�ǰ�±�ֵ
*@paramind- 
*@return �ƶ����ѡ���±�ֵ
*/
uint8 LeftAndRight_One(uint8** infodata, uint8 datanum, uint8 option, uint8 move_type)
{
	uint8 result = 0;  // λ��
	
	if(datanum <= 1)  // ����̫��
	{
		return 0;
	}
	else
	{
		if(move_type == 0)  // ����
		{
			if(option == 0)  // ��һ������
			{
				result = datanum-1;
      }
			else
			{
				result = option - 1;
			}
	  }
		else  // ����
		{
			if(option == datanum-1)// ���һ������
			{
				result = 0;
			}
			else
			{
				result = option+1;
			}
	  }
  }
	
	return result;  // �����±�
}
	
	
	



/**
 *@Description: set����ѡ�к�ѡ��ѡ��
 *@paramreq- type: 1:ѡ��ֵ��һ  0:ѡ��ֵ��һ
 *@paramind-  
 *@return  
 */
void UpAndDown_Two(INFO infomation, uint8 type)
{
}






















	
/*************     END     *************/	
	
	
	
	











