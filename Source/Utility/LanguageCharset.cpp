#include "Utility/LanguageCharset.h"

// OFFSET: 0x00ACD8B0
// FUNCTION: fn_17771
unsigned LanguageCharset(std::uint16_t language) {
  switch (language & 0x3FF) {
  case 1:
    return 178;
  case 2:
  case 0x19:
  case 0x22:
  case 0x23:
    return 204;
  case 4:
    if ((language >> 10) == 1) {
      return 136;
    }
    if ((language >> 10) == 2) {
      return 134;
    }
    return 161;
  case 5:
  case 0xE:
  case 0x15:
  case 0x18:
  case 0x1A:
  case 0x1B:
  case 0x24:
  case 0x2F:
    return 238;
  case 8:
    return 161;
  case 0xD:
    return 177;
  case 0x11:
    return 128;
  case 0x12:
    return 129;
  case 0x1E:
    return 222;
  case 0x1F:
    return 162;
  case 0x25:
  case 0x26:
  case 0x27:
    return 186;
  case 0x2A:
    return 163;
  default:
    return 0;
  }
}

// OFFSET: 0x00ACDAE0
// FUNCTION: fn_17773
unsigned LanguageCodePage(std::uint16_t language) {
  switch (LanguageCharset(language)) {
  case 128:
    return 932;
  case 129:
    return 949;
  case 134:
    return 936;
  case 136:
    return 950;
  case 161:
    return 1253;
  case 162:
    return 1254;
  case 163:
    return 1258;
  case 177:
    return 1255;
  case 178:
    return 1256;
  case 186:
    return 1257;
  case 204:
    return 1251;
  case 222:
    return 874;
  case 238:
    return 1250;
  default:
    return 1252;
  }
}
