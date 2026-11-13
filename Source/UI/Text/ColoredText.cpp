#include "Network/TextEncoding.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Text/TextContext.h"
#include "UI/Text/ColoredText.h"
#include "UI/Window/UIWindow.h"
#include "Utility/Text/TextTokens.h"

#include <algorithm>
#include <cstring>
#include <deque>
#include <string>
#include <windows.h>

// GLOBAL: gl_24512
unsigned char g_hexDigits[256] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0,
  0, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// OFFSET: 0x00A7CA20
// FUNCTION: fn_16948
const char* SegmentEnd(const char* text, const char* end) {
  if (text == nullptr) {
    return nullptr;
  }

  const char* current = text;

  while (current < end) {
    if (*current == '^' && end - current >= 6) {
      bool color = true;

      for (int index = 1; index <= 6; ++index) {
        if (!g_colorHexDigit[current[index]]) {
          color = false;
          break;
        }
      }

      if (color) {
        return current;
      }
    }

    current = CharNextExA(static_cast<WORD>(g_textCodePage), current, 0);
  }

  return current;
}

// OFFSET: 0x00A81BA0
// FUNCTION: fn_16982
const char* ParseColorCode(const char* text, std::uint32_t& color) {
  if (text == nullptr) {
    return nullptr;
  }

  if (*text != '^') {
    return text;
  }

  const unsigned char red = static_cast<unsigned char>(g_hexDigits[text[1]] * 16 + g_hexDigits[text[2]]);
  const unsigned char green = static_cast<unsigned char>(g_hexDigits[text[3]] * 16 + g_hexDigits[text[4]]);
  const unsigned char blue = static_cast<unsigned char>(g_hexDigits[text[5]] * 16 + g_hexDigits[text[6]]);
  color = red | (green << 8) | (blue << 16);
  return text + 7;
}

// OFFSET: 0x00A84E30
// FUNCTION: fn_17015
void UIWindow::DrawColoredText(
  int x, int y, const char* text, unsigned bytes, std::uint32_t& color,
  int family, int height, bool shadow
) {
  if (bytes == 0) {
    bytes = static_cast<unsigned>(std::strlen(text));
  }

  const char* current = text;
  const char* end = text + bytes;
  const char* marker = text + TextContext::Marker(text, static_cast<unsigned>(end - text));
  TextContext context(*this->geometry.surface);
  context.SetFont(family, height, 0, 0);

  if (this->HasTextBackground()) {
    context.SetBackgroundColor(this->TextBackground());
    context.SetBackgroundMode(OPAQUE);
  }

  while (current < end) {
    current = ParseColorCode(current, color);

    const char* next = SegmentEnd(current, end);
    std::string segment(current, next);

    if (marker && *marker != '\0') {
      segment.insert(segment.begin(), marker, marker + 3);
    }

    if (next != current) {
      if (shadow) {
        context.SetColor(0);
        context.Draw((x + 1), (y + 1), segment.c_str(), static_cast<int>(segment.size()));
      }

      context.SetColor(color);
      context.Draw(x, y, segment.c_str(), static_cast<int>(segment.size()));
      x = (x + context.Measure(current, static_cast<int>(next - current)).cx);
    }

    current = next;
  }

  if (this->HasTextBackground()) {
    context.SetBackgroundMode(TRANSPARENT);
  }
}

// OFFSET: 0x00A84B80
// FUNCTION: fn_17014
void UIWindow::DrawStyledColoredText(
  int x, int y, const char* text, unsigned bytes, std::uint32_t& color,
  int family, int height, unsigned char bold, unsigned char italic, bool shadow
) {
  if (bytes == 0) {
    bytes = static_cast<unsigned>(std::strlen(text));
  }

  const char* current = text;
  const char* end = text + bytes;
  const char* marker = text + TextContext::Marker(text, static_cast<unsigned>(end - text));
  TextContext context(*this->geometry.surface);
  context.SetFont(family, height, bold, italic);

  if (this->HasTextBackground()) {
    context.SetBackgroundColor(this->TextBackground());
    context.SetBackgroundMode(OPAQUE);
  }

  while (current < end) {
    current = ParseColorCode(current, color);

    const char* next = SegmentEnd(current, end);
    std::string segment(current, next);

    if (marker && *marker != '\0') {
      segment.insert(segment.begin(), marker, marker + 3);
    }

    if (next != current) {
      if (shadow) {
        context.SetColor(0);
        context.Draw((x + 1), (y + 1), segment.c_str(), static_cast<int>(segment.size()));
      }

      context.SetColor(color);
      context.Draw(x, y, segment.c_str(), static_cast<int>(segment.size()));
      x = (x + context.Measure(current, static_cast<int>(next - current)).cx);
    }

    current = next;
  }

  if (this->HasTextBackground()) {
    context.SetBackgroundMode(TRANSPARENT);
  }
}

