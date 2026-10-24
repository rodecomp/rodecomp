#include "Render/DeviceContext.h"
#include "Render/FontCache.h"
#include "Render/TextContext.h"

#include <algorithm>
#include <bit>
#include <cstdlib>
#include <cstring>
#include <malloc.h>

namespace {
  // STATIC: 0x01311F30
  // GLOBAL: gl_21912
  constexpr unsigned charsets[20] = {129, 0, 128, 134, 136, 222, 162, 0, 0, 204, 163, 178};
  int Add(int a, int b) {
    return std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(a) + static_cast<std::uint32_t>(b));
  }
  unsigned Hex(unsigned char value) {
    if (value >= '0' && value <= '9') {
      return value - '0';
    }
    if (value >= 'a' && value <= 'f') {
      return value - 'a' + 10;
    }
    return 0;
  }
  bool ValidHex(unsigned char value) {
    return (value >= '0' && value <= '9') || (value >= 'a' && value <= 'f') || (value >= 'A' && value <= 'F');
  }
  unsigned InlineCharset(const char* marker) {
    // Original validity accepts A-F, but conversion table maps uppercase to0.
    return 16 * Hex(static_cast<unsigned char>(marker[1])) + Hex(static_cast<unsigned char>(marker[2]));
  }
}

// OFFSET: 0x0054D4B0
// FUNCTION: fn_01934
TextContext::TextContext(CDC& surface, FontCache& fonts, const TextLocale& locale)
    : m_dc(nullptr), m_surface(&surface), m_fonts(&fonts), m_locale(&locale) {
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
    switch (this->m_locale->language) {
    case 1:
      if (this->m_locale->service == 6) {
        family = 8;
        height = Add(height, 2);
      } else {
        height = Add(height, 3);
        if (this->m_locale->service == 14) {
          family = 9;
        } else {
          family = 1;
          if (this->m_locale->serverLanguage && *this->m_locale->serverLanguage == 31) {
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
      height = Add(height, 3);
      break;
    case 6:
    case 8:
      family = 7;
      height = Add(height, 3);
      if (this->m_locale->service == 12) {
        height = Add(height, -2);
      } else if (this->m_locale->service == 18) {
        if (*this->m_locale->serverLanguage == 31) {
          family = 6;
        } else {
          height = Add(height, -2);
        }
      }
      break;
    case 7:
      family = 10;
      height = Add(height, 3);
      break;
    case 12:
      family = 11;
      height = Add(height, 3);
      break;
    }
  }
  this->m_family = family;
  this->m_height = height;
  this->m_charset = this->m_locale->language == 0 && family >= 20 ? charsets[0] : charsets[family];
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
// OFFSET: 0x00550150
// FUNCTION: fn_01968
void TextContext::SetBackground(COLORREF color, int mode) {
  SetBkColor(this->m_dc, color);
  SetBkMode(this->m_dc, mode);
}

// OFFSET: 0x00ACD9B0
// FUNCTION: fn_17772
unsigned TextContext::CodePage(unsigned charset) {
  switch (charset) {
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

// OFFSET: 0x0054F2E0
// FUNCTION: fn_01957
unsigned TextContext::Marker(const char* text, unsigned bytes, bool enabled) {
  if (!enabled) {
    return bytes;
  }
  const unsigned limit = std::min<unsigned>(static_cast<unsigned>(std::strlen(text)), bytes);
  if (std::bit_cast<std::int32_t>(limit - 2) <= 0) {
    return bytes;
  }
  for (unsigned i = 0; i < limit - 2; ++i) {
    if (
      text[i] == '|' && ValidHex(static_cast<unsigned char>(text[i + 1])) &&
      ValidHex(static_cast<unsigned char>(text[i + 2]))
    ) {
      return i;
    }
  }
  return bytes;
}

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
  SelectObject(this->m_dc, this->m_fonts->Get(this->m_family, this->m_height, charset, this->m_bold, this->m_italic));
  GetTextExtentPoint32W(this->m_dc, wide, count, &size);
  if (this->m_italic == 1) {
    int width = 0;
    for (int i = 0; i < count; ++i) {
      ABC abc;
      GetCharABCWidthsW(this->m_dc, wide[i], wide[i], &abc);
      width = Add(width, Add(abc.abcA, abc.abcB));
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
  // Original conversion uses context charset, while font lookup uses argument.
  const int count = MultiByteToWideChar(CodePage(this->m_charset), 0, text, bytes, wide, static_cast<int>(extent));
  SelectObject(this->m_dc, this->m_fonts->Get(this->m_family, this->m_height, charset, this->m_bold, this->m_italic));
  TextOutW(this->m_dc, x, y, wide, count);
  if (allocation > 0x400) {
    std::free(memory);
  }
}

// OFFSET: 0x0054FD00
// FUNCTION: fn_01961
SIZE TextContext::Measure(const char* text, int bytes) {
  const int first = Marker(text, bytes, this->m_locale->inlineCharset);
  SIZE result = this->MeasurePart(text, first, this->m_charset);
  if (first < bytes) {
    SIZE second = this->MeasurePart(text + first + 3, bytes - first - 3, InlineCharset(text + first));
    result.cx = Add(result.cx, second.cx);
    result.cy = std::max(result.cy, second.cy);
  }
  return result;
}
// OFFSET: 0x00550190
// FUNCTION: fn_01970
void TextContext::DrawPair(int x, int y, const char* text, int bytes) {
  const int first = Marker(text, bytes, this->m_locale->inlineCharset);
  this->DrawPart(x, y, text, first, this->m_charset);
  if (first < bytes) {
    const SIZE size = this->MeasurePart(text, first, this->m_charset);
    this->DrawPart(Add(x, size.cx), y, text + first + 3, bytes - first - 3, InlineCharset(text + first));
  }
}
// OFFSET: 0x00550220
// FUNCTION: fn_01971
void TextContext::Draw(int x, int y, const char* text, int bytes) {
  if (!this->m_locale->inlineCharset) {
    this->DrawPair(x, y, text, bytes);
    return;
  }
  const char* current = text;
  int scanned = 0, remaining = bytes;
  while (scanned < bytes) {
    scanned = Add(scanned, Marker(text + scanned, remaining, true));
    if (scanned >= bytes) {
      this->DrawPair(x, y, current, scanned);
      return;
    }
    if (remaining <= 3) {
      return;
    }
    const int segment = Add(Marker(text + scanned + 3, remaining - 3, true), 3);
    this->DrawPair(x, y, current, segment);
    const int first = Marker(current, segment, true);
    int width = this->MeasurePart(current, first, this->m_charset).cx;
    if (first < segment) {
      width =
        Add(width, this->MeasurePart(current + first + 3, segment - first - 3, InlineCharset(current + first)).cx);
    }
    current += segment;
    scanned = Add(scanned, segment);
    x = Add(x, width);
    remaining -= segment;
  }
}
