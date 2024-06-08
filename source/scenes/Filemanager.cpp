#include <scenes/scenes.hpp>

namespace BP {
Filemanager::Filemanager(std::string idevice) {
  this->device = idevice;
  this->dir = this->device;
  this->dircontent = RD7::FileSystem::GetDirContentsExt(dir, {"bcstm"});
  this->reload_list();
  this->changeddir = false;
}

void Filemanager::Draw(void) const {
  RD7::R2()->OnScreen(R2Screen_Top);
  if (config.rd7tf_theme())
    DrawWavedBg(R7Vec2(), R7Vec2(400, 240), RenderD7::GetTime());
  if (UI7::BeginMenu(Lang::HEAD_FILEMANAGER)) {
    if (config.clock()) {
      UI7::SetCursorPos(R7Vec2(395, 2));
      UI7::Label(Clock(), RD7TextFlags_AlignRight);
      UI7::RestoreCursor();
    }
    UI7::BrowserList(namelist, this->dirsel);
    UI7::SetCursorPos(R7Vec2(5, 222));
    UI7::Label(this->dir);
    UI7::RestoreCursor();
    UI7::EndMenu();
  }

  RD7::R2()->OnScreen(R2Screen_Bottom);
  if (UI7::BeginMenu(Lang::INFO)) {
    UI7::Label(
        Lang::PLAYING + std::string(playing ? now_playing : Lang::NOTHING),
        RD7TextFlags_Short);
    UI7::Label(Lang::DIRENTRYS + std::to_string(this->dircontent.size()));
    UI7::Label(Lang::LOADED +
               std::string(player.IsLoaded() ? Lang::YES : Lang::NO));
    UI7::Label(Lang::LOOP +
               std::string(player.IsLooping() ? Lang::YES : Lang::NO));
    UI7::Label(Lang::LOOPSTART + std::to_string(player.GetLoopStart()));
    UI7::Label(Lang::LOOPEND + std::to_string(player.GetLoopEnd()));
    UI7::Label(Lang::CURRENT + std::to_string(player.GetCurrent()));
    UI7::Label(Lang::TOTAL + std::to_string(player.GetTotal()));
    UI7::Label(Lang::CHANNELS + std::to_string(player.GetChannelCount()));
    UI7::Label(Lang::ERROR + player.GetErrorMessage());
    if (UI7::Button(Lang::PLAY)) {
      player.Play();
    }
    UI7::SameLine();
    if (UI7::Button(Lang::PAUSE)) {
      player.Pause();
    }
    UI7::SameLine();
    if (UI7::Button(Lang::STOP)) {
      playing = false;
      player.Stop();
    }
    UI7::SetCursorPos(R7Vec2(5, 215));
    UI7::Progressbar((float)player.GetCurrent() / (float)player.GetTotal());
    UI7::RestoreCursor();
    UI7::EndMenu();
  }
}

void Filemanager::Logic() {
  if (this->changeddir) {
    this->dircontent.clear();
    std::vector<RD7::FileSystem::Entry> temp =
        RD7::FileSystem::GetDirContentsExt(dir, {"bcstm"});

    for (int i = 0; i < (int)temp.size(); i++) {
      this->dircontent.push_back(temp[i]);
    }
    this->reload_list();
    this->changeddir = false;
  }
  if (hidKeysDown() & KEY_A) {
    if (this->dircontent.size() > 0) {
      if (this->dircontent[dirsel].dir) {
        if (dir.substr(dir.length() - 1, 1) != "/") dir += "/";
        dir += this->dircontent[this->dirsel].name;
        this->dircontent.clear();
        this->dirsel = 0;
        this->changeddir = true;
      } else {
        if (RD7::NameIsEndingWith(this->dircontent[this->dirsel].name,
                                  {"bcstm"})) {
          playing = false;
          if (player.LoadFile(this->dircontent[this->dirsel].path)) {
            player.Play();
            playing = true;
          }
          now_playing = this->dircontent[this->dirsel].name;
        }
      }
    }
  }
  if (hidKeysDown() & KEY_B) {
    if (strcmp(dir.c_str(), this->device.c_str()) == 0 ||
        strcmp(dir.c_str(), "/") == 0) {
      RD7::Scene::Back();
    } else {
      dir = RD7::FileSystem::GetParentPath(dir, this->device);
      dirsel = 0;
      changeddir = true;
    }
  }
  if (hidKeysDown() & KEY_X) {
    RD7::Scene::Back();
  }
  if (hidKeysDown() & KEY_UP && dirsel > 0) {
    dirsel--;
  }
  if (hidKeysDown() & KEY_DOWN && dirsel < (int)dircontent.size() - 1) {
    dirsel++;
  }
  if (hidKeysDown() & KEY_LEFT && dirsel - 6 > 0) {
    dirsel -= 6;
  }
  if (hidKeysDown() & KEY_RIGHT && dirsel + 6 < (int)dircontent.size() - 1) {
    dirsel += 6;
  }
}
}  // namespace BP