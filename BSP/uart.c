/**
*@file	���� uart.h	
*@brief			uart0 ������
*@version		
*@author		
*@date		
*/
#include "LPC17xx.h"
#include "uart.h"
#include "bsp_gpio.h"




#define LPC_UART		    LPC_UART0
#define UART_IRQn		    UART0_IRQn


#define IER_RBR		0x01
#define IER_THRE	0x02
#define IER_RLS		0x04

#define IIR_PEND	0x01
#define IIR_RLS		0x03
#define IIR_RDA		0x02
#define IIR_CTI		0x06
#define IIR_THRE	0x01

#define LSR_RDR		0x01
#define LSR_OE		0x02
#define LSR_PE		0x04
#define LSR_FE		0x08
#define LSR_BI		0x10
#define LSR_THRE	0x20
#define LSR_TEMT	0x40
#define LSR_RXFE	0x80

#define  port_RS485_RW_CTRL_PORT         (PORT1)
#define  port_RS485_RW_CTRL_BIT          (30)

#define  RS485_RW_CTRL_PORT_INIT()       BSP_GPIODirectionSet(port_RS485_RW_CTRL_PORT, port_RS485_RW_CTRL_BIT, DIR_OUTPUT, PULL_UP_RES)
#define  port_RS485_RX_EN()              BSP_GPIOValueSet(port_RS485_RW_CTRL_PORT, port_RS485_RW_CTRL_BIT, 0)
#define  port_RS485_TX_EN()              BSP_GPIOValueSet(port_RS485_RW_CTRL_PORT, port_RS485_RW_CTRL_BIT, 1)





//static uint8 s_TX_flag = 0;
static uint8 s_receive = 0;
static uint8 s_send = 0;
static uint8 s_buffer[UART_TX_BUFFER_SIZE] = {0};

void UART0_Init(void);

/**
 *@Description: UART0 �ж�(ԭ�����)    ��������
 *@paramreq- 
 *@paramind- 
 *@return ��
 */
/*
void UART0_IRQHandler(void)
{
	uint32 iir;
	uint32 i;
  
	iir = LPC_UART->IIR;  // IIR:�жϱ�ʶ�Ĵ���,ʶ��ȴ�������ж�(ֻ��)
	iir >>= 1;			  
	iir &= 0x07;			// ��ȡ�жϱ�ʶλ   check bit 1~3, interrupt identification 

  // �������ݴﵽ������Ȳ����ж�--������������Ч
	if(IIR_RDA == iir)	    // 0x02=>010 �������ݿ���(RDA)   Receive Data Available 
	{
		if(s_receive >= UART_TX_BUFFER_SIZE)
		{
			s_receive = 0;
		}
		s_buffer[s_receive] = LPC_UART->RBR;
		
		s_receive++;
	}
	// ��ʱ�ж�
	else if(IIR_CTI == iir)
  {
			// �жϽ��ջ���Ĵ������Ƿ�������
      if (LPC_UART->LSR & 0x01)
      {
				if(s_receive >= UART_TX_BUFFER_SIZE)
				{
					s_receive = 0;
        }
			  s_buffer[s_receive] = LPC_UART->RBR;
//				LPC_UART->THR = s_buffer[s_receive];
				s_receive++;;
      }
  }
	if(IIR_RLS == iir)
  {
		iir = LPC_UART->LSR;
    portNOP();
  }
}
*/




/**
 *@Description: UART0 �ж�(�̳̰�) �ܽ�������  ÿ����6����ȷ���ݵ���7,8������
 *@paramreq- 
 *@paramind- 
 *@return ��
 */
/*
void UART0_IRQHandler(void)
{
	uint8 IIRValue;
	uint8 LSRValue;
	uint8 Dummy;
	
	IIRValue = LPC_UART0->IIR; // �жϱ�ʶ�Ĵ���
	IIRValue >>= 1;
	IIRValue &= 0x07;
	if(IIRValue == IIR_RLS) // 0x03(0011)=>001(3:1):THRE�ж�   1(0):û�й�����ж�
	{
		LSRValue = LPC_UART0->LSR;
		if(LSRValue & (LSR_OE | LSR_PE | LSR_FE | LSR_RXFE | LSR_BI))
		{
			Dummy = LPC_UART0->RBR;
			return;
		}
		if(LSRValue & LSR_RDR)
		{
			s_buffer[s_receive] = LPC_UART0->RBR;
			s_receive++;
			if(s_receive == UART_TX_BUFFER_SIZE)
			{
				s_receive = 0;
			}
		}
	}
	else if(IIRValue == IIR_RDA) // 0x02(0010)==> 001(3:1):THRE�ж�   0(0):������һ��������ж�
	{
		if(s_receive == UART_TX_BUFFER_SIZE)
		{
			s_receive = 0;
		}
		s_buffer[s_receive] = LPC_UART0->RBR;
		s_receive++;
	}
	else if(IIRValue == IIR_CTI) // 0x06(0110)=>011(3:1):������״̬   1(0):û�й�����ж�
	{
//		UART0Status |= 0x100;
		if(s_receive == UART_TX_BUFFER_SIZE)
		{
			s_receive = 0;
		}
		s_buffer[s_receive] = LPC_UART0->RBR;
		s_receive++;
	}
	else if(IIRValue == IIR_THRE) // 0x01(0001)=>000(3:1):Modem�ж�   1(0):����һ��������ж�
	{
		LSRValue = LPC_UART0->LSR;
	}
}
*/




