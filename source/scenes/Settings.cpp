#include <scenes/scenes.hpp>

// #define RELEASE

namespace BP {
Settings::Settings() {
  auto dc = RD7::FileSystem::GetDirContent("romfs:/lang");
  int n = 0;
  for (auto &it : dc) {
    if (it.name == RD7::Lang::GetShortcut()) {
      lang_sel = n;
    }
    if (it.dir) {
      languages.push_back(it.name);
    }
    n++;
  }
  dfe = config.fade();
  rd7tf_theme = config.rd7tf_theme();
  dispc = config.clock();
  h24 = config.is24h();
  dps = config.disp_seconds();
  romfsb = config.romfs_browse();
  search_updates = config.search_updates();
  use_nightly = config.use_nightly();
}

void Settings::Draw(void) const {
  RD7::R2()->OnScreen(R2Screen_Top);
  if (config.rd7tf_theme()) {
    DrawWavedBg(R7Vec2(), R7Vec2(400, 240), RenderD7::GetTime());
  }
  if (UI7::BeginMenu(Lang::HEAD_SETTINGS)) {
    if (config.clock()) {
      UI7::SetCursorPos(R7Vec2(395, 2));
      UI7::Label(Clock(), RD7TextFlags_AlignRight);
      UI7::RestoreCursor();
    }
    UI7::Label(Lang::CREDITSL);
    UI7::Label(Lang::TPWMR);
    UI7::Label("  - " + Lang::VERSION + std::string(RENDERD7VSTRING));
    UI7::SetCursorPos(R7Vec2(5, 222));
    UI7::Label(Lang::VERSION + V_STRING);
    UI7::RestoreCursor();
#ifndef RELEASE
    UI7::SetCursorPos(R7Vec2(395, 222));
    UI7::Label("nightly: " + std::string(N_STRING), RD7TextFlags_AlignRight);
    UI7::RestoreCursor();
#endif
    UI7::EndMenu();
  }
  RD7::R2()->OnScreen(R2Screen_Bottom);
  if (UI7::BeginMenu(Lang::BGB, R7Vec2(), UI7MenuFlags_Scrolling)) {
    if (languages.size() != 0) {
      UI7::Label(Lang::LANGUAGE);
      UI7::Label("  - " + RD7::Lang::GetName() + " (" +
                 RD7::Lang::GetShortcut() + ")");
      UI7::Label("  - " + Lang::AUTHOR + RD7::Lang::GetAuthor());
      if (UI7::Button(Lang::NEXT + (lang_sel < (int)languages.size() - 1
                                        ? languages[lang_sel + 1]
                                        : languages[0]))) {
        lang_sel++;
        if (lang_sel + 1 > (int)languages.size()) {
          lang_sel = 0;
        }
        lang_reload = true;
      }
    }
    UI7::Label(Lang::APPEARANCE);
    UI7::Checkbox(Lang::TRANSISIONS, dfe);
    UI7::Checkbox(Lang::RD7TF_THEME, rd7tf_theme);
    UI7::Checkbox(Lang::CLOCK, dispc);
    UI7::Checkbox(Lang::i24HRS, h24);
    UI7::Checkbox(Lang::SHOWSECONDS, dps);
    UI7::Label(Lang::UPDATER);
    UI7::Checkbox(Lang::AUTOSEARCHUPDATE, search_updates);
    UI7::Checkbox(Lang::USENIGHTLY, use_nightly);
    if (UI7::Button(Lang::CHECK)) {
      BP::CheckForUpdate();
    }
    if (update_info.valid) {
      if (!update_info.nightly &&
          update_info.version.substr(1).compare(V_STRING) < 0) {
        UI7::Label("Already Up to date!");
      } else {
        UI7::Label(Lang::UPDATE + update_info.version);
        UI7::Label(Lang::INFO + ":\n" + update_info.text);
        if (!downloading && !installing) {
          if (UI7::Button(Lang::DOWNLOAD)) {
            RenderD7::Tasks::Create([&]() {
              downloading = true;
              bool fail = false;
              std::string fname = "BCSTM-Player.";
              fname += hb_mode ? "3dsx" : "cia";
              if (update_info.nightly) {
                auto ret = RD7::Net::Download2File(
                    "https://raw.githubusercontent.com/NPI-D7/nightlys/master/"
                    "builds/BCSTM-Player/" +
                        fname,
                    hb_mode ? thiz_path : "sdmc:/BCSTM-Player.cia");
                if (ret) {
                  fail = true;
                  RD7::PushMessage(
                      Lang::UPDATER,
                      Lang::UPDFAILED + "\n" +
                          std::to_string(RD7::Net::ErrorCode(ret)) + "/" +
                          std::to_string(RD7::Net::StatusCode(ret)));
                }
              } else {
                auto ret = RD7::Net::GitDownloadRelease(
                    "https://github.com/NPI-D7/BCSTM-Player", fname,
                    hb_mode ? thiz_path : "sdmc:/BCSTM-Player.cia");
                if (ret) {
                  fail = true;
                  RD7::PushMessage(
                      Lang::UPDATER,
                      Lang::UPDFAILED + "\n" +
                          std::to_string(RD7::Net::ErrorCode(ret)) + "/" +
                          std::to_string(RD7::Net::StatusCode(ret)));
                }
              }
              if (!hb_mode && !fail) {
                downloading = false;
                installing = true;
                Result r = RenderD7::InstallCia("sdmc:/BCSTM-Player.cia", true);
                if (R_FAILED(r)) {
                  RD7::PushMessage(Lang::UPDATER,
                                   Lang::UPDFAILED + "\n" + Lang::INSTERR);
                }
                std::filesystem::remove("sdmc:/BCSTM-Player.cia");
                installing = false;
              }
              RD7::PushMessage(Lang::UPDATER, Lang::UPDDONE);
              downloading = false;
            });
          }
        }
      }
    }
    if (downloading) {
      auto current = RD7::Net::GetProgressCurrent();
      auto total = RD7::Net::GetProgressTotal();
      UI7::Label(Lang::DOWNLOAD + ": " + RD7::FormatBytes(current) + "/" +
                 RD7::FormatBytes(total));
      UI7::Progressbar((float)current / (float)total);
    } else if (installing) {
      auto current = RD7::InstallGetInfo().current;
      auto total = RD7::InstallGetInfo().total;
      UI7::Label(Lang::INSTALLING + ": " + RD7::FormatBytes(current) + "/" +
                 RD7::FormatBytes(total));
      UI7::Progressbar((float)current / (float)total);
    }
    UI7::Label(Lang::DEVELOPER);
    UI7::Label(Lang::EXECMODE + (hb_mode ? "3dsx" : "cia"));
    UI7::Checkbox(Lang::SHOWTITLEOPT, romfsb);
    if (UI7::Button("RenderD7")) {
      RD7::LoadSettings();
    }
    UI7::EndMenu();
  }
}

void Settings::Logic() {
  if (dfe != config.fade()) {
    config.Set("fade", dfe);
  }
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
  if (rd7tf_theme != config.rd7tf_theme()) {
    config.Set("rd7tf_theme", rd7tf_theme);
    if (rd7tf_theme == true) {
      RD7::ThemeActive()->Load("romfs:/themes/rd7tf.theme");
    } else {
      RD7::ThemeActive()->Default();
    }
  }
  if (lang_reload) {
    RD7::Lang::Load(languages[lang_sel]);
    config.Set("lang", languages[lang_sel]);
    Lang::Update();
    lang_reload = false;
  }
  if (hidKeysDown() & KEY_B) {
    config.Save();
    RD7::Scene::Back();
  }
}
}  // namespace BP