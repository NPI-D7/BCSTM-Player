#pragma once
#include <rd7.hpp>
#include <d7-menu-core.hpp>

class MMM : public RenderD7::Scene {
public:
  void Draw(void) const override;
  void Logic() override;
  MMM();

private:
};

class Browse : public RenderD7::Scene {
public:
  void Draw(void) const override;
  void Logic() override;
  Browse();
private:
  void reload_list() {
    namelist.clear();
    for(const auto& it : dircontent)
      namelist.push_back(it.name);
  }
  std::string dir;
  std::vector<RenderD7::FileSystem::Entry> dircontent;
  std::vector<std::string> namelist;
  mutable int dirsel = 0;
  bool changeddir = false;
};

class Settings : public RenderD7::Scene {
public:
  void Draw(void) const override;
  void Logic() override;
  Settings();

private:
  std::vector<std::string> languages;
  mutable int lang_sel = 0;
};

class Titles : public RenderD7::Scene {
public:
  void Draw(void) const override;
  void Logic() override;
  Titles();

private:
  mutable int selection = 0;
  int maxtitles = 0;
  std::vector<std::string> namelist;
};

class RomfsBrowse : public RenderD7::Scene {
public:
  void Draw(void) const override;
  void Logic() override;
  RomfsBrowse();
  // static void FS_Thread(RenderD7::Parameter param);
private:
 void reload_list() {
    namelist.clear();
    for(const auto& it : dircontent)
      namelist.push_back(it.name);
  }
  std::string dir;
  std::vector<RenderD7::FileSystem::Entry> dircontent;
  std::vector<std::string> namelist;
  mutable int dirsel = 0;
  bool changeddir = false;
};