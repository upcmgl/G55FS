//---------------------------------------------------------------------------
//  1） 使用外部定义的函数任务
//  2） 函数ShowInfo用于输出调试信息
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit_TaskThread.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TTask::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TTask::TTask(bool CreateSuspended)
        : TThread(CreateSuspended)
{
     if(ThreadList==NULL)
     {
        ThreadList = new TStringList();
     }
     DWORD dwThreadID =  this->ThreadID;
     ThreadList->AddObject(AnsiString(dwThreadID),this);
}
//---------------------------------------------------------------------------
void __fastcall TTask::Execute()
{
        task_function();
}
//---------------------------------------------------------------------------

void __fastcall TTask::set_task(TMemo *memo,TASKFUNC TaskFunc)
{
     MemoInfo = memo;
     task_function = TaskFunc;
}

//---------------------------------------------------------------------------
void __fastcall TTask::ShowHeader(char *info)
{

    if(ThreadList!=NULL)
    {
       DWORD dwThreadID = GetCurrentThreadId();
       int  idx=  ThreadList->IndexOf(AnsiString(dwThreadID));
       if(idx>=0)
       {
          TTask  *curTask =(TTask *) ThreadList->Objects[idx];
          if(curTask)
          {
             curTask->str_header = AnsiString(info);
          }
       }
    }
}
void __fastcall TTask::ShowInfo(char *info)
{

    if(ThreadList!=NULL)
    {
       DWORD dwThreadID = GetCurrentThreadId();
       int  idx=  ThreadList->IndexOf(AnsiString(dwThreadID));
       if(idx>=0)
       {
          TTask  *curTask =(TTask *) ThreadList->Objects[idx];
          if(curTask)
          {
             curTask->thread_id=dwThreadID;
             curTask->str_info = AnsiString(info);
             curTask->Synchronize(curTask->UpdateInfo);
          }
       }
       else
       {
        //Form_ndfat->Memo_Data->Lines->Add(AnsiString(info));

       }
    }
}
//---------------------------------------------------------------------------
void __fastcall TTask::ShowInfo_ext(char *info)
{

    if(ThreadList!=NULL)
    {
       DWORD dwThreadID = GetCurrentThreadId();
       int  idx=  ThreadList->IndexOf(AnsiString(dwThreadID));
       if(idx>=0)
       {
          TTask  *curTask =(TTask *) ThreadList->Objects[idx];
          if(curTask)
          {
             curTask->thread_id=dwThreadID;
             curTask->str_info_ext = AnsiString(info);
             curTask->Synchronize(curTask->UpdateInfo_ext);

          }
       }
    }
}
//---------------------------------------------------------------------------
unsigned char __fastcall TTask::get_cur_task_no(void )
{

    if(ThreadList!=NULL)
    {
       DWORD dwThreadID = GetCurrentThreadId();
       int  idx=  ThreadList->IndexOf(AnsiString(dwThreadID));
       if(idx>=0)
       {
          TTask  *curTask =(TTask *) ThreadList->Objects[idx];
          if(curTask)
          {
              return   curTask->task_no;
          }
       }
    }
    return 0;
}
unsigned char* __fastcall TTask::get_cur_task_name(void )
{

    if(ThreadList!=NULL)
    {
       DWORD dwThreadID = GetCurrentThreadId();
       int  idx=  ThreadList->IndexOf(AnsiString(dwThreadID));
       if(idx>=0)
       {
          TTask  *curTask =(TTask *) ThreadList->Objects[idx];
          if(curTask)
          {
              return   curTask->task_name.c_str();
          }
       }
    }
    return 0;
}

//---------------------------------------------------------------------------
void __fastcall TTask::UpdateInfo()
{
     String str=IntToStr(thread_id);
     MemoInfo->Lines->Add(str+" "+str_header+str_info);
     str_header="";
}

void __fastcall TTask::UpdateInfo_ext()
{
    if(MemoInfo_ext)
    {
     String str=IntToStr(thread_id);
        MemoInfo_ext->Lines->Add(str+" "+str_info_ext);
    }
}
