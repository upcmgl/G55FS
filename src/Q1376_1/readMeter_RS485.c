#include "main_include.h"

typedef union{
    INT8U  value[2];
    struct{
       INT16U   meter_idx:11;
       INT16U   rec_flag:5;
    };
}PATCH_REC_FLAG_SHANGHAI;

//#ifdef __METRE_DOC_INIT_OPTIMIZE__
// //清除测量点初始化状态为完成
//void meter_doc_init_status_clear(INT16U meter_idx)
//{
//	INT8U buffer;
//
//	if((meter_idx==0) || (meter_idx>MAX_METER_COUNT)) return;
//	
//	buffer=0x00;
//	fwrite_array_exec(FILEID_METER_DOC_INIT,meter_idx-1,&buffer,1);
//}
////初始化单个测量点
//void meter_doc_init(INT16U meter_idx)
//{
//	INT8U buffer;
//
//	if((meter_idx==0) || (meter_idx>MAX_METER_COUNT)) return;
//
// 	OSMutexPend(&SIGNAL_FLASHSPI);//占用FLASH信号量，防止其它任务操作该文件或相应测量点文件
//	fread_array_exec(FILEID_METER_DOC_INIT,meter_idx-1,&buffer,1);
//	if(buffer==0xFF)
//	{
//        //删除测量点文件实时数据区及以后的数据
//		file_trim(meter_idx,PIM_METER_CUR_DATA);
//
//	    //清除临时文件测量点状态
//		buffer=0x00;
//		fwrite_array_exec(FILEID_METER_DOC_INIT,meter_idx-1,&buffer,1);
//	}
//  	OSMutexFree(&SIGNAL_FLASHSPI);
//
//
//}
////初始化所有测量点
//void meter_doc_init_all(void)
//{
//	INT16U meter_idx;
//	INT8U buffer[2];
//	
//	//1.检查事务标志是否开启
//        read_fmArray(FMADDR_METER_DOC_INIT,buffer,2);
//	if((buffer[0]!=0xAA) || (buffer[1]!=0x55))
//	{
//	  return;
//	}
//	//2.枚举全部档案
//	for(meter_idx=1;meter_idx<=MAX_METER_COUNT;meter_idx++)
//	{
//		//2.1.文件不存在不做任何处理
//		if(!file_exist(meter_idx))
//		{
//			continue;
//		}
//		//2.2.检查临时文件，该测量点是否已经初始化,没有初始化则初始化
//		meter_doc_init(meter_idx);
//	}
//
//
//	//3.清除事务标志、删除临时文件
//	buffer[0]=0x00;
//	buffer[1]=0x00;
//    write_fmArray(FMADDR_METER_DOC_INIT,buffer,2);
//
//    file_delete_exec(FILEID_METER_DOC_INIT);
//
//	//4.还原函数指针
//    fwrite_array  = fwrite_array_exec;
//	fread_array   = fread_array_exec;
//	file_delete   = file_delete_exec;

