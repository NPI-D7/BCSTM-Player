#include <scenes/scenes.hpp>

namespace BP {
MainMenu::MainMenu() { this->load_menu(); }
void MainMenu::Draw(void) const {
  RD7::OnScreen(Top);
  if (config.GetBool("rd7tf_theme"))
    DrawWavedBg(R7Vec2(), R7Vec2(400, 240), RenderD7::GetTime());
  if (UI7::BeginMenu(RD7::Lang::Get("HEAD_MAINMENU"))) {
    UI7::Label(
        RD7::Lang::Get("PLAYING") +
            std::string(playing ? now_playing : RD7::Lang::Get("NOTHING")),
        RD7TextFlags_Short);
    UI7::Label(RD7::Lang::Get("LOADED") +
               std::string(player.IsLoaded() ? RD7::Lang::Get("YES")
                                             : RD7::Lang::Get("NO")));
    UI7::Label(RD7::Lang::Get("LOOP") +
               std::string(player.IsLooping() ? RD7::Lang::Get("YES")
                                              : RD7::Lang::Get("NO")));
    UI7::Label(RD7::Lang::Get("LOOPSTART") +
               std::to_string(player.GetLoopStart()));
    UI7::Label(RD7::Lang::Get("LOOPEND") + std::to_string(player.GetLoopEnd()));
    UI7::Label(RD7::Lang::Get("CURRENT") + std::to_string(player.GetCurrent()));
    UI7::Label(RD7::Lang::Get("TOTAL") + std::to_string(player.GetTotal()));
    UI7::Label(RD7::Lang::Get("CHANNELS") +
               std::to_string(player.GetChannelCount()));
    UI7::Label(RD7::Lang::Get("SRATE") +
               std::to_string(player.GetSampleRate()));
    UI7::Label(RD7::Lang::Get("ERROR") + player.GetErrorMessage());
    UI7::SetCursorPos(R7Vec2(5, 215));
    UI7::Progressbar((float)player.GetCurrent() / (float)player.GetTotal());
    UI7::RestoreCursor();
    UI7::EndMenu();
  }
  RD7::OnScreen(Bottom);
  if (UI7::BeginMenu(RD7::Lang::Get("CONTROLCENTER"))) {
    UI7::BrowserList(this->menu, this->sel);
    UI7::EndMenu();
  }
}
void MainMenu::Logic() {
  // do only one request
  bool fade = config.GetBool("fade");
  if (hidKeysDown() & KEY_A) {
    if (this->sel == 0) {
      player.Play();
    } else if (this->sel == 1) {
      player.Pause();
    } else if (this->sel == 2) {
      playing = false;
      player.Stop();
    } else if (this->sel == 3) {
      RD7::Scene::Load(std::make_unique<Filemanager>(), fade);
    } else if (this->sel == 4) {
      RD7::Scene::Load(std::make_unique<Titles>(), fade);
    } else if (this->sel == 5 && romfs_is_mount) {
      RD7::Scene::Load(std::make_unique<Filemanager>("title:/"), fade);
    } else if (this->sel == 6) {
      RD7::Scene::Load(std::make_unique<Settings>(), fade);
    } else if (this->sel == 7) {
      if (fade) RD7::FadeOut();
      RD7::ExitApp();
    }
  }
  if (hidKeysDown() & KEY_START) {
    RD7::FadeOut();
    RD7::ExitApp();
  }
  if (hidKeysDown() & KEY_UP && sel > 0) {
    sel--;
  }
  if (hidKeysDown() & KEY_DOWN && sel < (int)menu.size() - 1) {
    sel++;
  }
  if (current_lang != RD7::Lang::GetShortcut()) {
    this->load_menu();
  }
}

void MainMenu::load_menu() {
  this->sel = 0;
  menu.clear();
  menu.push_back(RD7::Lang::Get("PLAY"));
  menu.push_back(RD7::Lang::Get("PAUSE"));
  menu.push_back(RD7::Lang::Get("STOP"));
  menu.push_back(RD7::Lang::Get("BROWSE"));
  menu.push_back(RD7::Lang::Get("TITLES") + " (Citra only)");
  menu.push_back("RomFS (Citra only)");
  menu.push_back(RD7::Lang::Get("SETTINGS"));
  menu.push_back(RD7::Lang::Get("EXIT"));
  this->current_lang = RD7::Lang::GetShortcut();
}
}  // namespace BP