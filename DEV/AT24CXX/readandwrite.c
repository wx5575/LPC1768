#include "readandwrite.h"
#include "Delay.h"
#include "datavalue.h"




/******************************************************************************
 *                           �豸��ֲ�ӿں궨��
******************************************************************************/

#define             port_WP_PORT                        (PORT2)
#define             port_WP_BIT                         (9)

#define             port_SDA_PORT                       (PORT0)
#define             port_SDA_BIT                        (19)

#define             port_SCL_PORT                       (PORT0)
#define             port_SCL_BIT                        (20)

/******************************************************************************
 *                          ���ļ��ڲ��꺯������  
******************************************************************************/

// �ߵ�ƽʱд������������
#define             WP_PORT_INIT()                      BSP_GPIODirectionSet(port_WP_PORT, port_WP_BIT, DIR_OUTPUT, PULL_UP_RES)
#define             WP_PORT_STATE_GET()                 BSP_GPIOValueGet(port_WP_PORT, port_WP_BIT)
#define             WP_EN_HIGH()                        BSP_GPIOValueSet(port_WP_PORT, port_WP_BIT, 1)
#define             WP_DISEN_LOW()                      BSP_GPIOValueSet(port_WP_PORT, port_WP_BIT, 0)

// SDA���ڲ�ͬ�����׶�Ҫ��̬�����������״̬
#define             SDA_PORT_INIT_OUTPUT()              BSP_GPIODirectionSet(port_SDA_PORT, port_SDA_BIT, DIR_OUTPUT, NONE_RES)
#define             SDA_HIGH()                          BSP_GPIOValueSet(port_SDA_PORT, port_SDA_BIT, 1)
#define             SDA_LOW()                           BSP_GPIOValueSet(port_SDA_PORT, port_SDA_BIT, 0)
#define             SDA_PORT_INIT_INPUT()               BSP_GPIODirectionSet(port_SDA_PORT, port_SDA_BIT, DIR_INPUT, NONE_RES)
#define             SDA_STATE_GET()                     BSP_GPIOValueGet(port_SDA_PORT, port_SDA_BIT)

#define             SCL_PORT_INIT()                     BSP_GPIODirectionSet(port_SCL_PORT, port_SCL_BIT, DIR_OUTPUT, NONE_RES)
#define             SCL_HIGH()                          BSP_GPIOValueSet(port_SCL_PORT, port_SCL_BIT, 1)
#define             SCL_LOW()                           BSP_GPIOValueSet(port_SCL_PORT, port_SCL_BIT, 0)

//---------------------------------------------------------------------------//

#define             IIC_WRITE_FLG                       (0x00)          // д 
#define             IIC_READ_FLG                        (0x01)          // ��

#define             IIC_DEVICE_ACK_SUCCESS              (1)
#define             IIC_DEVICE_ACK_FAIL                 (0)

//---------------------------------------------------------------------------//

#define             I2C_NONE                            (0)             // �ӵ�ַ�Ѿ�������߲���Ҫ�ӵ�ַ
#define             I2C_READ                            (1)             // ��ȡ����
#define             I2C_WRITE                           (2)             // д����
#define             I2C_ABNORMAL                        (0xff)          // I2C�쳣

//---------------------------------------------------------------------------//

#define             AT24C02                             (0)
#define             AT24C04                             (1)
#define             AT24C08                             (2)
#define             AT24C16                             (3)
#define             AT24C32                             (4)
#define             AT24C64                             (5)
#define             AT24C128                            (6)
#define             AT24C256                            (7)
#define             AT24C512                            (8)
#define             AT24C1024                           (9)

#define             DEVICE_TYPE                         (AT24C256)


#define             I2C_DEBUG                           (1)

/******************************************************************************
 *                       ���ļ�������ľ�̬ȫ�ֱ���
******************************************************************************/

static uint8   curr_proc_dev_index                 = 0;    // ��ǰ��������������
static uint8   curr_dev_type                       = 0;    // ��ǰ������������

/******************************************************************************
 *                           ���ļ���̬��������
******************************************************************************/

//rt_err_t at24cxx_control(rt_uint8_t cmd, void *args);
uint8 AT24Cxx_Control(uint8 cmd);
/******************************************************************************
 *                       ���ļ�������ľ�̬���ݽṹ
******************************************************************************/

