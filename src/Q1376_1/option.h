#ifndef __OPTION_H__
#define __OPTION_H__
//����ѡ�͵Ķ����ļ�
//GPRS��ģ��
#ifdef __GMETER__
#define STACK_UNUSE_ENC24J600
#define __ENABLE_INFRA__
#define __REPORT_NODE_EVENT__ //ȫ�¼�
#define __GMETER_CHECK__ //�������U�� usb.dat������ERC64
//#define __FIX_METERNUM_TO_ERTU_ADDR__ //�Ϻ����������Ϊ�ն˵�ַ
//#define __FIX_METERNUM_TO_ERTU_ADDR_JILIN__ //���ֵ��������Ϊ�ն˵�ַ
//#define __TEST_CTRL__ //�ز�תGPRS power_ctrl��ƽȡ�� �Ϻ�����GPRS��Ҫ��
//#define __CURVE__  //����ȡ���һ����Чֵ
//#define __HEBEI_HUANBAO__//�ӱ����������� 645���� ����FF+��ţ�����
//#define __APN32__
//#define __BEIJING_CURVE__
//#define __LVD_5V__
//#define __ZB2GPRS_485_NOT_OD__ //�ز�תGPRS 485ת�ӷ�ʽ ���ܿ�© ��ֱ�Ӻ͵��ӿڷ�ʽ��Ҫ��©��
#endif


//#define __SHANGHAI_WAIZHI__
#ifdef  __SHANGHAI_WAIZHI__
#define __FIX_METERNUM_TO_ERTU_ADDR__ //�Ϻ����������Ϊ�ն˵�ַ
#define __TEST_CTRL__ //�ز�תGPRS power_ctrl��ƽȡ�� �Ϻ�����GPRS��Ҫ��
#endif

#ifdef __HEBEI_HUANBAO__ 
#define __USE_DUAL_GPRS_CLIENT__
//#define __INFRA_DL645_READ__
#endif

#ifdef __PROVICE_JILIN__
//#define __REPORT_NODE_EVENT__ //ȫ�¼�
//#define __EVENT_04001501_BEFORE__
#endif

#ifdef __PROVICE_JIANGSU__ //����NBʹ��A9600
//#define __USE_EX_GPRS__ //Ŀǰֻ��A9600ʹ���ⲿЭ��  ���͹ܽſ�© 
//#define __GPRS_OD__    //A9600ģ�鷢�͹ܽſ�©
#endif

//��Դ�ɽڵ����� Ĭ��Ϊ2��
#define  __NOSRC_NODE_COUNT__  4

#define MAX_SW_TURN_COUNT     2
#define MAX_PULSE_PORT_COUNT  2

//ʵʱ��ȡ����F25����
//#define __READ_485_CY_DIRECT__

//һ������Ϊʵʱ��ȡ
#define __READ_C1DATA_VIA_MONITOR__ 

#define __ENABLE_CLOCK_FIX_PLC_REC__

//ʵʱ��ȡ485һ������
#define __READ_485_DIRECT__

//#define ENABLE_MSA_FRAME_CACHE //ʹ����վ���Ļ��湦��
#define MSA_FRAME_CACHE_VALID_SECONDS  600 //�����������վ������Чʱ�䣬��λs

//#define __BATCH_TRANSPARENT_METER_TASK__

#ifdef __POWER_CTRL__
  //�Զ������������
  #define __PULSE_METER_AUTO_ADD_F10__
  #undef __NOSRC_NODE_COUNT__
  #define __NOSRC_NODE_COUNT__ 2        //����ң��֧��2·
#endif

#define BAUDRATE_CAS_TO_CY   9600

#define __FILE_TRANSMIT_REVERSE__   //�ļ����䣬��ת��������

#define  BASE_STATION_CHANGE  //��Ƶ�����Ѿ�ͬ��Ϊ��3.0һ�£�����ȫ���򿪣����û��ڽ���ѡ���������
//#define __USE_SMS_CTRL_ALARM__    //ʹ�ö���֪ͨ���Ƹ澯��Ϣ

//#define __USE_DUAL_GPRS_CLIENT__  //ʹ��2��GPRS�ͻ��ˣ���֧����̫���Ŀͻ���

//#define __METER_EVENT_GRADE__       //�����¼��ȼ�����

//#define __REPORT_NODE_EVENT__       //��������¼������ϱ�

//#define __METER_CURVE_READ__    //�������߳���

//#define __PRECISE_TIME__      //��ȷ��ʱ

//Э�����ѡ��
//#define __PROVICE_YUNNAN__    //����ʡ(����F33��ͬ��1��������չ��ʵʱ������������2004��Լ)

//#define __PROVICE_HUBEI__       //����ʡ

//#define __PROVICE_GUIZHOU__       //����ʡ������2008

//#define __PROVICE_HUNAN__       //����ʡ���ص�����������㶳������,Ҫ����̨����������

//#define __PROVICE_JILIN__      //����ʡ(���ն���05060101,�ĳ�00010000)

//#define __PROVICE_SHANXI__       //ɽ�� Ҫ���ز������й�

//#define __PROVICE_TIANJIN__      //���

//#define __PROVICE_ZHEJIANG__     //�㽭

//#define __ZHEJIANG_EMERGENCY__     //�㽭 Ӧ������ �̱�ʱ���ز������뵵����������·�ɲ�ѯ�ڵ�Ĳ���

//#define __PROVICE_CHONGQING__    //����

//#define __PROVICE_XINJIANG__     //�½�

//#define __PROVICE_XINJIANGFK__     //�½�����

//#define __PROVICE_SICHUAN__     //�Ĵ�

//#define __PROVICE_SICHUAN_BOGAO__     //�Ĵ�����

//#define __DAYHOLD_GTOUP_REPEAT_TD__    //���������������·�ʱ���ж��ʱ��

//#define __PROVICE_HUBEI__     //����

//#define __PROVICE_FUJIAN__     //����

//#define __PROVICE_JIANGSU__     //����

//#define __JIANGSU_EXT_METER_PROTOCOL_11_12__     //����

//#define __PROVICE_GUANGXI__     //����

//#define __PROVICE_ANHUI__       //����

//#define __PROVICE_GANSU__       //����

//#define __PROVICE_SHANDONG__       //ɽ��

//#define __COUNTRY_ISRAEL__      //��ɫ��Ҫ��ʵʱ��������0000FF00:F129   ����APM���ݣ�

//#define __ISRAEL_CYCLE_REC__      //��ɫ�����ڳ�������F33�ĳ�����ʱ����г�����С���15���ӣ�����96���������

//#define __ISRAEL_HOUR_REC__       //�����ն��ᣬ��Сʱ���ᣬ0���Сʱ����ת���ն��ᣬ����������96���������

//#define __COUNTRY_SOUTH_AFRICA__   //�Ϸ�Ҫ����������ɫ����ͬ�������ն���ĳ�05063001

//#define __COUNTRY_IRAN__        //���ʣ��ڲ����չ������У���ʾ������������

//#define __COUNTRY_BRAZIL__        //����

//#define __ROADLAMP_CTRL__      //·�ƿ����� ��������·�ƿ��Ƶģ�

//#define __PLC_READ_VIP_F25__    //�ز������й��޹����ʡ���ѹ����

#define __PLC_READ_FXYG__    //�ز����������й�

#define __PLC_REC_CUR_WHEN_DAYHOLD_DENY__   //05060101������ʱ�������ʳ�����ǰ00010000,����ʳ���0001FF00,2013.8.6 ���а汾��

#define __PLC_REC_97_901F_DENY_9010__       //97�ز������ն���901F����ʱ���ĳ�9010

//#define __REDUCE_FLOW__    //��������������Ҫ��

//#define __TD_IS_TODAY__    //ʱ���ǵ��죬����ǰһ��,������ͨ��վ��ɫ

//#define __PROVICE_SHANGHAI__   //�Ϻ���

//#define __SHANGHAI_FOR_TEST_PASS__    //�Ϻ����ԣ�����1�����ݵ���ʱ���ԡ�

//#define __485_NET__    //485�������̱�

//#define __OPEN_PACKAGE_MONITOR__   //��������� ����

//#define __485_BIT_SEARCH__   //485��λ�ѱ�

//#define __SET_F25_METER_TYPE__   //���ݲ�������F25�������ǵ��ߺ����࣬�ز�����

//#define __PLC_READ_METER_TIME__    //�ز��������ʱ�ӣ�����ERC12

//#define  __ENABLE_ESAM__    //ʹ��ESAMģ��

//#define  __EN_CAP_CTRL__    //ʹ���޹�Ͷ�й���

//#define  __PLC_NET_F149_TYPE_1__     //�㽭�̱�ģʽ1 ���ȴ�+�̱��ʱ����3��10��17����
//#define  __PLC_NET_F149_TYPE_2__     //�㽭�̱�ģʽ2 ���ȴ�3��10��17���ӣ��̱�15����

//#define  __POWER_CTRL__     //����

//#define __TRANS_STATION__  //���վ������64��485������ó����ز����

#define  __STAT_LOADRATE__     //ͳ�Ƹ�����

#define __C1_DATA_FL_IS_4__    //����Ĭ��4���ʴ���

//#define  __DAYHOLD_F167__     //�ն�����ܱ��õ���Ϣ �洢93�������

//#define __PLC_REC_PLAN_FROM_F39__      //�ز�������F39�������г���

//#define __REC_DAY_HOLD_READ_SETTLEMENT_DAY_DATA__      //���������ݳ�����һ����������

//#define __COMPUTE_XLOST__        //��������

//#define  __TEMPERATURE_CURVE__   //�¶�����

//#define  __STAT_PLC_REC_FAIL__   //ͳ���ز�����ʧ��

//#define __TAIL_VOLTAGE_CURVE__    //�Ĵ�����ĩ�˵�ѹ����

//#define __PLC_REC_LOST_VI__      //�๦���ز�����ʧѹʧ����������͵��ܱ��̴���������洢10��

#define __CAST_NO_CONFIRM__      //���ַ��ȷ��

//define __VIP_METER_255__       //�ص��֧��255��

//defind __PLC_VIP_METER_REC_MODE_2__     //�ص������ʽ2

//defind __PLC_VIP_METER_REC_MODE_2_SUPPLEMENT_REC__     //�ص������ʽ2���������Сʱ����

//define __FTP_REMOTE_UPDATE__       //FtpԶ������  �Ϻ���չ

//define __PWLEN_IS_2BYTE__
//#define __RECONNECT_MSA_AFTER_CHANGE__   //�޸�ͨѶ�������Ƿ�����������վ

//#define __GB_645_YUNNAN__          //�������ϵ���Լ�������

//#define __GB_645_YUNNAN_CJQ_REC_2__   //�������ϵ���Լ�����ɼ������������������00010000,���򳭶�3�������

//#define __GB_645_YUNNAN_CHECK__       //�ͼ���ʱʹ�ã����ϵ���Լģʽ4ʱ�������ն���������05060101��05060201���¶��᳭����һ���������ݣ�����00010000�ķ��ϼ�¼

//#define __PROVICE_YUNNAN_PUER__      //------------------�����ն�

//#define __PROVICE_YUNNAN_TEST__      //���ϵ��Ժ��½̨���ͼ죬1��485����Ϊ00��FFʱ�����ڰ���1�������ã�2����������F26�е����г��޳���ʱ�䶼����1����ͳ��

//#define __PLC_REC_CJQ_EVENT__     //������ز������Ҳɼ�����ַ��Ч������Ҫ���ɼ����¼�

//#define __PLC_PRIOR_READ_C1DATA__      //�ز���Ԥ��һ������

//#define __PLC_PRIOR_READ_HIGH__        //�ز���Ԥ��һ�����ݣ�����ÿ�����ն���󳭶�������Ļ�����ȫ������ն��᳭��󣬲ų�

#define __PLC_PRIOR_READ_DATA_SAVE_FILE__   //�ز���Ԥ��һ�����ݱ������ļ�ϵͳ�У����򱣴���spansion��

//#define __PLC_PRIOR_READ_PER_HOUR__   //�ز���Ԥ��һ������ÿСʱÿСʱ����һ��

//#define __PLC_READ_LOAD_RECORD__       //�����ز��������ɼ�¼���ݣ���Ӧ����F89-F94,F103,F101,����60����ʱ����������10�������

//#define __NO_QGDW_TYPE__  //������Э�����ͣ�ͳͳ����376.1����

//#define __PLC_REC_CJQ_ERC_36_37__      //�ز�����ɼ�������erc36���ܱ�����¼���erc37

//#define __FORGE_METER_TIME__           //��վ������ʱ��ʱ�������г��������ؼ�������ʱ�䣬����ʹ��

#define __OTHER_DAYHOLDS_SAVE_SPOT_FLIE__ //�ز��๦�ܱ������ն������ݱ����ڲ������ļ��У��洢2��������

//�Ƿ������豸0�㸴λ
//#define __ALLOW_SELF_RESET_AT_ZERO__

//·��ѡ��, ����ʹ��iAD��·��Э��
//#define __ROUTER_iAD__

//#define __TCP_SERVER_MODE__		//tcp ������ģʽ
//#define __TCP_SERVER_CLIENT_MODE__  //tcp ���ģʽ
#define __REMOTE_ETH_PRIORITY__	 //��̫������ԭ�򣬲�����ΪGPRS����
#define __USE_SMS_HEAD__	//ʹ�ö���ͷ
#define __USE_SMS__			//ʹ�ö���
//#define __USE_FIXED_HEART_CYCLE__	//ʹ�ù̶���������

//#define __INFRA_DL645_READ__    //ͨ������ʹ��645�����������������
//#define __MAKE_CUTPOWER_EVENT__	//�����ʱ����ͣ���¼����Ĵ�����������Ҫ
#define __ENABLE_SYS_TOOLS__	//ϵͳ���Թ���
//#define __USE_2013_UPDATE__	//ʹ��2013Э���������


#define __ENABLE_CLOCK_FIX__   //ʹ��ʱ���޸�

#define __AUTO_SYS_INIT__       //ʹ���Զ���ʼ������
#define  __ROUTER_ADDR_MODE__   //·���ز����ڵ��ַ

#if defined(__ROUTER_iAD__)
  #define __ROUTER_iAD_DIRECT_RECORD__   //ֱ�ӳ���ģʽ
#endif


//����1�����ݵ����ȴ���ʱ����λ��
#define MAX_WAITTIME_OF_REQ_C1    60

#define __DEL_REPEAT_METERNO_AT_ADD__       //����±���ʱ���ԭ�����д�����ͬ��ţ���ɾ��ԭ����    

//�����������Һ����ʾ�����������º꣬�����ֹ����
#define __LCD__
#define __485_BAUDRATE_USE_DOC__
//#define __POWEROFF_NO_CHECK_CY__		//ͣ�ϵ粻��⽻��ͣ���¼��ֱ�������ն˵��縴λ

#define __METRE_DOC_INIT_OPTIMIZE__    //�������������ʼ��ʱ��ʼ�������㵵���Ż�����
#define __IMPORTANT_PARAM_BACKUP_RESTORE__   //��������������ָ���Ŀǰ֧���ն˻��������͵������ʹ�������ݣ�������ȡ��ԭ��
//#if defined __PROVICE_SHANGHAI__
//#elif defined __PROVICE_SHANDONG__
//#elif defined __PROVICE_ZHEJIANG__
//#else
#if defined __POWER_CTRL__
#define __READ_HENGTONG_METER__				 //��ͨ��Լ֧��
#define __READ_EDMI_METER__					 //�����Լ֧��
#endif
//����ʱָ��
//������ʾ����
//#define __DISPLAY_IN_CHINESE__
//#define __DISPLAY_QINGHAI__

//Ӣ����ʾ����
//#ifndef __DISPLAY_IN_CHINESE__
//#define __DISPLAY_IN_ENGLISH__
//#define __DISPLAY_IN_ENGLISH_GW__
//#endif


#if defined(__ZJWS__)

  #undef __TOPS__ 
  #undef __DISPLAY_IN_ENGLISH__
  #undef __PROVICE_YUNNAN__
  #undef __PROVICE_JILIN__
  #undef __DISPLAY_IN_ENGLISH_GW__
  #undef __DISPLAY_QINGHAI__

  //��������ͨ��ME3000ģ�� 
  #define __GPRS_ME3000__  
  #ifndef __DISPLAY_IN_CHINESE_ZJWS__
  #define __DISPLAY_IN_CHINESE_ZJWS__
  #endif
#endif //__ZJWS

#if defined(__DISPLAY_IN_CHINESE_ZJWS__)
  #ifndef __DISPLAY_IN_CHINESE__
  #define __DISPLAY_IN_CHINESE__
  #endif
#endif


//#define __QGDW_2013_STRUCT__   //�ڹ������涨��
//#define __REC_DH_485_EXT__
#ifdef __QGDW_2013_STRUCT__
#define __ENABLE_ESAM2__
#else
//#define __ENABLE_ESAM__
#endif

#ifdef __QGDW_376_2013_PROTOCOL__
    #define __376_2_2013__
    #define __FTP_REMOTE_UPDATE__
    #define __ERC_37__                         //erc37�����ܱ�����¼���¼
    #define __ERC_38__                         //erc38�����ܱ���ť���¼���¼
    #define __ERC_39__                         //ERC39������ʧ���¼���¼
    #define __ERC_40__                         //erc40���ų��쳣�¼���¼
   // #define __ERC_41_SHANGHAI__                //erc41����ʱ�¼���¼
    #ifndef __POWER_CTRL__
    #define __PLC_NET_F149__
    #define __PLC_NET_F149_TYPE_2__             //F149�̱�ģʽ
    #define __PLC_NET_NULL_RTU_ADD_DOC__        //�ղɼ�����������
    #define __TIMER_PLC_NET_MODE_1__            //�㽭ģʽ�ѱ�
    #define __PLC_PRIOR_READ_C1DATA__      //�ز���Ԥ��һ������
    #define __PLC_PRIOR_READ_DATA_SAVE_FILE__   //�ز���Ԥ��һ�����ݱ������ļ�ϵͳ�У����򱣴���spansion��
    #define __CONCENTRATOR_INITIATIVE__    //��������������ģʽ
    #define __MAX_SPOT_IDX_2040__          //���������֧��2040��
    #define __PLC_REC_PLAN_FROM_F39__      //�ز�������F39�������г���
    #endif
    #define __METER_DOC_F150__
    #define __PATCH_REC_DH__                    //����3���ն���
    //#define __VIP_PATCH_REC_DATA__              //�ص����07��Сʱ����
    #define __F39_DAYHOLD_F210__                    //�ն��Ṻ�õ���Ϣ
    //#define __DELETE_30_DAY_NO_REC_SPOT__       //ɾ��30��û�г����Ĳ�����
    #define __PLC_NET_ADD_FILE__                //�¹淶����F149 01ʱ��ӵ����������ѱ��ļ� 02����ļ������ӵ���
    #define __FILL_DEFAULT_C2_DATA__    //�����㲻����ʱ������2�������Ƿ����EE
    #define __CHECK_POWEROFF_NEW__		 //������ͣ�ϵ��¼�
    //#define __GW_NEW_EVENT_QUERY__       //�������¼������ѯ 
    #define __USE_2013_UPDATE__			 //ʹ��2013808��Э������
    #define __USE_DUAL_GPRS_CLIENT__//ʹ��2��GPRS�ͻ��ˣ���֧����̫���Ŀͻ���

//�����������Ӧ�ã����ӵ���Ŀ
/*
    #define __COMPUTE_XLOST__
    #define __USE_DUAL_GPRS_CLIENT__//ʹ��2��GPRS�ͻ��ˣ���֧����̫���Ŀͻ���
    #define __LINE_LOST_METER_OPEN__
    #define __PLC_READ_VIP_F25__           //�ص������ѹ�����������޹�����
	#define __PLC_READ_VIP_I__	
*/
#endif
#ifdef __QGDW_376_2009_PROTOCOL__
    #define __376_2_2013__
    #define __FTP_REMOTE_UPDATE__
    #ifndef __POWER_CTRL__
    #define __PLC_PRIOR_READ_C1DATA__      //�ز���Ԥ��һ������
    #define __PLC_PRIOR_READ_DATA_SAVE_FILE__   //�ز���Ԥ��һ�����ݱ������ļ�ϵͳ�У����򱣴���spansion��
    #endif
    #define __CONCENTRATOR_INITIATIVE__//�����������̷�������˹��������Ҫ��
    #define __FILL_DEFAULT_C2_DATA__    //�����㲻����ʱ������2�������Ƿ����EE
	#define __CHECK_POWEROFF_NEW__		 //������ͣ�ϵ��¼�
	#define __GW_NEW_EVENT_QUERY__       //�������¼������ѯ
    
    #define __REDUCE_485_READ_ITEM__  //����485�ĳ���������
//    #define __ROUTER_LME__ 
//    #define __METER_EVENT_GRADE__        //�����¼��ȼ�����
//    #define __METER_EVENT_GRADE_TEST__
//    #define __REPORT_NODE_EVENT__        //��������¼������ϱ�
//    #define __METER_CURVE_READ__         //�������߳���
#endif
#ifdef __QGDW_CHECK__
	#undef __485_BAUDRATE_USE_DOC__
	//#undef __CHECK_POWEROFF_NEW__		 //������ͣ�ϵ��¼�
	//#undef __GW_NEW_EVENT_QUERY__       //�������¼������ѯ
    #define __PLC_READ_VIP_F25__           //�ص������ѹ�����������޹�����
	#define __PLC_READ_VIP_I__				
    #define __METER_EVENT_GRADE__        //�����¼��ȼ�����
    #define __METER_EVENT_GRADE_TEST__
#ifndef __POWER_CTRL__
    #define __REPORT_NODE_EVENT__        //��������¼������ϱ�
#endif
    #define __METER_CURVE_READ__         //�������߳���
    
	#undef __ENABLE_SYS_TOOLS__
    #undef  __READ_HENGTONG_METER__				 //��ͨ��Լ֧��
	#undef  __READ_EDMI_METER__					 //�����Լ֧��


#endif
#ifdef __USE_ONE_FLASH__
    #undef __ENABLE_ESAM2__
#endif

#ifdef __QGDW_09_STRUCT__
	#ifdef __QGDW_2013_STRUCT__
	#undef __QGDW_2013_STRUCT__
	#endif
	#ifdef __ENABLE_ESAM2__
	#undef __ENABLE_ESAM2__
	#endif
	#undef __PLC_REC_PLAN_FROM_F39__			//Ĭ��ȡ����F39�������г���
    #undef __376_2_2013__						//ȡ��2013��376.2ģʽ
	//#undef __CHECK_POWEROFF_NEW__		 		//������ͣ�ϵ��¼�
	//#undef __GW_NEW_EVENT_QUERY__       		//�������¼������ѯ
#endif

#if defined(__PROVICE_NINGXIA__) && !defined(__QGDW_376_2013_PROTOCOL__)
	#define __CHECK_POWEROFF_NEW__		 //������ͣ�ϵ��¼�
	#define __GW_NEW_EVENT_QUERY__       //�������¼������ѯ
    #define __FTP_REMOTE_UPDATE__
	#undef __QGDW_2013_STRUCT__
	#undef __ENABLE_ESAM2__
	#define __USE_2013_UPDATE__
	#define __POWEROFF_NO_CHECK_CY__		//ͣ�ϵ粻��⽻��ͣ���¼��ֱ�������ն˵��縴λ
	//#define __COMPUTE_XLOST_30DAY__		//�������
	//#define __COMPUTE_PHASE_XLOST__
#endif

#if defined(__TOPS__)
  //����ʹ��TELITͨ��ģ��
  //#define __GPRS_TELIT__
  //#define __GPRS_MC8331A__
  //#define __GPRS_M72__
  //#define __GPRS_M590__
  //#define __GPRS_G610__
  //#define __GPRS_MY__
#endif  //__TOPS__
#ifdef __JIANGSU_CHECK__
    #define __TCP_SERVER_CLIENT_MODE__  //ʹ�û��ģʽ
    #define __TCP_SERVER_MODE__  		//ʹ�÷�����ģʽ
    #define __QGDW_CHECK__
#endif 
#ifdef __PROVICE_JIBEI__                        //------------------����
    #define __MAX_SPOT_IDX_2040__          //���������֧��2040��
    #define __PRECISE_TIME__                    //��ȷ��ʱ
    #define __CAST_DELAY_UNIT__                 //�㲥Уʱ��λ#endif
    #define __PRECISE_TIME_TEST__                    //��ȷ��ʱ
    #define __METER_EVENT_GRADE__        //�����¼��ȼ�����
    #define __METER_EVENT_GRADE_TEST__
    #define __REPORT_NODE_EVENT__        //��������¼������ϱ�
    #define __METER_CURVE_READ__         //�������߳���
//���£����ڿռ䲻�����ر�
    #undef __READ_HENGTONG_METER__				 //��ͨ��Լ֧��
    #undef __READ_EDMI_METER__					 //�����Լ֧��
    #undef __ENABLE_SYS_TOOLS__	//ϵͳ���Թ���
 //   #undef __CONCENTRATOR_INITIATIVE__    //��������������ģʽ
//    #undef __ENABLE_ESAM2__
    #undef  ALLOW_WRITES
    #undef __F39_DAYHOLD_F210__           //�ն��Ṻ�õ���Ϣ

    #undef __PLC_NET_F149__
    #undef __PLC_NET_F149_TYPE_2__             //F149�̱�ģʽ
    #undef __PLC_NET_NULL_RTU_ADD_DOC__        //�ղɼ�����������
    #undef __TIMER_PLC_NET_MODE_1__            //�㽭ģʽ�ѱ�

    #undef __ERC_39__                         //ERC39������ʧ���¼���¼
    #undef __ERC_40__                         //erc40���ų��쳣�¼���¼
#endif 
#ifdef __PROVICE_HENAN_YIBANG__                        //------------------�����ڰ�
    #define __PROVICE_HENAN__                   //����
    #define __PWLEN_IS_32BYTE__                 //�Ȱ���վ32�ֽ�PW
    #ifdef __QGDW_376_2013_PROTOCOL__
    #undef __QGDW_376_2013_PROTOCOL__
    #endif
#endif 
#ifdef __PROVICE_HENAN__                        //------------------����
 	#define __CHECK_POWEROFF_NEW__		 		//������ͣ�ϵ��¼�
	#define __GW_NEW_EVENT_QUERY__       		//�������¼������ѯ   
    #ifndef __POWER_CTRL__
    #define __PLC_PRIOR_READ_C1DATA__           //�ز���Ԥ��һ������
    #define __PLC_PRIOR_READ_HIGH__             //�ز���Ԥ��һ�����ݣ�����ÿ�����ն���󳭶�������Ļ�����ȫ������ն��᳭��󣬲ų�
    #define __PLC_REC_PLAN_FROM_F39__     //�ز�������F39�������г�������Ҫ�󳭶�����
    #define __F39_DAYHOLD_F210__      //F210����F39���÷�������
    #define __CONCENTRATOR_INITIATIVE__//���������������̷�����Ҫ��
    //#define __COMPUTE_XLOST__
    //#define __USE_DUAL_GPRS_CLIENT__//ʹ��2��GPRS�ͻ��ˣ���֧����̫���Ŀͻ���
    //#define __LINE_LOST_METER_OPEN__
    #define __PLC_READ_VIP_F25__           //�ص������ѹ�����������޹�����
	#define __PLC_READ_VIP_I__	
    #define __REDUCE_485_READ_ITEM__  //����485�ĳ���������
    #endif
    #define __ERC_37__                         //erc37�����ܱ�����¼���¼
//    #define __ERC_41__
    #define __LINE_LOST_METER_OPEN__
 //   #define __READ_07_485_DAYHOLD__				  //07��Լ485����ն��᳭��������������ʹ��0�㵱ǰֵת��
//    #define __MAX_485METER_COUNT_32__      //�������32��485��������ɣ�

#endif 

#ifdef __PROVICE_YUNNAN__           //------------------����
   #define __DL698_41_0928__                   //����DL698_41_0928�� (��������ʡ�в���,�������ϵ�)
   #define __ALLOW_QGDW129_2004__PROTOCOL__    //�Ƿ���ݸ���2004��Լ��2�ֽڣУף�
   #define __GB_645_YUNNAN__
   #define __GB_645_YUNNAN_CJQ_REC_2__   //�������ϵ���Լ�����ɼ������������������00010000,���򳭶�3�������
   #define __READ_485_DIRECT__                 //485һ������ʵʱ�㳭
   #undef  __NOSRC_NODE_COUNT__ 
   #define __NOSRC_NODE_COUNT__                   7    //��Դ�ɽڵ�����
   #define __TD_IS_TODAY__                    //ʱ���ǵ��죬����ǰһ��,������ͨ��վ��ɫ
    //#define __PLC_REC_CUR_WHEN_DAYHOLD_DENY__   //�ն��᳭����ʱ�ĳ���ǰ���ݱ�ʶ
	#undef __CHECK_POWEROFF_NEW__		 //������ͣ�ϵ��¼�
	#undef __GW_NEW_EVENT_QUERY__       //�������¼������ѯ
    #define __GB_645_YUNNAN_CHECK__
    #define __PROVICE_YUNNAN_TEST__  //ͳ����������Ҫ
    #define __PLC_REC_CUR_WHEN_DAYHOLD_DENY__
//    #define __PLC_direct_curdata_to_dayhold__//����汾��ƽʱ���ã�ֱ�ӳ���ǰֵ�洢Ϊ�ն��ᣬ���ϱ�ǩ�������ã�����Ҫ��չ
//    #define __DAYHOLD_F210__    //��ͨģʽ����Ҫ
    #define __UPDATE_NO_CHECK_DATE__ //�������ж������Ƿ���ǰ
//    #define __YUNANA_LZ_OLD_METER_PROTOL_FIRST__//����բʱ�������Ϲ�Լ��ǰ
    #define __DISABLE_ESAM_F5__//�ر�F5���ܣ�����������������վ���·��Ĳ���
    #define __DIRECT_SWITCH__//֧��ֱ�Ӻ�բ����
#endif

#ifdef __PROVICE_YUNNAN_PUER__           //------------------�����ն�
    #undef __ALLOW_QGDW129_2004__PROTOCOL__    //�Ƿ���ݸ���2004��Լ��2�ֽڣУף�
    #define __NO_QGDW_TYPE__                    //������Э�����ͣ�ͳͳ����376.1����
#endif

#ifdef __PROVICE_YUNNAN_ZHAOTONG__             //------------------������ͨ
    #undef __ALLOW_QGDW129_2004__PROTOCOL__    //�Ƿ���ݸ���2004��Լ��2�ֽڣУף�
    #define __NO_QGDW_TYPE__                   //������Э�����ͣ�ͳͳ����376.1����
    #define __TD_IS_TODAY__                    //ʱ���ǵ��죬����ǰһ��,������ͨ��վ��ɫ
#endif

