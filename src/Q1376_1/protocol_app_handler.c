///////////////////////////////////////////////////////////////////////////////
//
//   Q/GDW  1376.1 ���Ĵ�����  
//
///////////////////////////////////////////////////////////////////////////////


#include "../main_include.h"
#include "protocol_app_handler.h"
#include "app_3761.h"
#include "app_var_3761.h"
#include "ErtuParam.h"



INT8U  ACD;


   void  app_init_response(objRequest *pRequest,objResponse *pResp);
   void  process_frame_read(objRequest *pRequest,objResponse *pResp);
   void  process_frame_reset(objRequest *pRequest,objResponse *pResp);
   void  process_frame_readRelay(objRequest  *pRequest,objResponse *pResp);
   void  process_frame_readhis(objRequest *pRequest,objResponse *pResp);
   void  process_frame_set(objRequest *pRequest,objResponse *pResp);
   void  process_frame_query(objRequest *pRequest,objResponse *pResp);
   void  process_read_config_info(objRequest *pRequest,objResponse *pResp);
   void  process_vendor_debug(objRequest *pRequest,objResponse *pResp);
   void  process_test(objRequest *pRequest,objResponse *pResp);
   void  process_frame_ask_slave(objRequest *pRequest,objResponse *pResp);
   void  process_frame_ftp(objRequest *pRequest,objResponse *pResp);
   BOOLEAN process_remote_update_nm(objRequest *pRequest,objResponse *pResp);
   void  process_read_event(objRequest *pRequest,objResponse *pResp);
   void  process_frame_ctrl(objRequest *pRequest,objResponse *pResp);
   void  process_frame_ack(objRequest *pRequest,objResponse *pResp);
   void  process_frame_readTask(objRequest *pReqeust,objResponse *pResp);
   void  process_product_test(objRequest *pRequest,objResponse *pResp);
//void check_ertu_cascade(objRequest *pRequest,objResponse *pResp);
BOOLEAN check_ertu_address(objRequest *pRequest,objResponse *pResp);
BOOLEAN auth_verify_mac(INT8U *frame,objRequest  *pRequest,objResponse *pResp);
BOOLEAN check_is_cy_timing_frame(INT8U* frame,INT16U len);



/*+++
  ���ܣ�  �ж��Ƿ��Ѿ����յ��˱���ͷ��
  ��ע��
          1)Ҫ��frame�в����������ֽڣ������ֽڱ�����ϱ�׼��
---*/
void app_protocol_handler(objRequest* pRequest,objResponse *pResp)
{
void app_prepare_request(objRequest *pRequest);
char info[20];
//    if(get_system_flag(SYS_KEY_WAKEUP,SYS_FLAG_BASE))//��������ʱ������б��ģ��򹩵�ʱ���ӳ�
//    {
//        gSystemCtrl.battery_start_time = system_get_tick10ms();
//    }
//    if((pRequest->frame[POS_AFN]!=AFN_FTP) && (pRequest->frame[POS_AFN]!=AFN_DEBUG))
//    {
//        if(is_debug_enabled()  && is_monitor_frame(MONTIOR_3761))
//        {
//            record_log_frame(pRequest->frame,pRequest->frame_len,LOG_DEBUG);
//        }
//    }

//Ԥ����request����
    app_prepare_request(pRequest);
    //��ʼ��response����
    app_init_response(pRequest,pResp);

    //������б��ĵ��ն˵�ַ�Ƿ���Ч
#ifndef __ONENET__
    if(FALSE == check_ertu_address(pRequest,pResp))
    {

       return ;
    }
#endif    



    if(pRequest->userDataLen<4)
    {
        return ;//����û�������򣬲��Ϸ����ӵ�
    }
    switch(pRequest->frame[POS_AFN])
    {
    case  AFN_ACK:        //ȷ��/���ϱ���
             process_frame_ack(pRequest,pResp);
             return;
    case  AFN_RESET:      //��λ����
             process_frame_reset(pRequest,pResp);
             break;
    case  AFN_RELAY:      //����ת��
             process_frame_readRelay(pRequest,pResp);
             break;

    case  AFN_READCUR:    //����ǰ����
             process_frame_read(pRequest,pResp);
             break;

    case  AFN_READHIS:    //����ʷ����
             process_frame_readhis(pRequest,pResp);
             break;
    case  AFN_SET:        //���ò���
             process_frame_set(pRequest,pResp);
             break;

    case  AFN_READEVENT:  //����3������
//             process_read_event(pRequest,pResp);
             break;
    
    case  AFN_QUERY:      //��ѯ����
             process_frame_query(pRequest,pResp);
             break;
//    case  AFN_READTASK:  //������������
//             process_frame_readTask(pRequest,pResp);
//             break;
//    case  AFN_CTRL:     //��������
//             process_frame_ctrl(pRequest,pResp);
//             break;
//    #ifdef __ENABLE_ESAM2__
//    case  AFN_AUTH:      //��ԿЭ��
//            #ifndef __SOFT_SIMULATOR__
//             process_frame_auth(pRequest,pResp);
//            #endif
//             break;
//    #endif
    case  AFN_READCFG:  //�����ն�����
             process_read_config_info(pRequest,pResp);
             break;   
        #ifndef __SOFT_SIMULATOR__
    case  AFN_FTP:   //�����ն˳���
             process_frame_ftp(pRequest,pResp);
             break;
        #endif


    //TODO:��������ú���
//    case AFN_ASK_SLAVE: //���󱻼����ն������ϱ�
//    case AFN_ASK_SLAVE_NEXT:
//             process_frame_ask_slave(pRequest,pResp);
//             break;
    case AFN_DEBUG: //���ҵ���
             process_vendor_debug(pRequest,pResp);
             break;
//    case AFN_TEST://��·�ӿڼ��
//            process_test(pRequest,pResp);
//            break;
//    case AFN_PRODUCT_TEST:
//            process_product_test(pRequest,pResp);
//            break;
    default:
             app_send_NAK(pRequest,pResp);
             break;
    }


    return ;
}
/*+++
  ���ܣ�Ԥ�����������ṹ��
  ������
  ���أ�

  ������


---*/
void app_prepare_request(objRequest *pRequest)
{
    //��ȡ�û���Ч�������ĵ�ַ�����ݳ���
    pRequest->pos = POS_DATA;
    if(pRequest->frame_len>16)
    {
        pRequest->userDataLen = pRequest->frame_len - 16; //���ǵ�Ӧ�ò㴦��ʱ�ķ����ԣ�userDataLen������Ӧ�ò��û�������
    }
    else
    {
        pRequest->userDataLen = 0;
    }
    #ifdef __RTUADDR_IS_6BYTE__
    pRequest->userDataLen -= 2;
    #endif
   //��ȡ����֡�������������жϱ����Ƿ���ʱ���ǩ����Ϣ
    if(pRequest->frame[POS_SEQ]>>7 )
    {

       //����ʱ��
       pRequest->param = & pRequest->frame[pRequest->frame_len - 8];

       //��Ч���û����ݳ��ȼ���ʱ���ǩ
       if(pRequest->userDataLen > 6)
       {
           pRequest->userDataLen -= 6;
       }

    }
    else
    {
        pRequest->param = NULL;
    }
    pRequest->rseq =  pRequest->frame[POS_SEQ] & 0x0F;
}
/*+++
  ���ܣ���ʼ����Ӧ�ṹ
  ������
  ���أ�
        INT8U *frameData  ������ͷ
  ������
       1��������Ӧͷ
       2����ȡ�û����ݳ���
       3��������Ӧ����������

---*/
void app_init_response(objRequest *pRequest,objResponse *pResp)
{
    //INT8U get_cur_seq(void);
    void  app_set_max_response_length(objRequest *pRequest,objResponse *pResp);

    //��ȡ����֡��������ǰ���ֽ�ΪӦ��֡ͷ
    mem_cpy(pResp->frame, pRequest->frame,POS_DATA);


    pResp->channel = pRequest->channel;
//    pResp->pChannel= pRequest->pChannel;
//    pResp->CallBack = pRequest->CallBack;
//    pResp->protocol_type = pRequest->protocol_type;
    //������Ӧ֡��������λ��
    app_set_max_response_length(pRequest,pResp);
    //��Ӧ���ĵĴ���λ��
    pResp->pos = POS_DATA;

    //��ȡ����֡�������������жϱ����Ƿ���ʱ���ǩ����Ϣ
    if(pRequest->frame[POS_SEQ]>>7 )
    {
       //��Ч�û����ݳ��ȼ�ȥʱ���ǩ����
       pResp->max_reply_pos -= 6;
    }

    //����㲥��־
    pResp->work_flag[WORK_FLAG_CMD_FLAG] &= ~MSA_BROADCAST;

    //��ȡ��Ӧ֡���,��Ϊ��Ӧ֡�ĵ�һ֡��� 
    pResp->work_flag[WORK_FLAG_SEQ_RSEQ] &= 0xF0;
    pResp->work_flag[WORK_FLAG_SEQ_RSEQ] += (pRequest->rseq++ & 0x0F);
    pResp->work_flag[WORK_FLAG_SEQ_RSEQ] &= ~MASK_CON;
    if(pResp->channel == CHANNEL_FLASH)
    {
        gActiveSendAck.check_seq = pResp->work_flag[WORK_FLAG_SEQ_RSEQ]&0x0F;
        gActiveSendAck.has_acked = FALSE;
        if(gActiveSendAck.need_check_ack)
        {
            pResp->work_flag[WORK_FLAG_SEQ_RSEQ] |= MASK_CON;
    }
    }

    //Ĭ�ϵ�֡
    pResp->work_flag[WORK_FLAG_IS_FIRST_FRAME] = TRUE;
    //���ÿ�����C�Ĵ��䷽��
    pResp->frame[POS_CTRL] |= CTRLFUNC_SET_DIR;
    pResp->frame[POS_CTRL] &= 0xF0;
    //���������C��ACDλ
    pResp->frame[POS_CTRL] &= CTRLFUNC_CLR_ACD;
    //���������ϱ���������������PRM
    if(pResp->channel != CHANNEL_FLASH)
    {
    pResp->frame[POS_CTRL] &= CTRLFUNC_CLR_PRM;
    pResp->frame[POS_SEQ] &= ~0x10;   //ȥ��CON
    }

    /*
    #ifndef __PROVICE_SHANGHAI__
    if(ACD  &&  (system_flag & SYS_FRAME_ACD_FLAG) )
    {
       pResp->frame[POS_CTRL] |=  CTRLFUNC_SET_ACD;
       //��Ч�û����ݳ��ȼ�ȥ�¼�������
       PResp->max_reply_pos -= 2;
    }
    #endif
    */



    //����AFN������Ӧ֡�����룬��Ҫ������վ�ʹӶ�վ����֡��Ӧ�Ŀ�������Ӧʱ����
//    if(pResp->frame[POS_CTRL] & CTRLFUNC_SET_PRM)
//    {
//        
//    }
//    else
    {
          if((pRequest->frame[POS_CTRL] & 0x0F) == PRM_CTRLFUNC_TEST)
          {
               pResp->frame[POS_CTRL] |= CTRLFUNC_TEST;
          }
          else if(((pRequest->frame[POS_CTRL] & 0x0F) == PRM_CTRLFUNC_CURDATA)|| ((pRequest->frame[POS_CTRL] & 0x0F) == PRM_CTRLFUNC_HISDATA))
          {
               pResp->frame[POS_CTRL] |= CTRLFUNC_USERDATA;
          }
          else if((pRequest->frame[POS_CTRL] & 0x0F) ==PRM_CTRLFUNC_RESET)
          {
               pResp->frame[POS_CTRL] |= CTRLFUNC_CONFIRM;
          }
          else
          {
               if(pResp->channel != CHANNEL_FLASH)
               {
                    pResp->frame[POS_CTRL] |= CTRLFUNC_USERDATA;
               }
               else
               {
                    pResp->frame[POS_CTRL] |= PRM_CTRLFUNC_USERDATA;
               }
          }

    }

}

