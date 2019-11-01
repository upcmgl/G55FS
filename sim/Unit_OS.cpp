//---------------------------------------------------------------------------


#pragma hdrstop

#include "Unit_OS.h"

//tagUartObj UartObjGprs,UartObjMeter,UartObjDebug;

//ϵͳʱ��

//unsigned char info[200];
TDateTime OS_DATETIME_OFFSET;  //ϵͳʱ��ƫ�����ģ���豸��ʱ����ϵͳʱ��֮��
//---------------------------------------------------------------------------
//char HEX[]="0123456789ABCDEF";
extern char HEX[];
#pragma package(smart_init)

extern CRITICAL_SECTION cs;
INT32U os_get_systick_10ms(void)
{
    INT32U  tick10ms;
    tick10ms = SYSTEM.systick_in_10ms;
    return  tick10ms;

}
INT32U  system_get_tick10ms(void)
{
    return os_get_systick_10ms();
}


INT32U time_elapsed_10ms(INT32U start_time)
{
    return os_get_systick_10ms() - start_time;
}


void tpos_enterCriticalSection(void)
{
    EnterCriticalSection(&cs);
}

void tpos_leaveCriticalSection(void)
{
    LeaveCriticalSection(&cs);
}

void tpos_clrTaskWdt(void)
{
}
void ClrTaskWdt(void)
{
}

void tpos_TASK_YIELD(void)
{
  Sleep(1);
}


//
//  ���ֽ����ݱ��BCD��ʽ
//
unsigned char   byte2BCD(unsigned char bVal)
{
  return  ( (bVal/10)<<4) + (bVal%10);
}

unsigned char    BCD2byte(unsigned char bcd)
{
   return  (bcd>>4)*10 + (bcd&0x0F);
}


/*+++
  ����:����ʱ��
  ������
        INT8U  fmt01_dt[6]    ���ݸ�ʽ01
  ���أ�

  ����:RTC���ò���BCD��
       ��ַ 00    ��
            01    ��
            02    ʱ
            03    ��
            04    ��
            05    ��
            06    ��
---*/

unsigned char set_clock(unsigned char  fmt01_dt[7])
{
    unsigned char month,day,hour,minute,second;
    unsigned char result;
    int idx;
    INT16U year;
    AnsiString info,params;

    //ClrWdt();

    second = (fmt01_dt[0]);
    minute = (fmt01_dt[1]);
    hour = (fmt01_dt[2]);
    day = (fmt01_dt[3]);
    month = (fmt01_dt[4]);
    year = 2000+(fmt01_dt[5]);

    //���ı���ϵͳʱ��
    info.sprintf("%04d-%02d-%02d %02d:%02d:%02d",year, month, day,  hour, minute, second);


    TDateTime dt = TDateTime(info);
    OS_DATETIME_OFFSET = dt - TDateTime::CurrentDateTime();

 // ShellExecute(NULL, "open", info.c_str(), params.c_str(), NULL, SW_SHOWNORMAL);



    return TRUE;
}

/*+++
  ���ܣ���ȡϵͳ��ʱ��
---*/
//void getDateTime(void)
//{
//    unsigned short year,month,day;
//    unsigned short hour,minute,second,msec;
//    TDateTime  dt;
//
//    dt = TDateTime::CurrentDateTime();
//        dt += OS_DATETIME_OFFSET;
//    dt.DecodeDate(&year,&month,&day);
//    dt.DecodeTime(&hour,&minute,&second,&msec);
//
//    datetime[SECOND] = second;
//    datetime[MINUTE] = minute;
//    datetime[HOUR]   = hour;
//
//    datetime[DAY]   = day;
//    datetime[MONTH] = month;
//    datetime[YEAR]  = year-2000;
//    datetime[CENTURY]  = year/100;
//    datetime[MSECOND_H]  = msec>>8;
//    datetime[MSECOND_L]  = msec & 0xFF;
//}

