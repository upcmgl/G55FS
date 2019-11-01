#ifndef __PROTOCOL_OBJECT_OBIS_H__
#define __PROTOCOL_OBJECT_OBIS_H__

#define CLASS_EVENT              7      //�¼��ӿ���
#define CLASS_INPUT_OUTPUT      22      //��������豸�ӿ���
#define CLASS_HOLD               9      //�������ݽӿ���
#define CLASS_SUB_EVENT         24      //�����¼��ӿ���
#define CLASS_STAT               14      //����ͳ�ƽӿ�
#define CLASS_AVG                15      //�ۼ�ƽ���ӿ���
#define CLASS_MAX_MIN            16      //��ֵ���߽ӿ���

#define OBIS_CUSTOM_INFO        0xFFFD    //�����Զ�����Ϣ
#define OBIS_DEBUG_INFO         0xFFFE    //������Ϣ



#define OBIS_EVENT_CONFIG       0x9001    //�¼��ϱ�
#define OBIS_DC_PARAM           0x9210    //ֱ��ģ��������
#define OBIS_SWITCH             0x9211    //״̬���������
#define OBIS_RECTIME            0x9221    //����ʱ�β���

//�α��������
#define OBIS_DATETIME           0x4000    //���ܱ�����ʱ��
#define OBIS_COMM_ADDR          0x4001    //ͨ�ŵ�ַ
#define OBIS_SERVER_ADDR        0x202A    //Ŀ���������ַ
#define OBIS_METER_NO           0x4002    //���
#define OBIS_CUST_NO            0x4003    //�ͻ����
#define OBIS_GPS_SEAT           0x4004    //�豸����λ��
#define OBIS_GROUP_ADDR         0x4005    //���ַ
#define OBIS_CLOCK              0x4006    //ʱ��Դ
#define OBIS_LCD                0x4007    //LCD
#define OBIS_TIMEZONE_SWITCH    0x4008  //����ʱ�����л�ʱ��
#define OBIS_TIMESEG_SWITCH     0x4009  //����ʱ�α��л�ʱ��
#define OBIS_FL_PRICE_SWITCH    0x400A  //���׷�ʱ�����л�ʱ��
#define OBIS_TIMEZONE_SEG_NUM      0x400C   //ʱ��ʱ����
#define OBIS_HOLIDAY_TABLE         0x4011   //�������ձ�
#define OBIS_WEEKEND_FLAG          0x4012   //������������
#define OBIS_WEEKEND_TIMESEG       0x4013   //������?�õ���ʱ�α��
#define OBIS_CUR_TIMEZONE_TABLE 0x4014	  //��ǰ����ʱ����
#define OBIS_BAK_TIMEZONE_TABLE 0x4015	  //��������ʱ����
#define OBIS_CUR_FL_TABLE       0x4016	  //��ǰ����ʱ�α�
#define OBIS_BAK_FL_TABLE       0x4017	  //��������ʱ�α�
#define OBIS_CUR_FL_PRICE       0x4018	  //��ǰ�׷��ʵ��
#define OBIS_BAK_FL_PRICE       0x4019	  //�����׷��ʵ��
#define OBIS_CUR_JT_PRICE       0x401A      //��ǰ�׽��ݵ��
#define OBIS_BAK_JT_PRICE       0x401B      //�����׽��ݵ��
#define OBIS_EXCLUDED           0x4024    //�޳�
#define OBIS_CJQ_UPDATE_RESULT  0x4025    //�ɼ����������
#define OBIS_V_RATE             0x4030    //��ѹ�ϸ���
#define OBIS_ASSET_NO           0x4103    //�ʲ��������
#define OBIS_YGZH_FLAG          0x4112      //�й����������
#define OBIS_WGZH_1             0x4113      //�޹����������1
#define OBIS_WGZH_2             0x4114      //�޹����������2
#define OBIS_SETTLEMENT_DAY     0x4116    //������
#define OBIS_METER_TYPE         0x410B      //����ͺ�
#define OBIS_ROUTER             0x4200    //·�ɱ�
#define OBIS_ROUTER_INFO        0x4201    //·����Ϣ��Ԫ

#define OBIS_CASCADE            0x4202    //����ͨѶ����
//#define OBIS_COMM_STATE         0x4203    //�ն�����ͨ��״̬����
#define OBIS_BROADCAST_TIME     0x4204    //�ն˹㲥Уʱʱ��


