#include "Utility/TextTokens.h"

#include <cstring>

// GLOBAL: gl_24548
const unsigned char g_colorHexDigit[256] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// GLOBAL: gl_24543
unsigned char g_inlineCharset = 0xFF;

// FUNCTION: fn_01963
bool IsPipeColorToken(const char* text) {
  return g_inlineCharset == 0 && std::strlen(text) >= 3 && *text == '|' &&
         g_colorHexDigit[static_cast<unsigned char>(text[1])] != 0 && g_colorHexDigit[static_cast<unsigned char>(text[2])] != 0;
}

// FUNCTION: fn_01964
bool IsHexColorToken(const char* text, const char* end) {
  if (*text != '^' || end - text <= 6) {
    return false;
  }

  for (int index = 1; index <= 6; ++index) {
    if (g_colorHexDigit[static_cast<signed char>(text[index])] == 0) {
      return false;
    }
  }

  return true;
}

// FUNCTION: fn_13883
std::string __stdcall StripColorTokens(const char* text, std::uint32_t length) {
  std::string result;
  result.reserve(length);
  if (text != nullptr) {
    const char* current = text;
    const char* end = text + length;
    while (current < end) {
      if (IsHexColorToken(current, end)) {
        current += 7;
      } else if (IsPipeColorToken(current)) {
        current += 3;
      } else {
        result.push_back(*current++);
      }
    }
  }

  return result;
}
