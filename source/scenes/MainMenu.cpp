#include <scenes/scenes.hpp>

namespace BP {
MainMenu::MainMenu() {}
void MainMenu::Draw(void) const {
  RenderD7::OnScreen(Top);
  if (UI7::BeginMenu(RD7::Lang::Get("HEAD_MAINMENU"))) {
    UI7::Label(RD7::Lang::Get("STOP"));
    UI7::Label(RenderD7::Lang::Get("XORB"));
    if (playing) {
      UI7::Label("Player Info: ");
      UI7::Label("Playing: " + now_playing, RD7TextFlags_Short);
      UI7::Label("Loadet: " +
                 std::string(player.IsLoaded() ? "True" : "False"));
      UI7::Label("Loop: " + std::string(player.IsLooping() ? "True" : "False"));
      UI7::Label("LoopStart: " + std::to_string(player.GetLoopStart()));
      UI7::Label("LoopEnd: " + std::to_string(player.GetLoopEnd()));
      UI7::Label("Current: " + std::to_string(player.GetCurrent()));
      UI7::Label("Total: " + std::to_string(player.GetTotal()));
      UI7::Label("Channels: " + std::to_string(player.GetChannelCount()));
      UI7::SetCursorPos(R7Vec2(5, 215));
      UI7::Progressbar((float)player.GetCurrent() / (float)player.GetTotal());
      UI7::RestoreCursor();
    }
    UI7::EndMenu();
  }
  RenderD7::OnScreen(Bottom);
  if (UI7::BeginMenu(RD7::Lang::Get("CONTROLCENTER"))) {
    if (UI7::Button("Play")) {
      player.Play();
    }
    UI7::SameLine();
    if (UI7::Button("Pause")) {
      player.Pause();
    }
    UI7::SameLine();
    if (UI7::Button("Stop")) {
        playing = false;
      player.Stop();
    }
    if (UI7::Button(RD7::Lang::Get("BROWSE"))) {
      RenderD7::Scene::Load(std::make_unique<Filemanager>(), true);
    }
    if (UI7::Button("Titles")) {
      RenderD7::Scene::Load(std::make_unique<Titles>(), true);
    }
    if (UI7::Button(RD7::Lang::Get("SETTINGS"))) {
      RenderD7::Scene::Load(std::make_unique<Settings>(), true);
    }
    if (UI7::Button("Exit")) {
      RenderD7::FadeOut();
      RenderD7::ExitApp();
    }
    UI7::EndMenu();
  }
}
void MainMenu::Logic() {
  if (hidKeysDown() & KEY_START) {
    RenderD7::FadeOut();
    RenderD7::ExitApp();
  }
  if (hidKeysDown() & KEY_SELECT) {
    remove("sdmc:/BCSTM-Player/cache/sd");
    remove("sdmc:/BCSTM-Player/cache/nand");
  }
  if (hidKeysDown() & KEY_L && romfs_is_mount) {
    RenderD7::Scene::Load(std::make_unique<Filemanager>("title:/"));
  }
  if (hidKeysDown() & KEY_Y) {
    if (playing) {
      now_playing.clear();
      player.Stop();
      playing = false;
    }
  }
}
} // namespace BP