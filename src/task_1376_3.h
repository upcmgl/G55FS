#ifndef __TASK_1376_3_H__
#define __TASK_1376_3_H__

#define ONLINE_MODE_MIXED   0   //���ģʽ
#define ONLINE_MODE_CLIENT  1   //�ͻ���ģʽ
#define ONLINE_MODE_SERVER  2   //������ģʽ

void remote_send_app_frame(uint8_t *frame,uint16_t frame_len);
void remote_send_app_frame2(uint8_t *frame,uint16_t frame_len);
void remote_server_send_app_frame(uint8_t *frame,uint16_t frame_len);
void task_1376_3_service(void);
int16_t remote_read_byte(void);
int16_t remote_read_byte2(void);
int16_t remote_server_read_byte(void);
void force_gprs_power_off(void);
void update_gprs_param(void);
#ifdef __EDP__
INT8U cmdType;
INT32U cmd645;
INT8U autoReportType;
INT8U wait_report_flag; //��ʱ�ϱ���־λ��
INT8U warning_report_cmd;  //���ڸ澯��Ϣ�ϱ������
void storeUnsuccessData();
#endif
#endif