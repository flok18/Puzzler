

#include "StdInc.h"
#include "Flib1.h"
#include "Main.h"


HINSTANCE  Main_hInst   = NULL;


void Main_Entry(void)
{
	int  ii;


	Settings_Load();
	FileList_Init();


	for (ii = 1; ii < __argc; ++ii)
	{
		FileList_Add(__argv[ii]);
	}


    MainWindow_Create();

    MessageLoop();

    MainWindow_Release();


    FileList_Release();
	Settings_Free();
}


int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdline, int nCmdShow)
{
	Main_hInst = hInst;
	Main_Entry();
	return 0;
}

int __cdecl main(int argc, char **argv)
{
	Main_hInst = GetModuleHandle(NULL);
	Main_Entry();
	return 0;
}


