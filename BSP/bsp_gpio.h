#ifndef BSP_GPIO_H 
#define BSP_GPIO_H




#include <LPC17xx.h>			
#include "BSP_CpuConfig.h"			
			
// 端口方向宏定义
#define         DIR_OUTPUT                             (0)
#define         DIR_INPUT                              (1)
// 引脚模式宏定义
#define         PULL_UP_RES                            (0) // 上拉电阻
#define         AUTO_RES                               (1) // 中继模式
#define         NONE_RES                               (2) // 无上拉无下拉
#define         PULL_DOWN_RES                          (3) // 下拉电阻
// 端口号宏定义
#define         PORT0                                  (0)
#define         PORT1                                  (1)
#define         PORT2                                  (2)
#define         PORT3                                  (3)
#define         PORT4                                  (4)

// GPIO中断方式宏定义
#define         GPIO_INT_R                             (0)
#define         GPIO_INT_F                             (1)




#define                     MEAS_V_L_BIT               (0)
#define                     SENSE_EN_H_BIT             (0)
// 负载开时高 关时低 该信号打开时先于码值的输出 关闭时后于码值的输出
#define                     ON_OFF_EN_H_BIT            (1)
// OVP中断管脚(过压硬件中断信号脚)
#define                     OVP_INT_L_BIT              (13)
// 输出信号管脚(当出现上下限报警时输出高，否则输出低)
#define                     OUT_SIGNAL_H_BIT           (13)
#define                     HIGH_L_BIT                 (18)
#define                     OVP_CLR_L_BIT              (19)
#define                     CC_EN_L_BIT                (22)
#define                     CV_EN_L_BIT                (23)
#define                     CV_EN_L_BIT                (23)
#define                     DAC_REF_L_BIT              (25)
#define                     DAC_REF_L_BIT              (25)
#define                     SLW3_L_BIT                 (25)
#define                     SLW4_L_BIT                 (26)
#define                     SLW4_L_BIT                 (26)
#define                     CG_EN_L_BIT                (27)
#define                     CG_EN_L_BIT                (27)
#define                     CC_RANGE_L_BIT             (27)
// CCH时低电平
#define                     CC_RANGE_L_BIT             (27)
// 系统配置中若SENSE配置为使能 则该信号高电平 否则低电平
#define                     CV_RANGE_L_BIT             (28)
#define                     CV_RANGE_L_BIT             (28)
#define                     CR_EN_L_BIT                (28)
#define                     CR_EN_L_BIT                (28)
#define                     SLW2_L_BIT                 (29)
#define                     MEAS_I_L_BIT               (29)
#define                     SLW1_L_BIT                 (30)

/******************************************************************************
 *                         文件接口数据结构体声明
******************************************************************************/

typedef struct gpio_port
{
    uint8                       port;
    uint8                       bit;
}port;
			



/******************************************************************************
 *                            文件接口函数声明
******************************************************************************/ 

extern portuBASE_TYPE   BSP_GPIOIntEnable(portuBASE_TYPE port, portuBASE_TYPE bit, portuBASE_TYPE sense);

extern portuBASE_TYPE   BSP_GPIOIntDisable(portuBASE_TYPE port, portuBASE_TYPE bit);
			
extern portuBASE_TYPE   BSP_GPIOIntStausGet(portuBASE_TYPE port, portuBASE_TYPE bit, portuBASE_TYPE sense);

extern portuBASE_TYPE   BSP_GPIOIntClear(portuBASE_TYPE port, portuBASE_TYPE bit);

extern void             BSP_GPIOInit(portuBASE_TYPE port, portuBASE_TYPE direction, portuBASE_TYPE param);


extern portuBASE_TYPE   BSP_GPIOValueSet(portuBASE_TYPE port, portuBASE_TYPE bit, portuBASE_TYPE value);

extern portuBASE_TYPE   BSP_GPIOValueGet(portuBASE_TYPE port, portuBASE_TYPE bit);

extern portuBASE_TYPE   BSP_GPIOPortValueSet(portuBASE_TYPE port, portuBASE_TYPE value);

extern portBASE_TYPE    BSP_GPIOPortValueGet(portuBASE_TYPE port);

extern portBASE_TYPE    BSP_GPIOPortDDRGet(portuBASE_TYPE port);

extern portuBASE_TYPE   BSP_GPIOPortDDRSet(portuBASE_TYPE port, portuBASE_TYPE portDDRvalue);

extern portuBASE_TYPE   BSP_GPIODirectionSet(portuBASE_TYPE port, portuBASE_TYPE bit, portuBASE_TYPE direction, 
                                             portuBASE_TYPE param);

#endif
