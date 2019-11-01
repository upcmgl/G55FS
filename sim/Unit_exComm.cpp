//---------------------------------------------------------------------------
//   外部通信接口模拟任务
//
//    1) RS232
//    2) GPRS
//    3) 以太网
//    4) 级联口
//    5) 红外口
//---------------------------------------------------------------------------


#include <vcl.h>
#pragma hdrstop
#include "Unit_OS.h"

#include "Unit_exComm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TaskThread"
#pragma link "MSCommLib_OCX"
#pragma resource "*.dfm"
#include  "main_include.h"

tagUartObj UartObjDebug;

TTask_exComm *Task_exComm;
uint16_t app_make_active_test_frame(uint8_t *frame,uint8_t is_first);
INT8U  debug_uart_is_idle(void)
{
    if(UartObjDebug.send_len)
    {
        return 0;
    }
    return 1;
}
void debug_uart_init(INT32U baudrate)
{


}
INT16S debug_uart_read_byte(void)
{
    INT16S data;

    data = -1;

    if(UartObjDebug.read_pos == UartObjDebug.recv_pos)
        return data;
    data = UartObjDebug.recv_buf[UartObjDebug.read_pos ];
    UartObjDebug.read_pos ++;
    if(UartObjDebug.read_pos >=sizeof(UartObjDebug.recv_buf) ) UartObjDebug.read_pos = 0;

    return data;
}

INT8U debug_uart_send_buf(INT8U *data,INT16U len)
{

    #ifdef __SOFT_SIMULATOR__
        debug_println_hex(data,len,"send");
    #endif   
    if(Task_exComm->clntsckt_remote->Active)
    {
        Task_exComm->Remote_SendFrame(data,len);
    }
    else
    {
        if(Task_exComm->CheckBox_RS232->Checked)
        {
            Task_exComm->rs232com_SendFrame(data,len);
        }
        else
        {
            Task_exComm->RS232_SendFrame(data,len);
        }
    }
    return TRUE;
}
//---------------------------------------------------------------------------
__fastcall TTask_exComm::TTask_exComm(TComponent* Owner)
        : TMDIChildTaskThread(Owner)
{
    UartObjDebug.read_pos = 0;
    UartObjDebug.recv_pos = 0;

}
//---------------------------------------------------------------------------
void __fastcall TTask_exComm::BitBtn_StartClick(TObject *Sender)
{
       Memo_Info->Lines->Add("Start Task....");
       taskThread->set_task(Memo_Info,task_protocol_service);
       taskThread->Priority =  tpNormal;
       taskThread->task_no = 3;
       taskThread->Resume();
}

//---------------------------------------------------------------------------
void __fastcall TTask_exComm::BitBtn_RS232_ConnectClick(TObject *Sender)
{
    //启动RS232服务接口
    if(CheckBox_RS232->Checked)
    {
        //使用实际的串口，初始化串口波特率，关闭Socket服务
        ServerSocket_RS232->Active = false;
        if(!mscm_rs232->PortOpen)
        {
            mscm_rs232->CommPort=cbb_rs232->ItemIndex+1;

            AnsiString Speed=cbb_BaudRate->Text.Trim();
            AnsiString parity='N';
            if(cbb_Parity->ItemIndex==1)
                parity='E';
            else if(cbb_Parity->ItemIndex==2)
                parity='O';

            mscm_rs232->Settings=Speed+","+parity+",8,1";
            //mscm_rs232->Settings = "115200,e,8,1";
            mscm_rs232->InputMode=1;
            mscm_rs232->RThreshold=1;
            mscm_rs232->PortOpen=true;

            Memo_Info->Lines->Add("打开rs232口！");
        }
        else
        {
            Memo_Info->Lines->Add("rs232串口已经启动！");
        }
    }
    else
    {
        //使用虚拟的串口，启动TCP服务，关闭实际串口
        ServerSocket_RS232->Active = false;
        ServerSocket_RS232->Port = Edit_RS232_Port->Text.ToInt();
        ServerSocket_RS232->Active = true;
    }

//    DrvRS232Channel.read_ptr = 0;
//    DrvRS232Channel.recv_ptr = 0;
//    RS232Channel.channel_read = drv_rs232_read_byte;
//    RS232Channel.channel_send = srv_rs232_send_resp;

}
//---------------------------------------------------------------------------

