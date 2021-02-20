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

#include "buttons.hpp"
#include "mainMenu.hpp"

extern bool touching(touchPosition touch, Structs::ButtonPos button);

void Buttons::Draw(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.8f, WHITE, "Universal-Core Example -> Buttons", 400);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
	GFX::DrawBottom();
	// Draw Buttons.
	for (int i = 0; i < (int)this->buttons.size(); i++) {
		if (this->Selection == i) {
			Gui::Draw_Rect(this->buttons[i].x, this->buttons[i].y, this->buttons[i].w, this->buttons[i].h, C2D_Color32(0, 170, 170, 255));
		} else {
			Gui::Draw_Rect(this->buttons[i].x, this->buttons[i].y, this->buttons[i].w, this->buttons[i].h, C2D_Color32(0, 170, 100, 255));
		}
	}
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
}


void Buttons::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::setScreen(std::make_unique<MainMenu>(), true, false);
	}

	if (hDown & KEY_DOWN) {
		if (this->Selection < (int)this->buttons.size()/2-1 || this->Selection < (int)this->buttons.size()-1)	this->Selection++;
	}

	if (hDown & KEY_RIGHT) {
		if (this->Selection < (int)this->buttons.size()/2)	this->Selection += 3;
	}

	if (hDown & KEY_LEFT) {
		if (this->Selection > (int)this->buttons.size()/2-1)	this->Selection -= 3;
	}

	if (hDown & KEY_UP) {
		if (this->Selection > 0)	this->Selection--;
	}
}