/*+++
  ���ܣ� ������Ӧ�����������󳤶�
  ��ע�� �ýӿڿ��Ա�����.
---*/

void  app_set_max_response_length(objRequest *pRequest,objResponse *pResp)
{
    pResp->max_reply_pos = SIZE_OF_APP_PROTOCOL_FRAME - 2;
//    if(pResp->channel == CHANNEL_INFRA  )
//    {
//         pResp->max_reply_pos = 300;//����Ǻ���ڣ����屨����󳤶�Ϊ300�ֽ�
//    }
//    else
    {
//        if(!get_system_flag(SYS_ESAM,SYS_FLAG_BASE))
        {
            pResp->max_reply_pos = 916;//Զ�̿ڣ�Ӧ�𻺳������֧��916�ֽ�
        }
//        else if(pRequest->frame[POS_AFN]==AFN_AUTH)//�ڿ���ESAMʱֻ�������֤����ʹ��ȫ��������  ����G���Ƿ���Ҫ�����֤  __GMETER__
//        {
//    	    pResp->max_reply_pos = MAX_SIZE_PLMSDTP - 2;
//        }
    }

    #if defined(STACK_USE_SMS)
    if(pResp->channel==CHANNEL_SMS)
    {
	if(REMOTE_SMS.smsframetype==1)
	pResp->max_reply_pos = 138;
    }
    #endif
	//�㽭�������ϱ�ʱ��������粻ͨ�����ɶ��ű���
   	#ifdef  __SMS_REPORT__
        if(1!=g_eth_conn_flag && !REMOTE_GPRS.tcp_ok && CHANNEL_FLASH==resp->channel)
		{
	    		resp->max_reply_pos = 138;
		}
	#endif



	//�����UDP�����ư��Ĵ�СΪ540�ֽڣ�2011.3.31
     /*
     if((pResp->channel==CHANNEL_REMOTE) && (REMOTE_SMS.gprs_mode==1) )
	{
	    	pResp->max_reply_pos = 540;
	}

 	//��ʼ����Ӧͨ��Ϊ1��2011.5.17 by zyg
	if(1==REMOTE_SMS.work_mode)//�ͻ���ģʽ
    	resp->channel_idx=1;
	else if(2==REMOTE_SMS.work_mode)//������ģʽ
    	resp->channel_idx=2;
    //���ģʽ������ʼ�����ɷ���������

    //2011.3.16 by zyg

            */
#ifdef __USE_NB_COAP__
    if(pResp->channel == PROTOCOL_CHANNEL_REMOTE  )
    {
        pResp->max_reply_pos = SIZE_OF_APP_PROTOCOL_FRAME_COAP - 2;
    }
#endif
}



/*+++
   ���ܣ�����У��λ�����ÿ�����
   ������INT8U *frame      [IN]   ֡
        INT16U *framelen   [OUT]   ֡�ܳ���
---*/
void app_encodeFrame(objResponse *pResp)
{
    INT16U i;
    INT16U frameLen;
    INT8U cs;


    //ȷ��֡֡��ʼ������ȷ
    pResp->frame[0]=0x68;
    pResp->frame[5]=0x68;

    //��ȡ�û�����������
    frameLen = bin2_int16u(pResp->frame+1);
    frameLen >>=2;

    //����У��λ
    cs=0;
    frameLen += 6;   //���Ϲ̶����ȵı���ͷ
    for(i=6;i<frameLen;i++)  cs += pResp->frame[i];
    pResp->frame[frameLen++]=cs;

    //���ý����ַ�
    pResp->frame[frameLen++]=0x16;

    pResp->frame_len = frameLen;

}

