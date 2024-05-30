#pragma once

#include <bcstm.hpp>
#include <bg.hpp>
#include <config.hpp>
#include <d7-menu-core.hpp>
#include <filesystem>
#include <rd7.hpp>

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
// This is faster then searching the keywords evry
// frame from the json
namespace Lang {
void Update();
extern std::string HEAD_FILEMANAGER;
extern std::string NDSP_STATUS;
extern std::string HEAD_MAINMENU;
extern std::string HEAD_SETTINGS;
extern std::string HEAD_TITLES;
extern std::string BGB;
extern std::string TPWMR;
extern std::string BROWSE;
extern std::string LANGUAGE;
extern std::string SETTINGS;
extern std::string CONTROLCENTER;
extern std::string CREDITSL;
extern std::string VERSION;
extern std::string INFO;
extern std::string NOTHING;
extern std::string PLAYING;
extern std::string DIRENTRYS;
extern std::string LOADED;
extern std::string LOOP;
extern std::string LOOPSTART;
extern std::string LOOPEND;
extern std::string CURRENT;
extern std::string TOTAL;
extern std::string CHANNELS;
extern std::string SRATE;
extern std::string ERROR;
extern std::string YES;
extern std::string NO;
extern std::string PLAY;
extern std::string PAUSE;
extern std::string STOP;
extern std::string NFP;
extern std::string EXIT;
extern std::string TITLES;
extern std::string AUTHOR;
extern std::string TRANSISIONS;
extern std::string NEXT;
extern std::string RD7TF_THEME;
extern std::string CLOCK;
extern std::string i24HRS;
extern std::string SHOWSECONDS;
extern std::string APPEARANCE;
extern std::string DEVELOPER;
extern std::string SHOWTITLEOPT;
extern std::string UPDATER;
extern std::string AUTOSEARCHUPDATE;
extern std::string USENIGHTLY;
extern std::string CHECK;
extern std::string UPDATE;
extern std::string DOWNLOAD;
extern std::string UPDFAILED;
extern std::string UPDDONE;
extern std::string INSTERR;
extern std::string INSTALLING;
extern std::string EXECMODE;
extern std::string UPTFND;
}  // namespace Lang
}  // namespace BP