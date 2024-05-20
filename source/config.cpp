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
  Save();
}