void __fastcall TTask_exComm::ServerSocket_RS232Listen(TObject *Sender,
      TCustomWinSocket *Socket)
{
     Memo_Info->Lines->Add("RS232串口虚拟TCP服务开启!");
}
//---------------------------------------------------------------------------

void __fastcall TTask_exComm::ServerSocket_RS232Accept(TObject *Sender,
      TCustomWinSocket *Socket)
{
    AnsiString info;

    if((RS232_ClientSocket!=NULL) && (RS232_ClientSocket!=Socket))
    {
        try{
        RS232_ClientSocket->Close();
        }
        catch(...)
        {

        }
    }    RS232_ClientSocket =  Socket;
    
    info = "接受连接！ 远程主机:"+Socket->RemoteHost + "," + Socket->RemoteAddress + " 端口："+AnsiString(Socket->RemotePort);
    Memo_Info->Lines->Add(info);
}
//---------------------------------------------------------------------------

void __fastcall TTask_exComm::BitBtn_RS232_DisconClick(TObject *Sender)
{
     //停止RS232接口服务
    if(CheckBox_RS232->Checked)
    {
        //使用实际的串口，初始化串口波特率，关闭Socket服务
        ServerSocket_RS232->Active = false;
    }
    else
    {
       //使用虚拟的串口，启动TCP服务，关闭实际串口
       ServerSocket_RS232->Active = false;   
    }
     Memo_Info->Lines->Add("RS232串口服务关闭!");
}
//---------------------------------------------------------------------------


void __fastcall TTask_exComm::ServerSocket_RS232ClientRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
   unsigned char buffer[2048],*ptr;
   unsigned char rx_data;
   int   recvCount;
   AnsiString str;
   recvCount = Socket->ReceiveBuf(buffer,sizeof(buffer));
   ptr = buffer;
   while(recvCount--)
   {
        rx_data =  *ptr ++;

        //put recv char to recv buffer
        UartObjDebug.recv_buf[UartObjDebug.recv_pos] = rx_data;
        UartObjDebug.recv_pos ++;
        if(UartObjDebug.recv_pos >= sizeof(UartObjDebug.recv_buf))
        {
             UartObjDebug.recv_pos = 0;
        }
   }
}
//---------------------------------------------------------------------------
void __fastcall TTask_exComm::RS232_SendFrame(unsigned char *frame, unsigned short frameLen)
{
    if(RS232_ClientSocket==NULL)return;
    String str;
    str.sprintf("%04x\0",RS232_ClientSocket);
    Memo_Info->Lines->Add("after RS232_ClientSocket addr:"+str);
    if(!RS232_ClientSocket->Connected)return;

//        GetHexMessage(frame,frameLen,info);
//        debug_println(info);
        try{
      RS232_ClientSocket->SendBuf(frame,frameLen);
      }
      catch(...)
      {
          Memo_Info->Lines->Add("SendBuf error...");
      }
//      if(RS232Channel.pResp)
//      {
//          Memo_Info->Lines->Add("after ServerSocket_RS232ClientWrite complete...");
//          RS232Channel.pResp->state = APP_FRAME_SEND_COMPLETED;
//      }
}

