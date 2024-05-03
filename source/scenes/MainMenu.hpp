#pragma once

#include <common.hpp>

namespace BP {
class MainMenu : public RenderD7::Scene {
 public:
  void Draw(void) const override;
  void Logic() override;
  MainMenu();

 private:
};
}  // namespace BP