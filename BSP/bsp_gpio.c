/**
*@file		bsp_gpio.c
*@brief			gpio端口设置选择,中断设置等
*@version		
*@author		
*@date			
*/

#include "bsp_gpio.h"
#include <LPC17xx.h>


/**
*@Description: 判断gpio端口  端口合法性判断——判断指定的端口是否存在
*@paramreq- 
*@paramind- 
*@return 无
*/
static portuBASE_TYPE BSP_PortPinValidChk(portuBASE_TYPE port, portuBASE_TYPE bit)
{
    portuBASE_TYPE      rt   = 1;

    // 端口合法性判断——判断指定的端口是否存在
    switch (port)
    {
        case PORT0:
            
            switch (bit)                        // 可以用 if 语句来代替
            {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 15:
                case 16:
                case 17:
                case 18:
                case 19:
                case 20:
                case 21:
                case 22:
                case 23:
                case 24:
                case 25:
                case 26:
                case 27:
                case 28:
                case 29:
                case 30:
                    break;

                default:
                    
                    rt      = 0;
                    break;
            }
            break;

        case PORT1:
            
            switch (bit)
            {
                case 0:
                case 1:
                case 4:
                case 8:
                case 9:
                case 10:
                case 14:
                case 15:
                case 16:
                case 17:
                case 18:
                case 19:
                case 20:
                case 21:
                case 22:
                case 23:
                case 24:
                case 25:
                case 26:
                case 27:
                case 28:
                case 29:
                case 30:
                case 31:
                    break;

                default:
                    
                    rt         = 0;
                    break;
            }
            break;

        case PORT2:
            
            switch (bit)
            {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                    break;

                default:
                    
                    rt        = 0;
                    break;
            }
            break;

        case PORT3:
            
            switch (bit)
            {
                case 25:
                case 26:
                    break;

                default:
                    
                    rt       = 0;
                    break;
            }
            break;

        case PORT4:
            
            switch (bit)
            {
                case 28:
                case 29:
                    break;

                default:
                    
                    rt       = 0;
                    break;
            }
            break;

        default:
            
            rt         = 0;
            break;
    }

    return rt;
}  




/**
*@Description: 设置端口中断
*@paramreq- 
*@paramind- 
*@return 无
*/
portuBASE_TYPE BSP_GPIOIntEnable(portuBASE_TYPE port, portuBASE_TYPE bit, portuBASE_TYPE sense)
{
    // 只有PORT0和PORT2端口有中断
    if ((port != PORT0) && (port != PORT2))
    {
        return FAIL;
    }

    // 指定端口合法性判断
    if (FALSE == BSP_PortPinValidChk(port, bit))
    {
        return FAIL;
    }

    // 使能上升沿中断
    if (GPIO_INT_R == sense)
    {
        if (PORT0 == port)
        {
            LPC_GPIOINT->IO0IntEnR           |= (1 << bit);       // IntEnR : 上升沿中断使能寄存器
        }
        else
        {
            LPC_GPIOINT->IO2IntEnR           |= (1 << bit);
        }
    }
    // 使能下降沿中断
    else
    {
        if (PORT0 == port)
        {
            LPC_GPIOINT->IO0IntEnF           |= (1 << bit);       // IntEnF : 下降沿中断使能寄存器
        }
        else
        {
            LPC_GPIOINT->IO2IntEnF           |= (1 << bit);
        }
    }

    return SUCCESS;
}



/**
*@Description: 选择gpio端口
*@paramreq- 
*@paramind- 
*@return 无
*/
portuBASE_TYPE BSP_GPIOIntDisable(portuBASE_TYPE port, portuBASE_TYPE bit)
{
    // 只有PORT0和PORT2端口有中断
    if ((port != PORT0) && (port != PORT2))
    {
        return FAIL;
    }

    // 指定端口合法性判断
    if (FALSE == BSP_PortPinValidChk(port, bit))
    {
        return FAIL;
    }

    if (PORT0 == port)
    {
        LPC_GPIOINT->IO0IntEnR         &= ~(1 << bit);
        LPC_GPIOINT->IO0IntEnF         &= ~(1 << bit);
    }
    else
    {
        LPC_GPIOINT->IO2IntEnR         &= ~(1 << bit);
        LPC_GPIOINT->IO2IntEnF         &= ~(1 << bit);
    }

    return SUCCESS;
}



