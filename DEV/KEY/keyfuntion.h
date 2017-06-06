#ifndef  KEYFUNTION_H
#define  KEYFUNTION_H

#include "bsp_gpio.h"



/** ����ɨ��ֵ�궨�� */
#define            KEY_VAL_LOAD                   (0x00000401)
#define            KEY_VAL_UP                     (0x00000201)
#define            KEY_VAL_DOWN                   (0x00000101)
#define            KEY_VAL_LEFT                   (0x00000081)
#define            KEY_VAL_RIGHT                  (0x00000041)
#define            KEY_VAL_SET                    (0x00000021)
#define            KEY_VAL_ENTER                  (0x00000011)
#define            KEY_VAL_ESC                    (0x00000104)
#define            KEY_VAL_0                      (0x00000404)
#define            KEY_VAL_1                      (0x00000022)
#define            KEY_VAL_2                      (0x00000012)
#define            KEY_VAL_3                      (0x0000000A)
#define            KEY_VAL_4                      (0x00000102)
#define            KEY_VAL_5                      (0x00000082)
#define            KEY_VAL_6                      (0x00000042)
#define            KEY_VAL_7                      (0x00000009)
#define            KEY_VAL_8                      (0x00000402)      // 8    and  Trig
#define            KEY_VAL_9                      (0x00000202)      // 9    and  Local
#define            KEY_VAL_DOT                    (0x00000204)      // dot  and  system

#define            KEY_NULL                       (0x00000000)  //�޼�����ֵ 

#define            SCAN_NUM           (100)         // ɨ�����



/** ������ͼ�¼�궨�� */
#define            PANEL_NULL                         (0x00)            // ��Ч״̬
#define            PANEL_MAIN                         (0x01)            // �����
#define            PANEL_SET                          (0x02)            // ���� SET
#define            PANEL_SYSTEM                       (0x03)            // ���� SYSTEM
#define            PANEL_SET_EDIT                     (0x04)            // SET �༭״̬
#define            PANEL_SYSTEM_EDIT                  (0x05)            // SYSTEM �༭״̬


#define            CH_AMO                             (7)               // 7��



/** ����¼�ṹ��*/
typedef struct PANEL
{
	uint8 now;         // ��ǰ
	uint8 old;         // ��һ��
}PANELRECORD;
static PANELRECORD s_panel;   // ���״̬





/** �ⲿ���ú���  */
void InfoValue(void);
void KeyFunction(uint32 value);
uint32 ReadKeyValue(void);
void WriteKeyValue(uint32 value);
uint8 Read_Local(void);
uint8 Reversal_Local(void);
uint8 Read_Load(void);
uint8 Reversal_Load(void);
uint8 ReadPanel(void);


#endif


/*************     END     *************/	
