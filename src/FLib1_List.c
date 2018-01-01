
#include "StdInc.h"
#include "FLib1.h"



LIST* LIST_Create(void)
{
	LIST*  NewList;
	
	NewList = (LIST*)malloc(sizeof(LIST));
	memset(NewList, 0, sizeof(LIST));
	
	return NewList;
}


void LIST_Release(LIST* TheList)
{
	if (TheList)
	{
		if (TheList->aData)
		{
			free(TheList->aData);
		}
		free(TheList);
	}
}


void LIST_Init(LIST* TheList, size_t Size_Of_Struct, int Size_Step)
{
	TheList->mData     = 0;
	TheList->nData     = 0;
	TheList->aData     = NULL;
	TheList->DataSize  = (int)Size_Of_Struct;

	if (Size_Step > 0)
	{
		TheList->SizeStep  = Size_Step;
	}
	else
	{
		TheList->SizeStep  = 32;
	}
}


void LIST_Clear(LIST* TheList)
{
	if (TheList->aData)
	{
		free(TheList->aData);
	}
		
	TheList->mData     = 0;
	TheList->nData     = 0;
	TheList->aData     = NULL;		
}


int LIST_Count(LIST* TheList)
{
	return TheList->nData;
}


void* LIST_Get(LIST* TheList, int List_Index)
{
    if (List_Index < TheList->nData)
	{
        return (void*)(TheList->aData + (List_Index * TheList->DataSize));
    }
    return NULL;
}


void LIST_Set(LIST* TheList, int List_Index, void* DataPtr)
{
	unsigned char*   NewPtr;

    if (List_Index < TheList->nData)
	{
		NewPtr = TheList->aData + (List_Index * TheList->DataSize);
		if (DataPtr)
		{
			memcpy(NewPtr, DataPtr, TheList->DataSize);
		}
		else
		{
			memset(NewPtr, '\0', TheList->DataSize);		
		}
	}
}


void* LIST_Enum(LIST* TheList, int* List_Counter)
{
	int    CurCounter   = *List_Counter;
	void*  RetVal       = NULL;

	RetVal = LIST_Get(TheList, CurCounter);

    if (RetVal)
    {
        ++CurCounter;
        *List_Counter = CurCounter;
    }

	return RetVal;
}


void LIST_Reset(LIST* TheList, int Num_Entries)
{
	LIST_Clear(TheList);
	
	if (Num_Entries > 0)
	{
		TheList->nData = Num_Entries;
		TheList->mData = Num_Entries;
		TheList->aData = (unsigned char*)malloc(Num_Entries * TheList->DataSize);

		memset(TheList->aData, '\0', Num_Entries * TheList->DataSize);
	}
}


static void __LIST_Internal_Increment(LIST* TheList)
{
	size_t MemoryBytes;

    ++TheList->nData;

	if (TheList->nData > TheList->mData)
	{
		TheList->mData += TheList->SizeStep;

		MemoryBytes = (size_t)(TheList->mData * TheList->DataSize);

		if (TheList->aData)
		{
			TheList->aData = (unsigned char*)realloc(TheList->aData, MemoryBytes);
		}
		else
		{
			TheList->aData = (unsigned char*)malloc(MemoryBytes);
		}
	}
}


void* LIST_Add(LIST* TheList, void* DataPtr)
{
	__LIST_Internal_Increment(TheList);

	LIST_Set(TheList, TheList->nData - 1, DataPtr);

	return LIST_Get(TheList, TheList->nData - 1);
}


void* LIST_Insert(LIST* TheList, void* DataPtr, int Insert_Before)
{
    int               AnzMove;
    unsigned char*    DataTarget;
    unsigned char*    DataSource;

	// Parameter ggf. Korrigieren
	if (Insert_Before < 0 || Insert_Before > TheList->nData)
	{
		Insert_Before = TheList->nData;
	}

	// Anzahl zu Verschiebende Bloecke
	AnzMove = TheList->nData - Insert_Before;

	// Speicher Vergroessern
	__LIST_Internal_Increment(TheList);

	// Luecke Erstellen, Daten ueberspielen
	DataTarget = TheList->aData + ((TheList->nData - 1) * TheList->DataSize);
	DataSource = DataTarget - TheList->DataSize;

	while (AnzMove > 0)
    {
        memcpy(DataTarget, DataSource, TheList->DataSize);

		DataTarget -= TheList->DataSize;
		DataSource -= TheList->DataSize;

		--AnzMove;
    }

	LIST_Set(TheList, Insert_Before, DataPtr);

	return LIST_Get(TheList, Insert_Before);
}


void LIST_Delete(LIST* TheList, int List_Index)
{
    int               AnzMove;
    unsigned char*    DataTarget;
    unsigned char*    DataSource;

    if (List_Index < 0 ||
	    List_Index >= TheList->nData) return;

	// Löschen
	--(TheList->nData);
	AnzMove = TheList->nData - List_Index;

	DataTarget = TheList->aData + (List_Index * TheList->DataSize);
	DataSource = DataTarget + TheList->DataSize;

	while (AnzMove > 0)
	{
		memcpy(DataTarget, DataSource, TheList->DataSize);

		DataTarget += TheList->DataSize;
		DataSource += TheList->DataSize;

		--AnzMove;
	}
}


void LIST_Swap(LIST* TheList, int List_Index_1, int List_Index_2)
{
	int               HasMemoryAllocated;
    unsigned char*    Data1;
    unsigned char*    Data2;
	unsigned char*    Data3;

	
	if (TheList->nData < TheList->mData)
	{
		HasMemoryAllocated = 0;
		Data3 = TheList->aData + (TheList->nData * TheList->DataSize);
	}
	else
	{
		HasMemoryAllocated = 1;
		Data3 = (unsigned char*)malloc(TheList->DataSize);
	}

	Data1 = TheList->aData + (List_Index_1 * TheList->DataSize);
	Data2 = TheList->aData + (List_Index_2 * TheList->DataSize);

	memcpy(Data3, Data1, TheList->DataSize);
	memcpy(Data1, Data2, TheList->DataSize);
	memcpy(Data2, Data3, TheList->DataSize);

	if (HasMemoryAllocated)
	{
		free(Data3);
	}
}


void LIST_Save(LIST* TheList, FILE* TheFile)
{
	fwrite(&TheList->nData,    sizeof(int), 1, TheFile);
	fwrite(&TheList->DataSize, sizeof(int), 1, TheFile);

	if (TheList->nData > 0)
	{
		fwrite(TheList->aData, TheList->DataSize, TheList->nData, TheFile);
	}
}


int LIST_Load(LIST* TheList, FILE* TheFile)
{
	int  New_nData;
	int  New_DataSize;

	fread(&New_nData,    sizeof(int), 1, TheFile);
	fread(&New_DataSize, sizeof(int), 1, TheFile);

	if (New_nData < 0 || New_DataSize != TheList->DataSize) return (-1);
	
	LIST_Clear(TheList);
	
	if (New_nData > 0)
	{
		TheList->nData = New_nData;
		TheList->mData = New_nData;
		TheList->aData = (unsigned char*)malloc(New_nData * New_DataSize);

		fread(TheList->aData, New_DataSize, New_nData, TheFile);
	}
	
	return New_nData;
}

