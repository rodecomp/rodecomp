#ifndef RAGNAROK_SOURCE_UTILITY_TEXT_STRINGUTIL_H
#define RAGNAROK_SOURCE_UTILITY_TEXT_STRINGUTIL_H

#include <string>
#include <string_view>
#include <windows.h>

int CountCharacters(WORD codePage, const char* text, const char* end);
bool ContainsText(const std::string& text, std::string_view pattern);
bool IsNullOrEmpty(const char* text);
int RemoveAll(std::string& text, std::string_view pattern);
int ReplaceAll(std::string& text, std::string_view pattern, std::string_view replacement);

#endif