/*+++
 ���ܣ� ������Ӧ����֡
 ������ INT8U *Reply ��Ӧ֡,û�и������ݳ��ȣ�������
       int pos              ��Ӧ֡������õ�����λ��

 ��ע���������б��ĵ�ʱ���ǩ�Լ���ǰACD��־���ܻ����Ӹ�����EC,ʱ���ǩ��
---*/
void app_setFrameLenThenSendFrame(objRequest  *pRequest,objResponse *pResp)
{
   //INT8U get_cur_seq(void);

   BOOLEAN allow_acd = TRUE;

      //������Ϻ������������ʱ������ACDλ���������ܴ�
   #ifndef __PROVICE_SHANGHAI__
   if(AFN_TEST == pResp->frame[POS_AFN])
   {
            allow_acd = FALSE;
       }
   #endif

   if(allow_acd==FALSE)
   {
      //2010-6-21  ĳЩ��վ����ʶ���������ĵ�ACD����������ȥ������(����Эͬ����վ)
      pResp->frame[POS_CTRL] &= ~CTRLFUNC_SET_ACD;
      pResp->frame[POS_SEQ]  &= ~MASK_TpV;
   }
   else
   {
     //1. ����ACDֵ����Ƿ���Ҫ����EC
//      #ifdef __PROVICE_SHANGHAI__
//      if(pResp->frame[POS_CTRL] & CTRLFUNC_SET_ACD)
//      #else
//      //if(ACD && (system_flag & SYS_FRAME_ACD_FLAG) )
//      if(g_event.acd)
//      #endif
//      {
//        pResp->frame[POS_CTRL] |= CTRLFUNC_SET_ACD;
//        //EC
//        //��Ҫ�¼�������EC1
//        pResp->frame[pResp->pos++] = g_event.ec1;
//
//        //һ���¼�������EC2
//        pResp->frame[pResp->pos++] = g_event.ec2;
//      }

     //2. ����CMD_TPV
     if(pRequest->param)
     {
        pResp->work_flag[WORK_FLAG_SEQ_RSEQ] |= 0x80;  //set bit7=1
        mem_cpy(pResp->frame + pResp->pos,pRequest->param,6);
        pResp->pos+=6;
     }
   }


   //3. ����Ӧ��֡�е������򳤶�

   //3.1 ��ȥ�̶�����֡����ͷ
   pResp->pos -= 6;

   //3.2 ���ݳ�������2λ
   pResp->pos<<=2;

   //3.3  ��Լ���ͣ���������Ĺ�Լ���ͻش�
//   #ifdef __QGXD_FK2012__
//   pRequest->protocol_type =QGXD_FK2012;
//   #elif defined  __QNMDW_FK2012__
//   pRequest->protocol_type = QNMDW_FK2012;
//   #elif defined __QGDW_FK2005__
//   pRequest->protocol_type = QGDW_2005;
//   #else
//   pRequest->protocol_type = QGDW_376_1;
//   #endif

   //pResp->pos += pRequest->protocol_type;
   pResp->pos += QGDW_376_1;

   //3.4 ���ó���
   pResp->frame[1] =  pResp->pos;
   pResp->frame[2] =  pResp->pos >> 8;
   pResp->frame[3] =  pResp->frame[1];
   pResp->frame[4] =  pResp->frame[2];

   //4 ������Ӧ֡���
   pResp->frame[POS_SEQ] = pResp->work_flag[WORK_FLAG_SEQ_RSEQ];

   //5 ��Ӧ֡��ż�1,����֡ʱ����ʹ��
   pResp->work_flag[WORK_FLAG_SEQ_RSEQ] = (pResp->work_flag[WORK_FLAG_SEQ_RSEQ] +1 ) & 0x0F;
   //���������ϱ��ķ�֡���Ҳ��Ƿ�֡�����һ֡(FIN=0)������ȫ��֡��ŵĴ���
   if((pResp->channel == CHANNEL_FLASH) && ((pResp->frame[POS_SEQ] & MASK_FIN) == 0x00))
   {
       //get_cur_seq();
   }
   #ifdef __PROVICE_TIANJIN__
   if((pResp->frame[POS_AFN] == AFN_QUERY) && (0 == bin2_int16u(pResp->frame + POS_DATA))
             && (DT_F89 == bin2_int16u(pResp->frame + POS_DATA + 2))&& (pResp->channel == CHANNEL_RS232))
    {
    }
    else
    #endif
    {
   //�����ն��߼���ַ������������+�ն˵�ַ
        mem_cpy(pResp->frame+POS_RT,gSystemInfo.ertu_devid,CFG_DEVID_LEN);
    }

   // ����У��λ
   app_encodeFrame(pResp);

   if(pResp->channel==CHANNEL_NONE)
   {

     return;
   }
    if((pRequest->frame[POS_AFN]!=AFN_FTP) && (pRequest->frame[POS_AFN]!=AFN_DEBUG))
    {
//        if(is_debug_enabled() && is_monitor_frame(MONTIOR_3761))
//        {
//            record_log_frame(pResp->frame,pResp->frame_len,LOG_DEBUG);
//        }
    }

   //6 ������Ӧ֡
   app_send_ReplyFrame(pResp);

}


/*+++
  ������  ������������������־
          Ȼ����
  ������
        SEQ_RSEQ
  ���أ�
         TRUE  ��������
         FALSE ֹͣ����
  
---*/
BOOLEAN app_send_response_frame(objRequest  *pRequest,objResponse *pResp,INT8U last_frame)
{
    //������֡������֡��־
    pResp->work_flag[WORK_FLAG_SEQ_RSEQ] &= 0x9F;  //clear bit5,6
    
    //���ý���֡��־λ 
    if(last_frame)  pResp->work_flag[WORK_FLAG_SEQ_RSEQ] |=  MASK_FIN;
    
    if( pResp->work_flag[WORK_FLAG_IS_FIRST_FRAME] )
    {
        pResp->work_flag[WORK_FLAG_SEQ_RSEQ] |=  MASK_FIR;
        pResp->work_flag[WORK_FLAG_IS_FIRST_FRAME] = 0;
    }

     //������Ӧ����
    app_setFrameLenThenSendFrame(pRequest,pResp);
    
    return TRUE;
}

/*+++
  ���ܣ� ������Ӧ����
  ��ע��
         1����Ӧ�����ڷ�����ɺ���Ҫ����release�ͷš�
         2�����ͨ��������ģ���ֱ�ӵ���release.


---*/
void app_active_send_complete_callback(void)
{
    gActiveSendAck.has_send = TRUE;
    if(gActiveSendAck.need_check_ack == FALSE)
    {
        gActiveSendAck.has_acked = TRUE;
    }
    else
    {
        gActiveSendAck.last_send_timer = system_get_tick10ms();//�������һ�·���ʱ��ɣ�Ҳ���Բ����£���������
    }
}



/*++++
  ���ܣ� ����ȫ��ȷ��֡
  ������
---*/
void app_send_ACK(objRequest  *pRequest,objResponse *pResp)
{
    // clear prm =0
    // ���ϣ������ٻ������ݡ�
    pResp->frame[POS_CTRL] &= 0xF0;
    if((pRequest->frame[POS_CTRL] & 0x0F) ==PRM_CTRLFUNC_RESET)
    {
        pResp->frame[POS_CTRL] = CTRLFUNC_SET_DIR | CTRLFUNC_CONFIRM;
    }
    else
    {
        pResp->frame[POS_CTRL] = CTRLFUNC_SET_DIR | CTRLFUNC_USERDATA;
    }
    pResp->frame[POS_AFN] = AFN_ACK;
    pResp->work_flag[WORK_FLAG_SEQ_RSEQ]  |=  MASK_FIR|MASK_FIN;

    // PN = P0
    pResp->frame[POS_DATA] = 0x00;
    pResp->frame[POS_DATA+1] = 0x00;

    // FN = DT_F1  ȫ��ȷ��
    pResp->frame[POS_DATA+2] = DT_F1 & 0xFF;
    pResp->frame[POS_DATA+3] = DT_F1 >> 8;

    pResp->pos = POS_DATA+4;

    app_setFrameLenThenSendFrame(pRequest,pResp);
}

/*++++
  ���ܣ� ����ȫ������֡
  ������ GUN.SET
---*/
void  app_send_NAK(objRequest  *pRequest,objResponse *pResp)
{

    if(pRequest->channel == CHANNEL_FLASH)
    {
        //pResp->release(pResp);
        return;
    }
    // clear prm =0
    // ���ϣ������ٻ������ݡ�
    //pResp->frame[POS_CTRL] &= 0xF0;
    pResp->frame[POS_CTRL] = CTRLFUNC_SET_DIR | CTRLFUNC_NODATA; // ȫ�����Ϲ�����Ϊ9  �����ٻ�������

    pResp->frame[POS_AFN] = AFN_ACK;
    pResp->work_flag[WORK_FLAG_SEQ_RSEQ]  |=  MASK_FIR|MASK_FIN;

    // PN = P0
    pResp->frame[POS_DATA] = 0x00;
    pResp->frame[POS_DATA+1] = 0x00;

    // FN = DT_F2  ȫ������
    pResp->frame[POS_DATA+2] = DT_F2 & 0xFF;
    pResp->frame[POS_DATA+3] = DT_F2 >> 8;

    pResp->pos = POS_DATA+4;

    app_setFrameLenThenSendFrame(pRequest,pResp);
}


/*+++
   ����:�����ϵ���Ϣ���ʾת������ʾ������
    GUN.SET.fn  --> GUN.SET.fngroup
---*/
INT16U parse_fn_to_fngroup(INT16U fn,INT16U fngroup[8])
{
   INT8U ik,mask;
   INT16U fngroupcount;

   if(fn==DT_F0)
   {
       //fngroupcount=1;
       fngroup[0]=DT_F0;
       return 1; 
   }

   fngroupcount=0;
   for(ik=0;ik<8;ik++)
   {
      fngroup[ik]=0;
      mask = 1<<ik;
      if(fn & mask)
      {
         fngroup[fngroupcount++] = fn & (0xFF00|mask);
      } 
   }
   
   return fngroupcount;
}


/*+++
   ����:�����ϵ���Ϣ���ʾת������ʾ������
   ������INT16U pn  ԭ���PN
         INT16U pngroup[8]  �ֽ���PN
   ���أ��ֽ�����
---*/
INT16U parse_pn_to_pngroup(INT16U pn,INT16U pngroup[8])
{
   INT8U ik,mask;
   INT16U pngroupcount;

   if(pn==DA_P0)
   {
       pngroup[0] = DA_P0;
       return 1;
   }
   pngroupcount=0;
   for(ik=0;ik<8;ik++)
   {
      pngroup[ik]=0;
      mask = 1<<ik;
      if(pn & mask)
      {
         pngroup[pngroupcount++] = pn & (0xFF00|mask);
      } 
   }
   return pngroupcount;
}



//��fn  ת��ΪGUN.SET.fn_idx
//  ת���� DT_F1 == 1
//          DT_F2 == 2
//          DT_Fn == n
INT16U trans_set_fn_2_fnidx(INT16U fn)
{
    INT16U fn_idx;
    INT8U fn_low;


    fn_idx = fn>>8;
    fn_idx <<=3; //����8
    fn_low = fn & 0xFF;

    do
    {
      fn_idx ++;
      fn_low >>=1;
   }while(fn_low > 0);

   return fn_idx;
}



