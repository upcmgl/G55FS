#include "app_var_3761.h"
#include "app_3761.h"


//////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////
#define SOFT_VER_YEAR  0x19
#define SOFT_VER_MONTH 0x06
#define SOFT_VER_DAY   0x18


#define SOFT_VER_MAIN   '1'
#define SOFT_VER_DOT    '.'
#define SOFT_VER_MIN1   '0'
#define SOFT_VER_MIN2   '2'

#define HARDWARE_VER_H   '1'
#define HARDWARE_VER_DOT '.'
#define HARDWARE_VER_L1  '0'
#define HARDWARE_VER_L2  '1'


#define MAX_TASK_C1       4

#define MAX_TASK_C2       4





//*******************************************************
//
//       �ն˲����滮
//       ��Ҫ��AFN=04H����
//       �����洢�Ĺ滮Ϊ������+Ԥ��2�ֽ�

//       ���ݣ����ݲ��������ݶ��峤��
//       Ԥ�������ݲ��������������Ǻ������ܵ���չ���ݶ��׶���С
//
//       ע��˵�����ֽڳ���=����+Ԥ��
//
//
//*******************************************************

//�ն˵Ļ���������Ϣ��ռ��4000�ֽ�  ���ɳ�ʼ��
#define  EEADDR_BASIC_PARAM_START        0
#define  EEADDR_NONE             0xFFFFFF

#define  EEADDR_CFG_DEVID        (EEADDR_BASIC_PARAM_START)     //8=6+2���ն˵�ַ(���6�ֽ�)(Ԥ��2�ֽ�--����)
  #ifdef __RTUADDR_IS_6BYTE__
    #define  EEADDR_CFG_AREACODE        (EEADDR_CFG_DEVID + 1)  //3��������������
    #define  EEADDR_CFG_RTUADDR         (EEADDR_CFG_DEVID + 4)  //3���ն˵�ַ
  #else
    #define  EEADDR_CFG_AREACODE        (EEADDR_CFG_DEVID + 1)  //2��������������
    #define  EEADDR_CFG_RTUADDR         (EEADDR_CFG_DEVID + 3)  //2���ն˵�ַ
  #endif
#define  EEADDR_CFG_VENDOR          (EEADDR_CFG_DEVID + 8)         //6=4+2�����̴���
#define  EEADDR_SOFT_VER	       (EEADDR_CFG_VENDOR+6)	//6=4+2 ����汾��
#define  EEADDR_SOFT_DATE	       (EEADDR_SOFT_VER+6)		//5=3+2 ����汾��������
#define  EEADDR_CAPACITY	       (EEADDR_SOFT_DATE+5)		//13=11+2 ������Ϣ��
#define  EEADDR_PROTOCOL_VER	       (EEADDR_CAPACITY+13)		//6=4+2 Э��汾��
#define  EEADDR_HARDWARE_VER	       (EEADDR_PROTOCOL_VER+6)		//6=4+2 Ӳ���汾��
#define  EEADDR_HARDWARE_DATE	       (EEADDR_HARDWARE_VER+6)		//5=3+2 Ӳ���汾��������
#define  GPRS_INFO    ( EEADDR_HARDWARE_DATE+ 5)      //66=64+2,GPRS��Ϣ
    #define   GPRS_INFO_VENDOR   (GPRS_INFO +  5)   //20,VENDOR
    #define   GPRS_INFO_VERINFO  (GPRS_INFO + 25)   //20,VER_INFO
    #define   GPRS_INFO_SIM_ID   (GPRS_INFO + 45)   //20,SIM_ID
#define  EEADDR_SIMCARD_TYPE    (GPRS_INFO + 66)               //3=1+2,SIM����Ӫ������
#define  EEADDR_GPRS_IMSI       (EEADDR_SIMCARD_TYPE + 3)      //17=15+2,SIM��IMSI��
#define  EEADDR_GPRS_VERSION  (EEADDR_GPRS_IMSI + 17)   //31=29+2, Զ��ģ��汾��Ϣ���汾����

