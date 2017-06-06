#include "AD.h"
#include "lpc17xx.h"
#include "delay.h"
#include "bsp_gpio.h"
#include "test.h"






// 软件模拟SPI接口宏定义
#define   SPI_INTERFACE_SOFTWARE              (1)
#define   AD7766_DEBUG                        (0)
#define   INNER_AD_VAL_GET_MASK_CODE          (0x0000FFF0)



#define   port_SCK_PORT          (PORT0)  // SCK=>双向数字时钟引脚
#define   port_SCK_BIT           (15)

#define   port_MISO_PORT         (PORT0)  // SDO=>三态数字输出
#define   port_MISO_BIT          (17)

#define   port_MOSI_PORT         (PORT0)  // SDI=>串行数据输入
#define   port_MOSI_BIT          (18)

#define   AD_BIT_NUMBS           (32)     // LTC2440串行输出数据流的长度为32位,前3位代表状态信息...(中文手册11页)

#define   port_CS_PORT           (PORT0)  // CS引脚=>低态有效数字输入. 低电平:使能SDO数字输出并唤醒ADC, 高电平:低功耗  低转高:中断数据转移,启动一个新的变换
#define   port_CS_BIT            (16)

#define     SCK_PORT_INIT()        BSP_GPIODirectionSet(port_SCK_PORT, port_SCK_BIT, DIR_OUTPUT, PULL_UP_RES)
//#define     SCK_LOW()               BSP_GPIOValueSet(port_SCK_PORT, port_SCK_BIT, 0)
//#define     SCK_HIGH()              BSP_GPIOValueSet(port_SCK_PORT, port_SCK_BIT, 1)
#define     SCK_LOW()              (LPC_GPIO0->FIOPIN=(LPC_GPIO0->FIOPIN & (~(1 << port_SCK_BIT))))
#define     SCK_HIGH()             (LPC_GPIO0->FIOPIN=((LPC_GPIO0->FIOPIN & (~(1 << port_SCK_BIT))) | (1 << port_SCK_BIT)))

#define     MISO_PORT_INIT()       BSP_GPIODirectionSet(port_MISO_PORT, port_MISO_BIT, DIR_INPUT, AUTO_RES)

#define     MOSI_PORT_INIT()       BSP_GPIODirectionSet(port_MOSI_PORT, port_MOSI_BIT, DIR_OUTPUT, PULL_UP_RES)
#define     MOSI_HIGH()            BSP_GPIOValueSet(port_MOSI_PORT, port_MOSI_BIT, 1)
#define     MOSI_LOW()             BSP_GPIOValueSet(port_MOSI_PORT, port_MOSI_BIT, 0)

#define     CS_PORT_INIT()         BSP_GPIODirectionSet(port_CS_PORT, port_CS_BIT, DIR_OUTPUT, PULL_UP_RES)
//#define     CS_LOW()               BSP_GPIOValueSet(port_CS_PORT, port_CS_BIT, 0)
//#define     CS_HIGH()              BSP_GPIOValueSet(port_CS_PORT, port_CS_BIT, 1)
#define     CS_LOW()              (LPC_GPIO0->FIOPIN=(LPC_GPIO0->FIOPIN & (~(1 << port_CS_BIT))))   // 端口引脚值寄存器,将P0.16引脚设为低电平输出
#define     CS_HIGH()             (LPC_GPIO0->FIOPIN=((LPC_GPIO0->FIOPIN & (~(1 << port_CS_BIT))) | (1 << port_CS_BIT))) // 端口引脚寄存器   1:输出高电平                        

//#define     SYNC_PORT_INIT()      BSP_GPIODirectionSet(port_SYNC_PORT, port_SYNC_BIT, DIR_OUTPUT, PULL_UP_RES)
//#define     SYNC_LOW()            (LPC_GPIO1->FIOPIN=(LPC_GPIO1->FIOPIN & (~(1 << port_SYNC_BIT))))//BSP_GPIOValueSet(port_SYNC_PORT, port_SYNC_BIT, 0)
//#define     SYNC_HIGH()           (LPC_GPIO1->FIOPIN=((LPC_GPIO1->FIOPIN & (~(1 << port_SYNC_BIT))) | (1 << port_SYNC_BIT)))//BSP_GPIOValueSet(port_SYNC_PORT, port_SYNC_BIT, 1)

