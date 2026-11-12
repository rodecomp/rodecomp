#ifndef RAGNAROK_SOURCE_UI_TEXT_TEXTWRAPPING_H
#define RAGNAROK_SOURCE_UI_TEXT_TEXTWRAPPING_H

#include <string>
#include <vector>


void WrapPlainText(const char* text, std::vector<std::string>& lines, int width);

void WrapRichText(
  const char* text, std::vector<std::string>& lines, int width, bool carriageReturns
);

int ParseItemIdTag(const char* text, const char* end, int& length, int& itemId);

#endif
