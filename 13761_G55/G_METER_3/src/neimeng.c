#if defined(__PROVICE_NEIMENG__)
#include "main_include.h"
#include "neimeng.h"
#include "TaskPlmsdtp_nm.h"

/*
���ɹ�Э�鴦�����
*/

/*+++
 ���ܣ�����F85�����õ����Ч�Ժ��ص��û���־
 ����:
        INT8U *itemData,           [����]����
        INT16U *itemDataLen        [���]������Ŀ����
  ����:
        INT8U ���ý��
  F85���壺
      �������ñ�־	BIN	1
      ������������	BIN	256	��������
  ������
       1)��Ҫ��F30��F35ͬ��
---*/

INT8U set_ertu_F85_nm(INT8U *itemData,INT16U *itemDataLen)
{
    INT16U idx,pos;
    INT8U flag,is_use,data[41];

    flag=itemData[0];
    data[0]=0;
    pos=0;
    for(idx=0;idx<MAX_METER_COUNT;idx++)
    {
      if(0==flag)//�����Ч��
      {
            //F30
            is_use=get_bit_value(&itemData[idx/8+1],1,idx%8);  // 1-��ʾ��Ч 0-��ʾ��Ч
            is_use=is_use?0:1;
            set_spot_params((idx+1),PIM_REC_FLAG,&is_use,1);//0-��ʾ��Ч 1-��ʾ��Ч

      }else//�ص㻧��־
      {
           is_use=get_bit_value(&itemData[idx/8+1],1,idx%8);
           if(is_use)
           {
               data[pos++]=meter_idx2_seq(idx+1);
               data[pos++]=(meter_idx2_seq(idx+1)>>8);
               data[0]++;
               if(data[0]>=20)break;//���֧��20���ص��û�
           }
      }
    }
    if(1==flag)
    {
	    fwrite_ertu_params(EEADDR_SET_F35,data,41);
    }
    return OK;  //���سɹ�

}
/*+++
  ���ܣ���ѯF85����
  ������
        INT8U  *data  �����Ч�Ի��ص㻧��־[���]
        QUERY_CTRL *query_ctrl  ���ڲ�ѯ���Ƶı�����
  ���أ�
        INT16U  ��������
  ������
        1�����ݲ�ѯ���������ص��ܱ���Ч��־���ص��û���־
---*/
INT16U query_ertu_F85_nm(INT8U *data,QUERY_CTRL *query_ctrl)
{
    INT16U idx,meter_idx;
    INT8U is_use,temp[41],count;
    data[0]=query_ctrl->req[0];
    mem_set(data+1,256,0x00);
    //�����Ч��
    if(0==query_ctrl->req[0])
    {
       for(idx=0;idx<MAX_METER_COUNT;idx++)
       {
            get_spot_params((idx+1),PIM_REC_FLAG,&is_use,1);//0-��ʾ��Ч 1-��ʾ��Ч
            if(is_use == 0)
            {
            	set_bit_value(&data[idx/8+1],1,idx%8);  // 1-��ʾ��Ч 0-��ʾ��Ч
            }
       }

    }
    else //�ص㻧��־
    {
	    fread_ertu_params(EEADDR_SET_F35,temp,41);
        count=temp[0];
        if(count>20)count=20;
          for(idx=0;idx<count;idx++)
          {
               byte2int16(temp+idx*2+1,&meter_idx);
               set_bit_value(&data[(meter_idx-1)/8+1],1,(meter_idx-1)%8);  // 1-��ʾ��Ч 0-��ʾ��Ч
          }

    
    }
    query_ctrl->req_bytes++;
    return 256;
}
/*
�����ز�������ʼʱ��ͳ����������㳭��ʱ��

*/
void calc_seg_time()
{
    JCSET_F33  SET_F33;
    INT16U  offset;
    INT8U idx;
    INT8U start_time[2];
    offset=sizeof(JCSET_F33)*3;

    //��ȡ̨�����г������в�������:�ز�ͨ��
    fread_ertu_params(EEADDR_SET_F33+offset,(INT8U *)&SET_F33,sizeof(JCSET_F33));

    start_time[0]=SET_F33.seg.rec_timeseg[0][0];
    start_time[1]=SET_F33.seg.rec_timeseg[0][1];
    for(idx=0;idx<24;idx++)
    {
        if(start_time[0]+SET_F33.cycle*(idx+1)>24)break;
        SET_F33.seg.rec_timeseg[0][0]=start_time[0]+SET_F33.cycle*idx;
        SET_F33.seg.rec_timeseg[0][1]=start_time[1];

        SET_F33.seg.rec_timeseg[1][0]=start_time[0]+SET_F33.cycle*(idx+1);
        if(start_time[1]==0)
            SET_F33.seg.rec_timeseg[1][1]=59;
        else
            SET_F33.seg.rec_timeseg[1][1]=start_time[1]-1;
    }




}


/*
���������������
˵����ͳ���ز����С���������ķ�����ÿ���ڷ��������б��в�ѯ�ô�С�࣬����������ΪFF��˵��������Ч��
*/
INT16U read_c1_f10(INT8U *data,READ_HIS *readC1)
{
    INT16U spot_idx;
    INT16U meter_seq;
    INT16U meter_idx;
    INT16U meter_count;
    INT16U plcmeter_count;
    INT16U idx,temp_count;
    INT8U meter485_count,class_count;
    METER_DOCUMENT meter_doc;
    INT8U PLC_METER_COUNT_POS=6;

    meter_count=0;
    meter485_count=0;
    plcmeter_count=0;
    class_count=0;
    for(meter_seq=1;meter_seq<MAX_METER_COUNT;meter_seq++)
    {

        if(FALSE == file_exist(meter_seq))     continue;

        //��ȡ������Ϣ
        fread_array(meter_seq,PIM_METER_DOC,(INT8U *)&meter_doc,sizeof(METER_DOCUMENT));
        //��֤�����Ч��
        meter_idx = bin2_int16u(meter_doc.meter_idx);
        spot_idx  = bin2_int16u(meter_doc.spot_idx);

        if(spot_idx == 0) continue;
        if(meter_seq != meter_idx) continue;
         meter_count++;
        if((meter_doc.baud_port.port>=2) && (meter_doc.baud_port.port<=5))
        {
         	meter485_count++;
        }
        if(meter_doc.baud_port.port==31)
        {
           plcmeter_count++;
           for(idx=0;idx<0xFF;idx++)
           {
              if((data[PLC_METER_COUNT_POS+idx*3+1]==0xFF) && (data[PLC_METER_COUNT_POS+idx*3+2]==0xFF))break;//û�ҵ�
              if(data[PLC_METER_COUNT_POS+idx*3]!=meter_doc.meter_class.value) continue;//������Ҫ��
              if(idx+1>class_count)
              {
              	class_count=idx+1;
              }
              break;
           }
           data[PLC_METER_COUNT_POS+idx*3+1]=meter_doc.meter_class.value;
           temp_count=bin2_int16u(data+PLC_METER_COUNT_POS+idx*3+1);
           if(temp_count=0xFF)
           {
           		temp_count++;
           }
           temp_count++;
           data[PLC_METER_COUNT_POS+idx*3+1]=temp_count;
           data[PLC_METER_COUNT_POS+idx*3+2]=(temp_count>>8);
        }
        
    }
    data[0]=meter_count;
    data[1]=meter_count>>8;
    data[2]=meter485_count;
    data[3]=plcmeter_count;
    data[4]=plcmeter_count>>8;
    data[5]=class_count;
    return  (class_count*3+6);
}
/*
����������״̬
����485���ز��Ƿֿ��ģ����Ŀǰֻȡ�ز�
*/
INT16U read_c1_f11(INT8U *data,READ_HIS *readC1)
{
   INT16U pos,count1,count2;
   LLVC_REC_STATE  plc_rec_state;
   pos=0;
   read_fmArray(FMADDR_F11+1,&plc_rec_state,sizeof(LLVC_REC_STATE));
   if(plc_rec_state.currec_flag.finish==1)
   {
       byte2int16(plc_rec_state.total_count,&count1);
       byte2int16(plc_rec_state.read_count,&count2);
       if(count1==count2)
           data[pos++]=1;
       else
           data[pos++]=2;
   }else
   {
           data[pos++]=0;
   }
   //����������ʼʱ��
   data[pos++]=plc_rec_state.begin_datetime.year;
   data[pos++]=(plc_rec_state.begin_datetime.week_month & 0x1F);
   data[pos++]=plc_rec_state.begin_datetime.day;
   data[pos++]=plc_rec_state.begin_datetime.hour;
   data[pos++]=plc_rec_state.begin_datetime.minute;
   data[pos++]=plc_rec_state.begin_datetime.second;

   //�������ݽ���ʱ��
   data[pos++]=plc_rec_state.end_datetime.year;
   data[pos++]=(plc_rec_state.end_datetime.week_month & 0x1F);
   data[pos++]=plc_rec_state.end_datetime.day;
   data[pos++]=plc_rec_state.end_datetime.hour;
   data[pos++]=plc_rec_state.end_datetime.minute;
   data[pos++]=plc_rec_state.end_datetime.second;

   return pos;
}



