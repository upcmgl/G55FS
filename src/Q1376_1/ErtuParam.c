#include "app_3761.h"
#include "ErtuParam.h"
#include "../main_include.h"


//=====================================================================================
//
//                    �ն�������Ϣ
//
//=====================================================================================
//�Ƶ�ErtuParam.h
//#define SOFT_VER_YEAR  0x18
//#define SOFT_VER_MONTH 0x11
//#define SOFT_VER_DAY   0x02
//
//
//#define SOFT_VER_MAIN   '1'
//#define SOFT_VER_DOT    '.'
//#define SOFT_VER_MIN1   '0'
//#define SOFT_VER_MIN2   '2'
//
//#define HARDWARE_VER_H   '1'
//#define HARDWARE_VER_DOT '.'
//#define HARDWARE_VER_L1  '0'
//#define HARDWARE_VER_L2  '1'

struct _DataItem
{
  INT16U fn;  //��Ϣ���ʶ
  INT32U eeAddr;  //EEPROM�еĴ��λ��
  INT16U dataLen; //D0-D11Ϊ��������, ���ֽ�Ϊ����,���ֽ�Ϊ����������
};

const struct _DataItem DataItemList[]  =
{
   //-------------�ն�----------------
   {DT_F1, EEADDR_SET_F1,   6},    //�ն�ͨ�Ų���
   {DT_F3, EEADDR_SET_F3,  28},    //��վIP��ַ�Ͷ˿��Լ�APN
   {DT_F4, EEADDR_SET_F4,  16},    //��վ�绰����Ͷ������ĺ���
   {DT_F7, EEADDR_SET_F7,  64},    //�ն�IP��ַ�Ͷ˿�
   {DT_F8, EEADDR_SET_F8,   8},    //�ն�����ͨ�Ź�����ʽ����̫ר��������ר����
   {DT_F9, EEADDR_SET_F9,  16},    //�ն��¼���¼��������
   {DT_F10,EEADDR_SET_F10, 33},    //�ն˵��ܱ�/��������װ�����ò���,
   {DT_F16,EEADDR_SET_F16, 64},    //����ר���û���������

//   {DT_F25,EEADDR_SET_F25,  11},     //�������������
//   {DT_F26,EEADDR_SET_F26,  57},     //��������ֵ����
//   {DT_F27,PIM_METER_F27,  24},     //������ͭ���������
//   {DT_F28,PIM_METER_F28,   4},     //�����㹦�������ֶ���ֵ
//   {DT_F29,PIM_METER_F29,   12},     //�ն˵��ص��ܱ���ʾ��
//   {DT_F30,PIM_REC_FLAG,    1},     //�ն�̨�����г���ͣ��/Ͷ������
//   {DT_F31,PIM_AUX_NODES,  121},    //�ز��ӽڵ㸽���ڵ��ַ,�����20�������ڵ����
//
//   {DT_F33,EEADDR_SET_F33,  110},    //�ն�̨�����г������в������ã�֧�ֳ���˿�3��
//   {DT_F34,EEADDR_SET_F34,  186},    //���ն˽ӿڵ�ͨ��ģ��Ĳ�������:֧��31���˿�
//   {DT_F35,EEADDR_SET_F35,  256},    //�ն�̨�����г����ص㻧���ã���λ��Ӧ��ʾ��������ţ������Ա�ʾ2048����
//   {DT_F36,EEADDR_SET_F36,   4},    //ͨ��������������
//   {DT_F37,EEADDR_SET_F37,  23},   //�ն˼���ͨ�Ų�����֧��4��
//   {DT_F38,EEADDR_SET_F38,  32},   //1�������������ã�16*16*32 =8192�ֽڣ�
//   {DT_F39,EEADDR_SET_F39,  32},   //2�������������ã�16*16*32 =8192�ֽڣ�
//   {DT_F57,EEADDR_SET_F57,   3},    //�ն������澯����?��ֹ����
//   {DT_F58,EEADDR_SET_F58,   1},    //�ն��Զ��������
//   {DT_F59,EEADDR_SET_F59,   5},    //���ܱ��쳣�б���ֵ�趨
//
//    //------------����� 4������---------------------------
//   {DT_F65,EEADDR_SET_F65,   1029},    //��ʱ����1�������������� 1029*4=65856
//   {DT_F66,EEADDR_SET_F66,   1029},    //��ʱ����2�������������� 1029*4=65856
//   {DT_F67,EEADDR_SET_F67,    1},     //��ʱ����1��������������/ֹͣ���� 1*4
//   {DT_F68,EEADDR_SET_F68,    1},     //��ʱ����2��������������/ֹͣ���� 1*4
//
   #ifdef __RTUADDR_IS_6BYTE__
   {DT_F89,EEADDR_CFG_DEVID, 6},     //�ն˵�ַ
   #else
   {DT_F89,EEADDR_CFG_DEVID, 4},     //�ն˵�ַ
   #endif
//
//   {DT_F97,EEADDR_SET_F97,    25},    //ͣ�����ݲɼ����ò���(�洢λ�ÿ��������10�������㣬ʵ������ֻ�����������3��)
//   {DT_F98,EEADDR_SET_F98,  13},    //ͣ���¼������ֵ����
//
//    {DT_F104,PIM_METER_F104,2},        //���߶����ܶȲ���
//    #ifdef __METER_EVENT_GRADE__
//    {DT_F105,PIM_METER_F105,1},        //���ܱ��¼��ּ��������
//    {DT_F106,EEADDR_NONE,1},        //���ܱ��¼��ּ�����
//    {DT_F107,EEADDR_SET_F107,2},        //���ܱ��¼��ּ����ڲ���
//    #endif
   {DT_F301,EEADDR_SET_F301,264},    //������¼����
   {DT_F701,EEADDR_SET_F701,4},    //�������
   {DT_F702,EEADDR_SET_F702,5},    //����բ ��Ч��־1�ֽ�+��ʼ����ʱ�� 4�ֽ�

};


/*+++
   ���ܣ� �����豸��������ĵ�ַ�����ݳ���
   ����ֵ�� TRUE/FALSE
---*/
BOOLEAN  GetParamAddrLen(
        INT16U fn,    //[IN] ��Ϣ�����
        INT32U *addr,    //[OUT]�洢��ַ
        INT16U *len)     //[OUT]�������
{
   INT16U idx,min,max;
   
   *len=0;
   min=0;
   max = sizeof(DataItemList) / sizeof(struct _DataItem) -1;
   while(min<=max)
   {

      idx=(min+max)>>1;   //��2
      if(fn == DataItemList[idx].fn)
      {
           *addr = DataItemList[idx].eeAddr;
           *len  = DataItemList[idx].dataLen & 0x0FFF;  //��12�ֽ�Ϊ�������
           return TRUE;
      }
      else if(fn > DataItemList[idx].fn)
      {
         min = idx+1;
      }
      else 
      {
         if(idx==0) break; // ����Сֵ��ҪС
         max = idx-1;              
      }
   } 
   return FALSE;
}




//F1���ն˰汾��Ϣ,��ȡʱ��Ҫ�������豸���
const INT8U  ertu_cfg_verinfo[LEN_ERTU_CFG_VERINFO]=
{
   'T','O','P','S',                             //���̴���          ASCII	4

   '0','0', '0','0','0','0','0','0',            //�豸���          ASCII	8


   SOFT_VER_MAIN,SOFT_VER_DOT,SOFT_VER_MIN1,SOFT_VER_MIN2, //�ն�����汾��    ASCII	4



   SOFT_VER_DAY,SOFT_VER_MONTH,SOFT_VER_YEAR,   //�ն�����������ڣ�������  ���ݸ�ʽ20	3

   'G','5','5','0','0','0','0','0','0','4','M', //�ն�����������Ϣ��	ASCII	11

   '1','3','0','1',                             //�ն�ͨ�Ź�Լ�汾��	ASCII	4

   HARDWARE_VER_H,HARDWARE_VER_DOT,HARDWARE_VER_L1,HARDWARE_VER_L2,  //�ն�Ӳ���汾��	ASCII	4

   0x01,0x08,0x17,                              //�ն�Ӳ���������ڣ�������	���ݸ�ʽ20	3
};

//��ü���������ֵ��ʵ�ʳ��ȣ���һЩ�������Ƕ����ģ��Ȱ���󳤶ȱ��棨�ౣ����û�й�ϵ����Ȼ���ٸ��³���

INT16U get_ertu_params_length(INT8U *frame,INT16U fn,INT16U itemDataLen)
{
    INT16U resp_len;
    INT16U pos;
    INT8U  i,j,tmp;
    resp_len = itemDataLen;
    switch(fn)
    {
    case DT_F41:
        pos = 0;
        resp_len = 1;
        for(j=0;j<3;j++)
        {
            if(frame[0]&(0x01<<j))
            {
                resp_len ++;
                pos++;
                tmp = frame[pos];
                for(i=0;i<8;i++)
                {
                    if(tmp&(0x01<<i))
                    {
                        resp_len+=2;
                        pos+=2;
                    }
                }
            }
        }
         break;
    case DT_F97:
        resp_len = frame[2]+frame[3]*0x100;
        break;
    case DT_F251:
        if(frame[0] > 140)
        {
            frame[0] = 0;
        }
        resp_len = frame[0]+1;
        break;
    case DT_F252:
//        if(frame[0] > 3)
//        {
//            frame[0] = 3;
//        }
        resp_len = 1;
        pos = 1;
        for(i=0;i<frame[0];i++)
        {
            pos += 8;
            resp_len += 8;
            tmp = frame[pos];
            resp_len += 1;
            pos += 1;
            resp_len += tmp;
            pos += tmp;
        }
        break;
    case DT_F301:
        resp_len = frame[8]+9;
        break;
    case DT_F303:
        resp_len = 6+1+frame[6]*(1+6+6+1);
        break;
    default:
        break;
    }
    return resp_len;
} 


/*+++
  ����: �����ն������
  ����:
        INT8U *buffer,             [����]������
        INT16U fn,                 [����]��һ����Ϣ��
        INT8U *itemData,           [����]����
        INT16U *itemDataLen        [���]������Ŀ����
  ����:
        INT8U ���ý��
---*/
INT8U  set_ertu_params(INT16U fn,INT8U *itemData,INT16U *itemDataLen)
{
   INT32U   eeAddr;
   INT8U    error,itemData_tmp[50]={0};



   error = 0;

   switch(fn)
   {
   case DT_F796:
       error = set_ertu_custom_param(itemData,itemDataLen);
       break;
   default:
        //�����ն˲���,�̶���������,ֱ��д�뼴��
		
        //��ȡ���ݵ�Ԫ�ĳ��ȣ��洢��ַ
        if(TRUE == GetParamAddrLen(fn,&eeAddr,itemDataLen) )
        {
            if(EEADDR_NONE == eeAddr)
            {
                error = 1;
                break;
            }
            //д�����
            *itemDataLen = get_ertu_params_length(itemData,fn,(*itemDataLen));
			//�������ȶ��Ƿ���
			if(DT_F3==fn)
			{
				fread_ertu_params(eeAddr,itemData_tmp,(*itemDataLen));
			}
            fwrite_ertu_params(eeAddr,itemData,(*itemDataLen));

            if(DT_F36==fn)
            {
//                fread_ertu_params(EEADDR_SET_F36,g_run_param.max_month_gprs_bytes,4);
//                if(gSystemCtrl.net_disconnect_flag==0xAA)
//                {
//                        gSystemCtrl.net_disconnect_flag=0x00;
//
//                }
//                if(gSystemCtrl.gprs_bytes_over_flag!=0)
//                {
//                    gSystemCtrl.gprs_bytes_over_flag =0x00;
//                    fwrite_array(FILEID_RUN_DATA,FMDATA_GPRS_BYTES_OVER_CHECK_EVENT,&gSystemCtrl.gprs_bytes_over_flag,1); 
//                }
            }
			else if(DT_F3 == fn)
			{
				if(compare_string(itemData,itemData_tmp,(*itemDataLen))!=0)
				{
					gSystemInfo.reconnect = 1;						
				}
			}
            else if(DT_F89 == fn)
            {
				mem_cpy(gSystemInfo.ertu_devid,itemData,CFG_DEVID_LEN);
            }
            else if(DT_F701 == fn)
            {
                mem_cpy(&g_app_run_data.password,itemData,4);
            }            
            else if(DT_F702 == fn)
            {
                mem_cpy(&g_app_run_data.meter_control.is_control_valid,itemData,sizeof(tagAPP_CONTROL));
            }
        }
        else
        {
            error=1;
            *itemDataLen = MAX_SIZE_PLMSDTP;
        }
        break;
   }
   return error;
}


/*+++
  ���ܣ���ѯ����������;����
  ����:
        INT16U pn
        INT16U fn
        INT8U *params  �������Ĳ�ѯ���������
        INT8U *itemData  ��Ӧ����
        INT16U *itemDataLen   ��Ӧ�ĳ���
        QUERY_CTRL *query_ctrl ��ѯ���̵Ľṹ��
        BOOLEAN *more_frame_flag   �Ƿ����֡
  ����:
        ��ѯ���
  ������

---*/

INT8U process_frame_query_3761(INT16U fn,INT8U *itemData,INT16U *itemDataLen,QUERY_CTRL *query_ctrl)
{
    INT32U eeAddr;
    INT16U meter_count=0;
    INT8U error;
    error = 0;
    eeAddr = EEADDR_NONE;

    //���Ҳ����ĵ�ַ��������Ϣ
    if((fn!=0)&&(fn!=DT_F796) )
    {
        if(FALSE == GetParamAddrLen(fn,&eeAddr,itemDataLen) )
        {
            //�������ĳ����Ϣ�࣬���ͷ񶨱���
            error = 1;
            return error;
        }
    }
    switch(fn)
    {
        case DT_F89:
        #ifndef __RTUADDR_IS_6BYTE__
            fread_ertu_params(EEADDR_CFG_DEVID,itemData,4);
            *itemDataLen = 4;
        #else
            fread_ertu_params(EEADDR_CFG_DEVID,itemData,6);
            *itemDataLen = 6;
        #endif
            break;
        case DT_F796:
            query_ertu_custom_param(itemData,itemDataLen,(INT8U*)query_ctrl);
            break;
        default:
            if(EEADDR_NONE==eeAddr  || 0x00==fn)  break;

            fread_ertu_params(eeAddr,itemData,(*itemDataLen));
            *itemDataLen = get_ertu_params_length(itemData,fn,(*itemDataLen));
            if(DT_F10 == fn)
            {
                mem_cpy(gmeter_doc.meter_no,gSystemInfo.meter_no,6);
                gmeter_doc.meter_idx[0] = 1;
                gmeter_doc.meter_idx[1] = 0;
                gmeter_doc.spot_idx[0] = 1;
                gmeter_doc.spot_idx[1] = 0;
                gmeter_doc.protocol = GB645_2007;
                gmeter_doc.baud_port.port = COMMPORT_485_CY;
                gmeter_doc.fl_count = 4;
                gmeter_doc.baud_port.port = 1;
				#ifndef __METER_SIMPLE__
                gmeter_doc.meter_class.meter_class = 2;//����2 ����1
				#else
				gmeter_doc.meter_class.meter_class = 1;//����2 ����1
				#endif
                gmeter_doc.meter_class.user_class = 5;
                itemData[0]=1;
                itemData[1]=0;
                mem_cpy(itemData+2,(INT8U *)&gmeter_doc,sizeof(METER_DOCUMENT));
                *itemDataLen = sizeof(METER_DOCUMENT)+2;

                   meter_count = bin2_int16u(query_ctrl->req);
                   //�����������
                   query_ctrl->req_bytes = (meter_count<<1) + 2;
            }
            break;
    }
    return error;

}




INT8U process_frame_set_3761(INT16U fn,INT8U *itemData,INT16U *itemDataLen,INT16U DataLen)
{
    INT8U error=0;

    if(DT_F0 == fn)
    {
        //F0:�ն��߼���ַ,��Ҫ��Ψһ���������ݱ�ʶ
        if(DataLen == 4)
        {
            fwrite_ertu_params(EEADDR_CFG_DEVID,itemData,CFG_DEVID_LEN);
            mem_cpy(gSystemInfo.ertu_devid,itemData,CFG_DEVID_LEN);

            *itemDataLen = CFG_DEVID_LEN;

            if( (0xAA == *(itemData + *itemDataLen))   && (0x55 == *(itemData + *itemDataLen + 1)) )
            {
                //���̴���
                fwrite_ertu_params(EEADDR_CFG_VENDOR,itemData + *itemDataLen + 2,4);
                *itemDataLen += 6;
            }
        }
        else
        {
            error=1;
            *itemDataLen = DataLen + 4;  //��ʹѭ������
        }
    }
    else
    {
        error = set_ertu_params(fn,itemData,itemDataLen);
    }

    return error;
}



