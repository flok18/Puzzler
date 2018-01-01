
#include "StdInc.h"
#include "Puzzle.h"


typedef void (*Plugin_Proto_1)(void);
typedef void (*Plugin_Proto_2)(HWND);


static HINSTANCE       Plugin_DLL_Handle     = NULL;
static Plugin_Proto_2  Plugin_Fnc_Init       = NULL;
static Plugin_Proto_1  Plugin_Fnc_Release    = NULL;
static Plugin_Proto_1  Plugin_Fnc_Tick_Down  = NULL;
static Plugin_Proto_1  Plugin_Fnc_Tick_Up    = NULL;
static Plugin_Proto_1  Plugin_Fnc_Stop       = NULL;
static Plugin_Proto_1  Plugin_Fnc_Start      = NULL;



static void Puzz_Plugin_Lib_Free(void)
{
    if (Plugin_DLL_Handle)
    {
        FreeLibrary(Plugin_DLL_Handle);
        Plugin_DLL_Handle = NULL;
    }

    Plugin_Fnc_Init       = NULL;
    Plugin_Fnc_Release    = NULL;
    Plugin_Fnc_Tick_Down  = NULL;
    Plugin_Fnc_Tick_Up    = NULL;
    Plugin_Fnc_Stop       = NULL;
    Plugin_Fnc_Start      = NULL;
}


static void Puzz_Plugin_Lib_Init(void)
{
    if (Plugin_DLL_Handle == NULL)
    {
        Plugin_DLL_Handle = LoadLibrary("Puzzler_Plugin.dll");
        if (Plugin_DLL_Handle)
        {
            Plugin_Fnc_Init      = (Plugin_Proto_2)GetProcAddress(Plugin_DLL_Handle, "Puzz_Plugin_Init");
            Plugin_Fnc_Release   = (Plugin_Proto_1)GetProcAddress(Plugin_DLL_Handle, "Puzz_Plugin_Release");
            Plugin_Fnc_Tick_Down = (Plugin_Proto_1)GetProcAddress(Plugin_DLL_Handle, "Puzz_Plugin_Tick_Down");
            Plugin_Fnc_Tick_Up   = (Plugin_Proto_1)GetProcAddress(Plugin_DLL_Handle, "Puzz_Plugin_Tick_Up");
            Plugin_Fnc_Stop      = (Plugin_Proto_1)GetProcAddress(Plugin_DLL_Handle, "Puzz_Plugin_Stop");
            Plugin_Fnc_Start     = (Plugin_Proto_1)GetProcAddress(Plugin_DLL_Handle, "Puzz_Plugin_Start");
        }
    }
}


void Puzz_Plugin_Tick_Down(void)
{
    if (Plugin_Fnc_Tick_Down)
    {
        Plugin_Fnc_Tick_Down();
    }
}

void Puzz_Plugin_Tick_Up(void)
{
    if (Plugin_Fnc_Tick_Up)
    {
        Plugin_Fnc_Tick_Up();
    }
}

void Puzz_Plugin_Stop(void)
{
    if (Plugin_Fnc_Stop)
    {
        Plugin_Fnc_Stop();
    }
}

void Puzz_Plugin_Start(void)
{
    if (Plugin_Fnc_Start)
    {
        Plugin_Fnc_Start();
    }
}

void Puzz_Plugin_Release(void)
{
    if (Plugin_Fnc_Release)
    {
        Plugin_Fnc_Release();
    }

    Puzz_Plugin_Lib_Free();
}

void Puzz_Plugin_Init(HWND ParentWin)
{
    Puzz_Plugin_Lib_Init();

    if (Plugin_Fnc_Init)
    {
        Plugin_Fnc_Init(ParentWin);
    }
}

