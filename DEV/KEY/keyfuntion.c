#include "keyfuntion.h"
#include "lcd.h"
#include "move.h"
#include "datavalue.h"
#include "panel.h"
#include "stringhandle.h"
#include "readandwrite.h"
#include "string.h"
#include "resreadandwrite.h"










/* 本地静态变量  提供外部读写函数  */
static  uint32  KEYVALUE = PANEL_NULL;   // 记录按键值
static  uint8   s_local  = 1;            //  通信标志   0: 远程   1: 本地
static  uint8   s_load   = 0;            // 0: 关负载 1: 开



/** 保存set system 信息		*/												 
INFO set_Info;///<set信息
INFO system_Info;///<system信息



//static float s_setvalue_temp = 0;  // 电压电流临时变量
static unsigned char s_systemp = 0;       // 左右移动保存临时变量
static uint8 datastr[CH_AMO] = {'\0','\0','\0','\0','\0','\0','\0'};
static uint8 s_dotnum = 0; // 记录小数点个数
static uint8 s_valnum = 0; // 记录数字个数



/**  函数声明 */
void NumSet(uint32 value, uint8* record, uint8 size, uint8* valnum, uint8* dotnum);
void SetEdit(uint32 value);
void ClearString_One(unsigned char* str , unsigned char n); // 清空字符串



/**
 *@Description: 初始化set,system选项 用于上下左右移动
 *@paramreq- 
 *@paramind- 
 *@return 
 */
void InfoValue(void)
{
	uint8 systemstr[9]={1,9,2,0,0,0,0,0,0};
  uint8 setstr[3] = {1, 9, 1};                       // FILE   STEP  (保留)
  uint8 resstr[7] = {'1','0','0','.','0','0','\0'};  // RES 
//AT24Cxx_Write(383, systemstr, 2);


  // 第一页 (0-127) 保存测试参数,  第二页(128-255) 保存system 参数 , 第三页(383-511) 保存set参数  , 
  // 第四页 备用, 第五页(639-)之后保存电阻值
  AT24Cxx_Read(383, setstr, 2);               // 读取测试文件序号,测试步   测试文件序号用 一个uint8(十进制) 保存
  if((setstr[0]>=1) && (setstr[0]<=99) && (setstr[1]>=1) &&(setstr[1]<=99))      
	{
		Write_FILEvalue(setstr[0]);               // 值合法   将数值转换为字符串
		Write_STEPvalue(setstr[1]);
	}
	else
	{
		setstr[0] = s_setvalue[0];                // 默认值
		setstr[1] = s_setvalue[1];
		AT24Cxx_Write(383, setstr, 2);
	}
	GetAndSet(resstr, setstr[0], setstr[1], 1); // 读取电阻值
	Write_RESvalue(resstr, 6);
	
	set_Info.title     = Set_configtitle;
	set_Info.option[0] = Read_FILEvalue();      // 数据
	set_Info.option[1] = Read_STEPvalue();      
	set_Info.option[2] = Read_RESvalue();      
	set_Info.num[0] = 0;
	set_Info.num[1] = 0;
	set_Info.num[2] = 0;
	set_Info.amount = 3;                        // 


WriteSystemPara(systemstr, 0, 9);             //重新设置

	/** system 菜单的信息 标题、选项数组指针、选项数组大小、标题个数*/
	system_Info.title = System_configtitle;     // 标题指针
	system_Info.amount = 9;

	system_Info.option[0] = Pow_reca[0];        // 当前选项指针
	system_Info.opt[0] = Pow_reca;              // 选项数组指针
	system_Info.option[0] = Pow_recall;         // 数据
	system_Info.num[0] = GET_ARRAY_COUNT(Pow_reca);     // 二维数组元素个数
	
	system_Info.option[1] = Upp_soud[systemstr[1]];
	system_Info.num[1] = GET_ARRAY_COUNT(Upp_soud); 
	system_Info.opt[1] = Upp_soud;
	
	system_Info.option[2] = Knob_lock[systemstr[2]];
	system_Info.num[2] = GET_ARRAY_COUNT(Knob_lock);
	system_Info.opt[2] = Knob_lock;
	
	system_Info.option[3] = Key_sound[systemstr[3]];
	system_Info.num[3] = GET_ARRAY_COUNT(Key_sound);
	system_Info.opt[3] = Key_sound;
	
	system_Info.option[4] = Comm_type[systemstr[4]];
	system_Info.num[4] = GET_ARRAY_COUNT(Comm_type);
	system_Info.opt[4] = Comm_type;
	
	system_Info.option[5] = Comm_baud[systemstr[5]];
	system_Info.num[5] = GET_ARRAY_COUNT(Comm_baud);
	system_Info.opt[5] = Comm_baud;
	
	system_Info.option[6] = Comm_addr[systemstr[5]];
	system_Info.num[6] = GET_ARRAY_COUNT(Upp_soud);
	system_Info.opt[6] = Comm_addr;
	
	system_Info.option[7] = Load_defa[systemstr[7]];
	system_Info.num[7] = GET_ARRAY_COUNT(Load_defa);
	system_Info.opt[7] = Load_defa;
	
	system_Info.option[8] = Exit_save[systemstr[8]];
	system_Info.num[8] = GET_ARRAY_COUNT(Exit_save);
	system_Info.opt[8] = Exit_save;
	
	s_valnum = 0;                 // 清零
	s_dotnum = 0;
	s_panel.now = PANEL_MAIN;
	s_panel.old = PANEL_MAIN;	
}