void  process_frame_set(objRequest *pRequest,objResponse *pResp)
{
	INT16U pos_event;
    INT16U pn,fn;
    INT16U itemDataLen;
    INT16U pngroup[64] = {0};
    INT16U fngroup[8] = {0};
    INT16U tmp_pos;  //����pn��fn���ʱ�Ĵ���
    INT8U result_flag,error=0;
    INT8U pngroupcount;
    INT8U fngroupcount;
    INT8U fn_idx;
    INT8U png_idx;
    INT8U fng_idx;
	INT8U event_record[EVENT_RECORD_SIZE];
	
    ClrTaskWdt();

    //TODO:������Ĵ�����ͳһ�Ľӿ����AFN��Fn�Ƚ��д���

    //���ÿ����룺DIR=1,PRM=0, FUNC=8  ��Ӧ���ģ��û�����
    pResp->frame[POS_CTRL] = CTRLFUNC_SET_DIR | CTRLFUNC_USERDATA;
    pResp->work_flag[WORK_FLAG_SEQ_RSEQ] |= MASK_FIR | MASK_FIN;
    pResp->work_flag[WORK_FLAG_SEQ_RSEQ] &= ~MASK_CON;

    //������ý����־�ֽ�
    result_flag=0;

    pResp->pos = POS_DATA;
    pResp->frame[POS_AFN] = AFN_ACK;

    //���Ȱ��հ����ݱ�ʶ׼��
    fn = DT_F3;
    pn = DA_P0;
    set_pn_fn(pResp->frame, &pResp->pos, pn, fn);
    pResp->frame[pResp->pos++]=AFN_SET;

    //��ȡF3:�ն˲���״̬
    //fread_array(FILEID_RUN_DATA,PIM_C1_F3,BS248,31);


    //ERC3:���������¼
    event_record[EVENT_POS_ERC_LEN]=6;
    event_record[EVENT_POS_ERC_CONTENT]= pResp->frame[POS_MSA]>>1;  //����վ��ַ
    pos_event = EVENT_POS_ERC_CONTENT+1;

    while((pRequest->pos - POS_DATA) < pRequest->userDataLen)
    {

        //��ȡ���ݵ�Ԫ��ʶ
        pn = bin2_int16u(pRequest->frame + pRequest->pos);
        fn = bin2_int16u(pRequest->frame + pRequest->pos + 2);

         //�����Զ��������ն˵�ַ����������ǵ�һ��pnfnΪ0
        if((pn==0) && (fn==0) && ((pRequest->pos - POS_DATA)>0))
        {
            break;
        }

        //ȷ�ϱ����е����ݵ�Ԫ��ʶ
        set_pn_fn(pResp->frame, &pResp->pos, pn, fn);

        pRequest->pos += 4;

        //��Ϣ�㼯�ϴ���,����ʱ��ѭ��Fn����Pn,��С����˳��
        pngroupcount = parse_pn_to_pngroup(pn,pngroup);
        fngroupcount = parse_fn_to_fngroup(fn,fngroup);

        error=0;
        tmp_pos = 0;
        for(png_idx=0;png_idx < pngroupcount;png_idx++)
        {
            pn =  pngroup[png_idx];
            //��pnѭ������ʱ���ƶ�posָ�뱣֤fn�������ȷ��
            pRequest->pos -= tmp_pos;
            tmp_pos = 0;
            for(fng_idx=0;fng_idx<fngroupcount;fng_idx++)
            {
                fn = fngroup[fng_idx];

                //���ò�����־λ
                fn_idx = trans_set_fn_2_fnidx(fn);

                if(event_record[EVENT_POS_ERC_LEN] < EVENT_RECORD_SIZE-2)
                {
                    //��������¼���¼�����ݵ�Ԫ��ʶ

                    mem_cpy(event_record+pos_event,(INT8U *)&pn,2);
                    pos_event += 2;
                    mem_cpy(event_record+pos_event,(INT8U *)&fn,2);
                    pos_event += 2;
                    event_record[EVENT_POS_ERC_LEN]+=4;
                }

                error = process_frame_set_3761(fn,pRequest->frame+pRequest->pos,(INT16U*)&itemDataLen,pRequest->pos-POS_DATA);

                if((error == 0)&&(g_meter_set_flag == 0x55))
                {
                    if(DT_F1 == fn)
                    {
                        set_bit_value(&(gSystemInfo.ertu_param_change),1,0);
                    }
                    if(DT_F3 == fn)
                    {
                        set_bit_value(&(gSystemInfo.ertu_param_change),1,1);
                    }
                    if(DT_F16 == fn)
                    {
                        set_bit_value(&(gSystemInfo.ertu_param_change),1,2);
                    }
                    if(DT_F89 == fn)
                    {
                        set_bit_value(&(gSystemInfo.ertu_param_change),1,3);
                    }
                }

                if(DT_F1 == fn)
                {
                    //4.��ȡ����
                    fread_ertu_params(EEADDR_HEARTCYCLE,(INT8U*)&(gSystemInfo.heart_cycle),1);
                }
           
                if((pResp->channel == CHANNEL_RS232) && (DT_F3==fn))
                {
                    //set_system_flag(SYS_GPRS_CHG,SYS_FLAG_BASE);
                }

                //���F7���������ı��Ҳ�����̫����ô����������װ��F7����
                if((pResp->channel!=CHANNEL_ETH_SERVER) && (pResp->channel!=CHANNEL_ETH_CLIENT) && (DT_F7==fn))
                {
                    //tops_platform_set_eth_update_param();
                }

                pRequest->pos += itemDataLen;
                tmp_pos += itemDataLen;
                if(error!=0) break;
            }// end of for(fng_idx...
            if(error!=0) break;
        }//end of for(png_idx....

        pResp->frame[pResp->pos++]=error;
        if(error==0)
            result_flag |= 0x01;
        else
            result_flag |= 0x02;
        if(error!=0) break;


    } // end of while..

    //�������óɹ���д�ն˲�������״̬F3
    if(error == 0)
    {
        //fwrite_array(FILEID_RUN_DATA,PIM_C1_F3,BS248,31);
    }


    //�������ò���
    pn = DA_P0;
    if(result_flag !=0)
    {
        fn = (result_flag==1) ? DT_F1 :  DT_F2;
        pResp->pos=POS_DATA;
        set_pn_fn(pResp->frame, &pResp->pos, pn, fn);
    }


    app_setFrameLenThenSendFrame(pRequest,pResp);

  //  �ɹ��󣬼�¼��������¼�
//   if(error == 0)
//    event_params_set(event_record);

}

/*+++
   ���ܣ� ��ѯ����
   ����:
   objRequest *pRequest ����֡������
   objResponse *pResp   Ӧ��֡������

   ���б��ģ�
         ���ݵ�Ԫ��ʶ1
         ...
         ���ݵ�Ԫ��ʶn
         TP
   ���б��ģ�  
         ���ݵ�Ԫ��ʶ1
         ���ݵ�Ԫ1
         ...
         ���ݵ�Ԫ��ʶn
         ���ݵ�Ԫn
         EC 
         Tp
---*/

