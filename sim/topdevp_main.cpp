//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Unit_OS.h"
#include "Unit_FLASH.h"
#include "topdevp_main.h"
#include "About.h"

//#include  "main_include.h"
#include "Q1376_1/app_3761.h"


#include "Unit_readPortService.h"
#include "Unit_exComm.h"
#include "Unit_ProtocolHander1.h"
#include "Unit_vfm_memory.h"
unsigned char info[6000];
CRITICAL_SECTION cs;

objSYSTEM  SYSTEM;
OS_TCB * volatile pxCurrentTCB;

#include "dev_var.c"
#include "tops_fun_mem.c"
#include "tops_fun_str.c"
#include "tops_fun_date.c"
#include "tops_fun_tools.c"
#include "tops_fun_convert.c"
#include "tops_fun_common.c"


//#include "tpos/tpos_service.c"
//#include "tpos/os_tpos.c"
#include "os_vfm_fat.c"
#include "os_flash_vfm.c"
#include "OS_FAT14.c"
#include "dev_file.c"

#include "Q1376_1/protocol_app_handler.c"
#include "Q1376_1/plms_read_c1.c"
#include "Q1376_1/plms_read_c2.c"
#include "Q1376_1/app_fun_3761.c"
#include "Q1376_1/app_var_3761.c"
#include "Q1376_1/ErtuParam.c"
#include "Q1376_1/plms_cmd_relay.c"
#include "Q1376_1/readMeter_RS485.c"
//#include "HJ212/HJ212.c"

#include "frame_recv_service.c"

#include "task_protocol_service.c"
#include "task_meter.c"
//#include "task_nb_onenet.c"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TTOPDEVP_MainForm *TOPDEVP_MainForm;
//---------------------------------------------------------------------------

__fastcall TTOPDEVP_MainForm::TTOPDEVP_MainForm(TComponent *Owner)
	: TForm(Owner)
{
  sim_open_file_flash();
  vfm_init(false,NULL);
  static int hello;
 INT32U fmAddr =0;
 INT8U data_write[4096*2];
 INT8U data_read[4096*2];

   fat14_init();
   unsigned char file_no;
   static  char i=0;
   for( file_no=0;file_no<200;file_no++)
   {
     mem_set(data_write,4096,file_no+12);
       for(int file_part=0;file_part<4;file_part++)
       {
          fwrite_array(file_no,4096*file_part,data_write,4096);
       }
   }
   for( file_no=0;file_no<200;file_no++)
   {
       //em_set(data_write,4096,file_no);
       for(int file_part=0;file_part<4;file_part++)
       {
          fread_array(file_no,4096*file_part,data_read,4096);
           if(mem_all_is(data_read,4096,file_no+12)==FALSE)
           {
             i++;
           }
       }

   }

     SYSTEM.tpos_running = 1;
//    getDateTime();
	InitializeCriticalSection(&cs);
//    xList_REQUEST_Init();
//    xList_MEM300_Init();
//    init_time_service();
    SYSTEM.systick_in_10ms=GetTickCount() / 10;
    SYSTEM.tpos_running = 1;
}
//---------------------------------------------------------------------------
void __fastcall TTOPDEVP_MainForm::CreateMDIChild(String Name)
{

}
//---------------------------------------------------------------------------
void __fastcall TTOPDEVP_MainForm::HelpAbout1Execute(TObject *Sender)
{
	AboutBox->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TTOPDEVP_MainForm::FileExit1Execute(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TTOPDEVP_MainForm::num(TObject *Sender)
{
    String str;
//    getDateTime();
//    str.printf("%02d-%02d-%02d %02d:%02d:%02d",datetime[YEAR],datetime[MONTH],datetime[DAY],datetime[HOUR],datetime[MINUTE],datetime[SECOND]);
//    StatusBar->Panels->Items[0]->Text = str;
    static INT32U start_tick=GetTickCount()/10;
    SYSTEM.systick_in_10ms = GetTickCount()/10-start_tick;
}
//---------------------------------------------------------------------------
void __fastcall TTOPDEVP_MainForm::FormShow(TObject *Sender)
{
//    Form_ndfat->WindowState=wsMinimized;
//    if(Task_systemService!=NULL)
//    {
//        Task_systemService->BitBtn_StartClick(NULL);
//        Task_systemService->WindowState=wsMinimized;
//    }
    if(Task_ProtocolHandler1!=NULL)
    {
        Task_ProtocolHandler1->BitBtn_StartClick(NULL);
        Task_ProtocolHandler1->WindowState=wsMinimized;
    }
//    if(TaskCycleService!=NULL)
//    {
//        TaskCycleService->BitBtn_StartClick(NULL);
//        TaskCycleService->WindowState=wsMinimized;
//    }
    if(Task_exComm!=NULL)
    {
        Task_exComm->BitBtn_StartClick(NULL);
    }
//     //set_system_flag(APP_POWER_ONOFF_CY,APP_FLAG);
//    //config_log_record_param();
//    //app_run_init();
}
//---------------------------------------------------------------------------
void __fastcall TTOPDEVP_MainForm::act_DisplayTaskExecute(TObject *Sender)
{
//    TaskDisplay = new TTaskDisplay(this);
//    TaskDisplay->Show();
}
//---------------------------------------------------------------------------
void __fastcall TTOPDEVP_MainForm::act_ReadPortTaskExecute(TObject *Sender)
{
    Task_readPortService = new TTask_readPortService(this);
    Task_readPortService->Show();
}
//---------------------------------------------------------------------------
void __fastcall TTOPDEVP_MainForm::act_SystemServiceTaskExecute(
      TObject *Sender)
{
//    if(Task_systemService==NULL)
//    {
//        Task_systemService = new TTask_systemService(this);
//    }
//    Task_systemService->WindowState= wsNormal;
}
//---------------------------------------------------------------------------
void __fastcall TTOPDEVP_MainForm::act_ProtocolServiceTaskExecute(
      TObject *Sender)
{
    Task_exComm = new TTask_exComm(this);
    Task_exComm->Show();
}
//---------------------------------------------------------------------------
void __fastcall TTOPDEVP_MainForm::act_ProtocolHandlerTaskExecute(
      TObject *Sender)
{
    if(Task_ProtocolHandler1==NULL)
    {
        Task_ProtocolHandler1 = new TTask_ProtocolHandler1(this);
    }
    Task_ProtocolHandler1->WindowState= wsNormal;
}
//---------------------------------------------------------------------------
void __fastcall TTOPDEVP_MainForm::act_CycleServiceTaskExecute(
      TObject *Sender)
{
//    TaskCycleService = new TTaskCycleService(this);
//    TaskCycleService->Show();
}
//---------------------------------------------------------------------------
void __fastcall TTOPDEVP_MainForm::ItemFileToolsClick(TObject *Sender)
{
    //Form_ndfat = new TForm_ndfat(this);
//    Form_ndfat->WindowState=wsNormal;
}
//---------------------------------------------------------------------------
void __fastcall TTOPDEVP_MainForm::ItemFaskIndexToolsClick(TObject *Sender)
{
//    Form_DataViewer = new TForm_DataViewer(this);
//    Form_DataViewer->Show();
}
//---------------------------------------------------------------------------
void __fastcall TTOPDEVP_MainForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
     sim_close_file_flash();
//    //写掉电时间/流量统计、运行时间等
//    mem_cpy(g_app_run_data.cur_time,datetime,6);
//    fwrite_array(FILEID_POWEROFF,FLADDR_PLMT_CUR_DATE_TIME,(INT8U*)&g_app_run_data,sizeof(tagAPP_RUN_DATA));
}
//---------------------------------------------------------------------------

void __fastcall TTOPDEVP_MainForm::act_ReportTaskExecute(TObject *Sender)
{
//    TaskReport = new TTaskReport(this);
//    TaskReport->Show();
}
//---------------------------------------------------------------------------

void __fastcall TTOPDEVP_MainForm::act_PoweronoffTaskExecute(
      TObject *Sender)
{
//    TaskPowerOnoff = new TTaskPowerOnoff(this);
//    TaskPowerOnoff->Show();
}
//---------------------------------------------------------------------------

//TDateTime OS_DATETIME_OFFSET;  //系统时间偏差，用来模拟设备的时钟与系统时钟之差

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
    return TRUE;
}


