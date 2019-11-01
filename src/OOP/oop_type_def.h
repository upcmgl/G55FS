#ifndef __OOP_TYPE_DEF_H__
#define	__OOP_TYPE_DEF_H__
#include "../main_include.h"

//Э����Ϣ
#define PROTOCOL_VER           0x0010
#define MAX_RECV_FRAME_SIZE    2048ul
#define MAX_SEND_FRAME_SIZE    2048ul
#define MAX_APDU_SIZE          8000ul

#define FRAME_CHAIN_SIZE       100
#define MAX_APDU_FRAME_SIZE    (MAX_APDU_SIZE+FRAME_CHAIN_SIZE) 
#define MAX_RECV_FRAME_COUNT   5ul
#define LINK_DEFAULT_TIME_OUT  600000ul //��

#define DEFAULT_LINK_PASSWORD "111111"
#define DEFAULT_LINK_PASSWORD_LEN 6

#define SECURITYRESULT_OK               0
#define SECURITYRESULT_PWD_ERR          1
#define SECURITYRESULT_SYMMETRY_ERR     2
#define SECURITYRESULT_NONSYMMETRY_ERR  3
#define SECURITYRESULT_AUTOGRAPH_ERR    4

#define OP_GET                0     //��ȡ
#define OP_SET                1     //����
#define OP_ACTION             2     //����
#define OP_PROXY              3     //����

#define APDU_NONCODE          0     //����
#define APDU_ENCODE           1     //����

//�������Ͷ���
#define DT_NULL               0
#define DT_ARRAY              1
#define DT_STRUCTURE          2
#define DT_BOOLEAN            3
#define DT_BITSTRING          4
#define DT_DOUBLELONG         5
#define DT_DOUBLELONGUNSIGNED 6
#define DT_OCTETSTRING        9
#define DT_VISIBLESTRING     10
#define DT_UTF8STRING        12
#define DT_INTEGER           15
#define DT_LONG              16
#define DT_UNSIGNED          17
#define DT_LONGUNSIGNED      18
//#define DT_COMPACTARRAY      19
#define DT_LONG64            20
#define DT_LONG64UNSIGNED    21
#define DT_ENUM              22
#define DT_FLOAT32           23
#define DT_FLOAT64           24
#define DT_DATETIME          25
#define DT_DATE              26
#define DT_TIME              27
#define DT_DATETIME_S        28
#define DT_OI                80
#define DT_OAD               81
#define DT_ROAD              82
#define DT_OMD               83
#define DT_TI                84
#define DT_TSA               85
#define DT_MAC               86
#define DT_RN                87
#define DT_REGION            88
#define DT_SCALER_UNIT       89
#define DT_RSD               90
#define DT_CSD               91
#define DT_MS                92
#define DT_SID               93
#define DT_SID_MAC           94
#define DT_COMDCB            95
#define DT_RCSD              96
#define DT_EVENTMASK        254
#define DT_DONT_CARE        255

//��ȫģʽ�ֶ���
//����
#define SMW_PROXY_ACTION       0x0001     //�������
#define SMW_PROXY_SET          0x0002     //��������
#define SMW_PROXY_GET          0x0004     //�����ȡ
#define SMW_PROXY_TMP          0x0008     //������
//����
#define SMW_ACTION_ENCODE_MAC  0x0010     //����+������֤�����
#define SMW_ACTION_ENCODE      0x0020     //���ķ�ʽ����
#define SMW_ACTION_NONCODE_MAC 0x0040     //����+������֤�����
#define SMW_ACTION_NONCODE     0x0080     //���ķ�ʽ����
//����
#define SMW_SET_ENCODE_MAC     0x0100     //����+������֤�����
#define SMW_SET_ENCODE         0x0200     //���ķ�ʽ����
#define SMW_SET_NONCODE_MAC    0x0400     //����+������֤�����
#define SMW_SET_NONCODE        0x0800     //���ķ�ʽ����
//��ȡ
#define SMW_GET_ENCODE_MAC     0x1000     //����+������֤�����
#define SMW_GET_ENCODE         0x2000     //���ķ�ʽ����
#define SMW_GET_NONCODE_MAC    0x4000     //����+������֤�����
#define SMW_GET_NONCODE        0x8000     //���ķ�ʽ����

#define EXPLICIT_CONFIG_WORD   0x80       //��ʽģʽ��

typedef union{
    INT8U value;
    struct{
        INT8U attribute_idx:5;
        INT8U attribute_spec:3;
    };
}ATTRIBUTE_ID;

typedef union{
    INT8U value[4];
    struct{
        INT8U object_id[2];
        ATTRIBUTE_ID attribute_id;
        INT8U index_id;
    };
}OBJECT_ATTR_DESC;

