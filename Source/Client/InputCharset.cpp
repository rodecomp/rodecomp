#include "Client/InputCharset.h"
#include "Utility/LanguageCharset.h"

#include <cstdio>

// OFFSET: 0x00ACE360
// FUNCTION: fn_17781
const char* InputCharset::Prefix(bool alternate) const {
  // STATIC: 0x016D3964
  // GLOBAL: gl_24373
  static char prefix[8];

  if (this->enabled) {
    std::sprintf(prefix, "|%02x", LanguageCharset(alternate ? this->alternateLanguage : this->language));
  } else {
    prefix[0] = '\0';
  }

  return prefix;
}

unsigned InputCharset::CodePage(unsigned fallback) const {
  return this->enabled ? LanguageCodePage(this->language) : fallback;
}
