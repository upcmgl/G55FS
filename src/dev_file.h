#ifndef __DEV_FILE_H__
#define __DEV_FILE_H__

#define  FILE_SUCCESS                  0x00
#define  FILE_ERROR_SIZE               0x01
#define  FILE_ERROR_UNKOWN             0x02
#define  FALSE_SECTOR_BITS             (4096)

#define FILEID_ERTU_NAME				0xABCD   //�����ļ��ı���?
#define FILEID_ERTU						0
#define  FILEID_TEST					1
//#define  FILEID_TEST2					1
/*
 * �洢�滮
 * �洢����sector���滮��һ��sectorΪ4K������flashΪ4Mbyte��һ��1024��sector
 * �Ѵ洢����sector����ʹ�ã����ַ�����С����������sector
 * 
 */

//sector ��Χ 0~1023
//0~89  ����ʹ��
//90~101 ����ʹ�ã�ʹ��˫����ȡ���»��ƣ�90~93��4��sector��Ų���ʼ����������������2��sector��4000*2=8000����94~101,8��sector�����ɳ�ʼ����������������4��sector��4000*4=16000��

/*�������֣������ʶ512byte�����������С0x56200��0x200�ļ�ͷ��Ϣ+0x56000���򣩣�512�ֽ�ͷ��Ϣ+344k����
 * һ����Ҫ��С345KByte  87��sector
 * Ԥ��3��sector
 * ��������һ����Ҫ90��sector
 * MX ��������Ϊ512.5K���ܹ���Ҫ133��sector
 */
#define FLASH_SECTOR_UPDATE_START             0

#define FLASH_SECTOR_UPDATE_END               (129)

//�������� G55 ��һ��sector��ʼ��ǰ512�ֽڴ��Header����������Ŵ��롣G55һ��512K�ֽ�Flash��NorFlash��Ԥ�� 129*4096=516K�ֽ�
#define FLASH_SECOTR_UPDATE_DATA_START        (0)
#define FLASH_SECOTR_UPDATE_DATA_EDN          (128)
//������Ϣ G55 �ŵ���������һ��sector
#define FLASH_SECTOR_UPDATE_INFO              (129)

#define FLADDR_PROG_INFO_START                (0UL)   //����������Ϣ��ʼ��ַ
#define FLADDR_PROG_INFO_FLAG                 FLADDR_PROG_INFO_START            //32=16+16,16�ֽڱ�ʶ��4�ֽ�����������12�ֽ�Ԥ����Ϣ
#define FLADDR_PROG_FILE_TYPE                 (FLADDR_PROG_INFO_FLAG+32)          //2,�ļ�����
#define FLADDR_PROG_TOTAL_SEC_COUNT           (FLADDR_PROG_FILE_TYPE+2)         //2�ֽڣ��ܶ���
#define FLADDR_PROG_ONE_SEC_SIZE              (FLADDR_PROG_TOTAL_SEC_COUNT+2)  //2�ֽڣ�ÿ�γ���
#define FLADDR_PROG_FILE_SIZE                 (FLADDR_PROG_ONE_SEC_SIZE+2)  //4�ֽڣ��ļ���С
#define FLADDR_PROG_FRAME_LEN                 (FLADDR_PROG_FILE_SIZE+4)        //2�ֽڣ�֡����
#define FLADDR_PROG_END_FRAME_LEN             (FLADDR_PROG_FRAME_LEN+2)        //2�ֽڣ����һ֡����
#define FLADDR_UPDATE_TIME                    (FLADDR_PROG_END_FRAME_LEN+20)              //6,������ʼʱ��
#define FLADDR_PROG_DOWNLOAD_RECORD           (FLADDR_UPDATE_TIME+6)           //512�ֽڣ���λ��ʾÿ�����ر�־��1δ���أ�0�����أ��ɱ�ʾ4096���Σ����һ����   1024�ֽڿɱ�ʾ4M��512�ֽڣ��ɱ�ʾ2M��256�ֽڣ�1M

#define FLADDR_PROG_INFO_END                  (4095)  //������Ϣ������ַ


/*�����洢����
 *�洢�滮������˫����ȡ���»��ƣ�ÿ��sector4096��ǰ96byte�������ã�4000�ֽ�������
 * ���ɳ�ʼ������8000,2��sector��С��ʵ��ռ��4��sector
 * �ɳ�ʼ������16000,4��sector��С��ʵ��ռ��8��sector
 */
#define FLASH_SECTOR_DATA_SIZE                4000UL   //flashһ��sector��ŵ����ݴ�С��һ��sector������4096��ǰ��96��Ź�����Ϣ�����4000������
#define FLASH_SECTOR_ERTU_PARAM_START         (130)

