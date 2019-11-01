#ifndef __APP_CONSTDEF_H__
#define __APP_CONSTDEF_H__


//Ӧ�ò��־��
#define  APP_POWER_ONOFF_CY  0x01     //ͣ�ϵ��¼��������ɲ����������ն��Լ�����

  #define COMMPORT_485_CY    0x01     //�����ڱ�
  #define COMMPORT_485_REC   0x02     //����
  #define COMMPORT_485_CAS   0x03     //����
  #define COMMPORT_PLC       0x1F     //�ز����ö˿ڣ�����ʹ������˿�
  #define COMMPORT_PLC_REC   0x04     //�ز�����˿ڣ��������ڲ�ʹ�õĶ˿�
  
  #define COMMPORT_INFRA_RED   0xFE     //���غ����
  #define COMMPORT_GPRS   0xFD     	 //����GPRS��

#if defined  __PROVICE_JIANGSU__
     #define DATA_DEFAULT      0xEE
#else
     #define DATA_DEFAULT       0xFF
#endif

#define GB645_2007       30      //645-2007

#define EVENT_RECORD_SIZE  152

//WORK_FLAG ʹ�ö���
#define  WORK_FLAG_SEQ_RSEQ        0
#define  WORK_FLAG_IS_FIRST_FRAME  1    //��ʾ�Ƿ��Ƕ�֡�е���֡
#define  WORK_FLAG_CHECK_FLAG      2
#define  WORK_FLAG_METER_READ      3
#define  WORK_FLAG_IS_CAST         4    //��ʾ����֡�е��ն˵�ַ�Ƿ��ǹ㲥��ַ
#define  WORK_FLAG_CMD_FLAG        5    //���������־

#define MAX_SIZE_PLMSDTP  1900

#define PLMT_VER  "C3.0"

//
//��Ϣ��֤�볤��,  ��Ϊ2�ֽں�16�ֽ�
//
#if defined (__PWLEN_IS_2BYTE__)
#define PW_LEN  2
#elif defined (__PWLEN_IS_32BYTE__)
#define PW_LEN  32
#else
#define PW_LEN  16
#endif

#ifdef __RTUADDR_IS_6BYTE__
#define CFG_DEVID_LEN               6    //�����ն˵�ַ�ĳ���
#else
#define CFG_DEVID_LEN               4    //�����ն˵�ַ�ĳ���
#endif

//͸��ת������ת�����ݳ��ȣ���ȥ���ֽڳ��ȣ�PNFN��ת�������ֵ�����
#if defined __QGDW_FK2005__ || defined __PROVICE_NEIMENG__
#define RELAY_MINUS_LEN 9
#else
#define RELAY_MINUS_LEN 10
#endif

#define LIGHT   0
#define UNLIGHT 1

//͸��ģʽ����
#define PLC_TRANSIT_MODE   0xAA
#define GPRS_TRANSIT_MODE  0xBB

#define PROTECT_TEMPERATURE       83      //�����¶�
#define STARTPROTECT_TEMPERATURE  88  //�����������¶�ֵ��һ��Ҫ����PROTECT_TEMPERATURE���𵽻���������

//Q/GDW����Э����ඨ��
#define QGDW_2004        1        //����2004
#define QGDW_376_1       2        //Q/GDW376.1
#define QGDW_2005        1       //����2005����ԭ2004Э�������ͬ��Э���־Ҳ��ͬ������Ŀǰ��ʵ�ָ�����
#define QGXD_FK2012      2       //�������Э��2012
#define QNMDW_FK2012     3       //���ɹ�2012�����ͨѶ��Լ


#define MAX_ADDGROUP_COUNT        8      //�ܼ����������
#define MAX_ADDGROUP_SPOTCOUNT    8      //�ܼ����ڲ������������

#define MAX_COMMPORT_COUNT  5      //ͨ�Ŷ˿�����


#if defined __POWER_CTRL_I__  || defined __POWER_CTRL_II__
#define MAX_PULSE_PORT_COUNT 8
#else
#define MAX_PULSE_PORT_COUNT 2
#endif

//��������
#define MCLASS_COMMON      0x01     //��ͨ���  
#define MCLASS_CJQ         0x10     //�ɼ���
#define MCLASS_ZJQ         0x20     //�м���




//ת��ִ�н������
#define  TRANS_ERR         0x00   //����ִ��ת����
#define  TRANS_TIMEOUT     0x01   //ת�����ճ�ʱ
#define  TRANS_RECV_ERR    0x02   //ת�����մ���
#define  TRANS_RECV_ACK    0x03   //ת������ȷ
#define  TRANS_RECV_NAK    0x04   //ת�����շ���
#define  TRANS_RECV_DATA   0x05   //ת����������




//#if defined(__PROVICE_ANHUI__) 
//#define PATCH_REC_DH_DAY               62      //�����������
//#else
//#define PATCH_REC_DH_DAY                3      //�����������
//#endif
//#define PATCH_REC_DH_NUM                6      //ÿ�β��������ն���ͻ���




//�㲥�����־,������8����ͬ��������
#define  PLCTASK_CASTSET           0x01      //�㲥��������

//�㲥����״̬
#define PLCCASTTASK_NO            0x00     //�㲥����ȡ����������
#define PLCCASTTASK_OK            0x01     //�㲥�������
#define PLCCASTTASK_EXEC          0x02     //�㲥����δ���


////Զ��ͨ�������
//#define REMOTECOMM_GPRS    0x01      //GPRS,SMS
//#define REMOTECOMM_CDMA    0x02      //CDMA,SMS
//#define REMOTECOMM_PSTN    0x03      //PSTN
//#define REMOTECOMM_SMS     0x04      //GSM-SMS
//#define REMOTECOMM_NET     0x05      //TCP-IP
//#define REMOTECOMM_OTHER   0x06      //other

