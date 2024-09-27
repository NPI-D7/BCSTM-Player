#pragma once

#include <common.hpp>

namespace BP {
class Titles : public PD::Scene {
 public:
  void Update() override;
  Titles();

 private:
  mutable int selection = 0;
  int maxtitles = 0;
  std::vector<std::string> namelist;
};
}  // namespace BP