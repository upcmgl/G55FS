/* 
 * File:   tpos_config.h
 *
 * TPOS �������ļ�
 *
 */

#ifndef TPOS_CONFIG_H
#define	TPOS_CONFIG_H

//#include <xc.h>

//��������жϼ��𣬱�����1
#define configKERNEL_INTERRUPT_PRIORITY                         0x01

//ϵͳ�������������жϼ���ĿǰҲ������1��  �ݲ�֧�ִ��ж��е��÷�������
#define configMAX_SYSCALL_INTERRUPT_PRIORITY                    0x01

//����ؼ���������ʵ�ַ�ʽ�� ʹ��DI,EI ָ�ʽ�Ļ��� ����ȡֵ1  ������0��������Ŀǰ����ʹ��1��ֻ�ܶ���Ϊ0
#define configCRITICAL_SECTION_METHOD_DI_EI                      0

//���񹷵Ŀ�������,��λ 10ms
#define configMAX_TASK_SOFT_DOG_10MS                            30000


//PIC32MZ  CPO_STATUS  �Ĵ���
// 0: IE    1: EXL   2: ERL   3: R    4: UM   5~7: R    8~9: IM1..IM0
// 10~16:  IPL(IM8..IM2)     17:CEE    18: IPL(IM9)
// 19:NMI   20:SR  21:TS   22:BEV   23~24:R   25:RE  26:FR   27:RP  28~31:CU3..CU0

#define MCU_IPL_SHIFT				( 10UL )

//�жϼ���ֻ����CEE���²��֡�
#define MCU_ALL_IPL_BITS			( 0x7FUL << MCU_IPL_SHIFT )

//ϵͳ����������жϼ���
#define MCU_SYSCALL_IPL_BITS                   (configMAX_SYSCALL_INTERRUPT_PRIORITY << MCU_IPL_SHIFT)

//���������ж�:  ִ�к��������������
//#define tpos_ENABLE_TASK_INTERRUPTS()					\
//{									\
        //uint32_t ulStatus;					 	\
									//\
	///* Unmask all interrupts. */					\
	//ulStatus = _CP0_GET_STATUS();					\
	//ulStatus &= ~MCU_ALL_IPL_BITS;					\
	//_CP0_SET_STATUS( ulStatus );					\
//}
//
////��ֹ�����жϣ�  ִ�к��ٷ����������
//#define tpos_DISABLE_TASK_INTERRUPTS()					\
//{									\
	//uint32_t ulStatus;						\
									//\
	//ulStatus = _CP0_GET_STATUS();					\
	//ulStatus &= ~MCU_ALL_IPL_BITS;					\
	//_CP0_SET_STATUS( ( ulStatus | MCU_SYSCALL_IPL_BITS) ) ;        \
//}
//
//#define tpos_TASK_YIELD()						\
//{									\
       //PLIB_INT_SourceFlagSet(INT_ID_0, INT_SOURCE_TIMER_CORE);         \
//}

#endif	/* TPOS_CONFIG_H */

