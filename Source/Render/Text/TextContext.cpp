#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"
#include "Client/Input/InputCharset.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Utility/Text/TextTokens.h"

#include <algorithm>
#include <bit>
#include <cstdlib>
#include <cstring>
#include <malloc.h>

namespace {
  // STATIC: 0x01311F30
  // GLOBAL: gl_21912
  constexpr unsigned charsets[20] = {HANGUL_CHARSET,  ANSI_CHARSET, SHIFTJIS_CHARSET, GB2312_CHARSET,  CHINESEBIG5_CHARSET, THAI_CHARSET,
                                     TURKISH_CHARSET, ANSI_CHARSET, ANSI_CHARSET,     RUSSIAN_CHARSET, VIETNAMESE_CHARSET,  ARABIC_CHARSET};

  // INLINED
  unsigned Hex(unsigned char value) {
    if (value >= '0' && value <= '9') {
      return value - '0';
    }

    if (value >= 'a' && value <= 'f') {
      return value - 'a' + 10;
    }

    return 0;
  }

  // INLINED
  bool ValidHex(unsigned char value) {
    return (value >= '0' && value <= '9') || (value >= 'a' && value <= 'f') || (value >= 'A' && value <= 'F');
  }

  // INLINED
  unsigned InlineCharset(const char* marker) {
    return 16 * Hex(static_cast<unsigned char>(marker[1])) + Hex(static_cast<unsigned char>(marker[2]));
  }
}

// OFFSET: 0x0054D4B0
// FUNCTION: fn_01934
TextContext::TextContext(CDC& surface) : m_dc(nullptr), m_surface(&surface) {
  if (surface.GetDC(&this->m_dc)) {
    this->m_previous = GetCurrentObject(this->m_dc, OBJ_FONT);
    SetBkMode(this->m_dc, TRANSPARENT);
  }
}

// OFFSET: 0x0054D7E0
// FUNCTION: fn_01939
TextContext::~TextContext() {
  if (this->m_surface && this->m_dc) {
    SelectObject(this->m_dc, this->m_previous);
    this->m_surface->ReleaseDC(this->m_dc);
  }
}

// OFFSET: 0x0054FFE0
// FUNCTION: fn_01966
void TextContext::SetFont(int family, int height, unsigned char bold, unsigned char italic) {
  if (family == 0) {
    switch (g_language) {
    case 1:
      if (g_serviceType == 6) {
        family = 8;
        height = (height + 2);
      } else {
        height = (height + 3);
        if (g_serviceType == 14) {
          family = 9;
        } else {
          family = 1;
          if (g_serverLanguage && g_serverLanguage->alternateLanguage == 31) {
            family = 6;
          }
        }
      }
      break;
    case 2:
      family = 2;
      break;
    case 3:
      family = 3;
      break;
    case 4:
      family = 4;
      break;
    case 5:
      family = 5;
      height = (height + 3);
      break;
    case 6:
    case 8:
      family = 7;
      height = (height + 3);
      if (g_serviceType == 12) {
        height = (height - 2);
      } else if (g_serviceType == 18) {
        if (g_serverLanguage->alternateLanguage == 31) {
          family = 6;
        } else {
          height = (height - 2);
        }
      }
      break;
    case 7:
      family = 10;
      height = (height + 3);
      break;
    case 12:
      family = 11;
      height = (height + 3);
      break;
    }
  }

  this->m_family = family;
  this->m_height = height;
  this->m_charset = g_language == 0 && family >= 20 ? charsets[0] : charsets[family];
  this->m_bold = bold;
  this->m_italic = italic;
}

// OFFSET: 0x00550170
// FUNCTION: fn_01969
void TextContext::SetColor(COLORREF color) {
  SetTextColor(this->m_dc, color);
}

// OFFSET: 0x00550130
// FUNCTION: fn_01967
void TextContext::SetBackgroundColor(COLORREF color) {
  SetBkColor(this->m_dc, color);
}

// OFFSET: 0x00550150
// FUNCTION: fn_01968
void TextContext::SetBackgroundMode(int mode) {
  SetBkMode(this->m_dc, mode);
}

void TextContext::SetBackground(COLORREF color, int mode) {
  this->SetBackgroundColor(color);
  this->SetBackgroundMode(mode);
}

