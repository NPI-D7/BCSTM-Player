#include <common.hpp>
#include <scenes/scenes.hpp>

void Bcstm_Loop() {
  while (true) {
    if (BP::player.IsLoaded()) BP::player.Update();
    RD7::Thread::sleep(1 * 10);
  }
  BP::player.Stop();
}

int main(int argc, char* argv[]) {
  rd7_do_splash = true;
  d7mc_load_icons = false;
  RD7::Init::Main("BCSTM-Player");
  BP::thiz_path = argc == 0 ? "sdmc:/3ds/BCSTM-Player.3dsx" : argv[0];
  BP::hb_mode = argc != 0;
  RD7::Init::NdspFirm();
  BP::config.Load();
  if (BP::config.fade()) RD7::FadeIn();
  RD7::Lang::Load(BP::config.lang());
  BP::Lang::Update();
  if (BP::config.rd7tf_theme())
    RD7::ThemeActive()->Load("romfs:/themes/rd7tf.theme");
  aptSetSleepAllowed(false);

  RD7::Tasks::Create(Bcstm_Loop);
  if (BP::config.search_updates()) BP::CheckForUpdate();
  BP::player.Stop();  // for ui
  RD7::Scene::Load(std::make_unique<BP::MainMenu>());

  while (RD7::MainLoop()) {
    RD7::FrameEnd();
  }

  return 0;
}
