#ifndef __OOP_OBJECT_DESCRIBE_H__
#define	__OOP_OBJECT_DESCRIBE_H__
#include "protocol_app_const_def.h"

typedef struct{
  INT8U index;      //����
  INT8U data_type;  //��������
  INT16U len;
}tagObject_Index;

typedef struct{
  INT8U Attri_Id;   //���Ա��
  INT8U data_type;  //��������
  INT16U len;
  tagObject_Index *index_list;  //������
  INT8U index_list_size;
  INT8U right;
}tagObject_Attribute;

typedef struct{
  INT16U obis;                   //OBIS
  INT16U attri_max_len;         //������󳤶�
  INT32U offset;                //ƫ�Ƶ�ַ
  tagObject_Attribute  *attri_list;            //���Ա�
  INT8U  attri_list_size;            //���Ա��С
}tagParamObject;

#define READ_ONLY 0x01
#define WRITE_ONLY 0x02

//GPRS���ò���
#define OA_MASTER_IP             0 //����IP��ַ
#define OA_MASTER_PORT           1 //���ö˿�

//�ն�ip���ò���
#define OA_IP_CONFIG_MODE        0 //�ն�IP
#define OA_DEV_IP                1 //�ն�IP
#define OA_SUBNET_MASK           2 //��������
#define OA_GETEWAY               3 //���ص�ַ
#define OA_PPPOE_USER            4 //�û���
#define OA_PPPOE_PWD             5 //����


#define OA_WORK_MODE             0 //����ģʽ
#define OA_ONLINE_KIND           1 //���߷�ʽ
#define OA_CONNECT_KIND          2 //���ӷ�ʽ
#define OA_CONNECT_USE_MODE      3 //���ӷ�ʽӦ��ģʽ
#define OA_LISTEN_PORT_LIST      4//11 //�����˿��б�
#define OA_APN                   5 //APN
#define OA_USER                  6 //�û���
#define OA_PASSWORD              7 //����
#define OA_PROXY_IP              8 //�����������ַ
#define OA_PROXY_PORT            9 //����˿�
#define OA_TIMEOUT_RESEND_COUNT  10 //��ʱʱ�估�ط�����
#define OA_HEART_CYCLE           11 //��������

////////////////////////////////////////////////////////////////////
////����ͨѶ����
#define OA_SMS_CENTER_NO            0   //�������ĺ���
#define OA_MSA_SIM_NO               1   //��վ����
#define OA_DEST_NO                  2   //����֪ͨĿ�ĺ���

//���߹���ͨ�����Ա�
#define OA_GPRS_PARAM                       0   //150��ͨ������
#define OA_GPRS_MASTER_PARAM                1   //60����վͨ�Ų�����
#define OA_SMS_PARAM                        2   //200������ͨ�Ų���
#define OA_GPRS_VER                         3   //50���汾��Ϣ
#define OA_ALLOW_PROTOCOL                   4   //20��֧�ֹ�Լ�б�
#define OA_GRPS_CCID                        5   //30��SIM����ICCID
#define OA_GRPS_IMSI                        6   //20��IMSI
#define OA_GPRS_CSQ                         7   //3���ź�ǿ��
#define OA_GPRS_SIM_ID                      8   //20��sim������
#define OA_GPRS_IP                          9   //20������ip

//����ַ���Ա�
#define OA_METER_ADDR                       0   //10������ַ

//���ҵ������Ա�
#define OA_CFG_VENDO                       0    //���̴���
#define OA_FRAME_TYPE                      1    //��־����

//ESAM����
#define OA_SAFE_MODE                       0
#define OA_READ_MODE                       6

BOOLEAN get_param_attribute(INT8U attri_id,const tagObject_Attribute *attr_list,INT8U attr_list_size,tagObject_Attribute *attri);
BOOLEAN get_param_object(INT16U obis,tagParamObject *object);
INT32U get_object_offset(INT16U obis);
INT16U get_attribute_offset(const tagObject_Attribute *attr_list,INT8U attr_list_size,INT8U attr_id);
INT16U get_index_offset(const tagObject_Index *index_list,INT8U index,INT8U count);
INT16U calculate_ms_size(INT8U *data,INT8U data_type);
INT16U calculate_ms_size_without_type(INT8U *data,INT8U data_type);
INT16U get_object_attribute_item_len(INT8U *data,INT8U data_type);
INT16U get_object_attribute_item_value(INT8U *data,INT8U data_type,BOOLEAN only_value,INT8U find_index,INT8U **retdata,INT8U *attr_head_size);
BOOLEAN check_attribute_valid(INT8U *data,INT16U len,INT8U object_type,const tagObject_Attribute *attr_list,INT8U attr_list_size);

INT8U get_data_type_len(INT8U data_type);
INT16U get_oop_data_type_len(INT8U *data,INT8U data_type);
INT16U get_attribute_inbuffer(INT8U index,INT8U *data_buffer,INT8U *attribute,INT16U max_len,BOOLEAN only_value,INT8U parent_index);

INT16U get_apdu_app_link_request_size(INT8U *data,INT16U max_len);
INT16U get_apdu_link_response_size(INT8U *data,INT16U max_len);
INT16U get_apdu_get_request_size(INT8U *data,INT16U max_len);
INT16U get_apdu_set_request_size(INT8U *data,INT16U max_len);
INT16U get_apdu_action_request_size(INT8U *data,INT16U max_len);
INT16U get_apdu_report_response_size(INT8U *data,INT16U max_len);
INT16U get_apdu_proxy_request_size(INT8U *data,INT16U max_len);
INT16U get_apdu_security_request_size(INT8U *data,INT16U max_len);
INT16U get_apdu_security_response_size(INT8U *data,INT16U max_len);

void default_esam_oi_list(INT16U obis,INT8U mask,INT16U safe_config);
INT32U get_system_flag(INT32U flag,INT8U type);
void   set_system_flag(INT32U flag,INT8U type);
void  clr_system_flag(INT32U flag,INT8U type);
void  clr_system_flag_type(INT8U type);
void  clr_system_flag_all();

void delete_esam_oi_list(INT8U oi[2]);
void update_esam_oi_list(INT8U data[4]);
INT16U get_esam_safe_config(INT16U obis);
void load_esam_oi_list(void);

#endif
 