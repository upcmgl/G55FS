#ifndef __DEV_CONST_H__
#define __DEV_CONST_H__


//#define __HEBEI_HUANBAO__  //�ӱ������� ֧��645���� ����������֡
//#define __MX_UPDATE__
//__CTRL_METER__  Ԥ�ò�������բ
#ifdef __OOP_G_METER__
#define MAX_FRAME_HEADER_SIZE  14
#else
#define MAX_FRAME_HEADER_SIZE  10
#endif

#define MAX_REMOTE_DELAY_10MS  2000
//ģ���ͺŶ���
#define  REMOTE_MODEL_TYPE_UNKNOWN 0
#define  REMOTE_MODEL_TYPE_EC20    1
#define  REMOTE_MODEL_TYPE_EC21    2
#define  REMOTE_MODEL_TYPE_EC25    3
#define  REMOTE_MODEL_TYPE_A9500   4
#define  REMOTE_MODEL_TYPE_A9600   5
#define  REMOTE_MODEL_TYPE_M5310   6


//ģ�鳧�̶���
#define  REMOTE_MODEL_VENDOR_UNKNOWN   0
#define  REMOTE_MODEL_VENDOR_QUECTEL   1
#define  REMOTE_MODEL_VENDOR_LONGSHANG 2
#define  REMOTE_MODEL_VENDOR_CMI       3    //China Mobile Internet


#define ONENET_READ    1
#define ONENET_WRITE   2
#define ONENET_EXECUTE 3
#define ONENET_NOTIFY  4
#define ONENET_EVENT_ID 5

#define STRING_DATA      0x0100
#define OPAQUE_DATA      0x0200
#define INTERGER_DATA    0x0300
#define FLOAT_DATA       0x0400
#define BOOL_DATA        0x0500
#define HEX_STR_DATA     0x0600

#define KWH_FLOAT    1
#define KWH_STRING   2
#define KVarH_STRING 3
#define V_STRING     4
#define V_FLOAT      5
#define A_STRING     6
#define A_FLOAT      7
#define P_FLOAT      8
#define KW_STRING    9
#define KVar_STRING  10
#define FACTOR_FLOAT 11

//��չ��3332ת������
#define METER_INDEX    12   //���ܱ��ʲ����
#define METER_TYPE     13   //���ܱ�����
#define MODULE_IMEI    14   //IMEI
#define MODULE_ICCID   15   //ICCID
#define MODULE_IMSI   16   //IMSI
#define MODULE_CSQ     17   //�ź�ǿ�ȵȼ�
#define METER_ADDR     18   //���ͨ�ŵ�ַ
#define NOTIFY_CYCLE    19   //�����ϱ�����
#define SOFT_VERSION   20   //��������汾��
#define HARD_VERSION   21   //����Ӳ���汾��
#define VENDER_NO       22   //���ұ��
#define ONENET_TIME    23   //���ڡ����ڣ�����0���������죩��ʱ��
#define POWER_SUPPLY   24   //��ǰ����״̬
#define DATA_FLOW       25   // ��������
#define REMOTE_UPDATE   26  //Զ������ 
#define SZDC_QY         27 //ʱ�ӵ��Ƿѹ
#define SD_TSD          28 //�е�ͣ�ϵ�
#define FZDY_TSD        29 //������Դͣ�ϵ�
#define YGGL_FX         30 //�й����ʷ���
#define WGGL_FX         31 //�޹����ʷ���
#define A_GL            32 //A�����
#define B_GL            33 //B�����
#define C_GL            34 //C�����
#define A_DX            35 //A�����
#define B_DX            36 //B�����
#define C_DX            37 //C�����
#define KWH_FLOAT_MINUS 38 //������λ����ʾֵ������й�������޹���
#define RSRP            39 //RSRP
#define SINR            40 //�����
#define eNODEB          41 //С��eNodeB cell idǰ��λ
#define CELL_ID         42 //cell id 


#define ONENET_VOLTAGE          3316
#define ONENET_CURRENT          3317
#define ONENET_POWER            3328
#define ONENET_POWER_FACTOR     3329
#define ONENET_ENERGY           3331
#define ONENET_METER_INFO       3332
#define ONENET_METER_TIME       3333
#define ONENET_EVENT            3200

//����resource ID 
#define ONENET_SENSOR_VALUE     5700
#define ONENET_UNITS            5701
#define ONENET_RESOURCE_APP     5500


#define DAYHOLD_DATA    0x1000 
#define CYCLE_DATA      0x2000
#define MONTHHOLD_DATA  0x4000 


#define READ_METER_SUCCESS  0x00000001 //���ͨ�ųɹ�
#define CGATT_SUCCESS  0x00000002 //ģ��ע���ɹ�
#define LOGIN_SUCCESS  0x00000004 //�ѵ�¼
#endif