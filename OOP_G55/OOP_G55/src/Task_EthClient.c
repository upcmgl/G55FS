#include "main_include.h"
#include "Stack_Interface.h"
#ifdef STACK_USE_GPRS_PPP
/*++++++++++++++++++++++++++++++++++++++++++++++++++

  ���ܣ� Э��ջʹ�õ���̫��Զ������
      1.����������� PRS��̫����TCP/UDP CLIENT
 +++++++++++++++++++++++++++++++++++++++++++++++++++*/

void ETH_Clent_Idle(void)
{
    #ifdef __PATCH_REC_DH_REPORT__
    report_patch_dayhold();
    #endif
             #ifdef __PRECISE_TIME__
             check_precice_time(3);
             #endif
}
void ETH_Logon_Process(void)
{	
	INT16U frameLen = 0;
   if(is_available_eth())
	{
	
		frameLen = TCP_recv_cmd_frame_eth(cmd_buffer_eth,10000,MAX_SIZE_PLMSDTP);
   }
   #if defined STACK_USE_GENERIC_UDP_CLIENT_EXAMPLE
	else if(is_available_udp_eth())
	{
		frameLen = recv_cmd_frame(read_udp_eth,2000,cmd_buffer_eth, MAX_SIZE_PLMSDTP,2);
   }
   #endif
	//����յ����ݣ���������
	if(frameLen)
	{
		if(g_login_flag==2)//��¼ʱֻҪ�յ����ģ�����Ϊ��¼�ɹ�
		{
			g_login_flag=1;
		}
		if(REMOTE_CLIENT.ETH_STATUS == ETH_CHECK_LOGIN)
		{
		
            g_eth_conn_time=0;
            g_eth_disconn_time=0;
			   REMOTE_CLIENT.ETH_STATUS = ETH_LOGON;
			   #ifndef __USE_DUAL_GPRS_CLIENT__
            g_eth_conn_flag = 1;
            #endif
		}
		//����Զ��Ӧ���Ĵ������ݽṹʹ��Ȩ
		OSMutexPend(&SIGNAL_RESP_ETH);	  
		resp_eth.channel	 = CHANNEL_ETH;
		resp_eth.cmd_flag = 0x00;
		resp_eth.handle = (INT32U) &My_ETH_TCP_Socket; 
		process_CommandFrame(cmd_buffer_eth,&resp_eth);
		REMOTE_CLIENT.eth_idle_start = get_core_time();
        REMOTE_CLIENT.eth_20sec_idle = get_core_time();
        #ifndef __USE_DUAL_GPRS_CLIENT__
 		g_eth_conn_flag = 1; 
 		#endif
		OSMutexFree(&SIGNAL_RESP_ETH);
	}
	else
	{
		   //���ӳɹ������1����δ�յ���վ�·����ģ�����Ϊ����ʧ�ܣ�����Ҫ��������
		  if(REMOTE_CLIENT.ETH_STATUS == ETH_CHECK_LOGIN)
		  {
            if( second_elapsed(REMOTE_CLIENT.eth_idle_start) >= 60 )
            {
                  REMOTE_CLIENT.ETH_STATUS = ETH_SOCKET_CLOSE;
						return ;
            }
         }
         #ifndef __USE_DUAL_GPRS_CLIENT__
		  if(REMOTE_GPRS.req_eth)						 //��������̫��ͨ������
		  {
			  mem_cpy(resp_eth.buffer,resp_remote.buffer,eth_req_sendlen);
			  TCP_send_buffer_eth(resp_eth.buffer,eth_req_sendlen);
			  REMOTE_GPRS.req_eth = 0;
		  }
		  else
		  #endif
		  {

				//��������
				if(REMOTE_CLIENT.heart_cycle == 0 || REMOTE_CLIENT.heart_cycle > 30) REMOTE_CLIENT.heart_cycle = 5;
				if(minute_elapsed(REMOTE_CLIENT.eth_idle_start) >= REMOTE_CLIENT.heart_cycle)
				{
                   #ifndef __USE_DUAL_GPRS_CLIENT__
				   if(1 == g_eth_conn_flag)
				   #endif
				   {
					  //g_eth_conn_flag |= 0x80;
					  fread_ertu_params(EEADDR_HEARTCYCLE,&(REMOTE_CLIENT.heart_cycle),1);	 //��ȡ�������� 	 
					  //����Զ��Ӧ���Ĵ������ݽṹʹ��Ȩ
					  OSMutexPend(&SIGNAL_RESP_ETH);	  
					  resp_eth.channel 	= CHANNEL_ETH;
		 			  resp_eth.channel_idx = 1;
					  send_active_test_frame(&resp_eth,DT_F3);
                      DelayNmSec(500);  
					  OSMutexFree(&SIGNAL_RESP_ETH);
				   }
				}
                else if((second_elapsed(REMOTE_CLIENT.eth_idle_start)>=20) && (second_elapsed(REMOTE_CLIENT.eth_20sec_idle)>=20))
                {
                    ETH_Clent_Idle();
                    REMOTE_CLIENT.eth_20sec_idle=get_core_time();
                }
		  	}
	
	}



}

