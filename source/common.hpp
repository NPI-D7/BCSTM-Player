#pragma once

#include <bcstm.hpp>
#include <config.hpp>
#include <d7-menu-core.hpp>
#include <filesystem>
#include <pd.hpp>

#ifndef N_STRING
#define N_STRING "unknown"
#endif

#ifndef V_STRING
#define V_STRING "unknown"
#endif

namespace BP {
struct Update {
  std::string version = "";
  bool nightly = false;
  std::string text = "";
  bool valid = false;
};
extern Update update_info;
extern D7::BCSTM player;
extern bool playing;
extern std::string now_playing;
extern bool romfs_is_mount;
extern Config config;
extern std::string thiz_path;
extern bool hb_mode;
std::string Clock();
void CheckForUpdate();
}  // namespace BP