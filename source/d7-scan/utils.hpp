#pragma once
#include "smdh.hpp"

#include <3ds.h>
#include <string>

std::u16string UTF8toUTF16(const char *src);
std::string UTF16toUTF8(const std::u16string &src);
void utf8_convert(char *buf, const u16 *input, size_t bufsize);