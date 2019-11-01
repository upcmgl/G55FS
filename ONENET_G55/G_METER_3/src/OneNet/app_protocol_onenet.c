///////////////////////////////////////////////////////////////////////////////
//
//   onenet Э�鴦��  
//
///////////////////////////////////////////////////////////////////////////////

#include "../main_include.h"


typedef union
{
    INT16U value;
    struct{
        INT8U convert_type:8;//ת��������
        INT8U type:4;//�������ݸ�ʽ
        INT8U report_type:3;//�ϱ����� DAY_HOLD:001  CUR_DATA 010 MONTHHOLD 100
        INT8U ret:1;//����
    };
}tagDataType; 

typedef struct{
    INT16U obj;
    INT8U instance;
    INT16U id;
    INT32U item;
    tagDataType data_type;//��������1:
    INT8U  len;
}tagResourceId;




typedef union{
    INT8U  value[9];
    struct{
       tagDatetime      monthhold_report_time;     //��һ���¶����ϱ�ʱ��
       tagDatetime      dayahold_report_time;     //��һ���ն����ϱ�ʱ��
       tagDatetime      cycle_report_time;     //��һ�ε�ǰ�����ϱ�ʱ��
       INT16U           rpt_flag;//��Ҫ�ϱ������ݱ�־
       INT16U           rpt_idx;//resource id���е�idx 
       INT16U           ack_idx;//ƽ̨�ظ�idx
    };
}TASK_INFO;