#define  EEADDR_SET_F1           (EEADDR_GPRS_VERSION+31)      //8=6+2��F1-�ն�ͨ�Ų���
    #define  EEADDR_HEARTCYCLE        (EEADDR_SET_F1 + 5)       //1����������
#define  EEADDR_SET_F3           (EEADDR_SET_F1 + sizeof(tagSET_F1))           //50=44+6��F3-��վIP��ַ�Ͷ˿�(Ԥ��6�ֽ�--1��IP��ַ+�˿�)
    #define  EEADDR_APN               (EEADDR_SET_F3 + 50)      //34=32+2��APN  1376.1��APN��16�ֽڣ����⴦��
#define  EEADDR_SET_F7           (EEADDR_SET_F3 + sizeof(tagSET_F3))           //70=64+6��F7--�ն�IP��ַ�Ͷ˿�(Ԥ��6�ֽ�--1��IP��ַ+�˿�)
#define  EEADDR_SET_F8           (EEADDR_SET_F7 + sizeof(tagSET_F7))           //10=8+2��F8-�ն�����ͨ�ŷ�ʽ(��̫ר��������ר��)(Ԥ��2�ֽ�--����)
#define  EEADDR_SET_F16          (EEADDR_SET_F8 + sizeof(tagSET_F8))          //66=64+2��F16--����ר���û���������(Ԥ��2�ֽ�--����)
#define  EEADDR_SET_F221         (EEADDR_SET_F16 + 66)         //3=1+2��F221-���Ź��ܿ��������(Ԥ��2�ֽ�--����)
#define  EEADDR_SET_F33          (EEADDR_SET_F221 + 3)         //220=110*2��F33-�ն˳������в���(Ԥ��2�ֽ�--����)��֧��2���˿�
#define  EEADDR_SET_F301         (EEADDR_SET_F33 + 220)         //267=1+264+2��F301-������¼����(Ԥ��2�ֽ�--����)
#define  EEADDR_MES              (EEADDR_SET_F301+ 267)         //34 = 32+2 :MES��
#define  EEADDR_POWEROFF_RECORD  (EEADDR_MES+34)                //14 �ն�ͣ�ϵ��¼
#define  EEADDR_METER_SETFLAG    (EEADDR_POWEROFF_RECORD+14)     //1 �������Ƿ��������ã�0x55:�������ã�����:������
#define  EEADDR_CONFIG_ONENET    (EEADDR_METER_SETFLAG+1)       //1+2 onenet���߲���  00��FF ���� ���� ר��
#define  EEADDR_SET_F701         (EEADDR_CONFIG_ONENET+3)       //4+1 ������� �����ڲ�Ĭ��Ϊ12345602 ʹ��Ĭ������ʱ����Ҫ����
#define  EEADDR_SET_F702         (EEADDR_SET_F701+5)            //5+1 ���Ԥ������բʱ����� ����բ ��Ч��־1�ֽ�+��ʼ����ʱ�� 4�ֽ�
#define  EEADDR_SWITCH_PARAM     (EEADDR_SET_F702+6)            //4+4+1 Ĭ�ϲ��������л� 
                                                                //D0����������645Э�� �����½����֡

#define EEADDR_BASIC_PARAM_CUR_END   (EEADDR_SET_F702 + 6)
#define EEADDR_BASIC_PARAM_END   8000UL

//#if  (EEADDR_BASIC_PARAM_CUR_END > EEADDR_BASIC_PARAM_END)
//    #error "EEADDR_BASIC_PARAM IS OVERFLOW"
//#endif





