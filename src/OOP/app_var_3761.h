#ifndef __APP_VAR_3761_H__
#define __APP_VAR_3761_H__

#include "OOP/oop_type_def.h"
//#include "../main_include.h"
#include "../main_include.h"
//#include "oop_type_def.h"


#define SYS_FLAG_BASE  0
#define MAX_SYSTEM_FLAG_TYPE  1

//ͨ�����ʼ�ͨ�Ŷ˿ںţ�
// 	D0~D4�����ʾ���ܱ���������װ�ý����ն˵�ͨ�Ŷ˿ںţ���ֵ��Χ1~31��
// 	D5~D7�����ʾ���ܱ���������װ�ý����ն˵�ͨ�Ų����ʣ� 0~7���α�ʾ300��600��1200��2400��4800��7200��9600��19200��
typedef union{
  struct{
    INT8U port:5;
    INT8U baud:3;
  };
  INT8U  value;
}BAUD_PORT_PARAM;

//�й�����ʾֵ������λ��С��λ������
// 	D0~D1�������ʾͨ�Ž���ĵ��ܱ���й�����ʾֵ��С��λ��������ֵ��Χ0~3���α�ʾ1~4λС����
// 	D2~D3�������ʾͨ�Ž���ĵ��ܱ���й�����ʾֵ������λ��������ֵ��Χ0~3���α�ʾ4~7λ������
typedef union{
   struct{
     INT8U  fraction:2;
     INT8U  integer:2;
     INT8U  v_block:1;
        INT8U is_F210:1;  //���浵��ʱ���ݲ���������������
        INT8U porior_item:1; //����Ԥ�����������л�������ʹ��   #ifdef __PLC_PRIOR_F26_SWITCH__
        INT8U reserved:1;
   };
   INT8U  value;
}MBIT_INFO_PARAM;

//�û����༰������
typedef union{
    INT8U  value;
    struct{
       INT8U  meter_class:4;  //������
       INT8U  user_class:4;   //�û�����
    };
}METER_CLASS_PARAM;

// �����㵵����Ϣ,�������ò���F10
#define SIZEOF_METER_DOCUMENT   27
typedef union{
    INT8U  value[27];
    struct{
        INT8U          meter_idx[2];  //�������
        INT8U          spot_idx[2];   //�����������
        BAUD_PORT_PARAM      baud_port;     //���ʼ��˿�
        INT8U          protocol;      //���Э��
        INT8U          meter_no[6];   //����ַ
        INT8U          password[6];   //ͨ������
        INT8U          fl_count;      //���ʸ���
        MBIT_INFO_PARAM      mbit_info;     //��λ��
        INT8U          rtu_no[6];     //�ɼ�����ַ
        METER_CLASS_PARAM    meter_class;   //�û����༰������
    };
}METER_DOCUMENT;     //27�ֽ�

//�ն������ϱ���I��II��III�����ݣ�ȷ�ϻ��ƵĴ���
typedef struct{
    INT32U last_send_timer;
    INT16U timeout_second;
    INT8U  repeat_time;
    INT8U  check_seq;
    BOOLEAN need_check_ack;
    BOOLEAN has_acked;
    BOOLEAN has_send;
}tagActiveSendAck;

typedef struct{
    INT8U single_addr_counter[4];//����ַӦ��Э�̼�����
    INT8U report_counter[4];//�����ϱ������� 
    INT8U broadcast_no[4];//Ӧ�ù㲥ͨ�����к�
}ESAM_COUNTER;
typedef union{
    INT8U value[4];
    struct{
    INT8U esam_id[8];
    INT8U esam_ver[4];
    INT8U symmetry_ver[16];
    INT8U cert_ver[2];
    INT8U session_valid_time[4];
    INT8U session_left_time[4];
    ESAM_COUNTER counter;
    INT8U msa_cert_no[16];
    INT8U dev_cert_no[16];
    };
}ESAM_INFO;

typedef struct{
    INT16U mark;
    INT16U count;
    INT8U  object_list[MAX_ESAM_OI_COUNT][5];
}ESAM_OI_LIST;

#if 0
typedef struct{
    INT32U  oad;            //�������
    INT32U  item_07_block;  //07�����������
    INT32U  item_07;        //07����������
    INT16U  offset;         //���ݴ洢ƫ����
    INT8U   item_count;     //���ݳ���
    INT8U   data_len;       //���ݳ���
    INT16U  block_data_len; //���ݳ���
    INT16U  page_id;
    INT16U  page_count;
}tagREADITEM;