const tagResourceId  RESOURCE_ID[] = 
{
//   object,    instance,id,                item,               type,                       len
 #ifdef __METER_SIMPLE__
{	3316,       0,      5700,               0x02010100,          CYCLE_DATA|FLOAT_DATA|V_FLOAT,   2}, //A���ѹUa
{	3316,       0,      5701,               0xFFFFFFFF,          CYCLE_DATA|STRING_DATA|V_STRING,   1}, //A���ѹ��λ V
#else
{	3316,       0,      5700,               0x02010100,          CYCLE_DATA|FLOAT_DATA|V_FLOAT,   2}, //A���ѹUa
{	3316,       0,      5701,               0xFFFFFFFF,          CYCLE_DATA|STRING_DATA|V_STRING,   1}, //A���ѹ��λ V
{	3316,       1,      5700,               0x02010200,          CYCLE_DATA|FLOAT_DATA|V_FLOAT,   2}, //B���ѹUb
{	3316,       1,      5701,               0xFFFFFFFF,          CYCLE_DATA|STRING_DATA|V_STRING,   1},//B���ѹ��λV
{	3316,       2,      5700,               0x02010300,          CYCLE_DATA|FLOAT_DATA|V_FLOAT,   2}, //C���ѹUc
{	3316,       2,      5701,               0xFFFFFFFF,          CYCLE_DATA|STRING_DATA|V_STRING,   1},//C���ѹ��λV
#endif
#ifdef __METER_SIMPLE__
{	3317,       0,      5700,               0x02020100,          CYCLE_DATA|FLOAT_DATA|A_FLOAT,   3}, //A�����Ia  
{	3317,       0,      5701,               0xFFFFFFFF,          CYCLE_DATA|STRING_DATA|A_STRING,   1}, //A�������λ A
{	3317,       3,      5700,               0x02800001,          CYCLE_DATA|FLOAT_DATA|A_FLOAT,   3}, //�������I0 
{	3317,       3,      5701,               0xFFFFFFFF,          CYCLE_DATA|STRING_DATA|A_STRING,   1},//���������λA
#else
{	3317,       0,      5700,               0x02020100,          CYCLE_DATA|FLOAT_DATA|A_FLOAT,   3}, //A�����Ia  
{	3317,       0,      5701,               0xFFFFFFFF,          CYCLE_DATA|STRING_DATA|A_STRING,   1}, //A�������λ A
{	3317,       1,      5700,               0x02020200,          CYCLE_DATA|FLOAT_DATA|A_FLOAT,   3}, //B�����Ib
{	3317,       1,      5701,               0xFFFFFFFF,          CYCLE_DATA|STRING_DATA|A_STRING,   1},//B�������λA
{	3317,       2,      5700,               0x02020300,          CYCLE_DATA|FLOAT_DATA|A_FLOAT,   3}, //C�����Ic
{	3317,       2,      5701,               0xFFFFFFFF,          CYCLE_DATA|STRING_DATA|A_STRING,   1},//C�������λA
{	3317,       3,      5700,               0x02800001,          CYCLE_DATA|FLOAT_DATA|A_FLOAT,   3}, //�������I0 
{	3317,       3,      5701,               0xFFFFFFFF,          CYCLE_DATA|STRING_DATA|A_STRING,   1},//���������λA
#endif
#ifdef __METER_SIMPLE__
{	3328,       0,      5700,               0x02030000,          CYCLE_DATA|FLOAT_DATA|P_FLOAT,   3}, //���й����� 
{	3328,       0,      5701,               0xFFFFFFFF,          CYCLE_DATA|STRING_DATA|KW_STRING,   2}, //���й����� KW
{	3328,       1,      5700,               0x02030100,          CYCLE_DATA|FLOAT_DATA|P_FLOAT,   3}, //A���й�����Pa  
{	3328,       1,      5701,               0xFFFFFFFF,          CYCLE_DATA|STRING_DATA|KW_STRING,   2}, //A���й����ʵ�λ KW
#else
{	3328,       0,      5700,               0x02030000,          CYCLE_DATA|FLOAT_DATA|P_FLOAT,   3}, //���й����� 
{	3328,       0,      5701,               0xFFFFFFFF,          CYCLE_DATA|STRING_DATA|KW_STRING,   2}, //���й����� KW
{	3328,       1,      5700,               0x02030100,          CYCLE_DATA|FLOAT_DATA|P_FLOAT,   3}, //A���й�����Pa  
{	3328,       1,      5701,               0xFFFFFFFF,          CYCLE_DATA|STRING_DATA|KW_STRING,   2}, //A���й����ʵ�λ KW
{	3328,       2,      5700,               0x02030200,          CYCLE_DATA|FLOAT_DATA|P_FLOAT,   3}, //B���й�����Pb
{	3328,       2,      5701,               0xFFFFFFFF,          CYCLE_DATA|STRING_DATA|KW_STRING,   2},//B���й����ʵ�λ KW
{	3328,       3,      5700,               0x02030300,          CYCLE_DATA|FLOAT_DATA|P_FLOAT,   3}, //C���й�����Pc
{	3328,       3,      5701,               0xFFFFFFFF,          CYCLE_DATA|STRING_DATA|KW_STRING,   2},//C���й����ʵ�λ KW
#endif
#ifndef __METER_SIMPLE__
{	3328,       4,      5700,               0x02040000,          CYCLE_DATA|FLOAT_DATA|P_FLOAT,   3}, //���޹����� 
{	3328,       4,      5701,               0xFFFFFFFF,          CYCLE_DATA|STRING_DATA|KVar_STRING,   4}, //���޹����� KW
{	3328,       5,      5700,               0x02040100,          CYCLE_DATA|FLOAT_DATA|P_FLOAT,   3}, //A���޹�����Qa  
{	3328,       5,      5701,               0xFFFFFFFF,          CYCLE_DATA|STRING_DATA|KVar_STRING,   4}, //A���޹����ʵ�λ kvar
{	3328,       6,      5700,               0x02040200,          CYCLE_DATA|FLOAT_DATA|P_FLOAT,   3}, //B���޹�����Qb
{	3328,       6,      5701,               0xFFFFFFFF,          CYCLE_DATA|STRING_DATA|KVar_STRING,   4},//BA���޹����ʵ�λ kvar
{	3328,       7,      5700,               0x02040300,          CYCLE_DATA|FLOAT_DATA|P_FLOAT,   3}, //C���޹�����Qc
{	3328,       7,      5701,               0xFFFFFFFF,          CYCLE_DATA|STRING_DATA|KVar_STRING,   4},//CA���޹����ʵ�λ kvar
#endif
#ifdef __METER_SIMPLE__
{	3329,       0,      5700,               0x02060000,          CYCLE_DATA|FLOAT_DATA|FACTOR_FLOAT,   4}, //�ܹ�������
{	3329,       1,      5700,               0x02060100,          CYCLE_DATA|FLOAT_DATA|FACTOR_FLOAT,   4}, //��������facA
#else
{	3329,       0,      5700,               0x02060000,          CYCLE_DATA|FLOAT_DATA|FACTOR_FLOAT,   4}, //�ܹ�������
{	3329,       1,      5700,               0x02060100,          CYCLE_DATA|FLOAT_DATA|FACTOR_FLOAT,   4}, //��������facA
{	3329,       2,      5700,               0x02060200,          CYCLE_DATA|FLOAT_DATA|FACTOR_FLOAT,   4}, //��������facB  
{	3329,       3,      5700,               0x02060300,          CYCLE_DATA|FLOAT_DATA|FACTOR_FLOAT,   4}, //��������facC
#endif
#ifdef __METER_SIMPLE__
{	3331,       0,      5805,               0x00010000,          CYCLE_DATA|FLOAT_DATA|KWH_FLOAT,   4}, // ��ǰ�����й�����ʾֵ
{	3331,       0,      5701,               0xFFFFFFFF,          CYCLE_DATA|STRING_DATA|KWH_STRING,   3}, //��ǰ�����й�����ʾֵ��λ KWh
{	3331,       2,      5805,               0x00020000,          CYCLE_DATA|FLOAT_DATA|KWH_FLOAT,   4}, // ��ǰ�����й�����ʾֵ
{	3331,       2,      5701,               0xFFFFFFFF,          CYCLE_DATA|STRING_DATA|KWH_STRING,   3}, //��ǰ�����й�����ʾֵ��λ KWh
{	3331,       4,      5805,               0x05060101,          DAYHOLD_DATA|FLOAT_DATA|KWH_FLOAT,   4}, // (��1 ��)�ն��������й��ܵ���
{	3331,       4,      5701,               0xFFFFFFFF,          DAYHOLD_DATA|STRING_DATA|KWH_STRING,   3}, //(��1 ��)�ն��������й��ܵ��� KWh
{	3331,       5,      5805,               0x05060201,          DAYHOLD_DATA|FLOAT_DATA|KWH_FLOAT,   4}, // (��1 ��)�ն��ᷴ���й��ܵ���
{	3331,       5,      5701,               0xFFFFFFFF,          DAYHOLD_DATA|STRING_DATA|KWH_STRING,   3}, //(��1 ��)�ն��ᷴ���й��ܵ��� KWh
{	3331,       8,      5805,               0x00010001,          MONTHHOLD_DATA|FLOAT_DATA|KWH_FLOAT,   4}, //  �¶��������й��ܵ���ʾֵ ----��
{	3331,       8,      5701,               0xFFFFFFFF,          MONTHHOLD_DATA|STRING_DATA|KWH_STRING,   4}, //�¶��������й��ܵ���ʾֵ KWh
#else
{	3331,       0,      5805,               0x00010000,          CYCLE_DATA|FLOAT_DATA|KWH_FLOAT,   4}, // ��ǰ�����й�����ʾֵ
{	3331,       0,      5701,               0xFFFFFFFF,          CYCLE_DATA|STRING_DATA|KWH_STRING,   3}, //��ǰ�����й�����ʾֵ��λ KWh
{	3331,       1,      5805,               0x00030000,          CYCLE_DATA|FLOAT_DATA|KWH_FLOAT,   4}, // ��ǰ�����޹�����ʾֵ-----��
{	3331,       1,      5701,               0xFFFFFFFF,          CYCLE_DATA|STRING_DATA|KVarH_STRING,   5}, //��ǰ�����޹�����ʾֵ��λ Kvarh
{	3331,       2,      5805,               0x00020000,          CYCLE_DATA|FLOAT_DATA|KWH_FLOAT,   4}, // ��ǰ�����й�����ʾֵ
{	3331,       2,      5701,               0xFFFFFFFF,          CYCLE_DATA|STRING_DATA|KWH_STRING,   3}, //��ǰ�����й�����ʾֵ��λ KWh
{	3331,       3,      5805,               0x00040000,          CYCLE_DATA|FLOAT_DATA|KWH_FLOAT,   4}, // ��ǰ�����޹�����ʾֵ----��
{	3331,       3,      5701,               0xFFFFFFFF,          CYCLE_DATA|STRING_DATA|KVarH_STRING,   5}, //��ǰ�����޹�����ʾֵ��λ Kvarh
{	3331,       4,      5805,               0x05060101,          DAYHOLD_DATA|FLOAT_DATA|KWH_FLOAT,   4}, // (��1 ��)�ն��������й��ܵ���
{	3331,       4,      5701,               0xFFFFFFFF,          DAYHOLD_DATA|STRING_DATA|KWH_STRING,   3}, //(��1 ��)�ն��������й��ܵ��� KWh
{	3331,       5,      5805,               0x05060201,          DAYHOLD_DATA|FLOAT_DATA|KWH_FLOAT,   4}, // (��1 ��)�ն��ᷴ���й��ܵ���
{	3331,       5,      5701,               0xFFFFFFFF,          DAYHOLD_DATA|STRING_DATA|KWH_STRING,   3}, //(��1 ��)�ն��ᷴ���й��ܵ��� KWh
{	3331,       6,      5805,               0x05060301,          DAYHOLD_DATA|FLOAT_DATA|KWH_FLOAT,   4}, // (��1 ��)�ն��������޹��ܵ��� ------��
{	3331,       6,      5701,               0xFFFFFFFF,          DAYHOLD_DATA|STRING_DATA|KVarH_STRING,   5}, //(��1 ��)�ն��������޹��ܵ��� Kvarh
{	3331,       7,      5805,               0x05060401,          DAYHOLD_DATA|FLOAT_DATA|KWH_FLOAT,   4}, // (��1 ��)�ն��ᷴ���޹��ܵ��� ------��
{	3331,       7,      5701,               0xFFFFFFFF,          DAYHOLD_DATA|STRING_DATA|KVarH_STRING,   5}, //(��1 ��)�ն��ᷴ���޹��ܵ��� Kvarh
{	3331,       8,      5805,               0x00010001,          MONTHHOLD_DATA|FLOAT_DATA|KWH_FLOAT,   4}, //  �¶��������й��ܵ���ʾֵ ----��
{	3331,       8,      5701,               0xFFFFFFFF,          MONTHHOLD_DATA|STRING_DATA|KWH_STRING,   4}, //�¶��������й��ܵ���ʾֵ KWh
#endif
//��չ��3332ϵ��
{   3332,       0,      5750,               0xFFFFFFFF,          STRING_DATA|METER_INDEX,             4}, //���ܱ��ʲ����
{   3332,       1,      5750,               0xFFFFFFFF,          STRING_DATA|METER_TYPE,             4}, //���ܱ�����
{   3332,       2,      5750,               0xFFFFFFFF,          STRING_DATA|MODULE_IMEI,             4}, //IMEI
{   3332,       3,      5750,               0xFFFFFFFF,          STRING_DATA|MODULE_ICCID,             4}, //ICCID
{   3332,       4,      5750,               0xFFFFFFFF,          STRING_DATA|MODULE_IMSI,             4}, //IMSI
{   3332,       5,      5750,               0xFFFFFFFF,          STRING_DATA|MODULE_CSQ,             4}, //�ź�ǿ�ȵȼ�
{   3332,       6,      5750,               0xFFFFFFFF,          STRING_DATA|METER_ADDR,             4}, //���ͨ�ŵ�ַ
{   3332,       7,      5750,               0xFFFFFFFF,          STRING_DATA|NOTIFY_CYCLE,             4}, //�����ϱ�����
{   3332,       8,      5750,               0xFFFFFFFF,          STRING_DATA|SOFT_VERSION,             4}, //��������汾��
{   3332,       9,      5750,               0xFFFFFFFF,          STRING_DATA|HARD_VERSION,             4}, //����Ӳ���汾��
{   3332,       10,     5750,               0xFFFFFFFF,          STRING_DATA|VENDER_NO,             4}, //���ұ��
{   3332,       11,     5750,               0xFFFFFFFF,          STRING_DATA|POWER_SUPPLY,             4}, //��ǰ����״̬
{   3332,       12,     5750,               0xFFFFFFFF,          STRING_DATA|DATA_FLOW,             4}, //��������
{   3332,       13,     5750,               0xFFFFFFFF,          STRING_DATA|REMOTE_UPDATE,             4}, //Զ������
{   3333,       0,      5750,               0xFFFFFFFF,          STRING_DATA|ONENET_TIME,             4}, //���ڡ����ڣ�����0���������죩��ʱ��


};

