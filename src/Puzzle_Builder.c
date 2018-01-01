

#include "StdInc.h"
#include "Puzzle.h"



int  AUTO_PIECE_WIDTH             = 100;
int  AUTO_PIECE_HEIGHT            = 100;

#define  CONNECTOR_SIZE_LEN_QUOT      6
#define  CONNECTOR_SIZE_WEIGHT_QUOT   8



static void Puzzle_Build_Connector_Ver(Struct_Puzzle* ThePuzzle, Struct_PieceChain* ThePieceChain, int Side, int IsMale)
{
	int           Center_Y;
	int           Len_X;
	int           Black_Limit_Xm;
	int           Black_Limit_Ym;
	int           Black_Limit_Xp;
	int           Black_Limit_Yp;
	int           ii_x;
	int           ii_y;
	Union_Pixel*  To_Pix;


	Center_Y = (ThePuzzle->Piece_Size_Y + ThePuzzle->Frame_Offset_Y * 2) / 2;
	Black_Limit_Ym = Center_Y - ThePuzzle->Piece_Size_Y / CONNECTOR_SIZE_WEIGHT_QUOT;
	Black_Limit_Yp = Center_Y + ThePuzzle->Piece_Size_Y / CONNECTOR_SIZE_WEIGHT_QUOT;

	Len_X = ThePuzzle->Frame_Offset_X - 2;

	if (Side)
	{
		if (IsMale)
		{
			Black_Limit_Xm = ThePuzzle->Frame_Offset_X + ThePuzzle->Piece_Size_X + 1;
			Black_Limit_Xp = Black_Limit_Xm + Len_X;

			for (ii_y = Black_Limit_Ym; ii_y <= Black_Limit_Yp; ++ii_y)
			{
				for (ii_x = Black_Limit_Xm; ii_x <= Black_Limit_Xp; ++ii_x)
				{
					To_Pix = ThePieceChain->Image_Bitmap + (ii_y * ThePieceChain->Image_Size_X + ii_x);

					if (ii_y == Black_Limit_Ym || ii_y == Black_Limit_Yp)
					{
						To_Pix->RGBA.A = 1;
					}
					else if (ii_x == Black_Limit_Xp)
					{
						To_Pix->RGBA.A = 1;
					}
					else
					{
						To_Pix->RGBA.A = 0;
					}
				}
			}
		}
		else
		{
			Black_Limit_Xp = ThePuzzle->Frame_Offset_X + ThePuzzle->Piece_Size_X + 1;
			Black_Limit_Xm = Black_Limit_Xp - Len_X;

			for (ii_y = Black_Limit_Ym; ii_y <= Black_Limit_Yp; ++ii_y)
			{
				for (ii_x = Black_Limit_Xm; ii_x <= Black_Limit_Xp; ++ii_x)
				{
					To_Pix = ThePieceChain->Image_Bitmap + (ii_y * ThePieceChain->Image_Size_X + ii_x);

					if (ii_y == Black_Limit_Ym || ii_y == Black_Limit_Yp)
					{
						To_Pix->RGBA.A = 1;
					}
					else if (ii_x == Black_Limit_Xm)
					{
						To_Pix->RGBA.A = 1;
					}
					else
					{
						To_Pix->RGBA.A = 2;
					}
				}
			}
		}
	}
	else
	{
		if (IsMale)
		{
			Black_Limit_Xp = ThePuzzle->Frame_Offset_X - 1;
			Black_Limit_Xm = Black_Limit_Xp - Len_X;

			for (ii_y = Black_Limit_Ym; ii_y <= Black_Limit_Yp; ++ii_y)
			{
				for (ii_x = Black_Limit_Xm; ii_x <= Black_Limit_Xp; ++ii_x)
				{
					To_Pix = ThePieceChain->Image_Bitmap + (ii_y * ThePieceChain->Image_Size_X + ii_x);

					if (ii_y == Black_Limit_Ym || ii_y == Black_Limit_Yp)
					{
						To_Pix->RGBA.A = 1;
					}
					else if (ii_x == Black_Limit_Xm)
					{
						To_Pix->RGBA.A = 1;
					}
					else
					{
						To_Pix->RGBA.A = 0;
					}
				}
			}
		}
		else
		{
			Black_Limit_Xm = ThePuzzle->Frame_Offset_X - 1;
			Black_Limit_Xp = Black_Limit_Xm + Len_X;

			for (ii_y = Black_Limit_Ym; ii_y <= Black_Limit_Yp; ++ii_y)
			{
				for (ii_x = Black_Limit_Xm; ii_x <= Black_Limit_Xp; ++ii_x)
				{
					To_Pix = ThePieceChain->Image_Bitmap + (ii_y * ThePieceChain->Image_Size_X + ii_x);

					if (ii_y == Black_Limit_Ym || ii_y == Black_Limit_Yp)
					{
						To_Pix->RGBA.A = 1;
					}
					else if (ii_x == Black_Limit_Xp)
					{
						To_Pix->RGBA.A = 1;
					}
					else
					{
						To_Pix->RGBA.A = 2;
					}
				}
			}
		}
	}
}

