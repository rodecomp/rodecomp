#ifndef RAGNAROK_SOURCE_UTILITY_TEXT_TEXTTOKENS_H
#define RAGNAROK_SOURCE_UTILITY_TEXT_TEXTTOKENS_H

#include <cstdint>
#include <string>
#include <string_view>

extern const unsigned char g_colorHexDigit[256];
extern unsigned char g_inlineCharset;

__declspec(noinline) bool IsPipeColorToken(const char* text);
__declspec(noinline) bool IsHexColorToken(const char* text, const char* end);
int ParseItemIdTag(const char* text, const char* end, int& length, int& itemId);
std::string __stdcall StripColorTokens(std::string_view text);

#endif
