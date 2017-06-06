#include "LPC17xx.h"
#include "uart_test.h"

/*

#define UART0  LPC_UART0
#define PINCON LPC_PINCON

#define UART1  LPC_UART1

volatile uint32 UART0Status;
volatile uint8 UART0TxEmpty = 1;
volatile uint8 UART0Buffer[BUFSIZE];
static   uint32 UART0Count = 0;

volatile uint8 UART1TxEmpty = 1;


void UART0_IRQHandler(void)
{
	uint8 IIRValue;
	uint8 LSRValue;
	uint8 Dummy;
	
	IIRValue = LPC_UART0->IIR;
	IIRValue >>= 1;
	IIRValue &= 0x07;
	if(IIRValue == IIR_RLS)
	{
		LSRValue = LPC_UART0->LSR;
		if(LSRValue & (LSR_OE | LSR_PE | LSR_FE | LSR_RXFE | LSR_BI))
		{
			UART0Status = LSRValue;
			Dummy = LPC_UART0->RBR;
			return;
		}
		if(LSRValue & LSR_RDR)
		{
			UART0Buffer[UART0Count] = LPC_UART0->RBR;
			UART0Count++;
			if(UART0Count == BUFSIZE)
			{
				UART0Count = 0;
			}
		}
	}
	else if(IIRValue == IIR_RDA)
	{
		UART0Buffer[UART0Count] = LPC_UART0->RBR;
		UART0Count++;
		if(UART0Count == BUFSIZE)
		{
			UART0Count = 0;
		}
	}
	else if(IIRValue == IIR_CTI)
	{
		UART0Status |= 0x100;
	}
	else if(IIRValue == IIR_THRE)
	{
		LSRValue = LPC_UART0->LSR;
		if(LSRValue & LSR_THRE)
		{
			UART0TxEmpty = 1;
		}
		else
		{
			UART0TxEmpty = 0;
		}
	}
}




uint32 UARTInit(uint32 portNum,uint32 Baudrate)
{
	uint32 Fdiv;
	if(portNum == 0)
	{
		PINCON->PINSEL0 = 0x00000050;
		LPC_UART0->LCR = 0x83;
		Fdiv = (SystemFrequency/4/16)/Baudrate;
		LPC_UART0->DLM = Fdiv / 256;							
	  LPC_UART0->DLL = Fdiv % 256;
		LPC_UART0->LCR = 0x03;	
	  LPC_UART0->FCR = 0x87;
		
		NVIC_EnableIRQ(UART0_IRQn);
		LPC_UART0->IER = IER_RBR | IER_THRE | IER_RLS;
		return(TRUE);
	}
	else if(portNum == 1)
	{
		PINCON->PINSEL4 = 0x0000000A;
		LPC_UART1->LCR = 0x83;
		Fdiv = (SystemFrequency/4/16)/Baudrate;
		LPC_UART1->DLM = Fdiv / 256;							
	  LPC_UART1->DLL = Fdiv % 256;
		LPC_UART1->LCR = 0x03;	
	  LPC_UART1->FCR = 0x07;
		
		NVIC_EnableIRQ(UART1_IRQn);
		LPC_UART1->IER = IER_RBR | IER_THRE | IER_RLS;
		return(TRUE);
	}
	return(FALSE);
}




void UARTSend(uint32 portNum, uint8 *BufferPtr, uint32 Length)
{
	if(portNum == 0)
	{
		while(Length != 0)
		{
			while(!(UART0TxEmpty & 0x01));
			UART0->THR = *BufferPtr;
			UART0TxEmpty = 0;
			BufferPtr++;
			Length--;
		}
	}
	else
	{
		while(Length != 0)
		{
			while(!(UART1TxEmpty & 0x01));
			UART1->THR = *BufferPtr;
			UART1TxEmpty = 0;
			BufferPtr++;
			Length--;
		}
	}
	return;
}




void Test_02(void)
{
	uint8 i = UART0Count;
	UARTInit(0,9600);
	while(!(UART0TxEmpty & 0x01));
  UART0->THR = UART0Buffer[i];
}

*/





