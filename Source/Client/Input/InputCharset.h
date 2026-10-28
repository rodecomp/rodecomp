#ifndef RAGNAROK_SOURCE_CLIENT_INPUT_INPUTCHARSET_H
#define RAGNAROK_SOURCE_CLIENT_INPUT_INPUTCHARSET_H

#include <cstddef>
#include <cstdint>
#include <string>

struct InputCharset {
  /* 0000 */ unsigned char unknown_0000[0x8E];
  /* 008E */ std::uint16_t language;
  /* 0090 */ unsigned char unknown_0090[0xC];
  /* 009C */ std::string servicePrefix;
  /* 00B4 */ std::uint16_t alternateLanguage;

  const char* Prefix(bool alternate) const;

  const char* ServiceName(const char* name);

  unsigned CodePage(unsigned fallback) const;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(InputCharset, language) == 0x8E);
static_assert(offsetof(InputCharset, servicePrefix) == 0x9C);
static_assert(offsetof(InputCharset, alternateLanguage) == 0xB4);
#endif

extern InputCharset* g_serverLanguage;

#endif
