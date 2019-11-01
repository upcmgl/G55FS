#include "main_include.h"

typedef union{
    INT8U  value[2];
    struct{
       INT16U   meter_idx:11;
       INT16U   rec_flag:5;
    };
}PATCH_REC_FLAG_SHANGHAI;

//#ifdef __METRE_DOC_INIT_OPTIMIZE__
// //����������ʼ��״̬Ϊ���
//void meter_doc_init_status_clear(INT16U meter_idx)
//{
//	INT8U buffer;
//
//	if((meter_idx==0) || (meter_idx>MAX_METER_COUNT)) return;
//	
//	buffer=0x00;
//	fwrite_array_exec(FILEID_METER_DOC_INIT,meter_idx-1,&buffer,1);
//}
////��ʼ������������
//void meter_doc_init(INT16U meter_idx)
//{
//	INT8U buffer;
//
//	if((meter_idx==0) || (meter_idx>MAX_METER_COUNT)) return;
//
// 	OSMutexPend(&SIGNAL_FLASHSPI);//ռ��FLASH�ź�������ֹ��������������ļ�����Ӧ�������ļ�
//	fread_array_exec(FILEID_METER_DOC_INIT,meter_idx-1,&buffer,1);
//	if(buffer==0xFF)
//	{
//        //ɾ���������ļ�ʵʱ���������Ժ������
//		file_trim(meter_idx,PIM_METER_CUR_DATA);
//
//	    //�����ʱ�ļ�������״̬
//		buffer=0x00;
//		fwrite_array_exec(FILEID_METER_DOC_INIT,meter_idx-1,&buffer,1);
//	}
//  	OSMutexFree(&SIGNAL_FLASHSPI);
//
//
//}
////��ʼ�����в�����
//void meter_doc_init_all(void)
//{
//	INT16U meter_idx;
//	INT8U buffer[2];
//	
//	//1.��������־�Ƿ���
//        read_fmArray(FMADDR_METER_DOC_INIT,buffer,2);
//	if((buffer[0]!=0xAA) || (buffer[1]!=0x55))
//	{
//	  return;
//	}
//	//2.ö��ȫ������
//	for(meter_idx=1;meter_idx<=MAX_METER_COUNT;meter_idx++)
//	{
//		//2.1.�ļ������ڲ����κδ���
//		if(!file_exist(meter_idx))
//		{
//			continue;
//		}
//		//2.2.�����ʱ�ļ����ò������Ƿ��Ѿ���ʼ��,û�г�ʼ�����ʼ��
//		meter_doc_init(meter_idx);
//	}
//
//
//	//3.��������־��ɾ����ʱ�ļ�
//	buffer[0]=0x00;
//	buffer[1]=0x00;
//    write_fmArray(FMADDR_METER_DOC_INIT,buffer,2);
//
//    file_delete_exec(FILEID_METER_DOC_INIT);
//
//	//4.��ԭ����ָ��
//    fwrite_array  = fwrite_array_exec;
//	fread_array   = fread_array_exec;
//	file_delete   = file_delete_exec;

