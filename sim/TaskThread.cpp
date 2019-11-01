//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "stdio.h"
#include "TaskThread.h"
#include "Unit_OS.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
//---------------------------------------------------------------------
__fastcall TMDIChildTaskThread::TMDIChildTaskThread(TComponent *Owner)
	: TForm(Owner)
{
       taskThread = new TTask(true);
}
//---------------------------------------------------------------------
void __fastcall TMDIChildTaskThread::FormClose(TObject *Sender, TCloseAction &Action)
{

    TerminateThread((HANDLE)(taskThread->Handle),0);
    delete taskThread;
	Action = caFree;
}
//---------------------------------------------------------------------

void __fastcall TMDIChildTaskThread::BitBtn_ClearClick(TObject *Sender)
{
       Memo_Info->Lines->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TMDIChildTaskThread::BitBtn_StartClick(TObject *Sender)
{

       taskThread->set_task(Memo_Info,task_function);
       taskThread->Priority =  tpLower;
       taskThread->Resume();
}
//---------------------------------------------------------------------------
void debug_println_hex(unsigned char *frame,unsigned short frame_len,char * type_text)
{
    unsigned short year,month,day,hour,minute,second,Msec;

    #ifdef __SOFT_SIMULATOR__
    DecodeDate(Now(),year,month,day);
    DecodeTime(Now(),hour,minute,second,Msec);
    snprintf(info,100,"%02d-%02d-%02d %02d:%02d:%02d:%03d [%04s] : ",year,month,day,hour,minute,second,Msec,type_text);
    GetHexMessage(frame,frame_len,info+31);
    TTask::ShowInfo(info);
    #endif
}
void debug_println(char *info)
{
        TTask::ShowInfo(info);
}
void debug_print_header(char *info)
{
     TTask::ShowHeader(info);
}
void debug_println_ext(char *info)
{
     TTask::ShowInfo_ext(info);
}

unsigned char get_cur_task_no(void)
{
    return TTask::get_cur_task_no();
}
unsigned char* get_cur_task_name(void)
{
    return TTask::get_cur_task_name();
}
//---------------------------------------------------------------------------

void __fastcall TMDIChildTaskThread::BitBtn_PauseClick(TObject *Sender)
{
     if(BitBtn_Pause->Caption=="暂停")
     {
       BitBtn_Pause->Caption="继续";
       taskThread->Suspend();
     }
     else
     {
        BitBtn_Pause->Caption="暂停";
        taskThread->Resume();
     }
}
//---------------------------------------------------------------------------
/*+++
  功能：确保为12字节的电表地址
---*/
AnsiString __fastcall TMDIChildTaskThread::format2MeterAddr(AnsiString str)
{
     if(str==NULL || str =="") return "000000000000";
     if(str.Length() >=12)
     {
       str.SetLength(12);
       return str;
     }
     AnsiString zero="000000000000";
     zero.SetLength(12-str.Length());
     return zero+str;
}

/*+++
  功能: 把Hex字符串变换为Byte数组,返回字节数量
---*/
int  __fastcall TMDIChildTaskThread::Hex2Bin(AnsiString hex,unsigned char *bin)
{
    int pos=0;
    char ch1,ch2;

    hex = hex.UpperCase();
    for(int i=1;i<=hex.Length();i+=2)
    {
        ch1 = hex[i];
        ch2 = hex[i+1];

        if(ch1 <='9') ch1 -= '0';
        else          ch1 -= 'A'-10;

        if(ch2 <='9') ch2 -= '0';
        else          ch2 -='A'-10;

        bin[pos++] = (ch1 <<4) + ch2;
    }
    return pos;
}


/*** 动态装载动态库
   //设置默认打开目录
   if(OpenDialog->InitialDir==NULL  || OpenDialog->InitialDir=="")
   {
      OpenDialog->InitialDir=ExtractFileDir(Application->ExeName);
   }
   if(OpenDialog->Execute())
   {
       String MSG;
       if(hDLL!=NULL)  FreeLibrary(hDLL);
       hDLL=NULL;

       hDLL=LoadLibrary(OpenDialog->FileName.c_str());
       if(hDLL==NULL)
       {
           MSG="无法找到动态链接库:"+OpenDialog->FileName;
           Application->MessageBox(MSG.c_str(), "系统提示信息", MB_OK+MB_ICONINFORMATION);
       }

       try
       {
            hTaskFunction=(TASKFUNC)GetProcAddress(hDLL,"task_function");
       }
       catch(...)
       {
            Application->MessageBox("程序无法正常工作!","系统提示信息",MB_OK+MB_ICONINFORMATION);
            if(hDLL)  FreeLibrary(hDLL);
            hDLL=NULL;
       }
   }
   */

void __fastcall TMDIChildTaskThread::BitBtn_DelClick(TObject *Sender)
{
        TerminateThread((HANDLE)(taskThread->Handle),0);
        delete taskThread;
}
//---------------------------------------------------------------------------

void __fastcall TMDIChildTaskThread::BitBtn_CreateClick(TObject *Sender)
{
        taskThread = new TTask(true);
}
//---------------------------------------------------------------------------

