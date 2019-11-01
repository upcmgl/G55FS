#ifndef __VARIABLES_H__
#define __VARIABLES_H__
//RS485抄表
typedef union{
   INT8U  value;
   struct{
      INT8U    pause:1;     //暂停
      INT8U    redo:1;      //重新抄表
      INT8U    work:1;      //抄表中...
      INT8U    doc_chg:1;   //档案有变化。
      INT8U    timer_search_meter_exec_flag:1;  //0:当天没有执行 1：当天已执行
      INT8U    search_meter_flag:1;  //0:没有搜表 1：正在搜表
      INT8U    tmp:3;
   };
}RS485_STATUS;
//当前冻结任务标志
typedef union{
  INT8U value;
  struct{
     INT8U   hold:1;       //周期冻结
     INT8U   day_hold:1;   //日冻结
     INT8U   month_hold:1; //月冻结
     INT8U   former_feeday:1;  //抄读电表上一结算日数据
     INT8U   rec_day:1;    //抄表日冻结标志
     INT8U   only_rec_day:1;  //仅仅启动抄表日冻结数据抄读
     INT8U   patch_rec_day_hold:1;  //补抄日冻结，浙江要求
  };
}RS485_HOLD_FLAG;
typedef union{
    INT8U value;
    struct{
        INT8U is_cast:1;       
        INT8U is_exec:1;
        INT8U tmp:6;
    };
}RS485_PRECISE_TIME_CAST_FALG;
typedef union{
  INT8U value;
  struct{
     INT8U   hold:1;       //485抄表过程中经过了周期冻结
     INT8U   tmp:7;
  };
}RS485_REC_IN_FLAG;
typedef struct{
    INT16U meter_idx;
    INT16U spot_idx;
    INT16U         max_spot_idx;    //最大的485配置序号
    INT16U         idx_in_485;
    INT16U         curve_save_idx;
    INT32U         time_start;
    INT16U         acmeter_idx;     //交采电表的配置序号(用于按分钟周期抄读交采电表的状态量)。
    INT16U         acmeter_spot_idx; //交采电表测量点号
    INT16U         read_dayDH_flag;   //统计日冻结抄读标识 按位表示，对应meter_list  0:未吵到；1：吵到
    INT16U cur_meter_event_grade_idx; //事件等级轮抄  __METER_EVENT_GRADE__
    INT8U          meter_count[3];  //交采口，抄表485口，级联485口，三个口上的电表数量
    INT8U          rec_count[3];

    INT8U  pos_data;
    INT8U           data_len;      //数据长度，减去数据标识
    INT8U           rs485_port;   //当前抄读电表所在485口
    RS485_STATUS   status;
    INT8U  minute;
    INT8U  hour;
    INT8U  day;
    INT8U  month;
    INT8U  year;
    INT8U  weekday;   //星期几

    INT8U  yesterday[3];      //上一日：年-月-日
    INT8U  former_day;
    INT8U  former_month[2];   //上一月  【年-月】

    RS485_HOLD_FLAG hold_flag;
    INT8U           last_minute;
    INT8U           record_cycle;
    INT8U           hold_min;  //冻结分钟值，为了处理抄表周期不是整15的问题。
    BOOLEAN         block_read;
    INT8U           zxygz[5];          //异常事件时所需的正向有功总电能示值
    INT8U           zxwgz[4];
    INT8U           comm_ok_count;
    BOOLEAN         stat_month;
    BOOLEAN         break_work;
    INT8U           RCON;
    INT8U           cy_read_harmonic;
    INT8U           rec_used_time;    //抄表花费时间,单位：分钟
    INT8U           rec_one_used_time;  //抄一个周期的表，花费多长时间,单位：分钟

    INT8U           rec_day_port;    //抄表日冻结，抄表端口: bit0:交采  bit1:抄表485
                                     //为0，表示本日未进行抄表。否则表示本日抄表日数据已经抄读
    INT8U           fl_number;
    INT8U           is_allow_rec;    //F33中是否允许抄表，D0：交采；D1：抄表485；D2；级联485  D3:上海补抄任务 1：有任务
    RS485_REC_IN_FLAG rec_in_flag;   //抄表中使用的标识
    INT8U           resp_no_data_count;//抄表未响应计数器
    INT8U 			cymeter_fail_count;//交采通讯失败次数
    INT8U           port_func;      //口功能
    INT8U esam_meter_list_idx;
    INT8U is_recording;
    INT8U cur_curve_meter_list_idx;
    INT8U dayhold_day;
    RS485_PRECISE_TIME_CAST_FALG precise_time_cast_flag;
}RS485_METER_REC;




//485口抄读结构
 RS485_METER_REC GREC485_port[3];
 
#endif
