#ifndef RAGNAROK_SOURCE_UI_COLOREDTEXT_H
#define RAGNAROK_SOURCE_UI_COLOREDTEXT_H

#include <cstdint>

class UIWindow;
class FontCache;
struct TextLocale;

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
);

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
);
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
);

#endif
