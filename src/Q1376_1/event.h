/*
 * event.h
 *
 * Created: 2019-4-19 20:32:18
 *  Author: mgl
 */ 

#include "main_include.h"
#ifndef EVENT_H_
#define EVENT_H_
#define INVALID_EC_SEQ                   0xFFFFFFFF

#define EVENT_POS_SEQ                    0      //4,�¼����
#define EVENT_POS_RPT_FLAG               4      //1,�¼���־,D0-��ʾ��Ҫ�¼�/һ���¼�,D1��ʾ�Ƿ���Ҫ�ϱ�D2��ʾ�Ƿ��Ѿ��ϱ�
#define EVENT_POS_EC                     5      //1,����ʱ���¼�������
#define EVENT_POS_ERC_NO                 6      //1,�¼�����
#define EVENT_POS_ERC_LEN                7      //1�¼�����
#define EVENT_POS_ERC_DATA               8      //n�¼�����
#define EVENT_POS_ERC_TIME               8      //5���¼�����ʱ��
#define EVENT_POS_ERC_CONTENT            13      //N���¼�����

#define EVENT_MAX_SAVE_COUNT             15    //�����¼����洢����
#define EVENT_RECORD_SIZE               152

#define EVENT_HEAD_LEN                   6      //�¼���¼ͷ����
#define G_METER_ERC_RECORD_MAX        64        //ERC��¼������ʱ����Ϊ64����׼Э��41��,
#define EVENT_ERC_SECTOR_MAX          70       //ERC�¼���¼sector���������ֵ

#define EVENT_TYPE_BASE         0x00    //��ͨ�¼�
#define EVENT_TYPE_VIP          0x01    //��Ҫ�¼�
#define EVENT_TYPE_ALL          0x02    //��ͨ����Ҫ�¼�

typedef struct
{
	
}tagERCF1;
typedef struct
{
	
}tagERCF2;
typedef struct
{
	INT8U sector_index[G_METER_ERC_RECORD_MAX];             //��¼ÿ��erc�¼�������sector����Χ��0��39
	INT8U erc_cnt[G_METER_ERC_RECORD_MAX];					//��¼ÿ��erc�¼��ĸ���
	INT8U sector_free_index[EVENT_ERC_SECTOR_MAX-G_METER_ERC_RECORD_MAX];        //��¼������sector����ţ���Χ��0��39
}tagERCF3;
typedef struct 
{
	INT8U  ec1;                       //��Ҫ�¼�����
	INT8U  ec2;                       //һ���¼�����
	tagERCF1 ERC_F1;
	tagERCF2 ERC_F2;
	tagERCF3 ERC_F3;
}tagEVETN;
typedef struct
{
	INT8U erc_no;
	INT8U erc_cnt;
}tagERC_96bits_region;


INT8S reconstructe_eventERC_index();

void save_event_record(INT8U *event,INT8U event_flag);
void  event_params_set(INT8U *event);             //ERC3

#endif /* EVENT_H_ */