/*+++
  ���ܣ�����2004�����ò���F9
  ������
        INT8U *itemData  F9����
  ���أ�
        ��
  ������
        1����������485��ͽ��ɱ���ǰ64��������
        2������ʱɾ������������
---*/

void set_ertu_f9(INT8U *itemData)
{
   INT16U itemLen;
   DISSET_SPOT_PARAMS meter_doc;
   INT8U old_meter_count,new_meter_count,idx,meter_idx;

   fread_array(FILEID_QGDW_NM,EEADDR_SET_F9_NM,&old_meter_count,1);

   if(old_meter_count>64)old_meter_count=64;

   fwrite_array(FILEID_QGDW_NM,EEADDR_SET_F9_NM,itemData,4);

   new_meter_count=itemData[0];//���ܱ�/��������װ�������ܿ���

   while(new_meter_count<old_meter_count)
   {
         meter_idx=new_meter_count+1;
         //ɾ��������
         meter_doc.set_count=1;
         meter_doc.meter_seq=meter_idx;
         meter_doc.spot_idx=0;
           //�����̫�����У���ʹ����̫��������������ʹ�ô��ڻ�����
          if(1 != g_eth_conn_flag)
          {

           //������̫��Ӧ���Ĵ������ݽṹʹ��Ȩ
           OSMutexPend(&SIGNAL_RESP_ETH);
           set_ertu_meters(resp_eth.buffer,(INT8U*)&meter_doc,&itemLen);
           OSMutexFree(&SIGNAL_RESP_ETH);
          }
          else
          {
           OSMutexPend(&SIGNAL_RESP_LOCAL);
           set_ertu_meters(resp_local.buffer,(INT8U*)&meter_doc,&itemLen);
           OSMutexFree(&SIGNAL_RESP_LOCAL);
          }
          new_meter_count++;
   }

}

/*+++
  ���ܣ������ն˵��ܱ�/��������װ�����ò���[F10]
  ����:
        INT8U *buffer,        [������]����С�㹻500�ֽ�
        INT8U *params
        INT16U *itemDataLen    ������Ĳ�������
  ����:
        ���ý��

  ����:
  		1.�������ɹ�F10������376.1��һ����Ϊ�˱����������һ���ԣ�ʹ�ù�Լת���ķ����������ɹŹ�Լת��376.1
      2.�������ɹ�F10�Ĳ��������Ϊ64�������ǰ64��������ΪRS485������
      3.�������ɹ�û��ɾ�������㹦�ܣ�Ҫɾ��ֻ�����ò������������˸ı����ʱҪɾ������Ĳ�����
---*/
INT8U set_ertu_meters_nm(INT8U *buffer,INT8U *params,INT16U *itemDataLen)
{
    INT32U fmAddr;
    INT16U idx,pos;
    INT16U count;          //������������
    INT16U meter_idx;      //�������1~63
    INT16U spot_idx;      //�������1~63�� 0��ɾ��
    INT16U itemLen;
    DISSET_SPOT_PARAMS meter_doc;
    INT8U temp;

    pos=0;
    //���ε��ܱ�/��������װ����������
    count=params[pos++];

    *itemDataLen = count * 17 + 1;

    mem_set(buffer,PIM_CUR_ZXYG,0x00);

    //���δ���ÿ������
    for(idx=0; idx<count; idx++)
    {
        //���
        meter_idx=params[pos++];

        //�ж�����Ƿ����
        if( (meter_idx == 0 ) || (meter_idx > 64) )
        {
            return ERR;   //���ش���
        }
        //�������
        spot_idx=params[pos++];

        //�жϲ�������Ƿ����
        if( spot_idx > 64 )
        {
            return ERR;  //���ش���
        }

        //�����
        meter_doc.set_count=1;
        meter_doc.meter_seq=meter_idx;

        meter_doc.spot_idx=spot_idx;
        meter_doc.baud_port.value=params[pos++];
        meter_doc.protocol =params[pos++];
        mem_cpy(meter_doc.meter_no,params+pos,6);pos+=6;
        mem_cpy(meter_doc.meter_pass,params+pos,6);pos+=6;
        temp=params[pos++];
        meter_doc.meter_fl_count=(temp >> 4) & 0x0F;
        meter_doc.meter_frac_info=temp;
        mem_set(meter_doc.rtu_no,6,0xFF);
        meter_doc.meter_class.value = 0;
        //�����̫�����У���ʹ����̫��������������ʹ�ô��ڻ�����
       if(1 != g_eth_conn_flag)
       {

        //������̫��Ӧ���Ĵ������ݽṹʹ��Ȩ
        OSMutexPend(&SIGNAL_RESP_ETH);
        set_ertu_meters(resp_eth.buffer,(INT8U*)&meter_doc,&itemLen);
        OSMutexFree(&SIGNAL_RESP_ETH);
       }
       else
       {
        OSMutexPend(&SIGNAL_RESP_LOCAL);
        set_ertu_meters(resp_local.buffer,(INT8U*)&meter_doc,&itemLen);
        OSMutexFree(&SIGNAL_RESP_LOCAL);
       }
    }

    return OK;  //���سɹ�
}
/*+++
  ���ܣ������ն˵��ܱ�/��������װ�����ò���[F96]
  ����:
        INT8U *buffer,        [������]����С�㹻500�ֽ�
        INT8U *params
        INT16U *itemDataLen    ������Ĳ�������
  ����:
        ���ý��

  ����:
  		1.���������65��ʼ��ǰ64����F10ʹ��
---*/
INT8U set_ertu_meters_F96(INT8U *buffer,INT8U *params,INT16U *itemDataLen)
{
    INT32U fmAddr;
    INT16U idx,pos;
    INT16U count;          //������������
    INT16U meter_idx;      //�������1~63
    INT16U spot_idx;      //�������1~63�� 0��ɾ��
    INT16U itemLen;
    DISSET_SPOT_PARAMS meter_doc;
    INT8U temp;

    pos=0;
    //���ε��ܱ�/��������װ����������
    count=bin2_int16u(params+pos);
    pos+=2;

    *itemDataLen = count * 27 + 1;

    //���δ���ÿ������
    for(idx=0; idx<count; idx++)
    {
        //���
       meter_idx=bin2_int16u(params+pos);
       pos+=2;

        //�ж�����Ƿ����
        if( (meter_idx == 0 ) || (meter_idx > 64) )
        {
            return ERR;   //���ش���
        }
        //�������
       spot_idx=bin2_int16u(params+pos);
       pos+=2;

        //�жϲ�������Ƿ����
        if( spot_idx > 64 )
        {
            return ERR;  //���ش���
        }

        //�����
        meter_doc.set_count=1;
        meter_doc.meter_seq=meter_idx+65;

        meter_doc.spot_idx=spot_idx+65;
        meter_doc.baud_port.value=params[pos++];
        meter_doc.protocol =params[pos++];
        mem_cpy(meter_doc.meter_no,params+pos,6);pos+=6;
        mem_cpy(meter_doc.meter_pass,params+pos,6);pos+=6;
        meter_doc.meter_fl_count=params[pos++];
        meter_doc.meter_frac_info=params[pos++];
        mem_cpy(meter_doc.rtu_no,params+pos,6);pos+=6;
        meter_doc.meter_class.value = params[pos++];
        //�����̫�����У���ʹ����̫��������������ʹ�ô��ڻ�����
       if(1 != g_eth_conn_flag)
       {

        //������̫��Ӧ���Ĵ������ݽṹʹ��Ȩ
        OSMutexPend(&SIGNAL_RESP_ETH);
        set_ertu_meters(resp_eth.buffer,(INT8U*)&meter_doc,&itemLen);
        OSMutexFree(&SIGNAL_RESP_ETH);
       }
       else
       {
        OSMutexPend(&SIGNAL_RESP_LOCAL);
        set_ertu_meters(resp_local.buffer,(INT8U*)&meter_doc,&itemLen);
        OSMutexFree(&SIGNAL_RESP_LOCAL);
       }
    }

    return OK;  //���سɹ�
}

