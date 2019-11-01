//---------------------------------------------------------------------------


#pragma hdrstop

#include "Unit_OS.h"

//tagUartObj UartObjGprs,UartObjMeter,UartObjDebug;

//系统时钟

//unsigned char info[200];
TDateTime OS_DATETIME_OFFSET;  //系统时间偏差，用来模拟设备的时钟与系统时钟之差
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
//  把字节数据变成BCD格式
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
  功能:设置时钟
  参数：
        INT8U  fmt01_dt[6]    数据格式01
  返回：

  描述:RTC设置采用BCD码
       地址 00    秒
            01    分
            02    时
            03    周
            04    日
            05    月
            06    年
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

    //更改本地系统时间
    info.sprintf("%04d-%02d-%02d %02d:%02d:%02d",year, month, day,  hour, minute, second);


    TDateTime dt = TDateTime(info);
    OS_DATETIME_OFFSET = dt - TDateTime::CurrentDateTime();

 // ShellExecute(NULL, "open", info.c_str(), params.c_str(), NULL, SW_SHOWNORMAL);



    return TRUE;
}

/*+++
  功能：读取系统的时钟
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
//    //信息类别
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
//        if(DT_F1 == fn)      snprintf(info,100," 确认!");
//        else if(DT_F2 == fn) snprintf(info,100," 否认!");
//        else                 snprintf(info,100," 确认/否认！");
//        break;
//    case DL69842_AFN_INIT:
//        if(DT_F1 == fn)      snprintf(info,100," 初始化01H：F1硬件初始化");
//        else if(DT_F2 == fn) snprintf(info,100," 初始化01H：F2参数区初始化");
//        else if(DT_F3 == fn) snprintf(info,100," 初始化01H：F3数据区初始化");
//        else                 snprintf(info,100," 初始化01H：未知");
//        break;
//    case DL69842_AFN_QUERY:
//        if(DT_F1 == fn)      snprintf(info,100," 查询数据03H：F1厂商及版本信息");
//        else if(DT_F4 == fn) snprintf(info,100," 查询数据03H：F4主节点地址");
//        else if(DT_F5 == fn) snprintf(info,100," 查询数据03H：F5主节点状态字");
//        else if(DT_F9 == fn) snprintf(info,100," 查询数据03H：F9通信延时相关广播通信时长");
//        else if(DT_F10== fn) snprintf(info,100," 查询数据03H：F10本地通信模块运行模式信息");
//        else                 snprintf(info,100," 查询数据03H：未知");
//        break;
//    case DL69842_AFN_CTRL:
//        if(DT_F1 == fn)      snprintf(info,100," 控制命令05H：F1设置主节点地址");
//        else if(DT_F3 == fn) snprintf(info,100," 控制命令05H：F3启动广播");
//        else                 snprintf(info,100," 控制命令05H：未知");
//        break;
//    case DL69842_AFN_REPORT:
//        if(DT_F1 == fn)      snprintf(info,100," 路由上报06H：F1上报载波从节点信息");
//        else if(DT_F2 == fn) snprintf(info,100," 路由上报06H：F2上报抄读数据");
//        else if(DT_F3 == fn) snprintf(info,100," 路由上报06H：F3上报路由工况变动信息");
//        else                 snprintf(info,100," 路由上报06H：未知");
//        break;
//    case DL69842_AFN_RQUERY:
//        if(DT_F1 == fn)      snprintf(info,100," 路由查询10H：F1载波从节点数量");
//        else if(DT_F2 == fn) snprintf(info,100," 路由查询10H：F2载波从节点信息");
//        else if(DT_F3 == fn) snprintf(info,100," 路由查询10H：F3中继信息");
//        else if(DT_F4 == fn) snprintf(info,100," 路由查询10H：F4路由运行状态");
//        else if(DT_F5 == fn) snprintf(info,100," 路由查询10H：F5未抄读成功载波从节点信息");
//        else if(DT_F6 == fn) snprintf(info,100," 路由查询10H：F6主动注册的载波从节点信息");
//        else                 snprintf(info,100," 路由查询10H：未知");
//        break;
//    case DL69842_AFN_RSET:
//        if(DT_F1 == fn)      snprintf(info,100," 路由设置11H：F1添加载波从节点");
//        else if(DT_F2 == fn) snprintf(info,100," 路由设置11H：F2删除载波从节点");
//        else if(DT_F3 == fn) snprintf(info,100," 路由设置11H：F3设置固定中继");
//        else if(DT_F4 == fn) snprintf(info,100," 路由设置11H：F4设置工作模式");
//        else if(DT_F5 == fn) snprintf(info,100," 路由设置11H：F5激活载波从节点主动注册");
//        else if(DT_F6 == fn) snprintf(info,100," 路由设置11H：F6终止载波从节点主动注册");
//        else if(DT_F7 == fn) snprintf(info,100," 路由设置11H：F7节点补抄");
//        else if(DT_F8 == fn) snprintf(info,100," 路由设置11H：F8节点请求队列预告");
//        else if(DT_F9 == fn) snprintf(info,100," 路由设置11H：F9设置附属从节点绑定关系");
//        else                 snprintf(info,100," 路由设置11H：未知");
//        break;
//    case DL69842_AFN_RCTRL:
//        if(DT_F1 == fn)      snprintf(info,100," 路由控制12H：F1重启");
//        else if(DT_F2 == fn) snprintf(info,100," 路由控制12H：F2暂停");
//        else if(DT_F3 == fn) snprintf(info,100," 路由控制12H：F3恢复");
//        else if(DT_F4 == fn) snprintf(info,100," 路由控制12H：F4重新抄表");
//        else if(DT_F5 == fn) snprintf(info,100," 路由控制12H：F5重新组网");
//        else if(DT_F6 == fn) snprintf(info,100," 路由控制12H：F6全部解网");
//        else if(DT_F7 == fn) snprintf(info,100," 路由控制12H：F7解网暨刷新电表");
//        else if(DT_F8 == fn) snprintf(info,100," 路由控制12H：F8采集器刷新电表");
//        else                 snprintf(info,100," 路由控制12H：未知");
//        break;
//    case DL69842_AFN_RTRANS:
//        if(DT_F1 == fn)      snprintf(info,100," 路由监控13H：F1监控载波从节点");
//        else                 snprintf(info,100," 路由监控13H：未知");
//        break;
//    case DL69842_AFN_REQUEST:
//        if(DT_F1 == fn)      snprintf(info,100," 路由抄读14H：F1请求载波从节点抄读内容");
//        else if(DT_F2 == fn) snprintf(info,100," 路由抄读14H：F2路由请求集中器时钟");
//        else if(DT_F3 == fn) snprintf(info,100," 路由抄读14H：F3请求依通信延时修正通信数据");
//        else                 snprintf(info,100," 路由抄读14H：未知");
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
//   得到某月的天数
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
  功能：计算指定日期与1900-1-1比过去的天数
  参数：
        INT16U year,    四位年，>=1900
        INT8U month,    1-12
        INT8U day       1-31
  返回：
        INT16U  相比1900-1-1过去的天数
  描述：
        1）年》2000，直接从2000年开始计算。
---*/
INT16U  getPassedDays(INT16U year,INT8U month,INT8U day)
{
   INT16U days,cur_year;

   days = 0;
   cur_year = 1900;
   if(year >= 2000)
   {
      days += 36525;            // 从1900到1999年的天数
      cur_year = 2000;
   }

   //累计经过的年的天数
   for(;cur_year < year; cur_year++)
   {
      if(TRUE == isLeapYear(cur_year)) days += 366;
      else   days += 365;
   }

   //累加到月的天数
   month &= 0x0F;    //(把月份限制到低字节)
   for(cur_year=1;cur_year<month;cur_year++)
   {
       days += getMonthDays(year,cur_year);
   }

   //累加天数
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