/************************************
�����ն˲���   ��8000UL��ʼ �ɳ�ʼ��
*************************************/
#define  EEADDR_PARAM_INIT_START  (EEADDR_BASIC_PARAM_END)
//���ǵ������Ĳ����ļ��Ĺ滮��δ��ȫ�����еĺ�����AFN=04H�йصģ�������洢��ַ
#define  EEADDR_SET_F4           (EEADDR_PARAM_INIT_START)           //24=16+8��F4-��վ�绰����Ͷ������ĺ���(Ԥ��8�ֽ�--1���绰����)
#define  EEADDR_SET_F9           (EEADDR_SET_F4 + sizeof(tagSET_F4))           //34=32+2��F9-�ն��¼���¼����(Ԥ��2�ֽ�--8���¼�)
#define  EEADDR_SET_F10          (EEADDR_SET_F9 + sizeof(tagSET_F9))
#define  EEADDR_SET_F36          (EEADDR_SET_F10 + sizeof(METER_DOCUMENT))      //6=4+2��F36-�ն�����ͨ����������(Ԥ��2�ֽ�--����)
#define  EEADDR_FRAME_MONITOR    (EEADDR_SET_F36 + 6)         //2+2�����ļ�ؿ��� D0-gprs���ĵ��� D1-���ͨ�ű��ĵ��Կ��� D2-gprs��Ӧ�ò㱨�ĵ��Կ���
                                                                // D3-���Խӿ�ѡ��0Ϊdebug�������������ֵΪ���ͨ�ſ���� D4 ʱ��״̬��0�ն�δȡ��ʱ�Ӳ���������վ�� D5~D15����
#define     EEADDR_TASK_REPORT_CYCLE    (EEADDR_FRAME_MONITOR+4)     //4+2 4�ֽ�BCD                         
#define     EEADDR_TASK_REPORT      (EEADDR_TASK_REPORT_CYCLE+6)     //36+20 �����ϱ����� ʱ��
    #define TASK_REPORT_MONTHHOLD_TIME     0    
    #define TASK_REPORT_DAYHOLD_TIME       TASK_REPORT_MONTHHOLD_TIME+sizeof(tagDatetime)
    #define TASK_REPORT_CURDATA_TIME       TASK_REPORT_DAYHOLD_TIME+sizeof(tagDatetime)
    #define TASK_REPORT_REPORT_FLAG        TASK_REPORT_CURDATA_TIME+sizeof(tagDatetime)
    #define TASK_REPORT_IDX                TASK_REPORT_REPORT_FLAG+2
    #define TASK_REPORT_ACK_IDX            TASK_REPORT_IDX+2
#define     EEADDR_STAT_FLOW        (EEADDR_TASK_REPORT+56)        //11+2 ��������ͳ��
#define     EEADDR_LAST_STATUS      (EEADDR_STAT_FLOW+13)          //28=20״̬��+6ʱ��+2 ��һ��״̬��   ÿ�α仯�����
#define     EEADDR_LAST_ENENT_TIMES (EEADDR_LAST_STATUS+28)        //80=4*20  11��Ԥ��9 ����instance˳�����ƫ�Ƶ�ַĬ�� ���instance�仯��Ҫע��ƫ�Ƶ�ַ���ݵĳ�ʼ��
#ifdef __EDP__
#define     EEADDR_TIME_INTERVAL   (EEADDR_LAST_ENENT_TIMES+80)  //2 �޸��豸�ɼ����ݵ�ʱ���� Ĭ��3����
#define     EEADDR_UPDATE_FLAG     (EEADDR_TIME_INTERVAL+2)  //1  ��������flag 0xAA ��ʼ 0xBB �������� 0xCC�������
#define     EEADDR_NEW_VERSION  (EEADDR_UPDATE_FLAG+1)  //11  tops4+�°汾
#define     EEADDR_OLD_VERSION     (EEADDR_NEW_VERSION+11)  //11 tops4+�ϰ汾
#define     EEADDR_UPDATE_SUCESS   (EEADDR_OLD_VERSION+11)  //1  �����ɹ���־ 0
#define     EEADDR_IP_PORT_ONENET       (EEADDR_UPDATE_SUCESS+1)  //4+2 onenet IP��ַ�Ͷ˿�
#define     EEADDR_IP_PORT_UPDATE       (EEADDR_IP_PORT_ONENET+6)  //4+2 ģ��������ת IP��ַ�Ͷ˿�
#define     EEADDR_OVERFLOW_WARNING_A     (EEADDR_IP_PORT_UPDATE +6)  //4�ֽڣ������澯�����洢 A�ࡣ
#define     EEADDR_OVERFLOW_WARNING_B     (EEADDR_OVERFLOW_WARNING_A +4)  //4�ֽڣ������澯�����洢 B�ࡣ
#define     EEADDR_OVERFLOW_WARNING_C     (EEADDR_OVERFLOW_WARNING_B +4)  //4�ֽڣ������澯�����洢 C�ࡣ
#define     EEADDR_OPENMETER_WARNING    (EEADDR_OVERFLOW_WARNING_C +4) //4�ֽ� ����Ǹ澯�����洢
#define     EEADDR_CLEANNMETER_WARNING    (EEADDR_OPENMETER_WARNING +4) //4�ֽ� ����澯�����洢
#define     EEADDR_EDP_RAND_BEGIN         (EEADDR_CLEANNMETER_WARNING+4) // 1�ֽڣ�������������Ӻ�ʱ�䷶Χ�Ŀ�ʼʱ�䣬 ���ܵ�����ʱ���Ϊ10��20�е� 10 ;  /*��������Ӻ����ʼʱ��Ϊ10���ӣ�����ʱ��20����*/ 
#define     EEADDR_EDP_RAND_END            (EEADDR_EDP_RAND_BEGIN + 1)   // 1�ֽڣ�������������Ӻ�ʱ�䷶Χ�Ľ���ʱ�䣬 ���ܵ�����ʱ���Ϊ10��20�е�20��
#define     EEADDR_UPDATA_VERSION          (EEADDR_EDP_RAND_END+1)     //4�ֽڣ����ڱ���ƽ̨�·��������汾������1007����汾��1007�汾��
#define     EEADDR_REPORT_UNSUCESS  (EEADDR_UPDATE_SUCESS +1) //sizeof(struct strReportUnsuceed) �洢δ�ɹ��ϱ���������
#endif

