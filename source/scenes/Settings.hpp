#pragma once

#include <common.hpp>

namespace BP {
class Settings : public PD::Scene {
 public:
  void Update() override;
  Settings();

 private:
  std::vector<std::string> languages;
  mutable int lang_sel = 0;
  mutable bool dispc = false;
  mutable bool h24 = false;
  mutable bool dps = false;
  mutable bool romfsb = false;
  mutable bool downloading = false;
  mutable bool use_nightly = false;
  mutable bool search_updates = false;
  mutable bool lang_reload = false;
  mutable bool installing = false;
};
}  // namespace BP