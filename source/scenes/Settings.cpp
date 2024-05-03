#include <scenes/scenes.hpp>

#ifdef V_STRING
#else
#define V_STRING "none"
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
}

void Settings::Draw(void) const {
  RenderD7::OnScreen(Top);
  if (UI7::BeginMenu(RD7::Lang::Get("HEAD_SETTINGS"))) {
    UI7::Label(RD7::Lang::Get("CREDITSL"));
    UI7::Label(RD7::Lang::Get("TPWMR"));
    UI7::Label("  - " + RenderD7::Lang::Get("VERSION") + ": " +
               std::string(RENDERD7VSTRING));
    UI7::SetCursorPos(R7Vec2(5, 222));
    UI7::Label(RenderD7::Lang::Get("VERSION") + ": 2.0.0 preview");
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
}  // namespace BP