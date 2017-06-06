#include "AD.h"
#include "lpc17xx.h"
#include "delay.h"
#include "bsp_gpio.h"
#include "test.h"






// ���ģ��SPI�ӿں궨��
#define   SPI_INTERFACE_SOFTWARE              (1)
#define   AD7766_DEBUG                        (0)
#define   INNER_AD_VAL_GET_MASK_CODE          (0x0000FFF0)



#define   port_SCK_PORT          (PORT0)  // SCK=>˫������ʱ������
#define   port_SCK_BIT           (15)

#define   port_MISO_PORT         (PORT0)  // SDO=>��̬�������
#define   port_MISO_BIT          (17)

#define   port_MOSI_PORT         (PORT0)  // SDI=>������������
#define   port_MOSI_BIT          (18)

#define   AD_BIT_NUMBS           (32)     // LTC2440��������������ĳ���Ϊ32λ,ǰ3λ����״̬��Ϣ...(�����ֲ�11ҳ)

#define   port_CS_PORT           (PORT0)  // CS����=>��̬��Ч��������. �͵�ƽ:ʹ��SDO�������������ADC, �ߵ�ƽ:�͹���  ��ת��:�ж�����ת��,����һ���µı任
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
#define     CS_LOW()              (LPC_GPIO0->FIOPIN=(LPC_GPIO0->FIOPIN & (~(1 << port_CS_BIT))))   // �˿�����ֵ�Ĵ���,��P0.16������Ϊ�͵�ƽ���
#define     CS_HIGH()             (LPC_GPIO0->FIOPIN=((LPC_GPIO0->FIOPIN & (~(1 << port_CS_BIT))) | (1 << port_CS_BIT))) // �˿����żĴ���   1:����ߵ�ƽ                        

//#define     SYNC_PORT_INIT()      BSP_GPIODirectionSet(port_SYNC_PORT, port_SYNC_BIT, DIR_OUTPUT, PULL_UP_RES)
//#define     SYNC_LOW()            (LPC_GPIO1->FIOPIN=(LPC_GPIO1->FIOPIN & (~(1 << port_SYNC_BIT))))//BSP_GPIOValueSet(port_SYNC_PORT, port_SYNC_BIT, 0)
//#define     SYNC_HIGH()           (LPC_GPIO1->FIOPIN=((LPC_GPIO1->FIOPIN & (~(1 << port_SYNC_BIT))) | (1 << port_SYNC_BIT)))//BSP_GPIOValueSet(port_SYNC_PORT, port_SYNC_BIT, 1)

// �Ե�ѹȡ��ʱ�͵�ƽ ����ȡ��ʱ��
#define     port_MEAS_V_L_PORT      (PORT0)
#define     port_MEAS_V_L_BIT       (0)

// �Ե���ȡ��ʱ�͵�ƽ ��ѹȡ��ʱ��
#define    port_MEAS_I_L_PORT       (PORT1)
#define    port_MEAS_I_L_BIT        (29)







/* ��ѹ����ȡ���л������źź궨�� */
#define     MEAS_V_L_HIGH()        BSP_GPIOValueSet(port_MEAS_V_L_PORT, port_MEAS_V_L_BIT, 1)
#define     MEAS_V_L_LOW()         BSP_GPIOValueSet(port_MEAS_V_L_PORT, port_MEAS_V_L_BIT, 0)
#define     MEAS_I_L_HIGH()        BSP_GPIOValueSet(port_MEAS_I_L_PORT, port_MEAS_I_L_BIT, 1)
#define     MEAS_I_L_LOW()         BSP_GPIOValueSet(port_MEAS_I_L_PORT, port_MEAS_I_L_BIT, 0)


/******************************************************************************
 *                       ���ļ��������ȫ��ȫ�ֱ���
******************************************************************************/
     	
uint32  g_AD_ADValue[2*AD_SAMPLE_NUMBS_MAX_VAL] = {0};           // ADȡ��ֵ�洢��Ԫ

/******************************************************************************
 *                       ���ļ�������ľ�̬ȫ�ֱ���   
******************************************************************************/

static portuBASE_TYPE   s_ADValCnt                   = 0;        // ADȡ��ֵ������������
//static volatile uint8   s_ADSampleForListTestFlg     = TRUE;     // �б����ʹ�ܱ�־
static volatile uint8   s_ADSampleCompletedFlg       = FALSE;    // ADȡ����ɱ�־(Ϊ���ٴ������)

static uint16   s_InternalADVal[5]   = {0};




/**
 *@Description: ģ��SPI�ӿڷ���һ֡����
 *@paramreq- 
 *@paramind- 
 *@return 
 */