/*+++
  ���ܣ�����2004����ѯ���������Ϣ,û�е����洢������376.1����ת��
  ������
        INT8U *data,
        INT16U max_len      ������󳤶�
  ���أ�
        INT16U  �������ݳ���
---*/
BOOLEAN query_meter_info_nm(INT8U *data,INT16U *itemDataLen,INT16U max_len)
{
   INT16U pos,idx,meter_idx;
   INT8U  meter_count;
   union{
      INT8U  str[4];
      struct{
         INT8U   meter_count;
         INT8U   pulse_count;
         INT8U   analog_count;
         INT8U   agp_count;
      }d;
   }var;
    METER_DOCUMENT meter_doc;
   *itemDataLen=0;
   //��ȡ��������
   fread_array(FILEID_QGDW_NM,EEADDR_SET_F9_NM, var.str,4);

   meter_count = var.d.meter_count;

   if(meter_count > MAX_485METER_COUNT) meter_count  = 0;
   if(meter_count*17+1>max_len)return TRUE;//�޷�����

   data[0] = 0;
   pos = 1;
   for(idx=0;idx<meter_count;idx++)
   {
        if(FALSE == file_exist(idx+1))     continue;

        //��ȡ������Ϣ
        fread_array(idx+1,PIM_METER_DOC,(INT8U *)&meter_doc,sizeof(METER_DOCUMENT));
        byte2int16(meter_doc.meter_idx,&meter_idx);
        if(meter_idx==idx+1)
        {
            data[0] ++;
            data[pos++]=meter_doc.meter_idx[0];
            data[pos++]=meter_doc.spot_idx[0];
            data[pos++]=meter_doc.baud_port.value;
            data[pos++]=meter_doc.protocol;
            mem_cpy(data+pos,meter_doc.meter_no,6);pos+=6;
            mem_cpy(data+pos,meter_doc.password,6);pos+=6;
            data[pos++]=(meter_doc.fl_count<<4) | (meter_doc.mbit_info.value & 0x0F);
            
            //if(pos + 17 > max_len) break;
        }
   }
   *itemDataLen=pos;
   return FALSE;
}
/*+++
  ���ܣ���ѯ����F10
  ������
        INT8U *Reply,                [���] Ӧ�𻺳���
        INT16U *itemDataLen,         [���] ����Ӧ���ĳ���
        QUERY_CTRL *query_ctrl       [����] �������
  ���أ�
        �Ƿ���������ҪӦ�� TRUE/FALSE
  ������
         1����ѯ����
         ���β�ѯ����n	BIN	2
         ���β�ѯ�ĵ�1���������	BIN	2
          �� ��	����	����
         ���β�ѯ�ĵ�n���������	BIN	2
       2)   �޸�itemDataLenΪ������ֽڳ���
       3) query_ctrl.from_seq Ϊ�����������ʼλ��
---*/
BOOLEAN query_ertu_meters_F96(INT8U *Reply,INT16U *itemDataLen,QUERY_CTRL *query_ctrl)
{
    INT16U idx;
    INT16U pos;
    INT16U req_seq;
    INT16U meter_idx;
    INT16U meter_count;
    INT16U resp_count;
    INT8U meter_doc_len;
    METER_DOCUMENT meter_doc;
    BOOLEAN flag;

    pos = 0;
    resp_count=0;
    flag = FALSE;
    
    //���ε��ܱ�/��������װ����������
    byte2int16(query_ctrl->req,  &meter_count);
    query_ctrl->req+=2;
    Reply[pos++] = 0;
    Reply[pos++] = 0;

    for(idx=0;idx<meter_count;idx++)
    {
        if(idx<query_ctrl->from_seq)
        {
           query_ctrl->req +=2;
           continue;
        }

        //��������
        byte2int16(query_ctrl->req,  &req_seq);
        query_ctrl->req += 2;
        req_seq+=65;//��ѹ����65��ʼ

        if(FALSE == file_exist(req_seq))     continue;

        //��ȡ������Ϣ
        meter_doc_len = sizeof(METER_DOCUMENT);
        fread_array(req_seq,PIM_METER_DOC,(INT8U *)&meter_doc,sizeof(METER_DOCUMENT));


        //����Ƿ��Ѿ����ù���,����������������Ϊ�Ѿ����ù��ˣ�
        byte2int16(meter_doc.meter_idx,  &meter_idx);
        if(meter_idx == req_seq)
        {
            //���Ӧ�𻺳����Ƿ������������Ӧ����
            if( (pos+meter_doc_len) > query_ctrl->left_resp_len)
            {
               flag = TRUE;
               break;
            }
            //����meter_doc.��źͲ������
            meter_doc.meter_idx[0]-=65;
            meter_doc.spot_idx[0]-=65;
            //����������
            mem_cpy(Reply+pos,(INT8U *)&meter_doc,meter_doc_len);
            pos += meter_doc_len;
            resp_count ++;
            query_ctrl->from_seq = idx+1;
        }
    }

    //������д������������
    Reply[0] = resp_count;
    Reply[1] = resp_count>>8;

    //������Ӧʵ�ʳ���
    *itemDataLen = pos;

    //�����������
    query_ctrl->req_bytes = (meter_count<<1) + 2;

    //ѭ������ֻ�����ֿ��ܣ�1��ѭ��������2��������Ӧ�ռ�
    //�����ֽ����flag���б�����
    return flag;
}
/*+++
 ���ܣ�������վIP��ַ�Ͷ˿ڣ���Ҫ��һ��ת����
 ����:
        INT8U *itemData,           [����]����
        INT16U *itemDataLen        [���]������Ŀ����
  ����:
        INT8U ���ý��
  ������
       2004��Լ�� ��������IP��ַ�Ͷ˿ڣ�����ɣе�ַ�Ͷ˿ڡ�
       ����ʱ���������ֱ��ȥ����Ȼ�󱣴浽�����������ļ��С�

---*/
INT8U set_ertu_msa_info_nm(INT8U *itemData,INT16U *itemDataLen)
{
    tagSET_F3_nm *setF3;

    setF3  = (tagSET_F3_nm *)itemData;


    //�ȱ������ص�ַ�˿ڼ������ַ���˿�
    fwrite_array(FILEID_QGDW_NM,EEADDR_SET_F3_NM_EXT,setF3->d.gate_ipport,12);
   
    //�޸Ĳ����ṹ��
    mem_cpy(setF3->n.apn,setF3->d.apn,16);

    //����
    fwrite_ertu_params(EEADDR_SET_F3,itemData,sizeof(tagSET_F3));

    *itemDataLen = sizeof(tagSET_F3_nm);

    return  OK;
}