/*+++
 ���ܣ� ��PN  ת��Ϊpn_idx
 ������
        INT16U pn  ��Ϣ�㣬���ܱ�ʾһ����Ϣ�㣬ֻ���ǵ�һ����Ϣ�㡣
 ���أ�
        INT16U  ��Ϣ����ţ���ʾ������Ż��ܼ����
 ����:
         DA_P0==0
         DA_P1==1
         DA_Pn==n
---*/
INT16U  trans_set_pn_2_pnidx(INT16U pn)
{
    INT16U pn_idx;

    ClrTaskWdt();

    pn_idx = pn>>8;
    if(pn_idx == 0) return 0;


    //��1��8
    pn_idx = (pn_idx-1)<<3;

    switch(pn & 0xFF)
    {
    case 0x01:  pn_idx+=1; break;
    case 0x02:  pn_idx+=2; break;
    case 0x04:  pn_idx+=3; break;
    case 0x08:  pn_idx+=4; break;
    case 0x10:  pn_idx+=5; break;
    case 0x20:  pn_idx+=6; break;
    case 0x40:  pn_idx+=7; break;
    case 0x80:  pn_idx+=8; break;
    default:
                return 0;
    }
    return pn_idx;    
}


/*+++
  ���ܣ��Ѳ������PNת��Ϊ����������meter_seq
  ������
        INT16U pn  �������
        BOOLEAN    *flag485  �Ƿ���485���
  ���أ�
        �����ţ�  ���󷵻�0
---*/
INT16U  trans_pn_2_meter_seq(INT16U pn)
{
   INT16U spot_idx;

   //������
   spot_idx = trans_set_pn_2_pnidx(pn);


   //return trans_spot_idx_2_meter_idx(spot_idx);
   return 1;

}
        
/*+++
  ���ܣ��������ݵ�Ԫ��ʶ
  ������
        Reply,&GUN.RELAY.pos,GUN.RELAY.pn,GUN.RELAY.fn
---*/
void  set_pn_fn(INT8U *Reply,INT16U *pos,INT16U pn,INT16U fn)
{
    Reply[(*pos)++]  = pn;
    Reply[(*pos)++]  = pn>>8;
    Reply[(*pos)++]  = fn;
    Reply[(*pos)++]  = fn>>8;
}

/*+++
  ���ܣ��������ݵ�Ԫ��ʶ,FK2005��Լ��ʽ
  ������
        Reply,&GUN.RELAY.pos,GUN.RELAY.pn,GUN.RELAY.fn
---*/
void  set_pn_fn_2005(INT8U *Reply,INT16U *pos,INT16U pn,INT16U fn)
{
    INT16U pn_tmp;
    
    pn_tmp = pn & 0xFF00;
    pn = pn & 0x00FF;
    
    switch(pn_tmp)
    {
        case 0x0100:  pn |= 0x0100; break;
        case 0x0200:  pn |= 0x0200; break;
        case 0x0300:  pn |= 0x0400; break;
        case 0x0400:  pn |= 0x0800; break;
        case 0x0500:  pn |= 0x1000; break;
        case 0x0600:  pn |= 0x2000; break;
        case 0x0700:  pn |= 0x4000; break;
        case 0x0800:  pn |= 0x8000; break;
        default:
            break;
    }
    
    Reply[(*pos)++]  = pn;
    Reply[(*pos)++]  = pn>>8;
    Reply[(*pos)++]  = fn;
    Reply[(*pos)++]  = fn>>8;
}

/*+++
  ���ܣ�����FN����Ż�ȡ��ʷ��������
---*/
INT8U  get_fnprop_from_fnidx(INT16U fn_idx)
{
    //F1~F8
    if(fn_idx <=8)  return PN_METER|LOCK_DAY;

    //F9~F12
    if(fn_idx <=12) return PN_METER|LOCK_METERDAY;

    //F13~F16
    if(fn_idx <17)  return 0;

    //F17~F24
    if(fn_idx <=24) return PN_METER|LOCK_MONTH;

    //F25~F32
    if(fn_idx <=32) return PN_METER|LOCK_DAY;

    //F33~F39
    if(fn_idx <=39) return  PN_METER|LOCK_MONTH;

    //F40
    if(fn_idx == 40)  return 0;


    //F41~F43
    if(fn_idx <=43) return  PN_METER|LOCK_DAY;

    //F44
    if(fn_idx == 44)  return PN_METER|LOCK_MONTH;;

    //F45~F48
    if(fn_idx <49)  return 0;

    //F49~F50
    if(fn_idx <51)  return PN_RTUA|LOCK_DAY;

    //F51~F52
    if(fn_idx <53)  return PN_RTUA|LOCK_MONTH;

    //F53
    if(fn_idx <54)  return PN_RTUA|LOCK_DAY;

    //F54
    if(fn_idx <55)  return PN_RTUA|LOCK_MONTH;

    //F55~F56
    if(fn_idx <57)  return 0;

    //F57~F59
    if(fn_idx <60)  return PN_AGT|LOCK_DAY;

    //F60~F62
    if(fn_idx <63)  return PN_AGT|LOCK_MONTH;

    //F63~F64
    if(fn_idx <65)  return 0;

    if(fn_idx<=112) return PN_METER|LOCK_CURVE;

    if(fn_idx<=128) return PN_METER|LOCK_DAY;

    if(fn_idx<=129) return PN_METER|LOCK_DAY;
    if(fn_idx<=130) return PN_METER|LOCK_MONTH;

    if(fn_idx <136) return  0;

    if(fn_idx<=148) return PN_METER|LOCK_CURVE;

    if(fn_idx <161) return  0;

    //F161~F168
    if(fn_idx <169) return PN_METER|LOCK_DAY;

    //F169~F176
    if(fn_idx <177) return PN_METER|LOCK_METERDAY;

    //F177~F184
    if(fn_idx <=184) return PN_METER|LOCK_MONTH;

    //F185~F188
    if(fn_idx <=188) return PN_METER|LOCK_DAY;

    //F189~F192
    if(fn_idx <=192) return PN_METER|LOCK_METERDAY;

    //F193~F196
    if(fn_idx <=196) return PN_METER|LOCK_MONTH;

    //F197~F200
    if(fn_idx <=200) return  0;

    //F201~F208
    if(fn_idx <=208) return PN_METER|LOCK_MONTH;

    //F209
    if(fn_idx <=209) return PN_METER|LOCK_DAY;

    //F210~F212
    if(fn_idx <=212) return 0;

    //F213~F216
    if(fn_idx <=216) return PN_METER|LOCK_MONTH;

    //F217	̨�����г����ز����ڵ����������	p0	Td_c	����
    //F218	̨�����г����ز����ڵ�ɫ��������	p0	Td_c	����
    
    if(fn_idx <=218) return PN_RTUA | LOCK_CURVE;
    
    if(fn_idx <=220) return PN_METER | LOCK_CURVE;
    if(fn_idx <=221) return PN_METER | LOCK_DAY;
    if(fn_idx <=222) return PN_METER | LOCK_MONTH;

	#ifdef __METER_DAY_FREEZE_EVENT__ // 230~234 Ϊ����
	if(fn_idx <=234) return PN_METER | LOCK_DAY;
	#else
    if(fn_idx <=226) return PN_METER | LOCK_CURVE;

    if(fn_idx <=231) return PN_METER | LOCK_CURVE;
	#endif
    #ifdef __COUNTRY_ISRAEL__
    if(fn_idx == 237) return PN_METER | LOCK_DAY;
    #endif


    return 0;
}

/*+++
  ���ܣ� �������֡����ϢժҪ
  ��ע��
---*/
INT8U check_frame_pw(objRequest *pRequest)
{

   return TRUE;  
}

/*+++
   ���ܣ���λ����  F1,F2,F3-P0
   ����:
       objRequest *pRequest ����֡������
       objResponse *pResp Ӧ��֡������

   ���б��ģ�
           ���ݵ�Ԫ��ʶ(DA=0)
           PW
           TP
   ���б��ģ�
           ȷ��/���ϱ���

---*/
void  process_frame_reset(objRequest *pRequest,objResponse *pResp)
{
    INT16U pn,fn;
    INT16U reset_cmd;
//    INT16U pos;

    ClrTaskWdt();



     //���ݵ�Ԫ��ʶ
    pn = bin2_int16u(pRequest->frame +  pRequest->pos);
    fn = bin2_int16u(pRequest->frame +  pRequest->pos + 2);

 	#ifdef __PROVICE_CHONGQING__
	if(get_system_flag(SYS_RUN_STATUS,SYS_FLAG_BASE) )
	{
		app_send_NAK(pRequest,pResp);
        return;
	}
	#endif
//    reset_cmd=0xFF;

    if((pn != DA_P0) || (fn > DT_F4))
    {
        app_send_NAK(pRequest,pResp);
        return;
    }

    reset_cmd = fn;


    app_send_ACK(pRequest,pResp);

    if(reset_cmd!=DT_F1)
    {
        //app_begin_reset();
        //switch(reset_cmd)
        //{
            //case DT_F2:
                //app_data_reset();
                //break;
            //case DT_F3:
                //before_app_reset();
                //app_param_reset_default();
                //break;
            //case DT_F4:	
                //before_app_reset();
                //app_param_reset();
                //break;
        //}
        //DelayNmSec(5000);
        //ndfat_release_lock_file_system();
        //app_softReset();
        //app_end_reset();
		app_softReset();
    }
    else
    {
            //record_log_code(LOG_SYS_INIT_RESET,NULL,0,LOG_ALL);//��ʼ�������λ
            #ifndef __PROVICE_HUNAN__
            DelayNmSec(5000);
            app_softReset();
            #else
            app_softReset();
            #endif
    }
    //if need reset ertu, then delay 5s.
         //��Ҫִ�и�λ����,���ݵ�ǰACD״̬��������λ��־�֡�
//         system_flag |= SYS_PRG_RESET;
//         if(gprs_tcp_is_connected())
//         {
//             gprs_client_tcp_disconnect();
//             DelayNmSec(1000);
//         }
//         //��ʱ���ȴ�ͨ��ģ�鷢�ͱ��ĵ���վ
//         DelayNmSec(3000);
//         record_sys_operate(OP_SYS_INIT_RESET);//��ʼ����λ
//
//

}