/**
 *@Description: set 编辑状态处理  测试步和电阻值是同步的  测试页改变时重新读取电阻值,测试步不变,测试电阻重置
 *@paramreq-  value: 传入的按键码   
 *@paramind- 
 *@return 无
 */
void SetEdit(uint32 value)
{
	    uint8 i = s_display.position+s_display.point;  // 当前选项下标
	    float temp_one = 0;
      uint8 k = 0;
	    uint8 temp = 0;
	    unsigned int temp_two = 0;
	
			if(value == KEY_VAL_ENTER)                   // 确定
			{
				if(set_Info.title[i] == Set_configtitle[0])  // 测试页
				{
					temp = GET_ARRAY_COUNT(datastr);
					k = StrToUint(datastr, &temp_two, s_valnum, temp); // 字符串转数值  数值不正确
					if(k==1)  // 无异常 则表示测试页重新设置成功 
					{
						if(temp_two>=FILEMIN && temp_two<=FILEMAX) // 判断数据范围
						{
//							s_setvalue[0] = temp_two;  // 数据在范围 将新的测试页赋给当前测试页
							TestValue((float*)&temp_two, 0, 1);
//							IntToStr(set_Info.option[0],temp_two);  // 将当前测试页转换为字符串 
//							Write_FILEvalue(*datastr);
							set_Info.option[0] = Read_FILEvalue();
// 获取当前测试页的测试参数 保存到测试电阻数组
							k = s_setvalue[1]; // 获取当前测试步
							k = SetValue(k, 1, 0);
							TestValue((float*)&k, 1, 0);
							s_setvalue[2] = s_resvalue[k-1]; // 将该测试步下的电阻值重新赋值
							FloatToStr(set_Info.option[2], s_setvalue[2], 2); // 转为字符串
						}
					}
				}
				else if(set_Info.title[i] == Set_configtitle[1])  // 测试步
				{
					temp = GET_ARRAY_COUNT(datastr);
					k = StrToUint(datastr, &temp_two, s_valnum, temp); // 字符串转数值  相反
					if(k != 0)  // 无异常
					{
						if(temp_two>=STEPMIN && temp_two<=STEPMAX)
						{
							s_setvalue[1] = temp_two;  // 数据无异常 赋值
//							IntToStr(STEP, temp_two); // 测试步值转为字符串
//							Write_STEPvalue(datastr);
							set_Info.option[1] = Read_STEPvalue();
							/***** 这里最好是读取次数值并转化 ****/
							s_setvalue[2] = s_resvalue[temp_two-1]; // 电阻相应变化   
							FloatToStr(set_Info.option[2], s_setvalue[2], 2); // 电阻值转为字符串
						}
					}
				}
				else if(set_Info.title[i] == Set_configtitle[2])  // 测试电阻
				{
					k = StrToFloat(datastr, &temp_one, s_valnum-1);  // 有问题
					if(k != 0)
					{
						if(temp_one>RESMIN && temp_one<= RESMAX)
						{
							s_setvalue[2] = temp_one;    // 更新电阻值
							k = s_setvalue[1];           // 获取当前测试步
							s_resvalue[k-1] = temp_one;    // 更新测试电阻
							Write_RESvalue(datastr,s_valnum-1);
//							FloatToStr(STEP_RES, s_setvalue[2], 2); // 转为字符串        有问题。。。。。。。。。。。。
						}
					}
				}
				else
				{
				}
				if(s_display.position == set_Info.amount-1) // 最后一个
				{
					LCD_Empty(14, s_display.point, 6);
				  LCD_Write(set_Info.option[i],15,s_display.point);   // 显示值
				}
				else
				{
					LCD_Empty(14, 0, 6);  // 刷新第一行
					LCD_Write(set_Info.option[s_display.position],15,s_display.position);   // 显示值
					LCD_Empty(14, 1, 6);  // 刷新第二行
					LCD_Write(set_Info.option[s_display.position+1],15,1);   // 显示值                           显示的值出错
				}
				ClearString_One(datastr, CH_AMO-1);
				s_valnum = 0;                 // 清零
				s_dotnum = 0;
				s_panel.now = PANEL_SET;      // 当前面板
				s_panel.old = PANEL_MAIN;    // 前一面板
			}
			else if(value == KEY_VAL_ESC)    // 取消
			{
				LCD_Empty(14, s_display.point, 6);
				LCD_Write(set_Info.option[i],15,s_display.point);
				ClearString_One(datastr, CH_AMO-1);
				s_valnum = 0;                 // 清零
				s_dotnum = 0;
				s_panel.now = PANEL_SET;      // 当前面板
				s_panel.old = PANEL_MAIN;     // 前一面板
			}
			else if(value == KEY_VAL_LEFT)       // 左移
			{
//				LeftAndRight(set_Info.option,set_Info.num,set_Info.flag, 0);
			}
			else if(value == KEY_VAL_RIGHT)     // 右移
			{
//				LeftAndRight(set_Info.option,set_Info.num,set_Info.flag, 1);
			}
			else
			{
				NumSet(value, datastr, CH_AMO, &s_valnum, &s_dotnum);     // 按键码、保存数字的数组、当前按键次数、小数点个数
				LCD_Empty(14, s_display.point, 6);
				LCD_Write(datastr,15,s_display.point);
			}	
}




