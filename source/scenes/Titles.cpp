#include <scenes/scenes.hpp>

namespace BP {
Titles::Titles() {
  std::filesystem::create_directories(RenderD7::GetAppDirectory() + "/cache");
  D7MC::TitleManager::ScanSD(RenderD7::GetAppDirectory() + "/");
  maxtitles = (int)D7MC::TitleManager::sdtitles.size();
  for (const auto &it : D7MC::TitleManager::sdtitles)
    namelist.push_back(it->name());
}

void Titles::Draw(void) const {
  RD7::OnScreen(Top);
  if (config.rd7tf_theme())
    DrawWavedBg(R7Vec2(), R7Vec2(400, 240), RenderD7::GetTime());
  if (UI7::BeginMenu(Lang::HEAD_TITLES)) {
    if (config.clock()) {
      UI7::SetCursorPos(R7Vec2(395, 2));
      UI7::Label(Clock(), RD7TextFlags_AlignRight);
      UI7::RestoreCursor();
    }
    UI7::BrowserList(namelist, selection);
    UI7::EndMenu();
  }
  RD7::OnScreen(Bottom);
  if (UI7::BeginMenu(Lang::BGB)) {
    UI7::EndMenu();
  }
}

void Titles::Logic() {
  if (hidKeysDown() & KEY_B) {
    RD7::Scene::Back();
  }
  if (hidKeysDown() & KEY_A) {
    if (romfs_is_mount) {
      romfsUnmount("title");
      romfs_is_mount = false;
    }
    Result mntres = romfsMountFromTitle(
        D7MC::TitleManager::sdtitles[selection]->id(),
        D7MC::TitleManager::sdtitles[selection]->mediatype(), "title");
    if (R_FAILED(mntres)) {
      RD7::ResultDecoder d;
      d.Load(mntres);
      d.WriteLog();
      std::stringstream ss;
      ss << "Failed to mount romfs: " << d.GetCode() << std::endl;
      ss << "Module: " << d.GetModule() << std::endl;
      ss << "Level: " << d.GetLevel() << std::endl;
      ss << "Summary: " << d.GetSummary() << std::endl;
      ss << "Description: " << d.GetDescription() << std::endl;
      RD7::Error(ss.str());
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
}  // namespace BP