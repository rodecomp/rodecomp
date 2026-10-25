#ifndef RAGNAROK_SOURCE_CLIENT_INPUT_INPUTCHARSET_H
#define RAGNAROK_SOURCE_CLIENT_INPUT_INPUTCHARSET_H

#include <cstdint>

struct InputCharset {
  const bool& enabled;
  const std::uint16_t& language;
  const std::uint16_t& alternateLanguage;

  const char* Prefix(bool alternate) const;

  unsigned CodePage(unsigned fallback) const;
};

#endif
