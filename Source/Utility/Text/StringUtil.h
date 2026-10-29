#ifndef RAGNAROK_SOURCE_UTILITY_TEXT_STRINGUTIL_H
#define RAGNAROK_SOURCE_UTILITY_TEXT_STRINGUTIL_H

#include <string>
#include <string_view>
#include <vector>
#include <windows.h>

int CountCharacters(WORD codePage, const char* text, const char* end);
bool ContainsText(const std::string& text, std::string_view pattern);
std::string GetFileName(const std::string& path);
bool IsNullOrEmpty(const char* text);
int RemoveAll(std::string& text, std::string_view pattern);
int ReplaceAll(std::string& text, std::string_view pattern, std::string_view replacement);
std::string FormatDecimal(double value, int precision, bool trimZeros);
std::vector<std::string> Split(std::string_view text, std::string_view delimiter);
void TrimRight(std::string& text, std::string_view characters);
void TrimLeft(std::string& text, std::string_view characters);

#endif
