#ifndef __APP_VAR_3761_H__
#define __APP_VAR_3761_H__

#include "../main_include.h"
//#include "app_dev_const.h"
//#include "app_const_3761.h"
//#include "app_var.h"
//#include "tpos_readport_common.h"

#define MAX_ERC_NO              64
//INT8U __attribute__ ((aligned(4))) g_temp_buffer[4096];         //ȫ����ʱ����������
INT8U   g_temp_buffer[100];
typedef struct{
     INT8U   acd;
     INT8U   ec1;         //��Ҫ�¼�������
     INT8U   ec2;         //һ���¼�������
     INT8U   alarm_event;//��ʾ�澯ʹ��
     INT8U   ercflag[8]; //F8�¼������־λBS64
     INT8U   event_ec[4];//32λ���¼�������
     INT8U   report_flag[256];
     INT8U   report_idx;
}tagGlobalEVENT;
extern tagGlobalEVENT g_event;
/*
//==============================================================================
//���ݸ�ʽ14:  XXXXXX.XXXX
typedef struct{
     INT8U   value[5];
}DATAFMT_14;


//���ݸ�ʽ11:  XXXXXX.XX
typedef struct{
     INT8U   value[4];
}DATAFMT_11;

*/
//���ݸ�ʽ15
/*
typedef union{
    INT8U   value[5];
    struct{
       INT8U   bcd_minute;
       INT8U   bcd_hour;
       INT8U   bcd_day;
       INT8U   bcd_month;
       INT8U   bcd_year;
    };
}DATAFMT_15;
*/
/*
//������Ϣͷ
typedef union{
   INT8U  value;
   struct{
      INT8U  number:4;  //��Ϣ���
      INT8U  type:4;    //��Ϣ���ࣺ 0:��ͨ��Ϣ�� 1����Ҫ��Ϣ
   };
}CTRLP_NOTE_INFO_TYPE;

#define NOTE_INFO_LEN  202
typedef union{
    INT8U  vlaue[2];
    struct{
       CTRLP_NOTE_INFO_TYPE   info_type;
       INT8U                  info_len;
    };
}CTRL_NOTE_INFO;
*/
//����F1���ն�����ͨ�ſ�ͨ�Ų���
typedef union{
    INT8U value[6];
    struct{
        INT8U    machine_delaytime;   //�ն���������ʱʱ��RTS����λ20ms
        INT8U    send_delaytime;      //�ն���Ϊ����վ�����ʹ�����ʱʱ�䣬��λmin
        INT16U   resp_timeout;        //�ն˵ȴ��Ӷ�վ��Ӧ�ĳ�ʱʱ����ط�����
        INT8U    confirm_service;     //��Ҫ��վȷ�ϵ�ͨ�ŷ���(CON=1)�ı�ʶ
        INT8U    heartcycle;          //�������ڣ���λmin
    };
}SET_F1;

typedef union{
    INT8U value[8];
    struct{
        SET_F1   F1;                  //F1�Ĳ���
        INT8U    reserve[2];          //Ԥ��
    };
}tagSET_F1;

//����F2���ն�����ͨ�ſ������м�ת������
typedef union{
    INT8U    value[33];
    struct{
        INT8U    ertu_number;         //��ת�����ն˵�ַ��n
        INT16U   ertu_addr[16];       //��ת���ն˵�ַ
    };
}SET_F2;

typedef union{
    INT8U    value[35];
    struct{
        SET_F2   F2;                  //F2�Ĳ���
        INT8U    reserve[2];          //Ԥ��
    };
}tagSET_F2;

//����F3�� �ն���վIP��ַ�Ͷ˿�
//APN,1376.1����Ϊ16�ֽ�
typedef union{
    INT8U   value[44];
    struct{
        INT8U   ip1[4];
        INT8U   port1[2];
        INT8U   ip2[4];
        INT8U   port2[2];
        INT8U   APN[32];
     };
     struct{
        INT32U   msa_ip1;
        INT16U   msa_port1;
        INT32U   msa_ip2;
        INT16U   msa_port2;
        INT8U    gprs_APN[32];
     }w;
}SET_F3;

typedef union{
    INT8U    value[50];
    struct{                 //������У��ֵ�������ۼӺ͵ļ���ó�
        SET_F3   F3;                  //F3�Ĳ���
        INT8U    reserve[6];          //Ԥ��
    };
}tagSET_F3;

//����F4����վ�绰����Ͷ������ĺ���
typedef union{
    INT8U    value;
    struct{
        INT8U    number1:4;    //����1
        INT8U    number2:4;    //����2
    };
}NUMBER;     //����F4�ĺ����ʽ

typedef union{
    INT8U    value[16];
    struct{
        NUMBER    phone_number[8];    //��վ�绰����
        NUMBER    sms_number[8];      //�������ĺ���
    };
}SET_F4;

typedef union{
    INT8U    value[24];
    struct{
        SET_F4   F4;                  //F4�Ĳ���
        INT8U    reserve[8];          //Ԥ��
  };
}tagSET_F4;

//����F5���ն�����ͨ����Ϣ��֤����
typedef union{
    INT8U    value[3];
    struct{
        INT8U    message_plan;        //��Ϣ��֤������
        INT16U   plan_param;         //��Ϣ��֤��������
    };
}SET_F5;

typedef union{
    INT8U    value[5];
    struct{
        SET_F5   F5;                  //F5�Ĳ���
        INT8U    reserve[2];          //Ԥ��
    };
}tagSET_F5;

//����F6���ն����ַ����
#ifdef __RTUADDR_IS_6BYTE__
typedef union{
    INT8U    value[32];
    struct{
        INT8U   group_addr[8][4];       //�ն����ַ����8��
    };    
}SET_F6;

typedef union{
    INT8U    value[35];
    struct{
        INT8U    cs;                  //������У��ֵ�������ۼӺ͵ļ���ó�
        SET_F6  F6;                   //F6�Ĳ���
        INT8U    reserve[2];          //Ԥ��
    };
}tagSET_F6;

typedef union{
    INT8U    value[16];
    struct{
        INT16U   group_addr[8];       //�ն����ַ����8��
    };
}SET_F6_1;

typedef union{
    INT8U    value[19];
    struct{
        INT8U    cs;                  //������У��ֵ�������ۼӺ͵ļ���ó�
        SET_F6_1  F6;                   //F6�Ĳ���
        INT8U    reserve[2];          //Ԥ��
    };
}tagSET_F6_1;
#else
typedef union{
    INT8U    value[32];
    struct{
        INT8U   group_addr[8][4];       //�ն����ַ����8��
    };    
}SET_F6_1;

typedef union{
    INT8U    value[19];
    struct{
        SET_F6_1  F6;                   //F6�Ĳ���
        INT8U    reserve[2];          //Ԥ��
    };
}tagSET_F6_1;

typedef union{
    INT8U    value[16];
    struct{
        INT16U   group_addr[8];       //�ն����ַ����8��
    };
}SET_F6;

typedef union{
    INT8U    value[18];
    struct{
        SET_F6  F6;                   //F6�Ĳ���
        INT8U    reserve[2];          //Ԥ��
    };
}tagSET_F6;
#endif
//����F7�� �ն�IP��ַ�Ͷ˿�
typedef union{
    INT8U  value[12];
    struct{
       //�ն�IP��ַ
       INT8U   ip1;
       INT8U   ip2;
       INT8U   ip3;
       INT8U   ip4;
       //��������
       INT8U   mask1;
       INT8U   mask2;
       INT8U   mask3;
       INT8U   mask4;
       //���ص�ַ
       INT8U   gate1;
       INT8U   gate2;
       INT8U   gate3;
       INT8U   gate4;
       //���������
       INT8U   proxy_type;
       INT8U   proxy_ip1;
       INT8U   proxy_ip2;
       INT8U   proxy_ip3;
       INT8U   proxy_ip4;
       INT8U   proxy_port[2];
       INT8U   proxy_con_type;
       INT8U   proxy_user_name_len;
       INT8U   proxy_user_name[20];
       INT8U   proxy_pass_len;
       INT8U   proxy_pass[20];
       //�ն������˿�
       INT8U   listen_port[2];
    };
    struct{
       INT32U   local_ip;
       INT32U   mask_ip;
       INT32U   gate_ip;
    };
    struct{
       INT8U   ip_info[20];
       INT8U   proxy_user_name_len;
       INT8U   proxy_user_pass[43];
    }b; 
}tagERTU_ETH_IP;