//���ɳ�ʼ������4��
#define FLASH_SECTOR_ERTU_PARAM_NOINIT_START  (FLASH_SECTOR_ERTU_PARAM_START)
#define FLASH_SECOTR_ERTU_PARAM_NOINIT_END    (FLASH_SECTOR_ERTU_PARAM_START+3)

#define FLASH_SECTOR_ERTU_PARAM_INIT_START  (FLASH_SECOTR_ERTU_PARAM_NOINIT_END+1) //
#define FLASH_SECTOR_ERTU_PARAM_INIT_END    (FLASH_SECTOR_ERTU_PARAM_INIT_START+7) //

#define FLASH_SECTOR_ERTU_PARAM_END           (FLASH_SECTOR_ERTU_PARAM_INIT_END)

#define FLASH_ERTU_PARAM_MAX_LENTH            ((FLASH_SECTOR_ERTU_PARAM_END+1-FLASH_SECTOR_ERTU_PARAM_START)/2*FLASH_SECTOR_DATA_SIZE) 
#ifdef __13761__
//��������ERC�洢�滮
#define FLASH_ERTU_EVENT_ERC_START			(FLASH_SECTOR_ERTU_PARAM_END+1)
#define FLASH_ERTU_EVENT_ERC_VIP_START      (FLASH_ERTU_EVENT_ERC_START) //��Ҫ�ļ�10��sector
#define FLASH_ERTU_EVENT_ERC_VIP_END        (FLASH_ERTU_EVENT_ERC_VIP_START+9)
#define FLASH_ERTU_EVENT_ERC_GEN_START       (FLASH_ERTU_EVENT_ERC_VIP_END+1) //һ���ļ�10��sector
#define FLASH_ERTU_EVENT_ERC_GEN_END         (FLASH_ERTU_EVENT_ERC_GEN_START+9)
#define FLASH_ERTU_EVENT_ERC_STORE_START    (FLASH_ERTU_EVENT_ERC_GEN_END+1)  //��������F330+10��sector��
#define FLASH_ERTU_EVENT_ERC_STORE_END      (FLASH_ERTU_EVENT_ERC_STORE_START+EVENT_ERC_SECTOR_MAX-1) //EVENT_ERC_SECTOR_MAX��
//#define FLASH_ERTU_EVENT_ERC_STORE_LENGTH    ((FLASH_ERTU_EVENT_ERC_STORE_END+1-FLASH_ERTU_EVENT_ERC_VIP_START)/2*FLASH_SECTOR_DATA_SIZE)
#endif

#ifdef __EDP__   //�洢edpδ�ϱ��ɹ������ݡ�
#define  FLASH_EDP_STORT_DATA_START            (500)
#define  FLASH_EDP_STORT_DATA_SECTOR_CNT        (10)
#define  FLASH_EDP_STORT_DATA_START_END        (FLASH_EDP_STORT_DATA_START+FLASH_EDP_STORT_DATA_SECTOR_CNT)
#define  FLASH_EDP_RAND_REPORT_STORE_START            (FLASH_EDP_STORT_DATA_START_END) //  ��510��ʼ�洢�������ϱ����ݡ�
#define  FLASH_EDP_RAND_REPORT_STORE_CNT			(5)
#define  FLASH_EDP_RAND_REPORT_STORE_END           (FLASH_EDP_RAND_REPORT_STORE_START + FLASH_EDP_RAND_REPORT_STORE_CNT)
#endif 

#ifdef __OOP_G_METER__
#define PAGE_ID_DATA_BUFFER               (490)    //���ݰ��ƻ���ҳ
    #define PAGE_COUNT_DATA_BUFFER         1
/* 	������ݴ洢��ʼҳ */
#define FLASH_SECTOR_METER_DATA_START      500    //���￪ʼ�ǵ��������  

#define PAGE_ID_METER_DATA_START           FLASH_SECTOR_METER_DATA_START    
#define MAX_ONE_METER_DATA_PAGE_COUNT      475    //��������ռ�õ�ҳ��

#ifdef __PROVINCE_JIBEI_FUHEGANZHI__ //�������ɸ�֪��Ŀ
#define  ELECTRICAL_EQUIPMENT_NUM 30
#define  SECTOR_ELECTRICAL_EQUIPMENT_MINUTE_DATA_START  250
#define  MAX_SECTOR_ELECTRICAL_EQUIPMENT_MINUTE_DATA      2   //���ڼ������ɸ�֪��Ŀ��ÿ���豸��20��sector������֧����10���豸
#define  SECTOR_ELECTRICAL_EQUIPMENT_MINUTE_DATA_END    SECTOR_ELECTRICAL_EQUIPMENT_MINUTE_DATA_START+MAX_SECTOR_ELECTRICAL_EQUIPMENT_MINUTE_DATA*ELECTRICAL_EQUIPMENT_NUM
#endif
/* ������ݴ洢ҳ���嶨�� 
 * ===============================================================================
 */
