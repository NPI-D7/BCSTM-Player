#include <string.h>
#include <3ds.h>
#include <stdio.h>

#include "common.hpp"

#include "BCSTM.hpp"
#include "TitleManager.hpp"
#include "mainMenu.hpp"

bool touching(touchPosition touch, Structs::ButtonPos button) {
	if (touch.px >= button.x && touch.px <= (button.x + button.w) && touch.py >= button.y && touch.py <= (button.y + button.h))	return true;
	else	return false;
}

BCSTM file;

bool exiting = false;

Result Init()
{
	fadealpha = 255;
	fadein = true;
	gfxInitDefault();
	ndspInit();
	Gui::init();
	romfsInit();
	cfguInit();
	osSetSpeedupEnable(true);
	aptSetSleepAllowed(false);
	file.openFromFile("sdmc:/music.bcstm");
	file.play();
	Gui::setScreen(std::unique_ptr<MainMenu>(), false, true);
	return 0;

}
Result Exit()
{
	Gui::exit();
	gfxExit();
	cfguExit();
	romfsExit();
	return 0;
}
int main()
{
		
    Init();
	while (aptMainLoop())
	{

		file.tick();
		hidScanInput();
		u32 hDown = hidKeysDown();
		u32 hHeld = hidKeysHeld();
		touchPosition touch;
		hidTouchRead(&touch);

		Gui::clearTextBufs(); // Clear Text Buffer before.
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));

		// Screen Logic & Draw.
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		Gui::DrawScreen(false);
		Gui::ScreenLogic(hDown, hHeld, touch, true, false);
		C3D_FrameEnd(0);
		if (exiting) {
			if (!fadeout)	break;
		}

		// Call the fade effects here. :D
		Gui::fadeEffects(6, 6, false);
	}
	Exit();
        return 0;
}
