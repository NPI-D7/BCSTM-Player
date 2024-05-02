#include <scenes/scenes.hpp>

namespace BP {
Filemanager::Filemanager(std::string idevice) {
  this->device = idevice;
  this->dir = this->device;
  this->dircontent = RenderD7::FileSystem::GetDirContentsExt(dir, {"bcstm"});
  this->reload_list();
  this->changeddir = false;
}

void Filemanager::Draw(void) const {
  RenderD7::OnScreen(Top);
  if (UI7::BeginMenu(RD7::Lang::Get("HEAD_FILEMANAGER"))) {
    UI7::BrowserList(namelist, this->dirsel);
    UI7::SetCursorPos(R7Vec2(5, 222));
    UI7::Label(this->dir);
    UI7::RestoreCursor();
    UI7::EndMenu();
  }

  RenderD7::OnScreen(Bottom);
  if (UI7::BeginMenu("Info")) {
    UI7::Label("Playing: " + std::string(playing ? now_playing : "Nothing"),
               RD7TextFlags_Short);
    UI7::Label("DirEntrys: " + std::to_string(this->dircontent.size()));
    UI7::Label("Loadet: " + std::string(player.IsLoaded() ? "True" : "False"));
    UI7::Label("Loop: " + std::string(player.IsLooping() ? "True" : "False"));
    UI7::Label("LoopStart: " + std::to_string(player.GetLoopStart()));
    UI7::Label("LoopEnd: " + std::to_string(player.GetLoopEnd()));
    UI7::Label("Current: " + std::to_string(player.GetCurrent()));
    UI7::Label("Total: " + std::to_string(player.GetTotal()));
    UI7::Label("Channels: " + std::to_string(player.GetChannelCount()));
    UI7::Label("Error: " + player.GetErrorMessage());
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
    UI7::SetCursorPos(R7Vec2(5, 215));
    UI7::Progressbar((float)player.GetCurrent() / (float)player.GetTotal());
    UI7::RestoreCursor();
    UI7::EndMenu();
  }
}

void Filemanager::Logic() {
  if (this->changeddir) {

    this->dircontent.clear();
    std::vector<RenderD7::FileSystem::Entry> temp =
        RenderD7::FileSystem::GetDirContentsExt(dir, {"bcstm"});

    for (int i = 0; i < (int)temp.size(); i++) {
      this->dircontent.push_back(temp[i]);
    }
    this->reload_list();
    this->changeddir = false;
  }
  if (hidKeysDown() & KEY_A) {
    if (this->dircontent.size() > 0) {
      if (this->dircontent[dirsel].dir) {
        if (dir.substr(dir.length() - 1, 1) != "/")
          dir += "/";
        dir += this->dircontent[this->dirsel].name;
        this->dircontent.clear();
        this->dirsel = 0;
        this->changeddir = true;
      } else {
        if (RenderD7::NameIsEndingWith(this->dircontent[this->dirsel].name,
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
      RenderD7::Scene::Back();
    } else {
      dir = RenderD7::FileSystem::GetParentPath(dir, this->device);
      dirsel = 0;
      changeddir = true;
    }
  }
  if (hidKeysDown() & KEY_X) {
    RenderD7::Scene::Back();
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
} // namespace BP