//Զ��ͨ������
#define REMOTE_COMMAND_QUREY	        	0
#define REMOTE_COMMAND_RST			1
#define REMOTE_COMMAND_ON			2
#define REMOTE_COMMAND_OFF			3
#define REMOTE_COMMAND_ONLINE		        4
#define REMOTE_COMMAND_OFFLINE		        5
#define REMOTE_COMMAND_NOTECHG		        6

//Զ��ͨ��ͨ��״̬
#define REMOTE_CTS_GSMOFF		    	1	 //1:RESET 
#define REMOTE_CTS_GSMON		    	2	 //2:GSM POWER ON unreg 
#define REMOTE_CTS_GSMREG		    	3	 //3:gsm reg 
#define REMOTE_CTS_TCPLINK			4	 //4:tcp online
#define REMOTE_CTS_ONLINECOMMAND		5	 //5:tcponline command 

//liujing add
//�ײ�״̬����ʾ����
#define FOOT_SHOW_DATA                1
#define FOOT_SHOW_DATAANDFIRST        2
#define FOOT_SHOW_DATAANDMIDDLE       3
#define FOOT_SHOW_DATAANDLAST         4
#define FOOT_NOTSHOW_DATA             5

//����������������������
#define TRANS_JCSET_MODIFY   0x01    //�޸ĵ���
#define TRANS_JCSET_DEL      0x02    //ɾ������
#define TRANS_JCSET_ADD      0x04    //��������
#define TRANS_JCSET_CLR      0x08    //�������






//���б��ı�־����
#define MSA_BROADCAST  0x01  //��վ�㲥
#define DEV_ASK        0x02  //�豸ģ��������



//�¼�����壬�������¼������ļ���
//check_flag
#define  EXCLUDE_FLAG    0x01
#define  SWCTRL_FLAG     0x02
#define  VIPEVENT_FLAG   0x04
#define  SENDFRAME_FLAG  0x08
#define  CURVEDATE_FLAG  0x10
#define  VALIDEVT_FLAG   0x20
#define  NO_REPORT_FLAG  0x40

//�¼������ļ��У��¼���ʶ����
#define  VIP_EVENT_FLAG         0x01     //��Ҫ�¼���ʶ
#define  VALID_EVENT_FLAG       0x02     //��Ч�¼���ʶ
#define  EVENT_REPORT_FLAG      0x04     //�Ƿ����ϱ���ʶ
#define  EVENT_REPORT_SUC_FLAG  0x08     //�Ƿ��ϱ��ɹ���ʶ

//�����ϱ���ʶ����
#define  TASK_REPORT_FLAG       0x01     //�Ƿ����ϱ�,D0��Ч
#define  TASK_REPORT_IN_CYCLE   0x02     //�����������Ƿ����ϱ�


//�ļ�������ļ���ʶ����
#define PROG_FILE_FLAG    0x00  //������
#define DATA_FILE_FLAG    0x01    //�㽭��չռ��������
#define ASC168_FILE_FLAG  0x02
#define HZ1616_FILE_FLAG  0x03
#define PROG_COMM_MODULE  0x10  //ͨѶģ�����
#define PROG_METER_MODULE 0xA1  //���ɳ���
#define PROG_G14_MODULE   0xA2  //G14����
#define PROG_ROUTER_MODULE 0xA3  //·��


//��376.1�ļ������ļ�����
#define FILEPROP_FIR    0x00    //��ʼ֡���м�֡
#define FILEPROP_FIN    0x01    //����֡
//��376.1�ļ������ļ�ָ��
#define FILECMD_FRAME    0x00    //���ķ�ʽ
#define FILECMD_FTP      0x01    //FTP���䷽ʽ
#define FILECMD_GROUP    0x02    //�������ַ����



//�û�����Ŷ���
#define DEFAULT_USER_CLASS   0     //�û������ȱʡֵ
#define USER_CLASS_A         1     //A�࣬����ר���û�
#define USER_CLASS_B         2     //B�࣬��С��ר���û�
#define USER_CLASS_C         3     //C�࣬����һ�㹤��ҵ�û�
#define USER_CLASS_D         4     //D�࣬����һ�㹤��ҵ�û�
#define USER_CLASS_E         5     //E�࣬�����û�
#define USER_CLASS_F         6     //F�࣬������俼�˼�����

//�û�С���(�������)����
#define DEFAULT_METER_CLASS  0      //�������ȱʡֵ
#define METER_CLASS_SP       1      //�������ܱ�
#define METER_CLASS_TP       2      //�������ܱ�
#define METER_CLASS_APM     15     //APM���ݵ��(�½�ˮ��ú����APM)


//FCS����ʹ��

#define PPPINITFCS16 0xffff /* Initial FCS value */
#define PPPGOODFCS16 0xf0b8 /* Good final FCS value */





//�豸�����ֶ��壬���12��λ������device_flag����
#define DEVICE_UPDATE_13MODE	       0x0001		//֧��13��Э������
#define DEVICE_UPDATE_ZHEJIANG	   	   0x0002		//֧���㽭����ģʽ


//�����ո�ʽ�жϣ����������ʸ�4λ����
#define RECV_FORMAT_GB645           0x00
#define RECV_FORMAT_EDMI            0x10
#define RECV_FORMAT_HENGTONG        0x20    //��ͨ�¡��ɹ�Լ���ĸ�ʽһ��

// �������
#define OK                      0
#define ERR                     1
#define ERR_NOANSWER            1     //�м�����û�з���
#define ERR_BADFRAME            2     //�������ݷǷ�
#define ERR_AUTH                3     //����Ȩ�޲���
#define ERR_NOITEM              4     //�޴�������
#define ERR_NOROOM              4     //�ռ䲻��
#define ERR_EXPIRED             5     //����ʱ��ʧЧ
#define ERR_BADDST            0x11    //Ŀ���ַ������
#define ERR_SENDFAIL          0x12    //����ʧ��
#define ERR_SMSTOOLONG        0x13    //����Ϣ̫֡��

#define ESAM_RESET_OK        0x02



