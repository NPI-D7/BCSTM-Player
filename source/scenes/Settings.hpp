#pragma once

#include <common.hpp>

namespace BP {
class Settings : public RenderD7::Scene {
public:
  void Draw(void) const override;
  void Logic() override;
  Settings();

private:
  std::vector<std::string> languages;
  mutable int lang_sel = 0;
};
} // namespace BP