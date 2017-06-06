#include "DA.h"
#include "bsp_gpio.h"
#include <LPC17xx.h>





// 软件模拟SPI使能与否宏定义
#define     SPI_SOFTWARE_SIMULATOR              (1)


#if (SPI_SOFTWARE_SIMULATOR > 0)

#define             port_SCK_PORT                       (PORT1)
#define             port_SCK_BIT                        (20)

#define             port_CS_PORT                        (PORT1)
#define             port_CS_BIT                         (21)

#define             port_MOSI_PORT                      (PORT1)
#define             port_MOSI_BIT                       (24)


#define             SCK_PORT_INIT()                     BSP_GPIODirectionSet(port_SCK_PORT, port_SCK_BIT, DIR_OUTPUT, PULL_UP_RES)
#define             SCK_LOW()                           BSP_GPIOValueSet(port_SCK_PORT, port_SCK_BIT, 0)
#define             SCK_HIGH()                          BSP_GPIOValueSet(port_SCK_PORT, port_SCK_BIT, 1)
                    
#define             CS_PORT_INIT()                      BSP_GPIODirectionSet(port_CS_PORT, port_CS_BIT, DIR_OUTPUT, PULL_UP_RES)
#define             CS_LOW_EN()                         BSP_GPIOValueSet(port_CS_PORT, port_CS_BIT, 0)
#define             CS_HIGH_DISEN()                     BSP_GPIOValueSet(port_CS_PORT, port_CS_BIT, 1)

#define             MOSI_PORT_INIT()                    BSP_GPIODirectionSet(port_MOSI_PORT, port_MOSI_BIT, DIR_OUTPUT, PULL_UP_RES)
#define             MOSI_LOW()                          BSP_GPIOValueSet(port_MOSI_PORT, port_MOSI_BIT, 0)
#define             MOSI_HIGH()                         BSP_GPIOValueSet(port_MOSI_PORT, port_MOSI_BIT, 1)

#endif

#define             AD5545_DEBUG                        (1)






/**
 *@Description: 模拟SSP接口发送一帧数据
 *@paramreq- 
 *@paramind- 
 *@return 
 */
void SPISendData(uint32  data)
{
#if (SPI_SOFTWARE_SIMULATOR > 0)
    uint8   i;
    
    //LDAC_HIGH();
    portNOP();
    
    CS_LOW_EN();
    portNOP();        

    for(i = 32; i != 0; i-- )
    {
        SCK_LOW();
        portNOP();
        if(data & (1 << 31))
        {
            MOSI_HIGH();
        }                                                                
        else
        {
            MOSI_LOW();
        }                                                                
        SCK_HIGH();
        portNOP();                                          
        data <<= 1;    
    }
    
    portNOP();
    CS_HIGH_DISEN();
#else
    //LDAC_HIGH();
    portNOP();
    BSP_GPIOValueSet(PORT1, 21, 0);
    portNOP();
    while((1 << 4) & LPC_SSP0->SR);
    LPC_SSP0->DR        = (data >> 16);
    while((1 << 4) & LPC_SSP0->SR);
    LPC_SSP0->DR        = (data & 0x0000FFFF);
    while((1 << 4) & LPC_SSP0->SR);
    portNOP();
    BSP_GPIOValueSet(PORT1, 21, 1);
#endif                                                                    
}






/**
 *@Description: 
 *@paramreq- 
 *@paramind- 
 *@return 
 */
uint8 AD5545_Open(void)
{   
#if (SPI_SOFTWARE_SIMULATOR > 0)
    
    // 初始化所使用端口
    SCK_PORT_INIT();
    //LDAC_PORT_INIT();
    CS_PORT_INIT();
    MOSI_PORT_INIT();

    CS_HIGH_DISEN();

    #if (AD5545_DEBUG > 0)
   
//    SPISendData((1 << 16)|0x7FF);
//    SPISendData((1 << 16)|0xFFF);
//    SPISendData((1 << 16)|0x1FFF);
    SPISendData((3 << 16)|0x3FF);
    #endif

#else
    
    // 选择SSP0管脚功能
    BSP_GPIODirectionSet(PORT1, 20, DIR_OUTPUT, PULL_UP_RES);
    BSP_GPIODirectionSet(PORT1, 21, DIR_OUTPUT, PULL_UP_RES);
    BSP_GPIODirectionSet(PORT1, 24, DIR_OUTPUT, PULL_UP_RES);
    LPC_PINCON->PINSEL3     = (LPC_PINCON->PINSEL3 & 0xFFFCF0FF)|(0x00030F00);

    // SSEL变为普通IO口用
    BSP_GPIODirectionSet(PORT1, 21, DIR_OUTPUT, PULL_UP_RES);
    BSP_GPIOValueSet(PORT1, 21, 1);
    BSP_GPIODirectionSet(port_LDAC_PORT, port_LDAC_BIT, DIR_OUTPUT, PULL_UP_RES);

    // SSP0时钟预分频寄存器2分频
    LPC_SSP0->CPSR                  = 2;
    // 选择SPI模式 位速率分频器1分频 16位数据传输
    LPC_SSP0->CR0                   = 0x010F;
    // SSP控制寄存器禁能
    LPC_SSP0->CR1                  |= (1 << 1);

#if (AD5545_DEBUG > 0)
    SPISendData(0xFFFFFFFF);
    SPISendData((1 << 16)|0x7FF);
    SPISendData((1 << 16)|0xFFF);
    SPISendData((3 << 16)|0x3FFF);
#endif
#endif

    // 管脚选择为DAC输出管脚
    LPC_PINCON->PINSEL1     = (LPC_PINCON->PINSEL0 & 0xFFCFFFFF)|(0x00200000);
    // DAC输出管脚配置为无上下拉电阻模式
    LPC_PINCON->PINMODE1    = (LPC_PINCON->PINMODE1 & 0xFFCFFFFF)|(0x00200000);

#if (AD5545_DEBUG > 0)   
/*    LPC_DAC->DACR           = 100 << 6;  // 左移6位: DACR的0:5位保留,不应向其写入1
    LPC_DAC->DACR           = 200 << 6;
    LPC_DAC->DACR           = 400 << 6;
    LPC_DAC->DACR           = 600 << 6;
    LPC_DAC->DACR           = 800 << 6;
    LPC_DAC->DACR           = 1000 << 6;
*/		
		LPC_DAC->DACR           = 0xFFFF << 6;
#endif
    return RT_EOK;
}























