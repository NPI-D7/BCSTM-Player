#include <config.hpp>

void BP::Config::Load() {
  if (!RD7::FS::FileExist(RD7::GetAppDirectory() + "/config.json")) {
    make_new();
    return;
  }
  std::ifstream iff(RD7::GetAppDirectory() + "/config.json", std::ios::in);
  if (!iff.is_open()) {
    make_new();
    return;
  }
  iff >> config;
  iff.close();
  if (config.find("cfg_ver") == config.end()) {
    make_new();
    return;
  }
  if (config["cfg_ver"].get<int>() != config_version) {
    make_new();
    return;
  }
  reload();
}

void BP::Config::Save() {
  std::ofstream off(RD7::GetAppDirectory() + "/config.json", std::ios::out);
  off << config.dump(4);
  off.close();
}

void BP::Config::make_new() {
  config.clear();
  config["cfg_ver"] = config_version;
  config["lang"] = RD7::Lang::GetSys();
  config["fade"] = true;
  config["rd7tf_theme"] = false;
  config["clock"] = true;
  config["24h"] = false;
  config["disp_seconds"] = true;
  config["romfs_browse"] = false;
  config["search_updates"] = true;
  config["use_nightly"] = false;
  Save();
  reload();
}

void BP::Config::reload() {
  m_cfg_ver = GetInt("cfg_ver");
  m_lang = GetString("lang");
  m_fade = GetBool("fade");
  m_rd7tf_theme = GetBool("rd7tf_theme");
  m_clock = GetBool("clock");
  m_24h = GetBool("24h");
  m_disp_seconds = GetBool("disp_seconds");
  m_romfs_browse = GetBool("romfs_browse");
  m_search_updates = GetBool("search_updates");
  m_use_nightly = GetBool("use_nightly");
}