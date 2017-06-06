#ifndef  SYSTEM_PARAMETER_H
#define  SYSTEM_PARAMETER_H



#include "BSP_CpuConfig.h"




/** system 标题 */
const unsigned char* System_configtitle[] = {  
    "POWERON REC:" , //0
    "UPPER SOUND: " ,//1
    " KNOB LOCK: " , //2
    " KEY SOUND:" ,  //3
    "COMM TYPE:" ,   //4
    "BAUDRATE:" ,    //5..
    "ADDRESS:" ,     //6..
    "LOAD DEFAULT:" ,//7
    "EXIT SAVE:" };  //8

/** system选项   字符串数组  */
static unsigned char* Pow_reca[]  = {"LAST"}; // 等于99时
static unsigned char  Pow_recall[] = {'L','A','S','T','\n'}; // 变量
static unsigned char* Upp_soud[]  = {"OFF","ON"};
static unsigned char* Knob_lock[] = {"OFF","ON"};
static unsigned char* Key_sound[] = {"OFF","ON"};
static unsigned char* Comm_type[] = {"RS232","RS485","USB"};
static unsigned char* Comm_baud[] = {"2400","4800","9600","19200","38400"};
static unsigned char* Comm_addr[] = {"01","02","03","04","05","06","07","08","09","10"}; 
static unsigned char* Load_defa[] = {"NO","YES"};
static unsigned char* Exit_save[] = {"NO","YES"};




#endif



