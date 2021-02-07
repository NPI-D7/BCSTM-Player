#include <string.h>
#include <3ds.h>
#include <stdio.h>

#include "BCSTM.hpp"
#include "TitleManager.hpp"

int main()
{
    gfxInitDefault();
	ndspInit();
	consoleInit(GFX_TOP, NULL);
	printf("Scan SD!");
	TitleManager::ScanSD();
	consoleClear();
	printf("BCSTM-Player by Tobi-D7\nVersion:0.2.0\n\nPress X to Stop Player!\nPress Y to reopen file!\nPress Start to exit!\n");
	BCSTM file;
	file.openFromFile("sdmc:/music.bcstm");
	file.play();
	osSetSpeedupEnable(true);
	aptSetSleepAllowed(false);
	aptSetHomeAllowed(false);
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