BOOLEAN  app_get_id_to_item(INT32U objId,INT32U insId,INT32U resId,tagResourceId *pLib)
{
    INT16U size=0,idx=0;
    
    size = sizeof(RESOURCE_ID)/sizeof(tagResourceId);
    
    for(idx=0;idx<size;idx++)
    {
        if((RESOURCE_ID[idx].obj==objId)&&((RESOURCE_ID[idx].instance)==insId)&&(RESOURCE_ID[idx].id == resId))
        {
            pLib->item = RESOURCE_ID[idx].item;
            pLib->data_type.type = RESOURCE_ID[idx].data_type.type;
            pLib->len  = RESOURCE_ID[idx].len;
            pLib->data_type.convert_type = RESOURCE_ID[idx].data_type.convert_type;
            return TRUE; 
        }
    }
    pLib->item = 0xFFFFFFFF;
    return FALSE;
}


/*********************************
 * frame  ��Ҫ��ת��string
 * len    string���ͳ���
 ********************************/
void app_readresp_onenet_frame(tagWirelessObj *Obj,INT8U* frame,INT16U len,INT8U need_quotation)
{
 	INT8U str[100]={0};   
    if(len)
    {
        if(need_quotation)
        {
            sprintf((char*)str,"AT+MIPLREADRSP=0,%ld,%d,%4d,%d,%ld,%d,%d,\"", Obj->mid,Obj->result,Obj->objid,Obj->insid,Obj->resid,Obj->type,len);
        }
        else
        {
            sprintf((char*)str,"AT+MIPLREADRSP=0,%ld,%d,%4d,%d,%ld,%d,%d,", Obj->mid,Obj->result,Obj->objid,Obj->insid,Obj->resid,Obj->type,len);        
        }
    }
    else
    {
        sprintf((char*)str,"AT+MIPLREADRSP=0,%ld,%d\r\n", Obj->mid,Obj->result);
    }

    Obj->send_len = str_cmdlen(str);    
    mem_cpy(Obj->send_buf+Obj->send_len,frame,len);
    mem_cpy(Obj->send_buf,str,str_cmdlen(str));
    Obj->send_len += len;
    
    if(need_quotation)
    {
        sprintf((char*)Obj->send_buf+Obj->send_len,"\",0,0\r\n");
        Obj->send_len+=7;
    }
    else
    {
        sprintf((char*)Obj->send_buf+Obj->send_len,",0,0\r\n");
        Obj->send_len+=6;
    }

    remote_send_app_frame(Obj->send_buf,Obj->send_len); 
}
INT8U app_data_convert(INT8U* data,INT8U type)
{
    float res=0;
    BOOLEAN valid_bcd;
    INT8U str1[60],str2[60];
    INT8U buffer[20]={0};
    INT8U idx;
    INT8U report_period[4];
    tagDatetime datetime;
    INT8U monthFlow[4];
    INT8U ch;//Ϊ����ʱ�洢int2bcd��
    if(type==0)
    {
        return str_cmdlen(data);
    }
    
    switch(type)
    {
        case KWH_FLOAT:
            res = bcd2u32(data,4,&valid_bcd);
            sprintf((char*)data,"%.2f",res/100); 
            break;
        case KWH_STRING:
            sprintf((char*)data,"\"kWh\""); 
            break;  
        case KVarH_STRING:
            sprintf((char*)data,"\"kvarh\""); 
            break;
        case V_STRING:
            sprintf((char*)data,"\"V\""); 
            break;
        case V_FLOAT:
             res = bcd2u32(data,2,&valid_bcd);
            sprintf((char*)data,"%.1f",res/10); 
            break;
        case A_FLOAT:
            res = bcd2u32(data,3,&valid_bcd);
            sprintf((char*)data,"%.3f",res/1000); 
            break;
        case A_STRING:
            sprintf((char*)data,"\"A\""); 
            break;
        case P_FLOAT:
            res = bcd2u32(data,3,&valid_bcd);
            sprintf((char*)data,"%.4f",res/10000); 
            break;
        case KW_STRING:
            sprintf((char*)data,"\"kW\""); 
            break;
        case KVar_STRING:
            sprintf((char*)data,"\"kvar\""); 
            break;
        case FACTOR_FLOAT:
            res = bcd2u32(data,2,&valid_bcd);
            sprintf((char*)data,"%.3f",res/1000); 
            break;
        case  METER_INDEX: //���ܱ��ʲ���� 0
            memset(str1,0x00,32);
            app_read_his_item(0x04000403,str1,str2,255,NULL,2000);//str2�д���������Ҫ��ȡ��ֵ��
                *data='"';
                mem_cpy(data+1,str1,32);
            for(idx =0;idx<32;idx++)
            {
                if(*(data+idx+1)<10)
                    *(data+idx+1)+='0';
                else if(*(data+idx+1)<'Z')
                    *(data+idx+1)+='A';
                else if(*(data+idx+1)<'z')
                    *(data+idx+1)+='a';
            }
            *(data+33)='"';
            break;
        case  METER_TYPE: //���ܱ�����  �������ݶ������ж�
            sprintf((char*)data,"\"1\""); 
            break;
        case  MODULE_IMEI: //IMEI 2  imei_info[15]; 2  +CN:9903682 869976030068952
            //gprs_send_cmd_wait_OK((uint8_t *)"AT+CGSN=1\r\n",str1,sizeof(str1),100);
            *data='"';
            for(idx =0;idx<15;idx++)
            {
               *(data+idx+1) = gSystemInfo.imei_info[idx];
            }
            *(data+idx+1) ='"';
            break;
        case  MODULE_ICCID: //ICCID 3  +NID982B510241 898602B7151630249417
           // gprs_send_cmd_wait_OK((uint8_t *)"AT+NCCID\r\n",str1,sizeof(str1),100);
            *data='"';
            for(idx =0;idx<20;idx++)
            {
                *(data+idx+1) = gSystemInfo.myccid_info[idx];
            }
            *(data+idx+1) ='"';
            break;
        case  MODULE_IMSI: //IMSI 4   46405341
           // gprs_send_cmd_wait_OK((uint8_t *)"AT+CIMI\r\n",str1,sizeof(str1),100);
            *data='"';
            for(idx =0;idx<15;idx++)
            {
               *(data+idx+1) = gSystemInfo.imsi_info[idx];
            }
            *(data+idx+1) ='"';
            break;
        case  MODULE_CSQ: //�ź�ǿ�ȵȼ� gSystemInfo.csq  5
             *data='"';
             *(data+1)=gSystemInfo.csq/10+0x30;
             *(data+2)=gSystemInfo.csq- (*(data+1)-0x30)*10+0x30;
             *(data+3) ='"';
           // sprintf((char*)data,"%d",&(gSystemInfo.csq));
            break;
        case  METER_ADDR: //���ͨ�ŵ�ַ 6
             *data='"';
             for(idx=0;idx<6;idx++)
             {
                 *(data+idx*2+1)=(((gSystemInfo.meter_no[idx])>>4)&0x0f) +0x30;
                 *(data+idx*2+2)=((gSystemInfo.meter_no[idx])&0x0f)  +0x30;
             }
             *(data+idx*2+1) ='"';
            break;
        case  NOTIFY_CYCLE: //�����ϱ�����7
            fread_ertu_params(EEADDR_TASK_REPORT_CYCLE,report_period,4);
            hex2str(report_period,4, (data+1));
             *data='"';
             *(data+9) ='"';
            break;
        case  SOFT_VERSION: //��������汾��8
             *data='"';
             *(data+1)=SOFT_VER_MAIN;
             *(data+2)=SOFT_VER_DOT;
             *(data+3) =SOFT_VER_MIN1;
             *(data+4) =SOFT_VER_MIN2;
             *(data+5) ='"';
            break;
        case  HARD_VERSION: //����Ӳ���汾��9
             *data='"';
             *(data+1)=HARDWARE_VER_H;
             *(data+2)=HARDWARE_VER_DOT;
             *(data+3) =HARDWARE_VER_L1;
             *(data+4) =HARDWARE_VER_L2;
             *(data+5) ='"';
            break;
        case  VENDER_NO: //���ұ�� 10
            *data='"'; *(data+1)='T'; *(data+2)='O'; *(data+3) = 'P';
            *(data+4) = 'S'; *(data+5) ='"';
            break;
        case  ONENET_TIME: //ONENET_TIME ����ʱ�� 
            os_get_datetime(&datetime);
            *data ='"';
            ch = byte2BCD(datetime.year);
            *(data+1) = ((ch>>4)&0x0f)+'0';
            *(data+2) = ((ch)&0x0f)+'0';        
            ch = byte2BCD(datetime.month);
            *(data+3) = ((ch>>4)&0x0f)+'0';
            *(data+4) = ((ch)&0x0f)+'0';
            ch = byte2BCD(datetime.day);
            *(data+5) = ((ch>>4)&0x0f)+'0';
            *(data+6) = ((ch)&0x0f)+'0';
            ch = byte2BCD(datetime.weekday);
            *(data+7) = ((ch>>4)&0x0f)+'0';
            *(data+8) = ((ch)&0x0f)+'0';
            ch = byte2BCD(datetime.hour);
            *(data+9) = ((ch>>4)&0x0f)+'0';
            *(data+10) = ((ch)&0x0f)+'0';
            ch = byte2BCD(datetime.minute);
            *(data+11) = ((ch>>4)&0x0f)+'0';
            *(data+12) = ((ch)&0x0f)+'0';
            ch = byte2BCD(datetime.second);
            *(data+13) = ((ch>>4)&0x0f)+'0';
            *(data+14) = ((ch)&0x0f)+'0';
            *(data+15) = '"';
            break;     
        case  POWER_SUPPLY: //��ǰ����״̬11
            app_read_his_item(0x04000503,str1,str2,255,NULL,2000);//str2�д���������Ҫ��ȡ��ֵ��
            *data='"';
            mem_cpy(data+1,str1,32);
            if(str1[0]==0)
            {
                *(data+1) = '0';
                *(data+2) = '0';
            }
            else if(str1[0]&0x02)
            {
                *(data+1) = '0';
                *(data+2) = '1';
            }
            
            *(data+3)='"';
            break;
        case  DATA_FLOW: //��������12
            monthFlow[0] = g_app_run_data.gprs_bytes.month_bytes[3];
            monthFlow[1] = g_app_run_data.gprs_bytes.month_bytes[2];
            monthFlow[2] = g_app_run_data.gprs_bytes.month_bytes[1];
            monthFlow[3] = g_app_run_data.gprs_bytes.month_bytes[0];            
            hex2str(monthFlow,4, (data+1));
             *data='"';
             *(data+9) ='"';
            break;
        case REMOTE_UPDATE:
            *data='"';
            *(data+1)='a';
            *(data+2)='"';
            break;
        default:
           break;
    }
    return str_cmdlen(data);
}