void SubTask_Eth_Client(void)
{
   INT16U port;
   INT8U ip[4];


	switch(REMOTE_CLIENT.ETH_STATUS)
	{
	   case ETH_NONE:
         //��ͣ��̫������
         break;
      case ETH_SOCKET_OPEN://������վ
         #ifdef __USE_DUAL_GPRS_CLIENT__
         if(!GPRS_PPP_is_ok())
         {
             break;
         }    
        #endif
         //��̫�����ӣ�ʱ����Ʋ���
         if(REMOTE_CLIENT.eth_connect_wait >0 )
         {
            if( second_elapsed(REMOTE_CLIENT.eth_connect_fail_time) >= 60 )
            {
                REMOTE_CLIENT.eth_connect_wait--;  
   				REMOTE_CLIENT.eth_connect_fail_time = get_core_time();
            }
            break;
         }
        #ifndef __REMOTE_ETH_PRIORITY__
        //�����GPRS���ȣ���GPRS����ʱ��̫�������ӣ�GPRS�Ͽ�1���Ӻ���̫��������
        if(REMOTE_GPRS.tcp_ok)
        {
            REMOTE_CLIENT.eth_connect_wait = 1;
            break;
        }
        #endif    
         REMOTE_CLIENT.eth_connect_wait = 0 ;
			if(1==REMOTE_SMS.gprs_mode)//udp
			{
			#if defined STACK_USE_GENERIC_UDP_CLIENT_EXAMPLE
				if(eth_udp_is_closed())
				{
    				#ifdef __USE_DUAL_GPRS_CLIENT__
    				if(gprs_get_server(ip,&port,1))
    				#else
					if(gprs_get_server(ip,&port,REMOTE_CLIENT.eth_server_idx))
					#endif
					{
					eth_udp_set(ip,port,7800);
	 				EX_UDP_Socket_Open(1);//eth udp
    	            } 				
					
	 			}
			#endif
			}
			else		//tcp
			{
				if(eth_tcp_is_closed())
				{
    				#ifdef __USE_DUAL_GPRS_CLIENT__
    				if(gprs_get_server(ip,&port,1))
    				#else
					if(gprs_get_server(ip,&port,REMOTE_CLIENT.eth_server_idx))
					#endif
					{
					EX_TCPSocketSetIPPort(TCP_PURPOSE_ETH_CLIENT,ip,port);
					    EX_TCP_Socket_Open(TCP_PURPOSE_ETH_CLIENT);
					}
					else
					{ 
    					if(TRUE == eth_get_dns_host_name(port))
    					{
					EX_TCP_Socket_Open(TCP_PURPOSE_ETH_CLIENT);
					    }    
					}
					
	 			}
			}
			break;
		case ETH_SOCKET_CLOSE://���Ͽ�TCP��UDP����
			if(1==REMOTE_SMS.gprs_mode)//udp
			{
			#if defined STACK_USE_GENERIC_UDP_CLIENT_EXAMPLE
				eth_udp_close();
			#endif
			}
			else
			{
				EX_TCP_Socket_Close(TCP_PURPOSE_ETH_CLIENT);
			}
         REMOTE_CLIENT.ETH_STATUS = ETH_SOCKET_OPEN;//��������
 			break;
  		case ETH_SOCKET_CONNECTED://GPRS�����ӣ����ģʽ�£�ÿ�β��ź�������Ҫ��¼���������ÿ�����Ӿ���Ҫ��¼
			//���͵�¼����,��¼��ֻҪ�յ���վ�·����κα��ģ�����Ϊ��¼�ɹ�
            OSMutexPend(&SIGNAL_RESP_ETH);
			   resp_eth.channel     = CHANNEL_ETH;
            resp_eth.handle = (INT32U) &My_ETH_TCP_Socket;
            send_active_test_frame(&resp_eth,DT_F1);
            OSMutexFree(&SIGNAL_RESP_ETH);
   			REMOTE_CLIENT.ETH_STATUS = ETH_CHECK_LOGIN;
			   REMOTE_CLIENT.eth_idle_start = get_core_time();
			g_login_flag=2;

			break;
		case ETH_CHECK_LOGIN://����Ƿ��¼
		case ETH_LOGON://�ն��Ѿ���½
			ETH_Logon_Process();
			break;
		default:
			break;
	}//end switch(REMOTE_CLIENT.THE_STATUS)


}
void SubTask_Eth_Server(void)
{
    INT16U  frameLen;

    frameLen=0;
    
    if(eth_server_is_closed())
    {
    
        get_server_port();
        EX_TCPServerSetPort(TCP_PURPOSE_ETH_SERVER,eth_server_port);
        eth_server_tcp_open();
    
    }
    if(is_available_eth_server())
    {
        frameLen = TCP_recv_cmd_frame_eth_server(cmd_buffer_eth,10000,MAX_SIZE_PLMSDTP);
    }
    
    if(frameLen >0)
    {
    
    	OSMutexPend(&SIGNAL_RESP_ETH);	  
    	resp_eth.channel	 = CHANNEL_ETH_SERVER;
        resp_eth.channel_idx =2;
        
        process_CommandFrame(cmd_buffer_eth,&resp_eth);
        unlight_LED_MSA(DIR_SEND);
        OSMutexFree(&SIGNAL_RESP_ETH);
    }
}
void SubTask_Other(void)
{

}
/*++++
 ���ܣ� Э��ջʹ�õ�Զ��ͨ����̫���ͻ�������
 ������
        ��
 ���أ�
        ��
 ������

----*/
void task_eth_client(void)
{
 
   REMOTE_CLIENT.ETH_STATUS  = ETH_SOCKET_OPEN;
   REMOTE_CLIENT.eth_server_idx = 0;
   REMOTE_CLIENT.eth_connect_wait = 0;
	
   g_eth_conn_time = 0;
   g_eth_disconn_time = 0;
   g_eth_conn_flag = 0;


	
   ClrTaskWdt();
   DelayNmSec(20000);

   g_eth_conn_flag = 0;
   fread_ertu_params(EEADDR_HEARTCYCLE,&(REMOTE_CLIENT.heart_cycle),1);   //��ȡ��������
    #ifdef __ENABLE_CLOCK_FIX__
      check_wait_fix_clock();
    #endif
    #ifndef __REMOTE_ETH_PRIORITY__
    //�����GPRS���ȣ����GPRSһ��������ʱ��
        REMOTE_CLIENT.eth_connect_wait = 1;
    #endif

   for(;;)
   {
       ClrTaskWdt();
       SubTask_Eth_Client();

   }

}