/**
*@Description: 中断状态
*@paramreq- 
*@paramind- 
*@return 
*/
portuBASE_TYPE BSP_GPIOIntStausGet(portuBASE_TYPE port, portuBASE_TYPE bit, portuBASE_TYPE sense)
{
    portuBASE_TYPE  gpioIntStatusVal           = 0;

    // 只有PORT0和PORT2端口有中断
    if ((port != PORT0) && (port != PORT2))
    {
        return (portuBASE_TYPE)(-1);
    }

    // 指定端口合法性判断
    if (FALSE == BSP_PortPinValidChk(port, bit))
    {
        return (portuBASE_TYPE)(-1);
    }

    // 获取上升沿中断状态
    if (GPIO_INT_R == sense)
    {
        if (PORT0 == port)
        {
            gpioIntStatusVal         = LPC_GPIOINT->IO0IntStatR & (1 << bit);      // 上升沿的GPIO中断状态
        }
        else
        {
            gpioIntStatusVal         = LPC_GPIOINT->IO2IntStatR & (1 << bit);
        }
    }
    // 获取下降沿中断状态
    else
    {
        if (PORT0 == port)
        {
            gpioIntStatusVal         = LPC_GPIOINT->IO0IntStatF & (1 << bit);     // IntStatF:下降沿的GPIO中断状态寄存器
        }
        else
        {
            gpioIntStatusVal         = LPC_GPIOINT->IO2IntStatF & (1 << bit);
        }
    }

    return gpioIntStatusVal;
}



/**
*@Description: 中断清零
*@paramreq- 
*@paramind- 
*@return 无
*/
portuBASE_TYPE BSP_GPIOIntClear(portuBASE_TYPE port, portuBASE_TYPE bit)
{
    // 只有PORT0和PORT2端口有中断
    if ((port != PORT0) && (port != PORT2))
    {
        return FAIL;
    }

    // 指定端口合法性判断
    if (FALSE == BSP_PortPinValidChk(port, bit))
    {
        return FAIL;
    }

    if (PORT0 == port)
    {
        LPC_GPIOINT->IO0IntClr         |= (1 << bit);          // IntClr: 中断清零寄存器
    }
    else
    {
        LPC_GPIOINT->IO2IntClr         |= (1 << bit);
    }

    return SUCCESS;
}


/**
*@Description: 设置端口中断
*@paramreq- 
*@paramind- 
*@return 无
*/
void BSP_GPIOInit(portuBASE_TYPE port, portuBASE_TYPE direction, portuBASE_TYPE param)
{
	  uint8   portPinNumbArray[]  = {12, 12, 14, 14};
    uint8   i;
    
    
    for (i = 0; i < portPinNumbArray[port]; i++)
    {
        BSP_GPIODirectionSet(port, i, direction, param);    
    }	
}