void UART0_IRQHandler(void)
{
	uint8 IIRValue;
	uint8 LSRValue;
	uint8 RBRValue;
	
	
	IIRValue = LPC_UART0->IIR;          // �жϱ�ʶ�Ĵ���
	IIRValue &= 0x0F;                   // ��ȡIIR�Ĵ����ĵ�4λ
	if(IIRValue == 0x06)                // 0110:RX��״̬/����  011:������״̬   �жϸ�λ: UnLSR ������
	{
		s_buffer[0] = LPC_UART0->RBR;
		LSRValue = LPC_UART0->LSR;        //LSR:��״̬�Ĵ���     ��LSR ����ж�
	}
	else if(IIRValue == 0x04)           // 0100:RX���ݿ���  �жϸ�λ: UnLSR ������ �� UARTn FIFO���ڴ���ֵ
	{
		LSRValue = LPC_UART0->LSR;        // ��LSR ����ж�
		if(s_receive >= UART_TX_BUFFER_SIZE)
			s_receive = 0;
		s_buffer[s_receive] = LPC_UART0->RBR;
		s_receive++;
	}
	else if(IIRValue == 0x0C)           // �ַ���ʱָʾ     �жϸ�λ: UnRBR ������
	{
		s_buffer[0] = LPC_UART0->RBR;
		RBRValue = LPC_UART0->RBR;        // ��RBR ����ж�
	}
	else if(IIRValue == 0x02)           // THRE      �жϸ�λ: UnIIR������(���UnIIR���ж�Դ)��THRд����
	{
		IIRValue = LPC_UART0->IIR;        // ��IIR�жϸ�λ
//		while(LPC_UART0->LCR & 0x80);   // ������������ֹ����������ѭ��
//		LPC_UART0->THR &= 0x00;         // д THR ��λ�ж�   ���ﲻ�ı�THR������
	}
	else
	{
	}
}




/**
 *@Description: UART��ʼ�� ���� ʱ��Ƶ�� ������ ���ô� PCONP ����Ϊϵͳ��ʼ�����Ѿ���
 *@paramreq- 
 *@paramind- 
 *@return 
 */
