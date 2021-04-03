#include "Menu.hpp"
#include "BCSTM.hpp"
extern BCSTM player;
RenderD7::Sheet sheet;

int main()
{
    RenderD7::Init::Main();
    RenderD7::Init::NdspFirm(true);
    aptSetSleepAllowed(false);
    sheet.Load("romfs:/gfx/sprites.t3x");
    RenderD7::Scene::Load(std::make_unique<MMM>());
    while(RenderD7::MainLoop())
    {
        RenderD7::Scene::doDraw();
        RenderD7::Scene::doLogic(d7_hDown, d7_hHeld, d7_hUp, d7_touch);
        player.tick();
        C3D_FrameEnd(0);
    }
    sheet.Free();
    RenderD7::Exit::NdspFirm();
    RenderD7::Exit::Main();
}