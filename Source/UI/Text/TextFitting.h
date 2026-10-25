#ifndef RAGNAROK_SOURCE_UI_TEXT_TEXTFITTING_H
#define RAGNAROK_SOURCE_UI_TEXT_TEXTFITTING_H

#include <string>

class CDC;
class FontCache;
struct TextLocale;

std::string FitText(CDC& surface, FontCache& fonts, const TextLocale& locale, std::string text, int width);

#endif
