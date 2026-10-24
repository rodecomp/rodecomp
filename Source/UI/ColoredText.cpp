#include "Render/DeviceContext.h"
#include "Render/TextContext.h"
#include "UI/ColoredText.h"
#include "UI/UIWindow.h"

#include <bit>
#include <cstring>
#include <string>
#include <windows.h>

namespace {

  bool Hex(unsigned char value) {
    return (value >= '0' && value <= '9') || (value >= 'a' && value <= 'f') || (value >= 'A' && value <= 'F');
  }

  unsigned Digit(unsigned char value) {
    if (value >= '0' && value <= '9') {
      return value - '0';
    }

    if (value >= 'a' && value <= 'f') {
      return value - 'a' + 10;
    }

    if (value >= 'A' && value <= 'F') {
      return value - 'A' + 10;
    }

    return 0;
  }

  // OFFSET: 0x00A7CA20
  // FUNCTION: fn_16948
  const char* SegmentEnd(const char* text, const char* end, unsigned short codePage) {
    const char* current = text;

    while (current < end) {
      if (*current == '^' && end - current >= 6) {
        bool color = true;

        for (unsigned index = 1; index <= 6; ++index) {
          if (!Hex(static_cast<unsigned char>(current[index]))) {
            color = false;
            break;
          }
        }

        if (color) {
          return current;
        }
      }

      current = CharNextExA(codePage, current, 0);
    }

    return current;
  }

  int Add(int left, int right) {
    return std::bit_cast<int>(static_cast<unsigned>(left) + static_cast<unsigned>(right));
  }

}

// OFFSET: 0x00A84E30
// FUNCTION: fn_17015
void DrawColoredText(
  UIWindow& window,
  FontCache& fonts,
  const TextLocale& locale,
  unsigned short codePage,
  int x,
  int y,
  const char* text,
  unsigned bytes,
  std::uint32_t& color,
  int family,
  int height,
  bool shadow
) {
  if (bytes == 0) {
    bytes = static_cast<unsigned>(std::strlen(text));
  }

  const char* current = text;
  const char* end = text + bytes;
  const char* marker = text + TextContext::Marker(text, bytes, locale.inlineCharset);
  TextContext context(*window.geometry.surface, fonts, locale);
  context.SetFont(family, height, 0, 0);

  if (window.HasTextBackground()) {
    context.SetBackground(window.TextBackground(), OPAQUE);
  }

  while (current < end) {
    if (*current == '^') {
      color = (Digit(current[2]) + 16 * Digit(current[1])) | ((Digit(current[4]) + 16 * Digit(current[3])) << 8) |
              ((Digit(current[6]) + 16 * Digit(current[5])) << 16);
      current += 7;
    }

    const char* next = SegmentEnd(current, end, codePage);
    std::string segment(current, next);

    if (*marker != '\0') {
      segment.insert(0, marker, 3);
    }

    if (next != current) {
      if (shadow) {
        context.SetColor(0);
        context.Draw(Add(x, 1), Add(y, 1), segment.c_str(), static_cast<int>(segment.size()));
      }

      context.SetColor(color);
      context.Draw(x, y, segment.c_str(), static_cast<int>(segment.size()));
      x = Add(x, context.Measure(current, static_cast<int>(next - current)).cx);
    }

    current = next;
  }

  if (window.HasTextBackground()) {
    context.SetBackground(window.TextBackground(), TRANSPARENT);
  }
}

// OFFSET: 0x00A85720
// FUNCTION: fn_17016
void DrawOutlinedText(
  UIWindow& window,
  FontCache& fonts,
  const TextLocale& locale,
  int x,
  int y,
  const char* text,
  unsigned bytes,
  std::uint32_t color,
  std::uint32_t outline,
  int family,
  int height,
  unsigned char bold,
  unsigned char italic
) {
  if (bytes == 0) {
    bytes = static_cast<unsigned>(std::strlen(text));
  }

  TextContext context(*window.geometry.surface, fonts, locale);
  context.SetFont(family, height, bold, italic);

  if (window.HasTextBackground()) {
    context.SetBackground(window.TextBackground(), OPAQUE);
  }

  context.SetColor(outline);
  context.Draw(Add(x, -1), y, text, bytes);
  context.Draw(Add(x, 1), y, text, bytes);
  context.Draw(x, Add(y, -1), text, bytes);
  context.Draw(x, Add(y, 1), text, bytes);
  context.SetColor(color);
  context.Draw(x, y, text, bytes);

  if (window.HasTextBackground()) {
    context.SetBackground(window.TextBackground(), TRANSPARENT);
  }
}

// OFFSET: 0x00A848B0
// FUNCTION: fn_17013
void DrawOutlinedColoredText(
  UIWindow& window,
  FontCache& fonts,
  const TextLocale& locale,
  unsigned short codePage,
  int x,
  int y,
  const char* text,
  unsigned bytes,
  std::uint32_t& color,
  std::uint32_t outline,
  int family,
  int height,
  unsigned char bold
) {
  if (bytes == 0) {
    bytes = static_cast<unsigned>(std::strlen(text));
  }

  const char* current = text;
  const char* end = text + bytes;
  const char* marker = text + TextContext::Marker(text, bytes, locale.inlineCharset);
  TextContext context(*window.geometry.surface, fonts, locale);
  context.SetFont(family, height, bold, 0);

  while (current < end) {
    if (*current == '^') {
      color = (Digit(current[2]) + 16 * Digit(current[1])) | ((Digit(current[4]) + 16 * Digit(current[3])) << 8) |
              ((Digit(current[6]) + 16 * Digit(current[5])) << 16);
      current += 7;
    }

    const char* next = SegmentEnd(current, end, codePage);
    std::string segment(current, next);

    if (*marker != '\0') {
      segment.insert(0, marker, 3);
    }

    if (next != current) {
      const int length = static_cast<int>(segment.size());
      context.SetColor(outline);
      context.Draw(Add(x, -1), y, segment.c_str(), length);
      context.Draw(Add(x, 1), y, segment.c_str(), length);
      context.Draw(x, Add(y, -1), segment.c_str(), length);
      context.Draw(x, Add(y, 1), segment.c_str(), length);
      context.SetColor(color);
      context.Draw(x, y, segment.c_str(), length);
      x = Add(x, context.Measure(current, static_cast<int>(next - current)).cx);
    }

    current = next;
  }
}
