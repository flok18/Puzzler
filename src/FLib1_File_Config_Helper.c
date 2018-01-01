
#include "StdInc.h"
#include "FLib1.h"


static Config_File_Data*  	__Current_Config_Data  = NULL;
static int                  __Current_Config_Size  = 0;


void Config_File_Start_Eval(Config_File_Data* Source, int Num_Entries)
{
	__Current_Config_Data = Source;
	__Current_Config_Size = Num_Entries;
}

void Config_File_Eval_Bool(BOOL* Target_Bool)
{
	if (__Current_Config_Data && __Current_Config_Size)
	{
		if (__Current_Config_Data->Value)
		{
			switch (*__Current_Config_Data->Value)
			{
				case '1':
				case 'y': case 'Y':
				case 'j': case 'J':
				case 't': case 'T':
				case 'w': case 'W':
					*Target_Bool = TRUE;
					break;
				default:
					*Target_Bool = FALSE;
			}
		}
		++__Current_Config_Data;
		--__Current_Config_Size;
	}
}

void Config_File_Eval_Int(int* Target_Int)
{
	if (__Current_Config_Data && __Current_Config_Size)
	{
		if (__Current_Config_Data->Value)
		{
			*Target_Int = atoi(__Current_Config_Data->Value);
		}
		++__Current_Config_Data;
		--__Current_Config_Size;
	}
}

void Config_File_Eval_Double(double* Target_Double)
{
	if (__Current_Config_Data && __Current_Config_Size)
	{
		if (__Current_Config_Data->Value)
		{
			*Target_Double = atof(__Current_Config_Data->Value);
		}
		++__Current_Config_Data;
		--__Current_Config_Size;
	}
}

void Config_File_Eval_Static_String(const char** Target_Static_String)
{
	if (__Current_Config_Data && __Current_Config_Size)
	{
		if (__Current_Config_Data->Value)
		{
			*Target_Static_String = __Current_Config_Data->Value;
		}
		++__Current_Config_Data;
		--__Current_Config_Size;
	}
}

void Config_File_Eval_String_Buffer(char* Target_String_Buffer, size_t String_Buffer_Size)
{
	if (__Current_Config_Data && __Current_Config_Size)
	{
		if (__Current_Config_Data->Value)
		{
			strcpy_ss(Target_String_Buffer, String_Buffer_Size, __Current_Config_Data->Value);
		}
		++__Current_Config_Data;
		--__Current_Config_Size;
	}
}