//#define   EEADDR_SET_F97          (EEADDR_SET_F36 + 6)           //27=25+2��F97-ͣ�����ݲɼ����ò���
//#define   EEADDR_SET_F98          (EEADDR_SET_F97 + 27)          //15=13+2��F98-ͣ���¼������ֵ����
//#define   EEADDR_SET_F67          (EEADDR_SET_F66 + 65856)       //6=4+2��F67-��ʱ�ϱ�I��������������/ֹͣ����(Ԥ��2�ֽ�--����)
//#define   EEADDR_SET_F68          (EEADDR_SET_F67 + 64)          //6=4+2��F68-��ʱ�ϱ�II��������������/ֹͣ����(Ԥ��2�ֽ�--����)
//#define   EEADDR_C1_F7    (PIM_C1_F5 + 2)         //2,  �ն��¼���������ǰֵ
//#define   EEADDR_C1_F8    (PIM_C1_F7 + 2)         //8,  �ն��¼���־״̬
//#define   EEADDR_C1_F10   (ABANDON_PIM_C1_F9 + 2)         //8,  �ն˵���(��㿪ʼ),����(ÿ��1����㿪ʼ)ͨ������,  ���ӱ���
//#define   EEADDR_DAY_RST_COUNT       (PIM_ERTU_XLOST + 450)               //SAVE_POINT_NUMBER_DAY_HOLD*7, �ն��ո�λ����
//#define   EEADDR_DAY_BYTES           (FLADDR_DAY_RST_COUNT + SAVE_POINT_NUMBER_DAY_HOLD*7)        //SAVE_POINT_NUMBER_DAY_HOLD*7, �ն�������ͳ��
//#define   EEADDR_DAY_POWER_TIME  (FLADDR_DAY_BYTES + SAVE_POINT_NUMBER_DAY_HOLD*7)            //SAVE_POINT_NUMBER_DAY_HOLD*7, �ն��չ������
//#define   EEADDR_MONTH_RST_COUNT   (FLADDR_DAY_POWER_TIME + SAVE_POINT_NUMBER_DAY_HOLD*7)        //SAVE_POINT_NUMBER_MONTH_HOLD*6, �ն��¸�λ����
//#define   EEADDR_MONTH_BYTES   (FLADDR_MONTH_RST_COUNT + SAVE_POINT_NUMBER_MONTH_HOLD*6)        //SAVE_POINT_NUMBER_MONTH_HOLD*6, �ն�������ͳ��
//#define   EEADDR_MONTH_POWER_TIME  (FLADDR_MONTH_BYTES + SAVE_POINT_NUMBER_MONTH_HOLD*6)            //SAVE_POINT_NUMBER_MONTH_HOLD*6, �ն��¹������
//#define   EEADDR_SET_F65          (EEADDR_SET_F98 + 15)        //4116=1029*4��F65-��ʱ�ϱ�I��������������
//#define   EEADDR_SET_F66          (EEADDR_SET_F65 + )   //4116=1029*4��F66-��ʱ�ϱ�II��������������
//#define   MAX_METER_EVENT_RECORD_COUNT         512*50 ����            //����¼���¼    Ԥ�� �ݲ�ȷ��
#define EEADDR_INIT_CUR_END                  (EEADDR_LAST_ENENT_TIMES+80)
#define EEADDR_UNCS_PARAM_END                23999UL
//#if EEADDR_UNCS_PARAM_CUR_END > EEADDR_UNCS_PARAM_END
//    #error "EEADDR_UNCS_PARAM IS OVERFLOW"
//#endif

