/**
*@file			key.h
*@brief			�밴���и���һЩ�궨����߱���
*@version		
*@author		
*@date		
*/
#ifndef  KEY_H
#define  KEY_H


#include "BSP_CpuConfig.h"

     






/** ��̬�������� */
//static  uint32  s_keyvalue = 0;   // ��¼����ֵ



/** ����״̬�ṹ�� */
typedef struct STATE
{
	uint8 press;
	uint8 pop;
}KEYSTATE;
static KEYSTATE keystate;       // �������µ���״̬







/** �������� */
uint32 Key_Scan(void);           // ����ɨ��,���ذ���λ��

long Key_Open(void);


#endif
