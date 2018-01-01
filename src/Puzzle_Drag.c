
#include "StdInc.h"
#include "Puzzle.h"


int  ASSEMBLE_TOLERANCE  = 8;


static int Puzzle_Drag_Find_PieceChain(Struct_Puzzle* ThePuzzle, int MouseX, int MouseY)
{
	int                 Ret_Index;
	int                 ii_1;
	int                 ii_2;
	Struct_PieceChain*  AkChain;
	Struct_PieceInfo*   AkPiece;
	int                 Rect_Xm;
	int                 Rect_Xp;
	int                 Rect_Ym;
	int                 Rect_Yp;


	Ret_Index = PUZZLE_INVALID_INDEX;

	for (ii_1 = 0; ii_1 < ThePuzzle->nPieceChain; ++ii_1)
	{
		AkChain = ThePuzzle->aPieceChain + ii_1;

		for (ii_2 = 0; ii_2 < AkChain->nPieces; ++ii_2)
		{
			AkPiece = AkChain->aPieces + ii_2;

			Rect_Xm = AkChain->Position_X + AkPiece->Offset_X;
			Rect_Ym = AkChain->Position_Y + AkPiece->Offset_Y;
			Rect_Xp = Rect_Xm + ThePuzzle->Piece_Size_X;
			Rect_Yp = Rect_Ym + ThePuzzle->Piece_Size_Y;

			if (MouseX > Rect_Xm && MouseX < Rect_Xp &&
			    MouseY > Rect_Ym && MouseY < Rect_Yp)
			{
				Ret_Index = ii_1;
			}
		}
	}
	return Ret_Index;
}

static int Puzzle_Drag_Promote_PieceChain(Struct_Puzzle* ThePuzzle, int Index)
{
	int  Last_Index;

	Last_Index = ThePuzzle->nPieceChain - 1;

	while (Index < Last_Index)
	{
		PieceChain_Swap(ThePuzzle, Index, Index + 1);
		++Index;
	}
	return Last_Index;
}




static BOOL Puzzle_Drag_Check_Piece_Neighbor(Struct_Puzzle* ThePuzzle, Struct_PieceChain* Chain1, Struct_PieceChain* Chain2,
                                             Struct_PieceInfo* Piece1, Struct_PieceInfo* Piece2)
{
	int  DeltaX;
	int  DeltaY;

	if (Piece2->ID_Index_X == Piece1->ID_Index_X + 1 &&
	    Piece2->ID_Index_Y == Piece1->ID_Index_Y)
	{
		DeltaX = (Chain1->Position_X + Piece1->Offset_X + ThePuzzle->Piece_Size_X) - (Chain2->Position_X + Piece2->Offset_X);
		DeltaY = (Chain1->Position_Y + Piece1->Offset_Y) - (Chain2->Position_Y + Piece2->Offset_Y);

		if (abs(DeltaX) < ASSEMBLE_TOLERANCE &&
		    abs(DeltaY) < ASSEMBLE_TOLERANCE)
		{
			return TRUE;
		}
	}
	if (Piece1->ID_Index_X == Piece2->ID_Index_X + 1 &&
	    Piece1->ID_Index_Y == Piece2->ID_Index_Y)
	{
		DeltaX = (Chain2->Position_X + Piece2->Offset_X + ThePuzzle->Piece_Size_X) - (Chain1->Position_X + Piece1->Offset_X);
		DeltaY = (Chain2->Position_Y + Piece2->Offset_Y) - (Chain1->Position_Y + Piece1->Offset_Y);

		if (abs(DeltaX) < ASSEMBLE_TOLERANCE &&
		    abs(DeltaY) < ASSEMBLE_TOLERANCE)
		{
			return TRUE;
		}
	}

	if (Piece2->ID_Index_X == Piece1->ID_Index_X &&
	    Piece2->ID_Index_Y == Piece1->ID_Index_Y + 1)
	{
		DeltaX = (Chain1->Position_X + Piece1->Offset_X) - (Chain2->Position_X + Piece2->Offset_X);
		DeltaY = (Chain1->Position_Y + Piece1->Offset_Y + ThePuzzle->Piece_Size_Y) - (Chain2->Position_Y + Piece2->Offset_Y);

		if (abs(DeltaX) < ASSEMBLE_TOLERANCE &&
		    abs(DeltaY) < ASSEMBLE_TOLERANCE)
		{
			return TRUE;
		}
	}
	if (Piece1->ID_Index_X == Piece2->ID_Index_X &&
	    Piece1->ID_Index_Y == Piece2->ID_Index_Y + 1)
	{
		DeltaX = (Chain2->Position_X + Piece2->Offset_X) - (Chain1->Position_X + Piece1->Offset_X);
		DeltaY = (Chain2->Position_Y + Piece2->Offset_Y + ThePuzzle->Piece_Size_Y) - (Chain1->Position_Y + Piece1->Offset_Y);

		if (abs(DeltaX) < ASSEMBLE_TOLERANCE &&
		    abs(DeltaY) < ASSEMBLE_TOLERANCE)
		{
			return TRUE;
		}
	}

	return FALSE;
}



