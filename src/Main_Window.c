
#include "StdInc.h"
#include "Puzzle.h"
#include "FLib1.h"
#include "Bitmap.h"
#include "Main.h"


#include <Shellapi.h>


int  WIN_PIC_SIZE_SPACE_X   = 4;
int  WIN_PIC_SIZE_SPACE_Y   = 4;



static LPCSTR          MainWindow_ClassName          = "Puzzler_Window.1";
static HWND            MainWindow_Handle             = NULL;

static Struct_Puzzle   MyPuzzle;


#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp)  ((int)(short)LOWORD(lp))
#endif

#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp)  ((int)(short)HIWORD(lp))
#endif




static BOOL MainWindow_Start_Puzzle(const char* FromFile)
{
	static const char*   __static_Start_Puzzle_Last_File   = NULL;
	BOOL                 RetStatus                         = FALSE;
	Struct_Bitmap_Data   Orig_Bitmap;


	if (FromFile)
	{
		__static_Start_Puzzle_Last_File = FromFile;
	}

	if (__static_Start_Puzzle_Last_File)
	{
		Bitmap_Init(&Orig_Bitmap);

		if (Bitmap_Load(&Orig_Bitmap, __static_Start_Puzzle_Last_File,
		                MyPuzzle.Board.Size_X - WIN_PIC_SIZE_SPACE_X,
						MyPuzzle.Board.Size_Y - WIN_PIC_SIZE_SPACE_Y))
		{
			Puzzle_Create_Pieces(&MyPuzzle, Orig_Bitmap.Pixels, Orig_Bitmap.Size_X, Orig_Bitmap.Size_Y);
			RetStatus = TRUE;
		}

		Bitmap_Release(&Orig_Bitmap);
	}
	else
    {
        // No Pics... Also OK...
        RetStatus = TRUE;
    }

	Puzzle_Draw_Board(&MyPuzzle);
	Puzz_Plugin_Start();

	return RetStatus;
}


static void MainWindow_Button_Move_Pieces_Inside(HWND hwnd)
{
	RECT                WindowInterior;
	int                 ii;
	Struct_PieceChain*  AkChain;

	GetClientRect(hwnd, &WindowInterior);
	WindowInterior.left   += 1;
	WindowInterior.top    += 1;
	WindowInterior.right  -= 1;
	WindowInterior.bottom -= 1;

	for (ii = 0; ii < MyPuzzle.nPieceChain; ++ii)
	{
		AkChain = MyPuzzle.aPieceChain + ii;

		if (AkChain->Position_X + AkChain->Image_Size_X > WindowInterior.right  - WIN_PIC_SIZE_SPACE_X) AkChain->Position_X = WindowInterior.right  - AkChain->Image_Size_X - WIN_PIC_SIZE_SPACE_X;
		if (AkChain->Position_Y + AkChain->Image_Size_Y > WindowInterior.bottom - WIN_PIC_SIZE_SPACE_Y) AkChain->Position_Y = WindowInterior.bottom - AkChain->Image_Size_Y - WIN_PIC_SIZE_SPACE_Y;

		if (AkChain->Position_X < WindowInterior.left + WIN_PIC_SIZE_SPACE_X) AkChain->Position_X = WindowInterior.left + WIN_PIC_SIZE_SPACE_X;
		if (AkChain->Position_Y < WindowInterior.top  + WIN_PIC_SIZE_SPACE_Y) AkChain->Position_Y = WindowInterior.top  + WIN_PIC_SIZE_SPACE_Y;
	}

	Puzzle_Draw_Board(&MyPuzzle);
	InvalidateRect(hwnd, NULL, FALSE);
}


static void MainWindow_Button_Solve(HWND hwnd)
{
	RECT                WindowInterior;
	Struct_PieceChain*  AkChain;


	while (MyPuzzle.nPieceChain > 1)
	{
		Puzzle_Assemble_Chains(&MyPuzzle, 0, 1);
	}

	AkChain = MyPuzzle.aPieceChain;
    if (AkChain)
    {
        GetClientRect(hwnd, &WindowInterior);

        AkChain->Position_X = WindowInterior.right  / 2 - AkChain->Image_Size_X / 2;
        AkChain->Position_Y = WindowInterior.bottom / 2 - AkChain->Image_Size_Y / 2;
    }

	Puzzle_Draw_Board(&MyPuzzle);
	InvalidateRect(hwnd, NULL, FALSE);
}


static void MainWindow_Button_Reset(HWND hwnd)
{
	MainWindow_Start_Puzzle(NULL);
	InvalidateRect(hwnd, NULL, FALSE);
}

static void MainWindow_Button_Next_Picture(HWND hwnd)
{
	const char*          Image_File;


	Image_File = FileList_Get_Random_File();

    if (MainWindow_Start_Puzzle(Image_File) == FALSE)
    {
        MsgBox_Error(hwnd, "Kann Datei nicht laden...");
    }

	InvalidateRect(hwnd, NULL, FALSE);
}


