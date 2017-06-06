#include "resreadandwrite.h"
#include "readandwrite.h"




/**
 *@Description: 读写某一页的电阻值(包括小数点)  保存到数组
 *@paramreq- res:读写数据保存的数组指针   page:测试页  step:测试步   type:读写类型  1:读 0:写
 *@paramind- 
 *@return 
 */
uint8 GetAndSet(uint8* buffer, uint8 page, uint8 step, uint8 type)
{
	uint32 addr = 0;
	
	addr  = 256 + (99*6 + 1)*(page - 1); // 计算页起始地址
	addr += 6*(step - 1);                // 计算测试步起始地址
	
	if(type != 0)                        // 读数据
	{
		AT24Cxx_Read(addr, buffer, 6);
	}
	else
	{
		AT24Cxx_Write(addr, buffer,6);
	}
	
	return 1;
}




