/*+++

  ���ܣ��������ն˵�Ѳ��
  ������
      1�����ն����ϱ����ݣ����ϱ�������
      2�����ն˷����󱻼����ն������ϱ�����
      3���㽭��չ��ԭ376.1������Ҫ�ȵ��÷����ϱ�����.
---*/
void process_frame_ask_slave(objRequest *pRequest,objResponse *pResp)
{
    INT16U pos,fn;
    #ifdef __PROVICE_ZHEJIANG_TODO__
    INT8U pn;
    #endif

    #ifdef __PROVICE_ZHEJIANG_TODO__
    pResp->frame[POS_CTRL] = CTRLFUNC_SET_DIR | CTRLFUNC_USERDATA;
    pResp->work_flag[WORK_FLAG_SEQ_RSEQ] |= MASK_FIR | MASK_FIN;
    pResp->work_flag[WORK_FLAG_SEQ_RSEQ] &= ~MASK_CON;
    pos=POS_DATA;
    // 1�����ն˷�AFN=18 F1����ѯ���ն��Ƿ��������ϱ���
    // 2�����ն�Ӧ�ûظ�AFN=18 F1  AA�������� 55��������
    // 3��������ն������ݣ�����F1�� 55������ͨ��AFN=18 F2������ն����ݡ�
    // 4�����ն��ϱ�һ֡���ݡ�
    // 5���ظ�����1��ֱ���ϱ����ն�F1��AA
    if(AFN_ASK_SLAVE_NEXT == pRequest->frame[POS_AFN])
    {
        //���ݵ�Ԫ��ʶ
        pn = bin2_int16u(pRequest->frame + pRequest->pos);
        fn = bin2_int16u(pRequest->frame + pRequest->pos + 2);
        if(DT_F1 == fn)
        {
            set_pn_fn(pResp->frame, &pos, pn, fn);
            pResp->frame[pos++] =  send_report_frames(TRUE,FALSE) ?  0x55 : 0xAA;
            pResp->pos = pos;
            app_setFrameLenThenSendFrame(pRequest,pResp);
        }
        else
        {
            //������ϱ����ģ��򱨸�󷵻�,���򣬸�������������Ӧ��
            send_report_frames(TRUE,TRUE);
        }
        return;
    }  
    #endif
   
    //������ϱ����ģ��򱨸�󷵻�,���򣬸�������������Ӧ��
    //if(send_report_frames(TRUE,TRUE)) return;


    //Ӧ��
    //���ÿ����룺DIR=1,PRM=0, FUNC=8 ��Ӧ���ģ��û�����
    pResp->frame[POS_CTRL] = CTRLFUNC_SET_DIR | CTRLFUNC_USERDATA;
    pResp->work_flag[WORK_FLAG_SEQ_RSEQ] |= MASK_FIR | MASK_FIN;
    pResp->work_flag[WORK_FLAG_SEQ_RSEQ] &= ~MASK_CON;
    pos=POS_DATA;

   // if(AFN_ASK_SLAVE_NEXT == frame[POS_AFN])
   // {
        
  //  }
  //  else
  //  {
       pResp->frame[POS_AFN] = AFN_ACK;
       fn = DT_F2; //ȫ������
       set_pn_fn(pResp->frame, &pos, DA_P0, fn);
   // }
    pResp->pos = pos;
    app_setFrameLenThenSendFrame(pRequest,pResp);
}

//��ȡϵͳ�洢����
INT16U sys_cmdex_storedata(INT8U *param,INT8U *buffer)
{
/*
TYPE 	1�ֽ�  1-�ļ�ϵͳFLASH 2-�������ֿ�FLASH 3-���� 4-�ļ�ϵͳ  5-�ڴ�
OP   	1�ֽ�  0-��ȡ 1-����
ADDDR   4�ֽ�  ��ַ
LEN		2�ֽ�  ����
data  	n�ֽ�  ����Ϊ����������Ϊ����
*/

    INT32U 	addr;
    INT16U 	datalen;
    INT16U 	fileid;
    INT16U 	pageno;
    INT16U  pos;
    INT16U  idx;
    INT16U  result;
	INT8U 	type;
    INT8U 	op;


    type=param[0];
    op=param[1];
    addr=bin2_int32u(param+2);
    datalen=bin2_int16u(param+6);

    pos = 0;
    mem_cpy(buffer,param,8);
    pos+=8;

    result =8;
    if(type<5)
	{
			if(op==2)
			{
    			OSMutexPend(&SIGNAL_FLASHSPI);
				return result;
			}
			else if(op==3)
			{
    			OSMutexFree(&SIGNAL_FLASHSPI);
				return result;
			}
	}
    switch(type)
    {
       case 1: //1-�ļ�ϵͳFLASH
    		pageno=bin2_int16u(param+8);//��ȡ����ȡҳ��
            
    		mem_cpy(buffer+pos,param+8,2);//���� page no
            pos+=2;
            fread_flashArray(pageno,addr,buffer+pos,datalen);
		    result+=(6+datalen);
	      break;
//	   case 2://2-�������ֿ�FLASH 
//          //  os_spansion_readArray(addr,buffer+pos,datalen);
//		    result+=(6+datalen);
//			break;
       case 3://3-����
 	      read_fmArray(addr,buffer+pos,datalen);
		  result+=(6+datalen);
          break;
       case 4://4-�ļ�ϵͳ
    	    fileid=bin2_int16u(param+8);//��ȡ����ȡ�ļ���
            mem_cpy(buffer+pos,param+8,2);//�����ļ�ID
            pos+=2;
	    fread_array(fileid,addr,buffer+pos,datalen);
            result+=(6+datalen);
       	  break;
//       case 5://5-�ڴ�,�ݲ�ʵ��
//			/*
//    	    fileid=bin2_int16u(param+8);//��ȡ����ȡ�ļ���
//			datalen=printfile_pagehead(fileid,buffer+pos);
//		    result+=(6+datalen);
//			*/
//       		break;
//	   case 6://DS3231
//       	       //	iic_readBytes(addr,buffer+pos,datalen);
//		  	result+=(6+datalen);
//			break;
//	   case 7://������Ϣ
//			datalen=sizeof(tagGprsBytesInfo);
//			mem_cpy(buffer+pos,(INT8U*)&g_gprsbytesinfo,sizeof(tagGprsBytesInfo));
//		  	result+=(6+datalen);
//			break;
       default:
       	break;
 	}

	return   result;

}
/*+++

  ���ܣ� ���ҵ���
  ������
  ���أ�
  ������
      1)F1: ��ȡ�ļ�����   PN=P0�����ݵ�Ԫ���ݣ��ļ���(INT16U), ��ʼƫ�ƣ�32λ��ַ�����ֽ���ǰ�������ݳ��ȣ�INT16U��
      2)F2����ʼ������ PN=P0,���ݵ�Ԫ���ݣ����1-ϵͳ��־
      3)F3:д����    PN=P0�����ݵ�Ԫ���ݣ��ļ���(INT16U), ��ʼƫ�ƣ�32λ��ַ�����ֽ���ǰ�������ݳ��ȣ�INT16U��
---*/
void  process_vendor_debug(objRequest *pRequest,objResponse *pResp)
{
    INT16U pi,fi;
    INT8U ret=0;


    //is_first_frame = TRUE;
    pResp->work_flag[WORK_FLAG_IS_FIRST_FRAME] = TRUE;
	//���ÿ����룺DIR=1,PRM=0, FUNC=8  ��Ӧ���ģ��û�����
	pResp->frame[POS_CTRL] = CTRLFUNC_SET_DIR | CTRLFUNC_USERDATA;

    //��ȡ���ݵ�Ԫ��ʶ
    pi = bin2_int16u(pRequest->frame +  pRequest->pos);
    fi = bin2_int16u(pRequest->frame +  pRequest->pos + 2);
    pRequest->pos += 4;
    set_pn_fn(pResp->frame, &pResp->pos, pi, fi);
    switch(fi)
    {
    case DT_F1:  //��ȡ�ļ�����
          pResp->pos += sys_cmdex_storedata(pRequest->frame + pRequest->pos,pResp->frame + pResp->pos);
	 	  if((pRequest->frame[pRequest->pos + 1] == 2) || (pRequest->frame[pRequest->pos + 1] == 3))
		  {
            app_send_ACK(pRequest,pResp);
			return;
		 }
         break;
    case DT_F2:
        //ret=cmd_extend_system_data(pRequest->frame + pRequest->pos);
        if(ret == 0)
        {
            app_send_ACK(pRequest,pResp);
        }
        else
        {
            app_send_NAK(pRequest,pResp);
        }
        return;
    case DT_F3:
        //ret=cmd_write_system_data(pRequest->frame + pRequest->pos);
        if(ret > 0)
        {
            app_send_ACK(pRequest,pResp);
        }
        else
        {
            app_send_NAK(pRequest,pResp);
        }
        return;
    default:
         app_send_NAK(pRequest,pResp);
         return;
    }

    app_send_response_frame(pRequest,pResp,TRUE);
}



