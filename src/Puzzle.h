

#define PUZZLE_INVALID_INDEX  (-1)


typedef union {
    unsigned int  iValue;
	struct {
		char R;
		char G;
		char B;
		char A;
	} RGBA;
} Union_Pixel;

typedef struct {

	int                 Offset_X;
	int                 Offset_Y;

	int                 ID_Index_X;
	int                 ID_Index_Y;

} Struct_PieceInfo;

typedef struct {

	int                 nPieces;
	Struct_PieceInfo*   aPieces;

	int                 Position_X;
	int                 Position_Y;

	int                 Image_Size_X;
	int                 Image_Size_Y;
	Union_Pixel*        Image_Bitmap;
} Struct_PieceChain;


typedef struct {
	Union_Pixel*        RGB_Mem;
	int                 Size_X;
	int                 Size_Y;
	HDC                 hDC_Mem;
	HDC                 hDC_Win;
	HBITMAP             hBMP_Mem;
	HBITMAP             hBMP_Orig;
	HWND                hWND_Owner;
} Struct_Board;


typedef struct {

	int                 nPieceChain;
	Struct_PieceChain*  aPieceChain;

	int                 Num_Pieces_X;
	int                 Num_Pieces_Y;
	int                 Piece_Size_X;
	int                 Piece_Size_Y;
	int                 Frame_Offset_X;
	int                 Frame_Offset_Y;
	unsigned int        Background_Color;

	Struct_Board        Board;
	
	int                 Drag_PieceChain_Index;
	BOOL                Drag_Is_Active;
	int                 Drag_Start_X;
	int                 Drag_Start_Y;

} Struct_Puzzle;



extern int  AUTO_PIECE_WIDTH;
extern int  AUTO_PIECE_HEIGHT;
extern int  ASSEMBLE_TOLERANCE;

extern unsigned int Puzzle_Background_Color_1;
extern unsigned int Puzzle_Background_Color_2;



void Puzzle_Snap_Chain_Position(Struct_Puzzle* ThePuzzle, Struct_PieceChain* Ref_Chain, Struct_PieceChain* Repos_Chain);
void Puzzle_Assemble_Chains(Struct_Puzzle* ThePuzzle, int Chain_Index_1, int Chain_Index_2);

void Puzzle_Create_Pieces(Struct_Puzzle* ThePuzzle, DWORD* Source_Pixel, int Source_Size_X, int Source_Size_Y);

void Puzzle_Drag_End(Struct_Puzzle* ThePuzzle, int MouseX, int MouseY);
void Puzzle_Drag_Move(Struct_Puzzle* ThePuzzle, int MouseX, int MouseY);
void Puzzle_Drag_Begin(Struct_Puzzle* ThePuzzle, int MouseX, int MouseY);

void Pixel_Erase_Background(Union_Pixel* To_Bitmap, int To_Size_X, int To_Size_Y, unsigned int Color_Value);
void Pixel_Set_Full_Transparent(Union_Pixel* To_Bitmap, int To_Size_X, int To_Size_Y);
void Pixel_Transfer_Copy(Union_Pixel* To_Bitmap,   int To_Size_X,   int To_Size_Y,
                         Union_Pixel* From_Bitmap, int From_Size_X, int From_Size_Y,
						 int Rect_To_X, int Rect_To_Y,
						 int Rect_From_X, int Rect_From_Y, int Rect_Size_X, int Rect_Size_Y);
void Pixel_Transfer_Draw(Union_Pixel* To_Bitmap,   int To_Size_X,   int To_Size_Y,
                         Union_Pixel* From_Bitmap, int From_Size_X, int From_Size_Y,
					     int Rect_To_X, int Rect_To_Y,
						 int Rect_From_X, int Rect_From_Y, int Rect_Size_X, int Rect_Size_Y);
void Pixel_Transfer_Assemble(Union_Pixel* To_Bitmap,   int To_Size_X,   int To_Size_Y,
                             Union_Pixel* From_Bitmap, int From_Size_X, int From_Size_Y,
					         int Rect_To_X, int Rect_To_Y,
						     int Rect_From_X, int Rect_From_Y, int Rect_Size_X, int Rect_Size_Y);

void Puzz_Plugin_Tick_Down(void);
void Puzz_Plugin_Tick_Up(void);
void Puzz_Plugin_Stop(void);
void Puzz_Plugin_Start(void);
void Puzz_Plugin_Release(void);
void Puzz_Plugin_Init(HWND ParentWin);

void Puzzle_Init(Struct_Puzzle* ThePuzzle);
void Puzzle_Release(Struct_Puzzle* ThePuzzle);
void Puzzle_Reset(Struct_Puzzle* ThePuzzle);
Struct_PieceChain* PieceChain_Add(Struct_Puzzle* ThePuzzle);
void PieceChain_Delete(Struct_Puzzle* ThePuzzle, int DelIndex);
void PieceChain_Swap(Struct_Puzzle* ThePuzzle, int Index_1, int Index_2);
void Puzzle_Create_Board(Struct_Puzzle* ThePuzzle, HWND Ownder_Window, int Size_X, int Size_Y);
void Puzzle_Paint(Struct_Puzzle* ThePuzzle, HDC Target_HDC);
void Puzzle_Draw_Board(Struct_Puzzle* ThePuzzle);
int  Puzzle_Get_Random_Number(int Min, int Max);

void Board_Release(Struct_Board* TheBoard);
void Board_Create(Struct_Board* TheBoard, HWND Ownder_Window, int Size_X, int Size_Y);
void Board_Update(Struct_Board* TheBoard, HDC Target_HDC);


