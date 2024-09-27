#pragma once

#include <common.hpp>

namespace BP {
class MainMenu : public PD::Scene {
 public:
  void Update() override;
  MainMenu();

 private:
  void load_menu();
  std::vector<std::string> menu;
  mutable int sel = 0;
  std::string current_lang;
  bool rfsopt;
};
}  // namespace BP