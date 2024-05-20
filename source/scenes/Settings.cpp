#include <scenes/scenes.hpp>

// #define RELEASE
#ifndef N_STRING
#define N_STRING "unknown"
#endif

#ifndef V_STRING
#define V_STRING "unknown"
#endif

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
  dfe = config.GetBool("fade");
  rd7tf_theme = config.GetBool("rd7tf_theme");
  dispc = config.GetBool("clock");
  h24 = config.GetBool("24h");
}

void Settings::Draw(void) const {
  RD7::OnScreen(Top);
  if (config.GetBool("rd7tf_theme"))
    DrawWavedBg(R7Vec2(), R7Vec2(400, 240), RenderD7::GetTime());
  if (UI7::BeginMenu(RD7::Lang::Get("HEAD_SETTINGS"))) {
    if (config.GetBool("clock")) {
      UI7::SetCursorPos(R7Vec2(395, 2));
      UI7::Label(Clock(config.GetBool("24h")), RD7TextFlags_AlignRight);
      UI7::RestoreCursor();
    }
    UI7::Label(RD7::Lang::Get("CREDITSL"));
    UI7::Label(RD7::Lang::Get("TPWMR"));
    UI7::Label("  - " + RD7::Lang::Get("VERSION") +
               std::string(RENDERD7VSTRING));
    UI7::SetCursorPos(R7Vec2(5, 222));
    UI7::Label(RD7::Lang::Get("VERSION") + V_STRING);
    UI7::RestoreCursor();
#ifndef RELEASE
    UI7::SetCursorPos(R7Vec2(395, 222));
    UI7::Label("nightly: " + std::string(N_STRING), RD7TextFlags_AlignRight);
    UI7::RestoreCursor();
#endif
    UI7::EndMenu();
  }
  RD7::OnScreen(Bottom);
  if (UI7::BeginMenu(RD7::Lang::Get("BGB"))) {
    if (languages.size() != 0) {
      UI7::Label(RD7::Lang::Get("LANGUAGE") + RD7::Lang::GetName());
      UI7::Label("  " + RD7::Lang::Get("AUTHOR") + RD7::Lang::GetAuthor());
      if (UI7::Button(RD7::Lang::Get("NEXT") +
                      (lang_sel < (int)languages.size() - 1
                           ? languages[lang_sel + 1]
                           : languages[0]))) {
        lang_sel++;
        if (lang_sel + 1 > (int)languages.size()) {
          lang_sel = 0;
        }
        RD7::Lang::Load(languages[lang_sel]);
        config.Set("lang", languages[lang_sel]);
      }
    }
    UI7::Checkbox(RD7::Lang::Get("TRANSISIONS"), dfe);
    UI7::Checkbox(RD7::Lang::Get("RD7TF_THEME"), rd7tf_theme);
    UI7::Checkbox(RD7::Lang::Get("CLOCK"), dispc);
    UI7::Checkbox(RD7::Lang::Get("24HRS"), h24);
    if (UI7::Button("RenderD7")) {
      RD7::LoadSettings();
    }
    UI7::EndMenu();
  }
}

void Settings::Logic() {
  if (dfe != config.GetBool("fade")) {
    config.Set("fade", dfe);
  }
  if (dispc != config.GetBool("clock")) {
    config.Set("clock", dispc);
  }
  if (h24 != config.GetBool("24h")) {
    config.Set("24h", h24);
  }
  if (rd7tf_theme != config.GetBool("rd7tf_theme")) {
    config.Set("rd7tf_theme", rd7tf_theme);
    if (rd7tf_theme == true) {
      RD7::ThemeLoad("romfs:/themes/rd7tf.theme");
    } else {
      RD7::ThemeDefault();
    }
  }
  if (hidKeysDown() & KEY_B) {
    config.Save();
    RD7::Scene::Back();
  }
}
}  // namespace BP