/*
*   This file is part of Universal-Core-Example
*   Copyright (C) 2020 SuperSaiyajinStackie
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
*       * Requiring preservation of specified reasonable legal notices or
*         author attributions in that material or in the Appropriate Legal
*         Notices displayed by works containing it.
*       * Prohibiting misrepresentation of the origin of that material,
*         or requiring that modified versions of such material be marked in
*         reasonable ways as different from the original version.
*/

#include "mainMenu.hpp"
#include "stack.hpp"

void Stack::Draw(void) const {
	Gui::ScreenDraw(Top);
	Gui::Draw_Rect(0, 0, 400, 240, WHITE);
	GFX::DrawSprite(sprites_dev_by_idx, 7.5, 7.5);
	Gui::DrawString(8, 218, 0.50, C2D_Color32(0, 0, 0, 255), "2020");
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
	Gui::ScreenDraw(Bottom);
	Gui::Draw_Rect(0, 0, 320, 240, WHITE);
	GFX::DrawSprite(sprites_universal_core_idx, 0, 26);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
}


void Stack::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	this->delay++;
	// If the delay is larger than 130 -> Switch screen.
	if (this->delay > 130) {
		Gui::setScreen(std::make_unique<MainMenu>(), true, false);
	}
}