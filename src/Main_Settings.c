
#include "StdInc.h"
#include "Flib1.h"
#include "Puzzle.h"
#include "Main.h"


static Config_File        Settings_Config_File   = NULL;

#define  NUM_CONFIG_ENTRYS  5

static Config_File_Data  Settings_Config[NUM_CONFIG_ENTRYS] =
{
	{"WIN_PIC_SIZE_SPACE_X",        NULL},
	{"WIN_PIC_SIZE_SPACE_Y",        NULL},
	{"AUTO_PIECE_WIDTH",            NULL},
	{"AUTO_PIECE_HEIGHT",           NULL},
	{"ASSEMBLE_TOLERANCE",          NULL}
};


void Settings_Free(void)
{
	if (Settings_Config_File)
	{
		Config_File_Free(Settings_Config_File);
		Settings_Config_File = NULL;
	}
}

void Settings_Load(void)
{
	Settings_Config_File = Config_File_Load("Puzzler_Config.ini", Settings_Config, NUM_CONFIG_ENTRYS);

	if (Settings_Config_File)
	{
		Config_File_Start_Eval(Settings_Config, NUM_CONFIG_ENTRYS);

		Config_File_Eval_Int(&WIN_PIC_SIZE_SPACE_X);
		Config_File_Eval_Int(&WIN_PIC_SIZE_SPACE_Y);

		Config_File_Eval_Int(&AUTO_PIECE_WIDTH);
		Config_File_Eval_Int(&AUTO_PIECE_HEIGHT);

		Config_File_Eval_Int(&ASSEMBLE_TOLERANCE);
	}
}