#define POS_CTRL     6       //������
#define POS_RT       7       //�ն������������� 2�ֽ�
#define POS_UID      9       //�ն˵�ַ        2�ֽ�
#ifdef __RTUADDR_IS_6BYTE__
    #define POS_MSA      13      //��վ��ַ�Լ����ַ��־
    #define POS_AFN      14      //Ӧ�ò㹦����
    #define POS_SEQ      15      //֡����
    #define POS_DATA     16      //�û�������ʼ��ַ
#else
#define POS_MSA      11      //��վ��ַ�Լ����ַ��־
#define POS_AFN      12      //Ӧ�ò㹦����
#define POS_SEQ      13      //֡����
#define POS_DATA     14      //�û�������ʼ��ַ
#endif


#define    PN_RTUA          0x10     //�ն�
#define    PN_METER         0x20     //������
#define    PN_AGT           0x30     //�ܼ����
#define    PN_SIMULATE      0x40     //ֱ��ģ����

#define    LOCK_HOUR        0x01     //ʱ����
#define    LOCK_DAY         0x02     //�ն���
#define    LOCK_MONTH       0x03     //�¶���
#define    LOCK_METERDAY    0x04     //�����ն���
#define    LOCK_CURVE       0x05     //����

//�¼����
#define ERC1   1
#define ERC2   2
#define ERC3   3
#define ERC4   4
#define ERC5   5
#define ERC6   6
#define ERC7   7
#define ERC8   8
#define ERC9   9
#define ERC10  10
#define ERC11  11
#define ERC12  12
#define ERC13  13
#define ERC14  14
#define ERC15  15
#define ERC16  16
#define ERC17  17
#define ERC18  18
#define ERC19  19
#define ERC20  20
#define ERC21  21
#define ERC22  22
#define ERC23  23
#define ERC24  24
#define ERC25  25
#define ERC26  26
#define ERC27  27
#define ERC28  28
#define ERC29  29
#define ERC30  30
#define ERC31  31
#define ERC32  32
#define ERC33  33
#define ERC34  34
#define ERC35  35
#define ERC36  36 //�㽭��չΪ�ϱ�ʧ�ܵ��
#define ERC37  37 //��ɫ����չ
#define ERC38  38 //�㽭��չ�ϱ��ɼ���δ֪���
#define ERC39  39
#define ERC40  40 //���𱨾�
#define ERC41  41
#define ERC42  42
#define ERC43  43
#define ERC44  44 
#define ERC45  45
#define ERC49  49 //����ز�ʹ�õ� �豸�����¼���¼
#define ERC51  51
#define ERC52  52
#define ERC53  53
#define ERC54  54
#define ERC55  55
#define ERC56  56
#define ERC57  57
#define ERC58  58
#define ERC60  60
#define ERC61  61  //�Ϻ�ʱ��ƫ���¼�
#define ERC62  62  //̽�Ⳮ���¼�
#define ERC63  63  //��̨�����ܱ���Ϣ
#define ERC64  64  //���յ����¼�

#define ERC65  65  //�Ϻ�ר��ʧѹ�¼�
#define ERC66  66  //�Ϻ�ר����ӿ��ű������¼�
#define ERC67  67
#define ERC68  68

//ESAM��֤�������
#define HERR_OK            0  
#define HERR_SIGNATURE     1     //ǩ��У�����
#define HERR_CRYPTO        2     //����У�����
#define HERR_SYNMAC        3     //�Գ�MAC��֤ʧ��

//===========Ӧ�ò㹦�ܱ��� AFN_XXX============================

#define AFN_ACK            0x00     //ȷ��/���� �Լ��ն˶�����������ݲ��߱���Ӧ�����ķ��ϻش�
#define AFN_RESET          0x01     //��λ����
#define AFN_TEST           0x02     //��·�ӿڼ��,�����ڽ������紫���ŵ�
#define AFN_FEP            0x03     //�м�վ����
#define AFN_SET            0x04     //���ò���
#define AFN_CTRL           0x05     //��������
#define AFN_AUTH           0x06     //��ԿЭ��
#define AFN_ASK_SLAVE      0x08     //���󱻼����ն������ϱ�
#define AFN_READCFG        0x09     //�����ն�����
#define AFN_QUERY          0x0A     //��ѯ����
#define AFN_READTASK       0x0B     //������������
#define AFN_READCUR        0x0C     //����1�����ݣ�ʵʱ���ݣ�
#define AFN_READHIS        0x0D     //����2�����ݣ���ʷ���ݣ� 
#define AFN_READEVENT      0x0E     //����3�����ݣ��¼����ݣ�  
#define AFN_FTP            0x0F     //�ļ�����
#define AFN_REMOTE_UPDATE_NM      0x13     //���� Զ��������ʽ2
#define AFN_RELAY          0x10     //����ת��
#define AFN_PRODUCT_TEST   0x15     //���������չ
#define AFN_ASK_SLAVE_NEXT 0x18     //����������ն��ϱ�����(��Ϊ���նˣ�Ӧ�õȴ��ñ��ĺ���ܼ�����������)
#define AFN_PATCH_READHIS  0x1d     //���� 2������
#define AFN_DEBUG          0xFE     //���ҵ���





#define FUNC_MASK_CLIENT    0x80       //���ô��ͷ���
#define FUNC_MASK_DENY      0x40       //���÷񶨱�־

#define CTRLFUNC_SET_DIR        0x80       //���ô��䷽��λDIR=1    ʹ��|
#define CTRLFUNC_CLR_DIR        0x7F       //���ô��䷽��λDIR=0    ʹ��&
#define CTRLFUNC_CLR_PRM        0xBF       //����������־λPRM=0    ʹ��&
#define CTRLFUNC_SET_PRM        0x40       //����������־λPRM=1    ʹ��|
#define CTRLFUNC_CLR_ACD        0xDF       //����֡Ҫ�����λACD=0  ʹ��&
#define CTRLFUNC_SET_ACD        0x20       //����֡Ҫ�����λACD=1  ʹ��|
#define CTRLFUNC_CLR_FCB        0xDF       //����֡����λFCB=0      ʹ��&
#define CTRLFUNC_SET_FCB        0x20       //����֡����λFCB=1      ʹ��|
#define CTRLFUNC_CLR_FCV        0xEF       //����֡������ЧλFCV=0      ʹ��&
#define CTRLFUNC_SET_FCV        0x10       //����֡������ЧλFCV=1      ʹ��|