//������ݴ洢�滮��ÿ�����ռ��MAX_ONE_METER_DATA_PAGE_COUNTҳ--------------------------------------------------------------------------------------
//���ʵʱ����
#define PAGE_ID_CUR_DL_1                  0          //10ҳ�� 
    #define PAGE_COUNT_CUR_DL_1          2
#define PAGE_ID_CUR_DL_2            (PAGE_ID_CUR_DL_1 + PAGE_COUNT_CUR_DL_1)       //���ݿ鰴80�ֽڼ��㣬ʹ��2ҳ�洢����1�������ڿ�ʹ��18��
    #define PAGE_COUNT_CUR_DL_2          2
#define PAGE_ID_CUR_DL_3            (PAGE_ID_CUR_DL_2 + PAGE_COUNT_CUR_DL_2)
    #define PAGE_COUNT_CUR_DL_3          2
#define PAGE_ID_CUR_DL_4            (PAGE_ID_CUR_DL_3 + PAGE_COUNT_CUR_DL_3)
    #define PAGE_COUNT_CUR_DL_4          2
#define PAGE_ID_CUR_DL_5            (PAGE_ID_CUR_DL_4 + PAGE_COUNT_CUR_DL_4)
    #define PAGE_COUNT_CUR_DL_5          2
#define PAGE_ID_CUR_DL_6            (PAGE_ID_CUR_DL_5 + PAGE_COUNT_CUR_DL_5)
    #define PAGE_COUNT_CUR_DL_6          2
#define PAGE_ID_CUR_DL_7            (PAGE_ID_CUR_DL_6 + PAGE_COUNT_CUR_DL_6)
    #define PAGE_COUNT_CUR_DL_7          2

#define PAGE_ID_CUR_XL_1            (PAGE_ID_CUR_DL_7 + PAGE_COUNT_CUR_DL_7)   //14
    #define PAGE_COUNT_CUR_XL_1                2
#define PAGE_ID_CUR_XL_2            (PAGE_ID_CUR_XL_1 + PAGE_COUNT_CUR_XL_1)
    #define PAGE_COUNT_CUR_XL_2                2
#define PAGE_ID_CUR_XL_3            (PAGE_ID_CUR_XL_2 + PAGE_COUNT_CUR_XL_2)
    #define PAGE_COUNT_CUR_XL_3                2
#define PAGE_ID_CUR_XL_4            (PAGE_ID_CUR_XL_3 + PAGE_COUNT_CUR_XL_3)
    #define PAGE_COUNT_CUR_XL_4                2
#define PAGE_ID_CUR_XL_5            (PAGE_ID_CUR_XL_4 + PAGE_COUNT_CUR_XL_4)
    #define PAGE_COUNT_CUR_XL_5                2
#define PAGE_ID_CUR_XL_6            (PAGE_ID_CUR_XL_5 + PAGE_COUNT_CUR_XL_5)
    #define PAGE_COUNT_CUR_XL_6                2
#define PAGE_ID_CUR_XL_7            (PAGE_ID_CUR_XL_6 + PAGE_COUNT_CUR_XL_6)
    #define PAGE_COUNT_CUR_XL_7                2
#define PAGE_ID_CUR_XL_8            (PAGE_ID_CUR_XL_7 + PAGE_COUNT_CUR_XL_7)
    #define PAGE_COUNT_CUR_XL_8                2

#define PAGE_ID_VARIABLE_1            (PAGE_ID_CUR_XL_8 + PAGE_COUNT_CUR_XL_8)  //30
    #define PAGE_COUNT_VARIABLE_1           4
#define PAGE_ID_VARIABLE_2            (PAGE_ID_VARIABLE_1 + PAGE_COUNT_VARIABLE_1) //34
    #define PAGE_COUNT_VARIABLE_2           3
#define PAGE_ID_VARIABLE_3            (PAGE_ID_VARIABLE_2 + PAGE_COUNT_VARIABLE_2)  //37
    #define PAGE_COUNT_VARIABLE_3           3

#define PAGE_ID_PARAMS_1               (PAGE_ID_VARIABLE_3 + PAGE_COUNT_VARIABLE_3) //40
    #define PAGE_COUNT_PARAMS_1             2

//���������
#define PAGE_ID_HOLD_IDX                  (PAGE_ID_PARAMS_1 + PAGE_COUNT_PARAMS_1)      // 2ҳ,�������� //42
    #define PAGE_COUNT_HOLD_IDX                   2                 // 2ҳ����������

