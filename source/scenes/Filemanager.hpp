#pragma once

#include <common.hpp>

namespace BP {
class Filemanager : public RenderD7::Scene {
public:
  void Draw(void) const override;
  void Logic() override;
  Filemanager(std::string idevice = "sdmc:/");

private:
  void reload_list() {
    namelist.clear();
    for (const auto &it : dircontent)
      namelist.push_back(it.name);
  }
  std::string device;
  std::string dir;
  std::vector<RenderD7::FileSystem::Entry> dircontent;
  std::vector<std::string> namelist;
  mutable int dirsel = 0;
  bool changeddir = false;
};
} // namespace BP