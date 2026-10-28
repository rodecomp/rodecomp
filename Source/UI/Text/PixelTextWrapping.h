#ifndef RAGNAROK_SOURCE_UI_TEXT_PIXELTEXTWRAPPING_H
#define RAGNAROK_SOURCE_UI_TEXT_PIXELTEXTWRAPPING_H

#include <string>
#include <vector>

class CDC;
class FontCache;
struct TextLocale;

void WrapPixelText(
  CDC& surface, unsigned short codePage, const char* text, std::vector<std::string>& lines, int width
);

#endif