static BOOL Puzzle_Drag_Check_Chain_Neighbor(Struct_Puzzle* ThePuzzle, Struct_PieceChain* Chain1, Struct_PieceChain* Chain2)
{
	int                 ii_1;
	int                 ii_2;

	for (ii_1 = 0; ii_1 < Chain1->nPieces; ++ii_1)
	{
		for (ii_2 = 0; ii_2 < Chain2->nPieces; ++ii_2)
		{
			if (Puzzle_Drag_Check_Piece_Neighbor(ThePuzzle, Chain1, Chain2,
			                                     Chain1->aPieces + ii_1, Chain2->aPieces + ii_2))
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}


static int Puzzle_Drag_Find_Neighbor(Struct_Puzzle* ThePuzzle, int Ref_Chain_Index)
{
	int                 Ret_Index;
	int                 ii;
	Struct_PieceChain*  RefChain;
	Struct_PieceChain*  AkChain;


	Ret_Index = PUZZLE_INVALID_INDEX;
	RefChain  = ThePuzzle->aPieceChain + Ref_Chain_Index;

	for (ii = 0; ii < ThePuzzle->nPieceChain; ++ii)
	{
		if (ii != Ref_Chain_Index)
		{
			AkChain = ThePuzzle->aPieceChain + ii;

			if (Puzzle_Drag_Check_Chain_Neighbor(ThePuzzle, RefChain, AkChain))
			{
				Ret_Index = ii;
				break;
			}
		}
	}
	return Ret_Index;
}





void Puzzle_Drag_End(Struct_Puzzle* ThePuzzle, int MouseX, int MouseY)
{
	int  NeighborChain;

	if (ThePuzzle->Drag_Is_Active)
	{
		NeighborChain = Puzzle_Drag_Find_Neighbor(ThePuzzle, ThePuzzle->Drag_PieceChain_Index);
		if (NeighborChain >= 0)
		{
			Puzzle_Assemble_Chains(ThePuzzle, NeighborChain, ThePuzzle->Drag_PieceChain_Index);
			Puzz_Plugin_Tick_Down();
		}

		ThePuzzle->Drag_Is_Active = FALSE;
		ThePuzzle->Drag_PieceChain_Index = PUZZLE_INVALID_INDEX;

		Puzzle_Draw_Board(ThePuzzle);
		InvalidateRect(ThePuzzle->Board.hWND_Owner, NULL, FALSE);
	}

}


void Puzzle_Drag_Move(Struct_Puzzle* ThePuzzle, int MouseX, int MouseY)
{
	int                  MoveDeltaX;
	int                  MoveDeltaY;
	Struct_PieceChain*   Ak_PieceChain;


	if (ThePuzzle->Drag_Is_Active)
	{
		MoveDeltaX = MouseX - ThePuzzle->Drag_Start_X;
		MoveDeltaY = MouseY - ThePuzzle->Drag_Start_Y;

		if (MoveDeltaX || MoveDeltaY)
		{
			Ak_PieceChain = ThePuzzle->aPieceChain + ThePuzzle->Drag_PieceChain_Index;

			Ak_PieceChain->Position_X += MoveDeltaX;
			Ak_PieceChain->Position_Y += MoveDeltaY;

			ThePuzzle->Drag_Start_X = MouseX;
			ThePuzzle->Drag_Start_Y = MouseY;

			Puzzle_Draw_Board(ThePuzzle);
			InvalidateRect(ThePuzzle->Board.hWND_Owner, NULL, FALSE);
		}
	}
}


void Puzzle_Drag_Begin(Struct_Puzzle* ThePuzzle, int MouseX, int MouseY)
{
	int  ChainIndex;

	if (ThePuzzle->Drag_Is_Active == FALSE)
	{
		ChainIndex = Puzzle_Drag_Find_PieceChain(ThePuzzle, MouseX, MouseY);
		if (ChainIndex >= 0)
		{
			ThePuzzle->Drag_PieceChain_Index = Puzzle_Drag_Promote_PieceChain(ThePuzzle, ChainIndex);
			ThePuzzle->Drag_Is_Active        = TRUE;
			ThePuzzle->Drag_Start_X          = MouseX;
			ThePuzzle->Drag_Start_Y          = MouseY;
		}
	}
}




