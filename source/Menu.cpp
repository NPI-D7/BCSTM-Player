// Menu.cpp
#include "Menu.hpp"
#include "BCSTM.hpp"

#ifdef V_STRING
#else
#define V_STRING "none"
#endif

bool romfs_is_mount = false;

BCSTM player;

bool playing = false;
std::string currentlypl;

extern bool is3dsx;

void clearCache() {
  remove("sdmc:/BCSTM-Player/cache/sd");
  remove("sdmc:/BCSTM-Player/cache/nand");
}

MMM::MMM() {}
void MMM::Draw(void) const {
  RenderD7::OnScreen(Top);
  if (UI7::BeginMenu(RD7::Lang::Get("HEAD_MAINMENU"))) {
    UI7::Label(RD7::Lang::Get("STOP"));
    UI7::Label(RenderD7::Lang::Get("XORB"));
    if (playing) {
        UI7::Label("Player Info: ");
        UI7::Label("Playing: " + currentlypl, RD7TextFlags_Short);
        UI7::Label("Loadet: " + std::string(player.IsLoadet() ? "True" : "False"));
        UI7::Label("Loop: " + player.GetLoop());
        UI7::Label("LoopStart: " + player.GetLoopStart());
        UI7::Label("LoopEnd: " + player.GetLoopEnd());
        UI7::Label("Current: " + std::to_string((int)player.GetCurrent()));
        UI7::Label("Total: " + std::to_string((int)player.GetTotal()));
        UI7::Label("Channels: " + std::to_string(player.GetChannelCount()));
        UI7::SetCursorPos(R7Vec2(5, 215));
        UI7::Progressbar(player.GetCurrent() / player.GetTotal());
        UI7::RestoreCursor();
    }
    UI7::EndMenu();
  }
  RenderD7::OnScreen(Bottom);
  if (UI7::BeginMenu(RD7::Lang::Get("CONTROLCENTER"))) {
    if (UI7::Button(RD7::Lang::Get("BROWSE"))) {
      RenderD7::Scene::Load(std::make_unique<Browse>(), true);
    }
    if (UI7::Button("Titles")) {
      RenderD7::Scene::Load(std::make_unique<Titles>(), true);
    }
    if (UI7::Button(RD7::Lang::Get("SETTINGS"))) {
      RenderD7::Scene::Load(std::make_unique<Settings>(), true);
    }
    if (UI7::Button("Exit")) {
      RenderD7::ExitApp();
    }
    UI7::EndMenu();
  }
}
void MMM::Logic() {
  if (hidKeysDown() & KEY_START) {
    RenderD7::ExitApp();
  }
  if (hidKeysDown() & KEY_SELECT) {
    clearCache();
  }
  if (hidKeysDown() & KEY_L && romfs_is_mount) {
    RenderD7::Scene::Load(std::make_unique<RomfsBrowse>());
  }
  if (hidKeysDown() & KEY_Y) {
    if (playing) {
      currentlypl.clear();
      player.stop();
      playing = false;
    }
  }
}

Browse::Browse() {
  this->dir = "sdmc:/";
  this->dircontent = RenderD7::FileSystem::GetDirContentsExt(dir, {"bcstm"});
  this->reload_list();
  this->changeddir = false;
}

void Browse::Draw(void) const {
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
    UI7::Label("Playing: " + std::string(playing ? currentlypl : "Nothing"), RD7TextFlags_Short);
    UI7::Label("DirEntrys: " + std::to_string(this->dircontent.size()));
    UI7::Label("Loadet: " + std::string(player.IsLoadet() ? "True" : "False"));
    UI7::Label("Loop: " + player.GetLoop());
    UI7::Label("LoopStart: " + player.GetLoopStart());
    UI7::Label("LoopEnd: " + player.GetLoopEnd());
    UI7::Label("Current: " + std::to_string((int)player.GetCurrent()));
    UI7::Label("Total: " + std::to_string((int)player.GetTotal()));
    UI7::Label("Channels: " + std::to_string(player.GetChannelCount()));
    UI7::SetCursorPos(R7Vec2(5, 215));
    UI7::Progressbar(player.GetCurrent() / player.GetTotal());
    UI7::RestoreCursor();
    UI7::EndMenu();
  }
}

