#include "keyfuntion.h"
#include "lcd.h"
#include "move.h"
#include "datavalue.h"
#include "panel.h"
#include "stringhandle.h"
#include "readandwrite.h"
#include "string.h"
#include "resreadandwrite.h"










/* ���ؾ�̬����  �ṩ�ⲿ��д����  */
static  uint32  KEYVALUE = PANEL_NULL;   // ��¼����ֵ
static  uint8   s_local  = 1;            //  ͨ�ű�־   0: Զ��   1: ����
static  uint8   s_load   = 0;            // 0: �ظ��� 1: ��



/** ����set system ��Ϣ		*/												 
INFO set_Info;///<set��Ϣ
INFO system_Info;///<system��Ϣ



//static float s_setvalue_temp = 0;  // ��ѹ������ʱ����
static unsigned char s_systemp = 0;       // �����ƶ�������ʱ����
static uint8 datastr[CH_AMO] = {'\0','\0','\0','\0','\0','\0','\0'};
static uint8 s_dotnum = 0; // ��¼С�������
static uint8 s_valnum = 0; // ��¼���ָ���



/**  �������� */
void NumSet(uint32 value, uint8* record, uint8 size, uint8* valnum, uint8* dotnum);
void SetEdit(uint32 value);
void ClearString_One(unsigned char* str , unsigned char n); // ����ַ���



/**
 *@Description: ��ʼ��set,systemѡ�� �������������ƶ�
 *@paramreq- 
 *@paramind- 
 *@return 
 */
void InfoValue(void)
{
	uint8 systemstr[9]={1,9,2,0,0,0,0,0,0};
  uint8 setstr[3] = {1, 9, 1};                       // FILE   STEP  (����)
  uint8 resstr[7] = {'1','0','0','.','0','0','\0'};  // RES 
//AT24Cxx_Write(383, systemstr, 2);


  // ��һҳ (0-127) ������Բ���,  �ڶ�ҳ(128-255) ����system ���� , ����ҳ(383-511) ����set����  , 
  // ����ҳ ����, ����ҳ(639-)֮�󱣴����ֵ
  AT24Cxx_Read(383, setstr, 2);               // ��ȡ�����ļ����,���Բ�   �����ļ������ һ��uint8(ʮ����) ����
  if((setstr[0]>=1) && (setstr[0]<=99) && (setstr[1]>=1) &&(setstr[1]<=99))      
	{
		Write_FILEvalue(setstr[0]);               // ֵ�Ϸ�   ����ֵת��Ϊ�ַ���
		Write_STEPvalue(setstr[1]);
	}
	else
	{
		setstr[0] = s_setvalue[0];                // Ĭ��ֵ
		setstr[1] = s_setvalue[1];
		AT24Cxx_Write(383, setstr, 2);
	}
	GetAndSet(resstr, setstr[0], setstr[1], 1); // ��ȡ����ֵ
	Write_RESvalue(resstr, 6);
	
	set_Info.title     = Set_configtitle;
	set_Info.option[0] = Read_FILEvalue();      // ����
	set_Info.option[1] = Read_STEPvalue();      
	set_Info.option[2] = Read_RESvalue();      
	set_Info.num[0] = 0;
	set_Info.num[1] = 0;
	set_Info.num[2] = 0;
	set_Info.amount = 3;                        // 


WriteSystemPara(systemstr, 0, 9);             //��������

	/** system �˵�����Ϣ ���⡢ѡ������ָ�롢ѡ�������С���������*/
	system_Info.title = System_configtitle;     // ����ָ��
	system_Info.amount = 9;

	system_Info.option[0] = Pow_reca[0];        // ��ǰѡ��ָ��
	system_Info.opt[0] = Pow_reca;              // ѡ������ָ��
	system_Info.option[0] = Pow_recall;         // ����
	system_Info.num[0] = GET_ARRAY_COUNT(Pow_reca);     // ��ά����Ԫ�ظ���
	
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
	
	s_valnum = 0;                 // ����
	s_dotnum = 0;
	s_panel.now = PANEL_MAIN;
	s_panel.old = PANEL_MAIN;	
}





/**
 *@Description: set �༭״̬����  ���Բ��͵���ֵ��ͬ����  ����ҳ�ı�ʱ���¶�ȡ����ֵ,���Բ�����,���Ե�������
 *@paramreq-  value: ����İ�����   
 *@paramind- 
 *@return ��
 */