///////////////////////////////////////////////////////////////////////////////////////////
//
//         ������ݹ滮
//
//         ��ͨ������,��������4���ʵ�����ܱ�
//
//         ���� 93�յ��ն�������    12�µ��¶�������
//
//////////////////////////////////////////////////////////////////////////////////////////

//-----------------------------�����㵵��������-----------------------
//  �����㵵��
#define  PIM_METER_DOC                0        //27,���������Ϣ
#define        PIM_METER_BAUDPORT     4
#define        PIM_METER_PROTOCOL     5
#define        PIM_METER_NO           6
#define        PIM_METER_RTU         20
#define        PIM_METER_USERTYPE    26
#define  PIM_REC_FLAG               (PIM_READ_MASK + sizeof(READ_MASK))        //1,  F30:ͣ��/Ͷ������
#define  PIM_AUX_NODES              (PIM_REC_FLAG + 1)          //13, F31:�ز��ӽڵ㸽���ڵ��ַ,�����20�������ڵ����,20140411��Ϊ���2�������ڵ�


BOOLEAN  GetParamAddrLen(INT16U fn,INT32U *addr,INT16U *len);
INT8U  check_curve_fn_list(INT8U fn);
INT8U  fwrite_ertu_params(INT32U addr,INT8U *data,INT16U len);
INT8U  fread_ertu_params(INT32U addr,INT8U *data,INT16U len);
INT8U  fwrite_meter_params(INT16U meter_idx,INT32U addr,INT8U *data,INT16U len);
INT8U  fread_meter_params(INT16U meter_idx,INT32U addr,INT8U *data,INT16U len);
INT8U  fwrite_meter_data(INT16U meter_idx,INT32U offset,INT8U *data,INT16U len);
INT8U  fread_meter_data(INT16U meter_idx,INT32U offset,INT8U *data,INT16U len);

extern const INT8U ertu_cfg_verinfo[LEN_ERTU_CFG_VERINFO];

extern const INT8U  update_watermark[8];

INT8U check_device_code(DATHEADER *header);
INT16U compute_readcfg_fn_datalen(INT16U fn);
void readcfg_fn_data(INT16U fn,INT8U *resp);
INT8U  set_ertu_params(INT16U fn,INT8U *itemData,INT16U *itemDataLen);
INT16U set_custom_param(INT16U paramcode,INT8U *data);
INT16U set_ertu_custom_param(INT8U *data,INT16U *itemDataLen);
void query_ertu_custom_param(INT8U *buffer,INT16U *itemDataLen,INT8U *query_ctrl);
INT16U read_custom_param(INT16U paramcode,INT8U *data);    
INT16U app_get_F249_data(INT8U* resp);