/**
*@file			key.c
*@brief			��ȡ������ ʵ�ְ������� ��������
*@version		
*@author		
*@date		
*/

#include "key.h"
#include "bsp_gpio.h"
#include "keyfuntion.h"





// �˿ں궨��
#define                     port_KNOB_LEFT_PORT                 (PORT0)
#define                     port_KNOB_LEFT_BIT                  (24)
#define                     port_KNOB_RIGHT_PORT                (PORT0)
#define                     port_KNOB_RIGHT_BIT                 (23)




static  uint8    s_row ;               ///< ��
static  uint8    s_column ;            ///< ��
/** ���̾������� */
const port gpio_keycolumn[] = {{1, 1}, {1, 4}, {1, 8}};

/** ���̾���ֵ */
const port gpio_keyrow[]    = {{4, 28}, {4, 29}, {1, 17}, {1, 16},
                               {1, 15}, {1, 14}, {1, 10}, {1, 9}};





															 
/**
*@Description: ����ɨ��
*@paramreq- 
*@paramind- 
*@return uint32 ����ֵ
*/
uint32 Key_Scan(void)
{
	  uint32 row_val = KEY_NULL; // ��ֵ
    uint32 column_val = KEY_NULL; // ��ֵ
    ulong i = 0;

    //-----------------------------------------------------------------------//
    // ��ɨ����ɨ�У�������������ɨ�������Ϊ���룬����ʹ�ܣ���ɨ�����������
    // ʹ�ܣ�����ߵ�ƽ��Ȼ���ȡ��ɨ������ߵ�ƽ����ʱ��ȷ�����¼������У�Ȼ
    // ����ɨ�������Ϊ���������ʹ�ܣ�����ߵ�ƽ����ɨ�������Ϊ���룬����
    // ʹ�ܣ�Ȼ���ȡ��ɨ������ߵ�ƽ����ʱ��ȷ�����¼������У����˰��µİ���
    // �ھ�������ϵ�������ȷ�����޼�����ʱ��ɨ�赽�����п���ֵȫΪ�͡�
    //-----------------------------------------------------------------------//

    // ������ɨ��ܽ���Ϊ����� ��������ʹ��
    for (i = 0; i < s_row; i++)
    {
        BSP_GPIODirectionSet(gpio_keyrow[i].port, gpio_keyrow[i].bit, DIR_INPUT, PULL_DOWN_RES);
    }
		
    // ������ɨ��ܽ���Ϊ����� ��������ʹ�� ������ߵ�ƽ
    for (i = 0; i < s_column; i++)
    {
        BSP_GPIODirectionSet(gpio_keycolumn[i].port, gpio_keycolumn[i].bit, DIR_OUTPUT, PULL_UP_RES);
        BSP_GPIOValueSet(gpio_keycolumn[i].port, gpio_keycolumn[i].bit, 1);
    }
		
    portNOP();
		
    /* ɨ�������� */
    for (i = 0; i < s_row; i++)
    {
        row_val      <<= 1;//
        row_val      |= BSP_GPIOValueGet(gpio_keyrow[i].port, gpio_keyrow[i].bit);
    }

    // ������ɨ��ܽ���Ϊ����� ��������ʹ�� ������ߵ�ƽ
    for (i = 0; i < s_row; i++)
    {
        BSP_GPIODirectionSet(gpio_keyrow[i].port, gpio_keyrow[i].bit, DIR_OUTPUT, PULL_UP_RES);
        BSP_GPIOValueSet(gpio_keyrow[i].port, gpio_keyrow[i].bit, 1);
    }
		
    // ������ɨ��ܽ���Ϊ����� ��������ʹ��
    for (i = 0; i < s_column; i++)
    {
        BSP_GPIODirectionSet(gpio_keycolumn[i].port, gpio_keycolumn[i].bit, DIR_INPUT, PULL_DOWN_RES);
    }
    portNOP();
    // ɨ��������
    for (i = 0; i < s_column; i++)
    {
        column_val       <<= 1;
        column_val       |= BSP_GPIOValueGet(gpio_keycolumn[i].port, gpio_keycolumn[i].bit);
    }
    return ((row_val << s_column) | column_val);
}




/**
*@Description: ������ �����̺����ɨ��
*@paramreq- 
*@paramind- 
*@return ������
*/
long Key_Open(void)
{
	 uint8      i;
	 s_row    = GET_ARRAY_COUNT(gpio_keyrow);                // ��
	 s_column = GET_ARRAY_COUNT(gpio_keycolumn);             // ��
	 InfoValue();
	 /**  */
	keystate.press = 0;
	keystate.pop   = 0;
	
    // ������ɨ��ܽ���Ϊ�����
    for (i = 0; i < s_column; i++)
    {
        BSP_GPIODirectionSet(gpio_keycolumn[i].port, gpio_keycolumn[i].bit, DIR_INPUT, PULL_UP_RES);
    }

    // ������ɨ��ܽ���Ϊ�����
    for (i = 0; i < s_row; i++)
    {
        BSP_GPIODirectionSet(gpio_keyrow[i].port, gpio_keyrow[i].bit, DIR_OUTPUT, PULL_UP_RES);
    }

    // ���̵Ķ˿ڳ�ʼ��Ϊ�жϿ�
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