typedef enum  SUB_ADDR_TYPE
{
    ADDR_ONE_BYTE               = 0,
    ADDR_P0_WITH_ONE_BYTE,
    ADDR_P1_P0_WITH_ONE_BYTE,
    ADDR_P2_P1_P0_WITH_ONE_BYTE,
    ADDR_TWO_BYTES,
    ADDR_P0_WITH_TWO_BYTES,

}SubAddrType_TYPE;

typedef struct
{
    uint16                            m_pageNumbs;            // ҳ��
    uint16                             m_pageSize;             // ҳ��С
    SubAddrType_TYPE                  m_subAddrType;          // �����ֵ�ַ����
    uint8                             m_parallelNumbs;        // ��������

}AT24CXXPhyInfo;                 

// ������Ϣ����curr_dev_type����   
static AT24CXXPhyInfo t_AT24CXXPhyInfo[]    = {        

//#if (DEVICE_TYPE == AT24C02) 
    {32 , 8  , ADDR_ONE_BYTE              , 8},//0
//#elif   (DEVICE_TYPE == AT24C04)
    {32 , 16 , ADDR_P0_WITH_ONE_BYTE      , 4}, //1
//#elif   (DEVICE_TYPE == AT24C08)
    {64 , 16 , ADDR_P1_P0_WITH_ONE_BYTE   , 2},//2
//#elif   (DEVICE_TYPE == AT24C16)
    {128, 16 , ADDR_P2_P1_P0_WITH_ONE_BYTE, 1},//3
//#elif   (DEVICE_TYPE == AT24C32)
    {128, 32 , ADDR_TWO_BYTES             , 8},//4
//#elif   (DEVICE_TYPE == AT24C64)
    {256, 32 , ADDR_TWO_BYTES             , 8},//4
//#elif   (DEVICE_TYPE == AT24C128)
    {256, 64 , ADDR_TWO_BYTES             , 4},//4
//#elif   (DEVICE_TYPE == AT24C256)
    {512, 64 , ADDR_TWO_BYTES             , 4},//4
//#elif   (DEVICE_TYPE == AT24C512)
    {512, 128, ADDR_TWO_BYTES             , 4},//4
//#elif   (DEVICE_TYPE == AT24C1024)
    {512, 256, ADDR_P0_WITH_TWO_BYTES     , 2},//5
//#endif
};

typedef struct
{
    uint16                            m_accessAddr;               // ��ȡ��ַ ���洢���洢��Ԫ��ַ
    uint8                             m_slaAddr;                  // I2C������ַ
    uint8                             m_accessNumbBytes;          // ��ȡ�ֽ���
    uint8                             m_accessFinished;           // ��ȡ���
    uint8                             m_accessCtrl;               // ��ȡ����
    uint8                             m_accessAddrByteNumbs;      // ��ȡ��ַ�ֽ��� ��ͬ����������ͬ ��ַ���ֽ���Ҳ��ͬ
    uint8                            *m_paccessAddr;              // ��ȡ��ַ

}I2CCtrlInfo;                          

static volatile I2CCtrlInfo   t_I2CCtrlInfo = {0};

//---------------------------------------------------------------------------//

/**
 *@Description: ���� I2C   ���ģ�� I2C 
 *@paramreq- ��
 *@paramind- 
 *@return ��
 */
static void I2CStart(void)
{
	// ʱ��Ϊ���ڼ� �½���
  SDA_PORT_INIT_OUTPUT();  // SDA �˿�Ϊ ���
  portNOP();
  SDA_HIGH();
	SCL_HIGH();
	SoftDelayUs(4);
	SDA_LOW();
	SoftDelayUs(4);
	SCL_LOW();
  SoftDelayUs(4);    
}



/**
 *@Description: ���� I2C   ֹͣ�ź�
 *@paramreq- ��
 *@paramind- 
 *@return ��
 */
static void I2CStop()
{
	// ʱ��Ϊ���ڼ� ������
  SDA_LOW();
	SCL_HIGH();
	SoftDelayUs(4);
	SDA_HIGH();
	SoftDelayUs(4);
  SCL_LOW();
	SoftDelayUs(4);
}


/**
 *@Description: �ȴ����������շ���Ӧ�� 
 *@paramreq- ��
 *@paramind- 
 *@return ��
 */
