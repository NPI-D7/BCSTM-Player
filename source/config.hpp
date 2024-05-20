#pragma once

#include <rd7.hpp>

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

  void Reset() { make_new(); }

 private:
  const int config_version = 1;
  void make_new();
  nlohmann::json config;
};
}  // namespace BP