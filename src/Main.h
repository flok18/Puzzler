

extern HINSTANCE  Main_hInst;

extern int  WIN_PIC_SIZE_SPACE_X;
extern int  WIN_PIC_SIZE_SPACE_Y;


const char* FileList_Get_Random_File(void);
int FileList_Get_Count(void);
void FileList_Add(const char* CmdFile);
void FileList_Init(void);
void FileList_Release(void);

void Settings_Free(void);
void Settings_Load(void);

void MainWindow_Create(void);
void MainWindow_Release(void);




