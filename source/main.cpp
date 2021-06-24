#include "Menu.hpp"
#include "BCSTM.hpp"
#include "log.hpp"
#include "TitleManager.hpp"

//RenderD7::Image img;
extern BCSTM player;
RenderD7::Sheet sheet;
extern Log cachelog;

void CardScan(RenderD7::Parameter param) {
    while (true) {
        D7TM::CardLoop();
        player.tick();
        RenderD7::Thread::sleep(1000 * 1); // wait; also, this is needed to allow for concurrency (refer to the documentation for m3d::Thread::sleep())
    }
}

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
    RenderD7::Thread t1(CardScan, 1);
    t1.start();
    while(RenderD7::MainLoop())
    {
        RenderD7::Scene::doDraw();
        RenderD7::Scene::doLogic(d7_hDown, d7_hHeld, d7_hUp, d7_touch);
        //player.tick();
        C3D_FrameEnd(0);
    }
    t1.detach();
    sheet.Free();
    RenderD7::Exit::NdspFirm();
    RenderD7::Exit::Main();
}
