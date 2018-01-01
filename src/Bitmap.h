

typedef struct {
    int              Size_X;
    int              Size_Y;
    DWORD*           Pixels;
} Struct_Bitmap_Data;


void Bitmap_Flip_Y_Pixels(Struct_Bitmap_Data* Bmp, BYTE* Source, UINT SizeX, UINT SizeY);
void Bitmap_Init(Struct_Bitmap_Data* Bmp);
void Bitmap_Release(Struct_Bitmap_Data* Bmp);
BOOL Bitmap_Load(Struct_Bitmap_Data* Bmp, const char* BmpFileName, int Max_Size_X, int Max_Size_Y);

BOOL Bitmap_GDIP_Load(const char* FileName, Struct_Bitmap_Data* Bmp, BOOL Do_Flip_Y);

BOOL Bitmap_WIC_Load(const char* FileName, Struct_Bitmap_Data* Bmp, BOOL Do_Flip_Y);

void Bitmap_Shrink(Struct_Bitmap_Data* FromBM, Struct_Bitmap_Data* ToBM, int Resize_To_X, int Resize_To_Y);
void Bitmap_Resize_Bilinear(Struct_Bitmap_Data* FromBM, Struct_Bitmap_Data* ToBM, int Resize_To_X, int Resize_To_Y);
void Bitmap_Resize_Nearest_Neighbor(Struct_Bitmap_Data* FromBM, Struct_Bitmap_Data* ToBM, int Resize_To_X, int Resize_To_Y);