//}
//#endif
/*+++
  功能：抄表485口抄表任务（485端口2,交采口）
  参数：
        无
  返回：
        无
  描述：
        1）如果没有给抄表周期，则默认抄表周期为60分钟。
---*/
//目前交采也在这里面处理
void  task_record_4851meter(void)
{
    INT16U idx,meter_count,finish_count;
    INT16U meter_list[MAX_485METER_COUNT];   //抄表485口上，每个口最多32只485电表
    INT32U time_start;    //启动抄表时间
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
	//检查数据初始化事务是否存在，存在则执行
	#ifdef __METRE_DOC_INIT_OPTIMIZE__
	     //	meter_doc_init_all();   //在上面暂时没有处理 TODO？？
	#endif

    #ifdef __PROVICE_SHANGHAI__
    read_fmArray(FMADDR_SHANGHAI_VENDOR_FLAG,meter_no,2);
    if(meter_no[0] == 0xFF)
    {
        write_fmArray(FMADDR_SHANGHAI_VENDOR_FLAG,(INT8U*)ertu_cfg_verinfo+12,2);
    }
    #ifdef __PATCH_REC_DH__
    GREC485->is_allow_rec |= 0x08;//上海循环补抄标识
    #endif
    #endif

    #ifdef __ENABLE_CLOCK_FIX__
     // fix_clock_from_meter();  //认为模块不能修复电表时间
    #endif

//
//#ifndef __GMETER__
//   //初始化编程状态，通过读取编程状态剩余时间    
//    read_fmArray(FMADDR_ADMETER_NOTE_TIME,&time_start,sizeof(INT32U));
//    if((time_start >0) &&  (time_start<=CYMETER_NOTE_TIME))
//    {
//       display.display_status.b.progState = 1;   //当前为编程状态
//    }
//   else
//    {
//      display.display_status.b.progState = 0;     //当前为非编程状态    
//    }
//
//    //强制状态不同，引发对交采同步状态。
//    display.display_status.b.progNote = !display.display_status.b.progState; 
//#endif
//
    GREC485->cymeter_fail_count = 0;   //级联口 mgl
    GREC485->last_minute = 0xFF;
 //   GREC485->time_start = CORE_TIMER_TICK;
    GREC485->rec_one_used_time = 0;
    GREC485_port[COMMPORT_485_CY-1].last_minute = 0xFF;  //交采口  mgl
 //   GREC485_port[COMMPORT_485_CY-1].time_start = CORE_TIMER_TICK;//get_core_time();
//    DO_RS485_TASK:
//    GREC485->is_recording = FALSE;
//   // DelayNmSec(1000);
//   // goto DO_RS485_TASK;
//
//
//    //开机对交采电表进行1次对时.
//
//    //交采程序希望延时5秒后才进行对时。
//    DelayNmSec(5000);
//    GREC485->break_work = FALSE;
//#ifndef __GMETER__
//    //检查交采是否存在
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
//    //交采编程状态
//    //note_cy_prog_state();
//#endif
//    //不带交采以及不检查交采的终端，在这里更新停电时间
//    no_cy_check_power_on();
//    //
//#ifndef __GMETER__    
//    #ifndef __QGDW_2013_STRUCT__ 
//    //设置外部交采485口波特率
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
//    //提取抄表485口上的电表序号
//    meter_count = update_rec485_meter_seq(meter_list,COMMPORT_485_CAS);   //提取抄表口的电表序号
//    GREC485->hold_flag.patch_rec_day_hold = 1;
//
//    //#ifdef __485_BIT_SEARCH__
//    //设置485搜表功能
//    //start_485_bit_search();
//    //#endif
//    //抄表任务循环
//   
//    for(;;)
//    {
//       ClrTaskWdt();
//          if(update_csq_flag == 0xAA)
//          {
//          	 set_cymeter_csq(REMOTE_GPRS.tcp_ok,REMOTE_GPRS.csq);
//          }
//       //检查档案是否有变化
//       if(GREC485->status.doc_chg)
//       {
//          GREC485->status.doc_chg = 0;
//	        meter_count = update_rec485_meter_seq(meter_list,COMMPORT_485_CAS);
//
//            //档案变化时重新补抄
//            #ifdef __PATCH_REC_DH__
//            #ifdef __PROVICE_SHANGHAI__  //关闭补抄，通过一个抄表周期后在打开补抄标志
//            set_fmArray(FMADDR_PATCH_REC_485_FLAG+(COMMPORT_485_CAS-1)*2,0xFF,2);
//            GREC485->is_allow_rec |= 0x08;//上海循环补抄标识
//            #else
//            set_fmArray(FMADDR_PATCH_REC_485_FLAG+(COMMPORT_485_CAS-1)*2,0xFF,2);
//            #endif
//            #endif
//       }
//
//       //等待进入抄表周期
//	   if(wait_for_485_record_cycle(meter_list,COMMPORT_485_CAS) != 0x00)       //看是否进入抄表周期，返回0即进入周期 meter_list 提取出的电表列表
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
//             Led_ShowInfor(0,"校表模式");
//             continue ;
//          }
//       }
//       #endif
//       
//       GREC485->is_recording = TRUE;
//       GREC485->dayhold_day = datetime[DAY];
//       
//       //执行周期抄表任务
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
//			cymeter_reset();//复位交采
//		}
//#endif
//
//           //todo 如果是专变需要显示小字体
//       /* 
//       #ifndef __QGDW_CHECK__
//           #ifndef __PROVICE_CHONGQING__
//           #ifndef __PROVICE_ANHUI__
//           #ifndef __SOFT_SIMULATOR__
//           #ifdef __DISPLAY_IN_CHINESE__
//           Led_ShowInfor(0,"正在抄表RS4852");
//           #else
//           Led_ShowInfor(0,"Recording RS4852");
//           #endif
//           #endif
//           #endif
//        #endif
//        */
//        GREC485->rec_in_flag.hold = 0;
//
//       //计算星期几
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
//            //抄表过程中经过了15分钟冻结时间，但是未冻结，在下一个周期进行冻结15分曲线
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
//           acmeter_rec_status();    //交采电表抄读状态字
//
//           #ifdef __PROVICE_JIANGSU__
//           #ifdef __485_BIT_SEARCH__
//           timer_start_search_meter();  //江苏检查定时启动485搜表
//           #endif
//           #endif
//
//           if(meter_list[idx] == 0) break;
//
//           GREC485->comm_ok_count = 0;
//            GREC485->resp_no_data_count = 0;
//           //准备电表抄读的全局变量GREC485
//           GREC485->meter_idx = meter_list[idx];
//
//
//           //485电表数据文件序号
//           GREC485->idx_in_485 = flag485_idx(GREC485->meter_idx);
//
//           //文件号有效性检查
//           if(GREC485->idx_in_485 >= MAX_485METER_COUNT) continue;
//#ifndef __GMETER__
//            //配置了交采测量点却没有检测到交采硬件，在周期抄表时再次检测交采硬件，防止在上电时没有检测到导致一直不抄读交采
//            if(((system_flag & SYS_READ_485_CY)==0x00) && (GREC485->meter_idx == GREC485_port[COMMPORT_485_CY-1].acmeter_idx))
//            {
//                check_485_CY_isExist();
//            }
//
//            //没有检测到交采硬件不抄读交采数据
//           if(((system_flag & SYS_READ_485_CY)==0x00) && (GREC485->meter_idx == GREC485_port[COMMPORT_485_CY-1].acmeter_idx))  continue;
//#endif
//            //读取电表档案
//            fread_array(GREC485->meter_idx,PIM_METER_DOC,(INT8U *) &meter_doc, sizeof(METER_DOCUMENT));
//            //F33中设置了不允许自动抄表
//            if((meter_doc.baud_port.port == COMMPORT_485_CY) && (GREC485->is_allow_rec & 0x01)) continue;
//            if((meter_doc.baud_port.port == COMMPORT_485_CAS) && (GREC485->is_allow_rec & 0x02)) continue;
//
//            #if defined __PROVICE_SHANGHAI__ && !defined __POWER_CTRL__
//            //上海要求大小类号配置为：0 2和6 2按照正常485抄读，
//            //其他类型的485按照载波模式抄读，一天抄读一次，只抄读正向、反向有功冻结数据
//            //重点表曲线需要按照F35配置抄读 
//            if(meter_doc.meter_class.value != 0x02 && meter_doc.meter_class.value != 0x62) continue;
//            #endif
//
//            #ifdef __POWER_CTRL__
//            check_pulse_meter(&meter_doc,COMMPORT_485_CAS);
//            #endif
//            
//            
//            
//           //抄读485电表
//            if(do_rs485_meter_recording(&meter_doc) == FALSE)  //根据表档案来进行周期的读数。mgl
//            {
//                finish_count++;
//                goto UPDATE_STATE;
//            }
//           //检查是否需要中断抄表流程。
//           if(GREC485->break_work) break;
//
//           //如果仅仅是抄表日冻结任务，则不做后续冻结数据任务。
//            if(GREC485->hold_flag.only_rec_day) 
//               goto UPDATE_STATE;
//
//           //日冻结数据检查
//	        do_spot_stat_day_hold(&meter_doc,COMMPORT_485_CAS);
//
//           //月冻结数据检查
//	        do_spot_stat_month_hold(&meter_doc,COMMPORT_485_CAS);
//
//           finish_count++;
//
//           //检查485抄表失败
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
//        //计算一个周期的抄表使用时间
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
//            //读取电表档案
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
//		//每日验证一次，交采代码是否正确
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
//       Led_ShowInfor(0,"结束抄表RS4852");
//       #else
//       Led_ShowInfor(0,"Recording RS4852 End");
//       #endif
//       #endif
//       #endif
//   #endif
//    */
//        #ifdef __PATCH_REC_DH__
//        #ifdef __PROVICE_SHANGHAI__
//        //上海要求每个抄表周期后，都进行一次补抄
//        read_fmArray(FMADDR_PATCH_REC_485_FLAG+(COMMPORT_485_CAS-1)*2,rec_flag.value,2);
//        if((rec_flag.meter_idx > MAX_METER_COUNT) && (GREC485->is_allow_rec & 0x08))    //当天已将补抄完成
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
//          //计算生成总加组数据
//	      compute_addgroup_data(COMMPORT_485_CAS,0);  //总加组
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
//      //计算抄表花费时间
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
//    //重新执行抄表485口任务.
//    goto REC485_REC_METER;
}
