//----------------------------------------------------------------------------
#ifndef topdevp_mainH
#define topdevp_mainH
//----------------------------------------------------------------------------
#include "ChildWin.h"
#include <vcl\ComCtrls.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\Messages.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\Dialogs.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Menus.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\Classes.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Windows.hpp>
#include <vcl\System.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <StdActns.hpp>
#include <ToolWin.hpp>
#include <DBCtrls.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
//----------------------------------------------------------------------------
class TTOPDEVP_MainForm : public TForm
{
__published:
	TMainMenu *MainMenu1;
	TMenuItem *File1;
    TMenuItem *ItemDisplayTask;
    TMenuItem *ItemReadPortTask;
    TMenuItem *ItemSystemServiceTask;
	TMenuItem *Window1;
	TMenuItem *Help1;
	TMenuItem *N1;
	TMenuItem *FileExitItem;
	TMenuItem *WindowCascadeItem;
	TMenuItem *WindowTileItem;
	TMenuItem *WindowArrangeItem;
	TMenuItem *HelpAboutItem;
	TOpenDialog *OpenDialog;
    TMenuItem *ItemProtocolServiceTask;
    TMenuItem *ItemProtocolHandlerTask;
	TMenuItem *Edit1;
    TMenuItem *ItemFileTools;
    TMenuItem *ItemFaskIndexTools;
	TMenuItem *WindowMinimizeItem;
        TStatusBar *StatusBar;
        TActionList *ActionList1;
        TEditCut *EditCut1;
        TEditCopy *EditCopy1;
        TEditPaste *EditPaste1;
        TAction *FileNew1;
        TAction *FileSave1;
        TAction *FileExit1;
        TAction *FileOpen1;
        TAction *FileSaveAs1;
        TWindowCascade *WindowCascade1;
        TWindowTileHorizontal *WindowTileHorizontal1;
        TWindowArrange *WindowArrangeAll1;
        TWindowMinimizeAll *WindowMinimizeAll1;
        TAction *HelpAbout1;
        TWindowClose *FileClose1;
        TWindowTileVertical *WindowTileVertical1;
        TMenuItem *WindowTileItem2;
        TToolBar *ToolBar2;
        TToolButton *ToolButton8;
        TToolButton *ToolButton10;
        TToolButton *ToolButton11;
        TImageList *ImageList1;
    TTimer *Timer1;
    TMenuItem *ItemCycleServiceTask;
    TAction *Action1;
    TAction *act_DisplayTask;
    TAction *act_ReadPortTask;
    TAction *act_SystemServiceTask;
    TAction *act_ProtocolServiceTask;
    TAction *act_ProtocolHandlerTask;
    TAction *act_CycleServiceTask;
    TMenuItem *N2;
    TMenuItem *N3;
    TAction *act_ReportTask;
    TAction *act_PoweronoffTask;
        void __fastcall HelpAbout1Execute(TObject *Sender);
        void __fastcall FileExit1Execute(TObject *Sender);
    void __fastcall num(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall act_DisplayTaskExecute(TObject *Sender);
    void __fastcall act_ReadPortTaskExecute(TObject *Sender);
    void __fastcall act_SystemServiceTaskExecute(TObject *Sender);
    void __fastcall act_ProtocolServiceTaskExecute(TObject *Sender);
    void __fastcall act_ProtocolHandlerTaskExecute(TObject *Sender);
    void __fastcall act_CycleServiceTaskExecute(TObject *Sender);
    void __fastcall ItemFileToolsClick(TObject *Sender);
    void __fastcall ItemFaskIndexToolsClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall act_ReportTaskExecute(TObject *Sender);
    void __fastcall act_PoweronoffTaskExecute(TObject *Sender);
private:
	void __fastcall CreateMDIChild(const String Name);
public:
	virtual __fastcall TTOPDEVP_MainForm(TComponent *Owner);
};
//----------------------------------------------------------------------------
extern TTOPDEVP_MainForm *TOPDEVP_MainForm;
extern TMDIChild *__fastcall MDIChildCreate(void);
//----------------------------------------------------------------------------
#endif    