#ifdef __PROVICE_ANHUI__            //------------------����
   #ifndef __POWER_CTRL__
    #define __PLC_PRIOR_READ_C1DATA__      //�ز���Ԥ��һ������
    #define __MAX_SPOT_IDX_2040__          //���������֧��2040��
    #define __MAX_485METER_COUNT_32__      //�������32��485��������ɣ�
    #define __PLC_READ_VIP_FYG_WG__        //�ص���������й����������޹��ܵ���ʾֵ
    #define __PLC_READ_VIP_F25__           //�ص������ѹ�����������޹�����
	#define __PLC_READ_VIP_I__				
	#define __PLC_READ_VIP_YP__
	#define __PLC_READ_VIP_WP__
    //#define __PLC_REC_CJQ_ERC_36_37__      //�ز�����ɼ�������erc36���ܱ�����¼���erc37
    //#define __PLC_REC_ERC_36_OPEN__        //�ز�����ɼ�������erc36���ܱ�����¼�
    //#define __PLC_REC_ERC_37_OPEN__        //�ز�����ɼ�������erc37���ܱ���ť���¼�
    #define __ERC_51__                     //���ܱ�ͣ/�ϵ��¼���¼  �����Ǹ��ݴ����任����ʱ�䣬��һ�β����ɣ��仯�˼��ξ����ɼ���һ�������10��
    #define __PLC_REC_CJQ_CYCLE_ERC_52__   //�ز����ڳ����ɼ�������erc38�������¼�
    //#define __READ_PLC_NOISE__             //��ȡ����
    #define __PLC_REC_PLAN_FROM_F39__         //�ز�������F39�������г���
    #define __PLC_READ_METER_TIME__           //�ز��������ʱ�ӣ�����ERC12
    #define __ERC_13__                        //�ز�����״̬�֣�����erc13
    #define __RISECOMM_376_2_2013__
    //#define	__COMPUTE_XLOST__               //������
    //#define __USE_DUAL_GPRS_CLIENT__     //˫GPRS�ͻ���
   #endif 
    #define __STAT_LOADRATE__              //ͳ�Ƹ�����
    #define __PATCH_REC_DH__               //����62���ն���
    //#define __REC_DH_485_EXT__             //����485��չ�ն���F300-F315
    #define __CHECK_07_DAYHOLD_TIME_1__       //�����ն���ʱ�䣬ʱ�䲻�ԵĻ��������ն��ᣬ�����ǰ����ն���ʱ��洢����Ӧλ��
    #define __PATCH_REC_DAY_HOLD_TD_IS_CUR__  //�����ն���ʱ���ǳ�����ʱ�䣬���Ǵ洢ʱ�����һ��
    #define __PATCH_REC_DH_REPORT__           //�ϱ��������ն���
	//#undef __CHECK_POWEROFF_NEW__		 //������ͣ�ϵ��¼�
	//#undef __GW_NEW_EVENT_QUERY__       //�������¼������ѯ
    #define __CHECK_DAYHOLD_DATA__            //����ն���������ȷ��	
#endif

#ifdef __PROVICE_FUJIAN__            //------------------����
    #define __PLC_PRIOR_READ_C1DATA__      //�ز���Ԥ��һ������
    #define __PLC_READ_LOAD_RECORD__       //�����ز��������ɼ�¼���ݣ���Ӧ����F89-F94,F103,F101,����60����ʱ����������10�������
   //#define __SMS_REPORT__            //��������ϱ�����֧���¼��ϱ�
   //#define __SMS_TASK_REPORT__       //������������ϱ�
	#define __REMOTE_ETH_PRIORITY__ //��̫������ԭ��
    #define __PLC_READ_VIP_F25__           //�ز������й��޹����ʡ���ѹ����(����ֻ����ѹ)
#endif

#ifdef __PROVICE_GUIZHOU__          //------------------����
   #define __DL698_41_0928__
   #define __PLC_READ_FXYG__     //�ز����������й�
   #define __RECONNECT_MSA_AFTER_CHANGE__ 
#endif

#ifdef __PROVICE_SHANXI__           //------------------ɽ��
    #define __SET_F25_METER_TYPE__   //���ݲ������õ�F25ȷ���ز��Ķ๦���뵥��
    #define __PLC_READ_METER_TIME__    //�ز��������ʱ�ӣ�����ERC12
    #define __REDUCE_FLOW__    //��������������Ҫ��
    #define __COMPUTE_XLOST__
    #undef __READ_HENGTONG_METER__				 //��ͨ��Լ֧��
    #undef __READ_EDMI_METER__	
    //#define __USE_DUAL_GPRS_CLIENT__//ʹ��2��GPRS�ͻ��ˣ���֧����̫���Ŀͻ���
#endif

#ifdef  __PROVICE_HUNAN__           //------------------����
   #define __READ_PLC_NOISE__       //��ȡ����
    #define __REDUCE_FLOW__          //��������
    #define __OPEN_PACKAGE_MONITOR__      //���������
    #define __VOLTAGE_MONITOR__           //��ѹ����

    #undef __DISPLAY_IN_CHINESE_ZJWS__
    //#define __DISPLAY_HUNAN__
    #define __DISPLAY_GW_2013_HUNAN__
    #undef __DISPLAY_QINGHAI__
    #undef __DISPLAY_GW_2013__
    
    #define __READ_485_DIRECT__                //485һ������ʵʱ�㳭
    #define __NO_QGDW_TYPE__                    //������Э�����ͣ�ͳͳ����376.1����
    #define __VIP_PATCH_REC_DATA__             //�ص����07��Сʱ����
    //#define __READ_07_485_DAYHOLD__				  //07��Լ485����ն��᳭��������������ʹ��0�㵱ǰֵת��
    #define __PLC_PRIOR_READ_C1DATA__           //����Ԥ��һ��������
    //#define __PLC_PRIOR_READ_HIGH__             //�ز���Ԥ��һ�����ݣ�����ÿ�����ն���󳭶�������Ļ�����ȫ������ն��᳭��󣬲ų�
    #define __PLC_PRIOR_READ_DATA_SAVE_FILE__   //�ز���Ԥ��һ�����ݱ������ļ�ϵͳ�У����򱣴���spansion��
    #define __PLC_PRIOR_READ_PER_HOUR__         //�ز���Ԥ��һ������ÿСʱÿСʱ����һ��
    #define __PLC_READ_VIP_F25__                //�ز������й��޹����ʡ���ѹ����������ֻ����ѹ������
    #define __PLC_READ_VIP_I__                //�ز������й��޹����ʡ���ѹ����������ֻ����ѹ������
    #define __PLC_REC_PLAN_FROM_F39__           //�ز�������F39�������г���
    
    #define __F39_DAYHOLD_F32__                 //F39�����ն���F32
    #define __PLC_READ_VIP_LOAD_RECORD__        //�ص���������ɼ�¼
    
    #if (defined __ZhuZhou_CITY__) || (defined __HuaiHua_CITY__)    
    #undef __VIP_PATCH_REC_DATA__             //�ص����07��Сʱ����
    #undef __PLC_READ_VIP_LOAD_RECORD__       //�ص���������ɼ�¼
        #ifdef __ZhuZhou_CITY__
        #define __F_CLASS_METER_CURVE__             //F�����Զ���������
        #endif
    #endif
    
    #define __PLC_PRIOR_F26_SWITCH__            //���������¼����ʱ�л�������
	
	//#define __CHECK_07_DAYHOLD_TIME__    //��������ʱ�꣬����ʱ����󣬵�ǰ���ڲ������ն���
    //#define __COMPUTE_XLOST__	
   	//#define __USE_DUAL_GPRS_CLIENT__     //˫GPRS�ͻ���
    //ʹ�ú����ѱ�ģʽ���������㽭ģʽ
    #ifdef __QGDW_376_2013_PROTOCOL__
    #undef __PLC_NET_F149__
    #undef __PLC_NET_F149_TYPE_2__             //F149�̱�ģʽ
    #undef __PLC_NET_NULL_RTU_ADD_DOC__        //�ղɼ�����������
    #undef __TIMER_PLC_NET_MODE_1__            //�㽭ģʽ�ѱ�
    #undef __PLC_NET_ADD_FILE__                //�¹淶����F149 01ʱ��ӵ����������ѱ��ļ� 02����ļ������ӵ���
    #undef __F39_DAYHOLD_F210__                //�ն��Ṻ�õ���Ϣ
    #endif
    //#define __METER_CURVE_READ__         //�������߳���

#endif
#ifdef __PROVICE_HUNAN_FK__ 
	#define __CHECK_POWEROFF_NEW__		 //������ͣ�ϵ��¼�
	#define __GW_NEW_EVENT_QUERY__       //�������¼������ѯ
	#define __METER_CURVE_READ__         //�������߳���
#endif
#ifdef __PROVICE_SICHUAN_BOGAO__          //------------------�Ĵ�����
    #define __PROVICE_SICHUAN__
    #define __DAYHOLD_GTOUP_REPEAT_TD__   //���������������·�ʱ���ж��ʱ��
#endif
#ifdef __PROVICE_SICHUAN_HLD10S__           //--------------------�Ĵ������485����10S��ʱ
    #define __PROVICE_SICHUAN_HLD__
#endif
#ifdef __PROVICE_SICHUAN_HLD__                //------------------�Ĵ�������
    #define __PROVICE_SICHUAN__
    #define __INFRA_DL645_READ__          //����ͨ�����Ⳮ��������������ݣ�֧��9010~901F,����Ӧ07������
#endif

#ifdef __PROVICE_SICHUAN__          //------------------�Ĵ�
    #define __INFRA_DL645_READ__          //����ͨ�����Ⳮ��������������ݣ�֧��9010~901F,����Ӧ07������
    #undef __F39_DAYHOLD_F210__                    //�ն��Ṻ�õ���Ϣ
    #undef __VIP_PATCH_REC_DATA__                   //�ص����07��Сʱ����
    #define __DAYHOLD_F167__     //�ն�����ܱ��õ���Ϣ �洢93�������
//    #undef __PATCH_REC_DH__
//    #undef __PLC_READ_FXYG__
    #ifndef __POWER_CTRL__
    #define __PLC_PRIOR_READ_C1DATA__    //����Ԥ��һ��������
    #endif
    //#define __REC_PRICE_PARAM__           //������۲���
    #define __MAKE_CUTPOWER_EVENT__         //�����ʱ����ͣ���¼����Ĵ�����������Ҫ
 //   #define __F210_SIMPLE_VISION__   //F210�������
 //   #define __DAYHOLD_F210__
    #ifndef __REPORT_NODE_EVENT__
    #define __REPORT_NODE_EVENT__        //��������¼������ϱ�
    #endif
    
    #define __METER_EVENT_GRADE_TEST__   //C1F244��ȡ�¼��ļ�������
    #define __REPORT_NODE_EVENT__        //��������¼������ϱ�
    
    #ifndef __GW_NEW_EVENT_QUERY__
    #define __GW_NEW_EVENT_QUERY__       //�������¼������ѯ
    #endif
    
    #ifndef __CHECK_POWEROFF_NEW__
    #define __CHECK_POWEROFF_NEW__       //������ͣ�ϵ��¼�
    #endif