// 对电压取样时低电平 电流取样时高
#define     port_MEAS_V_L_PORT      (PORT0)
#define     port_MEAS_V_L_BIT       (0)

// 对电流取样时低电平 电压取样时高
#define    port_MEAS_I_L_PORT       (PORT1)
#define    port_MEAS_I_L_BIT        (29)







/* 电压电流取样切换控制信号宏定义 */
#define     MEAS_V_L_HIGH()        BSP_GPIOValueSet(port_MEAS_V_L_PORT, port_MEAS_V_L_BIT, 1)
#define     MEAS_V_L_LOW()         BSP_GPIOValueSet(port_MEAS_V_L_PORT, port_MEAS_V_L_BIT, 0)
#define     MEAS_I_L_HIGH()        BSP_GPIOValueSet(port_MEAS_I_L_PORT, port_MEAS_I_L_BIT, 1)
#define     MEAS_I_L_LOW()         BSP_GPIOValueSet(port_MEAS_I_L_PORT, port_MEAS_I_L_BIT, 0)


/******************************************************************************
 *                       本文件所定义的全局全局变量
******************************************************************************/
     	
uint32  g_AD_ADValue[2*AD_SAMPLE_NUMBS_MAX_VAL] = {0};           // AD取样值存储单元

/******************************************************************************
 *                       本文件所定义的静态全局变量   
******************************************************************************/

static portuBASE_TYPE   s_ADValCnt                   = 0;        // AD取样值个数计数变量
//static volatile uint8   s_ADSampleForListTestFlg     = TRUE;     // 列表测试使能标志
static volatile uint8   s_ADSampleCompletedFlg       = FALSE;    // AD取样完成标志(为快速处理服务)

static uint16   s_InternalADVal[5]   = {0};




/**
 *@Description: 模拟SPI接口发送一帧数据
 *@paramreq- 
 *@paramind- 
 *@return 
 */
uint32 SPIGetData(void)
{
    uint32 adValue = 0;
    portuBASE_TYPE   i;

    portNOP();
    CS_LOW(); // P0.16输出低电平
	
    //for (i = 0; i < AD_BIT_NUMBS; i++)
    // 现修改为不读取最后5位 因为读取到之后也要舍弃掉 不读取则可以节省时间
    for(i = 0; i < (AD_BIT_NUMBS-5); i++)  // AD_BIT_NUMBS:32
    {
			adValue <<= 1;

			// 此处设置取样速度(手册14页表3)，现在5位全0，速度880Hz
			if(i < 5)
			{
				//MOSI_LOW();
        LPC_GPIO0->FIOPIN = (LPC_GPIO0->FIOPIN & (~(1 << 18))); // P0.18设为低电平输出
      }
//    else if (i == 4)
//    {
//      //MOSI_HIGH();
//       LPC_GPIO0->FIOPIN = (LPC_GPIO0->FIOPIN & (~(1 << 18))) | (1 << 18);
//    }
			SCK_HIGH();
      portNOP();
      portNOP();
      portNOP();
      portNOP();
      portNOP();
      portNOP();
      portNOP();
      portNOP();
      portNOP();
      portNOP();
      portNOP();
      portNOP();
      portNOP();
      portNOP();
      portNOP();
      portNOP();

      if(1 == (((LPC_GPIO0->FIOPIN & (1 << port_MISO_BIT)) == 0)? (0):(1)))   // P0.17 为高电平       
      {
				adValue |= 0x01;     // FIOPIN:端口引脚值寄存器(R/W)   0:低电平   1:高电平
      }

        SCK_LOW();
        portNOP();
        portNOP();
        portNOP();
        portNOP();
        portNOP();
        portNOP();
        portNOP();
        portNOP();
        portNOP();
        portNOP();
        portNOP();
        portNOP();
        portNOP();
        portNOP();
        portNOP();
        portNOP();
    }

    portNOP();
    CS_HIGH();
    
    if(adValue >= (0x30000000>>5))
    {
			adValue = (0x30000000>>5) - 1;
    }
    else if(adValue < (0x10000000>>5))
    {
			adValue = 0x10000000>>5;
    }
		return adValue;                                                                    
} 






/**
 *@Description: 片上 ADC中断服务函数  用于温度检测  然后控制风扇转速
 *@paramreq- 
 *@paramind- 
 *@return 
 */