typedef union{
   INT8U  value[64];
   struct{
     tagERTU_ETH_IP  eth_ip;
   };
}SET_F7;

typedef union{
    INT8U  value[70];
    struct{
        SET_F7  F7;                   //F7�Ĳ���
        INT8U    reserve[6];          //Ԥ��
    };
}tagSET_F7;

//����F8���ն�����ͨ������ר��������ʽ
typedef union{
    INT8U  value[8];
    struct{
        INT8U  work_mode;              //����ͨ��ģ�飨GPRS��CDMA������ģʽ   BIN  1
        INT8U  redail_interval[2];     //�������ߡ�ʱ������ģʽ�ز����       BIN  2
        INT8U  passive_dailcount;      //��������ģʽ�ز�����                 BIN  1
        INT8U  passive_offline_time;   //��������ģʽ������ͨ���Զ�����ʱ��   BIN  1
        INT8U  time_segments[3];       //ʱ������ģʽ��������ʱ�α�־         BS24 3
    };
    struct{
        INT8U  online_mode:2;  //����ģʽ 1--����  2--����  3--ʱ��
        INT8U  tmp1:2;
        INT8U  serv_mode:2;  //������ģʽ��  0-���ģʽ  1--�ͻ���ģʽ  2--������ģʽ
        INT8U  tmp2:1;
        INT8U  conn_mode:1;//����ģʽ 0-TCP 1-UDP
    }mode;
}SET_F8;

typedef union{
    INT8U  value[11];
    struct{
        SET_F8   F8;
        INT8U    reserve[2];          //Ԥ��
    };
}tagSET_F8;

//����F9���ն��¼���¼����
typedef union{
    INT8U    value[32];
    struct{
        INT8U    event_valid[16];       //�¼���¼��Ч��ʶλ����λ��ʾ
        INT8U    evnet_VIP[16];          //�¼���Ҫ�Եȼ���ʶλ����λ��ʾ
    };
}SET_F9;

typedef union{
    INT8U    value[34];
    struct{
        SET_F9   F9;
        INT8U    reserve[2];          //Ԥ��
    };
}tagSET_F9;

//F11���ն���������(8·)
typedef union{
    INT8U    value[5];
    struct{
        INT8U    pulse_port;          //��������˿ں�
        INT8U    meter_idx;           //�����������
        INT8U    pulse_attr;          //�������ԣ�BS8��ʽ��
        INT16U   pulse_const;         //���ܱ���k
    };
}PULSE_PARAM;       //�������õĲ���

typedef union{
    INT8U    value[40];
    struct{
        PULSE_PARAM  pulse[8];       //�������ò���
    };
}SET_F11;

typedef union{
    INT8U    value[42];
    struct{
        SET_F11  F11;                 //F11�Ĳ���
        INT8U    reserve[2];          //Ԥ��
    };
}tagSET_F11;

//F12���ն�״̬���������
typedef union{
    INT8U    value[2];
    struct{
        INT8U    state_access;        //״̬�������ʶλ��BS8��ʽ����λ��ʾ
        INT8U    state_attr;          //״̬�����Ա�ʶλ��BS8��ʽ����λ��ʾ
    };
}SET_F12;

typedef union{
    INT8U    value[5];
    struct{
        INT8U    cs;                  //������У��ֵ�������ۼӺ͵ļ���ó�
        SET_F12  F12;                 //F12�Ĳ���
        INT8U    reserve[2];          //Ԥ��
    };
}tagSET_F12;

//F13���ն˵�ѹ/����ģ�������ò���(8·)
typedef union{
    INT8U    value[3];
    struct{
        INT8U    ad_port;             //��ѹ/����ģ��������˿ں�
        INT8U    meter_idx;           //�����������
        INT8U    ad_attr;             //��ѹ/����ģ��������
    };
}AD_PARAM;     //ģ�������õĲ���

typedef union{
    INT8U    value[24];
    struct{
        AD_PARAM  ad[8];              //��ѹ/����ģ�������ò���
    };
}SET_F13;

typedef union{
    INT8U    value[28];
    struct{
        INT8U    cs;                  //������У��ֵ�������ۼӺ͵ļ���ó�
        SET_F13  F13;                 //F13�Ĳ���
        INT8U    reserve[3];          //Ԥ��
    };
}tagSET_F13;

//�ܼӱ�־�Ľṹ��
typedef union{
    INT8U    value;
    struct{
        INT8U spot_idx:6;     //�������
        INT8U dir:1;        //����������ܼӼ����������Ƿ���0-����1-����
        INT8U flag:1;          //�ܼ�������������־��0-�ӣ�1-��
    };
}tagOP_FLAG;

//�ն��ܼ������ò�����Э���Ӧ�����ݸ�ʽ
typedef union{
    INT8U  value[10];
    struct{
       INT8U   agp;           //�ܼ������1-8
       INT8U   spot_count;    //����������(0-8)
       tagOP_FLAG   op_flag[8];    //�ܼӱ�־
    };
}tagAGP_SET;


typedef union{
    INT8U    value[83];
    struct{
        INT8U    cs;                  //������У��ֵ�������ۼӺ͵ļ���ó�
        tagAGP_SET   set_f14[8];      //���������64�����ղ������Ӧ�ܼ���ķ�ʽ�洢
        INT8U    reserve[2];          //Ԥ��
    };
}tagSET_F14;

//F15���й��ܵ������Խ���¼���������
typedef union{
    INT8U    value;
    struct{
        INT8U    time_interval:2;     //��������ʱ����,0��60min��1��30min��2��15min
        INT8U    reserve:5;           //Ԥ��
        INT8U    contrast_method:1;   //�Աȷ�����ʶ��0����ԶԱȣ�1�����ԶԱ�
    };
}DIFF_PARAM;      //�����ĵ�������ʱ�����估�Աȷ�����ʶ

typedef union{
    INT8U    value[9];
    struct{
        INT8U    diff_id;              //�й��ܵ�����������
        INT8U    contrast_agp;         //�Աȵ��ܼ������
        INT8U    refer_agp;            //���յ��ܼ������
        DIFF_PARAM  diff_param;        //�����ĵ�������ʱ�����估�Աȷ�����ʶ
        INT8U    relative_deviation;   //�Խ�����ƫ��ֵ
        INT32U   absolute_deviation;   //�Խ�޾���ƫ��ֵ
    };
}SET_F15;

typedef union{
    INT8U    value[75];
    struct{
        INT8U    cs;                  //������У��ֵ�������ۼӺ͵ļ���ó�
        SET_F15  set_f15[8];          //F15��������������������8
        INT8U    reserve[2];          //Ԥ��
    };
}tagSET_F15;

//F16������ר���û���������
typedef union{
    INT8U    value[64];
    struct{
        INT8U    net_username[32];     //����ר���û���
        INT8U    net_password[32];      //����ר������
    };
}SET_F16;


typedef union{
    INT8U    value[67];
    struct{
        INT8U    cs;                  //������У��ֵ�������ۼӺ͵ļ���ó�
        SET_F16  set_f16;             //F16�Ĳ���
        INT8U    reserve[2];          //Ԥ��
    };
}tagSET_F16;

//F17���ն˱�����ֵ
typedef union{
    INT8U    value[2];
    struct{
        INT8U    security_pyg[2];     //�ն˱�����ֵ�����ݸ�ʽ2
    };
}SET_F17;

typedef union{
    INT8U    value[5];
    struct{
        INT8U    cs;                  //������У��ֵ�������ۼӺ͵ļ���ó�
        SET_F17  set_f17;             //F17�Ĳ���
        INT8U    reserve[2];          //Ԥ��
    };
}tagSET_F17;

//F18���ն˹���ʱ��
typedef union{
    INT8U    value;
    struct{
        INT8U    power_ctrl_time1:2;             //ʱ��1��30min
        INT8U    power_ctrl_time2:2;             //ʱ��2��30min
        INT8U    power_ctrl_time3:2;             //ʱ��3��30min
        INT8U    power_ctrl_time4:2;             //ʱ��4��30min
    };
}POWER_CTRL_TIME;

