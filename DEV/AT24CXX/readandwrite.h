#ifndef  READANDWRITE_H
#define  READANDWRITE_H


#include "bsp_gpio.h"


// IO控制命令宏定义
#define             IO_CTRL_CMD_I2C_WP_ON               (0)     // 开始写保护
#define             IO_CTRL_CMD_I2C_WP_OFF              (1)     // 关闭写保护
#define             IO_CTRL_CMD_I2C_WP_STATUS_GET       (2)     // 写保护状态查询  写保护状态码下面进行了宏定义
#define             IO_CTRL_CMD_I2C_DEV_INDEX_SET       (3)     // 指定当前要操作的器件的索引号--索引号在下面宏定义了
                                                                // 指定索引号的器件的器件地址在该命令执行处手工添加上
#define             IO_CTRL_CMD_I2C_MUTEX_GET           (4)     // 获取I2C器件保护用互斥量

// 器件编号宏定义--本仪器也只有这一片I2C器件    供外部使用
#define             CAL_DATA_I2C_DEV                    (0)     // 该器件的器件地址为0xA2
#define             SET_PARAM_I2C_DEV                   (1)     // 该器件的器件地址为0xA0   AT24C512


static uint8 temp2 = 0;


uint8 AT24Cxx_Check(void);
uint8 AT24Cxx_Init(uint8 cmd, void *args);
uint32 AT24Cxx_Read(uint32 addr, void* buffer, uint32 size);
uint32 AT24Cxx_Write(uint32 addr, uint8* buffer, uint32 size);




#endif