//��̫��Զ������
//���������̫���ͻ����������̫������������ʹ��״̬��ʵ��
void task_eth_remote(void)
{
   INT32U remote_start;
   ClrTaskWdt();

   REMOTE_CLIENT.ETH_STATUS  = ETH_SOCKET_OPEN;
   REMOTE_CLIENT.eth_server_idx = 0;
   REMOTE_CLIENT.eth_connect_wait = 0;

   g_eth_conn_time = 0;
   g_eth_disconn_time = 0;
   g_eth_conn_flag = 0;

   DelayNmSec(20000);

   g_eth_conn_flag = 0;
   fread_ertu_params(EEADDR_HEARTCYCLE,&(REMOTE_CLIENT.heart_cycle),1);   //��ȡ��������
    #ifdef __ENABLE_CLOCK_FIX__
      check_wait_fix_clock();
    #endif
    #ifndef __REMOTE_ETH_PRIORITY__
    //�����GPRS���ȣ����GPRSһ��������ʱ��
        REMOTE_CLIENT.eth_connect_wait = 1;
    #endif

    remote_start = get_core_time();


	for(;;)
	{
		ClrTaskWdt();
		SubTask_Eth_Client();
		SubTask_Eth_Server();
        SubTask_Other();
	}


}

void task_udp_report(void)
{
   INT8U ip[4];
   INT16U port;
   INT32U timer;
   
   INT16U pos;
   INT16S rx_data;
   
    #ifdef __ENABLE_CLOCK_FIX__
      check_wait_fix_clock();
    #endif

    //Ҫ��չ��������udp�ϱ���վ��ip��ַ�Ͷ˿ڣ��������ʹ��
    //get_udp_report_param(ip,port);
    ip[0] = 219;
    ip[1] = 147;
    ip[2] = 26;
    ip[3] = 62;
    port = 2014;
    for(;;)
    {
        ClrTaskWdt();
		
        if(!REMOTE_GPRS.ppp_ok)
        {
            DelayNmSec(100);
            continue;
        }
		if(gprs_udp_is_closed())
		{
            //Ҫ��չ��������udp�ϱ���վ��ip��ַ�Ͷ˿ڣ��������ʹ��
            //if(get_udp_report_param(ip,port) == FALSE)
            //{
            //    DelayNmSec(100);
            //    continue;
            //}  
            
            ip[0] = 219;
            ip[1] = 147;
            ip[2] = 26;
            ip[3] = 62;
            port = 2014;  
            
    		gprs_udp_set(ip,port,7801);
			gprs_udp_open();
		}
		else
		{
    	    if(second_elapsed(timer) > 30)
    	    {
        	    timer = get_core_time();
        	    //sprintf(cmd_buffer_eth,"udp report test:%02d-%02d-%02d  %02d:%02d:%02d",datetime[YEAR],datetime[MONTH],datetime[DAY],datetime[HOUR],datetime[MINUTE],datetime[SECOND]);
        	    //gprs_udp_send_buffer(cmd_buffer_eth,35);
        	}
        }      	
	    if(is_available_udp_gprs())
	    {
    	    pos = 0;
    	    while(1)
    	    {
		        rx_data = read_udp_gprs(20);
		        if(rx_data != -1)
		        {
    		        cmd_buffer_eth[pos++] = rx_data;
    		    }
    		    else
    		    {
        		    //�������
        		    break;
        		}
        	}
        	if(pos)
        	{
            	
                //OSMutexPend(&SIGNAL_RS232);
                //rs232_send_buffer(cmd_buffer_eth,pos);  
                //OSMutexFree(&SIGNAL_RS232);
            }   	
        }
   }


}
#endif
