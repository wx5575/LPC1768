#include "readandwrite.h"
#include "Delay.h"
#include "datavalue.h"




/******************************************************************************
 *                           设备移植接口宏定义
******************************************************************************/

#define             port_WP_PORT                        (PORT2)
#define             port_WP_BIT                         (9)

#define             port_SDA_PORT                       (PORT0)
#define             port_SDA_BIT                        (19)

#define             port_SCL_PORT                       (PORT0)
#define             port_SCL_BIT                        (20)

/******************************************************************************
 *                          本文件内部宏函数定义  
******************************************************************************/

// 高电平时写操作将被忽略
#define             WP_PORT_INIT()                      BSP_GPIODirectionSet(port_WP_PORT, port_WP_BIT, DIR_OUTPUT, PULL_UP_RES)
#define             WP_PORT_STATE_GET()                 BSP_GPIOValueGet(port_WP_PORT, port_WP_BIT)
#define             WP_EN_HIGH()                        BSP_GPIOValueSet(port_WP_PORT, port_WP_BIT, 1)
#define             WP_DISEN_LOW()                      BSP_GPIOValueSet(port_WP_PORT, port_WP_BIT, 0)

// SDA脚在不同操作阶段要动态调整输入输出状态
#define             SDA_PORT_INIT_OUTPUT()              BSP_GPIODirectionSet(port_SDA_PORT, port_SDA_BIT, DIR_OUTPUT, NONE_RES)
#define             SDA_HIGH()                          BSP_GPIOValueSet(port_SDA_PORT, port_SDA_BIT, 1)
#define             SDA_LOW()                           BSP_GPIOValueSet(port_SDA_PORT, port_SDA_BIT, 0)
#define             SDA_PORT_INIT_INPUT()               BSP_GPIODirectionSet(port_SDA_PORT, port_SDA_BIT, DIR_INPUT, NONE_RES)
#define             SDA_STATE_GET()                     BSP_GPIOValueGet(port_SDA_PORT, port_SDA_BIT)

#define             SCL_PORT_INIT()                     BSP_GPIODirectionSet(port_SCL_PORT, port_SCL_BIT, DIR_OUTPUT, NONE_RES)
#define             SCL_HIGH()                          BSP_GPIOValueSet(port_SCL_PORT, port_SCL_BIT, 1)
#define             SCL_LOW()                           BSP_GPIOValueSet(port_SCL_PORT, port_SCL_BIT, 0)

//---------------------------------------------------------------------------//

#define             IIC_WRITE_FLG                       (0x00)          // 写 
#define             IIC_READ_FLG                        (0x01)          // 读

#define             IIC_DEVICE_ACK_SUCCESS              (1)
#define             IIC_DEVICE_ACK_FAIL                 (0)

//---------------------------------------------------------------------------//

#define             I2C_NONE                            (0)             // 子地址已经处理或者不需要子地址
#define             I2C_READ                            (1)             // 读取操作
#define             I2C_WRITE                           (2)             // 写操作
#define             I2C_ABNORMAL                        (0xff)          // I2C异常

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
 *                       本文件所定义的静态全局变量
******************************************************************************/

static uint8   curr_proc_dev_index                 = 0;    // 当前操作器件索引号
static uint8   curr_dev_type                       = 0;    // 当前操作器件类型

/******************************************************************************
 *                           本文件静态函数声明
******************************************************************************/

//rt_err_t at24cxx_control(rt_uint8_t cmd, void *args);
uint8 AT24Cxx_Control(uint8 cmd);
/******************************************************************************
 *                       本文件所定义的静态数据结构
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
    uint16                            m_pageNumbs;            // 页数
    uint16                             m_pageSize;             // 页大小
    SubAddrType_TYPE                  m_subAddrType;          // 数据字地址类型
    uint8                             m_parallelNumbs;        // 并联个数

}AT24CXXPhyInfo;                 

// 器件信息表，用curr_dev_type索引   
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
    uint16                            m_accessAddr;               // 存取地址 即存储器存储单元地址
    uint8                             m_slaAddr;                  // I2C器件地址
    uint8                             m_accessNumbBytes;          // 存取字节数
    uint8                             m_accessFinished;           // 存取完成
    uint8                             m_accessCtrl;               // 存取控制
    uint8                             m_accessAddrByteNumbs;      // 存取地址字节数 不同器件容量不同 地址的字节数也不同
    uint8                            *m_paccessAddr;              // 存取地址

}I2CCtrlInfo;                          

static volatile I2CCtrlInfo   t_I2CCtrlInfo = {0};

//---------------------------------------------------------------------------//

/**
 *@Description: 启动 I2C   软件模拟 I2C 
 *@paramreq- 无
 *@paramind- 
 *@return 无
 */
