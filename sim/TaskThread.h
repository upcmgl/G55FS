//----------------------------------------------------------------------------
#ifndef TaskThreadH
#define TaskThreadH
//----------------------------------------------------------------------------
#include <vcl\Controls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Windows.hpp>
#include <vcl\System.hpp>
#include <StdCtrls.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "Unit_TaskThread.h"
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
//----------------------------------------------------------------------------
class TMDIChildTaskThread : public TForm
{
__published:
        TPanel *Panel1;
        TBitBtn *BitBtn_Pause;
        TBitBtn *BitBtn_Start;
        TBitBtn *BitBtn_Clear;
        TBitBtn *BitBtn_Del;
        TBitBtn *BitBtn_Create;
        TMemo *Memo_Info;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall BitBtn_ClearClick(TObject *Sender);
        void __fastcall BitBtn_StartClick(TObject *Sender);
        void __fastcall BitBtn_PauseClick(TObject *Sender);
        void __fastcall BitBtn_DelClick(TObject *Sender);
        void __fastcall BitBtn_CreateClick(TObject *Sender);
private:

public:
        TTask   *taskThread;
        TASKFUNC    task_function;    //ÈÎÎñº¯Êý
	virtual __fastcall TMDIChildTaskThread(TComponent *Owner);
        AnsiString __fastcall format2MeterAddr(AnsiString str);
        int  __fastcall Hex2Bin(AnsiString hex,unsigned char *bin);


};
//----------------------------------------------------------------------------
#endif	
