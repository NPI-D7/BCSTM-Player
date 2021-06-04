#include "Menu.hpp"
#include "BCSTM.hpp"
#include "log.hpp"
#include "TitleManager.hpp"

//RenderD7::Image img;
extern BCSTM player;
RenderD7::Sheet sheet;
extern Log cachelog;

int main()
{
    RenderD7::Init::Main();
    mkdir("sdmc:/BCSTM-Player/", 0777);
    mkdir("sdmc:/BCSTM-Player/cache/", 0777);
    RenderD7::Lang::load(RenderD7::Lang::getSys());
    //img.LoadPng("romfs:/gfx/bg.png");
    cachelog.Init("/BCSTM-Player/cache.log");
    RenderD7::Init::NdspFirm(true);
    aptSetSleepAllowed(false);
    sheet.Load("romfs:/gfx/sprites.t3x");
    RenderD7::Scene::Load(std::make_unique<MMM>());
    while(RenderD7::MainLoop())
    {
        D7TM::CardLoop();
        RenderD7::Scene::doDraw();
        RenderD7::Scene::doLogic(d7_hDown, d7_hHeld, d7_hUp, d7_touch);
        player.tick();
        C3D_FrameEnd(0);
    }
    sheet.Free();
    RenderD7::Exit::NdspFirm();
    RenderD7::Exit::Main();
}