#define PAGE_ID_DAY_HOLD_1                (PAGE_ID_HOLD_IDX + PAGE_COUNT_HOLD_IDX)       //�ն���  //44
    #define PAGE_COUNT_DAY_HOLD_1                 3
#define PAGE_ID_DAY_HOLD_2                (PAGE_ID_DAY_HOLD_1 + PAGE_COUNT_DAY_HOLD_1)    //47
    #define PAGE_COUNT_DAY_HOLD_2                 8
#define PAGE_ID_DAY_HOLD_3                (PAGE_ID_DAY_HOLD_2 + PAGE_COUNT_DAY_HOLD_2)   //55
    #define PAGE_COUNT_DAY_HOLD_3                 5

// �¶������� 
#define PAGE_ID_MONTH_HOLD_1                (PAGE_ID_DAY_HOLD_3 + PAGE_COUNT_DAY_HOLD_3)   //60
    #define PAGE_COUNT_MONTH_HOLD_1               2
#define PAGE_ID_MONTH_HOLD_2                (PAGE_ID_MONTH_HOLD_1 + PAGE_COUNT_MONTH_HOLD_1) //62
    #define PAGE_COUNT_MONTH_HOLD_2               2
#define PAGE_ID_MONTH_HOLD_3                (PAGE_ID_MONTH_HOLD_2 + PAGE_COUNT_MONTH_HOLD_2)  //64
    #define PAGE_COUNT_MONTH_HOLD_3               2    

// Сʱ����     
#define PAGE_ID_HOUR_HOLD                 (PAGE_ID_MONTH_HOLD_3 + PAGE_COUNT_MONTH_HOLD_3)   //Сʱ���� Ŀǰ������Сʱ���ᣬ***��ʱ�ȿ���***//66
    #define PAGE_COUNT_HOUR_HOLD                  3
//���Ӷ���
#define PAGE_ID_MINUTE_HOLD               (PAGE_ID_HOUR_HOLD + PAGE_COUNT_HOUR_HOLD)   //15���Ӷ���     ����7��  //69
    #define PAGE_COUNT_MINUTE_HOLD                21

#define PAGE_ID_ACCOUNT_DAY_HOLD_1        (PAGE_ID_MINUTE_HOLD + PAGE_COUNT_MINUTE_HOLD)     //�����ն���  // 90
    #define PAGE_COUNT_ACCOUNT_DAY_HOLD_1          2
#define PAGE_ID_ACCOUNT_DAY_HOLD_2        (PAGE_ID_ACCOUNT_DAY_HOLD_1 + PAGE_COUNT_ACCOUNT_DAY_HOLD_1)   //92
    #define PAGE_COUNT_ACCOUNT_DAY_HOLD_2          2

#define PAGE_ID_TIME_ZONE_HOLD            (PAGE_ID_ACCOUNT_DAY_HOLD_2 + PAGE_COUNT_ACCOUNT_DAY_HOLD_2)  //ʱ�����л�  //94
    #define PAGE_COUNT_TIME_ZONE_HOLD              2

#define PAGE_ID_DAY_SEG_HOLD              (PAGE_ID_TIME_ZONE_HOLD + PAGE_COUNT_TIME_ZONE_HOLD)  //��ʱ�α��л�   //94
    #define PAGE_COUNT_DAY_SEG_HOLD                2

#define PAGE_ID_FL_PRICE_HOLD             (PAGE_ID_DAY_SEG_HOLD + PAGE_COUNT_DAY_SEG_HOLD)   //���ʵ����
    #define PAGE_COUNT_FL_PRICE_HOLD               2

#define PAGE_ID_LADDER_HOLD               (PAGE_ID_FL_PRICE_HOLD + PAGE_COUNT_FL_PRICE_HOLD)  //�����л�
    #define PAGE_COUNT_LADDER_HOLD                 2

//EVENT���ݶ���
#define PAGE_ID_EVENT_READ_STATE                 (PAGE_ID_LADDER_HOLD + PAGE_COUNT_LADDER_HOLD)       //2ҳ,3320,�¼��Ŀɶ���
    #define PAGE_COUNT_EVENT_READ_STATE                  2                 //2ҳ,3320,�¼��Ŀɶ���

#define PAGE_ID_EVENT_IDX                        (PAGE_ID_EVENT_READ_STATE + PAGE_COUNT_EVENT_READ_STATE)       //2ҳ����������
    #define PAGE_COUNT_EVENT_IDX                         2                 //2ҳ����������

#define PAGE_ID_EVENT_CUR_DATA_1                 (PAGE_ID_EVENT_IDX + PAGE_COUNT_EVENT_IDX)
    #define PAGE_COUNT_EVENT_CUR_DATA_1                  2
