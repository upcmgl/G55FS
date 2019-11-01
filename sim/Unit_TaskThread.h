//---------------------------------------------------------------------------

#ifndef Unit_TaskThreadH
#define Unit_TaskThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
typedef void  (*TASKFUNC)(void);
typedef void __fastcall (__closure *TThreadMethod)(void);

static TStringList *ThreadList=NULL;
class TTask : public TThread
{
private:
        TMemo      *MemoInfo;
        TMemo      *MemoInfo_ext;
        TASKFUNC    task_function;
protected:
        void __fastcall Execute();
public:
        AnsiString  str_info;
        AnsiString  str_header;
        AnsiString  str_info_ext;
        unsigned char  task_no;
        String task_name;
        unsigned int thread_id;  
        __fastcall TTask(bool CreateSuspended);
        void __fastcall TTask::UpdateInfo();
        void __fastcall TTask::UpdateInfo_ext();
        void __fastcall set_task(TMemo *memo,TASKFUNC TaskFunc);
        static void __fastcall ShowInfo(char *info);
        static void __fastcall ShowInfo_ext(char *info);
        static void __fastcall ShowHeader(char *info);
        static unsigned char __fastcall get_cur_task_no(void );
        static unsigned char* __fastcall get_cur_task_name(void );

};
//---------------------------------------------------------------------------
#endif
