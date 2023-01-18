#include "cache.hpp"
#include <TitleManager.hpp>
#include <array>
#include <msg.hpp>


static bool isValidId(u64 id) {
  // check for invalid titles.
  switch ((u32)id) {
  // Instruction Manual
  case 0x00008602:
  case 0x00009202:
  case 0x00009B02:
  case 0x0000A402:
  case 0x0000AC02:
  case 0x0000B402:
  // Internet Browser
  case 0x00008802:
  case 0x00009402:
  case 0x00009D02:
  case 0x0000A602:
  case 0x0000AE02:
  case 0x0000B602:
  case 0x20008802:
  case 0x20009402:
  case 0x20009D02:
  case 0x2000AE02:
  // Garbage
  case 0x00021A00:
    return false;
  }

  // Exclude Update Titles.
  u32 high = id >> 32;
  if (high == 0x0004000E) {
    return false;
  }
  // Exclude Home Menu Themes and also DLC's? Homebrews and game Titles are
  // smaller than 0x00040080. Basically exclude everything, which is larger than
  // 0x00040080.
  if (high >= 0x00040080) {
    return false;
  }

  return true;
}

void TitleManager::ScanSD(const std::string &appmaindir) {
  amInit();
  Result res = 0;
  u32 count = 0;
  sdtitles.clear();
  if (!Cache::Read(TitleManager::sdtitles, appmaindir + "cache/sd", false)) {
    DoMsg("Scanning SD", "This can Take a while...");
    res = AM_GetTitleCount(MEDIATYPE_SD, &count);
    if (R_FAILED(res)) {
      return;
    }
    titlecount = (int)count;
    std::vector<u64> ids(count);
    u64 *p = ids.data();
    res = AM_GetTitleList(NULL, MEDIATYPE_SD, count, p);
    if (R_FAILED(res)) {
      return;
    }

    for (u32 i = 0; i < count; i++) {
      currenttitle = i;

      if (isValidId(ids[i])) {
        auto title = std::make_shared<Title>();
        if (title->load(ids[i], MEDIATYPE_SD)) {
          sdtitles.push_back(title);
        }
      }
    }
    std::sort(sdtitles.begin(), sdtitles.end(),
              [](std::shared_ptr<Title> &l, std::shared_ptr<Title> &r) {
                return l->name() < r->name();
              });
    Cache::Create(sdtitles, appmaindir + "cache/sd", (int)count);
  }
}

void TitleManager::ScanCard(void) {
  amInit();
  Result res = 0;
  u32 count = 0;

  // clear title list if filled previously
  gamecard.clear();

  res = AM_GetTitleCount(MEDIATYPE_GAME_CARD, &count);
  if (R_FAILED(res)) {
    return;
  }
  D7TM::CardLoop();
  if (CardStatus == "NotInserted") {
    return;
  }
  if (CardTypeStatus == "TWL") {
    return;
  }

  // get title list and check if a title matches the ids we want
  std::vector<u64> ids(count);
  u64 *p = ids.data();
  res = AM_GetTitleList(NULL, MEDIATYPE_GAME_CARD, count, p);
  if (R_FAILED(res)) {
    return;
  }

  for (u32 i = 0; i < count; i++) {

    auto title = std::make_shared<Title>();
    if (title->load(ids[i], MEDIATYPE_GAME_CARD)) {
      gamecard.push_back(title);
    }
  }
  // sort the list alphabetically
  std::sort(gamecard.begin(), gamecard.end(),
            [](std::shared_ptr<Title> &l, std::shared_ptr<Title> &r) {
              return l->ID() < r->ID();
            });
  sdtitles.insert(sdtitles.begin(), gamecard[0]);
}

void TitleManager::ScanNand(const std::string &appmaindir) {
  amInit();
  Result res = 0;
  u32 count = 0;
  nandtitles.clear();
  if (!Cache::Read(nandtitles, appmaindir + "cache/nand", true)) {
    DoMsg("Scanning Nand", "This can Take a while...");
    res = AM_GetTitleCount(MEDIATYPE_NAND, &count);
    if (R_FAILED(res)) {
      return;
    }
    titlecount = (int)count;
    std::vector<u64> ids(count);
    u64 *p = ids.data();
    res = AM_GetTitleList(NULL, MEDIATYPE_NAND, count, p);
    if (R_FAILED(res)) {
      return;
    }

    for (u32 i = 0; i < count; i++) {
      currenttitle = i;

      if (isValidId(ids[i])) {
        auto title = std::make_shared<Title>();
        if (title->load(ids[i], MEDIATYPE_NAND)) {

          nandtitles.push_back(title);
        }
      }
    }
    std::sort(nandtitles.begin(), nandtitles.end(),
              [](std::shared_ptr<Title> &l, std::shared_ptr<Title> &r) {
                return l->name() < r->name();
              });
    Cache::Create(nandtitles, appmaindir + "cache/nand", (int)count);
  }
}