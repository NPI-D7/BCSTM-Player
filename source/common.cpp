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
std::string Clock() {
  const time_t ut = time(0);
  bool h24 = config.is24h();
  bool ds = config.disp_seconds();
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

// TODO: Maybe include some safety checks...
void CheckForUpdate() {
  // Dont create infinite tasks
  if (checking_for_update) return;
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
      if (update_info.version.compare(N_STRING) != 0) {
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
      if (update_info.version.substr(1).compare(V_STRING) > 0) {
        RenderD7::PushMessage(RD7::Lang::Get("UPDATER"),
                              RD7::Lang::Get("UPTFND"));
      }
    }
    checking_for_update = false;
  });
}
namespace Lang {
void Update() {
  RD7::Ftrace::ScopedTrace st("app", "lang_reload");
  HEAD_FILEMANAGER = RD7::Lang::Get("HEAD_FILEMANAGER");
  NDSP_STATUS = RD7::Lang::Get("NDSP_STATUS");
  HEAD_MAINMENU = RD7::Lang::Get("HEAD_MAINMENU");
  HEAD_SETTINGS = RD7::Lang::Get("HEAD_SETTINGS");
  HEAD_TITLES = RD7::Lang::Get("HEAD_TITLES");
  BGB = RD7::Lang::Get("BGB");
  TPWMR = RD7::Lang::Get("TPWMR");
  BROWSE = RD7::Lang::Get("BROWSE");
  LANGUAGE = RD7::Lang::Get("LANGUAGE");
  SETTINGS = RD7::Lang::Get("SETTINGS");
  CONTROLCENTER = RD7::Lang::Get("CONTROLCENTER");
  CREDITSL = RD7::Lang::Get("CREDITSL");
  VERSION = RD7::Lang::Get("VERSION");
  INFO = RD7::Lang::Get("INFO");
  NOTHING = RD7::Lang::Get("NOTHING");
  PLAYING = RD7::Lang::Get("PLAYING");
  DIRENTRYS = RD7::Lang::Get("DIRENTRYS");
  LOADED = RD7::Lang::Get("LOADED");
  LOOP = RD7::Lang::Get("LOOP");
  LOOPSTART = RD7::Lang::Get("LOOPSTART");
  LOOPEND = RD7::Lang::Get("LOOPEND");
  CURRENT = RD7::Lang::Get("CURRENT");
  TOTAL = RD7::Lang::Get("TOTAL");
  CHANNELS = RD7::Lang::Get("CHANNELS");
  SRATE = RD7::Lang::Get("SRATE");
  ERROR = RD7::Lang::Get("ERROR");
  YES = RD7::Lang::Get("YES");
  NO = RD7::Lang::Get("NO");
  PLAY = RD7::Lang::Get("PLAY");
  PAUSE = RD7::Lang::Get("PAUSE");
  STOP = RD7::Lang::Get("STOP");
  NFP = RD7::Lang::Get("NFP");
  EXIT = RD7::Lang::Get("EXIT");
  TITLES = RD7::Lang::Get("TITLES");
  AUTHOR = RD7::Lang::Get("AUTHOR");
  TRANSISIONS = RD7::Lang::Get("TRANSISIONS");
  NEXT = RD7::Lang::Get("NEXT");
  RD7TF_THEME = RD7::Lang::Get("RD7TF_THEME");
  CLOCK = RD7::Lang::Get("CLOCK");
  i24HRS = RD7::Lang::Get("24HRS");
  SHOWSECONDS = RD7::Lang::Get("SHOWSECONDS");
  APPEARANCE = RD7::Lang::Get("APPEARANCE");
  DEVELOPER = RD7::Lang::Get("DEVELOPER");
  SHOWTITLEOPT = RD7::Lang::Get("SHOWTITLEOPT");
  UPDATER = RD7::Lang::Get("UPDATER");
  AUTOSEARCHUPDATE = RD7::Lang::Get("AUTOSEARCHUPDATE");
  USENIGHTLY = RD7::Lang::Get("USENIGHTLY");
  CHECK = RD7::Lang::Get("CHECK");
  UPDATE = RD7::Lang::Get("UPDATE");
  DOWNLOAD = RD7::Lang::Get("DOWNLOAD");
  UPDFAILED = RD7::Lang::Get("UPDFAILED");
  UPDDONE = RD7::Lang::Get("UPDDONE");
  INSTERR = RD7::Lang::Get("INSTERR");
  INSTALLING = RD7::Lang::Get("INSTALLING");
  EXECMODE = RD7::Lang::Get("EXECMODE");
  UPTFND = RD7::Lang::Get("UPTFND");
}
std::string HEAD_FILEMANAGER;
std::string NDSP_STATUS;
std::string HEAD_MAINMENU;
std::string HEAD_SETTINGS;
std::string HEAD_TITLES;
std::string BGB;
std::string TPWMR;
std::string BROWSE;
std::string LANGUAGE;
std::string SETTINGS;
std::string CONTROLCENTER;
std::string CREDITSL;
std::string VERSION;
std::string INFO;
std::string NOTHING;
std::string PLAYING;
std::string DIRENTRYS;
std::string LOADED;
std::string LOOP;
std::string LOOPSTART;
std::string LOOPEND;
std::string CURRENT;
std::string TOTAL;
std::string CHANNELS;
std::string SRATE;
std::string ERROR;
std::string YES;
std::string NO;
std::string PLAY;
std::string PAUSE;
std::string STOP;
std::string NFP;
std::string EXIT;
std::string TITLES;
std::string AUTHOR;
std::string TRANSISIONS;
std::string NEXT;
std::string RD7TF_THEME;
std::string CLOCK;
std::string i24HRS;
std::string SHOWSECONDS;
std::string APPEARANCE;
std::string DEVELOPER;
std::string SHOWTITLEOPT;
std::string UPDATER;
std::string AUTOSEARCHUPDATE;
std::string USENIGHTLY;
std::string CHECK;
std::string UPDATE;
std::string DOWNLOAD;
std::string UPDFAILED;
std::string UPDDONE;
std::string INSTERR;
std::string INSTALLING;
std::string EXECMODE;
std::string UPTFND;
}  // namespace Lang
}  // namespace BP