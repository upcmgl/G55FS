//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit_ProtocolHander1.h"
#include  "main_include.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TaskThread"
#pragma resource "*.dfm"
TTask_ProtocolHandler1 *Task_ProtocolHandler1;
//---------------------------------------------------------------------------
__fastcall TTask_ProtocolHandler1::TTask_ProtocolHandler1(TComponent* Owner)
        : TMDIChildTaskThread(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TTask_ProtocolHandler1::BitBtn_StartClick(TObject *Sender)
{
       Memo_Info->Lines->Add("Start Task....");
       taskThread->set_task(Memo_Info,task_protocol_handler);
       taskThread->Priority =  tpNormal;
       if(chk1->Checked)
       {
        taskThread->task_no = 10;
        taskThread->task_name="PROTOCOL_HANDLER2";
       }
       else
       {
        taskThread->task_no = 2;
        taskThread->task_name="PROTOCOL_HANDLER1";
       }
       taskThread->Resume();
}
//---------------------------------------------------------------------------
