#ifndef RAGNAROK_SOURCE_UI_TEXT_COLOREDTEXT_H
#define RAGNAROK_SOURCE_UI_TEXT_COLOREDTEXT_H

#include <cstdint>

class UIWindow;
class FontCache;
struct TextLocale;

extern unsigned char g_hexDigits[256];

const char* ParseColorCode(const char* text, std::uint32_t& color);

void DrawColoredText(
  UIWindow& window, unsigned short codePage, int x, int y, const char* text, unsigned bytes, std::uint32_t& color,
  int family, int height, bool shadow
);

void DrawStyledColoredText(
  UIWindow& window, unsigned short codePage, int x, int y, const char* text, unsigned bytes, std::uint32_t& color,
  int family, int height, unsigned char bold, unsigned char italic, bool shadow
);

void DrawOutlinedText(
  UIWindow& window, int x, int y, const char* text, unsigned bytes, std::uint32_t color, std::uint32_t outline,
  int family, int height, unsigned char bold, unsigned char italic
);
void DrawOutlinedColoredText(
  UIWindow& window, unsigned short codePage, int x, int y, const char* text, unsigned bytes, std::uint32_t& color,
  std::uint32_t outline, int family, int height, unsigned char bold
);

#endif
