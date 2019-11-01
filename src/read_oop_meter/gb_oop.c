#include "gb_oop.h"
#include "../main_include.h"


INT8U make_oop_cur_frame(INT8U *frame,INT8U *meter_no,INT8U oad_count,INT8U* oad_data)
{
    INT8U pos;

    frame[OOP_POS_BEGIN] = 0x68;
    frame[OOP_POS_LEN] = 0;
    frame[OOP_POS_LEN+1] = 0;
    frame[OOP_POS_CTRL] = CTRLFUNC_PRM_SET | CTRLFUNC_FC_REQ_RESP;
    frame[OOP_POS_ADDR] = 0x05;
    if(check_is_all_ch(meter_no,6,0xAA))
    {
        frame[OOP_POS_ADDR]|=(1<<6);//ͨ���ַ
    }
    mem_cpy(frame+OOP_POS_ADDR+1,meter_no,6);
    pos = OOP_POS_ADDR + 7;
    frame[pos++] = 2; //�ͻ�����ַCA
    frame[pos++] = 0;  //HCS
    frame[pos++] = 0;
    frame[pos++] = CLIENT_APDU_GET;  //GET
    frame[pos++] = GET_REQUEST_NORMAL_LIST;  //��¼
    frame[pos++] = 2;  //PIID
    frame[pos++] = oad_count;    //count
    mem_cpy(frame+pos,oad_data,oad_count*4);
    pos += oad_count*4;
    //ʱ���ǩ
    frame[pos++] = 0;  //��ʱ����Ҫ��֤ʱ���ǩ
    //����
    frame[OOP_POS_LEN] = (pos+2-1);
    frame[OOP_POS_LEN+1] = (pos+2-1)>>8;
    //����HCSУ��λ
    fcs16(frame+OOP_POS_LEN,11);
    //����FCSУ��λ
    fcs16(frame+OOP_POS_LEN,pos-1);
    pos += 2;
    frame[pos++]=0x16;
    return pos;
}
//���ն���ʱ�������֯����
INT16U make_oop_hold_frame(INT8U *frame,INT8U *rtu_no,INT8U *meter_no,OBJECT_ATTR_DESC1* oad,INT8U oad_count,INT8U* oad_data,BOOLEAN add_hold_time_oad,INT8U* hold_time,BOOLEAN is_oop_cjq)
{
    INT16U idx,pos;
    INT16U obis;

    obis=(oad->object_id[0]<<8)+oad->object_id[1];
    if(obis==OBIS_DAY_HOLD)//������������ն��ᣬ�������ʱ���е�Сʱ�����ӡ���
    {
        hold_time[4]=0x00;
        hold_time[5]=0x00;
        hold_time[6]=0x00;
    }

    frame[OOP_POS_BEGIN] = 0x68;
    frame[OOP_POS_LEN] = 0;
    frame[OOP_POS_LEN+1] = 0;
    frame[OOP_POS_CTRL] = CTRLFUNC_PRM_SET | CTRLFUNC_FC_REQ_RESP;
    frame[OOP_POS_ADDR] = 0x05;
    if(check_is_all_ch(meter_no,6,0xAA))
    {
        frame[OOP_POS_ADDR]|=(1<<6);//ͨ���ַ
    }
    if (is_oop_cjq)
    {
        mem_cpy(frame+OOP_POS_ADDR+1,rtu_no,6);
    }
    else
    {
        mem_cpy(frame+OOP_POS_ADDR+1,meter_no,6);
    }
    pos = OOP_POS_ADDR + 7;
    frame[pos++] = 2; //�ͻ�����ַCA
    frame[pos++] = 0;  //HCS
    frame[pos++] = 0;
    frame[pos++] = CLIENT_APDU_GET;  //GET
    frame[pos++] = GET_REQUEST_RECORD;  //��¼
    frame[pos++] = 2;  //PIID
    {
        frame[pos++] = oad->object_id[0];
        frame[pos++] = oad->object_id[1];
        frame[pos++] = oad->attribute_id.value;
        frame[pos++] = oad->index_id;
        //rsd
        frame[pos++] = 0x01; //choice   ����
        frame[pos++] = 0x20; //oad
        frame[pos++] = 0x21;
        frame[pos++] = 0x02;
        frame[pos++] = 0x00;
        frame[pos++] = 0x1C; //����ʱ��
        frame[pos++] = (hold_time[0]);
        frame[pos++] = (hold_time[1]);
        frame[pos++] = (hold_time[2]);
        frame[pos++] = (hold_time[3]);
        frame[pos++] = (hold_time[4]);
        frame[pos++] = (hold_time[5]);
        frame[pos++] = (hold_time[6]);
    }
    //rcsd
    if (add_hold_time_oad)
    {
        frame[pos++] = oad_count+1;  //count
        frame[pos++] = 0x00;
        frame[pos++] = 0x20;
        frame[pos++] = 0x21;
        frame[pos++] = 0x02;
        frame[pos++] = 0x00;
    }
    else
    {
        frame[pos++] = oad_count;
    }

    for(idx=0;idx<oad_count;idx++)
    {
        frame[pos++]=0x00;
        mem_cpy(frame+pos,oad_data+idx*4,4);
        pos +=4;
    }
   
    //ʱ���ǩ
    frame[pos++] = 0;  //��ʱ����Ҫ��֤ʱ���ǩ
    //����
    frame[OOP_POS_LEN] = (pos+2-1);
    frame[OOP_POS_LEN+1] = (pos+2-1)>>8;
    //����HCSУ��λ
    fcs16(frame+OOP_POS_LEN,11);
    //����FCSУ��λ
    fcs16(frame+OOP_POS_LEN,pos-1);
    pos += 2;
    frame[pos++]=0x16;
    return pos;
}