#endif

#ifdef __PROVICE_HUBEI__            //------------------����
    #define __PLC_READ_VIP_F25__    //�ز��ص�����й��޹����ʡ���ѹ����
    //#define __PLC_READ_VIP_WP__    //�ز��ص�����޹�����

    #define __PLC_READ_FXYG__       //�ز����������й�
    #define __STAT_LOADRATE__      //ͳ�Ƹ�����
    #define __PLC_PRIOR_READ_C1DATA__     //����Ԥ��һ��������
 //   #define __HUBEI_REC_V_I__             //������ѹ����
//	#define __HUBEI_REC_POWER__             //��������
    #define __METER_DOC_F150__
  //  #define __COMPUTE_XLOST__
//    #define __USE_DUAL_GPRS_CLIENT__//ʹ��2��GPRS�ͻ��ˣ���֧����̫���Ŀͻ���
    #undef __ENABLE_SYS_TOOLS__	//ϵͳ���Թ���
    #undef __CONCENTRATOR_INITIATIVE__    //��������������ģʽ
    #undef __READ_HENGTONG_METER__				 //��ͨ��Լ֧��
    #undef __READ_EDMI_METER__					 //�����Լ֧��
  
    #define __METER_EVENT_GRADE__        //�����¼��ȼ�����
    #define __METER_EVENT_GRADE_TEST__   //C1F244��ȡ�¼��ļ�������
    #define __REPORT_NODE_EVENT__        //��������¼������ϱ�
    #define __GW_NEW_EVENT_QUERY__       //�������¼������ѯ
    #define __CHECK_POWEROFF_NEW__		 //������ͣ�ϵ��¼�
    #define  BASE_STATION_CHANGE
#endif

#ifdef __PROVICE_TIANJIN__          //------------------���
    #define __PLC_READ_VIP_F25__    //�ز��ص�����й��޹����ʡ���ѹ����
    #define __PLC_READ_FXYG__       //�ز����������й�
    #define __STAT_LOADRATE__     //ͳ�Ƹ�����
    #define __PLC_REC_PLAN_FROM_F39__  //�ز�������F39�������г���
    #define __PLC_PRIOR_READ_C1DATA__      //�ز���Ԥ��һ������
#endif

#ifdef __PROVICE_CHONGQING__        //------------------����
    #define __485_NET__
    #define __TD_IS_TODAY1__        //ʱ���ǵ��죬����ǰһ��, ֻ�����ն����
   
   	
   	#ifndef __POWER_CTRL__    
	    #define __PLC_READ_VIP_F25__    //�ز������й��޹����ʡ���ѹ����
	    //#define __DISPLAY_HEADER_LINE2__   //����ʾ��������
	    #define __PLC_READ_VIP_I__//�ص�����������
	    #define __PLC_READ_VIP_YP__
		#define __PLC_READ_VIP_WP__
	    #define __PLC_REC_PLAN_FROM_F39__ //09��13Ӳ����������F39���ý��г���??
	    
	    #ifdef __QGDW_09_STRUCT__//�������09Ӳ������Ҫ������13Э��
		    #define __CHECK_POWEROFF_NEW__		 //������ͣ�ϵ��¼�
			#define __GW_NEW_EVENT_QUERY__       //�������¼������ѯ
	   	#endif
	   	#define __DISPLAY_FOOTER_NO_GPRSSTATUS__    //�ײ�״̬������ʾGPRS��Ϣ
	   	//#define __COMPUTE_XLOST__                 //����������
    	//#define __USE_DUAL_GPRS_CLIENT__          //˫GPRS�ͻ���
   	#endif
#endif

#ifdef __PROVICE_XINJIANG__            //------------------�½�
    #define __USE_DUAL_GPRS_CLIENT__
    #define __BATCH_TRANSPARENT_METER_TASK__
    //#define __BATCH_TRANSPARENT_METER_TASK_DEBUG__
    //#define __BATCH_TRANSPARENT_METER_TASK_TMP__
    //#define __BATCH_TRANSPARENT_METER_TASK_TMP1__
#endif

#ifdef __PROVICE_XINJIANGFK__            //------------------�½�����
    #define __PROVICE_XINJIANG__
    #define __POWER_CTRL__               //����
    #define STACK_UNUSE_ENC24J600
#endif

#ifdef __COUNTRY_BRAZIL__                       //--------------------����
    #define __BRAZIL_HOUR_REC__                 //ÿСʱ�������ɼ�¼

    #undef __ERC_17__
    #undef __ERC_37__
	#undef __ERC_38__
	#undef __ERC_41__
    #undef __ERC_41_SHANGHAI__
	#undef __ERC_67__
    #undef __ERC_68__
    #undef __ERC_39__                         //ERC39������ʧ���¼���¼
    #undef __ERC_40__                         //erc40���ų��쳣�¼���¼
    #ifndef __POWER_CTRL__
    #undef __PLC_NET_F149__
    #undef __PLC_NET_F149_TYPE_2__             //F149�̱�ģʽ
    #undef __PLC_NET_NULL_RTU_ADD_DOC__        //�ղɼ�����������
    #undef __TIMER_PLC_NET_MODE_1__            //�㽭ģʽ�ѱ�
    #undef __PLC_PRIOR_READ_C1DATA__      //�ز���Ԥ��һ������
    #undef __PLC_PRIOR_READ_DATA_SAVE_FILE__   //�ز���Ԥ��һ�����ݱ������ļ�ϵͳ�У����򱣴���spansion��
    #undef __CONCENTRATOR_INITIATIVE__    //��������������ģʽ
    #undef __MAX_SPOT_IDX_2040__          //���������֧��2040��
    #undef __PLC_REC_PLAN_FROM_F39__      //�ز�������F39�������г���
    #endif
    #undef __METER_DOC_F150__
    #undef __PATCH_REC_DH__                    //����3���ն���
    //#define __VIP_PATCH_REC_DATA__              //�ص����07��Сʱ����
    #undef __F39_DAYHOLD_F210__                    //�ն��Ṻ�õ���Ϣ
    #undef __DELETE_30_DAY_NO_REC_SPOT__       //ɾ��30��û�г����Ĳ�����
    #undef __PLC_NET_ADD_FILE__                //�¹淶����F149 01ʱ��ӵ����������ѱ��ļ� 02����ļ������ӵ���
    #undef __FILL_DEFAULT_C2_DATA__    //�����㲻����ʱ������2�������Ƿ����EE
	#undef __CHECK_POWEROFF_NEW__		 //������ͣ�ϵ��¼�
	#undef __GW_NEW_EVENT_QUERY__       //�������¼������ѯ
#endif

#ifdef __COUNTRY_SOUTH_AFRICA__                 //--------------------�Ϸǣ�ͬ��ɫ�У����ĳ�����ʶ
    #define  __COUNTRY_ISRAEL__ 
    #define __ISRAEL_CYCLE_REC__      //��ɫ�����ڳ�������F33�ĳ�����ʱ����г�����С���15���ӣ�����96���������
    #define __COUNTRY_SOUTH_AFRICA_DISPLAY__ //�Ϸ���ʾ��XL_002
#endif

#ifdef  __COUNTRY_ISRAEL__                     //------------------��ɫ�У������˹㲥�����������ݱ�ʶ��AFN=05, FN=62,63 ���á�ȡ���㲥����
    #define __ISRAEL_HOUR_REC__       //�����ն��ᣬ��Сʱ���ᣬ0���Сʱ����ת���ն��ᣬ����������96���������
    #if defined(__ISRAEL_CYCLE_REC__)
       #undef __ISRAEL_HOUR_REC__
    #endif
     #define __HAVE_PLC_BROADCAST_CTRL__  //�㲥��������
    #undef  __NOSRC_NODE_COUNT__
    #define  __NOSRC_NODE_COUNT__  7     //��Ҫ֧�ּ����������¼�
    #undef __PLC_READ_FXYG__
    #define __ISRAEL_REC_CONTROL__
    //#define __ISRAEL_REC_ONE_ITEM__
    #define __USE_SMS__                  //ʹ�ö���
    #undef __PLC_READ_FXYG__
    #define __ISRAEL_PLC_REC_APM__
    #define __ISRAEL_PLC_REC_DAYHOLD_WG__
    #define __COUNTRY_ISRAEL_DISPLAY__ //���Ұ汾��ʾXL_001
    #undef __ERC_17__
    #undef __ERC_37__
	#undef __ERC_38__
	#undef __ERC_41__
    #undef __ERC_41_SHANGHAI__
	#undef __ERC_67__
    #undef __ERC_68__
    #undef __ERC_39__                         //ERC39������ʧ���¼���¼
    #undef __ERC_40__                         //erc40���ų��쳣�¼���¼
    #undef __PLC_NET_F149__
    #undef __PLC_NET_F149_TYPE_2__             //F149�̱�ģʽ
    #undef __PLC_NET_NULL_RTU_ADD_DOC__        //�ղɼ�����������
    #undef __TIMER_PLC_NET_MODE_1__            //�㽭ģʽ�ѱ�
    #undef __PLC_PRIOR_READ_C1DATA__      //�ز���Ԥ��һ������
    #undef __PLC_PRIOR_READ_DATA_SAVE_FILE__   //�ز���Ԥ��һ�����ݱ������ļ�ϵͳ�У����򱣴���spansion��
    #undef __CONCENTRATOR_INITIATIVE__    //��������������ģʽ
    #undef __MAX_SPOT_IDX_2040__          //���������֧��2040��
    #undef __PLC_REC_PLAN_FROM_F39__      //�ز�������F39�������г���

    #undef __METER_DOC_F150__
    #undef __PATCH_REC_DH__                    //����3���ն���
    //#define __VIP_PATCH_REC_DATA__              //�ص����07��Сʱ����
    #undef __F39_DAYHOLD_F210__                    //�ն��Ṻ�õ���Ϣ
    #undef __DELETE_30_DAY_NO_REC_SPOT__       //ɾ��30��û�г����Ĳ�����
    #undef __PLC_NET_ADD_FILE__                //�¹淶����F149 01ʱ��ӵ����������ѱ��ļ� 02����ļ������ӵ���
    #undef __FILL_DEFAULT_C2_DATA__    //�����㲻����ʱ������2�������Ƿ����EE
	#undef __CHECK_POWEROFF_NEW__		 //������ͣ�ϵ��¼�
	#undef __GW_NEW_EVENT_QUERY__       //�������¼������ѯ
    #undef __ENABLE_ESAM__              //��ʹ��esam
