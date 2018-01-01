
#include "StdInc.h"
#include "Bitmap.h"

// --------------------------------------------------------------------

void Bitmap_Flip_Y_Pixels(Struct_Bitmap_Data* Bmp, BYTE* Source, UINT SizeX, UINT SizeY)
{
    DWORD  StrideSize;
    DWORD  ImageSize;
    BYTE*  SourcePos;
    BYTE*  TargetPos;

    StrideSize = SizeX * sizeof(DWORD);
    ImageSize  = StrideSize * SizeY;

    Bmp->Size_X = SizeX;
    Bmp->Size_Y = SizeY;
    Bmp->Pixels = (DWORD*)malloc(ImageSize);

    SourcePos = Source + ImageSize;
    TargetPos = (BYTE*)Bmp->Pixels;

    while (SourcePos > Source)
    {
        SourcePos -= StrideSize;

        memcpy(TargetPos, SourcePos, StrideSize);

        TargetPos += StrideSize;
    }
}

// --------------------------------------------------------------------

void Bitmap_Init(Struct_Bitmap_Data* Bmp)
{
    Bmp->Size_X = 0;
    Bmp->Size_Y = 0;
    Bmp->Pixels = NULL;
}

void Bitmap_Release(Struct_Bitmap_Data* Bmp)
{
    if (Bmp->Pixels)
    {
        free(Bmp->Pixels);
    }
    Bitmap_Init(Bmp);
}

BOOL Bitmap_Load(Struct_Bitmap_Data* Bmp, const char* BmpFileName, int Max_Size_X, int Max_Size_Y)
{
	BOOL                 Load_Ret        = FALSE;
	int                  Shrink_Size_X;
	int                  Shrink_Size_Y;
	Struct_Bitmap_Data   Shrink_Bmp;
	
	
	// First Try WIC Interface
	Load_Ret = Bitmap_WIC_Load(BmpFileName, Bmp, FALSE);
	if (Load_Ret == FALSE)
	{
		// Then Try GDI-Plus
		Load_Ret = Bitmap_GDIP_Load(BmpFileName, Bmp, TRUE);
	}
	
	
	if (Load_Ret)
	{
		if (Bmp->Size_X > Max_Size_X || 
		    Bmp->Size_Y > Max_Size_Y)
		{
			Bitmap_Init(&Shrink_Bmp);
			
			Shrink_Size_X = Bmp->Size_X;
			Shrink_Size_Y = Bmp->Size_Y;
			
			if (Shrink_Size_X > Max_Size_X)
			{
				Shrink_Size_Y = MulDiv(Shrink_Size_Y, Max_Size_X, Shrink_Size_X);
				Shrink_Size_X = Max_Size_X;
			}
			if (Shrink_Size_Y > Max_Size_Y)
			{
				Shrink_Size_X = MulDiv(Shrink_Size_X, Max_Size_Y, Shrink_Size_Y);
				Shrink_Size_Y = Max_Size_Y;
			}

			Bitmap_Shrink(Bmp, &Shrink_Bmp, Shrink_Size_X, Shrink_Size_Y);
			
			Bitmap_Release(Bmp);
			Bmp->Size_X = Shrink_Bmp.Size_X;
			Bmp->Size_Y = Shrink_Bmp.Size_Y;
			Bmp->Pixels = Shrink_Bmp.Pixels;
		}
	}

    return Load_Ret;
}

// --------------------------------------------------------------------
