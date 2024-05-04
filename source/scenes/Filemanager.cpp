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
  RD7::OnScreen(Top);
  if (config.GetBool("rd7tf_theme"))
    DrawWavedBg(R7Vec2(), R7Vec2(400, 240), RenderD7::GetTime());
  if (UI7::BeginMenu(RD7::Lang::Get("HEAD_FILEMANAGER"))) {
    UI7::BrowserList(namelist, this->dirsel);
    UI7::SetCursorPos(R7Vec2(5, 222));
    UI7::Label(this->dir);
    UI7::RestoreCursor();
    UI7::EndMenu();
  }

  RD7::OnScreen(Bottom);
  if (UI7::BeginMenu(RD7::Lang::Get("INFO"))) {
    UI7::Label(
        RD7::Lang::Get("PLAYING") +
            std::string(playing ? now_playing : RD7::Lang::Get("NOTHING")),
        RD7TextFlags_Short);
    UI7::Label(RD7::Lang::Get("DIRENTRYS") +
               std::to_string(this->dircontent.size()));
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
    UI7::Label(RD7::Lang::Get("ERROR") + player.GetErrorMessage());
    if (UI7::Button(RD7::Lang::Get("PLAY"))) {
      player.Play();
    }
    UI7::SameLine();
    if (UI7::Button(RD7::Lang::Get("PAUSE"))) {
      player.Pause();
    }
    UI7::SameLine();
    if (UI7::Button(RD7::Lang::Get("STOP"))) {
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