#ifndef    BSP_CPUCONFIG_H
#define    BSP_CPUCONFIG_H


/*******************************************************************************
 *                            �����������궨��
********************************************************************************/

#define         ARM_KEIL                                        (0) 
#define         AVR_GCC                                         (1)

#define         COMPILE_ENV                                     (ARM_KEIL)
//#define       COMPILE_ENV                                     (AVR_GCC)
#define         RT_TICK_PER_SECOND	                             100

/*******************************************************************************
 *                              CPU��������
********************************************************************************/

typedef         unsigned char                       uint8;
typedef         signed char                         int8;
typedef         unsigned short int                  uint16;
typedef         signed short int                    int16;
typedef         unsigned long int                   uint32;
typedef         signed  long int                    int32;
typedef         unsigned long long int              uint64;
typedef         signed  long long int               int64;
typedef         unsigned long 			                ulong;



/** Thread error code definitions */
#define RT_EOK							0				/**< There is no error 					*/
#define RT_ERROR						1				/**< A generic error happens 			*/
#define RT_ETIMEOUT					2				/**< Timed out 							*/
#define RT_EFULL						3				/**< The resource is full				*/
#define RT_EEMPTY						4				/**< The resource is empty 				*/
#define RT_ENOMEM						5				/**< No memory							*/
#define RT_ENOSYS						6				/**< No system 							*/
#define RT_EBUSY						7				/**< Busy								*/

/*******************************************************************************
 *                              ������������
********************************************************************************/



#if (COMPILE_ENV == AVR_GCC)

    typedef     unsigned char                     portuBASE_TYPE;
    typedef     char									            portBASE_TYPE;  
    typedef 	  unsigned int     						      portTICK_TYPE;
    typedef     unsigned char                     portCPSR_TYPE;
#elif (COMPILE_ENV == ARM_KEIL)

    typedef     unsigned int                      portuBASE_TYPE;
    typedef     int									              portBASE_TYPE;  
    typedef 	  unsigned int     						      portTICK_TYPE;
    typedef     unsigned int                      portCPSR_TYPE;
#endif

#define    SUCCESS             (1)
#define    FAIL                (0)
#define    FALSE               (0)
#define    TRUE                (1)
#define    NULL                (0)
#define    ON                  ('1')



/*******************************************************************************
 *                             Ӧ�ó��ú꺯������
********************************************************************************/
//��ȡһά�ṹ�����鳤��
#define	        GET_ARRAY_COUNT(addr)					            (sizeof(addr)/sizeof(addr[0]))
//��ȡ��ά�ṹ������ÿһά����
#define	        GET_PLANAR_ARRAY_COUNT(addr)			        (sizeof(addr[0])/sizeof(addr[0][0]))

/*******************************************************************************
 *                             CPU��ָ��궨��
********************************************************************************/

#if (COMPILE_ENV == AVR_GCC)

    #define    	portNOP()      asm volatile ("nop")
#elif (COMPILE_ENV == ARM_KEIL)

    #define     portNOP()	     __NOP()
#endif


#endif