void  process_frame_query(objRequest *pRequest,objResponse *pResp)
{
    INT16U pn,fn;
    INT16U pngroup[64]={0};
    INT16U fngroup[8]={0};
    INT16U itemDataLen;
    INT16U pn_idx;
    QUERY_CTRL  query_ctrl;
    INT8U error;
    INT8U png_idx,fng_idx;
    INT8U pngroupcount;
    INT8U fngroupcount;
    BOOLEAN  more_frame_flag;


    ClrTaskWdt();
    //���ÿ����룺DIR=1,PRM=0, FUNC=8  ��Ӧ���ģ��û�����
    pResp->frame[POS_CTRL] = CTRLFUNC_SET_DIR | CTRLFUNC_USERDATA;


    //�״���ӦF10,F89��־
    query_ctrl.first_ctrl.first_flag = 0xFF;
    query_ctrl.from_seq = 0;
    //��֡��־��1
    pResp->work_flag[WORK_FLAG_IS_FIRST_FRAME]=1;


    /////////////////////////////////
    //    ��ʼ��֯��Ӧ֡
    /////////////////////////////////

//RESP_PARAMS_QUERY:

    pResp->pos = POS_DATA;
    while((pRequest->pos - POS_DATA) < pRequest->userDataLen)
    {
       ClrTaskWdt();

       //��ȡ���ݵ�Ԫ��ʶ
       pn = bin2_int16u(pRequest->frame + pRequest->pos);
       fn = bin2_int16u(pRequest->frame + pRequest->pos + 2);
       pRequest->pos += 4;

       //��Ϣ�㼯�ϴ���,����ʱ��ѭ��Fn����Pn,��С����˳��

       pngroupcount = parse_pn_to_pngroup(pn,pngroup);
       fngroupcount = parse_fn_to_fngroup(fn,fngroup);

       //ѭ������PN,��С�����˳��
       more_frame_flag = FALSE;
       for(png_idx=0;png_idx < pngroupcount;png_idx++)
       {
          //ת��������Ϊ��� DA_P0=0  DA_P1=1  DA_P2=2 DA_P3=3
          pn_idx = trans_set_pn_2_pnidx(pngroup[png_idx]);
          if(pn_idx > MAX_METER_COUNT)
          {
              continue;
          }
          //ѭ������FN,��С�����˳��
          for(fng_idx=0;fng_idx<fngroupcount;fng_idx++)
          {
             fn = fngroup[fng_idx];
             itemDataLen = 0;

             //�������ݱ�ʶ
             if(fn != 0)
             {
                 set_pn_fn(pResp->frame, &pResp->pos, pngroup[png_idx], fn);
             }

             query_ctrl.req_pos = pRequest->pos;
             query_ctrl.req = pRequest->frame + pRequest->pos;
             query_ctrl.left_resp_len =  pResp->max_reply_pos - pResp->pos - 4;
             query_ctrl.req_bytes = 0;

             error = process_frame_query_3761(fn,pResp->frame+pResp->pos,&itemDataLen,(QUERY_CTRL*)&query_ctrl);

             if(error != 0)
             {
                 app_send_NAK(pRequest,pResp);
                 return;
             }
             //����Ƿ��Ѿ����뵽����״̬,���״̬�Ĳ�������Ϊ��Ϣ�౾���������ܴ�֧�ֶ�δ���
             if(more_frame_flag==TRUE)
             {
                 if(POS_DATA == pResp->pos)
                 {
                      //���ܹ����з�������
                      app_send_NAK(pRequest,pResp);
                      return;
                 }
                 //�ƶ�Ӧ���ĵ�ǰλ��
                 pResp->pos += itemDataLen;

                 if(FALSE == app_send_response_frame(pRequest,pResp,0)) return;

                 pResp->pos = POS_DATA;
                 pRequest->pos=query_ctrl.req_pos;
                 //������֡
                 fng_idx--;
                 continue;
             }

             //�ж��Ƿ񳬹�����û�������Ч���ȣ�����ط�������������Ĵ���ͬ��
             //�糬��������ж�֡����
             if(pResp->pos + itemDataLen + 4 > pResp->max_reply_pos)
             {
                 if(POS_DATA == pResp->pos-4)   //ԭ������������룬��Ϊ�Ѿ�����һ��PNFN ,2011.3.31 by zyg    if((POS_DATA == pos)  && (TRUE==first_frame_flag))
                 {
                      //ֻ����֡���ܷ�֡ʱ���ŷ��ͷ���֡������ֱ�ӷ���
                      if(TRUE==pResp->work_flag[WORK_FLAG_IS_FIRST_FRAME])
                      app_send_NAK(pRequest,pResp);
                      return;
                 }

                 //���Խ��з�������,������Ӧ��
                 if(FALSE == app_send_response_frame(pRequest,pResp,0)) return;
                 pResp->pos = POS_DATA;
                 pRequest->pos=query_ctrl.req_pos;
                 //������֡
                 fng_idx--;
                 continue;
             }
             //�ƶ�Ӧ���ĵ�ǰλ��
             pResp->pos += itemDataLen;

             //�ƶ��������λ��
             pRequest->pos += query_ctrl.req_bytes;

          }//end of for(fng_idx...
       }//end of for(png_idx...
    } // end of while(idx...

    //������Ӧ��
    app_send_response_frame(pRequest,pResp,TRUE);

}


/*+++
  ���ܣ���Ӧ��վ�����ն����� [AFN=0x09]
  ������
        PLMS_RESP *resp,            ��Ӧ����ṹ
        INT16U frameLen,            �������û�����������
        INT8U *frameData    �������û�������
---*/
void  process_read_config_info(objRequest *pRequest,objResponse *pResp)
{
    INT16U pn,fn;
    INT16U itemDataLen;

    //���ÿ����룺DIR=1,PRM=0, FUNC=8  ��Ӧ���ģ��û�����
    pResp->frame[POS_CTRL] = CTRLFUNC_SET_DIR | CTRLFUNC_USERDATA;

    //��֡��־��1
    pResp->work_flag[WORK_FLAG_IS_FIRST_FRAME]=1;


    /////////////////////////////////
    //    ��ʼ��֯��Ӧ֡
    /////////////////////////////////

    RESP_PARAMS_READCONFIG:

    pResp->pos = POS_DATA;
    while((pRequest->pos - POS_DATA) < pRequest->userDataLen)
    {
        //��ȡ���ݵ�Ԫ��ʶ
        pn = bin2_int16u(pRequest->frame + pRequest->pos);
        fn = bin2_int16u(pRequest->frame + pRequest->pos + 2);
        pRequest->pos += 4;

        //�����Ϣ��PN, ��Ϣ��ֻ����P0,�������,����������һ�����ݵ�Ԫ��ʶ
        if(pn != DA_P0) continue;

        //�����Ϣ��FN,��Ϣ��DTֻ���ǵ�0��
        #ifdef __COUNTRY_ISRAEL__
        if ((fn & 0xFF00) && (fn != DT_F24))  continue;
        //#else
        //if(fn & 0xFF00) continue;
        #endif

        //���㱾�ζ�ȡ����Ϣ�����Ϣ����
        itemDataLen = compute_readcfg_fn_datalen(fn);
        if(itemDataLen == 0) continue;

        //�ж��Ƿ񳬹�����û�������Ч����
        //�糬��������ж�֡����
        if(pResp->pos + itemDataLen + 4 > pResp->max_reply_pos)
        {
           //������Ӧ��
           if(FALSE == app_send_response_frame(pRequest,pResp,0) ) return;
           pRequest->pos -= 4;

           //������֡
           goto RESP_PARAMS_READCONFIG;
        }

        //�������ݱ�ʶ
        set_pn_fn(pResp->frame, &pResp->pos, pn, fn);

        //��ȡ��Ϣ��FN������
        readcfg_fn_data(fn,pResp->frame + pResp->pos);
        if(fn==DT_F11)
        {
            //itemDataLen=readcfg_spot_info(pResp->frame+pResp->pos,pResp->max_reply_pos);
        }
        #ifdef __PROVICE_JIANGSU__
        //F12������ͨ��ģ��汾��Ϣ(˫ģ)
        if(fn == DT_F12)
        {
           itemDataLen = readcfg_plc_verinfo(pResp->frame+pResp->pos,pResp->max_reply_pos);
        }
        //F13���ɼ����汾��Ϣ
        if(fn == DT_F13)
        {
            itemDataLen = readcfg_cjq_verinfo(pRequest->frame + pRequest->pos,pResp->frame+pResp->pos,pResp->max_reply_pos);
        }
        #endif
		if(fn == DT_F249)
		{
			itemDataLen = app_get_F249_data(pResp->frame+pResp->pos);	
		}

        pResp->pos += itemDataLen;
    } // end of while(idx...

    //������û���κ���Ӧ����,���ͷ񶨱���
    if(pResp->pos == POS_DATA)
    {
         app_send_NAK(pRequest,pResp);
    }
    else
    {
        //������Ӧ��
        app_send_response_frame(pRequest,pResp,TRUE);
    }
}