// OFFSET: 0x00ACD9B0
// FUNCTION: fn_17772
unsigned TextContext::CodePage(unsigned charset) {
  switch (charset) {
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

// OFFSET: 0x0054F2E0
// FUNCTION: fn_01957
unsigned TextContext::Marker(const char* text, unsigned bytes) {
  if (!g_inlineCharset) {
    return bytes;
  }

  const unsigned limit = std::min<unsigned>(static_cast<unsigned>(std::strlen(text)), bytes);
  if (std::bit_cast<std::int32_t>(limit - 2) <= 0) {
    return bytes;
  }

  for (unsigned i = 0; i < limit - 2; ++i) {
    if (text[i] == '|' && ValidHex(static_cast<unsigned char>(text[i + 1])) && ValidHex(static_cast<unsigned char>(text[i + 2]))) {
      return i;
    }
  }

  return bytes;
}

// OFFSET: 0x005504E0
// FUNCTION: fn_01979
void __stdcall Sub5504E0() {}

// OFFSET: 0x005504F0
// FUNCTION: fn_01980
void __stdcall Sub5504F0() {}

// OFFSET: 0x00550500
// FUNCTION: fn_01981
void __stdcall Sub550500() {}

// OFFSET: 0x00550510
// FUNCTION: fn_01982
void __stdcall Sub550510() {}

// OFFSET: 0x00550520
// FUNCTION: fn_01983
SIZE TextContext::MeasurePart(const char* text, int bytes, unsigned charset) {
  SIZE size = {};
  if (!text || bytes <= 0) {
    return size;
  }

  const unsigned extent = 2u * static_cast<unsigned>(bytes);
  const unsigned allocation = extent < extent + 8 ? extent + 8 : 0;
  if (!allocation) {
    return size;
  }

  void* memory = allocation > 0x400 ? std::malloc(allocation) : _alloca(allocation);
  if (!memory) {
    return size;
  }

  auto* wide = reinterpret_cast<WCHAR*>(static_cast<char*>(memory) + 8);
  const int count = MultiByteToWideChar(CodePage(charset), 0, text, bytes, wide, static_cast<int>(extent));
  SelectObject(this->m_dc, g_fontCache.Get(this->m_family, this->m_height, charset, this->m_bold, this->m_italic));
  GetTextExtentPoint32W(this->m_dc, wide, count, &size);

  if (this->m_italic == 1) {
    int width = 0;

    for (int i = 0; i < count; ++i) {
      ABC abc;
      GetCharABCWidthsW(this->m_dc, wide[i], wide[i], &abc);
      width = (width + (abc.abcA + abc.abcB));
    }

    size.cx = width;
  }

  if (allocation > 0x400) {
    std::free(memory);
  }

  return size;
}

// OFFSET: 0x00550680
// FUNCTION: fn_01984
void TextContext::DrawPart(int x, int y, const char* text, int bytes, unsigned charset) {
  if (!text || bytes <= 0) {
    return;
  }

  const unsigned extent = 2u * static_cast<unsigned>(bytes);
  const unsigned allocation = extent < extent + 8 ? extent + 8 : 0;
  if (!allocation) {
    return;
  }

  void* memory = allocation > 0x400 ? std::malloc(allocation) : _alloca(allocation);
  if (!memory) {
    return;
  }

  auto* wide = reinterpret_cast<WCHAR*>(static_cast<char*>(memory) + 8);

  const int count = MultiByteToWideChar(CodePage(this->m_charset), 0, text, bytes, wide, static_cast<int>(extent));
  SelectObject(this->m_dc, g_fontCache.Get(this->m_family, this->m_height, charset, this->m_bold, this->m_italic));
  TextOutW(this->m_dc, x, y, wide, count);

  if (allocation > 0x400) {
    std::free(memory);
  }
}

// OFFSET: 0x0054FD00
// FUNCTION: fn_01961
SIZE TextContext::Measure(const char* text, int bytes) {
  const int first = Marker(text, bytes);
  SIZE result = this->MeasurePart(text, first, this->m_charset);
  if (first < bytes) {
    SIZE second = this->MeasurePart(text + first + 3, bytes - first - 3, InlineCharset(text + first));
    result.cx = (result.cx + second.cx);
    result.cy = std::max(result.cy, second.cy);
  }

  return result;
}

// OFFSET: 0x00550190
// FUNCTION: fn_01970
void TextContext::DrawPair(int x, int y, const char* text, int bytes) {
  const int first = Marker(text, bytes);
  this->DrawPart(x, y, text, first, this->m_charset);
  if (first < bytes) {
    const SIZE size = this->MeasurePart(text, first, this->m_charset);
    this->DrawPart((x + size.cx), y, text + first + 3, bytes - first - 3, InlineCharset(text + first));
  }
}

// OFFSET: 0x00550220
// FUNCTION: fn_01971
void TextContext::Draw(int x, int y, const char* text, int bytes) {
  if (!g_inlineCharset) {
    this->DrawPair(x, y, text, bytes);
    return;
  }

  const char* current = text;
  int scanned = 0, remaining = bytes;

  while (scanned < bytes) {
    scanned = (scanned + Marker(text + scanned, remaining));
    if (scanned >= bytes) {
      this->DrawPair(x, y, current, scanned);
      return;
    }

    if (remaining <= 3) {
      return;
    }

    const int segment = (Marker(text + scanned + 3, remaining - 3) + 3);
    this->DrawPair(x, y, current, segment);
    const int first = Marker(current, segment);
    int width = this->MeasurePart(current, first, this->m_charset).cx;

    if (first < segment) {
      width = (width + this->MeasurePart(current + first + 3, segment - first - 3, InlineCharset(current + first)).cx);
    }

    current += segment;
    scanned = (scanned + segment);
    x = (x + width);
    remaining -= segment;
  }
}
