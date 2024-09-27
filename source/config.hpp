#pragma once

#include <pd.hpp>

namespace BP {
class Config {
 public:
  Config() {}
  ~Config() {}
  void Load();
  void Save();

  template <typename T>
  void Set(const std::string& key, const T& value) {
    config[key] = value;
    reload();
  }

  std::string GetString(const std::string& key) {
    if (!config[key].is_string()) {
      return "";
    }
    return config[key].get<std::string>();
  }

  bool GetBool(const std::string& key) {
    if (!config[key].is_boolean()) {
      return false;
    }
    return config[key].get<bool>();
  }

  int GetInt(const std::string& key) {
    if (!config[key].is_number_integer()) {
      return 0;
    }
    return config[key].get<int>();
  }

  void Reset() { make_new(); }

  int cfg_ver() { return m_cfg_ver; }
  std::string lang() { return m_lang; }
  bool clock() { return m_clock; }
  bool is24h() { return m_24h; }
  bool disp_seconds() { return m_disp_seconds; }
  bool romfs_browse() { return m_romfs_browse; }
  bool search_updates() { return m_search_updates; }
  bool use_nightly() { return m_use_nightly; }

  void reload();

 private:
  const int config_version = 1;
  void make_new();
  nlohmann::json config;
  int m_cfg_ver;
  std::string m_lang;
  bool m_fade;
  bool m_rd7tf_theme;
  bool m_clock;
  bool m_24h;
  bool m_disp_seconds;
  bool m_romfs_browse;
  bool m_search_updates;
  bool m_use_nightly;
};
}  // namespace BP