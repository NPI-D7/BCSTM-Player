#pragma once
#include <nlc.hpp>
#include <3ds.h>
#include <citro3d.h>
#include <Menu.hpp>

inline void DoMsg(std::string title, std::string Text)
{
    nlc::nr::DrawBeg();
    nlc::nr2::DrawOnScreen();

    nlc::nr2::DrawRectSolid(0, 0, 400, 240, getcol("style_white"));
    nlc::nr2::DrawRectSolid(0, 0, 400, 26, getcol("style_black"));
    nlc::nr2::DrawText(5, 2, 0.7f, 0xffffffff, title);
    nlc::nr2::DrawText(5, 32, 0.7f, 0xff000000, Text);

    nlc::nr2::DrawOnScreen(1);
    nlc::nr2::DrawRectSolid(0, 0, 320, 240, getcol("style_black2"));
    C3D_FrameEnd(0);
    gspWaitForVBlank();
}