#ifndef __PLMS_READ_DATA_C1_H__
#define __PLMS_READ_DATA_C1_H__


//��N���ֽ�λ��
#define  MASK_DAYHOLD_D0    0x00
//#define  MASK_HOLD_D0    0x01
//#define  MASK_HOLD_D0    0x10
//#define  MASK_HOLD_D0    0x11
#define  MASK_CURVE         0x04




//Fn���ݽṹ
typedef struct
{
    INT32U  item;         //������
    INT8U   offset;       //ƫ����
    INT8U   len;          //���ݳ��� (����Э�鳤��1376.1)
    INT8U   num;          //����������� ����ʱ����Ҫʹ�ã�
    INT8U   prop;         //��N���ֽ�λ��  D0-D1��0��D0�ֽ� 1 ��D1�ֽ� 2 ��D2�ֽ� 3��D4�ֽڣ�D2: 1 ��������   D3-D7����
    INT16U  format;       //ת������ ��07��1376.1ת��������D15��1����BCD 0��Ĭ��BCD��
}tagFN_ITEM;
typedef union{
    INT8U value[73];
    struct{
        INT8U  read_num;//��������
        INT8U  read_point[16];//�����������ݶ���󳭶�16�Σ�����3��ÿ15minһ���㣬��󳭶�7��
        INT8U  start_time[5];
        INT8U  data_len[16];
        INT8U  data_midu; 
        INT16U work_flag;
        INT16U  offset[16];
    };
}READ_FHJU;


INT16S  app_read_his_item(INT32U item,INT8U* request,INT8U* resp,INT16U resp_left_len,INT8U* item_tmp,INT16S left_time_ms);
INT16U  app_get_fn_data_c2(objRequest *pRequest,objResponse *pResp);
INT16U   convert_645_to_376(INT16U format,INT8U *pResp,INT8U *param);
void app_prcess_inter_data(objRequest *pRequest,objResponse *pResp,tagFN_ITEM *pFn);
INT32U  app_get_item_with_td(INT32U item,INT8U item_prop,INT8U *td,INT8U hold_flag);
INT8U trans_C2_curve_midu(READ_CONTEXT *readC2);
INT8U get_read_param(READ_FHJU* read_fhju,INT8U midu,INT8U point,INT16U total_len,INT8U point_len);
#endif