//---------------------------------------------------------------------------
void __fastcall TTask_exComm::ServerSocket_RS232ClientWrite(
      TObject *Sender, TCustomWinSocket *Socket)
{
   if(RS232_ClientSocket == Socket)
   {
      //if(RS232Channel.pResp)
      {
          Memo_Info->Lines->Add("after ServerSocket_RS232ClientWrite complete...");
          //RS232Channel.pResp->state = APP_FRAME_SEND_COMPLETED;
      }
   }
}
//---------------------------------------------------------------------------
void __fastcall TTask_exComm::btn_remote_connectClick(TObject *Sender)
{
    clntsckt_remote->Active = false;
    clntsckt_remote->Host = cbb_addr->Text;
    clntsckt_remote->Port = cbb_port->Text.ToInt();
    clntsckt_remote->Active = true;
}
//---------------------------------------------------------------------------

void __fastcall TTask_exComm::clntsckt_remoteConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{

    uint8_t frame[100],len;
    Memo_Info->Lines->Add("****连接成功！");

    len = app_make_active_test_frame(frame,TRUE);
    Task_exComm->Remote_SendFrame(frame,len);
    //send_active_test(0,&RS232Channel);
}
//---------------------------------------------------------------------------

void __fastcall TTask_exComm::clntsckt_remoteRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
    unsigned char tmp[512];
    int len = Socket->ReceiveBuf(tmp,512);

    for(int i=0;i<len;i++)
    {
        UartObjDebug.recv_buf[UartObjDebug.recv_pos] = tmp[i];
        UartObjDebug.recv_pos ++;
        if(UartObjDebug.recv_pos >= sizeof(UartObjDebug.recv_buf))
        {
            UartObjDebug.recv_pos = 0;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TTask_exComm::Remote_SendFrame(unsigned char *frame, unsigned short frameLen)
{
    if(clntsckt_remote->Active && frame)
    {
        clntsckt_remote->Socket->SendBuf(frame,frameLen);
        GetHexMessage(frame,frameLen,info);
        debug_println(info);
//        if(RS232Channel.pResp)
//        {
//            Memo_Info->Lines->Add("remote send complete...");
//            RS232Channel.pResp->state = APP_FRAME_SEND_COMPLETED;
//        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TTask_exComm::btn1Click(TObject *Sender)
{
//    send_active_test(0,&RS232Channel);
}
//---------------------------------------------------------------------------

void __fastcall TTask_exComm::tmr1Timer(TObject *Sender)
{
    unsigned char frame[50],len;
    //if(clntsckt_remote->Active)
    {
        //send_active_test(1,&RS232Channel);
        len = app_make_active_test_frame(frame,FALSE);
        Task_exComm->Remote_SendFrame(frame,len);
        //Task_exComm->Remote_SendFrame(frame,len);
    }

}
//---------------------------------------------------------------------------

void __fastcall TTask_exComm::mscm_rs232Comm(TObject *Sender)
{
    OleVariant Re_Buf;
    int length = 0;
    unsigned char uart2RxValue;

    if(mscm_rs232->CommEvent == comEvReceive)
    {
        int Len=mscm_rs232->InBufferCount;   //接收字符的长度
        if(mscm_rs232->InBufferCount > 0)
        {
            Re_Buf=mscm_rs232->Input;
            length=Re_Buf.ArrayHighBound(1);// GetElement(0);
            for(int i=0;i<=length;i++)
            {
                uart2RxValue = Re_Buf.GetElement(i);

                UartObjDebug.recv_buf[UartObjDebug.recv_pos] = uart2RxValue;
                UartObjDebug.recv_pos ++;
                if(UartObjDebug.recv_pos >= sizeof(UartObjDebug.recv_buf))
                {
                     UartObjDebug.recv_pos = 0;
                }
            }
        }
    }

}
//---------------------------------------------------------------------------
void __fastcall TTask_exComm::rs232com_SendFrame(unsigned char *frame,int len)
{
    if(mscm_rs232->PortOpen)
    {
        OleVariant VarBuff;
        VarBuff=VarArrayCreate(OPENARRAY(int,(0,len-1)),varByte);     //注意   int,(0,n-1))   不应为   int,(0,n))
        for (int i=0;i<len;i++)
        {
            VarBuff.PutElement(frame[i],i);
        }
        mscm_rs232->Output = VarBuff;
    }
}



