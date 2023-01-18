#include "cache.hpp"
#include "TitleManager.hpp"
#include "utils.hpp"
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <msg.hpp>
#include <sstream>

struct Title_ {
  char name[128];
  char author[128];
  u64 id;
};

struct Lst_ {
  size_t count;
  size_t reg_count;
};

void Cache::Create(std::vector<std::shared_ptr<Title>> t,
                   const std::string &path, int countall) {
  DoMsg("Creating Cache...", path);
  std::vector<Title_> titles;
  remove(path.c_str());
  Lst_ templst;
  templst.count = countall;
  templst.reg_count = t.size();
  for (unsigned i = 0; i < t.size(); i++) {
    Title_ tempt;
    sprintf(tempt.name, t[i]->name().c_str());
    sprintf(tempt.author, t[i]->author().c_str());
    tempt.id = t[i]->ID();
    titles.push_back(tempt);
  }
  FILE *f = fopen(path.c_str(), "w");
  fwrite(&templst, sizeof(templst), 1, f);
  for (size_t i = 0; i < titles.size(); i++) {
    fwrite(&titles[i], sizeof(Title_), 1, f);
  }
  fclose(f);
}

bool Cache::Read(std::vector<std::shared_ptr<Title>> t, const std::string &path,
                 bool nand) {
  DoMsg("Reading Cache...", path);
  if (!std::filesystem::exists(std::filesystem::path(path))) {
    DoMsg("Reading Cache...", "File Not Found");
    return false;
  }

  // Check for Changes
  DoMsg("Reading Cache...", "Count Check");
  amInit();
  Result res = 0;
  u32 count__ = 0;
  int titlecount__ = 0;
  res = AM_GetTitleCount(nand ? MEDIATYPE_NAND : MEDIATYPE_SD, &count__);
  if (R_FAILED(res)) {
    DoMsg("Reading Cache...", "Don't Get Count");
    return false;
  }
  titlecount__ = (int)count__;
  FILE *fdata = fopen(path.c_str(), "rb");
  Lst_ temp;
  fread(&temp, sizeof(Lst_), 1, fdata);
  if (temp.count != (size_t)titlecount__) {
    DoMsg("Reading Cache...", "Wrong Count");
    return false;
  }
  DoMsg("Reading Cache...", "Data");
  for (size_t i = 0; i < temp.reg_count; i++) {

    auto newData = std::make_shared<Title>();
    Title_ temp2;
    fread(&temp2, sizeof(Title_), 1, fdata);
    std::string title = temp2.name;
    DoMsg("Reading Cache...", title);

    std::string __author__ = temp2.author;
    uint64_t newID = temp2.id;

    newData->LoadFromCache(newID, title, __author__,
                           nand ? MEDIATYPE_NAND : MEDIATYPE_SD);

    t.push_back(newData);
  }

  for (size_t f = 0; f < t.size(); f++) {
    TitleManager::sdtitles.push_back(t[f]);
  }
  return true;
}