#define PAGE_ID_EVENT_CUR_DATA_2                 (PAGE_ID_EVENT_CUR_DATA_1 + PAGE_COUNT_EVENT_CUR_DATA_1)
    #define PAGE_COUNT_EVENT_CUR_DATA_2                  2

//ʧѹ��Ƿѹ����ѹ������Ϊһ��
#define PAGE_ID_EVENT_3000_REOCRD_DATA           (PAGE_ID_EVENT_CUR_DATA_2 + PAGE_COUNT_EVENT_CUR_DATA_2)                   //ʧѹ *3
    #define PAGE_COUNT_EVENT_3000_REOCRD_DATA          2
#define PAGE_ID_EVENT_3001_REOCRD_DATA           (PAGE_ID_EVENT_3000_REOCRD_DATA + PAGE_COUNT_EVENT_3000_REOCRD_DATA*3)     //Ƿѹ *3
    #define PAGE_COUNT_EVENT_3001_REOCRD_DATA          2
#define PAGE_ID_EVENT_3002_REOCRD_DATA           (PAGE_ID_EVENT_3001_REOCRD_DATA + PAGE_COUNT_EVENT_3001_REOCRD_DATA*3)     //��ѹ *3
    #define PAGE_COUNT_EVENT_3002_REOCRD_DATA          2
#define PAGE_ID_EVENT_3003_REOCRD_DATA           (PAGE_ID_EVENT_3002_REOCRD_DATA + PAGE_COUNT_EVENT_3002_REOCRD_DATA*3)     //���� *3
    #define PAGE_COUNT_EVENT_3003_REOCRD_DATA          2

//ʧ��������������Ϊһ��
#define PAGE_ID_EVENT_3004_REOCRD_DATA           (PAGE_ID_EVENT_3003_REOCRD_DATA + PAGE_COUNT_EVENT_3003_REOCRD_DATA*3)     //ʧ�� *3
    #define PAGE_COUNT_EVENT_3004_REOCRD_DATA          2
#define PAGE_ID_EVENT_3005_REOCRD_DATA           (PAGE_ID_EVENT_3004_REOCRD_DATA + PAGE_COUNT_EVENT_3004_REOCRD_DATA*3)     //���� *3
    #define PAGE_COUNT_EVENT_3005_REOCRD_DATA          2
#define PAGE_ID_EVENT_3006_REOCRD_DATA           (PAGE_ID_EVENT_3005_REOCRD_DATA + PAGE_COUNT_EVENT_3005_REOCRD_DATA*3)     //���� *3
    #define PAGE_COUNT_EVENT_3006_REOCRD_DATA          2

#define PAGE_ID_EVENT_3007_REOCRD_DATA           (PAGE_ID_EVENT_3006_REOCRD_DATA + PAGE_COUNT_EVENT_3006_REOCRD_DATA*3)     //���������¼�
    #define PAGE_COUNT_EVENT_3007_REOCRD_DATA          2

//�й��������ޡ��޹���������Ϊһ��
#define PAGE_ID_EVENT_3009_REOCRD_DATA           (PAGE_ID_EVENT_3007_REOCRD_DATA + PAGE_COUNT_EVENT_3007_REOCRD_DATA)     //�й����������¼�
    #define PAGE_COUNT_EVENT_3009_REOCRD_DATA          2
#define PAGE_ID_EVENT_300A_REOCRD_DATA           (PAGE_ID_EVENT_3009_REOCRD_DATA + PAGE_COUNT_EVENT_3009_REOCRD_DATA)     //�й����������¼�
    #define PAGE_COUNT_EVENT_300A_REOCRD_DATA          2
#define PAGE_ID_EVENT_300B_REOCRD_DATA           (PAGE_ID_EVENT_300A_REOCRD_DATA + PAGE_COUNT_EVENT_300A_REOCRD_DATA)     //�޹����������¼� * 4
    #define PAGE_COUNT_EVENT_300B_REOCRD_DATA          2

#define PAGE_ID_EVENT_300C_REOCRD_DATA           (PAGE_ID_EVENT_300B_REOCRD_DATA + PAGE_COUNT_EVENT_300B_REOCRD_DATA*4)   //�ܹ��������������¼�
    #define PAGE_COUNT_EVENT_300C_REOCRD_DATA          2

#define PAGE_ID_EVENT_300D_REOCRD_DATA           (PAGE_ID_EVENT_300C_REOCRD_DATA + PAGE_COUNT_EVENT_300C_REOCRD_DATA)     //ȫʧѹ�¼�
    #define PAGE_COUNT_EVENT_300D_REOCRD_DATA          2

