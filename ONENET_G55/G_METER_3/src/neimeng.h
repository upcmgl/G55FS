#ifndef __PIC32MX_LLVC_NEIMENG_H__
#define __PIC32MX_LLVC_NEIMENG_H__
//�����ļ�����
#define     NMADDR_SET_F63               0                            //18432, ���õ�ѹ���������,һ�����18���ֽ�*1024=18432=18k
#define     NMADDR_SET_F90          NMADDR_SET_F63+18432             //10������������ѡ�����
#define     NMADDR_SET_F91          NMADDR_SET_F90+10                //11���ڶ�ͨѶͨ��������ʽ���ü�����ͨѶͨ�������ò���
#define     NMADDR_SET_F96          NMADDR_SET_F91+11                //5120�����õ���м�Լ��
#define     NMADDR_SET_F105         NMADDR_SET_F96+5120              //1025���ն˹�����м�����ַ����
#define     NMADDR_SET_F106         NMADDR_SET_F105+1025             //704���ն˵��ܱ��м�·�����ò���
#define     NMADDR_SET_F107         NMADDR_SET_F106+704             //11264,�������м�·�����ò���
#define     NMADDR_SET_F85          NMADDR_SET_F106+11264           //512,�����Ч��/���ص��û���־


//һ��������չ�ļ�����
#define     NMADDR_C1_RECSTATUS     0                               //3968,����״̬��1�����31�죬1024/8*31=3968,һ���ֽ�8����״̬

#define    NM_MAX_METER_COUNT       1024                            //���ɹ�Ŀǰ֧�ֵ����������
#define    ADD_METER_COUNT          10                              //������ӳ�䵽�������ʱ��Ҫ��64    
typedef union{
  INT8U value[40];
  struct{
    INT8U  ipport_1[6];
    INT8U  ipport_2[6];
    INT8U  gate_ipport[6];
    INT8U  proxy_ipport[6];
    INT8U  apn[16];
  }d;
  struct{
    INT8U  ipport_1[6];
    INT8U  ipport_2[6];
    INT8U  apn[16];
  }n;
}tagSET_F3_nm;
//����F63�����õ�ѹ���������
typedef union{
   INT8U  values[18];
   struct{
     INT8U  meter_idx[2];       //������
     INT8U  meter_type;              //�������01H��������485�ֱ� 02H���������ز��ֱ� 03H��������485̨���ܱ�
                                     //04H�������485�ֱ�05H��������ز��ֱ� 06H�������̨���ܱ�07H������������ֱ�08H��
                                     //�����������ܱ�09H�����������ֱ�  0AH������������ܱ�
     INT8U  meter_addr[6];     //����ַ ,6��BCD��
     INT8U  meter_const[2];     //�������1-65535
     INT8U  rtu_no[2];          //�����ɼ�����ַ1-65535����Ϊ�ܱ��ɼ����������485�ӿ�ֱ�ӳ��գ���ֵΪ0�������ز�����ֵΪ0��
     INT8U  rtu_port;           //�����ɼ������Ӻ�1-32��������������������ñ������ĸ��ɼ������ĸ����Ӻţ�����485�����Բ�ʹ�ã������ز�����Բ��ã�
     INT8U  ctrl_flag;          //�ص㻧������բ��־��ͣ�ñ�־����Լ����
     INT8U  data_item;          //�ɼ�������
     INT8U  read_interval;      //�ɼ���� ����СʱΪ��λ
     INT8U  meter_bytes;        //��λ��
   };
}tagSET_F63;

typedef union{
   INT8U  value;
   struct {
    INT8U protocol:3;
    INT8U use_flag:2;
    INT8U remote_ctrl:2;
    INT8U is_vip:1;
   };
}tag_F63_CTRL_FLAG;