/*+++

  ���ܣ� ��·�ӿڼ��
  ������
  ���أ�
  ������
      1)F3: Ŀǰ����������
---*/
void  process_test(objRequest *pRequest,objResponse *pResp)
{
    INT16U pos;
    INT16U pi,fi;
//    BOOLEAN is_first_frame;

    pos = POS_DATA;

    //is_first_frame = TRUE;
    pResp->work_flag[WORK_FLAG_IS_FIRST_FRAME] = TRUE;

    //��ȡ���ݵ�Ԫ��ʶ
    pi = bin2_int16u(pRequest->frame +  pRequest->pos);
    fi = bin2_int16u(pRequest->frame +  pRequest->pos + 2);
    pRequest->pos += 4;
    set_pn_fn(pResp->frame, &pos, pi, fi);
    switch(fi)
    {
    case DT_F3:  //����
        app_send_ACK(pRequest,pResp);
        return;
    default:
        app_send_NAK(pRequest,pResp);
        return;
    }
}

/*+++

  ���ܣ� ���������չ
  ������
  ���أ�
  ������
 *    1)F1~F8��Һ������
 *    2)F9~F16��ָʾ�Ʋ���(II�ͼ�����)
---*/
void process_product_test(objRequest *pRequest,objResponse *pResp)
{
    
    INT16U pi,fi;


    pResp->pos = POS_DATA;

    //is_first_frame = TRUE;
    pResp->work_flag[WORK_FLAG_IS_FIRST_FRAME] = TRUE;
    //���ÿ����룺DIR=1,PRM=0, FUNC=8  ��Ӧ���ģ��û�����
	pResp->frame[POS_CTRL] = CTRLFUNC_SET_DIR | CTRLFUNC_USERDATA;

    //��ȡ���ݵ�Ԫ��ʶ
    pi = bin2_int16u(pRequest->frame +  pRequest->pos);
    fi = bin2_int16u(pRequest->frame +  pRequest->pos + 2);
    pRequest->pos += 4;
    set_pn_fn(pResp->frame, &pResp->pos, pi, fi);
    switch(fi)
    {
    case DT_F9:  //ָʾ�ƿ���
         //��Ԥ����һ���ֽ����ں������ܵ���չ
         pRequest->pos ++;
         app_send_ACK(pRequest,pResp);
         #if defined (__SGRID_HARDWARE_II__)|| defined(__NGRID_HARDWARE_II__)
         output_led_test_status();
         #endif
         return;
    default:
         app_send_NAK(pRequest,pResp);
         return;
    }
}
//void  app_send_active_test(bool login,objResponse *pResp)
//{
//   //INT8U get_cur_seq(void);
//
//   BOOLEAN allow_acd = TRUE;
//   INT8U idx;
//   INT8U cur_seq;
//   INT16U fn;
//
//   fn = (login==true)?DT_F3:DT_F1;
//   
//    pResp->frame[POS_CTRL] = CTRLFUNC_SET_DIR | CTRLFUNC_SET_PRM |PRM_CTRLFUNC_TEST;
//
//    pResp->frame[POS_AFN] = AFN_TEST;
//    cur_seq = 1;//get_cur_seq();
//    pResp->work_flag[WORK_FLAG_SEQ_RSEQ]  =  MASK_FIR|MASK_FIN|MASK_CON | cur_seq;
//
//    // PN = P0
//    pResp->frame[POS_DATA] = 0x00;
//    pResp->frame[POS_DATA+1] = 0x00;
//
//    //FN
//    pResp->frame[POS_DATA+2] = fn & 0xFF;
//    pResp->frame[POS_DATA+3] = fn >> 8;
//
//    pResp->pos = POS_DATA+4;
//    #ifdef __QNMDW_FK2012__
//    for(idx=0;idx<6;idx++)
//    {
//        pResp->frame[pResp->pos] = byte2BCD( datetime[idx] );
//        if(idx == 4)
//        {
//            pResp->frame[pResp->pos] |= (datetime[WEEKDAY] << 5);   //���ڷ����µĸ�3λ
//        }
//        pResp->pos++;
//    }
//    for(idx=0;idx<4;idx++)
//    {
//        pResp->frame[pResp->pos] = 0xFF;
//        pResp->pos++;
//    }
//    #elif ((defined __YUNNAN_FK__) || (defined __QGDW_376_2009_PROTOCOL__)) //��ǩ��__QGDW_FK2005__����Ҫ��ͻ��Ӧ���Ǻ�FK2012��ͻ��δ����
//    {
//
//    }
//    #else
////    if(fn == DT_F3)
////    {
////       for(idx=0;idx<6;idx++)
////       {
////          pResp->frame[pResp->pos]= byte2BCD( datetime[idx] );
////          if(idx == 4)
////          {
////            pResp->frame[pResp->pos] |= (datetime[WEEKDAY] << 5);  //���ڷ����µĸ�3λ
////          }
////          pResp->pos ++;
////       }
////    }
//    #endif
//
//      //������Ϻ������������ʱ������ACDλ���������ܴ�
//   #ifndef __PROVICE_SHANGHAI__
//   if(AFN_TEST == pResp->frame[POS_AFN])
//   {
//            allow_acd = FALSE;
//   }
//   #endif
//
//   if(allow_acd==FALSE)
//   {
//      //2010-6-21  ĳЩ��վ����ʶ���������ĵ�ACD����������ȥ������(����Эͬ����վ)
//      pResp->frame[POS_CTRL] &= ~CTRLFUNC_SET_ACD;
//      pResp->frame[POS_SEQ]  &= ~MASK_TpV;
//   }
//   else
//   {
//     //1. ����ACDֵ����Ƿ���Ҫ����EC
////      #ifdef __PROVICE_SHANGHAI__
////      if(pResp->frame[POS_CTRL] & CTRLFUNC_SET_ACD)
////      #else
//      //if(ACD && (system_flag & SYS_FRAME_ACD_FLAG) )
////      if(g_event.acd)
////      #endif
////      {
////        pResp->frame[POS_CTRL] |= CTRLFUNC_SET_ACD;
////        //EC
////        //��Ҫ�¼�������EC1
////        pResp->frame[pResp->pos++] = g_event.ec1;
////
////        //һ���¼�������EC2
////        pResp->frame[pResp->pos++] = g_event.ec2;
////      }
//
//   }
//
//   //3.1 ��ȥ�̶�����֡����ͷ
//   pResp->pos -= 6;
//
//   //3.2 ���ݳ�������2λ
//   pResp->pos<<=2;
//
//   //3.3  ��Լ���ͣ���������Ĺ�Լ���ͻش�
//   #ifdef __QGXD_FK2012__
//   pResp->protocol_type =QGXD_FK2012;
//   #elif defined  __QNMDW_FK2012__
//   pResp->protocol_type = QNMDW_FK2012;
//   #elif defined __QGDW_FK2005__
//   pResp->protocol_type = QGDW_2005;
//   #else
//   //pResp->protocol_type = QGDW_376_1;
//   #endif
//   pResp->pos += QGDW_376_1;
//   //3.4 ���ó���
//   pResp->frame[1] =  pResp->pos;
//   pResp->frame[2] =  pResp->pos >> 8;
//   pResp->frame[3] =  pResp->frame[1];
//   pResp->frame[4] =  pResp->frame[2];
//
//   //4 ������Ӧ֡��ţ��������ϱ�����һ��֡��ţ�
//   pResp->frame[POS_SEQ] = pResp->work_flag[WORK_FLAG_SEQ_RSEQ];
//
//
//   //�����ն��߼���ַ������������+�ն˵�ַ
//   mem_cpy(pResp->frame+POS_RT,gSystemInfo.ertu_devid,CFG_DEVID_LEN);
//   pResp->frame[POS_MSA] = 0;
//
//   // ����У��λ
//   app_encodeFrame(pResp);
//
//   if(pResp->channel==CHANNEL_NONE)
//   {
//
//     return;
//   }
//   //6 ������Ӧ֡
//   if(!login)
//   {
//       //app_send_ReplyFrame_urgent(pResp);
//   }
//   else
//   {
//       app_send_ReplyFrame(pResp);
//   }
//}

