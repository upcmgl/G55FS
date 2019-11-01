#ifndef __DEV_VAR_H__
#define __DEV_VAR_H__

#include "os.h"
//#include "main_include.h"

#define PROTOCOL_CHANNEL_REMOTE   1
#define PROTOCOL_CHANNEL_METER    2
#define PROTOCOL_CHANNEL_DEBUG    3
#define PROTOCOL_CHANNEL_REMOTE_SERVER    4
#define PROTOCOL_CHANNEL_INFRA    5
#define PROTOCOL_CHANNEL_REMOTE2   6


#define REQUEST_STATE_IDLE          0
#define REQUEST_STATE_RECVING       1
#define REQUEST_STATE_WAIT_PROCESS  2
#define REQUEST_STATE_PROCESSING    3
#define REQUEST_STATE_PROCESSED     4

#define SIZE_OF_APP_PROTOCOL_FRAME  2100


#define SIZE_OF_APP_PROTOCOL_FRAME_COAP 300

typedef union{
    INT16U value;
    struct{
        INT16U GDW1376:1;
        INT16U GB645:1;
        INT16U ONENET:1;
        INT16U reserve:13;
    };
}tagProtocolFlag;

typedef struct{
    INT16U  pn;
    INT16U  fn;
    INT16U  pos_pnfn;
    INT16U  pos_fn_td;
    INT16U  left_resp_len;  //ʣ��Ŀ���Ӧ����ռ�
    INT8U   check_flag;
    INT8U   Td[7];
    INT8U   phy_count;
    INT16U  work_flag;    //��ʶ�����������͡��յ����� ��01 �µ����� 02 ���ߵ����� 03 ���ն������� 04 �¶��� 05 ���� 0x0A�� ������ 06  Сʱ���� 07 �ܼ��� ���ա��¡����ߣ��ܼ��� 08  Ԥ��Сʱ����09 iaDayhold ���� �� �� ����,0A Ԥ������
    BOOLEAN need_more_frame;
    BOOLEAN is_first_frame;
}READ_CONTEXT;

typedef union{
    INT8U value[2];
    struct{
        INT8U last_count;
        INT8U start_td[7];
        INT8U end_td[7];
        INT8U choice;        
    }a;
}SEPARATE_FRAME_PARAM; 
   
typedef union{
    INT8U value[2];
    struct{
        INT16U index_id[3];
        INT16U idx_start[4];
        INT16U frame_id;
        INT16U pos;
        INT16U userdata_pos;
        INT16U frame_len;
        INT8U more_frame;
        INT8U is_frist;
        INT8U apdu_service_code;
        INT8U batch_count;
        INT8U idx_count;
        INT8U prepare_param;
        SEPARATE_FRAME_PARAM param;
        INT8U frame[SIZE_OF_APP_PROTOCOL_FRAME];
    };
}SEPARATE_FRAME;

typedef struct {
    uint8_t  ref;
    uint32_t mid;
    uint8_t  result;
    uint16_t objid;
    int16_t  insid;
    int32_t  resid;
    uint8_t  type;
    uint16_t len;
    uint8_t  index;
    uint8_t  flag;
	uint8_t  msg_type;// 1 read 2 write 3 execute 4 notify
    uint8_t  state;
    uint32_t life_time;
    uint16_t ackid;
    uint8_t  event_id;
}tagOneNet;

typedef struct {
    INT32U           send_time_in_10ms;
    INT32U           recv_time_in_10ms;
    INT16U           frame_len;
    INT16U           recv_len;
    INT16U           cs;
    INT16U           pos;
    INT16U           userdata_pos;
    INT16U           addition_pos;
    INT16U           userDataLen;
    tagProtocolFlag  surported_protocol;
    tagProtocolFlag  cur_protocol;
    uint8_t          recv_success_flag;
    INT8U            channel;
    INT8U            state;
	INT8U           *param;//TP
    INT8U            rseq;//SEQ
    INT8U           *context;//��������
    INT8U           *fn;//Fn�����ṹ��
    INT8U            item_count;
    BOOLEAN          is_security;
    INT8U            frame[SIZE_OF_APP_PROTOCOL_FRAME];
    INT8U            security_info[30];//[0]-�������0-���� 1-���ģ�[1]-���ܽ����0-�ɹ�������-ʧ��  [2]-������֤��Ϣ��0-SID_MAC��1-RN��2-RN_MAC��3-SID
}objRequest;
typedef struct{
    INT16U    pos;
    INT16U    userdata_pos;
    INT16U    max_reply_pos;
    INT16U    frame_len;
    INT8U     channel;
    INT8U     work_flag[10];    //Э�鴦���б���һЩ����״̬
    INT8U     frame[4+SIZE_OF_APP_PROTOCOL_FRAME];
}objResponse;


