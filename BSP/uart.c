/**
*@file	关联 uart.h	
*@brief			uart0 的驱动
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
 *@Description: UART0 中断(原程序版)    接收数据
 *@paramreq- 
 *@paramind- 
 *@return 无
 */
/*
void UART0_IRQHandler(void)
{
	uint32 iir;
	uint32 i;
  
	iir = LPC_UART->IIR;  // IIR:中断标识寄存器,识别等待处理的中断(只读)
	iir >>= 1;			  
	iir &= 0x07;			// 获取中断标识位   check bit 1~3, interrupt identification 

  // 接收数据达到触发深度产生中断--即接收数据有效
	if(IIR_RDA == iir)	    // 0x02=>010 接收数据可用(RDA)   Receive Data Available 
	{
		if(s_receive >= UART_TX_BUFFER_SIZE)
		{
			s_receive = 0;
		}
		s_buffer[s_receive] = LPC_UART->RBR;
		
		s_receive++;
	}
	// 超时中断
	else if(IIR_CTI == iir)
  {
			// 判断接收缓冲寄存器中是否有数据
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
 *@Description: UART0 中断(教程版) 能接收数据  每发送6个正确数据到第7,8个出错
 *@paramreq- 
 *@paramind- 
 *@return 无
 */
/*
void UART0_IRQHandler(void)
{
	uint8 IIRValue;
	uint8 LSRValue;
	uint8 Dummy;
	
	IIRValue = LPC_UART0->IIR; // 中断标识寄存器
	IIRValue >>= 1;
	IIRValue &= 0x07;
	if(IIRValue == IIR_RLS) // 0x03(0011)=>001(3:1):THRE中断   1(0):没有挂起的中断
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
	else if(IIRValue == IIR_RDA) // 0x02(0010)==> 001(3:1):THRE中断   0(0):至少有一个挂起的中断
	{
		if(s_receive == UART_TX_BUFFER_SIZE)
		{
			s_receive = 0;
		}
		s_buffer[s_receive] = LPC_UART0->RBR;
		s_receive++;
	}
	else if(IIRValue == IIR_CTI) // 0x06(0110)=>011(3:1):接收线状态   1(0):没有挂起的中断
	{
//		UART0Status |= 0x100;
		if(s_receive == UART_TX_BUFFER_SIZE)
		{
			s_receive = 0;
		}
		s_buffer[s_receive] = LPC_UART0->RBR;
		s_receive++;
	}
	else if(IIRValue == IIR_THRE) // 0x01(0001)=>000(3:1):Modem中断   1(0):至少一个挂起的中断
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
	
	
	IIRValue = LPC_UART0->IIR;          // 中断标识寄存器
	IIRValue &= 0x0F;                   // 获取IIR寄存器的低4位
	if(IIRValue == 0x06)                // 0110:RX线状态/错误  011:接收线状态   中断复位: UnLSR 读操作
	{
		s_buffer[0] = LPC_UART0->RBR;
		LSRValue = LPC_UART0->LSR;        //LSR:线状态寄存器     读LSR 清除中断
	}
	else if(IIRValue == 0x04)           // 0100:RX数据可用  中断复位: UnLSR 读操作 或 UARTn FIFO低于触发值
	{
		LSRValue = LPC_UART0->LSR;        // 读LSR 清除中断
		if(s_receive >= UART_TX_BUFFER_SIZE)
			s_receive = 0;
		s_buffer[s_receive] = LPC_UART0->RBR;
		s_receive++;
	}
	else if(IIRValue == 0x0C)           // 字符超时指示     中断复位: UnRBR 读操作
	{
		s_buffer[0] = LPC_UART0->RBR;
		RBRValue = LPC_UART0->RBR;        // 读RBR 清除中断
	}
	else if(IIRValue == 0x02)           // THRE      中断复位: UnIIR读操作(如果UnIIR是中断源)或THR写操作
	{
		IIRValue = LPC_UART0->IIR;        // 读IIR中断复位
//		while(LPC_UART0->LCR & 0x80);   // 除数锁存器禁止访问则跳出循环
//		LPC_UART0->THR &= 0x00;         // 写 THR 复位中断   这里不改变THR的数据
	}
	else
	{
	}
}




/**
 *@Description: UART初始化 引脚 时钟频率 波特率 不用打开 PCONP 是因为系统初始化中已经打开
 *@paramreq- 
 *@paramind- 
 *@return 
 */
