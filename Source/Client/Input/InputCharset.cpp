#include "Client/Input/InputCharset.h"
#include "Utility/Text/LanguageCharset.h"
#include "Utility/Text/TextTokens.h"

#include <cstdio>

// OFFSET: 0x00ACE360
// FUNCTION: fn_17781
const char* InputCharset::Prefix(bool alternate) const {
  // STATIC: 0x016D3964
  // GLOBAL: gl_24373
  static char prefix[8];

  if (g_inlineCharset) {
    std::sprintf(prefix, "|%02x", LanguageCharset(alternate ? this->alternateLanguage : this->language));
  } else {
    prefix[0] = '\0';
  }

  return prefix;
}

// INLINED
unsigned InputCharset::CodePage(unsigned fallback) const {
  return g_inlineCharset ? LanguageCodePage(this->language) : fallback;
}

// OFFSET: 0x00D9CE80
// FUNCTION: fn_22314
void InputCharset::SetServicePrefix(std::string prefix) {
  this->servicePrefix = prefix;
}