/*+++
  ���ܣ�������б��ĵ��ն˵�ַ�Ƿ�߱���Ӧ����
  ������
        INT8U *frame,         �����
        PLMS_RESP *resp       ��Ӧ��
  ���أ�
         TRUE / FALSE
  ������
        1����ͨѶģ��֮��ͨѶʱ���ն˵�ַΪ5��0x00

---*/
BOOLEAN check_ertu_address(objRequest *pRequest,objResponse *pResp)
{
//    INT32U  eeAddr;
    INT32U rtuid;
    INT16U  group_id=0;
    INT8U devid[6]={0};

    pResp->work_flag[WORK_FLAG_IS_CAST] = 0;
    pResp->work_flag[WORK_FLAG_CMD_FLAG] = 0;

    //���ȼ��������������A1�Ƿ���ȷ����������Ӧ
    mem_cpy(devid,gSystemInfo.ertu_devid,CFG_DEVID_LEN);

    // ����ն˵�ַ����������Ƿ�Ϊ���������Ĺ㲥��ַ
    if( (devid[0] != pRequest->frame[POS_RT]) || (devid[1] != pRequest->frame[POS_RT+1]) )
    {
        #ifdef __PROVICE_HEBEI__
        //�ӱ��㲥��ַ0x22114433
        if(((pRequest->frame[POS_RT] == 0xFF) && (pRequest->frame[POS_RT+1] == 0xFF)) ||
           ((pRequest->frame[POS_RT] == 0x11) && (pRequest->frame[POS_RT+1] == 0x22)))
        #else
         //ɽ�������㲥��ַ0x9999FFFF 
        if(((pRequest->frame[POS_RT] == 0xFF) && (pRequest->frame[POS_RT+1] == 0xFF)) ||
           ((pRequest->frame[POS_RT] == 0x99) && (pRequest->frame[POS_RT+1] == 0x99)))
        #endif    
            pResp->work_flag[WORK_FLAG_CMD_FLAG] |= MSA_BROADCAST;
        else
            return FALSE;
    }

    //Ȼ�����ն˵�ַ���Ƿ���ȷ����������Ӧ
    #ifdef __RTUADDR_IS_6BYTE__
    rtuid = bin2_int32u(pRequest->frame+POS_UID);
    #else
    rtuid = bin2_int16u(pRequest->frame+POS_UID);
    #endif
    #ifndef __PROVICE_JIANGSU__
    if(rtuid==0)
    {
        #ifndef __PROVICE_JIANGXI__
        return FALSE;   //��Ч��ַ
        #endif
    }
    #endif
    if(pRequest->frame[POS_MSA] & 0x01)
    {
        //���ַ
#ifdef __POWER_CTRL__
        //���ַ�Ļ���Ҫ����Ƿ����޳�״̬
        if(gPowerCtrl.CtrlSetStatus.specStatus & 0x02)
        {
           if((pRequest->frame[POS_AFN] != AFN_CTRL)
               ||(pRequest->frame[POS_DATA]!= 0x00)||(pRequest->frame[POS_DATA+1]!= 0x00)
               ||(pRequest->frame[POS_DATA+2]!= 0x40)||(pRequest->frame[POS_DATA+3]!= 0x03)               )
           {
                return FALSE;
           }
        }
#endif
        //return FALSE;
    #if defined (__RTUADDR_IS_6BYTE__)
        if(rtuid == 0xFFFFFFFF)
    #elif defined (__PROVICE_HEBEI__) 
        if((rtuid == 0xFFFF)||(rtuid == 0x4433))     //�㲥��ַ
    #else
        if(rtuid == 0xFFFF)     //�㲥��ַ
    #endif
        {
            pResp->work_flag[WORK_FLAG_IS_CAST] = 1;
            return TRUE;
        }

//        fread_ertu_params(EEADDR_SET_F6,var.value,16);
//        for(idx=0;idx<8;idx++)
//        {
//            group_id = var.F6[idx][1]*0x100+var.F6[idx][0];
//            if(group_id == 0) continue; //���ַΪ0���������ַ
//            if(group_id == rtuid) break;
//        }
        if(group_id == rtuid)
        {
            pResp->work_flag[WORK_FLAG_IS_CAST] = 1;
            return TRUE;
        }
        else
        {
            return FALSE;   //��Ч��ַ
        }
 /*
        if(CMD_BROADCAST) return FALSE;

        eeAddr = EEADDR_F6;
        for(idx=0;idx<8;idx++)
        {
            read_fmArray(eeAddr,(INT8U *)&group_id,2);
            if(group_id == 0) continue; //���ַΪ0���������ַ
            if(group_id == rtuid) break;
        }
        if(group_id != rtuid) return FALSE;
  */
    }
    else 
    {
        #if defined (__RTUADDR_IS_6BYTE__)
        if( (pResp->work_flag[WORK_FLAG_CMD_FLAG] & MSA_BROADCAST)  && (rtuid != 0xFFFFFFFF) ) return FALSE;
        #elif defined (__PROVICE_HEBEI__)
        if( (pResp->work_flag[WORK_FLAG_CMD_FLAG] & MSA_BROADCAST)  && ((rtuid != 0xFFFF)&&(rtuid != 0x4433)) ) return FALSE;
        #else
        if( (pResp->work_flag[WORK_FLAG_CMD_FLAG] & MSA_BROADCAST)  && (rtuid != 0xFFFF) ) return FALSE;
        #endif
        //����ַ 
        if(compare_string(devid+2,pRequest->frame+POS_UID,CFG_DEVID_LEN-2) != 0)
        //if( (devid[2] != frame[POS_UID]) || (devid[3] != frame[POS_UID+1]) )
        {
             //������Ƿ��ǹ㲥��ַ
            #if defined (__RTUADDR_IS_6BYTE__)
            if(rtuid != 0xFFFFFFFF)  return FALSE;
            #elif defined (__PROVICE_HEBEI__)
            if((rtuid != 0xFFFF)&&(rtuid != 0x4433))  return FALSE;
            #else
             if(rtuid != 0xFFFF)  return FALSE;
            #endif

        }
    }
    return TRUE;
}

/*+++
   ���ܣ�ȷ��/���ϱ���
   ����:
       INT8U *Reply Ӧ��֡������
       INT16U frameLen  ����֡�����ݵ�Ԫ�򳤶�
       INT8U *frameData ����֡�����ݵ�Ԫ��

   ���б��ģ�
           ���ݵ�Ԫ��ʶ(DA=0)
           PW
           TP
---*/
void process_frame_ack(objRequest  *pRequest,objResponse *pResp)
{
    if((gActiveSendAck.need_check_ack == TRUE)&&(gActiveSendAck.check_seq == (pResp->frame[POS_SEQ] & 0x0F)))
    {
        #ifdef __ENABLE_4852_ERC14_REPORT__
        if(pRequest->channel == CHANNEL_485_2)
    	{
                g_erc14_report = 0;
                Channel_rs485_2.dir = 0;//����̬
    	}
        #endif
        gActiveSendAck.has_acked = TRUE;
    }

    //�������·״̬��Ӧ,��Ҳ���ܴ���
    //if( (resp->buffer[POS_CTRL] & 0x0F) == CTRLFUNC_TEST) return;

    //TODO:�����ϱ�״̬���
//    if(system_flag & SYS_POWER_LOST)
//    {
//         //�ϱ������¼���־λ
//        if(system_flag & SYS_POWER_LOST_RPT)
//        {
//           //�������Ƿ������
//           if(RPT_POWER_LOST.b.rtp_seq == pResp->frame[POS_SEQ])
//           {
//              system_flag &= ~SYS_POWER_LOST_RPT;   //����ϱ���־λ.
//           }
//        }
//    }

    //�ϱ������ն���ȴ�ȷ��֡
    #ifdef __PATCH_REC_DH__
    if(report_patch.wait_confirm.is_wait_confirm == 1)
    {
        if(report_patch.seqtime.seq == (pResp->frame[POS_SEQ] & 0x0F))
        {
            report_patch.wait_confirm.is_wait_confirm = 0;
            report_patch.wait_confirm.is_confirm = 1;
        }
    }
    #endif

    return;
}

/*
��������Ч�������л�ȡ��һ��������
����1��ԭʼDA��ʶ
����2��Ŀ��DA��ʶ
����3�����в������������ڼ���
����ֵ��TRUE-��ʾ��ǰ������ЧDA,FALSE-��ʶ��ǰ��������ЧDA������Ҫ�ٵ��øú������л�ȡ
*/
BOOLEAN get_next_pn(INT16U pn,INT16U *pn_new,INT16U *pn_idx)
{
   INT8U val;

   if(*pn_idx>=MAX_METER_COUNT)return FALSE;
   if((pn!=0x00FF) && (pn!=0xFFFF))//�������в����㣬����Ҫ����
   {
       *pn_new=pn;
       *pn_idx=MAX_METER_COUNT;
       return TRUE;
   }
   //����˼·��һ�ζ�ȡ8�������㣨1���ֽڣ���������ֽڴ�����Ч�������򣬷��ظ�������㣬���������ȡ��ֱ������
   while(*pn_idx<MAX_METER_COUNT)
   {
      (*pn_idx)++;
      val=meter_exist((*pn_idx));
      if(val==0)continue;//û����Ч�����㣬������ȡ

       //ת����λ��ʶ������Ϊʵ��DA
       val=0;
       set_bit_value(&val,1,(*pn_idx-1)%8);
       *pn_new=((INT8U)((*pn_idx-1)/8+1))<<8;
       *pn_new|=val;
       return TRUE;
    }

    return FALSE;
}

