#include <common.hpp>
#include <scenes/scenes.hpp>

void Bcstm_Loop() {
  while (true) {
    if (BP::player.IsLoaded()) BP::player.Update();
    PD::Thread::sleep(1 * 10);
  }
  BP::player.Stop();
}

int main(int argc, char* argv[]) {
  d7mc_load_icons = false;
  PD::Init::Main("BCSTM-Player");
  BP::thiz_path = argc == 0 ? "sdmc:/3ds/BCSTM-Player.3dsx" : argv[0];
  BP::hb_mode = argc != 0;
  PD::Init::NdspFirm();
  BP::config.Load();
  PD::Lang::Load(BP::config.lang());
  aptSetSleepAllowed(false);
  // Enable LRS as the Systemfont Renders Faster with it
  PD::LI::GetFeatures() |= PDLithiumFlags_LRS;

  PD::Tasks::Create(Bcstm_Loop);
  if (BP::config.search_updates()) BP::CheckForUpdate();
  BP::player.Stop();  // for ui
  PD::Scene::Load(std::make_unique<BP::MainMenu>());

  while (PD::MainLoop()) {
    PD::FrameEnd();
  }

  return 0;
}
