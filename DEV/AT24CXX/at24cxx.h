#ifndef  AT24CXX_H
#define  AT24CXX_H


#include "bsp_gpio.h"



static uint8 axx = 1;

//extern uint8 ceshi_str[5] = {'1','2','3','4','5'};
//extern uint8 bxx = 0;


uint8 Readaxx(void);
void writeaxx(uint8 value);


#endif