typedef union{
    INT8U    value[12];
    struct{
        POWER_CTRL_TIME  power_ctrl_time[12];          //����ʱ�Σ�30min����48��ʱ��
    };
}SET_F18;

typedef union{
    INT8U    value[15];
    struct{
        INT8U    cs;                  //������У��ֵ�������ۼӺ͵ļ���ó�
        SET_F18  set_f18;             //F18�Ĳ���
        INT8U    reserve[2];          //Ԥ��
    };
}tagSET_F18;

//F19���ն�ʱ�ι��ض�ֵ����ϵ��
typedef union{
    INT8U    value;
    struct{
        INT8U    power_ctrl_float_coff;     //ʱ�ι��ض�ֵ����ϵ��
    };
}SET_F19;

typedef union{
    INT8U    value[4];
    struct{
        INT8U    cs;                  //������У��ֵ�������ۼӺ͵ļ���ó�
        SET_F19  set_f19;             //F19�Ĳ���
        INT8U    reserve[2];          //Ԥ��
    };
}tagSET_F19;

//F20���ն��µ������ض�ֵ����ϵ��
typedef union{
    INT8U    value;
    struct{
        INT8U    energy_ctrl_float_coff;     //�µ������ض�ֵ����ϵ��
    };
}SET_F20;

typedef union{
    INT8U    value[4];
    struct{
        INT8U    cs;                  //������У��ֵ�������ۼӺ͵ļ���ó�
        SET_F20  set_f20;             //F20�Ĳ���
        INT8U    reserve[2];          //Ԥ��
    };
}tagSET_F20;

//F21���ն˵���������ʱ�κͷ�����

//F22���ն˵���������

//F23���ն˴߷Ѹ澯����
typedef union{
    INT8U    value[3];
    struct{
        INT8U    fee_alarm_flag[3];     //�߷Ѹ澯����/��ֹ��ʶλ,BS8��ʽ,D0~D23��˳���Ӧ��ʾ0~23�㣬��1��ʾ�澯
    };
}SET_F23;

typedef union{
    INT8U    value[6];
    struct{
        INT8U    cs;                  //������У��ֵ�������ۼӺ͵ļ���ó�
        SET_F23  set_f23;             //F23�Ĳ���ֵ
        INT8U    reserve[2];          //Ԥ��
    };
}tagSET_F23;

//F25�������������������������
//Ϊ�����ȡ������PT��CT�������ݽṹ
typedef union{
    INT8U    value[4];
    struct{
       INT16U  pt;     //��ѹ����������
       INT16U  ct;     //��������������
    };
}SPOT_PTCT;

typedef union{
    INT8U    value[11];
    struct{
        SPOT_PTCT  spot_ptct;        //PT��CT����
        INT16U     v_rated;          //���ѹ����λV
        INT8U      i_vated;          //���������λA
        INT8U      p_vated[3];       //����ɣ���λkVA
        INT8U      connect_mode;     //��Դ���߷�ʽ��BS8��ʽ��D0~D1��ʾ�������Դ���߷�ʽ��D2~D3��ʾ���������࣬D4~D7����
    };
}SET_F25;

typedef union{
    INT8U    value[14];
    struct{
        INT8U    cs;                  //������У��ֵ�������ۼӺ͵ļ���ó�
        SET_F25  set_f25;             //F25�Ĳ���ֵ
        INT8U    reserve[2];          //Ԥ��
    };
}tagSET_F25;

//F26����������ֵ��������������
typedef union{
    INT8U  value[5];
    struct{
       INT8U   set_max[2];          //����
       INT8U   last_time;           //����ʱ��
       INT8U   back_coff[2];        //�ָ�ϵ��
    };
}SET_F26_OVER_V;

typedef union{
    INT8U  value[6];
    struct{
       INT8U   set_max[3];          //����
       INT8U   last_time;           //����ʱ��
       INT8U   back_coff[2];        //�ָ�ϵ��
    };
}SET_F26_OVER_I;


typedef union{
   INT8U value[57];
   struct{
      INT8U           v_max[2];       //��ѹ�ϸ�����
      INT8U           v_min[2];       //��ѹ�ϸ�����
      INT8U           v_lost[2];      //��ѹ��������
      SET_F26_OVER_V  v_over_maxmax;  //��ѹ������
      SET_F26_OVER_V  v_over_minmin;  //��ѹ������
      SET_F26_OVER_I  i_over_maxmax;  //�����������
      SET_F26_OVER_I  i_over_max;     //���������
      SET_F26_OVER_I  in_over_max;    //���������
      SET_F26_OVER_I  s_over_maxmax;  //���ڹ��ʳ�������
      SET_F26_OVER_I  s_over_max;     //���ڹ��ʳ�����
      SET_F26_OVER_V  v_unb_over;     //��ѹ��ƽ��
      SET_F26_OVER_V  i_unb_over;     //������ƽ��
      INT8U           lost_v_last_time;  //����ʧѹʱ����ֵ
   };
}SET_F26;

typedef union{
    INT8U    value[60];
    struct{
        INT8U    cs;                  //������У��ֵ�������ۼӺ͵ļ���ó�
        SET_F26  set_f26;             //F26�Ĳ���
        INT8U    reserve[2];          //Ԥ��
    };
}tagSET_F26;


//F27��������ͭ�������������������

//F28�������㹦�������ֶ���ֵ����������

//F29���ն˵��ص��ܱ���ʾ�ţ���������

//F30��̨�����г���ͣ��/Ͷ�����ã���������
typedef union{
    INT8U    value;
    struct{
        INT8U    rec_flag;          //���г���ͣ��/Ͷ������
    };
}SET_F30;

typedef union{
    INT8U    value[4];
    struct{
        INT8U    cs;                  //������У��ֵ�������ۼӺ͵ļ���ó�
        SET_F30  set_f30;            //F30�Ĳ���ֵ
        INT8U    reserve[2];          //Ԥ��
    };
}tagSET_F30;


//F31���ӽڵ㸽���ڵ��ַ

//F33���ն˳������в�������
typedef union{
    INT8U    value[110];
    struct{
        INT8U    port;          // �ն�ͨ�Ŷ˿ں�	BIN	1	�������õ�
        INT8U    run_ctrl[2];   // ̨�����г������п�����	BS16	2
        INT8U    rec_days[4];   // ������-����	BS32	4
        INT8U    rec_time[2];   // ������-ʱ��	���ݸ�ʽ19	2
        INT8U    cycle;         // ������         BIN     1
        INT8U    cast_time[3];  // �Ե��㲥Уʱ��ʱʱ��	���ݸ�ʽ18	3
        INT8U    seg_count;       // ����ʱ������
        INT8U    rec_timeseg[96]; // ����ʱ����Ϣ   ʱ-��
    };
    struct{
        INT8U    tmp[13];            //����
        INT8U    seg_count;          // ����ʱ������
        INT8U    rec_timeseg[24][4]; //24��ʱ��
    }seg;
}SET_F33;

typedef union{
    INT8U    value[113];
    struct{
        INT8U    cs;                  //������У��ֵ�������ۼӺ͵ļ���ó�
        SET_F33  set_f33;             //F33�Ĳ���
        INT8U    reserve[2];          //Ԥ��
    };
}tagSET_F33;




typedef struct{
    INT8U    rec_days[4];        // ������-����	BS32	4
    INT8U    seg_count;          // ����ʱ������
    INT8U    rec_timeseg[24][4]; //24��ʱ��
}JCSET_F801;

typedef union{
    INT8U value[4];
    struct{
        INT8U    year;
        INT8U    month;
        INT8U    day;
        INT8U    hour;
        INT8U    v[6];
        INT8U    i[9];
    };
}CURVE_V_I;


//������ F33��  DL698_41_0928 �汾����
typedef struct{
    INT8U    port;            // �ն�ͨ�Ŷ˿ں�	BIN	1	�������õ�
    INT8U    run_ctrl[2];     // ̨�����г������п�����	BS16	2
    INT8U    rec_timeseg[12]; // ����ʱ����Ϣ  15���Ӽ������96��λ��ǽ�ֹ������1==��ֹ
    INT8U    rec_days[4];     // ������-����	BS32	4
    INT8U    rec_time[2];     // ������-ʱ��	���ݸ�ʽ19	2
    INT8U    cycle;           // ������         BIN     1
    INT8U    cast_time[3];    // �Ե��㲥Уʱ��ʱʱ��	���ݸ�ʽ18	3
}SET_F33_DL698_41_0928;