/*+++
  ���ܣ��������Ƿ���Ч
  ����:
        INT16U spot_idx  �������
  ����:
        ��ѯ���

  ����:

---*/
BOOLEAN meter_exist(INT16U spot_idx)
{
  //INT16U meter_idx;

  //meter_idx = meter_spot_2_meter_seq_map[spot_idx];
  //if(meter_idx & 0x8000)
  {
    return TRUE;
  }
  //return FALSE;
}

uint16_t app_make_active_test_frame(uint8_t *frame,uint8_t is_first)
{
    INT8U get_cur_seq(void);
    tagDatetime datetime;
    INT8U cur_seq;
    INT16U fn;
    INT16U pos;
    INT8U     protocol_type;
    INT16U frameLen;
    INT16U i;
    INT8U cs;
    
    os_get_datetime(&datetime);
	#ifdef __HEBEI_HUANBAO__
	    frame[0]=0xFF;
	    mem_cpy_reverse(frame+1,gmeter_doc.meter_no,6);
	    frameLen = 7;
	    return frameLen;
	#else
		fn = is_first?DT_F1:DT_F3;
   
		frame[POS_CTRL] = CTRLFUNC_SET_DIR | CTRLFUNC_SET_PRM |PRM_CTRLFUNC_TEST;

		frame[POS_AFN] = AFN_TEST;
		// PN = P0
		frame[POS_DATA] = 0x00;
		frame[POS_DATA+1] = 0x00;

		//FN
		frame[POS_DATA+2] = fn & 0xFF;
		frame[POS_DATA+3] = fn >> 8;

		pos = POS_DATA+4;
   
		if(fn == DT_F3)
		{
			frame[pos++]= byte2BCD( datetime.second );
			frame[pos++]= byte2BCD( datetime.minute );
			frame[pos++]= byte2BCD( datetime.hour );
			frame[pos++]= byte2BCD( datetime.day );
			frame[pos++]= byte2BCD( datetime.month )|(datetime.weekday<<5);
			frame[pos++]= byte2BCD( datetime.year );
		}

	//    if(g_event.acd)
	//    {
	//        frame[POS_CTRL] |= CTRLFUNC_SET_ACD;
	//        //EC
	//        //��Ҫ�¼�������EC1
	//        frame[pos++] = g_event.ec1;
	//
	//        //һ���¼�������EC2
	//        frame[pos++] = g_event.ec2;
	//    }

		//3.1 ��ȥ�̶�����֡����ͷ
		pos -= 6;

		//3.2 ���ݳ�������2λ
		pos<<=2;

		//3.3  ��Լ���ͣ���������Ĺ�Լ���ͻش�
		#ifdef __QGXD_FK2012__
		protocol_type =QGXD_FK2012;
		#elif defined  __QNMDW_FK2012__
		protocol_type = QNMDW_FK2012;
		#elif defined __QGDW_FK2005__
		protocol_type = QGDW_2005;
		#else
		protocol_type = QGDW_376_1;
		#endif
		pos += protocol_type;
		//3.4 ���ó���
		frame[1] =  pos;
		frame[2] =  pos >> 8;
		frame[3] =  frame[1];
		frame[4] =  frame[2];

		//4 ������Ӧ֡��ţ��������ϱ�����һ��֡��ţ�
		cur_seq = 1;//get_cur_seq();
		frame[POS_SEQ] = MASK_FIR|MASK_FIN|MASK_CON | cur_seq;


		//�����ն��߼���ַ������������+�ն˵�ַ
		mem_cpy(frame+POS_RT,gSystemInfo.ertu_devid,CFG_DEVID_LEN);
		frame[POS_MSA] = 0;

		//ȷ��֡֡��ʼ������ȷ
		frame[0]=0x68;
		frame[5]=0x68;

		//��ȡ�û�����������
		frameLen = bin2_int16u(frame+1);
		frameLen >>=2;

		//����У��λ
		cs=0;
		frameLen += 6;   //���Ϲ̶����ȵı���ͷ
		for(i=6;i<frameLen;i++)  cs += frame[i];
		frame[frameLen++]=cs;

		//���ý����ַ�
		frame[frameLen++]=0x16;

		return frameLen;
	#endif
}

void app_run_init(void)
{
    INT16U tmp=0;
    INT8U data[2]={0};
#ifdef __EDP__
	INT8U edp_interval =0;
	INT8U rand_report_begin =0,rand_report_end =0;	
#endif

//INT8U tmp_updata_verison[16];
//tmp_updata_verison[11] =0x89;
//fwrite_ertu_params(EEADDR_CLEANNMETER_WARNING,&tmp_updata_verison,16);
//tmp_updata_verison[11] =0x00;
//fread_ertu_params(EEADDR_CLEANNMETER_WARNING,&tmp_updata_verison,16);
    //��ȡ�ն˵�ַ
    fread_ertu_params(EEADDR_CFG_DEVID,gSystemInfo.ertu_devid,CFG_DEVID_LEN);
    fread_ertu_params(EEADDR_METER_SETFLAG,&g_meter_set_flag,1);
    #ifdef __SOFT_SIMULATOR__
    g_meter_set_flag = 0x55;
    #endif
    gSystemInfo.has_data_report = 0x00;
    gSystemInfo.is_debug = 0;//Ĭ�Ϸǵ���
    read_custom_param(CONST_FRAME_MONITOR, data);      //2�����ļ��
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
        //gSystemInfo.is_debug = 1;//�������߿��� ��������ģʽ onenet��������ʱĬ�ϲ����ߺ͸�λģ�� 
    }
    else
    {
        gSystemInfo.clock_ready = 0;
        //gSystemInfo.is_debug = 0;
    }

    fread_ertu_params(EEADDR_STAT_FLOW,(INT8U *)&g_app_run_data.gprs_bytes,sizeof(tagAPP_GPRS_BYTES));   //�ϵ����Ƚ�nor flash�е�������������
    fread_ertu_params(EEADDR_SET_F702,&g_app_run_data.meter_control.is_control_valid,sizeof(tagAPP_CONTROL));//����բ����
    fread_ertu_params(EEADDR_SET_F701,(INT8U*)&g_app_run_data.password,4);//�������
#ifdef __ONENET__
    fread_ertu_params(EEADDR_TASK_REPORT,task.value,sizeof(tagDatetime)*2);
#endif
    gSystemInfo.update = 0;//Ĭ����0 
    gSystemInfo.update_begin_tick=0;
	
	gSystemInfo.filename = NULL;
	gSystemInfo.offset = 0;
	gSystemInfo.data = NULL;
	gSystemInfo.len = 0;
	gSystemInfo.usb_write = 0;	
#ifdef __EDP__
   for(int i =0;i<5;i++)
   {
	   nor_flash_erase_page(FLASH_EDP_RAND_REPORT_STORE_START+i);
   }
	fread_ertu_params(EEADDR_TIME_INTERVAL,&edp_interval,1);
	if(edp_interval ==0xFF)
	{
		edp_interval =3;
	}
	gSystemInfo.edp_report_interval =edp_interval ;
	first_poweron_flash_rebuilt();
	first_poweron_flash_wait_report_rebuilt();
	fwrite_ertu_params(EEADDR_EDP_RAND_BEGIN,&rand_report_begin,1);
	fwrite_ertu_params(EEADDR_EDP_RAND_END,&rand_report_end,1);
	if((rand_report_begin == 0xFF)||(rand_report_begin == 0x00))
	{
		gSystemInfo.edp_rand_report_begin =1;   //�����ʱ��ʼʱ�䣬Ĭ��ֵ
	}
	else
	{
		gSystemInfo.edp_rand_report_begin =(INT8U)rand_report_begin;	
	}
	if((rand_report_end == 0xFF)||(rand_report_end == 0x00))
	{
		gSystemInfo.edp_rand_report_end = 3;//�����ʱ����ʱ�䣬Ĭ��ֵ
	}
	else
	{
		gSystemInfo.edp_rand_report_end =(INT8U)rand_report_end;
	}
   
#endif
	//fread_ertu_params(EEADDR_SWITCH_PARAM,&gSystemInfo.switch_param,4);
	//if(gSystemInfo.switch_param == 0xFFFFFFFF)
	//{
		//gSystemInfo.switch_param = 0;
		//fwrite_ertu_params(EEADDR_SWITCH_PARAM,&gSystemInfo.switch_param,4);	
	//}
}