void  app_protocol_handler_onenet(void)
{
    INT32U item=0;
    INT16S idx,idx2;
    INT8U frame[30]={0},res_len=0;
    tagWirelessObj *Obj;
    tagResourceId source_id,*pLib;
    Obj = &RequestOnenet;
    pLib = &source_id;
    INT8U  report_cycle[4];
    INT8U str[8];
    INT8U cs,pos; //У��ֵ
    uint8_t meter_send_buffer[50];
    INT8U  resp[200]={0};
    objRequest* Request;
    
    
    if(Obj->state != REQUEST_STATE_PROCESSING)
    {
        return;
    }
    read_c1_time_out = 3000;
    memset(Obj->send_buf,0x00,sizeof(Obj->send_buf));
    if(Obj->msg_type == ONENET_READ)
    {
        if(app_get_id_to_item(Obj->objid,Obj->insid,Obj->resid,pLib))
        {
            if(pLib->item!=0xFFFFFFFF)
            {
                //�������
                res_len = app_read_his_item(pLib->item,Obj->send_buf,Obj->recv_buf,255,NULL,read_c1_time_out);
            }
            else
            {
                //�ڲ�����
            }

            Obj->send_ptr = Obj->send_buf;
            Obj->state = REQUEST_STATE_PROCESSED;
            Obj->type = pLib->data_type.type; 
            if((res_len!=0)||(pLib->item==0xFFFFFFFF))
            {
                Obj->send_len = app_data_convert(Obj->send_buf,pLib->data_type.convert_type);
            }
            Obj->result = (Obj->send_len)? 1:0;
            app_readresp_onenet_frame(Obj,Obj->send_buf,Obj->send_len,1);      
        }
    }
    else if(Obj->msg_type == ONENET_WRITE)
    {
        Obj->state = REQUEST_STATE_PROCESSED;
        //д����
        if(app_get_id_to_item(Obj->objid,Obj->insid,Obj->resid,pLib))
        {
            if(pLib->data_type.convert_type == NOTIFY_CYCLE)
            {
                for(idx =0;idx<8;idx++)
                {
                    if(Obj->recv_buf[idx]==0)
                        break;
                }
                idx--;
                for(idx2 =7;idx2>=0;idx2--)
                {
                    if(idx>=0)
                    {
                        str[idx2] = Obj->recv_buf[idx];
                        idx--;
                    }
                    else
                        str[idx2]='0';        
                }
                str2hex(str, 8, report_cycle);
                fwrite_ertu_params(EEADDR_TASK_REPORT_CYCLE,report_cycle,4);
            }
            if(pLib->data_type.convert_type == ONENET_TIME)
            {
                //os_datetime;
                for(idx=0;idx<14;)
                {
                    str[idx/2] = (Obj->recv_buf[idx]-'0')*16+(Obj->recv_buf[idx+1]-'0');
                    idx+=2;
                }
                //�������ձ���
                idx = 0; //�鱨�ĳ�ʼλ��
                meter_send_buffer[idx++] =0x68;
                meter_send_buffer[idx++] =gSystemInfo.meter_no[0];meter_send_buffer[idx++] =gSystemInfo.meter_no[1];meter_send_buffer[idx++] =gSystemInfo.meter_no[2];
                meter_send_buffer[idx++] =gSystemInfo.meter_no[3];meter_send_buffer[idx++] =gSystemInfo.meter_no[4];meter_send_buffer[idx++] =gSystemInfo.meter_no[5];
                meter_send_buffer[idx++] = 0x68;
                meter_send_buffer[idx++] = 0x14;
                meter_send_buffer[idx++] = 0x10;
                meter_send_buffer[idx++] = 0x34;
                meter_send_buffer[idx++] = 0x34;
                meter_send_buffer[idx++] = 0x33;
                meter_send_buffer[idx++] = 0x37;
                meter_send_buffer[idx++] =0x35;meter_send_buffer[idx++] =0x89;meter_send_buffer[idx++] =0x67;meter_send_buffer[idx++] =0x45;
                meter_send_buffer[idx++] =0xAB;meter_send_buffer[idx++] =0x89;meter_send_buffer[idx++] =0x67;meter_send_buffer[idx++] =0x45;
                meter_send_buffer[idx++] =str[3] +0x33;meter_send_buffer[idx++] =str[2] +0x33;
                meter_send_buffer[idx++] =str[1] +0x33;meter_send_buffer[idx++] = str[0] +0x33;
               // for(idx=22;idx<pos;idx++) frame[idx]+=0x33;
                cs = 0;
                for(pos =0;pos<idx;pos++) cs += meter_send_buffer[pos];
                meter_send_buffer[idx++] = cs;
                meter_send_buffer[idx++] =0x16;
                app_trans_send_meter_frame(meter_send_buffer,28, resp,200,50);
                
                //д��ʱ����
                idx = 0; //�鱨�ĳ�ʼλ��
                meter_send_buffer[idx++] =0x68;
                meter_send_buffer[idx++] =gSystemInfo.meter_no[0];meter_send_buffer[idx++] =gSystemInfo.meter_no[1];meter_send_buffer[idx++] =gSystemInfo.meter_no[2];
                meter_send_buffer[idx++] =gSystemInfo.meter_no[3];meter_send_buffer[idx++] =gSystemInfo.meter_no[4];meter_send_buffer[idx++] =gSystemInfo.meter_no[5];
                meter_send_buffer[idx++] = 0x68;
                meter_send_buffer[idx++] = 0x14;
                meter_send_buffer[idx++] = 0x0F;
                meter_send_buffer[idx++] = 0x35;
                meter_send_buffer[idx++] = 0x34;
                meter_send_buffer[idx++] = 0x33;
                meter_send_buffer[idx++] = 0x37;
                meter_send_buffer[idx++] =0x35;meter_send_buffer[idx++] =0x89;meter_send_buffer[idx++] =0x67;meter_send_buffer[idx++] =0x45;
                meter_send_buffer[idx++] =0xAB;meter_send_buffer[idx++] =0x89;meter_send_buffer[idx++] =0x67;meter_send_buffer[idx++] =0x45;
                meter_send_buffer[idx++] =str[6] +0x33;meter_send_buffer[idx++] =str[5] +0x33;
                meter_send_buffer[idx++] =str[4] +0x33;
               // for(idx=22;idx<pos;idx++) frame[idx]+=0x33;
                cs = 0;
                for(pos =0;pos<idx;pos++) cs += meter_send_buffer[pos];
                meter_send_buffer[idx++] = cs;
                meter_send_buffer[idx++] =0x16;
                app_trans_send_meter_frame(meter_send_buffer,27, resp,200,50);
            }

        //gprs_writeresp_app_data_CMI_onenet(Obj->mid); 
        mem_set(resp,sizeof(resp),0x00);
        sprintf((char*)resp,"AT+MIPLWRITERSP=0,%ld,2\r\n", Obj->mid);
        remote_send_app_frame(resp,str_cmdlen(resp));
        //͸��ͨ��
        if(pLib->data_type.convert_type == REMOTE_UPDATE)
        {
            if(Obj->recv_buf[0]=='"')   //����·����������İ����ڡ���֮�ڣ���ô��if·�� ������else·��
            {
                for(idx = 0;idx <Obj->len -2;)
                {
                    if(Obj->recv_buf[idx+1] < 'A')
                        RequestRemote.frame[idx/2] = (Obj->recv_buf[idx+1]-'0')<<4;
                    else
                        RequestRemote.frame[idx/2] = (Obj->recv_buf[idx+1]-'A'+0xA)<<4;

                    if(Obj->recv_buf[idx+2] < 'A')
                        RequestRemote.frame[idx/2] = (Obj->recv_buf[idx+2]-'0') +RequestRemote.frame[idx/2] ;
                    else
                        RequestRemote.frame[idx/2] = (Obj->recv_buf[idx+2]-'A'+0xA) +RequestRemote.frame[idx/2];
                    idx = idx +2;
                }
            }
            else
            {
                for(idx = 0;idx <Obj->len;)
                    {
                        if(Obj->recv_buf[idx+1] < 'A')
                            RequestRemote.frame[idx/2] = (Obj->recv_buf[idx]-'0')<<4;
                        else
                            RequestRemote.frame[idx/2] = (Obj->recv_buf[idx]-'A'+0xA)<<4;

                        if(Obj->recv_buf[idx+2] < 'A')
                            RequestRemote.frame[idx/2] = (Obj->recv_buf[idx+1]-'0') +RequestRemote.frame[idx/2] ;
                        else
                            RequestRemote.frame[idx/2] = (Obj->recv_buf[idx+1]-'A'+0xA) +RequestRemote.frame[idx/2];
                        idx = idx +2;
                    }
            }

           
        Request = (objRequest*)&RequestRemote;
        Request->frame_len = get_1376_1_frame_len(Request->frame,MAX_FRAME_HEADER_SIZE);
        Request->cs  = init_1376_1_frame_checksum(Request->frame,Request->frame_len,Request->frame_len+1); //Ҫ��֤���ܵ����ݴ���Ԥ����ƴ�����ֽڡ�
        if(is_1376_1_frame_ready(Request->frame,Request->frame_len,Request->frame_len+1,&(Request->cs)))
        {
            //��Э���ʶ
            Request->cur_protocol.value = 0;
            Request->cur_protocol.GDW1376 = 1;
            Request->recv_time_in_10ms = system_get_tick10ms();
            Request->state = REQUEST_STATE_WAIT_PROCESS;
            Request->recv_success_flag = 1;
            return 1;
        }         
        else
        {
            Request->cur_protocol.GDW1376 = 0;
        }  
      } 
    }
        
         
    }
    else if(Obj->msg_type == ONENET_EXECUTE)
    {
    
    }

}