static uint8_t WaitAck(void)
{
	uint8_t errtime =255;           // ����Ͻ��շ���ACK����ʱֵΪ255��

	SCL_HIGH();
	SoftDelayUs(4);
	// SDA����Ϊ����
    SDA_PORT_INIT_INPUT();
    SoftDelayUs(4);
    // SDA����Ϊ0ʱ,��Ϊ��Ӧ�� 
	while(SDA_STATE_GET())
	{
		errtime--;
		// errtime�Ƿ�Ϊ0
    if(!errtime)
		{
			I2CStop();
			return  IIC_DEVICE_ACK_FAIL;			   //����Ϊ0(ʧ��) 
		}
	}
	SDA_PORT_INIT_OUTPUT();  // SDA����Ϊ���
	SCL_LOW();
	SoftDelayUs(4);

	return IIC_DEVICE_ACK_SUCCESS;
}

/**
 *@Description: //������Ϊ���շ���������Ϊ���ͷ�ʱ����Ƭ������Ӧ���ź� 
 *@paramreq- ��
 *@paramind- 
 *@return ��
 */
static void SendAck(void)
{   
	SDA_LOW();
  SoftDelayUs(4);
  SCL_HIGH();
    
  SoftDelayUs(4);
  SCL_LOW();
	SoftDelayUs(4);
}


/**
 *@Description: //������Ϊ���շ���������Ϊ���ͷ�ʱ����Ӧ���ź� 
 *@paramreq- ��
 *@paramind- 
 *@return ��
 */
static void SendNoAck(void)
{
	SDA_HIGH();
	SoftDelayUs(4);
	SCL_HIGH();
	SoftDelayUs(4);
	SCL_LOW();
  SoftDelayUs(4);
}



/**
 *@Description: д����  // ����������д��
 *@paramreq- ��
 *@paramind- 
 *@return ��
 */
static void I2CSendByte(uint8_t date)
{
	uint8_t i;

  for (i = 0; i < 8; i++)
	{
	  if (date & 0x80)
		{
			SDA_HIGH();
		}
		else
		{
			SDA_LOW();
		}
		
		date <<= 1;
		SoftDelayUs(4);
		SCL_HIGH();
		SoftDelayUs(4);
		SCL_LOW();
		SoftDelayUs(4);
	}
}



/**
 *@Description: ������
 *@paramreq- 
 *@paramind- 
 *@return ��
 */
static uint8_t I2CReceiveByte(void)
{
	  uint8_t data = 0;
    uint8_t i;
    
    SDA_PORT_INIT_INPUT(); // �л�SDA��Ϊ����״̬
    SoftDelayUs(4);
   
  for (i = 0; i < 8; i++)
	{
		data <<= 1;
		SCL_HIGH();
		SoftDelayUs(4);
		SoftDelayUs(4);
		if (1 == SDA_STATE_GET())  // �½������ݶ���
		{
			data |= 0x01;
		}
		SCL_LOW();
		SoftDelayUs(4);
	}
	SDA_PORT_INIT_OUTPUT();  // �л�SDA��Ϊ���״̬
  SoftDelayUs(4);
	
	return data;
}





/**
 *@Description: дһҳ����    AT24C512 һҳΪ128�ֽ�
 *@paramreq- slaAddr: AT24CXX ��ַ (C512Ϊ 00)    subAddr: �ڴ��ַ   acessAddr: ����ָ��    numbBytes: ���ݸ���
 *@paramind- 
 *@return ��
 */
static uint32 _I2CPageWrite(uint8_t slaAddr, uint16_t subAddr, uint8_t *acessAddr, uint8_t numbBytes)
{
    uint8 writtenBytes = 0;

    I2CStart();
    // ����������ַ��д��־
    I2CSendByte(slaAddr | IIC_WRITE_FLG);
    if(IIC_DEVICE_ACK_SUCCESS != WaitAck())
    {
			return writtenBytes;
    }

    if (t_AT24CXXPhyInfo[curr_dev_type].m_subAddrType >= ADDR_TWO_BYTES)
    {
        // ���͵�ַ���ֽ�
        I2CSendByte(subAddr >> 8);
        if(IIC_DEVICE_ACK_SUCCESS != WaitAck())
        {
					return writtenBytes;
        }
    }
    // ���͵�ַ���ֽ�
    I2CSendByte(subAddr & 0x00FF);
    if(IIC_DEVICE_ACK_SUCCESS != WaitAck())
    {
			return writtenBytes;
    }

    while (numbBytes--)
    {
        I2CSendByte(*acessAddr++);
        if(IIC_DEVICE_ACK_SUCCESS != WaitAck())
        {
					return writtenBytes;
        }
        writtenBytes++;
    }
    I2CStop();

    // �����20ms��ʱΪ�������ڲ����ʱ��
    SoftDelayMs(20);

    return writtenBytes;
}