uint32 SPIGetData(void)
{
    uint32 adValue = 0;
    portuBASE_TYPE   i;

    portNOP();
    CS_LOW(); // P0.16����͵�ƽ
	
    //for (i = 0; i < AD_BIT_NUMBS; i++)
    // ���޸�Ϊ����ȡ���5λ ��Ϊ��ȡ��֮��ҲҪ������ ����ȡ����Խ�ʡʱ��
    for(i = 0; i < (AD_BIT_NUMBS-5); i++)  // AD_BIT_NUMBS:32
    {
			adValue <<= 1;

			// �˴�����ȡ���ٶ�(�ֲ�14ҳ��3)������5λȫ0���ٶ�880Hz
			if(i < 5)
			{
				//MOSI_LOW();
        LPC_GPIO0->FIOPIN = (LPC_GPIO0->FIOPIN & (~(1 << 18))); // P0.18��Ϊ�͵�ƽ���
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

      if(1 == (((LPC_GPIO0->FIOPIN & (1 << port_MISO_BIT)) == 0)? (0):(1)))   // P0.17 Ϊ�ߵ�ƽ       
      {
				adValue |= 0x01;     // FIOPIN:�˿�����ֵ�Ĵ���(R/W)   0:�͵�ƽ   1:�ߵ�ƽ
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
 *@Description: Ƭ�� ADC�жϷ�����  �����¶ȼ��  Ȼ����Ʒ���ת��
 *@paramreq- 
 *@paramind- 
 *@return 
 */
void ADC_IRQHandler(void)
{
	static  uint32  times  = 0;
  portuBASE_TYPE  adVal  = 0;

	// ADDR5:A/Dͨ��5���ݼĴ���.ͨ��5����ɵ����һ��ת���Ľ��  15:4 Ϊ��ЧΪ. ����λ����,��Ӧ����д��1
  adVal = ((LPC_ADC->DR[5] & INNER_AD_VAL_GET_MASK_CODE) >> 4);   // 0x0000FFF0:��DR[5]�� 15:4 λ��ֵ
  times++;
  times %= 900;
	
	TemperatureSet(adVal);
	if(times != 0)
  {
		// CR:A/D���ƼĴ���  7:0=>ѡ�������ת�������  15:8=>PCLKʱ�ӷ�Ƶֵ(+1)  
		LPC_ADC->CR |= (1 << 24);    // ���������ڲ�ADת��  26:24=>001=>��������ת��
  }
	
}




/**
 *@Description: �ⲿ�ж�2    ADת�����ʱ������Ƭ���ⲿ�ж�  �ⲿ�жϺ�����ȡADֵ
 *@paramreq- 
 *@paramind- 
 *@return 
 */
void EINT2_IRQHandler(void)
{
    portuBASE_TYPE  statueReg  = 0;
    portuBASE_TYPE  value      = 0;
 
    LPC_SC->EXTINT |= (1 << 2);  // EXTINT:�ⲿ�жϱ�־�Ĵ���  0:EINT0  1:EINT1 .. 3:EINT3 ,ͨ��д��1������   ����жϱ�־ 

    // ���Ƶ�ѹ�����������  MEAS_I_L=>P1.29   MEAS_V_L=>P0.0
    if((s_ADValCnt % 2) == 0)
    {
      MEAS_I_L_LOW();            // �رյ�ѹȡ�� �л�������ȡ��
      MEAS_V_L_HIGH();           // MEAS_I_L,MEAS_V_L����ADG441,��ADG441Ӣ���ֲ�(����ҳTable4,����ҳTable5)֪=>1(IN):ON   0(IN):OFF
    }
    else
    {
      MEAS_V_L_LOW();  // �رյ���ȡ�� �л�����ѹȡ��
      MEAS_I_L_HIGH();
      // �ر�AD ���Ե��ȳ����д�AD ���ڲ��ر�������
      //NVIC_DisableIRQ(EINT2_IRQn);
    }

    SoftDelayUs(500);

#if (SPI_INTERFACE_SOFTWARE > 0)
    // ���ģ��SPI�ڶ�ȡAD���� ������ǰ�벿��Ϊ��ѹֵ ��벿��Ϊ����ֵ
    //s_ADValue[s_adValCnt++]   = (SPIGetData()&0x1FFFFFE0)>>5;
    // ��ȡʱ�����λû�ж�ȡ
    g_AD_ADValue[s_ADValCnt++]  = (SPIGetData()&0x00FFFFFF);
#else 
		
    LPC_SPI->SPDR = 0;                  // SPDR:SPI���ݼĴ���  SPI���յ����ݿ��ôӸüĴ�������    ���������ڼ䲻��д  7:0=>SPI˫�����ݶ˿�
    
		value = LPC_SPI->SPDR;              // 15:8=>���SPCR��λ2Ϊ1��λ11:8����1000,��ô��Щλ�Ĳ��ֻ�ȫ�����������ķ��ͺͽ���λ.��ѡ������16λʱ,��Щλ�нϸߵ�λ��Ϊ0                                                                                    
    
		while(!(0x01 & LPC_SPI->SPINT));    // SPINT:SPI�жϼĴ���  0:SPI�жϱ�־.���λд��1����
    
		statueReg = LPC_SPI->SPSR;          // �ж�״̬�Ĵ���,��ʱ����   3=>Ϊ1ʱ�ӻ���ֹ 4=>ģʽ���� 5=>����� 6=>д��ͻ 7=>SPI������ɱ�־
    
		LPC_SPI->SPINT = 0x01;              // �����ж�

		// ����ģʽʱдSPDR������SPI���ݴ���
    LPC_SPI->SPDR  = 0;                 // SPI���ݼĴ���  ��������ͨ��������д��üĴ���ʵ��,��Ϊ��������
    adValue       %= 66;
    while(!(0x01 & LPC_SPI->SPINT));    // SPI�ж�ʱSPINT=0 , ����ѭ��
    statueReg          = LPC_SPI->SPSR; // �ж�״̬�Ĵ���,��ʱ���� 
    LPC_SPI->SPINT     = 0x01;          // SPINT:SPI�жϼĴ���  0:SPI�жϱ�־.���λд��1����
#endif
    
    // ADȡ���ﵽ�涨����������(һ����ѹ��һ������ֵ����һ������) ��������ƽ��ֵ���ڼ�����ʾֵ
    if(s_ADValCnt == 2*AD_SAMPLE_NUMBS_MAX_VAL)
    {
        // �ر�AD ���Ե��ȳ����д�AD(ʵ�������ﲢû�йرգ�ֻ���´�ADת����ϲ�������ж϶�ȡ�����ˣ���Ϊ����ر����ж�)
        NVIC_DisableIRQ(EINT2_IRQn);
        s_ADValCnt = 0;
			  s_ADSampleCompletedFlg = TRUE;
    }
}




/**
 *@Description: SPI�жϷ�����
 *@paramreq- 
 *@paramind- 
 *@return 
 */
void SPI_IRQHandler(void)
{
	LPC_SPI->SPINT |= 0x01;          // SPINT:SPI�жϼĴ���,SPI�жϱ�־,��SPI�ӿ���λ�Բ����ж�.���λд��1����
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
	// �������ģ�⻹��Ӳ��SPI����Ҫ���õ�����
  CS_PORT_INIT();                  // ���ö˿�ģʽ    GPIO P0.16  ����ģʽ  ��������
  CS_HIGH();                       // ����ߵ�ƽ
	
// �������SPI��
#if (SPI_INTERFACE_SOFTWARE > 0) 
    SCK_PORT_INIT();   // 
    MISO_PORT_INIT();
    MOSI_PORT_INIT();
    MOSI_HIGH();
    SCK_LOW();

// ʹ��Ӳ��SPI��
#else
// �˿�ѡ��ΪSPI�ڹ��� û��ѡ��SSEL��MOSI
    LPC_PINCON->PINSEL0   = (LPC_PINCON->PINSEL0 & 0x3FFFFFFF)|(0xC0000000);
    LPC_PINCON->PINSEL1   = (LPC_PINCON->PINSEL1 & 0xFFFFFFF3)|(0x0000000C);

    // ���ʿ��ƼĴ����и�λ״̬��SPIʹ�� ����ʱ�ӷ�Ƶ�Ĵ���Ϊ1/4��Ƶ
    // (1 << 6)--��λ����   (1 << 5)--����ģʽ    (1 << 3)--�����س������½��ض�ȡ����   12λ���ݴ����ʽ
    LPC_SC->PCLKSEL0   = (LPC_SC->PCLKSEL0 & ~(2 << 16))|(2 << 16);
    LPC_SPI->SPCCR     = 8;
    LPC_SPI->SPCR     |= (1 << 5)|(1 << 2)|(1 << 7)|(0x0C << 8);//|(1 << 3)
       
    //NVIC_SetPriority(SPI_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
    //NVIC_EnableIRQ(SPI_IRQn);
#endif

    // ѡ���ⲿ�ж�2���ܡ�������ADת�����˻���ADоƬ��ĳ���ܽŲ����½��� �����䴥����Ƭ���ж����ѵ�Ƭ��һ��ת�����
    LPC_PINCON->PINSEL4     = (LPC_PINCON->PINSEL4 & 0xFCFFFFFF)|(0x01000000);
    // �ⲿ�ж�2���ش��� �½��ش���
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

    // �˴���ʼ����Ƭ��Ƭ��AD �����¶Ȳɼ����Ʒ���ת��
    // ʹ��ADת��(ADת����Ĭ��״̬�¹رյ�)
    LPC_SC->PCONP |= (1 << 12);
    // ����ʱ��Ĭ��Ϊ1/4�ں�ʱ��
    // ����ѡ����ΪADģ������ͨ��AD0.5
    LPC_PINCON->PINSEL3 = (LPC_PINCON->PINSEL3 & 0x3FFFFFFF)|(0xC0000000);

    // (1 << 5)ѡ��ͨ��5,(1 << 8)��APBʱ��100��Ƶ ת�����������,(1 << 21)ADת��������������ģʽ
    LPC_ADC->CR = (1 << 5)|(99 << 8)|(1 << 21);
    LPC_ADC->INTEN = (1 << 5);                                // ʹ��ͨ��5ת�������ж�    
    NVIC_SetPriority(ADC_IRQn, (1<<__NVIC_PRIO_BITS)-10);
    NVIC_EnableIRQ(ADC_IRQn);
		
		

LPC_ADC->CR  |= (1 << 24);                                    // ������Ƭ��Ƭ��AD  ҪʹAD�����ж���������


   return RT_EOK;		
}







/**
 *@Description: ���������
 *@paramreq- 
 *@paramind- 
 *@return 
 */
uint8 LTC2440_Control()
{
  s_ADValCnt  = 0;                // ����ADȡ���������� �������Թ��ܸı� ����Ѿ�ȡ����AD��ֵ
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

    // ADȡ����ɣ���������
    if(SUCCESS == GetSamFinishFlag())
    {
        for(i = 0; i < AD_SAMPLE_NUMBS_MAX_VAL; i++)   
        {
            if((1 << 23) & g_AD_ADValue[2*i])
            {
                // ����λΪ1 ����ֵ �����ԵĽ���
                // ȥ������λ ȥ����4λ(��8λ����һֱ����) ����ʵ���Ͼ�ֻʣ19λ��
                voltSampleVal -= (~((g_AD_ADValue[2*i] & 0x007FFFF0) >> 4)) & 0x0007FFFF;
            }
            else
            {
                // ����λΪ0 ����ֵ �����ԵĽ���(���Խӷ�)
                voltSampleVal += (((g_AD_ADValue[2*i] & 0x007FFFF0) >> 4)) & 0x0007FFFF;
            }
    
            if((1 << 23) & g_AD_ADValue[2*i+1])
            {
                // ����λΪ1 ����ֵ �����ԵĽ���
                // ȥ������λ ȥ����4λ(��8λ����һֱ����) ����ʵ���Ͼ�ֻʣ19λ��
                currSampleVal -= (~((g_AD_ADValue[2*i+1] & 0x007FFFF0) >> 4)) & 0x0007FFFF;
            }
            else
            {
                // ����λΪ0 ����ֵ �����ԵĽ���(���Խӷ�)
                currSampleVal += (((g_AD_ADValue[2*i+1] & 0x007FFFF0) >> 4)) & 0x0007FFFF;
            }
        }
    
        // ��ֵС�����ʾ�ⲿ��Դ�����뼫��Ϊ��
        if (voltSampleVal < 0)
        {
            // �Ѹ���ֵת��Ϊ��ֵ
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

    // ����ȡ�����ĵ�ѹ�͵�����ADֵ�ж�AD�Ƿ�������
    if(((0 == voltSampleVal)&&(0 == currSampleVal))
      ||((voltSampleVal > 174762)&&(currSampleVal > 174762))
      ||((voltSampleVal < -174762)&&(currSampleVal < -174762)))
    {
			;
    }
 
    // Ƭ��AD��⣬ʵ�����Ǽ���¶ȴ������Ƿ����Ӻ�
    voltSampleVal  = 0;
    for(i = 0; i < 5; i++)
    {
			voltSampleVal  += s_InternalADVal[i];
    }
    voltSampleVal    /= 5;
    // �ж�Ƭ��AD��ȡ��ֵ���Դ��ж��¶ȴ������Ƿ����Ӻ�
    if(voltSampleVal < 30)
    {
			;
    }
    NVIC_EnableIRQ(EINT2_IRQn);    // ���������´�ADת������
		return 1;
}









/******************************************************************************
 *                             END  OF  FILE                                   
******************************************************************************/
