#ifndef RAGNAROK_SOURCE_UI_TEXT_COLOREDTEXT_H
#define RAGNAROK_SOURCE_UI_TEXT_COLOREDTEXT_H

#include <cstdint>
#include <string>

class UIWindow;
class FontCache;
struct TextLocale;

extern unsigned char g_hexDigits[256];

const char* SegmentEnd(const char* text, const char* end);

std::string StripColorCodes(const char* text, unsigned bytes);

#endif