//�����붨��
//PRM=1 ֡������������վ,����վ�����ķ���վ���Ӷ�վ����Ӧ�ˡ�

//��λ����   ����/ȷ��
#define PRM_CTRLFUNC_RESET  1

//�û�����   ����/�޻ش�
#define PRM_CTRLFUNC_USERDATA  4

//��·����   ����/��Ӧ֡     ����/��Ӧ������˵�ù�����Ҫ�дӶ�վ����Ӧ���ġ�
#define PRM_CTRLFUNC_TEST    9

//����1������   ����/��Ӧ֡
#define PRM_CTRLFUNC_CURDATA    10

//����2������   ����/��Ӧ֡
#define PRM_CTRLFUNC_HISDATA    11


//PRM=0 ֡�������ԴӶ�վ

//�Ͽ�  ȷ��
#define CTRLFUNC_CONFIRM  0

//�û�����  ��Ӧ֡
#define CTRLFUNC_USERDATA  8

//���ϣ������ٻ�������  ��Ӧ֡
#define CTRLFUNC_NODATA   9

//��·״̬   ��Ӧ֡
#define CTRLFUNC_TEST    11





//֡ʱ���ǩ��Чλ
#define MASK_TpV   0x80

//��֡��־λ
#define MASK_FIR   0x40

//ĩ֡��־λ
#define MASK_FIN   0x20

//����ȷ�ϱ�־λ
#define MASK_CON   0x10



//��Ϣ��
#define DT_F0   0x0000

#define DT_F1   0x0001
#define DT_F2   0x0002
#define DT_F3   0x0004
#define DT_F4   0x0008
#define DT_F5   0x0010
#define DT_F6   0x0020
#define DT_F7   0x0040
#define DT_F8   0x0080

#define DT_F9   0x0101
#define DT_F10  0x0102
#define DT_F11  0x0104
#define DT_F12  0x0108
#define DT_F13  0x0110
#define DT_F14  0x0120
#define DT_F15  0x0140
#define DT_F16  0x0180


#define DT_F17  0x0201
#define DT_F18  0x0202
#define DT_F19  0x0204
#define DT_F20  0x0208
#define DT_F21  0x0210
#define DT_F22  0x0220
#define DT_F23  0x0240
#define DT_F24  0x0280


#define DT_F25  0x0301
#define DT_F26  0x0302
#define DT_F27  0x0304
#define DT_F28  0x0308
#define DT_F29  0x0310
#define DT_F30  0x0320
#define DT_F31  0x0340
#define DT_F32  0x0380


#define DT_F33  0x0401
#define DT_F34  0x0402
#define DT_F35  0x0404
#define DT_F36  0x0408
#define DT_F37  0x0410
#define DT_F38  0x0420
#define DT_F39  0x0440
#define DT_F40  0x0480

#define DT_F41  0x0501
#define DT_F42  0x0502
#define DT_F43  0x0504
#define DT_F44  0x0508
#define DT_F45  0x0510
#define DT_F46  0x0520
#define DT_F47  0x0540
#define DT_F48  0x0580


#define DT_F49  0x0601
#define DT_F50  0x0602
#define DT_F51  0x0604
#define DT_F52  0x0608
#define DT_F53  0x0610
#define DT_F54  0x0620
#define DT_F55  0x0640
#define DT_F56  0x0680


#define DT_F57  0x0701
#define DT_F58  0x0702
#define DT_F59  0x0704
#define DT_F60  0x0708
#define DT_F61  0x0710
#define DT_F62  0x0720
#define DT_F63  0x0740
#define DT_F64  0x0780

#define DT_F65  0x0801
#define DT_F66  0x0802
#define DT_F67  0x0804
#define DT_F68  0x0808
#define DT_F69  0x0810
#define DT_F70  0x0820
#define DT_F71  0x0840
#define DT_F72  0x0880

#define DT_F657 0x0805
#define DT_F668 0x080A


#define DT_F73  0x0901
#define DT_F74  0x0902
#define DT_F75  0x0904
#define DT_F76  0x0908
#define DT_F77  0x0910
#define DT_F78  0x0920
#define DT_F79  0x0940
#define DT_F80  0x0980


#define DT_F81  0x0A01
#define DT_F82  0x0A02
#define DT_F83  0x0A04
#define DT_F84  0x0A08
#define DT_F85  0x0A10
#define DT_F86  0x0A20
#define DT_F87  0x0A40
#define DT_F88  0x0A80

#define DT_F89  0x0B01
#define DT_F90  0x0B02
#define DT_F91  0x0B04
#define DT_F92  0x0B08
#define DT_F93  0x0B10
#define DT_F94  0x0B20
#define DT_F95  0x0B40
#define DT_F96  0x0B80


#define DT_F97   0x0C01
#define DT_F98   0x0C02
#define DT_F99   0x0C04
#define DT_F100  0x0C08
#define DT_F101  0x0C10
#define DT_F102  0x0C20
#define DT_F103  0x0C40
#define DT_F104  0x0C80

#define DT_F105  0x0D01
#define DT_F106  0x0D02
#define DT_F107  0x0D04
#define DT_F108  0x0D08
#define DT_F109  0x0D10
#define DT_F110  0x0D20
#define DT_F111  0x0D40
#define DT_F112  0x0D80

#define DT_F113  0x0E01
#define DT_F114  0x0E02
#define DT_F115  0x0E04
#define DT_F116  0x0E08
#define DT_F117  0x0E10
#define DT_F118  0x0E20
#define DT_F119  0x0E40
#define DT_F120  0x0E80

