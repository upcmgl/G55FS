#ifndef __PROTOCOL_APP_CONST_DEF_H__
#define __PROTOCOL_APP_CONST_DEF_H__

#define APDU_LINK_REQUEST              0x01      //Ԥ��������      	    	[1]	 LINK-Request
#define APDU_LINK_RESPONSE             0x81      //Ԥ������Ӧ       	    [129]LINK-Response

#define APDU_SECURITY_REQUEST          0x10      //��ȫ��֤����        	[4]	SecurityRequest
#define APDU_SECURITY_RESPONSE         0x90      //��ȫ��֤��Ӧ     	    [144]	SecurityResponse

#define CLIENT_APDU_CONNECT           0x02      //����Ӧ����������    	[2]	CONNECT-Request
#define CLIENT_APDU_RELEASE           0x03      // �Ͽ�Ӧ����������    	[3]	RELEASE-Request
#define CLIENT_APDU_GET               0x05      //��ȡ����            	[5]	GET-Request
#define CLIENT_APDU_SET               0x06      //��������            	[6]	SET-Request
#define CLIENT_APDU_ACTION            0x07      //��������            	[7]	ACTION-Request
#define CLIENT_APDU_REPORT            0x08     //�ϱ�Ӧ��            	[8]	REPORT-Response
#define CLIENT_APDU_PROXY             0x09     //��������            	[9]	PROXY-Request


#define SERVER_APDU_CONNECT           0x82      //����Ӧ����������    	[130]	CONNECT-Response
#define SERVER_APDU_RELEASE_RESP      0x83      //�Ͽ�Ӧ��������Ӧ 	    [131]	RELEASE-Response
#define SERVER_APDU_RELEASE_NOTI      0x84      //�Ͽ�Ӧ������֪ͨ  	[132]	RELEASE-Notification
#define SERVER_APDU_GET               0x85      //��ȡ��Ӧ         	    [133]	GET-Response
#define SERVER_APDU_SET               0x86      //������Ӧ       	    [134]	SET-Response
#define SERVER_APDU_ACTION            0x87      //������Ӧ        	    [135]	ACTION-Response
#define SERVER_APDU_REPORT            0x88      //�ϱ�֪ͨ        	    [136]	REPORT-Notification
#define SERVER_APDU_PROXY             0x89     //������Ӧ          	    [137]	PROXY-Response
#define SERVER_APDU_ERROR             0xEE     //������Ӧ          	    [238]	ERROR-Response

#define GET_REQUEST_NORMAL                0x01      //1)	��ȡһ���������ԣ�
#define GET_REQUEST_NORMAL_LIST           0x02      //2)	��ȡ����������ԣ�
#define GET_REQUEST_RECORD                0x03      //3)	��ȡһ����¼�Ͷ������ԣ�
#define GET_REQUEST_RECORD_LIST           0x04      //4)	��ȡ�����¼�Ͷ������ԣ�
#define GET_REQUEST_NEXT                  0x05      //5)	��ȡ��֡��Ӧ����һ�����ݿ飬���ڱ������������Ӧ������һ��GET.response APDU�д���ʱ��ʹ��
#define GET_FLASH_CONTENT                 0x06      //6��   ��ȡflash���ݡ�

#define GET_RESPONSE_NORMAL                0x01      //1)	��ȡһ���������ԣ�
#define GET_RESPONSE_NORMAL_LIST           0x02      //2)	��ȡ����������ԣ�
#define GET_RESPONSE_RECORD                0x03      //3)	��ȡһ����¼�Ͷ������ԣ�
#define GET_RESPONSE_RECORD_LIST           0x04      //4)	��ȡ�����¼�Ͷ������ԣ�
#define GET_RESPONSE_NEXT                  0x05      //5)	��֡��Ӧһ�����ݿ�

#define  SET_REQUEST_NORMAL                 0x01     //����һ��������������
#define  SET_REQUEST_NORMA_LLIST            0x02     //���ö��������������
#define  SET_THEN_GET_REQUEST_NORMAL_LIST   0x03     //���ú��ȡ���������������

#define ACTION_REQUEST                       0x01     //����һ�����󷽷�����
#define ACTION_REQUEST_LIST                  0x02     //����������󷽷�����
#define ACTION_THEN_GET_REQUEST_NORMAL_LIST  0x03     //����������󷽷����ȡ���������������

