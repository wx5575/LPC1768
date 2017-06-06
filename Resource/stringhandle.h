#ifndef    STRINGHANDLE_H_
#define    STRINGHANDLE_H_

#include "bsp_cpuconfig.h"



// data: 要转换为字符串的数据 
void IntToStr(unsigned char* str, long int data);
void FloatToStr(unsigned char* result,float num, unsigned char pre);
uint8 StrToFloat(uint8* str, float* number, uint8 pre);
uint8 StrToUint(uint8* str, unsigned int* number, uint8 pre, uint8 size);


portuBASE_TYPE LIB_StrInsert(int8 *rscStr, uint8 insertPosIndex, uint8 insertChar);
void LIB_ConvertNmubToChar(uint8 *prscSaveAddr, uint8 convertLen);
void LIB_DataSplit(uint32 rscData, portuBASE_TYPE splitLen, uint8 *pdestSaveAddr);
uint32 LIB_Get10nData(portuBASE_TYPE n);




























#endif