//}
//#endif
/*+++
  ���ܣ�����485�ڳ�������485�˿�2,���ɿڣ�
  ������
        ��
  ���أ�
        ��
  ������
        1�����û�и��������ڣ���Ĭ�ϳ�������Ϊ60���ӡ�
---*/
//Ŀǰ����Ҳ�������洦��
void  task_record_4851meter(void)
{
    INT16U idx,meter_count,finish_count;
    INT16U meter_list[MAX_485METER_COUNT];   //����485���ϣ�ÿ�������32ֻ485���
    INT32U time_start;    //��������ʱ��
    RS485_METER_REC* GREC485;
    INT8U meter_no[7];
    METER_DOCUMENT meter_doc;
    PATCH_REC_FLAG_SHANGHAI rec_flag;
    INT8U test;
    GREC485 =(RS485_METER_REC*) &GREC485_port[COMMPORT_485_CAS-1];
    
    #ifdef __SOFT_SIMULATOR__
        // debug_print("### start task_record_4851meter.....");
    #endif
	//tset

    ClrTaskWdt();
	//������ݳ�ʼ�������Ƿ���ڣ�������ִ��
	#ifdef __METRE_DOC_INIT_OPTIMIZE__
	     //	meter_doc_init_all();   //��������ʱû�д��� TODO����
	#endif

    #ifdef __PROVICE_SHANGHAI__
    read_fmArray(FMADDR_SHANGHAI_VENDOR_FLAG,meter_no,2);
    if(meter_no[0] == 0xFF)
    {
        write_fmArray(FMADDR_SHANGHAI_VENDOR_FLAG,(INT8U*)ertu_cfg_verinfo+12,2);
    }
    #ifdef __PATCH_REC_DH__
    GREC485->is_allow_rec |= 0x08;//�Ϻ�ѭ��������ʶ
    #endif
    #endif

    #ifdef __ENABLE_CLOCK_FIX__
     // fix_clock_from_meter();  //��Ϊģ�鲻���޸����ʱ��
    #endif

//
//#ifndef __GMETER__
//   //��ʼ�����״̬��ͨ����ȡ���״̬ʣ��ʱ��    
//    read_fmArray(FMADDR_ADMETER_NOTE_TIME,&time_start,sizeof(INT32U));
//    if((time_start >0) &&  (time_start<=CYMETER_NOTE_TIME))
//    {
//       display.display_status.b.progState = 1;   //��ǰΪ���״̬
//    }
//   else
//    {
//      display.display_status.b.progState = 0;     //��ǰΪ�Ǳ��״̬    
//    }
//
//    //ǿ��״̬��ͬ�������Խ���ͬ��״̬��
//    display.display_status.b.progNote = !display.display_status.b.progState; 
//#endif
//
    GREC485->cymeter_fail_count = 0;   //������ mgl
    GREC485->last_minute = 0xFF;
 //   GREC485->time_start = CORE_TIMER_TICK;
    GREC485->rec_one_used_time = 0;
    GREC485_port[COMMPORT_485_CY-1].last_minute = 0xFF;  //���ɿ�  mgl
 //   GREC485_port[COMMPORT_485_CY-1].time_start = CORE_TIMER_TICK;//get_core_time();
//    DO_RS485_TASK:
//    GREC485->is_recording = FALSE;
//   // DelayNmSec(1000);
//   // goto DO_RS485_TASK;
//
//
//    //�����Խ��ɵ�����1�ζ�ʱ.
//
//    //���ɳ���ϣ����ʱ5���Ž��ж�ʱ��
//    DelayNmSec(5000);
//    GREC485->break_work = FALSE;
//#ifndef __GMETER__
//    //��齻���Ƿ����
//    check_485_CY_isExist();
//    
//    if((DEVICE_SPEC & DEVICE_HAS_RTC)  && (system_flag & SYS_READ_485_CY))
//    {
//        adj_ad_meter_time();
//    }
//
//    #if defined(__CHECK_POWEROFF_NEW__)
//    DelayNmSec(5000);
//    #endif
//    //���ɱ��״̬
//    //note_cy_prog_state();
//#endif
//    //���������Լ�����齻�ɵ��նˣ����������ͣ��ʱ��
//    no_cy_check_power_on();
//    //
//#ifndef __GMETER__    
//    #ifndef __QGDW_2013_STRUCT__ 
//    //�����ⲿ����485�ڲ�����
//    #ifdef __PROVICE_SHANGHAI__
//    if(system_flag & SYS_READ_485_CY)
//    {
//    set_CY485_BaudRate(getCY485_BaudRate());
//	}
//    #endif
//    #endif
//#endif    
//    REC485_REC_METER:
//   
//    GREC485->status.value = 0x00;
//    GREC485->dayhold_day = datetime[DAY];
//    //��ȡ����485���ϵĵ�����
//    meter_count = update_rec485_meter_seq(meter_list,COMMPORT_485_CAS);   //��ȡ����ڵĵ�����
//    GREC485->hold_flag.patch_rec_day_hold = 1;
//
//    //#ifdef __485_BIT_SEARCH__
//    //����485�ѱ���
//    //start_485_bit_search();
//    //#endif
//    //��������ѭ��
//   
//    for(;;)
//    {
//       ClrTaskWdt();
//          if(update_csq_flag == 0xAA)
//          {
//          	 set_cymeter_csq(REMOTE_GPRS.tcp_ok,REMOTE_GPRS.csq);
//          }
//       //��鵵���Ƿ��б仯
//       if(GREC485->status.doc_chg)
//       {
//          GREC485->status.doc_chg = 0;
//	        meter_count = update_rec485_meter_seq(meter_list,COMMPORT_485_CAS);
//
//            //�����仯ʱ���²���
//            #ifdef __PATCH_REC_DH__
//            #ifdef __PROVICE_SHANGHAI__  //�رղ�����ͨ��һ���������ں��ڴ򿪲�����־
//            set_fmArray(FMADDR_PATCH_REC_485_FLAG+(COMMPORT_485_CAS-1)*2,0xFF,2);
//            GREC485->is_allow_rec |= 0x08;//�Ϻ�ѭ��������ʶ
//            #else
//            set_fmArray(FMADDR_PATCH_REC_485_FLAG+(COMMPORT_485_CAS-1)*2,0xFF,2);
//            #endif
//            #endif
//       }
//
//       //�ȴ����볭������
//	   if(wait_for_485_record_cycle(meter_list,COMMPORT_485_CAS) != 0x00)       //���Ƿ���볭�����ڣ�����0���������� meter_list ��ȡ���ĵ���б�
//       {
//          //note_cy_prog_state();
//          if(update_csq_flag == 0xAA)
//          {
//          	 set_cymeter_csq(REMOTE_GPRS.tcp_ok,REMOTE_GPRS.csq);
//          }
//          continue;
//       }
//       #if defined (__QGDW_2013_STRUCT__)
//       if(comm_485_CAS_to_CY.flag == 0xaa)
//       {
//          if(second_elapsed(comm_485_CAS_to_CY.timer) > 180)
//             comm_485_CAS_to_CY.flag = 0;
//          else
//          {
//             Led_ShowInfor(0,"У��ģʽ");
//             continue ;
//          }
//       }
//       #endif
//       
//       GREC485->is_recording = TRUE;
//       GREC485->dayhold_day = datetime[DAY];
//       
//       //ִ�����ڳ�������
//       #ifdef __SOFT_SIMULATOR__
//       snprintf(info,100,"**** do cycle recording... %02d-%02d-%02d %02d:%02d  hold:%d  day_hold:%d month_hold:%d",
//         GREC485->year,GREC485->month,GREC485->day,GREC485->hour,GREC485->minute,
//         GREC485->hold_flag.hold,GREC485->hold_flag.day_hold,GREC485->hold_flag.month_hold);
//       debug_println(info);
//       snprintf(info,100,"**** rec_day:%d  only_rec_day:%d  rec_day_port=%02X",GREC485->hold_flag.rec_day,
//           GREC485->hold_flag.only_rec_day,GREC485->rec_day_port);
//       debug_println(info);
//       #endif
//
//        #ifdef __PROVICE_SHANGHAI__
//        //if(GREC485->hold_flag.day_hold)
//        {
//            save_shanghai_daylock_info();
//            save_shanghai_month_daylock_info();
//        }
//        #endif
//        #ifndef __POWER_CTRL__
//		#if defined(__CHECK_POWEROFF_NEW__)
//        if(GREC485->hold_flag.day_hold)
//        {
//			save_dayhold_meterpoweroff(GREC485);
//		}
//		#endif
//        #endif
//		
//       finish_count = 0;
//       mem_set(GREC485->rec_count,3,0x00);
//	    update_rs485_rec_state(FALSE,COMMPORT_485_CAS);
//	    update_rs485_rec_state(FALSE,COMMPORT_485_CY);
//#ifndef  __GMETER__
//    	if((system_flag & SYS_READ_485_CY) && (GREC485->cymeter_fail_count>100))
//		{
//			GREC485->cymeter_fail_count  =0;
//			cymeter_reset();//��λ����
//		}
//#endif
//
//           //todo �����ר����Ҫ��ʾС����
//       /* 
//       #ifndef __QGDW_CHECK__
//           #ifndef __PROVICE_CHONGQING__
//           #ifndef __PROVICE_ANHUI__
//           #ifndef __SOFT_SIMULATOR__
//           #ifdef __DISPLAY_IN_CHINESE__
//           Led_ShowInfor(0,"���ڳ���RS4852");
//           #else
//           Led_ShowInfor(0,"Recording RS4852");
//           #endif
//           #endif
//           #endif
//        #endif
//        */
//        GREC485->rec_in_flag.hold = 0;
//
//       //�������ڼ�
//       GREC485->weekday = weekDay(GREC485->year,GREC485->month,GREC485->day);
//       GREC485->break_work = FALSE;
//       GREC485->cy_read_harmonic = FALSE;
//       time_start = get_core_time();
//       check_set_former_day();
//       
//       #ifdef __QGDW_2013_STRUCT__
//       if((GREC485_port[COMMPORT_485_CAS-1].port_func!=PORT_FUNC_CASCADE)&&(GREC485->meter_count[COMMPORT_485_CAS-1]>0) )
//               GREC485_port[COMMPORT_485_CAS-1].port_func=PORT_FUNC_RS485;
//       #endif
//
//       for(idx=0;idx<MAX_485METER_COUNT;idx++)
//       {
//           ClrTaskWdt();
//
//            //��������о�����15���Ӷ���ʱ�䣬����δ���ᣬ����һ�����ڽ��ж���15������
//            if(((datetime[MINUTE] % 15) == 0) && (GREC485->hold_flag.hold == 0))
//            {
//                GREC485->rec_in_flag.hold = 1;
//                GREC485->hold_min = datetime[MINUTE];
//            }
//
//            #ifdef __METER_CURVE_READ__
//            check_curve_midu_task();
//            set_curve_midu_task();
//            #endif
//
//            #ifdef __METER_EVENT_GRADE__
//            check_meter_event_cycle();
//            #endif
//
//           acmeter_rec_status();    //���ɵ����״̬��
//
//           #ifdef __PROVICE_JIANGSU__
//           #ifdef __485_BIT_SEARCH__
//           timer_start_search_meter();  //���ռ�鶨ʱ����485�ѱ�
//           #endif
//           #endif
//
//           if(meter_list[idx] == 0) break;
//
//           GREC485->comm_ok_count = 0;
//            GREC485->resp_no_data_count = 0;
//           //׼���������ȫ�ֱ���GREC485
//           GREC485->meter_idx = meter_list[idx];
//
//
//           //485��������ļ����
//           GREC485->idx_in_485 = flag485_idx(GREC485->meter_idx);
//
//           //�ļ�����Ч�Լ��
//           if(GREC485->idx_in_485 >= MAX_485METER_COUNT) continue;
//#ifndef __GMETER__
//            //�����˽��ɲ�����ȴû�м�⵽����Ӳ���������ڳ���ʱ�ٴμ�⽻��Ӳ������ֹ���ϵ�ʱû�м�⵽����һֱ����������
//            if(((system_flag & SYS_READ_485_CY)==0x00) && (GREC485->meter_idx == GREC485_port[COMMPORT_485_CY-1].acmeter_idx))
//            {
//                check_485_CY_isExist();
//            }
//
//            //û�м�⵽����Ӳ����������������
//           if(((system_flag & SYS_READ_485_CY)==0x00) && (GREC485->meter_idx == GREC485_port[COMMPORT_485_CY-1].acmeter_idx))  continue;
//#endif
//            //��ȡ�����
//            fread_array(GREC485->meter_idx,PIM_METER_DOC,(INT8U *) &meter_doc, sizeof(METER_DOCUMENT));
//            //F33�������˲������Զ�����
//            if((meter_doc.baud_port.port == COMMPORT_485_CY) && (GREC485->is_allow_rec & 0x01)) continue;
//            if((meter_doc.baud_port.port == COMMPORT_485_CAS) && (GREC485->is_allow_rec & 0x02)) continue;
//
//            #if defined __PROVICE_SHANGHAI__ && !defined __POWER_CTRL__
//            //�Ϻ�Ҫ���С�������Ϊ��0 2��6 2��������485������
//            //�������͵�485�����ز�ģʽ������һ�쳭��һ�Σ�ֻ�������򡢷����й���������
//            //�ص��������Ҫ����F35���ó��� 
//            if(meter_doc.meter_class.value != 0x02 && meter_doc.meter_class.value != 0x62) continue;
//            #endif
//
//            #ifdef __POWER_CTRL__
//            check_pulse_meter(&meter_doc,COMMPORT_485_CAS);
//            #endif
//            
//            
//            
//           //����485���
//            if(do_rs485_meter_recording(&meter_doc) == FALSE)  //���ݱ������������ڵĶ�����mgl
//            {
//                finish_count++;
//                goto UPDATE_STATE;
//            }
//           //����Ƿ���Ҫ�жϳ������̡�
//           if(GREC485->break_work) break;
//
//           //��������ǳ����ն���������������������������
//            if(GREC485->hold_flag.only_rec_day) 
//               goto UPDATE_STATE;
//
//           //�ն������ݼ��
//	        do_spot_stat_day_hold(&meter_doc,COMMPORT_485_CAS);
//
//           //�¶������ݼ��
//	        do_spot_stat_month_hold(&meter_doc,COMMPORT_485_CAS);
//
//           finish_count++;
//
//           //���485����ʧ��
//	        check_rs485_rec_fail_event(COMMPORT_485_CAS);
//UPDATE_STATE:
//	        update_rs485_rec_state(FALSE,meter_doc.baud_port.port);
//       }
//       #ifdef __QGDW_2013_STRUCT__
//       if(GREC485_port[COMMPORT_485_CAS-1].port_func!=PORT_FUNC_CASCADE)
//       {
//		   if(GREC485_port[COMMPORT_485_CAS-1].port_func==PORT_FUNC_RS485)
//		   {
//           GREC485_port[COMMPORT_485_CAS-1].port_func=PORT_FUNC_CYMETER;
//
//           InitUART_cas485(0,BAUDRATE_CAS_TO_CY,UART_PARITY_EVEN,240,120);
//		   }
//
//       }
//       #endif
//        
//       GREC485->meter_idx = MAX_METER_COUNT;
//
//       if(!GREC485->break_work)
//       {
//        //����һ�����ڵĳ���ʹ��ʱ��
//        time_start = second_elapsed(time_start);
//        GREC485->rec_one_used_time = time_start / 60;
//        if((time_start % 60) > 0) GREC485->rec_one_used_time++;
//	   }
//
//        #ifdef __SHANGHAI_CLASS_53_54_485_CURVE__
//        for(idx=1;idx<=MAX_METER_COUNT;idx++)
//        {
//            if (!GREC485->hold_flag.hold) break;
//            if (file_exist(idx) == FALSE) continue;
//            //��ȡ�����
//            fread_array(idx,PIM_METER_DOC,(INT8U *) &meter_doc,sizeof(METER_DOCUMENT));
//            
//            if (((meter_doc.meter_class.value == 0x53) || (meter_doc.meter_class.value == 0x54)) 
//            && (meter_doc.baud_port.port == COMMPORT_485_CAS))
//            {
//                rs485_read_class_53_54_curve(&meter_doc);
//            }
//        }
//        #endif
//	
//
//        #ifdef __PRECISE_TIME__
//        if (GREC485_port[COMMPORT_485_CAS-1].precise_time_cast_flag.is_cast)
//        {
//            exec_485_adj_time_port(1);
//            GREC485_port[COMMPORT_485_CAS-1].precise_time_cast_flag.is_cast = 0;
//            GREC485_port[COMMPORT_485_CAS-1].precise_time_cast_flag.is_exec = 1;
//        }
//        #endif
//
//		/*
//		//ÿ����֤һ�Σ����ɴ����Ƿ���ȷ
//		if(GREC485->hold_flag.day_hold)
//		{
//			check_cy_code_crc();		
//		}
//		*/
//    /*
//   #ifndef __QGDW_CHECK__
//       #ifndef __PROVICE_CHONGQING__
//       #ifndef __PROVICE_ANHUI__
//       #ifndef __SOFT_SIMULATOR__
//       #ifdef __DISPLAY_IN_CHINESE__
//       Led_ShowInfor(0,"��������RS4852");
//       #else
//       Led_ShowInfor(0,"Recording RS4852 End");
//       #endif
//       #endif
//       #endif
//   #endif
//    */
//        #ifdef __PATCH_REC_DH__
//        #ifdef __PROVICE_SHANGHAI__
//        //�Ϻ�Ҫ��ÿ���������ں󣬶�����һ�β���
//        read_fmArray(FMADDR_PATCH_REC_485_FLAG+(COMMPORT_485_CAS-1)*2,rec_flag.value,2);
//        if((rec_flag.meter_idx > MAX_METER_COUNT) && (GREC485->is_allow_rec & 0x08))    //�����ѽ��������
//        {
//            set_fmArray(FMADDR_PATCH_REC_485_FLAG+(COMMPORT_485_CAS-1)*2,0x00,2);
//        }
//        #else
//        read_fmArray(FMADDR_PATCH_REC_485_FLAG+(COMMPORT_485_CAS-1)*2,rec_flag.value,2);
//        if(rec_flag.value[0] == 0xFF && rec_flag.value[1] == 0xFF)
//        {
//            set_fmArray(FMADDR_PATCH_REC_485_FLAG+(COMMPORT_485_CAS-1)*2,0x00,2);
//        }
//        #endif  //#ifdef __PATCH_REC_DH__
//        #endif  //#ifdef __PROVICE_SHANGHAI__
//
//       
//       if(!GREC485->break_work)
//       {
//          if(GREC485->hold_flag.day_hold)
//          {
//              while(GREC485_port[COMMPORT_485_REC-1].dayhold_day != datetime[DAY])
//              {
//                  DelayNmSec(20);
//              }  
//              while((GREC485_port[COMMPORT_485_REC-1].is_recording == TRUE)  
//                   && (GREC485_port[COMMPORT_485_REC-1].hold_flag.day_hold))
//              {
//                 DelayNmSec(20);
//              }  
//          //���������ܼ�������
//	      compute_addgroup_data(COMMPORT_485_CAS,0);  //�ܼ���
//	      if(GREC485->hold_flag.day_hold)
//	      GREC485->hold_flag.day_hold = 0;
//       }
//          else
//          {
//              if(GREC485->record_cycle < GREC485_port[COMMPORT_485_REC-1].record_cycle)
//    	         compute_addgroup_data(COMMPORT_485_CAS,0);
//    	      else if(GREC485->record_cycle == GREC485_port[COMMPORT_485_REC-1].record_cycle)
//    	      {
//        	      /*
//                  while((GREC485_port[COMMPORT_485_REC-1].is_recording == TRUE))
//                  {
//                     DelayNmSec(20);
//                  }
//                  */
//    	         compute_addgroup_data(COMMPORT_485_CAS,0);
//          }
//       }
//       }
//       
//       GREC485->is_recording = FALSE;
//	   update_rs485_rec_state(TRUE,COMMPORT_485_CAS);
//	   update_rs485_rec_state(TRUE,COMMPORT_485_CY);
// 
//      //���㳭����ʱ��
//      if(!GREC485->hold_flag.only_rec_day)
//      {
//       GREC485->rec_used_time = minute_elapsed(GREC485->time_start);
//      }
//    }
//
//    #ifdef __SOFT_SIMULATOR__
//    debug_print("### end task_record_4851meter.....");
//    #endif
//
//    //����ִ�г���485������.
//    goto REC485_REC_METER;
}
