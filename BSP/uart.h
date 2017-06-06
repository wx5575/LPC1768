#ifndef UART_H
#define UART_H


#include "BSP_CpuConfig.h"


#define  UART_TX_BUFFER_SIZE	    64


typedef enum
{
    BAUD_RATE_2400=0,
    BAUD_RATE_4800,
    BAUD_RATE_9600,
    BAUD_RATE_14400,
    BAUD_RATE_19200,
    BAUD_RATE_38400,

}UartBaudRateType;
static uint16  BaudRate[] = {2400, 4800, 9600, 14400, 19200, 38400}; 


uint8  UartInit(void);
void RT_Test(void);
void TX_Test(void);
uint32 Uart_Send(uint8* buffer, uint32 size);

#endif

