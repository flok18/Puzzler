
#include "StdInc.h"
#include "FLib1.h"


const char*  _FloLib1_WindowClass_LastName           = NULL;
HINSTANCE    _FloLib1_WindowClass_LastInstance       = NULL;


void MessageLoop(void)
{
    MSG    aMsg;
    LPMSG  aMsgPointer  = &aMsg;
	
    while (GetMessage(aMsgPointer, NULL, 0, 0))
    {
        TranslateMessage (aMsgPointer);
        DispatchMessage (aMsgPointer);
    }
}

void MsgBox_Error(HWND ParentWinHand, const char* MsgText)
{
    MessageBox(ParentWinHand, MsgText, " Fehler...", MB_ICONHAND | MB_OK) ;
}

HWND Window_Create(HWND ParentWinHand, int PosX, int PosY, int GrX, int GrY, DWORD StyleFlags1, DWORD StyleFlags2, const char* Titel)
{
    int  ScreenGr;

    if (PosX < 0)
    {
        ScreenGr = GetSystemMetrics(SM_CXSCREEN);
        PosX = ScreenGr / 2 - GrX / 2;
    }
    if (PosY < 0)
    {
        ScreenGr = GetSystemMetrics(SM_CYSCREEN);
        PosY = ScreenGr / 2 - GrY / 2;
    }
    return CreateWindowEx(StyleFlags2, _FloLib1_WindowClass_LastName, Titel, StyleFlags1, PosX, PosY, GrX, GrY, ParentWinHand, NULL, _FloLib1_WindowClass_LastInstance, NULL);
}


void WindowClass_Register(const char* ClassName, HINSTANCE TheInstance, WNDPROC TheWinProc,
                          BOOL Use_Default_Icon, BOOL Use_Own_DC)
{
    WNDCLASSEX wndcls;

    if (TheInstance == NULL)
    {
        if (_FloLib1_WindowClass_LastInstance == NULL)
        {
            TheInstance = GetModuleHandle(NULL);
        }
        else
        {
            TheInstance = _FloLib1_WindowClass_LastInstance;
        }
    }

    wndcls.cbSize                  = sizeof(WNDCLASSEX);
    wndcls.style                   = CS_HREDRAW | CS_VREDRAW;
	if (Use_Own_DC)
	{
		// 'CS_OWNDC' => GetDC & BeginPaint always return the same DC
		wndcls.style |= CS_OWNDC;
	}
    wndcls.lpfnWndProc             = TheWinProc;
	// Alternative zu GWL_USERDATA: zugriff zum Zeiger mit Set/GetWindowLong(hwnd, 0);
    wndcls.cbClsExtra              = 0;
    wndcls.cbWndExtra              = 0;
    wndcls.hInstance               = TheInstance;
    if (Use_Default_Icon)
    {
    	wndcls.hIcon               = LoadIcon(NULL, IDI_APPLICATION);  // LoadIcon(ModuleInstance, MAKEINTRESOURCE(IconID));
    }
    else
    {
        wndcls.hIcon               = NULL;
    }
    wndcls.hCursor                 = LoadCursor(NULL, IDC_ARROW);
    wndcls.hbrBackground           = (HBRUSH)GetSysColorBrush(COLOR_BTNFACE);
    wndcls.lpszMenuName            = NULL;
    wndcls.lpszClassName           = ClassName;
    wndcls.hIconSm                 = NULL;
    
	RegisterClassEx(&wndcls);
	
    _FloLib1_WindowClass_LastName      = ClassName;
    _FloLib1_WindowClass_LastInstance  = TheInstance;
}



void WindowClass_UnRegister(const char* ClassName, HINSTANCE TheInstance)
{
    if (ClassName    == NULL) ClassName    = _FloLib1_WindowClass_LastName;
    if (TheInstance  == NULL) TheInstance  = _FloLib1_WindowClass_LastInstance;
    UnregisterClass(ClassName, TheInstance);
}

void Window_SetIcon(HWND WinHnd, int IconID)
{
    HICON       RetIcon        = NULL;
	HINSTANCE   ThisAppInst;   
	
	ThisAppInst = _FloLib1_WindowClass_LastInstance ? _FloLib1_WindowClass_LastInstance : GetModuleHandle(NULL);
	
    if (IconID)
	{
		// RetIcon = LoadIcon(ModuleInstance, MAKEINTRESOURCE(IconID));
		RetIcon = (HICON)LoadImage(ThisAppInst, MAKEINTRESOURCE(IconID), IMAGE_ICON, 0, 0, 0);
	}
    if (RetIcon)
    {
        SendMessage(WinHnd, WM_SETICON, ICON_SMALL, (LPARAM)RetIcon);
        SendMessage(WinHnd, WM_SETICON, ICON_BIG, (LPARAM)RetIcon);
        // SetClassLong(WinHnd, GCL_HICON, (LPARAM)RetIcon);
        DeleteObject(RetIcon);
    }
}

