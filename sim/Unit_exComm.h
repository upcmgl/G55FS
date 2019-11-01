//---------------------------------------------------------------------------

#ifndef Unit_exCommH
#define Unit_exCommH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TaskThread.h"
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <ScktComp.hpp>
#include "MSCommLib_OCX.h"
#include <OleCtrls.hpp>

//---------------------------------------------------------------------------
class TTask_exComm : public TMDIChildTaskThread
{
__published:	// IDE-managed Components
        TGroupBox *GroupBox1;
        TLabel *Label1;
        TCheckBox *CheckBox_RS232;
        TLabel *Label3;
        TEdit *Edit_RS232_Port;
        TBevel *Bevel1;
        TBitBtn *BitBtn_RS232_Connect;
        TBitBtn *BitBtn_RS232_Discon;
        TLabel *Label4;
        TCheckBox *CheckBox1;
        TLabel *Label6;
        TEdit *Edit_Infra_Port;
        TBitBtn *BitBtn_Infra_Connect;
        TBitBtn *BitBtn_Infra_Discon;
        TBevel *Bevel2;
        TServerSocket *ServerSocket_RS232;
    TButton *btn_remote_connect;
    TClientSocket *clntsckt_remote;
    TLabel *lbl1;
    TLabel *lbl2;
    TComboBox *cbb_addr;
    TComboBox *cbb_port;
    TTimer *tmr1;
    TMSComm *mscm_rs232;
    TLabel *Label2;
    TComboBox *cbb_rs232;
    TLabel *Label5;
    TComboBox *cbb_BaudRate;
    TLabel *Label7;
    TComboBox *cbb_Parity;
        void __fastcall BitBtn_RS232_ConnectClick(TObject *Sender);
        void __fastcall ServerSocket_RS232Listen(TObject *Sender,
          TCustomWinSocket *Socket);
        void __fastcall ServerSocket_RS232Accept(TObject *Sender,
          TCustomWinSocket *Socket);
        void __fastcall BitBtn_RS232_DisconClick(TObject *Sender);
        void __fastcall ServerSocket_RS232ClientRead(TObject *Sender,
          TCustomWinSocket *Socket);
        void __fastcall BitBtn_StartClick(TObject *Sender);
        void __fastcall ServerSocket_RS232ClientWrite(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall btn_remote_connectClick(TObject *Sender);
    void __fastcall clntsckt_remoteConnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall clntsckt_remoteRead(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall btn1Click(TObject *Sender);
    void __fastcall tmr1Timer(TObject *Sender);
    void __fastcall mscm_rs232Comm(TObject *Sender);
private:	// User declarations
         TCustomWinSocket  *RS232_ClientSocket;
public:		// User declarations
        __fastcall TTask_exComm(TComponent* Owner);
        void __fastcall RS232_SendFrame(unsigned char *frame, unsigned short frameLen);
        void __fastcall Remote_SendFrame(unsigned char *frame, unsigned short frameLen);
        void __fastcall rs232com_SendFrame(unsigned char *frame,int len);
};
//---------------------------------------------------------------------------
extern PACKAGE TTask_exComm *Task_exComm;
//---------------------------------------------------------------------------
#endif
