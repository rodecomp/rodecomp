#ifndef RAGNAROK_SOURCE_CLIENT_INPUTCHARSET_H
#define RAGNAROK_SOURCE_CLIENT_INPUTCHARSET_H

#include <cstdint>

struct InputCharset {
  const bool& enabled;
  const std::uint16_t& language;
  const std::uint16_t& alternateLanguage;

  const char* Prefix(bool alternate) const;
  // Shared selection used by composition refresh/language-change handling.
  unsigned CodePage(unsigned fallback) const;
};

#endif