uint8 UartInit(void)
{
	uint32 Fdiv;
	uint32 pclkdiv;
	uint32 pclk;

	LPC_SC->PCONP |= 0x08; // 开启UART0 时钟
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
		LPC_UART0->LCR = 0x83;		/* 8 bits, 禁止奇偶产生和校验, 1 Stop bit , 奇校验 , 禁止间隔发送*/
		Fdiv = (pclk / 16) / BaudRate[BAUD_RATE_9600];	/*baud rate */
	  LPC_UART0->DLM = Fdiv / 256;							
	  LPC_UART0->DLL = Fdiv % 256;
		// 访问完除数锁存寄存器后，关闭对除数锁存寄存器的访问
		LPC_UART0->LCR = 0x03;		/* DLAB = 0 */
	  // 接收FIFO中有8字节时产生接收数据有效中断，使能并复位接收和发送缓冲区
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
 *@Description: 打开UART中断
 *@paramreq- flag : 1: 开启中断   0: 关闭中断
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
 *@Description: 读 UART 接收的数据
 *@paramreq- 
 *@paramind- 
 *@return 
 */
uint32 UartReceive(void* buffer, uint16 size)
{
	uint8* ptr;
	uint16 i = 0;
	
	ptr = (uint8*)buffer;
	// 关中断
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
 *@Description: 发送数据
 *@paramreq-  buffer: 发送的数组      size: 发送的数据个数
 *@paramind- 
 *@return 
 */
uint32 Uart_Send(uint8* buffer, uint32 size)
{
	uint8 i = 0;
	while(size)
	{
		while (!(LPC_UART->LSR & LSR_THRE));	 // 等待空闲
		/* 向寄存器写入数据 */
		LPC_UART->THR = buffer[i];
		i++;
		size--;
	}
	return 1;
}





/**
 *@Description: 选择通信模式 配置波特率
 *@paramreq-  set: 0x00=>RS485   0x01=>RS232   0x10=>USB
 *@paramind- 
 *@return 
 */
void COM_Sel(uint8 set, uint8 init, uint8 baud)
{
	if(init == 0) // 刚开始要配置端口
	{
		BSP_GPIODirectionSet(0, 21, DIR_OUTPUT, PULL_UP_RES);   // 设置端口输入输出状态
		BSP_GPIODirectionSet(2, 8 , DIR_OUTPUT, PULL_UP_RES);  
	}
	switch(set)
	{
		case 0x00: // RS-485
//			BSP_GPIODirectionSet(0, 21, DIR_OUTPUT, PULL_UP_RES);   // 设置端口输入输出状态
//		  BSP_GPIODirectionSet(2, 8 , DIR_OUTPUT, PULL_UP_RES);  
		  BSP_GPIOValueSet(0, 21, 0); // 输出低电平
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
 *@Description: 测试程序
 *@paramreq-  
 *@paramind- 
 *@return 
 */
void RT_Test(void)
{
	RS485_RW_CTRL_PORT_INIT();  // 485初始化为接收状态
   port_RS485_RX_EN();
 	COM_Sel(0,0x00,0);
//	port_RS485_TX_EN();
	
	UartInit();
//	UART0_Init();
	UartOnOff(1); // 开启中断
}




void TX_Test(void)
{
//	uint8 i = s_receive;
	uint8 j = 4;
	int k = 0;
//	uint8 str[5] = {'A','B','C','D','E'};
//	while (!(LPC_UART->LSR & LSR_THRE));	 // 0x20 : LSR的第5位为1 => UnTHR为空(不包含有效字符) 跳出循环
		/* 向寄存器写入数据 */
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
	
	
	LPC_SC->PCONP |= 0x08;     // 开启UART0 时钟
	LPC_UART0->FCR = 0x07;     // 0111=> 清零FIFO中所有字节,清除发送DMA请求. 清零FIFO,清除接收DMA请求.高电平有效,使能对FIFO的访问
//	while (LPC_UART0->LSR & 1) // 0001=>RBR包含有效数据    如果包含有效数据
//	{
//		tmp = LPC_UART0->RBR;    // 读RBR    FIFO当中最早接收到的字节
//	}
//	while(!(LPC_UART0->LSR & 0x20)); //等待THR为空 
//	LPC_UART0->TER = 0;        // 停止数据发送
//	LPC_UART0->IER = 0;        // 禁止RDA中断  禁止THRE中断 禁止各种中断
	LPC_UART0->LCR = 0x83;     // 8 bits,禁止奇偶产生和校验,1 Stop bit,奇校验 ,禁止间隔发送
//	tmp = LPC_UART0->LSR;      // 线状态寄存器
	
	
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
		LPC_UART0->LCR = 0x83;		/* 8 bits, 禁止奇偶产生和校验, 1 Stop bit , 奇校验 , 禁止间隔发送*/
		Fdiv = (pclk / 16) / BaudRate[BAUD_RATE_9600];	/*baud rate */
	  LPC_UART0->DLM = Fdiv / 256;							
	  LPC_UART0->DLL = Fdiv % 256;
		// 访问完除数锁存寄存器后，关闭对除数锁存寄存器的访问
		LPC_UART0->LCR = 0x03;		/* DLAB = 0 */
	  // 接收FIFO中有8字节时产生接收数据有效中断，使能并复位接收和发送缓冲区
    LPC_UART0->FCR = 0x87;		/* Enable and reset TX and RX FIFO. */
}





















