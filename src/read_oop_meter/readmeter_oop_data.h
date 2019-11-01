#ifndef __READMETER_OOP_DATA_H__
#define	__READMETER_OOP_DATA_H__

//#include "../main_include.h"
#include "../tpos/inc/tpos_typedef.h"
typedef struct{
	INT32U  oad;            //�������
	INT32U  item_07_block;  // 
	INT32U  item_07;        //07����������
	INT16U  offset;         //���ݴ洢ƫ����
	INT8U   item_count;     //���ݳ���
	INT8U   data_len;       //���ݳ��� ��������+type+����
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

#if 1
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
		INT16U page_id;   /* �����ݴ洢ҳID */ 
		INT16U page_cnt;  /* �����ݴ洢ռ�ü�ҳ */ 
		INT16U save_size; /* �����ݴ洢��С */
		INT8U oad_count;
		INT8U oad[NORMAL_TASK_GROUP_MAX_OAD_COUNT][4];/* OAD ��Ҫ����OAD�����⣬��֡�ʹ������ݵ�ʱ��ʹ��?? */
		INT8U save_off[NORMAL_TASK_GROUP_MAX_OAD_COUNT][2]; /* �洢��ƫ��λ�� */
		INT8U mask_idx[NORMAL_TASK_GROUP_MAX_OAD_COUNT]; /* �����λ����Ϣ�������ɹ�����ݴ���Ϣ���?? */
		INT8U mask[NORMAL_TASK_OAD_GROUP_MASK_SIZE]; /* ���룬�洢��ʱ�����ʹ�� */
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
		INT8U                   acq_type; // �ɼ�����
		INT8U                   idx_tsk_attr;/* gTaskPlanAttribute[]��λ�� */
		INT8U                   flag;/* �������� flag  �����  */
		OBJECT_ATTR_DESC1       oad; // ��OAD  ˭֪����
		INT16U                  start_page;
		union{
			struct{
				NORMAL_TASK_SAVE_PARAMS norml_task_save_params;
				INT8U                   normal_task_oad_mask[NORMAL_TASK_OAD_GROUP_MASK_SIZE];
				INT8U                   idx_oad_start;
				INT8U                   read_oad_count;
			};
		};
		INT8U                   last_td[7];        //������
		INT8U                   hold_td[7];        //������

		INT8U send_frame[300];
		INT8U recv_frame[500];
	};
}READ_PARAMS;


extern READ_PARAMS     read_params;
#endif

typedef union {
	INT8U vaule[24];
	struct{
		INT8U tsk_start_time[8]; /* ����+ ���� datetime_s��ʽ ? 1C+ */ 
		INT8U save_time[8];      /* ����+ ���� datetime_s��ʽ ? 1C+ */ 
		INT8U success_time[8];   /* ����+ ���� datetime_s��ʽ ? 1C+ */ 
	};
}HEADER_TD;

/* page header 
 * �����洢ʹ��λ�ã�Ϊ�˵�ǰ���ݵ�ʹ�� ?????
 */
typedef struct{
    INT8U meter_mo[6];
    INT8U use_flag[4];
}PAGE_HEADER;


#define READ_TYPE_CUR        1
#define READ_TYPE_HOLD       2
//#define 
BOOLEAN get_plan_oad_params(READ_PARAMS *read_param, INT16U obis,INT8U index);
BOOLEAN oad_search_in_list(INT8U type, INT8U list_idx,INT8U oad[4],tagREADITEM *read_item);
void save_grp_read_oad_params(GROUP_OAD_PARAMS *grp_oad_params, tagREADITEM read_item, INT8U mask_idx);
void prepare_read_oop_data_cycle(READ_PARAMS *read_params);
void check_read_cycle_one_task();
void parse_save_oop_data(READ_PARAMS *read_param,BOOLEAN hold_flag,INT8U *frame,INT16U frame_len);
void clr_process_mask(READ_PARAMS *read_param);
void write_flash_data_from_save_idx(HEADER_TD hdr_td,INT32U save_idx,INT16U data_offset,INT8U* data,INT16U data_len,INT16U save_size,INT16U page_id,INT16U page_count);
void read_flash_data_from_save_idx(INT32U save_idx,INT16U data_offset,INT8U* data,INT16U data_len,INT16U save_size,INT16U page_id,INT16U page_count);
void read_flash_data(INT8U meter_no[6],INT16U data_offset,INT8U* data,INT16U data_len,INT16U save_size,INT16U page_id,INT16U page_count);
#endif