void GetHexMessage(BYTE *frame,int FrameLen,char* msg)
{
    for(int i=0;i<FrameLen;i++)
    {
       msg[i*2+0] =   HEX[  (frame[i] >> 4) & 0x0F];
       msg[i*2+1] =   HEX[  frame[i] & 0x0F];
    }
    msg[FrameLen*2]=0;
    //return AnsiString(msg);
}
//---------------------------------------------------------------------------
//void frameInfo(unsigned char *frame,unsigned char* info)
//{
//    INT16U afn,fn,pos;
//
//    //DL698.42
//    DL69842_RRR RRR;
//
//    mem_cpy(RRR.value,frame + POS_DL69842_INFO,sizeof(DL69842_RRR));
//
//    //��Ϣ���
//    pos = POS_DL69842_ADDR;
//    if(RRR.resp_relay_channel.modem)
//    {
//        pos +=12;
//    }
//
//    afn = frame[pos++];
//    fn = frame[pos++];
//    fn += frame[pos++]<<8;
//
//    switch(afn)
//    {
//    case DL69842_AFN_CONFIRM:
//        if(DT_F1 == fn)      snprintf(info,100," ȷ��!");
//        else if(DT_F2 == fn) snprintf(info,100," ����!");
//        else                 snprintf(info,100," ȷ��/���ϣ�");
//        break;
//    case DL69842_AFN_INIT:
//        if(DT_F1 == fn)      snprintf(info,100," ��ʼ��01H��F1Ӳ����ʼ��");
//        else if(DT_F2 == fn) snprintf(info,100," ��ʼ��01H��F2��������ʼ��");
//        else if(DT_F3 == fn) snprintf(info,100," ��ʼ��01H��F3��������ʼ��");
//        else                 snprintf(info,100," ��ʼ��01H��δ֪");
//        break;
//    case DL69842_AFN_QUERY:
//        if(DT_F1 == fn)      snprintf(info,100," ��ѯ����03H��F1���̼��汾��Ϣ");
//        else if(DT_F4 == fn) snprintf(info,100," ��ѯ����03H��F4���ڵ��ַ");
//        else if(DT_F5 == fn) snprintf(info,100," ��ѯ����03H��F5���ڵ�״̬��");
//        else if(DT_F9 == fn) snprintf(info,100," ��ѯ����03H��F9ͨ����ʱ��ع㲥ͨ��ʱ��");
//        else if(DT_F10== fn) snprintf(info,100," ��ѯ����03H��F10����ͨ��ģ������ģʽ��Ϣ");
//        else                 snprintf(info,100," ��ѯ����03H��δ֪");
//        break;
//    case DL69842_AFN_CTRL:
//        if(DT_F1 == fn)      snprintf(info,100," ��������05H��F1�������ڵ��ַ");
//        else if(DT_F3 == fn) snprintf(info,100," ��������05H��F3�����㲥");
//        else                 snprintf(info,100," ��������05H��δ֪");
//        break;
//    case DL69842_AFN_REPORT:
//        if(DT_F1 == fn)      snprintf(info,100," ·���ϱ�06H��F1�ϱ��ز��ӽڵ���Ϣ");
//        else if(DT_F2 == fn) snprintf(info,100," ·���ϱ�06H��F2�ϱ���������");
//        else if(DT_F3 == fn) snprintf(info,100," ·���ϱ�06H��F3�ϱ�·�ɹ����䶯��Ϣ");
//        else                 snprintf(info,100," ·���ϱ�06H��δ֪");
//        break;
//    case DL69842_AFN_RQUERY:
//        if(DT_F1 == fn)      snprintf(info,100," ·�ɲ�ѯ10H��F1�ز��ӽڵ�����");
//        else if(DT_F2 == fn) snprintf(info,100," ·�ɲ�ѯ10H��F2�ز��ӽڵ���Ϣ");
//        else if(DT_F3 == fn) snprintf(info,100," ·�ɲ�ѯ10H��F3�м���Ϣ");
//        else if(DT_F4 == fn) snprintf(info,100," ·�ɲ�ѯ10H��F4·������״̬");
//        else if(DT_F5 == fn) snprintf(info,100," ·�ɲ�ѯ10H��F5δ�����ɹ��ز��ӽڵ���Ϣ");
//        else if(DT_F6 == fn) snprintf(info,100," ·�ɲ�ѯ10H��F6����ע����ز��ӽڵ���Ϣ");
//        else                 snprintf(info,100," ·�ɲ�ѯ10H��δ֪");
//        break;
//    case DL69842_AFN_RSET:
//        if(DT_F1 == fn)      snprintf(info,100," ·������11H��F1����ز��ӽڵ�");
//        else if(DT_F2 == fn) snprintf(info,100," ·������11H��F2ɾ���ز��ӽڵ�");
//        else if(DT_F3 == fn) snprintf(info,100," ·������11H��F3���ù̶��м�");
//        else if(DT_F4 == fn) snprintf(info,100," ·������11H��F4���ù���ģʽ");
//        else if(DT_F5 == fn) snprintf(info,100," ·������11H��F5�����ز��ӽڵ�����ע��");
//        else if(DT_F6 == fn) snprintf(info,100," ·������11H��F6��ֹ�ز��ӽڵ�����ע��");
//        else if(DT_F7 == fn) snprintf(info,100," ·������11H��F7�ڵ㲹��");
//        else if(DT_F8 == fn) snprintf(info,100," ·������11H��F8�ڵ��������Ԥ��");
//        else if(DT_F9 == fn) snprintf(info,100," ·������11H��F9���ø����ӽڵ�󶨹�ϵ");
//        else                 snprintf(info,100," ·������11H��δ֪");
//        break;
//    case DL69842_AFN_RCTRL:
//        if(DT_F1 == fn)      snprintf(info,100," ·�ɿ���12H��F1����");
//        else if(DT_F2 == fn) snprintf(info,100," ·�ɿ���12H��F2��ͣ");
//        else if(DT_F3 == fn) snprintf(info,100," ·�ɿ���12H��F3�ָ�");
//        else if(DT_F4 == fn) snprintf(info,100," ·�ɿ���12H��F4���³���");
//        else if(DT_F5 == fn) snprintf(info,100," ·�ɿ���12H��F5��������");
//        else if(DT_F6 == fn) snprintf(info,100," ·�ɿ���12H��F6ȫ������");
//        else if(DT_F7 == fn) snprintf(info,100," ·�ɿ���12H��F7������ˢ�µ��");
//        else if(DT_F8 == fn) snprintf(info,100," ·�ɿ���12H��F8�ɼ���ˢ�µ��");
//        else                 snprintf(info,100," ·�ɿ���12H��δ֪");
//        break;
//    case DL69842_AFN_RTRANS:
//        if(DT_F1 == fn)      snprintf(info,100," ·�ɼ��13H��F1����ز��ӽڵ�");
//        else                 snprintf(info,100," ·�ɼ��13H��δ֪");
//        break;
//    case DL69842_AFN_REQUEST:
//        if(DT_F1 == fn)      snprintf(info,100," ·�ɳ���14H��F1�����ز��ӽڵ㳭������");
//        else if(DT_F2 == fn) snprintf(info,100," ·�ɳ���14H��F2·����������ʱ��");
//        else if(DT_F3 == fn) snprintf(info,100," ·�ɳ���14H��F3������ͨ����ʱ����ͨ������");
//        else                 snprintf(info,100," ·�ɳ���14H��δ֪");
//        break;
//    }
//
//    //return "";
//}
//---------------------------------------------------------------------------