void ADC_IRQHandler(void)
{
	static  uint32  times  = 0;
  portuBASE_TYPE  adVal  = 0;

	// ADDR5:A/D通道5数据寄存器.通道5上完成的最近一次转换的结果  15:4 为有效为. 其他位保留,不应向其写入1
  adVal = ((LPC_ADC->DR[5] & INNER_AD_VAL_GET_MASK_CODE) >> 4);   // 0x0000FFF0:得DR[5]的 15:4 位的值
  times++;
  times %= 900;
	
	TemperatureSet(adVal);
	if(times != 0)
  {
		// CR:A/D控制寄存器  7:0=>选择采样和转换输入脚  15:8=>PCLK时钟分频值(+1)  
		LPC_ADC->CR |= (1 << 24);    // 重新启动内部AD转换  26:24=>001=>立即启动转换
  }
	
}




/**
 *@Description: 外部中断2    AD转换完成时触发单片机外部中断  外部中断函数读取AD值
 *@paramreq- 
 *@paramind- 
 *@return 
 */
void EINT2_IRQHandler(void)
{
    portuBASE_TYPE  statueReg  = 0;
    portuBASE_TYPE  value      = 0;
 
    LPC_SC->EXTINT |= (1 << 2);  // EXTINT:外部中断标志寄存器  0:EINT0  1:EINT1 .. 3:EINT3 ,通过写入1来清零   清除中断标志 

    // 控制电压电流交替采样  MEAS_I_L=>P1.29   MEAS_V_L=>P0.0
    if((s_ADValCnt % 2) == 0)
    {
      MEAS_I_L_LOW();            // 关闭电压取样 切换到电流取样
      MEAS_V_L_HIGH();           // MEAS_I_L,MEAS_V_L连接ADG441,由ADG441英文手册(第五页Table4,第六页Table5)知=>1(IN):ON   0(IN):OFF
    }
    else
    {
      MEAS_V_L_LOW();  // 关闭电流取样 切换到电压取样
      MEAS_I_L_HIGH();
      // 关闭AD 测试调度程序中打开AD 现在不关闭自身了
      //NVIC_DisableIRQ(EINT2_IRQn);
    }

    SoftDelayUs(500);

#if (SPI_INTERFACE_SOFTWARE > 0)
    // 软件模拟SPI口读取AD数据 数组中前半部分为电压值 后半部分为电流值
    //s_ADValue[s_adValCnt++]   = (SPIGetData()&0x1FFFFFE0)>>5;
    // 读取时最后五位没有读取
    g_AD_ADValue[s_ADValCnt++]  = (SPIGetData()&0x00FFFFFF);
#else 
		
    LPC_SPI->SPDR = 0;                  // SPDR:SPI数据寄存器  SPI接收的数据可用从该寄存器读出    发送数据期间不能写  7:0=>SPI双向数据端口
    
		value = LPC_SPI->SPDR;              // 15:8=>如果SPCR的位2为1且位11:8不是1000,那么这些位的部分或全部含有其他的发送和接收位.当选择少于16位时,这些位中较高的位读为0                                                                                    
    
		while(!(0x01 & LPC_SPI->SPINT));    // SPINT:SPI中断寄存器  0:SPI中断标志.向该位写入1清零
    
		statueReg = LPC_SPI->SPSR;          // 中断状态寄存器,读时清零   3=>为1时从机终止 4=>模式错误 5=>读溢出 6=>写冲突 7=>SPI传输完成标志
    
		LPC_SPI->SPINT = 0x01;              // 清零中断

		// 主机模式时写SPDR将启动SPI数据传输
    LPC_SPI->SPDR  = 0;                 // SPI数据寄存器  发送数据通过将数据写入该寄存器实现,读为接收数据
    adValue       %= 66;
    while(!(0x01 & LPC_SPI->SPINT));    // SPI中断时SPINT=0 , 跳出循环
    statueReg          = LPC_SPI->SPSR; // 中断状态寄存器,读时清零 
    LPC_SPI->SPINT     = 0x01;          // SPINT:SPI中断寄存器  0:SPI中断标志.向该位写入1清零
#endif
    
    // AD取样达到规定的数据组数(一个电压和一个电流值构成一组数据) 这组数据平均值用于计算显示值
    if(s_ADValCnt == 2*AD_SAMPLE_NUMBS_MAX_VAL)
    {
        // 关闭AD 测试调度程序中打开AD(实际上这里并没有关闭，只是下次AD转化完毕不会进该中断读取数据了，因为这里关闭了中断)
        NVIC_DisableIRQ(EINT2_IRQn);
        s_ADValCnt = 0;
			  s_ADSampleCompletedFlg = TRUE;
    }
}




