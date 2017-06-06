#ifndef  DATAVALUE_H
#define  DATAVALUE_H

#include "bsp_cpuconfig.h"


/** set 设置范围宏定义 */
#define      RESMIN        (0)           // 电阻
#define      RESMAX        (600)
#define      FILEMIN       (1)           // 文件个数
#define      FILEMAX       (99)
#define      STEPMIN       (1)           // 测试步最小值
#define      STEPMAX       (99)          // 测试步最大值



/** 数据结构 保存set system 选项 */
typedef struct Information
{
	uint8 **title;           // 标题
	uint8 *option[9];        // 当前选项指针
	uint8 **opt[9];          // 选项指针(左右移动)
	uint8 num[9];            // 选项个数(用于左右移动)
	uint8 amount;            // 标题个数(用于上下移动)
}INFO;

typedef struct Infor
{
	uint8 **title;            // 标题
	uint8 **option[9];        // 当前选项指针
	uint8 num[9];             // 选项个数(用于左右移动)
}INFO_TWO;



/** system 标题 */
static unsigned char* System_configtitle[] = {  
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
static unsigned char  Pow_recall[] = {'L','A','S','T','\0'}; // 变量
static unsigned char* Upp_soud[]  = {"OFF","ON"};
static unsigned char* Knob_lock[] = {"OFF","ON"};
static unsigned char* Key_sound[] = {"OFF","ON"};
static unsigned char* Comm_type[] = {"RS232","RS485","USB"};
static unsigned char* Comm_baud[] = {"2400","4800","9600","19200","38400"};
static unsigned char* Comm_addr[] = {"01","02","03","04","05","06","07","08","09","10"}; 
static unsigned char* Load_defa[] = {"NO","YES"};
static unsigned char* Exit_save[] = {"NO","YES"};
/** 保存system的选项下标或值 */
static uint8 systemvalue[9]={0,0,0,0,0,0,0,0,0};            
static uint8 sys_default[9] = {1, 1, 1, 1, 0, 2, 0, 0, 1}; // 默认值  第一个测试文件  OFF,.....



/** set 标题  常量型字符串数组 */
static unsigned char* Set_configtitle[] = {"FILE:" ,"STEP:" ,"STEP RES:" }; 
/** set 数据   变量型字符串数组 */
static unsigned char FILE[3] = {'0','1','\0'};        // 文件
static unsigned char STEP[3] = {'0','1','\0'};        // 步
static unsigned char STEP_RES[7] = {'1','0','0','.','0','0','\0'};   // 电阻
static float s_setvalue[3] = {1,1,100.00};           // 文件、步(电阻数组下标)、电阻 当前值
static float s_resvalue[99] = {100};                 // 一页的电阻值 
static uint16 s_file_addr[99] = {0};                 // 保存测试文件的地址 , 开机时赋值










uint8 ReadSystemPara(uint8* str, uint8 opt, uint8 num);
uint8 WriteSystemPara(uint8* str, uint8 opt, uint8 num);

void Write_FILEvalue(uint8 file_value);
uint8* Read_FILEvalue(void);

void Write_STEPvalue(uint8 step_value);
uint8* Read_STEPvalue(void);

uint8 Write_RESvalue(uint8* res, uint8 num);
uint8* Read_RESvalue(void);

uint8 TestValue(float* teststr, uint8 opt, uint8 type);




uint8 FloatToUint8(uint8* str, float* data);
float Uint8ToFloat(unsigned char* str);

void FloatToBCDCode(unsigned char* str,float data);
float BCDCodeToFloat(unsigned char* str);

float SetValue(float temp, uint8 opt, uint8 type);

#endif

/*************     END     *************/	