#define OBIS_DEV_JZQ            0x4300    //������
#define OBIS_DEV_ENERGY         0x4306    //���ܱ�
#define OBIS_DEV_WATER          0x4307    //ˮ��
#define OBIS_DEV_GAS            0x4308    //����
#define OBIS_DEV_HEAT           0x4309    //�ȱ�

#define OBIS_LINK               0x4400    //Ӧ������
#define OBIS_LINK_PASS          0x4401    //�����֤�Ŀ���

#define OBIS_GPRS1               0x4500    //gprs1
#define OBIS_GPRS2               0x4501    //gprs2
#define OBIS_NET1               0x4510    //NET1
#define OBIS_NET2               0x4511    //NET2
#define OBIS_NET3               0x4512    //NET3
#define OBIS_NET4               0x4513    //NET4
#define OBIS_NET5               0x4514    //NET5
#define OBIS_NET6               0x4515    //NET6
#define OBIS_NET7               0x4516    //NET7
#define OBIS_NET8               0x4517    //NET8


//�ɼ���������
#define OBIS_METER_TABLE        0x6000      //�ɼ��������ñ�
#define OBIS_METER_INFO         0x6001      //�ɼ��������õ�Ԫ
#define OBIS_SEARCH_METER_TABLE 0x6002      //�ѱ����
#define OBIS_SEARCH_METER_INFO  0x6003      //�ѱ���Ϣ
#define OBIS_MAINNODE_CHANGE    0x6004      //һ����̨�����
#define OBIS_DATA_PROP_TABLE    0x6010      //����������������������
#define OBIS_DATA_PROP          0x6011      //������������
#define OBIS_TASK_TABLE         0x6012      //�������ñ�
#define OBIS_TASK               0x6013      //����������Ϣ
#define OBIS_NORMAL_TASK_TABLE  0x6014      //��ͨ�ɼ�������
#define OBIS_NORMAL_TASK        0x6015      //��ͨ�ɼ�����
#define OBIS_EVENT_TASK_TABLE   0x6016      //�¼��ɼ�������
#define OBIS_EVENT_TASK         0x6017      //�¼��ɼ�����
#define OBIS_RELAY_TASK_TABLE   0x6018      //͸��������
#define OBIS_RELAY_TASK         0x6019      //͸������
#define OBIS_RELAY_RESULT_TABLE 0x601A      //͸�����������
#define OBIS_RELAY_RESULT       0x601B      //͸���������
#define OBIS_REPORT_TASK_TABLE  0x601C      //�ϱ�������
#define OBIS_REPORT_TASK        0x601D      //�ϱ�����
#define OBIS_RULE_TABLE         0x601E      //�ɼ������
#define OBIS_RULE               0x601F      //�ɼ�����
#define OBIS_EXTEND_RULE_TABLE  0x6036      //��չ�ɼ������
#define OBIS_EXTEND_RULE        0x6037      //��չ�ɼ�����Ԫ



#define OBIS_EVENT_COLUMN_PROP_TABLE    0x6020      //�¼����Կ�
#define OBIS_EVENT_COLUMN_PROP          0x6021      //�¼�����

#define OBIS_ACQ_STATUS_TABLE          0x6032      //�ɼ�״̬��
#define OBIS_ACQ_STATUS                0x6033      //�ɼ�״̬

#define OBIS_ACQ_MONITOR_TABLE          0x6034      //�ɼ������ؼ�
#define OBIS_ACQ_MONITOR                0x6035      //�ɼ������ص�Ԫ

#define OBIS_ACQ_BEGIN_TD               0x6040      //�ɼ�����ʱ��
#define OBIS_ACQ_END_TD                 0x6041      //�ɼ��ɹ�ʱ��
#define OBIS_ACQ_SAVE_TD                0x6042      //�ɼ��洢ʱ��

//���������
#define OBIS_FILE_TABLE             0x7000    //�ļ�����
#define OBIS_FILE                   0x7001    //�ļ�
#define OBIS_SCRIPT_TABLE           0x7010    //�ű�����
#define OBIS_SCRIPT                 0x7011    //�ű�
#define OBIS_SCRIPT_RESULT_TABLE    0x7012    //�ű��������
#define OBIS_SCRIPT_RESULT          0x7013    //�ű����
#define OBIS_EXT_VAR_TABLE          0x7100    //��չ�������󼯺�
#define OBIS_EXT_PARAM_TABLE        0x7101    //��չ�α������󼯺�


//�ļ����������
#define OBIS_SLICING_TRANSMIT   0xF000    //��֡����
#define OBIS_BLOCK_TRANSMIT     0xF001    //�ֿ鴫��
#define OBIS_EXTEND_TRANSMIT    0xF002    //��չ����

//��ȫ�����
#define OBIS_ESAM           0xF100    //ESAM
#define OBIS_ESAM_PARAM     0xF101    //ESAM

//��������豸�����
#define OBIS_IO_RS232           0xF200    //����
#define OBIS_IO_RS485           0xF201    //RS485
#define OBIS_IO_INFRA           0xF202    //����
#define OBIS_IO_INPUT           0xF203    //����������
#define OBIS_IO_DC              0xF204    //ֱ��ģ����
#define OBIS_IO_OUTPUT          0xF205    //�̵������
#define OBIS_IO_ALAERT          0xF206    //�澯���
#define OBIS_IO_FUNC            0xF207    //�๦�ܶ���
#define OBIS_IO_AC              0xF208    //��������
#define OBIS_IO_PLC             0xF209    //�ز�/΢�������߽ӿ�
#define OBIS_IO_PULSE           0xF20A    //���������豸
#define OBIS_IO_BLUETOOTH       0xF20B    //�����豸
#define OBIS_IO_230M            0xF20C    //230M����ר��
#define OBIS_SLAVEUNIT           0xF300  //�Զ�����

#define OBIS_MTR_EVT_LOST_V         0x3000  //���ܱ�ʧѹ�¼�
#define OBIS_MTR_EVT_UNDERVOLTAGE   0x3001  //���ܱ�Ƿѹ�¼�
#define OBIS_MTR_EVT_OVERVOLTAGE    0x3002  //���ܱ��ѹ�¼�
#define OBIS_MTR_EVT_SEVER_PHASE    0x3003  //���ܱ�����¼�
#define OBIS_MTR_EVT_POWERONOFF     0x3011  //���ܱ�����¼�
#define OBIS_EVT_COMMODULTE_CHANGE    0x3030   //ͨ��ģ�����¼�

#define OBIS_EVT_INIT               0x3100  //�ն˳�ʼ���¼�
#define OBIS_EVT_VER_CHANGE         0x3101  //�ն˰汾����¼�
#define OBIS_EVT_STATUS_CHANGE      0x3104  //�ն�״̬����λ�¼�
#define OBIS_EVT_METER_CLOCK        0x3105  //���ܱ�ʱ�ӳ����¼�
#define OBIS_EVT_POWER_ONOFF        0x3106  //�ն�ͣ/�ϵ��¼�
#define OBIS_EVT_ESAM_ERROR         0x3109  //�ն���Ϣ��֤�����¼�
#define OBIS_EVT_DEV_ERROR          0x310A  //�ն˹��ϼ�¼
#define OBIS_EVT_AMOUNT_DOWN        0x310B  //���ܱ�ʾ���½��¼�
#define OBIS_EVT_AMOUNT_OVER        0x310C  //�����������¼�
#define OBIS_EVT_AMOUNT_FLY         0x310D  //���ܱ�����¼�
#define OBIS_EVT_AMOUNT_STOP        0x310E  //���ܱ�ͣ���¼�
#define OBIS_EVT_READMETER_FAIL     0x310F  //�ն˳���ʧ���¼�
#define OBIS_EVT_FLOW_OVER          0x3110  //��ͨ�����������¼�
#define OBIS_EVT_NEW_METER          0x3111  //����δ֪���ܱ��¼�
#define OBIS_EVT_MAINNODE_CHANGE    0x3112  //��̨�����ܱ��¼�
#define OBIS_EVT_SET_CLOCK          0x3114  //�ն˶�ʱ�¼�
#define OBIS_EVT_REMOTE_CTRL        0x3115  //ң����բ��¼
#define OBIS_EVT_CONTROL_LINE       0x3117  //�����·����״̬��λ�¼���¼
#define OBIS_EVT_PROGRAM            0x3118  //�ն˱�̼�¼
#define OBIS_EVT_I_EXCEPT           0x3119  //�ն˵�����·�쳣�¼�
#define OBIS_EVT_METER_STATUS_CHG   0x311A  //���ܱ�����״̬�л��¼�
#define OBIS_EVT_METER_SET_CLOCK    0x311B  //�ն˶Ե��Уʱ��¼
#define OBIS_EVT_METER_MONITOR      0x311C  //���ܱ����ݱ����ؼ�¼