/*********************************
 * frame  ��Ҫ��ת��string
 * len    string���ͳ���
 ********************************/
    void app_report_onenet_frame(tagWirelessObj *Obj,INT8U* frame,INT16U len,INT8U need_quotation)
{
    
//    if(Obj->type == ((STRING&0x0F00)>>8)) 
    if(need_quotation)
    {
        sprintf((char*)Obj->send_buf,"AT+MIPLNOTIFY=0,0,%4d,%d,%ld,%d,%d,\"",Obj->objid,Obj->insid,Obj->resid,Obj->type,len);
    }
    else
    {
        sprintf((char*)Obj->send_buf,"AT+MIPLNOTIFY=0,0,%4d,%d,%ld,%d,%d,",Obj->objid,Obj->insid,Obj->resid,Obj->type,len);        
    }  

    Obj->send_len = str_cmdlen(Obj->send_buf);    

    mem_cpy(Obj->send_buf+Obj->send_len,frame,len);
    Obj->send_len += len;
    
    OSMutexPend(&SIGNAL_SYSTEMINFO);
    gSystemInfo.ackid++;
    if(gSystemInfo.ackid>32767)
    {
        gSystemInfo.ackid =0;
    }
    OSMutexFree(&SIGNAL_SYSTEMINFO);
//    if(Obj->type == (STRING>>8))
    if(need_quotation)
    {
        sprintf((char*)Obj->send_buf+Obj->send_len,"\",0,0,%d\r\n",gSystemInfo.ackid);
        Obj->send_len+=9;
    }
    else
    {
        sprintf((char*)Obj->send_buf+Obj->send_len,",0,0,%d\r\n",gSystemInfo.ackid);
        Obj->send_len+=8;
    }

    remote_send_app_frame(Obj->send_buf,Obj->send_len); 
}