//	#undef __USE_2013_UPDATE__			 //ʹ��2013��Э������
#endif

#ifdef __PROVICE_PERU__
    
#endif
#ifdef __LINE_LOST_DEEP_APPLY__
    #define __COMPUTE_XLOST__
    #define __USE_DUAL_GPRS_CLIENT__//ʹ��2��GPRS�ͻ��ˣ���֧����̫���Ŀͻ���
    #define __LINE_LOST_METER_OPEN__
    #define __PLC_READ_VIP_F25__           //�ص������ѹ�����������޹�����
	#define __PLC_READ_VIP_I__	
    #define __UPDATE_NO_CHECK_DATE__ //�������ж������Ƿ���ǰ
#endif

#ifdef __PROVICE_STAR__       //˼��
    #define __STAR_EXT_METER_PROTOCOL_40__//˼���Լ
    #define __SMS_TXT__   //֧���ı�����
    #undef __DELETE_30_DAY_NO_REC_SPOT__
//	#define __CHECK_POWEROFF_NEW__		 //������ͣ�ϵ��¼�
//	#define __GW_NEW_EVENT_QUERY__       //�������¼������ѯ
    #define __ERC_51__                   //ͣ�ϵ��¼�¼�
    #define __METER_EVENT_GRADE__        //�����¼��ȼ�����
    #define __METER_EVENT_GRADE_TEST__
    #define __REPORT_NODE_EVENT__        //��������¼������ϱ�
    #define __METER_CURVE_READ__         //�������߳���
//    #define __STAR_PRIOR_F187__          //Ԥ��0CF187�Ե����
//    #define __PLC_PRIOR_READ_C1DATA__      //�ز���Ԥ��һ������
    #define __COMPUTE_XLOST__
    #define __REC_MONTH_HOLD_07__      //������һ����������
    #define __REC_MONTH_HOLD_07_XL__   //������һ��������������
#endif

#ifdef __PROVICE_ZHEJIANG__        //------------------�㽭
    #define __485_BIT_SEARCH__                  //485��λ�ѱ�
    #define __ERC_41_SHANGHAI__
    //#define __SMS_REPORT__                      //��������ϱ�����֧���¼��ϱ�
    //#define __SMS_TASK_REPORT__                 //������������ϱ�
    //#define __PLC_READ_FXYG__                   //���������й�
    //#define __READ_485_DIRECT__                 //485һ������ʵʱ�㳭
    #define __PLC_NET_F149__
    #define __PLC_NET_F149_TYPE_2__             //F149�̱�ģʽ-----��2009�ı��������£���2��δѡ��
    //#define __PLC_NET_NULL_RTU_ADD_DOC__        //�ղɼ�����������
    #define __ZHEJIANG_EMERGENCY__              //�㽭 Ӧ������ �̱�ʱ���ز������뵵����������·�ɲ�ѯ�ڵ�Ĳ���
    #define __RECONNECT_MSA_AFTER_CHANGE__      //GPRS�����޸ĺ�����
    #define __READ_07_485_DAYHOLD__             //07��Լ485����ն��᳭��������������ʹ��0�㵱ǰֵת��,���ֳ���05000101����1�Σ���ʱ�������ݣ���������05060101����1�Σ��ն�������
    #define __CHECK_07_DAYHOLD_TIME__           //�����ն���ʱ�䣬ʱ�䲻�ԵĻ��������ն���
    #undef __USE_SMS_HEAD__	                    //ʹ�ö���ͷ
    #define __PRECISE_TIME__                    //��ȷ��ʱ
    #define __CAST_DELAY_UNIT__                 //�㲥Уʱ��λ#endif
    #define __PRECISE_TIME_TEST__               //��ȷ��ʱ
    #define __METER_EVENT_GRADE__               //�����¼��ȼ�����
    #define __METER_EVENT_GRADE_TEST__
    #define __REPORT_NODE_EVENT__               //��������¼������ϱ�
    #define __EVENT_04001501_BEFORE__           //�¼�״̬�ַ����¼���¼ǰ��
    #define __EVENT_RECORD_BLOCK__              //�¼���¼�������ݿ鳭��
    #define __METER_CURVE_READ__                //�������߳���
    #define __GET_DH_AMOUNT_FROM_REAL__         //��������ն����������ʵʱֵȥ���㣬�������ն��ᣬ��ʱ����ʹ�ã���485���
	#define __DELETE_30_DAY_NO_REC_SPOT__       //ɾ��30��û�г����Ĳ�����
//	#define __COMPUTE_XLOST__                     //�������
//����4�������ڿռ䲻�����ر�
    #undef __READ_HENGTONG_METER__              //��ͨ��Լ֧��
    #undef __READ_EDMI_METER__                  //�����Լ֧��
    #undef __ENABLE_SYS_TOOLS__	                //ϵͳ���Թ���
    #undef __CONCENTRATOR_INITIATIVE__          //��������������ģʽ
    #undef __F39_DAYHOLD_F210__           //�ն��Ṻ�õ���Ϣ
	#undef __FTP_CUSTOM_UPDATE__
	#undef __FTP_REMOTE_UPDATE__
	#undef STACK_USE_FTP_CLIENT
	#undef __USE_2013_UPDATE__
    #undef __REDUCE_485_READ_ITEM__  //����485�ĳ���������

#endif

#ifdef __PROVICE_GUANGXI__         //------------------����
    #define __COMPUTE_XLOST__      //��������
    #define __PLC_REC_LOST_VI__    //����ʧ��ʧѹ��̴���
    #define __STAT_PLC_REC_FAIL__   //ͳ���ز�����ʧ��
#endif

#ifdef __PROVICE_SHANGHAI_00__     //------------------�Ϻ�0
    #define __PROVICE_SHANGHAI__
    #define __DISPLAY_QINGHAI__
#endif

#ifdef __PROVICE_SHANGHAI_01__     //------------------�Ϻ�1
    #define __PROVICE_SHANGHAI__
    #define __DISPLAY_GW1__
#endif

#ifdef __PROVICE_SHANGHAI_02__     //------------------�Ϻ�2
    #define __PROVICE_SHANGHAI__
    #define __DISPLAY_GW2__
#endif

#ifdef __PROVICE_SHANGHAI__        //------------------�Ϻ�
    #define __READ_485_DIRECT__               //485һ������ʵʱ�㳭
    #define __FTP_REMOTE_UPDATE__       //FtpԶ������  �Ϻ���չ
    #define __PLC_READ_FXYG__              //�ز����������й�
    #define __ALLOW_LETTER_METER_NO__      //������ĸ���
    #define __PATCH_REC_DH__               //����3���ն���  ������

    #ifndef __QGDW_2013_STRUCT__
    #define __USE_RS485_3__            //ʹ��485_3,��֧��695оƬ
    #endif //#ifndef __QGDW_2013_STRUCT__

//    #define __PATCH_REC_C2_DATA__      //AFN=1D ����2������
    #define __VIP_PATCH_REC_DATA__            //�ص����07��Сʱ����

    #ifndef __POWER_CTRL__
    #define __PLC_READ_VIP_V__         //�ز��ص������ѹ
	#define __PLC_READ_VIP_I__         //�ز��ص��������
	#define __PLC_READ_VIP_YP__        //�ز��ص�����й�����
	#define __PLC_READ_VIP_WP__        //�ز��ص�����޹�����
	#define __PLC_READ_VIP_Q__         //�ز��ص������������
    //#define __REC_MONTH_HOLD_07__             //07���¶����ǳ�����һ���������ݴ洢Ϊ�¶��ᣬ���Ҳ�����һ���µģ���ǰ���ն���ת��
    #endif //#ifndef __POWER_CTRL__

    #define __PLC_READ_METER_TIME__           //�ز��������ʱ�䣬����erc12
    #define __SHANGHAI09__                    //��ʾ�����Ϻ������⹦��
    //#define __PLC_READ_VIP_F25__              //�ص������ѹ�����������޹�����
    #define __CHECK_07_DAYHOLD_TIME_1__
    #define __PLC_REC_PLAN_FROM_F39__         //�ز�������F39�������г���
    #define __OTHER_DAYHOLDS_SAVE_SPOT_FLIE__   //�ز��๦�ܱ������ն������ݱ����ڲ������ļ��У��洢2��������
    #define __VIP_METER_IS_485__                //485��������Ϊ�ص��

    #ifndef __POWER_CTRL__
    #define __PLC_READ_VIP_LOAD_RECORD__        //�ص���������ɼ�¼
    #define __PLC_READ_VIP_FYG_WG__             //�ص���������й����������޹��ܵ���ʾֵ
    #endif

    #define __METER_DOC_CRC16__                //�����CRC16У��
    #define __REC_MONTH_HOLD_SHANGHAI_2009__   //�Ϻ���Լ������¶���

    #define __ERC_10__
    #define __ERC_13__
    
    #undef  __READ_HENGTONG_METER__				 //��ͨ��Լ֧��
	#undef  __READ_EDMI_METER__					 //�����Լ֧��
    #undef __ENABLE_SYS_TOOLS__	//ϵͳ���Թ���
    #undef __ENABLE_ESAM2__
    #undef __CONCENTRATOR_INITIATIVE__    //��������������ģʽ
    #undef __PLC_NET_F149__
    #undef __PLC_NET_F149_TYPE_2__             //F149�̱�ģʽ
    #undef __PLC_NET_NULL_RTU_ADD_DOC__        //�ղɼ�����������
    #undef __TIMER_PLC_NET_MODE_1__            //�㽭ģʽ�ѱ�
    #undef __ERC_37__                         //erc37�����ܱ�����¼���¼
    #undef __ERC_38__                         //erc38�����ܱ���ť���¼���¼
    #undef __ERC_39__                         //ERC39������ʧ���¼���¼
    #undef __ERC_40__                         //erc40���ų��쳣�¼���¼
    #undef __ERC_41_SHANGHAI__                //erc41����ʱ�¼���¼

    //#define __485_BAUDRATE_USE_DOC__            //485�������ʹ�õ��������ã������յ���Լ����
    //#define __PROVICE_SHANGHAI_CHECK_20131110__  //�Ϻ�2013.11.10�ͼ�汾ʹ�ã��ֳ��汾��Ҫʹ�øñ�ǩ
    #define __REMOTE_ETH_PRIORITY__	           //��̫������ԭ�򣬲�����ΪGPRS����

    #ifndef __POWER_CTRL__
    //#define __METER_CURVE_READ__         //�������߳���
    #define __METER_EVENT_GRADE__        //�����¼��ȼ�����
    #define __METER_EVENT_GRADE_TEST__   //C1F244��ȡ�¼��ļ�������
    #define __REPORT_NODE_EVENT__        //��������¼������ϱ�
    #define __GW_NEW_EVENT_QUERY__       //�������¼������ѯ
    #define __CHECK_POWEROFF_NEW__		 //������ͣ�ϵ��¼�
    #define __SHANGHAI_CLASS_53_54_485_CURVE__
    #endif //#ifndef __POWER_CTRL__