/*+++
  ����: �����ն�����,������Ϣ����������ݳ���
  ����:
        INT16U fn
  ����:
        INT16U ���ݳ���,���ĳ����Ϣ����,Ҳ�����䳤��
  ����:
      F1: �ն˰汾��Ϣ  	  41
      F2: ���������ͨ�Ŷ˿�����	  91	ͨ�Ŷ˿�������10�����ǡ�
      F3: ��������      	  16
      F4: �ն�֧�ֵĲ�������      32
      F5: �ն�֧�ֵĿ�������      32
      F6: �ն�֧�ֵ�1����������   32
      F7: �ն�֧�ֵ�2����������   32
      F8: �ն�֧�ֵ��¼���¼����   8
      F9: Զ��ͨѶģ��汾��Ϣ    56
      F10: ����ͨѶģ��汾��Ϣ   15
---*/
INT16U compute_readcfg_fn_datalen(INT16U fn)
{
   INT16U  datalen;

   datalen = 0;

   #ifdef __COUNTRY_ISRAEL__
   if(fn == DT_F24) return 1;
   #endif

   if(fn == DT_F249)  return 51;


   if((fn & 0xFF00) == 0)
   {
   #ifdef __PROVICE_SHANXI__
   if(fn & DT_F1)  datalen += LEN_ERTU_CFG_VERINFO+6;//sizeof(ertu_cfg_verinfo)+6;
   #else
   if(fn & DT_F1)  datalen += LEN_ERTU_CFG_VERINFO;//sizeof(ertu_cfg_verinfo);
   #endif
//   if(fn & DT_F2)  datalen += sizeof(ertu_cfg_ioports);
//   if(fn & DT_F3)  datalen += sizeof(ertu_cfg_options);
//   if(fn & DT_F4)  datalen += sizeof(ertu_cfg_opt_set);
//   if(fn & DT_F5)  datalen += sizeof(ertu_cfg_opt_ctrl);
//   if(fn & DT_F6)  datalen += (sizeof(ertu_cfg_opt_c1data)-2)*6+2;
//   if(fn & DT_F7)  datalen += (sizeof(ertu_cfg_opt_c2data)-2)*6+2;
//   if(fn & DT_F8)  datalen += sizeof(ertu_cfg_opt_events);
   }
   else if((fn & 0xFF00) == 0x0100)
   {
       if(fn & DT_F1)  datalen += 46;
       if(fn & DT_F2)  datalen += 15;
       if(fn & DT_F3)  datalen += 4;   //TODO:ɽ����չF11 �ն���Ч�����㣬�ȱ�֤��һ�����ȷ���
       if(fn & DT_F4)  datalen += 30;  //����F12  ����ͨ��ģ��汾��Ϣ��˫ģ��
       if(fn & DT_F5)  datalen += 8;   //����F13  �ɼ����汾��Ϣ
   }
   return datalen;
}



/*+++
  ����:�����ն�����,��ȡ��Ϣ�����������
  ����:
       INT16U fn      ��Ϣ��
       INT8U  resp    ������
---*/

void readcfg_fn_data(INT16U fn,INT8U *resp)
{
    INT16U pos;
    #ifdef __PROVICE_SHANXI__
    INT8U  temp[30];
    #endif

   pos = 0;


    #ifdef __PROVICE_HEILONGJIANG__
    if(fn == DT_F249)  //��������չ
    {
       mem_cpy(resp+pos,gSystemInfo.ertu_devid,4);//read_fmArray(EEADDR_CFG_DEVID,resp+pos,4);
       pos += 4;
       resp[pos++] = 6;  //6ר���ն�3��(FKXA2X)��7ר���ն�3��(FKXA4X)��8ר���ն�3��(FCXA2X)

       if(get_system_flag(SYS_CY_PAHSE_LINE,SYS_FLAG_BASE))
       {
           resp[pos++] = 1;
       }
       else
       {
           resp[pos++] = 2;
       }
       resp[pos++] = 0;
       resp[pos++] = 0x22;
       
       if(gSystemInfo.eth_client_state == ETH_NET_STATE_LOGGED_IN)
       {
           resp[pos++] = 3;//1 GPRS��2 CDMA��3��̫����4, 230 M,5 EPON, ����������
       }
       else if(REMOTE_COMMTYPE == REMOTECOMM_CDMA)
       {
           resp[pos++] = 2;//1 GPRS��2 CDMA��3��̫����4, 230 M,5 EPON, ����������
       }
       else
       {
           resp[pos++] = 1;//1 GPRS��2 CDMA��3��̫����4, 230 M,5 EPON, ����������
       }

       mem_cpy(resp+pos,gSystemInfo.gprs_ccid,20);//read_fmArray(GPRS_INFO_SIM_ID,resp+pos,20);
       for(idx=0;idx<20;idx++)
       {
          if((resp[pos+idx]>='0')&&(resp[pos+idx]<='9')
             || (resp[pos+idx]>='A')&&(resp[pos+idx]<='Z')
             || (resp[pos+idx]>='a')&&(resp[pos+idx]<='z'))
          {
          }
          else
            resp[pos+idx] = '0';
       }
       pos += 20;
       resp[pos++] = gSystemInfo.gprs_csq;//REMOTE_GPRS.csq;
       resp[pos++] = 4;
       mem_cpy(resp+pos,"XXXXXXXXXXXX",12);
       pos += 12;
       resp[pos++] = 0;
       resp[pos++] = 0;
       resp[pos++] = 0;
       resp[pos++] = 0;
       resp[pos++] = 0;
       resp[pos++] = 0;
       resp[pos++] = 0;
       resp[pos++] = get_readport_meter_count_from_fast_index(COMMPORT_485_REC)+get_readport_meter_count_from_fast_index(COMMPORT_485_CAS);//read_fmByte(FMADDR_485_MCOUNT);
       return ;
    }
    #endif

   if((fn & 0xFF00) == 0)
   {
       // F1: �ն˰汾��Ϣ  	  41
       if(fn & DT_F1)
       {
          read_C1_F1(resp+pos);
          pos += sizeof(ertu_cfg_verinfo);
          #ifdef __PROVICE_SHANXI__
          mem_cpy(temp,resp + 12,29);
          mem_cpy(resp + 18,temp,29);
          fread_ertu_params(EEADDR_VENDOR_CHIP,resp+12,6); //ɽ��Ҫ�����ն˳��Ҽ�оƬ����
          if((resp[16]==0xFF) && (resp[17]==0xFF))
          {
                resp[16]='T';
                resp[17]='C';
          }
          pos +=6;
          #endif
       }
//       // F2: ���������ͨ�Ŷ˿�����	  91	ͨ�Ŷ˿�������10�����ǡ�
//       if(fn & DT_F2)
//       {
//          mem_cpy(resp+pos, (INT8U *)ertu_cfg_ioports,sizeof(ertu_cfg_ioports));
//          //����mac��ַ
//          #if !defined(__SOFT_SIMULATOR__) && !defined(__QGDW_FK2005__)
//          mem_cpy(resp+pos+10,gSystemInfo.eth_mac_addr,6);
//          #endif
//          pos += sizeof(ertu_cfg_ioports);
//       }
//       // F3: ��������      	  16
//       if(fn & DT_F3)
//       {
//          mem_cpy(resp+pos, (INT8U *)ertu_cfg_options,sizeof(ertu_cfg_options));
//          pos += sizeof(ertu_cfg_options);
//       }
//       // F4: �ն�֧�ֵĲ�������      32
//       if(fn & DT_F4)
//       {
//          mem_cpy(resp+pos, (INT8U *)ertu_cfg_opt_set,sizeof(ertu_cfg_opt_set));
//          pos += sizeof(ertu_cfg_opt_set);
//       }
//       // F5: �ն�֧�ֵĿ�������      32
//       if(fn & DT_F5)
//       {
//          mem_cpy(resp+pos, (INT8U *)ertu_cfg_opt_ctrl,sizeof(ertu_cfg_opt_ctrl));
//          pos += sizeof(ertu_cfg_opt_ctrl);
//       }
//        // F6: �ն�֧�ֵ�1����������   32
//        if(fn & DT_F6)
//        {
//          mem_cpy(resp+pos, (INT8U *)ertu_cfg_opt_c1data,sizeof(ertu_cfg_opt_c1data));
//          pos += sizeof(ertu_cfg_opt_c1data);
//            for(idx=0;idx<5;idx++)
//            {
//                mem_cpy(resp+pos,((INT8U *)ertu_cfg_opt_c1data)+2,sizeof(ertu_cfg_opt_c1data)-2);
//                pos += sizeof(ertu_cfg_opt_c1data)-2;
//            }
//        }
//        // F7: �ն�֧�ֵ�2����������   32
//        if(fn & DT_F7)
//        {
//          mem_cpy(resp+pos, (INT8U *)ertu_cfg_opt_c2data,sizeof(ertu_cfg_opt_c2data));
//          pos += sizeof(ertu_cfg_opt_c2data);
//            for(idx=0;idx<5;idx++)
//            {
//                mem_cpy(resp+pos,((INT8U *)ertu_cfg_opt_c2data)+2,sizeof(ertu_cfg_opt_c2data)-2);
//                pos += sizeof(ertu_cfg_opt_c2data)-2;
//            }
//        }
//        // F8: �ն�֧�ֵ��¼���¼����   8
//        if(fn & DT_F8)
//        {
//          mem_cpy(resp+pos, (INT8U *)ertu_cfg_opt_events,sizeof(ertu_cfg_opt_events));
//          //pos += sizeof(ertu_cfg_opt_events);  //���澯
//        }
   }
   else if((fn & 0xFF00) == 0x0100)
   {
        // F9: Զ��ͨѶģ��汾��Ϣ   46
        if(fn & DT_F1)
        {
            mem_cpy(resp+pos,gSystemInfo.mygmr_info,sizeof(gSystemInfo.mygmr_info));
            pos += sizeof(gSystemInfo.mygmr_info);
            mem_cpy(resp+pos,gSystemInfo.myccid_info,sizeof(gSystemInfo.myccid_info));
            pos += sizeof(gSystemInfo.myccid_info);
        }
//        // F10: ����ͨѶģ��汾��Ϣ   15
//        if(fn & DT_F2)
//        {
//            mem_cpy(resp+pos, gSystemInfo.plc_ver_info,sizeof(gSystemInfo.plc_ver_info));
//            #ifdef __POWER_CTRL__
//            mem_set(resp+pos,15,0x00);
//            #endif
//        }
   }

}