//��ѹ�����򡢵��������򡢹���Ϊһ��
#define PAGE_ID_EVENT_300F_REOCRD_DATA           (PAGE_ID_EVENT_300D_REOCRD_DATA + PAGE_COUNT_EVENT_300D_REOCRD_DATA)     //��ѹ������
    #define PAGE_COUNT_EVENT_300F_REOCRD_DATA          2
#define PAGE_ID_EVENT_3010_REOCRD_DATA           (PAGE_ID_EVENT_300F_REOCRD_DATA + PAGE_COUNT_EVENT_300F_REOCRD_DATA)       //����������
    #define PAGE_COUNT_EVENT_3010_REOCRD_DATA          2
#define PAGE_ID_EVENT_3008_REOCRD_DATA           (PAGE_ID_EVENT_3010_REOCRD_DATA + PAGE_COUNT_EVENT_3010_REOCRD_DATA)       //����  *3
    #define PAGE_COUNT_EVENT_3008_REOCRD_DATA          2

//������Դʧ�硢����Ϊһ��
#define PAGE_ID_EVENT_300E_REOCRD_DATA           (PAGE_ID_EVENT_3008_REOCRD_DATA + PAGE_COUNT_EVENT_3008_REOCRD_DATA*3)     //������Դʧ���¼�
    #define PAGE_COUNT_EVENT_300E_REOCRD_DATA          2
#define PAGE_ID_EVENT_3011_REOCRD_DATA           (PAGE_ID_EVENT_300E_REOCRD_DATA + PAGE_COUNT_EVENT_300E_REOCRD_DATA)     //�����¼�
    #define PAGE_COUNT_EVENT_3011_REOCRD_DATA          2

#define PAGE_ID_EVENT_3012_REOCRD_DATA           (PAGE_ID_EVENT_3011_REOCRD_DATA + PAGE_COUNT_EVENT_3011_REOCRD_DATA)     //���ܱ����¼�
    #define PAGE_COUNT_EVENT_3012_REOCRD_DATA          2

#define PAGE_ID_EVENT_3013_REOCRD_DATA           (PAGE_ID_EVENT_3012_REOCRD_DATA + PAGE_COUNT_EVENT_3012_REOCRD_DATA)     //���ܱ������¼�
    #define PAGE_COUNT_EVENT_3013_REOCRD_DATA          2

#define PAGE_ID_EVENT_3014_REOCRD_DATA           (PAGE_ID_EVENT_3013_REOCRD_DATA + PAGE_COUNT_EVENT_3013_REOCRD_DATA)     //���ܱ����������¼�
    #define PAGE_COUNT_EVENT_3014_REOCRD_DATA          2

#define PAGE_ID_EVENT_3015_REOCRD_DATA           (PAGE_ID_EVENT_3014_REOCRD_DATA + PAGE_COUNT_EVENT_3014_REOCRD_DATA)     //���ܱ��¼������¼�
    #define PAGE_COUNT_EVENT_3015_REOCRD_DATA          2
	
#define PAGE_ID_EVENT_3016_REOCRD_DATA           (PAGE_ID_EVENT_3015_REOCRD_DATA + PAGE_COUNT_EVENT_3015_REOCRD_DATA)     //���ܱ�Уʱ�¼�
    #define PAGE_COUNT_EVENT_3016_REOCRD_DATA          2
	
#define PAGE_ID_EVENT_3017_REOCRD_DATA           (PAGE_ID_EVENT_3016_REOCRD_DATA + PAGE_COUNT_EVENT_3016_REOCRD_DATA)     //���ܱ�ʱ�α���¼�
    #define PAGE_COUNT_EVENT_3017_REOCRD_DATA          2

#define PAGE_ID_EVENT_3018_REOCRD_DATA           (PAGE_ID_EVENT_3017_REOCRD_DATA + PAGE_COUNT_EVENT_3017_REOCRD_DATA)     //���ܱ�ʱ������¼�
    #define PAGE_COUNT_EVENT_3018_REOCRD_DATA          2

#define PAGE_ID_EVENT_3019_REOCRD_DATA           (PAGE_ID_EVENT_3018_REOCRD_DATA + PAGE_COUNT_EVENT_3018_REOCRD_DATA)     //�����ձ���¼�
    #define PAGE_COUNT_EVENT_3019_REOCRD_DATA          2

#define PAGE_ID_EVENT_301A_REOCRD_DATA           (PAGE_ID_EVENT_3019_REOCRD_DATA + PAGE_COUNT_EVENT_3019_REOCRD_DATA)     //�����ձ���¼�
    #define PAGE_COUNT_EVENT_301A_REOCRD_DATA          2

//����ǡ�����ť��Ϊһ��
#define PAGE_ID_EVENT_301B_REOCRD_DATA           (PAGE_ID_EVENT_301A_REOCRD_DATA + PAGE_COUNT_EVENT_301A_REOCRD_DATA)     //������¼�
    #define PAGE_COUNT_EVENT_301B_REOCRD_DATA          2
