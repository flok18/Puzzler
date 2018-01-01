
#include "StdInc.h"
#include "Puzzle.h"


// ============================================================================


void Board_Release(Struct_Board* TheBoard)
{
	if (TheBoard->hDC_Mem)
	{
		if (TheBoard->hBMP_Orig)
		{
			SelectObject(TheBoard->hDC_Mem, TheBoard->hBMP_Orig);
		}
		if (TheBoard->hBMP_Mem)
		{
			DeleteObject(TheBoard->hBMP_Mem);
		}

		DeleteDC(TheBoard->hDC_Mem);
	}

	if (TheBoard->hDC_Win)
	{
		ReleaseDC(TheBoard->hWND_Owner, TheBoard->hDC_Win);
	}

	TheBoard->RGB_Mem      = NULL;
	TheBoard->hDC_Mem      = NULL;
	TheBoard->hDC_Win      = NULL;
	TheBoard->hBMP_Mem     = NULL;
	TheBoard->hBMP_Orig    = NULL;
	TheBoard->hWND_Owner   = NULL;
}


void Board_Create(Struct_Board* TheBoard, HWND Ownder_Window, int Size_X, int Size_Y)
{
	RECT             rcClient;
    BITMAPINFO       bmi;


	if (TheBoard->RGB_Mem) return;

    if (Size_X <= 0 || Size_Y <= 0)
    {
        GetClientRect(Ownder_Window, &rcClient);
        Size_X = rcClient.right  - rcClient.left;
        Size_Y = rcClient.bottom - rcClient.top;
    }

	memset(&bmi, 0, sizeof(BITMAPINFO));
    bmi.bmiHeader.biSize           = sizeof(BITMAPINFO);
    bmi.bmiHeader.biWidth          = Size_X;
    bmi.bmiHeader.biHeight         = -Size_Y;      // Order pixels from top to bottom
    bmi.bmiHeader.biPlanes         = 1;
    bmi.bmiHeader.biBitCount       = 32;           // last byte not used, 32 bit for alignment
    bmi.bmiHeader.biCompression    = BI_RGB;

    TheBoard->Size_X     = Size_X;
    TheBoard->Size_Y     = Size_Y;
    TheBoard->hWND_Owner = Ownder_Window;
    TheBoard->hDC_Win    = GetDC(Ownder_Window);
    TheBoard->hDC_Mem    = CreateCompatibleDC(TheBoard->hDC_Win);

    // Create DIB section to always give direct access to pixels
    TheBoard->hBMP_Mem  = CreateDIBSection(TheBoard->hDC_Win, &bmi, DIB_RGB_COLORS, (void**)&TheBoard->RGB_Mem, NULL, 0);
    TheBoard->hBMP_Orig = (HBITMAP)SelectObject(TheBoard->hDC_Mem, TheBoard->hBMP_Mem);
}

void Board_Update(Struct_Board* TheBoard, HDC Target_HDC)
{
	if (TheBoard->RGB_Mem)
	{
		BitBlt(Target_HDC, 0, 0, TheBoard->Size_X, TheBoard->Size_Y,
			   TheBoard->hDC_Mem, 0, 0, SRCCOPY);
	}
}