/*+++
  ���ܣ� �Զ��������ն˲���(04F796)
  ������ INT8U *data,           ��������
         INT16U *itemDataLen      ��������
  ���أ�
         ������
  ������
        1��data���ݴ�CS�����CS�ȴ���
---*/
INT16U set_ertu_custom_param(INT8U *data,INT16U *itemDataLen)
{
    INT16U code;
    INT16U size;
    INT8U count;
    INT8U idx;
    INT8U crc;

    count=*data;
    data++;
    *itemDataLen=1;
    for(idx=0;idx<count;idx++)
    {
        code=bin2_int16u(data);
        size=bin2_int16u(data+2);
        data+=4;
        crc = crc16_param(data+1,size);
        if(crc!=data[0])
        {
            (*itemDataLen)+=(size+5);
            return 1;
        }

        set_custom_param(code,data+1);
        data+=(size+1);
        (*itemDataLen)+=(size+5);
    }
    return 0;
}


/*++++++++++++++++++++++++++++++++++
//������
//   1��data�Ѿ�����CS���������������
+++++++++++++++++++++++++++++++++*/
INT16U set_custom_param(INT16U paramcode,INT8U *data)
{
    INT16U tmp;
    BOOLEAN error=FALSE;

    switch(paramcode)
    {
        case CONST_CFG_DEVID:				//1UL			//6	�ն˵�ַ
            error = fwrite_ertu_params(EEADDR_CFG_DEVID,data,CFG_DEVID_LEN);
            mem_cpy(gSystemInfo.ertu_devid,data,CFG_DEVID_LEN);
            break;
        case CONST_CFG_VENDOR:				//2UL			//4	���̴���
            error = fwrite_ertu_params(EEADDR_CFG_VENDOR,data,4);
            break;
//        case CONST_VENDOR_CHIP:				//3UL			//4	�ն˳��ң�ɽ����
//            error = fwrite_ertu_params(EEADDR_VENDOR_CHIP,data,4);
//            break;
//        case CONST_VENDOR_CHIP2:			//4UL			//2оƬ���ң�ɽ����
//            error = fwrite_ertu_params(EEADDR_VENDOR_CHIP+4,data,2);
//            break;
//        case CONST_SHANGHAI_VENDOR_FLAG:	//5UL			//2	�汾��ʶ���Ϻ���
//            error = fwrite_ertu_params(EEADDR_SHANGHAI_VENDOR_FLAG,data,2);
//            break;
//        case CONST_SOFT_VER:				//6UL			//1+4	����汾��
//            error = fwrite_ertu_params(EEADDR_SOFT_VER,data,4);
//            break;
//        case CONST_SOFT_DATE:				//7UL			//1+3	����汾��������
//            error = fwrite_ertu_params(EEADDR_SOFT_DATE,data,3);
//            break;
//        case CONST_CAPACITY:				//8UL			//1+11	������Ϣ��
//            fwrite_ertu_params(EEADDR_CAPACITY,data,11);
//            break;
//        case CONST_PROTOCOL_VER:			//9UL			//1+4	Э��汾��
//            error = fwrite_ertu_params(EEADDR_PROTOCOL_VER,data,4);
//            break;
//        case CONST_HARDWARE_VER:			//10UL		//1+4	Ӳ���汾��
//            error = fwrite_ertu_params(EEADDR_HARDWARE_VER,data,4);
//            break;
//        case CONST_HARDWARE_DATE:			//11UL		//1+3	Ӳ���汾��������
//            error = fwrite_ertu_params(EEADDR_HARDWARE_DATE,data,3);
//            break;
//        case CONST_ERTU_SWITCH:                        //12UL
//            //���ڸÿ��صģ��洢ʱ����Ϊ0��Ч�����úͶ�ȡʱ����ȡ��������
//            bit_value_opt_inversion(data,ERTU_SWITCH_LEN);
//            error = fwrite_ertu_params(EEADDR_ERTU_SWITCH,data,ERTU_SWITCH_LEN);     //1+4   �ն˵ı��뿪�أ�D0-�ն���ʱ�곭������
//            break;
//        case CONST_DISPLAY_PWD:
//            fwrite_array(FILEID_RUN_PARAM, FLADDR_LOCAL_PASSWORD, data, 6);      //1+6��Һ����ʾ����
//            break;
//        case CONST_RS232_BAUDRATE:
//            fwrite_array(FILEID_RUN_PARAM,FLADDR_RS232_BAUDRATE,data,4);     //1+4,���ڲ�����
//            break;
//        case CONST_LOG_PARAM:
//            fwrite_array(FILEID_RUN_PARAM,FLADDR_LOGEVEL,data,2);        //1+2����־����:��־����+��־�����ʽ
//            gSystemInfo.log_level=data[0];
//            gSystemInfo.log_out_type=data[1];
//            break;
        case CONST_FRAME_MONITOR:
            tmp = bin2_int16u(data);
            if(tmp&0x0004)
            {
                gSystemInfo.gprs_debug_flag = 1;
            }
            else
            {
                gSystemInfo.gprs_debug_flag = 0;
            }
            if(tmp&0x0008)
            {
                gSystemInfo.meter_debug_flag = 1;
            }
            else
            {
                gSystemInfo.meter_debug_flag = 0;
            }
            if(tmp&0x0001)
            {
                gSystemInfo.remote_app_debug_flag = 1;
            }
            else
            {
                gSystemInfo.remote_app_debug_flag = 0;
            }
            if(tmp&0x0010)
            {
                gSystemInfo.debug_info_interface = 1;
            }
            else
            {
                gSystemInfo.debug_info_interface = 0;
            }
            if(tmp&0x0020)
            {
                gSystemInfo.clock_ready = 1;
            }
            else
            {
                gSystemInfo.clock_ready = 0;
            }
            bit_value_opt_inversion(data,2);
            fwrite_ertu_params(EEADDR_FRAME_MONITOR, data, 2);      //2�����ļ��
            break;
//        case CONST_RS232_REPORT:
//            fwrite_array(FILEID_RUN_PARAM, FLADDR_RS232_REPORT, data, 1);       //1+1�������ϱ�
//            gSystemInfo.rs232_report_type = data[0];
//            #ifndef __SOFT_SIMULATOR__
//            if(0xBB == gSystemInfo.rs232_report_type)
//            {
//                channel_gprs_rs232_init();
//            }
//            #endif
//            break;
//        case CONST_GPRS_BAUDRATE:
//            fwrite_array(FILEID_RUN_PARAM, FLADDR_GPRS_BAUDRATE, data, 4);   //4+1,GPRS���ڲ�����
//            #ifndef __SOFT_SIMULATOR__
//            value = data[0]+data[1]*0x100+data[2]*0x10000+data[3]*0x1000000;
//            set_gprs_uart_baudrate(value);
//            #endif
//            break;
//        case CONST_INFRA_BAUDRATE:
//            fwrite_array(FILEID_RUN_PARAM, FLADDR_INFRA_BAUDRATE, data, 4);   //4+1,����ڲ�����
//            break;
//        case CONST_MNG485_BAUDRATE:
//            fwrite_array(FILEID_RUN_PARAM, FLADDR_RS4852_BAUDRATE, data, 4);   //4+1,485II������
//            break;
//        case CONST_GPRS_LOCK:
//            fwrite_array(FILEID_RUN_PARAM, FLADDR_GPRS_LOCK, data, 1);   //1+1,��Ƶ״̬
//            if(0xAA == data[0])
//                set_system_flag(SYS_LOCK_CHANNEL,SYS_FLAG_BASE);
//            else
//                clr_system_flag(SYS_LOCK_CHANNEL,SYS_FLAG_BASE);
//            break;
//        case CONST_PROVINCE_FEATURE:                                          //8,ʡ��������ASCII��
//            fwrite_array(FILEID_RUN_PARAM, FLADDR_PROVINCE_FEATURE,data,8);
//            mem_cpy(gSystemInfo.province_feature,data,8);
//            break;
//        case CONST_REMOTE_TYPE:
//            fwrite_array(FILEID_RUN_PARAM, FLADDR_REMOTE_TYPE,data,1);
//            gSystemInfo.remote_type = data[0];
//            break;
        case CONST_MES:
            fwrite_ertu_params(EEADDR_MES,data,32);
            //record_log_code(LOG_SYS_SET_MES,NULL,0,LOG_ALL);//����ʱ��¼һ����־�������Ժ����
            break;
//        case CONST_ERTU_PLMS_SWITCH:
//	        //���ڸÿ��صģ��洢ʱ����Ϊ0��Ч�����úͶ�ȡʱ����ȡ������
//	        bit_value_opt_inversion(data,ERTU_PLMS_SWITCH_LEN);
//	        error = fwrite_ertu_params(EEADDR_ERTU_PLMS_SWITCH,data,ERTU_PLMS_SWITCH_LEN);   //1+4 �ն˲���������
//	        break;
		case CONST_DZC_METER_SWTICH:   //34 ����G����U�� дUSB.dat�ļ�
		      //	tmp = app_write_usb_data(NULL,0,data,10);
            break;		

        case CONST_CONFIG_ONENET:
            //����ONENET���߲���
            fwrite_ertu_params(EEADDR_CONFIG_ONENET,data,1);
            gSystemInfo.tcp_link = 0;//�л���������²���
            break;
		case CONST_SWITCH_PARAM:
			fwrite_ertu_params(EEADDR_SWITCH_PARAM,data,4);
			break;
			
			
        default:
            error = TRUE;
            break;

    }

    return error;

}




