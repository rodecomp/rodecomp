#ifndef RAGNAROK_SOURCE_UI_TEXT_TEXTWRAPPING_H
#define RAGNAROK_SOURCE_UI_TEXT_TEXTWRAPPING_H

#include <string>
#include <vector>

struct ItemDescriptions;

void WrapPlainText(const char* text, std::vector<std::string>& lines, int width, unsigned short codePage);

void WrapRichText(
  const char* text, std::vector<std::string>& lines, int width, bool carriageReturns, unsigned short codePage, ItemDescriptions& items
);

#endif
