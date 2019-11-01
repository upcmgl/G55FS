#include "main_include.h"

 INT16S meter_uart_read_byte();
 INT16S debug_uart_read_byte(void);

uint8_t check_recv_process(objRequest* Request,int16_t (* read)(void))
{
    int16_t rx_data;
    uint16_t frameLen;
#ifdef __EDP__	
	static INT8U maybe376 =0;
#endif	
    if(Request->state == REQUEST_STATE_IDLE)
    {
        rx_data = read();
        if(rx_data == -1)
        {
            return 0;
        }
	
        mem_cpy(Request->frame,Request->frame+1,MAX_FRAME_HEADER_SIZE-1);
        Request->frame[MAX_FRAME_HEADER_SIZE-1] = rx_data;
        Request->recv_len = MAX_FRAME_HEADER_SIZE;
        #ifdef __OOP_G_METER__
        if(Request->surported_protocol.GDW1376 &&(check_frame_header_gb_oop_down(Request->frame)))
        {
            Request->cur_protocol.GDW1376 = 1;
           //���㱨�����峤�ȡ�
           frameLen =  get_gboop_frame_len(Request->frame,MAX_FRAME_HEADER_SIZE);
           if( frameLen > (SIZE_OF_APP_PROTOCOL_FRAME-50))
           {
               Request->cur_protocol.GDW1376 = 0;
           }
        }
        if(Request->surported_protocol.GB645 && (check_frame_header_gb_oop_up(Request->frame)))
        {
            Request->cur_protocol.GB645 = 1;
        }
        #else
        if(Request->surported_protocol.GDW1376 &&(is_1376_1_frame_header(Request->frame,MAX_FRAME_HEADER_SIZE)))
        {
            Request->cur_protocol.GDW1376 = 1;
           //���㱨�����峤�ȡ�
           frameLen =  get_1376_1_frame_len(Request->frame,MAX_FRAME_HEADER_SIZE);
           if( frameLen > (SIZE_OF_APP_PROTOCOL_FRAME-50))
           {
               Request->cur_protocol.GDW1376 = 0;
           }
#ifdef DEBUG
			system_debug_info("************has receive 376 frame  REQUEST_STATE_RECVING***************");
#endif		   
        }
        if(Request->surported_protocol.GB645 && (is_gb645_frame_header(Request->frame,MAX_FRAME_HEADER_SIZE)))
        {
            Request->cur_protocol.GB645 = 1;
        }

        #endif

#ifdef __EDP__
		if((rx_data == 0x68) &&(Request == &RequestRemote)) //������gprs�˿ڽ��������ݴ���
		{
			maybe376 = 11;  //������ʼ��10���ֽڡ�
		}
		if(maybe376>0)
		{
			maybe376--;
		}
		if((Request->surported_protocol.ONENET)&&(maybe376 ==0))
		{
			if((!Request->cur_protocol.value) && ((Request->frame[MAX_FRAME_HEADER_SIZE-1] &0x0F)==0)&&((Request->frame[MAX_FRAME_HEADER_SIZE-1] !=0)))
			{
				#ifdef DEBUG
				system_debug_info("************begin edp handle***************");
				system_debug_data(&(Request->frame[MAX_FRAME_HEADER_SIZE-1]),1);
				#endif
				Request->cur_protocol.ONENET = 1;
			}
		}
#endif		
        if(Request->cur_protocol.value)
        {
            Request->state = REQUEST_STATE_RECVING;
        }
    }    
    else if(Request->state == REQUEST_STATE_RECVING)
    {
        rx_data = read();
        if(rx_data == -1)
        {
            return 0;
        }
        Request->frame[Request->recv_len++] = rx_data;
        #ifdef __OOP_G_METER__
        if(Request->cur_protocol.GDW1376)
        {
            Request->frame_len = get_gboop_frame_len(Request->frame,MAX_FRAME_HEADER_SIZE);
            if(Request->recv_len >= Request->frame_len)
            {
                Request->cs = 0;
                if(is_oop_protocol_frame_ready(Request->frame,Request->frame_len,Request->recv_len,&(Request->cs)))
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
        if(Request->cur_protocol.GB645)
        {
            Request->frame_len = get_gboop_frame_len(Request->frame,MAX_FRAME_HEADER_SIZE);
            if(Request->recv_len >= Request->frame_len)
            {
                //Request->cs  = init_gb645_frame_checksum(Request->frame,Request->frame_len,Request->recv_len);
                Request->cs = 0;
                if(is_oop_protocol_frame_ready(Request->frame,Request->frame_len,Request->recv_len,&(Request->cs)))
                {
                    if(Request->channel == PROTOCOL_CHANNEL_METER)
                    {
                        //���ӿڽ��յ�645���ģ�ֱ��push������������
                        push_to_meter_frame(Request->frame,Request->frame_len);
                        Request->cur_protocol.value = 0;
                        Request->state = REQUEST_STATE_PROCESSED;
                    }
                    else
                    {
                        //��Э���ʶ
                        Request->cur_protocol.value = 0;
                        Request->cur_protocol.GB645 = 1;
                        Request->recv_time_in_10ms = system_get_tick10ms();
                        Request->state = REQUEST_STATE_WAIT_PROCESS;
                        #ifdef __SOFT_SIMULATOR__
                        debug_println_hex(RequestUart.frame,RequestUart.frame_len,"recv");    
                        #endif  
                    }
                    return 1;
                }
                else
                {
                    Request->cur_protocol.GB645 = 0;
                }
            }
        }
        #else
        if(Request->cur_protocol.GDW1376)
        {
            Request->frame_len = get_1376_1_frame_len(Request->frame,MAX_FRAME_HEADER_SIZE);
            if(Request->recv_len >= Request->frame_len)
            {
                Request->cs  = init_1376_1_frame_checksum(Request->frame,Request->frame_len,Request->recv_len);
                if(is_1376_1_frame_ready(Request->frame,Request->frame_len,Request->recv_len,&(Request->cs)))
                {
                    //��Э���ʶ
                    Request->cur_protocol.value = 0;
                    Request->cur_protocol.GDW1376 = 1;
                    Request->recv_time_in_10ms = system_get_tick10ms();
                    Request->state = REQUEST_STATE_WAIT_PROCESS;
                    Request->recv_success_flag = 1;
#ifdef DEBUG
			system_debug_info("************has receive 376 frame  REQUEST_STATE_WAIT_PROCESS***************");
#endif							
                    return 1;
				
                }
                else
                {
                    Request->cur_protocol.GDW1376 = 0;
                }
            }
        }
        if(Request->cur_protocol.GB645)
        {
            Request->frame_len = get_gb645_frame_len(Request->frame,MAX_FRAME_HEADER_SIZE);
            if(Request->recv_len >= Request->frame_len)
            {
                Request->cs  = init_gb645_frame_checksum(Request->frame,Request->frame_len,Request->recv_len);
                if(is_gb645_frame_ready(Request->frame,Request->frame_len,Request->recv_len,&(Request->cs)))
                {
                    if(Request->channel == PROTOCOL_CHANNEL_METER)
                    {
                        //���ӿڽ��յ�645���ģ�ֱ��push������������
                        push_to_meter_frame(Request->frame,Request->frame_len);
                        Request->cur_protocol.value = 0;
                        Request->state = REQUEST_STATE_PROCESSED;
                    }
                    else
                    {
                        //��Э���ʶ
                        Request->cur_protocol.value = 0;
                        Request->cur_protocol.GB645 = 1;
                        Request->recv_time_in_10ms = system_get_tick10ms();
                        Request->state = REQUEST_STATE_WAIT_PROCESS;
                        #ifdef __SOFT_SIMULATOR__
                        debug_println_hex(RequestUart.frame,RequestUart.frame_len,"recv");    
                        #endif  
                    }
                    return 1;
                }

                else
                {
                    Request->cur_protocol.GB645 = 0;
                }
            }
        }
        #endif
#ifdef __EDP__		
        if(Request->cur_protocol.ONENET)
        {
			Buffer buffer;
			buffer._data = Request->frame+MAX_FRAME_HEADER_SIZE-1;
			buffer._write_pos =(INT32U)Request->recv_len -MAX_FRAME_HEADER_SIZE +1;
            if(IsPkgComplete(&buffer))  //��edp֡��ȫ������
			{
				Request->cur_protocol.value = 0;
				Request->cur_protocol.ONENET = 1;
				Request->recv_time_in_10ms = system_get_tick10ms();
				Request->state = REQUEST_STATE_WAIT_PROCESS;
#ifdef DEBUG
			system_debug_info("\xFF\xFF\xFF\xFF");
			system_debug_data(Request->frame,Request->recv_len);
			system_debug_info("\xFF\xFF\xFF\xFF");
			system_debug_info("************is full edp frame***************");
#endif				
			}
#ifdef DEBUG
				//system_debug_info("\xFF\xFF\xFF\xFF");
				//system_debug_data(Request->frame,Request->recv_len);
				//system_debug_info("\xFF\xFF\xFF\xFF");
				//system_debug_info("************process edp handle***************");	
#endif			
        }
#endif
        if(Request->cur_protocol.value == 0)
        {
            //���ķǷ��ˣ���Ҫ����!
            mem_set(Request->frame,MAX_FRAME_HEADER_SIZE,0);
            Request->state = REQUEST_STATE_IDLE;
        }
        return 1;
    }
    else if(Request->state == REQUEST_STATE_WAIT_PROCESS)
    {
        
    }
    else if(Request->state == REQUEST_STATE_PROCESSING)
    {
        
    }
    else
    {
        mem_set(Request->frame,MAX_FRAME_HEADER_SIZE,0);
        Request->state = REQUEST_STATE_IDLE;
    }
    return 0;
}
void app_run_init(void);
void task_protocol_service(void )
{
    uint8_t has_work;
	
	app_run_init();
    //������һֱ���У�������delay
    RequestRemote.surported_protocol.value = 0;
    RequestRemote.surported_protocol.GDW1376 = 1;
    RequestRemote.surported_protocol.ONENET = 1;
	#ifdef __HEBEI_HUANBAO__
		RequestRemote.surported_protocol.GB645 = 1;		
	#endif
    RequestRemote.channel = PROTOCOL_CHANNEL_REMOTE;

	RequestRemote2.surported_protocol.value = 0;
    RequestRemote2.surported_protocol.GDW1376 = 1;
    RequestRemote2.surported_protocol.ONENET = 1;
	#ifdef __HEBEI_HUANBAO__
		RequestRemote2.surported_protocol.GB645 = 1;
	#endif
    RequestRemote2.channel = PROTOCOL_CHANNEL_REMOTE2;
    
    RequestRemoteServer.surported_protocol.value = 0;
    RequestRemoteServer.surported_protocol.GDW1376 = 1;
    RequestRemoteServer.channel = PROTOCOL_CHANNEL_REMOTE_SERVER;
    
    RequestDebug.surported_protocol.value = 0;
    RequestDebug.surported_protocol.GDW1376 = 1;
	RequestDebug.surported_protocol.GB645 = 1;
    RequestDebug.channel = PROTOCOL_CHANNEL_DEBUG;
    
    RequestUart.surported_protocol.value = 0;
    RequestUart.surported_protocol.GB645 = 1;
    RequestUart.surported_protocol.GDW1376 = 1;
    RequestUart.channel = PROTOCOL_CHANNEL_METER;


	RequestInfra.surported_protocol.value = 0;
    RequestInfra.surported_protocol.GDW1376 = 1;
    RequestInfra.channel = PROTOCOL_CHANNEL_INFRA;

    //app_run_init();
    has_work = 0;
    
    while(1)
    {
        ClrTaskWdt();
        
        has_work = 0;
        has_work += check_recv_process(&RequestUart,meter_uart_read_byte);
        has_work += check_recv_process(&RequestDebug,debug_uart_read_byte);


        #ifndef __SOFT_SIMULATOR__
        has_work += check_recv_process(&RequestInfra,ir_read_byte);
        has_work += check_recv_process(&RequestRemote,remote_read_byte);
		has_work += check_recv_process(&RequestRemote2,remote_read_byte2);
        
        //has_work += check_recv_process(&RequestRemoteServer,remote_server_read_byte);

        bat_charge_service();
        #endif
        if(has_work == 0)
        {
            //������������Ҫ����ʱ���������г���Ҳ�������л�������Ϊϵͳ�����������񶼲���Ҫִ��ʱ����Ĭ��ִ�е�һ������
            tpos_TASK_YIELD();
        }
    }
}

#ifndef __OOP_G_METER__
#include "Q1376_1/ErtuParam.h"

#ifdef __SOFT_SIMULATOR__
uint8_t dev_power_lost(void)
{
    tagDatetime datetime;

    os_get_datetime(&datetime);
    if(datetime.minute%2)
    {
        return 1;
    }
    return 0;
}
#endif

uint8_t event_plmt_power_off_erc14(objResponse *pResp, tagPOWER_ONOFF_RECORD *power_onoff_record, INT8U power_flag)
{
    uint8_t cur_seq,cs;
    uint16_t pos,frameLen,i;
    tagDatetime datetime;
    uint8_t *frame;
    uint8_t tmp_buffer[5];
    uint8_t flag[5];

    frame = pResp->frame;

    if((power_flag!=0x55)&&(power_flag!=0xAA))
    {
        //flag[0]=0xA6;
        //system_debug_data(flag,1);
        return 0;
    }

    os_get_datetime(&datetime);

    frame[POS_CTRL] = CTRLFUNC_SET_DIR | CTRLFUNC_SET_PRM |CTRLFUNC_SET_ACD|PRM_CTRLFUNC_USERDATA;

    frame[POS_AFN] = AFN_READEVENT;
    // PN = P0
    frame[POS_DATA] = 0x00;
    frame[POS_DATA+1] = 0x00;

    //FN
    frame[POS_DATA+2] = DT_F1 & 0xFF;
    frame[POS_DATA+3] = DT_F1 >> 8;

    pos = POS_DATA+4;

    frame[pos++] = 1;
    frame[pos++] = 0;
    frame[pos++] = 0;
    frame[pos++] = 1;

    frame[pos++] = ERC14;    //�¼�����
    frame[pos++] = 11;       //����
    frame[pos++] = 1;        //��Ч��ʶ��Ĭ�ϣ�

    buffer_bin_to_bcd(power_onoff_record->power_off_time+1,tmp_buffer,5);
    mem_cpy(frame+pos,tmp_buffer,5);
    pos += 5;
    if(power_flag == 0x55)
    {
        buffer_bin_to_bcd(power_onoff_record->power_on_time+1,tmp_buffer,5);
        mem_cpy(frame+pos,tmp_buffer,5);
    }
    else
    {
        mem_set(frame+pos,5,0xEE);
    }
    pos += 5;

    //EC
    frame[pos++] = 1;
    frame[pos++] = 0;
    //tp
    frame[pos++] = 0;
    frame[pos++] = byte2BCD(datetime.second);
    frame[pos++] = byte2BCD(datetime.minute);
    frame[pos++] = byte2BCD(datetime.hour);
    frame[pos++] = byte2BCD(datetime.day);
    frame[pos++] = 5;

    //3.4 ���ó���
    pos -= 6;
    pos<<=2;
    pos += QGDW_376_1;

    frame[1] =  pos;
    frame[2] =  pos >> 8;
    frame[3] =  frame[1];
    frame[4] =  frame[2];

    //4 ������Ӧ֡��ţ��������ϱ�����һ��֡��ţ�
    cur_seq = 1;//get_cur_seq();
    frame[POS_SEQ] = 0xF0|cur_seq;


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

    pResp->frame_len = frameLen;
    #ifdef __SOFT_SIMULATOR__
    pResp->channel = PROTOCOL_CHANNEL_DEBUG;
    #else
    pResp->channel = PROTOCOL_CHANNEL_REMOTE;
    #endif
    //flag[0]=0xAA;
    //system_debug_data(flag,1);
    app_send_ReplyFrame(pResp);

    return 1;

}
#endif
#ifndef __OOP_G_METER__
void ertu_poweronoff_service(objResponse *pResp)
{
    uint8_t is_event=0;
    tagDatetime datetime;
    static uint8_t poweroff_hold_time = 0;
    static uint8_t poweron_hold_time = 0;
    static uint8_t last_second=0xFF;
    static uint8_t is_read=1;
    static tagPOWER_ONOFF_RECORD power_onoff_record;
    uint8_t flag[5];
    char info[30];

    os_get_datetime(&datetime);
    if(datetime.second == last_second)
    {
        return;
    }
    else
    {
        last_second = datetime.second;
    }
    
    if(is_read)
    {
        fread_ertu_params(EEADDR_POWEROFF_RECORD,power_onoff_record.value,sizeof(tagPOWER_ONOFF_RECORD));
        is_read = 0;
//        flag[0]=0xC1;
//        system_debug_data(flag,1);
//        system_debug_data(&power_onoff_record.power_flag,1);
    }

    if(power_onoff_record.power_flag == 0xAA)      //����ͣ�������
    {
//        flag[0]=0xA7;
//        system_debug_data(flag,1);
        if(!dev_power_lost())   //����Ƿ��ϵ�
        {
//            flag[0]=0xA8;
//            system_debug_data(flag,1);

//            if(poweron_hold_time == 0)
//            {
//                 mem_cpy(power_onoff_record.power_on_time,datetime.value,6);
//            }
            poweron_hold_time++;
            if((poweron_hold_time>2) && (gSystemInfo.clock_ready != 0))
            {
//                sprintf(info,"REPORT ON:%d %d %d %d %d %d\r\n",datetime.year, datetime.month, datetime.day,datetime.hour, datetime.minute, datetime.second);
//                system_debug_info(info);
                
                //datetime_minus_seconds(&datetime.year, &datetime.month, &datetime.day, &datetime.hour, &datetime.minute, &datetime.second, poweron_hold_time-1);
                 mem_cpy(power_onoff_record.power_on_time,datetime.value,6);
                
//                sprintf(info,"POWER ON:%d %d %d %d %d %d\r\n",datetime.year, datetime.month, datetime.day,datetime.hour, datetime.minute, datetime.second);
//                system_debug_info(info);
                
                power_onoff_record.power_flag = 0x55;  //�����ϵ�
//                flag[0]=0xA9;
//                system_debug_data(flag,1);
            }
            else
            {
                return;
            }

        }
        else
        {
            poweron_hold_time=0;
            return;
        }
    }
    else if(power_onoff_record.power_flag != 0x55)
    {
//        flag[0]=0xA1;
//        system_debug_data(flag,1);
//        system_debug_data(&(power_onoff_record.power_flag),1);
        if(dev_power_lost())   //����Ƿ�ͣ��
        {
//            flag[0]=0xA2;
//            system_debug_data(flag,1);
//            system_debug_data(&poweroff_hold_time,1);
            if(poweroff_hold_time == 0)
            {
                mem_cpy(power_onoff_record.power_off_time,datetime.value,6);
            }
            poweroff_hold_time++;
            if(poweroff_hold_time>0)
            {
//                flag[0]=0xA3;
//                system_debug_data(flag,1);
                power_onoff_record.power_flag = 0xAA;  //����ͣ��
            }
            else
            {
                return;
            }

        }
        else
        {
            poweroff_hold_time=0;
            return;
        }
    }

    poweroff_hold_time=0;
    poweron_hold_time=0;
    power_onoff_record.prop_flag.ok_flag=TRUE;
    power_onoff_record.prop_flag.valid_flag=FALSE;

    gSystemInfo.has_data_report = 0x01; //����Ҫ���Ƕ��¼��������ϱ�  ��λ��λ��ʶ���ϱ�Ҫ��
    is_event = event_plmt_power_off_erc14(pResp,&power_onoff_record,power_onoff_record.power_flag);
    gSystemInfo.has_data_report &= ~(0x01);

    if(power_onoff_record.power_flag == 0x55)
    {
//        flag[0]=0xA4;
//        system_debug_data(flag,1);

        mem_set(power_onoff_record.value,sizeof(tagPOWER_ONOFF_RECORD),0x00);
        fwrite_ertu_params(EEADDR_POWEROFF_RECORD,power_onoff_record.value,sizeof(tagPOWER_ONOFF_RECORD));
        is_read = 1;
    }
    if((is_event)&&(power_onoff_record.power_flag != 0x00))
    {
//                flag[0]=0xA5;
//                system_debug_data(flag,1);

        fwrite_ertu_params(EEADDR_POWEROFF_RECORD,power_onoff_record.value,sizeof(tagPOWER_ONOFF_RECORD));
        is_read = 1;
}
}
#endif
static uint32_t tmr11;
void task_protocol_handler(void )
{
    int idx;
#ifdef __EDP__
	DateTime dt;
	INT8U day_old=0,minute_old =0,already_poweron_3minute =0;
	INT8U report_mask =0;   //�ϱ�״̬���롣
	static INT8U report_loop =0;
	while(!day_old)
	{
		DelayNmSec(10);
		tpos_datetime(&dt);
		day_old =dt.day;
		minute_old =dt.minute;
	}
	
#endif	
    void app_protocol_handler(objRequest* Request,objResponse *Response);
    void push_to_meter_frame(uint8_t *frame,uint16_t frame_len);
    while(1)
    {
        ClrTaskWdt();
        #ifndef __SOFT_SIMULATOR__
#ifndef __PROVINCE_JIANGSU__
        if(gSystemInfo.login_status)
#endif//__PROVINCE_JIANGSU__
        #endif
        {
			#if !defined(__ONENET__) && !defined(__OOP_G_METER__)
            ertu_poweronoff_service(&ResponseApp);
            #endif
        }

        if(RequestRemote.state == REQUEST_STATE_WAIT_PROCESS)
        {
            gSystemInfo.login_status = 1;
            ResponseApp.channel = PROTOCOL_CHANNEL_REMOTE;
            RequestRemote.state = REQUEST_STATE_PROCESSING;
            if(gSystemInfo.remote_app_debug_flag)
            {
                system_debug_data(RequestRemote.frame,RequestRemote.frame_len);
            }

            if(RequestRemote.cur_protocol.GDW1376 )
            {
            app_protocol_handler(&RequestRemote,&ResponseApp);
		
            }
            else if(RequestRemote.cur_protocol.GB645 )
            {
                idx = app_trans_send_meter_frame( RequestRemote.frame, RequestRemote.frame_len, ResponseApp.frame, sizeof( ResponseApp.frame), 300);
                if(idx)
                {
#ifndef __SOFT_SIMULATOR__
                    remote_send_app_frame(ResponseApp.frame, idx);
#endif
                }  
            }
#ifdef __EDP__
			else if(RequestRemote.cur_protocol.ONENET)
			{
				RequestRemote.recv_len-=MAX_FRAME_HEADER_SIZE-1;
				mem_cpy(RequestRemote.frame,RequestRemote.frame+MAX_FRAME_HEADER_SIZE-1,RequestRemote.recv_len);
				app_protocol_handler_edp(&RequestRemote,&ResponseApp);
			}		
#endif
            
            RequestRemote.cur_protocol.value = 0;
            RequestRemote.state = REQUEST_STATE_PROCESSED;
        }
        if(RequestRemote2.state == REQUEST_STATE_WAIT_PROCESS)
        {
            gSystemInfo.login_status2 = 1;
            ResponseApp.channel = PROTOCOL_CHANNEL_REMOTE2;
            RequestRemote2.state = REQUEST_STATE_PROCESSING;
            if(gSystemInfo.remote_app_debug_flag)
            {
                system_debug_data(RequestRemote2.frame,RequestRemote2.frame_len);
            }

            if(RequestRemote2.cur_protocol.GDW1376 )
            {
            app_protocol_handler(&RequestRemote2,&ResponseApp);
            }
            else if(RequestRemote2.cur_protocol.GB645 )
            {
                idx = app_trans_send_meter_frame( RequestRemote2.frame, RequestRemote2.frame_len, ResponseApp.frame, sizeof( ResponseApp.frame), 300);
                if(idx)
                {
#ifndef __SOFT_SIMULATOR__
                    remote_send_app_frame2(ResponseApp.frame, idx);
#endif
                }
            }
            
            RequestRemote2.cur_protocol.value = 0;
            RequestRemote2.state = REQUEST_STATE_PROCESSED;
        }
        if(RequestRemoteServer.state == REQUEST_STATE_WAIT_PROCESS)
        {
            ResponseApp.channel = PROTOCOL_CHANNEL_REMOTE_SERVER;
            RequestRemoteServer.state = REQUEST_STATE_PROCESSING;
            if(gSystemInfo.remote_app_debug_flag)
            {
                system_debug_data(RequestRemoteServer.frame,RequestRemoteServer.frame_len);
            }
            app_protocol_handler(&RequestRemoteServer,&ResponseApp);
            RequestRemoteServer.cur_protocol.value = 0;
            RequestRemoteServer.state = REQUEST_STATE_PROCESSED;
        }
        if(RequestDebug.state == REQUEST_STATE_WAIT_PROCESS)
        {
            ResponseApp.channel = PROTOCOL_CHANNEL_DEBUG;
            RequestDebug.state = REQUEST_STATE_PROCESSING;
            #ifdef __SOFT_SIMULATOR__
                debug_println_hex(RequestDebug.frame,RequestDebug.frame_len,"recv");    
            #endif            
                      
            if(RequestDebug.cur_protocol.GDW1376 )
            {
            app_protocol_handler(&RequestDebug,&ResponseApp);
            }
            else if(RequestDebug.cur_protocol.GB645 )
            {
                idx = app_trans_send_meter_frame( RequestDebug.frame, RequestDebug.frame_len, ResponseApp.frame, sizeof( ResponseApp.frame), 300);
//                if(idx)
//                {
//                    tpos_mutexPend(&SIGNAL_DEBUG_UART);
//                    debug_uart_send_buf(ResponseApp.frame, idx);
//                    tpos_mutexFree(&SIGNAL_DEBUG_UART);
//                }
            }
		
            
            RequestDebug.cur_protocol.value = 0;
            RequestDebug.state = REQUEST_STATE_PROCESSED;
        }
        if(RequestInfra.state == REQUEST_STATE_WAIT_PROCESS)
        {
            RequestInfra.channel = PROTOCOL_CHANNEL_INFRA;
            RequestInfra.state = REQUEST_STATE_PROCESSING;
            #ifdef __SOFT_SIMULATOR__
                debug_println_hex(RequestInfra.frame,RequestInfra.frame_len,"recv");    
            #endif            
            app_protocol_handler(&RequestInfra,&ResponseApp);
            RequestInfra.cur_protocol.value = 0;
            RequestInfra.state = REQUEST_STATE_PROCESSED;
        }
        if(RequestUart.state == REQUEST_STATE_WAIT_PROCESS)
        {
            ResponseApp.channel = PROTOCOL_CHANNEL_METER;
            RequestUart.state = REQUEST_STATE_PROCESSING;
  
            if(RequestUart.cur_protocol.GDW1376)
            {
                app_protocol_handler(&RequestUart,&ResponseApp);
            }
            RequestUart.cur_protocol.value = 0;
            RequestUart.state = REQUEST_STATE_PROCESSED;

        }
        #ifdef __ONENET__
        if(RequestOnenet.state == REQUEST_STATE_WAIT_PROCESS)
        {
            gSystemInfo.login_status = 1;
            RequestOnenet.state = REQUEST_STATE_PROCESSING;
            if(gSystemInfo.remote_app_debug_flag)
            {
                system_debug_data(RequestOnenet.recv_buf,RequestOnenet.recv_pos);
            }

            app_protocol_handler_onenet();   //Ӧ�ò�Э�鴦��
           // RequestOnenet.state = REQUEST_STATE_PROCESSED;        
        }
		#endif

        #ifdef __ONENET__
       // task_data_report_onenet();
        #endif
#ifdef __EDP__
	if(gSystemInfo.update!=0xAA)  //ֻ�в���edp������״̬�Ž���
	{
			tpos_datetime(&dt);
			if((abs(dt.minute -minute_old)>2)&&(!already_poweron_3minute))
			{
				already_poweron_3minute =1;  //ȷ���Ѿ�����2�����ˡ�
			}
			if(already_poweron_3minute)
			{
				if(gSystemInfo.edp_login_state)    //�����������˲����Ѿ���¼�������˿����ϱ���Щ�洢��δ�ϱ�����Ϣ�ˡ�
				{
					gSystemInfo.edp_enable_report_store =1;	
				}
				if(!gSystemInfo.edp_login_state)
				{
					gSystemInfo.edp_enable_report_store = 0;	
				}
				if(dt.day !=day_old)
				{
					report_mask|=0x1<<0;  //�ն���
					report_loop =0;
					day_old =dt.day;
#ifdef DEBUG
				system_debug_info("\r\n***************need to report interval day hold *************\r\n");
#endif				
				}
				if(abs(dt.minute -minute_old) >= gSystemInfo.edp_report_interval)
				{
					report_mask|=0x1<<1;  //ʵʱ����
					report_mask|=0x1<<2;  //�������ݿ�
					minute_old = dt.minute ;
					report_loop=0;
#ifdef DEBUG
				system_debug_info("\r\n***************need to report interval *************\r\n");
#endif 				
				}
			}
			for(report_loop ;report_loop <3;)
			{
				if(!autoReportType)   //�����Ӻ�����Ѿ���¼���Ǿͷ���376.3������,���û�е�¼��վ����ô��Ӧ��ֱ�Ӵ洢��
				{
					switch(report_mask&(0x1<<report_loop))
					{
						case  0x1:
							autoReportType = 11;
							break;
						case 0x02:
							autoReportType = 7;
							break;
						case 0x04:
							autoReportType = 14;
							break;
						default:
							break;
					}
					report_mask &=~(0x01<<report_loop);
				}
				else  //���û�е�¼�ɹ�����ôֱ�ӽ��д洢��
				{
				
				}
				report_loop ++;
				if(report_loop >=3)
				{
					report_loop =0;
				}
				break;
			}

	}
		
#endif	
        tpos_TASK_YIELD();
    }
}
void app_send_ReplyFrame(objResponse *Response)
{
    INT8U head[2]={0xFE,0xFE};
		
	//system_debug_data(Response->frame,Response->frame_len);
    if(Response->channel == PROTOCOL_CHANNEL_REMOTE)
    {
        if(gSystemInfo.remote_app_debug_flag)
        {
            system_debug_data(Response->frame,Response->frame_len);
        }
        #ifndef __SOFT_SIMULATOR__
#ifdef __ONENET__
        if(Response->frame_len *3 < SIZE_OF_APP_PROTOCOL_FRAME)
        {
            hex2str(Response->frame,Response->frame_len,Response->frame+Response->frame_len);
            mem_cpy(Response->frame,Response->frame+Response->frame_len,Response->frame_len*2);
            app_report_onenet_frame((tagWirelessObj*)&RequestOnenet,Response->frame,Response->frame_len*2,1);
        }
#else
        #ifndef __SOFT_SIMULATOR__
        remote_send_app_frame(Response->frame,Response->frame_len);
        #endif
#endif
        #endif
    }
    else if(Response->channel == PROTOCOL_CHANNEL_REMOTE2)
    {
        if(gSystemInfo.remote_app_debug_flag)
        {
            system_debug_data(Response->frame,Response->frame_len);
        }
        #ifndef __SOFT_SIMULATOR__
        remote_send_app_frame2(Response->frame,Response->frame_len);
        #endif
    }
    else if(Response->channel == PROTOCOL_CHANNEL_REMOTE_SERVER)
    {
        if(gSystemInfo.remote_app_debug_flag)
        {
            system_debug_data(Response->frame,Response->frame_len);
        }
        #ifndef __SOFT_SIMULATOR__
        remote_server_send_app_frame(Response->frame,Response->frame_len);
        #endif
    }
    else if(Response->channel == PROTOCOL_CHANNEL_METER)
    {
        tpos_mutexPend(&SIGNAL_METER_UART);
        meter_uart_send_buf(Response->frame,Response->frame_len);
        tpos_mutexFree(&SIGNAL_METER_UART);
    }
    else if(Response->channel == PROTOCOL_CHANNEL_DEBUG)
    {
        tpos_mutexPend(&SIGNAL_DEBUG_UART);
        debug_uart_send_buf(Response->frame,Response->frame_len);
        tpos_mutexFree(&SIGNAL_DEBUG_UART);
    }
#ifndef __SOFT_SIMULATOR__
    else if(Response->channel == PROTOCOL_CHANNEL_INFRA)
    {
        msleep(50);
        tpos_mutexPend(&SIGNAL_INFRA_UART);
		ir_send_buf(head,2);
        ir_send_buf(Response->frame,Response->frame_len);
        tpos_mutexFree(&SIGNAL_INFRA_UART);
    }
#endif
}
