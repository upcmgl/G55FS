//---------------------------------------------------------------------------

#ifndef Unit_readPortServiceH
#define Unit_readPortServiceH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TaskThread.h"
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "MSCommLib_OCX.h"
#include <OleCtrls.hpp>
#include <ScktComp.hpp>
//---------------------------------------------------------------------------
class TTask_readPortService : public TMDIChildTaskThread
{
__published:	// IDE-managed Components
    TPanel *pnl2;
    TButton *btn1;
    TClientSocket *clntsckt_RS485_1;
    TMSComm *mscm_comm_485;
    TComboBox *cbb_comm_485;
    TButton *btn2;
    TRadioButton *rbnet;
    TRadioButton *rbrs232;
    TEdit *edtwangkou;
    void __fastcall BitBtn_StartClick(TObject *Sender);
    void __fastcall comm_485_SendFrame(unsigned char *frame,int len);
    void __fastcall btn1Click(TObject *Sender);
    void __fastcall clntsckt_RS485_1Connect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall clntsckt_RS485_1Read(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall btn2Click(TObject *Sender);
    void __fastcall mscm_comm_485Comm(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TTask_readPortService(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TTask_readPortService *Task_readPortService;
//---------------------------------------------------------------------------
#endif