/**
*@Description: 键盘功能   根据键值和面板级别调用相关的函数
*@paramreq-  value: 传入的按键码   
*@paramind- 
*@return 无
*/
void KeyFunction(uint32 value)
{
	uint8 i = 0;
	uint8 k = 0;
	unsigned int temp = 0;
		
	switch(s_panel.now)
	{
		 /* 主界面下选择按键值 开机显示和测试状态显示 */	
		case PANEL_MAIN:                           // 主面板			
			if(value == KEY_VAL_LOAD)                // Load : 测试状态标志     
			{
				// s_load 不同文件值不同 需要修改形式
				if(s_load == 0)                        // 负载开关
				{
					s_load = 1;                          // 将标志位置 1 , 进入测试状态
					LCD_Empty(17,0,3);                   // 先清除
					LCD_Write("ON",17,0);                // LCD显示负载状态
				}
				else if(s_load == 1)
				{
					s_load = 0;                           // 当前为非测试状态
					LCD_Empty(17,0,3);
					LCD_Write("OFF",17,0);                // LCD显示负载状态
				}
				else
				{
				}
			}
			else if(value == KEY_VAL_SET)             // set 键
			{
				LCD_Clear();                            // 清屏
				i = 0x7E;
				LCD_Write(&i,0,0);   // 画箭头                   
				LCD_Write(Set_configtitle[0],1,0); // 显示第一行标题
				LCD_Write(set_Info.option[0],15,0); // 显示第一行数据
        LCD_Write(Set_configtitle[1],1,1);		// 显示第二行标题
        LCD_Write(set_Info.option[1],15,1);	// 显示第二行数据	
				
				s_display.position = 0;                 // 刷新移动标志
				s_display.point = 0;
				s_dotnum = 0; // 记录小数点个数
        s_valnum = 0; // 记录数字个数
				s_panel.now = PANEL_SET;               // 当前面板
				s_panel.old = PANEL_MAIN;              // 前一面板
			}
			else if(value == KEY_VAL_DOT)             // system键
			{
				LCD_Clear();  
				i = 0x7E;
				LCD_Write(&i,0,0);   // 画箭头
				LCD_Write(System_configtitle[0],1,0); // 显示第一行标题
				LCD_Write(system_Info.option[0],15,0); // 显示第一行数据
				
        LCD_Write(System_configtitle[1],1,1);		// 显示第二行标题
        LCD_Write(system_Info.option[1],15,1);	// 显示第二行数据	
				
				s_display.position = 0;                        // 刷新移动标志
				s_display.point = 0;
				s_dotnum = 0; // 记录小数点个数
        s_valnum = 0; // 记录数字个数
				s_panel.now = PANEL_SYSTEM; 
				s_panel.old = PANEL_MAIN; 
			}
			else if(value == KEY_VAL_9)               // local 键 远/近程
			{
				if(s_local == 1)
				{
					s_local = 0;
				}
				else
				{
					s_local = 1;
				}
			}
			else
			{
				;
			}
			break;
		
    /* set 界面下选择按键值 可以上下移动选项 */			
		case PANEL_SET:                              // set 界面
			if(value == KEY_VAL_UP)                    // 上移 
			{
				UpAndDown_One(set_Info, set_Info.num, &s_display, 0);  // 只有一个选项
			}
			else if(value == KEY_VAL_DOWN)             // 下移
			{
				UpAndDown_One(set_Info, set_Info.num, &s_display, 1);
			}
			else if(value == KEY_VAL_ENTER)              // 选中
			{
				i = s_display.position+s_display.point;    // 选中的位置
				if(system_Info.num[i] > 1)
				{
//				s_setvalue_temp = s_setvalue[i];             // 临时数据保存 
					LCD_Write("<",14,s_display.point);
				  LCD_Write(set_Info.option[i],15,s_display.point);    // 显示值
				  WriteLcdData('>');
				}
				else
				{
					LCD_Write("<",14,s_display.point);
//					FloatToStr(set_Info.option[i],s_setvalue[i], 0);     // 将当前数据转换为字符串
					LCD_Write(set_Info.option[i],15,s_display.point);    // 显示值
					WriteLcdData('>');
					// 显示光标
				}
				s_panel.now = PANEL_SET_EDIT;            // 当前面板
				s_panel.old = PANEL_SET;                 // 前一面板
			}
			else if(value == KEY_VAL_ESC)               // 退出
			{
				MainPanel();                              // 主界面
//				s_setvalue_temp = 0.0;                    // 清空临时数据
				s_display.point = 0;
				s_display.position = 0;
				s_panel.now = PANEL_MAIN;                 // 当前面板
				s_panel.old = PANEL_MAIN;                 // 前一面板
			}
			else
			{
				;
			}
			break;
		
		 /* system 界面下选择按键值 可以上下移动选项 */	
		case PANEL_SYSTEM:                            // system界面
			i = s_display.position+s_display.point;
			if(value == KEY_VAL_UP)                     // 上移 
			{
				UpAndDown_One(system_Info, systemvalue,&s_display,0);
				if(s_display.position == 0)
				{
					LCD_Empty(15, 0, 6);
					LCD_Write(system_Info.option[0],15,0);  // 画数据
				}
			}
			else if(value == KEY_VAL_DOWN)              // 下移
			{
				UpAndDown_One(system_Info, systemvalue,&s_display,1);
				if(s_display.position == 0)
				{
					LCD_Empty(15, 0, 6);
					LCD_Write(system_Info.option[0],15,0);  // 画数据
				}
			}
			else if(value == KEY_VAL_ENTER)             // 选中
			{
				if(system_Info.num[i] > 1)
				{
					LCD_Write("<",14,s_display.point);
					LCD_Write(system_Info.option[i],15,s_display.point);  // 选中值
					WriteLcdData('>');
				}
				else
				{
					LCD_Write("<",14,s_display.point);
					LCD_Write(system_Info.option[s_display.position+s_display.point],15,s_display.point);  // 选中值
					WriteLcdData('>');  //显示光标
				}
				s_systemp = systemvalue[i];               // 保存当前值
				s_panel.now = PANEL_SYSTEM_EDIT;         // 当前面板
				s_panel.old = PANEL_SYSTEM;              // 前一面板
			}
			else if(value == KEY_VAL_ESC)               // 退出
			{
//				AT24Cxx_Write(4, sys_default, 9);  // 保存默认参数 
//				AT24Cxx_Write(0xFFAF, sys_default, 9);
				MainPanel();                              // 主界面
				s_display.point = 0;
				s_display.position = 0;
				s_systemp = 0;
				s_panel.now = PANEL_MAIN;                // 当前面板
				s_panel.old = PANEL_MAIN;                // 前一面板
			}
			else
			{
				;
			}			
			break;
		
		/** set 编辑界面 编辑页和测试电阻 数字键可用 */	
		case PANEL_SET_EDIT:                           // set 选中状态
			 SetEdit(value);
			break;
		
		/* system 编辑界面 设置参数 */	
		case PANEL_SYSTEM_EDIT:                        // system 选中状态
			i = s_display.position+s_display.point;
			if(value == KEY_VAL_ENTER)                   // 确定
			{
				if(system_Info.num[i] == 1)
				{
					temp = GET_ARRAY_COUNT(datastr);
					k = StrToUint(datastr, &temp, s_valnum, temp);
					s_systemp = temp;
					if(k!=0 && (s_systemp>0 && s_systemp<=99))
					{
//						systemvalue[i] = (uint8)temp; //赋值
						WriteSystemPara(&systemvalue[i], i, 1);
						IntToStr(system_Info.option[i], s_systemp);
					}
					else
					{
						LCD_Empty(14, s_display.point, 6);
					  LCD_Write(system_Info.option[i],15,s_display.point);
					}
				}
				else
				{
					if(system_Info.title[i] == System_configtitle[8])        // 如果是 save 选项  则确定退出,返回主界面
					{
						MainPanel();                              // 主界面
				    s_display.point = 0;
				    s_display.position = 0;
				    s_systemp = 0;
				    s_panel.now = PANEL_MAIN;                // 当前面板
				    s_panel.old = PANEL_MAIN;                // 前一面板
					}
					else
					{
						LCD_Empty(14, s_display.point, 6);
					  systemvalue[i] = s_systemp;                            // 保存当前值
					  system_Info.option[i] = system_Info.opt[i][s_systemp]; // 当前指针指向改变
					  LCD_Write(system_Info.option[i],15,s_display.point);
					}
				}
				ClearString_One(datastr, CH_AMO-1);
				AT24Cxx_Write(128, systemvalue, 9);       // 保存更改的参数 
				s_panel.now = PANEL_SYSTEM;               // 当前面板
//AT24Cxx_Read(128, systemvalue, 9);
				s_panel.old = PANEL_MAIN;                 // 前一面板
			}
			else if(value == KEY_VAL_ESC)                // 取消
			{
				LCD_Empty(14, s_display.point, 6);
				LCD_Write(system_Info.option[i],15,s_display.point);
				s_systemp = 0;
				s_valnum  = 0;
				s_dotnum  = 0;
				ClearString_One(datastr, CH_AMO-1);
				s_panel.now = PANEL_SYSTEM;               // 当前面板
				s_panel.old = PANEL_MAIN;                 // 前一面板
			}
			else if(value == KEY_VAL_LEFT)               // 左移
			{
				if(system_Info.num[i] > 1)
				{
					s_systemp = LeftAndRight_One(system_Info.opt[i], system_Info.num[i], s_systemp, 0);
					LCD_Empty(15, s_display.point, 6);
				  LCD_Write(system_Info.opt[i][s_systemp],15,s_display.point);
					WriteLcdData('>');
				}
			}
			else if(value == KEY_VAL_RIGHT)               // 右移
			{
				if(system_Info.num[i] > 1)
				{
					s_systemp = LeftAndRight_One(system_Info.opt[i], system_Info.num[i], s_systemp, 1);
					LCD_Empty(15, s_display.point, 6);
				  LCD_Write(system_Info.opt[i][s_systemp],15,s_display.point);
					WriteLcdData('>');
				}
			}
			else
			{
				if(system_Info.num[i] < 2)  // 没有固定选项时可以按数字键
				{
					NumSet(value, &datastr[0], CH_AMO, &s_valnum, &s_dotnum);               // 数字按键
				  LCD_Empty(14, s_display.point, 6);
				  LCD_Write(datastr,15,s_display.point);
				}
				else
				{
					;
				}
			}
			break;
		
		default:
			break;
	}
}