//����: �ն�֧�������������õĳ���˿�: 1--�ز���   2--����485��   3--����485��(�������Ϊ����,�򲻿���)
//����Э����ж�Ӧ�˿ڵ�֧�֣�1--����485��2--����485��3--����485��31--�ز���
//typedef struct{
//    JCSET_F33  port_set[4];
//}SET_F33_ALL;


//F34������������ͨ��ģ��Ĳ�������

//F35��̨�����г����ص㻧����

//F36���ն�����ͨ��������������

//F37���ն˼���ͨ�Ų���

//����/��������־�������Ӧ�ı�����/�������ն˸���
typedef union{
   INT8U value;
   struct{
      INT8U  count:4;        //��slave=1ʱ��count=1,��ʾ�������նˡ�
      INT8U  reserved:3;
      INT8U  slave:1;        //0"��ʾ��������"1"��ʾ��������
   };
}CASCADE_FLAG;


//ͨ�ſ�����
typedef union{
    INT8U   port_ctrl;
    struct{
       INT8U   databits:2;    //����λ��
       INT8U   parity:1;      //żУ��
       INT8U   has_parity:1;  //����У��λ
       INT8U   stopbits:1;    //ֹͣλ
       INT8U   baudrate:3;    //������
    };
}PORT_COMM_CTRL;


typedef union{
   INT8U  value[23];
   struct{
       INT8U           port;             //�ն˼���ͨ�Ŷ˿ں�	BIN		1
       PORT_COMM_CTRL  ctrl;             //�ն˼���ͨ�ſ�����	BS8		1
       INT8U           frame_timeout;    //���յȴ����ĳ�ʱʱ��	BIN	100ms	1
       INT8U           byte_timeout;     //���յȴ��ֽڳ�ʱʱ��	BIN	10ms	1
       INT8U           try_count;        //������������վ������ʧ���ط�����	BIN	��	1
       INT8U           check_interval;   // ����Ѳ������	BIN	min	1
       CASCADE_FLAG    cascade_flag;     //����/��������־�������Ӧ�ı�����/�������ն˸���n	BIN		1
       INT8U           ertu_addr[4][4];  //4�����������ն�����������/���������ն�����������	BCD		2
   };
}SET_F37;

typedef union{
    INT8U    value[28];
    struct{
        INT8U    cs;                  //������У��ֵ�������ۼӺ͵ļ���ó�
        SET_F37  set_f37;             //F37�Ĳ���
        INT8U    reserve[4];          //Ԥ����һ���ն˵�ַ�ĳ���
    };
}tagSET_F37;

//F38��I��������������

//F39��II��������������

//F41��ʱ�ι��ض�ֵ
typedef union{
    INT8U    value[17];
    struct{
        INT8U    timeseg_flag;  //ʱ�κţ�D0~D7��˳���λ��ʾ��1~��8ʱ��
        INT8U    timeseg1_power_ctrl_value[2];    //ʱ��1���ʶ�ֵ
        INT8U    timeseg2_power_ctrl_value[2];    //ʱ��2���ʶ�ֵ
        INT8U    timeseg3_power_ctrl_value[2];    //ʱ��3���ʶ�ֵ
        INT8U    timeseg4_power_ctrl_value[2];    //ʱ��4���ʶ�ֵ
        INT8U    timeseg5_power_ctrl_value[2];    //ʱ��5���ʶ�ֵ
        INT8U    timeseg6_power_ctrl_value[2];    //ʱ��6���ʶ�ֵ
        INT8U    timeseg7_power_ctrl_value[2];    //ʱ��7���ʶ�ֵ
        INT8U    timeseg8_power_ctrl_value[2];    //ʱ��8���ʶ�ֵ
    };
}TIMESEG_CTRL_PLAN;

typedef union{
    INT8U    value[52];
    struct{
        INT8U    timeseg_plan_flag;   //������ʶ,D0~D2��λ��ʾ1~3�׷�������1��Ч
        TIMESEG_CTRL_PLAN    timeseg_plan_value[3];
    };
}SET_F41;

typedef union{
    INT8U    value[55];
    struct{
        INT8U    cs;
        SET_F41  set_f41;
        INT8U    reserve[2];
    };
}tagSET_F41;

//F42�����ݹ��ز���
typedef union{
    INT8U    value[6];
    struct{
        INT8U   relax_ctrl_set_value[2];    //���ݿض�ֵ
        INT8U   limit_power_begin_time[2];  //�޵���ʼʱ��
        INT8U   limit_power_duration;       //�޵����ʱ��(��ΧΪ1~48)
        INT8U   limit_power_day;            //ÿ���޵���:D1~D7��ʾ����һ~�����գ�D0=0
    };
}SET_F42;

typedef union{
    INT8U    value[9];
    struct{
        INT8U    cs;
        SET_F42  set_f42;
        INT8U    reserve[2];
    };
}tagSET_F42;

//F43�����ʿ��ƵĹ��ʼ��㻬��ʱ��
typedef struct{
    INT8U    cal_power_winsize;   //���ʿ��ƵĹ��ʼ��㻬��ʱ��(��ֵ��Χ��1~60)
}SET_F43;

typedef union{
    INT8U    value[4];
    struct{
        INT8U    cs;
        SET_F43  set_f43;
        INT8U    reserve[2];
    };
}tagSET_F43;

//F44��Ӫҵ��ͣ�ز���
typedef union{
    INT8U    value[8];
    struct{
        INT8U  stopwork_ctrl_begin_time[3];    //��ͣ��ʼʱ��
        INT8U  stopwork_ctrl_end_time[3];      //��ͣ����ʱ��
        INT8U  stopwork_ctrl_set_value[2];     //��ͣ�ع��ʶ�ֵ
    };
}SET_F44;

typedef union{
    INT8U    value[11];
    struct{
        INT8U    cs;
        SET_F44  set_f44;
        INT8U    reserve[2];
    };
}tagSET_F44;

//F45�������ִ��趨
typedef struct{
    INT8U    power_ctrl_sw_flag;    //�����ִ��趨
}SET_F45;

typedef union{
    INT8U    value[4];
    struct{
        INT8U    cs;
        SET_F45  set_f45;
        INT8U    reserve[2];
    };
}tagSET_F45;

#if ((defined __QGDW_376_2013_PROTOCOL__) && (!defined __QGDW_FK2005__))
//F46���µ����ض�ֵ
typedef union{
    INT8U    value[5];
    struct{
        INT8U  monthfee_ctrl_set_value[4];   //�µ����ض�ֵ
        INT8U  alarm_limit_coff;             //��������ֵϵ��
    };
}SET_F46;
#else
//F46���µ����ض�ֵ
typedef union{
    INT8U    value[4];
    struct{
        INT8U  monthfee_ctrl_set_value[4];   //�µ����ض�ֵ
    };
}SET_F46;
#endif
#if ((defined __QGDW_376_2013_PROTOCOL__) && (!defined __QGDW_FK2005__))
typedef union{
    INT8U    value[8];
    struct{
        INT8U    cs;
        SET_F46  set_f46;
        INT8U    reserve[2];
    };
}tagSET_F46;
#else
typedef union{
    INT8U    value[7];
    struct{
        INT8U    cs;
        SET_F46  set_f46;
        INT8U    reserve[2];
    };
}tagSET_F46;
#endif

//F47��������(��)�ز���
typedef union{
    INT8U    value[17];
    struct{
        INT8U    buyfee_number[4];      //���絥��
        INT8U    buyfee_flag;           //׷��/ˢ�±�ʶ
        INT8U    buyfee_value[4];       //������(��)ֵ
        INT8U    alarm_limit_value[4];   //��������ֵ
        INT8U    switch_limit_value[4];  //��բ����ֵ
    };
}SET_F47;

typedef union{
    INT8U    value[20];
    struct{
        INT8U    cs;
        SET_F47  set_f47;
        INT8U    reserve[2];
    };
}tagSET_F47;

//F48������ִ��趨
typedef struct{
    INT8U  fee_ctrl_sw_flag;    //����ִ��趨
}SET_F48;

typedef union{
    INT8U    value[4];
    struct{
        INT8U    cs;
        SET_F48  set_f48;
        INT8U    reserve[2];
    };
}tagSET_F48;