BOOLEAN    isLeapYear(INT16U year)
{
    BOOLEAN res;

    res = FALSE;
    if(0 ==  (year%4))
    {
         if(year%100!=0) res = TRUE;

         if(0 == (year % 400)) res = TRUE;
    }
    return res;
}

//
//   �õ�ĳ�µ�����
// 
INT8U getMonthDays(INT16U year,INT8U month)
{
    if(month==2)
    {
       if(TRUE == isLeapYear(year)) return 29;
       return 28;
    }    
    if((month==4)  || (month==6)  || (month==9)  ||   (month==11))
    {
       return 30;
    } 
    return 31;
}


/*+++
  ���ܣ�����ָ��������1900-1-1�ȹ�ȥ������
  ������
        INT16U year,    ��λ�꣬>=1900
        INT8U month,    1-12
        INT8U day       1-31
  ���أ�
        INT16U  ���1900-1-1��ȥ������
  ������
        1���꡷2000��ֱ�Ӵ�2000�꿪ʼ���㡣
---*/
INT16U  getPassedDays(INT16U year,INT8U month,INT8U day)
{
   INT16U days,cur_year;

   days = 0;
   cur_year = 1900;
   if(year >= 2000)
   {
      days += 36525;            // ��1900��1999�������
      cur_year = 2000;
   }

   //�ۼƾ������������
   for(;cur_year < year; cur_year++)
   {
      if(TRUE == isLeapYear(cur_year)) days += 366;
      else   days += 365;
   }

   //�ۼӵ��µ�����
   month &= 0x0F;    //(���·����Ƶ����ֽ�)
   for(cur_year=1;cur_year<month;cur_year++)
   {
       days += getMonthDays(year,cur_year);
   }

   //�ۼ�����
   days += (day-1);

   return days;
}
void toggle_LED_ERR(void)
{

}
BOOLEAN pin_power_lost(void)
{

    return FALSE;
}
void pin_battery_power(INT8U pin_level)
{

}