/**
*@Description: 按数字键记录到数组
*@paramreq-  value: 传入的按键码   record: 数组保存按键值 size: 数组大小 valnum: 数组按键次数 dotnum: 小数点个数
*@paramind- 
*@return 无
*/
void NumSet(uint32 value, uint8* record, uint8 size, uint8* valnum, uint8* dotnum)
{
	if(*valnum >= size) // 超出范围
	{
		return ;
	}
	else
	{
		record[*valnum] = '\0';
		switch(value)
		{
			case KEY_VAL_0:
				record[*valnum] = 0x30;
			  *valnum = *valnum+1;
			  break;
			case KEY_VAL_1:
				record[*valnum] = 0x31;
			  *valnum = *valnum+1;
			  break;
			case KEY_VAL_2:
				record[*valnum] = 0x32;
			*valnum = *valnum+1;
			  break;
			case KEY_VAL_3:
				record[*valnum] = 0x33;
			*valnum = *valnum+1;
			  break;
			case KEY_VAL_4:
				record[*valnum] = 0x34;
			*valnum = *valnum+1;
			  break;
			case KEY_VAL_5:
				record[*valnum] = 0x35;
			*valnum = *valnum+1;
			  break;
			case KEY_VAL_6:
				record[*valnum] = 0x36;
			*valnum = *valnum+1;
			  break;
			case KEY_VAL_7:
				record[*valnum] = 0x37;
			*valnum = *valnum+1;
			  break;
			case KEY_VAL_8:
				record[*valnum] = 0x38;
			*valnum = *valnum+1;
			  break;
			case KEY_VAL_9:
				record[*valnum] = 0x39;
			*valnum = *valnum+1;
			  break;
			case KEY_VAL_DOT:
				if((*dotnum == 0)&&(*valnum<=CH_AMO-2))
				{
					record[*valnum] = '.';
					*dotnum = 1;
					*valnum = *valnum+1;
				}
				else
				{
				}
				break;
			default:
				break;
//		return ; // 不是数字按键 返回
    }
	}
}