typedef struct _Date{
     INT16S year;
     INT16S  month;
     INT16S  day;
     INT16S  hour;
     INT16S  minute; //���ӻ������ʱ��Ϊ�˴�����
}CommandDate;

typedef struct {
    uint8_t app_is_ready;
    uint8_t meter_no[6];
	uint8_t managementNum[23];  //�ʲ�������22λ����ʵ����д洢����32λ��ֻ����һЩλΪ0x00
    uint8_t ertu_devid[10];
    uint8_t devid_len;
    uint8_t apn[32];
    uint8_t user[32];
    uint8_t pwd[32];
    uint8_t ip1[4];
    uint8_t port1[2];
    uint8_t ip2[4];
    uint8_t port2[2];
    uint8_t listen_port[2];
    uint8_t online_mode;
    uint8_t ppp_is_ok;//
    uint8_t tcp_link;
    uint8_t tcp_link2;
    uint8_t tcp_valid;
    uint8_t tcp_valid2;
    uint8_t relink_cnt;
    INT32U relink_timer;

    uint8_t remote_client_linked;
    uint8_t login_status;
    uint8_t login_status2;
    #ifdef __OOP_G_METER__
    uint8_t heart_cycle[2];
    #else
    uint16_t heart_cycle;
    #endif
    uint8_t gprs_debug_flag;        //gprs���ĵ���
    uint8_t meter_debug_flag;       //���ͨ�ű��ĵ��Կ���
    uint8_t remote_app_debug_flag;  //gprs��Ӧ�ò㱨�ĵ��Կ���
    uint8_t debug_info_interface;   //���Խӿ�ѡ��0Ϊdebug�������������ֵΪ���ͨ�ſ����
    uint8_t clock_ready;            //ʱ��״̬��0���ն�δȡ��ʱ�ӣ�����������վ��
    //gprs�����Ϣ
    uint8_t mygmr_info[26];
    uint8_t myccid_info[20];
    uint8_t imei_info[15];
    uint8_t imsi_info[15];
    uint8_t csq;
    uint8_t ppp_ip[4];
    uint8_t nouse_1376_3;
    uint8_t remote_model_type;
    uint8_t remote_model_vendor;
    uint8_t is_nb_net;
    uint8_t is_coap_net;
    uint8_t rsrp[4];
    uint8_t sinr[4];
    uint8_t cellid[8];
    uint8_t node_b[4];
    uint32_t cell_id;//32U 
    
    uint16_t lvd;
    uint16_t bat_mon;
    uint16_t ad_temp;
    int8_t   dev_temp;
    uint8_t battery_charge;
    uint32_t batter_power_time;
    uint8_t ertu_param_change;  //��λ��ʾ��0�������δ�ı䣬1��������ı䡣bit0:DT_F1....bit1:DT_F3....bit_2:DT_F16....bit_3:DT_F89
    uint8_t has_data_report;  //�����ݻ��¼��ϱ� ���ģʽ����Ҫ����TCP  ��λ��ʶ���¼��������ϱ�
    uint32_t life_time;
    uint8_t cgatt;//�Ƿ������� 1 �Ѿ�����
    uint32_t last_resource_ready;//1: ��Դ������� �����ϱ�����
    uint32_t month_bytes;//����ͳ��
    uint16_t day_bytes; //����ͳ��������
    uint16_t ackid;
    uint8_t local_status;//1 ����ͨѶ�ɹ� 0 ʧ��
    uint8_t remote_status;//1 Զ��ͨѶ�����ݽ��շ��� 0 ������
    uint8_t is_debug;//1 ����״̬ ���� �ǵ���״̬
    uint8_t update;//����       ���edpЭ�������Ļ���������flag 0xAA ��ʼ 0xBB �������� 0xCC�������  DDʧ��
    uint32_t update_begin_tick;//������ʼ
    uint32_t no_net_tick;//ע�᲻��������ʼʱ��tick
    uint32_t check_update_tick;//�������״̬��ʼʱ��    
    uint8_t ESAM_DEVID[8];
    uint8_t ESAM_RAND[8];
    uint8_t ESAM_CERT_STATE;
    uint8_t esam_type;
	
	uint8_t usb_read;
	uint8_t usb_write;
	uint8_t *filename;
	uint16_t offset;
	uint8_t *data;
	uint16_t len;
	uint8_t reconnect;
	uint32_t switch_param;//Ĭ�ϲ��������л�����
	INT8U  noResponseCnt;   //���δ��Ӧ������Ϊ�˱���һ�ε����ӦȻ�󱾵ص�Ϩ������⡣
#ifdef __OOP_G_METER__
	uint8_t dayHodeMode; //�ն���ģʽ��
	uint8_t plan_task_chg;/* ��������ı��־ */
	DateTime dt; /* ����������ʱ �洢ʱ�� �����ڸ���ʹ�� */	

#endif
#ifdef __EDP__
	INT8U save_ack;
	INT8U dev_config_ack;   //ģ���ϱ������ñ��ĺ󣬲ſ��Խ���������ʱ���Ѿ����ϵ�15�����
	INT8U edp_login_state;
	INT8U edp_ping_state;
	INT8U edp_report_interval;
	INT8U edp_enable_report_store;  //ʹ���ϱ���Щ�洢��δ�ϱ��ɹ������ݡ�
	INT32U edp_over_flow_A;        //A �����
	INT32U edp_over_flow_B;        //B �����
	INT32U edp_over_flow_C;        //C �����
	INT32U edp_open_meter;			//����Ǵ���
	INT32U edp_clean_meter;         //�����0����
	INT8U  edp_warning_ack;
	INT8U  edp_rand_report_begin;   // ��������Ӻ�ʱ�䷶Χ,��ʼʱ��
	INT8U  edp_rand_report_end;   // ��������Ӻ�ʱ�䷶Χ,����ʱ��
#endif
}tagSystemInfo;