#define PAGE_ID_EVENT_301C_REOCRD_DATA           (PAGE_ID_EVENT_301B_REOCRD_DATA + PAGE_COUNT_EVENT_301B_REOCRD_DATA)     //����Ŧ���¼�
    #define PAGE_COUNT_EVENT_301C_REOCRD_DATA          2
	
#define PAGE_ID_EVENT_301D_REOCRD_DATA           (PAGE_ID_EVENT_301C_REOCRD_DATA + PAGE_COUNT_EVENT_301C_REOCRD_DATA)     //��ѹ��ƽ��
    #define PAGE_COUNT_EVENT_301D_REOCRD_DATA          2
	
//������ƽ�⡢�������ز�ƽ��Ϊһ��	
#define PAGE_ID_EVENT_301E_REOCRD_DATA           (PAGE_ID_EVENT_301D_REOCRD_DATA + PAGE_COUNT_EVENT_301D_REOCRD_DATA)     //������ƽ��
    #define PAGE_COUNT_EVENT_301E_REOCRD_DATA          2
#define PAGE_ID_EVENT_302D_REOCRD_DATA           (PAGE_ID_EVENT_301E_REOCRD_DATA + PAGE_COUNT_EVENT_301E_REOCRD_DATA)     //�������ز�ƽ��
    #define PAGE_COUNT_EVENT_302D_REOCRD_DATA          2

//��բ����բΪһ��
#define PAGE_ID_EVENT_301F_REOCRD_DATA           (PAGE_ID_EVENT_302D_REOCRD_DATA + PAGE_COUNT_EVENT_302D_REOCRD_DATA)     //��բ�¼�
    #define PAGE_COUNT_EVENT_301F_REOCRD_DATA          2
#define PAGE_ID_EVENT_3020_REOCRD_DATA           (PAGE_ID_EVENT_301F_REOCRD_DATA + PAGE_COUNT_EVENT_301F_REOCRD_DATA)     //��բ�¼�
    #define PAGE_COUNT_EVENT_3020_REOCRD_DATA          2

#define PAGE_ID_EVENT_3021_REOCRD_DATA           (PAGE_ID_EVENT_3020_REOCRD_DATA + PAGE_COUNT_EVENT_3020_REOCRD_DATA)     //���ܱ�ڼ��ձ���¼�
    #define PAGE_COUNT_EVENT_3021_REOCRD_DATA          2

#define PAGE_ID_EVENT_3022_REOCRD_DATA           (PAGE_ID_EVENT_3021_REOCRD_DATA + PAGE_COUNT_EVENT_3021_REOCRD_DATA)     //���ܱ��й���Ϸ�ʽ����¼�
    #define PAGE_COUNT_EVENT_3022_REOCRD_DATA          2

#define PAGE_ID_EVENT_3023_REOCRD_DATA           (PAGE_ID_EVENT_3022_REOCRD_DATA + PAGE_COUNT_EVENT_3022_REOCRD_DATA)     //���ܱ��޹���Ϸ�ʽ����¼�
    #define PAGE_COUNT_EVENT_3023_REOCRD_DATA          2

#define PAGE_ID_EVENT_3024_REOCRD_DATA           (PAGE_ID_EVENT_3023_REOCRD_DATA + PAGE_COUNT_EVENT_3023_REOCRD_DATA)     //���ܱ���ʲ�������¼�
    #define PAGE_COUNT_EVENT_3024_REOCRD_DATA          2

#define PAGE_ID_EVENT_3025_REOCRD_DATA           (PAGE_ID_EVENT_3024_REOCRD_DATA + PAGE_COUNT_EVENT_3024_REOCRD_DATA)     //���ܱ���ݱ����¼�
    #define PAGE_COUNT_EVENT_3025_REOCRD_DATA          2

#define PAGE_ID_EVENT_3026_REOCRD_DATA           (PAGE_ID_EVENT_3025_REOCRD_DATA + PAGE_COUNT_EVENT_3025_REOCRD_DATA)     //���ܱ���Կ�����¼�
    #define PAGE_COUNT_EVENT_3026_REOCRD_DATA          2

#define PAGE_ID_EVENT_3028_REOCRD_DATA           (PAGE_ID_EVENT_3026_REOCRD_DATA + PAGE_COUNT_EVENT_3026_REOCRD_DATA)     //���ܱ����¼
    #define PAGE_COUNT_EVENT_3028_REOCRD_DATA          2