uint8 UartInit(void)
{
	uint32 Fdiv;
	uint32 pclkdiv;
	uint32 pclk;

	LPC_SC->PCONP |= 0x08; // ����UART0 ʱ��
	/* Init UART Hardware */
	if(LPC_UART == LPC_UART0)
	{
		LPC_PINCON->PINSEL0 &= ~0x000000F0;
		LPC_PINCON->PINSEL0 |= 0x00000050;  /* RxD0 is P0.3 and TxD0 is P0.2 */
		/* By default, the PCLKSELx value is zero, thus, the PCLK for
		all the peripherals is 1/4 of the SystemFrequency. */
		/* Bit 6~7 is for UART0 */
		pclkdiv = (LPC_SC->PCLKSEL0 >> 6) & 0x03;
		switch ( pclkdiv )
		{
		  case 0x00:
		  default:
			pclk = SystemFrequency/4;
			break;
		  case 0x01:
			pclk = SystemFrequency;
			break; 
		  case 0x02:
			pclk = SystemFrequency/2;
			break; 
		  case 0x03:
			pclk = SystemFrequency/8;
			break;
		}
		LPC_UART0->LCR = 0x83;		/* 8 bits, ��ֹ��ż������У��, 1 Stop bit , ��У�� , ��ֹ�������*/
		Fdiv = (pclk / 16) / BaudRate[BAUD_RATE_9600];	/*baud rate */
	  LPC_UART0->DLM = Fdiv / 256;							
	  LPC_UART0->DLL = Fdiv % 256;
		// �������������Ĵ����󣬹رնԳ�������Ĵ����ķ���
		LPC_UART0->LCR = 0x03;		/* DLAB = 0 */
	  // ����FIFO����8�ֽ�ʱ��������������Ч�жϣ�ʹ�ܲ���λ���պͷ��ͻ�����
    LPC_UART0->FCR = 0x87;		/* Enable and reset TX and RX FIFO. */
	}
	else if((LPC_UART1_TypeDef*)LPC_UART == LPC_UART1)
	{
		LPC_PINCON->PINSEL4 &= ~0x0000000F;
		LPC_PINCON->PINSEL4 |= 0x0000000A;	/* Enable RxD1 P2.1, TxD1 P2.0 */
		
		pclkdiv = (LPC_SC->PCLKSEL0 >> 8) & 0x03;
		switch (pclkdiv)
		{
		  case 0x00:
		  default:
			pclk = SystemFrequency/4;
			break;
		  case 0x01:
			pclk = SystemFrequency;
			break; 
		  case 0x02:
			pclk = SystemFrequency/2;
			break; 
		  case 0x03:
			pclk = SystemFrequency/8;
			break;
		}
		LPC_UART1->LCR = 0x83;		/* 8 bits, no Parity, 1 Stop bit */
		Fdiv = ( pclk / 16 ) / BaudRate[BAUD_RATE_9600] ;	/*baud rate */
	  LPC_UART1->DLM = Fdiv / 256;							
	  LPC_UART1->DLL = Fdiv % 256;
		LPC_UART1->LCR = 0x03;		/* DLAB = 0 */
	  LPC_UART1->FCR = 0x07;		/* Enable and reset TX and RX FIFO. */
	}

	/* Ensure a clean start, no data in either TX or RX FIFO. */
	while(( LPC_UART->LSR & (LSR_THRE|LSR_TEMT)) != (LSR_THRE|LSR_TEMT) );
	while( LPC_UART->LSR & LSR_RDR )
	{
		Fdiv = LPC_UART->RBR;	/* Dump data from RX FIFO */
	}
	LPC_UART->IER = IER_RBR | IER_THRE | IER_RLS;	/* Enable UART interrupt */

	return RT_EOK;
}



/**
 *@Description: ��UART�ж�
 *@paramreq- flag : 1: �����ж�   0: �ر��ж�
 *@paramind- 
 *@return 
 */
uint8 UartOnOff(uint8 flag)
{
	if(flag == 1)
	{
		NVIC_EnableIRQ(UART_IRQn);
	}
	else
	{
		NVIC_DisableIRQ(UART_IRQn);
	}
	return 1;
}



/**
 *@Description: �� UART ���յ�����
 *@paramreq- 
 *@paramind- 
 *@return 
 */
uint32 UartReceive(void* buffer, uint16 size)
{
	uint8* ptr;
	uint16 i = 0;
	
	ptr = (uint8*)buffer;
	// ���ж�
	while(size)
	{
		*ptr = s_buffer[i];
		i++;
		if(i >= UART_TX_BUFFER_SIZE)
			break;
		size--;
	}
		
	return ((uint32)ptr - (uint32)buffer);
}




/**
 *@Description: ��������
 *@paramreq-  buffer: ���͵�����      size: ���͵����ݸ���
 *@paramind- 
 *@return 
 */
uint32 Uart_Send(uint8* buffer, uint32 size)
{
	uint8 i = 0;
	while(size)
	{
		while (!(LPC_UART->LSR & LSR_THRE));	 // �ȴ�����
		/* ��Ĵ���д������ */
		LPC_UART->THR = buffer[i];
		i++;
		size--;
	}
	return 1;
}





/**
 *@Description: ѡ��ͨ��ģʽ ���ò�����
 *@paramreq-  set: 0x00=>RS485   0x01=>RS232   0x10=>USB
 *@paramind- 
 *@return 
 */
void COM_Sel(uint8 set, uint8 init, uint8 baud)
{
	if(init == 0) // �տ�ʼҪ���ö˿�
	{
		BSP_GPIODirectionSet(0, 21, DIR_OUTPUT, PULL_UP_RES);   // ���ö˿��������״̬
		BSP_GPIODirectionSet(2, 8 , DIR_OUTPUT, PULL_UP_RES);  
	}
	switch(set)
	{
		case 0x00: // RS-485
//			BSP_GPIODirectionSet(0, 21, DIR_OUTPUT, PULL_UP_RES);   // ���ö˿��������״̬
//		  BSP_GPIODirectionSet(2, 8 , DIR_OUTPUT, PULL_UP_RES);  
		  BSP_GPIOValueSet(0, 21, 0); // ����͵�ƽ
		  BSP_GPIOValueSet(2, 8 , 0);
			break;
		case 0x01: // RS-232
//			BSP_GPIODirectionSet(0, 21, DIR_OUTPUT, PULL_UP_RES);
//		  BSP_GPIODirectionSet(2, 8 , DIR_OUTPUT, PULL_UP_RES);
		  BSP_GPIOValueSet(0, 21, 1);
		  BSP_GPIOValueSet(2, 8 , 0);
			break;
		default: // USB
//			BSP_GPIODirectionSet(0, 21, DIR_OUTPUT, PULL_UP_RES);
//		  BSP_GPIODirectionSet(2, 8 , DIR_OUTPUT, PULL_UP_RES);
		  BSP_GPIOValueSet(0, 21, 0);
		  BSP_GPIOValueSet(2, 8 , 1);
	}
	
}