#define PROXY_GET_REQUEST_LIST               0x01   //�����ȡ����������Ķ��������������
#define PROXY_GET_REQUEST_RECORD             0x02	//�����ȡһ����������һ����¼�Ͷ�����������
#define PROXY_SET_REQUEST_LIST               0x03   //�������ö���������Ķ��������������
#define PROXY_SET_THEN_GET_REQUEST_LIST      0x04   //�������ú��ȡ����������Ķ��������������
#define PROXY_ACTION_REQUEST_LIST            0x05   //�����������������Ķ�����󷽷�����
#define PROXY_ACTION_THEN_GET_REQUEST_LIST   0x06   //����������ȡ����������Ķ�����󷽷�����������
#define PROXY_TRANS_COMMAND_REQUEST          0x07   //����͸��ת����������

#define REPORT_NORMAL_LIST                   0x01   //�ϱ����ɸ���������
#define REPORT_RECORD_LIST                   0x02   //�ϱ����ɸ���¼�Ͷ�������
#define REPORT_NOT_TRAN_DATA                 0x03   //�ϱ�͸������

#define APDU_LINK_MNG_REQ               0x03      //��·����
#define APDU_LINK_MNG_RESP              0x83      //��·����
#define APDU_REQ_OBJECT_ATTRIBUTE_REQ     0x0B      //�����������
#define APDU_REQ_OBJECT_ATTRIBUTE_RESP    0x8B      //�����������
#define APDU_REQ_BATCH_OBJECT_ATTRIBUTE_REQ     0x0C      //���������������
#define APDU_REQ_BATCH_OBJECT_ATTRIBUTE_RESP    0x8C      //���������������
#define APDU_REQ_RECORD_OBJECT_ATTRIBUTE_REQ     0x0D      //�����¼�Ͷ�������
#define APDU_REQ_RECORD_OBJECT_ATTRIBUTE_RESP    0x8D      //�����¼�Ͷ�������
#define APDU_REQ_BATCH_RECORD_OBJECT_ATTRIBUTE_REQ    0x11      //���������¼�Ͷ�������
#define APDU_REQ_COMPLEX_RECORD_OBJECT_ATTRIBUTE_REQ    0x12      //���󸴺ϼ�¼�Ͷ�������
#define APDU_REQ_COMPLEX_RECORD_OBJECT_ATTRIBUTE_RESP   0x92      //���󸴺ϼ�¼�Ͷ�������
#define APDU_REQ_SEPARATE_FRAME                  0x10      //�����֡����֡
#define APDU_REQ_DATA_REPORT1                   0x04      //�����ϱ�-������֪ͨ������¼
#define APDU_REQ_DATA_REPORT2                   0x05      //�����ϱ�-������֪ͨ���ϼ�¼

#define APDU_EXEC_AND_REQ_RECORD_OBJECT_ATTRIBUTE_REQ     0x2E      //ִ�з������ȡ��¼�Ͷ���
#define APDU_EXEC_AND_REQ_RECORD_OBJECT_ATTRIBUTE_RESP    0xAE      //ִ�з������ȡ��¼�Ͷ���


#define APDU_EXEC_OBJECT_METHOD_REQ     0x29      //ִ�ж��󷽷�
#define APDU_EXEC_OBJECT_METHOD_RESP    0xA9      //ִ�ж��󷽷�
#define APDU_BATCH_EXEC_OBJECT_METHOD   0x2A      //����ִ�ж��󷽷�
#define APDU_EXEC_READ_OBJECT_METHOD    0x2B      //ִ�к��ȡ

#define APDU_SET_OBJECT_ATTRIBUTE               0x1F       //���ö�������
#define APDU_SET_OBJECT_ATTRIBUTE_RESP          0x9F       //���ö�������
#define APDU_BATCH_SET_OBJECT_ATTRIBUTE         0x20       //�������ö�������
#define APDU_BATCH_SET_OBJECT_ATTRIBUTE_RESP    0xA0       //�������ö�������
#define APDU_SETGET_OBJECT_ATTRIBUTE               0x21    //���ú��ȡ��������
#define APDU_SETGET_OBJECT_ATTRIBUTE_RESP          0xA1    //���ú��ȡ��������

