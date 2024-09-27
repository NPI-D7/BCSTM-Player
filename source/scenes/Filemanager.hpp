#pragma once

#include <common.hpp>

namespace BP {
class Filemanager : public PD::Scene {
 public:
  void Update() override;
  Filemanager(std::string idevice = "sdmc:/");

 private:
  void reload_list() {
    namelist.clear();
    for (const auto &it : dircontent) namelist.push_back(it.name);
  }
  std::string device;
  std::string dir;
  std::vector<PD::FileSystem::Entry> dircontent;
  std::vector<std::string> namelist;
  mutable int dirsel = 0;
  bool changeddir = false;
};
}  // namespace BP