

#include "StdInc.h"
#include "Puzzle.h"



static int Puzzle_Assemble_Min_Val(int Val1, int Val2)
{
	if (Val1 < Val2)
	{
		return Val1;
	}
	return Val2;
}

static int Puzzle_Assemble_Max_Val(int Val1, int Val2)
{
	if (Val1 > Val2)
	{
		return Val1;
	}
	return Val2;
}


void Puzzle_Snap_Chain_Position(Struct_Puzzle* ThePuzzle, Struct_PieceChain* Ref_Chain, Struct_PieceChain* Repos_Chain)
{
	Struct_PieceInfo*  Ref_Piece;
	Struct_PieceInfo*  Repos_Piece;
	int                Soll_Delta_X;
	int                Soll_Delta_Y;
	int                Ist_Delta_X;
	int                Ist_Delta_Y;
	
	Ref_Piece   = Ref_Chain->aPieces;
	Repos_Piece = Repos_Chain->aPieces;

	Soll_Delta_X = Repos_Piece->ID_Index_X * ThePuzzle->Piece_Size_X - Ref_Piece->ID_Index_X * ThePuzzle->Piece_Size_X;
	Soll_Delta_Y = Repos_Piece->ID_Index_Y * ThePuzzle->Piece_Size_Y - Ref_Piece->ID_Index_Y * ThePuzzle->Piece_Size_Y;
	
	Ist_Delta_X = (Repos_Chain->Position_X + Repos_Piece->Offset_X) - (Ref_Chain->Position_X + Ref_Piece->Offset_X);
	Ist_Delta_Y = (Repos_Chain->Position_Y + Repos_Piece->Offset_Y) - (Ref_Chain->Position_Y + Ref_Piece->Offset_Y);
	
	Repos_Chain->Position_X += (Soll_Delta_X - Ist_Delta_X);
	Repos_Chain->Position_Y += (Soll_Delta_Y - Ist_Delta_Y);	
}




void Puzzle_Assemble_Chains(Struct_Puzzle* ThePuzzle, int Chain_Index_1, int Chain_Index_2)
{
	Struct_PieceChain*   Chain1;
	Struct_PieceChain*   Chain2;
	Struct_PieceChain*   Chain3;
	int                  ii;
	Struct_PieceInfo*    PieceInfo_Src;
	Struct_PieceInfo*    PieceInfo_Dest;
	

	Chain3 = PieceChain_Add(ThePuzzle);
	Chain1 = ThePuzzle->aPieceChain + Chain_Index_1;
	Chain2 = ThePuzzle->aPieceChain + Chain_Index_2;
	
	Puzzle_Snap_Chain_Position(ThePuzzle, Chain1, Chain2);

	Chain3->Position_X   = Puzzle_Assemble_Min_Val(Chain1->Position_X, Chain2->Position_X);
	Chain3->Position_Y   = Puzzle_Assemble_Min_Val(Chain1->Position_Y, Chain2->Position_Y);
	Chain3->Image_Size_X = Puzzle_Assemble_Max_Val(Chain1->Position_X + Chain1->Image_Size_X, Chain2->Position_X + Chain2->Image_Size_X) - Chain3->Position_X;
	Chain3->Image_Size_Y = Puzzle_Assemble_Max_Val(Chain1->Position_Y + Chain1->Image_Size_Y, Chain2->Position_Y + Chain2->Image_Size_Y) - Chain3->Position_Y;
	Chain3->Image_Bitmap = malloc(Chain3->Image_Size_X * Chain3->Image_Size_Y * sizeof(Union_Pixel));
	
	
	Pixel_Set_Full_Transparent(Chain3->Image_Bitmap, Chain3->Image_Size_X, Chain3->Image_Size_Y);
	
	Pixel_Transfer_Assemble(Chain3->Image_Bitmap, Chain3->Image_Size_X, Chain3->Image_Size_Y,
                            Chain1->Image_Bitmap, Chain1->Image_Size_X, Chain1->Image_Size_Y,
	     			        Chain1->Position_X - Chain3->Position_X, Chain1->Position_Y - Chain3->Position_Y,
						    0, 0, Chain1->Image_Size_X, Chain1->Image_Size_Y);

	Pixel_Transfer_Assemble(Chain3->Image_Bitmap, Chain3->Image_Size_X, Chain3->Image_Size_Y,
                            Chain2->Image_Bitmap, Chain2->Image_Size_X, Chain2->Image_Size_Y,
	     			        Chain2->Position_X - Chain3->Position_X, Chain2->Position_Y - Chain3->Position_Y,
						    0, 0, Chain2->Image_Size_X, Chain2->Image_Size_Y);

							
	Chain3->nPieces = Chain1->nPieces + Chain2->nPieces;
	Chain3->aPieces = malloc(Chain3->nPieces * sizeof(Struct_PieceInfo));

	
	PieceInfo_Dest = Chain3->aPieces;
	for (ii = 0; ii < Chain1->nPieces; ++ii)
	{
		PieceInfo_Src = Chain1->aPieces + ii;
		
		PieceInfo_Dest->Offset_X = Chain1->Position_X + PieceInfo_Src->Offset_X - Chain3->Position_X;
		PieceInfo_Dest->Offset_Y = Chain1->Position_Y + PieceInfo_Src->Offset_Y - Chain3->Position_Y;
		
		PieceInfo_Dest->ID_Index_X = PieceInfo_Src->ID_Index_X;
		PieceInfo_Dest->ID_Index_Y = PieceInfo_Src->ID_Index_Y;
		
		++PieceInfo_Dest;
	}
	for (ii = 0; ii < Chain2->nPieces; ++ii)
	{
		PieceInfo_Src = Chain2->aPieces + ii;
		
		PieceInfo_Dest->Offset_X = Chain2->Position_X + PieceInfo_Src->Offset_X - Chain3->Position_X;
		PieceInfo_Dest->Offset_Y = Chain2->Position_Y + PieceInfo_Src->Offset_Y - Chain3->Position_Y;

		PieceInfo_Dest->ID_Index_X = PieceInfo_Src->ID_Index_X;
		PieceInfo_Dest->ID_Index_Y = PieceInfo_Src->ID_Index_Y;

		++PieceInfo_Dest;
	}
	
	
	PieceChain_Delete(ThePuzzle, Chain_Index_1);
	if (Chain_Index_2 > Chain_Index_1) --Chain_Index_2;
	PieceChain_Delete(ThePuzzle, Chain_Index_2);
	
	if (ThePuzzle->nPieceChain == 1)
	{
		ThePuzzle->Background_Color = Puzzle_Background_Color_2;
		Puzz_Plugin_Stop();
	}
}



