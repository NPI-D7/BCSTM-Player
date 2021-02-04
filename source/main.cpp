#include <string.h>
#include <3ds.h>
#include <stdio.h>
#include "BCSTM.hpp"

int main()
{
	
    gfxInitDefault();
	ndspInit();
	consoleInit(GFX_TOP, NULL);
	printf("BCSTM-Player by Tobi-D7\n\nPress X to Stop Player!\nPress Y to reopen file!\nPress Start to exit!\n");
	BCSTM file;
	file.openFromFile("sdmc:/music.bcstm");
	file.play();
	if(!aptIsSleepAllowed)
	{
		aptSetSleepAllowed();
	}
	while (aptMainLoop())
	{
		file.tick();
		hidScanInput();
		u32 hDown = hidKeysDown();

		
		if (hDown & KEY_X)
		{
			file.stop();
		}
		if (hDown & KEY_Y)
		{
			file.openFromFile("sdmc:/music.bcstm");
			file.play();
		}
		if (hDown & KEY_START)
		{
			break;
		}
	}
    file.stop();
	gfxExit();
	ndspExit();
	aptExit();
	return 0;
}