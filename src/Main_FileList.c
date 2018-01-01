
#include "StdInc.h"
#include "Flib1.h"
#include "Puzzle.h"
#include "Main.h"


typedef struct {
	char       FullFileName[MAX_PATH];
	int        FileNamelen;
} Struct_FileList;

static LIST Current_FileList;


// ============================================================================


const char* FileList_Get_Random_File(void)
{
	Struct_FileList*    ListEntry;
	int                 Num;
	int                 Index;


	Num = LIST_Count(&Current_FileList);
	if (Num > 0)
	{
		if (Num < 2)
		{
			Index = 0;
		}
		else
		{
			Index = Puzzle_Get_Random_Number(0, Num - 1);
		}

		ListEntry = LIST_Get(&Current_FileList, Index);
		if (ListEntry)
		{
			return ListEntry->FullFileName;
		}
	}

	return NULL;
}



int FileList_Get_Count(void)
{
	return LIST_Count(&Current_FileList);
}


// ============================================================================


static BOOL FileList_Check_Extension(const char* FileName, const char* Extensions, size_t ExtensionLen)
{
    char*   Found;

    Found = strrchr(FileName, '.');
    if (Found)
    {
        if (strlen(Found) == ExtensionLen)
        {
            if (_stristr(Extensions, Found))
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}

static void FileList_Add_Simple(const char* NewFileName)
{
    Struct_FileList*  ListEntry;

	if (NewFileName)
	{
		ListEntry = LIST_Add(&Current_FileList, NULL);

		strcpy_ss(ListEntry->FullFileName, MAX_PATH, NewFileName);
		ListEntry->FileNamelen = (int)strlen(NewFileName);
	}
}

static void FileList_Wild_Enum_CB(WIN32_FIND_DATA* AkFindResult, char* Folder,  int Count, void* UserData)
{
	char  FullPath[MAX_PATH];

	if (FileList_Check_Extension(AkFindResult->cFileName, ".bmp/.jpg/.png", 4))
	{
		strcpy_ss(FullPath, sizeof(FullPath), Folder);
		strcat_ss(FullPath, sizeof(FullPath), AkFindResult->cFileName);

		FileList_Add_Simple(FullPath);
	}
}


void FileList_Add(const char* CmdFile)
{
	char  TmpPattern[MAX_PATH];

	if (CmdFile)
    {
        if (*CmdFile)
        {
			if (DirectoryExists(CmdFile))
			{
				strcpy_ss(TmpPattern, sizeof(TmpPattern), CmdFile);
				SetFolderBackslash(TmpPattern, sizeof(TmpPattern));
				strcat_ss(TmpPattern, sizeof(TmpPattern), "*");
				Execute_FileSearch_Simple(TmpPattern, FileList_Wild_Enum_CB, NULL);
			}
			else if (IfFileExist(CmdFile))
			{
				FileList_Add_Simple(CmdFile);
			}
			else if (strchr(CmdFile, '*'))
			{
				Execute_FileSearch_Simple(CmdFile, FileList_Wild_Enum_CB, NULL);
			}
        }
    }
}

void FileList_Init(void)
{
	LIST_Init(&Current_FileList, sizeof(Struct_FileList), 64);
}

void FileList_Release(void)
{
    LIST_Clear(&Current_FileList);
}



