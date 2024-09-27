#include <common.hpp>
#include <scenes/scenes.hpp>

namespace BP {
Titles::Titles() {
  std::filesystem::create_directories(PD::GetAppDirectory() + "/cache");
  D7MC::TitleManager::ScanSD(PD::GetAppDirectory() + "/");
  maxtitles = (int)D7MC::TitleManager::sdtitles.size();
  for (const auto &it : D7MC::TitleManager::sdtitles)
    namelist.push_back(it->name());
}

void Titles::Update() {
  // Render
  PD::LI::OnScreen(false);
  if (UI7::BeginMenu(PD::Lang::Get("HEAD_TITLES"))) {
    if (config.clock()) {
      UI7::SetCursorPos(NVec2(395, 2));
      UI7::Label(Clock(), PDTextFlags_AlignRight);
      UI7::RestoreCursor();
    }
    UI7::BrowserList(namelist, selection);
    UI7::EndMenu();
  }
  PD::LI::OnScreen(true);
  if (UI7::BeginMenu(PD::Lang::Get("BGB"))) {
    UI7::EndMenu();
  }
  // Logic
  if (hidKeysDown() & KEY_B) {
    PD::Scene::Back();
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
      PD::ResultDecoder d;
      d.Load(mntres);
      d.WriteLog();
      std::stringstream ss;
      ss << "Failed to mount romfs: " << d.GetCode() << std::endl;
      ss << "Module: " << d.GetModule() << std::endl;
      ss << "Level: " << d.GetLevel() << std::endl;
      ss << "Summary: " << d.GetSummary() << std::endl;
      ss << "Description: " << d.GetDescription() << std::endl;
      PD::Error(ss.str());
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