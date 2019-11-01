#ifndef __PLMS_READ_C1_H__
#define __PLMS_READ_C1_H__



//�ն����в����ļ�
#define FILE_RUN_PARA   FILEID_RUN_DATA

//ת������BCD:X, BIN:B
#define MeterDate_3761FMT1		0x01	//�������ת������:���������(���ֽ�)YYMMDDWW+HHMMSS(���ֽ�)ת��Ϊ3761A1
#define ProgrammDateTime_3761FMT1	0x02	//376.1 A.1 format YYWMMDDHHMMSS(BCD:YY DDHHMMSS, BIN:WMM(W:bit7-5,M:bit4,M:BCD) )
#define HarmonicFactor_3761FMT5	0x03	//г������645: XX.XX% ->376.1 A.5 S(bit7)X(bit6-4)XX.X%
#define Voltage97_3761FMT7		0x04	//97_645 xxx -> 376.1 A.7 xxx.x
#define ReactivePower97_3761FMT9	0x05 	//97_645 xx.xx -> 376.1 A.9 xx.xxxx
#define PhaseLossingNum97_3761FMT10	0x06	//97_645 NNNN -> 376.1 A.10 xxxxxx
#define Energy_3761FMT14	0x07	//645 A.1XXXXXX.XX ->376.1 A.14XXXXXX.XXXX
#define DemandTime07_3761FMT17		0x08 	//07_645 YYMMDDhhmmss -> 376.1 A.17 MMDDhhmm
#define Current97_3761FMT25		0x09 	//97_645 xx.xx -> 376.1 A.25  xxx.xxx
#define CellWorkingTime97_3761FMT27		0x0A	// 97_645 NNNNNN -> 376.1 xxxxxxxx
#define ZTZ3_3761FMT  0x0B    //FN161��״̬��3ת��
#define PhaseB_3761FMT  0x0C  //�ز���B��������
#define PhaseC_3761FMT  0x0D  //�ز���C��������
#define MeterDate_3761FMT1_SH		0x0E    //�Ϻ�һ��F162����
#define Energy_FMT14toFMT11	0x1F	//645 A.1XXXXXX.XXXX ->376.1 A.11XXXXXX.XX

//ת������
#define RATE            0x0F
#define REC_DATE	0x10	//  �ն�ʱ��-> 376.1 A.15
#define REC_DATE_SEC    0x11
#define MeterPowerStatus_3761FMT	0x12 //376.1 F161 ���ܱ�ͨ�ϵ�״̬
#define DXSJ_COMPLEMENT_3761FMT 0x13               //����ABC����� �����ܴ�����
#define Datetime2FMT15          0x14   //YYWMMDDHHMMSS -> YYWMMDDHHMM
#define SetPhaseV               0x15   //Ĭ�ϵ�ѹ���0 120 240
#define A3toA2                  0x16   //3�ֽڱ��2�ֽ�
#define Datetime09toFMT15       0x17   //09��Լ��ʱ��5�ֽ�
#define I3toI2                  0x18   //�����ĸ�ʽת��
#define RATE_1                  0x19   //����1
#define DELEWEEK                0x20   //ȥ������
#define JSJL_HSJ                0x21   //Уʱ��¼��ʱ��
#define JSJL_QSJ                0x22   //Уʱ��¼ǰʱ��
#define SYDL_ZDZ_2_376          0x23   //����ʣ��������ֵ ����˳�򵹻�
#define DLSJK_FORMAT3to4        0x24   //���ն�·����������ת��
#define BbreakerIto376          0x25   //����ʣ��������������ת��
#define CbreakerIto376          0x26   //����ʣ��������������ת��
#define GetSYDL                 0x27   //����ʣ���������ȡ��������
#define GET_REC_DATE            0x28   //��������ʱ��
#define GET_RQ                  0x29   //��������ȡ������
#define AbreakerIto376          0x30   //����ʣ��������������ת��
#define S1C_JS_JLto3761         0x31   //Уʱ��¼��ȡУʱǰʱ�� ����

//C8֮��滮�ı���ת������
#define DataCJT_to_3761FMT      0xC8//CJT188ת376.1
#define CJT_SQDB_TO_3761        0xC9//ˮ����ת376.1
#define CJT_RB_TO_3761          0xCA//�ȱ�ת376.1
#define CJT_MODBUS_TO_3761      0xCB// modbusת376.1
#define MODBUS_EE_TO_FALSEDATA  0xCC//ˮ�ű���������
#define CJT_SHUIMEN_TO_3761     0xCD//ˮ��Э��ת376.1
#define CJT_WUHAN_TO_3761       0xCE//�人ˮ��ת376.1
#define CJT_EN1434_RB_TO_3761   0xCF// EN1434ת376.1
#define XY645_TO_3761           0xD1//��Դˮ��645Э��ת376.1
#define CJT_HUBEI645_TO_3761    0xD2//
#define CJT_SQDB_TO_3761JIANGXI 0xD3//�������ֵ�ȼ����CJT188Э�飬���ǻظ�����ֻ��6�ֽ� ��3���ֽڵ���������С��λ
#define CJT_NINGBO_TO_3761      0xD4//
#define CJT_LIXIN_TO_3761       0xD5//
#define XINGYUAN_TO_3761        0xD6
#define CJT_FENXI645_RB_TO_3761 0xD7
#define FUJIAN645_TO_3761       0xD8
#define CJT_645_97_RB_TO_3761   0xD9
#define CJT_RB_TO_3761_reverse  0xDA//�ȱ�ת376.1,�ظ����ĵ�λ������ǰ������������ȱ�
#define CJT_DAYHOLD_TO_RECDAYHOLD      0xDB
#define CJT_JIAJIENENG_RB_TO_3761      0xDC
#define CJT_SQDB_TO_3761_HN            0xDD
#define CJT_DAYHOLD_TO_RECDAYHOLD_HN   0xDE
#define CJT_SQDB_SHORT_TO_3761         0xDF
#define SR8009_TO_3761                 0xE1 //�м��ж�����
#define CJT_SHANKE_TO_3761             0xE2
#define CJT_CHONGQING_TO_3761          0xE3
#define CJT_HANGZHOU_TO_3761           0xE4
#define CJT_MODBUS_RQB_TO_3761         0xE5
#define CJT_CHENGUANG_TO_3761          0xE6
#define CJT_SHANDONG_F28_TO_3761       0xE7
#define CJT_DAYHOLD_TO_RECDAYHOLD_RB     0xE8
#define CJT_DAYHOLD_TO_RECDAYHOLD_RB_HN  0xE9
#define CJT_SQDB_TO_3761_SH_RQB          0xEA
#define CJT_SQDB_TO_3761_SH_SB           0xEB
#define CJT_SQDB_TO_3761_0DF1_SZ         0xEC
#define CJT_SQDB_TO_3761_0DF161_SZ       0xED
#define CJT_SQDB_TO_3761_0CF129_SZ       0xEE
#define CJT_JINGDA_TO_3761               0xEF
#define CJT_CHENGUANG_NEW_TO_3761        0xF0
#define MAX_RES_SIZE    1000

//��ʼ�ֽ�
#define PHY_LEN          4
#define RES_LEN          1
//#define RES_DATA       5


//ʹ�û�������־
#define RES_BUF   0x01
#define REQ_BUF   0x02


#define  PIM_METER_DOC 0
//��������ʶ
#define   BLOCK_MARK 0x30
//�����ʶ
#define   BLOCK_FIRST     0x00
#define   SINGLE         0x01
#define   BLOCK           0x02


//����645-1997����λ�ö���
#define POS_GB645_FLAG1       0
#define POS_GB645_METERNO     1
#define POS_GB645_FLAG2       7
#define POS_GB645_CTRL        8
#define POS_GB645_DLEN        9
#define POS_GB645_ITEM       10
#define POS_GB645_97_DATA    12
#define POS_GB645_07_DATA    14

//��������ַ��
#define CUR_YGZ      0000
#define CUR_WGZ      0001
#define CUR_Q1_WGZ   0002
#define CUR_Q4_WGZ   0003
#define DATE_TIME    0004
#define CUR_P_YG     0005
#define CUR_P_WG     0006
#define CUR_P_SZ     0007//���ڹ���
#define CUR_Q        0008//��������
#define CUR_V        0009
#define CUR_I        0010
#define CUR_I_LX     0011//���ߵ���


//һ�����ݳ�ʱʱ��
#define  TIME_OUT_C1    6000  //��λ 10ms


void get_work_info(INT8U *resp);
void  process_frame_read(objRequest *pRequest,objResponse *pResp);
BOOLEAN get_read_c1data(objRequest *pRequest,objResponse *pResp,READ_CONTEXT *readC1);
INT16U app_get_fn_data_c1(objRequest *pRequest,objResponse *pResp);
INT16U read_C1_F1(INT8U *resp);

#endif       /* __PLMS_READ_C1_H__ */