/**
*@Description: 设置GPIO口引脚模式输入输出方向  PINSEL:引脚功能选择寄存器(TXD RXD...)
*@paramreq-  PINMODE: 引脚模式选择寄存器(上拉下拉)  FIODIR : 端口方向寄存器(输入输出)
*@paramind- 
*@return portuBASE_TYPE
*/
portuBASE_TYPE BSP_GPIODirectionSet(portuBASE_TYPE port, portuBASE_TYPE bit, portuBASE_TYPE direction, 
                                    portuBASE_TYPE param)
{
    portuBASE_TYPE      rt                       = SUCCESS;
    uint8               pinModeVal               = 0;                // 引脚模式值初始化为0--上拉使能
    uint8               portDirVal               = 0;                // 端口方向值初始化为0--输入

    if (DIR_OUTPUT == direction)
    {
        // 管脚方向为输出
        portDirVal         = 1;
    }

    if (PULL_UP_RES == param)
    {
        // 上拉电阻使能
        pinModeVal         = 0;
    }
    else if (AUTO_RES == param)
    {
        // 中继模式
        pinModeVal         = 1;
    }
    else if (NONE_RES == param)
    {
        // 无上拉无下拉电阻
        pinModeVal          = 2;
    }
    else
    {
        // 下拉电阻使能
        pinModeVal          = 3;
    }

    // 若当前引脚不处于GPIO模式时 需要将其变成GPIO模式
    switch (port)
    {
        case PORT0:
            
            switch (bit)                               // 可以用 if 语句来代替
            {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 15:
                case 16:
                case 17:
                case 18:
                case 19:
                case 20:
                case 21:
                case 22:
                case 23:
                case 24:
                case 25:
                case 26:
                    
								// PINSEL:引脚功能选择寄存器      PINMODE: 引脚模式选择寄存器  FIODIR : 端口方向寄存器
                    if (bit < 16)
                    {
                        // 先将引脚功能选择为GPIO  实际上二进制数11左移
                        LPC_PINCON->PINSEL0  &= ~(3 << 2*bit);                  
                        LPC_PINCON->PINMODE0  = (LPC_PINCON->PINMODE0 & (~(3 << 2*bit))) | (pinModeVal << 2*bit); 
                    }
                    else
                    {
                        LPC_PINCON->PINSEL1  &= ~(3 << 2*(bit - 16)); // 
                        LPC_PINCON->PINMODE1  = (LPC_PINCON->PINMODE1 & (~(3 << 2*(bit - 16)))) | (pinModeVal << 2*(bit - 16));
                    }
                    LPC_GPIO0->FIODIR = (LPC_GPIO0->FIODIR & (~(1 << bit))) | (portDirVal << bit);
                    break;

                // SDA0 SCL0--I2C总线开漏总线
                case 27:
                case 28:
                // USB特定引脚
                case 29:
                case 30:
                    
                    // 这几个管脚设置上下拉电阻无效
                    if (NONE_RES == param)
                    {
                        LPC_PINCON->PINSEL1         &= ~(3 << 2*(bit - 16));
                        LPC_GPIO0->FIODIR            = (LPC_GPIO0->FIODIR & (~(1 << bit))) | (portDirVal << bit);
                    }
                    else
                    {
                        rt        = FAIL;
                    }
                    break;

                default:
                    
                    rt           = FAIL;
                    break;
            }
            break;

        case PORT1:
            
            switch (bit)
            {
                case 0:
                case 1:
                case 4:
                case 8:
                case 9:
                case 10:
                case 14:
                case 15:
                case 16:
                case 17:
                case 18:
                case 19:
                case 20:
                case 21:
                case 22:
                case 23:
                case 24:
                case 25:
                case 26:
                case 27:
                case 28:
                case 29:
                case 30:
                case 31:
                    
                    if (bit < 16)
                    {
                        LPC_PINCON->PINSEL2             = ~(3 << 2*bit);
                        LPC_PINCON->PINMODE2            = (LPC_PINCON->PINMODE2 & (~(3 << 2*bit))) | (pinModeVal << 2*bit);
                    }
                    else
                    {
                        LPC_PINCON->PINSEL3            &= ~(3 << 2*(bit - 16));
                        LPC_PINCON->PINMODE3            = (LPC_PINCON->PINMODE3 & (~(3 << 2*(bit - 16)))) | (pinModeVal << 2*(bit - 16));
                    }
                    LPC_GPIO1->FIODIR                   = (LPC_GPIO1->FIODIR & (~(1 << bit))) | (portDirVal << bit);
                    break;

                default:
                    
                    rt       = FAIL;
                    break;
            }
            break;

        case PORT2:
            
            switch (bit)
            {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                    
                    LPC_PINCON->PINSEL4            &= ~(3 << 2*bit);
                    LPC_PINCON->PINMODE4            = (LPC_PINCON->PINMODE4 & (~(3 << 2*bit))) | (pinModeVal << 2*bit);
                    LPC_GPIO2->FIODIR               = (LPC_GPIO2->FIODIR & (~(1 << bit))) | (portDirVal << bit);
                    break;

                default:
                    
                    rt       = FAIL;
                    break;
            }
            break;

        case PORT3:
            
            switch (bit)
            {
                case 25:
                case 26:
                    
                    LPC_PINCON->PINSEL7      &= ~(3 << 2*(bit - 16));
                    LPC_PINCON->PINMODE7      = (LPC_PINCON->PINMODE7 & (~(3 << 2*(bit - 16)))) | (pinModeVal << 2*(bit - 16));
                    LPC_GPIO3->FIODIR         = (LPC_GPIO3->FIODIR & (~(1 << bit))) | (portDirVal << bit);
                    break;

                default:
                    
                    rt       = FAIL;
                    break;
            }
            break;

        case PORT4:
            
            switch (bit)
            {
                case 28:
                case 29:
                    
                    LPC_PINCON->PINSEL9        &= ~(3 << 2*(bit - 16));
                    LPC_PINCON->PINMODE9        = (LPC_PINCON->PINMODE9 & (~(3 << 2*(bit - 16)))) | (pinModeVal << 2*(bit - 16));
                    LPC_GPIO4->FIODIR           = (LPC_GPIO4->FIODIR & (~(1 << bit))) | (portDirVal << bit);
                    break;

                default:
                    
                    rt        = FAIL;
                    break;
            }
            break;

        default:
            
            rt      = FAIL;
            break;
    }

    return rt;
}