void update_rec_meter_status_neimeng(INT8U day,INT16U meter_idx);
void init_rec_meter_status_neimeng(BOOLEAN is_all,INT8U cur_day);
INT16U  get_gb645_data(INT8U *gb645,INT16U frameLen,BOOLEAN is_33);
BOOLEAN query_meter_info_neimeng(INT8U *data,INT16U *itemDataLen,INT16U max_len);
INT8U set_ertu_meters_neimeng(INT8U *buffer,INT8U *params,INT16U *itemDataLen);
void update_f85_use_flag(INT16U meter_idx,BOOLEAN valid_flag);
void update_f85_vip_flag(INT16U meter_idx,BOOLEAN valid_flag);
INT16U plc_readRelay_645meter(INT8U *resp,INT16U meter_idx,INT16U item);
void update_meter_use_flag(INT16U meter_idx,INT8U is_use);
void update_vip_param(void);
INT8U set_ertu_F85_nm(INT8U *itemData,INT16U *itemDataLen);
INT8U set_ertu_meters_F96(INT8U *buffer,INT8U *itemData,INT16U *itemDataLen);
BOOLEAN query_ertu_meters_F96(INT8U *Reply,INT16U *itemDataLen,QUERY_CTRL *query_ctrl);
void calc_seg_time();
INT16U read_c1_f10(INT8U *data,READ_HIS *readC1);
INT16U read_c1_f11(INT8U *data,READ_HIS *readC1);
INT8U get_vip_meter_idx(INT16U meter_idx);


INT16U set_spot_params(INT16U spot_idx,INT32U eeAddr,INT8U *params,INT16U itemDataLen);
//F3:�����ն���վ����
INT8U set_ertu_msa_info_nm(INT8U *itemData,INT16U *itemDataLen);
INT16U query_nm_msa_info(INT8U *data);

//F7:�����ղ���
INT8U set_ertu_recday_info_nm(INT8U *itemData,INT16U *itemDataLen);
INT16U query_nm_recday_info(INT8U *data);


//F10:�ն˵��ܱ�/��������װ�����ò���
INT16U query_nm_meter_info(INT8U *data,INT16U max_len);
INT8U set_ertu_recinterval_info_nm(INT8U *itemData,INT16U *itemDataLen,INT8U port_type);
INT16U query_nm_recinterval_info(INT8U *data,INT8U port_type);
INT8U set_ertu_recstarttime_info_nm(INT8U *itemData,INT16U *itemDataLen);
INT16U query_nm_recstarttime_info(INT8U *data);
INT16U query_nm_ertu_F85(INT8U *data,QUERY_CTRL *query_ctrl);
void set_ertu_f9(INT8U *itemData);
INT16U query_ertu_f27(INT16U pn_idx,INT8U *itemData);
INT16U set_ertu_f27(INT16U pn_idx,INT8U *itemData);
INT16U  set_spot_params_nm(INT16U spot_idx,INT16U fn,INT8U *params,INT16U itemDataLen);
INT16U get_F27F28_C1_nm(INT8U *data,C1F27 *cur_F27,C1F28 *cur_F28);
INT8U set_ertu_F60_nm(INT8U *params,INT16U *itemDataLen);
INT16U query_ertu_F60_nm(INT8U *data);
INT8U set_ertu_F21_nm(INT8U *params,INT16U *itemDataLen);
INT16U query_ertu_F21_nm(INT8U *data);
INT8U set_ertu_F22_nm(INT8U *params,INT16U *itemDataLen);
INT16U query_ertu_F22_nm(INT8U *data);
INT16U set_ertu_F25_nm(INT16U spot_idx,INT8U *params);
INT16U query_ertu_F25_nm(INT16U spot_idx,INT8U *data);
INT16U set_ertu_F26_nm(INT16U spot_idx,INT8U *params);
INT16U query_ertu_F26_nm(INT16U spot_idx,INT8U *data);
INT8U set_ertu_F81_nm(INT8U *params,INT16U *itemDataLen);
INT16U query_ertu_F81_nm(INT8U *data);
INT16U query_nm_F14(INT8U *Reply);
void default_param_neimeng(void);
#endif

