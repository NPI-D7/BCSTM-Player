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

#include "fileList.hpp"
#include "mainMenu.hpp"

extern bool touching(touchPosition touch, Structs::ButtonPos button);

FileList::FileList() {
	// Put stuff, which we need to initialize this screen here.
	for (int i = 0; i < 7; i++) {
		this->exampleVector.push_back({"Entry" + std::to_string(i)});
	}
}

void FileList::Draw(void) const {
	std::string fileList;
	GFX::DrawFileBrowseBG();
	Gui::DrawStringCentered(0, 2, 0.8f, WHITE, "Universal-Core Example -> FileList", 400);

	for (int i = (this->Selection<5) ? 0 : this->Selection-5;i< (int)this->exampleVector.size() && i <((this->Selection<5) ? 6 : this->Selection+1);i++) {
		if (i == this->Selection) {
			fileList += "> " + this->exampleVector[i] + "\n\n";
		} else {
			fileList += this->exampleVector[i] + "\n\n";
		}
	}
	for (int i=0;i<(((int)this->exampleVector.size()<6) ? 6-(int)this->exampleVector.size() : 0);i++) {
		fileList += "\n\n";
	}
	Gui::DrawString(26, 32, 0.52f, WHITE, fileList, 360);
	Gui::DrawStringCentered(0, 216, 0.8f, WHITE, "Entry Amount: " + std::to_string((int)exampleVector.size()), 400);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
	GFX::DrawFileBrowseBG(false);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha)); // Fade in/out effect
}


void FileList::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::setScreen(std::make_unique<MainMenu>(), true, false);
	}

	if (hDown & KEY_DOWN) {
		if (this->Selection < (int)this->exampleVector.size()-1)	this->Selection++;
	}
	if (hDown & KEY_UP) {
		if (this->Selection > 0)	this->Selection--;
	}
}