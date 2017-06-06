/**
*@file		bsp_gpio.c
*@brief			gpio�˿�����ѡ��,�ж����õ�
*@version		
*@author		
*@date			
*/

#include "bsp_gpio.h"
#include <LPC17xx.h>


/**
*@Description: �ж�gpio�˿�  �˿ںϷ����жϡ����ж�ָ���Ķ˿��Ƿ����
*@paramreq- 
*@paramind- 
*@return ��
*/
static portuBASE_TYPE BSP_PortPinValidChk(portuBASE_TYPE port, portuBASE_TYPE bit)
{
    portuBASE_TYPE      rt   = 1;

    // �˿ںϷ����жϡ����ж�ָ���Ķ˿��Ƿ����
    switch (port)
    {
        case PORT0:
            
            switch (bit)                        // ������ if ���������
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
*@Description: ���ö˿��ж�
*@paramreq- 
*@paramind- 
*@return ��
*/
portuBASE_TYPE BSP_GPIOIntEnable(portuBASE_TYPE port, portuBASE_TYPE bit, portuBASE_TYPE sense)
{
    // ֻ��PORT0��PORT2�˿����ж�
    if ((port != PORT0) && (port != PORT2))
    {
        return FAIL;
    }

    // ָ���˿ںϷ����ж�
    if (FALSE == BSP_PortPinValidChk(port, bit))
    {
        return FAIL;
    }

    // ʹ���������ж�
    if (GPIO_INT_R == sense)
    {
        if (PORT0 == port)
        {
            LPC_GPIOINT->IO0IntEnR           |= (1 << bit);       // IntEnR : �������ж�ʹ�ܼĴ���
        }
        else
        {
            LPC_GPIOINT->IO2IntEnR           |= (1 << bit);
        }
    }
    // ʹ���½����ж�
    else
    {
        if (PORT0 == port)
        {
            LPC_GPIOINT->IO0IntEnF           |= (1 << bit);       // IntEnF : �½����ж�ʹ�ܼĴ���
        }
        else
        {
            LPC_GPIOINT->IO2IntEnF           |= (1 << bit);
        }
    }

    return SUCCESS;
}



/**
*@Description: ѡ��gpio�˿�
*@paramreq- 
*@paramind- 
*@return ��
*/
portuBASE_TYPE BSP_GPIOIntDisable(portuBASE_TYPE port, portuBASE_TYPE bit)
{
    // ֻ��PORT0��PORT2�˿����ж�
    if ((port != PORT0) && (port != PORT2))
    {
        return FAIL;
    }

    // ָ���˿ںϷ����ж�
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
*@Description: �ж�״̬
*@paramreq- 
*@paramind- 
*@return 
*/
portuBASE_TYPE BSP_GPIOIntStausGet(portuBASE_TYPE port, portuBASE_TYPE bit, portuBASE_TYPE sense)
{
    portuBASE_TYPE  gpioIntStatusVal           = 0;

    // ֻ��PORT0��PORT2�˿����ж�
    if ((port != PORT0) && (port != PORT2))
    {
        return (portuBASE_TYPE)(-1);
    }

    // ָ���˿ںϷ����ж�
    if (FALSE == BSP_PortPinValidChk(port, bit))
    {
        return (portuBASE_TYPE)(-1);
    }

    // ��ȡ�������ж�״̬
    if (GPIO_INT_R == sense)
    {
        if (PORT0 == port)
        {
            gpioIntStatusVal         = LPC_GPIOINT->IO0IntStatR & (1 << bit);      // �����ص�GPIO�ж�״̬
        }
        else
        {
            gpioIntStatusVal         = LPC_GPIOINT->IO2IntStatR & (1 << bit);
        }
    }
    // ��ȡ�½����ж�״̬
    else
    {
        if (PORT0 == port)
        {
            gpioIntStatusVal         = LPC_GPIOINT->IO0IntStatF & (1 << bit);     // IntStatF:�½��ص�GPIO�ж�״̬�Ĵ���
        }
        else
        {
            gpioIntStatusVal         = LPC_GPIOINT->IO2IntStatF & (1 << bit);
        }
    }

    return gpioIntStatusVal;
}



/**
*@Description: �ж�����
*@paramreq- 
*@paramind- 
*@return ��
*/
portuBASE_TYPE BSP_GPIOIntClear(portuBASE_TYPE port, portuBASE_TYPE bit)
{
    // ֻ��PORT0��PORT2�˿����ж�
    if ((port != PORT0) && (port != PORT2))
    {
        return FAIL;
    }

    // ָ���˿ںϷ����ж�
    if (FALSE == BSP_PortPinValidChk(port, bit))
    {
        return FAIL;
    }

    if (PORT0 == port)
    {
        LPC_GPIOINT->IO0IntClr         |= (1 << bit);          // IntClr: �ж�����Ĵ���
    }
    else
    {
        LPC_GPIOINT->IO2IntClr         |= (1 << bit);
    }

    return SUCCESS;
}


/**
*@Description: ���ö˿��ж�
*@paramreq- 
*@paramind- 
*@return ��
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
*@Description: ����GPIO������ģʽ�����������  PINSEL:���Ź���ѡ��Ĵ���(TXD RXD...)
*@paramreq-  PINMODE: ����ģʽѡ��Ĵ���(��������)  FIODIR : �˿ڷ���Ĵ���(�������)
*@paramind- 
*@return portuBASE_TYPE
*/
portuBASE_TYPE BSP_GPIODirectionSet(portuBASE_TYPE port, portuBASE_TYPE bit, portuBASE_TYPE direction, 
                                    portuBASE_TYPE param)
{
    portuBASE_TYPE      rt                       = SUCCESS;
    uint8               pinModeVal               = 0;                // ����ģʽֵ��ʼ��Ϊ0--����ʹ��
    uint8               portDirVal               = 0;                // �˿ڷ���ֵ��ʼ��Ϊ0--����

    if (DIR_OUTPUT == direction)
    {
        // �ܽŷ���Ϊ���
        portDirVal         = 1;
    }

    if (PULL_UP_RES == param)
    {
        // ��������ʹ��
        pinModeVal         = 0;
    }
    else if (AUTO_RES == param)
    {
        // �м�ģʽ
        pinModeVal         = 1;
    }
    else if (NONE_RES == param)
    {
        // ����������������
        pinModeVal          = 2;
    }
    else
    {
        // ��������ʹ��
        pinModeVal          = 3;
    }

    // ����ǰ���Ų�����GPIOģʽʱ ��Ҫ������GPIOģʽ
    switch (port)
    {
        case PORT0:
            
            switch (bit)                               // ������ if ���������
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
                    
								// PINSEL:���Ź���ѡ��Ĵ���      PINMODE: ����ģʽѡ��Ĵ���  FIODIR : �˿ڷ���Ĵ���
                    if (bit < 16)
                    {
                        // �Ƚ����Ź���ѡ��ΪGPIO  ʵ���϶�������11����
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

                // SDA0 SCL0--I2C���߿�©����
                case 27:
                case 28:
                // USB�ض�����
                case 29:
                case 30:
                    
                    // �⼸���ܽ�����������������Ч
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
*@Description: ��д�˿�����ֵ
*@paramreq-   FIOPIN: ����ֵ�Ĵ���
*@paramind- 
*@return ��
*/
portuBASE_TYPE BSP_GPIOValueSet(portuBASE_TYPE port, portuBASE_TYPE bit, portuBASE_TYPE value)
{
    // ָ���˿ںϷ����ж�
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
*@Description: ���˿�����ֵ
*@paramreq-   FIOPIN: ����ֵ�Ĵ���
*@paramind- 
*@return ��
*/
portuBASE_TYPE BSP_GPIOValueGet(portuBASE_TYPE port, portuBASE_TYPE bit)
{
    portuBASE_TYPE      portVal         = 0;

    // ָ���˿ںϷ����ж�
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
    
    // �ж�Ҫ��ȡ��λ�Ƿ�Ϊ0 ��0����0���򷵻�1
    return ((portVal & (1 << bit)) == 0)? (0):(1);
}





/**
*@Description: ���ö˿�����ֵ
*@paramreq- FIOPIN: ����ֵ�Ĵ���
*@paramind- 
*@return ��
*/
portuBASE_TYPE BSP_GPIOPortValueSet(portuBASE_TYPE port, portuBASE_TYPE value)
{
    // ָ���˿ںϷ����ж�
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
*@Description: ���˿�����ֵ
*@paramreq- FIOPIN: ����ֵ�Ĵ���
*@paramind- 
*@return ��
*/
portBASE_TYPE BSP_GPIOPortValueGet(portuBASE_TYPE port)
{
    portuBASE_TYPE      portVal      = 0;

    // ָ���˿ںϷ����ж�
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
*@Description: ��ȡIO�������������
*@paramreq-   FIODIR: �˿ڷ�����ƼĴ���
*@paramind- 
*@return ��
*/
portBASE_TYPE BSP_GPIOPortDDRGet(portuBASE_TYPE port)
{
    portBASE_TYPE               portDDRVal  = 0;
    // ָ���˿ںϷ����ж�
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
*@Description: ����IO�������������
*@paramreq-     FIODIR: �˿ڷ�����ƼĴ���
*@paramind- 
*@return ��
*/
portuBASE_TYPE BSP_GPIOPortDDRSet(portuBASE_TYPE port, portuBASE_TYPE portDDRvalue)
{
    // ָ���˿ںϷ����ж�
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