void SetEdit(uint32 value)
{
	    uint8 i = s_display.position+s_display.point;  // ��ǰѡ���±�
	    float temp_one = 0;
      uint8 k = 0;
	    uint8 temp = 0;
	    unsigned int temp_two = 0;
	
			if(value == KEY_VAL_ENTER)                   // ȷ��
			{
				if(set_Info.title[i] == Set_configtitle[0])  // ����ҳ
				{
					temp = GET_ARRAY_COUNT(datastr);
					k = StrToUint(datastr, &temp_two, s_valnum, temp); // �ַ���ת��ֵ  ��ֵ����ȷ
					if(k==1)  // ���쳣 ���ʾ����ҳ�������óɹ� 
					{
						if(temp_two>=FILEMIN && temp_two<=FILEMAX) // �ж����ݷ�Χ
						{
//							s_setvalue[0] = temp_two;  // �����ڷ�Χ ���µĲ���ҳ������ǰ����ҳ
							TestValue((float*)&temp_two, 0, 1);
//							IntToStr(set_Info.option[0],temp_two);  // ����ǰ����ҳת��Ϊ�ַ��� 
//							Write_FILEvalue(*datastr);
							set_Info.option[0] = Read_FILEvalue();
// ��ȡ��ǰ����ҳ�Ĳ��Բ��� ���浽���Ե�������
							k = s_setvalue[1]; // ��ȡ��ǰ���Բ�
							k = SetValue(k, 1, 0);
							TestValue((float*)&k, 1, 0);
							s_setvalue[2] = s_resvalue[k-1]; // ���ò��Բ��µĵ���ֵ���¸�ֵ
							FloatToStr(set_Info.option[2], s_setvalue[2], 2); // תΪ�ַ���
						}
					}
				}
				else if(set_Info.title[i] == Set_configtitle[1])  // ���Բ�
				{
					temp = GET_ARRAY_COUNT(datastr);
					k = StrToUint(datastr, &temp_two, s_valnum, temp); // �ַ���ת��ֵ  �෴
					if(k != 0)  // ���쳣
					{
						if(temp_two>=STEPMIN && temp_two<=STEPMAX)
						{
							s_setvalue[1] = temp_two;  // �������쳣 ��ֵ
//							IntToStr(STEP, temp_two); // ���Բ�ֵתΪ�ַ���
//							Write_STEPvalue(datastr);
							set_Info.option[1] = Read_STEPvalue();
							/***** ��������Ƕ�ȡ����ֵ��ת�� ****/
							s_setvalue[2] = s_resvalue[temp_two-1]; // ������Ӧ�仯   
							FloatToStr(set_Info.option[2], s_setvalue[2], 2); // ����ֵתΪ�ַ���
						}
					}
				}
				else if(set_Info.title[i] == Set_configtitle[2])  // ���Ե���
				{
					k = StrToFloat(datastr, &temp_one, s_valnum-1);  // ������
					if(k != 0)
					{
						if(temp_one>RESMIN && temp_one<= RESMAX)
						{
							s_setvalue[2] = temp_one;    // ���µ���ֵ
							k = s_setvalue[1];           // ��ȡ��ǰ���Բ�
							s_resvalue[k-1] = temp_one;    // ���²��Ե���
							Write_RESvalue(datastr,s_valnum-1);
//							FloatToStr(STEP_RES, s_setvalue[2], 2); // תΪ�ַ���        �����⡣����������������������
						}
					}
				}
				else
				{
				}
				if(s_display.position == set_Info.amount-1) // ���һ��
				{
					LCD_Empty(14, s_display.point, 6);
				  LCD_Write(set_Info.option[i],15,s_display.point);   // ��ʾֵ
				}
				else
				{
					LCD_Empty(14, 0, 6);  // ˢ�µ�һ��
					LCD_Write(set_Info.option[s_display.position],15,s_display.position);   // ��ʾֵ
					LCD_Empty(14, 1, 6);  // ˢ�µڶ���
					LCD_Write(set_Info.option[s_display.position+1],15,1);   // ��ʾֵ                           ��ʾ��ֵ����
				}
				ClearString_One(datastr, CH_AMO-1);
				s_valnum = 0;                 // ����
				s_dotnum = 0;
				s_panel.now = PANEL_SET;      // ��ǰ���
				s_panel.old = PANEL_MAIN;    // ǰһ���
			}
			else if(value == KEY_VAL_ESC)    // ȡ��
			{
				LCD_Empty(14, s_display.point, 6);
				LCD_Write(set_Info.option[i],15,s_display.point);
				ClearString_One(datastr, CH_AMO-1);
				s_valnum = 0;                 // ����
				s_dotnum = 0;
				s_panel.now = PANEL_SET;      // ��ǰ���
				s_panel.old = PANEL_MAIN;     // ǰһ���
			}
			else if(value == KEY_VAL_LEFT)       // ����
			{
//				LeftAndRight(set_Info.option,set_Info.num,set_Info.flag, 0);
			}
			else if(value == KEY_VAL_RIGHT)     // ����
			{
//				LeftAndRight(set_Info.option,set_Info.num,set_Info.flag, 1);
			}
			else
			{
				NumSet(value, datastr, CH_AMO, &s_valnum, &s_dotnum);     // �����롢�������ֵ����顢��ǰ����������С�������
				LCD_Empty(14, s_display.point, 6);
				LCD_Write(datastr,15,s_display.point);
			}	
}