//INT16U esam_get_esam_id(INT8U *buffer,INT16U max_len)
//{
//    return 0;
//}
INT16U esam_get_offline_counter(INT8U *buffer,INT16U max_len)
{
    return 0;
}
//INT16U esam_get_chip_state(INT8U *buffer,INT16U max_len)
//{
//    return 0;
//}
INT16U esam_get_key_version(INT8U *buffer,INT16U max_len)
{
    return 0;
}
INT16U esam_get_cert_id(INT8U *buffer,INT16U max_len)
{
    return 0;
}
INT16U esam_get_public_key(INT8U *buffer,INT16U max_len)
{
    return 0;
}
INT16U esam_session_init(INT8U *data,INT8U *buffer,INT16U max_len)
{
    return 0;
}
INT16U esam_session_key_consult(INT8U *data,INT8U *buffer,INT16U max_len)
{
    return 0;
}
bool esam_symmetric_key_update(INT8U *data,INT8U *buffer,INT16U max_len)
{
    return FALSE;
}
bool esam_ca_certificate_update(INT8U *data,INT8U *buffer,INT16U max_len)
{
    return FALSE;
}
INT16U esam_internal_auth(INT8U *data,INT8U *buffer,INT16U max_len)
{
    return 0;
}
bool esam_external_auth(INT8U *data,INT8U *buffer,INT16U max_len)
{
    return FALSE;
}
INT16U esam_get_random_num(INT8U randonlen,INT8U *buffer,INT16U max_len)
{
    return 0;
}
bool esam_cert_state_change(INT8U *data,INT8U *buffer,INT16U max_len)
{
    return FALSE;
}
bool esam_set_offline_counter(INT8U *data,INT8U *buffer,INT16U max_len)
{
    return FALSE;
}
bool esam_change_data_authorize(INT8U *data,INT8U *buffer,INT16U max_len)
{
    return FALSE;
}
bool esam_verify_mac2(INT8U kid,INT8U *data,INT16U datalen,INT8U *buffer,INT16U max_len,INT8U mac[4])
{
    return FALSE;
}
bool esam_group_broadcast(INT8U kid1,INT8U kid2,INT16U group_id,INT8U *data,INT16U datalen,INT8U *buffer,INT16U max_len,INT8U mac[4])
{
    return FALSE;
}
INT16U esam_identity_authentication(INT8U P2,INT8U *data,INT8U datalen,INT8U *buffer,INT16U max_len)
{
    return 0;
}
INT16U esam_set_time(INT8U cur_datatime[7],INT8U task_data[40],INT8U meter_er[32],INT8U meter_rdn[4],INT8U *buffer,INT16U max_len)
{
    return 0;
}
void esam_get_info(void)
{
    return ;
}
//void __attribute__(unsigned char xx)
//{
//
//}
//unsigned char optimize(char *info)
//{
//
//
//}
void drv_esam_spi_WriteArray(INT8U *buffer,INT16U len)
{


}
INT16U drv_esam_spi_ReadArray(INT8U *buffer,INT16U max_len,INT8U * headdata,INT16U *err_code)
{


    return 0;
}
void rs232_debug_info(INT8U *buffer,INT16U len)
{
        GetHexMessage(buffer,len,info);
        debug_println(info);
}