typedef union{
    INT8U value[5];
    struct{
        INT8U tag;
        OBJECT_ATTR_DESC oad;
    };
}OBJECT_ATTR_DESC_TAG;
//DOUBLE LONG
typedef struct{
    INT8U tag;
    INT8U value[4];
}COSEM_SIGNED32;

typedef struct{
    INT8U tag;
    INT8U value[4];
}COSEM_UNSIGNED32;

typedef struct{
    INT8U tag;
    INT8U value[8];
}COSEM_SIGNED64;

typedef struct{
    INT8U tag;
    INT8U value[8];
}COSEM_UNSIGNED64;

typedef struct{
    INT8U tag;
    INT8U value[2];
}COSEM_UNSIGNED16;


typedef struct{
    INT8U tag;
    INT8U value[2];
}COSEM_SIGNED16;

typedef struct{
    INT8U tag;
    INT8U value;
}COSEM_UNSIGNED;

typedef struct{
    INT8U tag;
    INT8U value;
}COSEM_ENUM;


typedef struct{
    INT8U tag;
    INT8U value;
}COSEM_BOOLEAN;

typedef struct{
    INT8U tag;
    INT8S value;
}COSEM_INTEGER;

typedef struct{
    INT8U tag;
    INT8U value[8];
}COSEM_FLOAT64;

typedef struct{
    INT8U tag;
    INT8U value[3];
}COSEM_TIME;

typedef struct{
    INT8U tag;
    INT8U value[7];
}COSEM_BCDDATETIME,COSEM_DATETIME_S;
typedef struct{
    INT8U year[2];
    INT8U month;
    INT8U day_of_month;
    INT8U day_of_week;
    INT8U hour;
    INT8U minute;
    INT8U second;
    INT8U milliseconds[2];
}COSEM_DATETIME_VALUE;

typedef union{
    INT8U value[4];
    struct{
        INT8U tag;
        INT8U year[2];
        INT8U month;
        INT8U day_of_month;
        INT8U day_of_week;
        INT8U hour;
        INT8U minute;
        INT8U second;
        INT8U milliseconds[2];
    };
}COSEM_DATETIME;
//���ܱ��ţ���ַ����
typedef struct{
    INT8U tag;
    INT8U len;
    INT8U value[6];
}COSEM_METER_NO;

//���ܱ��ţ���ַ����
typedef struct{
    INT8U tag;
    INT8U len;
    INT8U addr_len;
    INT8U value[6];
}COSEM_TSA;
typedef struct{
    INT8U tag;
    INT8U len;
    INT8U value[12];
}COSEM_COMM_PW,COSEM_ASSET_NO;

typedef struct{
    INT8U tag;
    INT8U len;
    INT8U value;
}COSEM_BIT_STRING;

typedef struct{
    INT8U tag;
    INT8U len;
    INT8U value[2];
}COSEM_BIT_STRING_2;

typedef struct{
    INT8U tag;
    INT8U value[7];
}COSEM_BCDDateTime;

typedef struct{
    INT8U tag;
    INT8U value[7];
}COSEM_DateTime_S;

typedef struct{
    INT8U tag;
    INT8U scaler;
    INT8U unit;
}COSEM_Scaler_Unit;
typedef struct{
    INT8U tag;
    INT8U  unit;
    INT8U value[2];
}COSEM_TI;
typedef struct{
    INT8U tag;
    INT8U value[2];
}COSEM_OI;

typedef struct{
    INT8U tag;
    INT8U count;
    COSEM_BCDDateTime sendtime;
    COSEM_TI          trans_delay_time;
}TimeTag;

typedef struct{
    INT8U choice;
    INT8U count;
    union{
    INT8U user_calss[2];
    INT8U meter_seq[2][2];
    INT8U value[2];
    };
}METER_SET; //�����

typedef union{
    INT8U value[2];
    INT8U choice;
    struct{
        INT8U choice;
        OBJECT_ATTR_DESC oad;
        INT8U value[17];  //���ǵ����ﻹ��ʱ�������ʼ�1.
    }choice_1_2_3;
    struct{
        INT8U choice;
        INT8U acq_time[7];
        METER_SET meter_set;
    }choice_4_5;
    struct{
        INT8U choice;
        INT8U acq_time_begin[7];
        INT8U acq_time_end[7];
        INT8U time_interval[3];
        METER_SET meter_set;
    }choice_6_7_8;
    struct{
        INT8U choice;
        INT8U last_idx;
    }choice_9;
    struct{
        INT8U choice;
        INT8U last_count;
        METER_SET meter_set;
    }choice_10;
}RSD;

