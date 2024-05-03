#include <common.hpp>
#include <scenes/scenes.hpp>

void Bcstm_Loop() {
  while (true) {
    if (BP::player.IsLoaded()) BP::player.Update();
    RenderD7::Thread::sleep(1 * 10);
  }
  BP::player.Stop();
}

int main() {
  rd7_do_splash = true;
  d7mc_load_icons = false;
  RenderD7::Init::Main("BCSTM-Player");
  RenderD7::Init::NdspFirm();
  RenderD7::FadeIn();
  std::filesystem::create_directories(
      std::filesystem::path("sdmc:/BCSTM-Player/cache/"));
  RenderD7::Lang::Load(RenderD7::Lang::GetSys());
  aptSetSleepAllowed(false);

  RenderD7::Tasks::create((ThreadFunc)Bcstm_Loop);
  RenderD7::Scene::Load(std::make_unique<BP::MainMenu>());

  while (RenderD7::MainLoop()) {
    RenderD7::FrameEnd();
  }

  return 0;
}
