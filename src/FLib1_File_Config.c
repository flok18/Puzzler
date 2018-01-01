

#ifndef _STANDALONE_ONE_SOURCE

	#include "StdInc.h"
	#include "FLib1.h"

#else

	typedef struct Config_File_s *Config_File;

	typedef struct {
		const char*  Title;
		char*        Value;
	} Config_File_Data;

#endif



static void __Remove_Current_Char_In_String(char* StringTo)
{
	char*  StringFrom = StringTo + 1;

	while (*StringFrom)
	{
		*StringTo = *StringFrom;

		++StringTo;
		++StringFrom;
	}

	*StringTo = '\0';
}


static char* __Trim_String_Left(char* String)
{
	while (*String == ' ')
	{
		++String;
	}
	return String;
}


static void __Trim_String_Right(char* String)
{
	char*  TrimPos  = String;

	while (*TrimPos)
	{
		++TrimPos;
	}

	while (TrimPos > String)
	{
		--TrimPos;

		if (*TrimPos == ' ')
		{
			*TrimPos = '\0';
		}
		else
		{
			break;
		}
	}
}


static char* __Get_Next_Cmd_Arg(char* String)
{
	if (*String == '"')
	{
		++String;
		while (*String)
		{
			if (*String == '"')
			{
				++String;
				break;
			}
			++String;
		}
	}
	else
	{
		while (*String)
		{
			if (*String <= ' ')
			{
				++String;
				break;
			}
			++String;
		}
	}
	return String;
}


static char* __Split_Next_Cmd_Arg(char** In_Out_String)
{
	char*  String  = *In_Out_String;

	if (*String == '"')
	{
		++String;
		*In_Out_String = String;

		while (*String)
		{
			if (*String == '"')
			{
				*String = '\0';
				++String;
				break;
			}
			else if (*String == '\\')
			{
				if (String[1] == '"')
				{
					__Remove_Current_Char_In_String(String);
				}
			}
			++String;
		}
	}
	else
	{
		while (*String)
		{
			if (*String <= ' ')
			{
				*String = '\0';
				++String;
				break;
			}
			++String;
		}
	}
	return String;
}


static void __PreProcess_Memory_String(char* MemoryString)
{
	while (*MemoryString)
	{
        switch (*MemoryString)
        {
            case '\t':
                *MemoryString = ' ';
                break;
            case '\r':
                *MemoryString = '\n';
                break;
		}
		++MemoryString;
	}
}


static Config_File_Data* __Find_In_Table(Config_File_Data* Data_Table, int Num_Entries, const char* String)
{
	while (Num_Entries > 0)
	{
		if (_stricmp(Data_Table->Title, String) == 0)
		{
			return Data_Table;
		}

		++Data_Table;
		--Num_Entries;
	}
	return NULL;
}


static char* __Load_Commandline_2_Memory(void)
{
	char*   CmdLine;
	char*   Memory          = NULL;
	size_t  MemorySize;

	CmdLine = GetCommandLine();
	CmdLine = __Get_Next_Cmd_Arg(CmdLine);

	if (CmdLine)
	{
		CmdLine = __Trim_String_Left(CmdLine);

		MemorySize = strlen(CmdLine);
		if (MemorySize)
		{
			++MemorySize;
			Memory = malloc((int)MemorySize);
			if (Memory)
			{
				memcpy(Memory, CmdLine, MemorySize);
			}
		}
	}
	return Memory;
}


static char* __Load_File_2_Memory(const char* FileName)
{
	HANDLE      FileHandle;
	DWORD       FileSize;
	DWORD       BytesRead     = 0;
	char*       Memory        = NULL;


	if (FileName)
	{
		if (*FileName)
		{
			// Datei Oeffnen
			FileHandle = CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
			if (FileHandle != INVALID_HANDLE_VALUE)
			{
				// Datei Groesse
				FileSize = GetFileSize(FileHandle, NULL);
				if (FileSize != 0 &&
					FileSize != 0xFFFFFFFF)
				{
					// Speicher Reservieren
					Memory = malloc((int)FileSize + 1);
					if (Memory)
                    {
                        // In Speicher Einlesen
                        ReadFile(FileHandle, Memory, FileSize, &BytesRead, NULL);
                        Memory[FileSize] = '\0';
                    }
				}

				// Datei Schliessen
				CloseHandle(FileHandle);
			}
		}
	}

    return Memory;
}


static void __Parse_Config_Pair(char* String1, char* String2, Config_File_Data* Data_Table, int Num_Entries)
{
	Config_File_Data*  Found_Entry;


	String1 = __Trim_String_Left(String1);
	__Trim_String_Right(String1);

	String2 = __Trim_String_Left(String2);
	__Trim_String_Right(String2);


	Found_Entry = __Find_In_Table(Data_Table, Num_Entries, String1);
	if (Found_Entry)
	{
		Found_Entry->Value = String2;
	}
}


static void __Parse_Config_Line_String(char* LineString, Config_File_Data* Data_Table, int Num_Entries)
{
	char*  FoundSplit;

	if (*LineString)
	{
		if (*LineString != ';')
		{
			FoundSplit = strchr(LineString, '=');
			if (FoundSplit)
			{
				*FoundSplit = '\0';
				++FoundSplit;

				__Parse_Config_Pair(LineString, FoundSplit, Data_Table, Num_Entries);
			}
		}
	}
}


static void __Parse_Config_File_Buffer(char* BufferString, Config_File_Data* Data_Table, int Num_Entries)
{
	char*  FoundSplit;

    for (;;)
    {
		FoundSplit = strchr(BufferString, '\n');

		if (FoundSplit)
		{
			*FoundSplit = '\0';
			__Parse_Config_Line_String(BufferString, Data_Table, Num_Entries);
			BufferString = FoundSplit + 1;
		}
		else
		{
			__Parse_Config_Line_String(BufferString, Data_Table, Num_Entries);
			break;
		}
    }
}


static void __Parse_Config_CmdLne_Buffer(char* BufferString, Config_File_Data* Data_Table, int Num_Entries)
{
	char*  FoundSplit;
	char*  FoundArgEnd;

    for (;;)
    {
		FoundSplit = strchr(BufferString, '=');

		if (FoundSplit)
		{
			*FoundSplit = '\0';
			++FoundSplit;

			FoundSplit  = __Trim_String_Left(FoundSplit);
			FoundArgEnd = __Split_Next_Cmd_Arg(&FoundSplit);

			__Parse_Config_Pair(BufferString, FoundSplit, Data_Table, Num_Entries);

			BufferString = FoundArgEnd;
		}
		else
		{
			break;
		}
    }
}


void Config_File_Free(Config_File MemBuff)
{
	if (MemBuff)
	{
		free(MemBuff);
		MemBuff = NULL;
	}
}


Config_File Config_File_Load(const char* FileName, Config_File_Data* Data_Table, int Num_Entries)
{
	char*   MemBuff   = NULL;

	if (FileName)
	{
		MemBuff = __Load_File_2_Memory(FileName);
		if (MemBuff)
		{
			__PreProcess_Memory_String(MemBuff);
			__Parse_Config_File_Buffer(MemBuff, Data_Table, Num_Entries);
		}
	}
	else
	{
		MemBuff = __Load_Commandline_2_Memory();
		if (MemBuff)
		{
			__PreProcess_Memory_String(MemBuff);
			__Parse_Config_CmdLne_Buffer(MemBuff, Data_Table, Num_Entries);
		}
	}
	return (Config_File)MemBuff;
}