static void Puzzle_Build_Connector_Hor(Struct_Puzzle* ThePuzzle, Struct_PieceChain* ThePieceChain, int Side, int IsMale)
{
	int           Center_X;
	int           Len_Y;
	int           Black_Limit_Xm;
	int           Black_Limit_Ym;
	int           Black_Limit_Xp;
	int           Black_Limit_Yp;
	int           ii_x;
	int           ii_y;
	Union_Pixel*  To_Pix;


	Center_X = (ThePuzzle->Piece_Size_X + ThePuzzle->Frame_Offset_X * 2) / 2;
	Black_Limit_Xm = Center_X - ThePuzzle->Piece_Size_X / CONNECTOR_SIZE_WEIGHT_QUOT;
	Black_Limit_Xp = Center_X + ThePuzzle->Piece_Size_X / CONNECTOR_SIZE_WEIGHT_QUOT;

	Len_Y = ThePuzzle->Frame_Offset_Y - 2;

	if (Side)
	{
		if (IsMale)
		{
			Black_Limit_Ym = ThePuzzle->Frame_Offset_Y + ThePuzzle->Piece_Size_Y + 1;
			Black_Limit_Yp = Black_Limit_Ym + Len_Y;

			for (ii_y = Black_Limit_Ym; ii_y <= Black_Limit_Yp; ++ii_y)
			{
				for (ii_x = Black_Limit_Xm; ii_x <= Black_Limit_Xp; ++ii_x)
				{
					To_Pix = ThePieceChain->Image_Bitmap + (ii_y * ThePieceChain->Image_Size_X + ii_x);

					if (ii_x == Black_Limit_Xm || ii_x == Black_Limit_Xp)
					{
						To_Pix->RGBA.A = 1;
					}
					else if (ii_y == Black_Limit_Yp)
					{
						To_Pix->RGBA.A = 1;
					}
					else
					{
						To_Pix->RGBA.A = 0;
					}
				}
			}
		}
		else
		{
			Black_Limit_Yp = ThePuzzle->Frame_Offset_Y + ThePuzzle->Piece_Size_Y + 1;
			Black_Limit_Ym = Black_Limit_Yp - Len_Y;

			for (ii_y = Black_Limit_Ym; ii_y <= Black_Limit_Yp; ++ii_y)
			{
				for (ii_x = Black_Limit_Xm; ii_x <= Black_Limit_Xp; ++ii_x)
				{
					To_Pix = ThePieceChain->Image_Bitmap + (ii_y * ThePieceChain->Image_Size_X + ii_x);

					if (ii_x == Black_Limit_Xm || ii_x == Black_Limit_Xp)
					{
						To_Pix->RGBA.A = 1;
					}
					else if (ii_y == Black_Limit_Ym)
					{
						To_Pix->RGBA.A = 1;
					}
					else
					{
						To_Pix->RGBA.A = 2;
					}
				}
			}
		}
	}
	else
	{
		if (IsMale)
		{
			Black_Limit_Yp = ThePuzzle->Frame_Offset_Y - 1;
			Black_Limit_Ym = Black_Limit_Yp - Len_Y;

			for (ii_y = Black_Limit_Ym; ii_y <= Black_Limit_Yp; ++ii_y)
			{
				for (ii_x = Black_Limit_Xm; ii_x <= Black_Limit_Xp; ++ii_x)
				{
					To_Pix = ThePieceChain->Image_Bitmap + (ii_y * ThePieceChain->Image_Size_X + ii_x);

					if (ii_x == Black_Limit_Xm || ii_x == Black_Limit_Xp)
					{
						To_Pix->RGBA.A = 1;
					}
					else if (ii_y == Black_Limit_Ym)
					{
						To_Pix->RGBA.A = 1;
					}
					else
					{
						To_Pix->RGBA.A = 0;
					}
				}
			}
		}
		else
		{
			Black_Limit_Ym = ThePuzzle->Frame_Offset_Y - 1;
			Black_Limit_Yp = Black_Limit_Ym + Len_Y;

			for (ii_y = Black_Limit_Ym; ii_y <= Black_Limit_Yp; ++ii_y)
			{
				for (ii_x = Black_Limit_Xm; ii_x <= Black_Limit_Xp; ++ii_x)
				{
					To_Pix = ThePieceChain->Image_Bitmap + (ii_y * ThePieceChain->Image_Size_X + ii_x);

					if (ii_x == Black_Limit_Xm || ii_x == Black_Limit_Xp)
					{
						To_Pix->RGBA.A = 1;
					}
					else if (ii_y == Black_Limit_Yp)
					{
						To_Pix->RGBA.A = 1;
					}
					else
					{
						To_Pix->RGBA.A = 2;
					}
				}
			}
		}
	}
}