//F49�����ظ澯ʱ��
typedef struct{
    INT8U  power_ctrl_alarm_time[8];    //���ظ澯ʱ��,8���ִ�
}SET_F49;

typedef union{
    INT8U    value[11];
    struct{
        INT8U    cs;
        SET_F49  set_f49;
        INT8U    reserve[2];
    };
}tagSET_F49;

//F57���ն������澯����/��ֹ����

//F58���ն��Զ��������

//F59�����ܱ��쳣�б�ֵ�趨

//F60��г����ֵ

//F61��ֱ��ģ�����������

//F65����ʱ�ϱ�I��������������

//F66����ʱ�ϱ�II��������������

//F67����ʱ�ϱ�I��������������/ֹͣ����

//F68����ʱ�ϱ�II��������������/ֹͣ����

//F73������������

//F74��������Ͷ/�����в���

//F75����������������

//F76��������Ͷ/�п��Ʒ�ʽ

//F81��ֱ��ģ��������仯

//F82��ֱ��ģ������ֵ

//F83��ֱ��ģ�����������

//F89���ն��߼���ַ

//F90����������ͨ�Ų���

//F91���ն˵���λ����Ϣ
//F97:ͣ�����ݲɼ����ò���
typedef union
{
    INT8U value;
    struct{
        INT8U is_use:1;     //�Ƿ���Ч
        INT8U is_random:1;  //�Ƿ����
        INT8U reserve:6;    //����
    };
}tagPOWER_ONOFF_READ_FLAG;
typedef union{
    INT8U    value[13];
    struct{
        tagPOWER_ONOFF_READ_FLAG    read_flag; //ͣ�����ݲɼ���־
        INT8U    read_interval_time;           //ͣ���¼���ȡʱ����
        INT8U    read_max_time;                //ͣ���¼���ȡʱ����ֵ
        INT8U    meter_count[2];               //Ҫ��ȡͣ���¼����ܱ����,���֧��4��
        INT8U    meter_list[8];                //Ҫ��ȡ���ܱ�������,���֧��4��
    };
}tagSET_F97;
//F98��ͣ���¼������ֵ����
typedef union{
    INT8U    value[13];
    struct{
        INT8U    power_lost_min_time[2];                 //ͣ��ʱ����С��Ч���
        INT8U    power_lost_max_time[2];                 //ͣ��ʱ�������Ч���
        INT8U    start_stop_time_deviation[2];           //ͣ���¼���ֹʱ��ƫ����ֵ
        INT8U    time_interval_deviation[2];             //ͣ���¼�ʱ������ƫ����ֵ
        INT8U    power_off_v_limit[2];                   //�ն�ͣ�緢����ѹ��ֵ
        INT8U    power_on_v_limit[2];                    //�ն�ͣ��ָ���ѹ��ֵ
        INT8U    report_flag;                            //ͣ���¼������ϱ���־λ
    };
}tagSET_F98;
typedef struct
{
    tagSET_F98  power_onoff_param;
    tagSET_F97  power_onoff_config;
}tagPowerOnoffEvent;
//F150��������״̬

//�ɼ����¹����ĵ����Ϣ
typedef union{
    INT8U  value[128];
    struct{
       INT16U  meter_seq[64];
    };
}CJQ_METER_INFO;


//
//ͨ�����ʼ�ͨ�Ŷ˿ںţ�
// 	D0~D4�����ʾ���ܱ���������װ�ý����ն˵�ͨ�Ŷ˿ںţ���ֵ��Χ1~31��
// 	D5~D7�����ʾ���ܱ���������װ�ý����ն˵�ͨ�Ų����ʣ� 0~7���α�ʾ300��600��1200��2400��4800��7200��9600��19200��
//
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

//
// �����㵵����Ϣ,�������ò���F10
//

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




//
//  �������Ϣ
//
typedef struct{
    INT8U       actphase;       // �ز�����ͨ����λ	BS8	1
    INT8U       plc_q;           // �ز��ź�Ʒ��	BS8	1
}METER_REC_STATE;   //18�ֽ�


//��ӽڵ�Ľڵ���Ϣ
typedef union{
   INT8U   value[9];
   struct{
      INT8U node_no[6];
      INT8U node_seq[2];
      INT8U node_sepc;
   };
}ROUTER_NODE_ITEM;

#ifdef __376_2_2013__
typedef union{
    INT8U value[7];
    struct{
        INT8U node_no[6];
        INT8U node_sepc;
    };
}ROUTER_NODE_ITEM_NEW;
#endif

//������ӽڵ���Ϣ,5���ڵ����
typedef union{
   INT8U value[46];
   struct{
        INT8U count;
      ROUTER_NODE_ITEM  node[5];
   };
    #ifdef __376_2_2013__
    struct{
        INT8U count;
        ROUTER_NODE_ITEM_NEW node[5];
    }b;
    #endif
}ROUTER_ADD_5NODE_INFO;

//ɾ���ز��ӽڵ�ڵ�
typedef union{
   INT8U value[7];
   struct{
      INT8U count;     //�����ȡ1
      INT8U node_no[6];
   };
}ROUTER_DEL_NODE_INFO;

typedef union{
   INT8U value[7];
   struct{
      INT8U count;
      INT8U node_no[10][6];
   };
}ROUTER_NOTICE_NODE_INFO;




//������м�����λ��Ϣ
typedef union{
   INT8U   value;
   struct{
      INT8U   readphase:2;    //������λ
      INT8U   relay:5;        //�м���� 0-15
      INT8U   flag:1;         //�ɹ���־
   };
}RELAYPHASE;

//������м�����λ��Ϣ
typedef union{
    INT8U   value;
    struct{
        INT8U is_meter_event:1;    //�Ƿ񳭶�����¼�������   __METER_EVENT_GRADE__
        INT8U is_curve:1;          //�Ƿ񳭶��������������   __METER_CURVE_READ__
        INT8U is_report_meter_event:1;     //�Ƿ񳭶��������������   __METER_CURVE_READ__
        INT8U tmp:5;               //����
    };
}REC_CTRL_INFO;



//����ʱ��
typedef union
{
    INT8U value[4];
    struct{
       INT8U  from_hour;
       INT8U  from_minute;
       INT8U  end_hour;
       INT8U  end_minute;
    };
}REC_TIMESEG;





//DL/T698  ���ݸ�ʽ01
/*
typedef union{
     INT8U     bcd_datetime[6];
     struct{
       INT8U       second;
       INT8U       minute;
       INT8U       hour;
       INT8U       day;
       INT8U       week_month;
       INT8U       year;
     };
}DATAFMT_01;
 */
