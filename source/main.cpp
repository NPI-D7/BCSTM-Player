#include <common.hpp>
#include <scenes/scenes.hpp>

void Bcstm_Loop() {
  while (true) {
    if (BP::player.IsLoaded()) BP::player.Update();
    RD7::Thread::sleep(1 * 10);
  }
  BP::player.Stop();
}

int main() {
  rd7_do_splash = true;
  d7mc_load_icons = false;
  RD7::Init::Main("BCSTM-Player");
  RD7::Init::NdspFirm();
  BP::config.Load();
  if (BP::config.GetBool("fade")) RD7::FadeIn();
  std::filesystem::create_directories(
      std::filesystem::path("sdmc:/BCSTM-Player/cache/"));
  RD7::Lang::Load(BP::config.GetString("lang"));
  if (BP::config.GetBool("rd7tf_theme"))
    RD7::ThemeLoad("romfs:/themes/rd7tf.theme");
  aptSetSleepAllowed(false);

  RD7::Tasks::create((ThreadFunc)Bcstm_Loop);
  BP::player.Stop();  // for ui
  RD7::Scene::Load(std::make_unique<BP::MainMenu>());

  while (RD7::MainLoop()) {
    RD7::FrameEnd();
  }

  return 0;
}