static void MainWindow_First_Size_Set(HWND hwnd)
{
	RECT  WindowInterior;

	GetClientRect(hwnd, &WindowInterior);

	Puzzle_Create_Board(&MyPuzzle, hwnd,
	                    WindowInterior.right - WindowInterior.left,
						WindowInterior.bottom - WindowInterior.top);

	MainWindow_Button_Next_Picture(hwnd);
}



static void MainWindow_Paint(HWND hwnd)
{
    HDC          DevCtx;
    PAINTSTRUCT  ps;

    DevCtx = BeginPaint(hwnd, &ps);
    Puzzle_Paint(&MyPuzzle, DevCtx);
    EndPaint(hwnd, &ps);
}


static void MainWindow_Menu_Click(HWND hwnd, int MenuID)
{
	switch(MenuID)
	{
		case 101:  // Next Picture
			MainWindow_Button_Next_Picture(hwnd);
			break;
		case 102:  // Move Pieces Inside
			MainWindow_Button_Move_Pieces_Inside(hwnd);
			break;
		case 103:  // Solve
			MainWindow_Button_Solve(hwnd);
			break;
		case 104:  // Solve
			MainWindow_Button_Reset(hwnd);
			break;
	}
}


void MainWindow_DropFileEvent(HWND hwnd, HDROP DropHandle)
{
	UINT   NumFiles;
	UINT   ii;
	char   Buffer[MAX_PATH];
    BOOL   RestartAfterLoad     = FALSE;


    if (FileList_Get_Count() == 0) RestartAfterLoad = TRUE;

	NumFiles = DragQueryFile(DropHandle, 0xFFFFFFFF, NULL, 0);

	for (ii = 0; ii < NumFiles; ++ii)
	{
		if (DragQueryFile(DropHandle, ii, Buffer, sizeof(Buffer)))
		{
		    FileList_Add(Buffer);
		}
	}

	DragFinish(DropHandle);

	if (RestartAfterLoad)
    {
        MainWindow_Button_Next_Picture(hwnd);
    }
}

static void MainWindow_Resize(HWND hwnd, int GrX, int GrY)
{
	static int  __static_Resize_Call_Counter  = 0;

	++__static_Resize_Call_Counter;
	if (__static_Resize_Call_Counter == 2)
	{
		MainWindow_First_Size_Set(hwnd);
	}
}


static LRESULT CALLBACK MainWindow_WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
		case WM_CREATE:
			Puzz_Plugin_Init(hwnd);
			break;

        case WM_DESTROY:
            MainWindow_Handle = NULL;
            break;

		case WM_COMMAND:
			if (lParam == 0 && HIWORD(wParam) == 0)
			{
				MainWindow_Menu_Click(hwnd, LOWORD(wParam));
			}
			break;

        case WM_SIZE:
			if (wParam & SIZE_MINIMIZED) break;
			MainWindow_Resize(hwnd, LOWORD(lParam), HIWORD(lParam));
            break;

		case WM_PAINT:
			MainWindow_Paint(hwnd);
			break;

        case WM_CLOSE:
			Puzz_Plugin_Release();
			// Exit Message Loop...
			// (Then, 'MainWindow_Release' will be called)
            PostQuitMessage(0);
			// Omit 'WM_DESTROY'
			// (it is called manuelly in 'MainWindow_Release')
			return 0;

		case WM_LBUTTONDOWN:
			Puzzle_Drag_Begin(&MyPuzzle, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;

		case WM_LBUTTONUP:
			Puzzle_Drag_End(&MyPuzzle, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;

   		case WM_MOUSEMOVE:
			Puzzle_Drag_Move(&MyPuzzle, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;

		case WM_TIMER:
			Puzz_Plugin_Tick_Up();
			break;

		case WM_DROPFILES:
			MainWindow_DropFileEvent(hwnd, (HDROP)wParam);
			break;
    }

    return DefWindowProc(hwnd , message , wParam , lParam);
}



void MainWindow_Create(void)
{
	Puzzle_Init(&MyPuzzle);

	WindowClass_Register(MainWindow_ClassName, Main_hInst, MainWindow_WindowProc, FALSE, TRUE);

	MainWindow_Handle = Window_Create(NULL, 0, 0, 640, 480, WS_OVERLAPPEDWINDOW | WS_MAXIMIZE, 0,
                                      " Puzzler  -  Drag'n'Drop Pictures to this Window to Play...");

    Menu_Start(MainWindow_Handle);
		Menu_Add("  Next Picture  ",        101);
		Menu_Add("  Move Pieces Inside  ",  102);
		Menu_Add("  Solve  ",               103);
		Menu_Add("  Reset  ",               104);
    Menu_End();

	Window_SetIcon(MainWindow_Handle, 8001);
	ShowWindow(MainWindow_Handle, SW_MAXIMIZE);

	DragAcceptFiles(MainWindow_Handle, TRUE);
}


void MainWindow_Release(void)
{
	if (MainWindow_Handle)
	{
		DestroyWindow(MainWindow_Handle);
	}

	Puzzle_Release(&MyPuzzle);

	WindowClass_UnRegister(MainWindow_ClassName, NULL);
}



