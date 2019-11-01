//---------------------------------------------------------------------------

#ifndef Unit_ProtocolHander1H
#define Unit_ProtocolHander1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TaskThread.h"
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TTask_ProtocolHandler1 : public TMDIChildTaskThread
{
__published:	// IDE-managed Components
    TCheckBox *chk1;
        void __fastcall BitBtn_StartClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TTask_ProtocolHandler1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TTask_ProtocolHandler1 *Task_ProtocolHandler1;
//---------------------------------------------------------------------------
#endif