#endif

#ifdef __PROVICE_SHANGHAI_CHECK_20131110__     //�Ϻ�2013.11.10�ͼ�汾ʹ��
    #define __ERC_10__
    #define __ERC_13__
    #define __ERC_17__                         //ERC17����ѹ��ƽ���
    #define __ERC_33__
    #define __ERC_37__                         //erc37�����ܱ�����¼���¼
    #define __ERC_38__                         //erc38�����ܱ���ť���¼���¼
    #define __ERC_39__                         //ERC39������ʧ���¼���¼
    #define __ERC_40__                         //erc40���ų��쳣�¼���¼
    #define __ERC_41_SHANGHAI__                //erc41����ʱ�¼���¼
    //#define __ERC_67__                         //ERC67�����ܱ�����¼���¼
    #define __ERC_68__                         //ERC68�����ܱ��ɿ��������¼���¼
#endif

#ifdef __PROVICE_JIANGSU__        //------------------����
    //#define __PLC_REC_PLAN_FROM_F39__      //�ز�������F39�������г���
    #define __JIANGSU_EXT_METER_PROTOCOL_11_12__  //������97��Լ 11,12
    #define __DETECT_BLOCK_READ__       //�����
    #define __TCP_SERVER_CLIENT_MODE__  //ʹ�û��ģʽ
    #define __TCP_SERVER_MODE__  		//ʹ�÷�����ģʽ
    #define __485_BIT_SEARCH__          //485�ѱ�

    //#define __PLC_READ_VIP_FYG_WG__
    //#define __PLC_READ_VIP_V__
    //#define __VIP_METER_255_JIANGSU__       //�ص��֧��255��
    //#define __VIP_METER_255__           //�ص��֧��255��
    //#define __PLC_VIP_METER_REC_MODE_2__     //�ص������ʽ2
    //#define __PLC_VIP_METER_REC_MODE_2_SUPPLEMENT_REC__     //�ص������ʽ2���������Сʱ����

    #undef __GPRS_MC8331A__
	#define __PLC_PRIOR_READ_C1DATA__			  //�ز���Ԥ��һ������
    #define __DISPLAY_FOOTER_NO_GPRSSTATUS__
//��ʱ�곭�������ǩ��
    #define __READ_07_485_DAYHOLD__               //07��Լ485����ն��᳭��������������ʹ��0�㵱ǰֵת��,���ֳ���05000101����1�Σ���ʱ�������ݣ���������05060101����1�Σ��ն�������
    #define __REC_MONTH_HOLD_07__             //07���¶����ǳ�����һ���������ݴ洢Ϊ�¶��ᣬ���Ҳ�����һ���µģ���ǰ���ն���ת��
    #define __REC_DAY_HOLD_READ_SETTLEMENT_DAY_DATA__      //���������ݳ�����һ����������
    #define __CHECK_07_DAYHOLD_TIME__           //�����ն���ʱ�䣬ʱ�䲻�ԵĻ��������ն���

	#define	__CURVE_96POINTS_BREAK_FRAME__		//�������ݰ�96�����֡����ֹ��վ���ܴ������ɷ�֡����
	#define __POWEROFF_NO_CHECK_CY__		//ͣ�ϵ粻��⽻��ͣ���¼��ֱ�������ն˵��縴λ
    #undef __TIMER_PLC_NET_MODE_1__            //�㽭ģʽ�ѱ�
    #undef __PLC_NET_F149_TYPE_1__
    #undef __PLC_NET_F149_TYPE_2__
	#define __OVERMAX_NO_MAXMAX__			 //Խ������ʱ������Խ����
	#define __COMPUTE_XLOST__                 //�������
#endif

#ifdef __PROVICE_GANSU__                //------------------------����
    #define __USE_FIXED_HEART_CYCLE__	//ʹ�ù̶���������
    #undef __PLC_PRIOR_READ_C1DATA__   //�ز���Ԥ��һ������
    #define __DAYHOLD_F167__
#endif

#ifdef __PROVICE_JILIN_FK__                    //------------------------���ָ���
    #define __PROVICE_JILIN__
    //#define __POWER_CTRL__     //����    
#endif

#ifdef __PROVICE_JILIN__                //------------------------����
    #ifndef __POWER_CTRL__
    //#define __RTUADDR_IS_6BYTE__	    //�ն˵�ַ��6���ֽڳ���
    #define __PLC_REC_PLAN_FROM_F39__      //�ز�������F39�������г���
    #define __REC_DAY_HOLD_READ_SETTLEMENT_DAY_DATA__  //���������ݳ�����һ����������
    #define __PLC_REC_VOLTMETER__                      //�ز�������ѹ��
    #define __READ_07_485_DAYHOLD__                    //07��Լ485����ն��᳭��������������ʹ��0�㵱ǰֵת��
    #define __TD_IS_TODAY1__                           //ʱ���ǵ��죬����ǰһ��, ֻ�����ն����
    #define __TD_IS_CUR_MONTH__                        //ʱ���ǵ��£�����ǰ��, ֻ�����¶����
    //#define __READ_CYMETER_DAYHOLD_JINLIN_ITEM__       //���ֽ��ɳ���05000101������������Ǳ�׼����������
    #define __READ_07_485_DAYHOLD_JINLIN_ITEM__        //����485����05000101������������Ǳ�׼����������
        #endif
	#define __CHECK_POWEROFF_NEW__		 //������ͣ�ϵ��¼�
	//#define __GW_NEW_EVENT_QUERY__       //�������¼������ѯ
    #undef BAUDRATE_CAS_TO_CY
    #define BAUDRATE_CAS_TO_CY  2400

    #undef __ENABLE_SYS_TOOLS__	//ϵͳ���Թ���
    #undef __CONCENTRATOR_INITIATIVE__    //��������������ģʽ
    #undef __ENABLE_ESAM2__
    //#undef  ALLOW_WRITES
    
#endif

#ifdef __PROVICE_HEILONGJIANG__
    #define __READ_07_485_DAYHOLD__
    #define __REDUCE_485_READ_ITEM__  //����485�ĳ���������
//	#undef __CHECK_POWEROFF_NEW__		 //������ͣ�ϵ��¼�
//	#undef __GW_NEW_EVENT_QUERY__       //�������¼������ѯ    
	#define __METER_EVENT_GRADE__        //�����¼��ȼ�����
#endif

#ifdef __PROVICE_SHANDONG__       //ɽ��
    #define __READ_07_485_DAYHOLD__
    //#define __PLC_REC_CUR_WHEN_DAYHOLD_DENY__   //07�ز����ն�������ʱ���ĳ���ǰ����
    #define __QGDW2013_VRATE__   //ɽ��ʹ��2013Э��ĵ�ѹ�ϸ���
    #define __CHECK_07_DAYHOLD_TIME__           //�����ն���ʱ�䣬ʱ�䲻�ԵĻ��������ն���
    #define  __PLC_READ_VIP_F25__
    //#define __PLC_READ_VIP_V__                //ɽ��ֻ����ѹ
//    #define __MAX_485METER_COUNT_64__
    #define __GET_DH_AMOUNT_FROM_REAL__     //��������ն����������ʵʱֵȥ���㣬�������ն��ᣬ��ʱ����ʹ�ã���485���
    #define __VOLTAGE_MONITOR__      //��ѹ����
    #define __PLC_PRIOR_READ_C1DATA__      //�ز���Ԥ��һ������
#endif
#ifdef __PROVICE_SHAANXI__       //����
    //#define __PLC_REC_CUR_WHEN_DAYHOLD_DENY__   //07�ز����ն�������ʱ���ĳ���ǰ����
#endif
#ifdef __PROVICE_NEIMENG__
//�Ƿ���ݸ���2004��Լ��2�ֽڣУף�
   #define __ALLOW_QGDW129_2004__PROTOCOL__
   #define __PWLEN_IS_2BYTE__
#endif

#ifdef __PROVICE_DELIX__
    #define __PLC_READ_VIP_F25__                //�ز������й��޹����ʡ���ѹ����������ֻ����ѹ������
	#define __UPDATE_F170_D7__					//����F170��D7λ���������ʾ
    #define __COMPUTE_XLOST__
    
#endif
#ifdef __PROVICE_YUNNAN_FK__
   
	#ifdef __CHECK_POWEROFF_NEW__		 //������ͣ�ϵ��¼�
    #undef __CHECK_POWEROFF_NEW__
    #endif
	#ifdef __GW_NEW_EVENT_QUERY__       //�������¼������ѯ
    #undef __GW_NEW_EVENT_QUERY__
    #endif
   #define __QGDW_FK2005__
   #define __PWLEN_IS_2BYTE__
   #define __READ_EDMI_METER__
#endif
#ifdef __PROVICE_YUNNAN_AT__
  #define __UPDATE_NO_CHECK_DATE__ //�������ж������Ƿ���ǰ
  #undef __ENABLE_ESAM2__
  #define __HENAN_STATION__
  #define __GLYS_SP__     //97�����������⴦�����ܰ��鳭
#endif

#ifdef __HENAN_STATION__
  #define __UPDATE_NO_CHECK_DATE__ //�������ж������Ƿ���ǰ
  #undef __ENABLE_ESAM2__
#endif

#ifdef __PROVICE_LIAONING__
   //#define __COMPUTE_XLOST__
   #undef __USE_DUAL_GPRS_CLIENT__
   #undef __LOST_CHECK__
  // #define __LAOBIAO_TEST_1min_report__//����������ϱ�1���ӣ�Ҫ��__QGDW_CHECK__����������ʱ������ԭ��δ����
 //  #define __QGDW_CHECK__
    #define __RECONNECT_MSA_AFTER_CHANGE__//�޸Ĳ���������ִ�У���F3
    #define __PLC_READ_VIP_F25__           //�ص������ѹ�����������޹�����
	#define __PLC_READ_VIP_I__
    #define __VOLTAGE_MONITOR__           //��ѹ����
    #define __PRECISE_TIME__                    //��ȷ��ʱ
    #define __CAST_DELAY_UNIT__                 //�㲥Уʱ��λ#endif
    #define __PRECISE_TIME_TEST__                    //��ȷ��ʱ
    //#define __METER_CURVE_READ__         //�������߳���
    #define __METER_EVENT_GRADE__        //�����¼��ȼ�����
    #define __METER_EVENT_GRADE_TEST__
    #define __REPORT_NODE_EVENT__        //��������¼������ϱ�
    #define __METER_CURVE_READ__         //�������߳���
    #define __ERC_41__