/**
 *@Description: ����������
 *@paramreq- slaAddr: AT24CXX ��ַ (C512Ϊ 00)    subAddr: �ڴ��ַ   acessAddr: ����ָ��    numbBytes: ���ݸ���
 *@paramind- 
 *@return ��
 */
static uint32 _I2CSequentialRead(uint8_t slaAddr, uint16_t subAddr, uint8_t *acessAddr, uint32_t numbBytes)
{
    uint32 readBytes = numbBytes;

    I2CStart();
    // ����������ַ��д��־
    I2CSendByte(slaAddr | IIC_WRITE_FLG);
    if(IIC_DEVICE_ACK_SUCCESS != WaitAck())
    {
			return 0;
    }

    if(t_AT24CXXPhyInfo[curr_dev_type].m_subAddrType >= ADDR_TWO_BYTES)
    {
        // ���͵�ַ���ֽ�
        I2CSendByte(subAddr >> 8);
        if(IIC_DEVICE_ACK_SUCCESS != WaitAck())
        {
					return 0;
        }
    }
    // ���͵�ַ���ֽ�
    I2CSendByte(subAddr & 0x00FF);
    if(IIC_DEVICE_ACK_SUCCESS != WaitAck())
    {
			return 0;
    }

    I2CStart();
    // ����������ַ�Ӷ���־
    I2CSendByte(slaAddr | IIC_READ_FLG);
    if(IIC_DEVICE_ACK_SUCCESS != WaitAck())
    {
			return 0;
    }

    while (numbBytes)
    {
        *acessAddr++ = I2CReceiveByte();
        if (1 == numbBytes)
        {
					SendNoAck();
        } 
        else
        {
					SendAck();
        }
        numbBytes--;
    }

    I2CStop();
    return readBytes;
}






/**
 *@Description: ��д����  
 *@paramreq- accessAddr: �ڴ��ַ      buffer: ����ָ��   numbBytes: ��д�ĸ���  accessCtrl: ��д��־
 *@paramind- 
 *@return ��
 */
