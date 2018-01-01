
#include "StdInc.h"
#include "FLib1.h"


void SetFolderBackslash(char* FolderString, size_t FolderStringSize)
{
	char*    TmpPtr;

	if (FolderString && FolderStringSize)
	{
		TmpPtr = strchr(FolderString, '\0');
		if (TmpPtr)
		{
			if (TmpPtr > FolderString &&
			    TmpPtr < FolderString + FolderStringSize - 1)
			{
				--TmpPtr;
				if (*TmpPtr != '\\')
				{
					++TmpPtr;
					*TmpPtr = '\\';
					++TmpPtr;
					*TmpPtr = '\0';
				}
			}
		}
	}
}


int Execute_FileSearch_Simple(const char* Pattern, void(*CallBackFnc)(WIN32_FIND_DATA*, char*,  int, void*), void* AnyPtr)
{
	int                FindCounter                    = 0;
	char               FolderOfPattern[MAX_PATH];
	char*              PosBS;
	size_t             NumCpy;
	HANDLE             AkSearchHandle;
	WIN32_FIND_DATA    AkFindResult;
	
	
	*FolderOfPattern = '\0';
	PosBS = strrchr(Pattern, '\\');
	if (PosBS)
	{
		NumCpy = PosBS - Pattern;
		++NumCpy;
		if (NumCpy < sizeof(FolderOfPattern))
		{
			memcpy(FolderOfPattern, Pattern, NumCpy);
			FolderOfPattern[NumCpy] = '\0';
		}
	}

	// Erster Eintrag suchen (wahrscheinlich "." oder "..")
	AkSearchHandle = FindFirstFile(Pattern, &AkFindResult);

	// Falls was gefunden...
	if (AkSearchHandle != INVALID_HANDLE_VALUE)
	{
		// Testen kein Verzeichnis
		if ((AkFindResult.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
			++FindCounter;
			if (CallBackFnc)
			{
				CallBackFnc(&AkFindResult, FolderOfPattern, FindCounter, AnyPtr);
			}
		}

		// Weitere Einträge abrufen
		while (FindNextFile(AkSearchHandle, &AkFindResult))
		{
			// Testen kein Verzeichnis
			if ((AkFindResult.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
			{
				++FindCounter;
				if (CallBackFnc)
				{
					CallBackFnc(&AkFindResult, FolderOfPattern, FindCounter, AnyPtr);
				}
			}
		}

		// Suche Schliessen
		FindClose(AkSearchHandle);
	}
	
	return FindCounter;
}


int IfFileExist(const char* FullFilePath)
{
	if (FullFilePath == NULL) return 0;
	if (GetFileAttributes(FullFilePath) == INVALID_FILE_ATTRIBUTES)
	{
    	return 0;
	}
	else
	{
	    return 1;
	}
}

BOOL DirectoryExists(const char* dirName)
{
	DWORD attribs = GetFileAttributes(dirName);

	if (attribs == INVALID_FILE_ATTRIBUTES)
	{
		return FALSE;
	}
	return (attribs & FILE_ATTRIBUTE_DIRECTORY);
}