/**
 *@Description: SPI中断服务函数
 *@paramreq- 
 *@paramind- 
 *@return 
 */
void SPI_IRQHandler(void)
{
	LPC_SPI->SPINT |= 0x01;          // SPINT:SPI中断寄存器,SPI中断标志,由SPI接口置位以产生中断.向该位写入1清零
}





/**
 *@Description: 
 *@paramreq- 
 *@paramind- 
 *@return 
 */
uint8 LTC2440_Open(void)
{
#if (AD7766_DEBUG > 0)
    portuBASE_TYPE       statueReg;
#endif
	// 无论软件模拟还是硬件SPI都需要配置的引脚
  CS_PORT_INIT();                  // 配置端口模式    GPIO P0.16  正常模式  输入引脚
  CS_HIGH();                       // 输出高电平
	
// 软件仿真SPI口
#if (SPI_INTERFACE_SOFTWARE > 0) 
    SCK_PORT_INIT();   // 
    MISO_PORT_INIT();
    MOSI_PORT_INIT();
    MOSI_HIGH();
    SCK_LOW();

// 使用硬件SPI口
#else
// 端口选择为SPI口功能 没有选择SSEL和MOSI
    LPC_PINCON->PINSEL0   = (LPC_PINCON->PINSEL0 & 0x3FFFFFFF)|(0xC0000000);
    LPC_PINCON->PINSEL1   = (LPC_PINCON->PINSEL1 & 0xFFFFFFF3)|(0x0000000C);

    // 功率控制寄存器中复位状态下SPI使能 外设时钟分频寄存器为1/4分频
    // (1 << 6)--高位在先   (1 << 5)--主机模式    (1 << 3)--上升沿出数据下降沿读取数据   12位数据传输格式
    LPC_SC->PCLKSEL0   = (LPC_SC->PCLKSEL0 & ~(2 << 16))|(2 << 16);
    LPC_SPI->SPCCR     = 8;
    LPC_SPI->SPCR     |= (1 << 5)|(1 << 2)|(1 << 7)|(0x0C << 8);//|(1 << 3)
       
    //NVIC_SetPriority(SPI_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
    //NVIC_EnableIRQ(SPI_IRQn);
#endif

    // 选择外部中断2功能————AD转化完了会在AD芯片的某个管脚产生下降沿 故用其触发单片机中断提醒单片机一次转换完成
    LPC_PINCON->PINSEL4     = (LPC_PINCON->PINSEL4 & 0xFCFFFFFF)|(0x01000000);
    // 外部中断2边沿触发 下降沿触发
    LPC_SC->EXTMODE        |= (1 << 2);
    LPC_SC->EXTPOLAR       &= (~(1 << 2));
    NVIC_SetPriority(EINT2_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
    NVIC_EnableIRQ(EINT2_IRQn);
  #if (AD7766_DEBUG > 0)
    
    while(1)
    {
        adValue %= 100;
        LPC_SPI->SPDR       = 0;
        aaaaa[adValue++]    = LPC_SPI->SPDR;
        LPC_SPI->SPDR       = 0;
        while(!(0x01 & LPC_SPI->SPINT));
        statueReg           = LPC_SPI->SPSR;
        LPC_SPI->SPINT      = 0x01;   
    }
#endif

    // 此处初始化单片机片内AD 用于温度采集控制风扇转速
    // 使能AD转化(AD转换器默认状态下关闭的)
    LPC_SC->PCONP |= (1 << 12);
    // 外设时钟默认为1/4内核时钟
    // 引脚选择功能为AD模拟输入通道AD0.5
    LPC_PINCON->PINSEL3 = (LPC_PINCON->PINSEL3 & 0x3FFFFFFF)|(0xC0000000);

    // (1 << 5)选择通道5,(1 << 8)对APB时钟100分频 转换由软件控制,(1 << 21)AD转换处于正常工作模式
    LPC_ADC->CR = (1 << 5)|(99 << 8)|(1 << 21);
    LPC_ADC->INTEN = (1 << 5);                                // 使能通道5转换结束中断    
    NVIC_SetPriority(ADC_IRQn, (1<<__NVIC_PRIO_BITS)-10);
    NVIC_EnableIRQ(ADC_IRQn);
		
		

LPC_ADC->CR  |= (1 << 24);                                    // 启动单片机片内AD  要使AD进入中断这句必须有


   return RT_EOK;		
}







/**
 *@Description: 不合理设计
 *@paramreq- 
 *@paramind- 
 *@return 
 */
uint8 LTC2440_Control()
{
  s_ADValCnt  = 0;                // 重置AD取样数据组数 表明测试功能改变 清除已经取样的AD数值
  NVIC_EnableIRQ(EINT2_IRQn);
	return 0;
}



/**
 *@Description: 
 *@paramreq- 
 *@paramind- 
 *@return 
 */
uint8 GetSamFinishFlag(void)
{
	if(TRUE == s_ADSampleCompletedFlg)
	{
		s_ADSampleCompletedFlg = FALSE;
		return SUCCESS;
	}
	return FAIL;
}




/**
 *@Description: 
 *@paramreq- 
 *@paramind- 
 *@return 
 */
uint8 DEV_AD_SelfChk(void)
{
    portuBASE_TYPE  i;
    int32  voltSampleVal  = 0;
    int32  currSampleVal  = 0;

    // AD取样完成，分析数据
    if(SUCCESS == GetSamFinishFlag())
    {
        for(i = 0; i < AD_SAMPLE_NUMBS_MAX_VAL; i++)   
        {
            if((1 << 23) & g_AD_ADValue[2*i])
            {
                // 符号位为1 负码值 正极性的接入
                // 去掉符号位 去掉低4位(低8位数据一直在跳) 这样实际上就只剩19位了
                voltSampleVal -= (~((g_AD_ADValue[2*i] & 0x007FFFF0) >> 4)) & 0x0007FFFF;
            }
            else
            {
                // 符号位为0 正码值 负极性的接入(极性接反)
                voltSampleVal += (((g_AD_ADValue[2*i] & 0x007FFFF0) >> 4)) & 0x0007FFFF;
            }
    
            if((1 << 23) & g_AD_ADValue[2*i+1])
            {
                // 符号位为1 负码值 正极性的接入
                // 去掉符号位 去掉低4位(低8位数据一直在跳) 这样实际上就只剩19位了
                currSampleVal -= (~((g_AD_ADValue[2*i+1] & 0x007FFFF0) >> 4)) & 0x0007FFFF;
            }
            else
            {
                // 符号位为0 正码值 负极性的接入(极性接反)
                currSampleVal += (((g_AD_ADValue[2*i+1] & 0x007FFFF0) >> 4)) & 0x0007FFFF;
            }
        }
    
        // 码值小于零表示外部电源的输入极性为正
        if (voltSampleVal < 0)
        {
            // 把负码值转化为正值
            voltSampleVal = ~voltSampleVal + 1;
        }
        else
        {
            voltSampleVal = 0 - voltSampleVal;
        }
        voltSampleVal  /= AD_SAMPLE_NUMBS_MAX_VAL;
    
        if(currSampleVal < 0)
        {
            currSampleVal = ~currSampleVal + 1;
        }
        else
        {
            currSampleVal = 0 - currSampleVal;
        }
        currSampleVal /= AD_SAMPLE_NUMBS_MAX_VAL;
    }

    // 根据取样到的电压和电流的AD值判断AD是否工作正常
    if(((0 == voltSampleVal)&&(0 == currSampleVal))
      ||((voltSampleVal > 174762)&&(currSampleVal > 174762))
      ||((voltSampleVal < -174762)&&(currSampleVal < -174762)))
    {
			;
    }
 
    // 片内AD监测，实际上是监测温度传感器是否连接好
    voltSampleVal  = 0;
    for(i = 0; i < 5; i++)
    {
			voltSampleVal  += s_InternalADVal[i];
    }
    voltSampleVal    /= 5;
    // 判断片内AD的取样值，以此判断温度传感器是否连接好
    if(voltSampleVal < 30)
    {
			;
    }
    NVIC_EnableIRQ(EINT2_IRQn);    // 重新启动下次AD转化过程
		return 1;
}









/******************************************************************************
 *                             END  OF  FILE                                   
******************************************************************************/