// OFFSET: 0x00A85720
// FUNCTION: fn_17016
void UIWindow::DrawOutlinedText(
  int x, int y, const char* text, unsigned bytes, std::uint32_t color, std::uint32_t outline,
  int family, int height, unsigned char bold, unsigned char italic
) {
  if (bytes == 0) {
    bytes = static_cast<unsigned>(std::strlen(text));
  }

  TextContext context(*this->geometry.surface);
  context.SetFont(family, height, bold, italic);

  if (this->HasTextBackground()) {
    context.SetBackgroundColor(this->TextBackground());
    context.SetBackgroundMode(OPAQUE);
  }

  context.SetColor(outline);
  context.Draw((x - 1), y, text, bytes);
  context.Draw((x + 1), y, text, bytes);
  context.Draw(x, (y - 1), text, bytes);
  context.Draw(x, (y + 1), text, bytes);
  context.SetColor(color);
  context.Draw(x, y, text, bytes);

  if (this->HasTextBackground()) {
    context.SetBackgroundMode(TRANSPARENT);
  }
}

// OFFSET: 0x00A848B0
// FUNCTION: fn_17013
void UIWindow::DrawOutlinedColoredText(
  int x, int y, const char* text, unsigned bytes, std::uint32_t& color,
  std::uint32_t outline, int family, int height, unsigned char bold
) {
  if (bytes == 0) {
    bytes = static_cast<unsigned>(std::strlen(text));
  }

  const char* current = text;
  const char* end = text + bytes;
  const char* marker = text + TextContext::Marker(text, static_cast<unsigned>(end - text));
  TextContext context(*this->geometry.surface);
  context.SetFont(family, height, bold, 0);

  while (current < end) {
    current = ParseColorCode(current, color);

    const char* next = SegmentEnd(current, end);
    std::string segment(current, next);

    if (marker && *marker != '\0') {
      segment.insert(segment.begin(), marker, marker + 3);
    }

    if (next != current) {
      const int length = static_cast<int>(segment.size());
      context.SetColor(outline);
      context.Draw((x - 1), y, segment.c_str(), length);
      context.Draw((x + 1), y, segment.c_str(), length);
      context.Draw(x, (y - 1), segment.c_str(), length);
      context.Draw(x, (y + 1), segment.c_str(), length);
      context.SetColor(color);
      context.Draw(x, y, segment.c_str(), length);
      x = (x + context.Measure(current, static_cast<int>(next - current)).cx);
    }

    current = next;
  }
}

// OFFSET: 0x00A87300
// FUNCTION: fn_17062
std::string StripColorCodes(const char* text, unsigned bytes) {
  std::string result;
  const char* const end = text + bytes;
  const char* const marker = text + TextContext::Marker(text, static_cast<unsigned>(end - text));

  while (text < end) {
    if (text != nullptr && *text == '^') {
      text += 7;
    }

    const char* const next = SegmentEnd(text, end);
    std::string segment(text, next);

    if (marker != nullptr && *marker != '\0') {
      segment.insert(segment.begin(), marker, marker + 3);
    }

    if (next != text) {
      result += segment;
    }

    text = next;
  }

  return result;
}

// OFFSET: 0x00A874B0
// FUNCTION: fn_17063
bool ContainsValue(const std::deque<int>& values, const int& value) {
  return values.end() != std::find(values.begin(), values.end(), value);
}