#define DT_F121  0x0F01
#define DT_F122  0x0F02
#define DT_F123  0x0F04
#define DT_F124  0x0F08
#define DT_F125  0x0F10
#define DT_F126  0x0F20
#define DT_F127  0x0F40
#define DT_F128  0x0F80

#define DT_F129  0x1001
#define DT_F130  0x1002
#define DT_F131  0x1004
#define DT_F132  0x1008
#define DT_F133  0x1010
#define DT_F134  0x1020
#define DT_F135  0x1040
#define DT_F136  0x1080


#define DT_F137  0x1101
#define DT_F138  0x1102
#define DT_F139  0x1104
#define DT_F140  0x1108
#define DT_F141  0x1110
#define DT_F142  0x1120
#define DT_F143  0x1140
#define DT_F144  0x1180


#define DT_F145  0x1201
#define DT_F146  0x1202
#define DT_F147  0x1204
#define DT_F148  0x1208
#define DT_F149  0x1210
#define DT_F150  0x1220
#define DT_F151  0x1240
#define DT_F152  0x1280

#define DT_F153  0x1301
#define DT_F154  0x1302
#define DT_F155  0x1304
#define DT_F156  0x1308
#define DT_F157  0x1310
#define DT_F158  0x1320
#define DT_F159  0x1340
#define DT_F160  0x1380


#define DT_F161  0x1401
#define DT_F162  0x1402
#define DT_F163  0x1404
#define DT_F164  0x1408
#define DT_F165  0x1410
#define DT_F166  0x1420
#define DT_F167  0x1440
#define DT_F168  0x1480

#define DT_F169  0x1501
#define DT_F170  0x1502
#define DT_F171  0x1504
#define DT_F172  0x1508
#define DT_F173  0x1510
#define DT_F174  0x1520
#define DT_F175  0x1540
#define DT_F176  0x1580


#define DT_F177  0x1601
#define DT_F178  0x1602
#define DT_F179  0x1604
#define DT_F180  0x1608
#define DT_F181  0x1610
#define DT_F182  0x1620
#define DT_F183  0x1640
#define DT_F184  0x1680

#define DT_F185  0x1701
#define DT_F186  0x1702
#define DT_F187  0x1704
#define DT_F188  0x1708
#define DT_F189  0x1710
#define DT_F190  0x1720
#define DT_F191  0x1740
#define DT_F192  0x1780

#define DT_F193  0x1801
#define DT_F194  0x1802
#define DT_F195  0x1804
#define DT_F196  0x1808
#define DT_F197  0x1810
#define DT_F198  0x1820
#define DT_F199  0x1840
#define DT_F200  0x1880


#define DT_F201  0x1901
#define DT_F202  0x1902
#define DT_F203  0x1904
#define DT_F204  0x1908
#define DT_F205  0x1910
#define DT_F206  0x1920
#define DT_F207  0x1940
#define DT_F208  0x1980



#define DT_F209  0x1A01
#define DT_F210  0x1A02
#define DT_F211  0x1A04
#define DT_F212  0x1A08
#define DT_F213  0x1A10
#define DT_F214  0x1A20
#define DT_F215  0x1A40
#define DT_F216  0x1A80



#define DT_F217  0x1B01
#define DT_F218  0x1B02
#define DT_F219  0x1B04
#define DT_F220  0x1B08
#define DT_F221  0x1B10
#define DT_F222  0x1B20
#define DT_F223  0x1B40
#define DT_F224  0x1B80


#define DT_F225  0x1C01
#define DT_F226  0x1C02
#define DT_F227  0x1C04
#define DT_F228  0x1C08
#define DT_F229  0x1C10
#define DT_F230  0x1C20
#define DT_F231  0x1C40
#define DT_F232  0x1C80

#define DT_F233  0x1D01
#define DT_F234  0x1D02
#define DT_F235  0x1D04
#define DT_F236  0x1D08
#define DT_F237  0x1D10
#define DT_F238  0x1D20
#define DT_F239  0x1D40
#define DT_F240  0x1D80

#define DT_F241  0x1E01
#define DT_F242  0x1E02
#define DT_F243  0x1E04
#define DT_F244  0x1E08
#define DT_F245  0x1E10
#define DT_F246  0x1E20
#define DT_F247  0x1E40
#define DT_F248  0x1E80

#define DT_F249  0x1F01
#define DT_F250  0x1F02
#define DT_F251  0x1F04
#define DT_F252  0x1F08
#define DT_F253  0x1F10
#define DT_F254  0x1F20
#define DT_F255  0x1F40
#define DT_F256  0x1F80

#define DT_F296  0x2480
#define DT_F297  0x2501
#define DT_F298  0x2502
#define DT_F299  0x2504
#define DT_F300  0x2508
#define DT_F301  0x2510
#define DT_F302  0x2520
#define DT_F303  0x2540
#define DT_F304  0x2580

#define DT_F305  0x2601
#define DT_F306  0x2602
#define DT_F307  0x2604
#define DT_F308  0x2608
#define DT_F309  0x2610
#define DT_F310  0x2620
#define DT_F311  0x2640
#define DT_F312  0x2680

#define DT_F313  0x2701
#define DT_F314  0x2702
#define DT_F315  0x2704
#define DT_F316  0x2708
#define DT_F317  0x2710
#define DT_F318  0x2720
#define DT_F319  0x2740
#define DT_F320  0x2780

#define DT_F321  0x2801
#define DT_F322  0x2802
#define DT_F324  0x2808
#define DT_F325  0x2810

#define DT_F400  0x3180
#define DT_F401  0x3201
#define DT_F402  0x3202

#define DT_F701  0x5710
#define DT_F702  0x5720

#define DT_F796  0x6308
#define DT_F797  0x6310
#define DT_F798  0x6320
#define DT_F799  0x6340

#define DT_F801  0x6401
#define DT_F802  0x6402

#define DT_F2040  0xFE80

#define DT_F2048  0xFF80
//��Ϣ��
#define DA_P0  0x0000