static uint32 I2CInnerAccess(uint32 accessAddr, uint8 *buffer, uint32 numbBytes, uint32 accessCtrl)
{
    uint32  actualProBytes  = 0; // ��д�ɹ������ַ���
    uint16  currStepActualProBytes  = 0; // ��ǰ����д�ɹ����ֽ���
    uint16  accessBytes; // ��ǰҳ��д���ַ���
    uint16  currPageRemainedBytes; // ��ǰҳʣ��
    
    //�жϲ����Ƿ�Ϸ�
    if(0 == numbBytes)
    {
			return actualProBytes;
    }
    // �ж��ֵ�ַ�Ƿ�Ϸ�
    if (accessAddr > t_AT24CXXPhyInfo[curr_dev_type].m_pageNumbs*t_AT24CXXPhyInfo[curr_dev_type].m_pageSize)
    {
			return actualProBytes;
    }

    
    /** �����һ��д����ʱ��д��ҳ����д����ֽ��� ��Ϊ�漰��ҳ�ڵ�ַ��ת���� �ȼ����������ֽ� */
    currPageRemainedBytes  = accessAddr % t_AT24CXXPhyInfo[curr_dev_type].m_pageSize; // һҳʣ����
    currPageRemainedBytes  = t_AT24CXXPhyInfo[curr_dev_type].m_pageSize - currPageRemainedBytes; // ҳ�Ŀ�ʼ�ֽ�

    for(; numbBytes > 0; )
    {
        //����Ҫ�������ֽ���
        if(numbBytes > currPageRemainedBytes)
        {
					accessBytes = currPageRemainedBytes; // ���Ҫд����������ҳʣ����,��д���ҳ
        }
        else
        {
					accessBytes = numbBytes;
        }
        
        //I2Cʵ�ʲ��������
        {
					//�ӵ�ַ�����ж�
					if (t_AT24CXXPhyInfo[curr_dev_type].m_subAddrType == ADDR_ONE_BYTE)
          {
						t_I2CCtrlInfo.m_accessAddr 	= accessAddr;		 // ��ȡ��ַ 			
        		t_I2CCtrlInfo.m_accessAddrByteNumbs = 1;		// ��ȡ��ַΪ1�ֽ�	
        	}
        	else if (t_AT24CXXPhyInfo[curr_dev_type].m_subAddrType == ADDR_P0_WITH_ONE_BYTE)
        	{	
        		t_I2CCtrlInfo.m_slaAddr = (uint8_t)(t_I2CCtrlInfo.m_slaAddr | ((accessAddr >> 7) & 0x02));  // ���ºϳ�������ַ
        		t_I2CCtrlInfo.m_accessAddr = (uint8_t)accessAddr;   // ��ȡ��ַ 			
        		t_I2CCtrlInfo.m_accessAddrByteNumbs = 1;    // ��ȡ��ַΪ1�ֽ� 	
        	}
        	else if (t_AT24CXXPhyInfo[curr_dev_type].m_subAddrType == ADDR_P1_P0_WITH_ONE_BYTE)
        	{	
        		t_I2CCtrlInfo.m_slaAddr  = (uint8_t)(t_I2CCtrlInfo.m_slaAddr | ((accessAddr >> 7) & 0x06));
        		t_I2CCtrlInfo.m_accessAddr 	 = (uint8_t)accessAddr; 			
        		t_I2CCtrlInfo.m_accessAddrByteNumbs   = 1; 	 	
        	}
            else if(t_AT24CXXPhyInfo[curr_dev_type].m_subAddrType == ADDR_P2_P1_P0_WITH_ONE_BYTE)
            {
							t_I2CCtrlInfo.m_slaAddr = (uint8_t)(t_I2CCtrlInfo.m_slaAddr | ((accessAddr >> 7) & 0x0E));
        		  t_I2CCtrlInfo.m_accessAddr 	= (uint8_t)accessAddr; 			
        		  t_I2CCtrlInfo.m_accessAddrByteNumbs = 1;
            }
            else if (t_AT24CXXPhyInfo[curr_dev_type].m_subAddrType == ADDR_TWO_BYTES)
            {
                t_I2CCtrlInfo.m_accessAddr = accessAddr; 			
        		t_I2CCtrlInfo.m_accessAddrByteNumbs = 2;
            }
            else if (t_AT24CXXPhyInfo[curr_dev_type].m_subAddrType == ADDR_P0_WITH_TWO_BYTES)
            {
							t_I2CCtrlInfo.m_slaAddr = (uint8_t)(t_I2CCtrlInfo.m_slaAddr | ((accessAddr >> 15) & 0x02));
        		  t_I2CCtrlInfo.m_accessAddr 	= accessAddr; 			
        		  t_I2CCtrlInfo.m_accessAddrByteNumbs = 2;
            }
            t_I2CCtrlInfo.m_paccessAddr      = buffer;        // ���ݶ�дָ��
            t_I2CCtrlInfo.m_accessNumbBytes  = accessBytes;   // Ҫ��ȡ�ĸ���
            t_I2CCtrlInfo.m_accessFinished   = FALSE;
            t_I2CCtrlInfo.m_accessCtrl       = accessCtrl;
        
            if (I2C_WRITE == accessCtrl) 
            {
							AT24Cxx_Control(IO_CTRL_CMD_I2C_WP_OFF); // �ر�д���� д����Ҫ��WP���õ͵�ƽ,����д
              currStepActualProBytes = _I2CPageWrite(t_I2CCtrlInfo.m_slaAddr, accessAddr , buffer, accessBytes); // д�ַ�
            }
            else
            {
							currStepActualProBytes = _I2CSequentialRead(t_I2CCtrlInfo.m_slaAddr, accessAddr, buffer, accessBytes); // ���ַ�
            }

            actualProBytes +=  currStepActualProBytes; // ��д�ɹ�  ���ֽ����ۼ�
						
            /** ��û�гɹ�д��ָ�����ֽ��������� ����������������� ������ʵ�ʲ������ֽ��� */
            if (accessBytes != currStepActualProBytes)
            {
							break;
            } 	
        
            if (I2C_WRITE == accessCtrl)
            {
							//����д����
							AT24Cxx_Control(IO_CTRL_CMD_I2C_WP_ON); // WP�øߵ�ƽ
            }
        }
        numbBytes  -= accessBytes;  // ʣ��û�в������ֽ���
        accessAddr += currPageRemainedBytes; // ������ַ�ƶ�
        buffer     += currPageRemainedBytes; // ����ָ���ƶ�
        currPageRemainedBytes = t_AT24CXXPhyInfo[curr_dev_type].m_pageSize;
    }
    return actualProBytes;
}







