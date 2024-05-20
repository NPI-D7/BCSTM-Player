#pragma once

#include <bcstm.hpp>
#include <bg.hpp>
#include <config.hpp>
#include <d7-menu-core.hpp>
#include <filesystem>
#include <rd7.hpp>

namespace BP {
extern D7::BCSTM player;
extern bool playing;
extern std::string now_playing;
extern bool romfs_is_mount;
extern Config config;
std::string Clock(bool h24 = false);
}  // namespace BP