/*+++
  ����: ��ȡ��չ���ն˲���F796
  ������
        BOOLEAN flag  ��ȡʱ�Ƿ��������λ��TRUE ��У��
  ����:

---*/
INT16U read_custom_param(INT16U paramcode,INT8U *data)
{
    INT16U size;
    BOOLEAN error;

    error= FALSE;
    size=0;

    switch(paramcode)
    {
        case CONST_CFG_DEVID:				//1UL			//6	�ն˵�ַ
            error = fread_ertu_params(EEADDR_CFG_DEVID,data,CFG_DEVID_LEN);
            size=CFG_DEVID_LEN+1;
            break;
        case CONST_CFG_VENDOR:				//2UL			//4	���̴���
            error = fread_ertu_params(EEADDR_CFG_VENDOR,data,4);
            if(error || check_is_all_FF(data,4))
            {
                mem_cpy(data,(INT8U *)ertu_cfg_verinfo,4);
                error = FALSE;
            }
            size=5;
            break;
//        case CONST_VENDOR_CHIP:				//3UL			//4	�ն˳��ң�ɽ����
//            error = fread_ertu_params(EEADDR_VENDOR_CHIP,data,4);
//            size=5;
//            break;
//        case CONST_VENDOR_CHIP2:			//4UL			//2оƬ���ң�ɽ����
//            error = fread_ertu_params(EEADDR_VENDOR_CHIP+4,data,2);
//            size=3;
//            break;
//        case CONST_SHANGHAI_VENDOR_FLAG:	//5UL			//2	�汾��ʶ���Ϻ���
//            error = fread_ertu_params(EEADDR_SHANGHAI_VENDOR_FLAG,data,2);
//            size=3;
//            break;
//        case CONST_SOFT_VER:				//6UL			//4+1	����汾��
//            error = fread_ertu_params(EEADDR_SOFT_VER,data,4);
//            if(error || check_is_all_FF(data,4))
//            {
//                mem_cpy(data,(INT8U *)ertu_cfg_verinfo+12,4);
//                error = FALSE;
//            }
//            size=5;
//            break;
//        case CONST_SOFT_DATE:				//7UL			//3+1	����汾��������
//            error = fread_ertu_params(EEADDR_SOFT_DATE,data,3);
//            if(error || check_is_all_FF(data,3))
//            {
//                mem_cpy(data,(INT8U *)ertu_cfg_verinfo+16,3);
//                error = FALSE;
//            }
//            size=4;
//            break;
//        case CONST_CAPACITY:				//8UL			//11+1	������Ϣ��
//            error = fread_ertu_params(EEADDR_CAPACITY,data,11);
//            if(error || check_is_all_FF(data,11))
//            {
//                mem_cpy(data,(INT8U *)ertu_cfg_verinfo+19,11);
//                if(NandFlashCfgInfo.block_count==2048)
//                {
//                    data[7]='2';
//                    data[8]='5';
//                    data[9]='6';
//                }
//                error = FALSE;
//            }
//            size=12;
//            break;
//        case CONST_PROTOCOL_VER:			//9UL			//4+1	Э��汾��
//            error = fread_ertu_params(EEADDR_PROTOCOL_VER,data,4);
//            if(error || check_is_all_FF(data,4))
//            {
//                mem_cpy(data,(INT8U *)ertu_cfg_verinfo+30,4);
//                error = FALSE;
//            }
//            size=5;
//            break;
//        case CONST_HARDWARE_VER:			//10UL		//4+1	Ӳ���汾��
//            error = fread_ertu_params(EEADDR_HARDWARE_VER,data,4);
//            if(error || check_is_all_FF(data,4))
//            {
//                mem_cpy(data,(INT8U *)ertu_cfg_verinfo+34,4);
//                if(mem_is_same(data,"3.01",4))
//                {
//                    //�汾��Ϣ��3.01�Ļ���˵����ͨ�ð汾���������ʵ��Ӳ���汾������
//                    if(gSystemInfo.hardware_version == 0x02)   
//                    {
//                        mem_cpy(data,"3.02",4);
//                    }
//                }
//                error = FALSE;
//            }
//            size=5;
//            break;
//        case CONST_HARDWARE_DATE:			//11UL		//3+1	Ӳ���汾��������
//            error = fread_ertu_params(EEADDR_HARDWARE_DATE,data,3);
//            if(error || check_is_all_FF(data,3))
//            {
//                mem_cpy(data,(INT8U *)ertu_cfg_verinfo+38,3);
//                if(mem_is_same(data,"\x02\x01\x15",3))
//                {
//                    //�汾��Ϣ��3.01�Ļ���˵����ͨ�ð汾���������ʵ��Ӳ���汾������
//                    if(gSystemInfo.hardware_version == 0x02)
//                    {
//                        mem_cpy(data,"\x16\x02\x17",3);
//                    }
//                }
//                error = FALSE;
//            }
//            size=4;
//            break;
//        case CONST_ERTU_SWITCH:
//            error = fread_ertu_params(EEADDR_ERTU_SWITCH,data,ERTU_SWITCH_LEN);     //4+1   �ն˵ı��뿪�أ�D0-�ն���ʱ�곭������
//            //���ڸÿ��صģ��洢ʱ����Ϊ0��Ч�����úͶ�ȡʱ����ȡ��������
//            if(error)
//            {
//                mem_set(data,ERTU_SWITCH_LEN,0xFF);
//                error = FALSE;
//            }
//            bit_value_opt_inversion(data,ERTU_SWITCH_LEN);
//            size=ERTU_SWITCH_LEN + 1;
//            break;
//        case CONST_DJB_VER:                                                //16+1  �����İ汾��Ϣ
//            mem_cpy(data,gSystemInfo.djb_ver_info,16);
//            error = FALSE;
//            size = 17;
//            break;
//        case CONST_LOG_PARAM:                                           //2+1  ��־����:��־����+��־�����ʽ
//            fread_array(FILEID_RUN_PARAM,FLADDR_LOGEVEL,data,2);
//            error = FALSE;
//            size = 2+1;
//            break;
//        case CONST_DISPLAY_PWD:                                         //6+1  Һ����ʾ����
//            fread_array(FILEID_RUN_PARAM,FLADDR_LOCAL_PASSWORD,data,6);
//            error = FALSE;
//            size = 6+1;
//            break;
//        case CONST_RS232_BAUDRATE:                                      //4+1  ���ڲ�����
//            fread_array(FILEID_RUN_PARAM,FLADDR_RS232_BAUDRATE,data,4);
//            error = FALSE;
//            size = 4+1;
//            break;
        case CONST_FRAME_MONITOR:
            fread_ertu_params(EEADDR_FRAME_MONITOR, data, 2);      //1+2�����ļ��
            bit_value_opt_inversion(data,2);
            error = FALSE;
            size = 2+1;
            break;
//        case CONST_RS232_REPORT:
//            fread_array(FILEID_RUN_PARAM, FLADDR_RS232_REPORT, data, 1);       //1+1�������ϱ�
//            error = FALSE;
//            size = 1+1;
//            break;
//        case CONST_GPRS_BAUDRATE:
//            fread_array(FILEID_RUN_PARAM, FLADDR_GPRS_BAUDRATE, data, 4);   //4+1,GPRS���ڲ�����
//            error = FALSE;
//            size = 4+1;
//            break;
//        case CONST_INFRA_BAUDRATE:
//            fread_array(FILEID_RUN_PARAM, FLADDR_INFRA_BAUDRATE, data, 4);   //4+1,����ڲ�����
//            error = FALSE;
//            size = 4+1;
//            break;
//        case CONST_MNG485_BAUDRATE:   //��Ӧ485II������
//            fread_array(FILEID_RUN_PARAM, FLADDR_RS4852_BAUDRATE, data, 4);   //4+1,485II������
//            error = FALSE;
//            size = 4+1;
//            break;
//        case CONST_GPRS_LOCK:
//            fread_array(FILEID_RUN_PARAM, FLADDR_GPRS_LOCK, data, 1);   //1+1,��Ƶ״̬
//            error = FALSE;
//            size = 1+1;
//            break;
//        case CONST_PROVINCE_FEATURE:                                 //8,ʡ������
//            fread_array(FILEID_RUN_PARAM, FLADDR_PROVINCE_FEATURE, data, 8);
//            error = FALSE;
//            size = 8+1;
//            break;
//        case CONST_REMOTE_TYPE:                           //1,Զ��ͨ������
//            fread_array(FILEID_RUN_PARAM, FLADDR_REMOTE_TYPE, data, 1);
//            error = FALSE;
//            size = 1+1;
//            break;
        case CONST_MES:
            fread_ertu_params(EEADDR_MES, data, 32);
            error = FALSE;
            size = 32+1;
            break;
//        case CONST_ERTU_PLMS_SWITCH:
//            error = fread_ertu_params(EEADDR_ERTU_PLMS_SWITCH,data,ERTU_PLMS_SWITCH_LEN);     //4+1   �ն˲��������֣�D0-GPRS�����Ƿ��޸�
//            //���ڸÿ��صģ��洢ʱ����Ϊ0��Ч�����úͶ�ȡʱ����ȡ��������
//            if(error)
//            {
//                mem_set(data,ERTU_PLMS_SWITCH_LEN,0xFF);
//                error = FALSE;
//            }
//            bit_value_opt_inversion(data,ERTU_PLMS_SWITCH_LEN);
//            size=ERTU_PLMS_SWITCH_LEN + 1;
//            break;
//        case CONST_GPRS_AUTH_TYPE:
//            fread_array(FILEID_RUN_PARAM, FLADDR_GPRS_AUTH_TYPE, data, 1);   //1+1,��Ȩ���ͣ�55-pap��AA-chap������Ĭ��
//            error = FALSE;
//            size = 1+1;
//            break;

        case CONST_DZC_METER_SWTICH:	//34UL	����G����U�� дU��USB.dat�ļ����� ����������֤ 
		       //	size = app_read_usb_data(NULL,0,data,10);
			if(size)
			{
				size = 10+1;
			}
			else
			{
				size=0;
			}
            break;		
        case CONST_CONFIG_ONENET:
            //����ONENET���߲���
            fread_ertu_params(EEADDR_CONFIG_ONENET,data,1);
            size = 1+1;
            break;
		case CONST_SWITCH_PARAM:
			fread_ertu_params(EEADDR_SWITCH_PARAM,data,4);
			size = 4+1;
			break;			
			
			
        default:
            size = 1;
            error = TRUE;
            break;
    }
//    if(flag)
//    {
//        crc = crc16_param(data,(size-1));
//        if(error)
//        {
//            mem_set(data,size,0xFF);
//            crc=0xAA;
//            size = 0;
//        }
//        else
//        {
//            mem_cpy_right(data+1,data,(size-1));
//        }
//        data[0]=crc;
//    }
//    else
//    {
//        size -= 1;
//        if(error)
//        {
//            mem_set(data,size,0xFF);
//            size = 0;
//        }
//    }
    return size;

}



