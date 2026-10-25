#include "Utility/Text/LanguageCharset.h"

#include <windows.h>

// OFFSET: 0x00ACD8B0
// FUNCTION: fn_17771
unsigned LanguageCharset(std::uint16_t language) {
  switch (PRIMARYLANGID(language)) {
  case LANG_ARABIC:
    return ARABIC_CHARSET;
  case LANG_BULGARIAN:
  case LANG_RUSSIAN:
  case LANG_UKRAINIAN:
  case LANG_BELARUSIAN:
    return RUSSIAN_CHARSET;
  case LANG_CHINESE:
    if (SUBLANGID(language) == SUBLANG_CHINESE_TRADITIONAL) {
      return CHINESEBIG5_CHARSET;
    }
    if (SUBLANGID(language) == SUBLANG_CHINESE_SIMPLIFIED) {
      return GB2312_CHARSET;
    }
    return GREEK_CHARSET;
  case LANG_CZECH:
  case LANG_HUNGARIAN:
  case LANG_POLISH:
  case LANG_ROMANIAN:
  case LANG_CROATIAN:
  case LANG_SLOVAK:
  case LANG_SLOVENIAN:
  case LANG_MACEDONIAN:
    return EASTEUROPE_CHARSET;
  case LANG_GREEK:
    return GREEK_CHARSET;
  case LANG_HEBREW:
    return HEBREW_CHARSET;
  case LANG_JAPANESE:
    return SHIFTJIS_CHARSET;
  case LANG_KOREAN:
    return HANGUL_CHARSET;
  case LANG_THAI:
    return THAI_CHARSET;
  case LANG_TURKISH:
    return TURKISH_CHARSET;
  case LANG_ESTONIAN:
  case LANG_LATVIAN:
  case LANG_LITHUANIAN:
    return BALTIC_CHARSET;
  case LANG_VIETNAMESE:
    return VIETNAMESE_CHARSET;
  default:
    return ANSI_CHARSET;
  }
}

// OFFSET: 0x00ACDAE0
// FUNCTION: fn_17773
unsigned LanguageCodePage(std::uint16_t language) {
  switch (LanguageCharset(language)) {
  case SHIFTJIS_CHARSET:
    return 932;
  case HANGUL_CHARSET:
    return 949;
  case GB2312_CHARSET:
    return 936;
  case CHINESEBIG5_CHARSET:
    return 950;
  case GREEK_CHARSET:
    return 1253;
  case TURKISH_CHARSET:
    return 1254;
  case VIETNAMESE_CHARSET:
    return 1258;
  case HEBREW_CHARSET:
    return 1255;
  case ARABIC_CHARSET:
    return 1256;
  case BALTIC_CHARSET:
    return 1257;
  case RUSSIAN_CHARSET:
    return 1251;
  case THAI_CHARSET:
    return 874;
  case EASTEUROPE_CHARSET:
    return 1250;
  default:
    return 1252;
  }
}
