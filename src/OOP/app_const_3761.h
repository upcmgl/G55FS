#ifndef __APP_CONSTDEF_H__
#define __APP_CONSTDEF_H__

#define FUNC_MASK_CLIENT    0x80       //���ô��ͷ���
#define FUNC_MASK_DENY      0x40       //���÷񶨱�־

#define CTRLFUNC_SET_DIR        0x80       //���ô��䷽��λDIR=1    ʹ��|
#define CTRLFUNC_CLR_DIR        0x7F       //���ô��䷽��λDIR=0    ʹ��&
#define CTRLFUNC_CLR_PRM        0xBF       //����������־λPRM=0    ʹ��&
#define CTRLFUNC_SET_PRM        0x40       //����������־λPRM=1    ʹ��|
#define CTRLFUNC_CLR_ACD        0xDF       //����֡Ҫ�����λACD=0  ʹ��&
#define CTRLFUNC_SET_ACD        0x20       //����֡Ҫ�����λACD=1  ʹ��|
#define CTRLFUNC_CLR_FCB        0xDF       //����֡����λFCB=0      ʹ��&
#define CTRLFUNC_SET_FCB        0x20       //����֡����λFCB=1      ʹ��|
#define CTRLFUNC_CLR_FCV        0xEF       //����֡������ЧλFCV=0      ʹ��&
#define CTRLFUNC_SET_FCV        0x10       //����֡������ЧλFCV=1      ʹ��|

//�ϱ������ն���ÿ֡���ݸ���
#define ENCRYPT_NONE  0
#define ENCRYPT_TOPS1  1
#define ENCRYPT_TOPS2  2

#define CHANNEL_REMOTE  0x01
#define CHANNEL_RS232   0x02
//#define CHANNEL_INFRA   0x04
#define CHANNEL_FLASH   0x08
#define CHANNEL_STACK   0x10
#define CHANNEL_ETH     0x20
#define CHANNEL_SMS     0x40
#define CHANNEL_NONE    0x80
//����ͨ�������ã�����û�а�λʹ�ã����������ͨ��������λ����
#define CHANNEL_GPRS_SERVER    0x81
#define CHANNEL_GPRS_CLIENT    0x82
#define CHANNEL_ETH_SERVER    0x83
#define CHANNEL_ETH_CLIENT     0x84
#define CHANNEL_BEIDOU        0x85

#define LEN_ERTU_CFG_VERINFO        41

//�ɼ���Ϣ�����ܹ�ռ�洢�ռ�11K
#define MAX_TASK_COUNT                    20
#define MAX_NORMAL_PLAN_COUNT             20
#define MAX_EVENT_PLAN_COUNT              10
#define MAX_METER_DOC_SIZE                200
#define MAX_TASK_INFO_SIZE                100
#define MAX_NORMAL_PLAN_SIZE              400
#define MAX_EVENT_PLAN_SIZE               400
#define ACQ_CTRL_INFO_SATRT               8000
#define ACQ_METER_DOC_INFO                ACQ_CTRL_INFO_SATRT
#define ACQ_TASK_INDEX                    (ACQ_METER_DOC_INFO+900)
#define ACQ_NORMAL_PALM_INDEX             (ACQ_TASK_INDEX+25)
#define ACQ_EVENT_PLAN_INDEX              (ACQ_NORMAL_PALM_INDEX+15)
#define ACQ_TASK_INFO                     (ACQ_EVENT_PLAN_INDEX+15+45)    //45Ԥ��
#define ACQ_NORMAL_PLAN_INFO              (ACQ_TASK_INFO+MAX_TASK_COUNT*MAX_TASK_INFO_SIZE)
#define ACQ_EVENT_PLAN_INFO               (ACQ_NORMAL_PLAN_INFO+MAX_NORMAL_PLAN_COUNT*MAX_NORMAL_PLAN_SIZE)
#define ACQ_CTRL_INFO_END                 (ACQ_EVENT_PLAN_INFO+MAX_EVENT_PLAN_COUNT*MAX_EVENT_PLAN_SIZE)
//ESAM_OAD_LIST�ܹ�ռ�洢�ռ�1K
#define MAX_ESAM_OI_COUNT                 64
#define MAX_ESAM_PARAM_SIZE               1000
#define ESAM_PARAM_START                  ACQ_CTRL_INFO_END
#define ESAM_PARAM_END                    (ESAM_PARAM_START+MAX_ESAM_PARAM_SIZE)

#define EVENT_STORE_MAX                         15
#define TERMINAL_POWER_ONOFF                    (ESAM_PARAM_END+2)  //�ն�ͣ�ϵ硣
#define DAY_BYTES                               (TERMINAL_POWER_ONOFF+sizeof(StrTerminalPowerOnOffEventBlock)+2)//������ 2���ֽ�
#define MONTH_BYTES								(DAY_BYTES+2) //������ 8���ֽ�


			
#endif