/*+++
  ���ܣ�����2004����ѯ��վ����
  ������
        INT8U  *data  ��վ����[���]
  ���أ�
        INT16U  ��������
---*/
INT16U query_nm_msa_info(INT8U *data)
{
   tagSET_F3 F3;
   tagSET_F3_nm *F3_nm;

   F3_nm = (tagSET_F3_nm *)data;
   fread_ertu_params(EEADDR_SET_F3,F3.value,sizeof(tagSET_F3));

   //����IP������IP
   mem_set(F3_nm->value,sizeof(tagSET_F3_nm),0x00);
   mem_cpy(F3_nm->value,F3.value,12);

   //APN
   mem_cpy(F3_nm->d.apn,F3.APN,16);

   //��ȡ���ص�ַ�˿ڼ������ַ���˿�
   fread_array(FILEID_QGDW_NM,EEADDR_SET_F3_NM_EXT,F3_nm->d.gate_ipport,12);
   
   return sizeof(tagSET_F3_nm);
}


/*+++
 ���ܣ����ó����ղ�������Ҫ��һ��ת����
 ����:
        INT8U *itemData,           [����]����
        INT16U *itemDataLen        [���]������Ŀ����
  ����:
        INT8U ���ý��
  ������
       1)�Ѳ������ݱ��浽����˿ڵ�F33��(QGDW376.1)��

---*/
INT8U set_ertu_recday_info_nm(INT8U *itemData,INT16U *itemDataLen)
{
    JCSET_F33  SET_F33;
    INT16U  offset;

    offset=sizeof(JCSET_F33);
    //��ȡ̨�����г������в�������:����RS485ͨ��
    fread_ertu_params(EEADDR_SET_F33+offset,(INT8U *)&SET_F33,sizeof(JCSET_F33));

    SET_F33.port = COMMPORT_485_REC;
    mem_cpy(SET_F33.rec_days,itemData,6);
    *itemDataLen = 6;
    fwrite_ertu_params(EEADDR_SET_F33+offset,(INT8U *)&SET_F33,sizeof(JCSET_F33));

    return OK;
}

/*+++
  ���ܣ�����2004����ѯ�����ղ���
  ������
        INT8U  *data  ������[���]
  ���أ�
        INT16U  ��������
  ������
        1����QGDW376.1��F33��ȡ�����̶��ӳ���485�ڵ�ȡ��.
---*/
INT16U query_nm_recday_info(INT8U *data)
{
    JCSET_F33  SET_F33;
    INT16U  offset;

    offset=sizeof(JCSET_F33);
    //��ȡ̨�����г������в�������:����RS485ͨ��
    fread_ertu_params(EEADDR_SET_F33+offset,(INT8U *)&SET_F33,sizeof(JCSET_F33));

    mem_cpy(data,SET_F33.rec_days,6);

    return 6;
}



/*+++
  ���ܣ�����2004����ѯ���������Ϣ
  ������
        INT8U *data,
        INT16U max_len      ������󳤶�
  ���أ�
        INT16U  �������ݳ���
---*/
INT16U query_nm_meter_info(INT8U *data,INT16U max_len)
{
   INT16U pos,idx;
   INT8U  meter_count;
   union{
      INT8U  str[4];
      struct{
         INT8U   meter_count;
         INT8U   pulse_count;
         INT8U   analog_count;
         INT8U   agp_count;
      }d;
      struct{
          INT8U   meter_idx;       //�������
          INT8U   spot_idx;        //�������
          INT8U   meter_info[15];
      }e;
   }var;

   //��ȡ��������
   fread_array(FILEID_QGDW_NM,EEADDR_SET_F9_NM, var.str,4);

   meter_count = var.d.meter_count;

   if(meter_count > MAX_485METER_COUNT) meter_count  = 0;

   data[0] = 0;
   pos = 1;
   for(idx=0;idx<meter_count;idx++)
   {
      fread_array(FILEID_QGDW_NM,EEADDR_SET_F10_NM+idx*17,var.str,17);

      if( (var.e.meter_idx+1) == idx)
      {
          data[0] ++;
          mem_cpy(data+pos,var.str,17);
          pos+=17;
          if(pos + 17 > max_len) break;
      }
   }

   return pos;
}


/*+++
 ���ܣ����ó�����
 ����:
        INT8U *itemData,           [����]����
        INT16U *itemDataLen        [���]������Ŀ����
        INT8U port_type            [����]�˿���� 0-482/����  1-�ز�
  ����:
        INT8U ���ý��
  ������
       1)�Ѳ������ݱ��浽����˿ڵ�F33��(QGDW376.1)��

---*/
INT8U set_ertu_recinterval_info_nm(INT8U *itemData,INT16U *itemDataLen,INT8U port_type)
{
    JCSET_F33  SET_F33;
    INT16U  offset;

    offset=sizeof(JCSET_F33);
    if(1==port_type)offset=sizeof(JCSET_F33)*3;

    //��ȡ̨�����г������в�������
    fread_ertu_params(EEADDR_SET_F33+offset,(INT8U *)&SET_F33,sizeof(JCSET_F33));

    SET_F33.port = COMMPORT_485_REC;
    SET_F33.cycle=itemData[0];
    *itemDataLen = 1;
    fwrite_ertu_params(EEADDR_SET_F33+offset,(INT8U *)&SET_F33,sizeof(JCSET_F33));
    if(1==port_type)calc_seg_time();//����ز��������ı��ˣ�����Ҫ���¼��㳭��ʱ��
    return OK;
}

