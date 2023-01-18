#pragma once
#include "title.hpp"
#include <3ds.h>
#include <memory>
#include <vector>

namespace TitleManager {
void ScanSD(const std::string &appmaindir);
void ScanNand(const std::string &appmaindir);
void ScanCard();
inline std::vector<std::shared_ptr<Title>> sdtitles;
inline std::vector<std::shared_ptr<Title>> gamecard;
inline std::vector<std::shared_ptr<Title>> nandtitles;
inline int titlecount;
inline int currenttitle;
} // namespace TitleManager