/**
*@Description: ���̹���   ���ݼ�ֵ����弶�������صĺ���
*@paramreq-  value: ����İ�����   
*@paramind- 
*@return ��
*/
void KeyFunction(uint32 value)
{
	uint8 i = 0;
	uint8 k = 0;
	unsigned int temp = 0;
		
	switch(s_panel.now)
	{
		 /* ��������ѡ�񰴼�ֵ ������ʾ�Ͳ���״̬��ʾ */	
		case PANEL_MAIN:                           // �����			
			if(value == KEY_VAL_LOAD)                // Load : ����״̬��־     
			{
				// s_load ��ͬ�ļ�ֵ��ͬ ��Ҫ�޸���ʽ
				if(s_load == 0)                        // ���ؿ���
				{
					s_load = 1;                          // ����־λ�� 1 , �������״̬
					LCD_Empty(17,0,3);                   // �����
					LCD_Write("ON",17,0);                // LCD��ʾ����״̬
				}
				else if(s_load == 1)
				{
					s_load = 0;                           // ��ǰΪ�ǲ���״̬
					LCD_Empty(17,0,3);
					LCD_Write("OFF",17,0);                // LCD��ʾ����״̬
				}
				else
				{
				}
			}
			else if(value == KEY_VAL_SET)             // set ��
			{
				LCD_Clear();                            // ����
				i = 0x7E;
				LCD_Write(&i,0,0);   // ����ͷ                   
				LCD_Write(Set_configtitle[0],1,0); // ��ʾ��һ�б���
				LCD_Write(set_Info.option[0],15,0); // ��ʾ��һ������
        LCD_Write(Set_configtitle[1],1,1);		// ��ʾ�ڶ��б���
        LCD_Write(set_Info.option[1],15,1);	// ��ʾ�ڶ�������	
				
				s_display.position = 0;                 // ˢ���ƶ���־
				s_display.point = 0;
				s_dotnum = 0; // ��¼С�������
        s_valnum = 0; // ��¼���ָ���
				s_panel.now = PANEL_SET;               // ��ǰ���
				s_panel.old = PANEL_MAIN;              // ǰһ���
			}
			else if(value == KEY_VAL_DOT)             // system��
			{
				LCD_Clear();  
				i = 0x7E;
				LCD_Write(&i,0,0);   // ����ͷ
				LCD_Write(System_configtitle[0],1,0); // ��ʾ��һ�б���
				LCD_Write(system_Info.option[0],15,0); // ��ʾ��һ������
				
        LCD_Write(System_configtitle[1],1,1);		// ��ʾ�ڶ��б���
        LCD_Write(system_Info.option[1],15,1);	// ��ʾ�ڶ�������	
				
				s_display.position = 0;                        // ˢ���ƶ���־
				s_display.point = 0;
				s_dotnum = 0; // ��¼С�������
        s_valnum = 0; // ��¼���ָ���
				s_panel.now = PANEL_SYSTEM; 
				s_panel.old = PANEL_MAIN; 
			}
			else if(value == KEY_VAL_9)               // local �� Զ/����
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
		
    /* set ������ѡ�񰴼�ֵ ���������ƶ�ѡ�� */			
		case PANEL_SET:                              // set ����
			if(value == KEY_VAL_UP)                    // ���� 
			{
				UpAndDown_One(set_Info, set_Info.num, &s_display, 0);  // ֻ��һ��ѡ��
			}
			else if(value == KEY_VAL_DOWN)             // ����
			{
				UpAndDown_One(set_Info, set_Info.num, &s_display, 1);
			}
			else if(value == KEY_VAL_ENTER)              // ѡ��
			{
				i = s_display.position+s_display.point;    // ѡ�е�λ��
				if(system_Info.num[i] > 1)
				{
//				s_setvalue_temp = s_setvalue[i];             // ��ʱ���ݱ��� 
					LCD_Write("<",14,s_display.point);
				  LCD_Write(set_Info.option[i],15,s_display.point);    // ��ʾֵ
				  WriteLcdData('>');
				}
				else
				{
					LCD_Write("<",14,s_display.point);
//					FloatToStr(set_Info.option[i],s_setvalue[i], 0);     // ����ǰ����ת��Ϊ�ַ���
					LCD_Write(set_Info.option[i],15,s_display.point);    // ��ʾֵ
					WriteLcdData('>');
					// ��ʾ���
				}
				s_panel.now = PANEL_SET_EDIT;            // ��ǰ���
				s_panel.old = PANEL_SET;                 // ǰһ���
			}
			else if(value == KEY_VAL_ESC)               // �˳�
			{
				MainPanel();                              // ������
//				s_setvalue_temp = 0.0;                    // �����ʱ����
				s_display.point = 0;
				s_display.position = 0;
				s_panel.now = PANEL_MAIN;                 // ��ǰ���
				s_panel.old = PANEL_MAIN;                 // ǰһ���
			}
			else
			{
				;
			}
			break;
		
		 /* system ������ѡ�񰴼�ֵ ���������ƶ�ѡ�� */	
		case PANEL_SYSTEM:                            // system����
			i = s_display.position+s_display.point;
			if(value == KEY_VAL_UP)                     // ���� 
			{
				UpAndDown_One(system_Info, systemvalue,&s_display,0);
				if(s_display.position == 0)
				{
					LCD_Empty(15, 0, 6);
					LCD_Write(system_Info.option[0],15,0);  // ������
				}
			}
			else if(value == KEY_VAL_DOWN)              // ����
			{
				UpAndDown_One(system_Info, systemvalue,&s_display,1);
				if(s_display.position == 0)
				{
					LCD_Empty(15, 0, 6);
					LCD_Write(system_Info.option[0],15,0);  // ������
				}
			}
			else if(value == KEY_VAL_ENTER)             // ѡ��
			{
				if(system_Info.num[i] > 1)
				{
					LCD_Write("<",14,s_display.point);
					LCD_Write(system_Info.option[i],15,s_display.point);  // ѡ��ֵ
					WriteLcdData('>');
				}
				else
				{
					LCD_Write("<",14,s_display.point);
					LCD_Write(system_Info.option[s_display.position+s_display.point],15,s_display.point);  // ѡ��ֵ
					WriteLcdData('>');  //��ʾ���
				}
				s_systemp = systemvalue[i];               // ���浱ǰֵ
				s_panel.now = PANEL_SYSTEM_EDIT;         // ��ǰ���
				s_panel.old = PANEL_SYSTEM;              // ǰһ���
			}
			else if(value == KEY_VAL_ESC)               // �˳�
			{
//				AT24Cxx_Write(4, sys_default, 9);  // ����Ĭ�ϲ��� 
//				AT24Cxx_Write(0xFFAF, sys_default, 9);
				MainPanel();                              // ������
				s_display.point = 0;
				s_display.position = 0;
				s_systemp = 0;
				s_panel.now = PANEL_MAIN;                // ��ǰ���
				s_panel.old = PANEL_MAIN;                // ǰһ���
			}
			else
			{
				;
			}			
			break;
		
		/** set �༭���� �༭ҳ�Ͳ��Ե��� ���ּ����� */	
		case PANEL_SET_EDIT:                           // set ѡ��״̬
			 SetEdit(value);
			break;
		
		/* system �༭���� ���ò��� */	
		case PANEL_SYSTEM_EDIT:                        // system ѡ��״̬
			i = s_display.position+s_display.point;
			if(value == KEY_VAL_ENTER)                   // ȷ��
			{
				if(system_Info.num[i] == 1)
				{
					temp = GET_ARRAY_COUNT(datastr);
					k = StrToUint(datastr, &temp, s_valnum, temp);
					s_systemp = temp;
					if(k!=0 && (s_systemp>0 && s_systemp<=99))
					{
//						systemvalue[i] = (uint8)temp; //��ֵ
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
					if(system_Info.title[i] == System_configtitle[8])        // ����� save ѡ��  ��ȷ���˳�,����������
					{
						MainPanel();                              // ������
				    s_display.point = 0;
				    s_display.position = 0;
				    s_systemp = 0;
				    s_panel.now = PANEL_MAIN;                // ��ǰ���
				    s_panel.old = PANEL_MAIN;                // ǰһ���
					}
					else
					{
						LCD_Empty(14, s_display.point, 6);
					  systemvalue[i] = s_systemp;                            // ���浱ǰֵ
					  system_Info.option[i] = system_Info.opt[i][s_systemp]; // ��ǰָ��ָ��ı�
					  LCD_Write(system_Info.option[i],15,s_display.point);
					}
				}
				ClearString_One(datastr, CH_AMO-1);
				AT24Cxx_Write(128, systemvalue, 9);       // ������ĵĲ��� 
				s_panel.now = PANEL_SYSTEM;               // ��ǰ���
//AT24Cxx_Read(128, systemvalue, 9);
				s_panel.old = PANEL_MAIN;                 // ǰһ���
			}
			else if(value == KEY_VAL_ESC)                // ȡ��
			{
				LCD_Empty(14, s_display.point, 6);
				LCD_Write(system_Info.option[i],15,s_display.point);
				s_systemp = 0;
				s_valnum  = 0;
				s_dotnum  = 0;
				ClearString_One(datastr, CH_AMO-1);
				s_panel.now = PANEL_SYSTEM;               // ��ǰ���
				s_panel.old = PANEL_MAIN;                 // ǰһ���
			}
			else if(value == KEY_VAL_LEFT)               // ����
			{
				if(system_Info.num[i] > 1)
				{
					s_systemp = LeftAndRight_One(system_Info.opt[i], system_Info.num[i], s_systemp, 0);
					LCD_Empty(15, s_display.point, 6);
				  LCD_Write(system_Info.opt[i][s_systemp],15,s_display.point);
					WriteLcdData('>');
				}
			}
			else if(value == KEY_VAL_RIGHT)               // ����
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
				if(system_Info.num[i] < 2)  // û�й̶�ѡ��ʱ���԰����ּ�
				{
					NumSet(value, &datastr[0], CH_AMO, &s_valnum, &s_dotnum);               // ���ְ���
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
*@Description: �����ּ���¼������
*@paramreq-  value: ����İ�����   record: ���鱣�水��ֵ size: �����С valnum: ���鰴������ dotnum: С�������
*@paramind- 
*@return ��
*/
void NumSet(uint32 value, uint8* record, uint8 size, uint8* valnum, uint8* dotnum)
{
	if(*valnum >= size) // ������Χ
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
//		return ; // �������ְ��� ����
    }
	}
}




/**
*@Description: ����ַ���  ע�� : �����ں�������������� str �Ĵ�С , GET_ARRAY_COUNT(str) Ϊ str ָ��Ĵ�С
*@paramreq-  str : �ַ���ָ��  n: ����ǰ n ��
*@paramind- 
*@return ��
*/
void ClearString_One(unsigned char* str , unsigned char n)
{
	uint8 i = 0;	
	
  for(i=0;i<n;i++)   // Ҫ�Ƚϴ�С
  {
		str[i] = '\0';
  }	
}



/**  ���ڱ����ķ�װ   �������ؾ�̬�����Ķ�ȡ������   */
/** 
*@Description: ֻ������ȡ��ֵ   ���ڲ���   ��װ
*@paramreq-   
*@paramind- 
*@return ��ǰ��ֵ
*/
uint32 ReadKeyValue(void)
{
	uint32 value = KEYVALUE;
	return (value);
}


/**
*@Description: ֻ�������ü�ֵ
*@paramreq-   value:Ҫ���õİ���ֵ������ ɨ��õ��İ���ֵ
*@paramind- 
*@return 
*/
void WriteKeyValue(uint32 value)
{
	KEYVALUE = value;
}



/**
*@Description: ��ȡԶ����״̬��־
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



