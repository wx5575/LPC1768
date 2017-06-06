/**
*@file			
*@brief			LCD ����������LCD��������д�ַ�����
*@version		
*@author		
*@date		
*/
#include "lcd.h"
#include "bsp_gpio.h"



/******************************************************************************
 *                            ���ļ��ڲ��궨��
******************************************************************************/ 
#define         WRITE_INSTRUCTION               (0x00)              // ����Һ�����ĵ�ַ����������Һ�����Ķ�д����ʱ��,дָ��
#define         READ_INSTRUCTION                (0x02)              // ��ָ��
#define         WRITE_DATA                      (0x01)              // д����

#define         LCD_RAM_ADDR_CAL(x, y)          (128 + y*64 + x)    // ����X Y�������LCD��RAM��ַ

#define         port_LCD_WRT_RD_PORT            (PORT0)             // Һ����WR RD�ź� ʵ�����������źŸ�����û���ô� ����һֱ���ڵ͵�ƽ 
#define         port_LCD_WR_BIT                 (5)
#define         port_LCD_RD_BIT                 (4)
#define         port_LCD_RS_RW_PORT             (PORT0)             // Һ����R/W RS�źŹܽ�
#define         port_LCD_RS_BIT                 (8)
#define         port_LCD_RW_BIT                 (7)
#define         port_LCD_EN_PORT                (PORT0)             // Һ����EN�źŹܽ�
#define         port_LCD_EN_BIT                 (6)
#define         port_LCD_DATA_BUS_PORT          (PORT2)             // Һ����������D0-D7 Ŀǰ�ĳ���ֻ����Ӧ��Щ��������ͬһ�˿ڵ�����λ���ϵ����
#define         port_LCD_DATA_BUS_LOW_BIT       (0)                 // ������λ��P1.18-P1.25 �����λ��Ϊ18

#define         port_LCD_EN                     (0)                 // LCD�ߵ�ƽʹ��(��Ƭ������� LCD�õ���)
#define         port_LCD_DISEN                  (1)                 // LCD�͵�ƽ����(��Ƭ������� LCD�õ���)


static uint32 ReadLcdRegister(uint32 dataType);
static void WriteLcdRegister(uint32 dataType, uint32 data);



/**
*@Description: ���ö�LCDģʽ
*@paramreq- 
*@paramind- 
*@return 
*/
static uint32 ReadLcdInstruction(void)
{
    return ReadLcdRegister(READ_INSTRUCTION);                  
}



/**
*@Description: дLCD
*@paramreq- 
*@paramind- 
*@return 
*/
void WriteLcdInstruction(uint32 instruction)
{
    uint32                  i   = 0;

    // ��æ״̬ ��ѯָ���Ĵ�������������æ״̬ �������˲�
    while (((ReadLcdInstruction() & 0x80) != 0) && (i++ < 1000));
    // ��������
    WriteLcdRegister(WRITE_INSTRUCTION,instruction);
}




/**
*@Description: 
*@paramreq- 
*@paramind- 
*@return 
*/
static uint32 ReadLcdRegister(uint32 dataType)
{
    uint32                  data;
	  uint32                  i;
    portuBASE_TYPE          portAddrVal = 0;

	// �Ĵ�������ѡ��
    BSP_GPIOValueSet(port_LCD_RS_RW_PORT, port_LCD_RS_BIT, dataType & 0x01);
    BSP_GPIOValueSet(port_LCD_RS_RW_PORT, port_LCD_RW_BIT, (dataType & 0x02) >> 1);

    portNOP();                   // ��ʱ
    portNOP();
    
    // ���ݿڷ�����Ϊ�����
    portAddrVal                 = BSP_GPIOPortDDRGet(port_LCD_DATA_BUS_PORT);
    BSP_GPIOPortDDRSet(port_LCD_DATA_BUS_PORT, portAddrVal & ~(0xFF << port_LCD_DATA_BUS_LOW_BIT));

	// Һ��������ʹ��
    BSP_GPIOValueSet(port_LCD_EN_PORT, port_LCD_EN_BIT, port_LCD_EN);
	for (i = 0; i < 5; i++);
	// ������     
    data                        = (BSP_GPIOPortValueGet(port_LCD_DATA_BUS_PORT) >> port_LCD_DATA_BUS_LOW_BIT) & 0xFF;
    // Һ������������
    BSP_GPIOValueSet(port_LCD_EN_PORT, port_LCD_EN_BIT, port_LCD_DISEN);
	
    // ���ݿڷ�����Ϊ�����
	BSP_GPIOPortDDRSet(port_LCD_DATA_BUS_PORT, portAddrVal | (0xFF << port_LCD_DATA_BUS_LOW_BIT));

	   return data;
}




