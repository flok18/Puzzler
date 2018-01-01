

typedef struct {
	int               mData;
	int               nData;
	unsigned char*    aData;
	int               DataSize;
	int               SizeStep;
} LIST;

LIST*  LIST_Create(void);
void   LIST_Release(LIST* TheList);
void   LIST_Init(LIST* TheList, size_t Size_Of_Struct, int Size_Step);
void   LIST_Clear(LIST* TheList);
int    LIST_Count(LIST* TheList);
void*  LIST_Get(LIST* TheList, int List_Index);
void   LIST_Set(LIST* TheList, int List_Index, void* DataPtr);
void*  LIST_Enum(LIST* TheList, int* List_Counter);
void   LIST_Reset(LIST* TheList, int Num_Entries);
void*  LIST_Add(LIST* TheList, void* DataPtr);
void*  LIST_Insert(LIST* TheList, void* DataPtr, int Insert_Before);
void   LIST_Delete(LIST* TheList, int List_Index);
void   LIST_Swap(LIST* TheList, int List_Index_1, int List_Index_2);
void   LIST_Save(LIST* TheList, FILE* TheFile);
int    LIST_Load(LIST* TheList, FILE* TheFile);


// ============================================================================


typedef struct Config_File_s *Config_File;

typedef struct {
	const char*  Title;
	char*        Value;
} Config_File_Data;

void        Config_File_Free(Config_File MemBuff);
Config_File Config_File_Load(const char* FileName, Config_File_Data* Data_Table, int Num_Entries);

void Config_File_Start_Eval(Config_File_Data* Source, int Num_Entries);
void Config_File_Eval_Bool(BOOL* Target_Bool);
void Config_File_Eval_Int(int* Target_Int);
void Config_File_Eval_Double(double* Target_Double);
void Config_File_Eval_Static_String(const char** Target_Static_String);
void Config_File_Eval_String_Buffer(char* Target_String_Buffer, size_t String_Buffer_Size);


// ============================================================================


extern const char*  _FloLib1_WindowClass_LastName;
extern HINSTANCE    _FloLib1_WindowClass_LastInstance;

void MessageLoop(void);
void MsgBox_Error(HWND ParentWinHand, const char* MsgText);
HWND Window_Create(HWND ParentWinHand, int PosX, int PosY, int GrX, int GrY, DWORD StyleFlags1, DWORD StyleFlags2, const char* Titel);
void WindowClass_Register(const char* ClassName, HINSTANCE TheInstance, WNDPROC TheWinProc,
                          BOOL Use_Default_Icon, BOOL Use_Own_DC);
void WindowClass_UnRegister(const char* ClassName, HINSTANCE TheInstance);
void Window_SetIcon(HWND WinHnd, int IconID);


// ============================================================================


char* _stristr(const char* QuellString, const char* SuchString);
int sprintf_ss(char* Buffer, size_t BufferLen, const char* format, ...);
int strcpy_ss(char* dst, size_t max, const char* src);
int strcat_ss(char* dst, size_t max, const char* src);

HMENU Menu_Start(HWND WindowHandle);
HMENU Menu_Begin(const char* MenuName);
void Menu_Add(const char* MenuName, int MenuID);
void Menu_End(void);
int Menu_ShowPopup(HWND WindowHandle, HMENU MenuHandle);
int Menu_ShowPopupForWindow(HWND WindowHandle, HMENU MenuHandle);
void Menu_CheckItem(HMENU MenuHandle, int SubMenu_ID, int New_Status);

void SetFolderBackslash(char* FolderString, size_t FolderStringSize);
int Execute_FileSearch_Simple(const char* Pattern, void(*CallBackFnc)(WIN32_FIND_DATA*, char*,  int, void*), void* AnyPtr);
int IfFileExist(const char* FullFilePath);
BOOL DirectoryExists(const char* dirName);