#define DA_P1  0x0101
#define DA_P2  0x0102
#define DA_P3  0x0104
#define DA_P4  0x0108
#define DA_P5  0x0110
#define DA_P6  0x0120
#define DA_P7  0x0140
#define DA_P8  0x0180


#define DA_P9   0x0201
#define DA_P10  0x0202
#define DA_P11  0x0204
#define DA_P12  0x0208
#define DA_P13  0x0210
#define DA_P14  0x0220
#define DA_P15  0x0240
#define DA_P16  0x0280

#define DA_P17  0x0401
#define DA_P18  0x0402
#define DA_P19  0x0404
#define DA_P20  0x0408
#define DA_P21  0x0410
#define DA_P22  0x0420
#define DA_P23  0x0440
#define DA_P24  0x0480







//=============================================================================
//  �����������
//=============================================================================
#define  EXEC_SUCESS            0x00   //�����ɹ�
#define  EXEC_BADDATA           0x01   //���ݳ���
#define  EXEC_BADPWD            0x02   //�����
#define  EXEC_INVALID           0x03   //����Ч����
#define  EXEC_BADVER            0x04   //�ն˰汾���
#define  EXEC_BUSY              0xC0   //æ

#define  EXEC_ROUTER_NORESP     0xF1   //·��������Ӧ



//Ĭ�Ϸ��� ���Ҫ��û������ʱ���ʲ���д0
#if defined(__PROVICE_TIANJIN__)
    #define DEFAULT_FL       0x01        
#elif defined (__PROVICE_ANHUI__) || defined(__PROVICE_JIANGSU__) || defined(__PROVICE_HEILONGJIANG__) 
    #define DEFAULT_FL       0x04
#else
    #define DEFAULT_FL       0x00
#endif

//�㽭�̱����
#define PLC_NET_TIME1        10   //60   //15         //�̱�ʱ��15����
#define PLC_NET_TIME2        20         //�̱�ʱ��120����
#define PLC_NET_INTERVAL1    3          //���1����
#define PLC_NET_INTERVAL2    10         //���10����
#define PLC_NET_INTERVAL3    17         //���17����
#define PLC_NET_INTERVAL4    240        //���240����


//�������  __POWER_CTRL__     //����
#define ERTU_CTRLTYPE_TIMESEG    1        //ʱ�ο�
#define ERTU_CTRLTYPE_RELAX      2        //���ݿ�
#define ERTU_CTRLTYPE_STOPWORK   3        //��ͣ��
#define ERTU_CTRLTYPE_FLOATDOWN  4        //�¸���
#define ERTU_CTRLTYPE_BUYFEE     5        //����ѿ�
#define ERTU_CTRLTYPE_MONTHFEE   6        //�µ�ѿ�
#define ERTU_CTRLTYPE_REMOTE     7        //ң��
#define ERTU_CTRLTYPE_PROTECT    8        //����
#define ERTU_BUYFEE_ALARM        9        //�߷Ѹ澯

#define ERTU_INACT               0x01    //Ͷ��
#define ERTU_WARNING             0x02    //�澯
#define ERTU_SWITCH_OFF          0x04    //��բ
#define ERTU_DISMISS             0x08    //���
#define ERTU_SWITCH_ON           0x10    //��բ

#define LED_SW1_RED     0x0001
#define LED_SW2_RED     0x0002
#define LED_SW3_RED     0x0004
#define LED_SW4_RED     0x0008
#define LED_PCTRL   0x0010
#define LED_FCTRL   0x0020
#define LED_PROTECT 0x0040

#define LED_SW1_GREEN 0x0100
#define LED_SW2_GREEN 0x0200
#define LED_SW3_GREEN 0x0400
#define LED_SW4_GREEN 0x0800


//��������ģʽ����2011.3.6
#define ONLINE_ALLTIME						0x01				//��������
#define ONLINE_ACTIVE					        0x02				//��������
#define ONLINE_TIME 							0x03				//ʱ������

//���ݳ���ʱ����д��Ĭ��ֵ
#ifdef __PROVICE_JIANGSU__
#define REC_DATA_IS_DENY					0xEE				//�����ط���
#define REC_DATA_IS_DEFAULT					0xEF				//����������,Ҳ����Ĭ����д
#else
#define REC_DATA_IS_DENY					0xEE				//�����ط���
#define REC_DATA_IS_DEFAULT					0xEE				//����������,Ҳ����Ĭ����д
#endif  //__PROVICE_JIANGSU__

#define SMS_ALLOW_REPORT					0x01	//ʹ�ܶ��ŷ����ϱ�
#define SMS_ALLOW_TASK_REPORT				0x02    //�����ϱ�����
#define SMS_REPORT_EVENT					0x04    //��ǰ�ϱ��¼�
#define SMS_REPORT_TASK						0x08    //��ǰ�ϱ�����
#define SMS_SAVE_DEBUG_INFO				0x10	  //�洢���ŵ�����Ϣ,����F221
#define SMS_EVENT_USE						0x20    //�洢�����¼��ϱ���־������F221		
#define SMS_TASK_USE						0x40    //�洢�����¼��ϱ���־������F221		
#define SMS_USE								0x80    //ʹ�ܶ��Ź��ܣ�����F221
#define ETH_CONNECT_SUCCESS_TIME			20     //Ԥ����̫�����ӳ�ʱ��
//#define HASH_FUN_COUNT                 16     //��ϣ�㷨���������ڸ��ݱ�����ɴ洢λ��

//GPRS ����ʧ��״̬
#define GPRS_NORMAL					        0		//����
#define GPRS_NOMODEL					        1		//��ģ��
#define GPRS_NOCARD						     2		//�޿�
#define GPRS_NONET						     3		//������
#define GPRS_DIALFAIL					     4		//����ʧ��
#define GPRS_CONNECTFAIL				     5		//δ����

//��ʾˢ������
#define LCD_HEADER					        1		//״̬��
#define LCD_WORKAREA					        2		//��������
#define LCD_FOOTER						     4		//��ʾ��

//GPRS״̬
#define	GPRS_NONE       					0x0000 
#define	GPRS_SOCKET_OPEN					0x0001 
#define	GPRS_SOCKET_CLOSE					0x0002
#define  GPRS_SOCKET_CONNECTED              0x0004
#define	GPRS_CHECK_LOGIN					0x0008 
#define	GPRS_LOGON							0x0010 
#define	GPRS_PPP_OPEN						0x0020 
#define	GPRS_PPP_CLOSE						0x0040

//ETH״̬
#define	ETH_NONE       					    0x0000 
#define	ETH_SOCKET_OPEN					    0x0001 
#define	ETH_SOCKET_CLOSE					0x0002 
#define  ETH_SOCKET_CONNECTED               0x0004
#define	ETH_CHECK_LOGIN					    0x0008 
#define	ETH_LOGON							0x0010 


#define	REMOTE_MODEL_NONE					0x1F 
#define	REMOTE_MODEL_AT					    0x0F 
#define	REMOTE_MODEL_NOTAT					0x07 

//�ϱ������ն���ÿ֡���ݸ���
#define ENCRYPT_NONE  0
#define ENCRYPT_TOPS1  1
#define ENCRYPT_TOPS2  2

//��ѹ���ӹ���
#define MAX_V_MONITOR_COUNT                  10       //��ѹ�����б����ֵ����Сֵ�ĸ���
#define VIP_V_MONITOR_CYCLE                  1        //�ص��ѹ���Ӳ����㳭����ѹ�����ڣ���λ��Сʱ

#define LOG_RECORD_MASK1				0x8000		  //��־��¼�����λ1����ʾ��¼�ĸ�����Ϣ����¼������ǰһ����־������Ŀ����������־����Ǹ�����Ϣ

//�˳����ɱ��ʱ��
#if defined(__PROVICE_JILIN__) || defined(__PROVICE_JILIN_FK__)
#define CYMETER_NOTE_TIME                    480       //�˳����ɱ��ʱ��
#else
#define CYMETER_NOTE_TIME                    240        //�˳����ɱ��ʱ��
#endif

#define PORT_FUNC_RS485              0x01     //����485��
#define PORT_FUNC_CYMETER            0x02     //����485��
#define PORT_FUNC_CASCADE            0x03     //����485��

#define GPRS_TO_RS232                       0x01       //GPRSת����
#define PPP_TO_RS232                        0x02       //PPPת����
#define RS485_TO_RS232                      0x04       //485ת����
#define CYMETER_TO_RS232                    0x08       //����ת����
#define INFRA_TO_RS232                      0x10       //�����ת����
#define PLC_TO_RS232                        0x20       //�ز���ת����
#define RS232_TO_RS4852                     0x40       //����ת������
#define RS232_TO_CYMETER                    0x80       //����ת���ɿ�


#define DEBUG_3761							0x01	   //����386.1����
#define DEBUG_CYMETER						0x02	   //���潻�ɱ���

#define FAST_IDX_ADD_ROUTER_FLAG   0x8000        //�Ƿ����·�ɱ�־
#define FAST_IDX_RTU_FLAG          0x4000        //�ɼ�����־    //__zy_set_meter__
#define FAST_IDX_METER_FLAG        0x2000        //�������вɼ��ն��µ���־λ
#define FAST_IDX_DZC_METER_FLAG    0x1000        //�Ƿ�Ϊ�ɼ��ն���ˮ���ȱ�
#define FAST_IDX_MASK              0x0FFF        //����������

#define CYCLE_REC_MODE_CONCENTRATOR        1     //���������������ڳ���ģʽ
#define CYCLE_REC_MODE_ROUTER              2     //·�����������ڳ���ģʽ
#define CYCLE_REC_MODE_ALL                 3     //���ֶ�֧��
#define CYCLE_REC_MODE_PARALLEL            10     //�����������Ĳ��г���ģʽ

#define CAST_DELAY_UNIT_1        1   //1:1s  2:0.1s
#define CAST_DELAY_UNIT_2        2   //1:1s  2:0.1s


/*����������*/
#define  HARD_RESET     0x01                 /*Ӳ������*/
#define  DATA_RESET     0x02                 /*��������ʼ��*/
#define  PARAMS_RESET   0x03                 /*��������ʼ��*/
#define  CY_CLEAR       0x04                 /*��������*/

#define CT  0x01
#define PT  0x02