//�ռ䲻�����ر�
    #undef __READ_HENGTONG_METER__				 //��ͨ��Լ֧��
    #undef __READ_EDMI_METER__					 //�����Լ֧��
    #undef __ENABLE_SYS_TOOLS__	//ϵͳ���Թ���
    #undef __CONCENTRATOR_INITIATIVE__    //��������������ģʽ
    #undef __ENABLE_ESAM2__
    #undef  ALLOW_WRITES
    #undef __F39_DAYHOLD_F210__           //�ն��Ṻ�õ���Ϣ   
#endif

#ifdef __DISPLAY_QINGHAI__ 
  #undef __DISPLAY_IN_CHINESE_ZJWS__
  #ifndef __DISPLAY_IN_CHINESE__
  #define __DISPLAY_IN_CHINESE__
  #endif 
#endif

#if defined(__GPRS_ME3000__)
  #undef __GPRS_TELIT__
  #undef __GPRS_M72__ 
  #undef __GPRS_MC8331A__ 
#endif

//����Э��ѡ��ѡ��һ��

//����DL698
#define  __DL698__

#define __QGW376_1__

//����������������Э��
//#define  __BJFXXC__

//����Լ
//#define  __TJ2008__

//�ӱ�ʡ������˾��Լ
//#define __HBPLC__



//ͨ�Ŷ˿ڶ���
//COMMPORT_PLC  �ڲ����ϵȼ۵�COMPORT_PLC_REC
/*
#if defined (__PROVICE_YUNNAN__)
  #define COMMPORT_485_REC   0x01     //����
  #define COMMPORT_485_CAS   0x02     //����
  #define COMMPORT_485_CY    0x03     //�����ڱ�
  #define COMMPORT_PLC       0x1F     //�ز����ö˿ڣ�����ʹ������˿�
  #define COMMPORT_PLC_REC   0x04     //�ز�����˿ڣ��������ڲ�ʹ�õĶ˿�
#elif defined (__PROVICE_CHONGQING__)
*/
#if ( (defined __PROVICE_CHONGQING__) && !(defined __QGDW_376_2013_PROTOCOL__) )//����09�淶ʹ����������
  #define COMMPORT_485_REC   0x01     //����
  #define COMMPORT_485_CAS   0x02     //����
  #define COMMPORT_485_CY    0x03     //�����ڱ�
  #define COMMPORT_PLC       0x1F     //�ز����ö˿ڣ�����ʹ������˿�
  #define COMMPORT_PLC_REC   0x04     //�ز�����˿ڣ��������ڲ�ʹ�õĶ˿�
#else
  #define COMMPORT_485_CY    0x01     //�����ڱ�
  #define COMMPORT_485_REC   0x02     //����
  #define COMMPORT_485_CAS   0x03     //����
  #define COMMPORT_PLC       0x1F     //�ز����ö˿ڣ�����ʹ������˿�
  #define COMMPORT_PLC_REC   0x04     //�ز�����˿ڣ��������ڲ�ʹ�õĶ˿�
#endif

//------------------�㽭���涨��
//#define __DISPLAY_FOOTER_NO_GPRSSTATUS__      //�ײ�״̬���Ƿ���GPRS״̬
//#define __DISPLAY_HEADER_NORM_GPRSSTATUS__    //����GPRSָʾʹ�ñ�־ģʽ
//#define __DISPLAY_FOOTER_JIANGSU_GPRSSTATUS__ //�ײ�״̬��ʹ�÷��
//#define __DISPLAY_MENUITEM_CURSOR3__          //��ǰ�˵�����
//#define __DISPLAY_GPRS_SIGNAL1__              //GPRS�ź�ǿ�ȷ��(Ĭ��ϸ��)
//#define __DISPLAY_FOOTER_LINE1__        //�ײ���ʾ����ʾһ������(Ĭ����ʾϸ��)
//#define __DISPLAY_HEADER_LINE1__        //����״̬����ʾһ������(Ĭ����ʾϸ��)
//#define __DISPLAY_LCD_TEST1__           //Һ�����Լ����,���Ʒ��
//#define __DISPLAY_SUBTITLE1__           //�������м���ʾ��������

//��һ�����
#ifdef __DISPLAY_ZHEJIANG__
#define __DISPLAY_QINGHAI__
#endif        

#ifdef __DISPLAY_ZHEJIANG1__
#undef __DISPLAY_QINGHAI__
#define __DISPLAY_GW1__
#define __DISPLAY_FOOTER_JIANGSU_GPRSSTATUS__
#endif        

#ifdef __DISPLAY_ZHEJIANG2__
#undef __DISPLAY_QINGHAI__
#define __DISPLAY_GW2__
#endif        

#ifdef __DISPLAY_ZHEJIANG3__
#undef __DISPLAY_QINGHAI__
#define __DISPLAY_GW3__
#endif        

//�ڶ������
//����
#ifdef __DISPLAY_ZHEJIANG4__
#define __DISPLAY_QINGHAI__
#define __DISPLAY_FOOTER_NO_GPRSSTATUS__
#define __DISPLAY_MENUITEM_CURSOR1__
#define __DISPLAY_SUBTITLE1__
#define __DISPLAY_LCD_TEST2__ 
#endif        

//����
#ifdef __DISPLAY_ZHEJIANG5__
#undef __DISPLAY_QINGHAI__
#define __DISPLAY_GW1__
#define __DISPLAY_FOOTER_NO_GPRSSTATUS__
#define __DISPLAY_MENUITEM_CURSOR1__
#define __DISPLAY_SUBTITLE1__ 
#define __DISPLAY_LCD_TEST2__ 
#endif        

//����
#ifdef __DISPLAY_ZHEJIANG6__
#undef __DISPLAY_QINGHAI__
#define __DISPLAY_GW2__
#define __DISPLAY_FOOTER_NO_GPRSSTATUS__
#define __DISPLAY_MENUITEM_CURSOR1__
#define __DISPLAY_SUBTITLE1__ 
#define __DISPLAY_LCD_TEST2__ 
#endif        

//����
#ifdef __DISPLAY_ZHEJIANG7__
#undef __DISPLAY_QINGHAI__
#define __DISPLAY_GW3__
#define __DISPLAY_FOOTER_NO_GPRSSTATUS__
#define __DISPLAY_MENUITEM_CURSOR1__
#define __DISPLAY_SUBTITLE1__ 
#define __DISPLAY_LCD_TEST2__ 
#endif        

//���������
//����
#ifdef __DISPLAY_ZHEJIANG8__
#define __DISPLAY_QINGHAI__
#define __DISPLAY_FOOTER_NO_GPRSSTATUS__
#define __DISPLAY_MENUITEM_CURSOR2__
#define __DISPLAY_HEADER_NORM_GPRSSTATUS__
#define __DISPLAY_GPRS_SIGNAL1__
#define __DISPLAY_SUBTITLE2__ 
#define __DISPLAY_HEADER_LINE1__
#define __DISPLAY_FOOTER_LINE1__
#define __DISPLAY_LCD_TEST4__ 
#endif        

//����
#ifdef __DISPLAY_ZHEJIANG9__
#undef __DISPLAY_QINGHAI__
#define __DISPLAY_GW1__
#define __DISPLAY_FOOTER_NO_GPRSSTATUS__
#define __DISPLAY_HEADER_NORM_GPRSSTATUS__
#define __DISPLAY_MENUITEM_CURSOR2__
#define __DISPLAY_GPRS_SIGNAL1__
#define __DISPLAY_SUBTITLE2__ 
#define __DISPLAY_LCD_TEST4__ 
#endif        

//����
#ifdef __DISPLAY_ZHEJIANG10__
#undef __DISPLAY_QINGHAI__
#define __DISPLAY_GW2__
#define __DISPLAY_FOOTER_NO_GPRSSTATUS__
#define __DISPLAY_HEADER_NORM_GPRSSTATUS__
#define __DISPLAY_MENUITEM_CURSOR2__
#define __DISPLAY_GPRS_SIGNAL1__
#define __DISPLAY_SUBTITLE2__ 
#define __DISPLAY_HEADER_LINE1__
#define __DISPLAY_FOOTER_LINE1__
#define __DISPLAY_LCD_TEST4__ 
#endif        

//����
#ifdef __DISPLAY_ZHEJIANG11__
#undef __DISPLAY_QINGHAI__
#define __DISPLAY_GW3__
#define __DISPLAY_FOOTER_NO_GPRSSTATUS__
#define __DISPLAY_HEADER_NORM_GPRSSTATUS__
#define __DISPLAY_MENUITEM_CURSOR2__
#define __DISPLAY_GPRS_SIGNAL1__
#define __DISPLAY_SUBTITLE2__ 
#define __DISPLAY_HEADER_LINE1__
#define __DISPLAY_FOOTER_LINE1__
#define __DISPLAY_LCD_TEST4__ 
#endif        


#if defined(STACK_USE_GPRS_PPP) &&  defined(__USE_SMS__)
    #define STACK_USE_SMS
#endif

#ifdef __PLC_READ_VIP_F25__
	#define __PLC_READ_VIP_V__
	//#define __PLC_READ_VIP_I__
	//#define __PLC_READ_VIP_YP__
	//#define __PLC_READ_VIP_WP__
	//#define __PLC_READ_VIP_Q__
        //#define __PLC_READ_VIP_WG_4XX__
#endif

#if defined(__PLC_READ_VIP_LOAD_RECORD__)
    #define __PLC_READ_VIP_YP__
    #define __PLC_READ_VIP_WP__
    #define __PLC_READ_VIP_V__
    #define __PLC_READ_VIP_I__
    #define __PLC_READ_VIP_Q__
    #define __PLC_READ_VIP_WG_4XX__
#endif

//�������
#ifdef __COMPUTE_XLOST__
    #define __COMPUTE_XLOST_30DAY__
    #define __COMPUTE_PHASE_XLOST__
    #ifndef __PROVICE_ZHEJIANG__ 
//    #define __LOST_CHECK__           //ֻ��һ�α�
    #endif
#endif

//���ɹ㲥���ظ�
//#define  __ADJ_ACK__


#ifdef __SOFT_SIMULATOR__
#undef __ENABLE_ESAM2__
#undef __ENABLE_ESAM__
#endif

#ifdef BASE_STATION_CHANGE
#define CPIN_CHECK    		  1
#define UN_LOCK_CHANNEL       2
#endif

#endif  // fule