void query_ertu_custom_param(INT8U *buffer,INT16U *itemDataLen,INT8U *param)
{
    INT16U code;
    INT16U size;
    INT8U *data;
    INT8U count;
    INT8U idx;
    //BOOLEAN check_crc;
    QUERY_CTRL *query_ctrl;
    
    query_ctrl = (QUERY_CTRL *)param;
    
    count=*(query_ctrl->req);
    query_ctrl->req++;
    query_ctrl->req_bytes += count * 2 + 1;

    buffer[0]=count;
    buffer++;

    *itemDataLen=0;
    for(idx=0;idx<count;idx++)
    {
        code=bin2_int16u(query_ctrl->req);
        mem_cpy(buffer,query_ctrl->req,2);
        query_ctrl->req+=2;
        buffer+=2;

        data=buffer+2;
        //check_crc = TRUE;
        size=read_custom_param(code,data+1);
        //����ն˲�֧�ָò������򷵻صĳ���Ϊ0������վ���س���Ϊ1,������У����
        if(size==0)
        {
            size =1;
        }
        buffer[0]=size-1;
        buffer[1]=(size-1)>>8;
        buffer+=2;
        buffer+=size;
        *itemDataLen+=(size+4);
    }
    if(*itemDataLen>0)
    {
        (*itemDataLen)++;
    }

}

INT16U app_get_F249_data(INT8U* resp)
{
	INT8U pos=0,idx=0;
	
	mem_set(resp,60,0x00);
	fread_ertu_params(EEADDR_CFG_DEVID,resp+pos,4);
	pos += 4;
	resp[pos++] = 6;  //6ר���ն�3��(FKXA2X)��7ר���ն�3��(FKXA4X)��8ר���ն�3��(FCXA2X)
	resp[pos++] = 2;  //d1 d0 ��ֵ1~3���α�ʾ�������ߡ��������ߡ�������ߣ�0Ϊ����
	resp[pos++] = 0;
	resp[pos++] = 0x22;
	resp[pos++] = 1;//1 GPRS��2 CDMA��3��̫����4, 230 M,5 EPON, ����������
	mem_cpy(resp+pos,gSystemInfo.myccid_info,20);
	for(idx=0;idx<20;idx++)
	{
		if((resp[pos+idx]>='0')&&(resp[pos+idx]<='9')
		|| (resp[pos+idx]>='A')&&(resp[pos+idx]<='Z')
		|| (resp[pos+idx]>='a')&&(resp[pos+idx]<='z'))
		{
		}
		else
		resp[pos+idx] = '0';
	}
	pos += 20;
	resp[pos++] = gSystemInfo.csq;
	resp[pos++] = 4;
	mem_cpy(resp+pos,"XXXXXXXXXXXX",12);
	pos += 12;
	resp[pos++] = 0;
	resp[pos++] = 0;
	resp[pos++] = 0;
	resp[pos++] = 0;
	resp[pos++] = 0;
	resp[pos++] = 0;
	resp[pos++] = 0;
	resp[pos++] = 1;
	return pos;

}

