#ifndef RAGNAROK_SOURCE_UI_TEXT_COLOREDTEXT_H
#define RAGNAROK_SOURCE_UI_TEXT_COLOREDTEXT_H

#include <cstdint>
#include <string>

class UIWindow;
class FontCache;
struct TextLocale;

extern unsigned char g_hexDigits[256];

const char* SegmentEnd(const char* text, const char* end);
const char* ParseColorCode(const char* text, std::uint32_t& color);

void DrawColoredText(
  UIWindow& window, int x, int y, const char* text, unsigned bytes, std::uint32_t& color,
  int family, int height, bool shadow
);

void DrawStyledColoredText(
  UIWindow& window, int x, int y, const char* text, unsigned bytes, std::uint32_t& color,
  int family, int height, unsigned char bold, unsigned char italic, bool shadow
);

void DrawOutlinedText(
  UIWindow& window, int x, int y, const char* text, unsigned bytes, std::uint32_t color, std::uint32_t outline,
  int family, int height, unsigned char bold, unsigned char italic
);
void DrawOutlinedColoredText(
  UIWindow& window, int x, int y, const char* text, unsigned bytes, std::uint32_t& color,
  std::uint32_t outline, int family, int height, unsigned char bold
);

std::string StripColorCodes(const char* text, unsigned bytes);

#endif