void Browse::Logic() {
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
          player.stop();
          player.openFromFile(this->dircontent[this->dirsel].path);

          player.play();
          currentlypl = this->dircontent[this->dirsel].name;
          playing = true;
        }
      }
    }
  }
  if (hidKeysDown() & KEY_B) {

    if (strcmp(dir.c_str(), "sdmc:/") == 0 || strcmp(dir.c_str(), "/") == 0) {
      RenderD7::Scene::Back();
    } else {
      dir = RenderD7::FileSystem::GetParentPath(dir, "sdmc:/");
      dirsel = 0;
      changeddir = true;
    }
  }
  if (hidKeysDown() & KEY_X) {
    RenderD7::Scene::Load(std::make_unique<MMM>(), true);
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

Settings::Settings() {
  auto dc = RD7::FileSystem::GetDirContent("romfs:/lang");
  int n = 0;
  for (auto &it : dc) {
    if(it.name == RD7::Lang::GetShortcut()) {
      lang_sel = n;
    }
    if (it.dir) {
      languages.push_back(it.name);
    }
    n++;
  }
}

void Settings::Draw(void) const {
  RenderD7::OnScreen(Top);
  if (UI7::BeginMenu(RD7::Lang::Get("HEAD_SETTINGS"))) {
    UI7::Label(RD7::Lang::Get("CREDITSL"));
    UI7::SetCursorPos(R7Vec2(5, 222));
    UI7::Label("Version: 1.5.0");
    UI7::RestoreCursor();
#ifndef RELASE
    UI7::SetCursorPos(R7Vec2(395, 222));
    UI7::Label("nightly: " + std::string(V_STRING), RD7TextFlags_AlignRight);
    UI7::RestoreCursor();
#endif
    UI7::EndMenu();
  }
  RD7::OnScreen(Bottom);
  if (UI7::BeginMenu(RD7::Lang::Get("BGB"))) {
    if (languages.size() != 0) {
      if (UI7::Button(RD7::Lang::Get("LANGUAGE") + languages[lang_sel])) {
        lang_sel++;
        if (lang_sel + 1 > (int)languages.size()) {
          lang_sel = 0;
        }
        RD7::Lang::Load(languages[lang_sel]);
      }
    }
    UI7::EndMenu();
  }
}

void Settings::Logic() {
  if (hidKeysDown() & KEY_B) {
    RenderD7::Scene::Back();
  }
}
int abc = 0;
Titles::Titles() {
  D7MC::TitleManager::ScanSD("sdmc:/BCSTM-Player/");
  maxtitles = (int)D7MC::TitleManager::sdtitles.size();
  for (const auto &it : D7MC::TitleManager::sdtitles)
    namelist.push_back(it->name());
  /*D7MC::TitleManager::ScanNand("sdmc:/BCSTM-Player/");
  abc = (int)D7MC::TitleManager::nandtitles.size();*/
}

void Titles::Draw(void) const {
  RenderD7::OnScreen(Top);
  if (UI7::BeginMenu("BCSTM-Player -> Titles")) {
    UI7::BrowserList(namelist, selection);
    UI7::EndMenu();
  }
  RD7::OnScreen(Bottom);
  if (UI7::BeginMenu(RD7::Lang::Get("BGB"))) {
    UI7::EndMenu();
  }
}

void Titles::Logic() {
  if (hidKeysDown() & KEY_B) {
    RenderD7::Scene::Back();
  }
  if (hidKeysDown() & KEY_A) {
    romfsUnmount("title");
    romfs_is_mount = false;
    Result mntres = romfsMountFromTitle(
        D7MC::TitleManager::sdtitles[selection]->id(),
        D7MC::TitleManager::sdtitles[selection]->mediatype(), "title");
    if (R_FAILED(mntres)) {
      RenderD7::ResultDecoder decc;
      decc.Load(mntres);
      decc.WriteLog();
    } else {
      romfs_is_mount = true;
    }
  }
  if (hidKeysDown() & KEY_DOWN &&
      selection < (int)D7MC::TitleManager::sdtitles.size() - 1) {
    selection++;
  }
  if (hidKeysDown() & KEY_UP && selection > 0) {
    selection--;
  }
}

RomfsBrowse::RomfsBrowse() {
  dir = "title:/";
  dircontent = RenderD7::FileSystem::GetDirContentsExt(dir, {"bcstm"});
  this->reload_list();
  this->changeddir = false;
}

void RomfsBrowse::Draw(void) const {
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
    UI7::Label("Playing: " + std::string(playing ? currentlypl : "Nothing"), RD7TextFlags_Short);
    UI7::Label("DirEntrys: " + std::to_string(this->dircontent.size()));
    UI7::Label("Loadet: " + std::string(player.IsLoadet() ? "True" : "False"));
    UI7::Label("Loop: " + player.GetLoop());
    UI7::Label("LoopStart: " + player.GetLoopStart());
    UI7::Label("LoopEnd: " + player.GetLoopEnd());
    UI7::Label("Current: " + std::to_string((int)player.GetCurrent()));
    UI7::Label("Total: " + std::to_string((int)player.GetTotal()));
    UI7::Label("Channels: " + std::to_string(player.GetChannelCount()));
    UI7::SetCursorPos(R7Vec2(5, 215));
    UI7::Progressbar(player.GetCurrent() / player.GetTotal());
    UI7::RestoreCursor();
    UI7::EndMenu();
  }
}

void RomfsBrowse::Logic() {
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
        chdir(this->dircontent[this->dirsel].name.c_str());
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
          player.stop();
          player.openFromFile(this->dircontent[this->dirsel].path);

          player.play();
          currentlypl = this->dircontent[this->dirsel].name;
          playing = true;
        }
      }
    }
  }
  if (hidKeysDown() & KEY_B) {

    if (strcmp(dir.c_str(), "title:/") == 0 || strcmp(dir.c_str(), "/") == 0) {
      RenderD7::Scene::Back();
    } else {
      dir = RenderD7::FileSystem::GetParentPath(dir, "title:/");
      dirsel = 0;
      changeddir = true;
    }
  }
  if (hidKeysDown() & KEY_X) {
    RenderD7::Scene::Load(std::make_unique<MMM>());
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