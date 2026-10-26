#ifndef RAGNAROK_SOURCE_UTILITY_TEXT_TEXTTOKENS_H
#define RAGNAROK_SOURCE_UTILITY_TEXT_TEXTTOKENS_H

#include <cstdint>
#include <string>

extern const unsigned char g_colorHexDigit[256];
extern unsigned char g_inlineCharset;

bool IsPipeColorToken(const char* text);
bool IsHexColorToken(const char* text, const char* end);
std::string __stdcall StripColorTokens(const char* text, std::uint32_t length);

#endif