static void Puzzle_Build_Alpha_Frame_Rect(Struct_Puzzle* ThePuzzle, Struct_PieceChain* ThePieceChain)
{
	int           Black_Limit_Xm;
	int           Black_Limit_Ym;
	int           Black_Limit_Xp;
	int           Black_Limit_Yp;
	int           ii_x;
	int           ii_y;
	Union_Pixel*  To_Pix;


	Black_Limit_Xm = ThePuzzle->Frame_Offset_X - 1;
	Black_Limit_Xp = ThePuzzle->Frame_Offset_X + ThePuzzle->Piece_Size_X + 1;
	Black_Limit_Ym = ThePuzzle->Frame_Offset_Y - 1;
	Black_Limit_Yp = ThePuzzle->Frame_Offset_Y + ThePuzzle->Piece_Size_Y + 1;
	To_Pix         = ThePieceChain->Image_Bitmap;

	for (ii_y = 0; ii_y < ThePieceChain->Image_Size_Y; ++ii_y)
	{
		for (ii_x = 0; ii_x < ThePieceChain->Image_Size_X; ++ii_x)
		{
			if (ii_x < Black_Limit_Xm || ii_x > Black_Limit_Xp ||
			    ii_y < Black_Limit_Ym || ii_y > Black_Limit_Yp)
			{
				To_Pix->RGBA.A = 2;
			}
			else if (ii_x == Black_Limit_Xm || ii_x == Black_Limit_Xp ||
			         ii_y == Black_Limit_Ym || ii_y == Black_Limit_Yp)
			{
				To_Pix->RGBA.A = 1;
			}
			else
			{
				To_Pix->RGBA.A = 0;
			}

			++To_Pix;
		}
	}
}