//���ն���ʱ������ͼ����֯����
INT8U make_oop_hold_begin_end_ti_frame(INT8U *frame,INT8U *meter_no,OBJECT_ATTR_DESC1* oad,OBJECT_ATTR_DESC1* query_oad,INT8U oad_count,INT8U* oad_data,BOOLEAN add_hold_time_oad,INT8U* hold_time,INT8U* last_time,INT8U* ti)
{

    //INT8U* hcs;
    INT8U idx,pos;
    //INT8U cs;

    frame[OOP_POS_BEGIN] = 0x68;
    frame[OOP_POS_LEN] = 0;
    frame[OOP_POS_LEN+1] = 0;
    frame[OOP_POS_CTRL] = CTRLFUNC_PRM_SET | CTRLFUNC_FC_REQ_RESP;
    frame[OOP_POS_ADDR] = 0x05;
    if(check_is_all_ch(meter_no,6,0xAA))
    {
        frame[OOP_POS_ADDR]|=(1<<6);//ͨ���ַ
    }
    {
        mem_cpy(frame+OOP_POS_ADDR+1,meter_no,6);
    }
    pos = OOP_POS_ADDR + 7;
    frame[pos++] = 2; //�ͻ�����ַCA
    frame[pos++] = 0;  //HCS
    frame[pos++] = 0;
    frame[pos++] = CLIENT_APDU_GET;  //GET
    frame[pos++] = GET_REQUEST_RECORD;  //��¼
    frame[pos++] = 2;  //PIID
    {
        frame[pos++] = oad->object_id[0] ;   //oad
        frame[pos++] = oad->object_id[1];
        frame[pos++] = oad->attribute_id.value;
        frame[pos++] = oad->index_id;
        frame[pos++] = 0x02; //choice
        frame[pos++] = query_oad->object_id[0]; //oad
        frame[pos++] = query_oad->object_id[1];
        frame[pos++] = query_oad->attribute_id.value;
        frame[pos++] = query_oad->index_id;
        frame[pos++] = DT_DATETIME_S; //��ʼʱ��
        frame[pos++] = (last_time[0]);
        frame[pos++] = (last_time[1]);
        frame[pos++] = (last_time[2]);
        frame[pos++] = (last_time[3]);
        frame[pos++] = (last_time[4]);
        frame[pos++] = (last_time[5]);
        frame[pos++] = (last_time[6]);
        frame[pos++] = DT_DATETIME_S; //����ʱ��
        frame[pos++] = (hold_time[0]);
        frame[pos++] = (hold_time[1]);
        frame[pos++] = (hold_time[2]);
        frame[pos++] = (hold_time[3]);
        frame[pos++] = (hold_time[4]);
        frame[pos++] = (hold_time[5]);
        frame[pos++] = (hold_time[6]);
        frame[pos++] = DT_TI;
        frame[pos++] = ti[0];
        frame[pos++] = ti[1];
        frame[pos++] = ti[2];
    }
    //rcsd
    if (add_hold_time_oad)
    {
        frame[pos++] = oad_count+1;  //count
        frame[pos++] = 0x00;
        frame[pos++] = 0x20;
        frame[pos++] = 0x21;
        frame[pos++] = 0x02;
        frame[pos++] = 0x00;
    }
    else
    {
        frame[pos++] = oad_count;
    }
    for(idx=0;idx<oad_count;idx++)
    {
        frame[pos++]=0x00;
        mem_cpy(frame+pos,oad_data+idx*4,4);
        pos +=4;
    }


    //ʱ���ǩ
    frame[pos++] = 0;  //��ʱ����Ҫ��֤ʱ���ǩ
    //����
    frame[OOP_POS_LEN] = (pos+2-1);
    frame[OOP_POS_LEN+1] = (pos+2-1)>>8;
    //����HCSУ��λ
    fcs16(frame+OOP_POS_LEN,11);
    //����FCSУ��λ
    fcs16(frame+OOP_POS_LEN,pos-1);
    pos += 2;
    frame[pos++]=0x16;
    return pos;
}
