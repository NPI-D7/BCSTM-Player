#include <common.hpp>
#include <format>

namespace BP {
D7::BCSTM player;
bool playing = false;
std::string now_playing = "";
bool romfs_is_mount = false;
Config config;
Update update_info;
bool checking_for_update = false;
std::string thiz_path;
bool hb_mode;
bool c_is_auto = false;
std::string Clock() {
  const time_t ut = time(0);
  bool h24 = config.GetBool("24h");
  bool ds = config.GetBool("disp_seconds");
  auto ts = localtime(&ut);
  if (!h24) {
    int hr = ts->tm_hour % 12;
    if (hr == 0) hr = 12;
    if (!ds) {
      return std::format("{}:{:02} {}", hr, ts->tm_min,
                         ts->tm_hour >= 12 ? "PM" : "AM");
    }
    return std::format("{}:{:02}:{:02} {}", hr, ts->tm_min, ts->tm_sec,
                       ts->tm_hour >= 12 ? "PM" : "AM");
  }
  if (!ds) {
    return std::format("{}:{:02}", ts->tm_hour, ts->tm_min);
  }
  return std::format("{}:{:02}:{:02}", ts->tm_hour, ts->tm_min, ts->tm_sec);
}
void CheckForUpdate(bool is_auto) {
  // Dont create infinite tasks
  if (checking_for_update) return;
  if (!c_is_auto && is_auto) {
    c_is_auto = is_auto;
  }
  RenderD7::Tasks::Create([&]() {
    checking_for_update = true;
    update_info = Update();
    update_info.nightly = config.GetBool("use_nightly");
    std::string url;
    if (update_info.nightly) {
      url = "https://api.github.com/repos/NPI-D7/BCSTM-Player/commits";
    } else {
      url = "https://api.github.com/repos/NPI-D7/BCSTM-Player/releases/latest";
    }
    nlohmann::json js;
    auto ret = RD7::Net::JsonApiRequest(url, js);
    if (ret) {
      update_info = Update();
      checking_for_update = false;
      return;
    }
    if (update_info.nightly) {
      update_info.version =
          js[0]["sha"].get_ref<const std::string&>().substr(0, 7);
      update_info.text = js[0]["commit"]["message"];
      update_info.valid = true;
      if (update_info.version.compare(N_STRING) != 0 && c_is_auto) {
        RenderD7::PushMessage(RD7::Lang::Get("UPDATER"),
                              RD7::Lang::Get("UPTFND"));
      }
    } else {
      update_info.version = js["tag_name"];
      if (update_info.version[0] != 'v') {
        update_info = Update();
        checking_for_update = false;
        return;
      }
      update_info.text = js["body"];
      update_info.valid = true;
      if (update_info.version.compare(V_STRING) > 0 && c_is_auto) {
        RenderD7::PushMessage(RD7::Lang::Get("UPDATER"),
                              RD7::Lang::Get("UPTFND"));
      }
    }
    checking_for_update = false;
  });
}
}  // namespace BP