typedef struct{
    COSEM_TSA meter_no;
    COSEM_ENUM baud_rate;
    COSEM_ENUM protocol;
    OBJECT_ATTR_DESC_TAG port;
    COSEM_COMM_PW comm_pw;
    COSEM_UNSIGNED fl_count;
    COSEM_UNSIGNED user_class;
    COSEM_ENUM conn_mode;
    COSEM_SIGNED16 rated_voltage;
    COSEM_SIGNED16 rated_current;
}METER_BASE_INFO;

typedef struct{
    INT8U tag;
    INT8U count;
    COSEM_TSA rtu_no;
    COSEM_ASSET_NO asset_no;
    COSEM_UNSIGNED16 pt;
    COSEM_UNSIGNED16 ct;
}METER_EXT_INFO;

typedef union{
    INT8U value[2];
    struct{
    COSEM_UNSIGNED16 meter_seq;   //��վ�������
    INT8U meter_rec_no[2];       //�ڲ���¼���
    METER_BASE_INFO meter_base_info;
    METER_EXT_INFO meter_ext_info;
    };
}COSEM_METER_INFO;

typedef union{
	INT8U value;
	struct{
		INT8U in_cycle:1;/* ���ڳ��������� */
		INT8U new_cycle:1;/* ������,���¼������� */
		INT8U finished:1;/* ��ɱ�־? TODO: */
		INT8U res:5;//
	};
}ctrl_flg;
typedef struct{
	INT8U 	task_running_type;
	INT8U   task_running_begin_hour;
	INT8U   task_running_begin_minute;
	INT8U   task_running_end_hour;
	INT8U   task_running_end_minute;
}task_running_time_description; // ��������ʱ������
typedef struct{
	INT8U task_id;
	INT16U run_frq;
	INT8U run_unit; //ִ��ʱ�䵥λ��0-�� 1-�� 2-ʱ 3-�� 4-��
	INT8U task_type;  //�������ͣ���ͨ�ɼ����¼��ȵȡ�
	INT8U plan_no;  //�������
	INT8U acq_type;  //�ɼ�����
	INT8U acq_content[3];
	INT8U master_oad[4];
	INT8U planOad[50][4];   //ָ�򷽰��·��ĳ���OAD,��������·�50��������ʹ��ĳ����ʱ��flash�ж�����	
	INT8U oadcnt;           //�洢��oad�ĸ���
	INT8U store_time_flag;  //�洢ʱ��ѡ������ʼʱ�䣬����23��59�ֵȵ�
	task_running_time_description  task_running_time;  // ��������ʱ��
	INT8U begin_date[7];    //
	INT8U end_date[7];      //
	INT8U cycle_date[7];// ��������ʱ�� 
	INT8U last_cycle[7];// 6~0 ������ʱ����
	ctrl_flg ctl_flg; /* ���Ʊ�־ */
	INT8U mask[8];/* ��ʼ���� */
	INT8U procs_mask[8];// ����������룬����������ִ�е�ʱ���ݴ�֮ǰ�����������
	INT8U point_num;/* �ɼ����� */
	INT8U read_num;/*  ��ȡ���� 0:������tiֵ */
	INT8U page_id[2];/* �洢page_id,�������߿��� */
}TASK_PLAN_ATTRI_DESCRIBE;  //������������
TASK_PLAN_ATTRI_DESCRIBE  gTaskPlanAttribute[MAX_TASK_COUNT];//-------------------------------
INT8U gtask_plan_sequence[2][MAX_TASK_COUNT]; //0����������������1�����ڷ�����������ʼ����ʼ��Ϊ0��

typedef struct{
	INT8U plan_id;
	INT8U acq_type;  //�ɼ�����
	INT8U store_time;  //�洢ʱ��ѡ������ʼʱ�䣬����23��59�ֵȵ�
	INT16U max_store;  //��������Ҫ�����Ĵ洢�ռ�
	INT8U  oad_cnt;    //������Ҫ�����oad����
}tag_PLAN_ATTRI_DESCRIBE;//������������
typedef struct{
	tag_PLAN_ATTRI_DESCRIBE  tagPlanDescribe[MAX_NORMAL_PLAN_COUNT];
	INT8U planOad[50][4];   //ָ�򷽰��·��ĳ���OAD,��������·�50��������ʹ��ĳ����ʱ��flash�ж�����
}PLAN_ATTRI_DESCRIBE;
PLAN_ATTRI_DESCRIBE  gPlanAttribute;//---------------------------------

#define DEV_VER_VENDOR_NO_POS      2ul
#define DEV_VER_SOFT_VER_POS       8ul
#define DEV_VER_SOFT_DATE_POS      14ul
#define DEV_VER_HARDWARE_VER_POS   22ul
#define DEV_VER_HARDWARE_DATE_POS  28ul
#define DEV_VER_VENDOR_CUSTOM_POS  36ul

#endif