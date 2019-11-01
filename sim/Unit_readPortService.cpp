//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "stdio.h"
#include "Unit_readPortService.h"
#include "Unit_OS.h"
#include  "main_include.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TaskThread"
#pragma link "MSCommLib_OCX"
#pragma resource "*.dfm"
TTask_readPortService *Task_readPortService;

#include  "main_include.h"

tagUartObj UartObjMeter;

//----------------------------------------------------------------
INT8U  meter_uart_is_idle(void)
{
    if(UartObjMeter.send_len)
    {
        return 0;
    }
    return 1;
}
void meter_uart_init (INT32U baudrate)
{


}
INT16S meter_uart_read_byte()
{
    INT16S data;

    data = -1;

    if(UartObjMeter.read_pos == UartObjMeter.recv_pos)
    {
        return -1;
    }
    data = UartObjMeter.recv_buf[UartObjMeter.read_pos ++];
    if (UartObjMeter.read_pos >= sizeof(UartObjMeter.recv_buf)) UartObjMeter.read_pos = 0;

    return data;
}

INT8U meter_uart_send_buf(INT8U *frame,INT16U frame_len)
{
    #ifdef __SOFT_SIMULATOR__
        debug_println_hex(frame,frame_len,"send");
    #endif   
    if(Task_readPortService->rbrs232->Checked)
    {
        Task_readPortService->comm_485_SendFrame(frame,frame_len);
        return TRUE;
    }
    else
    {
        //检查响应对象是否能否发送
        Task_readPortService->clntsckt_RS485_1->Socket->SendBuf(frame,frame_len);
        return TRUE;
    }
    return FALSE;
}
//----------------------------------------------------------------

//---------------------------------------------------------------------------
__fastcall TTask_readPortService::TTask_readPortService(TComponent* Owner)
        : TMDIChildTaskThread(Owner)
{

    UartObjMeter.read_pos = 0;
    UartObjMeter.recv_pos = 0;
}
//---------------------------------------------------------------------------
void __fastcall TTask_readPortService::BitBtn_StartClick(TObject *Sender)
{
    Memo_Info->Lines->Add("Start Task....");
    taskThread->set_task(Memo_Info,task_meter_service);
    taskThread->task_no = 5;
    taskThread->Priority = tpNormal;
    taskThread->Resume();
}
//---------------------------------------------------------------------------



void __fastcall TTask_readPortService::btn1Click(TObject *Sender)
{

    clntsckt_RS485_1->Port= StrToInt(edtwangkou->Text);
    clntsckt_RS485_1->Active = true;
}
//---------------------------------------------------------------------------


void __fastcall TTask_readPortService::clntsckt_RS485_1Connect(
      TObject *Sender, TCustomWinSocket *Socket)
{
    Memo_Info->Lines->Add("****连接虚拟485电表成功！");
}
//---------------------------------------------------------------------------



void __fastcall TTask_readPortService::clntsckt_RS485_1Read(
      TObject *Sender, TCustomWinSocket *Socket)
{
    unsigned short year,month,day,hour,minute,second,Msec;

    unsigned char tmp[512];
    int len = Socket->ReceiveBuf(tmp,512);
        #ifdef __SOFT_SIMULATOR__
        if(len>0)
    DecodeDate(Now(),year,month,day);
    DecodeTime(Now(),hour,minute,second,Msec);
    snprintf(info,100,"%02d-%02d-%02d %02d:%02d:%02d:%03d [%04s] : ",year,month,day,hour,minute,second,Msec,"aaaa");
    GetHexMessage(tmp,len,info+31);
        Memo_Info->Lines->Add((char*)info);
        #endif

    for(int i=0;i<len;i++)
    {
        UartObjMeter.recv_buf[UartObjMeter.recv_pos++] = tmp[i];
        if(UartObjMeter.recv_pos >= sizeof(UartObjMeter.recv_buf)) UartObjMeter.recv_pos = 0;
    }
}
//---------------------------------------------------------------------------

void __fastcall TTask_readPortService::btn2Click(TObject *Sender)
{
        mscm_comm_485->CommPort=cbb_comm_485->ItemIndex+1;

//        AnsiString Speed=cbb_BaudRate->Text.Trim();
//        AnsiString parity='N';
//        if(cbb_Parity->ItemIndex==1)
//            parity='E';
//        else if(cbb_Parity->ItemIndex==2)
//            parity='O';
//
//        mscm_plc->Settings=Speed+","+parity+",8,1";
        mscm_comm_485->Settings = "2400,e,8,1";
        mscm_comm_485->InputMode=1;
        mscm_comm_485->RThreshold=1;
        mscm_comm_485->PortOpen=true;

        Memo_Info->Lines->Add("串口已经启动！");
}
//---------------------------------------------------------------------------

void __fastcall TTask_readPortService::mscm_comm_485Comm(TObject *Sender)
{
    OleVariant Re_Buf;
    int length = 0;
    unsigned char uart2RxValue;

    if(mscm_comm_485->CommEvent == comEvReceive)
    {
        int Len=mscm_comm_485->InBufferCount;   //接收字符的长度
        if(mscm_comm_485->InBufferCount > 0)
        {
            Re_Buf=mscm_comm_485->Input;
            length=Re_Buf.ArrayHighBound(1);// GetElement(0);
            for(int i=0;i<=length;i++)
            {
                uart2RxValue = Re_Buf.GetElement(i);

                UartObjMeter.recv_buf[UartObjMeter.recv_pos++] = uart2RxValue;
                if(UartObjMeter.recv_pos >= sizeof(UartObjMeter.recv_buf)) UartObjMeter.recv_pos = 0;
            }
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TTask_readPortService::comm_485_SendFrame(unsigned char *frame,int len)
{
    if(mscm_comm_485->PortOpen)
    {
        OleVariant VarBuff;
        VarBuff=VarArrayCreate(OPENARRAY(int,(0,len-1)),varByte);     //注意   int,(0,n-1))   不应为   int,(0,n))
        for (int i=0;i<len;i++)
        {
            VarBuff.PutElement(frame[i],i);
        }
        mscm_comm_485->Output = VarBuff;
    }
}