/*
//�ص��������
typedef union{
    INT8U  value[8];
    struct{
        INT8U  year;
        INT8U  month;
        INT8U  day;
        INT8U  hour;
        INT8U  zxygz[4];
        #if defined (__PLC_READ_FXYG__) || defined(__PLC_READ_VIP_FYG_WG__)
        INT8U  fxygz[4];
        #endif
        #ifdef __PLC_READ_VIP_FYG_WG__
        INT8U  zxwgz[4];
        INT8U  fxwgz[4];
        #endif
        #if defined(__PLC_READ_VIP_F25__) || defined(__PLC_READ_VIP_LOAD_RECORD__)
        INT8U p_yg[12];      //��,A,B,C���й�����  ���ݸ�ʽ09  4*3
        INT8U p_wg[12];      //��,A,B,C���޹�����  ���ݸ�ʽ09  4*3
        INT8U Voltage[6];    //A,B,C���ѹ  ���ݸ�ʽ07   3*2
        INT8U I[12];         //A,B,C,N�����  ���ݸ�ʽ25   4*3
        INT8U Q[8];          //��,A,B,C�๦������   4*2
        #endif
    };
}C_VIP_ZXYGZ;

//��ѹ��һ������
typedef union{
    INT8U  value[7];
    struct{
        INT8U  rec_date[5];
        INT8U  Voltage_A[2];

    };
}C_VOLTMETER;
typedef union{
    INT8U  value[23];
    struct{
        INT8U  rec_date[5];
        INT8U  Block[15];

    };
}C_VOLTMETER_BLOCK;  //���ɼ�¼�ĸ�ʽ
typedef union{
    INT8U  value[37];
    struct{
        INT8U  rec_date[5];
        INT8U  Block[24];
        INT8U  glys[8];
    };
}C_POWER_BLOCK;  //���ɼ�¼�ĸ�ʽ

typedef union{
    INT8U  value[9];
    struct{
        INT8U  rec_date[5];
        INT8U  CURVE_YWG[4];

    };
}C_CURVE_PATCH;

//�ص��������

#if (defined(__PLC_READ_FXYG__) && defined(__PLC_READ_VIP_F25__))
  //���������й���F25�Ķ��� 
  typedef union{
     INT8U  value[8];
     struct{
        INT8U  year;
        INT8U  month;
        INT8U  day;
        INT8U  hour;
        INT8U  zxygz[4];

        //__PLC_READ_FXYG__
        INT8U  fxygz[4]; 
    
        //__PLC_READ_VIP_F25__
        INT8U p_yg[12];      //��,A,B,C���й�����  ���ݸ�ʽ09  4*3
        INT8U p_wg[12];      //��,A,B,C���޹�����  ���ݸ�ʽ09  4*3
        INT8U Voltage[6];    //A,B,C���ѹ  ���ݸ�ʽ07   3*2
        INT8U I[12];         //A,B,C,N�����  ���ݸ�ʽ25   4*3 
     };
  }C_VIP_ZXYGZ;
#elif defined(__PLC_READ_FXYG__)
   //���������й��Ķ���
   typedef union{
     INT8U  value[8];
     struct{
        INT8U  year;
        INT8U  month;
        INT8U  day;
        INT8U  hour;
        INT8U  zxygz[4];

        //__PLC_READ_FXYG__
        INT8U  fxygz[4];     
    };
   }C_VIP_ZXYGZ;
#elif defined(__PLC_READ_VIP_F25__)
   //����F25�Ķ���
   typedef union{
      INT8U  value[8];
      struct{
           INT8U  year;
           INT8U  month;
           INT8U  day;
           INT8U  hour;
           INT8U  zxygz[4];

           //__PLC_READ_VIP_F25__
           INT8U p_yg[12];      //��,A,B,C���й�����  ���ݸ�ʽ09  4*3
           INT8U p_wg[12];      //��,A,B,C���޹�����  ���ݸ�ʽ09  4*3
           INT8U Voltage[6];    //A,B,C���ѹ  ���ݸ�ʽ07   3*2
           INT8U I[12];         //A,B,C,N�����  ���ݸ�ʽ25   4*3 
      };
   }C_VIP_ZXYGZ;
#elif defined(__PROVICE_JIANGSU__)
typedef union{
    INT8U  value[8];
    struct{
        INT8U  year;
        INT8U  month;
        INT8U  day;
        INT8U  hour;
        INT8U  zxygz[4];
        INT8U  zxwgz[4];
        INT8U  fxygz[4];
        INT8U  fxwgz[4];    
    };
}C_VIP_ZXYGZ;
#else
     //��׼����
     typedef union{
         INT8U  value[8];
         struct{
            INT8U  year;
            INT8U  month;
            INT8U  day;
            INT8U  hour;
            INT8U  zxygz[4];
      };
     }C_VIP_ZXYGZ;

#endif   */


//�Զ��壺GPRS��Ϣ
typedef union{
   INT8U value[66];
   struct{
      INT8U csq;           //�ź�ǿ��
      INT8U ber;           //������
      INT8U op;            //��ǰ����
      INT8U gprs_end_state;         //����״̬
      char          temp;          //��ǰ�¶�
      INT8U mac[6];        //mac��ַ
      INT8U g14_ver[8];    //g14�汾��Ϣ��V3.0û��
      INT8U ctrl_ver[5];   //����ģ��汾��Ϣ
      INT8U soft_ver[7];   //�ڲ��汾
      INT8U provice[3];   //ʡ�������֣�V3.0��ʱû��
      INT8U ram_status;   //�ڴ��Ƿ���,AA-���� 0-��
      INT8U osc_status;   //�������Ƿ��쳣��0xAA-���� 1-�쳣
      INT8U rtcc_status;   //RTCC����״̬��0xAA-���� 1-�쳣
      INT8U error_status;//D0-����ͨѶ����,1Ϊ���� D1-ʱ�ӹ���״̬ D2-����ͨѶ���� D3-ESAMͨѶ����
      INT8U check_key;                //��װ��ⰴ��bit7-2��6��������1��⵽��0δ��⵽����bit1-0: 01 ����������⣻
      INT8U reserved[5];      //����
      INT8U sim_id[20];    //SIM��ID
      INT8U djb_ver[16]; //�����汾 
      INT8U gprs_type[3];//ģ�����ͣ��ַ�
      INT8U gprs_imsi[15];//imsi
      INT8U config_ver[2];           //���ýű��ļ��汾
      INT8U config_provice[4];       //���ýű��ļ�ʡ�ݴ���
      INT8U bad_block_count[2];       //Flash�л�������
      INT8U esam_type;//01-13�淶esam��02-698 esam������-δ֪
   };
}tagWORKINFO;
/*
typedef union{
    INT8U  value[2];
    struct{
        INT16U   meter_idx:11;
        INT16U   rec_flag:5;
    };
}PATCH_REC_FLAG;

typedef union{
    INT8U  value[5];
    struct{
        INT8U item[4];
        INT8U level;
    };
}PARAM_F106;  //#ifdef __METER_EVENT_GRADE__

////////////////////////////////////////////////////////////////////
//
//         ��ѯ�����ݽṹ
//
////////////////////////////////////////////////////////////////////
*/
typedef union
{
   INT8U  first_flag;
   struct{
      INT8U  first_F10_flag:1;
      INT8U  first_F89_flag:1;
      INT8U  first_F200_flag:1;
      INT8U  first_F112_flag:1;
      INT8U  first_F106_flag:1;   //__METER_EVENT_GRADE__
      INT8U  first_F306_flag:1;   //__BATCH_TRANSPARENT_METER_CYCLE_TASK__
      INT8U  first_F310_flag:1;
      INT8U  reserved:1;
   };
}QUERY_FIRST_CTRL;

typedef struct{
     INT8U              *req;         //�������
     INT16U             req_bytes;    //�������������ֽڳ���
     INT16U             from_seq;     //��ʼ����λ��
     INT16U             req_pos;      //��ʼ��������λ��
     INT16U             left_resp_len; //��ǰ������Ӧ�ı��Ŀռ�
     QUERY_FIRST_CTRL   first_ctrl;  //�״δ����־
}QUERY_CTRL;

