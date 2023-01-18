#include "utils.hpp"

#include <memory>
#include <stdarg.h>

std::u16string UTF8toUTF16(const char *src) {
  char16_t tmp[256] = {0};
  utf8_to_utf16((uint16_t *)tmp, (uint8_t *)src, 256);
  return std::u16string(tmp);
}

static std::string utf16DataToUtf8(const char16_t *data, size_t size,
                                   char16_t delim = 0) {
  std::string ret;
  ret.reserve(size);
  char addChar[4] = {0};
  for (size_t i = 0; i < size; i++) {
    if (data[i] == delim) {
      return ret;
    } else if (data[i] < 0x0080) {
      addChar[0] = data[i];
      addChar[1] = '\0';
    } else if (data[i] < 0x0800) {
      addChar[0] = 0xC0 | ((data[i] >> 6) & 0x1F);
      addChar[1] = 0x80 | (data[i] & 0x3F);
      addChar[2] = '\0';
    } else {
      addChar[0] = 0xE0 | ((data[i] >> 12) & 0x0F);
      addChar[1] = 0x80 | ((data[i] >> 6) & 0x3F);
      addChar[2] = 0x80 | (data[i] & 0x3F);
      addChar[3] = '\0';
    }
    ret.append(addChar);
  }
  return ret;
}

std::string UTF16toUTF8(const std::u16string &src) {
  return utf16DataToUtf8(src.data(), src.size());
}

void utf8_convert(char *buf, const u16 *input, size_t bufsize) {
  size_t units = utf16_to_utf8((uint8_t *)buf, input, bufsize);
  if (units < 0)
    units = 0;
  buf[units] = 0;
}