/**
 *@Description: AT24Cxx ����  , ��д����   ����������ַ
 *@paramreq- cmd: ������
 *@paramind- 
 *@return ��
 */
uint8 AT24Cxx_Control(uint8 cmd)
{
	uint8 rt = RT_EOK;
    switch (cmd)              
    {
			case IO_CTRL_CMD_I2C_WP_ON:
				if (SET_PARAM_I2C_DEV == curr_proc_dev_index)
				{
					WP_EN_HIGH();  // д����
        }
        break;

        case IO_CTRL_CMD_I2C_WP_OFF:
					if(SET_PARAM_I2C_DEV == curr_proc_dev_index)
          {
						WP_DISEN_LOW(); // ��д
          }
          break;

        case IO_CTRL_CMD_I2C_DEV_INDEX_SET:
            if(CAL_DATA_I2C_DEV == curr_proc_dev_index)
            {
							t_I2CCtrlInfo.m_slaAddr  = 0xA2;
							// ��ΪУ׼������AT24C256�����ò���ʹ��AT2C512������Ƭѡ��ͬʱҪָ����ǰʹ�õ���������
              curr_dev_type = AT24C256;
            }
            else if(SET_PARAM_I2C_DEV == curr_proc_dev_index)
            {
                t_I2CCtrlInfo.m_slaAddr  = 0xA0;
                // ��ΪУ׼������AT24C256�����ò���ʹ��AT2C512������Ƭѡ��ͬʱҪָ����ǰʹ�õ���������
                curr_dev_type  = AT24C512;
            }
            break;

				default:
					rt = RT_ERROR;
          break;
    }
    return rt;
}





// ������
#if(I2C_DEBUG > 0)
//    uint8 a[10] = {0x22,0x21,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x30};
//    uint8 b[10] = {0};
uint8 a[2] = {0x22,0x21};
uint8 b[2] = {0};
#endif
/**
 *@Description: ����I2C   ����Ƿ���� AT24Cxx       
 *@paramreq-  
 *@paramind- 
 *@return   1: ����    0: ʧ��
 */ 
uint8 AT24Cxx_Check(void)
{
	uint16 flag = 0;
	WP_PORT_INIT(); // �˿�����
  SCL_PORT_INIT();

	curr_proc_dev_index = SET_PARAM_I2C_DEV ;
// ������
#if (I2C_DEBUG > 0) 
  AT24Cxx_Control(IO_CTRL_CMD_I2C_DEV_INDEX_SET);
  flag = I2CInnerAccess(0, a, 2, I2C_WRITE); 
//	flag = AT24Cxx_Write(0, a, 10);
	if(flag != 0)
	{
		flag = 0;
	}
  flag = I2CInnerAccess(0, b, 2, I2C_READ);
//	flag = AT24Cxx_Read(0, b, 10);
	if(flag != 0)
	{
		flag = 1;
		return 1;
	}
	
#endif 
	return RT_EOK;
}

/********************  ����  **********************/
/**
 *@Description:  AT24Cxx  ��ʼ��       
 *@paramreq-  cmd: AT24Cxx ���Ʋ���          args: AT24Cxx �ĵ�ַ
 *@paramind- 
 *@return   1: ����    0: ʧ��
 */ 
uint8 AT24Cxx_Init(uint8 cmd, void *args)       //  ���� AT24Cxx ʲôʱ�������
{
	WP_PORT_INIT();
  SCL_PORT_INIT();
	if(cmd == IO_CTRL_CMD_I2C_DEV_INDEX_SET)
	{
		curr_proc_dev_index  = (uint8)((uint32)args);
	}
	AT24Cxx_Control(cmd);
	
	return RT_EOK;
}



/**
 *@Description:  AT24Cxx ������      
 *@paramreq-  addr: ��ȡ��ʼ��ַ    buffer: ��ȡ���������������ָ��  size: Ҫ��ȡ�����ݸ���
 *@paramind- 
 *@return   1: ����    0: ʧ��
 */
uint32 AT24Cxx_Read(uint32 addr, void* buffer, uint32 size)
{
	uint32 count = 0;
	count = I2CInnerAccess(addr, buffer, size, I2C_READ);
	return count;
}




