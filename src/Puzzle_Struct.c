
#include "StdInc.h"
#include "Puzzle.h"


unsigned int Puzzle_Background_Color_1 = RGB(184, 138, 92);
unsigned int Puzzle_Background_Color_2 = RGB(138, 184, 92);


// ============================================================================


void Puzzle_Init(Struct_Puzzle* ThePuzzle)
{
	memset(ThePuzzle, 0, sizeof(Struct_Puzzle));

	ThePuzzle->Drag_PieceChain_Index = PUZZLE_INVALID_INDEX;
}


// ============================================================================


static void PieceChain_Release(Struct_PieceChain* ThePieceChain)
{
	if (ThePieceChain->Image_Bitmap)
	{
		free(ThePieceChain->Image_Bitmap);
		ThePieceChain->Image_Bitmap = NULL;
	}

	if (ThePieceChain->aPieces)
	{
		free(ThePieceChain->aPieces);
		ThePieceChain->aPieces = NULL;
	}
	ThePieceChain->nPieces = 0;
}

void Puzzle_Release(Struct_Puzzle* ThePuzzle)
{
	int  ii;


	for (ii = 0; ii < ThePuzzle->nPieceChain; ++ii)
	{
		PieceChain_Release(ThePuzzle->aPieceChain + ii);
	}

	if (ThePuzzle->aPieceChain)
	{
		free(ThePuzzle->aPieceChain);
		ThePuzzle->aPieceChain = NULL;
	}
	ThePuzzle->nPieceChain = 0;

	Board_Release(&ThePuzzle->Board);
	
	ThePuzzle->Drag_PieceChain_Index = PUZZLE_INVALID_INDEX;
}


void Puzzle_Reset(Struct_Puzzle* ThePuzzle)
{
	int  ii;

	for (ii = 0; ii < ThePuzzle->nPieceChain; ++ii)
	{
		PieceChain_Release(ThePuzzle->aPieceChain + ii);
	}

	if (ThePuzzle->aPieceChain)
	{
		free(ThePuzzle->aPieceChain);
		ThePuzzle->aPieceChain = NULL;
	}
	ThePuzzle->nPieceChain = 0;
}


// ============================================================================


Struct_PieceChain* PieceChain_Add(Struct_Puzzle* ThePuzzle)
{
    Struct_PieceChain*   NewPtr;
	int                  NewIndex;

	NewIndex = ThePuzzle->nPieceChain;
    ++(ThePuzzle->nPieceChain);
    ThePuzzle->aPieceChain = (Struct_PieceChain*)realloc(ThePuzzle->aPieceChain, ThePuzzle->nPieceChain * sizeof(Struct_PieceChain));

    NewPtr = ThePuzzle->aPieceChain + NewIndex;
	memset(NewPtr, 0, sizeof(Struct_PieceChain));

    return NewPtr;
}

void PieceChain_Delete(Struct_Puzzle* ThePuzzle, int DelIndex)
{
	int   ii;

	PieceChain_Release(ThePuzzle->aPieceChain + DelIndex);

	--(ThePuzzle->nPieceChain);
	for (ii = DelIndex; ii < ThePuzzle->nPieceChain; ++ii)
	{
		memcpy(ThePuzzle->aPieceChain + ii, ThePuzzle->aPieceChain + ii + 1, sizeof(Struct_PieceChain));
	}
}

void PieceChain_Swap(Struct_Puzzle* ThePuzzle, int Index_1, int Index_2)
{
	Struct_PieceChain  TmpMem;

	if (Index_1 != Index_2)
	{
		memcpy(&TmpMem,                          ThePuzzle->aPieceChain + Index_1, sizeof(Struct_PieceChain));
		memcpy(ThePuzzle->aPieceChain + Index_1, ThePuzzle->aPieceChain + Index_2, sizeof(Struct_PieceChain));
		memcpy(ThePuzzle->aPieceChain + Index_2, &TmpMem,                          sizeof(Struct_PieceChain));
	}
}


// ============================================================================


void Puzzle_Create_Board(Struct_Puzzle* ThePuzzle, HWND Ownder_Window, int Size_X, int Size_Y)
{
	Board_Create(&ThePuzzle->Board, Ownder_Window, Size_X, Size_Y);

	ThePuzzle->Background_Color = Puzzle_Background_Color_2;
}


void Puzzle_Paint(Struct_Puzzle* ThePuzzle, HDC Target_HDC)
{
	Board_Update(&ThePuzzle->Board, Target_HDC);
}


void Puzzle_Draw_Board(Struct_Puzzle* ThePuzzle)
{
	int                  ii;
	Struct_PieceChain*   Ak_PieceChain;
	Union_Pixel*         Target_Buffer;
	int                  Target_Size_X;
	int                  Target_Size_Y;
	
	
	Target_Buffer = ThePuzzle->Board.RGB_Mem;
	Target_Size_X = ThePuzzle->Board.Size_X;
	Target_Size_Y = ThePuzzle->Board.Size_Y;
	
	if (Target_Buffer == NULL) return;

	
	Pixel_Erase_Background(Target_Buffer, Target_Size_X, Target_Size_Y, ThePuzzle->Background_Color);

	Ak_PieceChain = ThePuzzle->aPieceChain;
	for (ii = 0; ii < ThePuzzle->nPieceChain; ++ii)
	{
		Pixel_Transfer_Draw(Target_Buffer, Target_Size_X, Target_Size_Y,
							Ak_PieceChain->Image_Bitmap, Ak_PieceChain->Image_Size_X, Ak_PieceChain->Image_Size_Y,
							Ak_PieceChain->Position_X, Ak_PieceChain->Position_Y,
							0, 0, Ak_PieceChain->Image_Size_X, Ak_PieceChain->Image_Size_Y);
		++Ak_PieceChain;
	}
}


// ============================================================================


int Puzzle_Get_Random_Number(int Min, int Max)
{
    static BOOL  __static_Puzzle_Rand_Init = FALSE;
    int          Delta;
	int          RandVal;


    Delta = Max - Min;
    if (Delta == 0)
    {
        return Min;
    }
	else if (Delta < 0)
	{
#ifdef __RAND_ALLOW_SWAP
		Min   = Max;
		Delta = (-Delta);
#else
        return Max;
#endif
	}

    if (__static_Puzzle_Rand_Init == FALSE)
    {
		RandVal = abs((int)GetTickCount());

        srand((unsigned int)RandVal);

        __static_Puzzle_Rand_Init = TRUE;
    }
	else
	{
		RandVal = abs(rand());
	}

    Min += (RandVal % (Delta + 1));
    return Min;
}