/*
//�ӽ������ò��������
typedef union{
      INT8U   value[29];
      struct{
             INT16U       set_count;    //��������������������1
             INT16U       meter_seq;   //������ţ�����������
             INT16U       spot_idx;    //������ţ����������룬Ĭ��Ϊ������
             BAUD_PORT    baud_port;     //���ʼ��˿�
             INT8U        protocol;       //���Э��
             INT8U        meter_no[6];    //����ַ�����ֽ���ǰ
             INT8U        meter_pass[6];   //���ͨ�����룬Ĭ��ΪFFFFFFFFFF
             INT8U        meter_fl_count;  //���ܱ���ʸ���
             INT8U        meter_frac_info;  //���ʾֵ����λ��С��λ������Ϣ
             INT8U        rtu_no[6];       //�����ɼ�����ַ��485�����Ҫ���ز��������Ҫ��Ĭ����00
             METER_CLASS meter_class;    //�û�����ţ�С��ţ�ѡ�����롣
      };
}DISSET_SPOT_PARAMS;



//��ɫ��������������
typedef union{
   INT8U value[85];
   struct{
      INT8U          rec_date[5];
      INT8U          data[80];
   }b;
}C2_F13,C2_F14;

typedef union{
   INT8U value[53];
   struct{
      INT8U          rec_date[5];
      INT8U          data[48];
   }b;
}C2_F15;

//һ�����ݣ�F127������¼���¼״̬
typedef union{
   INT8U  value[7];
   struct{
      INT8U          rec_date[5];
      INT8U          data[2];
   }b;
}C1_F127;

//��ɫ�е�ǰ����
typedef union{
   INT8U  value[90];
   struct{
      INT8U          rec_date[5];
      INT8U          zdl[4];
      INT8U          jdl[4];
      INT8U          fdl[4];
      INT8U          pdl[4];
      INT8U          gdl[4];
      INT8U          syje[4];
      INT8U          event_flag[2];
      INT8U          ua[2];
      INT8U          ub[2];
      INT8U          uc[2];
      INT8U          ia[3];
      INT8U          ib[3];
      INT8U          ic[3];
      INT8U           q[2];
      INT8U           f[2];
      INT8U          wg1[20];
      INT8U          wg2[20];
   }b;
}C1_CURDATA;

//��ɫ���޹�����
typedef union{
   INT8U  value[45];
   struct{
      INT8U          rec_date[5];
      INT8U          wg1[20];
      INT8U          wg2[20];
   }b;
}C2_WGDATA;

//����Сʱ��������
typedef union{
    INT8U  value[129];
    struct{
        INT8U          rec_date[5];
        INT8U          load_record1[15];
        INT8U          load_record2[15];
        INT8U          load_record3[15];
        INT8U          load_record4[15];
        INT8U load_record5[16];
        INT8U load_record6[16];
        INT8U load_record7[16];
        INT8U load_record8[16];
    };
}HOUR_LOAD_RECORD;

typedef union{
    INT8U  value[258];
    struct{
        INT8U   td[3];
        INT8U   meter_count[2];
        INT8U   vip_count;
        INT8U   fail_count[2];
        INT8U   rec_status[250];
   };
}C2_F55;

typedef union{
    INT8U  value[28];
    struct{
        INT8U   rec_date[5];
        INT8U   v_a[2];
        INT8U   v_b[2];
        INT8U   v_c[2];
        INT8U   i_a[3];
        INT8U   i_b[3];
        INT8U   i_c[3];
        INT8U   zxyg[4];
        INT8U   fxyg[4];
   };
}C2_LOAD_RECORD;

typedef union{
    INT8U  value[23];
    struct{
        INT8U   td[3];
        INT8U   meter_count[10][2];
   };
}C2_F56;

typedef union{
    INT8U value[15];
    struct{
      INT8U td[3];
      INT8U  p_max[2];
      INT8U  p_max_time[3];
      INT8U  p_min[2];
      INT8U  p_min_time[3];
      INT8U  p_zero_time[2];
    };
}C2_F57;

typedef union{
    INT8U value[14];
    struct{
      INT8U td[2];
      INT8U  p_max[2];
      INT8U  p_max_time[3];
      INT8U  p_min[2];
      INT8U  p_min_time[3];
      INT8U  p_zero_time[2];
    };
}C2_F60;


typedef union{
    INT8U  value[9];
    struct{
        INT8U   td[3];
        INT8U   min_csq;
        INT8U   min_time[2];
        INT8U   max_csq;
        INT8U   max_time[2];
   };
}C2_F247;

typedef union{
    INT8U  value[6];
    struct{
        INT8U port;
        INT8U flag;
        INT8U bcd_start_time[3];
        INT8U exec_hour;
    };
}SET_F111; //__PLC_NET_JIANGSU__

//��������־��
typedef union{
    INT8U value;
    struct{
       INT8U  sale_amount:1;   //�۵��������־  1���Ѽ���
       INT8U  sum_amount:1;    //�ܵ�����ȡ��־��1���Ѷ�ȡ
       INT8U  time_seg:1;      //�Ƿ���ʱ����    1�� ʱ�������   0��ʱ���ڼ���
    };
}tagXLOSTCALFLAG;

//�鳭��ʶ
typedef union{
    INT8U  value[7];
    struct{
        INT8U   fl_flag:5;          //��λ��¼֧�ֵķ�����,Ŀǰ���֧��4���� 1Ϊ��֧�� 0Ϊ֧��
        INT8U   block_flag:3;  //��¼�Ƿ�֧�ֿ鳭  111Ϊδ̽��  001Ϊ�鳭 000Ϊ����
        INT8U fl_time[6];    //̽����� 8��
    };
}METER_BLOCK_FLAG;

typedef union{
    INT8U value[3];
    struct{
    INT8U   flag;
    INT8U   seq;
    INT8U   port;
    };
}G14_ACK;

typedef union{
    INT8U  value[9];
    struct{
        INT8U create_date[3];    //�·������ʱ��
        INT8U task_format[2];    //�����ʽ
        INT8U task_type;         //��������
        INT8U task_data_len[2];
        INT8U exec_type;         //1:���ֵ�� 2��ȫ�����
    };
}BATCH_TASK_HEADER_XIAO;

typedef union{
    INT8U  value[7];
    struct{
        INT8U create_date[3];    //�·������ʱ��
        INT8U task_format[2];    //�����ʽ
        INT8U task_type;         //��������
        INT8U task_data_len[2];
        INT8U exec_type;         //1:���ֵ�� 2��ȫ�����
        INT8U common_er[32];     //ͨ�����ģ�
        INT8U task_data[256];    //�����������Ϊ256
        INT8U meter_count[2];
    };
}BATCH_TASK_HEADER;

typedef union{
    INT8U  value[7];
    struct{
        INT8U task_state;        //0xAA:������δִ�У�A0���ɹ���0A:ʧ�ܣ� ���� ����Ч
        INT8U task_ctrl[4];      
        INT8U meter_no[8];
        INT8U meter_er[32];      //����
        INT8U meter_rondon[4];   //��������
        INT8U meter_esam_id[8];  //���esam���
    };
}BATCH_TASK_AUTH;    //�����֤

typedef union{
    INT8U  value[7];
    struct{
        INT8U task_state;        //0xAA:������δִ�У�A0���ɹ���0A:ʧ�ܣ� ���� ����Ч
        INT8U task_ctrl[4];
        INT8U meter_no[8];
        INT8U meter_er[32];      //����
    };
}BATCH_TASK_TIME;   //��ʱ����

typedef union{
    INT16U  value;
    struct{
        INT16U  meter_idx:11;
        INT16U  tmp:4;
        INT16U  rec_task_flag:1;
    };
}METER_IDX;

typedef union{
    INT8U  value[6];
    struct{
        METER_IDX meter_idx;
        INT8U time[2];
        INT8U voltage[2];
    };
}VOLTAGE_MONITOR;   //��ѹ����

typedef union{
    INT8U value;
    struct{
        INT8U type:3;
        INT8U is_huali:2;
        INT8U tmp:2;
        INT8U is_type:1;
    };
}SWITCH_FLAG;

typedef struct{
    INT32U count[2][15];
    INT8U  ptr;
}PULSECOUNTER;

typedef struct{
   INT32U timer;
   INT8U  flag;
   INT8U  meter_no[6];
}COMMPORT485_CAS_TO_CY;

typedef  union{
     INT8U  value[400];
     struct{   
        INT8U save_F1_4[63];
        INT8U save_F16[64];
        INT8U save_F8[8];
        INT8U save_F7[64];
     };
}INIT_DATA;


typedef struct
{
    INT8U *task_info;
    INT16U task_info_len;
    INT8U *resp;
    INT16U resp_len;
    INT8U port; //����˿�
    INT8U taskid;//����ִ������id
    INT8U max_wait_time;//���ȴ�ʱ��
    PORT_COMM_CTRL port_ctrl;
    BOOLEAN is_delay;
}RECTASK_URGENT_EXEC_OBJECT;


typedef struct
{
    INT8U cur_date[3];          //ͳ��ʱ��
    INT8U day_power_time[4];    //���չ���ʱ��
    INT8U month_power_time[4];  //���¹���ʱ��
}tagAPP_STAT_RUNTIME;
typedef struct
{
    INT8U cur_date[3];          //ͳ��ʱ��
    INT8U day_bytes[4];         //��ͨ������
    INT8U month_bytes[4];       //��ͨ������
}tagAPP_GPRS_BYTES;
typedef struct{
    INT8U  pulse_count[5][4];
    INT8U  meter_value[5][4];
    INT8U  max_xl[5][3];
    INT8U  max_xl_time[5][4];
}tagPulseMeterSaveData;
typedef struct
{
    INT8U               cur_time[6];//��ǰϵͳʱ��
    tagAPP_GPRS_BYTES   gprs_bytes;  //GPRS����ͳ��
    tagAPP_STAT_RUNTIME runtime_stat;//����ʱ��ͳ��
    tagPulseMeterSaveData pulse_meter_save_data[MAX_PULSE_PORT_COUNT];//�������Ҫ���������
}tagAPP_RUN_DATA;

typedef struct
{
    INT8U   max_month_gprs_bytes[4];//��������ֵ
}tagAPP_RUN_PARAM;

typedef struct
{
    INT32U phy;
    INT32U item;
    INT16U left_len;
    INT16U meter_seq;
    INT16U flag;//����
    INT8U  len;//test
    INT8U  block_prop;//D0~D1 ��ʾ�鳭�������� 00�����ȿ鳭���ɳ��Ե���  01���̶��鳭 02 �̶�����   Ĭ��00
    INT8U  *resp;

}READ_PHY_C1;



//�ܼ������
typedef struct{
    INT32S agp_p_yg[MAX_ADDGROUP_COUNT][60];
//    INT32S agp_p_wg[MAX_ADDGROUP_COUNT][60];//�޹�����Ŀǰ�ò�����ֻҪ��һ����ǰֵ�Ϳ����ˣ�������ĵ���ֵ
    INT32S agp_p_wg[MAX_ADDGROUP_COUNT];
    INT8U  cur_ptr;
}tagAGP_P;

#define BATCH_TRANS_METER_TASK_FLAG_NO_EXEC             0     //������,δִ��
#define BATCH_TRANS_METER_TASK_FLAG_CANT_EXEC           1     //����ִ��
#define BATCH_TRANS_METER_TASK_FLAG_EXEC_NO_RESP        2     //��ִ�У�δ��Ӧ
#define BATCH_TRANS_METER_TASK_FLAG_FINISH              3     //ִ�����
#define BATCH_TRANS_METER_TASK_FLAG_BEFORE_EXCEPTION    4     //ǰ������ظ��쳣Ӧ��
#define BATCH_TRANS_METER_TASK_FLAG_RESP_LONG           5     //ִ�����,�ظ�����̫��
#define BATCH_TRANS_METER_TASK_FLAG_TIME_OUT            6     //ִ�����,��ʱ

typedef union{
    INT8U value;
    struct{
        INT8U report:1;
        INT8U wait_read:1;
        INT8U task_end:1;
        INT8U deny:1;
        INT8U node_prior:1;  //__BATCH_TRANSPARENT_METER_TASK_COST_CONTROL__
        INT8U tmp:3;
    };
}BATCH_TRANSPARENT_METER_TASK_FALG;

typedef union{
    INT8U value[4];
    struct{
        INT8U task_id[2];
        INT8U task_level;   //0��������1�������ն��᳭����2�������ն��᳭��
        BATCH_TRANSPARENT_METER_TASK_FALG ctrl_flag;  //D0-�ϱ���־λ  D1-�����ȴ���־λ  D2~D7����
        #ifdef __BATCH_TRANSPARENT_METER_TASK_TMP1__
        INT8U vaild_minute[2];  // 0-��������Чʱ��  ����������ִ����Чʱ������λΪ����
        #else
        INT8U vaild_minute;  // 0-��������Чʱ��  ����������ִ����Чʱ������λΪ����
        #endif
        INT8U z_count;
        INT8U count;
        INT8U begin_time[6];
        INT8U end_time[6];
        INT8U flag;         //0xAA ������
    };
}BATCH_TRANSPARENT_METER_TASK_HEADER;       //__BATCH_TRANSPARENT_METER_TASK__

#define BATCH_TRANSPARENT_METER_TASK_FRAME_LEN         160        //ÿ�����ĵĳ���
#define BATCH_TRANSPARENT_METER_TASK_FRAME_MAX_COUNT   15         //ÿ�������㻺�汨�ĵ��������

typedef union{
    INT8U value[4];
    struct{
        INT8U flag;  //FE:������δִ��  FC������ִ��  F8��ִ�����
        INT8U frame_seq;
        INT8U req_frame_len;
        INT8U req_frame[BATCH_TRANSPARENT_METER_TASK_FRAME_LEN];
        INT8U resp_frame_len;
        INT8U resp_date[6];
        INT8U resp_frame[BATCH_TRANSPARENT_METER_TASK_FRAME_LEN];
    };
}BATCH_TRANSPARENT_METER_TASK;       //__BATCH_TRANSPARENT_METER_TASK__


typedef union{
    INT8U value[10];
    struct{
        INT8U flag;
        INT8U cycle;
        INT8U meter_date[4];
        INT8U meter_error[4];
    };
}PRECISE_TIME_METER_CTRL; //__PRECISE_TIME__

#define MAX_BATCH_METER_CYCLE_TASK_BYTE_NUM                        1
#define BATCH_TRANSPARENT_METER_CYCLE_TASK_FRAME_LEN             200          //ÿ�����ĵĳ���
#define BATCH_TRANSPARENT_METER_CYCLE_TASK_FRAME_MAX_COUNT         8          //ÿ�������㻺�汨�ĵ��������
#define BATCH_TRANSPARENT_METER_CYCLE_TASK_RESP_FRAME_MAX_COUNT   10          //ÿ�������㻺��Ӧ���ĵ��������
typedef union{
    INT8U value[4];
    struct{
        INT8U task_id;  //FE:������δִ��  FC������ִ��  F8��ִ�����
        INT8U task_flag;
        INT8U begin_time[5];
        INT8U unit;
        INT8U frog;
        INT8U end_time[5];
        INT8U protocol;
        INT8U resp_byte_num;
        INT8U req_frame_len;
    };
}BATCH_TRANSPARENT_METER_CYCLE_TASK_HEADER;        //__BATCH_TRANSPARENT_METER_CYCLE_TASK__

typedef union{
    INT8U value[4];
    struct{
        BATCH_TRANSPARENT_METER_CYCLE_TASK_HEADER header;
        INT8U req_frame[BATCH_TRANSPARENT_METER_CYCLE_TASK_FRAME_LEN];
    };
}BATCH_TRANSPARENT_METER_CYCLE_TASK_CMD;        //__BATCH_TRANSPARENT_METER_CYCLE_TASK__

typedef union{
    INT8U value[4];
    struct{
        INT8U cycle_date[5];
        INT8U resp_frame_len;
        INT8U resp_date[5];
        INT8U resp_frame[BATCH_TRANSPARENT_METER_CYCLE_TASK_FRAME_LEN];
    };
}BATCH_TRANSPARENT_METER_CYCLE_TASK_REPS;       //__BATCH_TRANSPARENT_METER_CYCLE_TASK__
*/