#define PAGE_ID_EVENT_302A_REOCRD_DATA           (PAGE_ID_EVENT_3028_REOCRD_DATA + PAGE_COUNT_EVENT_3028_REOCRD_DATA)     //�㶨�ų������¼�
    #define PAGE_COUNT_EVENT_302A_REOCRD_DATA          2

#define PAGE_ID_EVENT_302B_REOCRD_DATA           (PAGE_ID_EVENT_302A_REOCRD_DATA + PAGE_COUNT_EVENT_302A_REOCRD_DATA)     //���ɿ��������¼�
    #define PAGE_COUNT_EVENT_302B_REOCRD_DATA          2

#define PAGE_ID_EVENT_302C_REOCRD_DATA           (PAGE_ID_EVENT_302B_REOCRD_DATA + PAGE_COUNT_EVENT_302B_REOCRD_DATA)     //��Դ�쳣�¼�
    #define PAGE_COUNT_EVENT_302C_REOCRD_DATA          2


#define PAGE_ID_METER_DATA_END                   (PAGE_ID_EVENT_302C_REOCRD_DATA + PAGE_COUNT_EVENT_302C_REOCRD_DATA)     //������ݽ���page_id ����ʹ����220ҳ��

#endif
#define  MAX_METER_SEQ      1
//һ���¼���¼�ļ�
#define FILEID_EVENT_GEN    (MAX_METER_SEQ+1)

//��Ҫ�¼���¼�ļ�
#define FILEID_EVENT_VIP    (MAX_METER_SEQ+2)

//�����ϱ����Ļ����ļ�
#define FILEID_FRAMES       (MAX_METER_SEQ+3)

//������Ϣ�ļ�:���ձ��λ�ý��д�ţ������Դ��16����ÿ�����202�ֽڡ�
//�洢λ�� offset = idx*202
#define FILEID_NOTE_INFO    (MAX_METER_SEQ+4)


//�ص㻧�����ļ���
#define FILEID_VIP_METER    (MAX_METER_SEQ+5)



//1�����������ļ�
// �ļ����û����� USER���û�С�� TYPE ���з����洢��ÿ�����ռ��32�ֽڡ�
// ��Ϣ������ n         (0-31)
// ��1-n��Ϣ��Ԫ��־λ

#define FILEID_USERSET_C1   (MAX_METER_SEQ+25)

//2�����������ļ�
#define FILEID_USERSET_C2   (MAX_METER_SEQ+26)
#define MAX_485METER_COUNT		1
//485���������
#define DATAFILEID_485        (MAX_METER_SEQ+27)
#define FILEID_485_CUR        (DATAFILEID_485)                             //MAX_485METER_COUNT,��ǰ����
#define FILEID_485_DH         (DATAFILEID_485 + MAX_485METER_COUNT)        //MAX_485METER_COUNT,�ն�������
#define FILEID_485_CURVE      (DATAFILEID_485 + MAX_485METER_COUNT*2)      //MAX_485METER_COUNT,������˲ʱ����������
#define FILEID_485_MH         (DATAFILEID_485 + MAX_485METER_COUNT*3)      //MAX_485METER_COUNT,�¶�������
#define FILEID_485_CURVE_YWG  (DATAFILEID_485 + MAX_485METER_COUNT*4)      //MAX_485METER_COUNT,���������������޹�������ʾֵ����
#define FILEID_485_HARM_MAX_I (DATAFILEID_485 + MAX_485METER_COUNT*5)      //MAX_485METER_COUNT,г���������ֵͳ���ն�������
#define FILEID_485_HARM_MAX_V (DATAFILEID_485 + MAX_485METER_COUNT*6)      //MAX_485METER_COUNT,г����ѹ���ֵͳ���ն�������
#define FILEID_485_HARM_OVER  (DATAFILEID_485 + MAX_485METER_COUNT*7)      //MAX_485METER_COUNT,г��Խ��ͳ���ն�������

/*  
 * ===============================================================================
 */
//������ز����ӿ�
void delete_update_file(void);
INT16U fread_array_update_info(INT16U addr,INT8U *buf,INT16U len);
INT16U fwrite_array_update_info(INT16U addr,INT8U *buf,INT16U len);
INT16U fread_array_update_data(INT32U addr,INT8U *buf,INT16U len);
INT16U fwrite_array_update_data(INT32U addr,INT8U *buf,INT16U len);

//�ն˲�����ز����ӿ�
INT8U  fwrite_ertu_params(INT32U addr,INT8U *data,INT16U len);
INT8U  fread_ertu_params(INT32U addr,INT8U *data,INT16U len);
#ifdef __13761__
//��������ERC��ز����ӿ�
INT8U fwrite_erc_params(INT32U addr,INT8U *data,INT16U len);
INT8U fread_erc_params(INT32U addr,INT8U *data,INT16U len);
#endif
#endif