/**
*@Description: 读写端口引脚值
*@paramreq-   FIOPIN: 引脚值寄存器
*@paramind- 
*@return 无
*/
portuBASE_TYPE BSP_GPIOValueSet(portuBASE_TYPE port, portuBASE_TYPE bit, portuBASE_TYPE value)
{
    // 指定端口合法性判断
    if (FALSE == BSP_PortPinValidChk(port, bit))
    {
        return FAIL;
    }

    switch (port)
    {
        case PORT0:              
            
            LPC_GPIO0->FIOPIN         = (LPC_GPIO0->FIOPIN & (~(1 << bit))) | (value << bit);
            break;

        case PORT1:
            
            LPC_GPIO1->FIOPIN         = (LPC_GPIO1->FIOPIN & (~(1 << bit))) | (value << bit);
            break;

        case PORT2:
            
            LPC_GPIO2->FIOPIN         = (LPC_GPIO2->FIOPIN & (~(1 << bit))) | (value << bit);
            break;

        case PORT3:
            
            LPC_GPIO3->FIOPIN         = (LPC_GPIO3->FIOPIN & (~(1 << bit))) | (value << bit);
            break;

        case PORT4:
            
            LPC_GPIO4->FIOPIN         = (LPC_GPIO4->FIOPIN & (~(1 << bit))) | (value << bit);
            break;
    }

    return SUCCESS;
}



/**
*@Description: 读端口引脚值
*@paramreq-   FIOPIN: 引脚值寄存器
*@paramind- 
*@return 无
*/
portuBASE_TYPE BSP_GPIOValueGet(portuBASE_TYPE port, portuBASE_TYPE bit)
{
    portuBASE_TYPE      portVal         = 0;

    // 指定端口合法性判断
    if (FALSE == BSP_PortPinValidChk(port, bit))
    {
        return (portuBASE_TYPE)(-1);
    }

    switch (port)
    {
        case PORT0:
            
            portVal         = LPC_GPIO0->FIOPIN;
            break;

        case PORT1:
            
            portVal         = LPC_GPIO1->FIOPIN;
            break;

        case PORT2:
            
            portVal         = LPC_GPIO2->FIOPIN;
            break;

        case PORT3:
            
            portVal         = LPC_GPIO3->FIOPIN;
            break;

        case PORT4:
            
            portVal         = LPC_GPIO4->FIOPIN;
            break;
    }
    
    // 判断要读取的位是否为0 是0返回0否则返回1
    return ((portVal & (1 << bit)) == 0)? (0):(1);
}





