#include "DA.h"
#include "bsp_gpio.h"
#include <LPC17xx.h>





// ���ģ��SPIʹ�����궨��
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
 *@Description: ģ��SSP�ӿڷ���һ֡����
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
    
    // ��ʼ����ʹ�ö˿�
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
    
    // ѡ��SSP0�ܽŹ���
    BSP_GPIODirectionSet(PORT1, 20, DIR_OUTPUT, PULL_UP_RES);
    BSP_GPIODirectionSet(PORT1, 21, DIR_OUTPUT, PULL_UP_RES);
    BSP_GPIODirectionSet(PORT1, 24, DIR_OUTPUT, PULL_UP_RES);
    LPC_PINCON->PINSEL3     = (LPC_PINCON->PINSEL3 & 0xFFFCF0FF)|(0x00030F00);

    // SSEL��Ϊ��ͨIO����
    BSP_GPIODirectionSet(PORT1, 21, DIR_OUTPUT, PULL_UP_RES);
    BSP_GPIOValueSet(PORT1, 21, 1);
    BSP_GPIODirectionSet(port_LDAC_PORT, port_LDAC_BIT, DIR_OUTPUT, PULL_UP_RES);

    // SSP0ʱ��Ԥ��Ƶ�Ĵ���2��Ƶ
    LPC_SSP0->CPSR                  = 2;
    // ѡ��SPIģʽ λ���ʷ�Ƶ��1��Ƶ 16λ���ݴ���
    LPC_SSP0->CR0                   = 0x010F;
    // SSP���ƼĴ�������
    LPC_SSP0->CR1                  |= (1 << 1);

#if (AD5545_DEBUG > 0)
    SPISendData(0xFFFFFFFF);
    SPISendData((1 << 16)|0x7FF);
    SPISendData((1 << 16)|0xFFF);
    SPISendData((3 << 16)|0x3FFF);
#endif
#endif

    // �ܽ�ѡ��ΪDAC����ܽ�
    LPC_PINCON->PINSEL1     = (LPC_PINCON->PINSEL0 & 0xFFCFFFFF)|(0x00200000);
    // DAC����ܽ�����Ϊ������������ģʽ
    LPC_PINCON->PINMODE1    = (LPC_PINCON->PINMODE1 & 0xFFCFFFFF)|(0x00200000);

#if (AD5545_DEBUG > 0)   
/*    LPC_DAC->DACR           = 100 << 6;  // ����6λ: DACR��0:5λ����,��Ӧ����д��1
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