/**
*@Description: 清空字符串  注意 : 不能在函数里面计算数组 str 的大小 , GET_ARRAY_COUNT(str) 为 str 指针的大小
*@paramreq-  str : 字符串指针  n: 数组前 n 个
*@paramind- 
*@return 无
*/
void ClearString_One(unsigned char* str , unsigned char n)
{
	uint8 i = 0;	
	
  for(i=0;i<n;i++)   // 要比较大小
  {
		str[i] = '\0';
  }	
}



/**  用于变量的封装   包括本地静态变量的读取和设置   */
/** 
*@Description: 只用来读取键值   便于操作   封装
*@paramreq-   
*@paramind- 
*@return 当前键值
*/
uint32 ReadKeyValue(void)
{
	uint32 value = KEYVALUE;
	return (value);
}


/**
*@Description: 只用来设置键值
*@paramreq-   value:要设置的按键值可以是 扫描得到的按键值
*@paramind- 
*@return 
*/
void WriteKeyValue(uint32 value)
{
	KEYVALUE = value;
}



/**
*@Description: 读取远近程状态标志
*@paramreq-   
*@paramind- 
*@return 
*/
uint8 Read_Local(void)
{
	return (s_local);
}


uint8 Reversal_Local(void)
{
	if(s_load != 0)
	{
		s_local = 0;
	}
	else
	{
		s_local = 1;
	}
	return (1);
}


uint8 Read_Load(void)
{
	return (s_load);
}


uint8 Reversal_Load(void)
{
	if(s_load != 0)
	{
		s_load = 0;
	}
	else
	{
		s_load = 1;
	}
	return (1);
}




uint8 ReadPanel(void)
{
	return(s_panel.now);
}




/*************     END     *************/	



