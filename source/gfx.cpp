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

#include "common.hpp"

 // Include the spritesheet extern.

void GFX::DrawTop(void) {
	Gui::ScreenDraw(Top);
	Gui::Draw_Rect(0, 0, 400, 30, C2D_Color32(0, 160, 160, 255));
	Gui::Draw_Rect(0, 30, 400, 180, C2D_Color32(0, 120, 120, 255));
	Gui::Draw_Rect(0, 210, 400, 30, C2D_Color32(0, 160, 160, 255));
}

void GFX::DrawBottom(void) {
	Gui::ScreenDraw(Bottom);
	Gui::Draw_Rect(0, 0, 320, 30, C2D_Color32(0, 160, 160, 255));
	Gui::Draw_Rect(0, 30, 320, 180, C2D_Color32(0, 120, 120, 255));
	Gui::Draw_Rect(0, 210, 320, 30, C2D_Color32(0, 160, 160, 255));
}

void GFX::DrawFileBrowseBG(bool isTop) {
	if (isTop == true) {
		Gui::ScreenDraw(Top);
		Gui::Draw_Rect(0, 0, 400, 27, C2D_Color32(0, 160, 210, 255));
		Gui::Draw_Rect(0, 27, 400, 31, C2D_Color32(0, 160, 160, 255));
		Gui::Draw_Rect(0, 58, 400, 31, C2D_Color32(0, 120, 120, 255));
		Gui::Draw_Rect(0, 89, 400, 31, C2D_Color32(0, 160, 160, 255));
		Gui::Draw_Rect(0, 120, 400, 31, C2D_Color32(0, 120, 120, 255));
		Gui::Draw_Rect(0, 151, 400, 31, C2D_Color32(0, 160, 160, 255));
		Gui::Draw_Rect(0, 182, 400, 31, C2D_Color32(0, 120, 120, 255));
		Gui::Draw_Rect(0, 213, 400, 27, C2D_Color32(0, 160, 210, 255));
	} else {
		Gui::ScreenDraw(Bottom);
		Gui::Draw_Rect(0, 0, 320, 27, C2D_Color32(0, 160, 210, 255));
		Gui::Draw_Rect(0, 27, 320, 31, C2D_Color32(0, 160, 160, 255));
		Gui::Draw_Rect(0, 58, 320, 31, C2D_Color32(0, 120, 120, 255));
		Gui::Draw_Rect(0, 89, 320, 31, C2D_Color32(0, 160, 160, 255));
		Gui::Draw_Rect(0, 120, 320, 31, C2D_Color32(0, 120, 120, 255));
		Gui::Draw_Rect(0, 151, 320, 31, C2D_Color32(0, 160, 160, 255));
		Gui::Draw_Rect(0, 182, 320, 31, C2D_Color32(0, 120, 120, 255));
		Gui::Draw_Rect(0, 213, 320, 27, C2D_Color32(0, 160, 210, 255));
	}
}

void GFX::DrawSprite(int img, int x, int y, float ScaleX, float ScaleY) {
	Gui::DrawSprite(sprites, img, x, y, ScaleX, ScaleY);
}