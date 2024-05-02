#include <scenes/scenes.hpp>

namespace BP {
Titles::Titles() {
  D7MC::TitleManager::ScanSD("sdmc:/BCSTM-Player/");
  maxtitles = (int)D7MC::TitleManager::sdtitles.size();
  for (const auto &it : D7MC::TitleManager::sdtitles)
    namelist.push_back(it->name());
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
    if (romfs_is_mount) {
      romfsUnmount("title");
      romfs_is_mount = false;
    }
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
} // namespace BP