void Puzzle_Create_Pieces(Struct_Puzzle* ThePuzzle, DWORD* Source_Pixel, int Source_Size_X, int Source_Size_Y)
{
	int                  Num_Pieces;
	int                  Half_Error_Pixels_X;
	int                  Half_Error_Pixels_Y;
	int                  ii_x;
	int                  ii_y;
	Struct_PieceChain*   Ak_PieceChain;
	Struct_PieceInfo*    Ak_PieceInfo;
	int                  Connector_Typ;


	Puzzle_Reset(ThePuzzle);

	ThePuzzle->Background_Color = Puzzle_Background_Color_1;

	ThePuzzle->Num_Pieces_X = (Source_Size_X + (AUTO_PIECE_WIDTH  / 2)) / AUTO_PIECE_WIDTH;
	ThePuzzle->Num_Pieces_Y = (Source_Size_Y + (AUTO_PIECE_HEIGHT / 2)) / AUTO_PIECE_HEIGHT;

	if (ThePuzzle->Num_Pieces_X < 1) ThePuzzle->Num_Pieces_X = 1;
	if (ThePuzzle->Num_Pieces_Y < 1) ThePuzzle->Num_Pieces_Y = 1;

	ThePuzzle->Piece_Size_X = Source_Size_X / ThePuzzle->Num_Pieces_X + 1;
	ThePuzzle->Piece_Size_Y = Source_Size_Y / ThePuzzle->Num_Pieces_Y + 1;

	ThePuzzle->Frame_Offset_X = ThePuzzle->Piece_Size_X / CONNECTOR_SIZE_LEN_QUOT + 1;
	ThePuzzle->Frame_Offset_Y = ThePuzzle->Piece_Size_Y / CONNECTOR_SIZE_LEN_QUOT + 1;

	Half_Error_Pixels_X = (ThePuzzle->Num_Pieces_X * ThePuzzle->Piece_Size_X - Source_Size_X) / 2;
	Half_Error_Pixels_Y = (ThePuzzle->Num_Pieces_Y * ThePuzzle->Piece_Size_Y - Source_Size_Y) / 2;


	Num_Pieces = ThePuzzle->Num_Pieces_X * ThePuzzle->Num_Pieces_Y;

	ThePuzzle->nPieceChain = Num_Pieces;
	ThePuzzle->aPieceChain = malloc(Num_Pieces * sizeof(Struct_PieceChain));
	memset(ThePuzzle->aPieceChain, 0, Num_Pieces * sizeof(Struct_PieceChain));


	Ak_PieceChain = ThePuzzle->aPieceChain;
	for (ii_y = 0; ii_y < ThePuzzle->Num_Pieces_Y; ++ii_y)
	{
		for (ii_x = 0; ii_x < ThePuzzle->Num_Pieces_X; ++ii_x)
		{
			Ak_PieceChain->nPieces = 1;
			Ak_PieceChain->aPieces = malloc(Ak_PieceChain->nPieces * sizeof(Struct_PieceInfo));
			Ak_PieceInfo = Ak_PieceChain->aPieces;

			Ak_PieceInfo->Offset_X   = ThePuzzle->Frame_Offset_X;
			Ak_PieceInfo->Offset_Y   = ThePuzzle->Frame_Offset_Y;
			Ak_PieceInfo->ID_Index_X = ii_x;
			Ak_PieceInfo->ID_Index_Y = ii_y;

			Ak_PieceChain->Image_Size_X = ThePuzzle->Piece_Size_X + 2 * ThePuzzle->Frame_Offset_X;
			Ak_PieceChain->Image_Size_Y = ThePuzzle->Piece_Size_Y + 2 * ThePuzzle->Frame_Offset_Y;
			Ak_PieceChain->Image_Bitmap = malloc(Ak_PieceChain->Image_Size_X * Ak_PieceChain->Image_Size_Y * sizeof(Union_Pixel));

			Ak_PieceChain->Position_X = Puzzle_Get_Random_Number(1, ThePuzzle->Board.Size_X - Ak_PieceChain->Image_Size_X - 1);
			Ak_PieceChain->Position_Y = Puzzle_Get_Random_Number(1, ThePuzzle->Board.Size_Y - Ak_PieceChain->Image_Size_Y - 1);

			Pixel_Erase_Background(Ak_PieceChain->Image_Bitmap, Ak_PieceChain->Image_Size_X, Ak_PieceChain->Image_Size_Y, ThePuzzle->Background_Color);

			Pixel_Transfer_Copy(Ak_PieceChain->Image_Bitmap, Ak_PieceChain->Image_Size_X, Ak_PieceChain->Image_Size_Y,
								(Union_Pixel*)Source_Pixel, Source_Size_X, Source_Size_Y,
								0, 0,
								ii_x * ThePuzzle->Piece_Size_X - ThePuzzle->Frame_Offset_X - Half_Error_Pixels_X,
								ii_y * ThePuzzle->Piece_Size_Y - ThePuzzle->Frame_Offset_Y - Half_Error_Pixels_Y,
								Ak_PieceChain->Image_Size_X, Ak_PieceChain->Image_Size_Y);

			Puzzle_Build_Alpha_Frame_Rect(ThePuzzle, Ak_PieceChain);

			++Ak_PieceChain;
		}
	}


	for (ii_y = 0; ii_y < ThePuzzle->Num_Pieces_Y; ++ii_y)
	{
		for (ii_x = 1; ii_x < ThePuzzle->Num_Pieces_X; ++ii_x)
		{
			Ak_PieceChain = ThePuzzle->aPieceChain + (ii_y * ThePuzzle->Num_Pieces_X + ii_x);
			Connector_Typ = Puzzle_Get_Random_Number(0, 1);

			Puzzle_Build_Connector_Ver(ThePuzzle, Ak_PieceChain, 0, Connector_Typ);

			--Ak_PieceChain;
			Connector_Typ ^= 1;

			Puzzle_Build_Connector_Ver(ThePuzzle, Ak_PieceChain, 1, Connector_Typ);
		}
	}


	for (ii_x = 0; ii_x < ThePuzzle->Num_Pieces_X; ++ii_x)
	{
		for (ii_y = 1; ii_y < ThePuzzle->Num_Pieces_Y; ++ii_y)
		{
			Ak_PieceChain = ThePuzzle->aPieceChain + (ii_y * ThePuzzle->Num_Pieces_X + ii_x);
			Connector_Typ = Puzzle_Get_Random_Number(0, 1);

			Puzzle_Build_Connector_Hor(ThePuzzle, Ak_PieceChain, 0, Connector_Typ);

			Ak_PieceChain -= ThePuzzle->Num_Pieces_X;
			Connector_Typ ^= 1;

			Puzzle_Build_Connector_Hor(ThePuzzle, Ak_PieceChain, 1, Connector_Typ);
		}
	}

}



