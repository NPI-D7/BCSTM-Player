#include <common.hpp>
#include <format>

namespace BP {
D7::BCSTM player;
bool playing = false;
std::string now_playing = "";
bool romfs_is_mount = false;
Config config;
std::string Clock(bool h24) {
  const time_t ut = time(0);
  auto ts = localtime(&ut);
  if (!h24) {
    int hr = ts->tm_hour % 12;
    if (hr == 0) hr = 12;
    return std::format("{}:{:02}:{:02} {}", hr, ts->tm_min, ts->tm_sec,
                       ts->tm_hour >= 12 ? "PM" : "AM");
  }
  return std::format("{}:{:02}:{:02}", ts->tm_hour, ts->tm_min, ts->tm_sec);
}
}  // namespace BP