#define OBIS_EVT_POWER_CTRL         0x3200  //������բ��¼
#define OBIS_EVT_AMOUNT_CTRL        0x3201  //�����բ��¼
#define OBIS_EVT_BUYFEE             0x3202  //����������ü�¼
#define OBIS_EVT_AMOUNT_ALRAM       0x3203  //��ظ澯�¼���¼

#define OBIS_UNIT_NORAML             0x3301  //��׼ʱ���¼��Ԫ
#define OBIS_UNIT_PROGRAM            0x3302  //��̼�¼�¼���Ԫ
#define OBIS_UNIT_NEW_METER          0x3303  //����δ֪����¼���Ԫ
#define OBIS_UNIT_MAINNODE_CHANGE    0x3304  //��̨�����ܱ��¼���Ԫ
#define OBIS_UNIT_POWER_CTRL         0x3305  //������բ��¼��Ԫ
#define OBIS_UNIT_AMOUNT_CTRL        0x3306  //�����բ��¼��Ԫ
#define OBIS_UNIT_AMOUNT_ALRAM       0x3307  //��ظ澯�¼���Ԫ
#define OBIS_UNIT_XL_OVER            0x3308  //���ܱ����������¼���Ԫ
#define OBIS_UNIT_POWER_ONOFF        0x3309  //�ն�ͣ�ϵ��¼��Ԫ
#define OBIS_UNIT_REMOTE_CTRL        0x330A  //ң���¼���¼��Ԫ
#define OBIS_UNIT_YGAMOUNT_OVER      0x330B  //�й��ܵ������Խ���¼���¼��Ԫ
#define OBIS_UNIT_EVENT_RESET        0x330C  //�¼������¼���¼��Ԫ
#define OBIS_UNIT_METER_SETCLOCK     0x330D  //�ն˶Ե��Уʱ��¼��Ԫ
#define OBIS_UNIT_METER_STATUS_CHG   0x330E  //���ܱ�����״̬�л��¼���Ԫ
#define OBIS_UNIT_METER_MONITOR      0x330F  //���ܱ����ݱ����ؼ�¼��Ԫ
#define OBIS_UNIT_COMMODULTE_CHANGE   0x3312  //ͨ��ģ�����¼���Ԫ

#define OBIS_UNIT_METER_CLOCK        0x3313  //���ܱ�ʱ�ӳ����¼���¼��Ԫ

#define OBIS_NETFLOW            0x2200     //ͨ������
#define OBIS_POWERTIME          0x2203     //����ʱ��
#define OBIS_RESETCOUNT         0x2204     //��λ����

#define OBIS_AMOUNT_ZXYG        0x0010    //�����й�����
#define OBIS_AMOUNT_FXYG        0x0020    //�����й�����
#define OBIS_AMOUNT_ZXWG        0x0030    //�����޹�����
#define OBIS_AMOUNT_FXWG        0x0040    //�����޹�����
#define OBIS_VOLTAGE            0x2000    //��ѹ
#define OBIS_ELECTRICITY        0x2001    //����
#define OBIS_V_ANGLE            0x2002    //��ѹ���
#define OBIS_VI_ANGLE           0x2003    //��ѹ�������
#define OBIS_ACTIVE_POWER       0x2004    //�й�����
#define OBIS_REACTIVE_POWER     0x2005    //�޹�����
#define OBIS_APPARENT_POWER     0x2006    //���ڹ���
#define OBIS_POWER_FACTOR       0x200A    //��������
#define OBIS_1XX_WG             0x0050    //һ�����޹�
#define OBIS_2XX_WG             0x0060    //�������޹�
#define OBIS_3XX_WG             0x0070    //�������޹�
#define OBIS_4XX_WG             0x0080    //�������޹�
#define OBIS_ZXYG_ZDXL          0x1010    //�����й��������
#define OBIS_FXYG_ZDXL          0x1020    //�����й��������
#define OBIS_ZXWG_ZDXL          0x1030    //����޹�1�������
#define OBIS_FXWG_ZDXL          0x1040    //����޹�2�������
#define OBIS_ClOCK_BATTERY_V    0x2011      //ʱ�ӵ�ص�ѹ
#define OBIS_ClOCK_BATTERY_T    0x2013      //ʱ�ӵ�ع���ʱ��
#define OBIS_METER_STATE        0x2014    //���ܱ�״̬��

#define OBIS_CUR_YGXL           0x2017      //��ǰ�й�����
#define OBIS_CUR_WGXL           0x2018      //��ǰ�޹�����
#define OBIS_CUR_SZXL           0x2019      //��ǰ��������
#define OBIS_V_UNBALANCE        0x2026      //��ѹ��ƽ��
#define OBIS_I_UNBALANCE        0x2027      //������ƽ��
#define OBIS_AMPERE_VALUE       0x2029      //��ʱֵ

#define OBIS_METER_V_RATE_A     0x2131    //����A���ѹ�ϸ���

#define OBIS_WALLET_FILE        0x202C      //����ǰ��Ǯ���ļ�
#define OBIS_OVERDRAFT_AMOUNT   0x202D      //����ǰ��͸֧���
#define OBIS_CUMULATIVE_AMOUNT  0x202E      //�ۼƹ�����

#define OBIS_EVENT_BEGIN_TIME       0x201E      //�¼�����ʱ��
#define OBIS_EVENT_END_TIME         0x2020      //�¼�����ʱ��
#define OBIS_EVENT_SEQ              0x2022      //�¼���¼���
#define OBIS_EVENT_SOURCE           0x2024      //�¼�����Դ
#define OBIS_EVENT_REPORT_STATUS    0x3300      //ͨ���ϱ�״̬

#define OBIS_HOLD_TIME             0x2021      //����ʱ��
#define OBIS_HOLD_SEQ              0x2023      //�����¼���


//�������
#define OBIS_ADDGROUP_1         0x2301    //�ܼ���1
#define OBIS_ADDGROUP_2         0x2302    //�ܼ���2
#define OBIS_ADDGROUP_3         0x2303    //�ܼ���3
#define OBIS_ADDGROUP_4         0x2304    //�ܼ���4
#define OBIS_ADDGROUP_5         0x2305    //�ܼ���5
#define OBIS_ADDGROUP_6         0x2306    //�ܼ���6
#define OBIS_ADDGROUP_7         0x2307    //�ܼ���7
#define OBIS_ADDGROUP_8         0x2308    //�ܼ���8

#define OBIS_PULSE_METER_1         0x2401    //���������1
#define OBIS_PULSE_METER_2         0x2402    //���������2
#define OBIS_PULSE_METER_3         0x2403    //���������3
#define OBIS_PULSE_METER_4         0x2404    //���������4
#define OBIS_PULSE_METER_5         0x2405    //���������5
#define OBIS_PULSE_METER_6         0x2406    //���������6
#define OBIS_PULSE_METER_7         0x2407    //���������7
#define OBIS_PULSE_METER_8         0x2408    //���������8

#define OBIS_ACC_WAR_FLOW           0x2500   //�ۼ�ˮ���ȣ�����
#define OBIS_ACC_GAS_FLOW           0x2501  //�ۼ�������
#define OBIS_ACC_HOT_FLOW           0x2502  //�ۼ�����
#define OBIS_HOT_POWER              0x2503  //�ȹ���
#define OBIS_ACC_WORK_TIME          0x2504  //�ۼƹ���ʱ��
#define OBIS_WARTER_TEMPERATURE     0x2505  //ˮ��
#define OBIS_STATUS_ST              0x2506  //���Ǳ�״̬ST