typedef struct{
    tagREADITEM *read_item_list;
    INT8U *read_mask;
    INT8U read_item_count;
    INT8U read_mask_size;
    INT16U obis;
    INT16U patch_count; //��������
}READ_LIST_PROP;
#endif


/*  */
#if 0
typedef union{
	INT8U value;
	struct{
		INT8U attribute_idx:5;
		INT8U attribute_spec:3;
	};
}ATTRIBUTE_ID1;
typedef union{
	INT8U value[4];
	struct{
		INT8U object_id[2];
		ATTRIBUTE_ID1 attribute_id;
		INT8U index_id;
	};
}OBJECT_ATTR_DESC1;

#define NORMAL_TASK_OAD_GROUP_MASK_SIZE        4U
#define NORMAL_TASK_GROUP_MAX_OAD_COUNT       (NORMAL_TASK_OAD_GROUP_MASK_SIZE*8)
typedef union{
    INT8U value[4];
    struct{
        INT8U oad_count;
        INT8U oad[NORMAL_TASK_GROUP_MAX_OAD_COUNT][4];/* OAD ��Ҫ����OAD�����⣬��֡�ʹ������ݵ�ʱ��ʹ��?? */
        INT8U save_off[NORMAL_TASK_GROUP_MAX_OAD_COUNT][2]; /* �洢��ƫ��λ�� */
        INT8U mask_idx[NORMAL_TASK_GROUP_MAX_OAD_COUNT]; /* �����λ����Ϣ�������ɹ�����ݴ���Ϣ���?? */
        /* Ԥ��Ӧ�𳤶� */
        INT8U resp_byte_num[NORMAL_TASK_GROUP_MAX_OAD_COUNT];
        OBJECT_ATTR_DESC1 master_oad;
    };
}GROUP_OAD_PARAMS;
typedef union{
    INT8U value[4];
    struct{
        INT8U plan_id;
        INT8U plan_type;
        INT8U depth[2];
        INT8U z_size[2];
        INT8U process_mask[8]; /* OAD��������  */
        INT8U plan_oad[64][4];/* ������OAD�ȴ浽����� */
        INT8U max_oad_cnt;// oad �������
        INT8U oad_idx;// oad ����λ�� �´δ�֮��ʼ����
        GROUP_OAD_PARAMS group_oad_params;
        INT8U record_point_count; //ÿ֡���Գ��������������
        INT8U cycle_record_count;
        INT8U last_time;
        INT8U time_interval[3];
        INT8U save_opt;//�洢ʱ��ѡ��
    };
}NORMAL_TASK_SAVE_PARAMS;

typedef union{
    INT8U value[2];
    struct{
        INT8U                   task_idx[2];
        INT8U                   plan_type;
        INT8U                   idx_mask;
        INT8U                   idx_oad_group;
        INT8U                   idx_oad;
        //OBJECT_ATTR_DESC        oad;
        union{
            struct{
                NORMAL_TASK_SAVE_PARAMS norml_task_save_params;
                //READ_ITEM               read_item;
                INT8U                   normal_task_oad_mask[NORMAL_TASK_OAD_GROUP_MASK_SIZE];
                INT8U                   idx_oad_start;
                INT8U                   read_oad_count;
            };
        };
        union{
            
            //OAD_SAVE_PARAMS         oad_save_params;
            //OAD_SAVE_PARAMS_1       oad_save_params_1;
        };
        INT8U                   last_td[7];        //������
        INT8U                   hold_td[7];        //������
        //READ_MASK               read_mask;         //��������

        INT8U send_frame[300];
        INT8U recv_frame[500];
    };
}READ_PARAMS;


extern READ_PARAMS     read_params;
#endif
extern METER_DOCUMENT  gmeter_doc;
extern INT32S   read_c1_time_out;//���ݳ�ʱ����
extern tagActiveSendAck gActiveSendAck;//�����ϱ����ݵ�ȷ�ϻ���

extern ESAM_INFO g_esam_info;
extern ESAM_OI_LIST g_esam_oi_list;
extern ESAM_OI_LIST g_esam_oi_list_default;
extern INT32U sys_flag[MAX_SYSTEM_FLAG_TYPE];  //ϵͳ��־��,��δʹ��

extern INT16U  frame_receive;

#ifdef __PROVIENCE_LIAONING_OOP__  //˳������ ʵʱ���ն���,15��������
#define  G_TASK_PLAN_CNT  5
extern  INT8U gtask[G_TASK_PLAN_CNT][200];
extern  INT8U gplan[G_TASK_PLAN_CNT][200];
#endif
#endif