/**
*@Description: ����LCD�˿�
*@paramreq- 
*@paramind- 
*@return 
*/
static void WriteLcdRegister(uint32 dataType, uint32 data)
{
    portuBASE_TYPE          i;
    portuBASE_TYPE          portVal     = 0;

      
    // �Ĵ�������ѡ�� ��Ȼʱ��ͼ��û�����ֳ������źŵ��Ⱥ��ϵ �������������������ڻ����ϵ���˳��
    // �ͻ��������ʱд���� ��ʱ����������˳���������ͬʱ�ͳ�ȥ��û���� 
    BSP_GPIOValueSet(port_LCD_RS_RW_PORT, port_LCD_RW_BIT, (dataType & 0x02) >> 1);
    BSP_GPIOValueSet(port_LCD_RS_RW_PORT, port_LCD_RS_BIT, dataType & 0x01);

    portNOP();
    portNOP();

	// Һ��������ʹ��
    BSP_GPIOValueSet(port_LCD_EN_PORT, port_LCD_EN_BIT, port_LCD_EN);
	for(i = 0; i < 5; i++);
    // д����
    portVal       = BSP_GPIOPortValueGet(port_LCD_DATA_BUS_PORT);
    portVal       = (portVal & ~(0xFF << port_LCD_DATA_BUS_LOW_BIT)) | (data << port_LCD_DATA_BUS_LOW_BIT);
    BSP_GPIOPortValueSet(port_LCD_DATA_BUS_PORT, portVal);
	// Һ������������
    BSP_GPIOValueSet(port_LCD_EN_PORT, port_LCD_EN_BIT, port_LCD_DISEN); 
}





/**
*@Description: LCDдһ����
*@paramreq- data: 16��������
*@paramind- 
*@return 
*/
void WriteLcdData(uint32 data)
{
    uint32    i = 0;

    // ��æ״̬ ��ѯָ���Ĵ�������������æ״̬ �������˲�
    while (((ReadLcdInstruction() & 0x80) != 0) && (i++ < 1000));   // ��æ״̬
    // д����
    WriteLcdRegister(WRITE_DATA,data);
}




/**
*@Description: д�ַ�����LCD
*@paramreq- buffer: �ַ���ָ��    x:��(0-19)   y: �� (0:��һ��)
*@paramind- 
*@return 
*/
void LCD_Write(unsigned  char*  buffer, unsigned short x,unsigned short y)                
{
    WriteLcdInstruction(LCD_RAM_ADDR_CAL(x, y)); 
	
	while(*buffer != '\0')                   // �ַ�����β
	{
				WriteLcdData(*((char *)buffer));   // дһ���ַ�
        portNOP();
        portNOP();
		    buffer++;
	}
	
}




/**
*@Description: LCD��ʼ��  �ȳ�ʼ������������
*@paramreq- 
*@paramind- 
*@return 
*/
int16 Lcd_Init(void)
{
    portBASE_TYPE       i;

    // ���� LCD �������� D0-D7 Ϊ����� ����������ʹ��
    for (i = 0; i < 8; i++)
    {
        BSP_GPIODirectionSet(port_LCD_DATA_BUS_PORT, port_LCD_DATA_BUS_LOW_BIT + i, DIR_OUTPUT, PULL_UP_RES);
    }

    // ���� WR RD �źŹܽ�Ϊ��� ����������ʹ��
    BSP_GPIODirectionSet(port_LCD_WRT_RD_PORT, port_LCD_WR_BIT, DIR_OUTPUT, PULL_UP_RES);
    BSP_GPIODirectionSet(port_LCD_WRT_RD_PORT, port_LCD_RD_BIT, DIR_OUTPUT, PULL_UP_RES);

    // ���ÿ���Һ���� RS RW ���ŵĹܽ�Ϊ��� ����������ʹ��    
    BSP_GPIODirectionSet(port_LCD_RS_RW_PORT, port_LCD_RS_BIT, DIR_OUTPUT, PULL_UP_RES);
    BSP_GPIODirectionSet(port_LCD_RS_RW_PORT, port_LCD_RW_BIT, DIR_OUTPUT, PULL_UP_RES);

    // ���ÿ���Һ���� EN ���ŵĹܽ�Ϊ��� ����������ʹ��
    BSP_GPIODirectionSet(port_LCD_EN_PORT, port_LCD_EN_BIT, DIR_OUTPUT, PULL_UP_RES);

    // WR �� RD�ź���Ϊ�͵�ƽ ���Ժ�����ı�
    BSP_GPIOValueSet(port_LCD_WRT_RD_PORT, port_LCD_WR_BIT, 0);
    BSP_GPIOValueSet(port_LCD_WRT_RD_PORT, port_LCD_RD_BIT, 0);
		
    return RT_EOK;
}




/**
*@Description: LCD���� , ֻ�п�����LCD����д
*@paramreq- 
*@paramind- 
*@return 
*/
int16 Lcd_Open(void)
{
    // ����
    WriteLcdInstruction(CLR_LCD_CMD_CODE);
	// ������������
    WriteLcdInstruction(FUNC_SET_CMD);
    // ��ʾģʽ����
    WriteLcdInstruction(DISP_CTRL_CMD);
	// ����ģʽ����
    WriteLcdInstruction(ENTER_MODE_CMD);
	
    #if (LCD_DEBUG_SERV > 0)
        
        // 128 + y*64 + x
        WriteLcdInstruction(128 + 4);                                   
    #endif
    return RT_EOK;
}



/**
*@Description: LCD����ַ�
*@paramreq-    x:�ڼ���    y: ��   num: ����ĸ���
*@paramind- 
*@return 
*/
void LCD_Empty(uint8 x, uint8 y, uint8 num)
{
	uint8 i;
	for(i=0;i<=num; i++)
	{
		LCD_Write(" ", x+i, y);
	}
}


/**
*@Description: ����
*@paramreq-    
*@paramind- 
*@return 
*/
void LCD_Clear(void)
{
	WriteLcdInstruction(CLR_LCD_CMD_CODE);
}