typedef struct
{
    INT8U cur_date[3];          //ͳ��ʱ��    ����˳��-������
    INT8U day_bytes[4];         //��ͨ������
    INT8U month_bytes[4];       //��ͨ������
}tagAPP_GPRS_BYTES;
typedef struct
{
    INT8U is_control_valid;  //���Ʋ����Ƿ���Ч
    INT8U turn_off[2];          //��բʱ��
    INT8U turn_on[2];           //��բʱ��
}tagAPP_CONTROL;

typedef struct
{
    INT8U               cur_time[6];//��ǰϵͳʱ��
    tagAPP_GPRS_BYTES   gprs_bytes;  //GPRS����ͳ��
    tagAPP_CONTROL      meter_control;    //����բԤ�ò���
    INT8U               password[4];//����Ȩ��
    uint32_t status;//��λ��ʶģ��״̬��1��Ч D0:ģ�鱾��ͨ�ųɹ� D1:��ע�� D2���ѵ�½
}tagAPP_RUN_DATA;
   
typedef union{
   INT8U  value[4];
   struct{
    INT8U     times[3];//����
    INT8U     state;//���� :0xAA �ָ���0x55 
   };    
}tagEVENT;

//typedef struct{
     //INT8U   acd;
     //INT8U   ec1;         //��Ҫ�¼�������
     //INT8U   ec2;         //һ���¼�������
////     INT8U   alarm_event;//��ʾ�澯ʹ��
////     INT8U   ercflag[8]; //F8�¼������־λBS64
////     INT8U   event_ec[4];//32λ���¼�������
////     INT8U   report_flag[256];
////     INT8U   report_idx;
//}tagGlobalEVENT;

#define UPDATE_PACKET_BUFFER_SIZE  512
//������������ṹ
#ifndef __SOFT_SIMULATOR__
typedef struct
{
    FSFILE* usbFile;
	INT8U update_group_flag;//�Ƿ�������
    INT8U is_transmit_reverse;//��ת����
    
    INT16U frame_count;//֡����
    INT8U frame_crc[32];
    INT16U total_sec_count;//�ܶ���
    INT16U sec_size;//�δ�С
    INT8U  sec_status[512];//һ֡1024����֧��512*8*1024=4M, һ֡512������֧��2M
    INT8U buffer[UPDATE_PACKET_BUFFER_SIZE];
}tagUPDATE_OBJECT;
#endif
typedef struct{
    INT32U filesize;
    INT32U attachsize;
    INT16U filetype;
    INT16U filever;
    INT8U  md5[16];
    INT8U  updatetspec[4];
    INT8S   updatetver[4];
    INT8S   areacode[4];
    INT8U  updatetdate[3];
    INT8U  encryptno;
    INT8U  updatettype;
    INT8U  temp[3];
    INT8U  attachfile[20];
    INT8U  device_code_flag[64];
}DATHEADER;