#define OBIS_REMOTE_CTRL        0x8000    //ң��
#define OBIS_PROTECT            0x8001    //����
#define OBIS_ALARM_FEE          0x8002    //�߷Ѹ澯
#define OBIS_CHINESE_INFO       0x8003    //������Ϣ
#define OBIS_CHINESE_INFO_VIP   0x8004    //��Ҫ������Ϣ
#define OBIS_SECURITY_VALUE     0x8100    //������ֵ
#define OBIS_PCTRL_TIMESEG      0x8101    //����ʱ��
#define OBIS_PCTRL_ALARM_TIME   0x8102    //���ظ澯ʱ��
#define OBIS_TIMESEG_CTRL       0x8103    //ʱ�ι���
#define OBIS_RELAX_CTRL         0x8104    //���ݹ���
#define OBIS_STOPWORK_CTRL      0x8105    //Ӫҵ��ͣ��
#define OBIS_FLOATDOWN_CTRL     0x8106    //��ǰ�����¸���
#define OBIS_BUYFEE_CTRL        0x8107    //�����
#define OBIS_MONTHFEE_CTRL      0x8108    //�µ��
#define OBIS_TIMESEG_CFG_UNIT   0x8109    //ʱ�ι������õ�Ԫ
#define OBIS_RELAY_CFG_UNIT     0x810A    //���ݿ����õ�Ԫ
#define OBIS_STOPWORK_CFG_UNIT  0x810B    //Ӫҵ��ͣ�����õ�Ԫ
#define OBIS_BUYFEE_CFG_UNIT    0x810C    //��������õ�Ԫ
#define OBIS_MONTHFEE_CFG_UNIT  0x810D    //�µ�����õ�Ԫ
#define OBIS_CTRL_OBJ           0x810E    //���ƶ���
#define OBIS_RELAY_TURN         0x810F    //��բ�ִ�
#define OBIS_FCTRL_VALUE        0x8110    //��ض�ֵ

#define OBIS_INSTANTANEOUS_HOLD     0x5000      //˲ʱ����
#define OBIS_SEC_HOLD               0x5001      //�붳��
#define OBIS_MINUTE_HOLD            0x5002      //���Ӷ���
#define OBIS_HOUR_HOLD              0x5003      //Сʱ����
#define OBIS_DAY_HOLD               0x5004      //�ն���
#define OBIS_SETTLEMENT_DAY_HOLD    0x5005      //�����ն���
#define OBIS_MONTH_HOLD             0x5006      //�¶���
#define OBIS_YEAR_HOLD              0x5007      //�궳��
#define OBIS_TZONE_SWITCH_HOLD      0x5008      //ʱ�����л�����
#define OBIS_DAY_PERIOD_HOLD        0x5009      //��ʱ�α��л�����
#define OBIS_ELECTRICITY_HOLD       0x500A      //���ʵ���л�����
#define OBIS_LADDER_SWITCH_HOLD     0x500B      //�����л�����
#define OBIS_MON_VOLTAGE_HOLD       0x5010      //��ѹ�ϸ����¶���
#define OBIS_LADDER_SETTLE_HOLD     0x5011      //���ݽ��㶳��


#define OBIS_STAT_MINUTE            0x2100	//��������ͳ��	ͳ�����ڵ�λΪ����
#define OBIS_STAT_HOUR              0x2101	//Сʱ����ͳ��	ͳ�����ڵ�λΪСʱ
#define OBIS_STAT_DAY               0x2102	//������ͳ��	ͳ�����ڵ�λΪ��
#define OBIS_STAT_MONTH             0x2103	//������ͳ��	ͳ�����ڵ�λΪ��
#define OBIS_STAT_YEAR              0x2104	//������ͳ�� 	ͳ�����ڵ�λΪ��
#define OBIS_AVG_MINUTE             0x2110	//����ƽ��	ͳ�����ڵ�λΪ����
#define OBIS_AVG_HOUR               0x2111	//Сʱƽ��	ͳ�����ڵ�λΪʱ
#define OBIS_AVG_DAY                0x2112	//��ƽ��	ͳ�����ڵ�λΪ��
#define OBIS_AVG_MONTH              0x2113	//��ƽ��	ͳ�����ڵ�λΪ��
#define OBIS_AVG_YEAR               0x2114	//��ƽ��	ͳ�����ڵ�λΪ��
#define OBIS_MAX_MIN_MINUTE         0x2120	//���Ӽ�ֵ	ͳ�����ڵ�λΪ����
#define OBIS_MAX_MIN_HOUR           0x2121	//Сʱ��ֵ	ͳ�����ڵ�λΪʱ
#define OBIS_MAX_MIN_DAY            0x2122	//�ռ�ֵ	ͳ�����ڵ�λΪ��
#define OBIS_MAX_MIN_MONTH          0x2123	//�¼�ֵ	ͳ�����ڵ�λΪ��
#define OBIS_MAX_MIN_YEAR           0x2124	//�꼫ֵ	ͳ�����ڵ�λΪ��

#endif