#ifndef  READANDWRITE_H
#define  READANDWRITE_H


#include "bsp_gpio.h"


// IO��������궨��
#define             IO_CTRL_CMD_I2C_WP_ON               (0)     // ��ʼд����
#define             IO_CTRL_CMD_I2C_WP_OFF              (1)     // �ر�д����
#define             IO_CTRL_CMD_I2C_WP_STATUS_GET       (2)     // д����״̬��ѯ  д����״̬����������˺궨��
#define             IO_CTRL_CMD_I2C_DEV_INDEX_SET       (3)     // ָ����ǰҪ������������������--������������궨����
                                                                // ָ�������ŵ�������������ַ�ڸ�����ִ�д��ֹ������
#define             IO_CTRL_CMD_I2C_MUTEX_GET           (4)     // ��ȡI2C���������û�����

// ������ź궨��--������Ҳֻ����һƬI2C����    ���ⲿʹ��
#define             CAL_DATA_I2C_DEV                    (0)     // ��������������ַΪ0xA2
#define             SET_PARAM_I2C_DEV                   (1)     // ��������������ַΪ0xA0   AT24C512


static uint8 temp2 = 0;


uint8 AT24Cxx_Check(void);
uint8 AT24Cxx_Init(uint8 cmd, void *args);
uint32 AT24Cxx_Read(uint32 addr, void* buffer, uint32 size);
uint32 AT24Cxx_Write(uint32 addr, uint8* buffer, uint32 size);




#endif


