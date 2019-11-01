//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------
USEFORM("topdevp_main.cpp", TOPDEVP_MainForm);
USEFORM("about.cpp", AboutBox);
USEFORM("Unit_exComm.cpp", Task_exComm);
USEFORM("Unit_ProtocolHander1.cpp", Task_ProtocolHandler1);
USEFORM("Unit_readPortService.cpp", Task_readPortService);
USEFORM("TaskThread.cpp", MDIChildTaskThread);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    Application->Initialize();
	Application->CreateForm(__classid(TTOPDEVP_MainForm), &TOPDEVP_MainForm);
                 Application->CreateForm(__classid(TTask_exComm), &Task_exComm);
                 Application->CreateForm(__classid(TTask_ProtocolHandler1), &Task_ProtocolHandler1);
                 Application->CreateForm(__classid(TTask_readPortService), &Task_readPortService);
                 Application->Run();
    return 0;
}
//---------------------------------------------------------------------