/*+++
  ���ܣ�����2004����ѯ������
  ������
        INT8U  *data  ������[���]
        INT8U  port_type�˿�����[����]
  ���أ�
        INT16U  ��������
  ������
        1����QGDW376.1��F33��ȡ������ָ��ͨ����ȡ.
---*/
INT16U query_nm_recinterval_info(INT8U *data,INT8U port_type)
{
    JCSET_F33  SET_F33;
    INT16U  offset;

    offset=sizeof(JCSET_F33);
    if(1==port_type)offset=sizeof(JCSET_F33)*3;

    //��ȡ̨�����г������в�������
    fread_ertu_params(EEADDR_SET_F33+offset,(INT8U *)&SET_F33,sizeof(JCSET_F33));

    data[0]=SET_F33.cycle;
    return 1;
}



/*+++
 ���ܣ�����F33����ʱ��1��ʼʱ��
 ����:
        INT8U *itemData,           [����]����
        INT16U *itemDataLen        [���]������Ŀ����
  ����:
        INT8U ���ý��
  ������
       1)�Ѳ������ݱ��浽����˿ڵ�F33��(QGDW376.1)��

---*/
INT8U set_ertu_recstarttime_info_nm(INT8U *itemData,INT16U *itemDataLen)
{
    JCSET_F33  SET_F33;
    INT16U  offset;

    offset=sizeof(JCSET_F33)*3;
    //��ȡ̨�����г������в�������:�ز�ͨ��
    fread_ertu_params(EEADDR_SET_F33+offset,(INT8U *)&SET_F33,sizeof(JCSET_F33));

    SET_F33.port = COMMPORT_PLC_REC;
    SET_F33.seg.rec_timeseg[0][0]=itemData[0];
    SET_F33.seg.rec_timeseg[0][1]=itemData[1];
    *itemDataLen = 2;
    fwrite_ertu_params(EEADDR_SET_F33+offset,(INT8U *)&SET_F33,sizeof(JCSET_F33));
    calc_seg_time();//����ز��������ı��ˣ�����Ҫ���¼��㳭��ʱ��

    return OK;
}

/*+++
  ���ܣ�����2004����ѯ�ز�����ʱ��1��ʼʱ��
  ������
        INT8U  *data  ������[���]
  ���أ�
        INT16U  ��������
  ������
        1����QGDW376.1��F33��ȡ�����̶����ز�ͨ���ڵ�ȡ��.
---*/
INT16U query_nm_recstarttime_info(INT8U *data)
{
    JCSET_F33  SET_F33;
    INT16U  offset;
    offset=sizeof(JCSET_F33)*3;

    //��ȡ̨�����г������в�������:�����ز�ͨ��
    fread_ertu_params(EEADDR_SET_F33+offset,(INT8U *)&SET_F33,sizeof(JCSET_F33));

    data[0]=SET_F33.seg.rec_timeseg[0][0];
    data[1]=SET_F33.seg.rec_timeseg[0][1];
    return 2;
}

INT16U query_ertu_f27(INT16U pn_idx,INT8U *itemData)
{
   INT8U len;
   fread_array(FILEID_QGDW_NM,EEADDR_SET_F27_NM+pn_idx*65,itemData,65);
   len=itemData[0]*2+1;
   if(len>65)
   {
      itemData[0]=0;
      len=1;
   }
   return len;
}
INT16U set_ertu_f27(INT16U pn_idx,INT8U *itemData)
{
   INT8U len;
   len=itemData[0]*2+1;
   if(len>0)
   {
      fwrite_array(FILEID_QGDW_NM,EEADDR_SET_F27_NM+pn_idx*65,itemData,len);
   }
   return len;

}
INT16U get_F27F28_C1_nm(INT8U *data,C1F27 *cur_F27,C1F28 *cur_F28)
{
   INT16U pos;
   INT8U meter_status;
   INT8U run_status;

   pos=0;
   meter_status=0;
   run_status=0;
   
   //�������㷽ʽ
   if(get_bit_value((INT8U*)(cur_F28->status+7),1,1))
      set_bit_value(&meter_status,1,1);
   
   //��ص�ѹ
   if(get_bit_value((INT8U*)(cur_F28->status+7),1,2))
      set_bit_value(&meter_status,1,2);

   //�й����ܷ���
   if(get_bit_value((INT8U*)(cur_F28->status+7),1,4))
      set_bit_value(&meter_status,1,4);
   
   //�޹����ܷ���
   if(get_bit_value((INT8U*)(cur_F28->status+7),1,5))
      set_bit_value(&meter_status,1,5);
   
   //A��ϵ�
   if(get_bit_value((INT8U*)(cur_F28->status+10),1,7))
      set_bit_value(&run_status,1,0);
   //B��ϵ�
   if(get_bit_value((INT8U*)(cur_F28->status+11),1,7))
      set_bit_value(&run_status,1,1);
   //C��ϵ�
   if(get_bit_value((INT8U*)(cur_F28->status+12),1,7))
      set_bit_value(&run_status,1,2);

   //A���ѹ
    if(get_bit_value((INT8U*)(cur_F28->status+10),1,2))
       set_bit_value(&run_status,1,4);
    //B���ѹ
    if(get_bit_value((INT8U*)(cur_F28->status+11),1,2))
       set_bit_value(&run_status,1,5);
    //C���ѹ
    if(get_bit_value((INT8U*)(cur_F28->status+12),1,2))
       set_bit_value(&run_status,1,6);

   //�ն˳���ʱ��,���ܱ�����
   mem_cpy((INT8U*)(data+pos),(INT8U*)cur_F27->read_date,11);
   pos+=11;
   //�������״̬��
   data[pos++]=meter_status;
   //����״̬��
   data[pos++]=run_status;
   //���һ�α��ʱ��
   mem_cpy((INT8U*)(data+pos),(INT8U*)(cur_F27->last_program_date+1),3);
   data[pos+3]=(cur_F27->last_program_date[4] & 0x1F);
   pos+=4;
   //���һ�������������ʱ��
   mem_cpy((INT8U*)(data+pos),(INT8U*)(cur_F27->last_xlclear_date+1),3);
   data[pos+3]=(cur_F27->last_xlclear_date[4] & 0x1F);
   pos+=4;
  //��̴��� 
  mem_cpy((INT8U*)(data+pos),(INT8U*)cur_F27->program_count,2);
  pos+=2;
  //��������������
  mem_cpy((INT8U*)(data+pos),(INT8U*)cur_F27->xlclear_count,2);
  pos+=2;
  //��ع���ʱ��
  mem_cpy((INT8U*)(data+pos),(INT8U*)cur_F27->battery_time,3);
  pos+=3;
  
  return pos;


}