/**
 *@Description: ���Գ���
 *@paramreq-  
 *@paramind- 
 *@return 
 */
void RT_Test(void)
{
	RS485_RW_CTRL_PORT_INIT();  // 485��ʼ��Ϊ����״̬
   port_RS485_RX_EN();
 	COM_Sel(0,0x00,0);
//	port_RS485_TX_EN();
	
	UartInit();
//	UART0_Init();
	UartOnOff(1); // �����ж�
}




void TX_Test(void)
{
//	uint8 i = s_receive;
	uint8 j = 4;
	int k = 0;
//	uint8 str[5] = {'A','B','C','D','E'};
//	while (!(LPC_UART->LSR & LSR_THRE));	 // 0x20 : LSR�ĵ�5λΪ1 => UnTHRΪ��(��������Ч�ַ�) ����ѭ��
		/* ��Ĵ���д������ */
//	LPC_UART->THR = s_buffer[i-1];
	while(j!=0)
	{
		while(!(LPC_UART->LSR & LSR_THRE));
//		LPC_UART->THR = str[j];
		j--;
	}
	j = 4;
	for(k=0;k<65530;k++);
}





void UART0_Init(void)
{
	uint32 tmp;
	uint32 Fdiv;
	uint32 pclkdiv;
	uint32 pclk;
	
	
	LPC_SC->PCONP |= 0x08;     // ����UART0 ʱ��
	LPC_UART0->FCR = 0x07;     // 0111=> ����FIFO�������ֽ�,�������DMA����. ����FIFO,�������DMA����.�ߵ�ƽ��Ч,ʹ�ܶ�FIFO�ķ���
//	while (LPC_UART0->LSR & 1) // 0001=>RBR������Ч����    ���������Ч����
//	{
//		tmp = LPC_UART0->RBR;    // ��RBR    FIFO����������յ����ֽ�
//	}
//	while(!(LPC_UART0->LSR & 0x20)); //�ȴ�THRΪ�� 
//	LPC_UART0->TER = 0;        // ֹͣ���ݷ���
//	LPC_UART0->IER = 0;        // ��ֹRDA�ж�  ��ֹTHRE�ж� ��ֹ�����ж�
	LPC_UART0->LCR = 0x83;     // 8 bits,��ֹ��ż������У��,1 Stop bit,��У�� ,��ֹ�������
//	tmp = LPC_UART0->LSR;      // ��״̬�Ĵ���
	
	
	LPC_UART0->IER |= 0x07;  
	LPC_UART0->FCR = 0x01; 
	LPC_UART0->TER = 0x80; 
	
	
	  LPC_PINCON->PINSEL0 &= ~0x000000F0;
		LPC_PINCON->PINSEL0 |= 0x00000050;  /* RxD0 is P0.3 and TxD0 is P0.2 */
		pclkdiv = (LPC_SC->PCLKSEL0 >> 6) & 0x03;
		switch ( pclkdiv )
		{
		  case 0x00:
		  default:
			pclk = SystemFrequency/4;
			break;
		  case 0x01:
			pclk = SystemFrequency;
			break; 
		  case 0x02:
			pclk = SystemFrequency/2;
			break; 
		  case 0x03:
			pclk = SystemFrequency/8;
			break;
		}
		LPC_UART0->LCR = 0x83;		/* 8 bits, ��ֹ��ż������У��, 1 Stop bit , ��У�� , ��ֹ�������*/
		Fdiv = (pclk / 16) / BaudRate[BAUD_RATE_9600];	/*baud rate */
	  LPC_UART0->DLM = Fdiv / 256;							
	  LPC_UART0->DLL = Fdiv % 256;
		// �������������Ĵ����󣬹رնԳ�������Ĵ����ķ���
		LPC_UART0->LCR = 0x03;		/* DLAB = 0 */
	  // ����FIFO����8�ֽ�ʱ��������������Ч�жϣ�ʹ�ܲ���λ���պͷ��ͻ�����
    LPC_UART0->FCR = 0x87;		/* Enable and reset TX and RX FIFO. */
}





