typedef union
{
    INT8U value;
    struct{
        INT8U ok_flag:1;     //�����쳣��־
        INT8U valid_flag:1;  //��Ч��Ч��־
        INT8U reserve:6;    //����
    };
}tagPOWER_ONOFF_PROPFLAG;

typedef union{
   INT8U  value[14];
   struct{
      INT8U power_flag;
      tagPOWER_ONOFF_PROPFLAG   prop_flag;  //����ʱ��  ��-ʱ-��-��-��
      INT8U     power_off_time[6];
      INT8U     power_on_time[6];
   };
}tagPOWER_ONOFF_RECORD;

#ifdef __ONENET__
typedef struct{
    uint8_t *send_ptr;
    uint16_t send_len;
    uint16_t recv_pos;
    uint16_t read_pos;
    uint8_t  recv_buf[SIZE_OF_APP_PROTOCOL_FRAME];
    uint8_t  send_buf[SIZE_OF_APP_PROTOCOL_FRAME];
    uint8_t  ref;
    uint32_t mid;
    uint8_t  result;
    uint16_t objid;
    int16_t  insid;
    int32_t  resid;
    uint8_t  type;
    uint16_t len;
    uint8_t  index;
    uint8_t  flag;
	uint8_t  msg_type;// 1 read 2 write 3 execute 4 notify
    uint8_t  state;
    uint32_t life_time;
    uint16_t ackid;
}tagWirelessObj;

typedef union{
    INT8U  value[36];
    struct{
       tagDatetime      monthhold_report_time;     //��һ���¶����ϱ�ʱ��
       tagDatetime      dayhold_report_time;     //��һ���ն����ϱ�ʱ��
       tagDatetime      cycle_report_time;     //��һ�ε�ǰ�����ϱ�ʱ��
       INT16U           rpt_flag;//��Ҫ�ϱ������ݱ�־
       INT16U           rpt_idx;//resource id���е�idx 
       INT16U           ack_idx;//ƽ̨�ظ�idx
    };
}TASK_INFO;
#else
typedef struct{
    uint8_t *send_ptr;
    uint16_t send_len;
    uint16_t recv_pos;
    uint16_t read_pos;
    uint8_t  recv_buf[SIZE_OF_APP_PROTOCOL_FRAME];
}tagWirelessObj;
#endif

extern objMutex SIGNAL_FLASH;
extern objMutex SIGNAL_METER_UART;
extern objMutex SIGNAL_DEBUG_UART;
extern objMutex SIGNAL_INFRA_UART;
extern objMutex SIGNAL_FILE_OP;
extern objMutex SIGNAL_REMOTE_UART;
extern objMutex SIGNAL_REMOTE_UART_SEND;
extern objMutex    SIGNAL_SYSTEMINFO;
extern objMutex    SIGNAL_METER;
extern objMutex SIGNAL_EASM;
extern objMutex   SIGNAL_TEMP_BUFFER;
extern objMutex   SIGNAL_EVENT;
extern objMutex SIGNAL_RUN_DATA;


extern objRequest  RequestRemote,RequestRemote2;
extern objRequest  RequestRemoteServer;
extern objRequest  RequestUart;
extern objRequest  RequestDebug;

extern objResponse ResponseApp;
extern objRequest  RequestInfra;

extern SEPARATE_FRAME separate_frame_mem;

extern tagSystemInfo gSystemInfo;
extern tagAPP_RUN_DATA g_app_run_data;
//
//extern tagGlobalEVENT g_event;
#ifndef __SOFT_SIMULATOR__
extern tagUPDATE_OBJECT update_object;    //������������
extern DATHEADER update_header;                  //�����ļ�ͷ����
#endif
extern uint8_t g_meter_set_flag;
#ifdef __ONENET__
extern tagWirelessObj RequestOnenet,ReportOnenet;
extern TASK_INFO task;
#endif
#endif