/**
*@Description: 设置端口引脚值
*@paramreq- FIOPIN: 引脚值寄存器
*@paramind- 
*@return 无
*/
portuBASE_TYPE BSP_GPIOPortValueSet(portuBASE_TYPE port, portuBASE_TYPE value)
{
    // 指定端口合法性判断
    if (FALSE == BSP_PortPinValidChk(port, NULL))
    {
        return FAIL;
    }

    switch (port)
    {
        case PORT0:
            
            LPC_GPIO0->FIOPIN          = value;
            break;

        case PORT1:
            
            LPC_GPIO1->FIOPIN          = value;
            break;

        case PORT2:
            
            LPC_GPIO2->FIOPIN          = value;
            break;

        case PORT3:
            
            LPC_GPIO3->FIOPIN          = value;
            break;

        case PORT4:
            
            LPC_GPIO4->FIOPIN          = value;
            break;
    }

    return SUCCESS;
}



/**
*@Description: 读端口引脚值
*@paramreq- FIOPIN: 引脚值寄存器
*@paramind- 
*@return 无
*/
portBASE_TYPE BSP_GPIOPortValueGet(portuBASE_TYPE port)
{
    portuBASE_TYPE      portVal      = 0;

    // 指定端口合法性判断
    if (FALSE == BSP_PortPinValidChk(port, NULL))
    {
        return -1;
    }

    switch (port)
    {
        case PORT0:
            
            portVal             = LPC_GPIO0->FIOPIN;
            break;

        case PORT1:
            
            portVal             = LPC_GPIO1->FIOPIN;
            break;

        case PORT2:
            
            portVal             = LPC_GPIO2->FIOPIN;
            break;

        case PORT3:
            
            portVal             = LPC_GPIO3->FIOPIN;
            break;

        case PORT4:
            
            portVal             = LPC_GPIO4->FIOPIN;
            break;
    }

    return portVal;
}



/**
*@Description: 获取IO口输入输出方向
*@paramreq-   FIODIR: 端口方向控制寄存器
*@paramind- 
*@return 无
*/
portBASE_TYPE BSP_GPIOPortDDRGet(portuBASE_TYPE port)
{
    portBASE_TYPE               portDDRVal  = 0;
    // 指定端口合法性判断
    if (FALSE == BSP_PortPinValidChk(port, NULL))
    {
        return -1;
    }

    switch (port)
    {
        case PORT0:
            
            portDDRVal          = LPC_GPIO0->FIODIR;
            break;

        case PORT1:
            
            portDDRVal          = LPC_GPIO1->FIODIR;
            break;

        case PORT2:
            
            portDDRVal          = LPC_GPIO2->FIODIR;
            break;

        case PORT3:
            
            portDDRVal          = LPC_GPIO3->FIODIR;
            break;

        case PORT4:
            
            portDDRVal          = LPC_GPIO4->FIODIR;
            break;
    }

    return portDDRVal;
}



/**
*@Description: 设置IO口输入输出方向
*@paramreq-     FIODIR: 端口方向控制寄存器
*@paramind- 
*@return 无
*/
portuBASE_TYPE BSP_GPIOPortDDRSet(portuBASE_TYPE port, portuBASE_TYPE portDDRvalue)
{
    // 指定端口合法性判断
    if (FALSE == BSP_PortPinValidChk(port, NULL))
    {
        return FAIL;
    }

    switch (port)
    {
        case PORT0:
            
            LPC_GPIO0->FIODIR         = portDDRvalue;
            break;

        case PORT1:
            
            LPC_GPIO1->FIODIR         = portDDRvalue;
            break;

        case PORT2:
            
            LPC_GPIO2->FIODIR         = portDDRvalue;
            break;

        case PORT3:
            
            LPC_GPIO3->FIODIR         = portDDRvalue;
            break;

        case PORT4:
            
            LPC_GPIO4->FIODIR         = portDDRvalue;
            break;
    }

    return SUCCESS;
}

