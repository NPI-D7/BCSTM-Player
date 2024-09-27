#include <common.hpp>
#include <scenes/scenes.hpp>

// #define RELEASE

namespace BP {
Settings::Settings() {
  auto dc = PD::FileSystem::GetDirContent("romfs:/lang");
  int n = 0;
  for (auto &it : dc) {
    if (it.name == PD::Lang::GetShortcut()) {
      lang_sel = n;
    }
    if (it.dir) {
      languages.push_back(it.name);
    }
    n++;
  }
  dispc = config.clock();
  h24 = config.is24h();
  dps = config.disp_seconds();
  romfsb = config.romfs_browse();
  search_updates = config.search_updates();
  use_nightly = config.use_nightly();
}

void Settings::Update() {
  // Render
  PD::LI::OnScreen(false);
  if (UI7::BeginMenu(PD::Lang::Get("HEAD_SETTINGS"))) {
    if (config.clock()) {
      UI7::SetCursorPos(NVec2(395, 2));
      UI7::Label(Clock(), PDTextFlags_AlignRight);
      UI7::RestoreCursor();
    }
    UI7::Label(PD::Lang::Get("CREDITSL"));
    UI7::Label(PD::Lang::Get("TPWMR"));
    UI7::Label("  - " + PD::Lang::Get("VERSION") + std::string(PDVSTRING));
    UI7::SetCursorPos(NVec2(5, 222));
    UI7::Label(PD::Lang::Get("VERSION") + V_STRING);
    UI7::RestoreCursor();
#ifndef RELEASE
    UI7::SetCursorPos(NVec2(395, 222));
    UI7::Label("nightly: " + std::string(N_STRING), PDTextFlags_AlignRight);
    UI7::RestoreCursor();
#endif
    UI7::EndMenu();
  }
  PD::LI::OnScreen(true);
  if (UI7::BeginMenu(PD::Lang::Get("BGB"), NVec2(), UI7MenuFlags_Scrolling)) {
    if (languages.size() != 0) {
      UI7::Label(PD::Lang::Get("LANGUAGE"));
      UI7::Label("  - " + PD::Lang::GetName() + " (" + PD::Lang::GetShortcut() +
                 ")");
      UI7::Label("  - " + PD::Lang::Get("AUTHOR") + PD::Lang::GetAuthor());
      if (UI7::Button(PD::Lang::Get("NEXT") +
                      (lang_sel < (int)languages.size() - 1
                           ? languages[lang_sel + 1]
                           : languages[0]))) {
        lang_sel++;
        if (lang_sel + 1 > (int)languages.size()) {
          lang_sel = 0;
        }
        lang_reload = true;
      }
    }
    UI7::Label(PD::Lang::Get("APPEARANCE"));
    UI7::Checkbox(PD::Lang::Get("CLOCK"), dispc);
    UI7::Checkbox(PD::Lang::Get("24HRS"), h24);
    UI7::Checkbox(PD::Lang::Get("SHOWSECONDS"), dps);
    UI7::Label(PD::Lang::Get("UPDATER"));
    UI7::Checkbox(PD::Lang::Get("AUTOSEARCHUPDATE"), search_updates);
    UI7::Checkbox(PD::Lang::Get("USENIGHTLY"), use_nightly);
    if (UI7::Button(PD::Lang::Get("CHECK"))) {
      BP::CheckForUpdate();
    }
    if (update_info.valid) {
      if (!update_info.nightly &&
          update_info.version.substr(1).compare(V_STRING) < 0) {
        UI7::Label("Already Up to date!");
      } else {
        UI7::Label(PD::Lang::Get("UPDATE") + update_info.version);
        UI7::Label(PD::Lang::Get("INFO") + ":\n" + update_info.text);
        if (!downloading && !installing) {
          if (UI7::Button(PD::Lang::Get("DOWNLOAD"))) {
            PD::Tasks::Create([&]() {
              downloading = true;
              bool fail = false;
              std::string fname = "BCSTM-Player.";
              fname += hb_mode ? "3dsx" : "cia";
              if (update_info.nightly) {
                auto ret = PD::Net::Download2File(
                    "https://raw.githubusercontent.com/NPI-D7/nightlys/master/"
                    "builds/BCSTM-Player/" +
                        fname,
                    hb_mode ? thiz_path : "sdmc:/BCSTM-Player.cia");
                if (ret) {
                  fail = true;
                  PD::PushMessage(PD::Lang::Get("UPDATER"),
                                  PD::Lang::Get("UPDFAILED") + "\n" +
                                      std::to_string(PD::Net::ErrorCode(ret)) +
                                      "/" +
                                      std::to_string(PD::Net::StatusCode(ret)));
                }
              } else {
                auto ret = PD::Net::GitDownloadRelease(
                    "https://github.com/NPI-D7/BCSTM-Player", fname,
                    hb_mode ? thiz_path : "sdmc:/BCSTM-Player.cia");
                if (ret) {
                  fail = true;
                  PD::PushMessage(PD::Lang::Get("UPDATER"),
                                  PD::Lang::Get("UPDFAILED") + "\n" +
                                      std::to_string(PD::Net::ErrorCode(ret)) +
                                      "/" +
                                      std::to_string(PD::Net::StatusCode(ret)));
                }
              }
              if (!hb_mode && !fail) {
                downloading = false;
                installing = true;
                Result r = PD::InstallCia("sdmc:/BCSTM-Player.cia", true);
                if (R_FAILED(r)) {
                  PD::PushMessage(PD::Lang::Get("UPDATER"),
                                  PD::Lang::Get("UPDFAILED") + "\n" +
                                      PD::Lang::Get("INSTERR"));
                }
                std::filesystem::remove("sdmc:/BCSTM-Player.cia");
                installing = false;
              }
              PD::PushMessage(PD::Lang::Get("UPDATER"),
                              PD::Lang::Get("UPDDONE"));
              downloading = false;
            });
          }
        }
      }
    }
    if (downloading) {
      auto current = PD::Net::GetProgressCurrent();
      auto total = PD::Net::GetProgressTotal();
      UI7::Label(PD::Lang::Get("DOWNLOAD") + ": " + PD::FormatBytes(current) +
                 "/" + PD::FormatBytes(total));
      UI7::Progressbar((float)current / (float)total);
    } else if (installing) {
      auto current = PD::InstallGetInfo().current;
      auto total = PD::InstallGetInfo().total;
      UI7::Label(PD::Lang::Get("INSTALLING") + ": " + PD::FormatBytes(current) +
                 "/" + PD::FormatBytes(total));
      UI7::Progressbar((float)current / (float)total);
    }
    UI7::Label(PD::Lang::Get("DEVELOPER"));
    UI7::Label(PD::Lang::Get("EXECMODE") + (hb_mode ? "3dsx" : "cia"));
    UI7::Checkbox(PD::Lang::Get("SHOWTITLEOPT"), romfsb);
    if (UI7::Button("Palladium")) {
      PD::LoadSettings();
    }
    UI7::EndMenu();
  }
  // Logic
  if (dispc != config.clock()) {
    config.Set("clock", dispc);
  }
  if (h24 != config.is24h()) {
    config.Set("24h", h24);
  }
  if (dps != config.disp_seconds()) {
    config.Set("disp_seconds", dps);
  }
  if (romfsb != config.romfs_browse()) {
    config.Set("romfs_browse", romfsb);
  }
  if (use_nightly != config.use_nightly()) {
    config.Set("use_nightly", use_nightly);
  }
  if (search_updates != config.search_updates()) {
    config.Set("search_updates", search_updates);
  }
  if (lang_reload) {
    PD::Lang::Load(languages[lang_sel]);
    config.Set("lang", languages[lang_sel]);
    lang_reload = false;
  }
  if (hidKeysDown() & KEY_B) {
    config.Save();
    PD::Scene::Back();
  }
}
}  // namespace BP