void task_data_report_onenet(void)
{
    INT32U passed_sec=0;
    INT32U rpt_cycle=0,diff=0;
    static INT32U last_tick=0;
    INT16U res_len=0;
    INT8U idx1=0,buffer[5]={0};
    BOOLEAN is_valid=FALSE;
    tagDatetime datetime;
    TASK_INFO task;
    tagResourceId *pLib;
    tagWirelessObj *Obj;
    
    Obj = &RequestOnenet;
    if(gSystemInfo.tcp_link==0)
    {
        return;
    }
    
    if(second_elapsed(last_tick) < 5)
    {
        return;
    }
    else
    {
        last_tick = system_get_tick10ms();
    }
    //30���ղ���������Ϣ���ϱ�
    if((gSystemInfo.last_resource_ready==0)||(second_elapsed(gSystemInfo.last_resource_ready) < 30))
    {
        return;
    }        
    
    os_get_datetime(&datetime);
    //Ԥ��1���ӵ����ʱ��
    if((datetime.hour==0)&&(datetime.minute < 1))
    {
        return;
    }
    
    //task.rpt_flag �����Ƿ���Ҫ�ϱ� 
    fread_ertu_params(EEADDR_TASK_REPORT_CYCLE,buffer,4);
    rpt_cycle = bcd2u32(buffer,4,&is_valid);
    if((is_valid==FALSE)||(rpt_cycle==0))
    {
        rpt_cycle = 60;
        buffer[0]=0x60;
        buffer[1]=0x00;
        buffer[2]=0x00;
        buffer[3]=0x00;
        fwrite_ertu_params(EEADDR_TASK_REPORT_CYCLE,buffer,4);
    }
    
    fread_ertu_params(EEADDR_TASK_REPORT,task.value,sizeof(TASK_INFO));
    if(task.rpt_idx>(sizeof(RESOURCE_ID)/sizeof(tagResourceId)))
    {
        mem_set(task.value,sizeof(TASK_INFO),0x00);
        fwrite_ertu_params(EEADDR_TASK_REPORT,task.value,sizeof(TASK_INFO)); 
        return;
    }
    
    if((task.rpt_flag==0)||(task.rpt_flag==0xFF))
    {
        if((rpt_cycle == 0xFFFF)||(rpt_cycle == 0))
        {
            rpt_cycle = 60;
        }
        passed_sec =  diff_sec_between_dt(&datetime,&task.cycle_report_time);
        if(datetime.day != task.dayahold_report_time.day)
        {
            task.rpt_flag |= DAYHOLD_DATA;
        }
        if(datetime.month != task.dayahold_report_time.month)
        {
            task.rpt_flag |= MONTHHOLD_DATA;
        }        
        if(passed_sec > rpt_cycle*60)
        {
            task.rpt_flag |= CYCLE_DATA;
        }
    }
    else
    {
        task.rpt_idx++;
    }
            
    //δ���ϱ�����
    if((task.rpt_flag==0)||(task.rpt_flag==0xFF))
    {
        return;
    } 
    
    mem_set(Obj,sizeof(tagWirelessObj),0x00);
    for(;task.rpt_idx<(sizeof(RESOURCE_ID)/sizeof(tagResourceId));task.rpt_idx++)
    {
        pLib = &(RESOURCE_ID)[task.rpt_idx];
        if(pLib->data_type.report_type == 0)
        {
            continue;
        }
        
        if(pLib->item!=0xFFFFFFFF)
        {
            //�����ط����2��
            for(idx1=0;idx1<2;idx1++)
            {
                read_c1_time_out = 1000;
                res_len = app_read_his_item(pLib->item,Obj->recv_buf,Obj->send_buf,255,NULL,2000);
                if(res_len)
                {
                    break;
                }
            }
        }
        //ack_id ʹ�ñ��������Ϊ�ϱ���־ ��λ����ƫ�Ƶ�ַ
        Obj->ackid = task.rpt_idx;
        res_len = app_data_convert(Obj->recv_buf,pLib->data_type.convert_type);
        Obj->insid = pLib->instance;
        Obj->objid = pLib->obj;
        Obj->resid = pLib->id;
        Obj->type = pLib->data_type.type;
        Obj->mid = 0;
        Obj->result = 1;
        app_report_onenet_frame(Obj,Obj->recv_buf,res_len,0);
        //����rpt_idx
        fwrite_ertu_params(EEADDR_TASK_REPORT+TASK_REPORT_REPORT_FLAG,&(task.rpt_flag),2);
        fwrite_ertu_params(EEADDR_TASK_REPORT+TASK_REPORT_IDX,(INT8U*)&(task.rpt_idx),2);
        if(task.rpt_idx<(sizeof(RESOURCE_ID)/sizeof(tagResourceId)))
        {
            return;
        }
    }
    
    if(task.rpt_idx==sizeof(RESOURCE_ID)/sizeof(tagResourceId))
    {    
        //ȫ���ϱ���ɳ�ʼ������ �����ϱ�ʱ��
        if(task.rpt_flag & DAYHOLD_DATA)
        {
            mem_cpy(task.dayahold_report_time.value,datetime.value,sizeof(tagDatetime));
            //��¼�ϱ�ʱ��Ϊ���� ��ֹ�ϱ�����ռ�õ�ʱ���ۼ�
            task.dayahold_report_time.hour = 0;
            task.dayahold_report_time.minute = 0;
            task.dayahold_report_time.second = 0;
        }
        if(task.rpt_flag & MONTHHOLD_DATA)
        {
            mem_cpy(task.monthhold_report_time.value,datetime.value,sizeof(tagDatetime));
            //��¼�ϱ�ʱ��Ϊ���� ��ֹ�ϱ�����ռ�õ�ʱ���ۼ�
            task.dayahold_report_time.day = 1;
            task.dayahold_report_time.hour = 0;
            task.dayahold_report_time.minute = 0;
            task.dayahold_report_time.second = 0;
        }        
        if(task.rpt_flag & CYCLE_DATA)
        {
            mem_cpy(task.cycle_report_time.value,datetime.value,sizeof(tagDatetime));
            //��¼�ϱ�ʱ��Ϊ������ ��ֹ�ϱ�����ռ�õ�ʱ���ۼ�
            //task.cycle_report_time.hour = datetime.hour;
            //task.cycle_report_time.minute = task.cycle_report_time.minute/ rpt_cycle * rpt_cycle;
            //task.cycle_report_time.second = 0;
            datetime.hour=0;
            datetime.minute=0;
            datetime.second=0;
            diff = diff_sec_between_dt(&datetime,&task.cycle_report_time) % (rpt_cycle*60);
            //diff = getPassedSeconds((tagDatetime*)&datetime,2000) % (rpt_cycle*60);
            datetime_minus_seconds(&task.cycle_report_time.year, &task.cycle_report_time.month, &task.cycle_report_time.day, &task.cycle_report_time.hour, &task.cycle_report_time.minute, &task.cycle_report_time.second, diff);
        } 
        task.ack_idx = 0;
        task.rpt_idx = 0;
        task.rpt_flag = 0;
        fwrite_ertu_params(EEADDR_TASK_REPORT,task.value,sizeof(TASK_INFO));
    }
    else 
    {
        mem_set(task.value,sizeof(TASK_INFO),0x00);
        fwrite_ertu_params(EEADDR_TASK_REPORT,task.value,sizeof(TASK_INFO));
    }
}