#define CONST_CFG_DEVID				1UL			//6	�ն˵�ַ
#define CONST_CFG_VENDOR			2UL			//4	���̴���
#define CONST_VENDOR_CHIP			3UL			//4	�ն˳��ң�ɽ����
#define CONST_VENDOR_CHIP2			4UL			//2оƬ���ң�ɽ����
#define CONST_SHANGHAI_VENDOR_FLAG	5UL			//2	�汾��ʶ���Ϻ���
#define CONST_SOFT_VER				6UL			//4+1	����汾��
#define CONST_SOFT_DATE				7UL			//3+1	����汾��������
#define CONST_CAPACITY				8UL			//11+1	������Ϣ��
#define CONST_PROTOCOL_VER			9UL			//4+1	Э��汾��
#define CONST_HARDWARE_VER			10UL		//4+1	Ӳ���汾��
#define CONST_HARDWARE_DATE			11UL		//3+1	Ӳ���汾��������
#define CONST_ERTU_SWITCH           12UL             //4+1  �ն˵ı��뿪��
#define CONST_LOG_PARAM             13UL               //2+1����־���������ʽ
#define CONST_DISPLAY_PWD           14UL         //8����ʾ����
#define CONST_RS232_BAUDRATE        15UL         //4�����ڲ�����
#define CONST_FRAME_MONITOR         16UL         //2�����ļ��
#define CONST_RS232_REPORT          17UL         //1�������ϱ�,AA-��ͨ���� BB-GPRS����	����-���ô����ϱ�
#define CONST_GPRS_BAUDRATE         18UL         //4��GPRS���ڲ�����,Ĭ��115200
#define CONST_INFRA_BAUDRATE        19UL         //4������ڲ�����
#define CONST_MNG485_BAUDRATE       20UL         //4��485ά���ڲ�����
#define CONST_GPRS_LOCK		        21UL         //1��GPRS��Ƶ
#define CONST_PROVINCE_FEATURE      22UL         //8,ʡ��������3�ֽ�ʡ��+2�ֽ�����+3�ֽڱ���
#define CONST_REMOTE_TYPE           23UL         //1,Զ��ͨ��ѡ��,AA-GPRS BB-��̫�� ����-����Ӧ
#define CONST_MES                   24UL         //16,����MES�ţ��弶���ʱ���ã����ñ���
#define CONST_ERTU_PLMS_SWITCH      25UL         //4���ն˵Ĳ���������
#define CONST_230M_CHANNEL          29UL         //1��I��ר��230Ƶ������ֵ��Χ1~16
#define CONST_GPRS_AUTH_TYPE        33UL         //1����Ȩ���ͣ�55-pap��AA-chap������Ĭ��
#define CONST_DZC_METER_SWTICH      34UL         //3��ˮ���ȱ�Э���л���3���ֽ�BCD�ֱ���ˮ���ȱ�Э���
#define CONST_DZC_CJT_SWTICH        35UL         //2����ת����ͨѶЭ���л�
#define CONST_DZC_485_BAUDRATE      36UL         //4���ı�485��ת����ͨѶ������
#define CONST_ERTU_SWITCH_HUBEI_F801  37UL       //1����F801�����л�
#define CONST_CONFIG_ONENET         45UL         //1=2 1�ֽ� 00/FF Ϊ���� ����Ϊר��
#define CONST_SWITCH_PARAM          25UL         //4 Ĭ�ϲ������ÿ��� D0����������645����֡ ����645Э��
#define CONST_DJB_VER               99UL         //�����汾��Ϣ��16+1
//0~99 Ϊϵͳʹ�ã����й���ͳһ����
//100����ΪӦ�ó���ʹ�ã�����ֻ����376.1��
#define CONST_POWERON_PROTECT_TIME  100UL        //1376.1����ʹ�ã��Ĵ�������ϵ�ʱ�Զ�����ʱ�䣬��λ15min��0��FFΪ��Ч
//����ID���壬ϵͳʹ�� 0~999��Ӧ�ò�ʹ��1000~9999
//���ﶨ��Ӧ�ò�����񹷣���1000��ʼ


#define CHANNEL_REMOTE  0x01
#define CHANNEL_RS232   0x02
//#define CHANNEL_INFRA   0x04
#define CHANNEL_FLASH   0x08
#define CHANNEL_STACK   0x10
#define CHANNEL_ETH     0x20
#define CHANNEL_SMS     0x40
#define CHANNEL_NONE    0x80
//����ͨ�������ã�����û�а�λʹ�ã����������ͨ��������λ����
#define CHANNEL_GPRS_SERVER    0x81
#define CHANNEL_GPRS_CLIENT    0x82
#define CHANNEL_ETH_SERVER    0x83
#define CHANNEL_ETH_CLIENT     0x84
#define CHANNEL_BEIDOU        0x85

#define  TIME_OUT_C1    6000  //��λ 10ms
#define  MAX_METER_COUNT  2000
/*************
 * ת����������
 **************/
//ת������
#define RATE                    0x0F
#define REC_DATE                0x10	//  �ն�ʱ��-> 376.1 A.15
#define REC_DATE_SEC            0x11
#define MeterPowerStatus_3761FMT 0x12 //376.1 F161 ���ܱ�ͨ�ϵ�״̬
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
#define Energy_3761FMT14_BLOCK  0x31   //�����й����ݿ�ת��
#define METER_STATUS            0x32   //���״̬�ֱ�λ




//�ڲ����� �궨��
#define ZDCB_SJ_XN              0x80000001
#define FL_S_XN                 0x80000002
#define XB_CS_XN                0x80000003
#define XSDJ_SJ_SB_XN           0x80000004
#define RDJ_SJ_SB_XN            0x80000005
#define YDJ_SJ_SB_XN            0x80000006
#define QX_SJ_SB_XN             0x80000007
#define DBCD_XX_XN              0x80000008
#define DBYX_ZTZ_BWBZ_XN        0x80000009
#define ZBB_DY_XJ_XN            0x8000000A
#define DX_BC_XN                0x8000000B
#define YM_JS_DNL_XN            0x8000000C
#define ABCN_I_CURVE            0x8000000E     //ABCN��������ߣ���ת�������ɵ���Ϊ2�ֽ�  
#define ABC_DUANXIANG_COUNT     0x8000000F     //ABC������������ת��������Ҫ��Ϊ2�ֽ�
#define FK2005_XSDJ_SJ_MD_XN    0x80000010      //����2005Сʱ���ᣬʱ�����ܶ�ռһ�ֽ�
#define METER_ZH_CURVE          0x80000011      //���ܱ��ۺ��������ߣ���ת������
#define ZXYG_REC_STATE_CURVE    0x80000012      //�����й��ܼ�����״̬��Ϣ���ߣ����������й��ܵ�ֵ�ֶ����״ֵ̬
#define JILIN_CURVE             0x80000013      //�������Ӱ��ֵ
#define CHECK_IS_HAVE_EE        0x80000014      //�ж��Ƿ���EE ֻҪ����Ч���� fn����ΪȫEE ���Ĵ����� 
#define DXB_XLQL_EE             0x80000015      //�ж��Ƿ���EE ֻҪ����Ч���� fn����ΪȫEE ��������̨�壩




#define NOT_BCD                 0x8000
#define NOT_FL_DATA             0x01
#define NOT_SINGLE              0x08 //�����֧�ֵ�����


#define LEN_ERTU_CFG_VERINFO        41

#define HUANBAO_JIBEI          00000001 //����������645Э������ �����½����֡



#endif




