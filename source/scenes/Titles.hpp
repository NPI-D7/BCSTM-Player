#pragma once

#include <common.hpp>

namespace BP {
class Titles : public RD7::Scene {
 public:
  void Draw(void) const override;
  void Logic() override;
  Titles();

 private:
  mutable int selection = 0;
  int maxtitles = 0;
  std::vector<std::string> namelist;
};
}  // namespace BP