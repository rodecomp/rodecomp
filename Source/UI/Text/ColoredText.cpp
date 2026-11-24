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
#include <vector>
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
const char* UIWindow::SegmentEnd(const char* text, const char* end) {
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
const char* UIWindow::ParseColorCode(const char* text, std::uint32_t& color) {
  if (text == nullptr) {
    return nullptr;
  }

  if (*text == '^') {
    std::uint32_t value = (static_cast<unsigned char>(g_hexDigits[text[5]] * 16 + g_hexDigits[text[6]]) << 8 |
                           static_cast<unsigned char>(g_hexDigits[text[3]] * 16 + g_hexDigits[text[4]])) << 8 |
                          static_cast<unsigned char>(g_hexDigits[text[1]] * 16 + g_hexDigits[text[2]]);
    color = value;
    return text + 7;
  }

  return text;
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
      SIZE size;
      context.Measure(current, static_cast<int>(next - current), &size);
      x = (x + size.cx);
    }

    current = next;
  }

  if (this->HasTextBackground()) {
    context.SetBackgroundMode(TRANSPARENT);
  }
}

struct ColoredTextSegment {
  // INLINED
  ColoredTextSegment(const char* begin, const char* end, std::uint32_t color) : prefix(begin, end), color(color), text(begin, end) {
  }

  // INLINED
  explicit ColoredTextSegment(std::uint32_t color) : color(color) {
  }

  std::vector<unsigned char> prefix;
  std::uint32_t color;
  std::vector<unsigned char> text;
};

template __declspec(noinline) void std::allocator<ColoredTextSegment>::deallocate(ColoredTextSegment* const, const std::size_t);

// OFFSET: 0x00A850E0
// FUNCTION: fn_45733
void UIWindow::DrawRightColoredText(
  int right, int y, const char* text, unsigned bytes, std::uint32_t& color, int family, int height
) {
  if (bytes == 0) {
    bytes = static_cast<unsigned>(std::strlen(text));
  }

  const char* current = text;
  const char* const end = text + bytes;
  const char* next = this->SegmentEnd(current, end);
  std::uint32_t segmentColor = color;
  const ColoredTextSegment first(current, next, segmentColor);
  std::vector<ColoredTextSegment> segments(1, first);
  std::vector<unsigned char> prefix;

  while (current < end) {
    next = this->SegmentEnd(current, end);
    prefix.insert(prefix.end(), reinterpret_cast<const unsigned char*>(current), reinterpret_cast<const unsigned char*>(next));
    if (next >= end) {
      break;
    }

    segmentColor = 0;
    const char* segmentStart = this->ParseColorCode(next, segmentColor);
    ColoredTextSegment segment(segmentColor);
    segment.prefix.insert(segment.prefix.end(), prefix.begin(), prefix.end());
    const char* segmentEnd = this->SegmentEnd(segmentStart, end);
    if (segmentEnd < end) {
      segment.text.insert(segment.text.end(), reinterpret_cast<const unsigned char*>(segmentStart), reinterpret_cast<const unsigned char*>(segmentEnd));
      segment.prefix.insert(segment.prefix.end(), reinterpret_cast<const unsigned char*>(segmentStart), reinterpret_cast<const unsigned char*>(segmentEnd));
    } else {
      segment.text.insert(segment.text.end(), reinterpret_cast<const unsigned char*>(segmentStart), reinterpret_cast<const unsigned char*>(end));
      segment.prefix.insert(segment.prefix.end(), reinterpret_cast<const unsigned char*>(segmentStart), reinterpret_cast<const unsigned char*>(end));
    }
    segments.push_back(segment);
    current = segmentStart;
  }

  TextContext context(*this->geometry.surface);
  context.SetFont(family, height, 0, 0);

  for (std::vector<ColoredTextSegment>::const_iterator it = segments.begin(); it != segments.end(); ++it) {
    const ColoredTextSegment& segment = *it;
    color = segment.color;
    if (segment.text.size() != 0) {
      SIZE size = {};
      context.Measure(reinterpret_cast<const char*>(segment.prefix.data()), static_cast<int>(segment.prefix.size()), &size);
      context.SetColor(color);
      context.DrawPair(right - size.cx, y, reinterpret_cast<const char*>(segment.text.data()), static_cast<int>(segment.text.size()));
    }
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
      SIZE size;
      context.Measure(current, static_cast<int>(next - current), &size);
      x = (x + size.cx);
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
      context.SetColor(outline);
      context.Draw((x - 1), y, segment.c_str(), static_cast<int>(segment.size()));
      context.Draw((x + 1), y, segment.c_str(), static_cast<int>(segment.size()));
      context.Draw(x, (y - 1), segment.c_str(), static_cast<int>(segment.size()));
      context.Draw(x, (y + 1), segment.c_str(), static_cast<int>(segment.size()));
      context.SetColor(color);
      context.Draw(x, y, segment.c_str(), static_cast<int>(segment.size()));
      SIZE size;
      context.Measure(current, static_cast<int>(next - current), &size);
      x = (x + size.cx);
    }

    current = next;
  }
}

// OFFSET: 0x00A87300
// FUNCTION: fn_17062
std::string UIWindow::StripColorCodes(const char* text, unsigned bytes) {
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
bool ContainsValue(const std::deque<UIWindow*>& values, UIWindow* const& value) {
  return values.end() != std::find(values.begin(), values.end(), value);
}