/*+++
  ���ܣ�����г����ֵ
---*/
INT8U set_ertu_F60_nm(INT8U *params,INT16U *itemDataLen)
{
   INT8U temp[42];

   mem_set(temp,sizeof(temp),0);
   mem_cpy(temp,params,6);

   fwrite_ertu_params(EEADDR_SET_F60,temp,sizeof(temp));

   fwrite_ertu_params(EEADDR_SET_F60+42,params+6,38);
   
   *itemDataLen = 80;
   return 0;

}
/*+++
  ���ܣ���ѯг����ֵ
---*/
INT16U query_ertu_F60_nm(INT8U *data)
{

   fread_ertu_params(EEADDR_SET_F60,data,6);
   fread_ertu_params(EEADDR_SET_F60+42,data+6,38);

   return 80;

}
/*+++
  ���ܣ������ն˵���������ʱ�κͷ�����,����ת��2004��ԼΪ376.1��Լ
---*/
INT8U set_ertu_F21_nm(INT8U *params,INT16U *itemDataLen)
{
   INT8U temp[49];
   INT8U idx;

   for(idx=0;idx<24;idx++)
   {
      temp[idx*2]=(params[idx] & 0x0F);
      temp[idx*2+1]=((params[idx] >>4)& 0x0F);
   }
   temp[48]=params[24];
   fwrite_ertu_params(EEADDR_SET_F21,temp,sizeof(temp));
   
   *itemDataLen = 25;
   return 0;


}
/*+++
  ���ܣ���ѯ�ն˵���������ʱ�κͷ�����
---*/
INT16U query_ertu_F21_nm(INT8U *data)
{
   INT8U temp[49];
   INT8U idx;

   fread_ertu_params(EEADDR_SET_F21,temp,49);
   for(idx=0;idx<24;idx++)
   {
      data[idx]=(temp[idx*2+1]<<4);
      data[idx]|=temp[idx*2];
   }
   data[24]=temp[48];

   return 25;

}
/*+++
  ���ܣ������ն˵���������
---*/
INT8U set_ertu_F22_nm(INT8U *params,INT16U *itemDataLen)
{
   INT8U fl_num;

   fl_num=14;
   fwrite_ertu_params(EEADDR_SET_F22,&fl_num,1);

   fwrite_ertu_params(EEADDR_SET_F22+1,params,56);

   *itemDataLen = 56;

   return 0;

}
/*+++
  ���ܣ���ѯ�ն˵���������
---*/
INT16U query_ertu_F22_nm(INT8U *data)
{

   fread_ertu_params(EEADDR_SET_F22+1,data,56);

   return 56;

}
/*+++
  ���ܣ����ò������������
---*/
INT16U set_ertu_F25_nm(INT16U spot_idx,INT8U *params)
{
   INT8U temp[11];

   mem_set(temp,sizeof(temp),0);
   mem_cpy(temp,params,7);
   temp[10]=params[7];

   fwrite_array(spot_idx,PIM_METER_F25,temp,sizeof(temp));
   
   return  8;


}
/*+++
  ���ܣ���ѯ�������������
---*/
INT16U query_ertu_F25_nm(INT16U spot_idx,INT8U *data)
{

   fread_array(spot_idx,PIM_METER_F25,data,11);
   data[7]=data[10];
   return 8;

}
/*+++
  ���ܣ����ò�������ֵ����
---*/
INT16U set_ertu_F26_nm(INT16U spot_idx,INT8U *params)
{
   INT8U temp[57];
   INT8U pos,idx;
   pos=0;
   idx=0;

   mem_set(temp,sizeof(temp),0);

   mem_cpy(temp,params,8);
   pos+=8;
   idx+=8;
   //��ѹ������
   temp[pos++]=0x02;
   temp[pos++]=0x50;
   temp[pos++]=0x01;
   //��ѹ������
   temp[pos++]=params[idx++];
   temp[pos++]=params[idx++];
   temp[pos++]=0x02;
   temp[pos++]=0x50;
   temp[pos++]=0x01;
   //����������ޣ��������ޣ�
   ConvertIfromNm(temp+pos,params+idx);
   pos+=3;
   idx+=2;
   temp[pos++]=0x02;
   temp[pos++]=0x50;
   temp[pos++]=0x01;
   //��������ޣ���������ޣ�
   ConvertIfromNm(temp+pos,params+idx);
   pos+=3;
   idx+=2;
   temp[pos++]=0x02;
   temp[pos++]=0x50;
   temp[pos++]=0x01;
   //�����������
   ConvertIfromNm(temp+pos,params+idx);
   pos+=3;
   idx+=2;
   temp[pos++]=0x02;
   temp[pos++]=0x50;
   temp[pos++]=0x01;
   //���ڹ��������� 
   temp[pos++]=params[idx++];
   temp[pos++]=params[idx++];
   temp[pos++]=params[idx++];
   temp[pos++]=0x02;
   temp[pos++]=0x50;
   temp[pos++]=0x01;

   //���ڹ������� 
   temp[pos++]=params[idx++];
   temp[pos++]=params[idx++];
   temp[pos++]=params[idx++];
   temp[pos++]=0x02;
   temp[pos++]=0x50;
   temp[pos++]=0x01;
   //�����ѹ��ƽ����ֵ
   temp[pos++]=params[idx++];
   temp[pos++]=params[idx++];
   temp[pos++]=0x02;
   temp[pos++]=0x50;
   temp[pos++]=0x01;

   //���������ƽ����ֵ
   temp[pos++]=params[idx++];
   temp[pos++]=params[idx++];
   temp[pos++]=0x02;
   temp[pos++]=0x50;
   temp[pos++]=0x01;
   //����ʧѹʱ����ֵ
   temp[pos++]=params[idx++];
   
   fwrite_array(spot_idx,PIM_METER_F26,temp,pos);
   
   return idx;


}
/*+++
  ���ܣ���ѯ��������ֵ����
---*/
INT16U query_ertu_F26_nm(INT16U spot_idx,INT8U *data)
{
   INT8U temp[57];
   INT8U pos,idx;

   pos=0;
   idx=0;

   fread_array(spot_idx,PIM_METER_F26,temp,57);
   //   ��ѹ�ϸ�����   ��ѹ�ϸ�����   ��ѹ��������   ��ѹ�����ޣ���ѹ���ޣ�
   mem_cpy(data,temp,8);
   pos+=11;
   idx+=8;

   //��ѹ������
   data[idx++]=temp[pos++];  
   data[idx++]=temp[pos++];  
   pos+=3;
   
   //����������ޣ��������ޣ�
   ConvertI2Nm(data+idx,temp+pos);
   pos+=6;
   idx+=2;
   //��������ޣ���������ޣ�
   ConvertI2Nm(data+idx,temp+pos);
   pos+=6;
   idx+=2;
   //�����������
   ConvertI2Nm(data+idx,temp+pos);
   pos+=6;
   idx+=2;
   //���ڹ��������� 
   data[idx++]=temp[pos++];  
   data[idx++]=temp[pos++];  
   data[idx++]=temp[pos++];  
   pos+=3;
   //���ڹ������� 
   data[idx++]=temp[pos++];  
   data[idx++]=temp[pos++];  
   data[idx++]=temp[pos++];  
   pos+=3;
   //�����ѹ��ƽ����ֵ
   data[idx++]=temp[pos++];  
   data[idx++]=temp[pos++];  
   pos+=3;
   //���������ƽ����ֵ
   data[idx++]=temp[pos++];  
   data[idx++]=temp[pos++];  
   pos+=3;
   //����ʧѹʱ����ֵ
   data[idx++]=temp[pos++];  
  return idx;

}
/*+++
  ���ܣ�����ֱ��ģ����������
---*/
INT8U set_ertu_F81_nm(INT8U *params,INT16U *itemDataLen)
{
   double dval;
   INT8U temp[4];

   mem_set(temp,4,0);
   if(fmt02_to_double(params,&dval))
   {
      OSTASK_PROTECT_BEGIN; 
      dval*=20;   
      OSTASK_PROTECT_END;
      double_to_fmt02(dval,temp+2);
   }
   fwrite_ertu_params(EEADDR_SET_F81,temp,4);
   
   *itemDataLen = 2;
   return 0;


}
/*+++
  ���ܣ���ѯֱ��ģ����������
---*/
INT16U query_ertu_F81_nm(INT8U *data)
{
   double dval;
   INT8U temp[2];

   fread_ertu_params(EEADDR_SET_F81+2,temp,2);
   if(fmt02_to_double(temp,&dval))
   {
      OSTASK_PROTECT_BEGIN; 
      dval/=20;
      OSTASK_PROTECT_END;
      double_to_fmt02(dval,temp);

   }
   data[0]=temp[0];
   data[1]=temp[1];
   return 2;

}
INT16U query_nm_F14(INT8U *Reply)
{
   INT8U idx,len,buffer[80],count,pos;

   //��ȡ��ǰ��������
   fread_ertu_params(EEADDR_SET_F14,buffer,80);

   //����Ч��������
   count = 0;
   pos = 1;
   for(idx=0;idx<MAX_ADDGROUP_COUNT;idx++)
   {
       if(buffer[idx*10] == (idx+1) )
       {
           count ++;
           len =  buffer[idx*10+1]+2;
           mem_cpy(Reply+pos,buffer+idx*10,len);
           pos += len;
       }
   }
   Reply[0] = count;
   return pos;
}
/*+++
 ���ܣ�����ʱ
 ����: INT16s T   ���ʱ��
 ���أ� �ɹ�OK
        ����
---*/
INT8U  set_ertu_time_t(INT16S T)
{
    INT8U bcd[7],mytime[6];
    INT8U weekofday; 

    if(0==T)return 0;

    mem_cpy((void*)mytime,(void*)datetime,6);
    if(T<0)
      datetime_minus_seconds(&mytime[YEAR],&mytime[MONTH],&mytime[DAY],&mytime[HOUR],&mytime[MINUTE],&mytime[SECOND],T*-1);
    else
      datetime_add_seconds(&mytime[YEAR],&mytime[MONTH],&mytime[DAY],&mytime[HOUR],&mytime[MINUTE],&mytime[SECOND],T);


    weekofday =  weekDay(mytime[YEAR],mytime[MONTH],mytime[DAY]);

    bcd[0] = byte2BCD(mytime[SECOND]);         //��
    bcd[1] = byte2BCD(mytime[MINUTE]);         //��
    bcd[2] = byte2BCD(mytime[HOUR]);        //ʱ
    bcd[3] = byte2BCD(weekofday);             //����
    bcd[4] = byte2BCD(mytime[DAY]);         //��
    bcd[5] = byte2BCD(mytime[MONTH]);      //��
    bcd[6] = byte2BCD(mytime[YEAR]);         //��

    return set_ertu_time(bcd,TRUE,TRUE);
}
/*+++
  ���ܣ����ɹ���չ���ܣ��Ե�������բ����բ����
  ������
       INT8U *frameData,      ���ݵ�Ԫ
       INT16U *itemDataLen    ��������ݵ�Ԫʵ�ʴ���ĳ���
---*/
INT8U nm_ctrl_meter(INT8U *frameData,INT16U *itemDataLen)
{
   INT16U meter_seq;
   INT8U  error;
   error=0;
   *itemDataLen=7;
   //���ܱ��������
   meter_seq = frameData[0] + frameData[1]*0x100;


   return error;
}
/*+++
  ���ܣ���ȡ����ļ���Ϣ
  ������
       INT16U *sec_count        [���] �ܶ���
       INT16U *sec_data_size    [���] ÿ�����ݳ���
---*/
void ftp_read_file_info(INT16U *sec_count,INT16U *sec_data_size)
{
	INT32U	filesize;
    INT8U filehead[]={'T','O','P','S','C','O','M','M'};
    INT8U buffer[8];

    *sec_count=0;
    *sec_data_size=1;

    //�ȶ�ȡ�ļ�ˮӡ��������ļ��Ƿ����
	os_spansion_readArray(FLADDR_FILE_INFO,buffer,8);//д�ļ�ˮӡ
	if(0 == compare_string(filehead,buffer,8))
    {
        //��ȡ�ļ������Ϣ
		os_spansion_readArray(FLADDR_FILE_INFO+38,buffer,8);//�ļ������Ϣ
        mem_cpy(sec_data_size,buffer,2);
        mem_cpy(&filesize,buffer+2,4);
        OSTASK_PROTECT_BEGIN;
        *sec_count=filesize / *sec_data_size;
        if(filesize % *sec_data_size !=0)
        {
            *sec_count++;
        }
     	OSTASK_PROTECT_END;
    }
}
/*+++
  ���ܣ�����ÿ���ݰ���ʹ�ñ�־
  ������
       IINT16U package_no        �ڼ������ݰ�
---*/
void ftp_update_package_flag(INT16U package_no)
{
	INT8U val;
    
    os_spansion_readArray(FLADDR_PACKAGE_RECORD+package_no/8,&val,1);
    set_bit_value(&val,1,package_no %8);
    os_spansion_writeArray(FLADDR_PACKAGE_RECORD+package_no/8,&val,1);
}

