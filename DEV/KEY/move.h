/**
*@file			
*@brief			�ƶ��õ�һЩ�����������
*@version		
*@author		
*@date		
*/
#ifndef  MOVE_H
#define  MOVE_H

#include "datavalue.h"



/* �ƶ���־�ṹ�� */
typedef struct MOVE
{
	uint8 position;        // ��һ��������±�
	uint8 point;           // �ƶ�ָ��ָ�������  0:��һ��  1: �ڶ���
}MOVEPOSITION;
static MOVEPOSITION s_display;         // �ƶ���¼


void UpAndDown_One(INFO infomation, uint8* str, MOVEPOSITION* movepos, uint8 move_type);
uint8 LeftAndRight_One(uint8** infodata, uint8 datanum, uint8 option, uint8 move_type);

#endif