#define APDU_AGENT_SET_OBJECT_ATTRIBUTE          0x22    //�������ö�������
#define APDU_AGENT_SETGET_OBJECT_ATTRIBUTE       0x23    //�������ú��ȡ��������
#define APDU_AGENT_GET_OBJECT_ATTRIBUTE          0x0E   //���������������
#define APDU_AGENT_GET_RECORD_OBJECT_ATTRIBUTE   0x0F   //���������¼�Ͷ�������
#define APDU_AGENT_EXEC_OBJECT_METHOD            0x2C   //����ִ�ж��󷽷�
#define APDU_AGENT_EXEC_READ_OBJECT_METHOD       0x2E   //����ִ�к��ȡ
#define APDU_EVENT_NOTIFY1                        0x04   //�¼�������֪ͨ
#define APDU_EVENT_NOTIFY2                        0x05   //�¼�������֪ͨ


#define CTRLFUNC_DIR_SET        0x80       //���ô��䷽��λDIR=1    ʹ��|
#define CTRLFUNC_DIR_CLR        0x7F       //���ô��䷽��λDIR=0    ʹ��&
#define CTRLFUNC_PRM_SET        0x40       //����������־λPRM=1    ʹ��|
#define CTRLFUNC_PRM_CLR        0xBF       //����������־λPRM=0    ʹ��&
#define CTRLFUNC_SC_SET         0x08       //���������־λSC=1     ʹ��|
#define CTRLFUNC_SC_CLR         0xF7       //���������־λSC=0     ʹ��&
#define CTRLFUNC_FC_LINK_MNG    0x01       //���ù�����        ʹ��|
#define CTRLFUNC_FC_REQ_RESP    0x03       //���ù�����        ʹ��|
#define CTRLFUNC_FC_REPORT      0x04       //���ù�����        ʹ��|
#define CTRL_FUNC_USERDATA      0x03        //�û�����

//#define ATTRIBUTE_ALL           0xFF
////#define MAX_EVENT_COUNT         0x10       //Ŀǰ֧������¼�����
//#define SFF_FIRST_FRAME         0x01       //��֡��־����֡��־
//#define SFF_LAST_FRAME          0x02       //��֡��־��ĩ֡��־
//#define SFF_BLOCK_KIND          0x04       //��֡��־,�����֡
//
//#define SERVER_ADDR_TYPE_FIXED  0xA0       //��·���������ַ�̶�����
//#define SERVER_ADDR_TYPE_VAR    0xB0       //��·���������ַ�ɱ䳤��

//���ݴ��ڽ��DAR
#define DAR_SECCESS                         0       //�ɹ�
#define DAR_HARDWARE_FAULT                  1       //Ӳ��ʧЧ
#define DAR_TEMPORARY_FAILURE               2       //��ʱʧЧ
#define DAR_READ_WRITE_DENIED               3       //�ܾ���д
#define DAR_OBJECT_UNDEFINED                4       //����δ����
#define DAR_OBJECT_CLASS_INCONSISTENT       5       //����ӿ��಻����
#define DAR_OBJECT_UNAVAILABLE              6       //���󲻴���
#define DAR_TYPE_UNMATCHED                  7       //���Ͳ�ƥ��
#define DAR_SCOPE_OF_ACCESS_VIOLATED        8       //Խ��
#define DAR_DATA_BLOCK_UNAVAILABLE          9       //���ݿ鲻����
#define DAR_LONG_GET_ABORTED                10      //��֡������ȡ��
#define DAR_NO_LONG_GET_IN_PROGRESS         11      //�����ڷ�֡����״̬
#define DAR_LONG_WRITE_ABORTED              12      //��дȡ��
#define DAR_NO_LONG_WRITE_IN_PROGRESS       13      //�����ڿ�д״̬
#define DAR_DATA_BLOCK_NUMBER_INVALID       14      //���ݿ������Ч
#define DAR_UNAUTHORIZED_ACCESS             15      //�������δ��Ȩ
#define DAR_COMM_RATE_NO_CHANGE             16      //ͨ�����ʲ��ܸ���
#define DAR_YEAR_TIME_ZONE_COUT_OVER        17      //��ʱ������
#define DAR_DAY_TIME_INTERVAL_COUNT_VOER    18      //��ʱ������
#define DAR_RATE_EXCEEDED                   19      //��������
#define DAR_SECURITY_AUTH_UNMATCHED         20      //��ȫ��֤��ƥ��
#define DAR_DUPLICATE_RECHARGE              21      //�ظ���ֵ
#define DAR_ESAM_VERIFY_FAIL                22      //ESAM��֤ʧ��
#define DAR_SECURITY_AUTH_FAIL              23      //��ȫ��֤ʧ��
#define DAR_CUST_NO_UNMATCHED               24      //�ͻ���Ų�ƥ��
#define DAR_RECHARGE_COUNT_ERROR            25      //��ֵ��������
#define DAR_BUY_FEE_OVER                    26      //���糬�ڻ�
#define DAR_ADDRESS_EXCEPT                  27      //��ַ�쳣
#define DAR_SYMMETRIC_DECRYPT_ERROR         28      //�Գƽ��ܴ���
#define DAR_ASYMMETRIC_DECRYPT_ERROR        29     //�ǶԳƽ��ܴ���
#define DAR_SIGNATURE_ERROR                 30      //ǩ������
#define DAR_METER_HANGING                   31      //���ܱ����
#define DAR_TIME_TAG_INVALID                32      //ʱ���ǩ��Ч
#define DAR_TIME_OUT                        33      //����ʱ
#define DAR_ESAM_P1P2_ERROR                 34      //ESAM��P1P2����ȷ
#define DAR_ESAM_LC_ERROR                   35      //ESAM��LC����
#define DAR_OTHER_REASON                    255     //����

#define SYS_POWER_LOST             0x00000001   //��Դ�����־
#define SYS_GPRS_CHG               0x00000002   //GPRS������������
#define SYS_RST_GPRS               0x00000004   //GPRS��λ��־
#define SYS_PRG_RESET              0x00000008   //ϵͳ�����λ��־
#define SYS_PLC_REC_CJQ            0x00000010   //�ز�����ʱ�Ƿ����βɼ�������    0:���ղɼ�����ʽ����  1�����ε����еĲɼ�����ַ�����ձ�ų���
#define SYS_ESAM                   0x00000020   //�Ƿ�ʹ��ESAM��mac��֤   0:��ʹ��;1:ʹ��
#define SYS_CY_METER               0x00000040   //�Ƿ��н���Ӳ��   1:��;0:û��
#define SYS_READ_485_CY            0x00000040   //�Ƿ��н���Ӳ��   1:��;0:û��
#define SYS_PROG_KEY               0x00000080   //�Ƿ��˱�̼�
#define SYS_CY_RESET               0x00000100   //���ɷ����˸�λ   1������ 0��δ����
#define SYS_CY_CLR_DATA            0x00000200   //���ɷ����������¼�   1������ 0��δ����
#define SYS_CY_CLOCK_LOST          0x00000400   //����ʱ�Ӷ�ʧ  1:����  0:δ����
#define SYS_KEY_WAKEUP             0x00000800  //��������״̬  1-�ڰ�������״̬��0-���ڰ�������״̬
#define SYS_CY_PAHSE_LINE          0x00001000   //���߱�־��0-������ 1-������
#define SYS_FRAME_ACD_FLAG         0x00002000   //������ACD��־�����־
#define SYS_CLOCK_LOST	  	       0x00004000   		//ʱ��ʧЧ   1������ 0��δ����
#define SYS_CLOCK_DOUBT	  	       0x00008000   		//ʱ������   1������ 0��δ����
#define SYS_RUN_STATUS             0x00010000   //������Ͷ��״̬ 1:Ͷ�� 0:δͶ  ������չ __PROVICE_CHONGQING__
#define SYS_ESAM_READMETER         0x00020000   //ESAM����

//
//
//#define PRJ_NORMAL_TASK                         1   //��ͨ�ɼ�����
//#define PRJ_EVENT_TASK                          2   //�¼��ɼ�����
//#define PRJ_RELAY_TASK                          3   //͸������
//#define PRJ_REPORT_TASK                         4   //�ϱ�����
//#define PRJ_SCRIPT_TASK                         5   //�ű�����

typedef struct{
    INT8U *request_frame;
    INT8U *resp_frame;
    INT8U *security_info;
    INT8U *param;
    INT16U request_pos;
    INT16U request_data_pos;
    INT16U resp_pos;
    INT16U left_len;
    INT16U separate_flalg_pos;
    INT16U addition_pos;
    INT16U userDataLen;
    INT16U frame_len;
    INT8U  query_record_count;//Ĭ�ϲ�ѯ��¼����0Ϊȫ��������Ϊָ����¼����Ŀǰ�����¼��ϱ���ÿ��ֻ��һ���¼� 
}OBJECT_REQ_CONTEXT;
#endif

