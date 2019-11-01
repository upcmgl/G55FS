#ifndef __VARIABLES_H__
#define __VARIABLES_H__
//RS485����
typedef union{
   INT8U  value;
   struct{
      INT8U    pause:1;     //��ͣ
      INT8U    redo:1;      //���³���
      INT8U    work:1;      //������...
      INT8U    doc_chg:1;   //�����б仯��
      INT8U    timer_search_meter_exec_flag:1;  //0:����û��ִ�� 1��������ִ��
      INT8U    search_meter_flag:1;  //0:û���ѱ� 1�������ѱ�
      INT8U    tmp:3;
   };
}RS485_STATUS;
//��ǰ���������־
typedef union{
  INT8U value;
  struct{
     INT8U   hold:1;       //���ڶ���
     INT8U   day_hold:1;   //�ն���
     INT8U   month_hold:1; //�¶���
     INT8U   former_feeday:1;  //���������һ����������
     INT8U   rec_day:1;    //�����ն����־
     INT8U   only_rec_day:1;  //�������������ն������ݳ���
     INT8U   patch_rec_day_hold:1;  //�����ն��ᣬ�㽭Ҫ��
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
     INT8U   hold:1;       //485��������о��������ڶ���
     INT8U   tmp:7;
  };
}RS485_REC_IN_FLAG;
typedef struct{
    INT16U meter_idx;
    INT16U spot_idx;
    INT16U         max_spot_idx;    //����485�������
    INT16U         idx_in_485;
    INT16U         curve_save_idx;
    INT32U         time_start;
    INT16U         acmeter_idx;     //���ɵ����������(���ڰ��������ڳ������ɵ���״̬��)��
    INT16U         acmeter_spot_idx; //���ɵ��������
    INT16U         read_dayDH_flag;   //ͳ���ն��᳭����ʶ ��λ��ʾ����Ӧmeter_list  0:δ������1������
    INT16U cur_meter_event_grade_idx; //�¼��ȼ��ֳ�  __METER_EVENT_GRADE__
    INT8U          meter_count[3];  //���ɿڣ�����485�ڣ�����485�ڣ��������ϵĵ������
    INT8U          rec_count[3];

    INT8U  pos_data;
    INT8U           data_len;      //���ݳ��ȣ���ȥ���ݱ�ʶ
    INT8U           rs485_port;   //��ǰ�����������485��
    RS485_STATUS   status;
    INT8U  minute;
    INT8U  hour;
    INT8U  day;
    INT8U  month;
    INT8U  year;
    INT8U  weekday;   //���ڼ�

    INT8U  yesterday[3];      //��һ�գ���-��-��
    INT8U  former_day;
    INT8U  former_month[2];   //��һ��  ����-�¡�

    RS485_HOLD_FLAG hold_flag;
    INT8U           last_minute;
    INT8U           record_cycle;
    INT8U           hold_min;  //�������ֵ��Ϊ�˴��������ڲ�����15�����⡣
    BOOLEAN         block_read;
    INT8U           zxygz[5];          //�쳣�¼�ʱ����������й��ܵ���ʾֵ
    INT8U           zxwgz[4];
    INT8U           comm_ok_count;
    BOOLEAN         stat_month;
    BOOLEAN         break_work;
    INT8U           RCON;
    INT8U           cy_read_harmonic;
    INT8U           rec_used_time;    //������ʱ��,��λ������
    INT8U           rec_one_used_time;  //��һ�����ڵı����Ѷ೤ʱ��,��λ������

    INT8U           rec_day_port;    //�����ն��ᣬ����˿�: bit0:����  bit1:����485
                                     //Ϊ0����ʾ����δ���г��������ʾ���ճ����������Ѿ�����
    INT8U           fl_number;
    INT8U           is_allow_rec;    //F33���Ƿ�������D0�����ɣ�D1������485��D2������485  D3:�Ϻ��������� 1��������
    RS485_REC_IN_FLAG rec_in_flag;   //������ʹ�õı�ʶ
    INT8U           resp_no_data_count;//����δ��Ӧ������
    INT8U 			cymeter_fail_count;//����ͨѶʧ�ܴ���
    INT8U           port_func;      //�ڹ���
    INT8U esam_meter_list_idx;
    INT8U is_recording;
    INT8U cur_curve_meter_list_idx;
    INT8U dayhold_day;
    RS485_PRECISE_TIME_CAST_FALG precise_time_cast_flag;
}RS485_METER_REC;




//485�ڳ����ṹ
 RS485_METER_REC GREC485_port[3];
 
#endif
