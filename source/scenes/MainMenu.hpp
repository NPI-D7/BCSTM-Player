#pragma once

#include <common.hpp>

namespace BP {
class MainMenu : public RD7::Scene {
 public:
  void Draw(void) const override;
  void Logic() override;
  MainMenu();

 private:
  void load_menu();
  std::vector<std::string> menu;
  mutable int sel = 0;
  std::string current_lang;
};
}  // namespace BP