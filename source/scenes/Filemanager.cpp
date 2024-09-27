#include <common.hpp>
#include <scenes/scenes.hpp>

namespace BP {
Filemanager::Filemanager(std::string idevice) {
  this->device = idevice;
  this->dir = this->device;
  this->dircontent = PD::FileSystem::GetDirContentsExt(dir, {"bcstm"});
  this->reload_list();
  this->changeddir = false;
}

void Filemanager::Update() {
  // Render
  PD::LI::OnScreen(false);
  if (UI7::BeginMenu(PD::Lang::Get("HEAD_FILEMANAGER"))) {
    if (config.clock()) {
      UI7::SetCursorPos(NVec2(395, 2));
      UI7::Label(Clock(), PDTextFlags_AlignRight);
      UI7::RestoreCursor();
    }
    UI7::BrowserList(namelist, this->dirsel);
    UI7::SetCursorPos(NVec2(5, 222));
    UI7::Label(this->dir);
    UI7::RestoreCursor();
    UI7::EndMenu();
  }

  PD::LI::OnScreen(true);
  if (UI7::BeginMenu(PD::Lang::Get("INFO"))) {
    UI7::Label(
        PD::Lang::Get("PLAYING") +
            std::string(playing ? now_playing : PD::Lang::Get("NOTHING")),
        PDTextFlags_Short);
    UI7::Label(PD::Lang::Get("DIRENTRYS") +
               std::to_string(this->dircontent.size()));
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
    UI7::Label(PD::Lang::Get("ERROR") + player.GetErrorMessage());
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
    UI7::SetCursorPos(NVec2(5, 215));
    UI7::Progressbar((float)player.GetCurrent() / (float)player.GetTotal());
    UI7::RestoreCursor();
    UI7::EndMenu();
  }
  // Logic
  if (this->changeddir) {
    this->dircontent.clear();
    std::vector<PD::FileSystem::Entry> temp =
        PD::FileSystem::GetDirContentsExt(dir, {"bcstm"});

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
        if (PD::NameIsEndingWith(this->dircontent[this->dirsel].name,
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
      PD::Scene::Back();
    } else {
      dir = PD::FileSystem::GetParentPath(dir, this->device);
      dirsel = 0;
      changeddir = true;
    }
  }
  if (hidKeysDown() & KEY_X) {
    PD::Scene::Back();
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