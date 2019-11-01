//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------
USEFORM("topdevp_main.cpp", TOPDEVP_MainForm);
USEFORM("ChildWin.cpp", MDIChild);
USEFORM("about.cpp", AboutBox);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Application->Initialize();
	Application->CreateForm(__classid(TTOPDEVP_MainForm), &TOPDEVP_MainForm);
                 Application->CreateForm(__classid(TAboutBox), &AboutBox);
                 Application->Run();

	return 0;
}
//---------------------------------------------------------------------
