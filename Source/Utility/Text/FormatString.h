#ifndef RAGNAROK_SOURCE_UTILITY_TEXT_FORMATSTRING_H
#define RAGNAROK_SOURCE_UTILITY_TEXT_FORMATSTRING_H

#include <cstddef>
#include <string>

const char* FormatString(const char* format, ...);
int StringFormat(char* buffer, std::size_t sizeInBytes, std::size_t count, const char* format, ...);
std::string FormatOwnedString(const char* format, ...);
std::string IntToString(int value, unsigned int digits);
std::string IntToSignedString(int value, bool grouped, bool showPlus);

#endif