void unlight_LED_ERR(void)
{


}
void force_gprs_power_off(void)
{

}


void app_light_remote_rx_led(void)
{

}
void app_light_remote_tx_led(void)
{

}
void update_cy_adjust_param(void)
{

}
void output_led_test_status(void)
{


}
void tops_platform_set_eth_update_param(void)
{

}
void  ertu_set_net_mode(unsigned char data)
{

}unsigned short esam_get_esam_id(unsigned char *buffer,unsigned short max_len)
{
    return 0;
}
unsigned short esam_get_chip_state(unsigned char *buffer,unsigned short max_len)
{

    return 0;
}
unsigned short tesam_get_esam_id(unsigned char *buffer,unsigned short max_len)
{
    return 0;
}
unsigned short tesam_get_chip_state(unsigned char *buffer,unsigned short max_len)
{

    return 0;
}
void drv_esam_spi_high_Initialize(void)
{
}
void  tpos_mutexPend(objMutex *pMutex)
{
     if(tpos_mutexGet(pMutex,0) == FALSE)
     {
        tpos_softReset();
     }
}
void tpos_mutexFree(objMutex *pMutex)
{
   if(!SYSTEM.tpos_running) return;

   tpos_enterCriticalSection();

   //???????1
   if(pMutex->e.signal) pMutex->e.signal--;
   if(pMutex->e.signal == 0)
   {
       //?????????????????
       pMutex->e.taskNo = 0;

       //??????????????????????
       if(pMutex->e.waitFlag)
       {
            tpos_TASK_YIELD();
       }
   }
   tpos_leaveCriticalSection();
}
INT8U tpos_mutexGet(objMutex *pMutex, INT32U timeout_10ms)
{
    volatile OS_TCB  *pTCB;

    if(!SYSTEM.tpos_running) return TRUE;

    while(1)
    {
       tpos_enterCriticalSection();
       //????
       if(0 == pMutex->e.signal)
       {
          //????

          pMutex->e.signal ++;
          pMutex->e.taskNo = get_cur_task_no();
          pMutex->e.waitFlag = 0;
          #ifndef __SOFT_SIMULATOR__
          OSTCBCur->task_dog = 0;
          #endif
          tpos_leaveCriticalSection();
          return TRUE;
       }

       //???,?????????
       if(pMutex->e.taskNo  == get_cur_task_no() )
       {
          pMutex->e.signal ++;    //?????
          tpos_leaveCriticalSection();
          return TRUE;
       }
       //???? ??????????????
       #ifndef __SOFT_SIMULATOR__
       OSTCBCur->task_dog = 0;
       OSTCBCur->wait_mutex = pMutex;
       #endif
       pMutex->e.waitFlag = 1;
       //??????
       tpos_TASK_YIELD();
       //??????????
       #ifndef __SOFT_SIMULATOR__
       pTCB = OSTCBCur;
       #endif
       tpos_TASK_YIELD();
       tpos_leaveCriticalSection();

       #ifndef __SOFT_SIMULATOR__
       if(pMutex->e.signal > 0)
       {
           //??????? ?????????
           if(timeout_10ms > 0)
           {
              //?????????????
              if(pTCB->task_dog  > timeout_10ms)
              {
                  return FALSE;
              }
           }
       }
       #endif
    } // endof while(1)
    return FALSE;
}
void  tpos_softReset(void)
{
}