static void I2CStart(void)
{
	// 时钟为高期间 下降沿
  SDA_PORT_INIT_OUTPUT();  // SDA 端口为 输出
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
 *@Description: 发送 I2C   停止信号
 *@paramreq- 无
 *@paramind- 
 *@return 无
 */
static void I2CStop()
{
	// 时钟为高期间 上升沿
  SDA_LOW();
	SCL_HIGH();
	SoftDelayUs(4);
	SDA_HIGH();
	SoftDelayUs(4);
  SCL_LOW();
	SoftDelayUs(4);
}


/**
 *@Description: 等待从器件接收方的应答 
 *@paramreq- 无
 *@paramind- 
 *@return 无
 */
static uint8_t WaitAck(void)
{
	uint8_t errtime =255;           // 因故障接收方无ACK，超时值为255。

	SCL_HIGH();
	SoftDelayUs(4);
	// SDA口设为输入
    SDA_PORT_INIT_INPUT();
    SoftDelayUs(4);
    // SDA出现为0时,则为有应答 
	while(SDA_STATE_GET())
	{
		errtime--;
		// errtime是否为0
    if(!errtime)
		{
			I2CStop();
			return  IIC_DEVICE_ACK_FAIL;			   //返回为0(失败) 
		}
	}
	SDA_PORT_INIT_OUTPUT();  // SDA口设为输出
	SCL_LOW();
	SoftDelayUs(4);

	return IIC_DEVICE_ACK_SUCCESS;
}

/**
 *@Description: //主器件为接收方，从器件为发送方时，单片机发出应答信号 
 *@paramreq- 无
 *@paramind- 
 *@return 无
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
 *@Description: //主器件为接收方，从器件为发送方时，非应答信号 
 *@paramreq- 无
 *@paramind- 
 *@return 无
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
 *@Description: 写数据  // 上升沿数据写入
 *@paramreq- 无
 *@paramind- 
 *@return 无
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
 *@Description: 读数据
 *@paramreq- 
 *@paramind- 
 *@return 无
 */
static uint8_t I2CReceiveByte(void)
{
	  uint8_t data = 0;
    uint8_t i;
    
    SDA_PORT_INIT_INPUT(); // 切换SDA脚为输入状态
    SoftDelayUs(4);
   
  for (i = 0; i < 8; i++)
	{
		data <<= 1;
		SCL_HIGH();
		SoftDelayUs(4);
		SoftDelayUs(4);
		if (1 == SDA_STATE_GET())  // 下降沿数据读出
		{
			data |= 0x01;
		}
		SCL_LOW();
		SoftDelayUs(4);
	}
	SDA_PORT_INIT_OUTPUT();  // 切换SDA脚为输出状态
  SoftDelayUs(4);
	
	return data;
}





/**
 *@Description: 写一页数据    AT24C512 一页为128字节
 *@paramreq- slaAddr: AT24CXX 地址 (C512为 00)    subAddr: 内存地址   acessAddr: 数据指针    numbBytes: 数据个数
 *@paramind- 
 *@return 无
 */
static uint32 _I2CPageWrite(uint8_t slaAddr, uint16_t subAddr, uint8_t *acessAddr, uint8_t numbBytes)
{
    uint8 writtenBytes = 0;

    I2CStart();
    // 发送器件地址加写标志
    I2CSendByte(slaAddr | IIC_WRITE_FLG);
    if(IIC_DEVICE_ACK_SUCCESS != WaitAck())
    {
			return writtenBytes;
    }

    if (t_AT24CXXPhyInfo[curr_dev_type].m_subAddrType >= ADDR_TWO_BYTES)
    {
        // 发送地址高字节
        I2CSendByte(subAddr >> 8);
        if(IIC_DEVICE_ACK_SUCCESS != WaitAck())
        {
					return writtenBytes;
        }
    }
    // 发送地址低字节
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

    // 这里的20ms延时为器件的内部编程时间
    SoftDelayMs(20);

    return writtenBytes;
}




/**
 *@Description: 连续读数据
 *@paramreq- slaAddr: AT24CXX 地址 (C512为 00)    subAddr: 内存地址   acessAddr: 数据指针    numbBytes: 数据个数
 *@paramind- 
 *@return 无
 */
static uint32 _I2CSequentialRead(uint8_t slaAddr, uint16_t subAddr, uint8_t *acessAddr, uint32_t numbBytes)
{
    uint32 readBytes = numbBytes;

    I2CStart();
    // 发送器件地址加写标志
    I2CSendByte(slaAddr | IIC_WRITE_FLG);
    if(IIC_DEVICE_ACK_SUCCESS != WaitAck())
    {
			return 0;
    }

    if(t_AT24CXXPhyInfo[curr_dev_type].m_subAddrType >= ADDR_TWO_BYTES)
    {
        // 发送地址高字节
        I2CSendByte(subAddr >> 8);
        if(IIC_DEVICE_ACK_SUCCESS != WaitAck())
        {
					return 0;
        }
    }
    // 发送地址低字节
    I2CSendByte(subAddr & 0x00FF);
    if(IIC_DEVICE_ACK_SUCCESS != WaitAck())
    {
			return 0;
    }

    I2CStart();
    // 发送器件地址加读标志
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
 *@Description: 读写函数  
 *@paramreq- accessAddr: 内存地址      buffer: 数据指针   numbBytes: 读写的个数  accessCtrl: 读写标志
 *@paramind- 
 *@return 无
 */
static uint32 I2CInnerAccess(uint32 accessAddr, uint8 *buffer, uint32 numbBytes, uint32 accessCtrl)
{
    uint32  actualProBytes  = 0; // 读写成功的总字符数
    uint16  currStepActualProBytes  = 0; // 当前步读写成功的字节数
    uint16  accessBytes; // 当前页读写的字符数
    uint16  currPageRemainedBytes; // 当前页剩余
    
    //判断参数是否合法
    if(0 == numbBytes)
    {
			return actualProBytes;
    }
    // 判断字地址是否合法
    if (accessAddr > t_AT24CXXPhyInfo[curr_dev_type].m_pageNumbs*t_AT24CXXPhyInfo[curr_dev_type].m_pageSize)
    {
			return actualProBytes;
    }

    
    /** 求出第一次写操作时所写的页可以写入的字节数 因为涉及到页内地址反转问题 先计算多出来的字节 */
    currPageRemainedBytes  = accessAddr % t_AT24CXXPhyInfo[curr_dev_type].m_pageSize; // 一页剩余量
    currPageRemainedBytes  = t_AT24CXXPhyInfo[curr_dev_type].m_pageSize - currPageRemainedBytes; // 页的开始字节

    for(; numbBytes > 0; )
    {
        //计算要操作的字节数
        if(numbBytes > currPageRemainedBytes)
        {
					accessBytes = currPageRemainedBytes; // 如果要写的数量大于页剩余量,先写完此页
        }
        else
        {
					accessBytes = numbBytes;
        }
        
        //I2C实际操作程序段
        {
					//子地址类型判断
					if (t_AT24CXXPhyInfo[curr_dev_type].m_subAddrType == ADDR_ONE_BYTE)
          {
						t_I2CCtrlInfo.m_accessAddr 	= accessAddr;		 // 存取地址 			
        		t_I2CCtrlInfo.m_accessAddrByteNumbs = 1;		// 存取地址为1字节	
        	}
        	else if (t_AT24CXXPhyInfo[curr_dev_type].m_subAddrType == ADDR_P0_WITH_ONE_BYTE)
        	{	
        		t_I2CCtrlInfo.m_slaAddr = (uint8_t)(t_I2CCtrlInfo.m_slaAddr | ((accessAddr >> 7) & 0x02));  // 重新合成器件地址
        		t_I2CCtrlInfo.m_accessAddr = (uint8_t)accessAddr;   // 存取地址 			
        		t_I2CCtrlInfo.m_accessAddrByteNumbs = 1;    // 存取地址为1字节 	
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
            t_I2CCtrlInfo.m_paccessAddr      = buffer;        // 数据读写指针
            t_I2CCtrlInfo.m_accessNumbBytes  = accessBytes;   // 要读取的个数
            t_I2CCtrlInfo.m_accessFinished   = FALSE;
            t_I2CCtrlInfo.m_accessCtrl       = accessCtrl;
        
            if (I2C_WRITE == accessCtrl) 
            {
							AT24Cxx_Control(IO_CTRL_CMD_I2C_WP_OFF); // 关闭写保护 写操作要把WP线置低电平,允许写
              currStepActualProBytes = _I2CPageWrite(t_I2CCtrlInfo.m_slaAddr, accessAddr , buffer, accessBytes); // 写字符
            }
            else
            {
							currStepActualProBytes = _I2CSequentialRead(t_I2CCtrlInfo.m_slaAddr, accessAddr, buffer, accessBytes); // 读字符
            }

            actualProBytes +=  currStepActualProBytes; // 读写成功  总字节数累加
						
            /** 若没有成功写入指定的字节数的数据 则结束整个操作过程 并返回实际操作的字节数 */
            if (accessBytes != currStepActualProBytes)
            {
							break;
            } 	
        
            if (I2C_WRITE == accessCtrl)
            {
							//开启写保护
							AT24Cxx_Control(IO_CTRL_CMD_I2C_WP_ON); // WP置高电平
            }
        }
        numbBytes  -= accessBytes;  // 剩余没有操作的字节数
        accessAddr += currPageRemainedBytes; // 操作地址移动
        buffer     += currPageRemainedBytes; // 数组指针移动
        currPageRemainedBytes = t_AT24CXXPhyInfo[curr_dev_type].m_pageSize;
    }
    return actualProBytes;
}







/**
 *@Description: AT24Cxx 控制  , 读写控制   设置器件地址
 *@paramreq- cmd: 控制码
 *@paramind- 
 *@return 无
 */
uint8 AT24Cxx_Control(uint8 cmd)
{
	uint8 rt = RT_EOK;
    switch (cmd)              
    {
			case IO_CTRL_CMD_I2C_WP_ON:
				if (SET_PARAM_I2C_DEV == curr_proc_dev_index)
				{
					WP_EN_HIGH();  // 写保护
        }
        break;

        case IO_CTRL_CMD_I2C_WP_OFF:
					if(SET_PARAM_I2C_DEV == curr_proc_dev_index)
          {
						WP_DISEN_LOW(); // 可写
          }
          break;

        case IO_CTRL_CMD_I2C_DEV_INDEX_SET:
            if(CAL_DATA_I2C_DEV == curr_proc_dev_index)
            {
							t_I2CCtrlInfo.m_slaAddr  = 0xA2;
							// 因为校准参数用AT24C256，设置参数使用AT2C512，所以片选的同时要指定当前使用的器件类型
              curr_dev_type = AT24C256;
            }
            else if(SET_PARAM_I2C_DEV == curr_proc_dev_index)
            {
                t_I2CCtrlInfo.m_slaAddr  = 0xA0;
                // 因为校准参数用AT24C256，设置参数使用AT2C512，所以片选的同时要指定当前使用的器件类型
                curr_dev_type  = AT24C512;
            }
            break;

				default:
					rt = RT_ERROR;
          break;
    }
    return rt;
}





// 调试用
#if(I2C_DEBUG > 0)
//    uint8 a[10] = {0x22,0x21,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x30};
//    uint8 b[10] = {0};
uint8 a[2] = {0x22,0x21};
uint8 b[2] = {0};
#endif
/**
 *@Description: 启动I2C   检测是否存在 AT24Cxx       
 *@paramreq-  
 *@paramind- 
 *@return   1: 正常    0: 失败
 */ 
uint8 AT24Cxx_Check(void)
{
	uint16 flag = 0;
	WP_PORT_INIT(); // 端口设置
  SCL_PORT_INIT();

	curr_proc_dev_index = SET_PARAM_I2C_DEV ;
// 调试用
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

/********************  测试  **********************/
/**
 *@Description:  AT24Cxx  初始化       
 *@paramreq-  cmd: AT24Cxx 控制参数          args: AT24Cxx 的地址
 *@paramind- 
 *@return   1: 正常    0: 失败
 */ 
uint8 AT24Cxx_Init(uint8 cmd, void *args)       //  测试 AT24Cxx 什么时候可以用
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
 *@Description:  AT24Cxx 读数据      
 *@paramreq-  addr: 读取开始地址    buffer: 读取到的数据所保存的指针  size: 要读取的数据个数
 *@paramind- 
 *@return   1: 正常    0: 失败
 */
uint32 AT24Cxx_Read(uint32 addr, void* buffer, uint32 size)
{
	uint32 count = 0;
	count = I2CInnerAccess(addr, buffer, size, I2C_READ);
	return count;
}




/**
 *@Description: 写数据到 AT24Cxx      
 *@paramreq-  addr: 开始写的地址    buffer: 要写的数据指针  size: 写的数据个数
 *@paramind- 
 *@return   1: 正常    0: 失败
 */
uint32 AT24Cxx_Write(uint32 addr, uint8* buffer, uint32 size)
{
	uint32 count = 0;
	count = I2CInnerAccess(addr, buffer, size, I2C_WRITE);
	return count;
}







/**
 *@Description: 检查电阻值 
 *@paramreq-  res: 保存电阻 BCD 码的指针    max: 电阻最大值  value: 解码保存的值
 *@paramind- 
 *@return   1: 正常    0:结束     2: 超出范围
 */
uint8 CheckResValue(uint8* res, float max, float* value)
{
	float temp = 0;
	if(res[0] == 0xFF && res[1] == 0xFF && res[2] == 0xFF && res[3] == 0xFF)
	{
		return 0; // 结束标志
	}
	else
	{
		temp = BCDCodeToFloat(res);// 解码
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
 *@Description: 读测试电阻数据并且判断是否结束   
 * 结束标志连续4个 : 0xFF 一页数据99 float , 99*4个uint8.后面4个uint8作为结束标志  总共99*4+4
 *@paramreq- res: 保存一页测试电阻数据的指针   addr: 页开始地址   num: 一个测试页的数据量
 *@paramind-  value: 默认值    max: 电阻最大值
 *@return 读取的数组个数
 */
uint8 ReadResValue(float* res, float value, float max, uint32 addr, uint8 num)
{
//	uint8 count = 0; // 记录读取的数据个数  
	uint8 i = 0;
	uint8 flag = 0; // 读写成功标志
	uint32 addr_temp = addr; // 数据开始地址
	uint8 temp[4] = {0,0,0,0};// 电阻 BCD 码 保存的临时数组

  for(i=0;i<num;i++)
  {
		flag = AT24Cxx_Write(addr_temp, temp, 4); // 读数据
		if(flag != 0) // 读成功
		{
			flag = 0;
			flag = CheckResValue(temp, max, res+i); // 检查数据

			if(flag == 0)
			{
				return i; // 结束
			}
			else if(flag == 2)
			{
				*(res+i) = value; // 赋默认值
				FloatToBCDCode(temp,value); // 将默认值编码
				AT24Cxx_Read(addr_temp, temp, 4); // 给此处写入默认值
			}
			else
			{
			}
			flag = 0; 			
		}
		else
		{
			flag = AT24Cxx_Write(addr_temp, temp, 4); // 再读一次
			if(flag == 0)
			{
				return i; // 返回
			}
			else
			{
				if(flag == 0)
			  {
					return i; // 结束
			  }
				else if(flag == 2)
			  {
					*(res+i) = value; // 赋默认值
					FloatToBCDCode(temp,value); // 将默认值编码
					AT24Cxx_Read(addr_temp, temp, 4); // 给此处写入默认值
			  }
				else
				{
        }
				flag = 0;
			}
		}
		addr_temp = 4 + addr_temp; // 指向下一个地址
	}
	return i;
}






/**
 *@Description:  开机时读取system 参数     参数保存在第三页 开始地址为  256
 *@paramreq- sysstr: 参数保存值    sys_default:参数默认值    max:个参数最大值    num: 参数个数
 *@paramind- 
 *@return   1: 正常    0: 失败
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












