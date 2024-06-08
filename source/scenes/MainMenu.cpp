#include <scenes/scenes.hpp>

namespace BP {
MainMenu::MainMenu() { this->load_menu(); }
void MainMenu::Draw(void) const {
  RD7::R2()->OnScreen(R2Screen_Top);
  if (config.rd7tf_theme())
    DrawWavedBg(R7Vec2(), R7Vec2(400, 240), RenderD7::GetTime());
  if (UI7::BeginMenu(Lang::HEAD_MAINMENU)) {
    if (config.clock()) {
      UI7::SetCursorPos(R7Vec2(395, 2));
      UI7::Label(Clock(), RD7TextFlags_AlignRight);
      UI7::RestoreCursor();
    }
    UI7::Label(
        Lang::PLAYING + std::string(playing ? now_playing : Lang::NOTHING),
        RD7TextFlags_Short);
    UI7::Label(Lang::LOADED +
               std::string(player.IsLoaded() ? Lang::YES : Lang::NO));
    UI7::Label(Lang::LOOP +
               std::string(player.IsLooping() ? Lang::YES : Lang::NO));
    UI7::Label(Lang::LOOPSTART + std::to_string(player.GetLoopStart()));
    UI7::Label(Lang::LOOPEND + std::to_string(player.GetLoopEnd()));
    UI7::Label(Lang::CURRENT + std::to_string(player.GetCurrent()));
    UI7::Label(Lang::TOTAL + std::to_string(player.GetTotal()));
    UI7::Label(Lang::CHANNELS + std::to_string(player.GetChannelCount()));
    UI7::Label(Lang::SRATE + std::to_string(player.GetSampleRate()));
    UI7::Label(Lang::ERROR + player.GetErrorMessage());
    UI7::SetCursorPos(R7Vec2(5, 215));
    UI7::Progressbar((float)player.GetCurrent() / (float)player.GetTotal());
    UI7::RestoreCursor();
    UI7::EndMenu();
  }
  RD7::R2()->OnScreen(R2Screen_Bottom);
  if (UI7::BeginMenu(Lang::CONTROLCENTER)) {
    UI7::BrowserList(this->menu, this->sel);
    UI7::EndMenu();
  }
}
void MainMenu::Logic() {
  // do only one request
  bool fade = config.fade();
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
      RD7::Scene::Load(std::make_unique<Settings>(), fade);
    } else if (this->sel == 5) {
      if (fade) RD7::FadeOut();
      RD7::ExitApp();
    } else if (this->sel == 6) {
      RD7::Scene::Load(std::make_unique<Titles>(), fade);
    } else if (this->sel == 7 && romfs_is_mount) {
      RD7::Scene::Load(std::make_unique<Filemanager>("title:/"), fade);
    }
  }
  if (hidKeysDown() & KEY_START) {
    if (fade) RD7::FadeOut();
    RD7::ExitApp();
  }
  if (hidKeysDown() & KEY_UP && sel > 0) {
    sel--;
  }
  if (hidKeysDown() & KEY_DOWN && sel < (int)menu.size() - 1) {
    sel++;
  }
  if (current_lang != RD7::Lang::GetShortcut() ||
      rfsopt != config.romfs_browse()) {
    this->load_menu();
  }
}

void MainMenu::load_menu() {
  this->rfsopt = config.romfs_browse();
  this->current_lang = RD7::Lang::GetShortcut();
  this->sel = 0;
  menu.clear();
  menu.push_back(Lang::PLAY);
  menu.push_back(Lang::PAUSE);
  menu.push_back(Lang::STOP);
  menu.push_back(Lang::BROWSE);
  menu.push_back(Lang::SETTINGS);
  menu.push_back(Lang::EXIT);
  if (rfsopt) {
    menu.push_back(Lang::TITLES + " (Citra only)");
    menu.push_back("RomFS (Citra only)");
  }
}
}  // namespace BP