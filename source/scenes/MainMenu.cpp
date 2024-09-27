#include <common.hpp>
#include <scenes/scenes.hpp>

namespace BP {
MainMenu::MainMenu() { this->load_menu(); }

void MainMenu::Update() {
  // Render
  PD::LI::OnScreen(false);
  if (UI7::BeginMenu(PD::Lang::Get("HEAD_MAINMENU"))) {
    if (config.clock()) {
      UI7::SetCursorPos(NVec2(395, 2));
      UI7::Label(Clock(), PDTextFlags_AlignRight);
      UI7::RestoreCursor();
    }
    UI7::Label(
        PD::Lang::Get("PLAYING") +
            std::string(playing ? now_playing : PD::Lang::Get("NOTHING")),
        PDTextFlags_Short);
    UI7::Label(PD::Lang::Get("LOADED") +
               std::string(player.IsLoaded() ? PD::Lang::Get("YES")
                                             : PD::Lang::Get("NO")));
    UI7::Label(PD::Lang::Get("LOOP") + std::string(player.IsLooping()
                                                       ? PD::Lang::Get("YES")
                                                       : PD::Lang::Get("NO")));
    UI7::Label(PD::Lang::Get("LOOPSTART") +
               std::to_string(player.GetLoopStart()));
    UI7::Label(PD::Lang::Get("LOOPEND") + std::to_string(player.GetLoopEnd()));
    UI7::Label(PD::Lang::Get("CURRENT") + std::to_string(player.GetCurrent()));
    UI7::Label(PD::Lang::Get("TOTAL") + std::to_string(player.GetTotal()));
    UI7::Label(PD::Lang::Get("CHANNELS") +
               std::to_string(player.GetChannelCount()));
    UI7::Label(PD::Lang::Get("SRATE") + std::to_string(player.GetSampleRate()));
    UI7::Label(PD::Lang::Get("ERROR") + player.GetErrorMessage());
    UI7::SetCursorPos(NVec2(5, 215));
    UI7::Progressbar((float)player.GetCurrent() / (float)player.GetTotal());
    UI7::RestoreCursor();
    UI7::EndMenu();
  }
  PD::LI::OnScreen(true);
  if (UI7::BeginMenu(PD::Lang::Get("CONTROLCENTER"))) {
    if (UI7::Button(PD::Lang::Get("PLAY"))) {
      player.Play();
    }
    UI7::SameLine();
    if (UI7::Button(PD::Lang::Get("PAUSE"))) {
      player.Pause();
    }
    UI7::SameLine();
    if (UI7::Button(PD::Lang::Get("STOP"))) {
      playing = false;
      player.Stop();
    }
    if (UI7::Button(PD::Lang::Get("BROWSE"))) {
      PD::Scene::Load(std::make_unique<Filemanager>());
    }
    if (UI7::Button(PD::Lang::Get("SETTINGS"))) {
      PD::Scene::Load(std::make_unique<Settings>());
    }
    if (rfsopt) {
      if (UI7::Button(PD::Lang::Get("TITLES") + " (Citra only)")) {
        PD::Scene::Load(std::make_unique<Titles>());
      }
      UI7::SameLine();
      if (UI7::Button("RomFS (Citra only)")) {
        PD::Scene::Load(std::make_unique<Filemanager>("title:/"));
      }
    }
    if (UI7::Button(PD::Lang::Get("EXIT"))) {
      PD::ExitApp();
    }
    UI7::EndMenu();
  }
  // Logic
  // do only one request
  if (hidKeysDown() & KEY_START) {
    PD::ExitApp();
  }
  if (current_lang != PD::Lang::GetShortcut() ||
      rfsopt != config.romfs_browse()) {
    this->load_menu();
  }
}

void MainMenu::load_menu() {
  this->rfsopt = config.romfs_browse();
  this->current_lang = PD::Lang::GetShortcut();
}
}  // namespace BP