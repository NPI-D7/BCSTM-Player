#pragma once

#include <common.hpp>

namespace BP {
class Settings : public RD7::Scene {
 public:
  void Draw(void) const override;
  void Logic() override;
  Settings();

 private:
  std::vector<std::string> languages;
  mutable int lang_sel = 0;
  // do fade effects
  mutable bool dfe = false;
  mutable bool rd7tf_theme = false;
  mutable bool dispc = false;
  mutable bool h24 = false;
};
}  // namespace BP