/**
 *@Description: д���ݵ� AT24Cxx      
 *@paramreq-  addr: ��ʼд�ĵ�ַ    buffer: Ҫд������ָ��  size: д�����ݸ���
 *@paramind- 
 *@return   1: ����    0: ʧ��
 */
uint32 AT24Cxx_Write(uint32 addr, uint8* buffer, uint32 size)
{
	uint32 count = 0;
	count = I2CInnerAccess(addr, buffer, size, I2C_WRITE);
	return count;
}







/**
 *@Description: ������ֵ 
 *@paramreq-  res: ������� BCD ���ָ��    max: �������ֵ  value: ���뱣���ֵ
 *@paramind- 
 *@return   1: ����    0:����     2: ������Χ
 */
uint8 CheckResValue(uint8* res, float max, float* value)
{
	float temp = 0;
	if(res[0] == 0xFF && res[1] == 0xFF && res[2] == 0xFF && res[3] == 0xFF)
	{
		return 0; // ������־
	}
	else
	{
		temp = BCDCodeToFloat(res);// ����
		if(temp>max)
		{
			return 2;
		}
		else
		{
			*value = temp;
			return 1;
		}
	}
}





/**
 *@Description: �����Ե������ݲ����ж��Ƿ����   
 * ������־����4�� : 0xFF һҳ����99 float , 99*4��uint8.����4��uint8��Ϊ������־  �ܹ�99*4+4
 *@paramreq- res: ����һҳ���Ե������ݵ�ָ��   addr: ҳ��ʼ��ַ   num: һ������ҳ��������
 *@paramind-  value: Ĭ��ֵ    max: �������ֵ
 *@return ��ȡ���������
 */
uint8 ReadResValue(float* res, float value, float max, uint32 addr, uint8 num)
{
//	uint8 count = 0; // ��¼��ȡ�����ݸ���  
	uint8 i = 0;
	uint8 flag = 0; // ��д�ɹ���־
	uint32 addr_temp = addr; // ���ݿ�ʼ��ַ
	uint8 temp[4] = {0,0,0,0};// ���� BCD �� �������ʱ����

  for(i=0;i<num;i++)
  {
		flag = AT24Cxx_Write(addr_temp, temp, 4); // ������
		if(flag != 0) // ���ɹ�
		{
			flag = 0;
			flag = CheckResValue(temp, max, res+i); // �������

			if(flag == 0)
			{
				return i; // ����
			}
			else if(flag == 2)
			{
				*(res+i) = value; // ��Ĭ��ֵ
				FloatToBCDCode(temp,value); // ��Ĭ��ֵ����
				AT24Cxx_Read(addr_temp, temp, 4); // ���˴�д��Ĭ��ֵ
			}
			else
			{
			}
			flag = 0; 			
		}
		else
		{
			flag = AT24Cxx_Write(addr_temp, temp, 4); // �ٶ�һ��
			if(flag == 0)
			{
				return i; // ����
			}
			else
			{
				if(flag == 0)
			  {
					return i; // ����
			  }
				else if(flag == 2)
			  {
					*(res+i) = value; // ��Ĭ��ֵ
					FloatToBCDCode(temp,value); // ��Ĭ��ֵ����
					AT24Cxx_Read(addr_temp, temp, 4); // ���˴�д��Ĭ��ֵ
			  }
				else
				{
        }
				flag = 0;
			}
		}
		addr_temp = 4 + addr_temp; // ָ����һ����ַ
	}
	return i;
}






/**
 *@Description:  ����ʱ��ȡsystem ����     ���������ڵ���ҳ ��ʼ��ַΪ  256
 *@paramreq- sysstr: ��������ֵ    sys_default:����Ĭ��ֵ    max:���������ֵ    num: ��������
 *@paramind- 
 *@return   1: ����    0: ʧ��
 */
uint8 ReadSystem(uint8* sysstr, uint8* sys_default, uint8* max, uint8 num)
{
	uint8 i = 0;
	uint8 flag = 0;
	uint32 addr = 256;
	
	for(i=0;i<num;i++)
	{
		addr = addr + i;
		flag = AT24Cxx_Read(addr, sysstr+i, 1);
		if(flag != 0)
		{
			if(sysstr[i] > max[i])
			{
				sysstr[i] = sys_default[i];
				AT24Cxx_Write(addr, sys_default+i, 1);
			}
			flag = 0;
		}
		else
		{
			sysstr[i] = sys_default[i];
			AT24Cxx_Write(addr, sys_default+i, 1);
		}
	}
	return 1;
}