//�й�����ʾֵ������λ��С��λ������
// 	D0~D1�������ʾͨ�Ž���ĵ��ܱ���й�����ʾֵ��С��λ��������ֵ��Χ0~3���α�ʾ1~4λС����
// 	D2~D3�������ʾͨ�Ž���ĵ��ܱ���й�����ʾֵ������λ��������ֵ��Χ0~3���α�ʾ4~7λ������


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


//typedef union
//{
//    INT8U value;
//    struct{
//        INT8U set_address:1;  //����������ַ
//        INT8U update_version:1;  //��Ҫ��ѯDJBоƬ�汾
//        INT8U update_ad_data:1;  //��Ҫ��ȡAD��������
//        INT8U reset_flag:1;      //��Ҫ��λdjbоƬ
//        INT8U bat_ad_flag:1;     //���õ�ز�����־
//        INT8U reserve:3;
//    };
//}tagDJB_TASK_FLAG;
//typedef union
//{
//    INT8U value;
//    struct{
//        INT8U has_battery:1;   //�Ƿ��б��õ�ر�־
//        INT8U battery_full:1;  //����Ƿ����
//        INT8U battery_supply:1;//�Ƿ��Ǳ��õ�ع���״̬
//        INT8U reserve:5;
//    };
//}tagBAT_STATE;

typedef struct{
    INT32U item;                //������
    INT16U baud;                //������
    INT8U  cs_bit;              //У��λ
    INT8U  data_bit;            //����λ
    INT8U  stop_bit;            //ֹͣλ
    INT8U  recv_frame_time_out; //���յȴ����ĳ�ʱʱ��
    INT8U  recv_byte_time_out;  //���յȴ��ֽڳ�ʱʱ��
}TRANS_READ_PARAM;



extern METER_DOCUMENT  gmeter_doc;
extern INT32S   read_c1_time_out;//���ݳ�ʱ����
extern tagActiveSendAck gActiveSendAck;//�����ϱ����ݵ�ȷ�ϻ���
extern INT8U meter_status[14];//�������״̬��
#endif

