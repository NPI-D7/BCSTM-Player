#include <config.hpp>
#include <fstream>

void BP::Config::Load() {
  if (!PD::FS::FileExist(PD::GetAppDirectory() + "/config.json")) {
    make_new();
    return;
  }
  std::ifstream iff(PD::GetAppDirectory() + "/config.json", std::ios::in);
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
  std::ofstream off(PD::GetAppDirectory() + "/config.json", std::ios::out);
  off << config.dump(4);
  off.close();
}

void BP::Config::make_new() {
  config.clear();
  config["cfg_ver"] = config_version;
  config["lang"] = PD::Lang::GetSys();
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
  m_clock = GetBool("clock");
  m_24h = GetBool("24h");
  m_disp_seconds = GetBool("disp_seconds");
  m_romfs_browse = GetBool("romfs_browse");
  m_search_updates = GetBool("search_updates");
  m_use_nightly = GetBool("use_nightly");
}