#include "resreadandwrite.h"
#include "readandwrite.h"




/**
 *@Description: ��дĳһҳ�ĵ���ֵ(����С����)  ���浽����
 *@paramreq- res:��д���ݱ��������ָ��   page:����ҳ  step:���Բ�   type:��д����  1:�� 0:д
 *@paramind- 
 *@return 
 */
uint8 GetAndSet(uint8* buffer, uint8 page, uint8 step, uint8 type)
{
	uint32 addr = 0;
	
	addr  = 256 + (99*6 + 1)*(page - 1); // ����ҳ��ʼ��ַ
	addr += 6*(step - 1);                // ������Բ���ʼ��ַ
	
	if(type != 0)                        // ������
	{
		AT24Cxx_Read(addr, buffer, 6);
	}
	else
	{
		AT24Cxx_Write(addr, buffer,6);
	}
	
	return 1;
}




































