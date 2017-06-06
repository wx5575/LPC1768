#ifndef  TEST_H
#define  TEST_H

#include "BSP_CpuConfig.h"


float VoltageGet(void);
float EletricityGet(void);
float ResistanceGet(void);

unsigned char StepGet(void);
unsigned char FileGet(void);


uint8 ExamineSelf(void);              // ¿ª»ú×Ô¼ì

void TemperatureSet(uint32 value);

uint8 FAN_Ctrol(uint32 samp, uint8 load);

void readaxx(void);

uint32 GetADValue(uint32 *adVal, uint8 *volt, uint8 *curr);






#endif