uint8_t os_get_datetime(tagDatetime *datetime)
{
    void    mem_cpy(void *dst,void *src,uint16_t len);
    uint8_t  mem_is_same(void *dst,void * src,uint16_t len);
    tagDatetime datetime1,datetime2;
    uint8_t try_count;

    unsigned short year,month,day;
    unsigned short hour,minute,second,msec;
    TDateTime  dt;

    try_count = 0;
    #ifdef __SOFT_SIMULATOR__

    dt = TDateTime::CurrentDateTime();
    dt += OS_DATETIME_OFFSET;
    dt.DecodeDate(&year,&month,&day);
    dt.DecodeTime(&hour,&minute,&second,&msec);

    datetime->second = second;
    datetime->minute = minute;
    datetime->hour   = hour;

    datetime->day   = day;
    datetime->month = month;
    datetime->year  = year-2000;
    datetime->century  = year/100;
    datetime->msecond_h  = msec>>8;
    datetime->msecond_l = msec & 0xFF;
    #else
    while(1)
    {
        mem_cpy(datetime1.value,(uint8_t*)os_datetime.value,sizeof(tagDatetime));
        mem_cpy(datetime2.value,(uint8_t*)os_datetime.value,sizeof(tagDatetime));
        if(mem_is_same(datetime1.value,datetime2.value,sizeof(tagDatetime)))
        {
            mem_cpy(datetime->value,datetime1.value,sizeof(tagDatetime));
            return 1;
        }
        if(try_count++ > 10)
        {
            break;
        }
    }
    return 0;
    #endif
}
uint8_t os_set_datetime(tagDatetime *datetime)
{
    return 0;
}
void  tpos_mutexPend(objMutex *pMutex)
{
     if(tpos_mutexGet(pMutex,0) == FALSE)
     {
        tpos_softReset();
     }
}

/*+++
 * ??? ?????
 * ???
 *      1????????????????????????????????????
 ---*/
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

void tpos_sleep(INT32U sleep_time)
{
    INT32U  wait_tick,tick_start;

    wait_tick = sleep_time / 10 + 1;

    if(SYSTEM.tpos_running)
    {
        //?????????10ms????????
        tpos_enterCriticalSection();
        //OSTCBCur->delay_tick = wait_tick;
        //OSTCBCur->task_dog = 0;
        tpos_TASK_YIELD();
        tpos_leaveCriticalSection();
    }
    else
    {
       //???????????????
       tick_start = system_get_tick10ms();
       while(system_get_tick10ms() - tick_start < wait_tick)
       {
           //TODO:  ??
       }
    }
}

void system_debug_info(char* info)

{

}
void system_debug_data(uint8_t* data,uint16_t data_len)
{

}
void DelayNmSec(INT32U delay)
{

}
INT16U read_c1_f14(INT8U *buffer)
{}
void remote_send_app_frame(uint8_t *frame,uint16_t frame_len)
{}
void remote_send_app_frame2(uint8_t *frame,uint16_t frame_len)
{}