/*+++
  ���ܣ����ɹ�Զ������F1
  ������
       PLMS_RESP *resp        Ӧ��ṹ�����
       INT16U frameLen       ���ݵ�Ԫ�ĳ���
       INT8U *frameData,      ���ݵ�Ԫ
---*/
void process_ftp_nm_F1(PLMS_RESP *resp,INT16U frameLen,INT8U *frameData)
{
    INT16U pos,idx,idx1;
    INT16U pn,fn;
    INT16U nCount;
    INT16U fileDataLen;
    INT16U sec_count;
    INT16U cur_sec;
    INT16U sec_data_size;
    INT32U progAddr;
    INT32U startAddr;
    INT32U progLen;

    INT8U fileFlag,fileProp,fileCmd;
    INT8U resetFlag;

    idx=0;
    
    byte2int16(frameData,&nCount);  //�ܶ���
    idx+=2;
    progAddr = bin2_int32u(frameData+idx);   //��i�α�ʶ
    startAddr = progAddr;        //��ǰ����ʼ��ַ
    idx+=4;
    byte2int16(frameData+idx,&fileDataLen);  //��i�����ݳ���Lf
    idx+=2;

    fn = DT_F2; //ȫ������
    resetFlag=FALSE;

    if(fileCmd==0)   //д������뵽�洢��
    {
       fn = DT_F1;
       
      // ���ó���ʼ�ֽڵ�ַ
      progAddr += FLADDR_PROG_START;

      //�������ʼ��ַ������������������������9������
      if(FLADDR_PROG_START == progAddr)
      {
         os_spansion_erase_sector(0,9);
      }

       //��������

       if(fileDataLen < 0x8000)
       {
          #ifdef __DEBUG_RECINFO__
          snprintf(info,100,"rtsp_prog: addr=%d   len=%d",progAddr,fileDataLen);
          println_info(info);
          #endif

          //��695�����ݽ��н���
          #ifdef __32MX695F512L_H
           for(idx1=0;idx1<fileDataLen;idx1++)
           {
              frameData[idx+idx1] = frameData[idx+idx1] ^ __RTSP_SPEC__;
           }
          #endif

          os_spansion_writeArray(progAddr,frameData+idx,fileDataLen);
       }
       else
       {
          //����frameData��Ϊ���������������������ʹ��700���ֽڣ�
          mem_set(frameData+idx,656,frameData[idx]);
          fileDataLen &=0x7FFF;

          #ifdef __DEBUG_RECINFO__
          snprintf(info,100,"rtsp_prog: addr=%d   len=%d",progAddr,fileDataLen);
          println_info(info);
          #endif

          while(fileDataLen>0)
          {
             nCount =  fileDataLen;
             if(nCount > 656) nCount = 656;
             os_spansion_writeArray(progAddr,frameData+idx,nCount);
             progAddr += nCount;
             fileDataLen -= nCount;
          }
       }
   }


    
    ENDFTP:

    set_pn_fn(resp->buffer, &pos, pn, fn);

    //Ӧ��ȷ������

    //����Э����δ�����ļ���������ʽ�������ʱ���ļ���ʶ���ļ����Դ���
     resp->buffer[pos++] = fileFlag;
     resp->buffer[pos++] = fileProp;


     ftp_read_file_info(&sec_count,&sec_data_size);
     //���ݵ�ǰ��ַ���㣬�ڼ���
     cur_sec=startAddr / sec_data_size;
     if(startAddr % sec_data_size !=0)
     {
       cur_sec++;
     }
     //�ܿ���
     resp->buffer[pos++] = sec_count;
     resp->buffer[pos++] = sec_count>>8;
     //��ǰ����
     resp->buffer[pos++] = cur_sec;
     resp->buffer[pos++] = cur_sec>>8;

     //���¶ϵ�������־
     ftp_update_package_flag(cur_sec);

    SetFrameLenThenSendFrame(resp,pos);

}
/*+++
  ���ܣ����ɹ�Զ������F2,��ȡ�ϵ�������Ϣ
  ������
       PLMS_RESP *resp        Ӧ��ṹ�����
       INT16U frameLen       ���ݵ�Ԫ�ĳ���
       INT8U *frameData,      ���ݵ�Ԫ
---*/
void process_ftp_nm_F2(PLMS_RESP *resp,INT16U frameLen,INT8U *frameData)
{
	INT32U	filesize,cur_offset;
    INT16U 	pos,idx,data_len,sec_count,read_count,index;
    INT8U filehead[]={'T','O','P','S','C','O','M','M'};
    INT8U buffer[36];
    INT8U break_point;
    
    pos = POS_DATA;
    break_point=0;
    
    set_pn_fn(resp->buffer, &pos, DA_P0, DT_F2);
    //�ȶ�ȡ�ļ�ˮӡ��������ļ��Ƿ����
	os_spansion_readArray(FLADDR_FILE_INFO,buffer,8);//д�ļ�ˮӡ
	if(0 == compare_string(filehead,buffer,8))
    {
    	resp->buffer[pos++] =  1;//�ļ�����
        //��ȡ�ļ������Ϣ
		os_spansion_readArray(FLADDR_FILE_INFO+8,buffer,36);//�ļ������Ϣ
        mem_cpy(&data_len,buffer+30,2);
        mem_cpy(&filesize,buffer+32,4);
        if(filesize>537600)//����ļ����ȴ���ʵ�������ȣ�����������
        {
            filesize=0;
        }
        if((data_len>=2048) || (data_len==0))
        {
            data_len=512;
        }
        OSTASK_PROTECT_BEGIN;
        sec_count=filesize/data_len;
        if(filesize % data_len !=0)
        {
            sec_count++;
        }
        read_count=sec_count/32;
        if(sec_count % 32 !=0)
        {
            read_count++;
        }
        
     	OSTASK_PROTECT_END;
        
        mem_cpy(resp->buffer+pos+4,buffer,30);//����汾��Ϣ
        //��ȡ�ϵ�������Ϣ
        for(idx=0;idx<read_count;idx++)
        {
            //һ�ζ�32�ֽ�
			os_spansion_readArray(FLADDR_PACKAGE_RECORD+idx*32,buffer,32);
            //��λ��飬�����Ϊ��Ϊ0��˵��û������
			for(index=0;index<256;index++)
            {
            	cur_offset=idx*256+index;
                if(0 !=get_bit_value(buffer,32,index))
                {
			        mem_cpy(resp->buffer+pos,&cur_offset,4);//��һ�����ݶ�ƫ��
                    break_point=2;
                    break;
                }
                //��������ļ��ܶ�������˵���Ѿ��������
            	if(cur_offset>=sec_count)
                {
                   break_point=1;
                   break;
                }
            }
            //����Ѿ��ҵ��˶ϵ���Ѿ�������ϣ��������ټ�������
            if(break_point>0)
            {
                   break;
            }
		}
    }
    else
    {
        mem_set(resp->buffer+pos,5,0);
        pos+=5;
        mem_set(resp->buffer+pos,30,0xFF);//����汾��Ϣ
        pos+=30;
    }
    SetFrameLenThenSendFrame(resp,pos);



}
/*+++
  ���ܣ����ɹ�Զ������F3,��λ�ļ�����
  ������
       PLMS_RESP *resp        Ӧ��ṹ�����
       INT16U frameLen       ���ݵ�Ԫ�ĳ���
       INT8U *frameData,      ���ݵ�Ԫ
---*/
void process_ftp_nm_F3(PLMS_RESP *resp,INT16U frameLen,INT8U *frameData)
{
    INT16U 	pos;
    
    pos = POS_DATA;
	os_spansion_erase_sector(0,9);
    set_pn_fn(resp->buffer, &pos, DA_P0, DT_F3);
    resp->buffer[pos++] =  0x55;//55��ʾ�ɹ� AA��ʾʧ��
    SetFrameLenThenSendFrame(resp,pos);

}
/*+++
  ���ܣ����ɹ�Զ������F4,�ļ����
  ������
       PLMS_RESP *resp        Ӧ��ṹ�����
       INT16U frameLen       ���ݵ�Ԫ�ĳ���
       INT8U *frameData,      ���ݵ�Ԫ
---*/
void process_ftp_nm_F4(PLMS_RESP *resp,INT16U frameLen,INT8U *frameData)
{
    INT16U 	pos;
    INT8U filehead[]={'T','O','P','S','C','O','M','M'};
    pos = POS_DATA;
	//�洢�����Ϣ��FLADDR_FILE_INFO
    if(frameLen==256)
    {
    	os_spansion_writeArray(FLADDR_FILE_INFO+8,frameData,frameLen);
        os_spansion_writeArray(FLADDR_FILE_INFO,filehead,8);//д�ļ�ˮӡ
    }
    set_pn_fn(resp->buffer, &pos, DA_P0, DT_F4);
    resp->buffer[pos++] =  0x55;//55��ʾ�ɹ� AA��ʾʧ��
    SetFrameLenThenSendFrame(resp,pos);

}

void default_param_neimeng(void)
{




}
#endif


