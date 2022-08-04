#include "Menu.hpp"
#include "BCSTM.hpp"

extern BCSTM player;
RenderD7::Sheet sheet;

//Thread
void PlayerT(RenderD7::Parameter m_parameter)
{
     while (true) {
        player.tick();
        RenderD7::Thread::sleep(1000 * 1);
    }
}

int main(int argc, char* argv[])
{
    if(!RenderD7::FileSystem::Init(argv[0]))
    {
        RenderD7::AddOvl(std::make_unique<RenderD7::Toast>("PHYSFS Error", RenderD7::FileSystem::GetPhysfsError()));
    }
    RenderD7::Init::Main("BCSTM-Player");
    RenderD7::Init::NdspFirm(true);
    aptSetSleepAllowed(false);
    sheet.Load("romfs:/gfx/sprites.t3x");
    RenderD7::Thread t1(PlayerT, 1);
    t1.start();
    RenderD7::Scene::Load(std::make_unique<MMM>());
    while(RenderD7::MainLoop())
    {
        //player.tick();
        RenderD7::FrameEnd();
    }
    sheet.Free();
    t1.kill();
    RenderD7::Exit::NdspFirm();
    RenderD7::Exit::Main();
}